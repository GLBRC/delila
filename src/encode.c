/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "encode.p" */
/*
encode a book of sequences into strings of integers.
  by Gary Stormo.

modified by:

  Dr. Thomas D. Schneider
  toms@alum.mit.edu
  permanent email: toms@alum.mit.edu (use only if first address fails)
  https://alum.mit.edu/www/toms/

modules needed: delmods, delman, matmods *

name
   encode: encodes a book of sequences into strings of integers

synopsis
   encode(inst: in, book: in, encseq: out, encodep: in, output: out)

files
   inst: the instructions generating the book; for aligning the sequences
       If the inst file is empty, then the sequences are aligned by
       the zero coordinate of the book (this allows the use of the
       "default coordinate zero" option of Delila) or by the first
       base of the piece, as defined by the first parameter.

   book: the sequences to be encoded

   encseq: the encoded sequences

   encodep: parameter file for describing how the sequences are to be
         encoded.

   The first parameter, the first character on the first line, defines how
   to align the pieces.  See the alist program for the detailed logic.
   There are three choices, as in alist:

      'f' (for 'first') then the sequences are always aligned by their
      first base.

      'i' then the sequences are aligned by the delila instructions.  If
      the inst file is empty, alignment is forced to the 'b' mode.

      'b' (for 'internal') then the alignment is on the internal zero of
      the book's sequence.  This option is to be used when "default
      coordinate zero" is used in the Delila instructions.

   The remaining parameters are stored as a list of parameter records, of
   which there may be any number.  Each parameter record has five lines of
   information which it must include (all i's and j's are integers):


1.  i j specify the nucleotides, relative to the aligned base,
           over which this parameter record is to operate; these may
           be any integers, but i <= j is required;
   2.  i   is the size of the windows to be encoded; within the window
           the number of each oligonucleotide of length 'coding' are
           determined and printed as part of the total sequence vector;
   3.  i   is the shift to the next window to be encoded;
   4.  i : j1 j2 j3 ...  is the 'coding'-level and arrangement; the
           'coding'-level, i, is the number of nucleotides in the oligos we
           are counting, i.e., 1 means monos, 2 means dis, ...;  if i > 1
           then we can also skip bases between the ones we are encoding;
           if the i is followed next by a colon, there must be i-1 integers
           (j1..j(i-1)) which specify the number of bases to be skipped
           between the ones which are encoded; for example, if we have the
           sequence xyz and we are interested in the di-nucleotides we can
           get the xy by the parameter '2 : 0', or we could get the xz by
           parameter '2 : 1'; if there is no colon all the skips are
           assumed to be zero;
   5.  i   is the shift to the next coding site within the window;
           this allows us to encode only some of the oligos within a window,
           such as only those that are in-frame;
   multiple parameter records can be concatenated in the encodep file
   and then each sequence in the book will be encoded according to each
   parameter record into a single vector of integers.

   output: for messages to the user

description

   This program is used to encode a book of sequences into a string of
   integers.  Each sequence in the book is encoded into a single string of
   integers (ended by an 'end of sequence' symbol) according to the user
   specified parameters, which are in the file 'encodep'.

To Compile:

gcc encode.c -o encode -I/home/mplace/bin/p2c/src -L /home/mplace/bin/p2c/src -lm -lp2c

To Run:


*/

#include <getopt.h>  /* getopt API */ 
#include <stdio.h>   /* printf */
#include <stdlib.h> 
#include </home/mplace/bin/p2c/src/p2c.h>

#define version         1.42
#define dnamax          1024
#define namelength      100
#define linelength      200
#define maxvecpart      64
#define vpagewidth      64
#define pagewidth       32
#define codingmax       6
#define maxstring       2000
#define fillermax       50

typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;

typedef Char filler[fillermax];

typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;

typedef long chset[5];
typedef Char alpha[namelength];

typedef struct name {
  alpha letters;
  char length;
} name;

typedef struct line {
  Char letters[linelength];
  uchar length;
  struct line *next;
} line;

typedef enum {
  plus, minus, dircomplement, dirhomologous
} direction;
typedef enum {
  linear, circular
} configuration;
typedef enum {
  on, off
} state;

typedef struct header {
  name keynam;
  line *fulnam, *note;
} header;

typedef enum {
  a, c, g, t
} base;

typedef short dnarange;

/* p2c: encode.p, line 242:
 * Note: Field width for seq assumes enum base has 4 elements [105] */
typedef uchar seq[(dnamax + 3) / 4];

typedef struct dnastring {
  seq part;
  dnarange length;
  struct dnastring *next;
} dnastring;

typedef struct orgkey {
  header hea;
  line *mapunit;
} orgkey;

typedef struct chrkey {
  header hea;
  double mapbeg, mapend;
} chrkey;

typedef struct piekey {
  header hea;
  double mapbeg;
  configuration coocon;
  direction coodir;
  long coobeg, cooend;
  configuration piecon;
  direction piedir;
  long piebeg, pieend;
} piekey;

typedef struct piece {
  piekey key;
  dnastring *dna;
} piece;

typedef struct reference {
  name pienam;
  double mapbeg;
  direction refdir;
  long refbeg, refend;
} reference;

typedef struct genkey {
  header hea;
  reference ref;
} genkey;

typedef struct trakey {
  header hea;
  reference ref;
} trakey;

typedef struct markey {
  header hea;
  reference ref;
  state sta;
  line *phenotype;
  struct marker *next;
} markey;

typedef struct marker {
  markey key;
  dnastring *dna;
} marker;

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

Static _TEXT inst;
Static _TEXT book, encseq, encodep;
Static long regions, length_, alignedbase;
Static piece *apiece;
Static parameter *firstparam;
Static long fourpowers[codingmax + 1];
Static vector seqvector;
Static boolean noinst;
Static long theline;
Static Char alignmenttype;
Static line *freeline;
Static dnastring *freedna;
Static boolean readnumber;
Static long number;
Static boolean numbered, skipunnum;
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

Static long copylines(fin, fout, n)
_TEXT *fin, *fout;
long n;
{
  long index = 0;

  while (!BUFEOF(fin->f) && index < n) {
    copyaline(fin, fout);
    index++;
  }

  return index;
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

Static Void writevector(thefile, v, y, z)
_TEXT *thefile;
vector v;
long y, z;
{
  long pos = 0;
  long i, j, numparts, lastpart;
  vectorpart *firstpart;
  long x;

  x = (long)(vpagewidth / (y + 1.0));
  firstpart = v.part;
  numparts = (v.length - 1) / maxvecpart + 1;
  lastpart = ((v.length - 1) & (maxvecpart - 1)) + 1;
  if (z == 0) {
    for (i = 1; i < numparts; i++) {
      for (j = 0; j < maxvecpart; j++) {
	fprintf(thefile->f, " %*ld",
		(int)y, (long)floor(v.part->numbers[j] + 0.5));
	pos++;
	if (pos % x == 0)
	  putc('\n', thefile->f);
      }
      v.part = v.part->next;
    }
    for (j = 0; j < lastpart; j++) {
      fprintf(thefile->f, " %*ld",
	      (int)y, (long)floor(v.part->numbers[j] + 0.5));
      pos++;
      if (pos < v.length && pos % x == 0)
	putc('\n', thefile->f);
    }
  } else {
    for (i = 1; i < numparts; i++) {
      for (j = 0; j < maxvecpart; j++) {
	fprintf(thefile->f, " %*.*f", (int)y, (int)z, v.part->numbers[j]);
	pos++;
	if (pos % x == 0)
	  putc('\n', thefile->f);
      }
      v.part = v.part->next;
    }
    for (j = 0; j < lastpart; j++) {
      fprintf(thefile->f, " %*.*f", (int)y, (int)z, v.part->numbers[j]);
      pos++;
      if (pos < v.length && pos % x == 0)
	putc('\n', thefile->f);
    }
  }
  putc('\n', thefile->f);
  v.part = firstpart;
}

Static double dotproduct(vectora, vectorb)
vector vectora, vectorb;
{
  long i;
  double j = 0.0;

  if (vectora.length != vectorb.length) {
    printf(" function dotproduct: vector lengths must be equal\n");
    halt();
  }
  for (i = 1; i <= vectora.length; i++)
    j += vget(vectora, i) * vget(vectorb, i);
  return j;
}

Static double magnitude(v)
vector *v;
{
  return sqrt(dotproduct(*v, *v));
}

Static Void normalize(v)
vector *v;
{
  long i;
  double length;
  long FORLIM;

  length = magnitude(v);
  FORLIM = v->length;
  for (i = 1; i <= FORLIM; i++)
    vput(v, i, vget(*v, i) / length);
}

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

Static Void getlineDelila(l)
line **l;
{
  if (freeline != NULL) {
    *l = freeline;
    freeline = freeline->next;
  } else
    *l = (line *)Malloc(sizeof(line));
  (*l)->length = 0;
  (*l)->next = NULL;
}

Static Void getdna(l)
dnastring **l;
{
  if (freedna != NULL) {
    *l = freedna;
    freedna = freedna->next;
  } else
    *l = (dnastring *)Malloc(sizeof(dnastring));
  (*l)->length = 0;
  (*l)->next = NULL;
}

Static Void clearline(l)
line **l;
{
  line *lptr;

  if (*l == NULL)
    return;
  lptr = *l;
  *l = (*l)->next;
  lptr->next = freeline;
  freeline = lptr;
}

Static Void writeline(afile, l, carriagereturn)
_TEXT *afile;
line *l;
boolean carriagereturn;
{
  long index, FORLIM;
  FORLIM = l->length;

  for (index = 0; index < FORLIM; index++)
    putc(l->letters[index], afile->f);
  if (carriagereturn)
    putc('\n', afile->f);
}

Static Void showfreedna()
{
  long counter = 0;
  dnastring *l;

  l = freedna;
  while (l != NULL) {
    counter++;
    printf("%ld", counter);
    printf(", length = %d\n", l->length);
    l = l->next;
  }
}

Static Void cleardna(l)
dnastring **l;
{
  dnastring *lptr;

  if (*l == NULL)
    return;
  lptr = *l;
  *l = (*l)->next;
  lptr->next = freedna;
  freedna = lptr;
}

Static Void clearheader(h)
header *h;
{
  clearline(&h->fulnam);
  while (h->note != NULL)
    clearline(&h->note);
}

Static Void clearpiece(p)
piece **p;
{
  while ((*p)->dna != NULL)
    cleardna(&(*p)->dna);
  clearheader(&(*p)->key.hea);
}

Static base chartobase(ch)
Char ch;
{
  base Result;

  switch (ch) {
  case 'a':
    Result = a;
    break;
  case 'c':
    Result = c;
    break;
  case 'g':
    Result = g;
    break;
  case 't':
    Result = t;
    break;
  }
  return Result;
}


Static Char basetochar(ba)
base ba;
{
  Char Result;

  switch (ba) {
  case a:
    Result = 'a';
    break;
  case c:
    Result = 'c';
    break;
  case g:
    Result = 'g';
    break;
  case t:
    Result = 't';
    break;
  }
  return Result;
}


Static base complement(ba)
base ba;
{
  base Result;

  switch (ba) {
  case a:
    Result = t;
    break;
  case c:
    Result = g;
    break;
  case g:
    Result = c;
    break;
  case t:
    Result = a;
    break;
  }
  return Result;
}


Static Char chomplement(b)
Char b;
{
  return (basetochar(complement(chartobase(b))));
}

Static long pietoint(p, pie)
long p;
piece *pie;
{
  long i;
  piekey *WITH;

  WITH = &pie->key;
  switch (WITH->piedir) {

  case dirhomologous:
  case plus:
    if (p >= WITH->piebeg)
      i = p - WITH->piebeg + 1;
    else
      i = p - WITH->coobeg + WITH->cooend - WITH->piebeg + 2;
    break;
  case dircomplement:
  case minus:
    if (p <= WITH->piebeg)
      i = WITH->piebeg - p + 1;
    else
      i = WITH->cooend - p + WITH->piebeg - WITH->coobeg + 2;
    break;
  }
  return i;
}

Static long inttopie(i, pie)
long i;
piece *pie;
{
  long p;
  piekey *WITH;

  WITH = &pie->key;
  switch (WITH->piedir) {

  case dirhomologous:
  case plus:
    p = WITH->piebeg + i - 1;
    if (p > WITH->cooend) {
      if (WITH->coocon == circular)
	p += WITH->coobeg - WITH->cooend - 1;
    }
    break;

  case dircomplement:
  case minus:
    p = WITH->piebeg - i + 1;
    if (p < WITH->coobeg) {
      if (WITH->coocon == circular)
	p += WITH->cooend - WITH->coobeg + 1;
    }
    break;
  }
  return p;
}

Static long piecelength(pie)
piece *pie;
{
  return (pietoint(pie->key.pieend, pie));
}

Static Char getto(thefile, theline, ch)
_TEXT *thefile;
long *theline;
long *ch;
{
  Char achar = ' ';
  boolean done = false;

  while (!done) {
    if (BUFEOF(thefile->f)) {
      done = true;
      break;
    }
    achar = P_peek(thefile->f);
    if (P_inset(achar, ch)) {
      done = true;
      break;
    }
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    (*theline)++;
  }
  if (P_inset(achar, ch))
    return achar;
  else {
    return ' ';
  
  }
}

Static Void skipstar(thefile)
_TEXT *thefile;
{
  if (BUFEOF(thefile->f)) {
    printf(" procedure skipstar: end of book found\n");
    halt();
    return;
  }
  if (P_peek(thefile->f) != '*') {
    printf(" procedure skipstar: bad book\n");
    printf(" \"*\" expected as first character on the line, but \"%c\" was found\n",
	   P_peek(thefile->f));
    halt();
  }
  getc(thefile->f);
  if (P_peek(thefile->f) != ' ') {
    printf(" procedure skipstar: bad book\n");
    printf(" \"* \" expected on a line but \"*%c\" was found\n",
	   P_peek(thefile->f));
    halt();
  }
  getc(thefile->f);
}

Static Void brreanum(thefile, theline, reanum)
_TEXT *thefile;
long *theline;
double *reanum;
{
  skipstar(thefile);
  fscanf(thefile->f, "%lg%*[^\n]", reanum);
  getc(thefile->f);
  (*theline)++;
}

Static Void brnumber(thefile, theline, num)
_TEXT *thefile;
long *theline, *num;
{
  skipstar(thefile);
  fscanf(thefile->f, "%ld%*[^\n]", num);
  getc(thefile->f);
  (*theline)++;
}

Static Void brname(thefile, theline, nam)
_TEXT *thefile;
long *theline;
name *nam;
{
  long i;
  Char c_;

  skipstar(thefile);
  nam->length = 0;
  do {
    nam->length++;
    c_ = getc(thefile->f);
    if (c_ == '\n')
      c_ = ' ';
    nam->letters[nam->length - 1] = c_;
  } while (!(P_eoln(thefile->f) || nam->length >= namelength ||
	     nam->letters[nam->length - 1] == ' '));
  if (nam->letters[nam->length - 1] == ' ')
    nam->length--;
  if (nam->length < namelength) {
    for (i = nam->length; i < namelength; i++)
      nam->letters[i] = ' ';
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

Static Void brline(thefile, theline, l)
_TEXT *thefile;
long *theline;
line **l;
{
  long i = 0;
  Char acharacter;
  skipstar(thefile);

  while (!P_eoln(thefile->f) && i < linelength) {
    i++;
    acharacter = getc(thefile->f);
    if (acharacter == '\n')
      acharacter = ' ';
    (*l)->letters[i-1] = acharacter;
  }

  if (!P_eoln(thefile->f)) {
    printf("***********************************************\n");
    printf("* WARNING: brline: book line length exceeded\n");
    printf("* linelength > %ld characters\n", (long)linelength);
    printf("* Only %ld characters read from book\n", (long)linelength);
    printf("***********************************************\n");
  }
  (*l)->length = i;
  (*l)->next = NULL;
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

Static Void brdirect(thefile, theline, direct)
_TEXT *thefile;
long *theline;
direction *direct;
{
  Char ch;
  skipstar(thefile);
  fscanf(thefile->f, "%c%*[^\n]", &ch);
  getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  (*theline)++;
  if (ch == '+')
    *direct = plus;
  else
    *direct = minus;
}

Static Void brconfig(thefile, theline, config)
_TEXT *thefile;
long *theline;
configuration *config;
{
  Char ch;
  skipstar(thefile);
  fscanf(thefile->f, "%c%*[^\n]", &ch);
  getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  (*theline)++;
  if (ch == 'l')
    *config = linear;
  else
    *config = circular;
}

Static Void brnotenumber(thefile, theline, note)
_TEXT *thefile;
long *theline;
line **note;
{
  *note = NULL;
  numbered = false;
  number = 0;
 
  if (P_peek(thefile->f) != 'n')
    return;
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
  if (P_peek(thefile->f) != 'n') {
    skipstar(thefile);
    if (!P_eoln(thefile->f)) {
      if (P_peek(thefile->f) == '#') {
	numbered = true;
	getc(thefile->f);
	fscanf(thefile->f, "%ld", &number);
      }
    }
    do {
      fscanf(thefile->f, "%*[^\n]");
      getc(thefile->f);
      (*theline)++;
    } while (P_peek(thefile->f) != 'n');
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    (*theline)++;
    return;
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

Static Void brnote(thefile, theline, note)
_TEXT *thefile;
long *theline;
line **note;
{
  line *newnote, *previousnote;

  *note = NULL;
  if (P_peek(thefile->f) != 'n')
    return;
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
  if (P_peek(thefile->f) != 'n') {
    getlineDelila(note);
    newnote = *note;
    while (P_peek(thefile->f) != 'n') {
      brline(thefile, theline, &newnote);
      previousnote = newnote;

      getlineDelila(&newnote->next);
      newnote = newnote->next;
    }

    clearline(&newnote);
    previousnote->next = NULL;
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    (*theline)++;
    return;
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

Static Void brheader(thefile, theline, hea)
_TEXT *thefile;
long *theline;
header *hea;
{
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
  brname(thefile, theline, &hea->keynam);
  getlineDelila(&hea->fulnam);
  brline(thefile, theline, &hea->fulnam);

  if (readnumber)
    brnotenumber(thefile, theline, &hea->note);
  else
    brnote(thefile, theline, &hea->note);
}

Static Void copyheader(fromhea, tohea)
header fromhea, *tohea;
{
  memcpy(tohea->keynam.letters, fromhea.keynam.letters, sizeof(alpha));
  tohea->keynam.length = fromhea.keynam.length;
  tohea->note = fromhea.note;
  tohea->fulnam = fromhea.fulnam;
}

Static Void brpiekey(thefile, theline, pie)
_TEXT *thefile;
long *theline;
piekey *pie;
{
  brheader(thefile, theline, &pie->hea);
  brreanum(thefile, theline, &pie->mapbeg);
  brconfig(thefile, theline, &pie->coocon);
  brdirect(thefile, theline, &pie->coodir);
  brnumber(thefile, theline, &pie->coobeg);
  brnumber(thefile, theline, &pie->cooend);
  brconfig(thefile, theline, &pie->piecon);
  brdirect(thefile, theline, &pie->piedir);
  brnumber(thefile, theline, &pie->piebeg);
  brnumber(thefile, theline, &pie->pieend);
}

Static Void brdna(thefile, theline, dna)
_TEXT *thefile;
long *theline;
dnastring **dna;
{
  Char ch;
  dnastring *workdna;
  long SET[5];
  long TEMP;

  getdna(dna);
  workdna = *dna;
  ch = getto(thefile, theline, P_addset(P_expset(SET, 0L), 'd'));
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
  ch = getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  while (ch == '*') {
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
    do {
      ch = getc(thefile->f);
      if (ch == '\n')
	ch = ' ';
      if (ch == 't' || ch == 'g' || ch == 'c' || ch == 'a') {
	if (workdna->length == dnamax) {
	  getdna(&workdna->next);
	  workdna = workdna->next;
	}
	workdna->length++;
	TEMP = workdna->length - 1;
	P_clrbits_B(workdna->part, TEMP, 1, 3);
	P_putbits_UB(workdna->part, TEMP, (int)chartobase(ch), 1, 3);
      }
    } while (!P_eoln(thefile->f));
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    (*theline)++;
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

Static Void brpiece(thefile, theline, pie)
_TEXT *thefile;
long *theline;
piece **pie;
{
  brpiekey(thefile, theline, &(*pie)->key);
  if (numbered || !skipunnum)
    brdna(thefile, theline, &(*pie)->dna);
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

Static Void brinit(book, theline)
_TEXT *book;
long *theline;
{
  if (*book->name != '\0') {
    if (book->f != NULL)
      book->f = freopen(book->name, "r", book->f);
    else
      book->f = fopen(book->name, "r");
  } else
    rewind(book->f);
  if (book->f == NULL)
    _EscIO2(FileNotFound, book->name);
  RESETBUF(book->f, Char);
  if (!BUFEOF(book->f)) {
    if (P_peek(book->f) != '*') {
      if (P_peek(book->f) != 'h')
	printf(" this is not the first line of a book:\n");
      else
	printf(" bad book:\n");
      putchar(' ');

      while (!(P_eoln(book->f) | BUFEOF(book->f))) {
	putchar(P_peek(book->f));
	getc(book->f);
      }
      putchar('\n');
      halt();
    }
  }

  else {
    printf(" book is empty\n");
    halt();
  }

  freeline = NULL;
  freedna = NULL;
  readnumber = true;
  number = 0;
  numbered = false;
  skipunnum = false;
  *theline = 1;
}

Static Void getpiece(thefile, theline, pie)
_TEXT *thefile;
long *theline;
piece **pie;
{
  Char ch;
  long SET[5];

  ch = getto(thefile, theline, P_addset(P_expset(SET, 0L), 'p'));
  if (ch != ' ') {
    brpiece(thefile, theline, pie);
 
  } else
    clearpiece(pie);
}

Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}

Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;
  FORLIM = s->length;

  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}

Static Void fillstring(s, a_)
stringDelila *s;
Char *a_;
{
  long length = fillermax;
  long index;
  clearstring(s);

  while (length > 1 && a_[length-1] == ' ')
    length--;
  if (length == 1 && a_[length-1] == ' ') {
    printf("fillstring: the string is empty\n");
    halt();
  }

  for (index = 0; index < length; index++)
    s->letters[index] = a_[index];
  s->length = length;
  s->current = 1;
}

Static Void filltrigger(t_, a_)
trigger *t_;
Char *a_;
{
  fillstring(&t_->seek, a_);
}

Static Void resettrigger(t_)
trigger *t_;
{
  t_->state = 0;
  t_->skip = false;
  t_->found = false;
}

Static Void testfortrigger(ch, t_)
Char ch;
trigger *t_;
{
  t_->state++;

  if (t_->seek.letters[t_->state - 1] == ch) {
    t_->skip = false;
    if (t_->state == t_->seek.length)
      t_->found = true;
    else
      t_->found = false;
    return;
  }
 
  if (t_->seek.letters[0] == ch) {
    t_->state = 1;
    t_->skip = false;
    t_->found = false;
    return;
  }
  t_->state = 0;
  t_->skip = true;
  t_->found = false;
}

Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) == ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) != ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}

Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}

Static Void findblank(afile)
_TEXT *afile;
{
  Char ch;

  do {
    ch = getc(afile->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != ' ');
}

Static Void findnonblank(afile, ch)
_TEXT *afile;
Char *ch;
{
  *ch = ' ';
  while (!BUFEOF(afile->f) && *ch == ' ') {
    *ch = getc(afile->f);
    if (*ch == '\n')
      *ch = ' ';
    if (P_eoln(afile->f)) {
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
    }
  }
}

#define maximumrange    10000
#define semicolon       ';'

/* Local variables for align: */
struct LOC_align {
  _TEXT *inst;
  Char ch;
  trigger endcomment, endcurly;
} ;

Local Void skipcomment(f, LINK)
_TEXT *f;
struct LOC_align *LINK;
{
  boolean comment = true;

  resettrigger(&LINK->endcomment);
  while (comment) {
    if (BUFEOF(f->f)) {
      printf("A comment does not end!\n");
      halt();
    }
    if (P_eoln(f->f)) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
      continue;
    }

    LINK->ch = getc(f->f);
    if (LINK->ch == '\n')
      LINK->ch = ' ';
    testfortrigger(LINK->ch, &LINK->endcomment);
    if (LINK->endcomment.found) {
      comment = false;
     }
  }
}

Local Void skipcurly(f, LINK)
_TEXT *f;
struct LOC_align *LINK;
{
  boolean comment = true;

  resettrigger(&LINK->endcurly);
  while (comment) {
    if (BUFEOF(f->f)) {
      printf("A comment does not end!\n");
      halt();
    }
    if (P_eoln(f->f)) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
      continue;
    }

    LINK->ch = getc(f->f);
    if (LINK->ch == '\n')
      LINK->ch = ' ';
    testfortrigger(LINK->ch, &LINK->endcurly);
    if (LINK->endcurly.found) {
      comment = false;
     }
  }
}

Local Void skipquote(quote, LINK)
trigger quote;
struct LOC_align *LINK;
{
  Char kind;
  kind = quote.seek.letters[0];

  do {
    findnonblank(LINK->inst, &LINK->ch);
  } while (!((LINK->ch == kind) | BUFEOF(LINK->inst->f)));
  if (LINK->ch != kind) {
    printf("end of quote starting with %c not found\n", kind);
    halt();
  }
}



Static Void align(inst_, book, theline, pie, length, alignedbase)
_TEXT *inst_, *book;
long *theline;
piece **pie;
long *length, *alignedbase;
{
  struct LOC_align V;
  long p, p1;
  boolean done = false;
  long thebase;
  boolean indefault = false;
  /*
*/

  trigger gettrigger, defaulttrigger, nametrigger, piecetrigger, settrigger;
  trigger begincomment, begincurly;
  trigger quote1trigger, quote2trigger;
  boolean dotteddone;
  name *WITH;

  V.inst = inst_;
  filltrigger(&defaulttrigger,
	      "default                                           ");
  filltrigger(&gettrigger,
	      "get                                               ");
  filltrigger(&nametrigger,
	      "name                                              ");
  filltrigger(&piecetrigger,
	      "piece                                             ");
  filltrigger(&settrigger,
	      "set                                               ");
  filltrigger(&begincomment,
	      "(*                                                ");
  filltrigger(&V.endcomment,
	      "*)                                                ");
  filltrigger(&begincurly,
	      "{                                                 ");
  filltrigger(&V.endcurly,
	      "}                                                 ");
  filltrigger(&quote1trigger,
	      "'                                                 ");
  filltrigger(&quote2trigger,
	      "\"                                                 ");

  resettrigger(&defaulttrigger);
  resettrigger(&gettrigger);
  resettrigger(&nametrigger);
  resettrigger(&piecetrigger);
  resettrigger(&settrigger);
  resettrigger(&begincomment);
  resettrigger(&begincurly);
  resettrigger(&quote1trigger);
  resettrigger(&quote2trigger);

  if (BUFEOF(book->f))
    return;
  getpiece(book, theline, pie);
  if (BUFEOF(book->f))
    return;
  *length = pietoint((*pie)->key.pieend, *pie);

  while (!done) {
    if (BUFEOF(V.inst->f)) {
      *alignedbase = 1;
      done = true;
      break;
    }
    if (P_eoln(V.inst->f)) {
      fscanf(V.inst->f, "%*[^\n]");
      getc(V.inst->f);
      continue;
    }

    V.ch = getc(V.inst->f);

    if (V.ch == '\n')
      V.ch = ' ';
    testfortrigger(V.ch, &begincomment);
    testfortrigger(V.ch, &begincurly);
    if (begincomment.found || begincurly.found) {
      if (V.ch == '*') {
	skipcomment(V.inst, &V);
	resettrigger(&begincomment);
      } else {
	resettrigger(&begincurly);
	skipcurly(V.inst, &V);
      }
      continue;
    }
    testfortrigger(V.ch, &gettrigger);
    if (gettrigger.found) {
      findnonblank(V.inst, &V.ch);
      findblank(V.inst);
      fscanf(V.inst->f, "%ld", &thebase);
      *alignedbase = pietoint(thebase, *pie);
      done = true;
    }

    testfortrigger(V.ch, &quote1trigger);
    if (quote1trigger.found)
      skipquote(quote1trigger, &V);

    testfortrigger(V.ch, &quote2trigger);
    if (quote2trigger.found)
      skipquote(quote2trigger, &V);

    testfortrigger(V.ch, &defaulttrigger);
    if (defaulttrigger.found) {
      indefault = true;
      resettrigger(&defaulttrigger);
    }
    if (V.ch == semicolon)
      indefault = false;

    testfortrigger(V.ch, &settrigger);
    if (settrigger.found) {
      indefault = true;
      resettrigger(&settrigger);
    }
    if (V.ch == semicolon)
      indefault = false;


    testfortrigger(V.ch, &piecetrigger);
    if (indefault)
      continue;

    if (!piecetrigger.found)
      continue;
    skipblanks(V.inst);
    WITH = &(*pie)->key.hea.keynam;
    p = 1;
    dotteddone = false;
    while (!dotteddone) {
      if (P_eoln(V.inst->f)) {
	dotteddone = true;
	break;
      }
      V.ch = getc(V.inst->f);

      if (V.ch == '\n')
	V.ch = ' ';
   
      if (V.ch == '.')
	dotteddone = true;
      if (WITH->letters[p-1] == '.')
	dotteddone = true;
   

      if (WITH->letters[p-1] != V.ch && !dotteddone && V.ch != ';') {
	printf("The piece name in the book: \n");
/* p2c: encode.p, line 1745: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
	printf("%.*s\n", WITH->length, WITH->letters);
	printf("does not match the inst file piece name:\n");

	for (p1 = 0; p1 <= p - 2; p1++)
	  putchar(WITH->letters[p1]);

	putchar(V.ch);

	done = P_eoln(V.inst->f);
	while (!done) {
	  done = P_eoln(V.inst->f);
	  if (done)
	    break;
	  V.ch = getc(V.inst->f);
	  if (V.ch == '\n')
	    V.ch = ' ';
	  if (V.ch == ' ' || V.ch == ';')
	    done = true;
	  if (!done)
	    putchar(V.ch);
	}
	putchar('\n');
	/*
*/
	for (p1 = 1; p1 < p; p1++)
	  putchar(' ');
	printf("^\n");
	halt();
      }
      p++;
      if (p > WITH->length) {
	dotteddone = true;

      }
    }
  }


  if (*alignedbase > -maximumrange && *alignedbase <= *length + maximumrange)
    return;
  printf(" In procedure align:\n");
  printf(" read in base was %ld\n", thebase);
  printf(" in internal coordinates: %ld\n", *alignedbase);
  printf(" maximum range was %ld\n", (long)maximumrange);
  printf(" piece length was %ld\n", *length);
  WITH = &(*pie)->key.hea.keynam;
/* p2c: encode.p, line 1792: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  printf(" piece name: %.*s\n", WITH->length, WITH->letters);
  printf(" piece number: %ld\n", number);
  printf(" aligned base is too far away... see the code\n");
  halt();
}

#undef maximumrange
#undef semicolon



Static base getbase(position, pie)
long position;
piece *pie;
{
  dnastring *workdna;
  long p, spot, thelength;
  thelength = piecelength(pie);
 
  while (position < 1)
    position += thelength;
  while (position > thelength)
    position -= thelength;

  workdna = pie->dna;
  p = workdna->length;
  while (position > p) {
   
    workdna = workdna->next;
    if (workdna == NULL) {
      printf("error in function getbase!\n");
      halt();
    }
    p += workdna->length;
  }
  
  if (true) {
    spot = workdna->length - p + position;
  
    if (spot <= 0) {
      printf("error in getbase, spot (= %ld) must be positive\n", spot);
      halt();
    }
    if (spot > workdna->length) {
      printf("error in getbase, spot (=%ld) must be less than length (=%d)\n",
	     spot, workdna->length);
      halt();
    }
 
    return ((base)P_getbits_UB(workdna->part, spot - 1, 1, 3));
  }
  printf("error in getbase: request off end of piece\n");
  halt();
}


Static Void initialize()
{
  long i;

  printf(" encode, version %4.2f\n", version);
  if (*inst.name != '\0') {
    if (inst.f != NULL)
      inst.f = freopen(inst.name, "r", inst.f);
    else
      inst.f = fopen(inst.name, "r");
  } else
    rewind(inst.f);
  if (inst.f == NULL)
    _EscIO2(FileNotFound, inst.name);
  RESETBUF(inst.f, Char);
  if (BUFEOF(inst.f))
    noinst = true;
  else
    noinst = false;
  brinit(&book, &theline);
  if (*encseq.name != '\0') {
    if (encseq.f != NULL)
      encseq.f = freopen(encseq.name, "w", encseq.f);
    else
      encseq.f = fopen(encseq.name, "w");
  } else {
    if (encseq.f != NULL)
      rewind(encseq.f);
    else
      encseq.f = tmpfile();
  }
  if (encseq.f == NULL)
    _EscIO2(FileNotFound, encseq.name);
  SETUPBUF(encseq.f, Char);
  firstparam = (parameter *)Malloc(sizeof(parameter));
  apiece = (piece *)Malloc(sizeof(piece));
  fourpowers[0] = 1;
  for (i = 1; i <= codingmax; i++)
    fourpowers[i] = fourpowers[i-1] * 4;
}


Static Void setparam()
{
  parameter *newparam, *param;
  spacelist *firstspaces, *newspaces;
  long i;
  Char ch;
  long FORLIM;

  if (*encodep.name != '\0') {
    if (encodep.f != NULL)
      encodep.f = freopen(encodep.name, "r", encodep.f);
    else
      encodep.f = fopen(encodep.name, "r");
  } else
    rewind(encodep.f);
  if (encodep.f == NULL)
    _EscIO2(FileNotFound, encodep.name);
  RESETBUF(encodep.f, Char);
  if (BUFEOF(encodep.f)) {
    printf(" error: empty parameter file (encodep)\n");
    halt();
    return;
  }

  fscanf(encodep.f, "%c%*[^\n]", &alignmenttype);
  getc(encodep.f);
  if (alignmenttype == '\n')
    alignmenttype = ' ';
  if (alignmenttype != 'b' && alignmenttype != 'i' && alignmenttype != 'f') {
    printf("alignment type inst must be f, b, or i\n");
    halt();
  }

  param = firstparam;
  regions = 0;
  do {
    regions++;
    fscanf(encodep.f, "%ld%ld%*[^\n]", param->range,
	   &param->range[(long)stop]);
    getc(encodep.f);
    if (param->range[(long)start] > param->range[(long)stop]) {
      printf(
	" error in parameter file: end of range cannot be less than beginning of range\n");
      halt();
    }
    fscanf(encodep.f, "%ld%*[^\n]", &param->window);
    getc(encodep.f);
    if (param->window <= 0) {
      printf(" error in parameter file: window size must be positive\n");
      halt();
    }
    fscanf(encodep.f, "%ld%*[^\n]", &param->wshift);
    getc(encodep.f);
    if (param->wshift <= 0) {
      printf(" error in parameter file: window shift must be positive\n");
      halt();
    }
    fscanf(encodep.f, "%ld", &param->coding);
    if (param->coding <= 0) {
      printf(" error in parameter file: coding must be positive\n");
      halt();
    }
    if (param->coding > codingmax) {
      printf(" error in parameter file: coding can not be greater than %ld\n",
	     (long)codingmax);
      halt();
    }
    if (param->coding > codingmax) {
      printf(" error in coding file: requested coding level too large\n");
      halt();
    }
    param->spaces = (spacelist *)Malloc(sizeof(spacelist));
    ch = ' ';
    while (!P_eoln(encodep.f) && ch == ' ') {
      ch = getc(encodep.f);
      if (ch == '\n')
	ch = ' ';
    }
    if (ch == ':') {
      firstspaces = (spacelist *)Malloc(sizeof(spacelist));
      param->spaces = firstspaces;
      fscanf(encodep.f, "%ld", &param->spaces->skips);
      FORLIM = param->coding;
      for (i = 2; i < FORLIM; i++) {
	newspaces = (spacelist *)Malloc(sizeof(spacelist));
	param->spaces->next = newspaces;
	param->spaces = newspaces;
	fscanf(encodep.f, "%ld", &param->spaces->skips);
      }
      param->spaces->next = NULL;
      param->spaces = firstspaces;
    } else
      param->spaces = NULL;
    fscanf(encodep.f, "%*[^\n]");
    getc(encodep.f);
    fscanf(encodep.f, "%ld%*[^\n]", &param->cshift);
    getc(encodep.f);
    if (param->cshift <= 0) {
      printf(" error in parameter file: coding shift must be positive\n");
      halt();
    }

    param->wvlength = (long)floor(exp(param->coding * log(4.0)) + 0.5);

    param->pvlength = param->wvlength *
	(long)((double)(param->range[(long)stop] - param->range[(long)start]) /
	       param->wshift + 1);
    if (BUFEOF(encodep.f))
      param->next = NULL;
    else {
      newparam = (parameter *)Malloc(sizeof(parameter));
      param->next = newparam;
      param = newparam;
    }
  } while (!BUFEOF(encodep.f));
}

Static Void encheader()
{
  parameter *param;
  spacelist *aspace;
  long i;
  parameter *WITH;
  long FORLIM;

  fprintf(encseq.f, " encode %4.2f; encoding of sequences in\n", version);
  putc(' ', encseq.f);
  copyaline(&book, &encseq);
  fprintf(encseq.f, "\n %ld independently coded regions\n\n", regions);

  param = firstparam;
  while (param != NULL) {
    WITH = param;
    fprintf(encseq.f, " %ld to %ld",
	    WITH->range[(long)start], WITH->range[(long)stop]);
    fprintf(encseq.f, " is encoded as:\n");
    fprintf(encseq.f, "%6c%ld long windows\n", ' ', WITH->window);
    fprintf(encseq.f, "%6c%ld bases shift to new window\n", ' ', WITH->wshift);
    fprintf(encseq.f, "%6c%ld", ' ', WITH->coding);
    if (WITH->coding > 1) {
      fprintf(encseq.f, " :");
      if (WITH->spaces == NULL) {
	FORLIM = WITH->coding;
	for (i = 1; i < FORLIM; i++)
	  fprintf(encseq.f, " 0");
      } else {
	aspace = WITH->spaces;
	FORLIM = WITH->coding;
	for (i = 1; i < FORLIM; i++) {
	  fprintf(encseq.f, " %ld", aspace->skips);
	  aspace = aspace->next;
	}
      }
    }
    fprintf(encseq.f, " are the coding units\n");
    fprintf(encseq.f, "%6c%ld bases shift to new coding site\n",
	    ' ', WITH->cshift);
    param = param->next;
    putc('\n', encseq.f);
  }
  fprintf(encseq.f, " %ld is the vector length\n\n", vectorsize(firstparam));
}

/* this procedure takes one sequence (which has been aligned) and encodes
   it into a string of integers according to the parameters the user has
   specified.  each window of the sequence causes a vector to be printed,
   the size of which is determined by the coding level specified.  for
   example, if the coding level is 1 (monos are being counted) then the
   vector has four elements.  if the coding level is 2 (dis are being
   counted) then the vector has 16 elements.  the vectors for each window
   are concatenated to give the vector for each sequence, which is ended
   with the 'end of sequence' symbol.
   the procedure takes advantage of the fact that the type 'base' is an
   ordered set of the nucleotides, with a,c,g,t being assigned internally
   the values 0,1,2,3.  the total number of elements of a vector is
   4**coding-level, and these correspond to the number of oligos of each
   type from all a's to all t's.  this makes it easy to increase the
   vector element for the oligo which exists at the coding site.  for
   example, there are 64 tri-nucleotides and the vector which encodes
   them has elements 0 (for the oligo aaa) through 63 (for the oligo ttt).
   the number of the oligo cgt would be stored in element 27, as seen from
       16 * 1   (16 for 4**(coding-level - 1) and 1 for the c)
     +  4 * 2   ( 4 for 4**(coding-level - 2) and 2 for the g)
     +  1 * 3   ( 1 for 4**(coding-level - 3) and 3 for the t)
     ---------
         27
   another example is finding the position of the oligo taac in the vector
   of all the 256 tetramers, from position 0 to 255.
       64 * 3   (64 = 4**(coding-level - 1) and 3 for the t)
    +  16 * 0   (16 = 4**(coding-level - 2) and 0 for the a)
    +   4 * 0   ( 4 = 4**(coding-level - 3) and 0 for the a)
    +   1 * 1   ( 1 = 4**(coding-level - 4) and 1 for the c)
    ---------
        193    ( is the vector element corresponding to the oligo taac )
*/
Static Void codeit(apiece, alignedbase, v)
piece *apiece;
long alignedbase;
vector *v;
{
  parameter *param;
  spacelist *aspace;
  long startsite, firstpos, pos, sitesize;
  long element = 1;
  long welement, length, i, FORLIM;
  length = piecelength(apiece);
  param = firstparam;
  FORLIM = v->length;

  for (i = 1; i <= FORLIM; i++)
    vput(v, i, 0.0);

  do {

    aspace = param->spaces;
    sitesize = param->coding;
    while (aspace != NULL) {
      sitesize += aspace->skips;
      aspace = aspace->next;
    }

    startsite = alignedbase + param->range[(long)start];
    do {

      firstpos = startsite;
      do {

	if (firstpos > 0) {
	  if (firstpos + sitesize - 1 <= length ||
	      apiece->key.piecon == circular && firstpos <= length) {
	    pos = firstpos;
	    welement = 0;
	    aspace = param->spaces;
	    for (i = param->coding - 1; i >= 0; i--) {
	      welement += fourpowers[i] * (int)getbase(pos, apiece);
	      if (aspace != NULL) {
		pos += aspace->skips + 1;
		aspace = aspace->next;
	      } else
		pos++;

	      if (pos > length)
		pos -= length;
	    }

	    vput(v, element + welement, vget(*v, element + welement) + 1);

	  }
	}
	firstpos += param->cshift;
      } while (firstpos < startsite + param->window);

      startsite += param->wshift;

      element += param->wvlength;
    } while (startsite <= alignedbase + param->range[(long)stop]);
    param = param->next;
  } while (param != NULL);
}

/* Print help for user */
void usage() {
  printf("\n");
  printf(" encode: create a book of sequences into strings of integers\n\n");
  printf("\n  encode -b book -i instructions -p encodep\n\n");
  printf(" parameters: \n");
  printf("   -b book generated by delila using instructions\n");
  printf("   -i revised instruction set, output from alist \n");
  printf("   -p encodep parameters to control the program.\n\n");
  printf(" Outputs:\n\n");
  printf("   encseq: the encoded sequences \n");
  printf("   output: messages to user\n");
  printf("\n");
  printf("  version %4.2f\n", version);
  exit(EXIT_SUCCESS);
}


int main(int argc, Char **argv)
{
  line *WITH;
  long FORLIM;
  extern char *optarg;
	extern int optind;
	int c, err = 0; 
  /* flags marking arguments passed */
  int bflag=0;       /* book output file name  */
	int iflag=0;       /* instruction file flag */
  int pflag=0;       /* encodep file */
	char *bookName     = "book.txt";
  char *instructions = "instructions.txt";
  char *parameters   = "encodep.txt";

/* Process command line arguments  */
while ((c = getopt(argc, argv, "b:i:p:")) != -1)
		switch (c) {
		case 'b':
      bflag = 1;
			bookName = optarg;
      printf("bflag %s\n", bookName);
      break;
		case 'i':
      iflag = 1;
			instructions = optarg;
			printf("instructions %s\n", instructions);
      break;
    case 'p':
      pflag = 1;
      parameters  = optarg;
      printf("parameters %s\n", parameters);
      break;
    case '?':
			err = 1;
			break;
		}

  /* Is the book file name present */  
	if (bflag == 0) {	/* -b bookname was mandatory */ 
		fprintf(stderr, "%s: missing -b bookname\n", argv[0]);
		usage();
		exit(1);
	} 

  /* Instruction file ? */
  if (iflag == 0) { /* -i was mandatory */        
		fprintf(stderr, "%s: missing -i instruction file\n", argv[0]);
		usage();
		exit(1);
  } 

  /* parameters file  */  
  if (pflag == 0) { 
    fprintf(stderr, "%s: missing -p encodep file \n", argv[0]);
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
  encodep.f = NULL;
  strcpy(encodep.name, parameters);
  encseq.f = NULL;
  strcpy(encseq.name, "encseq");
  book.f = NULL;
  strcpy(book.name, bookName);
  inst.f = NULL;
  strcpy(inst.name, instructions);
  initialize();
  setparam();
  encheader();
  seqvector.length = vectorsize(firstparam);
  makevector(&seqvector, seqvector.length);
  if (noinst) {
    if (alignmenttype == 'i') {
      alignmenttype = 'b';
      printf("There are no instructions so alignment type is forced to b mode\n");
    }
  }

  while (!BUFEOF(book.f)) {
    if (alignmenttype != 'i') {
      getpiece(&book, &theline, &apiece);
      if (!BUFEOF(book.f)) {
	length_ = piecelength(apiece);
	switch (alignmenttype) {

	case 'i':
	  printf("prgm error\n");
	  halt();
	  break;

	case 'b':
	  alignedbase = pietoint(0L, apiece);
	  break;

	case 'f':
	  alignedbase = 1;
	  break;
	}
      }
    } else
      align(&inst, &book, &theline, &apiece, &length_, &alignedbase);
    if (BUFEOF(book.f))
      break;
    codeit(apiece, alignedbase, &seqvector);
    writevector(&encseq, seqvector, 3L, 0L);
    clearpiece(&apiece);
  }
_L1:
  if (inst.f != NULL)
    fclose(inst.f);
  if (book.f != NULL)
    fclose(book.f);
  if (encseq.f != NULL)
    fclose(encseq.f);
  if (encodep.f != NULL)
    fclose(encodep.f);
  exit(EXIT_SUCCESS);
  
  return 0;
}

/* End. */
