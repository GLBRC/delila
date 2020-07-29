/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rseq.p" */

#include <getopt.h>  /* getopt API */ 
#include <stdio.h>   /* printf */
#include <stdlib.h> 
#include </home/mplace/bin/p2c/src/p2c.h>
/*
 rseq: rsequence calculated from encoded sequences

  Dr. Thomas D. Schneider
  toms@alum.mit.edu
  permanent email: toms@alum.mit.edu (use only if first address fails)
  https://alum.mit.edu/www/toms/

  module libraries required: delman, matmods, prgmods, auxmods

  name
      rseq: rsequence calculated from encoded sequences

synopsis
      rseq(encseq: in, cmp: in, rsdata: out, output: out)

files
      encseq: the output of the encode program
      cmp: a composition from the comp program.
          if cmp is empty, then equal frequencies are assumed.
      rsdata: a display of the information content of each position
         of the sequences, with the sampling error variance.
         This output is ready to be used as input to rsgra or as data
         for genhis for plotting.
      output: messages to the user.

description
      Encoded sequences from encseq are converted to a table of frequencies
      for each base (b) at each aligned position (l).  rsequence(l)
      and the variance var(hnb) are calculated and shown along with
      their running sums.  rsequence and the variance due to sampling
      error are shown for the whole site, but the running sums let one
      find rsequence and the variance for any subrange desired.
         n, the number of example sequences may vary with position, so
      both n and e(hnb) are shown.

documentation
      Schneider, T.D., G.D. Stormo, L. Gold and A. Ehrenfeucht (1986)
      The information content of binding sites on nucleotide sequences.
      J. Mol. Biol. 188: 415-431.

author
      Thomas D. Schneider

bugs
      Does not handle di-nucleotides or longer oligos

technical notes
      Constants maxsize (procedure calehnb) and kickover (procedure
      makehnblist) determine the largest n for which e(hnb) is used.  Above
      this, ae(hnb) is used.  Do not set these below 50 without careful
      analysis.  Other constants are in module rseq.const.

To Compile:

gcc rseq.c -o rseq -I/home/mplace/bin/p2c/src -L /home/mplace/bin/p2c/src -lm -lp2c

To Run:

rseq  -c cmp -e encseq 

*/
#define version         5.41
#define negativeinfinity  (-1000000L)
#define tolerance       (-20.0)
#define posfield        4
#define infofield       8
#define infodecim       5
#define nfield          4
#define wfield          10
#define maxvecpart      64
#define vpagewidth      64

typedef struct spacelist {
  long skips;
  struct spacelist *next;
} spacelist;

typedef enum {
  start, stop
} endpoints;

typedef struct parameter {
  long range[2];
  long window, wshift, coding;
  spacelist *spaces;
  long cshift;
  long wvlength;
  long pvlength;
  struct parameter *next;
} parameter;

typedef struct vectorpart {
  double numbers[maxvecpart];
  struct vectorpart *next;
} vectorpart;

typedef struct vector {
  long length;
  vectorpart *part;
} vector;

typedef enum {
  a, c, g, t
} base;

typedef struct compnode {
  long count;
  struct compnode *son[4];
} compnode;

typedef struct spider {
  long depth;
  compnode *place;
  struct spider *next;
} spider;

typedef struct comptotal {
  long count;
  struct comptotal *next;
} comptotal;

typedef struct path {
  base bas;
  struct path *next;
} path;

typedef struct ehnblist {
  long n;
  double ehnb, varhnb;
  Char aflag;
  struct ehnblist *next;
} ehnblist;

Static _TEXT encseq, cmp, rsdata;
Static jmp_buf _JL1;

Static Void halt()
{
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}

Static Void copyaline(fin, fout)
_TEXT *fin, *fout;
{
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
  putc('\n', fout->f);
}

#define debugging       false
#define boundary        2

Static boolean emptyfile(afile)
_TEXT *afile;
{
  boolean Result;
  long lines = 0, chars = 0;
  Char ch;

  if (*afile->name != '\0') {
    if (afile->f != NULL)
      afile->f = freopen(afile->name, "r", afile->f);
    else
      afile->f = fopen(afile->name, "r");
  } else
    rewind(afile->f);
  if (afile->f == NULL)
    _EscIO2(FileNotFound, afile->name);
  RESETBUF(afile->f, Char);
  while (!BUFEOF(afile->f) && chars < boundary) {
    if (P_eoln(afile->f)) {
      lines++;
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      continue;
    }
    ch = getc(afile->f);
    if (ch == '\n')
      ch = ' ';
    if (debugging) {
      printf("ord(ch) = %12d\n", ch);
      printf("    ch  = %c\n", ch);
    }
    chars++;
  }
  if (chars < boundary)
    Result = true;
  else
    Result = false;
  if (debugging) {
    printf("lines = %ld\n", lines);
    printf("chars = %ld\n", chars);
    if (chars < boundary)
      printf("    empty (file)\n");
    else
      printf("NOT empty (file)\n");
  }
  if (*afile->name != '\0') {
    if (afile->f != NULL)
      afile->f = freopen(afile->name, "r", afile->f);
    else
      afile->f = fopen(afile->name, "r");
  } else
    rewind(afile->f);
  if (afile->f == NULL)
    _EscIO2(FileNotFound, afile->name);
  RESETBUF(afile->f, Char);
  return Result;
}

#undef debugging
#undef boundary

Static long vectorsize(param)
parameter *param;
{
  long size = 0;

  while (param != NULL) {
    size += param->pvlength;
    param = param->next;
  }
  return size;
}

Static long paramsize(param)
parameter *param;
{
  long rangesize, numwindows;
  rangesize = param->range[(long)stop] - param->range[(long)start] + 1;
  numwindows = (long)((rangesize - 1.0) / param->wshift + 1);
  return (numwindows * param->wvlength);
}

Static Void readencpar(thefile, param, regions, vectorlength)
_TEXT *thefile;
parameter **param;
long *regions, *vectorlength;
{
  parameter *aparam, *newparam;
  spacelist *firstspaces, *newspaces;
  long i, j;
  Char ch;
  long FORLIM, FORLIM1;

  if (*thefile->name != '\0') {
    if (thefile->f != NULL)
      thefile->f = freopen(thefile->name, "r", thefile->f);
    else
      thefile->f = fopen(thefile->name, "r");
  } else
    rewind(thefile->f);
  if (thefile->f == NULL)
    _EscIO2(FileNotFound, thefile->name);
  RESETBUF(thefile->f, Char);
  if (emptyfile(thefile)) {
    printf(" encoded sequence file is empty\n");
    halt();
  }
  aparam = (parameter *)Malloc(sizeof(parameter));
  if (*param == NULL)
    *param = (parameter *)Malloc(sizeof(parameter));
  aparam = *param;

  for (i = 1; i <= 2; i++) {
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
  }

  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  fscanf(thefile->f, "%ld%*[^\n]", regions);
  getc(thefile->f);

  FORLIM = *regions;
  for (i = 1; i <= FORLIM; i++) {
    fscanf(thefile->f, "%ld", aparam->range);
    do {
      ch = getc(thefile->f);
      if (ch == '\n')
	ch = ' ';
    } while (ch != 'o');
    fscanf(thefile->f, "%ld%*[^\n]", &aparam->range[(long)stop]);
    getc(thefile->f);

    fscanf(thefile->f, "%ld%*[^\n]", &aparam->window);
    getc(thefile->f);
    fscanf(thefile->f, "%ld%*[^\n]", &aparam->wshift);
    getc(thefile->f);

    fscanf(thefile->f, "%ld", &aparam->coding);
    aparam->spaces = (spacelist *)Malloc(sizeof(spacelist));
    if (aparam->coding > 1) {
      do {
	ch = getc(thefile->f);
	if (ch == '\n')
	  ch = ' ';
      } while (ch != ':');
      firstspaces = (spacelist *)Malloc(sizeof(spacelist));
      aparam->spaces = firstspaces;
      fscanf(thefile->f, "%ld", &aparam->spaces->skips);
      FORLIM1 = aparam->coding;
      for (j = 2; j < FORLIM1; j++) {
	newspaces = (spacelist *)Malloc(sizeof(spacelist));
	aparam->spaces->next = newspaces;
	aparam->spaces = newspaces;
	fscanf(thefile->f, "%ld", &aparam->spaces->skips);
      }
      aparam->spaces->next = NULL;
      aparam->spaces = firstspaces;
    } else
      aparam->spaces = NULL;
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    fscanf(thefile->f, "%ld%*[^\n]", &aparam->cshift);
    getc(thefile->f);
    aparam->wvlength = (long)floor(exp(aparam->coding * log(4.0)) + 0.5);
    aparam->pvlength = paramsize(aparam);

    if (i < *regions) {
      newparam = (parameter *)Malloc(sizeof(parameter));
      aparam->next = newparam;
      aparam = newparam;
    }
  }
  aparam->next = NULL;
  fscanf(thefile->f, "%ld%*[^\n]", vectorlength);
  getc(thefile->f);
  if (*vectorlength == vectorsize(*param))
    return;
  printf(" vector length in the encoded file\n");
  printf(" does not correspond to the parameters\n");
  halt();
}

Static double vget(v, pos)
vector v;
long pos;
{
  long i;

  if (pos > v.length || pos < 1) {
    printf(
      " error in call to function vget: position %ld is beyond the end of the vector\n",
      pos);
    halt();
  }


  for (i = 1; i <= (pos - 1) / maxvecpart; i++)
    v.part = v.part->next;

  return (v.part->numbers[(pos - 1) & (maxvecpart - 1)]);
}

Static Void vput(v, pos, number)
vector *v;
long pos;
double number;
{
  long i;
  vectorpart *firstpart;

  if (pos > v->length || pos < 1) {
    printf(
      " error in call to function vput: position %ld is beyond the end of the vector\n",
      pos);
    halt();
  }

  firstpart = v->part;
  for (i = 1; i <= (pos - 1) / maxvecpart; i++)
    v->part = v->part->next;


  v->part->numbers[(pos - 1) & (maxvecpart - 1)] = number;
  v->part = firstpart;
}

Static Void makevector(v, l)
vector *v;
long l;
{
  long numparts, i;
  vectorpart *firstpart, *newpart;

  if (l < 1) {
    printf(" makevector: positive length required\n");
    halt();
  }

  v->length = l;
  v->part = (vectorpart *)Malloc(sizeof(vectorpart));
  firstpart = v->part;
  numparts = (v->length - 1) / maxvecpart + 1;
  for (i = 1; i < numparts; i++) {
    newpart = (vectorpart *)Malloc(sizeof(vectorpart));
    v->part->next = newpart;
    v->part = newpart;
  }
  v->part->next = NULL;
  v->part = firstpart;
}

Static Void readvector(thefile, v)
_TEXT *thefile;
vector *v;
{
  long i, j, numparts, lastpart;
  vectorpart *firstpart;

  firstpart = v->part;
  numparts = (v->length - 1) / maxvecpart + 1;
  lastpart = ((v->length - 1) & (maxvecpart - 1)) + 1;
  for (i = 1; i < numparts; i++) {
    for (j = 0; j < maxvecpart; j++)
      fscanf(thefile->f, "%lg", &v->part->numbers[j]);
    v->part = v->part->next;
  }
  for (j = 0; j < lastpart; j++)
    fscanf(thefile->f, "%lg", &v->part->numbers[j]);
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  v->part = firstpart;
}

Static Void vset(thevalue, v)
double thevalue;
vector *v;
{
  long i, FORLIM;
  FORLIM = v->length;

  for (i = 1; i <= FORLIM; i++)
    vput(v, i, thevalue);
}

Static Void vadd(a_, b)
vector a_, *b;
{
  long i;

  if (a_.length != b->length) {
    printf(" function vadd: vector lengths must be equal\n");
    halt();
  }

  for (i = 1; i <= a_.length; i++)
    vput(b, i, vget(*b, i) + vget(a_, i));
}

Static Void skipoligo(thefile)
_TEXT *thefile;
{
  Char ch;

  do {
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != 't' && ch != 'g' && ch != 'c' && ch != 'a');
  do {
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != ' ');
}

typedef struct list {
  compnode *item;
  struct list *next;
} list;

/* Local variables for readcomp: */
struct LOC_readcomp {
  list *freeitem;
} ;

Local Void getitem(l, LINK)
list **l;
struct LOC_readcomp *LINK;
{
  if (LINK->freeitem != NULL) {
    *l = LINK->freeitem;
    LINK->freeitem = LINK->freeitem->next;
  } else
    *l = (list *)Malloc(sizeof(list));
  (*l)->next = NULL;
}

Local Void clearitem(l, LINK)
list **l;
struct LOC_readcomp *LINK;
{
  list *lptr;

  if (*l == NULL)
    return;
  lptr = *l;
  *l = (*l)->next;
  lptr->next = LINK->freeitem;
  LINK->freeitem = lptr;
}

Static Void readcomp(comp, compmax, readmax, root, monocomptotal)
_TEXT *comp;
long *compmax, readmax;
compnode **root;
comptotal **monocomptotal;
{
  struct LOC_readcomp V;
  list *listitem, *first, *last;
  comptotal *comptot, *newcomptot;
  long detcomp, level, number;
  Char ch;
  base ba;

  if (*comp->name != '\0') {
    if (comp->f != NULL)
      comp->f = freopen(comp->name, "r", comp->f);
    else
      comp->f = fopen(comp->name, "r");
  } else
    rewind(comp->f);
  if (comp->f == NULL)
    _EscIO2(FileNotFound, comp->name);
  RESETBUF(comp->f, Char);
  if (emptyfile(comp)) {
    printf(" error: no composition file provided\n");
    halt();
  }
  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%ld%*[^\n]", &detcomp);
  getc(comp->f);
  fscanf(comp->f, "%*[^\n]");

  getc(comp->f);
  if (readmax < 1) {
    printf("\n warning: 0 or negative oligo length requested\n");
    printf(" composition used is depth %ld\n\n", detcomp);
    *compmax = detcomp;
  } else if (readmax > detcomp) {
    printf("\n warning: requested composition oligo length (%ld)\n", readmax);
    printf(" is larger than the determined composition oligo length (%ld).\n",
	   detcomp);
    printf(" composition used is to depth %ld\n\n", detcomp);
    *compmax = detcomp;
  } else
    *compmax = readmax;

  *root = (compnode *)Malloc(sizeof(compnode));
  *monocomptotal = (comptotal *)Malloc(sizeof(comptotal));
  comptot = (comptotal *)Malloc(sizeof(comptotal));
  first = (list *)Malloc(sizeof(list));
  Malloc(sizeof(list));
/* p2c: rseq.p: Note: Eliminated unused assignment statement [338] */
  first->item = *root;
  first->next = NULL;
  last = first;
  V.freeitem = (list *)Malloc(sizeof(list));
  V.freeitem->next = NULL;
  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%ld%*[^\n]", &number);
  getc(comp->f);
  (*root)->count = number;

  do {
    ch = getc(comp->f);
    if (ch == '\n')
      ch = ' ';
    ch = getc(comp->f);
    if (ch == '\n')
      ch = ' ';
    if (ch == '*') {
      fscanf(comp->f, "%*[^\n]");
      getc(comp->f);
      fscanf(comp->f, "%ld%*[^\n]", &level);
      getc(comp->f);
      if (level == 1)
	*monocomptotal = comptot;
      else {
	newcomptot = (comptotal *)Malloc(sizeof(comptotal));
	comptot->next = newcomptot;
	comptot = newcomptot;
      }
      comptot->count = 0;
      comptot->next = NULL;
    } else {
      for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1)) {
	skipoligo(comp);
	fscanf(comp->f, "%ld", &number);
	if (number != 0) {
	  first->item->son[(long)ba] = (compnode *)Malloc(sizeof(compnode));
	  first->item->son[(long)ba]->count = number;
	  comptot->count += number;
	  getitem(&listitem, &V);
	  last->next = listitem;
	  last = listitem;
	  last->next = NULL;
	  last->item = first->item->son[(long)ba];
	} else
	  first->item->son[(long)ba] = NULL;
      }
      clearitem(&first, &V);
      fscanf(comp->f, "%*[^\n]");
      getc(comp->f);
    }
  } while (level != *compmax);

  do {
    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1)) {
      skipoligo(comp);
      fscanf(comp->f, "%ld", &number);
      if (number != 0) {
	first->item->son[(long)ba] = (compnode *)Malloc(sizeof(compnode));
	first->item->son[(long)ba]->count = number;
	comptot->count += number;
      } else
	first->item->son[(long)ba] = NULL;
    }
    clearitem(&first, &V);
    fscanf(comp->f, "%*[^\n]");
    getc(comp->f);
  } while (first != NULL);
}

Static long getcount(root, start_)
compnode *root;
path *start_;
{
  compnode *place = root;
  path *point = start_;

  while (place != NULL && point != NULL) {
    place = place->son[(long)point->bas];
    point = point->next;
  }
  if (place == NULL)
    return 0;
  else
    return (place->count);
}

Static Void getmonocomposition(cmp, gna, gnc, gng, gnt, data, equalmono)
_TEXT *cmp;
long *gna, *gnc, *gng, *gnt;
_TEXT *data;
boolean *equalmono;
{
  long compmax;
  compnode *root;
  comptotal *monocomptotal;
  path *start_;
  base b1;
  long count;

  if (*cmp->name != '\0') {
    if (cmp->f != NULL)
      cmp->f = freopen(cmp->name, "r", cmp->f);
    else
      cmp->f = fopen(cmp->name, "r");
  } else
    rewind(cmp->f);
  if (cmp->f == NULL)
    _EscIO2(FileNotFound, cmp->name);
  RESETBUF(cmp->f, Char);
  if (emptyfile(cmp)) {
    fprintf(data->f, "* the composition file is empty.\n");
    fprintf(data->f, "* (equal mononucleotides assumed)\n");
    *gna = 1;
    *gnc = 1;
    *gng = 1;
    *gnt = 1;
    *equalmono = true;
    return;
  }
  putc('*', data->f);
  copyaline(cmp, data);
  putc('*', data->f);
  copyaline(cmp, data);

  readcomp(cmp, &compmax, 1L, &root, &monocomptotal);


  start_ = (path *)Malloc(sizeof(path));
  start_->next = NULL;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    start_->bas = b1;
    count = getcount(root, start_);
    switch (b1) {
    case a:
      *gna = count;
      break;
    case c:
      *gnc = count;
      break;
    case g:
      *gng = count;
      break;
    case t:
      *gnt = count;
      break;
    }
  }
  Free(start_);
  *equalmono = false;
}

#define maxsize         200
#define accuracy        10000

Static Void calehnb(n, gna, gnc, gng, gnt, hg, ehnb, varhnb)
long n, gna, gnc, gng, gnt;
double *hg, *ehnb, *varhnb;
{
  double log2Delila = log(2.0);
  double logn, nlog2;
  long gn;
  double logpa, logpc, logpg, logpt;
  double logfact[maxsize + 1];
  double mplog2p[maxsize + 1];
  long i;
  double logi;
  long na;
  long nc = 0, ng = 0, nt = 0;
  boolean done = false;
  double pnb;
  double hnb, pnbhnb;
  double sshnb = 0.0;
  double total = 0.0;
  long counter = 0;

  if (n > maxsize) {
    printf(" procedure calehnb: n > maxsize (%ld>%ld)\n", n, (long)maxsize);
    halt();
  }

  logn = log((double)n);
  nlog2 = n * log2Delila;
  gn = gna + gnc + gng + gnt;
  logpa = log((double)gna / gn);
  logpc = log((double)gnc / gn);
  logpg = log((double)gng / gn);
  logpt = log((double)gnt / gn);
  *hg = -((gna * logpa + gnc * logpc + gng * logpg + gnt * logpt) /
	  (gn * log2Delila));

  *ehnb = 0.0;
  logfact[0] = 0.0;
  mplog2p[0] = 0.0;
  for (i = 1; i <= n; i++) {
    logi = log((double)i);
    logfact[i] = logfact[i-1] + logi;
    mplog2p[i] = i * (logn - logi) / nlog2;
  }

  na = n;

  do {

    pnb = exp(logfact[n] - logfact[na] - logfact[nc] - logfact[ng] -
	      logfact[nt] + na * logpa + nc * logpc + ng * logpg + nt * logpt);
    hnb = mplog2p[na] + mplog2p[nc] + mplog2p[ng] + mplog2p[nt];
    pnbhnb = pnb * hnb;
    *ehnb += pnbhnb;
    sshnb += pnbhnb * hnb;
    counter++;
    total += pnb;

    if (nt > 0) {
      if (ng > 0) {
	ng--;
	nt++;
      } else if (nc > 0) {

	nc--;
	ng = nt + 1;
	nt = 0;
      } else if (na > 0) {

	na--;
	nc = nt + 1;
	nt = 0;
      } else
	done = true;
    } else {
      if (ng > 0) {
	ng--;
	nt++;
      } else if (nc > 0) {
	nc--;
	ng++;
      } else {
	na--;
	nc++;
      }
    }
  } while (!done);

  *varhnb = sshnb - *ehnb * *ehnb;

  if (accuracy != (long)floor(accuracy * total + 0.5)) {
    printf(" procedure calehnb: the sum of probabilities is\n");
    printf(" not accurate to one part in %ld\n", (long)accuracy);
    printf(" the sum of the probabilities is %10.8f\n", total);
  }

  if (counter == (long)floor((n + 1.0) * (n + 2) * (n + 3) / 6 + 0.5))
    return;

  printf(" procedure calehnb: program error, the number of\n");
  printf(" calculations is in error\n");
  halt();
}

#undef maxsize
#undef accuracy

Static Void calaehnb(n, gna, gnc, gng, gnt, hg, aehnb, avarhnb)
long n, gna, gnc, gng, gnt;
double *hg, *aehnb, *avarhnb;
{
  double log2Delila = log(2.0);
  long gn;
  double pa, pc, pg, pt, e;
  gn = gna + gnc + gng + gnt;
  pa = (double)gna / gn;
  pc = (double)gnc / gn;
  pg = (double)gng / gn;
  pt = (double)gnt / gn;
  *hg = -((pa * log(pa) + pc * log(pc) + pg * log(pg) + pt * log(pt)) / log2Delila);
  e = 3 / (2 * log2Delila * n);
  *aehnb = *hg - e;
  *avarhnb = e * e;
}

#define kickover        50

/* Local variables for makehnblist: */
struct LOC_makehnblist {
  long gna, gnc, gng, gnt;
  double *hg;
  long num;
} ;

Local Void fill(l, LINK)
ehnblist **l;
struct LOC_makehnblist *LINK;
{
  if (LINK->num == 0) {
    (*l)->aflag = ' ';
    return;
  }

  (*l)->n = LINK->num;

  if (LINK->num <= kickover) {
    calehnb(LINK->num, LINK->gna, LINK->gnc, LINK->gng, LINK->gnt, LINK->hg,
	    &(*l)->ehnb, &(*l)->varhnb);
    (*l)->aflag = 'e';
  } else {
    calaehnb(LINK->num, LINK->gna, LINK->gnc, LINK->gng, LINK->gnt, LINK->hg,
	     &(*l)->ehnb, &(*l)->varhnb);
    (*l)->aflag = 'a';
  }
}

Static Void makehnblist(n, gna_, gnc_, gng_, gnt_, l, hg_)
vector n;
long gna_, gnc_, gng_, gnt_;
ehnblist **l;
double *hg_;
{
  struct LOC_makehnblist V;
  long nindex = 1;
  ehnblist *lindex, *spare;
  boolean done;
  V.gna = gna_;
  V.gnc = gnc_;
  V.gng = gng_;
  V.gnt = gnt_;
  V.hg = hg_;
  *l = (ehnblist *)Malloc(sizeof(ehnblist));
  V.num = (long)floor(vget(n, nindex) + 0.5);
  fill(l, &V);
  (*l)->next = NULL;
  for (nindex = 2; nindex <= n.length; nindex++) {
    V.num = (long)floor(vget(n, nindex) + 0.5);

    if (V.num < (*l)->n) {
      spare = (ehnblist *)Malloc(sizeof(ehnblist));
      spare->next = *l;
      *l = spare;
      fill(l, &V);
    }    else {
      lindex = *l;
      done = false;

      while (!done) {
	if (lindex->next == NULL) {
	  done = true;
	  break;
	}
	if (lindex->next->n <= V.num)
	  lindex = lindex->next;
	else
	  done = true;
      }

      if (lindex->n != V.num) {
	if (lindex->next == NULL) {
	  lindex->next = (ehnblist *)Malloc(sizeof(ehnblist));
	  lindex = lindex->next;
	  lindex->next = NULL;
	}	else {
	  spare = (ehnblist *)Malloc(sizeof(ehnblist));
	  spare->next = lindex->next;
	  lindex->next = spare;
	  lindex = spare;
	}

	fill(&lindex, &V);
      }
    }
  }
}

#undef kickover

Static Void gethnb(l, n, ehnb, varhnb, aflag)
ehnblist *l;
long n;
double *ehnb, *varhnb;
Char *aflag;
{
  ehnblist *lindex = l;
  while (lindex->n != n && lindex->next != NULL)
    lindex = lindex->next;

  if (lindex->n != n) {
    printf(" gethnb: program error.\n");
    printf(" unable to find n = %ld\n", n);
    halt();
  }

  *ehnb = lindex->ehnb;
  *varhnb = lindex->varhnb;
  *aflag = lindex->aflag;
}

Static Void header(data, encseq)
_TEXT *data, *encseq;
{
  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "w", data->f);
    else
      data->f = fopen(data->name, "w");
  } else {
    if (data->f != NULL)
      rewind(data->f);
    else
      data->f = tmpfile();
  }
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  SETUPBUF(data->f, Char);
  fprintf(data->f,
	  "* rseq %4.2f rsequence calculated from encoded sequences from:\n",
	  version);
  if (*encseq->name != '\0') {
    if (encseq->f != NULL)
      encseq->f = freopen(encseq->name, "r", encseq->f);
    else
      encseq->f = fopen(encseq->name, "r");
  } else
    rewind(encseq->f);
  if (encseq->f == NULL)
    _EscIO2(FileNotFound, encseq->name);
  RESETBUF(encseq->f, Char);
  if (emptyfile(encseq)) {
    printf(" encseq is empty\n");
    halt();
  }
  putc('*', data->f);
  copyaline(encseq, data);
  putc('*', data->f);
  copyaline(encseq, data);
  fprintf(data->f, "*\n");
}

Static Void checkparams(theparameters)
parameter *theparameters;
{
  if (theparameters->window != 1) {
    printf(" rseq requires an encoding window length of 1\n");
    halt();
  }

  if (theparameters->wshift != 1) {
    printf(" rseq requires encoding window shifts of 1\n");
    halt();
  }

  if (theparameters->coding != 1) {
    printf(" rseq can only handle mononucleotide encodings\n");
    halt();
  }

  if (theparameters->cshift != 1) {
    printf(" rseq can only handle coding shifts of 1\n");
    halt();
  }

  if (theparameters->next != NULL) {
    printf(" rseq can only handle one encoding parameter set\n");
    halt();
  }
}

Static Void sumvectors(encseq, nbl, firstparam)
_TEXT *encseq;
vector *nbl;
parameter **firstparam;
{
  long regions, vsize;
  vector v;
  *firstparam = NULL;
  readencpar(encseq, firstparam, &regions, &vsize);
  makevector(nbl, vsize);
  makevector(&v, vsize);
  vset(0.0, nbl);

  while (!BUFEOF(encseq->f)) {
    readvector(encseq, &v);
    vadd(v, nbl);
  }
}

Static Void makenl(nbl, nl, firstparam)
vector nbl, *nl;
parameter **firstparam;
{
  parameter *aparam;
  long l;
  long v = 0;
  long w;
  double total;
  long FORLIM;

  makevector(nl, nbl.length);
  vset(0.0, nl);
  aparam = *firstparam;

  while (aparam != NULL) {
    l = aparam->range[(long)start];
    do {

      total = 0.0;
      FORLIM = aparam->wvlength;
      for (w = 1; w <= FORLIM; w++)
	total += vget(nbl, w + v);

      FORLIM = aparam->wvlength;
      for (w = 1; w <= FORLIM; w++)
	vput(nl, w + v, total);


      v += aparam->wvlength;

      l += aparam->wshift;
    } while (l <= aparam->range[(long)stop]);


    aparam = aparam->next;
  }
}

Static Void compressnl(nl, firstparam, thefrom, theto)
vector nl;
parameter *firstparam;
long *thefrom, *theto;
{
  parameter *aparam = firstparam;
  long l;
  double total;
  long v = 0;
  *thefrom = LONG_MAX;
  *theto = -LONG_MAX;

  while (aparam != NULL) {
    l = aparam->range[(long)start];

    do {

      total = vget(nl, v + 1);
      if (total > 0)
	*theto = l;
      if (*thefrom >= LONG_MAX) {
	if (total > 0)
	  *thefrom = l;
      }

      v += aparam->wvlength;
      l += aparam->wshift;
    } while (l <= aparam->range[(long)stop]);

    aparam = aparam->next;
  }
}

#define cmpnmin         1000

Static Void prepareehnb(cmp, data, nl, ehnb)
_TEXT *cmp, *data;
vector nl;
ehnblist **ehnb;
{
  long gmono[4];
  long genomicn;
  boolean equalmono;
  double hg;

  fprintf(data->f, "* genomic probabilities from:\n");
  getmonocomposition(cmp, gmono, &gmono[1], &gmono[2], &gmono[3], data,
		     &equalmono);
  genomicn = gmono[0] + gmono[1] + gmono[2] + gmono[3];
  if (!equalmono) {
    if (genomicn < cmpnmin) {
      printf(" ************  WARNING ***********\n");
      printf(" there should be at least\n");
      printf(" %4ld bases of composition information\n", (long)cmpnmin);
      printf(" to avoid needing to adjust for sampling\n");
      printf(" error in the genomic hg.\n");
      printf(" the error with %5ld bases is %*.*f bits\n",
	     genomicn, infofield, infodecim, 3 / (genomicn * 2 * log(2.0)));
    }
  }

  fprintf(data->f,
    "* the genomic composition used is:  a = %ld, c = %ld, g = %ld, t = %ld\n",
    gmono[0], gmono[1], gmono[2], gmono[3]);
  fprintf(data->f, "*\n");

  makehnblist(nl, gmono[0], gmono[1], gmono[2], gmono[3], ehnb, &hg);
  fprintf(data->f, "* genomic information, hg: %*.*f bits/base\n",
	  infofield, infodecim, hg);
  fprintf(data->f, "*\n");
}

#undef cmpnmin

Static Void colinfo(data)
_TEXT *data;
{
  fprintf(data->f, "* l        position in aligned set\n");
  fprintf(data->f, "* a..t     number of each base at the position l\n");
  fprintf(data->f, "* rs(l)    rsequence(l)\n");
  fprintf(data->f, "* rs       rsequence, running sum of rs(l)\n");
  fprintf(data->f, "* var(hnb) variance of hnb\n");
  fprintf(data->f, "* sum-var  running sum of var(hnb)\n");
  fprintf(data->f, "* n(l)     number of sequence examples at l\n");
  fprintf(data->f, "* e(hnb)   expectation of hnb\n");
  fprintf(data->f,
	  "* f        flag for calculation of e(hnb) and var(hnb):\n");
  fprintf(data->f, "*          e = exact, a = approximation\n");
}

Static Void colid(data)
_TEXT *data;
{
  fprintf(data->f, "*%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s f\n",
	  posfield, " l", (int)(nfield + 1), " a", (int)(nfield + 1), " c",
	  (int)(nfield + 1), " g", (int)(nfield + 1), " t",
	  (int)(infofield + 1), " rs(l)", (int)(infofield + 1), " rs",
	  (int)(infofield + 1), " var(hnb)", (int)(infofield + 1), " sum-var",
	  (int)(posfield + 1), " n(l)", (int)(infofield + 1), " e(hnb)");
}

Static Void finalcomments(data, rs, sumvarhnb)
_TEXT *data;
double rs, sumvarhnb;
{
  fprintf(data->f, "* rsequence = %*.*f +/- %*.*f bits/site\n",
	  infofield, infodecim, rs, infofield, infodecim, sqrt(sumvarhnb));
}

Static Void makers(data, nbl, nl, ehnblist_, theparameters, thefrom, theto)
_TEXT *data;
vector nbl, nl;
ehnblist *ehnblist_;
parameter *theparameters;
long thefrom, theto;
{
  Char aflag;
  long b;
  long bstart;
  long bstop;
  double ehnb, freq;
  long l;
  double ln2 = log(2.0);
  long rangestart, rangestop;
  double rsl;
  double rs = 0.0, sumvarhnb = 0.0;
  long symbols;
  double varhnb, wbl;
  _TEXT TEMP;

  rangestart = theparameters->range[(long)start];
  rangestop = theparameters->range[(long)stop];
  symbols = theparameters->wvlength;
  fprintf(data->f, "* %*ld %*ld is the range\n",
	  posfield, thefrom, posfield, theto);
  fprintf(data->f, "*\n");
  colinfo(data);
  fprintf(data->f, "*\n");
  colid(data);

  for (l = thefrom; l <= theto; l++) {
    fprintf(data->f, " %*ld", posfield, l);

    bstart = symbols * (l - rangestart) + 1;
    bstop = symbols * (l - rangestart) + symbols;
    gethnb(ehnblist_, (long)floor(vget(nl, bstart) + 0.5), &ehnb, &varhnb,
	   &aflag);
    rsl = 0.0;
    for (b = bstart; b <= bstop; b++) {
      fprintf(data->f, " %*ld", nfield, (long)floor(vget(nbl, b) + 0.5));
      freq = vget(nbl, b) / vget(nl, b);
      if (freq > 0.0)
	wbl = ehnb + log(freq) / ln2;
      else
	wbl = negativeinfinity;

      rsl += freq * wbl;
    }

    rs += rsl;
    sumvarhnb += varhnb;

    fprintf(data->f, " %*.*f %*.*f % .*E % .*E  %*ld %*.*f %c\n",
	    infofield, infodecim, rsl, infofield, infodecim, rs,
	    P_max((int)(infofield + 3) - 7, 1), varhnb,
	    P_max((int)(infofield + 3) - 7, 1), sumvarhnb, nfield,
	    (long)floor(vget(nl, bstart) + 0.5), infofield, infodecim, ehnb,
	    aflag);
  }

  TEMP.f = stdout;
  *TEMP.name = '\0';
  finalcomments(&TEMP, rs, sumvarhnb);
  finalcomments(data, rs, sumvarhnb);
}

Static Void themain(encseq, cmp, rsdata)
_TEXT *encseq, *cmp, *rsdata;
{
  vector nbl, nl;
  ehnblist *ehnb;
  parameter *theparameters;
  long thefrom, theto;

  printf(" rseq %4.2f\n", version);
  header(rsdata, encseq);
  sumvectors(encseq, &nbl, &theparameters);
  checkparams(theparameters);
  makenl(nbl, &nl, &theparameters);
  compressnl(nl, theparameters, &thefrom, &theto);
  prepareehnb(cmp, rsdata, nl, &ehnb);
  makers(rsdata, nbl, nl, ehnb, theparameters, thefrom, theto);
}

/* Print help for user */
void usage() {
  printf("\n");
  printf(" rseq: rsequence calculated from encoded sequences\n");
  printf("\n  rseq -c cmp -e encseq\n\n");
  printf(" parameters: \n");
  printf("   -c cmp, a composition file from the comp program \n");
  printf("   -e the output of the encode program\n\n");
  printf(" Outputs:\n");
  printf("   rsdata: a display of the information content of each position  \n");
  printf("           of the sequences, with the sampling error variance.\n");
  printf("   output: messages to user\n");
  printf("\n");
  printf("  version %4.2f\n", version);
  exit(EXIT_SUCCESS);
}

int main(int argc, Char **argv)
{
  list *WITH;
  long FORLIM;
  extern char *optarg;
	extern int optind;
	int c, err = 0; 
  /* flags marking arguments passed */
  int cflag=0;       /* cmp file name  */
  int eflag=0;       /* encseq file */
	char *cmpFile     = "cmp.txt";
  char *encseqFile  = "encseq.txt";

/* Process command line arguments  */
while ((c = getopt(argc, argv, "c:e:")) != -1)
		switch (c) {
		case 'c':
      cflag = 1;
			cmpFile = optarg;
      printf("cflag %s\n", cmpFile);
      break;
		case 'e':
      eflag = 1;
      encseqFile  = optarg;
      printf("parameters %s\n", encseqFile);
      break;
    case '?':
			err = 1;
			break;
		}

  /* Is the book file name present */  
	if (cflag == 0) {	/* -c cmp file is mandatory */ 
		fprintf(stderr, "%s: missing -c cmp file \n", argv[0]);
		usage();
		exit(1);
	} 

  /* parameters file  */  
  if (eflag == 0) { 
    fprintf(stderr, "%s: missing -e encseq file \n", argv[0]);
		usage();
		exit(1);
    }  

  if (err) {
		usage();
		exit(1);
	}

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  rsdata.f = NULL;
  strcpy(rsdata.name, "rsdata");
  cmp.f = NULL;
  strcpy(cmp.name, cmpFile);
  encseq.f = NULL;
  strcpy(encseq.name, encseqFile);
  themain(&encseq, &cmp, &rsdata);
_L1:
  if (encseq.f != NULL)
    fclose(encseq.f);
  if (cmp.f != NULL)
    fclose(cmp.f);
  if (rsdata.f != NULL)
    fclose(rsdata.f);
  exit(EXIT_SUCCESS);
  return 0;
}/* End. */
