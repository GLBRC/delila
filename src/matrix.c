/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "matrix.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         3.29
/*


*/



/*
























































*/



#define headerlines     7

#define nodot           ':'
/*

*/

#define pagelength      48

#define maxarray        150000L

#define prime           '\''




#define dnamax          3000
#define namelength      20
#define linelength      80



typedef enum {
  zero, one
} bit;




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

/* p2c: matrix.p, line 143:
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



Static _TEXT xbook, ybook, hlist, mlist, matrixp;

Static piece *xseq, *yseq;
Static long xnumber, ynumber;
Static double minlenmaxene;
/*

*/


/*
*/
/* p2c: matrix.p, line 233:
 * Note: Field width for bitarray assumes enum bit has 2 elements [105] */
Static uchar bitarray[(maxarray + 7) / 8];
Static long index_;


Static long xlength, ylength;

Static long length_;






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






Static base getbase(position, pie)
long position;
piece *pie;
{
  /*
*/
  dnastring *workdna;
  long p = dnamax;

  workdna = pie->dna;
  while (position > p) {
    p += dnamax;
    workdna = workdna->next;
  }
  return ((base)P_getbits_UB(workdna->part, position - p + dnamax - 1, 1, 3));
}


/* Local variables for numberdigit: */
struct LOC_numberdigit {
  long number, place, absolute;
  Char acharacter;
} ;

Local Void digit(LINK)
struct LOC_numberdigit *LINK;
{
  long tenplace, z, d;

  tenplace = LINK->place * 10;
  z = LINK->absolute - LINK->absolute / tenplace * tenplace;
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
  V.absolute = labs(V.number);
  if (V.absolute < V.place / 10) {
    V.acharacter = ' ';
    return V.acharacter;
  }
  if (V.absolute >= V.place)
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
  if (n == 0)
    return 1;
  else {
    return ((long)(log((double)labs(n)) / ln10 + epsilon) + 2);
    /*


*/

  }
}

#undef ln10
#undef epsilon



Static Void numberbar(afile, spaces, firstnumber, lastnumber, linesused)
_TEXT *afile;
long spaces, firstnumber, lastnumber, *linesused;
{
  /*
*/
  long logplace, spacecount, number;

  if (labs(firstnumber) > labs(lastnumber))
    *linesused = numbersize(firstnumber);
  else
    *linesused = numbersize(lastnumber);

  for (logplace = *linesused - 1; logplace >= 0; logplace--) {
    for (spacecount = 1; spacecount <= spaces; spacecount++)
      putc(' ', afile->f);
    for (number = firstnumber; number <= lastnumber; number++)
      fputc(numberdigit(number, logplace), afile->f);
    putc('\n', afile->f);
  }
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


Static Void initialize()
{
  long i;
  boolean getenergy;

  /*
*/
  printf(" matrix %4.2f\n", version);
  brinit(&xbook);
  brinit(&ybook);

  hlistvsbooks(&hlist, &xbook, &ybook);
  if (*hlist.name != '\0') {
    if (hlist.f != NULL)
      hlist.f = freopen(hlist.name, "r", hlist.f);
    else
      hlist.f = fopen(hlist.name, "r");
  } else
    rewind(hlist.f);
  if (hlist.f == NULL)
    _EscIO2(FileNotFound, hlist.name);
  RESETBUF(hlist.f, Char);
  if (*mlist.name != '\0') {
    if (mlist.f != NULL)
      mlist.f = freopen(mlist.name, "w", mlist.f);
    else
      mlist.f = fopen(mlist.name, "w");
  } else {
    if (mlist.f != NULL)
      rewind(mlist.f);
    else
      mlist.f = tmpfile();
  }
  if (mlist.f == NULL)
    _EscIO2(FileNotFound, mlist.name);
  SETUPBUF(mlist.f, Char);

  xseq = (piece *)Malloc(sizeof(piece));
  yseq = (piece *)Malloc(sizeof(piece));


  fprintf(mlist.f, " matrix %4.2f from:\n", version);
  if (copylines(&hlist, &mlist, (long)headerlines) != headerlines) {
    printf(" hlist is too short to even have a correct header\n");
    halt();
  }


  if (*hlist.name != '\0') {
    if (hlist.f != NULL)
      hlist.f = freopen(hlist.name, "r", hlist.f);
    else
      hlist.f = fopen(hlist.name, "r");
  } else
    rewind(hlist.f);
  if (hlist.f == NULL)
    _EscIO2(FileNotFound, hlist.name);
  RESETBUF(hlist.f, Char);
  for (i = 1; i <= 3; i++) {
    fscanf(hlist.f, "%*[^\n]");
    getc(hlist.f);
  }
  fscanf(hlist.f, "%lg%*[^\n]", &minlenmaxene);
  getc(hlist.f);
  fscanf(hlist.f, "%*[^\n]");
  getc(hlist.f);
  getc(hlist.f);
  if (P_peek(hlist.f) == 'e')
    getenergy = true;
  else
    getenergy = false;
  for (i = 6; i <= headerlines; i++) {
    fscanf(hlist.f, "%*[^\n]");
    getc(hlist.f);
  }


  if (*matrixp.name != '\0') {
    if (matrixp.f != NULL)
      matrixp.f = freopen(matrixp.name, "r", matrixp.f);
    else
      matrixp.f = fopen(matrixp.name, "r");
  } else
    rewind(matrixp.f);
  if (matrixp.f == NULL)
    _EscIO2(FileNotFound, matrixp.name);
  RESETBUF(matrixp.f, Char);
  if (!BUFEOF(matrixp.f)) {
    fscanf(matrixp.f, "%lg%*[^\n]", &minlenmaxene);
    getc(matrixp.f);
  }


  if (!getenergy) {
    if (minlenmaxene <= 0.0) {
      printf(" no energies listed in this hlist,");
      printf(" change parameter\n");
      printf(" to request positive lengths\n");
      halt();
    }
  }


  if (minlenmaxene > 0)
    fprintf(mlist.f,
      " only helixes longer than or equal to %ld base pairs are printed\n",
      (long)minlenmaxene);
  else
    fprintf(mlist.f,
      " only helixes stronger than or equal to %5.2f kcal are printed\n",
      minlenmaxene);
  fprintf(mlist.f, "\n symbols used:\n");
  fprintf(mlist.f, " %c every helix through this point is ", nodot);
  if (minlenmaxene > 0)
    fprintf(mlist.f, "less than %ld base pairs long.\n", (long)minlenmaxene);
  else
    fprintf(mlist.f, "weaker than %5.2f kcals.\n", minlenmaxene);
  fprintf(mlist.f, " 1 gt base pair\n");
  fprintf(mlist.f, " 2 at base pair\n");
  fprintf(mlist.f, " 3 gc base pair\n");
}


/*

*/

Static Void setindex(x, y)
long x, y;
{
  index_ = x + (y - 1) * xlength;
}


Static Void clearbitarray()
{
  long index, maximum;

  maximum = xlength * ylength;
  if (maximum > maxarray) {
    printf(" array size (maxarray) exceeded\n");
    halt();
  }

  for (index = 0; index < maximum; index++)
    P_clrbits_B(bitarray, index, 0, 3);
}


Static Void writebitarray(x, y)
long x, y;
{
  setindex(x, y);
  P_putbits_UB(bitarray, index_ - 1, (int)one, 0, 3);
  /*
*/
}


Static bit readbitarray(x, y)
long x, y;
{
  setindex(x, y);
  /*
*/
  return ((bit)P_getbits_UB(bitarray, index_ - 1, 0, 3));
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


Static Void checknames()
{
  /*
*/
  fscanf(hlist.f, "%*[^\n]");
  getc(hlist.f);
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


Static Void fillbitarray()
{
  boolean done = false;
  long x0, y0, l;
  boolean getenergy;
  double energy;
  long x, y, FORLIM;

  getenergy = (minlenmaxene <= 0);
  while (!done) {
    gethelix(&hlist, &x0, &y0, &length_, getenergy, &energy, &done);

    if (done || ((getenergy || length_ < minlenmaxene) &&
		 !(getenergy && energy <= minlenmaxene)))
      continue;
    x = pietoint(x0, xseq);
    y = pietoint(y0, yseq) + length_ - 1;

    FORLIM = length_;
    for (l = 1; l <= FORLIM; l++) {
      writebitarray(x, y);
      x++;
      y--;
    }
  }
}


Static Void piecebar(afile, spaces, pie, firstnumber, lastnumber, linesused)
_TEXT *afile;
long spaces;
piece *pie;
long firstnumber, lastnumber, *linesused;
{
  /*





*/
  long logpower, spacecount, number;
  struct {
    FILE *f;
    FILEBUFNC(f,long);
    Char name[_FNSIZE];
  } numbers;
  long width, index;

  *numbers.name = '\0';
  *linesused = 0;
  if (*numbers.name != '\0')
    numbers.f = fopen(numbers.name, "wb");
  else
    numbers.f = tmpfile();
  if (numbers.f == NULL)
    _EscIO2(FileNotFound, numbers.name);
  SETUPBUF(numbers.f, long);
  for (index = firstnumber; index <= lastnumber; index++) {
    number = inttopie(index, pie);
    fwrite(&number, sizeof(long), 1, numbers.f);
    width = numbersize(number);
    if (width > *linesused)
      *linesused = width;
  }

  for (logpower = *linesused - 1; logpower >= 0; logpower--) {
    for (spacecount = 1; spacecount <= spaces; spacecount++)
      putc(' ', afile->f);
    if (*numbers.name != '\0')
      numbers.f = freopen(numbers.name, "rb", numbers.f);
    else
      rewind(numbers.f);
    if (numbers.f == NULL)
      _EscIO2(FileNotFound, numbers.name);
    RESETBUF(numbers.f, long);
    while (!BUFEOF(numbers.f)) {
      fread(&number, sizeof(long), 1, numbers.f);
      fputc(numberdigit(number, logpower), afile->f);
    }
    putc('\n', afile->f);
  }
  fclose(numbers.f);
}



Static Void makedna(afile, spaces, pie, firstnumber, lastnumber)
_TEXT *afile;
long spaces;
piece *pie;
long firstnumber, lastnumber;
{
  long spacecount, index;

  for (spacecount = 1; spacecount <= spaces - 3; spacecount++)
    putc(' ', afile->f);
  fprintf(afile->f, "5%c ", prime);
  for (index = firstnumber; index <= lastnumber; index++)
    fputc(basetochar(getbase(index, pie)), afile->f);
  fprintf(afile->f, " 3%c\n", prime);
}



Static Void makepageheader(pageheader)
_TEXT *pageheader;
{
  long lines;

  if (*pageheader->name != '\0') {
    if (pageheader->f != NULL)
      pageheader->f = freopen(pageheader->name, "w", pageheader->f);
    else
      pageheader->f = fopen(pageheader->name, "w");
  } else {
    if (pageheader->f != NULL)
      rewind(pageheader->f);
    else
      pageheader->f = tmpfile();
  }
  if (pageheader->f == NULL)
    _EscIO2(FileNotFound, pageheader->name);
  SETUPBUF(pageheader->f, Char);
  fprintf(pageheader->f, "\f");
  fprintf(pageheader->f, " matrix,  x down: %ld %.*s y across: %ld %.*s\n\n",
	  xnumber, namelength, xseq->key.hea.keynam.letters, ynumber,
	  namelength, yseq->key.hea.keynam.letters);
  piecebar(pageheader, 10L, yseq, 1L, ylength, &lines);
  makedna(pageheader, 10L, yseq, 1L, ylength);
  fprintf(pageheader->f, "        5%c\n", prime);
}



Static Void printpageheader(pageheader)
_TEXT *pageheader;
{
  if (*pageheader->name != '\0') {
    if (pageheader->f != NULL)
      pageheader->f = freopen(pageheader->name, "r", pageheader->f);
    else
      pageheader->f = fopen(pageheader->name, "r");
  } else
    rewind(pageheader->f);
  if (pageheader->f == NULL)
    _EscIO2(FileNotFound, pageheader->name);
  RESETBUF(pageheader->f, Char);
  while (!BUFEOF(pageheader->f)) {
    while (!P_eoln(pageheader->f)) {
      putc(P_peek(pageheader->f), mlist.f);
      getc(pageheader->f);
    }
    fscanf(pageheader->f, "%*[^\n]");
    getc(pageheader->f);
    putc('\n', mlist.f);
  }
}


Static Void printbitarray()
{
  _TEXT pageheader;
  base bx, by;
  long x, y, FORLIM, FORLIM1;

  pageheader.f = NULL;
  *pageheader.name = '\0';
  makepageheader(&pageheader);
  FORLIM = xlength;
  for (x = 1; x <= FORLIM; x++) {
    if (x % pagelength == 0 || x == 1)
      printpageheader(&pageheader);
    bx = getbase(x, xseq);
    fprintf(mlist.f, " %6ld %c ", inttopie(x, xseq), basetochar(bx));
    FORLIM1 = ylength;
    for (y = 1; y <= FORLIM1; y++) {
      if (readbitarray(x, y) == one) {
	by = getbase(y, yseq);
	if (bx == complement(by)) {
	  switch (bx) {

	  case a:
	    putc('2', mlist.f);
	    break;

	  case c:
	    putc('3', mlist.f);
	    break;

	  case g:
	    putc('3', mlist.f);
	    break;

	  case t:
	    putc('2', mlist.f);
	    break;
	  }
	} else
	  putc('1', mlist.f);
      } else
	putc(nodot, mlist.f);
    }
    putc('\n', mlist.f);
  }
  fprintf(mlist.f, "        3%c\n", prime);
  if (pageheader.f != NULL)
    fclose(pageheader.f);
}


Static Void printmatrix()
{
  checknames();
  clearbitarray();
  fillbitarray();
  printbitarray();
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  matrixp.f = NULL;
  strcpy(matrixp.name, "matrixp");
  mlist.f = NULL;
  strcpy(mlist.name, "mlist");
  hlist.f = NULL;
  strcpy(hlist.name, "hlist");
  ybook.f = NULL;
  strcpy(ybook.name, "ybook");
  xbook.f = NULL;
  strcpy(xbook.name, "xbook");
  initialize();
  while (!BUFEOF(xbook.f)) {
    getpiece(&xbook, &xseq);
    xnumber = number;
    xlength = piecelength(xseq);
    if (BUFEOF(xbook.f))
      break;
    if (*ybook.name != '\0') {
      if (ybook.f != NULL)
	ybook.f = freopen(ybook.name, "r", ybook.f);
      else
	ybook.f = fopen(ybook.name, "r");
    } else
      rewind(ybook.f);
    if (ybook.f == NULL)
      _EscIO2(FileNotFound, ybook.name);
    RESETBUF(ybook.f, Char);
    while (!BUFEOF(ybook.f)) {
      getpiece(&ybook, &yseq);
      ynumber = number;
      ylength = piecelength(yseq);
      if (BUFEOF(ybook.f))
	break;
      printmatrix();
      clearpiece(&yseq);
    }
    clearpiece(&xseq);
  }
_L1:
  if (xbook.f != NULL)
    fclose(xbook.f);
  if (ybook.f != NULL)
    fclose(ybook.f);
  if (hlist.f != NULL)
    fclose(hlist.f);
  if (mlist.f != NULL)
    fclose(mlist.f);
  if (matrixp.f != NULL)
    fclose(matrixp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
