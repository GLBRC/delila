/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "malign.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*









*/



#define version         2.70
/*




























*/



/*

























































































































































































































































































*/


#define maxlen          5000
#define maxnseq         10000


#define maxchange       50
/*
*/
#define minmaxforchange  3
/*


*/
#define realignlimit    10

#define kickover        50
/*

*/


#define infofield       8
#define infodecim       5

#define posfield        4
#define nfield          4




/*

*/
#define dnamax          5000
#define namelength      100
#define linelength      200


/*
*/

#define fillermax       50




#define maxstring       2000







typedef long chset[5];




typedef Char alpha[namelength];

/*
*/

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

/* p2c: malign.p, line 426:
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
  long state_;
  boolean skip, found;
} trigger;



typedef struct seqrec {
  base alseq[maxlen];
  long length;
  name piecename;
  long alignread, alignstart, aligncurrent, alignmin, alignmax;
  struct seqrec *next;
} seqrec;

typedef long matrix[4][maxlen];
typedef double poslist[maxlen];
typedef double fltable[maxnseq + 1];

typedef struct optlist {
  long albest[maxnseq];
  double hbest;
  long num;
  struct optlist *next;
} optlist;


Static _TEXT inst, book, malignp, uncert, newalign, optalign, optinst,
	     malignxyin;

Static long winleft, winright;
/*
*/
Static long shiftmin, shiftmax, iseed, nranseq, nshuffle, ifpaired, standout,
	    npassout, nshiftout, ntolpass;
Static double tolerance;
Static matrix nbl;
Static poslist hl;
Static fltable flogf, fldiff, fldoubl;
Static seqrec *sepfirst, *sep, *seplast;
Static optlist *oapfirst, *oap, *oaplast;
Static double randDelila, horiginal, hcurrent, hpass;
Static long numseq, window, shuffle, numopt, globshift, iseedparam, iseed2,
	    iseedsave, notchanged, intolerance;
Static boolean paired;

Static Char alignmenttype;
/*
*/
Static long theline;






Static line *freeline;
Static dnastring *freedna;

Static boolean readnumber;
/*
*/
Static long number;
Static boolean numbered, skipunnum;


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






Static boolean isblankDelila(c_)
Char c_;
{
  return (c_ == ' ' || c_ == '\t');
}


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




Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
  clearstring(ribbon);
  ribbon->next = NULL;
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
  t_->state_ = 0;
  t_->skip = false;
  t_->found = false;
}


Static Void testfortrigger(ch, t_)
Char ch;
trigger *t_;
{
  /*









*/
  t_->state_++;
  /*



*/
  if (t_->seek.letters[t_->state_ - 1] == ch) {
    t_->skip = false;
    if (t_->state_ == t_->seek.length)
      t_->found = true;
    else
      t_->found = false;
    return;
  }
  /*
*/
  if (t_->seek.letters[0] == ch) {
    t_->state_ = 1;
    t_->skip = false;
    t_->found = false;
    return;
  }
  t_->state_ = 0;
  t_->skip = true;
  t_->found = false;
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
/* p2c: malign.p, line 1714: Note:
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
/* p2c: malign.p, line 1761: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  printf(" piece name: %.*s\n", WITH->length, WITH->letters);
  printf(" piece number: %ld\n", number);
  printf(" aligned base is too far away... see the code\n");
  halt();
}

#undef maximumrange
#undef semicolon






Static Void flfill(h, dh, ddh, n)
double *h, *dh, *ddh;
long n;
{
  /*
*/
  double freq, nreal, ln2 = log(2.0);
  long i;

  h[0] = 0.0;
  nreal = n;
  for (i = 1; i <= n; i++) {
    freq = i / nreal;
    h[i] = -(freq * log(freq) / ln2);
    dh[i-1] = h[i] - h[i-1];
    if (i > 1)
      ddh[i-2] = h[i] - h[i-2];
  }
}


#define pow18           262144L
#define pow19           524288L
#define pow30           1073741824L

#define pow31m          2147483647.0



Static Void randomDelila(randDelila, iseed)
double *randDelila;
long *iseed;
{
  /*











*/
  long i, nrep;

  nrep = ((*iseed) & 7) + 4;
  for (i = 1; i <= nrep; i++) {
    if (((*iseed) & 1) == (((*iseed) & (pow19 - 1)) >= pow18))
      *iseed /= 2;
    else
      *iseed = *iseed / 2 + pow30;
  }

  *randDelila = *iseed / pow31m;
}

#undef pow18
#undef pow19
#undef pow30
#undef pow31m



Static Void randbase(nuc, iseed)
base *nuc;
long *iseed;
{
  randomDelila(&randDelila, iseed);
  if (randDelila < 0.25) {
    *nuc = a;
    return;
  }
  if (randDelila < 0.5) {
    *nuc = c;
    return;
  }
  if (randDelila < 0.75)
    *nuc = g;
  else
    *nuc = t;
}



Static Void readparam(pin)
_TEXT *pin;
{
  if (*pin->name != '\0') {
    if (pin->f != NULL)
      pin->f = freopen(pin->name, "r", pin->f);
    else
      pin->f = fopen(pin->name, "r");
  } else
    rewind(pin->f);
  if (pin->f == NULL)
    _EscIO2(FileNotFound, pin->name);
  RESETBUF(pin->f, Char);
  fscanf(pin->f, "%ld%ld%*[^\n]", &winleft, &winright);
  getc(pin->f);
  fscanf(pin->f, "%ld%ld%*[^\n]", &shiftmin, &shiftmax);
  getc(pin->f);
  fscanf(pin->f, "%ld%*[^\n]", &iseedparam);
  getc(pin->f);
  fscanf(pin->f, "%ld%*[^\n]", &nranseq);
  getc(pin->f);
  fscanf(pin->f, "%ld%*[^\n]", &nshuffle);
  getc(pin->f);
  fscanf(pin->f, "%ld%*[^\n]", &ifpaired);
  getc(pin->f);
  fscanf(pin->f, "%ld%*[^\n]", &standout);
  getc(pin->f);
  fscanf(pin->f, "%ld%*[^\n]", &npassout);
  getc(pin->f);
  fscanf(pin->f, "%ld%*[^\n]", &nshiftout);
  getc(pin->f);
  fscanf(pin->f, "%lg%*[^\n]", &tolerance);
  getc(pin->f);
  fscanf(pin->f, "%ld%*[^\n]", &ntolpass);
  getc(pin->f);

  if (!BUFEOF(pin->f)) {
    fscanf(pin->f, "%c%*[^\n]", &alignmenttype);
    getc(pin->f);
    if (alignmenttype == '\n')
      alignmenttype = ' ';
    if (alignmenttype != 'b' && alignmenttype != 'i' && alignmenttype != 'f') {
      printf("alignment type must be f, b, or i\n");
      halt();
    }
  } else
    alignmenttype = 'i';

  window = winright - winleft + 1;

  if (window <= 0 || window >= maxlen) {
    printf("parameter error: ");
    printf("window is%12ld, must be between 1 and%12ld\n",
	   window, maxlen - 1L);
    halt();
  }
  if (shiftmin > 0 || shiftmax < 0) {
    printf("parameter error: ");
    printf("shiftmin must be < or = 0, shiftmax must be > or = 0\n");
    halt();
  }
  iseed = iseedparam;
  iseed2 = iseed;
  if (iseed <= 0) {
    printf("parameter error: ");
    printf("random seed must be > 0\n");
    halt();
  }
  paired = (ifpaired > 0);
  if (tolerance > 0.0 && ntolpass > 0)
    return;
  printf("parameter error: ");
  printf("tolerance and # of passes below tolerance must be > 0\n");
  halt();
}



Static Void writeparam(fout)
_TEXT *fout;
{
  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);
  fprintf(fout->f, "* malign%5.2f\n", version);
  fprintf(fout->f, "*\n");
  if (*book.name != '\0') {
    if (book.f != NULL)
      book.f = freopen(book.name, "r", book.f);
    else
      book.f = fopen(book.name, "r");
  } else
    rewind(book.f);
  if (book.f == NULL)
    _EscIO2(FileNotFound, book.name);
  RESETBUF(book.f, Char);
  copyaline(&book, fout);
  if (*book.name != '\0') {
    if (book.f != NULL)
      book.f = freopen(book.name, "r", book.f);
    else
      book.f = fopen(book.name, "r");
  } else
    rewind(book.f);
  if (book.f == NULL)
    _EscIO2(FileNotFound, book.name);
  RESETBUF(book.f, Char);
  fprintf(fout->f, "*\n");
  fprintf(fout->f, "* user specified parameters:\n");
  fprintf(fout->f,
    "* %5ld%5ld   window for calculating uncertainty, relative to aligned base\n",
    winleft, winright);
  fprintf(fout->f, "* %5ld%5ld   minimum and maximum shift of aligned base\n",
	  shiftmin, shiftmax);
  fprintf(fout->f, "* %12ld   random seed\n", iseedparam);
  fprintf(fout->f,
	  "* %6ld   number of random sequences (0 for sequences in book)\n",
	  nranseq);
  fprintf(fout->f, "* %6ld   number of runs after random shuffle\n", nshuffle);
  fprintf(fout->f,
    "* %6ld   independent sequences (0), or pairs of complementary sequences (1)\n",
    ifpaired);
  fprintf(fout->f,
	  "* %6ld   no (-1), limited (0) or full (1) standard output\n",
	  standout);
  fprintf(fout->f, "* %6ld   period at which to output H and alignment\n",
	  npassout);
  fprintf(fout->f, "* %6ld   period at which to output L and H(L)\n",
	  nshiftout);
  fprintf(fout->f, "* %10.6f   tolerance in change of H\n", tolerance);
  fprintf(fout->f, "* %6ld   maximum number of passes below tolerance\n",
	  ntolpass);
  fprintf(fout->f, "*\n");
}



Static Void readaligned(inst, book)
_TEXT *inst, *book;
{
  piece *pie;
  long commonshift, length, alignedbase, almax, i, FORLIM;

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
  pie = (piece *)Malloc(sizeof(piece));
  numseq = 0;

  while (!BUFEOF(book->f)) {
    switch (alignmenttype) {

    case 'i':
      align(inst, book, &theline, &pie, &length, &alignedbase);
      break;

    case 'b':
    case 'f':
      getpiece(book, &theline, &pie);
      length = piecelength(pie);
      alignedbase = -inttopie(1L, pie);
      break;
    }

    if (BUFEOF(book->f))
      break;
    numseq++;
    if (numseq > maxnseq) {
      printf("procedure readaligned error:\n");
      printf("number of sequences greater than limit of maxnseq = %ld\n",
	     (long)maxnseq);
      halt();
    }
    if (length > maxlen) {
      printf("procedure readaligned error:\n");
      printf("sequence %ld, length %ld, is longer than limit of maxlen = %ld bases\n",
	     numseq, length, (long)maxlen);
      halt();
    }
    if (dnamax < maxlen) {
      printf("dnamax=%ld must be greater than or equal to maxlen=%ld\n",
	     (long)dnamax, (long)maxlen);
      halt();
    }
    sep = (seqrec *)Malloc(sizeof(seqrec));
    if (numseq == 1)
      sepfirst = sep;
    else
      seplast->next = sep;

    for (i = 0; i < length; i++)
      sep->alseq[i] = (base)P_getbits_UB(pie->dna->part, i, 1, 3);
    sep->length = length;


    sep->piecename.length = pie->key.hea.keynam.length;
    FORLIM = sep->piecename.length;
    for (i = 0; i < FORLIM; i++)
      sep->piecename.letters[i] = pie->key.hea.keynam.letters[i];

    sep->alignread = alignedbase;
    sep->alignstart = alignedbase;
    sep->aligncurrent = alignedbase;
    sep->alignmin = alignedbase + shiftmin;
    sep->alignmax = alignedbase + shiftmax;

    if (alignmenttype != 'b') {
      almax = length - winright;
      /*




*/


      if (sep->alignmin <= -winleft)
	sep->alignmin = 1 - winleft;


      almax = length - winright;
      if (sep->alignmax > almax)
	sep->alignmax = almax;
    }


    if (sep->alignmax < alignedbase || sep->alignmin > alignedbase) {
      printf("Procedure readaligned error:\n");
      printf("In sequence %ld, the aligned base is too close to one end:\n",
	     numseq);
/* p2c: malign.p, line 2018: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
      printf("piece %.*s\n",
	     pie->key.hea.keynam.length, pie->key.hea.keynam.letters);

      printf("Parameters from malignp:\n");
      printf("  alignedbase = %ld\n", alignedbase);
      printf("  shiftmin = %4ld\n", shiftmin);
      printf("  shiftmax = %4ld\n", shiftmax);
      printf("  winleft  = %4ld\n", winleft);
      printf("  winright = %4ld\n", winright);

      if (sep->alignmax < alignedbase)
	printf("sep^.alignmax < alignedbase\n");

      if (sep->alignmin > alignedbase)
	printf("sep^.alignmin > alignedbase\n");

      printf("sep^.alignmin = alignedbase + shiftmin = %ld\n", sep->alignmin);
      printf("sep^.alignmax = alignedbase + shiftmax = %ld\n", sep->alignmax);

      if (sep->alignmin != alignedbase + shiftmin) {
	printf("EXCEPT that sep^.alignmin < (1-winleft)\n");
	printf("SO sep^.alignmin = 1-winleft\n");
	printf("WHERE winleft = %ld\n", winleft);
      }
      halt();
    }


    if (paired && !(numseq & 1)) {
      /*
*/
      commonshift = seplast->alignread - seplast->alignmin;

      if (commonshift > sep->alignmax - alignedbase)
	commonshift = sep->alignmax - alignedbase;

      seplast->alignmin = seplast->alignread - commonshift;
      sep->alignmax = alignedbase + commonshift;

      /*
*/
      commonshift = seplast->alignmax - seplast->alignread;

      if (commonshift > alignedbase - sep->alignmin)
	commonshift = alignedbase - sep->alignmin;

      seplast->alignmax = seplast->alignread + commonshift;
      sep->alignmin = alignedbase - commonshift;
    }

    seplast = sep;
  }

  sep->next = NULL;

  if (numseq == 0) {
    printf("no sequences found in the book\n");
    halt();
  }

  if (paired && (numseq & 1)) {
    printf("Since the sequences should be paired\n");
    printf("(malignp parameter ifpaired is %ld),\n", ifpaired);
    printf("there should be an even number of them.\n");
    printf("However, there are %ld, an odd number.\n", numseq);
    halt();
  }


  Free(pie);
}



Static Void genrandseqs()
{
  long seqno, i, len, FORLIM;

  numseq = nranseq;
  len = winright - winleft + shiftmax - shiftmin + 1;
  FORLIM = numseq;
  for (seqno = 1; seqno <= FORLIM; seqno++) {
    sep = (seqrec *)Malloc(sizeof(seqrec));
    if (seqno == 1)
      sepfirst = sep;
    else
      seplast->next = sep;
    for (i = 0; i < len; i++)
      randbase(&sep->alseq[i], &iseed);
    sep->length = len;
    sep->alignmin = 1 - winleft;
    sep->alignread = sep->alignmin - shiftmin;
    sep->alignmax = sep->alignread + shiftmax;
    sep->alignstart = sep->alignread;
    sep->aligncurrent = sep->alignread;
    seplast = sep;
  }
  sep->next = NULL;
}



Static Void fillnbl()
{
  base b;
  long i, seqno, offset, FORLIM, FORLIM1;

  sep = sepfirst;
  for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
    FORLIM1 = window;
    for (i = 0; i < FORLIM1; i++)
      nbl[(long)b][i] = 0;
  }
  FORLIM1 = numseq;
  for (seqno = 1; seqno <= FORLIM1; seqno++) {
    offset = sep->aligncurrent + winleft - 1;
    FORLIM = window;
    /*
*/
    for (i = 0; i < FORLIM; i++) {
      b = sep->alseq[i + offset];
      nbl[(long)b][i]++;
    }
    sep = sep->next;
  }
}



Static Void hcalc(nbl, flogf, hl, htot, window)
long (*nbl)[maxlen];
double *flogf;
double *hl;
double *htot;
long window;
{
  long i;
  base b;
  double temp;

  *htot = 0.0;
  for (i = 0; i < window; i++) {
    temp = 0.0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      temp += flogf[nbl[(long)b][i]];
    *htot += temp;
    hl[i] = temp;
  }
}



Static Void changenbl(sep, winlen, increment)
seqrec **sep;
long winlen, increment;
{
  /*
*/
  long i, offset;
  base b;
  seqrec *WITH;

  WITH = *sep;
  offset = WITH->aligncurrent + winleft - 1;
  for (i = 0; i < winlen; i++) {
    b = WITH->alseq[i + offset];
    nbl[(long)b][i] += increment;
  }
}



Static Void shiftseq()
{
  /*
*/
  long alatmin[maxlen];
  double dh, dhcur;
  double dhmin = 1.0e10;
  long numbermin, alcur, i, offset;
  seqrec *WITH;
  long FORLIM, FORLIM1;

  WITH = sep;

  changenbl(&sep, window, -1L);
  FORLIM = WITH->alignmax;


  for (alcur = WITH->alignmin; alcur <= FORLIM; alcur++) {
    dh = 0.0;
    offset = alcur + winleft - 1;

    FORLIM1 = window;
    /*


*/
    for (i = 0; i < FORLIM1; i++)
      dh += fldiff[nbl[(long)WITH->alseq[i + offset]][i]];

    /*
*/
    if (alcur == WITH->aligncurrent)
      dhcur = dh;

    if (dh <= dhmin) {
      if (dh < dhmin)
	numbermin = 0;
      dhmin = dh;
      numbermin++;
      alatmin[numbermin-1] = alcur;
    }
  }
  if (numbermin > 1) {
    randomDelila(&randDelila, &iseed2);
    /*
*/
    numbermin = (long)(0.99999 + numbermin * randDelila);
    if (numbermin == 0)
      numbermin = 1;
    /*

*/
  }
  if (alatmin[numbermin-1] == WITH->aligncurrent)
    notchanged++;
  else {
    notchanged = 0;
    hcurrent += dhmin - dhcur;
  }
  WITH->aligncurrent = alatmin[numbermin-1];


  changenbl(&sep, window, 1L);
}



Static Void doubleshift()
{
  /*
*/
  long alatmin[maxlen];
  double dh, dhcol, dhcur;
  double dhmin = 1.0e10;
  long numbermin, alcur, halfwindow, offsetnex, i, offset;
  base b, bn;
  seqrec *nxp;
  long FORLIM;

  halfwindow = (window + 1) / 2;
  nxp = sep->next;



  changenbl(&sep, halfwindow, -1L);
  changenbl(&nxp, halfwindow, -1L);
  FORLIM = sep->alignmax;


  for (alcur = sep->alignmin; alcur <= FORLIM; alcur++) {
    dh = 0.0;
    offset = alcur + winleft - 1;
    offsetnex = nxp->aligncurrent + sep->aligncurrent - alcur + winleft - 1;
    for (i = 0; i < halfwindow; i++) {
      b = sep->alseq[i + offset];
      bn = nxp->alseq[i + offsetnex];
      /*
*/
      if (b == bn)
	dhcol = fldoubl[nbl[(long)b][i]];
      else
	dhcol = fldiff[nbl[(long)b][i]] + fldiff[nbl[(long)bn][i]];
      dh += dhcol;
    }

    /*
*/
    if (window & 1)
      dh += dh - dhcol;
    else
      dh += dh;

    if (alcur == sep->aligncurrent)
      dhcur = dh;
    if (dh <= dhmin) {
      if (dh < dhmin)
	numbermin = 0;
      dhmin = dh;
      numbermin++;
      alatmin[numbermin-1] = alcur;
    }
  }
  if (numbermin > 1) {
    randomDelila(&randDelila, &iseed2);
    /*
*/
    numbermin = (long)(0.99999 + numbermin * randDelila);
    if (numbermin == 0)
      numbermin = 1;
    /*

*/
  }
  if (alatmin[numbermin-1] == sep->aligncurrent)
    notchanged += 2;
  else {
    notchanged = 0;
    hcurrent += dhmin - dhcur;
  }
  sep->aligncurrent = alatmin[numbermin-1];
  nxp->aligncurrent = nxp->alignread + sep->alignread - sep->aligncurrent;


  changenbl(&sep, halfwindow, 1L);
  changenbl(&nxp, halfwindow, 1L);
}



Static Void hloutput(fout, npass, shiftno)
_TEXT *fout;
long npass, shiftno;
{
  /*
*/
  long i, FORLIM;

  FORLIM = window;
  for (i = 0; i < FORLIM; i++) {
    fprintf(fout->f, " %5ld %5ld", npass, shiftno);

    fprintf(fout->f, " %3ld %10.6f\n", winleft + i, hl[i]);
  }

}



Static Void alignoutput(fout, npass)
_TEXT *fout;
long npass;
{
  long seqno, FORLIM;

  sep = sepfirst;
  fprintf(fout->f, "pass%6ld,  H =%12.7f    relative alignments:\n",
	  npass, hcurrent);
  FORLIM = numseq;
  for (seqno = 1; seqno <= FORLIM; seqno++) {
    fprintf(fout->f, "%4ld", sep->aligncurrent - sep->alignread);
    sep = sep->next;
    if (seqno % 20 == 0)
      putc('\n', fout->f);
  }
  if (numseq % 20 != 0)
    putc('\n', fout->f);
}



Static Void albestout(fout, oap, numperline, field)
_TEXT *fout;
optlist *oap;
long numperline, field;
{
  long seqno, FORLIM;

  FORLIM = numseq;
  for (seqno = 1; seqno <= FORLIM; seqno++) {
    fprintf(fout->f, "%*ld", (int)field, oap->albest[seqno-1]);
    if (seqno % numperline == 0)
      putc('\n', fout->f);
  }
  if (numseq % numperline != 0)
    putc('\n', fout->f);
}


/*
*/
Local Char sign(i)
long i;
{
  if (i >= 0)
    return '+';
  else
    return '-';
}



Static Void instoutput(fout, oap)
_TEXT *fout;
optlist *oap;
{
  long seqno, i;
  long thesign = 1;
  long FORLIM, FORLIM1;

  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);
  fprintf(fout->f, "title \"malign %4.2f\";\n", version);

  fprintf(fout->f, "(* begin module numbering *)\n");
  fprintf(fout->f, "(* number only the pieces, starting at 1 *)\n");
  fprintf(fout->f, "default numbering piece;\n");
  fprintf(fout->f, "default numbering 1;\n");
  fprintf(fout->f, "default out-of-range reduce-range;\n");
  fprintf(fout->f, "(* end module numbering *)\n");

  fprintf(fout->f, "(* data on this alignment:\n");
  fprintf(fout->f, "The H value: %*.*f\n", infofield, infodecim, oap->hbest);
  fprintf(fout->f, "Number of times achieved: %ld\n", oap->num);
  fprintf(fout->f, "*)\n\n");

  fprintf(fout->f, "organism org;\n");
  fprintf(fout->f, "chromosome chr;\n");

  sep = sepfirst;
  FORLIM = numseq;

  for (seqno = 1; seqno <= FORLIM; seqno++) {
    fprintf(fout->f, "piece ");
    FORLIM1 = sep->piecename.length;
    for (i = 0; i < FORLIM1; i++)
      putc(sep->piecename.letters[i], fout->f);
    sep = sep->next;
    putc(';', fout->f);
    fprintf(fout->f, " (* piece %ld *)\n", seqno);

    /*


*/

    if (paired && !(seqno & 1))
      thesign = -1;
    else
      thesign = 1;

    fprintf(fout->f, "get from%10ld %c%ld to same %c%ld;\n",
	    oap->albest[seqno-1], sign(thesign * winleft), labs(winleft),
	    sign(thesign * winright), labs(winright));

    /*


*/

  }
}



Static Void optalignout(fout)
_TEXT *fout;
{
  /*
*/
  long optno, seqno, FORLIM, FORLIM1;

  writeparam(fout);
  fprintf(fout->f,
	  "* relative aligned bases for the set of optimal alignments\n\n");
  fprintf(fout->f, "%5ld sequences,%5ld optimal alignments in%6ld runs\n",
	  numseq, numopt, shuffle);
  fprintf(fout->f, "%12.7f = H for original alignment\n", horiginal);
  oap = oapfirst;
  FORLIM = numopt;
  for (optno = 1; optno <= FORLIM; optno++) {
    fprintf(fout->f,
	    "\n%4ld)%6ld occurrences, H =%12.7f, relative aligned bases:\n",
	    optno, oap->num, oap->hbest);
    sep = sepfirst;

    FORLIM1 = numseq;
    /*
*/
    for (seqno = 1; seqno <= FORLIM1; seqno++) {
      fprintf(fout->f, "%4ld", oap->albest[seqno-1] - sep->alignread);
      if (seqno % 20 == 0)
	putc('\n', fout->f);
      sep = sep->next;
    }
    if (numseq % 20 != 0)
      putc('\n', fout->f);
    oap = oap->next;
  }
}



Static Void optinstout(fout)
_TEXT *fout;
{
  /*
*/
  long seqno, FORLIM;

  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);
  fprintf(fout->f, "%12ld %12ld\n", numseq, numopt);
  oap = oapfirst;
  do {
    fprintf(fout->f, "%5ld %12.7f\n", oap->num, oap->hbest);
    FORLIM = numseq;
    for (seqno = 1; seqno <= FORLIM; seqno++) {
      fprintf(fout->f, " %8ld", oap->albest[seqno-1]);
      if (seqno % 10 == 0)
	putc('\n', fout->f);
    }
    if (numseq % 10 != 0)
      putc('\n', fout->f);
    oap = oap->next;
  } while (oap != NULL);
}



Static Void optoutput()
{
  /*

*/
  long seqno;
  piece *pie;
  long length, alignedbase, FORLIM;

  optalignout(&optalign);
  if (*book.name != '\0') {
    if (book.f != NULL)
      book.f = freopen(book.name, "r", book.f);
    else
      book.f = fopen(book.name, "r");
  } else
    rewind(book.f);
  if (book.f == NULL)
    _EscIO2(FileNotFound, book.name);
  RESETBUF(book.f, Char);
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
  pie = (piece *)Malloc(sizeof(piece));
  FORLIM = numseq;
  for (seqno = 0; seqno < FORLIM; seqno++) {
    switch (alignmenttype) {

    case 'i':
      align(&inst, &book, &theline, &pie, &length, &alignedbase);
      break;

    case 'b':
    case 'f':
      getpiece(&book, &theline, &pie);
      length = piecelength(pie);
      alignedbase = -inttopie(1L, pie);
      break;
    }

    oap = oapfirst;
    /*
*/
    do {
      oap->albest[seqno] = inttopie(oap->albest[seqno], pie);
      oap = oap->next;
    } while (oap != NULL);
  }

  optinstout(&optinst);
  /*

*/
}


#define maxsize         200
#define accuracy        10000



Static Void calehnb(n, gna, gnc, gng, gnt, hg, ehnb, varhnb)
long n, gna, gnc, gng, gnt;
double *hg, *ehnb, *varhnb;
{
  /*









*/
  /*
*/

  double log2Delila = log(2.0);
  double logn, nlog2;

  long gn;
  double logpa, logpc, logpg, logpt;

  /*
*/
  double logfact[maxsize + 1];

  /*
*/
  double mplog2p[maxsize + 1];

  long i;
  double logi;

  long na;
  long nc = 0, ng = 0, nt = 0;
  boolean done = false;

  double pnb;
  /*
*/
  double hnb, pnbhnb;
  double sshnb = 0.0;


  double total = 0.0;
  /*
*/
  long counter = 0;

  /*
*/


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

  /*
*/
  logfact[0] = 0.0;
  mplog2p[0] = 0.0;
  for (i = 1; i <= n; i++) {
    logi = log((double)i);
    logfact[i] = logfact[i-1] + logi;
    mplog2p[i] = i * (logn - logi) / nlog2;
  }


  na = n;

  /*

























*/

  do {
    /*









*/

    pnb = exp(logfact[n] - logfact[na] - logfact[nc] - logfact[ng] -
	      logfact[nt] + na * logpa + nc * logpc + ng * logpg + nt * logpt);

    hnb = mplog2p[na] + mplog2p[nc] + mplog2p[ng] + mplog2p[nt];

    pnbhnb = pnb * hnb;

    *ehnb += pnbhnb;

    sshnb += pnbhnb * hnb;

    /*
*/
    counter++;
    /*







*/
    total += pnb;

    /*



*/
    if (nt > 0) {
      if (ng > 0) {
	ng--;
	nt++;
      } else if (nc > 0) {
	/*
*/
	nc--;
	ng = nt + 1;
	nt = 0;
      } else if (na > 0) {
	/*
*/
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

  /*
*/
  if (accuracy != (long)floor(accuracy * total + 0.5)) {
    printf(" procedure calehnb: the sum of probabilities is\n");
    printf(" not accurate to one part in %ld\n", (long)accuracy);
    printf(" the sum of the probabilities is %10.8f\n", total);
  }

  /*

*/
  if (counter == (long)floor((n + 1.0) * (n + 2) * (n + 3) / 6 + 0.5))
    return;
  /*


*/
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
  /*











*/
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



Static Void xyoutput(malignxyin)
_TEXT *malignxyin;
{
  double Hbefore;
  long rank = 0;

  double Ehnb;
  long gna = 1000, gnc = 1000, gng = 1000, gnt = 1000;
  double hg, varhnb;

  if (*malignxyin->name != '\0') {
    if (malignxyin->f != NULL)
      malignxyin->f = freopen(malignxyin->name, "w", malignxyin->f);
    else
      malignxyin->f = fopen(malignxyin->name, "w");
  } else {
    if (malignxyin->f != NULL)
      rewind(malignxyin->f);
    else
      malignxyin->f = tmpfile();
  }
  if (malignxyin->f == NULL)
    _EscIO2(FileNotFound, malignxyin->name);
  SETUPBUF(malignxyin->f, Char);
  fprintf(malignxyin->f, "* malign%5.2f\n", version);
  fprintf(malignxyin->f, "* columns definitions:\n");
  fprintf(malignxyin->f,
	  "* 1: rank:  from 1 to the number of alignment classes\n");
  fprintf(malignxyin->f,
	  "* 2: occurrences:  how many times the class was found\n");
  fprintf(malignxyin->f,
	  "* 3: H: the uncertainty of the alignment, in bits\n");
  fprintf(malignxyin->f,
	  "* 4: R: the information content of the alignment, in bits\n");
  oap = oapfirst;


  if (numseq <= kickover)
    calehnb(numseq, gna, gnc, gng, gnt, &hg, &Ehnb, &varhnb);
  else
    calaehnb(numseq, gna, gnc, gng, gnt, &hg, &Ehnb, &varhnb);

  Hbefore = Ehnb * (winright - winleft + 1);

  do {
    rank++;
    fprintf(malignxyin->f, " %10ld", rank);
    fprintf(malignxyin->f, " %10ld", oap->num);
    fprintf(malignxyin->f, " %*.*f", infofield, infodecim, oap->hbest);
    fprintf(malignxyin->f, " %*.*f\n",
	    infofield, infodecim, Hbefore - oap->hbest);
    oap = oap->next;
  } while (oap != NULL);
}



Static Void realign()
{
  /*

*/
  long alhist[maxchange];
  long histmax = 0, binmax = -1;
  long bin, alshift;
  long alshiftmin = -10000, alshiftmax = 10000;

  for (bin = 0; bin < maxchange; bin++)
    alhist[bin] = 0;
  sep = sepfirst;
  do {
    bin = maxchange / 2 + sep->alignread - sep->aligncurrent;
    if (bin > 0 && bin <= maxchange)
      alhist[bin-1]++;
    alshift = sep->alignmin - sep->aligncurrent;
    if (alshift > alshiftmin)
      alshiftmin = alshift;
    alshift = sep->alignmax - sep->aligncurrent;
    if (alshift < alshiftmax)
      alshiftmax = alshift;
    sep = sep->next;
    /*


*/
  } while (sep != NULL);
  for (bin = 1; bin <= maxchange; bin++) {
    if (alhist[bin-1] > histmax) {
      histmax = alhist[bin-1];
      binmax = bin;
    }
  }



  if (binmax <= 0)
    return;
  if (alhist[binmax-1] <= minmaxforchange)
    return;
  globshift = binmax - maxchange / 2;
  if (globshift > alshiftmax)
    globshift = alshiftmax;
  if (globshift < alshiftmin)
    globshift = alshiftmin;
  sep = sepfirst;
  while (globshift != 0 && sep != NULL) {
    sep->aligncurrent += globshift;
    sep = sep->next;
  }
}



Static Void findbestalignment()
{
  long shiftno;
  long npass = 0;
  double htemp;
  boolean ifhlout, dohlout;

  ifhlout = (shuffle > nshuffle && nshiftout >= 0);

  if (ifhlout) {
    fprintf(uncert.f,
      "* Position, L, and uncertainty, H(L), as the alignment improves\n");
    fprintf(uncert.f, "*\n");
    fprintf(uncert.f, "* %12ld positions\n", window);
    fprintf(uncert.f, "* pass number, shift number, position L, H(L)\n");
    hloutput(&uncert, 0L, 0L);
  }

  if (npassout >= 0) {
    fprintf(newalign.f, "\nrun number:%6ld,  original random seed:%15ld\n",
	    shuffle, iseedsave);
    alignoutput(&newalign, 0L);
  }
  notchanged = 0;
  intolerance = 0;
  do {
    npass++;
    hpass = hcurrent;
    sep = sepfirst;
    shiftno = 0;
    do {
      shiftno++;
      if (paired) {
	if (shiftno & 1)
	  doubleshift();
      } else
	shiftseq();

      sep = sep->next;

      /*
*/
      if (ifhlout) {
	dohlout = (shiftno == numseq);
	if (nshiftout > 0)
	  dohlout = (dohlout || shiftno % nshiftout == 0);
	if (dohlout) {
	  if (paired)
	    fillnbl();
	  hcalc(nbl, flogf, hl, &htemp, window);
	  hloutput(&uncert, npass, shiftno);
	}
      }
    } while (sep != NULL && (notchanged <= numseq || globshift != 0));

    /*
*/
    if (hpass - hcurrent < tolerance)
      intolerance++;
    else
      intolerance = 0;

    /*
*/
    if (!paired && npass < realignlimit)
      realign();
    fillnbl();
    hcalc(nbl, flogf, hl, &hcurrent, window);
    if (standout > 0)
      printf("%6ld%6ld%13.7f\n", shuffle, npass, hcurrent);
    if (npassout > 0) {
      if (npass % npassout == 0)
	alignoutput(&newalign, npass);
    }
  } while (intolerance <= ntolpass && notchanged < numseq);
  if (standout == 0)
    printf("%6ld%6ld%13.7f\n", shuffle, npass, hcurrent);

  /*






*/
  if (npassout == 0) {
    alignoutput(&newalign, npass);
    return;
  }
  if (npassout > 0) {
    if (npass % npassout != 0) {
      alignoutput(&newalign, npass);

    }
  }
}



Static Void randomalign()
{
  long altrunc, alrange;

  sep = sepfirst;
  do {
    randomDelila(&randDelila, &iseed);
    alrange = sep->alignmax - sep->alignmin;
    altrunc = (long)(randDelila * (alrange + 1));
    if (altrunc > alrange)
      altrunc = alrange;
    sep->alignstart = sep->alignmin + altrunc;
    sep->aligncurrent = sep->alignstart;
    sep = sep->next;
    if (paired) {
      sep->alignstart = sep->alignmax - altrunc;
      sep->aligncurrent = sep->alignstart;
      sep = sep->next;
    }
  } while (sep != NULL);
}



Static Void optcheckadd()
{
  /*
*/
  boolean match = false, termloop = false;
  long seqno, FORLIM;

  oap = oapfirst;
  oaplast = NULL;

  /*
*/
  if (oap != NULL) {
    if (hcurrent >= oap->hbest) {
      do {
	if (hcurrent == oap->hbest) {
	  match = true;
	  sep = sepfirst;
	  seqno = 1;


	  while (match && seqno <= numseq) {
	    if (oap->albest[seqno-1] != sep->aligncurrent)
	      match = false;
	    seqno++;
	    sep = sep->next;
	  }


	  if (match)
	    oap->num++;
	}
	oaplast = oap;
	oap = oap->next;
	if (oap == NULL)
	  termloop = true;
	else {
	  if (hcurrent < oap->hbest)
	    termloop = true;
	}
      } while (!(termloop || match));
    }
  }
  if (match)
    return;
  oap = (optlist *)Malloc(sizeof(optlist));

  /*
*/
  if (oaplast == NULL) {
    oap->next = oapfirst;
    oapfirst = oap;
  } else {
    /*
*/
    oap->next = oaplast->next;
    oaplast->next = oap;
  }
  numopt++;
  oap->num = 1;
  oap->hbest = hcurrent;
  sep = sepfirst;
  FORLIM = numseq;
  for (seqno = 0; seqno < FORLIM; seqno++) {
    oap->albest[seqno] = sep->aligncurrent;
    sep = sep->next;
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  malignxyin.f = NULL;
  strcpy(malignxyin.name, "malignxyin");
  optinst.f = NULL;
  strcpy(optinst.name, "optinst");
  optalign.f = NULL;
  strcpy(optalign.name, "optalign");
  newalign.f = NULL;
  strcpy(newalign.name, "newalign");
  uncert.f = NULL;
  strcpy(uncert.name, "uncert");
  malignp.f = NULL;
  strcpy(malignp.name, "malignp");
  book.f = NULL;
  strcpy(book.name, "book");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  printf("malign%5.2f\n", version);
  if (*uncert.name != '\0')
    uncert.f = fopen(uncert.name, "w");
  else
    uncert.f = tmpfile();
  if (uncert.f == NULL)
    _EscIO2(FileNotFound, uncert.name);
  SETUPBUF(uncert.f, Char);
  readparam(&malignp);

  if (nranseq == 0)
    readaligned(&inst, &book);
  else
    genrandseqs();

  flfill(flogf, fldiff, fldoubl, numseq);

  if (npassout >= 0) {
    writeparam(&newalign);
    fprintf(newalign.f,
      "* relative alignments at start, end of each run and during run if npassout>0\n");
  }

  flfill(flogf, fldiff, fldoubl, numseq);
  if (standout >= 0)
    printf("   run  pass  uncertainty\n");
  shuffle = 0;
  oapfirst = NULL;
  numopt = 0;
  iseedsave = iseed;

  flfill(flogf, fldiff, fldoubl, numseq);
  do {
    flfill(flogf, fldiff, fldoubl, numseq);
    shuffle++;
    fillnbl();
    hcalc(nbl, flogf, hl, &hcurrent, window);
    if (shuffle == 1)
      horiginal = hcurrent;
    findbestalignment();
    optcheckadd();
    iseedsave = iseed;
    randomalign();
  } while (shuffle <= nshuffle);

  optoutput();
  xyoutput(&malignxyin);
_L1:
  if (inst.f != NULL)
    fclose(inst.f);
  if (book.f != NULL)
    fclose(book.f);
  if (malignp.f != NULL)
    fclose(malignp.f);
  if (uncert.f != NULL)
    fclose(uncert.f);
  if (newalign.f != NULL)
    fclose(newalign.f);
  if (optalign.f != NULL)
    fclose(optalign.f);
  if (optinst.f != NULL)
    fclose(optinst.f);
  if (malignxyin.f != NULL)
    fclose(malignxyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */