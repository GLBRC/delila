/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "index.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         9.33
/*






*/



/*

































































*/



#define mapmax          9000000L
/*
*/

#define columnfiller    '-'
/*
*/
#define columnseparation  ' '

#define pienumwid       3
#define posnumwid       8



#define dnamax          1024
#define namelength      100
#define linelength      200





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

/* p2c: index.p, line 154:
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



typedef long position;

/*
*/

typedef struct pdata {
  piece *p;
  long n;
} pdata;

typedef struct place {
  pdata *pd;
  dnastring *dnalink;
  unsigned dnaloc : 11;
} place;


Static _TEXT book, ind, indexp;
Static long theline;


Static long window, pbefore, pwindow, pafter;
/*
*/
Static boolean teaching, firstoligo;

Static long numberofoligos, numberofsequences, numberoflinears;

/*

*/

/*
*/
Static place map[mapmax];

/*
























*/






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



Static long copylines(fin, fout, n)
_TEXT *fin, *fout;
long n;
{
  /*
*/
  long index = 0;

  while (!BUFEOF(fin->f) && index < n) {
    copyaline(fin, fout);
    index++;
  }

  return index;
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






Static base stepbase(startdna, dna, d)
dnastring *startdna, **dna;
dnarange *d;
{
  /*




*/
  long TEMP;

  if (*d != dnamax && *d != (*dna)->length) {
    (*d)++;
    TEMP = *d - 1;
    return ((base)P_getbits_UB((*dna)->part, TEMP, 1, 3));
  }
  *d = 1;
  *dna = (*dna)->next;
  if (*dna == NULL)
    *dna = startdna;
  TEMP = *d - 1;
  return ((base)P_getbits_UB((*dna)->part, TEMP, 1, 3));
}




Static Void missparam(param)
_TEXT *param;
{
  /*

*/
  if (BUFEOF(param->f)) {
    printf(" missing parameter\n");
    halt();
  }
}


Static Void readindexp()
{
  if (*indexp.name != '\0') {
    if (indexp.f != NULL)
      indexp.f = freopen(indexp.name, "r", indexp.f);
    else
      indexp.f = fopen(indexp.name, "r");
  } else
    rewind(indexp.f);
  if (indexp.f == NULL)
    _EscIO2(FileNotFound, indexp.name);
  RESETBUF(indexp.f, Char);
  if (!BUFEOF(indexp.f)) {
    missparam(&indexp);
    fscanf(indexp.f, "%ld%*[^\n]", &window);
    getc(indexp.f);
    missparam(&indexp);
    fscanf(indexp.f, "%ld%*[^\n]", &pbefore);
    getc(indexp.f);
    missparam(&indexp);
    fscanf(indexp.f, "%ld%*[^\n]", &pwindow);
    getc(indexp.f);
    missparam(&indexp);
    fscanf(indexp.f, "%ld%*[^\n]", &pafter);
    getc(indexp.f);
  } else {
    window = 10;
    pbefore = 0;
    pwindow = 10;
    pafter = 0;
  }

  if (window <= 0) {
    printf("alphabetizing window must be positive integer\n");
    halt();
  }
  if (pbefore < 0 || pwindow < 0 || pafter < 0) {
    printf("printing windows must be zero or positive integers\n");
    halt();
  }
  if (pwindow != window) {
    printf(" warning: printing window size (%ld)\n", pwindow);
    printf(" is not equal to alphabetizing window size (%ld).\n", window);
  }

  if (P_peek(indexp.f) == 't')
    teaching = true;
  else
    teaching = false;
  fscanf(indexp.f, "%*[^\n]");

  getc(indexp.f);
  if (P_peek(indexp.f) == 'f')
    firstoligo = true;
  else {
    firstoligo = false;

  }
}


Static Void initialize()
{
  printf(" index %4.2f\n", version);
  brinit(&book, &theline);
  readnumber = true;
  skipunnum = true;

  if (*ind.name != '\0') {
    if (ind.f != NULL)
      ind.f = freopen(ind.name, "w", ind.f);
    else
      ind.f = fopen(ind.name, "w");
  } else {
    if (ind.f != NULL)
      rewind(ind.f);
    else
      ind.f = tmpfile();
  }
  if (ind.f == NULL)
    _EscIO2(FileNotFound, ind.name);
  SETUPBUF(ind.f, Char);

  readindexp();
}


/* Local variables for readsequences: */
struct LOC_readsequences {
  pdata *pied;
  dnastring *dn;
  dnarange lo;
  position m;
} ;

Local Void fillmap(LINK)
struct LOC_readsequences *LINK;
{
  place *WITH;

  WITH = &map[LINK->m-1];
  WITH->pd = LINK->pied;
  WITH->dnalink = LINK->dn;
  WITH->dnaloc = LINK->lo;
}


Static Void readsequences()
{
  /*


*/
  struct LOC_readsequences V;
  piece *pie;
  long length;
  base b;
  pdata *WITH;
  position FORLIM;

  numberofoligos = 0;
  numberofsequences = 0;
  numberoflinears = 0;

  while (!BUFEOF(book.f)) {
    pie = (piece *)Malloc(sizeof(piece));
    getpiece(&book, &theline, &pie);
    if (BUFEOF(book.f) || !numbered && skipunnum) {

      Free(pie);
      continue;
    }
    numberofsequences++;


    clearheader(&pie->key.hea);


    V.pied = (pdata *)Malloc(sizeof(pdata));
    WITH = V.pied;


    WITH->p = pie;
    WITH->n = number;

    if (WITH->p->key.piecon == linear)
      numberoflinears++;
    V.m = numberofoligos + 1;
    V.dn = pie->dna;
    V.lo = 1;
    fillmap(&V);

    if (firstoligo) {
      numberofoligos = V.m;

      if (numberofoligos > mapmax) {
	printf(" more than mapmax = %ld bases in the book", mapmax);
	printf(" can not be handled by index.\n");
	halt();
      }
      continue;
    }
    length = piecelength(pie);

    if (numberofoligos + length > mapmax) {
      printf(" more than mapmax = %ld bases in the book", mapmax);
      printf(" can not be handled by index.\n");
      halt();
    }


    FORLIM = numberofoligos + length;
    for (V.m = numberofoligos + 2; V.m <= FORLIM; V.m++) {
      b = stepbase(pie->dna, &V.dn, &V.lo);
      fillmap(&V);
    }
    numberofoligos += length;
  }
}


Static Void writeheader()
{
  /*
*/
  fprintf(ind.f, "* index %4.2f of \n", version);
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
  copyaline(&book, &ind);

  fprintf(ind.f, "*\n");
  fprintf(ind.f, "* %3ld is the length of the alphabetized window\n", window);
  fprintf(ind.f, "* %3ld bases before the window start are printed\n",
	  pbefore);
  fprintf(ind.f, "* %3ld bases are in the central printed window\n", pwindow);
  fprintf(ind.f, "* %3ld bases after the window are printed\n", pafter);
  fprintf(ind.f, "* %3ld oligos are in the book \n", numberofoligos);

  fprintf(ind.f, "* %3ld sequence", numberofsequences);
  if (numberofsequences != 1)
    putc('s', ind.f);
  fprintf(ind.f, " indexed:\n");

  fprintf(ind.f, "* %3ld linear and %ld circular\n",
	  numberoflinears, numberofsequences - numberoflinears);

  if (teaching) {
    fprintf(ind.f, "* teaching mode is on.\n\n");
    fprintf(ind.f, "* I will first pass a window across all the\n");
    fprintf(ind.f, "* sequences in the book, so that every oligo is\n");
    fprintf(ind.f, "* under the window once.\n\n");
    fprintf(ind.f, "* seq # is the number of the sequence in the book");
    fprintf(ind.f, " that this view comes from;\n");
    fprintf(ind.f, "* pos   is the position of the 5' base of the view");
    fprintf(ind.f, " in the sequence;\n");
    fprintf(ind.f, "* dir   is the direction of this view relative");
    fprintf(ind.f, " to the piece coordinates;\n");
    fprintf(ind.f, "* similarity is the number of bases that match");
    fprintf(ind.f, " between this view\n");
    fprintf(ind.f,
	    "* and the previous view (only up to the first mismatch).\n");
  }

  if (firstoligo)
    fprintf(ind.f, "* first oligo is the only one used in sorting\n");
  else
    fprintf(ind.f, "* all oligos are used for sorting\n");

  fprintf(ind.f, "*\n");

  /*




*/
  fprintf(ind.f, "*%*csequence %*c%*c#%*cposition dir similarity name\n",
	  (int)pbefore, ' ', (int)(pwindow + pafter - 8), ' ', pienumwid, ' ',
	  posnumwid, ' ');
}


Static boolean lessthan(alow, blow)
position alow, blow;
{
  /*
*/
  boolean Result;

  dnastring *adna, *bdna;
  dnarange aloc, bloc;

  base abase, bbase;
  boolean aline, bline;
  dnastring *astart, *bstart;

  long w = 1;
  boolean done = false;
  place *WITH;
  piece *WITH1;

  /*

*/
  WITH = &map[alow-1];
  WITH1 = WITH->pd->p;
  aline = (WITH1->key.piecon == linear);
  astart = WITH1->dna;
  adna = WITH->dnalink;
  aloc = WITH->dnaloc;
  abase = (base)P_getbits_UB(adna->part, aloc - 1, 1, 3);

  /*

*/
  WITH = &map[blow-1];
  WITH1 = WITH->pd->p;
  bline = (WITH1->key.piecon == linear);
  bstart = WITH1->dna;
  bdna = WITH->dnalink;
  bloc = WITH->dnaloc;
  bbase = (base)P_getbits_UB(bdna->part, bloc - 1, 1, 3);




  while (!done) {
    if ((int)abase != (int)bbase) {
      Result = ((int)abase < (int)bbase);
      done = true;
      break;
    }
    w++;
    if (w > window) {
      Result = false;
      done = true;
      break;
    }

    abase = stepbase(astart, &adna, &aloc);
    bbase = stepbase(bstart, &bdna, &bloc);

    /*
*/
    if (aloc == 1) {
      if (adna == astart) {
	if (aline) {
	  Result = true;
	  done = true;
	}
      }
    }

    if (bloc == 1) {
      if (bdna == bstart) {
	if (bline) {
	  Result = false;
	  done = true;
	}
      }
    }
    /*


*/
  }
  return Result;
}


Static Void swap_(a_, b)
position a_, b;
{
  place hold;

  hold = map[a_-1];
  map[a_-1] = map[b-1];
  map[b-1] = hold;
}



Static Void quicksort(left, right)
position left, right;
{
  /*












*/
  position lower = left;
  position upper, center;

  center = (left + right) / 2;
  upper = right;

  do {
    while (lessthan(lower, center))
      lower++;
    while (lessthan(center, upper))
      upper--;

    if (lower <= upper) {
      if (lower == center)
	center = upper;
      else if (upper == center)
	center = lower;
      swap_(lower, upper);
      lower++;
      upper--;
    }

  } while (lower <= upper);

  if (left < upper)
    quicksort(left, upper);
  if (lower < right)
    quicksort(lower, right);
}


/* Local variables for writeindex: */
struct LOC_writeindex {
  position current;




  piece *ppie, *cpie;

  dnastring *pdn, *cdn;
  dnarange plo, clo;

  dnastring *ppiestart, *cpiestart;
  boolean pcircle, ccircle;


  dnastring *dn;
  dnarange lo;


  long windowposition;

  long steps;
  boolean offend;
} ;

/*
*/

Local Void setup(LINK)
struct LOC_writeindex *LINK;
{
  long int_ = 0;
  long bk;

  /*
*/
  long j = 0;
  place *WITH;

  WITH = &map[LINK->current-1];


  LINK->cdn = WITH->dnalink;
  LINK->clo = WITH->dnaloc;
  LINK->cpie = WITH->pd->p;
  LINK->cpiestart = LINK->cpie->dna;
  LINK->ccircle = (LINK->cpie->key.piecon == circular);
  LINK->dn = LINK->cpiestart;
  while (LINK->dn != LINK->cdn) {
    int_ += dnamax;
    LINK->dn = LINK->dn->next;
  }
  int_ += LINK->clo;
  LINK->windowposition = int_;

  /*
*/
  if (pbefore > 0) {
    int_ -= pbefore;
    if (int_ < 1) {
      if (LINK->ccircle) {
	LINK->steps = pbefore;
	while (int_ < 1)
	  int_ += piecelength(LINK->cpie);
      } else {
	for (bk = 1; bk <= 1 - int_; bk++)
	  putc(columnfiller, ind.f);

	int_ = 1;
	LINK->steps = LINK->windowposition - int_;
      }
    } else {

      LINK->steps = pbefore;
    }
  } else
    LINK->steps = 0;

  /*

*/
  LINK->dn = LINK->cpiestart;
  while (j + dnamax < int_) {
    j += dnamax;
    LINK->dn = LINK->dn->next;
  }
  LINK->lo = int_ - j - 1;
}

Local Void wacycle(LINK)
struct LOC_writeindex *LINK;
{
  /*
*/
  base ba;
  long counter, FORLIM;

  while (LINK->steps > 0) {
    ba = stepbase(LINK->cpiestart, &LINK->dn, &LINK->lo);
    LINK->steps--;

    /*
*/
    if (LINK->lo != 1) {
      fputc(basetochar(ba), ind.f);
      continue;
    }
    if (LINK->dn != LINK->cpiestart) {
      fputc(basetochar(ba), ind.f);
      continue;
    }
    if (LINK->ccircle) {
      fputc(basetochar(ba), ind.f);
      continue;
    }
    LINK->offend = true;

    if (LINK->steps >= 0) {
      FORLIM = LINK->steps + 1;
      for (counter = 1; counter <= FORLIM; counter++)
	putc(columnfiller, ind.f);
    }
    LINK->steps = 0;
  }
}

Local Void writebefore(LINK)
struct LOC_writeindex *LINK;
{
  while (LINK->steps > 0) {
    fputc(basetochar(stepbase(LINK->cpiestart, &LINK->dn, &LINK->lo)), ind.f);
    LINK->steps--;
  }
}

Local Void writewindow(LINK)
struct LOC_writeindex *LINK;
{
  LINK->offend = false;
  if (pwindow <= 0)
    return;

  fputc(basetochar(stepbase(LINK->cpiestart, &LINK->dn, &LINK->lo)), ind.f);
  LINK->steps = pwindow - 1;
  wacycle(LINK);
}

Local Void writeafter(LINK)
struct LOC_writeindex *LINK;
{
  long counter, FORLIM;

  if (pafter <= 0)
    return;
  if (LINK->offend) {
    FORLIM = pafter;
    for (counter = 1; counter <= FORLIM; counter++)
      putc(columnfiller, ind.f);
  } else {
    LINK->steps = pafter;
    wacycle(LINK);
  }
}

Local Void writepiename(LINK)
struct LOC_writeindex *LINK;
{
  boolean done = false;
  long i = 0;
  name *WITH;

  WITH = &LINK->cpie->key.hea.keynam;

  while (!done) {
    i++;
    putc(WITH->letters[i-1], ind.f);
    if (i >= WITH->length)
      done = true;
    else if (WITH->letters[i-1] == ' ')
      done = true;
  }

}

Local Void writepiecenumber(LINK)
struct LOC_writeindex *LINK;
{
  fprintf(ind.f, "%*ld", pienumwid, map[LINK->current-1].pd->n);
}

Local Void writeposition(LINK)
struct LOC_writeindex *LINK;
{
  fprintf(ind.f, "%*ld",
	  posnumwid, inttopie(LINK->windowposition, LINK->cpie));
}

Local Void writedirection(LINK)
struct LOC_writeindex *LINK;
{
  putc(' ', ind.f);
  if (LINK->cpie->key.piedir == plus)
    putc('+', ind.f);
  else
    putc('-', ind.f);
}

Local Void writesimilarity(LINK)
struct LOC_writeindex *LINK;
{
  /*
*/
  long similarity = 0;
  boolean done = false;


  dnastring *adn, *bdn;
  dnarange alo, blo;
  base abase, bbase;

  adn = LINK->pdn;
  alo = LINK->plo;

  bdn = LINK->cdn;
  blo = LINK->clo;


  abase = (base)P_getbits_UB(adn->part, alo - 1, 1, 3);
  bbase = (base)P_getbits_UB(bdn->part, blo - 1, 1, 3);


  while (!done) {
    if ((int)abase != (int)bbase) {
      done = true;
      break;
    }
    similarity++;
    if (similarity >= window) {
      done = true;
      break;
    }
    abase = stepbase(LINK->ppiestart, &adn, &alo);
    bbase = stepbase(LINK->cpiestart, &bdn, &blo);

    if (alo == 1) {
      if (adn == LINK->ppiestart) {
	if (!LINK->pcircle)
	  done = true;
      }
    }


    if (blo == 1) {
      if (bdn == LINK->cpiestart) {
	if (!LINK->ccircle)
	  done = true;
      }
    }

  }


  fprintf(ind.f, "%6ld", similarity);
}

Local Void updateprevious(LINK)
struct LOC_writeindex *LINK;
{
  LINK->ppie = LINK->cpie;
  LINK->ppiestart = LINK->cpiestart;
  LINK->pdn = LINK->cdn;
  LINK->plo = LINK->clo;
  LINK->pcircle = LINK->ccircle;
}

Local Void writeline_(LINK)
struct LOC_writeindex *LINK;
{
  setup(LINK);
  /*




*/
  writebefore(LINK);
  putc(columnseparation, ind.f);
  writewindow(LINK);
  putc(columnseparation, ind.f);
  writeafter(LINK);
  putc(columnseparation, ind.f);
  writepiecenumber(LINK);
  putc(columnseparation, ind.f);
  writeposition(LINK);
  putc(columnseparation, ind.f);
  writedirection(LINK);
  putc(columnseparation, ind.f);
  writesimilarity(LINK);
  putc(columnseparation, ind.f);
  writepiename(LINK);
  putc('\n', ind.f);
  updateprevious(LINK);
}



Static Void writeindex()
{
  struct LOC_writeindex V;
  place *WITH;
  position FORLIM;

  /*

*/
  WITH = &map[numberofoligos-1];

  V.pdn = WITH->dnalink;
  V.plo = WITH->dnaloc;
  V.ppie = WITH->pd->p;
  V.ppiestart = V.ppie->dna;
  V.pcircle = (V.ppie->key.piecon == circular);
  FORLIM = numberofoligos;
  for (V.current = 1; V.current <= FORLIM; V.current++)
    writeline_(&V);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  indexp.f = NULL;
  strcpy(indexp.name, "indexp");
  ind.f = NULL;
  strcpy(ind.name, "ind");
  book.f = NULL;
  strcpy(book.name, "book");
  initialize();
  readsequences();
  writeheader();

  if (numberofsequences == 0)
    printf(" no numbered sequences in this book\n");
  else {
    if (teaching) {
      writeindex();
      /*
*/

      fprintf(ind.f, "\f\n");
      fprintf(ind.f, " now i will sort the index alphabetically.\n");
    }

    quicksort(1L, numberofoligos);

    if (teaching) {
      fprintf(ind.f, " i am done sorting.\n");
      fprintf(ind.f, " how long would it have taken you?\n");
      fprintf(ind.f, " the sorted index is like a telephone directory.\n");
      fprintf(ind.f, " it looks like this:\n\n");
    }

    writeindex();
  }
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (ind.f != NULL)
    fclose(ind.f);
  if (indexp.f != NULL)
    fclose(indexp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
