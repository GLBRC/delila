/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "mfoldfea.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.34
/*









*/
#define updateversion   1.00



/*

































































































































*/



/*


















































*/



#define maxstring       2000



#define minribl         (-2000)
#define maxribl         2000
#define defnegativeinfinity  (-1000)
/*
*/



#define infofield       10
#define infodecim       6

#define nfield          8




#define dnamax          1024
#define namelength      100
#define linelength      80




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;





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

/* p2c: mfoldfea.p, line 296:
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
  long number;

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
  double Aparam, Bparam, Cparam, Dparam;
  /*

















*/


  double evencoordinate;
  /*

*/


  boolean unsatisfied;
  /*
*/
  double fromrange, torange;
  long number, desiredline;
  /*

*/
  /*







*/
  featurecarrier *others, *mothers;

  struct featuretype *next;
} featuretype;







typedef struct pairing {
  /*



*/
  long i;
  Char thebase;
  long five, three, basepaired, hstnum;
  struct pairing *next;
} pairing;

typedef struct structure {
  stringDelila idnumber;
  /*
*/
  long sequnumber;
  stringDelila id, nametag;
  long length;
  double energy;
  pairing *pairs;
  long orientation;
  struct structure *next;
} structure;





Static _TEXT cootab, book, mfoldfeatures;






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
  long i;
  piekey *WITH;

  WITH = &pie->key;
  switch (WITH->piedir) {

  case plus:
    if (p >= WITH->piebeg)
      i = p - WITH->piebeg + 1;
    else
      i = p - WITH->coobeg + WITH->cooend - WITH->piebeg + 2;
    break;

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
  long p;
  piekey *WITH;

  WITH = &pie->key;
  switch (WITH->piedir) {

  case plus:
    p = WITH->piebeg + i - 1;
    if (p > WITH->cooend) {
      if (WITH->coocon == circular)
	p += WITH->coobeg - WITH->cooend - 1;
    }
    break;

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
  while (!P_eoln(thefile->f)) {
    i++;
    acharacter = getc(thefile->f);
    if (acharacter == '\n')
      acharacter = ' ';
    (*l)->letters[i-1] = acharacter;
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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
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



Static Void attachcharacterstring(c_, a_)
Char c_;
stringDelila *a_;
{
  if (a_->length == maxstring) {
    printf("attachcharacterstring: maxstring exceeded\n");
    halt();
  }
  a_->length++;
  a_->letters[a_->length - 1] = c_;
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



Static Void numberrealstring(s, n, thewidth, thedecimal)
stringDelila *s;
double n;
long thewidth, thedecimal;
{
  /*









*/
  /*


*/
  long decimalplace, d, digits;
  /*


*/
  long i, power;
  boolean minus_;
  long newwidth, sign;
  Char symbol;
  long FORLIM;
  _TEXT TEMP;

  /*
*/
  emptystring(s);
  if (thewidth > 0) {
    power = 1;
    for (i = 1; i <= thedecimal; i++)
      power *= 10;
    digits = (long)floor(power * fabs(n) + 0.5);
    if (thedecimal > 0)
      decimalplace = 1;
    else
      decimalplace = 0;
    if (n < 0)
      sign = 1;
    else
      sign = 0;
    newwidth = numbersize(digits) + thedecimal + sign;

    /*


*/
    if (fabs(n) < 1.0)
      newwidth++;


    if (thewidth > newwidth)
      newwidth = thewidth;

    /*













*/

    for (d = newwidth - decimalplace - 1; d >= 0; d--) {
      symbol = numberdigit(digits, d);
      /*





*/
      if (symbol == '-')
	symbol = '0';
      if (symbol == '+')
	symbol = '0';
      if (symbol == ' ')
	symbol = '0';
      s->length++;
      s->letters[s->length - 1] = symbol;
      if (d == thedecimal) {
	if (thedecimal > 0) {
	  /*




*/
	  s->length++;
	  s->letters[s->length - 1] = '.';
	}
      }
    }
    /*



*/


    s->current = 1;
    while (s->letters[s->current - 1] == '0' && s->current < s->length) {
      if (s->letters[s->current] != '.')
	s->letters[s->current - 1] = ' ';
      s->current++;
      /*



*/

    }

    if (n < 0) {
      s->current = 1;
      minus_ = false;
      while (s->letters[s->current - 1] == ' ') {
	if (s->letters[s->current] != ' ') {
	  s->letters[s->current - 1] = '-';
	  minus_ = true;
	}
	s->current++;
	if (s->current < maxstring)
	  continue;
	printf("numberrealstring:");
	printf(" cannot place minus sign in string\n");
	printf("placing number at %ld\n", s->current);
	printf("the number is n =%*.*f\n", (int)newwidth, (int)thedecimal, n);
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, s);
	halt();
      }
      if (!minus_) {
	printf("numberrealstring ERROR: missing minus sign for number %*.*f\n",
	       (int)newwidth, (int)thedecimal, n);
	printf("Increase one of the number widths.\n");
	halt();
      }
    }
  }

  /*

*/
  if (s->length <= thewidth)
    return;

  /*





*/

  while (s->letters[0] == ' ') {
    FORLIM = s->length;
    for (i = 1; i < FORLIM; i++)
      s->letters[i-1] = s->letters[i];
    s->length--;
  }
}


#define tabcharacter    9



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
    if (c_ == tabcharacter)
      c_ = ' ';
    s->letters[i] = c_;
  }
}

#undef tabcharacter



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
  WITH->number = 0;

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

  WITH->Ri = 0.0;
  WITH->Z = 0.0;
  WITH->probability = 0.0;

  WITH->Tparam = ' ';
  WITH->Aparam = 0.0;
  WITH->Bparam = 0.0;
  WITH->Cparam = 0.0;
  WITH->Dparam = 0.0;
  WITH->evencoordinate = 0.0;
  WITH->unsatisfied = true;
  WITH->fromrange = 0.0;
  WITH->torange = 0.0;
  WITH->number = 0;
  WITH->desiredline = 0;
  WITH->others = NULL;
  WITH->mothers = NULL;
  WITH->next = NULL;
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
  fprintf(afile->f,
	  "%12ld n, number of sequences used to create the matrix\n",
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
  }
  /*









*/
  else {
    if (f->definition->matrix != NULL) {
      fprintf(afile->f, " %*.*f", infofield, infodecim, f->Ri);
      fprintf(afile->f, " %*.*f", infofield, infodecim, f->Z);
      fprintf(afile->f, " %*.*f", infofield, infodecim, f->probability);
    } else {
      fprintf(afile->f, " %c", f->Tparam);
      fprintf(afile->f, " %*.*f", infofield, infodecim, f->Aparam);
      fprintf(afile->f, " %*.*f", infofield, infodecim, f->Bparam);
      fprintf(afile->f, " %*.*f", infofield, infodecim, f->Cparam);
      fprintf(afile->f, " %*.*f", infofield, infodecim, f->Dparam);
    }
  }
  if (f->others == NULL)
    return;

  if (f->others->afeature != NULL)
    fprintf(afile->f, " -> ");
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







Static Void unnumber(idnumber, id, n)
stringDelila idnumber, *id;
long *n;
{
  /*
*/
  Char c_;
  long d;
  long power = 1;
  _TEXT TEMP;

  /*



*/
  *n = 0;
  copystring(idnumber, id);
  while (id->length > 0 && id->letters[id->length - 1] != '#') {
    c_ = id->letters[id->length - 1];
    d = c_ - '0';
    if ((unsigned long)d > 9) {
      printf("unnumber: the id string has a bad number after the \"#\":\n");
      printf("The entire id string is \"");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, id);
      printf("\"\n");
      halt();
    }
    *n += d * power;
    power *= 10;


    id->length--;
  }

  id->length--;
  if (id->length > 0)
    return;

  printf("unnumber: id does not have a \"#\"\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &idnumber);
  putchar('\n');
  halt();
}



Static Void readcoordinatetable(cootab, st)
_TEXT *cootab;
structure **st;
{
  /*













*/
  /*

*/
  structure *h;
  pairing *c_;
  boolean toldversion = false;
  stringDelila word;
  _TEXT TEMP;
  pairing *WITH1;

  printf("Reading in structures:\n");
  if (*cootab->name != '\0') {
    if (cootab->f != NULL)
      cootab->f = freopen(cootab->name, "r", cootab->f);
    else
      cootab->f = fopen(cootab->name, "r");
  } else
    rewind(cootab->f);
  if (cootab->f == NULL)
    _EscIO2(FileNotFound, cootab->name);
  RESETBUF(cootab->f, Char);
  if (BUFEOF(cootab->f)) {
    printf("cootab is empty\n");
    halt();
  }

  *st = (structure *)Malloc(sizeof(structure));
  (*st)->next = NULL;
  h = *st;

  while (!BUFEOF(cootab->f)) {


    fscanf(cootab->f, "%ld", &h->length);
    /*

*/
    skipcolumn(cootab);
    skipblanks(cootab);
    /*

*/
    grabtoken(cootab, &word);
    if (word.letters[0] != '=') {
      printf("readcoordinatetable: \"=\" not found\n");
      /*

*/
      printf("checking for \"[\"\n");
      if (word.letters[word.length - 1] != ']') {
	printf("readcoordinatetabe: could not find \"]\"\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &word);
	putchar('\n');
	halt();
      }
      h->energy = 0.0;
      grabtoken(cootab, &h->idnumber);
    } else {
      fscanf(cootab->f, "%lg", &h->energy);
      grabtoken(cootab, &h->idnumber);
    }


    if (h->idnumber.letters[0] == '[') {
      /*


*/
      grabtoken(cootab, &h->idnumber);
      grabtoken(cootab, &h->idnumber);
      unnumber(h->idnumber, &h->id, &h->sequnumber);
      if (!toldversion) {
	toldversion = true;
	printf("reading mfold 3.1 format\n");
      }
    } else {
      /*

*/
      unnumber(h->idnumber, &h->id, &h->sequnumber);
      if (!toldversion) {
	toldversion = true;
	printf("reading mfold 3.1 format\n");
      }
    }
    fscanf(cootab->f, "%*[^\n]");


    getc(cootab->f);
    printf("  ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, h->id);
    printf(" length = %2ld", h->length);
    printf(" energy = %5.1f", h->energy);
    printf(" idnumber = ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &h->idnumber);
    printf(" sequnumber = %ld\n", h->sequnumber);


    h->pairs = (pairing *)Malloc(sizeof(pairing));
    c_ = h->pairs;
    c_->next = NULL;
    do {
      WITH1 = c_;
      fscanf(cootab->f, "%ld", &WITH1->i);
      /*

*/
      skipblanks(cootab);
      WITH1->thebase = getc(cootab->f);
      if (WITH1->thebase == '\n')
	WITH1->thebase = ' ';
      fscanf(cootab->f, "%ld", &WITH1->five);
      fscanf(cootab->f, "%ld", &WITH1->three);
      fscanf(cootab->f, "%ld", &WITH1->basepaired);
      fscanf(cootab->f, "%ld", &WITH1->hstnum);
      fscanf(cootab->f, "%*[^\n]");
      /*

*/

      getc(cootab->f);
      if (BUFEOF(cootab->f)) {
	/*

*/
	if (WITH1->i != h->length) {
	  printf("table ended at record %ld This is not the reported length (%ld)\n",
		 WITH1->i, h->length);
	  halt();
	}
      } else {
	/*

*/
	if (WITH1->i != h->length) {
	  c_->next = (pairing *)Malloc(sizeof(pairing));
	  /*


*/
	  c_ = c_->next;
	  c_->next = NULL;
	}
	/*

*/
      }
    } while (c_->i != h->length);

    if (BUFEOF(cootab->f))
      break;
    /*

*/
    h->next = (structure *)Malloc(sizeof(structure));
    h = h->next;
    h->next = NULL;
    /*

*/

  }
}



Static Void writecoordinatetable(f, st)
_TEXT *f;
structure *st;
{
  structure *s = st;
  pairing *p;
  structure *WITH;
  pairing *WITH1;

  while (s != NULL) {
    WITH = s;
    fprintf(f->f, "%5ld", WITH->length);
    fprintf(f->f, " ENERGY = ");
    fprintf(f->f, "%7.1f    ", WITH->energy);
    writestring(f, &WITH->id);
    putc('\n', f->f);
    p = WITH->pairs;
    while (p != NULL) {
      WITH1 = p;
      fprintf(f->f, "%5ld", WITH1->i);
      fprintf(f->f, " %c", WITH1->thebase);
      fprintf(f->f, " %7ld", WITH1->five);
      fprintf(f->f, " %4ld", WITH1->three);
      fprintf(f->f, " %4ld", WITH1->basepaired);
      fprintf(f->f, " %4ld\n", WITH1->hstnum);
      p = p->next;
    }
    s = s->next;
  }
}



Static pairing *findlink(n, pa)
long n;
pairing *pa;
{
  pairing *Result;
  pairing *s = pa;
  boolean done = false;

  while (!done) {
    /*

*/
    if (s->i == n) {
      Result = s;
      done = true;
    } else {
      if (s->next == NULL) {
	printf("linkct: can't locate pairing %ld\n", n);
	halt();
      }
      s = s->next;
    }
  }
  return Result;
}



Static Void linkct(st)
structure **st;
{
  pairing *a_, *b;
  structure *WITH;

  WITH = *st;
  a_ = WITH->pairs;
  b = a_;

}


#define dot             '.'

#define thedecimal      1
#define thewidth        5



Static Void buildnames(st)
structure **st;
{
  stringDelila anumber, helix;
  pairing *last;
  structure *s, *WITH;
  _TEXT TEMP;

  printf("Building names of structures:\n");


  clearstring(&helix);
  helix.letters[0] = 'h';
  helix.letters[1] = 'e';
  helix.letters[2] = 'l';
  helix.letters[3] = 'i';
  helix.letters[4] = 'x';
  helix.length = 5;

  s = *st;
  while (s != NULL) {
    WITH = s;

    /*










*/

    copystring(helix, &WITH->nametag);
    attachcharacterstring(dot, &WITH->nametag);
    attachstring(&WITH->nametag, WITH->id);

    /*
*/
    attachcharacterstring('#', &WITH->nametag);
    numberstring(WITH->sequnumber, &anumber);
    attachstring(&WITH->nametag, anumber);

    attachcharacterstring('[', &WITH->nametag);

    numberstring(WITH->pairs->hstnum, &anumber);
    attachstring(&WITH->nametag, anumber);
    attachcharacterstring(dot, &WITH->nametag);

    last = findlink((*st)->length, (*st)->pairs);
    /*


*/
    numberstring(last->hstnum, &anumber);
    attachstring(&WITH->nametag, anumber);
    attachcharacterstring(']', &WITH->nametag);


    anumber.length = thewidth;
    numberrealstring(&anumber, WITH->energy, 1L, (long)thedecimal);

    attachstring(&WITH->nametag, anumber);

    printf("  ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->nametag);
    putchar('\n');

    s = s->next;
  }
}

#undef dot
#undef thedecimal
#undef thewidth



Static Void buildcoordinates(book, st)
_TEXT *book;
structure **st;
{
  /*
*/
  piece *apiece;
  pairing *c_;
  boolean done;
  structure *s;
  long theline;
  long thenumber = 0;
  piekey *WITH;
  name *WITH1;
  _TEXT TEMP;

  printf("Building coordinate systems of structures:\n");


  brinit(book, &theline);
  apiece = (piece *)Malloc(sizeof(piece));

  while (!BUFEOF(book->f)) {
    getpiece(book, &theline, &apiece);
    if (BUFEOF(book->f)) {
      if (s != NULL) {
	printf("buildnames: end of book found before end of structures\n");
	halt();
      }
      continue;
    }


    s = *st;
    done = false;
    thenumber++;
    while (!done) {
      if (isnamestring(apiece->key.hea.keynam, s->id) &&
	  thenumber == s->sequnumber) {
	/*


*/

	WITH = &apiece->key;
	WITH1 = &WITH->hea.keynam;
/* p2c: mfoldfea.p, line 2623: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
	printf("%.*s", WITH1->length, WITH1->letters);
	printf(" %ld", WITH->piebeg);
	printf(" %ld", WITH->pieend);
	switch (WITH->piedir) {

	case minus:
	  printf(" %2d", -1);
	  break;

	case plus:
	  printf(" %2d", 1);
	  break;
	}
	putchar(' ');
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &s->idnumber);
	printf(" sequence number: %ld\n", thenumber);


	if (s->length != piecelength(apiece)) {
	  printf(
	    "piece length in book (%ld) does not match piece length of structure (%ld)\n",
	    piecelength(apiece), s->length);
	  halt();
	}

	c_ = s->pairs;
	while (c_ != NULL) {
	  c_->hstnum = inttopie(c_->i, apiece);
	  switch (WITH->piedir) {

	  case minus:
	    s->orientation = -1;
	    break;

	  case plus:
	    s->orientation = 1;
	    break;
	  }
	  /*


*/
	  c_ = c_->next;
	}

      }

      s = s->next;
      if (s == NULL)
	done = true;
    }
    /*






*/



    clearpiece(&apiece);
  }
}



Static Void buildfeatures(thefile, st)
_TEXT *thefile;
structure *st;
{
  /*
*/
  pairing *c_;
  definetype *d;
  featuretype *f;
  structure *s;

  printf("Writing features\n");


  d = (definetype *)Malloc(sizeof(definetype));
  cleardefinition(&d);
  d->background.letters[0] = '-';
  d->background.length = 1;


  f = (featuretype *)Malloc(sizeof(featuretype));
  clearfeature(&f);

  s = st;
  while (s != NULL) {
    copystring(s->nametag, &d->nametag);
    c_ = s->pairs;


    clearstring(&d->negparts);
    clearstring(&d->posparts);
    d->marks = 0;
    while (c_ != NULL) {
      if (c_->basepaired > 0) {
	if (c_->i < c_->basepaired)
	  attachcharacterstring('(', &d->negparts);
	else
	  attachcharacterstring(')', &d->negparts);
	d->marks++;
	d->locations[d->marks - 1] = c_->i - 1.0;
      }
      c_ = c_->next;
    }
    copystring(d->negparts, &d->posparts);
    copystring(s->id, &f->id);
    copystring(s->nametag, &f->nametag);


    f->coordinate = s->pairs->hstnum;
    f->orientation = s->orientation;
    writeadefinition(thefile, d);
    writeafeature(thefile, f);
    fprintf(thefile->f, "\n\n");

    s = s->next;
  }
}


/* Local variables for PatentMessage: */
struct LOC_PatentMessage {
  _TEXT *f;
} ;

Local Void makebar(LINK)
struct LOC_PatentMessage *LINK;
{
  fprintf(LINK->f->f,
    "**********************************************************************\n");
}



Static Void PatentMessage(f_)
_TEXT *f_;
{
  /*

*/
  struct LOC_PatentMessage V;

  V.f = f_;
  makebar(&V);
  fprintf(V.f->f,
    "* By downloading this code you agree to the Source Code Use License: *\n");
  fprintf(V.f->f,
    "* https://alum.mit.edu/www/toms/Source_Code_Use_License.txt           *\n");
  fprintf(V.f->f,
    "* Contact: https://alum.mit.edu/www/toms/contacts.html                *\n");
  makebar(&V);
}



Static Void themain(cootab, book, mfoldfeatures)
_TEXT *cootab, *book, *mfoldfeatures;
{
  /*

*/
  structure *st;
  _TEXT TEMP;

  printf("mfoldfea %4.2f\n", version);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  PatentMessage(&TEMP);
  /*







*/

  if (*cootab->name != '\0') {
    if (cootab->f != NULL)
      cootab->f = freopen(cootab->name, "r", cootab->f);
    else
      cootab->f = fopen(cootab->name, "r");
  } else
    rewind(cootab->f);
  if (cootab->f == NULL)
    _EscIO2(FileNotFound, cootab->name);
  RESETBUF(cootab->f, Char);
  if (*mfoldfeatures->name != '\0') {
    if (mfoldfeatures->f != NULL)
      mfoldfeatures->f = freopen(mfoldfeatures->name, "w", mfoldfeatures->f);
    else
      mfoldfeatures->f = fopen(mfoldfeatures->name, "w");
  } else {
    if (mfoldfeatures->f != NULL)
      rewind(mfoldfeatures->f);
    else
      mfoldfeatures->f = tmpfile();
  }
  if (mfoldfeatures->f == NULL)
    _EscIO2(FileNotFound, mfoldfeatures->name);
  SETUPBUF(mfoldfeatures->f, Char);
  fprintf(mfoldfeatures->f, "* mfoldfea %4.2f\n", version);

  readcoordinatetable(cootab, &st);
  buildcoordinates(book, &st);
  /*



*/

  buildnames(&st);
  buildfeatures(mfoldfeatures, st);

  printf("mfoldfea is done.\n");

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  mfoldfeatures.f = NULL;
  strcpy(mfoldfeatures.name, "mfoldfeatures");
  book.f = NULL;
  strcpy(book.name, "book");
  cootab.f = NULL;
  strcpy(cootab.name, "cootab");
  themain(&cootab, &book, &mfoldfeatures);
_L1:
  if (cootab.f != NULL)
    fclose(cootab.f);
  if (book.f != NULL)
    fclose(book.f);
  if (mfoldfeatures.f != NULL)
    fclose(mfoldfeatures.f);
  exit(EXIT_SUCCESS);
}












/* End. */
