/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "alist.p" */
/*





To Compile:
gcc  alist.c -o alist  -I/home/mplace/bin/p2c/src -L /home/mplace/bin/p2c/src -lm -lp2c
*/




#include <getopt.h>  /* getopt API */ 
#include <stdio.h>   /* printf */
#include <stdlib.h> 
#include </home/mplace/bin/p2c/src/p2c.h>

#define version         6.64
#define updateversion   6.20
#define pagelength      59
#define headerlines     10
#define headerclines    5
#define maxnumberwidth  7
#define columnmax       20
#define defaultllx      7.10999
#define defaultlly      7.01995
#define defaulturx      588.15
#define defaultury      784.98
#define deffontsize     15
#define defcolorlistcontrol  'C'
#define defcharacterratio  0.60
#define topofpage       27.0
#define defdeltaXcm     1.0
#define defdeltaYcm     (-1.0)
#define defscaleimage   1.0
#define dnamax          300
#define namelength      100
#define linelength      80
#define pwid            8
#define pdec            5
#define pdecolor        4
#define maxstring       2000
#define fillermax       50

typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;

/*
*/
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
  char length;
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

/* p2c: alist.p, line 561:
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

Static _TEXT inst, book, alistp, namebook, namelist, avalues, list, clist, colors;
Static piece *apiece;
Static long length_, alignedbase, fromparam, toparam;
Static long fromdo, todo;
Static boolean userrange;
Static Char displaylevel;
/*
*/
Static Char shownumbers;
/*
*/

Static Char usefullname;
/*
*/
Static long fullnamewidth;
Static Char paging;

Static long index_, indexfill;

Static long cpagelength;
/*

*/

Static long linenumber, clinenumber;
/*


*/

Static long headerclinenumber;

Static long pagenumber, cpagenumber;


Static orgkey org;
Static boolean orgchange, orgopen;
Static chrkey chr;
Static boolean chrchange, chropen;
Static piece *pie;
Static boolean piechange, pieopen;

Static Char nametype;
/*
*/
Static long namewidth, namelistwidth;
Static boolean namelistuse;
/*
*/
Static long numberwidth, positionwidth, sequences;

Static Char b;
Static long column;
/*
*/

Static Char alignmenttype;
/*

*/

Static Char cnamelist;
Static name programname;
Static Char thedirection;

Static boolean readvalues;
Static Char columnname[columnmax];
Static long columnlength, columntoread, columnwid, columndec;
Static double columnvalue;

Static long theline;
Static double parameterversion;


Static double llx, lly, urx, ury;
Static Char edgecontrol;
Static double edgeleft, edgeright, edgelow, edgehigh;
Static long blanks, displaywidth;

Static double cmfactor;

/*

*/
Static long fontsize;

Static Char colorlistcontrol;
Static boolean mapcontrol;
Static double deltaXcm, deltaYcm, scaleimage;
/*
*/

Static Char headercontrol;






Static line *freeline;
Static dnastring *freedna;

Static boolean readnumber;
/*
*/
Static long number;
Static boolean numbered, skipunnum;


/*
*/





Static Void crash()
{
  /*

*/
  _TEXT bogus;

  bogus.f = NULL;
  *bogus.name = '\0';
  printf(" program crash.\n");
  if (*bogus.name != '\0')
    bogus.f = fopen(bogus.name, "r");
  else
    rewind(bogus.f);
  if (bogus.f == NULL)
    _EscIO2(FileNotFound, bogus.name);
  RESETBUF(bogus.f, Char);
  fclose(bogus.f);
}


Static jmp_buf _JL1;



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



Static Void copynoreturn(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");

  getc(fin->f);
}



Static Void copytocomma(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  boolean done = false;

  while (!done) {
    if (P_eoln(fin->f)) {
      done = true;
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
    } else {
      if (P_peek(fin->f) == ',')
	done = true;
      putc(P_peek(fin->f), fout->f);
      getc(fin->f);
    }
  }
}



Static Void splitbooktitle(book, list)
_TEXT *book, *list;
{
  /*


*/
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
  copytocomma(book, list);
  copytocomma(book, list);
  fprintf(list->f, "\n ");
  copyaline(book, list);
}












Static Void emptystring(ribbon)
stringDelila *ribbon;
{
  /*

*/
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}


Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  emptystring(ribbon);
  ribbon->next = NULL;
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*



*/
  printf("remove initializestring routine!\n");
  printf("replace it with clearstring routine!\n");
  halt();
  clearstring(ribbon);
  ribbon->next = NULL;
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
  /*
*/


  /*


*/
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



/*




*/

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
  /*









*/
  t_->state++;
  /*



*/
  if (t_->seek.letters[t_->state - 1] == ch) {
    t_->skip = false;
    if (t_->state == t_->seek.length)
      t_->found = true;
    else
      t_->found = false;
    return;
  }
  /*
*/
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


#define tab             9






Static boolean isblankDelila(c_)
Char c_;
{
  return (c_ == ' ' || c_ == tab);
}

#undef tab


Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while (isblankDelila(P_peek(thefile->f)) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((!isblankDelila(P_peek(thefile->f))) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
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


/*
*/

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
  /*
*/
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
    /*




*/
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
  /*


*/
  /*
*/
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
  /*




*/
  /*
*/
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
  /*





*/
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
    /*







*/
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
  /*

*/
  *note = NULL;
  numbered = false;
  number = 0;
  /*
*/

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
  /*
*/
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

  /*


*/
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
  /*


*/
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
  /*
*/
  /*
*/
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
    /*


*/
    /*



*/
  } else
    clearpiece(pie);
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
/*


*/

#define semicolon       ';'


/* Local variables for align: */
struct LOC_align {
  _TEXT *inst;
  Char ch;
  trigger endcomment, endcurly;
} ;

/*
*/

/*
















*/

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
      /*

*/
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
      /*

*/
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
  /*




*/
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
    /*

*/
    /*

*/
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
      /*

*/
      if (V.ch == '.')
	dotteddone = true;
      if (WITH->letters[p-1] == '.')
	dotteddone = true;
      /*


*/

      if (WITH->letters[p-1] != V.ch && !dotteddone && V.ch != ';') {
	printf("The piece name in the book: \n");
/* p2c: alist.p, line 1979: Note:
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
/* p2c: alist.p, line 2026: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  printf(" piece name: %.*s\n", WITH->length, WITH->letters);
  printf(" piece number: %ld\n", number);
  printf(" aligned base is too far away... see the code\n");
  halt();
}

#undef maximumrange
#undef semicolon


#define maximumrange    10000






Static Void maxminalignment(inst, book, theline, fromparam, toparam,
			    alignmenttype)
_TEXT *inst, *book;
long *theline, *fromparam, *toparam;
Char alignmenttype;
{
  /*





*/
  /*
*/
  long distance;
  piece *pie;
  long length, alignedbase;

  pie = (piece *)Malloc(sizeof(piece));

  *fromparam = LONG_MAX;
  *toparam = -LONG_MAX;

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
  if (*inst->name != '\0') {
    if (inst->f != NULL)
      inst->f = freopen(inst->name, "r", inst->f);
    else
      inst->f = fopen(inst->name, "r");
  } else
    rewind(inst->f);
  if (inst->f == NULL)
    _EscIO2(FileNotFound, inst->name);
  RESETBUF(inst->f, Char);
  while (!BUFEOF(book->f)) {
    switch (alignmenttype) {

    case 'i':
      align(inst, book, theline, &pie, &length, &alignedbase);
      break;

    case 'b':
    case 'f':
      getpiece(book, theline, &pie);
      length = piecelength(pie);
      break;
    }

    if (BUFEOF(book->f))
      break;
    switch (alignmenttype) {

    case 'f':
      alignedbase = 0;
      *fromparam = 1;
      distance = length - alignedbase;
      if (*toparam < distance)
	*toparam = distance;
      break;

    case 'i':
      distance = 1 - alignedbase;
      if (*fromparam > distance)
	*fromparam = distance;

      distance = length - alignedbase;
      if (*toparam < distance)
	*toparam = distance;
      break;

    case 'b':
      alignedbase = pietoint(0L, pie);
      distance = 1 - alignedbase;
      if (*fromparam > distance)
	*fromparam = distance;

      distance = length - alignedbase;
      if (*toparam < distance)
	*toparam = distance;
      break;
    }

    clearpiece(&pie);
  }

  if (*toparam - *fromparam > maximumrange) {
    printf(" in procedure maxminalignment:\n");
    printf(" alignedbase = %ld\n", alignedbase);
    printf(" fromparameter = %ld\n", *fromparam);
    printf(" toparameter = %ld\n", *toparam);
    printf(" this exceeds the maximum range allowed (%ld)\n",
	   (long)maximumrange);
    printf(" see notes in the procedure. \n");
    /*



*/
    halt();
  }


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
  if (*inst->name != '\0') {
    if (inst->f != NULL)
      inst->f = freopen(inst->name, "r", inst->f);
    else
      inst->f = fopen(inst->name, "r");
  } else
    rewind(inst->f);
  if (inst->f == NULL)
    _EscIO2(FileNotFound, inst->name);
  RESETBUF(inst->f, Char);
  Free(pie);
}

#undef maximumrange



Static boolean withinalignment(alignedposition, alignedbase, length)
long alignedposition, alignedbase, length;
{
  /*
*/
  long p;

  p = alignedposition + alignedbase;
  return (p > 0 && p <= length);
}



Static base getbase(position, pie)
long position;
piece *pie;
{
  /*





*/
  dnastring *workdna;
  long p, spot, thelength;

  /*

*/

  thelength = piecelength(pie);
  while (position < 1)
    position += thelength;
  while (position > thelength)
    position -= thelength;

  workdna = pie->dna;
  p = workdna->length;
  while (position > p) {
    /*

*/
    workdna = workdna->next;
    if (workdna == NULL) {
      printf("error in function getbase!\n");
      halt();
    }
    p += workdna->length;
  }
  /*

*/
  if (true) {
    spot = workdna->length - p + position;
    /*


*/
    if (spot <= 0) {
      printf("error in getbase, spot (= %ld) must be positive\n", spot);
      halt();
    }
    if (spot > workdna->length) {
      printf("error in getbase, spot (=%ld) must be less than length (=%d)\n",
	     spot, workdna->length);
      halt();
    }
    /*

*/
    return ((base)P_getbits_UB(workdna->part, spot - 1, 1, 3));
  }
  printf("error in getbase: request off end of piece\n");
  halt();
}


/* Local variables for numberdigit: */
struct LOC_numberdigit {
  long number, place, myabsolute;
  Char acharacter;
} ;

Local Void digit(LINK)
struct LOC_numberdigit *LINK;
{
  long tenplace, z, d;

  tenplace = LINK->place * 10;
  z = LINK->myabsolute - LINK->myabsolute / tenplace * tenplace;
  if (LINK->place == 1)
    d = z;
  else
    d = z / LINK->place;
  switch (d) {

  case 0:
    LINK->acharacter = '0';
    break;

  case 1:
    LINK->acharacter = '1';
    break;

  case 2:
    LINK->acharacter = '2';
    break;

  case 3:
    LINK->acharacter = '3';
    break;

  case 4:
    LINK->acharacter = '4';
    break;

  case 5:
    LINK->acharacter = '5';
    break;

  case 6:
    LINK->acharacter = '6';
    break;

  case 7:
    LINK->acharacter = '7';
    break;

  case 8:
    LINK->acharacter = '8';
    break;

  case 9:
    LINK->acharacter = '9';
    break;
  }
}

Local Void sign(LINK)
struct LOC_numberdigit *LINK;
{
  if (LINK->number < 0)
    LINK->acharacter = '-';
  else
    LINK->acharacter = '+';
}





Static Char numberdigit(number_, logplace)
long number_, logplace;
{
  /*






*/
  struct LOC_numberdigit V;
  long count;

  V.number = number_;
  V.place = 1;
  for (count = 1; count <= logplace; count++)
    V.place *= 10;

  if (V.number == 0) {
    if (V.place == 1)
      V.acharacter = '0';
    else
      V.acharacter = ' ';
    return V.acharacter;
  }
  V.myabsolute = labs(V.number);
  if (V.myabsolute < V.place / 10) {
    V.acharacter = ' ';
    return V.acharacter;
  }
  if (V.myabsolute >= V.place)
    digit(&V);
  else
    sign(&V);
  return V.acharacter;
}


#define ln10            2.30259
#define epsilon         0.00001



Static long numbersize(n)
long n;
{
  long size;

  if (n == 0)
    return 1;
  else {
    size = (long)(log((double)labs(n)) / ln10 + epsilon) + 1;

    /*


*/
    if (n < 0)
      size++;
    return size;
  }
}

#undef ln10
#undef epsilon



Static long firstlastmax(firstnumber, lastnumber)
long firstnumber, lastnumber;
{
  /*
*/
  long firstlines, lastlines;

  firstlines = numbersize(firstnumber);
  if (firstnumber > 0)
    firstlines++;
  lastlines = numbersize(lastnumber);
  if (lastnumber > 0)
    lastlines++;
  if (firstlines > lastlines)
    return firstlines;
  else
    return lastlines;
}


Static Void numberbar(afile, spaces, firstnumber, lastnumber, linesused)
_TEXT *afile;
long spaces, firstnumber, lastnumber, *linesused;
{
  /*
*/
  long logplace, number, spacecount;

  /*






*/
  *linesused = firstlastmax(firstnumber, lastnumber);
  /*






*/
  for (logplace = *linesused - 1; logplace >= 0; logplace--) {
    for (spacecount = 1; spacecount <= spaces; spacecount++)
      putc(' ', afile->f);
    for (number = firstnumber; number <= lastnumber; number++)
      fputc(numberdigit(number, logplace), afile->f);
    putc('\n', afile->f);
  }
}





Static Void getpositions(inst, book, positionwidth, numberwidth, count,
			 alignmenttype)
_TEXT *inst, *book;
long *positionwidth, *numberwidth, *count;
Char alignmenttype;
{
  /*





*/
  long abmin = 0, abmax = 0;
  long coordinate;
  long maxnumber = 0;
  piece *pie;
  long length, alignedbase;

  pie = (piece *)Malloc(sizeof(piece));
  brinit(book, &theline);
  if (*inst->name != '\0') {
    if (inst->f != NULL)
      inst->f = freopen(inst->name, "r", inst->f);
    else
      inst->f = fopen(inst->name, "r");
  } else
    rewind(inst->f);
  if (inst->f == NULL) {


    /*



*/

    _EscIO2(FileNotFound, inst->name);
  }
  RESETBUF(inst->f, Char);

  *count = 0;
/* p2c: alist.p: Note: Eliminated unused assignment statement [338] */
  while (!BUFEOF(book->f)) {
    switch (alignmenttype) {

    case 'i':
      align(inst, book, &theline, &pie, &length, &alignedbase);
      break;

    case 'b':
    case 'f':
      getpiece(book, &theline, &pie);
      break;
    }

    if (BUFEOF(book->f))
      break;
    (*count)++;
    if (labs(number) > maxnumber)
      maxnumber = labs(number);

/* p2c: alist.p: Note: Eliminated unused assignment statement [338] */
/* p2c: alist.p: Note: Eliminated unused assignment statement [338] */
    switch (alignmenttype) {

    case 'f':
      coordinate = inttopie(1L, pie);
      break;

    case 'b':
    case 'i':
      coordinate = inttopie(alignedbase, pie);
      break;
    }
    if (labs(coordinate) > labs(abmax))
      abmax = coordinate;
    if (coordinate < abmin)
      abmin = coordinate;

    clearpiece(&pie);
  }


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
  if (*inst->name != '\0') {
    if (inst->f != NULL)
      inst->f = freopen(inst->name, "r", inst->f);
    else
      inst->f = fopen(inst->name, "r");
  } else
    rewind(inst->f);
  if (inst->f == NULL)
    _EscIO2(FileNotFound, inst->name);
  RESETBUF(inst->f, Char);
  Free(pie);

  /*


*/
  *numberwidth = numbersize(maxnumber);

  /*


*/
  if (abmin == 0 && abmax == 0) {
    *positionwidth = 1;
    return;
  }
  if (numbersize(abmax) > numbersize(abmin))
    *positionwidth = numbersize(abmax);
  else
    *positionwidth = numbersize(abmin);
}


Static Void noheader(a_)
_TEXT *a_;
{
  fprintf(a_->f, " %% NOHEADER FOR PACKAGING INTO ANOTHER FIGURE\n");
}


Static Void removeit(a_)
_TEXT *a_;
{
  fprintf(a_->f, " %% REMOVE FOR PACKAGING INTO ANOTHER FIGURE\n");
}


Static Void pnumberbar(afile, spaces, firstnumber, lastnumber, linesused)
_TEXT *afile;
long spaces, firstnumber, lastnumber, *linesused;
{
  /*




*/
  _TEXT internal;

  internal.f = NULL;
  *internal.name = '\0';
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  numberbar(&internal, spaces, firstnumber, lastnumber, linesused);
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  while (!BUFEOF(internal.f)) {
    putc('(', afile->f);
    while (!P_eoln(internal.f)) {
      putc(P_peek(internal.f), afile->f);
      getc(internal.f);
    }
    fscanf(internal.f, "%*[^\n]");
    getc(internal.f);
    fprintf(afile->f, ") sn\n");
  }
  if (internal.f != NULL)
    fclose(internal.f);
}



Static Void clearname(n)
name *n;
{
  long i;

  n->length = 0;
  for (i = 0; i < namelength; i++)
    n->letters[i] = ' ';
}


Static Void writename(f, n)
_TEXT *f;
name n;
{
  long i;

  for (i = 0; i < n.length; i++)
    putc(n.letters[i], f->f);
}


Static Void copyname(a_, b)
name a_, *b;
{
  long i;

  for (i = 0; i < a_.length; i++)
    b->letters[i] = a_.letters[i];
  b->length = a_.length;
}


Static boolean equalname(a_, b)
name a_, b;
{
  long i = 1;
  boolean same = true;

  if (b.length != a_.length)
    return false;
  while (same && i <= a_.length) {
    same = (b.letters[i-1] == a_.letters[i-1]);
    i++;
/* p2c: alist.p: Note: Eliminated unused assignment statement [338] */
  }
  return same;
}



Static Void brorgkey(thefile, theline, org)
_TEXT *thefile;
long *theline;
orgkey *org;
{
  brheader(thefile, theline, &org->hea);
  getlineDelila(&org->mapunit);
  brline(thefile, theline, &org->mapunit);
}



Static Void brchrkey(thefile, theline, chr)
_TEXT *thefile;
long *theline;
chrkey *chr;
{
  brheader(thefile, theline, &chr->hea);
  brreanum(thefile, theline, &chr->mapbeg);
  brreanum(thefile, theline, &chr->mapend);
}



Static Void getocp(thefile, theline, org, orgchange, orgopen, chr, chrchange,
		   chropen, pie, piechange, pieopen)
_TEXT *thefile;
long *theline;
orgkey *org;
boolean *orgchange, *orgopen;
chrkey *chr;
boolean *chrchange, *chropen;
piece **pie;
boolean *piechange, *pieopen;
{
  /*




























*/
  Char ch = 'a';
  chrkey newchr;
  orgkey neworg;
  piece *newpie;
  long SET[5];

  while (ch != 'p' && ch != ' ') {
    P_addset(P_expset(SET, 0L), 'o');
    P_addset(SET, 'c');
    ch = getto(thefile, theline, P_addset(SET, 'p'));
    if (ch == ' ') {
      *pieopen = false;
      break;
    }
    switch (ch) {

    case 'o':
      if (*orgopen) {
	fscanf(thefile->f, "%*[^\n]");
	getc(thefile->f);
	/*
*/
	*orgopen = false;
      } else {
	brorgkey(thefile, theline, &neworg);
	if (strncmp(neworg.hea.keynam.letters, org->hea.keynam.letters,
		    sizeof(alpha)) &&
	    neworg.hea.keynam.length != org->hea.keynam.length) {
	  *orgchange = true;
	  copyheader(neworg.hea, &org->hea);

	  org->mapunit = neworg.mapunit;
	  clearline(&neworg.mapunit);

	}
	/*





*/

	else
	  *orgchange = false;
	*orgopen = true;
      }
      break;

    case 'c':
      if (*chropen) {
	fscanf(thefile->f, "%*[^\n]");
	getc(thefile->f);
	/*
*/
	*chropen = false;
      } else {
	brchrkey(thefile, theline, &newchr);
	if (strncmp(newchr.hea.keynam.letters, chr->hea.keynam.letters,
		    sizeof(alpha)) &&
	    newchr.hea.keynam.length != chr->hea.keynam.length) {
	  /*





*/
	  *chrchange = true;
	  copyheader(newchr.hea, &chr->hea);

	  chr->mapbeg = newchr.mapbeg;
	  chr->mapend = newchr.mapend;
	} else
	  *chrchange = false;
	*chropen = true;
      }
      break;

    case 'p':
      if (*pieopen) {
	*pieopen = false;
	ch = 'a';
      } else {
	newpie = (piece *)Malloc(sizeof(piece));
	brpiece(thefile, theline, &newpie);
	if (*pie == NULL)
	  *piechange = true;
	else {
	  if (strncmp(newpie->key.hea.keynam.letters,
		      (*pie)->key.hea.keynam.letters, sizeof(alpha)) &&
	      newpie->key.hea.keynam.length != (*pie)->key.hea.keynam.length)
	    *piechange = true;
	  else
	    *piechange = false;
	}
	*pieopen = true;
	/*




*/
	if (*pie != NULL) {
	  clearpiece(pie);
	  Free(*pie);
	}
	*pie = newpie;
      }
      break;
    }
  }
}


#define debugging       false

#define boundary        2




Static boolean emptyfile(afile)
_TEXT *afile;
{
  /*
















*/
  boolean Result;
  /*
*/
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



Static Void getname(namebook, theline, nametype, typefound, aname, org,
		    orgchange, orgopen, chr, chrchange, chropen, pie,
		    piechange, pieopen)
_TEXT *namebook;
long theline;
Char nametype, *typefound;
name *aname;
orgkey *org;
boolean *orgchange, *orgopen;
chrkey *chr;
boolean *chrchange, *chropen;
piece **pie;
boolean *piechange, *pieopen;
{
  /*


*/
  /*

*/
  /*

*/

  if (BUFEOF(namebook->f)) {
    *typefound = ' ';
    /*





*/
    return;
  }
  *typefound = '.';
  do {
    getocp(&book, &theline, org, orgchange, orgopen, chr, chrchange, chropen,
	   pie, piechange, pieopen);
    if (*pieopen) {
      *typefound = 'p';
      *aname = (*pie)->key.hea.keynam;
    } else if (*chropen) {
      *typefound = 'c';
      *aname = chr->hea.keynam;
    } else if (*orgopen) {
      *typefound = 'o';
      *aname = org->hea.keynam;
    } else
      *typefound = ' ';
  } while (*typefound != nametype && *typefound != 'o' && *typefound != ' ');
}



Static Void maxname(namebook, theline, nametype, namewidth, tofile,
		    linenumber, org, orgchange, orgopen, chr, chrchange,
		    chropen, pie, piechange, pieopen)
_TEXT *namebook;
long theline;
Char nametype;
long *namewidth;
_TEXT *tofile;
long *linenumber;
orgkey *org;
boolean *orgchange, *orgopen;
chrkey *chr;
boolean *chrchange, *chropen;
piece **pie;
boolean *piechange, *pieopen;
{
  /*

*/
  name aname;
  Char typefound = '.';

  if (*namebook->name != '\0') {
    if (namebook->f != NULL)
      namebook->f = freopen(namebook->name, "r", namebook->f);
    else
      namebook->f = fopen(namebook->name, "r");
  } else
    rewind(namebook->f);
  if (namebook->f == NULL)
    _EscIO2(FileNotFound, namebook->name);
  RESETBUF(namebook->f, Char);
  *namewidth = 0;
  if (emptyfile(namebook))
    return;
  brinit(namebook, &theline);
  *orgchange = false;
  *chrchange = false;
  *piechange = false;
  *orgopen = false;
  *chropen = false;
  *pieopen = false;
  if (headercontrol == 'h') {
    switch (nametype) {

    case 'g':
      fprintf(tofile->f, "gene");
      break;

    case 't':
      fprintf(tofile->f, "transcript");
      break;

    case 'p':
      fprintf(tofile->f, "piece");
      break;
    }
    fprintf(tofile->f, " names from:\n");
    splitbooktitle(namebook, tofile);
    theline++;
    *linenumber += 2;
  }
  *orgopen = false;
  while (typefound != ' ') {
    getname(namebook, theline, nametype, &typefound, &aname, org, orgchange,
	    orgopen, chr, chrchange, chropen, pie, piechange, pieopen);
    if (typefound != ' ' && typefound != 'o') {
      if (typefound == nametype) {
	if (aname.length > *namewidth)
	  *namewidth = aname.length;
      }
    }
  }
}



Static Void maxfullnamewidth(book, theline, fullnamewidth)
_TEXT *book;
long theline, *fullnamewidth;
{
  /*
*/
  line *fullname;
  Char typefound;
  long SET[5];

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
  *fullnamewidth = 0;


  while (!BUFEOF(book->f)) {
    typefound = getto(book, &theline, P_addset(P_expset(SET, 0L), 'p'));
    if (typefound != 'p')
      continue;
    fscanf(book->f, "%*[^\n]");
    getc(book->f);
    fscanf(book->f, "%*[^\n]");
    getc(book->f);
    getlineDelila(&fullname);
    brline(book, &theline, &fullname);
    /*




*/

    if (fullname->length > *fullnamewidth)
      *fullnamewidth = fullname->length;
    /*

*/
    clearline(&fullname);


    typefound = getto(book, &theline, P_addset(P_expset(SET, 0L), 'p'));
    if (typefound != 'p') {
      printf("book structure is bad: a piece does not have an end\n");
      halt();
    }
    fscanf(book->f, "%*[^\n]");
    getc(book->f);
    theline++;
  }

  /*

*/


  /*

*/

}



Static Void printname(namebook, theline, nametype, namewidth, tofile, ctofile,
		      org, orgchange, orgopen, chr, chrchange, chropen, pie,
		      piechange, pieopen, linenumber, clinenumber)
_TEXT *namebook;
long theline;
Char nametype;
long namewidth;
_TEXT *tofile, *ctofile;
orgkey *org;
boolean *orgchange, *orgopen;
chrkey *chr;
boolean *chrchange, *chropen;
piece **pie;
boolean *piechange, *pieopen;
long *linenumber, *clinenumber;
{
  /*


*/
  name aname;
  Char typefound;
  long index;

  do {
    getname(namebook, theline, nametype, &typefound, &aname, org, orgchange,
	    orgopen, chr, chrchange, chropen, pie, piechange, pieopen);
    if (typefound != ' ') {
      switch (typefound) {

      case 'o':
	fprintf(tofile->f, "\n organism %.*s\n", namelength, aname.letters);
	fprintf(ctofile->f, "\n organism %.*s\n", namelength, aname.letters);
	*linenumber += 2;
	*clinenumber += 2;
	break;

      case 'g':
      case 't':
      case 'p':
	putc(' ', tofile->f);
	putc(' ', ctofile->f);
	for (index = 0; index < namewidth; index++)
	  putc(aname.letters[index], tofile->f);
	for (index = 0; index < namewidth; index++)
	  putc(aname.letters[index], ctofile->f);
	break;
      }
    }
  } while (typefound != nametype && typefound != ' ');
}



Static Void maxnamelist(namelist, namelistwidth)
_TEXT *namelist;
long *namelistwidth;
{
  Char c_;
  long currentmax = 1, count = 0;

  /*

*/
  if (*namelist->name != '\0') {
    if (namelist->f != NULL)
      namelist->f = freopen(namelist->name, "r", namelist->f);
    else
      namelist->f = fopen(namelist->name, "r");
  } else
    rewind(namelist->f);
  if (namelist->f == NULL)
    _EscIO2(FileNotFound, namelist->name);
  RESETBUF(namelist->f, Char);
  if (emptyfile(namelist)) {
    *namelistwidth = 0;
    /*


*/
    return;
  }
  while (!BUFEOF(namelist->f)) {
    if (P_eoln(namelist->f)) {
      fscanf(namelist->f, "%*[^\n]");
      getc(namelist->f);
      if (currentmax > *namelistwidth)
	*namelistwidth = currentmax;
      currentmax = 1;
      continue;
    }
    c_ = getc(namelist->f);
    if (c_ == '\n')
      c_ = ' ';
    count++;
    currentmax++;
  }
}



Static Void protectcharacter(c_, protectioncharacter, needed)
Char c_, *protectioncharacter;
boolean *needed;
{
  /*





*/
  if (c_ == '\\' || c_ == '%' || c_ == ')' || c_ == '(') {
    *protectioncharacter = '\\';
    *needed = true;
  } else {
    *protectioncharacter = ' ';
    *needed = false;
  }
}



Static Void protectpostscript(afile, c_)
_TEXT *afile;
Char c_;
{
  /*




*/
  boolean needed;
  Char protectionchar;

  protectcharacter(c_, &protectionchar, &needed);
  if (needed)
    putc(protectionchar, afile->f);
}


Static Void figureblanks(blanks)
long *blanks;
{
  *blanks = namelistwidth + 1;

  if (!emptyfile(&namebook))
    *blanks += namewidth + 1;

  /*


*/


  if (displaylevel == 'p')
    *blanks += namewidth + positionwidth + 3;
  if (displaylevel == 'c')
    *blanks += positionwidth + 3;

  if (numbered)
    *blanks += numberwidth + 1;

  if (usefullname == 'l')
    *blanks += fullnamewidth + 1;

}


#define debugging       false


Static Void startpostscript(a_, colors, programname, deltaXcm, deltaYcm,
  scaleimage, defaultllx_, defaultlly_, defaulturx_, defaultury_, fromdo,
  todo, sequences, cmfactor, headerclinenumber)
_TEXT *a_, *colors;
name programname;
double deltaXcm, deltaYcm, scaleimage, defaultllx_, defaultlly_, defaulturx_,
       defaultury_;
long fromdo, todo, sequences;
double cmfactor;
long headerclinenumber;
{
  /*
*/
  /*







*/
  long i;
  double red, green, blue;
  Char symbol;

  double currentdisplay, requireddisplay, pointcorrection;
  long lines;
  /*
*/

  long numbarlines, actuallines;


  cpagelength = (long)((defaultury_ - defaultlly_) / fontsize);


  actuallines = sequences;


  if (headercontrol != '0') {
    numbarlines = firstlastmax(fromdo, todo);

    actuallines += numbarlines + 1;
  }


  if (headercontrol == 'h')
    actuallines += headerclinenumber;

  /*




*/
  /*





*/

  if (actuallines < cpagelength)
    lines = actuallines;
  else
    lines = cpagelength;


  if (debugging) {
    printf("-------------------\n");
    printf("fromdo      = %ld\n", fromdo);
    printf("  todo      = %ld\n", todo);
    printf("numbersize(fromdo)     = %ld\n", numbersize(fromdo));
    printf("numbersize(  todo)     = %ld\n", numbersize(todo));
    printf("numbarlines = %ld\n", numbarlines);
    printf("sequences   = %ld\n", sequences);
    printf("headerlines = %ld\n", (long)headerlines);
    printf("actuallines = %ld\n", actuallines);
    printf("cpagelength = %ld\n", cpagelength);
    printf("lines       = %ld\n", lines);
    printf("-------------------\n");
    printf("cmfactor    = %1.1f\n", cmfactor);
    printf("defaultury         = %1.1f\n", defaultury_);
    printf("defaultlly         = %1.1f\n", defaultlly_);
    printf("(defaultury-defaultlly)/cmfactor = %1.1f\n",
	   (defaultury_ - defaultlly_) / cmfactor);
    printf("(defaultury-defaultlly) = %1.1f\n", defaultury_ - defaultlly_);
    printf("-------------------\n\n");
  }


  if (edgecontrol != 'p') {
    figureblanks(&blanks);
    /*







*/
    /*


*/
    displaywidth = blanks + todo - fromdo + 4;
    if (columnlength > columnwid)
      displaywidth += columnlength;
    else
      displaywidth += columnwid;
    /*


*/
    /*


*/

    if (colorlistcontrol != 'R') {
      llx = defaultllx_;
      lly = defaultlly_;
      urx = defaulturx_;
      ury = defaultury_;

      currentdisplay = urx - llx;


      requireddisplay = fontsize * defcharacterratio * displaywidth;

      pointcorrection = requireddisplay - currentdisplay;

      /*




*/

      urx += pointcorrection;

      llx -= edgeleft * cmfactor;
      lly -= edgelow * cmfactor;
      urx += edgeright * cmfactor;
      ury += edgehigh * cmfactor;


      lly += (cpagelength - lines) * fontsize;
      /*


*/
    }

    else {
      llx = defaultlly_;
      lly = defaultllx_;
      urx = defaultury_;
      ury = defaulturx_;

      currentdisplay = ury - lly;


      requireddisplay = fontsize * defcharacterratio * displaywidth;
      pointcorrection = requireddisplay - currentdisplay;
      /*



*/

      ury += pointcorrection;

      llx -= edgehigh * cmfactor;
      lly -= edgeleft * cmfactor;
      urx += edgelow * cmfactor;
      ury += edgeright * cmfactor;


      urx += (lines - cpagelength) * fontsize;
    }



    llx += deltaXcm * cmfactor;
    lly += deltaYcm * cmfactor;
    urx += deltaXcm * cmfactor;
    ury += deltaYcm * cmfactor;

  }



  fprintf(a_->f, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(a_->f, "%%%%Title: ");
  for (i = 0; i < programname.length; i++)
    putc(programname.letters[i], a_->f);
  fprintf(a_->f, " %4.2f\n", version);
  fprintf(a_->f, "%%%%Creator: Tom Schneider\n");
  fprintf(a_->f, "%%%%BoundingBox: %5ld %5ld %5ld %5ld\n",
	  (long)floor(llx + 0.5), (long)floor(lly + 0.5),
	  (long)floor(urx + 0.5), (long)floor(ury + 0.5));
  fprintf(a_->f, "%%%%Pages: 1\n");
  fprintf(a_->f, "%%%%DocumentFonts:\n");
  fprintf(a_->f, "%%%%EndComments\n");
  fprintf(a_->f, "%%%%EndProlog\n");


  fprintf(a_->f,
	  "/cmfactor 72 2.54 div def %% defines points -> centimeters\n");
  fprintf(a_->f, "/cm { cmfactor mul} def %% defines centimeters\n");

  fprintf(a_->f, "/llx %5.1f def\n", llx);
  fprintf(a_->f, "/lly %5.1f def\n", lly);
  fprintf(a_->f, "/urx %5.1f def\n", urx);
  fprintf(a_->f, "/ury %5.1f def\n", ury);

  fprintf(a_->f, "/edgecontrol (%c) def\n", edgecontrol);
  fprintf(a_->f, "/edgeleft  %*.*f cm def\n", pwid, pdec, edgeleft);
  fprintf(a_->f, "/edgeright %*.*f cm def\n", pwid, pdec, edgeright);
  fprintf(a_->f, "/edgelow   %*.*f cm def\n", pwid, pdec, edgelow);
  fprintf(a_->f, "/edgehigh  %*.*f cm def\n\n", pwid, pdec, edgehigh);


  fprintf(a_->f, "/deltaXcm %10.5f cm def\n", deltaXcm);
  fprintf(a_->f, "/deltaYcm %10.5f cm def\n", deltaYcm);
  fprintf(a_->f, "/scaleimage %10.5f def\n", scaleimage);
  fprintf(a_->f, "/colorlistcontrol (%c) def\n\n", colorlistcontrol);

  fprintf(a_->f,
	  "%% based on typefaces program from page 40 of the Blue book\n\n");
  fprintf(a_->f, "%% variables\n");
  fprintf(a_->f, "/defcharacterratio %*.*f def\n",
	  pwid, pdec, defcharacterratio);
  fprintf(a_->f, "/fontsize %ld def\n", fontsize);
  fprintf(a_->f, "/lineseparation fontsize def\n");
  fprintf(a_->f, "/thefont /Courier-Bold def\n\n");
  fprintf(a_->f, "%% set the font\n");
  /*



*/


  fprintf(a_->f, "/f {findfont fontsize scalefont setfont} def\n");
  fprintf(a_->f, "thefont f\n\n");
  fprintf(a_->f, "/dosymbol {%% dosymbol [symbol] - make a symbol\n");
  fprintf(a_->f, "%% see page 272 in the Red book\n");
  fprintf(a_->f, "/thissymbol exch def\n");
  fprintf(a_->f, "%% switch to the Symbol font\n");
  fprintf(a_->f, "/Symbol f\n");
  fprintf(a_->f, "thissymbol show\n");
  fprintf(a_->f, "%% switch back to the standard font\n");
  fprintf(a_->f, "thefont f\n");
  fprintf(a_->f, "} def\n");

  fprintf(a_->f, "/n {\n");
  fprintf(a_->f, "/currenty currenty lineseparation sub def\n");

  fprintf(a_->f, "0 currenty moveto} def\n");
  fprintf(a_->f, "/s {show} def\n");
  fprintf(a_->f, "/sn {s n} def\n\n");

  fprintf(a_->f, "colorlistcontrol (R) ne\n");
  fprintf(a_->f, "{ %% normal page display: portrait mode\n");
  fprintf(a_->f, "  /topofpage %*.*f cm def\n", pwid, pdec, topofpage);
  fprintf(a_->f, "  /startpage{\n");


  fprintf(a_->f, "  deltaXcm fontsize add deltaYcm translate\n");
  fprintf(a_->f, "  scaleimage dup scale\n");
  fprintf(a_->f, "  0 topofpage moveto\n");

  fprintf(a_->f, "  /currenty currentpoint exch pop def\n");
  fprintf(a_->f, "  } def\n");
  fprintf(a_->f, "}\n");

  fprintf(a_->f, "{ %% page rotation to landscape mode\n");
  fprintf(a_->f, "  /topofpage %*.*f cm def\n", pwid, pdec, 0.0);
  fprintf(a_->f, "  /startpage{\n");


  fprintf(a_->f, "  deltaXcm fontsize 1.5 mul add\n");
  fprintf(a_->f, "  deltaYcm fontsize defcharacterratio 2 mul mul add\n");
  fprintf(a_->f, "  translate\n");
  fprintf(a_->f, "  +90 rotate\n");
  fprintf(a_->f, "  scaleimage dup scale\n");
  fprintf(a_->f, "  0 topofpage moveto\n");

  fprintf(a_->f, "  /currenty currentpoint exch pop def\n");
  fprintf(a_->f, "  } def\n");
  fprintf(a_->f, "}\n");
  fprintf(a_->f, "ifelse\n\n");
  if (*colors->name != '\0') {
    if (colors->f != NULL)
      colors->f = freopen(colors->name, "r", colors->f);
    else
      colors->f = fopen(colors->name, "r");
  } else {


    rewind(colors->f);
  }
  if (colors->f == NULL)
    _EscIO2(FileNotFound, colors->name);
  RESETBUF(colors->f, Char);
  if (!BUFEOF(colors->f)) {
    while (!BUFEOF(colors->f)) {
      if (P_peek(colors->f) == '*') {
	fscanf(colors->f, "%*[^\n]");
	getc(colors->f);
	continue;
      }

      if (P_peek(colors->f) == '\\')
	getc(colors->f);
      fscanf(colors->f, "%c%lg%lg%lg%*[^\n]", &symbol, &red, &green, &blue);
      getc(colors->f);

      if (symbol == '\n')
	symbol = ' ';
      putc('/', a_->f);
      protectpostscript(a_, symbol);
      fprintf(a_->f, "%c{", symbol);
      if (red == 1.0 || red == 0.0)
	fprintf(a_->f, "%ld", (long)floor(red + 0.5));
      else
	fprintf(a_->f, "%*.*f", pwid, pdecolor, red);
      putc(' ', a_->f);
      if (green == 1.0 || green == 0.0)
	fprintf(a_->f, "%ld", (long)floor(green + 0.5));
      else
	fprintf(a_->f, "%*.*f", pwid, pdecolor, green);
      putc(' ', a_->f);
      if (blue == 1.0 || blue == 0.0)
	fprintf(a_->f, "%ld", (long)floor(blue + 0.5));
      else
	fprintf(a_->f, "%*.*f", pwid, pdecolor, blue);
      fprintf(a_->f, " setrgbcolor (%c) s} def\n", symbol);
    }
  } else {
    fprintf(a_->f, "/a {0 1 0 setrgbcolor (a) s} def\n");
    fprintf(a_->f, "/c {0 0.9372 1 setrgbcolor (c) s} def\n");
    fprintf(a_->f, "/g {1 0.7 0 setrgbcolor (g) s} def\n");
    fprintf(a_->f, "/t {1 0 0 setrgbcolor (t) s} def\n\n");
  }
  fprintf(a_->f, "/b {0 0 0 setrgbcolor} def\n");
  fprintf(a_->f, "/e {( ) s} def\n");

}

#undef debugging


Static Void readcolumnname(f)
_TEXT *f;
{
  boolean done = false;

  if (*f->name != '\0') {
    if (f->f != NULL)
      f->f = freopen(f->name, "r", f->f);
    else
      f->f = fopen(f->name, "r");
  } else
    rewind(f->f);
  if (f->f == NULL)
    _EscIO2(FileNotFound, f->name);
  RESETBUF(f->f, Char);
  readvalues = false;
  columnlength = 0;
  if (!BUFEOF(f->f)) {
    getc(f->f);
    if (!P_eoln(f->f)) {
      getc(f->f);
      if (!P_eoln(f->f)) {
	while (!P_eoln(f->f)) {
	  columnlength++;
	  columnname[columnlength-1] = getc(f->f);
	  if (columnname[columnlength-1] == '\n')
	    columnname[columnlength-1] = ' ';
	  readvalues = true;
	}
	fscanf(f->f, "%*[^\n]");
	getc(f->f);
      }
    }
  }

  /*
*/
  if (!readvalues)
    return;

  while (!done) {
    if (BUFEOF(f->f)) {
      readvalues = false;
      done = true;
    } else {
      if (P_peek(f->f) == '*') {
	fscanf(f->f, "%*[^\n]");
	getc(f->f);
      } else
	done = true;
    }
  }
}


Static Void writecolumnname(f)
_TEXT *f;
{
  long i, FORLIM;

  FORLIM = columnlength;
  for (i = 0; i < FORLIM; i++)
    putc(columnname[i], f->f);
}


Static Void getcolumnvalue(f, column, cvalue)
_TEXT *f;
long column;
double *cvalue;
{
  long c_;
  boolean gotit = false;

  while (!BUFEOF(f->f) && !gotit) {
    if (P_peek(f->f) == '*') {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
      continue;
    }
    for (c_ = 1; c_ < column; c_++)
      skipcolumn(f);
    if (BUFEOF(f->f))
      break;
    fscanf(f->f, "%lg%*[^\n]", cvalue);
    getc(f->f);
    gotit = true;
  }
  if (gotit)
    return;
  printf("getcolumnvalue:  could not locate the column %ld value\n", column);
  printf("in the avalues file at sequence number %ld\n", number);
  if (BUFEOF(f->f))
    printf("end of file avalues was found\n");
  halt();
}


Static Void showalignment()
{
  long lines, blanks, index, FORLIM;

  figureblanks(&blanks);
  numberbar(&list, blanks, fromdo, todo, &lines);


  for (index = 1; index <= blanks; index++)
    putc(' ', list.f);
  FORLIM = todo;
  for (index = fromdo; index <= FORLIM; index++)
    putc('.', list.f);

  if (readvalues) {
    putc(' ', list.f);
    writecolumnname(&list);
  }

  putc('\n', list.f);

  linenumber += lines + 1;
}


Static Void cshowalignment()
{
  /*
*/
  long lines, blanks, index, FORLIM;

  figureblanks(&blanks);

  /*




*/

  pnumberbar(&clist, blanks, fromdo, todo, &lines);


  putc('(', clist.f);
  for (index = 1; index <= blanks; index++)
    putc(' ', clist.f);
  FORLIM = todo;
  for (index = fromdo; index <= FORLIM; index++)
    putc('.', clist.f);

  if (readvalues) {
    putc(' ', clist.f);
    writecolumnname(&clist);
  }
  fprintf(clist.f, ") sn");

  if (cpagenumber > 1)
    removeit(&clist);
  else
    putc('\n', clist.f);

  clinenumber += lines + 1;
}


Static Void showpage(a_)
_TEXT *a_;
{
  fprintf(a_->f, "b\n");
  fprintf(a_->f, "showpage");
  removeit(a_);
}


/* Local variables for checknumber: */
struct LOC_checknumber {
  _TEXT *afile;
  boolean ok;
} ;

Local Void conclude(LINK)
struct LOC_checknumber *LINK;
{
  _TEXT TEMP;

  printf("Including this character, the rest of the data line is:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  copyaline(LINK->afile, &TEMP);
  LINK->ok = false;
}


Static boolean checknumber(afile_)
_TEXT *afile_;
{
  /*
*/
  struct LOC_checknumber V;

  V.afile = afile_;
  V.ok = true;
  if (BUFEOF(V.afile->f)) {
    V.ok = false;
    printf("A number was expected on a data line, but");
    printf(" the end of the file was found instead.\n");
    return false;
  }
  skipblanks(V.afile);
  if (P_eoln(V.afile->f)) {
    printf("A number was expected on a data line, but");
    printf(" the end of the line was found instead.\n");
    conclude(&V);
  }
  if (P_peek(V.afile->f) == '+' || P_peek(V.afile->f) == '-' ||
      P_peek(V.afile->f) == '.' || P_peek(V.afile->f) == '9' ||
      P_peek(V.afile->f) == '8' || P_peek(V.afile->f) == '7' ||
      P_peek(V.afile->f) == '6' || P_peek(V.afile->f) == '5' ||
      P_peek(V.afile->f) == '4' || P_peek(V.afile->f) == '3' ||
      P_peek(V.afile->f) == '2' || P_peek(V.afile->f) == '1' ||
      P_peek(V.afile->f) == '0')
    return V.ok;
  printf("A number was expected on a data line, but");
  printf(" the character \"%c\" was found instead.\n", P_peek(V.afile->f));
  conclude(&V);
  return V.ok;
}



Static Void copyfile(fin, fout)
_TEXT *fin, *fout;
{
  while (!BUFEOF(fin->f))
    copyaline(fin, fout);
}



Static Void almostcopyfile(fin, fout)
_TEXT *fin, *fout;
{
  /*



*/
  while (!BUFEOF(fin->f)) {
    if (P_peek(fin->f) != 'a')
      copyaline(fin, fout);
    else {
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
    }
  }
}



Static Void upgradeto596()
{
  /*
*/
  _TEXT internal;

  internal.f = NULL;
  *internal.name = '\0';
  parameterversion = 5.96;
  printf("upgrading to version %4.2f ...\n", parameterversion);
  if (*alistp.name != '\0') {
    if (alistp.f != NULL)
      alistp.f = freopen(alistp.name, "r", alistp.f);
    else
      alistp.f = fopen(alistp.name, "r");
  } else
    rewind(alistp.f);
  if (alistp.f == NULL)
    _EscIO2(FileNotFound, alistp.name);
  RESETBUF(alistp.f, Char);

  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  almostcopyfile(&alistp, &internal);

  if (*alistp.name != '\0') {
    if (alistp.f != NULL)
      alistp.f = freopen(alistp.name, "w", alistp.f);
    else
      alistp.f = fopen(alistp.name, "w");
  } else {
    if (alistp.f != NULL)
      rewind(alistp.f);
    else
      alistp.f = tmpfile();
  }
  if (alistp.f == NULL)
    _EscIO2(FileNotFound, alistp.name);
  SETUPBUF(alistp.f, Char);
  fprintf(alistp.f,
    "%4.2f        version of alistp that this parameter file is designed for.\n",
    parameterversion);

  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  copyfile(&internal, &alistp);



  fprintf(alistp.f,
    "n 0 0 0 0   edgecontrol (p=page), edgeleft, edgeright, edgelow, edgehigh in cm\n");

  if (*alistp.name != '\0') {
    if (alistp.f != NULL)
      alistp.f = freopen(alistp.name, "r", alistp.f);
    else
      alistp.f = fopen(alistp.name, "r");
  } else
    rewind(alistp.f);
  if (alistp.f == NULL)
    _EscIO2(FileNotFound, alistp.name);
  RESETBUF(alistp.f, Char);
  if (internal.f != NULL)
    fclose(internal.f);
}



Static Void upgradeto598()
{
  /*
*/
  _TEXT internal;

  internal.f = NULL;
  *internal.name = '\0';
  parameterversion = 5.98;
  printf("upgrading to version %4.2f ...\n", parameterversion);


  if (*alistp.name != '\0') {
    if (alistp.f != NULL)
      alistp.f = freopen(alistp.name, "r", alistp.f);
    else
      alistp.f = fopen(alistp.name, "r");
  } else
    rewind(alistp.f);
  if (alistp.f == NULL)
    _EscIO2(FileNotFound, alistp.name);
  RESETBUF(alistp.f, Char);
  fscanf(alistp.f, "%*[^\n]");
  getc(alistp.f);
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  copyfile(&alistp, &internal);


  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  if (*alistp.name != '\0') {
    if (alistp.f != NULL)
      alistp.f = freopen(alistp.name, "w", alistp.f);
    else
      alistp.f = fopen(alistp.name, "w");
  } else {
    if (alistp.f != NULL)
      rewind(alistp.f);
    else
      alistp.f = tmpfile();
  }
  if (alistp.f == NULL)
    _EscIO2(FileNotFound, alistp.name);
  SETUPBUF(alistp.f, Char);
  fprintf(alistp.f,
    "%4.2f        version of alistp that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, &alistp);


  fprintf(alistp.f,
    "%c %ld        mapcontrol: C=do map, R= rotate, char height (72 points/inch)\n",
    defcolorlistcontrol, (long)deffontsize);

  fprintf(alistp.f,
    "%3.1f %4.1f %3.1f amount to move image in x and y (cm) and scale factor\n",
    defdeltaXcm, defdeltaYcm, defscaleimage);
  if (*alistp.name != '\0') {
    if (alistp.f != NULL)
      alistp.f = freopen(alistp.name, "r", alistp.f);
    else
      alistp.f = fopen(alistp.name, "r");
  } else
    rewind(alistp.f);
  if (alistp.f == NULL)
    _EscIO2(FileNotFound, alistp.name);
  RESETBUF(alistp.f, Char);
  if (internal.f != NULL)
    fclose(internal.f);
}

Static Void upgradeto620()
{
  /*
*/
  _TEXT internal;

  internal.f = NULL;
  *internal.name = '\0';
  parameterversion = 6.20;
  printf("upgrading to version %4.2f ...\n", parameterversion);


  if (*alistp.name != '\0') {
    if (alistp.f != NULL)
      alistp.f = freopen(alistp.name, "r", alistp.f);
    else
      alistp.f = fopen(alistp.name, "r");
  } else
    rewind(alistp.f);
  if (alistp.f == NULL)
    _EscIO2(FileNotFound, alistp.name);
  RESETBUF(alistp.f, Char);
  fscanf(alistp.f, "%*[^\n]");
  getc(alistp.f);
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  copyfile(&alistp, &internal);


  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  if (*alistp.name != '\0') {
    if (alistp.f != NULL)
      alistp.f = freopen(alistp.name, "w", alistp.f);
    else
      alistp.f = fopen(alistp.name, "w");
  } else {
    if (alistp.f != NULL)
      rewind(alistp.f);
    else
      alistp.f = tmpfile();
  }
  if (alistp.f == NULL)
    _EscIO2(FileNotFound, alistp.name);
  SETUPBUF(alistp.f, Char);
  fprintf(alistp.f,
    "%4.2f        version of alistp that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, &alistp);


  fprintf(alistp.f,
    "h           headercontrol: h(eader); 0: no header, no numbar; else numbar\n");

  if (*alistp.name != '\0') {
    if (alistp.f != NULL)
      alistp.f = freopen(alistp.name, "r", alistp.f);
    else
      alistp.f = fopen(alistp.name, "r");
  } else
    rewind(alistp.f);
  if (alistp.f == NULL)
    _EscIO2(FileNotFound, alistp.name);
  RESETBUF(alistp.f, Char);
  if (internal.f != NULL)
    fclose(internal.f);
}

Static Void upgradeparameters(alistp)
_TEXT *alistp;
{
  fscanf(alistp->f, "%lg%*[^\n]", &parameterversion);
  getc(alistp->f);
  if (parameterversion >= updateversion)
    return;
  printf("\007You have an old parameter file, version %4.2f!\n",
	 parameterversion);
  if (parameterversion < 5.96)
    upgradeto596();

  if (*alistp->name != '\0') {
    if (alistp->f != NULL)
      alistp->f = freopen(alistp->name, "r", alistp->f);
    else
      alistp->f = fopen(alistp->name, "r");
  } else
    rewind(alistp->f);
  if (alistp->f == NULL)
    _EscIO2(FileNotFound, alistp->name);
  RESETBUF(alistp->f, Char);
  fscanf(alistp->f, "%lg%*[^\n]", &parameterversion);
  getc(alistp->f);
  if (parameterversion < 5.98)
    upgradeto598();

  if (*alistp->name != '\0') {
    if (alistp->f != NULL)
      alistp->f = freopen(alistp->name, "r", alistp->f);
    else
      alistp->f = fopen(alistp->name, "r");
  } else
    rewind(alistp->f);
  if (alistp->f == NULL)
    _EscIO2(FileNotFound, alistp->name);
  RESETBUF(alistp->f, Char);
  fscanf(alistp->f, "%lg%*[^\n]", &parameterversion);
  getc(alistp->f);
  if (parameterversion < 6.18)
    upgradeto620();

  if (*alistp->name != '\0') {
    if (alistp->f != NULL)
      alistp->f = freopen(alistp->name, "r", alistp->f);
    else
      alistp->f = fopen(alistp->name, "r");
  } else
    rewind(alistp->f);
  if (alistp->f == NULL)
    _EscIO2(FileNotFound, alistp->name);
  RESETBUF(alistp->f, Char);
  fscanf(alistp->f, "%lg%*[^\n]", &parameterversion);
  getc(alistp->f);
  if (parameterversion < updateversion) {
    printf("Sorry! I am unable to fully upgrade your parameter file\n");
    printf("from version %4.2f to version %4.2f!\n",
	   parameterversion, updateversion);
    printf("Start from a fresh copy or edit this one.\n");
    halt();
  } else
    printf("... upgrade successful!\n");

  printf("See this page for the new documentation:\n");
  printf("https://alum.mit.edu/www/toms/delila/alist.html\n");
}


/* Local variables for readparameters: */
struct LOC_readparameters {
  boolean checkout;
} ;

Local Void cn(LINK)
struct LOC_readparameters *LINK;
{
  LINK->checkout = checknumber(&alistp);
  if (!LINK->checkout)
    halt();
}



Static Void readparameters()
{
  struct LOC_readparameters V;

  /*
*/
  fromdo = fromparam;
  todo = toparam;
  userrange = false;
  displaylevel = ' ';
  shownumbers = ' ';
  usefullname = ' ';
  paging = ' ';
  alignmenttype = 'i';
  column = 6;
  columnwid = 6;
  columndec = 2;
  edgecontrol = 'p';
  fontsize = deffontsize;
  deltaXcm = defdeltaXcm;
  deltaYcm = defdeltaYcm;
  scaleimage = defscaleimage;
  colorlistcontrol = ' ';
  mapcontrol = false;
  headercontrol = 'h';


  if (*alistp.name != '\0') {
    if (alistp.f != NULL)
      alistp.f = freopen(alistp.name, "r", alistp.f);
    else
      alistp.f = fopen(alistp.name, "r");
  } else
    rewind(alistp.f);
  if (alistp.f == NULL)
    _EscIO2(FileNotFound, alistp.name);
  RESETBUF(alistp.f, Char);
  if (BUFEOF(alistp.f))
    return;
  upgradeparameters(&alistp);

  fscanf(alistp.f, "%ld%ld%*[^\n]", &fromdo, &todo);
  getc(alistp.f);
  if (fromdo > todo) {
    printf("from position (%ld) must be less than or equal to to position (%ld)\n",
	   fromdo, todo);
    halt();
  }
  userrange = true;

  if (!BUFEOF(alistp.f)) {
    fscanf(alistp.f, "%c%c%c%*[^\n]", &displaylevel, &usefullname,
	   &shownumbers);
    getc(alistp.f);
    if (displaylevel == '\n')
      displaylevel = ' ';
    if (usefullname == '\n')
      usefullname = ' ';
    if (shownumbers == '\n')
      shownumbers = ' ';
  }

  if (!BUFEOF(alistp.f)) {
    fscanf(alistp.f, "%c%*[^\n]", &paging);
    getc(alistp.f);
    if (paging == '\n')
      paging = ' ';
  }

  if (!BUFEOF(alistp.f)) {
    fscanf(alistp.f, "%c%*[^\n]", &alignmenttype);
    getc(alistp.f);
    if (alignmenttype == '\n')
      alignmenttype = ' ';
    if (alignmenttype != 'b' && alignmenttype != 'i' && alignmenttype != 'f') {
      printf("alignment type must be f, b, or i\n");
      halt();
    }
  }

  if (BUFEOF(alistp.f)) {
    printf("you are missing parameter columntoread etc of alistp!\n");
    halt();
  }
  cn(&V);
  fscanf(alistp.f, "%ld%ld%ld%*[^\n]", &columntoread, &columnwid, &columndec);
  getc(alistp.f);

  if (BUFEOF(alistp.f)) {
    printf("you are missing parameter edgecontrol etc of alistp!\n");
    halt();
  }

  edgecontrol = getc(alistp.f);
  if (edgecontrol == '\n')
    edgecontrol = ' ';
  if (edgecontrol != 'p')
    fscanf(alistp.f, "%lg%lg%lg%lg", &edgeleft, &edgeright, &edgelow,
	   &edgehigh);
  fscanf(alistp.f, "%*[^\n]");

  getc(alistp.f);
  if (BUFEOF(alistp.f)) {
    printf("you are missing parameter fontsize etc of alistp!\n");
    halt();
  }

  colorlistcontrol = getc(alistp.f);
  if (colorlistcontrol == '\n')
    colorlistcontrol = ' ';
  if (colorlistcontrol == 'R' || colorlistcontrol == 'C') {
    mapcontrol = true;
    cn(&V);
    fscanf(alistp.f, "%ld%*[^\n]", &fontsize);
    getc(alistp.f);
  } else {
    fscanf(alistp.f, "%*[^\n]");
    getc(alistp.f);
    mapcontrol = false;
  }

  cn(&V);
  fscanf(alistp.f, "%lg", &deltaXcm);
  cn(&V);
  fscanf(alistp.f, "%lg", &deltaYcm);
  cn(&V);
  fscanf(alistp.f, "%lg%*[^\n]", &scaleimage);
  getc(alistp.f);

  fscanf(alistp.f, "%c%*[^\n]", &headercontrol);
  getc(alistp.f);

  if (headercontrol == '\n')
    headercontrol = ' ';
}

int main(int argc, Char **argv)
{
  line *WITH;
  long FORLIM;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  colors.f = NULL;
  strcpy(colors.name, "colors");
  clist.f = NULL;
  strcpy(clist.name, "clist");
  list.f = NULL;
  strcpy(list.name, "list");
  avalues.f = NULL;
  strcpy(avalues.name, "avalues");
  namelist.f = NULL;
  strcpy(namelist.name, "namelist");
  namebook.f = NULL;
  strcpy(namebook.name, "namebook");
  alistp.f = NULL;
  strcpy(alistp.name, "alistp");
  book.f = NULL;
  strcpy(book.name, "book");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  printf("alist %4.2f\n", version);
  readparameters();


  cmfactor = 72 / 2.54;

  if (*list.name != '\0') {
    if (list.f != NULL)
      list.f = freopen(list.name, "w", list.f);
    else
      list.f = fopen(list.name, "w");
  } else {
    if (list.f != NULL)
      rewind(list.f);
    else
      list.f = tmpfile();
  }
  if (list.f == NULL)
    _EscIO2(FileNotFound, list.name);
  SETUPBUF(list.f, Char);
  if (*clist.name != '\0') {
    if (clist.f != NULL)
      clist.f = freopen(clist.name, "w", clist.f);
    else
      clist.f = fopen(clist.name, "w");
  } else {
    if (clist.f != NULL)
      rewind(clist.f);
    else
      clist.f = tmpfile();
  }
  if (clist.f == NULL)
    _EscIO2(FileNotFound, clist.name);
  SETUPBUF(clist.f, Char);

  linenumber = 0;
  if (headercontrol == 'h') {
    fprintf(list.f, "alist %4.2f, aligned listing of book:\n", version);
    linenumber++;
    splitbooktitle(&book, &list);
    linenumber += 2;
  }


  clinenumber = 0;
  if (headercontrol == 'h' || headercontrol != '0')
    headerclinenumber = headerclines;
  else
    headerclinenumber = 0;

  pagenumber = 1;
  cpagenumber = 1;


  readcolumnname(&avalues);


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
  if (BUFEOF(inst.f)) {
    if (alignmenttype == 'i') {
      printf("forcing alignment to be on book because there are no instructions\n");
      alignmenttype = 'b';
    }
  }


  apiece = (piece *)Malloc(sizeof(piece));
  brinit(&book, &theline);
  if (displaylevel == 'p')
    maxname(&book, theline, 'p', &namewidth, &list, &linenumber, &org,
	    &orgchange, &orgopen, &chr, &chrchange, &chropen, &pie,
	    &piechange, &pieopen);
  else
    maxname(&namebook, theline, 'g', &namewidth, &list, &linenumber, &org,
	    &orgchange, &orgopen, &chr, &chrchange, &chropen, &pie,
	    &piechange, &pieopen);
  if (*namebook.name != '\0') {
    if (namebook.f != NULL)
      namebook.f = freopen(namebook.name, "r", namebook.f);
    else
      namebook.f = fopen(namebook.name, "r");
  } else
    rewind(namebook.f);
  if (namebook.f == NULL)
    _EscIO2(FileNotFound, namebook.name);
  RESETBUF(namebook.f, Char);
  orgchange = false;
  chrchange = false;
  piechange = false;
  orgopen = false;
  chropen = false;
  pieopen = false;

  maxnamelist(&namelist, &namelistwidth);
  namelistuse = (namelistwidth > 0);
  if (*namelist.name != '\0') {
    if (namelist.f != NULL)
      namelist.f = freopen(namelist.name, "r", namelist.f);
    else
      namelist.f = fopen(namelist.name, "r");
  } else
    rewind(namelist.f);
  if (namelist.f == NULL)
    _EscIO2(FileNotFound, namelist.name);
  RESETBUF(namelist.f, Char);
  maxfullnamewidth(&book, theline, &fullnamewidth);
  brinit(&book, &theline);

  getpositions(&inst, &book, &positionwidth, &numberwidth, &sequences,
	       alignmenttype);
  printf("%ld sequences in the book\n", sequences);

  maxminalignment(&inst, &book, &theline, &fromparam, &toparam, alignmenttype);

  printf("available range: %ld %ld\n", fromparam, toparam);

  programname.letters[0] = 'a';
  programname.letters[1] = 'l';
  programname.letters[2] = 'i';
  programname.letters[3] = 's';
  programname.letters[4] = 't';
  programname.length = 5;
  startpostscript(&clist, &colors, programname, deltaXcm, deltaYcm,
		  scaleimage, defaultllx, defaultlly, defaulturx, defaultury,
		  fromdo, todo, sequences, cmfactor, headerclinenumber);

  fprintf(clist.f, "startpage\n");
  if (headercontrol == 'h') {
    fprintf(clist.f, "(alist %4.2f, aligned listing of book: ) sn", version);
    noheader(&clist);
    clinenumber++;


    putc('(', clist.f);
    copytocomma(&book, &clist);
    copytocomma(&book, &clist);
    fprintf(clist.f, ") sn");
    noheader(&clist);
    clinenumber++;

    putc('(', clist.f);
    putc(' ', clist.f);
    copynoreturn(&book, &clist);
    fprintf(clist.f, ") sn");
    noheader(&clist);
    clinenumber++;

    noheader(&clist);

    printf("alignment by ");
    fprintf(list.f, "The alignment is by ");
    switch (alignmenttype) {

    case 'f':
      printf("first base\n");
      fprintf(list.f, "first base\n");
      break;

    case 'i':
      printf("delila instructions\n");
      fprintf(list.f, "delila instructions\n");
      break;

    case 'b':
      printf("book coordinates\n");
      fprintf(list.f, "book coordinates\n");
      break;
    }
    linenumber += 2;

    /*




*/
    fprintf(list.f, "The book is from:       %ld to %ld\n",
	    fromparam, toparam);
    linenumber++;
    fprintf(clist.f, "(The book is from:       %ld to %ld) sn",
	    fromparam, toparam);
    noheader(&clist);
    clinenumber++;
    /*

*/

    fprintf(list.f, "This alignment is from: %ld to %ld\n\n", fromdo, todo);
    linenumber += 2;
    fprintf(clist.f, "(This alignment is from: %ld to %ld) sn", fromdo, todo);
    noheader(&clist);
    clinenumber++;


  }



  if (headercontrol == 'h' || headercontrol != '0') {
    showalignment();
    cshowalignment();
  }

  while (!BUFEOF(book.f)) {
    switch (alignmenttype) {
    case 'i':
      align(&inst, &book, &theline, &apiece, &length_, &alignedbase);
      break;
    case 'b':
    case 'f':
      getpiece(&book, &theline, &apiece);
      length_ = piecelength(apiece);
      break;
    }

    if (BUFEOF(book.f))
      break;

    switch (alignmenttype) {
    case 'f':
      alignedbase = 1;
      break;
    case 'i':
      /* blank case */
      break;
    case 'b':
      alignedbase = pietoint(0L, apiece);
      break;
    }

    fprintf(clist.f, "b\n");

    if (usefullname == 'l') {
      putc('(', clist.f);
      WITH = apiece->key.hea.fulnam;
      FORLIM = WITH->length;
      for (index_ = 1; index_ <= FORLIM; index_++) {
	putc(WITH->letters[index_-1], list.f);
	putc(WITH->letters[index_-1], clist.f);
      }

      FORLIM = fullnamewidth + 1;
      for (index_ = WITH->length + 1; index_ <= FORLIM; index_++) {
	putc(' ', clist.f);
	putc(' ', list.f);
      }
      fprintf(clist.f, ") show\n");
    }

    if (displaylevel == 'p' || displaylevel == 'c') {
      if (displaylevel == 'p') {
	FORLIM = namewidth;
	for (index_ = 1; index_ <= FORLIM; index_++)
	  putc(apiece->key.hea.keynam.letters[index_-1], list.f);
      }

      /*
*/

      fprintf(list.f, " %*ld",
	      (int)positionwidth, inttopie(alignedbase, apiece));

      putc('(', clist.f);
      if (displaylevel == 'p') {
	FORLIM = namewidth;
	for (index_ = 1; index_ <= FORLIM; index_++)
	  putc(apiece->key.hea.keynam.letters[index_-1], clist.f);
      }

      fprintf(clist.f, " %*ld",
	      (int)positionwidth, inttopie(alignedbase, apiece));

      fprintf(clist.f, ") s ");
    }
    /*
*/

    else if (!emptyfile(&namebook))
      printname(&namebook, theline, nametype, namewidth, &list, &clist, &org,
		&orgchange, &orgopen, &chr, &chrchange, &chropen, &pie,
		&piechange, &pieopen, &linenumber, &clinenumber);

    if (namelistuse) {
      putc(' ', list.f);
      fprintf(clist.f, "( ");
      index_ = 0;
      while (!P_eoln(namelist.f)) {
	index_++;
	cnamelist = getc(namelist.f);
	if (cnamelist == '\n')
	  cnamelist = ' ';
	putc(cnamelist, list.f);
	putc(cnamelist, clist.f);
      }

      FORLIM = namelistwidth - index_;
      for (indexfill = 1; indexfill < FORLIM; indexfill++) {
	putc(' ', list.f);
	putc(' ', clist.f);
      }
      fprintf(clist.f, ") show\n");
      fscanf(namelist.f, "%*[^\n]");
      getc(namelist.f);
    }

    if (displaylevel == 'p' || displaylevel == 'c') {
      if (apiece->key.piedir == minus)
	thedirection = '-';
      else
	thedirection = '+';
      fprintf(list.f, " %c", thedirection);
      fprintf(clist.f, "( %c) s\n", thedirection);
    }


    if (numbered && shownumbers != '-')
      fprintf(list.f, " %*ld ", (int)numberwidth, number);
    /*

*/
    if (numbered && shownumbers != '-')
      fprintf(clist.f, "( %*ld ) s\n", (int)numberwidth, number);
    /*

*/
    column = 0;
    FORLIM = todo;
    for (index_ = fromdo; index_ <= FORLIM; index_++) {
      if (withinalignment(index_, alignedbase, length_)) {
	b = basetochar(getbase(index_ + alignedbase, apiece));
	putc(b, list.f);
	fprintf(clist.f, " %c", b);
      } else {
	putc(' ', list.f);
	fprintf(clist.f, " e");
      }
      column += 2;
      /*
*/
      if (column % 76 == 0) {
	putc('\n', clist.f);
	column = 0;
      }
    }


    if (readvalues) {
      getcolumnvalue(&avalues, columntoread, &columnvalue);
      fprintf(list.f, " %*.*f", (int)columnwid, (int)columndec, columnvalue);
      fprintf(clist.f, " ( %*.*f) b s",
	      (int)columnwid, (int)columndec, columnvalue);
    }

    putc('\n', list.f);
    fprintf(clist.f, " n\n");

    linenumber++;
    clinenumber++;
    if (paging != 'n') {
      if (linenumber > cpagelength) {
	fprintf(list.f, "\f");
	pagenumber++;
	fprintf(list.f, "page %ld\n", pagenumber);
	linenumber = 2;
	showalignment();
      }

      /*
*/

      if (clinenumber >= cpagelength) {
	cpagenumber++;
	/*
*/
	showpage(&clist);
	fprintf(clist.f, "startpage");
	removeit(&clist);
	fprintf(clist.f, "(page %ld) sn", cpagenumber);
	removeit(&clist);
	clinenumber = 2;
	clinenumber = 1;
	cshowalignment();
      }

    }

    clearpiece(&apiece);
  }
  if (clinenumber > 2)
    showpage(&clist);
  removeit(&clist);
  fprintf(clist.f, "%%%%Trailer\n");

_L1:
  if (inst.f != NULL)
    fclose(inst.f);
  if (book.f != NULL)
    fclose(book.f);
  if (alistp.f != NULL)
    fclose(alistp.f);
  if (namebook.f != NULL)
    fclose(namebook.f);
  if (namelist.f != NULL)
    fclose(namelist.f);
  if (avalues.f != NULL)
    fclose(avalues.f);
  if (list.f != NULL)
    fclose(list.f);
  if (clist.f != NULL)
    fclose(clist.f);
  if (colors.f != NULL)
    fclose(colors.f);
  exit(EXIT_SUCCESS);

  return 0;
}



/* End. */
