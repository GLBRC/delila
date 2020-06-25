/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "encfrq.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*



*/



#define version         1.53
/*
*/



/*














































*/



#define vectormax       1616


/*
*/
/*

*/

typedef struct spacelist {
  long skips;
  struct spacelist *next;
} spacelist;

/*

*/
typedef enum {
  start, stop
} endpoints;

typedef struct parameter {
  long range[2];
  /*
*/
  long window, wshift, coding;
  /*
*/
  spacelist *spaces;
  long cshift;


  long wvlength;
  /*
*/
  long pvlength;
  /*
*/
  /*
*/

  struct parameter *next;
} parameter;



typedef long vector[vectormax];


typedef enum {
  a, c, g, t
} base;




typedef struct compnode {
  long count;
  struct compnode *son[4];
  /*
*/
} compnode;


typedef struct spider {
  /*


*/
  long depth;
  compnode *place;
  struct spider *next;
} spider;

/*
*/

typedef struct comptotal {
  long count;
  struct comptotal *next;
} comptotal;


typedef struct path {
  base bas;
  struct path *next;
} path;


Static _TEXT encseq, cmp, fout;


Static jmp_buf _JL1;


/*
*/


Static Void halt()
{
  /*





*/
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







Static long paramsize(param)
parameter *param;
{
  long rangesize, numwindows;

  /*
*/
  rangesize = param->range[(long)stop] - param->range[(long)start] + 1;
  numwindows = (long)((rangesize - 1.0) / param->wshift + 1);
  return (numwindows * param->wvlength);
}


Static Void readencpar(thefile, param, regions, endseq)
_TEXT *thefile;
parameter **param;
long *regions, *endseq;
{
  /*


*/
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
  if (BUFEOF(thefile->f)) {
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


  fscanf(thefile->f, "%ld%*[^\n]", endseq);
  getc(thefile->f);


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
}




/*



*/

Static boolean beginpv(aparam, element)
parameter *aparam;
long element;
{
  if (aparam != NULL)
    return (element == 0);
  else
    return true;
}


Static boolean endpv(aparam, element)
parameter *aparam;
long element;
{
  /*
*/
  if (aparam != NULL)
    return (element >= aparam->pvlength);
  else
    return true;
}


Static boolean beginwv(aparam, element)
parameter *aparam;
long element;
{
  /*
*/
  if (aparam != NULL)
    return (element % aparam->wvlength == 0);
  else
    return false;
}


Static boolean endwv(aparam, element)
parameter *aparam;
long element;
{
  /*
*/
  if (aparam != NULL)
    return ((element + 1) % aparam->wvlength == 0);
  else {
    return true;

  }
}


Static long encposition(aparam, element)
parameter *aparam;
long element;
{
  /*

*/
  return (aparam->range[(long)start] + element / aparam->wvlength * aparam->wshift);
}



Static Void encread(afile, val, firstparam, aparam, element, wvend, pvend,
		    vbegin, seqend, wvsum, wvempty)
_TEXT *afile;
long *val;
parameter **firstparam, **aparam;
long *element;
boolean *wvend, *pvend, *vbegin, *seqend;
long *wvsum;
boolean *wvempty;
{
  /*

*/
  /*
















*/
  long regions, endseq;
  parameter *WITH;

  *vbegin = false;
  if (*aparam == NULL) {
    *aparam = *firstparam;
    if (*aparam == NULL) {
      readencpar(afile, firstparam, &regions, &endseq);
      if (endseq >= 0) {
	printf(" encread: endseq must be negative.\n");
	halt();
      }
      *aparam = *firstparam;
    }
    *element = 0;
    *seqend = false;
    *vbegin = true;
  } else {
    (*element)++;
    if (endpv(*aparam, *element)) {
      *aparam = (*aparam)->next;
      if (*aparam == NULL)
	*aparam = *firstparam;
      *element = 0;
    }
  }

  fscanf(afile->f, "%ld", val);

  if (*val < 0) {
    if (*element != 0) {
      printf(
	" encread: end of vector detected but not time to begin: element = %ld\n",
	*element);
      halt();
    }
    fscanf(afile->f, "%*[^\n]");

    getc(afile->f);
    if (BUFEOF(afile->f))
      *seqend = true;
    else {
      fscanf(afile->f, "%ld", val);
      *vbegin = true;
    }
  }

  if (!*seqend) {
    WITH = *aparam;
    if (beginwv(*aparam, *element))
      *wvsum = 0;
    *wvsum += *val;

    *wvend = endwv(*aparam, *element);
    *pvend = endpv(*aparam, *element);

    if (*wvend)
      *wvempty = (*wvsum == 0);
    else
      *wvempty = false;
    return;
  }
  *wvend = true;
  *pvend = true;
  *vbegin = false;
  *wvempty = false;
}





Static Void evln(afile, element, curparam)
_TEXT *afile;
long element;
parameter *curparam;
{
  /*
*/
  if (beginwv(curparam, element))
    putc('\n', afile->f);
  else if (curparam == NULL)
    putc('\n', afile->f);


  if (beginpv(curparam, element))
    putc('\n', afile->f);
}


#define codingmax       4


/* Local variables for evheader: */
struct LOC_evheader {
  _TEXT *afile;
  Char bases[codingmax];
} ;

Local Void space(i, LINK)
long i;
struct LOC_evheader *LINK;
{
  long index;

  for (index = 1; index <= i; index++)
    putc(' ', LINK->afile->f);
}

Local Void advance(level, LINK)
long level;
struct LOC_evheader *LINK;
{
  switch (LINK->bases[level-1]) {

  case 'a':
    LINK->bases[level-1] = 'c';
    break;

  case 'c':
    LINK->bases[level-1] = 'g';
    break;

  case 'g':
    LINK->bases[level-1] = 't';
    break;

  case 't':
    LINK->bases[level-1] = 'a';
    advance(level - 1, LINK);
    break;
  }
}


Static Void evheader(afile_, posfield, valfield, firstparam, curparam)
_TEXT *afile_;
long posfield, valfield;
parameter *firstparam, **curparam;
{
  /*

*/
  struct LOC_evheader V;
  long i, j, n, size;
  /*
*/
  spacelist *s;
  parameter *WITH;
  long FORLIM, FORLIM1, FORLIM2;

  V.afile = afile_;
  if (*curparam == NULL)
    *curparam = firstparam;

  WITH = *curparam;
  if (WITH->coding > codingmax) {
    printf(" evheader: coding levels must not be higher than %ld\n",
	   (long)codingmax);
    halt();
  }


  size = WITH->coding;
  s = (*curparam)->spaces;
  while (s != NULL) {
    size += s->skips;
    s = s->next;
  }


  if (labs(valfield) < size) {
    printf(
      " evheader: valfield (%ld) can not be less than coding level and spaces (%ld)\n",
      valfield, size);
    halt();
  }


  FORLIM = WITH->coding;
  for (i = 0; i < FORLIM; i++)
    V.bases[i] = 'a';



  space(posfield * 2 + 5, &V);
  FORLIM = WITH->wvlength;
  for (i = 1; i <= FORLIM; i++) {
    space(labs(valfield) - size + 1, &V);
    s = WITH->spaces;
    FORLIM1 = WITH->coding;
    for (n = 0; n < FORLIM1; n++) {
      putc(V.bases[n], V.afile->f);
      if (s != NULL) {
	FORLIM2 = s->skips;
	for (j = 1; j <= FORLIM2; j++)
	  putc('x', V.afile->f);
	s = s->next;
      }
    }


    if (i != WITH->wvlength)
      advance(WITH->coding, &V);
  }
}

#undef codingmax


/* Local variables for evprint: */
struct LOC_evprint {
  _TEXT *afile;
  long posfield;
  double val;
  long valfield, valdecimal;
  boolean ln;
  parameter **curparam;
  long *element;
} ;

Local Void sider(LINK)
struct LOC_evprint *LINK;
{
  long position;
  parameter *WITH;

  /*
*/
  WITH = *LINK->curparam;
  position = encposition(*LINK->curparam, *LINK->element);
  fprintf(LINK->afile->f, " %*ld to %*ld",
	  (int)LINK->posfield, position, (int)LINK->posfield,
	  position + WITH->window - 1);
}

Local Void middler(LINK)
struct LOC_evprint *LINK;
{
  putc(' ', LINK->afile->f);
  if (LINK->valfield > 0) {
    if (LINK->valdecimal <= 0)
      fprintf(LINK->afile->f, "%*ld",
	      (int)LINK->valfield, (long)floor(LINK->val + 0.5));
    else
      fprintf(LINK->afile->f, "%*.*f",
	      (int)LINK->valfield, (int)LINK->valdecimal, LINK->val);
    return;
  }
  if (LINK->ln) {
    printf(" evprint: if valfield is negative, then ln must be false\n");
    halt();
  }
}

Local Void step(LINK)
struct LOC_evprint *LINK;
{
  (*LINK->element)++;

  if (endpv(*LINK->curparam, *LINK->element)) {
    *LINK->curparam = (*LINK->curparam)->next;
    *LINK->element = 0;
  }
}


Static Void evprint(afile_, posfield_, val_, valfield_, valdecimal_, ln_,
		    firstparam, curparam_, element_)
_TEXT *afile_;
long posfield_;
double val_;
long valfield_, valdecimal_;
boolean ln_;
parameter *firstparam, **curparam_;
long *element_;
{
  /*
*/
  /*
*/
  /*


*/

  /*




































*/
  struct LOC_evprint V;

  V.afile = afile_;
  V.posfield = posfield_;
  V.val = val_;
  V.valfield = valfield_;
  V.valdecimal = valdecimal_;
  V.ln = ln_;
  V.curparam = curparam_;
  V.element = element_;
  if (V.ln) {
    if (beginpv(*V.curparam, *V.element)) {
      evheader(V.afile, V.posfield, V.valfield, firstparam, V.curparam);
      putc('\n', V.afile->f);
    }
  }

  if (beginwv(*V.curparam, *V.element))
    sider(&V);

  middler(&V);

  step(&V);

  if (V.ln)
    evln(V.afile, *V.element, *V.curparam);
}








Static Void skipoligo(thefile)
_TEXT *thefile;
{
  /*


*/
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


/*


*/

typedef struct list {
  compnode *item;
  struct list *next;
} list;


/* Local variables for readcomp: */
struct LOC_readcomp {
  list *freeitem;
} ;

/*

*/
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
  /*






*/
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
  if (BUFEOF(comp->f)) {
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
/* p2c: encfrq.p: Note: Eliminated unused assignment statement [338] */
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
  /*

*/
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
  /*



*/
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




Static long vectorsize(param)
parameter *param;
{
  /*
*/
  long size = 0;

  while (param != NULL) {
    size += param->pvlength;
    param = param->next;
  }
  return size;
}




Static Void clearvector(v)
long *v;
{
  long index;

  for (index = 0; index < vectormax; index++)
    v[index] = 0;
}


#define posfield        4
#define valfield        6
#define decfield        3


#define maxcode         2
#define zflag           3


#define symbol          '.'


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *encseq, *cmp, *fout;
  vector sumx, n;

  parameter *firstparam, *aparam;
  long element;
} ;

Local Void header(LINK)
struct LOC_themain *LINK;
{
  long regions, endseq;

  printf(" encfrq %4.2f\n", version);
  if (*LINK->fout->name != '\0') {
    if (LINK->fout->f != NULL)
      LINK->fout->f = freopen(LINK->fout->name, "w", LINK->fout->f);
    else
      LINK->fout->f = fopen(LINK->fout->name, "w");
  } else {
    if (LINK->fout->f != NULL)
      rewind(LINK->fout->f);
    else
      LINK->fout->f = tmpfile();
  }
  if (LINK->fout->f == NULL)
    _EscIO2(FileNotFound, LINK->fout->name);
  SETUPBUF(LINK->fout->f, Char);
  fprintf(LINK->fout->f, " encfrq %4.2f encoded sequence frequencies from:\n",
	  version);
  if (*LINK->encseq->name != '\0') {
    if (LINK->encseq->f != NULL)
      LINK->encseq->f = freopen(LINK->encseq->name, "r", LINK->encseq->f);
    else
      LINK->encseq->f = fopen(LINK->encseq->name, "r");
  } else
    rewind(LINK->encseq->f);
  if (LINK->encseq->f == NULL)
    _EscIO2(FileNotFound, LINK->encseq->name);
  RESETBUF(LINK->encseq->f, Char);
  copyaline(LINK->encseq, LINK->fout);
  copyaline(LINK->encseq, LINK->fout);
  putc('\n', LINK->fout->f);

  LINK->aparam = NULL;
  LINK->firstparam = NULL;
  readencpar(LINK->encseq, &LINK->firstparam, &regions, &endseq);

  if (vectorsize(LINK->firstparam) > vectormax) {
    printf(" the vectors are %ld long.  this exceeds the maximum which is %ld\n",
	   vectorsize(LINK->firstparam), (long)vectormax);
    halt();
  }
}

Local Void scan(LINK)
struct LOC_themain *LINK;
{
  long pos, x, index;

  long wvsum;
  boolean wvend, pvend, vbegin, seqend, wvempty;
  parameter *WITH;
  long FORLIM;

  clearvector(LINK->sumx);
  clearvector(LINK->n);

  do {
    encread(LINK->encseq, &x, &LINK->firstparam, &LINK->aparam,
	    &LINK->element, &wvend, &pvend, &vbegin, &seqend, &wvsum,
	    &wvempty);

    if (!seqend) {
      if (vbegin)
	pos = 1;
      else
	pos++;

      LINK->sumx[pos-1] += x;

      if (wvend) {
	if (!wvempty) {
	  WITH = LINK->aparam;
	  FORLIM = pos - WITH->wvlength;
	  for (index = pos - 1; index >= FORLIM; index--)
	    LINK->n[index] += wvsum;
	}
      }
    }
  } while (!seqend);
}

Local Void makefreq(LINK)
struct LOC_themain *LINK;
{
  long pos;
  double freq;
  long FORLIM;

  LINK->element = 0;
  LINK->aparam = NULL;

  FORLIM = vectorsize(LINK->firstparam);
  for (pos = 0; pos < FORLIM; pos++) {
    if (LINK->n[pos] == 0) {
      if (beginpv(LINK->aparam, LINK->element)) {
	evheader(LINK->fout, (long)posfield, (long)valfield, LINK->firstparam,
		 &LINK->aparam);
	putc('\n', LINK->fout->f);
      }
      evprint(LINK->fout, (long)posfield, 0.0, -1L, 0L, false,
	      LINK->firstparam, &LINK->aparam, &LINK->element);
      fprintf(LINK->fout->f, "%*c", valfield, ' ');
      evln(LINK->fout, LINK->element, LINK->aparam);
    } else {
      freq = (double)LINK->sumx[pos] / LINK->n[pos];

      evprint(LINK->fout, (long)posfield, freq, (long)valfield,
	      (long)decfield, true, LINK->firstparam, &LINK->aparam,
	      &LINK->element);
    }
  }
}

/* Local variables for makez: */
struct LOC_makez {
  struct LOC_themain *LINK;
  long curcoding;
  /*
*/

  double pmono[4];
  double pdi[16];

  long ztable[16];
} ;

/* Local variables for makeprobabilities: */
struct LOC_makeprobabilities {
  struct LOC_makez *LINK;
} ;

Local Void equalmono(LINK)
struct LOC_makeprobabilities *LINK;
{
  double e = 1.0 / 4;
  long p;

  for (p = 0; p <= 3; p++)
    LINK->LINK->pmono[p] = e;
  fprintf(LINK->LINK->LINK->fout->f, " equal mononucleotides assumed\n");
}

Local Void equaldi(LINK)
struct LOC_makeprobabilities *LINK;
{
  double e = 1.0 / 16;
  long p;

  for (p = 0; p <= 15; p++)
    LINK->LINK->pdi[p] = e;
  fprintf(LINK->LINK->LINK->fout->f, " equal   dinucleotides assumed\n");
}

Local Void makeprobabilities(LINK)
struct LOC_makez *LINK;
{
  struct LOC_makeprobabilities V;

  long compmax;
  long readmax = maxcode;
  compnode *root;
  comptotal *monocomptotal;
  path *start_;
  base b1, b2;
  double total = 0.0;
  long place;

  V.LINK = LINK;
  if (*LINK->LINK->cmp->name != '\0') {
    if (LINK->LINK->cmp->f != NULL)
      LINK->LINK->cmp->f = freopen(LINK->LINK->cmp->name, "r",
				   LINK->LINK->cmp->f);
    else
      LINK->LINK->cmp->f = fopen(LINK->LINK->cmp->name, "r");
  } else
    rewind(LINK->LINK->cmp->f);
  if (LINK->LINK->cmp->f == NULL)
    _EscIO2(FileNotFound, LINK->LINK->cmp->name);
  RESETBUF(LINK->LINK->cmp->f, Char);
  if (BUFEOF(LINK->LINK->cmp->f)) {
    equalmono(&V);
    equaldi(&V);
    return;
  }
  fprintf(LINK->LINK->fout->f, " probabilities from:\n");
  copyaline(LINK->LINK->cmp, LINK->LINK->fout);
  copyaline(LINK->LINK->cmp, LINK->LINK->fout);

  readcomp(LINK->LINK->cmp, &compmax, readmax, &root, &monocomptotal);


  start_ = (path *)Malloc(sizeof(path));
  start_->next = NULL;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    start_->bas = b1;
    LINK->pmono[(int)b1] = getcount(root, start_);
    total += LINK->pmono[(int)b1];
  }

  for (place = 0; place <= 3; place++)
    LINK->pmono[place] /= total;

  if (compmax < 2) {
    equaldi(&V);
    return;
  }

  start_->next = (path *)Malloc(sizeof(path));
  start_->next->next = NULL;
  total = 0.0;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    start_->bas = b1;
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
      start_->next->bas = b2;
      place = (int)b1 * 4 + (int)b2;
      LINK->pdi[place] = getcount(root, start_);
      total += LINK->pdi[place];
    }
  }

  for (place = 0; place <= 15; place++)
    LINK->pdi[place] /= total;
}

Local Void makeheader(LINK)
struct LOC_makez *LINK;
{
  evheader(LINK->LINK->fout, (long)posfield, (long)valfield,
	   LINK->LINK->firstparam, &LINK->LINK->aparam);
  fprintf(LINK->LINK->fout->f, "  z footprint\n");
}

/* Local variables for zfootprint: */
struct LOC_zfootprint {
  struct LOC_makez *LINK;
} ;

Local Void letter(i, LINK)
long i;
struct LOC_zfootprint *LINK;
{
  switch (i) {

  case 0:
    putc('a', LINK->LINK->LINK->fout->f);
    break;

  case 1:
    putc('c', LINK->LINK->LINK->fout->f);
    break;

  case 2:
    putc('g', LINK->LINK->LINK->fout->f);
    break;

  case 3:
    putc('t', LINK->LINK->LINK->fout->f);
    break;
  }
}

Local Void zfootprint(LINK)
struct LOC_makez *LINK;
{
  struct LOC_zfootprint V;
  long column, size, z;

  V.LINK = LINK;
  switch (LINK->curcoding) {

  case 1:
    size = 3;
    break;

  case 2:
    size = 15;
    break;
  }
  putc(' ', LINK->LINK->fout->f);
  for (column = 0; column <= size; column++) {
    putc(' ', LINK->LINK->fout->f);
    z = LINK->ztable[column];
    if (labs(z) > 0) {
      if (z > 0)
	putc('+', LINK->LINK->fout->f);
      else
	putc('-', LINK->LINK->fout->f);

      if (labs(z) > 9)
	putc('+', LINK->LINK->fout->f);
      else
	fprintf(LINK->LINK->fout->f, "%ld", labs(z));

      if (LINK->curcoding == 2)
	letter(column / 4, &V);

      letter(column & 3, &V);
    } else {
      fprintf(LINK->LINK->fout->f, "%c%c%c", symbol, symbol, symbol);
      if (LINK->curcoding == 2)
	putc(symbol, LINK->LINK->fout->f);
    }
  }
}

#undef symbol

Local Void makez(LINK)
struct LOC_themain *LINK;
{
  struct LOC_makez V;

  long pos;
  double p, q, mean, stdev, z;
  long warn, FORLIM;
  parameter *WITH;

  V.LINK = LINK;
  fprintf(LINK->fout->f, "\f");
  fprintf(LINK->fout->f, " z values for frequencies: z footprint\n\n");
  fprintf(LINK->fout->f, " z  p(x >= z)\n");
  fprintf(LINK->fout->f, " 1    0.3174\n");
  fprintf(LINK->fout->f, " 2    0.0456\n");
  fprintf(LINK->fout->f, " 3    0.0026\n\n");
  makeprobabilities(&V);
  fprintf(LINK->fout->f, "\n * means insufficient data for z estimate\n\n");

  LINK->element = 0;
  LINK->aparam = LINK->firstparam;
  FORLIM = vectorsize(LINK->firstparam);
  for (pos = 0; pos < FORLIM; pos++) {
    if (LINK->n[pos] == 0) {
      if (beginpv(LINK->aparam, LINK->element))
	makeheader(&V);
      evprint(LINK->fout, (long)posfield, 0.0, -1L, 0L, false,
	      LINK->firstparam, &LINK->aparam, &LINK->element);
      fprintf(LINK->fout->f, "%*c", valfield, ' ');
      evln(LINK->fout, LINK->element, LINK->aparam);
    } else {
      WITH = LINK->aparam;


      if (WITH->coding > maxcode) {
	printf(" maximum coding is %ld for this program\n", (long)maxcode);
	halt();
      }
      V.curcoding = WITH->coding;
      switch (V.curcoding) {

      case 1:
	p = V.pmono[LINK->element % WITH->wvlength];
	break;

      case 2:
	p = V.pdi[LINK->element % WITH->wvlength];
	break;
      }
      q = 1 - p;
      mean = LINK->n[pos] * p;
      stdev = sqrt(mean * q);

      z = (LINK->sumx[pos] - 0.5 - mean) / stdev;

      if (mean < 5.0 || LINK->n[pos] * q < 5.0)
	warn = 1;
      else
	warn = 0;


      WITH = LINK->aparam;

      if (fabs(z) >= zflag)
	V.ztable[LINK->element % WITH->wvlength] = (long)z;
      else
	V.ztable[LINK->element % WITH->wvlength] = 0;
      if (beginpv(LINK->aparam, LINK->element))
	makeheader(&V);
      evprint(LINK->fout, (long)posfield, z, valfield - warn, decfield - warn,
	      false, LINK->firstparam, &LINK->aparam, &LINK->element);
      if (warn == 1)
	putc('*', LINK->fout->f);

      if (beginwv(LINK->aparam, LINK->element) | endpv(LINK->aparam,
						       LINK->element))
	zfootprint(&V);


      evln(LINK->fout, LINK->element, LINK->aparam);
    }
    if (beginpv(LINK->aparam, LINK->element))
      putc('\n', LINK->fout->f);

  }
}

#undef maxcode
#undef zflag


Static Void themain(encseq_, cmp_, fout_)
_TEXT *encseq_, *cmp_, *fout_;
{
  struct LOC_themain V;

  V.encseq = encseq_;
  V.cmp = cmp_;
  V.fout = fout_;
  header(&V);
  scan(&V);
  makefreq(&V);
  makez(&V);
}

#undef posfield
#undef valfield
#undef decfield


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fout.f = NULL;
  strcpy(fout.name, "fout");
  cmp.f = NULL;
  strcpy(cmp.name, "cmp");
  encseq.f = NULL;
  strcpy(encseq.name, "encseq");
  themain(&encseq, &cmp, &fout);
_L1:
  if (encseq.f != NULL)
    fclose(encseq.f);
  if (cmp.f != NULL)
    fclose(cmp.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
