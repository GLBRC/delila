/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "orf.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.35
/*




































*/
#define updateversion   1.00



/*















































































































































*/


#define globaldebug     false

#define superdebug      false
/*
*/



#define dnamax          1024
#define namelength      100
#define linelength      200



#define maxstring       2000




#define minribl         (-10001)
#define maxribl         10001
#define defnegativeinfinity  (-1000)
/*
*/





#define defbasesperline  30
#define defaastate      1
#define defframeallowed  7
#define defcodelength   3
#define defseqlines     1

#define defpageaction   'c'

#define defpagelength   55

#define defhopi         "HOPI"
#define defnkfc         "NKFC"
#define defndaf         "NDAF"
#define defsof          "SOF"

#define prime           '\''

#define shiftright      3
/*
*/
#define tabcharacter    9
#define maxdatacontrol  4
#define maxsof          3

/*
*/



#define defaultllx      7.10999
#define defaultlly      7.01995
#define defaulturx      588.15
#define defaultury      784.98

/*





*/

/*





*/


#define pwid            8
#define pdec            5
#define pdecolor        4
/*
*/

#define defcharacterratio  0.6
/*


*/

#define tolerateratio   10000
/*

*/

#define linemax         1000

#define tenspacemark    '*'
#define fivespacemark   '.'
/*
*/




#define infofield       10
#define infodecim       6

#define nfield          8





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

/* p2c: orf.p, line 346:
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




typedef Char aminoacid[3];



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;



typedef struct waveparam {

  Char extreme;
  double wavelocation, wavebit, waveamplitude, wavelength;
  /*




*/
  double dashon;
  /*
*/
  double dashoff, dashoffset, thickness;
  struct waveparam *next;
} waveparam;




/*


*/
typedef double rblarray[(long)t - (long)a + 1][maxribl - minribl + 1];

typedef struct ribltype {
  stringDelila riblname, *riblheader;
  rblarray data;
  long numbers[(long)t - (long)a + 1][maxribl - minribl + 1];
  long frombase, tobase;
  double mean, stdev, consensus, anticonsensus, averageRi;
  long n;
  Char symmetry;
  waveparam *waves;
  double cmperbase, cmperbit, Ribound, Zbound, Pbound;
} ribltype;

/*
*/






typedef struct petaltype {

  long el;
  Char c_;

  double eh, es, eb;

  double fh, fs, fb;

  double spare;
} petaltype;

typedef struct definetype {
  /*

*/
  stringDelila nametag, background, negparts, posparts;
  double locations[maxstring];
  /*
*/


  long marks;
  double min, max;
  long definitionnumber;
  ribltype *matrix;
  /*



*/
  double RiboundFeature, ZboundFeature, PboundFeature;
  petaltype petal;
  boolean isgap;
  /*
*/
  boolean istotal;
  /*
*/
  boolean ismulti;
  /*
*/
  struct definetype *next;
} definetype;


typedef struct featurecarrier {
  /*
*/
  struct featuretype *afeature;
  struct featurecarrier *next;
} featurecarrier;

typedef struct featuretype {
  /*




*/
  stringDelila id;
  long featurenumber;
  double coordinate;
  long orientation;
  stringDelila nametag, othertag;

  definetype *definition;

  /*
*/
  double Ri, Z, probability;

  /*

*/
  Char Tparam;
  /*
*/
  double Aparam, Bparam, Cparam, Dparam, relcoo;
  /*

















*/

  double evencoordinate;
  /*

*/


  boolean unsatisfied;
  /*
*/
  double fromrange, torange;
  long desiredline;
  /*

*/
  /*







*/
  featurecarrier *others, *mothers;

  /*
*/
  struct featuretype *duplicateprevious;
  /*



*/

  boolean usedInGroup;

  struct featuretype *next;
} featuretype;



Static _TEXT genomebook, scanfeatures, orfp, orffeatures;


Static long groupnumber;







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



Static Void copystring(a_, b)
stringDelila a_, *b;
{
  /*
*/
  long l;

  b->length = a_.length;
  for (l = 0; l < a_.length; l++)
    b->letters[l] = a_.letters[l];
  b->current = a_.current;
  b->next = a_.next;
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
  /*



*/
  emptystring(ribbon);
  ribbon->previous = NULL;
  ribbon->next = NULL;
}


Static Void disposestring(ribbon)
stringDelila **ribbon;
{
  stringDelila *t_;

  t_ = *ribbon;

  while (t_->next != NULL)
    t_ = t_->next;

  while (t_->previous != NULL) {
    t_ = t_->previous;
    Free(t_->next);
  }
  Free(t_);
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


#define tabcharacter_   9



Static Void detabstring(s)
stringDelila *s;
{
  /*

*/
  Char c_;
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++) {
    c_ = s->letters[i];
    /*


*/
    if (c_ == tabcharacter_)
      c_ = ' ';
    s->letters[i] = c_;
  }
}

#undef tabcharacter_



Static boolean equalstring(a_, b)
stringDelila a_, b;
{
  /*





*/
  long index;
  boolean equal;

  if (a_.length == b.length) {
    index = 1;
    do {
      equal = (a_.letters[index-1] == b.letters[index-1]);
      index++;
    } while (equal && index <= a_.length);
    return equal;
  } else
    return false;
}



Static Void grabtoken(thefile, thestring)
_TEXT *thefile;
stringDelila *thestring;
{
  Char c_;
  boolean done = false;

  skipblanks(thefile);
  thestring->length = 0;
  while (!done) {
    if (P_eoln(thefile->f)) {
      done = true;
      break;
    }
    c_ = getc(thefile->f);
    if (c_ == '\n')
      c_ = ' ';
    if (c_ == ' ')
      done = true;
    else {
      thestring->length++;
      thestring->letters[thestring->length - 1] = c_;
    }
  }
}



Static Void readstringline(afile, buffer)
_TEXT *afile;
stringDelila *buffer;
{
  long index = 0;

  clearstring(buffer);
  while (!P_eoln(afile->f) && index < maxstring) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
  }
  if (!P_eoln(afile->f)) {
    printf("readstringline: a line exceeds maximum string size (%ld)\n",
	   (long)maxstring);
    halt();
  }
  buffer->length = index;
  buffer->current = 1;
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
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



Static Void writeNOTABstring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  /*
*/
  stringDelila notabs;

  copystring(*s, &notabs);
  detabstring(&notabs);
  writestring(tofile, &notabs);
}



Static Void writequotestring(afile, s)
_TEXT *afile;
stringDelila s;
{
  /*


*/
  detabstring(&s);
  putc('"', afile->f);
  writestring(afile, &s);
  putc('"', afile->f);
}



Static Void writestringlines(afile, l)
_TEXT *afile;
stringDelila *l;
{
  stringDelila *h;

  if (l == NULL)
    return;
  h = l;
  while (h != NULL) {
    writestring(afile, h);
    putc('\n', afile->f);
    h = h->next;
  }
}


#define tabcharacter_   9



Static Void readquotestring(afile, s)
_TEXT *afile;
stringDelila *s;
{
  /*
*/
  Char c_;
  long i, FORLIM;

  emptystring(s);
  skipblanks(afile);
  c_ = getc(afile->f);
  if (c_ == '\n')
    c_ = ' ';
  if (c_ != '"') {
    printf("quote string expected but \" missing\n");
    printf("character found instead:\"%c\"\n", c_);
    halt();
  }
  if (P_eoln(afile->f)) {
    printf("missing end of quote string \" string\n");
    halt();
  }
  do {
    c_ = getc(afile->f);
    if (c_ == '\n')
      c_ = ' ';
    if (c_ != '"') {
      s->length++;
      s->letters[s->length - 1] = c_;
    }
  } while (!(((c_ == '"') | P_eoln(afile->f)) || s->length == maxstring));
  if (s->length == maxstring) {
    printf("A quote string exceeded %ld characters.\n", (long)maxstring);
    printf("Use a shorter string or increase constant maxstring.\n");
    halt();
  }
  if (c_ != '"') {
    printf("The second \" mark of a string is missing.\n");
    halt();
  }

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++) {
    if (s->letters[i] == ' ')
      s->letters[i] = (Char)tabcharacter_;
  }
}

#undef tabcharacter_







Static Void readawaveparameter(afile, wp)
_TEXT *afile;
waveparam *wp;
{
  /*

*/

  /*










*/
  fscanf(afile->f, "%c%*[^\n]", &wp->extreme);
  getc(afile->f);
  if (wp->extreme == '\n')
    wp->extreme = ' ';
  fscanf(afile->f, "%lg%*[^\n]", &wp->wavelocation);
  getc(afile->f);
  fscanf(afile->f, "%lg%*[^\n]", &wp->wavebit);
  getc(afile->f);
  fscanf(afile->f, "%lg%*[^\n]", &wp->waveamplitude);
  getc(afile->f);
  fscanf(afile->f, "%lg%*[^\n]", &wp->wavelength);
  getc(afile->f);
  if (wp->wavelength <= 0.0) {
    printf("wave parameters: wavelength must be positive\n");
    halt();
  }
  /*


*/
  if (BUFEOF(afile->f)) {
    wp->dashon = 0.0;
    wp->dashoff = 0.0;
    wp->dashoffset = 0.0;
    wp->thickness = 0.0;
    return;
  }
  /*
*/
  if (P_peek(afile->f) != 'd') {
    fscanf(afile->f, "%lg%*[^\n]", &wp->dashon);
    getc(afile->f);
    wp->dashoff = wp->dashon;
    wp->dashoffset = 0.0;
  } else {
    getc(afile->f);
    /*




*/
    fscanf(afile->f, "%lg%lg%lg%*[^\n]", &wp->dashon, &wp->dashoff,
	   &wp->dashoffset);
    getc(afile->f);

    /*






























*/

  }
  if (BUFEOF(afile->f))
    wp->thickness = 0.0;
  else {
    fscanf(afile->f, "%lg%*[^\n]", &wp->thickness);
    getc(afile->f);
  }
}


/* Local variables for readwaveparameters: */
struct LOC_readwaveparameters {
  _TEXT *afile;
  boolean done;
} ;

Local Void waystoend(LINK)
struct LOC_readwaveparameters *LINK;
{
  boolean clear = false;

  if (BUFEOF(LINK->afile->f))
    LINK->done = true;
  if (!LINK->done) {
    while (!clear && !LINK->done) {
      if (BUFEOF(LINK->afile->f)) {
	LINK->done = true;
	clear = true;
      } else if (P_peek(LINK->afile->f) == '*') {
	fscanf(LINK->afile->f, "%*[^\n]");
	getc(LINK->afile->f);
      } else
	clear = true;
    }
  }
  if (!LINK->done) {
    if (P_eoln(LINK->afile->f))
      LINK->done = true;
  }
  if (LINK->done)
    return;
  if (P_peek(LINK->afile->f) != '.')
    return;
  fscanf(LINK->afile->f, "%*[^\n]");
  getc(LINK->afile->f);
  LINK->done = true;
}


Static Void readwaveparameters(afile_, w)
_TEXT *afile_;
waveparam **w;
{
  /*

*/
  /*

*/
  struct LOC_readwaveparameters V;
  boolean morethanone = false;
  waveparam *p;

  V.afile = afile_;
  V.done = false;
  *w = NULL;
  waystoend(&V);
  if (V.done) {
    *w = NULL;
    return;
  }
  *w = (waveparam *)Malloc(sizeof(waveparam));
  p = *w;
  while (!V.done) {
    waystoend(&V);
    if (V.done) {
      p->next = NULL;
      continue;
    }
    if (P_peek(V.afile->f) == '*') {
      fscanf(V.afile->f, "%*[^\n]");
      getc(V.afile->f);
      continue;
    }
    if (morethanone) {
      p->next = (waveparam *)Malloc(sizeof(waveparam));
      p = p->next;
    }
    /*

*/
    readawaveparameter(V.afile, p);

    morethanone = true;
  }
}



Static Void readheader(afile, ch, theheader)
_TEXT *afile;
Char ch;
stringDelila **theheader;
{
  /*
*/
  stringDelila *h;

  if (BUFEOF(afile->f)) {
    clearstring(*theheader);
    return;
  }
  if (P_peek(afile->f) != ch) {
    *theheader = NULL;
    return;
  }
  *theheader = (stringDelila *)Malloc(sizeof(stringDelila));
  h = *theheader;
  while (P_peek(afile->f) == '*') {
    readstringline(afile, h);
    if (!BUFEOF(afile->f)) {
      if (P_peek(afile->f) == '*') {
	h->next = (stringDelila *)Malloc(sizeof(stringDelila));
	h = h->next;
      } else
	h->next = NULL;
    }
  }
}


#define clearvalue      5.0


/* Local variables for getriblmatrix: */
struct LOC_getriblmatrix {
  _TEXT *afile;
} ;

Local Void testforend(LINK)
struct LOC_getriblmatrix *LINK;
{
  if (BUFEOF(LINK->afile->f)) {
    printf("part or all of the ribl matrix is missing\n");
    halt();
  }
}

Local Void skip(LINK)
struct LOC_getriblmatrix *LINK;
{
  testforend(LINK);
  while (P_peek(LINK->afile->f) == '*') {
    fscanf(LINK->afile->f, "%*[^\n]");
    getc(LINK->afile->f);
    testforend(LINK);
  }
}



Static Void getriblmatrix(afile_, matrix)
_TEXT *afile_;
ribltype **matrix;
{
  /*

*/
  struct LOC_getriblmatrix V;
  /*
*/
  base b;
  long l, thel;

  stringDelila teststring, riblstart;
  ribltype *WITH;
  _TEXT TEMP;
  long FORLIM;

  V.afile = afile_;
  *matrix = (ribltype *)Malloc(sizeof(ribltype));
  WITH = *matrix;


  grabtoken(V.afile, &riblstart);
  teststring.letters[0] = 'r';
  teststring.letters[1] = 'i';
  teststring.letters[2] = 'b';
  teststring.letters[3] = 'l';
  teststring.length = 4;

  if (!equalstring(riblstart, teststring)) {
    printf("The ribl matrix must begin with 'ribl \"name\"'\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, riblstart);
    printf(" seen instead\n");
    halt();
  }

  skipblanks(V.afile);
  readquotestring(V.afile, &(*matrix)->riblname);
  fscanf(V.afile->f, "%*[^\n]");

  getc(V.afile->f);
  readheader(V.afile, '*', &WITH->riblheader);

  fscanf(V.afile->f, "%ld%ld%*[^\n]", &WITH->frombase, &WITH->tobase);
  getc(V.afile->f);

  if (WITH->frombase < minribl) {
    printf("Matrix is too large for program.\n");
    printf("frombase (=%ld)\n", WITH->frombase);
    printf("minribl (=%ld)\n", (long)minribl);
    printf("Decrease minribl or increase frombase\n");
    halt();
  }

  if (WITH->tobase > maxribl) {
    printf("Matrix is too large for program.\n");
    printf("tobase (=%ld)\n", WITH->tobase);
    printf("maxribl (=%ld)\n", (long)maxribl);
    printf("Increase maxribl or decrease tobase\n");
    halt();
  }


  FORLIM = WITH->frombase;
  for (l = minribl; l < FORLIM; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      (*matrix)->data[(long)b - (long)a][l - minribl] = clearvalue;
  }
  for (l = WITH->tobase + 1; l <= maxribl; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      (*matrix)->data[(long)b - (long)a][l - minribl] = clearvalue;
  }

  FORLIM = WITH->tobase;
  for (l = WITH->frombase; l <= FORLIM; l++) {
    testforend(&V);
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fscanf(V.afile->f, "%lg", &(*matrix)->data[(long)b - (long)a]
	     [l - minribl]);
    fscanf(V.afile->f, "%ld", &thel);
    if (thel != l) {
      printf("positions in ribl do not match from-to range\n");
      halt();
    }
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fscanf(V.afile->f, "%ld", &(*matrix)->numbers[(long)b - (long)a]
	     [l - minribl]);
    fscanf(V.afile->f, "%*[^\n]");
    getc(V.afile->f);
  }

  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->mean);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->stdev);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->consensus);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->anticonsensus);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->averageRi);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%ld%*[^\n]", &WITH->n);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%c%*[^\n]", &WITH->symmetry);
  getc(V.afile->f);
  if (WITH->symmetry == '\n')
    WITH->symmetry = ' ';
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->Ribound);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->Zbound);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->Pbound);
  getc(V.afile->f);
  /*



*/
  readwaveparameters(V.afile, &WITH->waves);
}

#undef clearvalue


#define wid             4



Static Void writewaveparameters(afile, w)
_TEXT *afile;
waveparam *w;
{
  waveparam *i = w;
  waveparam *WITH;

  while (i != NULL) {
    WITH = i;
    fprintf(afile->f,
      "%c  extreme: char; h or l, the high or low extreme to be defined\n",
      WITH->extreme);
    fprintf(afile->f,
      "%*.1f  wavelocation: real; the location in bases of the extreme\n",
      wid, WITH->wavelocation);
    fprintf(afile->f,
	    "%*.1f  wavebit: real; the location in bits of the extreme\n",
	    wid, WITH->wavebit);
    fprintf(afile->f,
	    "%*.1f  waveamplitude: real; the amplitude of the wave in bits\n",
	    wid, WITH->waveamplitude);
    fprintf(afile->f,
	    "%*.1f  wavelength: real; the wave length of the wave in bases\n",
	    wid, WITH->wavelength);
    /*







*/
    fprintf(afile->f,
	    "%*.1f %*.1f %*.1f dashon, dashoff, dashoffset (in bases)\n",
	    wid, WITH->dashon, wid, WITH->dashoff, wid, WITH->dashoffset);
    fprintf(afile->f,
      "%*.1f  thickness: real; the thickness of the cosine wave.  <=0 means default\n",
      wid, WITH->thickness);
    i = i->next;
  }
  fprintf(afile->f, ".\n");
}

#undef wid



Static Void putriblmatrix(afile, matrix)
_TEXT *afile;
ribltype *matrix;
{
  long lindex, FORLIM;

  if (matrix == NULL)
    return;
  fprintf(afile->f, "ribl");
  putc(' ', afile->f);
  writequotestring(afile, matrix->riblname);
  putc('\n', afile->f);

  writestringlines(afile, matrix->riblheader);
  fprintf(afile->f, "%*ld %*ld frombase, tobase\n",
	  nfield, matrix->frombase, nfield, matrix->tobase);
  FORLIM = matrix->tobase;
  for (lindex = matrix->frombase; lindex <= FORLIM; lindex++) {
    fprintf(afile->f, "  %*.*f %*.*f %*.*f %*.*f %*ld",
	    infofield, infodecim, matrix->data[0][lindex - minribl],
	    infofield, infodecim, matrix->data[(long)c - (long)a]
	    [lindex - minribl], infofield, infodecim,
	    matrix->data[(long)g - (long)a][lindex - minribl], infofield,
	    infodecim, matrix->data[(long)t - (long)a][lindex - minribl],
	    nfield, lindex);
    fprintf(afile->f, " %*ld %*ld %*ld %*ld\n",
	    nfield, matrix->numbers[0][lindex - minribl], nfield,
	    matrix->numbers[(long)c - (long)a][lindex - minribl], nfield,
	    matrix->numbers[(long)g - (long)a][lindex - minribl], nfield,
	    matrix->numbers[(long)t - (long)a][lindex - minribl]);
  }
  fprintf(afile->f, "*\n");
  fprintf(afile->f, "%*.*f bits = mean (Rsequence of selected region)\n",
	  infofield, infodecim, matrix->mean);
  fprintf(afile->f, "%*.*f bits = standard deviation\n",
	  infofield, infodecim, matrix->stdev);
  fprintf(afile->f, "*\n");
  fprintf(afile->f, "%*.*f bits = Ri of consensus sequence from %ld to %ld\n",
	  infofield, infodecim, matrix->consensus, matrix->frombase,
	  matrix->tobase);
  fprintf(afile->f,
	  "%*.*f bits = Ri of anticonsensus sequence from %ld to %ld\n",
	  infofield, infodecim, matrix->anticonsensus, matrix->frombase,
	  matrix->tobase);
  fprintf(afile->f, "*\n");
  fprintf(afile->f,
	  "%*.*f bits = average Ri for random sequence from %ld to %ld\n",
	  infofield, infodecim, matrix->averageRi, matrix->frombase,
	  matrix->tobase);
  fprintf(afile->f, "*\n");
  fprintf(afile->f, "%ld n, number of sequences used to create the matrix\n",
	  matrix->n);
  fprintf(afile->f, "*\n");
  switch (matrix->symmetry) {

  case 'a':
    fprintf(afile->f, "asymmetric");
    break;

  case 'e':
    fprintf(afile->f, "even");
    break;

  case 'o':
    fprintf(afile->f, "odd");
    break;
  }
  fprintf(afile->f, " symmetry of the matrix\n");
  fprintf(afile->f, "*\n");
  fprintf(afile->f, "%*.*f Ri bound: lower bound on Ri\n",
	  infofield, infodecim, matrix->Ribound);
  fprintf(afile->f, "%*.*f Z bound: lower bound on Z\n",
	  infofield, infodecim, matrix->Zbound);
  fprintf(afile->f, "%*.*f P bound: upper probability\n",
	  infofield, infodecim, matrix->Pbound);
  fprintf(afile->f, "*\n");
  writewaveparameters(afile, matrix->waves);
  putc('\n', afile->f);
}



Static Void locatefeature(aname, coo, featurelist, thefeature, StopAtTotal)
stringDelila aname;
double coo;
featuretype *featurelist, **thefeature;
boolean StopAtTotal;
{

  /*





*/
  boolean done = false;

  *thefeature = featurelist;
  do {
    if (*thefeature == NULL)
      done = true;
    else {
      /*





*/
      if (equalstring((*thefeature)->nametag, aname)) {
	if ((*thefeature)->coordinate == coo)
	  done = true;
	else
	  *thefeature = (*thefeature)->next;
      } else if ((*thefeature)->definition->istotal && StopAtTotal) {

	*thefeature = NULL;
      } else {
	*thefeature = (*thefeature)->next;

      }
    }
  } while (!done);
}




Static Void writeadefinition(afile, d)
_TEXT *afile;
definetype *d;
{
  long i, FORLIM;

  fprintf(afile->f, "define");
  putc(' ', afile->f);
  writequotestring(afile, d->nametag);
  putc(' ', afile->f);
  writequotestring(afile, d->background);
  putc(' ', afile->f);
  writequotestring(afile, d->negparts);
  putc(' ', afile->f);
  writequotestring(afile, d->posparts);
  FORLIM = d->marks;
  for (i = 0; i < FORLIM; i++)
    fprintf(afile->f, " %1.1f", d->locations[i]);
  putc('\n', afile->f);
  /*





*/
}


#define pwid_           8
#define pdec_           5



Static Void showadefinition(afile, d)
_TEXT *afile;
definetype *d;
{
  long i, FORLIM;
  petaltype *WITH1;

  fprintf(afile->f, "DEFINITION: \n");
  fprintf(afile->f, "nametag: ");
  writequotestring(afile, d->nametag);
  fprintf(afile->f, "\nbackground: ");

  writequotestring(afile, d->background);
  fprintf(afile->f, "\nnegative parts: ");

  writequotestring(afile, d->negparts);
  fprintf(afile->f, "\npositive parts: ");

  writequotestring(afile, d->posparts);
  fprintf(afile->f, "\nlocations: ");

  FORLIM = d->marks;
  for (i = 0; i < FORLIM; i++)
    fprintf(afile->f, " %1.1f", d->locations[i]);
  fprintf(afile->f, "\nmarks: %ld\n", d->marks);

  fprintf(afile->f, "min: %5.1f\n", d->min);
  fprintf(afile->f, "max: %5.1f\n", d->max);

  fprintf(afile->f, "petal: ");
  WITH1 = &d->petal;
  fprintf(afile->f, "%ld", WITH1->el);
  fprintf(afile->f, " %c", WITH1->c_);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->eh);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->es);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->eb);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->fh);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->fs);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->fb);
  fprintf(afile->f, " %*.*f\n", pwid_, pdec_, WITH1->spare);

  /*






*/


}

#undef pwid_
#undef pdec_



Static Void showdefinitions(afile, definitionlist)
_TEXT *afile;
definetype *definitionlist;
{
  definetype *f = definitionlist;

  while (f != NULL) {
/* p2c: orf.p: Note: Eliminated unused assignment statement [338] */
    showadefinition(afile, f);
    putc('\n', afile->f);
    f = f->next;
  }
}



Static Void writeafeature(afile, f)
_TEXT *afile;
featuretype *f;
{
  /*




*/
  featurecarrier *o;

  /*
*/
  if (f == NULL) {
    printf("writeafeature: nil feature pointer!\n");
    halt();
  }
  fprintf(afile->f, "@ ");
  writeNOTABstring(afile, &f->id);
  fprintf(afile->f, " %1.1f", f->coordinate);
  putc(' ', afile->f);
  if (f->orientation > 0)
    fprintf(afile->f, "+%ld", f->orientation);
  else
    fprintf(afile->f, "%ld", f->orientation);
  putc(' ', afile->f);
  writequotestring(afile, f->nametag);
  putc(' ', afile->f);
  writequotestring(afile, f->othertag);
  if (f->definition == NULL) {
    fprintf(afile->f, " %*.*f", infofield, infodecim, 0.0);
    fprintf(afile->f, " %*.*f", infofield, infodecim, 0.0);
    fprintf(afile->f, " %*.*f", infofield, infodecim, 0.0);
    fprintf(afile->f, " %3.1f", 0.0);
  }
  /*









*/
  else {
    if (f->definition->matrix != NULL) {
      fprintf(afile->f, " %1.*f", infodecim, f->Ri);
      fprintf(afile->f, " %1.*f", infodecim, f->Z);
      fprintf(afile->f, " %1.*f", infodecim, f->probability);
    }
    /*



*/
    else {
      fprintf(afile->f, " %c", f->Tparam);
      /*




*/
      fprintf(afile->f, " %1.*f", infodecim, f->Aparam);
      fprintf(afile->f, " %1.*f", infodecim, f->Bparam);
      fprintf(afile->f, " %1.*f", infodecim, f->Cparam);
      fprintf(afile->f, " %1.*f", infodecim, f->Dparam);
    }
    fprintf(afile->f, " %1.1f", f->relcoo);
    fprintf(afile->f, " #%ld", f->featurenumber);
  }
  if (f->others == NULL)
    return;
  if (f->others->afeature != NULL)
    fprintf(afile->f, " > ");
  o = f->others;
  while (o != NULL) {
    if (o != f->others)
      fprintf(afile->f, ", ");
    if (o->afeature != NULL) {
      putc('[', afile->f);
      writeNOTABstring(afile, &o->afeature->nametag);
      putc(']', afile->f);
    }
    o = o->next;
  }
}




Static Void showafeature(afile, f)
_TEXT *afile;
featuretype *f;
{
  featurecarrier *o;
  long totals = 0, gaps = 0;

  fprintf(afile->f, "FEATURE #%ld:\n", f->featurenumber);
  fprintf(afile->f, "id: ");
  writequotestring(afile, f->id);
  fprintf(afile->f, "\ncoordinate: %1.1f\n", f->coordinate);


  fprintf(afile->f, "orientation: ");
  if (f->orientation > 0)
    fprintf(afile->f, "+%ld\n", f->orientation);
  else
    fprintf(afile->f, "%ld\n", f->orientation);

  fprintf(afile->f, "nametag: ");
  writequotestring(afile, f->nametag);
  fprintf(afile->f, "\nothertag: ");

  writequotestring(afile, f->othertag);
  fprintf(afile->f, "\nunsatisfied: ");

  if (f->unsatisfied)
    fprintf(afile->f, "true\n");
  else
    fprintf(afile->f, "false\n");

  fprintf(afile->f, "fromrange: %5.1f\n", f->fromrange);
  fprintf(afile->f, "  torange: %5.1f\n", f->torange);
  fprintf(afile->f, "desiredline: %5ld\n", f->desiredline);
  if (f->duplicateprevious == NULL)
    fprintf(afile->f, "NOT a duplicate feature\n");
  else
    fprintf(afile->f, "DUPLICATE of feature #%ld\n",
	    f->duplicateprevious->featurenumber);

  if (f->others != NULL) {
    fprintf(afile->f, "This feature points at these OTHERS:\n");
    fprintf(afile->f, "feature -> ");
    o = f->others;
    while (o != NULL) {
      if (o != f->others)
	fprintf(afile->f, ", ");
      if (o->afeature != NULL) {
	putc('[', afile->f);
	writeNOTABstring(afile, &o->afeature->nametag);
	putc(']', afile->f);
      } else
	fprintf(afile->f, "[EMPTY]");
      o = o->next;
    }
    /*





*/

    putc('\n', afile->f);
    o = f->others;
    while (o != NULL) {
      if (o->afeature != NULL) {
	fprintf(afile->f, "= ");
	writeafeature(afile, o->afeature);
	putc('\n', afile->f);
      }
      o = o->next;
    }
  } else
    fprintf(afile->f, "This feature does not point to any OTHERS.\n");

  if (f->mothers != NULL) {
    fprintf(afile->f,
	    "The MOTHERS of this feature (that point to this feature) are:\n");
    o = f->mothers;
    while (o != NULL) {
      if (o->afeature != NULL) {
	fprintf(afile->f, "feature <- \"");
	writeNOTABstring(afile, &o->afeature->nametag);
	fprintf(afile->f, "\" #%ld\n", o->afeature->featurenumber);
	/*

*/
	if (o->afeature->definition->istotal)
	  totals++;
	if (o->afeature->definition->isgap)
	  gaps++;
      }

      o = o->next;
    }
    o = f->mothers;

    while (o != NULL) {
      if (o->afeature != NULL) {
	fprintf(afile->f, "= ");
	writeafeature(afile, o->afeature);
	putc('\n', afile->f);
      }
      o = o->next;
    }

  } else
    fprintf(afile->f, "This feature has no MOTHERS.\n");


  fprintf(afile->f, "%ld gap", gaps);
  if (gaps != 1)
    putc('s', afile->f);
  fprintf(afile->f, "\n%ld total", totals);
  if (totals != 1)
    putc('s', afile->f);
  putc('\n', afile->f);
  if (totals > 1) {
    printf("showafeature: more than one total!\n");
    halt();
  }
  if (gaps > 1 && totals < 1) {
    printf("showafeature: %ld gaps but no total!\n", gaps);

    halt();
  }
}



Static Void showfeatures(afile, featurelist, paging)
_TEXT *afile;
featuretype *featurelist;
boolean paging;
{
  /*

*/
  featuretype *f = featurelist;

  while (f != NULL) {
/* p2c: orf.p: Note: Eliminated unused assignment statement [338] */
    showafeature(afile, f);
    f = f->next;
    if (f == NULL)
      break;

    if (paging)
      fprintf(afile->f, "\f");
    else
      putc('\n', afile->f);
  }
}


#define tab             9

#define debugging       false


Local Void IsGapOrIsTotal(d)
definetype **d;
{
  /*
*/
  stringDelila totstring, gapstring;
  long deflocation, gaplocation, totlocation;
  boolean done = false;
  _TEXT TEMP;

  if (debugging) {
    printf("IsGapOrIsTotal test of this definition:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeadefinition(&TEMP, *d);
  }

  clearstring(&totstring);

  totstring.letters[0] = (Char)tab;
  totstring.letters[1] = 't';
  totstring.letters[2] = 'o';
  totstring.letters[3] = 't';
  totstring.letters[4] = 'a';
  totstring.letters[5] = 'l';
  totstring.length = 6;
  clearstring(&gapstring);

  /*

*/


  gapstring.letters[0] = (Char)tab;
  gapstring.letters[1] = 'G';
  gapstring.letters[2] = 'a';
  gapstring.letters[3] = 'p';
  gapstring.length = 4;
  (*d)->isgap = true;
  deflocation = (*d)->nametag.length;
  gaplocation = gapstring.length;
  while (!done) {
    /*




*/
    if ((*d)->nametag.letters[deflocation-1] !=
	gapstring.letters[gaplocation-1]) {
      (*d)->isgap = false;
      done = true;
    }
    deflocation--;
    gaplocation--;
    /*


*/
    if (deflocation < 1)
      done = true;
    if (gaplocation < 1)
      done = true;
  }

  /*




















*/


  (*d)->istotal = true;
  done = false;

  deflocation = (*d)->nametag.length;
  while (!done) {
    /*

*/
    if ((*d)->nametag.letters[deflocation-1] == (Char)tab)
      done = true;
    deflocation--;
    if (deflocation < 1) {
      done = true;
      (*d)->istotal = false;
    }
  }
  /*

*/
  if ((*d)->istotal == true) {
    /*

*/
    totlocation = totstring.length;
    done = false;
    /*


*/
    while (!done) {
      /*




*/
      if ((*d)->nametag.letters[deflocation-1] !=
	  totstring.letters[totlocation-1]) {
	(*d)->istotal = false;
	done = true;
      }
      deflocation--;
      totlocation--;
      /*


*/
      if (deflocation < 1)
	done = true;
      if (totlocation < 1)
	done = true;
    }
  }


  if ((*d)->isgap || (*d)->istotal)
    (*d)->ismulti = true;
  else
    (*d)->ismulti = false;

  if (!debugging)
    return;


  TEMP.f = stdout;
  *TEMP.name = '\0';
  /*

*/
  showadefinition(&TEMP, *d);
  /*




*/
  if ((*d)->isgap)
    printf("Is a gap\n");
  else
    printf("Is NOT a gap\n");
  if ((*d)->istotal)
    printf("Is a total\n");
  else
    printf("Is NOT a total\n");
  printf("IsGapOrIsTotal: end &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
}

#undef tab
#undef debugging







Static Void readadefinition(afile, d)
_TEXT *afile;
definetype **d;
{
  long i;
  double location;
  definetype *WITH;
  _TEXT TEMP;
  long FORLIM;


  skipnonblanks(afile);
  skipblanks(afile);
  WITH = *d;
  /*



*/
  readquotestring(afile, &WITH->nametag);


  readquotestring(afile, &WITH->background);
  if (WITH->background.length < 1 || WITH->background.length > 2) {
    printf("Background string must contain only 1 or 2 characters.\n");
    printf("Instead, it contained: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->background);
    putchar('\n');
    halt();
  }

  readquotestring(afile, &WITH->negparts);
  readquotestring(afile, &WITH->posparts);
  if (WITH->negparts.length != WITH->posparts.length) {
    printf("While reading definition of ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->nametag);
    printf("\nthe negative part ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->negparts);
    printf(" and\n");
    printf("the positive part ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->posparts);
    printf(" are not the same length.\n");
    printf("(%ld<>%ld)\n", WITH->negparts.length, WITH->posparts.length);
    halt();
  }

  WITH->marks = WITH->negparts.length;
  if (WITH->marks == 0)
    WITH->marks = 2;

  FORLIM = WITH->marks;

  for (i = 0; i < FORLIM; i++)
    fscanf(afile->f, "%lg", &WITH->locations[i]);

  WITH->matrix = NULL;
  WITH->RiboundFeature = -LONG_MAX;
  WITH->ZboundFeature = LONG_MAX;
  WITH->PboundFeature = 0.0;

  WITH->ismulti = false;

  WITH->next = NULL;




  WITH->min = LONG_MAX;
  WITH->max = -LONG_MAX;
  FORLIM = WITH->marks;
  for (i = 0; i < FORLIM; i++) {
    location = WITH->locations[i];
    if (location < WITH->min)
      WITH->min = location;
    if (location > WITH->max)
      WITH->max = location;
  }
  if (WITH->min == LONG_MAX) {
    printf("readadefinition: program error at min\n");
    halt();
  }
  if (WITH->max == -LONG_MAX) {
    printf("readadefinition: program error at max\n");
    halt();
  }


  IsGapOrIsTotal(d);

}



Static Void cleardefinition(d)
definetype **d;
{
  long l;
  definetype *WITH;
  petaltype *WITH1;

  WITH = *d;
  clearstring(&WITH->nametag);
  clearstring(&WITH->background);
  clearstring(&WITH->negparts);
  clearstring(&WITH->posparts);
  for (l = 0; l < maxstring; l++)
    WITH->locations[l] = 0.0;
  WITH->marks = 0;
  WITH->min = 0.0;
  WITH->max = 0.0;
  WITH->definitionnumber = 0;
  WITH->matrix = NULL;
  WITH->RiboundFeature = 0.0;
  WITH->ZboundFeature = 0.0;
  WITH->PboundFeature = 0.0;

  WITH1 = &WITH->petal;

  WITH1->el = 0;
  WITH1->c_ = ' ';
  WITH1->eh = 0.0;
  WITH1->es = 0.0;
  WITH1->eb = 0.0;
  WITH1->fh = 0.0;
  WITH1->fs = 0.0;
  WITH1->fb = 0.0;
  WITH1->spare = 0.0;
  WITH->isgap = false;
  WITH->istotal = false;
  WITH->ismulti = false;
  WITH->next = NULL;
}



Static boolean equaldefinition(a_, b)
definetype *a_, *b;
{
  boolean same = true;
  long i, FORLIM;

  if (!equalstring(a_->nametag, b->nametag))
    return false;
  if (!equalstring(a_->background, b->background))
    return false;
  if (!equalstring(a_->negparts, b->negparts))
    return false;

  if (a_->min != b->min)
    return false;
  if (a_->max != b->max)
    return false;
  /*

*/
  if (a_->marks != b->marks)
    return true;
  FORLIM = a_->marks;
  for (i = 0; i < FORLIM; i++) {
    if (a_->locations[i] != b->locations[i]) {
      same = false;
/* p2c: orf.p: Note: Eliminated unused assignment statement [338] */
/* p2c: orf.p: Note: Eliminated unused assignment statement [338] */
/* p2c: orf.p: Note: Eliminated unused assignment statement [338] */
    }
  }
/* p2c: orf.p: Note: Eliminated unused assignment statement [338] */
/* p2c: orf.p: Note: Eliminated unused assignment statement [338] */
  return same;
  /*






*/
}



Static Void clearfeature(f)
featuretype **f;
{
  featuretype *WITH;

  WITH = *f;
  clearstring(&WITH->id);
  WITH->coordinate = 0.0;
  WITH->orientation = 1;
  clearstring(&WITH->nametag);
  clearstring(&WITH->othertag);
  WITH->definition = NULL;
  WITH->duplicateprevious = NULL;
  WITH->Ri = 0.0;
  WITH->Z = 0.0;
  WITH->probability = 0.0;
  WITH->Tparam = ' ';
  WITH->Aparam = 0.0;
  WITH->Bparam = 0.0;
  WITH->Cparam = 0.0;
  WITH->Dparam = 0.0;
  WITH->relcoo = 0.0;
  WITH->evencoordinate = 0.0;
  WITH->unsatisfied = true;
  WITH->fromrange = 0.0;
  WITH->torange = 0.0;
  WITH->featurenumber = 0;
  WITH->desiredline = 0;
  WITH->others = NULL;
  WITH->mothers = NULL;
  WITH->usedInGroup = false;
  WITH->next = NULL;
}


/* Local variables for readafeature: */
struct LOC_readafeature {
  featuretype **f;
} ;

Local Void reportandhalt(LINK)
struct LOC_readafeature *LINK;
{
  _TEXT TEMP;

  printf("Feature details:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeafeature(&TEMP, *LINK->f);
  putchar('\n');
  halt();
}



Static Void readafeature(afile, definitionlist, f_)
_TEXT *afile;
definetype *definitionlist;
featuretype **f_;
{
  /*




*/
  struct LOC_readafeature V;
  definetype *d;
  featuretype *WITH;
  _TEXT TEMP;

  V.f = f_;
  printf("readafeature BUBBA\n");
  WITH = *V.f;
  skipnonblanks(afile);
  skipblanks(afile);
  grabtoken(afile, &WITH->id);
  fscanf(afile->f, "%lg", &WITH->coordinate);
  fscanf(afile->f, "%ld", &WITH->orientation);
  readquotestring(afile, &WITH->nametag);
  readquotestring(afile, &(*V.f)->othertag);


  if (WITH->orientation != -1 && WITH->orientation != 1) {
    printf("while reading feature id ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->id);
    printf("\nfeature orientation must be -1 or +1\n");
    printf("It is %ld\n", WITH->orientation);
    reportandhalt(&V);
  }



  d = definitionlist;
  while (d != NULL) {
    if (equalstring(d->nametag, (*V.f)->nametag))
      WITH->definition = d;
    d = d->next;
  }
  if (WITH->definition == NULL) {
    printf("No definition was found for feature ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->nametag);
    putchar('\n');
    reportandhalt(&V);
  }
  d = WITH->definition;


  if (d->matrix != NULL) {
    fscanf(afile->f, "%lg%lg%lg", &WITH->Ri, &WITH->Z, &WITH->probability);
    if (!P_eoln(afile->f))
      fscanf(afile->f, "%lg", &WITH->relcoo);
    else
      WITH->relcoo = 0.0;
  } else {
    WITH->Ri = -LONG_MAX;
    WITH->Z = LONG_MAX;
    WITH->probability = 1.0;


    /*

*/

    skipblanks(afile);
    if (P_eoln(afile->f)) {
      WITH->Tparam = ' ';
      WITH->Aparam = 0.0;
      WITH->Bparam = 0.0;
      WITH->Cparam = 0.0;
      WITH->Dparam = 0.0;
      WITH->relcoo = 0.0;
    } else {
      if (P_peek(afile->f) == '9' || P_peek(afile->f) == '8' ||
	  P_peek(afile->f) == '7' || P_peek(afile->f) == '6' ||
	  P_peek(afile->f) == '5' || P_peek(afile->f) == '4' ||
	  P_peek(afile->f) == '3' || P_peek(afile->f) == '2' ||
	  P_peek(afile->f) == '1' || P_peek(afile->f) == '0' ||
	  P_peek(afile->f) == '+' || P_peek(afile->f) == '-') {
	fscanf(afile->f, "%lg", &WITH->Aparam);
	fscanf(afile->f, "%lg", &WITH->Bparam);
	fscanf(afile->f, "%lg", &WITH->Cparam);
	WITH->Dparam = 0.0;
      }

      else {
	WITH->Tparam = getc(afile->f);
	if (WITH->Tparam == '\n')
	  WITH->Tparam = ' ';
	fscanf(afile->f, "%lg", &WITH->Aparam);
	fscanf(afile->f, "%lg", &WITH->Bparam);
	fscanf(afile->f, "%lg", &WITH->Cparam);
	fscanf(afile->f, "%lg", &WITH->Dparam);
      }
      if (!P_eoln(afile->f))
	fscanf(afile->f, "%lg", &WITH->relcoo);
      else
	WITH->relcoo = 0.0;
    }
  }


  /*



*/

  WITH->unsatisfied = true;
  WITH->fromrange = WITH->definition->min;
  WITH->torange = WITH->definition->max;

  /*

*/
  WITH->desiredline = 0;
  WITH->next = NULL;

  /*


*/
  WITH->others = NULL;
  WITH->mothers = NULL;

  printf("BUBBA HERE in readafeature\n");
}


/* Local variables for featureinlist: */
struct LOC_featureinlist {
  featuretype *newfeature, *f;
  boolean found;
} ;

Local Void checkorpos(LINK)
struct LOC_featureinlist *LINK;
{
  /*

*/
  LINK->found = (LINK->newfeature->orientation == LINK->f->orientation &&
		 LINK->newfeature->coordinate == LINK->f->coordinate);
}




Static boolean featureinlist(featurelist, newfeature_, oldfeature)
featuretype *featurelist, *newfeature_, **oldfeature;
{
  /*

*/
  struct LOC_featureinlist V;
  /*


*/
  boolean done = false;

  V.newfeature = newfeature_;
  /*







*/

  V.found = false;
  V.f = featurelist;
  *oldfeature = NULL;
  /*



*/
  while (!done) {
    /*



*/

    if (equalstring(V.newfeature->id, V.f->id) &
	equalstring(V.newfeature->nametag, V.f->nametag)) {
      if (V.f->definition == NULL)
	checkorpos(&V);
      else if (V.f->definition->matrix == NULL)
	checkorpos(&V);
      else {
	V.found = (V.newfeature->Ri == V.f->Ri &&
		   ((V.f->definition->matrix->symmetry == 'o' &&
		     V.newfeature->coordinate == V.f->coordinate) ||
		    (V.f->definition->matrix->symmetry == 'a' &&
		     V.newfeature->orientation == V.f->orientation &&
		     V.newfeature->coordinate == V.f->coordinate) ||
		    (V.f->definition->matrix->symmetry == 'e' &&
		     1 == fabs(V.newfeature->coordinate - V.f->coordinate) &&
		     V.newfeature->orientation != V.f->orientation) ||
		    (V.f->definition->matrix->symmetry == 'e' &&
		     V.newfeature->coordinate == V.f->coordinate &&
		     V.newfeature->orientation == V.f->orientation)));

	/*
*/
	/*




*/









	/*


*/







      }
    }

    if (V.found) {
      *oldfeature = V.f;
      done = true;
      /*









*/
    } else if (V.f->next == NULL)
      done = true;
    else
      V.f = V.f->next;
  }

  /*



*/
  return V.found;
}



Static Void plural(thefile, number)
_TEXT *thefile;
long number;
{
  if (number != 1)
    putc('s', thefile->f);
}



Static Void locatedefinition(aname, definitionlist, thedefinition)
stringDelila aname;
definetype *definitionlist, **thedefinition;
{
  /*
*/
  boolean done = false;

  *thedefinition = definitionlist;
  do {
    if (*thedefinition == NULL)
      done = true;
    else {
      if (equalstring((*thedefinition)->nametag, aname))
	done = true;
      else {
	*thedefinition = (*thedefinition)->next;

      }
    }
  } while (!done);
}


#define debugSplitstring  true


#define countmarkdebug  false


/* Local variables for readdefinitionsandfeatures: */
struct LOC_readdefinitionsandfeatures {
  definetype **definitionlist;
  featuretype **featurelist, *f;


  /*



*/
  boolean ingroup;
  /*
*/
  featuretype *groupfeature, *aGapFeature, *groupTotalFeature;
  featurecarrier *groupGapCarrier, *groupTotalCarrier;
  featuretype *leftgap, *rightgap, *ff;
  /*
*/
  featuretype *gg;
  /*
*/
  stringDelila *nameparts, *anamepart, leftstring, gapstring, rightstring;
  long gap;
  Char aletter;
} ;

Local Void yell(LINK)
struct LOC_readdefinitionsandfeatures *LINK;
{
  printf("ERROR in reading definitions and features\n");
}

Local Void barout(LINK)
struct LOC_readdefinitionsandfeatures *LINK;
{
  printf("**********************************************************************\n");
}

Local Void splitstring(s, break_, terminator, parts, LINK)
stringDelila s;
Char break_, terminator;
stringDelila **parts;
struct LOC_readdefinitionsandfeatures *LINK;
{
  /*








*/
  long l = 1;
  boolean done = false;
  stringDelila *spointer;
  _TEXT TEMP;

  if (superdebug) {
    if (debugSplitstring) {
      printf("IN splitstring the string to split is:\n");
      putchar('"');
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &s);
      printf("\"\n");
      printf("======> break character is: \"%c\"\n", break_);
    }
  }
  disposestring(parts);
  *parts = (stringDelila *)Malloc(sizeof(stringDelila));
  spointer = *parts;
  clearstring(spointer);
  while (!done) {
    if (s.letters[l-1] == terminator)
      done = true;
    else {
      if (s.letters[l-1] == break_) {
	if (superdebug) {
	  if (debugSplitstring) {
	    printf("debugSplitstring: \"");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writestring(&TEMP, spointer);
	    printf("\"\n");
	  }
	}
	spointer->next = (stringDelila *)Malloc(sizeof(stringDelila));
	spointer = spointer->next;
	clearstring(spointer);
      } else {
	spointer->length++;
	spointer->letters[spointer->length - 1] = s.letters[l-1];
	if (superdebug) {
	  if (debugSplitstring) {
	    if (s.letters[l-1] == ' ')
	      printf(
		"debugSplitstring: splitstring write a space $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	    if (s.letters[l-1] == '\t')
	      printf(
		"debugSplitstring: splitstring write a TAB  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	    if (false)
	      printf("WRITING: ord(%c)=%d\n", s.letters[l-1], s.letters[l-1]);
	  }
	}
      }
      l++;
      if (l > maxstring)
	done = true;
    }
    if (l == maxstring) {
      printf("splitstring: maxed out! maxstring = l = %ld\n", l);
      halt();
    }
  }
  if (!superdebug)
    return;
  if (!debugSplitstring)
    return;
  printf("debugSplitstring: splitstring result: \"");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, spointer);
  printf("\"\n");
  printf("OUT splitstring\n");
}

#undef debugSplitstring

/* Local variables for threadgroup: */
struct LOC_threadgroup {
  struct LOC_readdefinitionsandfeatures *LINK;
} ;

/* Local variables for countmarks: */
struct LOC_countmarks {
  struct LOC_threadgroup *LINK;
  featuretype *f;
  long requiredcount;
} ;

Local Void countmarksprint(LINK)
struct LOC_countmarks *LINK;
{
  _TEXT TEMP;

  printf("++++++++ countmarks ++++++++");
  if (LINK->requiredcount < 0)
    printf(" final check\n");
  else
    printf(" looking for %ld marks\n", LINK->requiredcount);
  printf(
    "++++++++ Counting marks for this total feature in group number: %ld FAILED: \n",
    groupnumber);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  showadefinition(&TEMP, LINK->f->definition);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  showafeature(&TEMP, LINK->f);
  printf("%ld marks are in this feature\n", LINK->f->definition->marks);

  if (true)
    return;

  printf("\n******* countmarks readdefinitionsandfeatures: *******\n");
  printf("******* DEFINITIONS:\n");
  if (false) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showdefinitions(&TEMP, *LINK->LINK->LINK->definitionlist);
  }
  printf("\n******* FEATURES:\n");
  if (true) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showfeatures(&TEMP, *LINK->LINK->LINK->featurelist, true);
  }
  printf("\n******* countmarks readdefinitionsandfeatures: *******\n");
  /*

*/
}


Local Void countmarks(f_, requiredcount_, LINK)
featuretype *f_;
long requiredcount_;
struct LOC_threadgroup *LINK;
{
  /*





*/
  struct LOC_countmarks V;
  long expectedcount, observedcount;
  featurecarrier *otherptr;
  _TEXT TEMP;

  V.LINK = LINK;
  V.f = f_;
  V.requiredcount = requiredcount_;
  if (countmarkdebug)
    printf("+++++++++++++++++++++ BEGIN countmarks\n");
  if (V.f->definition->istotal) {
    if (V.f->others != NULL) {
      otherptr = V.f->others;
      observedcount = 0;
      while (otherptr != NULL) {
	if (countmarkdebug) {
	  putchar('\n');
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writeafeature(&TEMP, otherptr->afeature);
	  printf("\nloop observedcount=%ld\n", observedcount);
	  printf("isgap:   %s\n",
		 otherptr->afeature->definition->isgap ? " TRUE" : "FALSE");
	  printf("istotal: %s\n",
		 otherptr->afeature->definition->istotal ? " TRUE" : "FALSE");
	}
	if (!otherptr->afeature->definition->isgap &&
	    !otherptr->afeature->definition->istotal)
	  observedcount++;
	otherptr = otherptr->next;
      }
      if (countmarkdebug)
	printf("**** final observedcount=%ld\n", observedcount);

      if (V.requiredcount == -1)
	expectedcount = V.f->definition->marks;
      else
	expectedcount = V.requiredcount;
      if (observedcount != expectedcount) {
	countmarksprint(&V);
	printf("countmarks: expected: %ld observed: %ld\n",
	       expectedcount, observedcount);
	printf(
	  "**** Wrong number of marks found  in linked list for this total feature. ****\n");
	halt();
      }

    } else {
      if (countmarkdebug)
	printf("countmarks no others\n");
    }
  } else {
    printf("countmarks feature is not a total, cannot count marks\n");
    halt();
  }
  if (countmarkdebug)
    printf("+++++++++++++++++++++ END countmarks\n");
}

#undef countmarkdebug


Local Void addmotherlinks(agap, thefeature, LINK)
featuretype **agap, **thefeature;
struct LOC_threadgroup *LINK;
{
  /*
*/
  featurecarrier *mom, *previousMom;
  /*
*/
  featurecarrier *gapthing;
  boolean done = false;
  _TEXT TEMP;

  /*





*/
  if ((*thefeature)->mothers == NULL) {
    if (false) {
      printf("************ ALL FEATURES: *************\n");
      printf("**** (also listed in the list file) ****\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      showfeatures(&TEMP, *LINK->LINK->featurelist, false);
      printf("****************************************\n");
      printf("**** addmotherlinks: no mothers found for the feature:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      showafeature(&TEMP, *thefeature);
      /*


*/
    }


    /*
*/
    if (!((*thefeature)->definition->isgap ||
	  (*thefeature)->definition->istotal))
      halt();
    return;
  }
  mom = (*thefeature)->mothers;
  previousMom = mom;
  while (!done) {
    if (mom->afeature == *thefeature) {
      done = true;
      break;
    }
    if (mom->afeature->definition->istotal) {
      gapthing = (featurecarrier *)Malloc(sizeof(featurecarrier));
      if (mom == previousMom) {
	gapthing->next = (*thefeature)->mothers;
	(*thefeature)->mothers = gapthing;
      } else {
	previousMom->next = gapthing;
	gapthing->next = mom;
      }
      gapthing->afeature = *agap;
      done = true;
      if (mom->next != NULL) {
	printf("addmotherlinks: total points to something!\n");
	halt();
      }

      /*


*/
      continue;
    }
    /*
*/

    previousMom = mom;
    mom = mom->next;
    if (mom == NULL) {
      printf("addmotherlinks: failed to find total\n");
      halt();
    }
  }
  /*


*/
  /*

*/
}

Local Void threadgroup(LINK)
struct LOC_readdefinitionsandfeatures *LINK;
{
  /*
*/
  struct LOC_threadgroup V;
  long m, FORLIM;
  _TEXT TEMP;

  V.LINK = LINK;
  if (!LINK->ingroup)
    return;
  if (LINK->groupfeature == NULL) {
    LINK->groupfeature = LINK->f;

    return;
  }
  /*


*/

  if (!LINK->f->definition->istotal)
    return;
  /*



*/
  LINK->groupTotalFeature = LINK->f;
  /*







*/



  /*



*/
  /*






*/

  /*
*/
  LINK->groupTotalCarrier = NULL;


  /*



*/
  splitstring(LINK->groupTotalFeature->nametag, '-', '\t', &LINK->nameparts,
	      LINK);


  /*



*/

  LINK->anamepart = LINK->nameparts;
  FORLIM = LINK->groupTotalFeature->definition->marks;
  for (m = 1; m <= FORLIM; m++) {
    LINK->gg = LINK->groupfeature;

    while (LINK->gg != NULL) {
      /*














*/

      /*

*/
      if (equalstring(*LINK->anamepart, LINK->gg->nametag)) {
	if (LINK->gg->definition->isgap || LINK->gg->definition->istotal) {
	  printf("*** PROGRAM SHOULD NEVER BE HERE!\n");
	  printf("*** Total found matching gap or total\n");
	  halt();
	}


	else {
	  if (LINK->groupTotalCarrier == NULL) {
	    LINK->groupTotalFeature->others =
	      (featurecarrier *)Malloc(sizeof(featurecarrier));
	    LINK->groupTotalCarrier = LINK->groupTotalFeature->others;
	    LINK->groupTotalCarrier->next = NULL;
	  }
	  /*

*/
	  else {
	    LINK->groupTotalCarrier->next = (featurecarrier *)
					    Malloc(sizeof(featurecarrier));
	    LINK->groupTotalCarrier = LINK->groupTotalCarrier->next;
	    LINK->groupTotalCarrier->next = NULL;
	  }
	  /*

*/
	  LINK->groupTotalCarrier->afeature = LINK->gg;


	  if (LINK->gg->mothers == NULL) {
	    LINK->gg->mothers = (featurecarrier *)Malloc(sizeof(featurecarrier));
	    LINK->gg->mothers->next = NULL;
	    LINK->gg->mothers->afeature = LINK->groupTotalFeature;


	  }

	  countmarks(LINK->groupTotalFeature, m, &V);
	}
      }
      /*

*/

      LINK->gg = LINK->gg->next;
    }




    LINK->anamepart = LINK->anamepart->next;
  }
  /*



*/
  /*

*/


  if (LINK->anamepart != NULL) {
    printf("total names not all used\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showafeature(&TEMP, LINK->f);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showadefinition(&TEMP, LINK->f->definition);
    halt();
  }
  countmarks(LINK->groupTotalFeature, -1L, &V);


  /*



*/

  /*

*/
  LINK->gg = LINK->groupfeature;
  while (!LINK->gg->definition->istotal && LINK->gg != NULL) {
    if (LINK->gg->definition->isgap) {
      if (superdebug) {
	printf("BUBBA gg^.number=%ld ++++++++++++++++++++\n",
	       LINK->gg->featurenumber);
	printf("BUBBA gg is: \n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, LINK->gg);
	putchar('\n');
      }

      splitstring(LINK->gg->nametag, '-', '\t', &LINK->nameparts, LINK);
      /*



*/

      /*









*/

      LINK->leftstring = *LINK->nameparts;
      LINK->gapstring = *LINK->nameparts->next;

      LINK->rightstring = *LINK->nameparts->next->next;


      LINK->gap = 0;
      if (LINK->gapstring.letters[0] != '(' &&
	  LINK->gapstring.letters[LINK->gapstring.length - 1] != ')') {
	printf("Gap string found without surrounding \"()\"\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showafeature(&TEMP, LINK->gg);
	halt();
      } else {
	LINK->gapstring.current = 2;
	while (LINK->gapstring.current < LINK->gapstring.length) {
	  LINK->aletter = LINK->gapstring.letters[LINK->gapstring.current - 1];
	  if (LINK->aletter != '9' && LINK->aletter != '8' &&
	      LINK->aletter != '7' && LINK->aletter != '6' &&
	      LINK->aletter != '5' && LINK->aletter != '4' &&
	      LINK->aletter != '3' && LINK->aletter != '2' &&
	      LINK->aletter != '1' && LINK->aletter != '0') {
	    printf("Gap contains non-digit: \"%c\"\n", LINK->aletter);
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    showafeature(&TEMP, LINK->gg);
	    halt();
	  }
	  LINK->gap = LINK->gap * 10 + LINK->aletter - '0';
	  LINK->gapstring.current++;
	}
      }

      if (superdebug) {
	printf("BUBBA gap is %ld\n", LINK->gap);
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &LINK->leftstring);
	printf("<-  leftstring BUBBA\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &LINK->rightstring);
	printf("<- rightstring BUBBA\n");
      }
      if (LINK->gap != LINK->gg->torange - LINK->gg->fromrange) {
	printf("Gap in nametag does not match range:\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showafeature(&TEMP, LINK->gg);
	halt();
      }
      LINK->aGapFeature = LINK->gg;



      LINK->leftgap = NULL;
      LINK->rightgap = NULL;
      LINK->ff = LINK->groupfeature;
      while (LINK->ff != LINK->groupTotalFeature &&
	     (LINK->leftgap == NULL || LINK->rightgap == NULL)) {
	if (!(LINK->ff->definition->isgap || LINK->ff->definition->istotal)) {
	  if ((long)floor(LINK->ff->relcoo + 0.5) ==
	      LINK->aGapFeature->fromrange) {
	    if (equalstring(LINK->leftstring, LINK->ff->nametag))
	      LINK->leftgap = LINK->ff;

	  }

	  if ((long)floor(LINK->ff->relcoo + 0.5) == LINK->aGapFeature->torange) {
	    if (equalstring(LINK->rightstring, LINK->ff->nametag))
	      LINK->rightgap = LINK->ff;


	  }

	}

	LINK->ff = LINK->ff->next;

      }

      if (superdebug) {
	printf("BUBBA leftgap: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, LINK->leftgap);
	printf("\nBUBBA rightgap: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, LINK->rightgap);
	putchar('\n');
      }


      if (LINK->leftgap == NULL || LINK->rightgap == NULL) {
	printf("gap ends not found:\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showafeature(&TEMP, LINK->aGapFeature);
	halt();
      }
      if (LINK->leftgap->mothers == NULL) {
	printf("ERROR: missing total for leftgap\n");
	halt();
      }
      if (LINK->rightgap->mothers == NULL) {
	printf("ERROR: missing total for rightgap\n");
	halt();
      }

      /*
*/

      /*
*/

      if (LINK->aGapFeature->others != NULL) {
	printf("ERROR: aGapFeature not nil!\n");
	halt();
      }
      LINK->aGapFeature->others = (featurecarrier *)Malloc(sizeof(featurecarrier));
      LINK->groupGapCarrier = LINK->aGapFeature->others;
      LINK->groupGapCarrier->next = NULL;
      LINK->groupGapCarrier->afeature = LINK->leftgap;

      LINK->groupGapCarrier->next = (featurecarrier *)Malloc(sizeof(featurecarrier));
      LINK->groupGapCarrier = LINK->groupGapCarrier->next;
      LINK->groupGapCarrier->next = NULL;
      LINK->groupGapCarrier->afeature = LINK->rightgap;

      /*
*/

      addmotherlinks(&LINK->aGapFeature, &LINK->rightgap, &V);
      addmotherlinks(&LINK->aGapFeature, &LINK->leftgap, &V);
      if (superdebug) {
	printf("BUBBA final aGapFeature is: ========\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showafeature(&TEMP, LINK->aGapFeature);
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, LINK->aGapFeature);
	putchar('\n');
	/*





*/
      }

    }
    /*



*/

    LINK->gg = LINK->gg->next;

    /*

*/

  }

  if (superdebug) {
    printf("BUBBA: show AFTER threading:\n");
    LINK->gg = LINK->groupfeature;
    while (LINK->gg != NULL) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeafeature(&TEMP, LINK->gg);
      putchar('\n');
      LINK->gg = LINK->gg->next;
    }
  }



  /*

*/
  LINK->gg = LINK->groupfeature;
  while (LINK->gg != NULL) {
    if (LINK->gg->duplicateprevious != NULL && !LINK->gg->definition->isgap) {
      addmotherlinks(&LINK->aGapFeature, &LINK->gg->duplicateprevious, &V);
      /*




*/
      /*
*/
      /*


*/

      if (!LINK->aGapFeature->definition->isgap)
	LINK->aGapFeature->others->afeature = LINK->gg->duplicateprevious;
      /*






*/
    }

    /*

*/
    LINK->gg = LINK->gg->next;
  }
  /*

*/
  /*

*/


  if (superdebug) {
    printf("BUBBA: show AFTER motherlinks:========\n");
    LINK->gg = LINK->groupfeature;
    while (LINK->gg != NULL) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeafeature(&TEMP, LINK->gg);
      putchar('\n');
      LINK->gg = LINK->gg->next;
    }
  }

  LINK->groupTotalFeature = NULL;
  LINK->ingroup = false;

  LINK->groupfeature = NULL;
  /*
*/


}



Static Void readdefinitionsandfeatures(afile, KeepDuplicates, definitionlist_,
				       featurelist_)
_TEXT *afile;
boolean *KeepDuplicates;
definetype **definitionlist_;
featuretype **featurelist_;
{
  /*











































*/
  struct LOC_readdefinitionsandfeatures V;
  stringDelila aname;
  long countdefinitions = 0, countfeatures = 0, countribls = 0,
       countboundaries = 0, countpetals = 0;
  definetype *d;
  boolean done;
  definetype *dprevious, *dtest;
  boolean emptyafile;
  boolean firstfeature = true;

  Char identifier;
  ribltype *matrix;
  boolean missingmatrix;
  featuretype *newfeature;
  /*
*/
  featuretype *oldfeature;
  definetype *thedefinition;
  /*
*/
  long uniquedefinitions = 0, uniquefeatures = 0;
  _TEXT TEMP;
  petaltype *WITH;

  V.definitionlist = definitionlist_;
  V.featurelist = featurelist_;
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
  emptyafile = BUFEOF(afile->f);
  *V.definitionlist = NULL;
  *V.featurelist = (featuretype *)Malloc(sizeof(featuretype));
  clearfeature(V.featurelist);
  d = *V.definitionlist;
  V.f = *V.featurelist;
  /*


*/


  V.groupfeature = NULL;
  V.ingroup = false;
  V.nameparts = (stringDelila *)Malloc(sizeof(stringDelila));
  clearstring(V.nameparts);
  while (!BUFEOF(afile->f)) {
    if ((P_peek(afile->f) == '*') | P_eoln(afile->f)) {
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      continue;
    }
    skipblanks(afile);


    identifier = P_peek(afile->f);

    if (identifier == 'd') {
      if (*V.definitionlist == NULL) {
	*V.definitionlist = (definetype *)Malloc(sizeof(definetype));
	cleardefinition(V.definitionlist);
	d = *V.definitionlist;
	dprevious = d;
	/*

*/
      } else {
	dprevious = d;
	d->next = (definetype *)Malloc(sizeof(definetype));
	d = d->next;
	cleardefinition(&d);
      }
      readadefinition(afile, &d);
      /*

*/
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      countdefinitions++;
      uniquedefinitions++;
      d->definitionnumber = uniquedefinitions;


      dtest = *V.definitionlist;
      done = false;
      while (!done) {
	if (dtest == NULL) {
	  done = true;
	  break;
	}
	if (equaldefinition(dtest, d) &&
	    dtest->definitionnumber != d->definitionnumber) {
	  /*

*/

	  done = true;
	  /*
*/
	  barout(&V);
	  printf("*** WARNING: A duplicate definition was found at definition %ld:\n",
		 countdefinitions);
	  printf("*** ");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writequotestring(&TEMP, d->nametag);
	  printf(".\n");
	  printf("*** The first definition (number %ld) will be used.\n",
		 dtest->definitionnumber);

	  /*


















*/
	  barout(&V);

	  /*













*/



	  dprevious->next = NULL;
	  Free(d);
	  done = true;
	  /*
*/

	  d = dprevious;
	  /*



*/

	  /*



*/

	  /*


*/

	  uniquedefinitions--;
	}
	/*

*/

	if (!done)
	  dtest = dtest->next;
      }
      /*



*/
      continue;
    }


    if (identifier == 'r') {
      getriblmatrix(afile, &matrix);

      locatedefinition(matrix->riblname, *V.definitionlist, &thedefinition);
      if (thedefinition == NULL) {
	printf("A definition for Ribl matrix ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, matrix->riblname);
	printf(" was not found in the features.\n");
	halt();
      }

      /*
*/
      if (thedefinition->min < matrix->frombase ||
	  thedefinition->max > matrix->tobase) {
	printf("Definition:\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeadefinition(&TEMP, thedefinition);
	printf("\nViolates The From-To Bounds of its Matrix:\n");
	printf("matrix frombase: %ld\n", matrix->frombase);
	printf("matrix   tobase: %ld\n", matrix->tobase);
	halt();
      }


      thedefinition->matrix = matrix;
      countribls++;
      continue;
    }


    if (identifier == 'b') {
      skipnonblanks(afile);
      skipblanks(afile);
      readquotestring(afile, &aname);
      locatedefinition(aname, *V.definitionlist, &thedefinition);
      if (thedefinition == NULL) {
	printf("WARNING: A definition for the boundaries: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf("          was not found in the features.\n");
      } else {
	fscanf(afile->f, "%lg%lg%lg%*[^\n]", &thedefinition->RiboundFeature,
	       &thedefinition->ZboundFeature, &thedefinition->PboundFeature);
	getc(afile->f);
	countboundaries++;
      }
      continue;
    }


    if (identifier == 'p') {
      skipnonblanks(afile);
      skipblanks(afile);
      readquotestring(afile, &aname);
      locatedefinition(aname, *V.definitionlist, &thedefinition);
      if (thedefinition == NULL) {
	printf("WARNING: A definition for the color rectangle: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf("\n         was not found in the features.\n");
	fscanf(afile->f, "%*[^\n]");
	getc(afile->f);
	continue;
      }
      WITH = &thedefinition->petal;
      fscanf(afile->f, "%ld", &WITH->el);
      /*

*/
      skipblanks(afile);
      WITH->c_ = getc(afile->f);
      if (WITH->c_ == '\n')
	WITH->c_ = ' ';
      if (WITH->c_ != 'r' && WITH->c_ != 'h') {
	yell(&V);
	printf(" feature\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf(": color type must be r or h, not \"%c\"\n", WITH->c_);
	halt();
      }
      fscanf(afile->f, "%lg%lg%lg%lg%lg%lg%lg%*[^\n]", &WITH->eh, &WITH->es,
	     &WITH->eb, &WITH->fh, &WITH->fs, &WITH->fb, &WITH->spare);
      getc(afile->f);

      /*















*/

      if (WITH->eh > 1 || WITH->es > 1 || WITH->eb > 1 || WITH->fh > 1 ||
	  WITH->fs > 1 || WITH->fb > 1) {
	yell(&V);
	printf(" feature\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf(": eh, es, eb, fh, fs, and fb \n");
	printf("must have values less than 1\n");
	halt();
      }

      /*











*/

      countpetals++;
      continue;
    }




    if (identifier == '@') {
      countfeatures++;

      if (firstfeature) {
	firstfeature = false;
	readafeature(afile, *V.definitionlist, &V.f);
	fscanf(afile->f, "%*[^\n]");
	getc(afile->f);
	/*
*/
	V.f->featurenumber = countfeatures;
	uniquefeatures++;
      }

      else {
	newfeature = (featuretype *)Malloc(sizeof(featuretype));
	clearfeature(&newfeature);
	readafeature(afile, *V.definitionlist, &newfeature);
	fscanf(afile->f, "%*[^\n]");
	getc(afile->f);
	/*
*/
	if (!featureinlist(*V.featurelist, newfeature, &oldfeature)) {
	  V.f->next = newfeature;
	  V.f = V.f->next;
	  V.f->featurenumber = countfeatures;
	  uniquefeatures++;
	}


	else {
	  printf("This feature was found more than once:\n");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writeafeature(&TEMP, newfeature);
	  putchar('\n');

	  /*











*/




	  V.f->next = newfeature;
	  V.f = V.f->next;
	  V.f->featurenumber = countfeatures;

	  if (*KeepDuplicates) {
	    printf("It is being kept.\n");
	    V.f->duplicateprevious = NULL;
	    /*

*/
	  }

	  else {
	    printf("It will be ignored.\n");
	    V.f->duplicateprevious = oldfeature;
	    /*


*/

	  }
	}
      }

      threadgroup(&V);

      /*



*/
      continue;
    }

    if (identifier == 'g') {
      groupnumber++;
      V.ingroup = true;
      fscanf(afile->f, "%*[^\n]");
      /*


*/
      getc(afile->f);
    }

    else {
      printf(
	"features file lines must be blank or begin with one of \"*d@grbp\", not \"%c\".\n",
	identifier);
      halt();
    }
  }

  /*

*/
  if (*V.featurelist != NULL) {
    if ((*V.featurelist)->id.length == 0)
      emptyafile = true;
  }

  if (!emptyafile) {
    /*
*/
    if (*V.definitionlist != NULL) {
      d = *V.definitionlist;
      missingmatrix = false;
      while (d != NULL) {
	/*



*/
	if (d->background.length == 2) {
	  if (d->background.letters[1] == 'I' ||
	      d->background.letters[1] == 'i' ||
	      d->background.letters[1] == 'L' ||
	      d->background.letters[1] == 'l' ||
	      d->background.letters[1] == 'W' ||
	      d->background.letters[1] == 'w') {
	    if (d->matrix == NULL) {
	      missingmatrix = true;
	      printf("Definition:\n");
	      TEMP.f = stdout;
	      *TEMP.name = '\0';
	      writeadefinition(&TEMP, d);
	      printf("\nis missing a Ribl weight matrix\n");
	    }
	  }
	}
	d = d->next;
      }
      if (missingmatrix) {
	printf("readdefinitionsandfeatures: missingmatrix is true\n");
	halt();
      }
    }
  }

  printf("**************************************************\n");
  if (emptyafile) {
    printf("** Empty Features File ***************************\n");
    printf("** The features file may have characters in it, **\n");
    printf("** but it contains no features.                 **\n");
    printf("**************************************************\n");

    Free(*V.featurelist);
    *V.featurelist = NULL;
  } else {
    printf("%ld definition", countdefinitions);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    plural(&TEMP, countdefinitions);
    printf(" read in\n");

    printf("%ld unique definition", uniquedefinitions);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    plural(&TEMP, uniquedefinitions);
    printf(" found\n");

    printf("%ld definition", countribls);
    if (countribls == 1)
      printf(" has an Ribl matrix\n");
    else
      printf("s have Ribl matricies\n");

    printf("%ld definition", countboundaries);
    if (countboundaries == 1)
      printf(" boundry defined\n");
    else
      printf(" boundries defined\n");

    printf("%ld", countpetals);
    if (countpetals == 1)
      printf(" petal (color rectangle behind a walker) defined\n");
    else
      printf(" petals (color rectangles behind walkers) defined\n");

    printf("%ld feature", countfeatures);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    plural(&TEMP, countfeatures);
    printf(" read in\n");

    printf("%ld unique feature", uniquefeatures);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    plural(&TEMP, uniquefeatures);
    printf(" found\n");

    /*






*/



    if (false) {
      printf("\n******* @ end of readdefinitionsandfeatures: *******\n");
      printf("******* DEFINITIONS:\n");
      if (false) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showdefinitions(&TEMP, *V.definitionlist);
      }
      printf("\n******* FEATURES:\n");
      if (true) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showfeatures(&TEMP, *V.featurelist, true);
      }
      printf("\n******* @ end of readdefinitionsandfeatures: *******\n");
      /*

*/
    }

  }
  printf("**************************************************\n");
}



Static boolean isnamestring(n, s)
name n;
stringDelila s;
{
  boolean Result;
  long c_ = 1;
  boolean done = false;

  /*





*/
  if (n.length != s.length)
    return false;
  while (!done) {
    /*

*/
    if (s.letters[c_-1] == n.letters[c_-1]) {
      c_++;
      if (c_ > s.length) {
	Result = true;
	done = true;
      }
    } else {
      Result = false;
      done = true;
    }
  }
  return Result;
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



Static Void writename(afile, s)
_TEXT *afile;
name s;
{
  long i;

  for (i = 0; i < s.length; i++)
    putc(s.letters[i], afile->f);
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



Static Void numberstring(anumber, astring)
long anumber;
stringDelila *astring;
{
  /*
*/
  long d, size;

  clearstring(astring);
  size = numbersize(anumber) - 1;
  for (d = 0; d <= size; d++)
    astring->letters[size - d] = numberdigit(anumber, d);
  astring->length = size + 1;
}



Static Void concatstring(a_, b, c_)
stringDelila *a_, *b, *c_;
{
  long abindex;
  long cindex = 0;
  long FORLIM;

  emptystring(c_);
  c_->length = a_->length + b->length;
  if (c_->length > maxstring) {
    printf("string too long in concatstring\n");
    halt();
  }

  FORLIM = a_->length;
  for (abindex = 0; abindex < FORLIM; abindex++) {
    cindex++;
    c_->letters[abindex] = a_->letters[abindex];
  }

  FORLIM = a_->length + b->length;
  for (abindex = a_->length; abindex < FORLIM; abindex++) {
    cindex++;
    c_->letters[abindex] = b->letters[abindex - a_->length];
  }
}



Static Void attachstring(a_, b)
stringDelila *a_, b;
{
  stringDelila hold;

  copystring(*a_, &hold);
  concatstring(&hold, &b, a_);
}


/* Local variables for themain: */
struct LOC_themain {

  piece *apiece;
  long sequences, orfs;
  featuretype *f;
  base b;
  long pe, pi;
} ;



Local Void report(afile, LINK)
_TEXT *afile;
struct LOC_themain *LINK;
{
  fprintf(afile->f, "*\n");
  fprintf(afile->f, "* %ld sequences are in the genomebook\n",
	  LINK->sequences);
  fprintf(afile->f, "* %ld ORFs are in the genomebook\n", LINK->orfs);
}

Local base grabbase(l, LINK)
long l;
struct LOC_themain *LINK;
{
  /*
*/
  LINK->pe = (long)floor(LINK->f->coordinate + l * LINK->f->orientation + 0.5);
  LINK->pi = pietoint(LINK->pe, LINK->apiece);
  LINK->b = getbase(LINK->pi, LINK->apiece);
  return LINK->b;
  /*

*/
}







Static Void themain(genomebook, scanfeatures, orfp, orffeatures)
_TEXT *genomebook, *scanfeatures, *orfp, *orffeatures;
{
  struct LOC_themain V;
  double parameterversion;
  boolean debug = false;
  long theline;
  definetype *definitionlist;
  /*
*/
  featuretype *featurelist;
  boolean KeepDuplicates = true;

  stringDelila ir;
  base b0, b1, b2;

  long codon;
  boolean done, stopcodon;
  long pielength;
  featuretype *totalfeature;

  definetype *d;
  /*
*/
  ribltype *matrix;
  /*
*/

  featuretype *orf;
  definetype *orfdef;
  featuretype *fhold;
  /*
*/
  definetype *dhold;
  /*
*/

  stringDelila codonword;
  /*
*/
  stringDelila orfname;

  long shortestorf, longestorf;

  long pieorientation;

  featuretype *newfeature;
  stringDelila *WITH1;
  _TEXT TEMP;

  printf("orf %4.2f\n", version);
  if (*orfp->name != '\0') {
    if (orfp->f != NULL)
      orfp->f = freopen(orfp->name, "r", orfp->f);
    else
      orfp->f = fopen(orfp->name, "r");
  } else
    rewind(orfp->f);
  if (orfp->f == NULL)
    _EscIO2(FileNotFound, orfp->name);
  RESETBUF(orfp->f, Char);
  fscanf(orfp->f, "%lg%*[^\n]", &parameterversion);
  getc(orfp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(orfp->f, "%ld%*[^\n]", &shortestorf);
  getc(orfp->f);
  fscanf(orfp->f, "%ld%*[^\n]", &longestorf);
  getc(orfp->f);

  readdefinitionsandfeatures(scanfeatures, &KeepDuplicates, &definitionlist,
			     &featurelist);


  /*








*/

  clearstring(&ir);


  ir.letters[0] = 'i';
  ir.letters[1] = 'r';
  ir.length = 2;
  orfdef = (definetype *)Malloc(sizeof(definetype));
  cleardefinition(&orfdef);


  WITH1 = &orfdef->nametag;

  WITH1->letters[0] = 'o';
  WITH1->letters[1] = 'r';
  WITH1->letters[2] = 'f';
  WITH1->length = 3;
  WITH1 = &orfdef->background;

  WITH1->letters[0] = '#';
  WITH1->length = 1;
  WITH1 = &orfdef->negparts;

  WITH1->letters[0] = '<';
  WITH1->letters[1] = ']';
  WITH1->length = 2;
  WITH1 = &orfdef->posparts;

  WITH1->letters[0] = '[';
  WITH1->letters[1] = '>';
  WITH1->length = 2;
  orfdef->locations[0] = 0.0;
  orfdef->locations[1] = 2.0;
  orfdef->marks = 2;
  orfdef->min = 0.0;
  orfdef->max = 1.0;

  emptystring(&codonword);

  codonword.letters[0] = ' ';
  codonword.letters[1] = 'c';
  codonword.letters[2] = 'o';
  codonword.letters[3] = 'd';
  codonword.letters[4] = 'o';
  codonword.letters[5] = 'n';
  codonword.letters[6] = 's';
  codonword.length = 7;
  emptystring(&orfname);

  orfname.letters[0] = 'o';
  orfname.letters[1] = 'r';
  orfname.letters[2] = 'f';
  orfname.length = 3;
  /*




















*/

  brinit(genomebook, &theline);
  if (*orffeatures->name != '\0') {
    if (orffeatures->f != NULL)
      orffeatures->f = freopen(orffeatures->name, "w", orffeatures->f);
    else
      orffeatures->f = fopen(orffeatures->name, "w");
  } else {
    if (orffeatures->f != NULL)
      rewind(orffeatures->f);
    else
      orffeatures->f = tmpfile();
  }
  if (orffeatures->f == NULL)
    _EscIO2(FileNotFound, orffeatures->name);
  SETUPBUF(orffeatures->f, Char);

  fprintf(orffeatures->f, "* orf %4.2f\n", version);
  copyaline(genomebook, orffeatures);

  writeadefinition(orffeatures, orfdef);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeadefinition(&TEMP, orfdef);
  putchar('\n');

  V.apiece = (piece *)Malloc(sizeof(piece));
  V.sequences = 0;
  V.orfs = 0;
  printf("reading genomebook\n");

  while (!BUFEOF(genomebook->f)) {
    getpiece(genomebook, &theline, &V.apiece);
    if (BUFEOF(genomebook->f))
      break;
    V.sequences++;

    printf("piece ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writename(&TEMP, V.apiece->key.hea.keynam);
    putchar('\n');

    V.f = featurelist;
    while (V.f != NULL) {
      if (isnamestring(V.apiece->key.hea.keynam, V.f->id)) {
	if (debug)
	  printf("ids MATCH!\n");


	if (equalstring(V.f->nametag, ir)) {
	  putchar('\n');
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writeafeature(&TEMP, V.f);
	  putchar('\n');

	  if (debug)
	    printf("names MATCH!\n");
	  /*

















*/

	  /*



*/


	  /*

















*/

	  codon = 0;
	  stopcodon = false;

	  done = false;
	  pielength = piecelength(V.apiece);
	  switch (V.apiece->key.piedir) {

	  case plus:
	    pieorientation = 1;
	    break;

	  case minus:
	    pieorientation = -1;
	    break;
	  }
	  printf("\n%ld ", (long)floor(V.f->coordinate + 0.5));
	  while (!done) {
	    switch (V.f->orientation * pieorientation) {

	    case 1:
	      b0 = grabbase(codon, &V);
	      b1 = grabbase(codon + 1, &V);
	      b2 = grabbase(codon + 2, &V);
	      break;

	    case -1:
	      b0 = complement(grabbase(codon, &V));
	      b1 = complement(grabbase(codon + 1, &V));
	      b2 = complement(grabbase(codon + 2, &V));
	      break;
	    }
	    fputc(basetochar(b0), stdout);
	    fputc(basetochar(b1), stdout);
	    fputc(basetochar(b2), stdout);
	    putchar('|');



	    if (b0 == t && (b1 == a && b2 == a || b1 == g && b2 == a ||
			    b1 == a && b2 == g)) {
	      done = true;
	      stopcodon = true;
	      V.orfs++;

	      if (debug) {
		printf("stop codon! %c%c%c\n",
		       basetochar(b0), basetochar(b1), basetochar(b2));
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writeafeature(&TEMP, V.f);
		printf("\n----- this feature\n");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		showafeature(&TEMP, V.f);
		printf("----- the first mother of this feature GAP\n");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		showafeature(&TEMP, V.f->mothers->afeature);
		printf("----- the second mother of this feature TOTAL\n");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		showafeature(&TEMP, V.f->mothers->next->afeature);
		printf("-----\n");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writeafeature(&TEMP, totalfeature);

		printf("\n----- set Dparam to be codons\n");
	      }

	      /*






*/

	      totalfeature = V.f->mothers->next->afeature;
	      /*




*/







	      totalfeature->Bparam = codon / 3.0 + 1;




	      if (V.f->orientation * pieorientation == 1)
		totalfeature->Cparam =
		  inttopie(pietoint((long)floor(V.f->coordinate + 0.5),
				    V.apiece) + codon, V.apiece);
	      else
		totalfeature->Cparam =
		  inttopie(pietoint((long)floor(V.f->coordinate + 0.5),
				    V.apiece) - codon, V.apiece);
	      /*
*/





	      if (totalfeature->Bparam >= shortestorf &&
		  totalfeature->Bparam <= longestorf)
		totalfeature->Dparam = 1.0;
	      else
		totalfeature->Dparam = 0.0;

	      printf("%ld - %ld codon orf\n",
		     (long)floor(totalfeature->Cparam + 0.5),
		     (long)floor(totalfeature->Bparam + 0.5));
	      orf = (featuretype *)Malloc(sizeof(featuretype));
	      clearfeature(&orf);
	      copystring(orfname, &orf->nametag);

	      orf->id = V.f->id;
	      orf->coordinate = totalfeature->Cparam;



	      /*

*/

	      orf->orientation = totalfeature->orientation;

	      /*
*/

	      numberstring((long)floor(codon / 3.0 + 0.5) + 1, &orf->othertag);
	      attachstring(&orf->othertag, codonword);


	      orf->Tparam = 'R';
	      orf->Aparam = 1.0;
	      orf->Bparam = 0.0;
	      orf->Cparam = 0.0;
	      orf->Dparam = 1.0;


	      /*












*/



	      orf->definition = orfdef;
	      printf("orf:\n");
	      TEMP.f = stdout;
	      *TEMP.name = '\0';
	      writeafeature(&TEMP, orf);
	      putchar('\n');
	      /*



*/
	      /*






*/

	      fhold = V.f->next;
	      V.f->next = orf;
	      orf->next = fhold;
	      /*


*/

	      if (debug) {
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writeafeature(&TEMP, totalfeature);
		putchar('\n');
	      }

	    }
	    codon += 3;

	    /*

*/
	    if (pietoint((long)floor(V.f->coordinate + 0.5), V.apiece) +
		codon * V.f->orientation * pieorientation > pielength)
	      done = true;

	    /*
*/
	    if (pietoint((long)floor(V.f->coordinate + 0.5), V.apiece) + codon <=
		pielength)
	      continue;

	    printf("- ORF IS NOT TERMINATED WITH A STOP\n");
	    printf("pielength %ld\n", pielength);
	    printf("codon %ld\n", codon);
	    printf("round(f^.coordinate) %ld\n",
		   (long)floor(V.f->coordinate + 0.5));
	    printf("pietoint(round(f^.coordinate),apiece) %ld\n",
		   pietoint((long)floor(V.f->coordinate + 0.5), V.apiece));
	    printf("pietoint(round(f^.coordinate),apiece)+codon %ld\n",
	      pietoint((long)floor(V.f->coordinate + 0.5), V.apiece) + codon);
	    done = true;
	  }


	}
      }
      V.f = V.f->next;
    }

    clearpiece(&V.apiece);
  }

  printf("genomebook reading is done\n");

  /*


*/
  V.f = featurelist;
  d = (definetype *)Malloc(sizeof(definetype));
  if (*scanfeatures->name != '\0') {
    if (scanfeatures->f != NULL)
      scanfeatures->f = freopen(scanfeatures->name, "r", scanfeatures->f);
    else
      scanfeatures->f = fopen(scanfeatures->name, "r");
  } else
    rewind(scanfeatures->f);
  if (scanfeatures->f == NULL)
    _EscIO2(FileNotFound, scanfeatures->name);
  RESETBUF(scanfeatures->f, Char);
  while (!BUFEOF(scanfeatures->f)) {
    if (P_eoln(scanfeatures->f)) {
      fscanf(scanfeatures->f, "%*[^\n]");
      getc(scanfeatures->f);
      putc('\n', orffeatures->f);
      continue;
    }
    if (debug)
      printf("scanfeatures^ = \"%c\"\n", P_peek(scanfeatures->f));
    if (P_peek(scanfeatures->f) != '@' && P_peek(scanfeatures->f) != 'r' &&
	P_peek(scanfeatures->f) != 'd' && P_peek(scanfeatures->f) != 'g' &&
	P_peek(scanfeatures->f) != ' ' && P_peek(scanfeatures->f) != 'p' &&
	P_peek(scanfeatures->f) != 'b' && P_peek(scanfeatures->f) != '*') {
      printf("Illegal character in scanfeatures: \"%c\"\n",
	     P_peek(scanfeatures->f));
      printf("The first character of scanfeature lines\n");
      printf(" can only contain \"*bp dr@g\"\n");
      halt();
    }
    switch (P_peek(scanfeatures->f)) {

    case '*':
    case 'b':
    case 'p':
    case ' ':
    case 'g':
      /*
*/
      copyaline(scanfeatures, orffeatures);
      fscanf(scanfeatures->f, "%*[^\n]");
      getc(scanfeatures->f);
      break;

    case 'd':
      readadefinition(scanfeatures, &d);
      writeadefinition(orffeatures, d);
      printf("BUBBAnow, read a definition:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeadefinition(&TEMP, d);
      fscanf(scanfeatures->f, "%*[^\n]");
      getc(scanfeatures->f);
      break;

    case 'r':
      getriblmatrix(scanfeatures, &matrix);
      putriblmatrix(orffeatures, matrix);
      break;

    case '@':
      printf("@ location\n");
      /*
*/

      /*






*/

      newfeature = (featuretype *)Malloc(sizeof(featuretype));
      clearfeature(&newfeature);

      readafeature(scanfeatures, definitionlist, &newfeature);
      fscanf(scanfeatures->f, "%*[^\n]");
      getc(scanfeatures->f);
      printf("current feature newfeature: ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeafeature(&TEMP, newfeature);
      putchar('\n');

      /*

*/
      /*


*/
      writeafeature(orffeatures, V.f);
      putc('\n', orffeatures->f);
      /*



*/
      if (newfeature->definition->istotal) {
	printf("****** COMPARE newfeature TO f: *******\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, newfeature);
	printf("=newfeature\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, V.f);
	printf("=f\n");
	printf("***************************************\n");

	printf("feature newfeature is a total: \n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, newfeature);
	printf("\nHERE is the spot to output the orf, just after the total\n");
	printf("feature f: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, V.f);
	putchar('\n');

	/*


*/
      }
      V.f = V.f->next;


      if (V.f != NULL) {
	if (equalstring(orfname, V.f->nametag)) {
	  writeafeature(orffeatures, V.f);
	  putc('\n', orffeatures->f);
	  V.f = V.f->next;
	}
      }
      break;
    }
  }
  /*


*/


  if (V.f != NULL) {
    while (V.f != NULL) {
      printf("\nTHIS FEATURE WAS NOT USED, writing it out:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeafeature(&TEMP, V.f);
      putchar('\n');
      writeafeature(orffeatures, V.f);
      putc('\n', orffeatures->f);
      V.f = V.f->next;
    }
  }

  report(orffeatures, &V);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  report(&TEMP, &V);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  orffeatures.f = NULL;
  strcpy(orffeatures.name, "orffeatures");
  orfp.f = NULL;
  strcpy(orfp.name, "orfp");
  scanfeatures.f = NULL;
  strcpy(scanfeatures.name, "scanfeatures");
  genomebook.f = NULL;
  strcpy(genomebook.name, "genomebook");
  themain(&genomebook, &scanfeatures, &orfp, &orffeatures);
_L1:
  if (genomebook.f != NULL)
    fclose(genomebook.f);
  if (scanfeatures.f != NULL)
    fclose(scanfeatures.f);
  if (orfp.f != NULL)
    fclose(orfp.f);
  if (orffeatures.f != NULL)
    fclose(orffeatures.f);
  exit(EXIT_SUCCESS);
}



/* End. */
