/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rep.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*



*/



#define version         1.75
/*

*/



/*



























































*/



#define headerlines     7
#define defdmin         0
/*
*/

#define defdmax         LONG_MAX
/*
*/

#define defminlength    3
/*
*/

#define defmaxlength    LONG_MAX
/*
*/

#define debugging       true

#define prime           '"'





#define dnamax          3000
#define namelength      20
#define linelength      80




typedef struct prec {
  Char mode, typbk, energy;
  /*
*/
  long minlength, maxlength, dmax, dmin;
} prec;

typedef struct reprec {
  long x5p;
  /*
*/
  long y5p;
  /*
*/
  long rlength, distance;
  /*

*/
  double deltagee;
} reprec;






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

/* p2c: rep.p, line 170:
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





Static _TEXT xbook, ybook, hlist, fout, pout, repp;







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




Static Char getto(thefile, ch)
_TEXT *thefile;
long *ch;
{
  /*
*/
  Char achar = ' ';

  while ((!P_inset(achar, ch)) & (!BUFEOF(thefile->f))) {
    fscanf(thefile->f, "%c%*[^\n]", &achar);
    getc(thefile->f);
    if (achar == '\n')
      achar = ' ';
  }
  if (P_inset(achar, ch))
    return achar;
  else
    return ' ';
}



Static Void skipstar(thefile)
_TEXT *thefile;
{
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



Static Void brreanum(thefile, reanum)
_TEXT *thefile;
double *reanum;
{
  skipstar(thefile);
  fscanf(thefile->f, "%lg%*[^\n]", reanum);
  getc(thefile->f);
}



Static Void brnumber(thefile, num)
_TEXT *thefile;
long *num;
{
  skipstar(thefile);
  fscanf(thefile->f, "%ld%*[^\n]", num);
  getc(thefile->f);
}



Static Void brname(thefile, nam)
_TEXT *thefile;
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
}



Static Void brline(thefile, l)
_TEXT *thefile;
line **l;
{
  long i = 0;
  long j;
  Char acharacter;
  long FORLIM;

  skipstar(thefile);
  while (!P_eoln(thefile->f)) {
    i++;
    acharacter = getc(thefile->f);
    if (acharacter == '\n')
      acharacter = ' ';
    (*l)->letters[i-1] = acharacter;
  }
  if (i < (*l)->length) {
    FORLIM = (*l)->length;
    for (j = i; j < FORLIM; j++)
      (*l)->letters[j] = ' ';
  }
  (*l)->length = i;
  (*l)->next = NULL;
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
}



Static Void brdirect(thefile, direct)
_TEXT *thefile;
direction *direct;
{
  Char ch;

  skipstar(thefile);
  fscanf(thefile->f, "%c%*[^\n]", &ch);
  getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == '+')
    *direct = plus;
  else
    *direct = minus;
}



Static Void brconfig(thefile, config)
_TEXT *thefile;
configuration *config;
{
  Char ch;

  skipstar(thefile);
  fscanf(thefile->f, "%c%*[^\n]", &ch);
  getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == 'l')
    *config = linear;
  else
    *config = circular;
}



Static Void brnotenumber(thefile, note)
_TEXT *thefile;
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
  if (P_peek(thefile->f) == 'n') {
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    return;
  }
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
  } while (P_peek(thefile->f) != 'n');
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
}



Static Void brnote(thefile, note)
_TEXT *thefile;
line **note;
{
  line *newnote, *previousnote;

  *note = NULL;
  if (P_peek(thefile->f) != 'n')
    return;
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  if (P_peek(thefile->f) == 'n') {
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    return;
  }
  getlineDelila(note);
  newnote = *note;
  while (P_peek(thefile->f) != 'n') {
    brline(thefile, &newnote);
    previousnote = newnote;

    getlineDelila(&newnote->next);
    newnote = newnote->next;
  }

  clearline(&newnote);
  previousnote->next = NULL;
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
}



Static Void brheader(thefile, hea)
_TEXT *thefile;
header *hea;
{
  brname(thefile, &hea->keynam);


  getlineDelila(&hea->fulnam);
  brline(thefile, &hea->fulnam);


  if (readnumber)
    brnotenumber(thefile, &hea->note);
  else
    brnote(thefile, &hea->note);
}



Static Void brpiekey(thefile, pie)
_TEXT *thefile;
piekey *pie;
{
  brheader(thefile, &pie->hea);
  brreanum(thefile, &pie->mapbeg);
  brconfig(thefile, &pie->coocon);
  brdirect(thefile, &pie->coodir);
  brnumber(thefile, &pie->coobeg);
  brnumber(thefile, &pie->cooend);
  brconfig(thefile, &pie->piecon);
  brdirect(thefile, &pie->piedir);
  brnumber(thefile, &pie->piebeg);
  brnumber(thefile, &pie->pieend);
}



Static Void brdna(thefile, dna)
_TEXT *thefile;
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
  ch = getto(thefile, P_addset(P_expset(SET, 0L), 'd'));
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
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
}



Static Void brpiece(thefile, pie)
_TEXT *thefile;
piece **pie;
{
  brpiekey(thefile, &(*pie)->key);
  if (numbered || !skipunnum)
    brdna(thefile, &(*pie)->dna);
}



Static Void brinit(book)
_TEXT *book;
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
}






Static Void getpiece(thefile, pie)
_TEXT *thefile;
piece **pie;
{
  Char ch;
  long SET[5];

  ch = getto(thefile, P_addset(P_expset(SET, 0L), 'p'));
  if (ch != ' ') {
    brpiece(thefile, pie);
    ch = getto(thefile, P_addset(P_expset(SET, 0L), 'p'));
  }
}


Local Void die()
{
  printf(" no helix list data\n");
  halt();
}






Static Void findcolon(thefile)
_TEXT *thefile;
{
  boolean found = false;

  while (!found) {
    if (BUFEOF(thefile->f)) {
      die();
      continue;
    }
    if (P_peek(thefile->f) != ':') {
      getc(thefile->f);
      continue;
    }
    getc(thefile->f);
    if (BUFEOF(thefile->f))
      die();
    else if (P_peek(thefile->f) == ' ') {
      getc(thefile->f);
      found = true;
    }
  }
}



Static Void gethelix(hlist, x, y, length, getenergy, energy, done)
_TEXT *hlist;
long *x, *y, *length;
boolean getenergy;
double *energy;
boolean *done;
{
  /*



*/
  if (BUFEOF(hlist->f)) {
    *done = true;
    return;
  }
  getc(hlist->f);
  if (P_peek(hlist->f) != ' ') {
    *done = true;
    return;
  }
  *done = false;
  findcolon(hlist);
  fscanf(hlist->f, "%ld", x);
  findcolon(hlist);
  fscanf(hlist->f, "%ld", y);
  findcolon(hlist);
  fscanf(hlist->f, "%ld", length);
  if (getenergy)
    fscanf(hlist->f, "%lg", energy);
  else
    *energy = 0.0;
  fscanf(hlist->f, "%*[^\n]");
  getc(hlist->f);
}



Static Void initbook(xbook, ybook, xseq, yseq)
_TEXT *xbook, *ybook;
piece **xseq, **yseq;
{
  /*
*/
  brinit(xbook);
  brinit(ybook);
  *xseq = (piece *)Malloc(sizeof(piece));
  *yseq = (piece *)Malloc(sizeof(piece));
}



Static boolean complines(a_, b)
_TEXT *a_, *b;
{
  /*







*/
  boolean Result;
  Char chara, charb;
  boolean done = false;

  if (BUFEOF(a_->f) & BUFEOF(b->f))
    return true;
  if (BUFEOF(a_->f) | BUFEOF(b->f))
    return false;
  while (!done) {
    if (P_eoln(a_->f))
      chara = ' ';
    else {
      chara = getc(a_->f);
      if (chara == '\n')
	chara = ' ';
    }

    if (P_eoln(b->f))
      charb = ' ';
    else {
      charb = getc(b->f);
      if (charb == '\n')
	charb = ' ';
    }

    if (P_eoln(a_->f) & P_eoln(b->f)) {
      done = true;
      Result = true;
    } else if (chara != charb) {
      done = true;
      Result = false;
    }
  }
  fscanf(a_->f, "%*[^\n]");
  getc(a_->f);
  fscanf(b->f, "%*[^\n]");
  getc(b->f);
  return Result;
}


Local Void gettitleline(xbook, ybook, hlist)
_TEXT *xbook, *ybook, *hlist;
{
  /*
*/
  if (*xbook->name != '\0') {
    if (xbook->f != NULL)
      xbook->f = freopen(xbook->name, "r", xbook->f);
    else
      xbook->f = fopen(xbook->name, "r");
  } else
    rewind(xbook->f);
  if (xbook->f == NULL)
    _EscIO2(FileNotFound, xbook->name);
  RESETBUF(xbook->f, Char);
  if (*ybook->name != '\0') {
    if (ybook->f != NULL)
      ybook->f = freopen(ybook->name, "r", ybook->f);
    else
      ybook->f = fopen(ybook->name, "r");
  } else
    rewind(ybook->f);
  if (ybook->f == NULL)
    _EscIO2(FileNotFound, ybook->name);
  RESETBUF(ybook->f, Char);
  if (*hlist->name != '\0') {
    if (hlist->f != NULL)
      hlist->f = freopen(hlist->name, "r", hlist->f);
    else
      hlist->f = fopen(hlist->name, "r");
  } else
    rewind(hlist->f);
  if (hlist->f == NULL)
    _EscIO2(FileNotFound, hlist->name);
  RESETBUF(hlist->f, Char);
  fscanf(hlist->f, "%*[^\n]");
  getc(hlist->f);
}

Local Void findtitle(hlist)
_TEXT *hlist;
{
  long n;

  for (n = 1; n <= 4; n++)
    getc(hlist->f);
}



Static Void hlistvsbooks(hlist, xbook, ybook)
_TEXT *hlist, *xbook, *ybook;
{
  /*

*/
  boolean xasx, yasy, xasy, yasx;

  gettitleline(xbook, ybook, hlist);
  findtitle(hlist);
  xasx = complines(xbook, hlist);
  findtitle(hlist);
  yasy = complines(ybook, hlist);
  gettitleline(xbook, ybook, hlist);
  findtitle(hlist);
  yasx = complines(ybook, hlist);
  findtitle(hlist);
  xasy = complines(xbook, hlist);
  if (!xasx)
    printf(" xbook input file does not correspond to xbook in hlist.\n");
  if (!yasy)
    printf(" ybook input file does not correspond to ybook in hlist.\n");
  if (xasy && !xasx)
    printf(" xbook input file corresponds to ybook in hlist.\n");
  if (yasx && !yasy)
    printf(" ybook input file corresponds to xbook in hlist.\n");
  if (xasy && yasx && !xasx && !yasy)
    printf(" xbook and ybook input files are reversed. try again.\n");
  if (!(xasx && yasy))
    halt();
}



Static Void readparameters(repp, para)
_TEXT *repp;
prec *para;
{
  if (*repp->name != '\0') {
    if (repp->f != NULL)
      repp->f = freopen(repp->name, "r", repp->f);
    else
      repp->f = fopen(repp->name, "r");
  } else
    rewind(repp->f);
  if (repp->f == NULL)
    _EscIO2(FileNotFound, repp->name);
  RESETBUF(repp->f, Char);
  if (BUFEOF(repp->f)) {
    printf(" the repp input file is empty.\n");
    halt();
  }

  fscanf(repp->f, "%c%*[^\n]", &para->mode);
  getc(repp->f);
  if (para->mode == '\n')
    para->mode = ' ';
  if (para->mode != 'd' && para->mode != 'i') {
    printf(" the mode parameter in repp should be d(irect) or i(nverted)\n");
    halt();
  }

  fscanf(repp->f, "%c%*[^\n]", &para->typbk);
  getc(repp->f);
  if (para->typbk == '\n')
    para->typbk = ' ';
  if (para->typbk != 'r' && para->typbk != 'u') {
    printf(" type of book parameter in repp should be r(elated) or u(nrelated)\n");
    halt();
  }

  fscanf(repp->f, "%c%*[^\n]", &para->energy);
  getc(repp->f);
  if (para->energy == '\n')
    para->energy = ' ';
  if (para->energy != 'e' && para->energy != 'n') {
    printf(" the third line of repp should have\n");
    printf(" e(nergies reported) or n(o energies reported)\n");
    halt();
  }

  /*
*/
  if (BUFEOF(repp->f))
    para->minlength = defminlength;
  else {
    fscanf(repp->f, "%ld%*[^\n]", &para->minlength);
    getc(repp->f);
  }
  if (BUFEOF(repp->f))
    para->maxlength = defmaxlength;
  else {
    fscanf(repp->f, "%ld%*[^\n]", &para->maxlength);
    getc(repp->f);
  }
  if (BUFEOF(repp->f))
    para->dmin = defdmin;
  else {
    fscanf(repp->f, "%ld%*[^\n]", &para->dmin);
    getc(repp->f);
  }
  if (BUFEOF(repp->f))
    para->dmax = defdmax;
  else {
    fscanf(repp->f, "%ld%*[^\n]", &para->dmax);
    getc(repp->f);
  }
}



Static Void checkhlistparams(hlist, para, getenergy)
_TEXT *hlist;
prec *para;
boolean *getenergy;
{
  /*



*/
  long i, helminlength;
  Char blank, gupairs, energycalculated;

  if (*hlist->name != '\0') {
    if (hlist->f != NULL)
      hlist->f = freopen(hlist->name, "r", hlist->f);
    else
      hlist->f = fopen(hlist->name, "r");
  } else
    rewind(hlist->f);
  if (hlist->f == NULL)
    _EscIO2(FileNotFound, hlist->name);
  RESETBUF(hlist->f, Char);
  for (i = 1; i <= 3; i++) {
    fscanf(hlist->f, "%*[^\n]");
    getc(hlist->f);
  }
  fscanf(hlist->f, "%ld", &helminlength);
  if (helminlength > para->minlength) {
    printf(" the minimum length in repp is less than in hlist\n");
    printf(" program will continue with minlength equal to the\n");
    printf(" minimum length from the helix program.\n");
    para->minlength = helminlength;
  }
  fscanf(hlist->f, "%*[^\n]");
  getc(hlist->f);
  blank = getc(hlist->f);
  if (blank == '\n')
    blank = ' ';
  gupairs = getc(hlist->f);
  if (gupairs == '\n')
    gupairs = ' ';
  if (gupairs == 'g') {
    if (para->mode == 'i') {
      printf(" gu pairs are allowed in hlist.\n");
      printf(" is that what you intended?\n");
      printf(" the program will run even if that is not what you wanted. \n");
    } else {
      printf(" your hlist has gu pairs.  analysis of direct\n");
      printf(" repeats cannot use an hlist that allows gupairs.\n");
      /*


*/
      halt();
    }
  }
  fscanf(hlist->f, "%*[^\n]");
  getc(hlist->f);
  blank = getc(hlist->f);
  if (blank == '\n')
    blank = ' ';
  energycalculated = getc(hlist->f);
  if (energycalculated == '\n')
    energycalculated = ' ';
  if (energycalculated == 'e' && para->energy == 'e')
    *getenergy = true;
  else
    *getenergy = false;
  fscanf(hlist->f, "%*[^\n]");
  getc(hlist->f);
}



Static Void foutheading(hlist, fout, getenergy, para)
_TEXT *hlist, *fout;
boolean *getenergy;
prec *para;
{
  long line_;

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
  if (*hlist->name != '\0') {
    if (hlist->f != NULL)
      hlist->f = freopen(hlist->name, "r", hlist->f);
    else
      hlist->f = fopen(hlist->name, "r");
  } else
    rewind(hlist->f);
  if (hlist->f == NULL)
    _EscIO2(FileNotFound, hlist->name);
  RESETBUF(hlist->f, Char);
  fprintf(fout->f, " rep %4.2f\n", version);
  for (line_ = 1; line_ <= headerlines; line_++) {
    putc(' ', fout->f);
    copyaline(hlist, fout);
  }
  switch (para->mode) {

  case 'i':
    fprintf(fout->f, "   i : mode, inverted repeats found\n");
    break;

  case 'd':
    fprintf(fout->f, "   d : mode, direct repeats found\n");
    break;
  }

  switch (para->typbk) {

  case 'u':
    fprintf(fout->f, "   u : xbook and ybook are unrelated\n");
    break;

  case 'r':
    fprintf(fout->f, "   r : xbook and ybook are related\n");
    break;
  }

  switch (para->energy) {

  case 'e':
    if (*getenergy)
      fprintf(fout->f, "   e : \"energies\" reported in fout\n");
    else
      fprintf(fout->f,
	      "   \"energies\" requested but not available from hlist\n");
    break;

  case 'n':
    fprintf(fout->f, "   n : no \"energies\" reported in fout\n");
    break;
  }
  fprintf(fout->f, " %3ld = minimum repeat length\n", para->minlength);
  fprintf(fout->f, " %3ld = maximum repeat length\n", para->maxlength);
  fprintf(fout->f, " %3ld = minimum distance between repeats\n", para->dmin);
  fprintf(fout->f, " %3ld = maximum distance between repeats\n\n", para->dmax);
}



Static Void poutheading(hlist, pout, getenergy, para)
_TEXT *hlist, *pout;
boolean *getenergy;
prec *para;
{
  long line_;

  if (*pout->name != '\0') {
    if (pout->f != NULL)
      pout->f = freopen(pout->name, "w", pout->f);
    else
      pout->f = fopen(pout->name, "w");
  } else {
    if (pout->f != NULL)
      rewind(pout->f);
    else
      pout->f = tmpfile();
  }
  if (pout->f == NULL)
    _EscIO2(FileNotFound, pout->name);
  SETUPBUF(pout->f, Char);
  if (*hlist->name != '\0') {
    if (hlist->f != NULL)
      hlist->f = freopen(hlist->name, "r", hlist->f);
    else
      hlist->f = fopen(hlist->name, "r");
  } else
    rewind(hlist->f);
  if (hlist->f == NULL)
    _EscIO2(FileNotFound, hlist->name);
  RESETBUF(hlist->f, Char);
  fprintf(pout->f, " rep %4.2f\n", version);
  for (line_ = 1; line_ <= headerlines; line_++) {
    putc(' ', pout->f);
    copyaline(hlist, pout);
  }

  switch (para->energy) {

  case 'e':
    if (*getenergy)
      fprintf(pout->f, "   e: \"energies\" reported in pout\n");
    else
      fprintf(pout->f,
	      "   \"energies\" requested but not available from hlist\n");
    break;

  case 'n':
    fprintf(pout->f, "   n: no \"energies\" reported in pout\n");
    break;
  }
  fprintf(pout->f, "\n %3ld = minimum palindrome length\n", para->minlength);
  fprintf(pout->f, " %3ld = maximum palindrome length\n\n", para->maxlength);
  fprintf(pout->f, " note that the half-repeat length of a palindrome\n");
  fprintf(pout->f, " is half that of an inverted repeat.  thus if the\n");
  fprintf(pout->f, " minimum repeat length is 4, agct will be reported\n");
  fprintf(pout->f, " in pout but agnnnct will not be reported in fout\n\n");
}




Static Void checkseqdata(para, xseq, yseq, fout)
prec para;
piece *xseq, *yseq;
_TEXT *fout;
{
  if (para.typbk == 'r' &&
      (xseq->key.mapbeg != yseq->key.mapbeg ||
       xseq->key.coocon != yseq->key.coocon ||
       xseq->key.coodir != yseq->key.coodir ||
       xseq->key.coobeg != yseq->key.coobeg ||
       xseq->key.cooend != yseq->key.cooend)) {
    printf(" book coordinates indicate that xbook and ybook\n");
    printf(" do not meet the criteria of relatedness,\n");
    printf(" although the parameter file calls\n");
    printf(" for them to be treated as related.\n");
    printf(" please see the explanation of\n");
    printf(" *related* in delman.describe.rep.\n");
    halt();
  }
  if (para.typbk == 'r' && para.mode == 'i' &&
      xseq->key.piedir != yseq->key.piedir) {
    printf(" analysis of inverted repeats with\n");
    printf(" rep requires that the books have the \n");
    printf(" same orientation\n");
    halt();
  }
  if (para.typbk == 'r' && para.mode == 'd' &&
      xseq->key.piedir == yseq->key.piedir) {
    printf(" analysis of direct repeats with rep\n");
    printf(" requires that the books have opposite\n");
    printf(" directions.\n");
    halt();
  }








  if (para.typbk == 'u' && para.mode == 'd' &&
      xseq->key.piedir == yseq->key.piedir) {
    printf(" generally speaking, analysis of direct repeats\n");
    printf(" requires that xbook and ybook have\n");
    printf(" different directions.  here xbook and\n");
    printf(" ybook have the same direction.  are you\n");
    printf(" sure that no mistake has been made?\n");
    printf(" the program continues, but caution is urged\n");
    fprintf(fout->f, "        * * * warning * * * \n\n");
    fprintf(fout->f, " generally speaking, analysis of direct repeats\n");
    fprintf(fout->f, " requires that xbook and ybook have\n");
    fprintf(fout->f, " different directions.  here xbook and\n");
    fprintf(fout->f, " ybook have the same direction.  are you\n");
    fprintf(fout->f, " sure that no mistake has been made?\n");
    fprintf(fout->f, " the program continues, but caution is urged\n\n");
    fprintf(fout->f, " note also:  this warning adds an additional\n");
    fprintf(fout->f, " 19 lines of text to the body of the file fout\n");
    fprintf(fout->f, " from the program rep.  should you wish to\n");
    fprintf(fout->f, " use a fout file that contains this warning\n");
    fprintf(fout->f, " as input for a program designed to use as input\n");
    fprintf(fout->f, " the standard fout file from program rep,\n");
    fprintf(fout->f, " it may be necessary first to delete 19 lines\n");
    fprintf(fout->f, " beginning with * * * warning * * *\n\n\n");
  }
  if (para.typbk != 'u' || para.mode != 'i' ||
      xseq->key.piedir == yseq->key.piedir)
    return;
  printf(" generally speaking, analysis of inverted \n");
  printf(" repeats requires that xbook and ybook have the\n");
  printf(" same direction.  here xbook and ybook have \n");
  printf(" different directions.  are you sure that no \n");
  printf(" mistake has been made?  the program continues,\n");
  printf(" but caution is urged.\n");
  fprintf(fout->f, "        * * * warning * * * \n\n");
  fprintf(fout->f, " generally speaking, analysis of inverted \n");
  fprintf(fout->f, " repeats requires that xbook and ybook have the \n");
  fprintf(fout->f, " same direction.  here xbook and ybook have \n");
  fprintf(fout->f, " different directions.  are you sure that no \n");
  fprintf(fout->f, " mistake has been made?  the program continues,\n");
  fprintf(fout->f, " but caution is urged.\n\n");
  fprintf(fout->f, " note also:  this warning adds an additional\n");
  fprintf(fout->f, " 19 lines of text to the body of the file fout\n");
  fprintf(fout->f, " from the program rep.  should you wish to\n");
  fprintf(fout->f, " use a fout file that contains this warning\n");
  fprintf(fout->f, " as input for a program designed to use as input\n");
  fprintf(fout->f, " the standard fout file from program rep,\n");
  fprintf(fout->f, " it may be necessary first to delete 19 lines\n");
  fprintf(fout->f, " beginning with * * * warning * * *\n\n\n");
}



Static Void anarep(para, xseq, yseq, length, energy, coextensive, xout, yout,
		   rout, palindrome, finished)
prec para;
piece *xseq, *yseq;
long length;
double energy;
boolean coextensive;
long *xout, *yout;
reprec *rout;
boolean *palindrome, *finished;
{

  /*



*/
  long dist, cirdist;

  /*
*/
  *palindrome = false;
  *finished = false;
  /*




*/
  if (para.mode == 'd') {
    if (xseq->key.piedir == minus)
      *xout += 1 - length;
    else if (yseq->key.piedir == minus)
      *yout += 1 - length;
    /*
*/
    if (*xout < xseq->key.coobeg)
      *xout += xseq->key.cooend - xseq->key.coobeg + 1;
    else if (*yout < yseq->key.coobeg)
      *yout += yseq->key.cooend - yseq->key.coobeg + 1;
  }
  if (para.typbk == 'r' && *xout == *yout) {
    if (para.mode == 'i') {
      *palindrome = true;

    } else
      *finished = true;
  }
  /*
*/
  if (para.typbk == 'u')
    dist = 0;
  else if (!*finished && !*palindrome) {
    if (para.mode == 'd') {
      dist = *yout - *xout;
      if (xseq->key.coocon == circular) {
	/*
*/
	cirdist = xseq->key.cooend - *yout + *xout - xseq->key.coobeg + 1;
	if (cirdist < dist)
	  dist = cirdist;
      }
    } else {
      /*
*/
      dist = *yout - *xout - length + 1;
      if (xseq->key.coocon == circular) {
	cirdist = xseq->key.cooend - xseq->key.coobeg - dist - length * 2 + 3;
	if (cirdist < dist)
	  dist = cirdist;
      }
    }
  }
  /*
*/
  if (coextensive && dist < 0 && !*palindrome)
    *finished = true;
  if (length < para.minlength || length > para.maxlength)
    *finished = true;
  if (para.typbk == 'r' && !*palindrome) {
    if (dist < para.dmin || dist > para.dmax)
      *finished = true;
  }
  if (*finished)
    return;

  rout->x5p = *xout;
  rout->y5p = *yout;
  rout->rlength = length;
  rout->distance = dist;
  rout->deltagee = energy;
}



Static Void writetofile(para, rout, bout, getenergy, palindrome)
prec *para;
reprec *rout;
_TEXT *bout;
boolean getenergy, palindrome;
{
  /*
*/
  fprintf(bout->f, " %7ld", rout->x5p);
  if (!palindrome)
    fprintf(bout->f, " %8ld", rout->y5p);
  fprintf(bout->f, " %8ld", rout->rlength);
  if (para->typbk == 'r' && !palindrome)
    fprintf(bout->f, " %9ld ", rout->distance);
  if (getenergy)
    fprintf(bout->f, " %9.2f kcal", rout->deltagee);
  putc('\n', bout->f);
}


Local boolean overlap(xseq, yseq, para)
piece **xseq, **yseq;
prec *para;
{
  /*
*/
  /*
*/
  boolean Result = false;

  if (para->typbk != 'r')
    return false;
  switch (para->mode) {

  case 'd':
    if ((*xseq)->key.piebeg == (*yseq)->key.pieend &&
	(*xseq)->key.pieend == (*yseq)->key.piebeg)
      Result = true;
    break;

  case 'i':
    if ((*xseq)->key.piebeg == (*yseq)->key.piebeg &&
	(*xseq)->key.pieend == (*yseq)->key.pieend)
      Result = true;
    break;
  }
  return Result;
}



Static Void procrep(hlist, para, getenergy, xseq, yseq, fout, pout)
_TEXT *hlist;
prec *para;
boolean *getenergy;
piece **xseq, **yseq;
_TEXT *fout, *pout;
{
  /*


*/
  double energy;
  boolean done;
  long xout, yout, length;
  reprec rout;
  boolean palindrome, finished, coextensive;

  coextensive = overlap(xseq, yseq, para);
  if (coextensive || para->typbk == 'u')
    putc('\n', fout->f);
  else
    fprintf(fout->f,
      " repeats (or parts of repeats) may be duplicated in regions of sequence overlap\n");
  putc(' ', fout->f);
  if (BUFEOF(hlist->f)) {
    printf("premature end of hlist\n");
    halt();
  } else
    copyaline(hlist, fout);
  fprintf(fout->f, "%6c%cx:%6c%cy:%9s", '5', prime, '5', prime, "length:");
  if (para->typbk == 'r')
    fprintf(fout->f, "%10s", "distance:");
  if (*getenergy)
    fprintf(fout->f, "%13s", "energy:");
  putc('\n', fout->f);
  if (para->mode == 'i' && para->typbk == 'r') {
    putc(' ', pout->f);
    fprintf(pout->f, "%2c%c end:", '5', prime);
    fprintf(pout->f, "%9s", "length:");
    if (*getenergy)
      fprintf(pout->f, "%13s", "energy:");
    putc('\n', pout->f);
  } else {
    if (*pout->name != '\0') {
      if (pout->f != NULL)
	pout->f = freopen(pout->name, "w", pout->f);
      else
	pout->f = fopen(pout->name, "w");
    } else {
      if (pout->f != NULL)
	rewind(pout->f);
      else
	pout->f = tmpfile();
    }
    if (pout->f == NULL)
      _EscIO2(FileNotFound, pout->name);
    SETUPBUF(pout->f, Char);
  }
  do {
    gethelix(hlist, &xout, &yout, &length, *getenergy, &energy, &done);
    if (!done) {
      anarep(*para, *xseq, *yseq, length, energy, coextensive, &xout, &yout,
	     &rout, &palindrome, &finished);
      if (!palindrome && !finished)
	writetofile(para, &rout, fout, *getenergy, palindrome);
      else if (!finished)
	writetofile(para, &rout, pout, *getenergy, palindrome);
    }

  } while (!done);
}



Static Void themain(hlist, xbook, ybook, fout, repp)
_TEXT *hlist, *xbook, *ybook, *fout, *repp;
{
  /*











*/
  piece *xseq, *yseq;
  prec para;
  boolean getenergy;

  printf(" rep %4.2f\n", version);
  initbook(xbook, ybook, &xseq, &yseq);
  hlistvsbooks(hlist, xbook, ybook);
  readparameters(repp, &para);
  checkhlistparams(hlist, &para, &getenergy);
  foutheading(hlist, fout, &getenergy, &para);
  poutheading(hlist, &pout, &getenergy, &para);
  while (!BUFEOF(xbook->f)) {
    getpiece(xbook, &xseq);
    if (!BUFEOF(xbook->f)) {
      if (*ybook->name != '\0') {
	if (ybook->f != NULL)
	  ybook->f = freopen(ybook->name, "r", ybook->f);
	else
	  ybook->f = fopen(ybook->name, "r");
      } else
	rewind(ybook->f);
      if (ybook->f == NULL)
	_EscIO2(FileNotFound, ybook->name);
      RESETBUF(ybook->f, Char);
      while (!BUFEOF(ybook->f)) {
	getpiece(ybook, &yseq);
	if (!BUFEOF(ybook->f)) {
	  checkseqdata(para, xseq, yseq, fout);
	  procrep(hlist, &para, &getenergy, &xseq, &yseq, fout, &pout);
	}
	clearpiece(&yseq);
      }
    }
    clearpiece(&xseq);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  repp.f = NULL;
  strcpy(repp.name, "repp");
  pout.f = NULL;
  strcpy(pout.name, "pout");
  fout.f = NULL;
  strcpy(fout.name, "fout");
  hlist.f = NULL;
  strcpy(hlist.name, "hlist");
  ybook.f = NULL;
  strcpy(ybook.name, "ybook");
  xbook.f = NULL;
  strcpy(xbook.name, "xbook");
  themain(&hlist, &xbook, &ybook, &fout, &repp);
_L1:
  if (xbook.f != NULL)
    fclose(xbook.f);
  if (ybook.f != NULL)
    fclose(ybook.f);
  if (hlist.f != NULL)
    fclose(hlist.f);
  if (fout.f != NULL)
    fclose(fout.f);
  if (pout.f != NULL)
    fclose(pout.f);
  if (repp.f != NULL)
    fclose(repp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
