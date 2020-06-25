/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "keymat.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*









*/



#define version         5.40
/*




*/



/*







































































*/



#define debug           false

#define headerlines     7

#define nodot           ':'
/*

*/
#define prime           '\''

#define pagelength      48
#define listmax         1000
/*


*/




#define dnamax          3000
#define namelength      20
#define linelength      80



typedef struct helix {
  long x5pos, y3pos, length;
  Char symbol;
  struct helix *across;
} helix;

typedef struct holder {
  long x;
  helix *across;
  struct holder *down;
} holder;

typedef Char symbolholder[37];
typedef double stopholder[37];




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

/* p2c: keymat.p, line 183:
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



Static _TEXT xbook, ybook, hlist, kmlist, keymatp;

Static piece *xseq, *yseq;
Static long ylength, xlength, xnumber, ynumber;

Static double minlenmaxene;
/*

*/
Static long xscale, yscale;
Static Char gensymbol;
/*

*/
Static Char erange;
/*



*/
Static symbolholder symbols;
Static stopholder stops;







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


#define missingparam    " keymatp is missing some parameters."



Static Void initialize()
{
  long i;
  boolean getenergy;
  /*

*/
  double hlistminmax;

  /*
*/

  printf(" keymat %4.2f\n", version);
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
  if (*kmlist.name != '\0') {
    if (kmlist.f != NULL)
      kmlist.f = freopen(kmlist.name, "w", kmlist.f);
    else
      kmlist.f = fopen(kmlist.name, "w");
  } else {
    if (kmlist.f != NULL)
      rewind(kmlist.f);
    else
      kmlist.f = tmpfile();
  }
  if (kmlist.f == NULL)
    _EscIO2(FileNotFound, kmlist.name);
  SETUPBUF(kmlist.f, Char);

  xseq = (piece *)Malloc(sizeof(piece));
  yseq = (piece *)Malloc(sizeof(piece));


  fprintf(kmlist.f, " keymat %4.2f from:\n", version);
  if (copylines(&hlist, &kmlist, (long)headerlines) != headerlines) {
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
  fscanf(hlist.f, "%lg%*[^\n]", &hlistminmax);
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


  if (*keymatp.name != '\0') {
    if (keymatp.f != NULL)
      keymatp.f = freopen(keymatp.name, "r", keymatp.f);
    else
      keymatp.f = fopen(keymatp.name, "r");
  } else
    rewind(keymatp.f);
  if (keymatp.f == NULL)
    _EscIO2(FileNotFound, keymatp.name);
  RESETBUF(keymatp.f, Char);
  if (BUFEOF(keymatp.f)) {
    minlenmaxene = hlistminmax;
    xscale = 1;
    yscale = 1;
    gensymbol = 'n';
    erange = 'n';
  } else {
    fscanf(keymatp.f, "%lg%*[^\n]", &minlenmaxene);
    getc(keymatp.f);
    if (minlenmaxene > 0) {
      if (false) {
	printf(" non-integer length requested in keymatp\n");
	halt();
      }
    }


    if (!getenergy) {
      if (minlenmaxene < 0) {
	printf(" no energies listed in this hlist,\n");
	printf(" change parameter to request\n");
	printf(" positive lengths.\n");
	halt();
      }
    }


    if (minlenmaxene > 0) {
      if (minlenmaxene < hlistminmax) {
	printf(" caution:\n");
	printf(" hlist only has helices that are");
	printf(" %ld bases or longer.\n", (long)hlistminmax);
	printf(" %ld bases was requested.\n\n", (long)minlenmaxene);
	minlenmaxene = hlistminmax;
      }
    }
    if (minlenmaxene <= 0) {
      if (minlenmaxene > hlistminmax) {
	printf(" caution:\n");
	printf(" hlist only has helices that are");
	printf(" stronger than or equal to %5.2f kcals.\n", hlistminmax);
	printf(" %5.2f kcal was requested.\n\n", minlenmaxene);
	minlenmaxene = hlistminmax;
      }
    }


    if (BUFEOF(keymatp.f)) {
      puts(missingparam);
      halt();
    } else
      fscanf(keymatp.f, "%ld", &xscale);
    if (P_eoln(keymatp.f) | BUFEOF(keymatp.f)) {
      printf(" missing the y-scaling parameter in keymatp.\n");
      printf(" keymatp must contain x and y scaling factors.\n");
      halt();
    } else {
      fscanf(keymatp.f, "%ld%*[^\n]", &yscale);
      getc(keymatp.f);
    }
    if (xscale < 0 || yscale < 0) {
      printf(" scaling parameters must be positive integers.\n");
      halt();
    }

    if (BUFEOF(keymatp.f)) {
      puts(missingparam);
      halt();
    } else {
      while ((P_peek(keymatp.f) == ' ') & (!P_eoln(keymatp.f)))
	getc(keymatp.f);
      if (!P_eoln(keymatp.f)) {
	fscanf(keymatp.f, "%c%*[^\n]", &gensymbol);
	getc(keymatp.f);
	if (gensymbol == '\n')
	  gensymbol = ' ';
      } else {
	puts(missingparam);
	halt();
      }
    }


    if (gensymbol != 'e' && gensymbol != 'y' && gensymbol != 'x' &&
	gensymbol != 'n') {
      printf(" '%c' is not a valid symbol-parameter\n", gensymbol);
      printf(" please correct keymatp.\n");
      halt();
    }


    if (gensymbol == 'e') {
      if (BUFEOF(keymatp.f)) {
	puts(missingparam);
	halt();
      }
      fscanf(keymatp.f, "%c%*[^\n]", &erange);
      getc(keymatp.f);
      if (erange == '\n')
	erange = ' ';
      if (erange != 'a' && erange != 'l' && erange != 'n') {
	printf(" 4th symbol in keymatp must be one of nla.\n");
	halt();
      }
      if (erange == 'a') {
	printf("alphanumerics is not available now\n");
	printf("because it bombs for unknown reasons\n");
	halt();
      }
    } else
      erange = 'n';
  }


  if (minlenmaxene > 0)
    fprintf(kmlist.f,
	    " %ld base pairs is the shortest length helix printed.\n",
	    (long)minlenmaxene);
  else
    fprintf(kmlist.f, " %5.2f kcal is the weakest energy helix printed.\n",
	    minlenmaxene);
  fprintf(kmlist.f,
	  " %ld %ld are the x and y scaling factors, respectively.\n",
	  xscale, yscale);
  if (xscale > 1 || yscale > 1) {
    fprintf(kmlist.f, " '*' indicates helices in this region.\n");
    return;
  }
  if (gensymbol == 'n') {
    fprintf(kmlist.f, " n(umber) symbol-type chosen\n\n");
    fprintf(kmlist.f, " symbols used:\n");
    fprintf(kmlist.f, " %c every helix thru this point is ", nodot);
    if (minlenmaxene > 0)
      fprintf(kmlist.f, "less than %ld base pairs long.\n",
	      (long)minlenmaxene);
    else
      fprintf(kmlist.f, "weaker than %5.2f kcals\n", minlenmaxene);
    fprintf(kmlist.f, " 1 gt base pair\n");
    fprintf(kmlist.f, " 2 at base pair\n");
    fprintf(kmlist.f, " 3 gc base pair\n");
  } else if (gensymbol == 'x')
    fprintf(kmlist.f, " x-book bases will be printed in helices.\n");
  else if (gensymbol == 'y')
    fprintf(kmlist.f, " y-book bases will be printed in helices.\n");
  else if (gensymbol == 'e') {
    fprintf(kmlist.f, " e(nergy) of each helix will be printed.\n");
    putc(' ', kmlist.f);
    switch (erange) {

    case 'a':
      fprintf(kmlist.f, "a(lphanumerics) a to z, 0 to 9");
      break;

    case 'l':
      fprintf(kmlist.f, "l(etters) a to z");
      break;

    case 'n':
      fprintf(kmlist.f, "n(umbers) 0 to 9");
      break;
    }
    fprintf(kmlist.f, " is the range for the energy display\n");
  } else {
    printf(" something is wrong in the end of initialize\n");
    halt();
  }
  putc('\n', kmlist.f);
}

#undef missingparam


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



Static Void getlowenergy(hlist, absbottom)
_TEXT *hlist;
double *absbottom;
{
  /*


*/
  long i;
  double holdenergy;
  /*
*/
  boolean done = false;

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
  for (i = 1; i <= headerlines + 1; i++) {
    fscanf(hlist->f, "%*[^\n]");
    getc(hlist->f);
  }
  *absbottom = 0.0;
  if (BUFEOF(hlist->f)) {
    printf(" no helix data in hlist.\n");
    halt();
  }
  while (!done) {
    getc(hlist->f);
    if (P_peek(hlist->f) != ' ') {
      fscanf(hlist->f, "%*[^\n]");
      getc(hlist->f);
    }
    /*
*/
    if (BUFEOF(hlist->f)) {
      done = true;
      break;
    }
    for (i = 1; i <= 3; i++)
      findcolon(hlist);
    fscanf(hlist->f, "%ld", &i);
    fscanf(hlist->f, "%lg%*[^\n]", &holdenergy);
    getc(hlist->f);
    if (holdenergy < *absbottom)
      *absbottom = holdenergy;
    if (BUFEOF(hlist->f))
      done = true;
  }
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
  for (i = 1; i <= headerlines; i++) {
    fscanf(hlist->f, "%*[^\n]");
    getc(hlist->f);
  }
}


#define delta           0.01
/*



*/

#define abovesub        '>'
#define belowsub        '<'



Static Void maketable(abstop, absbottom, subtop, subbottom, symbols, stops,
		      kmlist)
double abstop, absbottom, subtop, subbottom;
Char *symbols;
double *stops;
_TEXT *kmlist;
{

  /*

*/
  double top, bottom, range, rangewidth, start, stop;

  Char symb;

  long numbsymb, i, j;

  boolean done = false;
  boolean sub;



  for (i = 0; i <= 36; i++) {
    symbols[i] = '?';
    stops[i] = 0.0;
  }


  if (subtop == 0 && subbottom == 0)
    sub = false;
  else
    sub = true;


  if (erange == 'n')
    numbsymb = 9;
  else if (erange == 'l')
    numbsymb = 26;
  else if (erange == 'a')
    numbsymb = 35;
  else {
    printf(" problem assigning numbsymb in maktable.\n");
    halt();
  }


  if (!sub) {
    top = abstop;
    bottom = absbottom;
  } else {
    top = subtop;
    bottom = subbottom;
  }
  range = top - bottom;

  /*
*/
  if (range / numbsymb < delta) {
    while (range / numbsymb < delta) {
      numbsymb--;
      if (numbsymb < 1) {
	printf(" range given is too small.\n");
	halt();
      }
    }
    rangewidth = delta;
  } else
    rangewidth = ((long)(range / numbsymb * 100) + 1.0) / 100;


  start = top;
  stop = start - rangewidth;
  if (erange == 'n')
    symb = '1';
  else
    symb = 'a';

  if (sub) {
    for (j = 1; j <= 10; j++)
      putc(' ', kmlist->f);
    fprintf(kmlist->f, "above %8.2f  --  '%c'\n", start, abovesub);
  }

  /*
*/
  stops[0] = start;
  symbols[0] = abovesub;

  i = 2;
  /*

*/


  while (!done) {
    stops[i-1] = stop;
    symbols[i-1] = symb;
    fprintf(kmlist->f, " %8.2f    to %8.2f  --  '%c'\n", start, stop, symb);
    start = stop - delta;
    if (stop - rangewidth <= bottom) {
      stop = bottom;
      if (symb == 'z')
	symb = '1';
      else
	symb++;
      i++;
      stops[i-1] = stop;
      symbols[i-1] = symb;
      fprintf(kmlist->f, " %8.2f    to %8.2f  --  '%c'\n", start, stop, symb);
      done = true;
      /*

*/
      continue;
    }

    stop -= rangewidth;
    if (symb == 'z')
      symb = '1';
    else
      symb++;
    if (i == numbsymb + 1)
      done = true;
    else
      i++;
  }

  if (sub) {
    for (j = 1; j <= 10; j++)
      putc(' ', kmlist->f);
    fprintf(kmlist->f, "below %8.2f  --  '%c'\n", bottom, belowsub);
  }
  symbols[i] = belowsub;
  stops[i] = -LONG_MAX;
  fprintf(kmlist->f, "\n lowest energy found in hlist was: %8.2f\n\n",
	  absbottom);
}

#undef delta
#undef abovesub
#undef belowsub


Local Void yell()
{
  printf(" incorrect range parameters detected.\n");
  halt();
}



Static Void dotable(keymatp, kmlist, hlist, symbols, stops)
_TEXT *keymatp, *kmlist, *hlist;
Char *symbols;
double *stops;
{

  /*

*/

  /*
*/
  double abstop, absbottom, subtop, subbottom;

  long i;



  if (minlenmaxene < 0)
    abstop = minlenmaxene;
  else
    abstop = 0.0;
  getlowenergy(hlist, &absbottom);
  if (BUFEOF(keymatp->f)) {
    subtop = 0.0;
    subbottom = 0.0;
    /*
*/
  } else {
    fscanf(keymatp->f, "%lg", &subtop);
    while ((!(isdigit(P_peek(keymatp->f)) | (P_peek(keymatp->f) == '-'))) &
	   (!P_eoln(keymatp->f)))
      getc(keymatp->f);
    if (!P_eoln(keymatp->f)) {
      fscanf(keymatp->f, "%lg%*[^\n]", &subbottom);
      getc(keymatp->f);
    } else
      yell();
  }


  if (abstop <= 0) {
    if (absbottom - abstop > 0.0001) {
      printf(" there are no helixes in hlist with energy less than %8.2f\n",
	     absbottom);
      printf(" your request was for energies less than or equal to %8.2f\n",
	     abstop);
      halt();
    }
    /*









*/
  }
  if (subtop < subbottom)
    yell();
  if (subtop == subbottom && subtop != 0)
    yell();
  if (subtop != subbottom) {
    if (abstop <= subtop || subbottom <= absbottom)
      yell();
  }

  /*
*/

  fprintf(kmlist->f, " symbols used:\n");

  maketable(abstop, absbottom, subtop, subbottom, symbols, stops, kmlist);
}



Static Void getrecord(freerecord, newhelix)
helix **freerecord, **newhelix;
{
  helix *WITH;

  if (*freerecord == NULL)
    *newhelix = (helix *)Malloc(sizeof(helix));
  else {
    *newhelix = *freerecord;
    *freerecord = (*freerecord)->across;
  }
  WITH = *newhelix;
  WITH->x5pos = 0;
  WITH->y3pos = 0;
  WITH->length = 0;
  WITH->symbol = '?';
  WITH->across = NULL;
}



Static Void getholder(freeholder, newholder)
holder **freeholder, **newholder;
{
  /*
*/
  holder *WITH;

  if (*freeholder == NULL)
    *newholder = (holder *)Malloc(sizeof(holder));
  else {
    *newholder = *freeholder;
    *freeholder = (*freeholder)->down;
  }
  WITH = *newholder;
  WITH->x = 0;
  WITH->across = NULL;
  WITH->down = NULL;
}



Static Char getsymbol(energy, symbols, stops)
double energy;
Char *symbols;
double *stops;
{
  /*
*/
  Char Result;
  long i = 1;
  boolean done = false;

  if (energy > 0 || energy < -LONG_MAX) {
    printf(" energy encountered that is out of the range i");
    printf(" can handle.\n");
    halt();
  }
  while (!done) {
    /*
*/
    if (energy >= stops[i-1]) {
      Result = symbols[i-1];
      done = true;
    } else
      i++;
    if (i > 37) {
      printf(" problem in getsymbol.\n");
      halt();
    }
  }
  return Result;
}



Static Void readhelix(hlist, member, symbols, stops, done)
_TEXT *hlist;
helix *member;
Char *symbols;
double *stops;
boolean *done;
{
  /*





*/
  long x = 0, y = 0, len = 0;
  double energy = 0.0;
  boolean helixok = false;

  do {
    if (BUFEOF(hlist->f))
      *done = true;
    else {
      getc(hlist->f);
      if (P_peek(hlist->f) != ' ')
	*done = true;
      else {
	findcolon(hlist);
	fscanf(hlist->f, "%ld", &x);
	findcolon(hlist);
	fscanf(hlist->f, "%ld", &y);
	findcolon(hlist);
	fscanf(hlist->f, "%ld", &len);

	if (gensymbol == 'e' || minlenmaxene < 0) {
	  fscanf(hlist->f, "%lg%*[^\n]", &energy);
	  getc(hlist->f);
	} else {
	  fscanf(hlist->f, "%*[^\n]");
	  getc(hlist->f);
	}
	/*
*/

	if (minlenmaxene > 0) {
	  if (len >= minlenmaxene)
	    helixok = true;
	}
	if (minlenmaxene < 0) {
	  if (energy <= minlenmaxene)
	    helixok = true;
	}
      }
    }
  } while (!(helixok || *done));
  if (*done)
    return;

  member->x5pos = pietoint(x, xseq);
  member->y3pos = pietoint(y, yseq);

  member->y3pos += len - 1;

  member->length = len;

  member->across = NULL;
  /*
*/
  if (gensymbol == 'e')
    member->symbol = getsymbol(energy, symbols, stops);
}



Static Void sortacross(theholder, newhelix)
holder *theholder;
helix **newhelix;
{
  helix *follower;
  boolean done = false;

  if ((*newhelix)->y3pos < theholder->across->y3pos) {
    (*newhelix)->across = theholder->across;
    theholder->across = *newhelix;
    return;
  }
  follower = theholder->across;
  (*newhelix)->across = follower->across;
  while (!done) {
    if ((*newhelix)->across == NULL) {
      done = true;
      break;
    }
    if ((*newhelix)->y3pos < (*newhelix)->across->y3pos)
      done = true;
    else {
      follower = follower->across;
      (*newhelix)->across = (*newhelix)->across->across;
    }
  }
  follower->across = *newhelix;
}



Static Void sorthelix(freeholder, root, newhelix)
holder **freeholder, **root;
helix **newhelix;
{
  /*
*/
  holder *newholder, *last, *next;
  boolean done = false;

  if ((*newhelix)->x5pos < (*root)->x) {
    getholder(freeholder, &newholder);
    newholder->x = (*newhelix)->x5pos;
    newholder->across = *newhelix;
    newholder->down = *root;
    *root = newholder;
    return;
  }
  if ((*newhelix)->x5pos == (*root)->x) {
    sortacross(*root, newhelix);
    return;
  }
  last = *root;
  next = (*root)->down;
  while (!done) {
    if (next == NULL) {
      done = true;
      break;
    }
    if (next->x >= (*newhelix)->x5pos)
      done = true;
    else {
      last = next;
      next = next->down;
    }
  }
  if (next == NULL) {
    getholder(freeholder, &newholder);
    newholder->x = (*newhelix)->x5pos;
    newholder->across = *newhelix;
    newholder->down = NULL;
    last->down = newholder;
    return;
  }
  if (next->x <= (*newhelix)->x5pos) {
    sortacross(next, newhelix);
    return;
  }

  getholder(freeholder, &newholder);
  newholder->x = (*newhelix)->x5pos;
  newholder->across = *newhelix;
  newholder->down = next;
  last->down = newholder;
}



Static Void makelist(root, freeholder, freerecord, symbols, stops)
holder **root, **freeholder;
helix **freerecord;
Char *symbols;
double *stops;
{

  /*






































*/
  helix *newhelix;
  long j = 1;

  boolean done = false;


  getholder(freeholder, root);
  getrecord(freerecord, &newhelix);
  readhelix(&hlist, newhelix, symbols, stops, &done);
  if (done) {
    printf(" no helices in hlist satisfy parameters.\n");
    printf(" change parameters to allow helices to be printed.\n");
    halt();
  }
  (*root)->x = newhelix->x5pos;
  (*root)->across = newhelix;


  while (!done && j <= listmax) {
    getrecord(freerecord, &newhelix);
    readhelix(&hlist, newhelix, symbols, stops, &done);
    if (!done) {
      sorthelix(freeholder, root, &newhelix);
      j++;
    } else {
      newhelix->across = *freerecord;
      *freerecord = newhelix;
    }
    if (j > listmax) {
      printf(" there are more than %ld helices in hlist.\n", (long)listmax);
      printf(" change parameters to eliminate some helices,\n");
      printf(" or alter constant listmax.\n");
      halt();
    }
  }

}



Static Void showlist(root, thefile)
holder *root;
_TEXT *thefile;
{
  /*
*/
  helix *row;
  holder *next = root;

  while (next != NULL) {
    fprintf(thefile->f, " new row:\n");
    row = next->across;
    fprintf(thefile->f, " x is: %ld", inttopie(next->x, xseq));
    while (row != NULL) {
      fprintf(thefile->f, " x5pos: %ld", inttopie(row->x5pos, xseq));
      fprintf(thefile->f, " y3pos: %ld", inttopie(row->y3pos, yseq));
      fprintf(thefile->f, " length: %ld", row->length);
      fprintf(thefile->f, " next: ");
      row = row->across;
    }
    putc('\n', thefile->f);
    next = next->down;
    if (next == NULL)
      fprintf(thefile->f, " reached end of list.\n");
  }
  putc('\n', thefile->f);
}



Static Void checknames()
{
  /*
*/
  fscanf(hlist.f, "%*[^\n]");
  getc(hlist.f);
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
  boolean done = false;

  *numbers.name = '\0';
  *linesused = 0;
  if (*numbers.name != '\0')
    numbers.f = fopen(numbers.name, "wb");
  else
    numbers.f = tmpfile();
  if (numbers.f == NULL)
    _EscIO2(FileNotFound, numbers.name);
  SETUPBUF(numbers.f, long);
  index = firstnumber;
  while (!done) {
    number = inttopie(index, pie);
    fwrite(&number, sizeof(long), 1, numbers.f);
    width = numbersize(number);
    if (width > *linesused)
      *linesused = width;
    index += yscale;
    if (index > lastnumber) {
      done = true;
      /*
*/
    }
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
  /*


*/
  long spacecount, index, FORLIM;

  for (spacecount = 1; spacecount <= spaces - 3; spacecount++)
    putc(' ', afile->f);
  fprintf(afile->f, "5%c ", prime);
  if (yscale == 1) {
    for (index = firstnumber; index <= lastnumber; index++)
      fputc(basetochar(getbase(index, pie)), afile->f);
  } else {
    FORLIM = (long)((double)lastnumber / yscale) + 1;
    for (index = firstnumber; index <= FORLIM; index++)
      putc(' ', afile->f);
  }
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
  fprintf(pageheader->f, " keymat,  x down: %ld %.*s y across: %ld %.*s\n\n",
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
      putc(P_peek(pageheader->f), kmlist.f);
      getc(pageheader->f);
    }
    fscanf(pageheader->f, "%*[^\n]");
    getc(pageheader->f);
    putc('\n', kmlist.f);
  }
}


/* Local variables for printlist: */
struct LOC_printlist {
  holder *freeholder;
  helix *freerecord;
  long y;
} ;

Local Void blankrow(LINK)
struct LOC_printlist *LINK;
{
  LINK->y = 1;
  while (LINK->y <= ylength) {
    putc(nodot, kmlist.f);
    LINK->y++;
  }
  putc('\n', kmlist.f);
}

Local Void writenodots(y, next, LINK)
long *y, next;
struct LOC_printlist *LINK;
{
  /*
*/
  while (*y < next) {
    putc(nodot, kmlist.f);
    (*y)++;
  }
}

Local Void freer(ptr, LINK)
helix *ptr;
struct LOC_printlist *LINK;
{
  ptr->across = LINK->freerecord;
  LINK->freerecord = ptr;
  ptr = NULL;
}

Local Void freeh(ptr, LINK)
holder **ptr;
struct LOC_printlist *LINK;
{
  holder *temp;

  temp = *ptr;
  *ptr = (*ptr)->down;
  temp->down = LINK->freeholder;
  LINK->freeholder = temp;
  temp->across = NULL;
}



Static Void printlist(root, freeholder_, freerecord_)
holder *root, *freeholder_;
helix *freerecord_;
{
  struct LOC_printlist V;
  _TEXT pageheader;
  base bx, by;
  long x = 1;
  helix *current;

  V.freeholder = freeholder_;
  V.freerecord = freerecord_;
  pageheader.f = NULL;
  *pageheader.name = '\0';
  makepageheader(&pageheader);
  while (x <= xlength) {
    if (x % pagelength == 0 || x == 1)
      printpageheader(&pageheader);

    bx = getbase(x, xseq);
    fprintf(kmlist.f, " %6ld ", inttopie(x, xseq));
    fprintf(kmlist.f, "%c ", basetochar(bx));

    if (root == NULL)
      blankrow(&V);
    else if (x < root->x)
      blankrow(&V);
    else {
      V.y = 1;
      while (root->across != NULL) {
	current = root->across;
	writenodots(&V.y, current->y3pos, &V);


	if (gensymbol == 'e')
	  putc(current->symbol, kmlist.f);
	else if (gensymbol == 'n') {
	  by = getbase(current->y3pos, yseq);
	  if (bx == complement(by)) {
	    switch (bx) {

	    case a:
	      putc('2', kmlist.f);
	      break;

	    case c:
	      putc('3', kmlist.f);
	      break;

	    case g:
	      putc('3', kmlist.f);
	      break;

	    case t:
	      putc('2', kmlist.f);
	      break;
	    }
	  } else
	    putc('1', kmlist.f);
	} else if (gensymbol == 'x')
	  fputc(basetochar(bx), kmlist.f);
	else if (gensymbol == 'y') {
	  by = getbase(current->y3pos, yseq);
	  fputc(basetochar(by), kmlist.f);
	}

	V.y++;
	root->across = current->across;
	current->across = NULL;
	current->x5pos++;
	current->y3pos--;
	current->length--;
	if (current->length < 1 || current->x5pos <= x)
	  freer(current, &V);
	else
	  sorthelix(&V.freeholder, &root, &current);
      }
      while (V.y <= ylength) {
	putc(nodot, kmlist.f);
	V.y++;
      }
      putc('\n', kmlist.f);
      freeh(&root, &V);
    }
    x++;
  }
  fprintf(kmlist.f, "        3%c\n", prime);
  if (pageheader.f != NULL)
    fclose(pageheader.f);
}


/* Local variables for sprintlist: */
struct LOC_sprintlist {
  holder **root, **freeholder;
  helix **freerecord;
  long y;
} ;

Local Void blankrow_(LINK)
struct LOC_sprintlist *LINK;
{
  LINK->y = 1;
  while (LINK->y <= ylength) {
    putc(nodot, kmlist.f);
    LINK->y += yscale;
  }
  putc('\n', kmlist.f);
}

/* Local variables for freer_: */
struct LOC_freer_ {
  struct LOC_sprintlist *LINK;
} ;

Local Void loopnfree(ptr1, ptr2, LINK)
helix **ptr1, **ptr2;
struct LOC_freer_ *LINK;
{
  *ptr1 = (*ptr2)->across;
  (*ptr2)->across = NULL;
  if ((*ptr2)->length < 1) {
    (*ptr2)->across = *LINK->LINK->freerecord;
    *LINK->LINK->freerecord = *ptr2;
  } else
    sorthelix(LINK->LINK->freeholder, LINK->LINK->root, ptr2);
  *ptr2 = *ptr1;
}

Local Void freer_(hptr, rptr, LINK)
holder **hptr;
helix **rptr;
struct LOC_sprintlist *LINK;
{
  /*


*/
  struct LOC_freer_ V;
  helix *ptr;

  V.LINK = LINK;
  if ((*hptr)->across == *rptr) {
    loopnfree(&(*hptr)->across, rptr, &V);
    return;
  }
  ptr = (*hptr)->across;
  while (ptr->across != *rptr)
    ptr = ptr->across;
  loopnfree(&ptr->across, rptr, &V);
}

/* Local variables for freeh_: */
struct LOC_freeh_ {
  struct LOC_sprintlist *LINK;
} ;

Local Void hloopnfree(hptr1, hptr2, LINK)
holder **hptr1, **hptr2;
struct LOC_freeh_ *LINK;
{
  *hptr1 = (*hptr2)->down;
  (*hptr2)->down = *LINK->LINK->freeholder;
  *LINK->LINK->freeholder = *hptr2;
  *hptr2 = *hptr1;
}

Local Void freeh_(root, hptr, LINK)
holder **root, **hptr;
struct LOC_sprintlist *LINK;
{
  /*
*/
  struct LOC_freeh_ V;
  holder *ptr;

  V.LINK = LINK;
  if (*root == *hptr) {
    hloopnfree(root, hptr, &V);
    return;
  }
  ptr = *root;
  while (ptr->down != *hptr)
    ptr = ptr->down;
  hloopnfree(&ptr->down, hptr, &V);
}



Static Void sprintlist(root_, freeholder_, freerecord_)
holder **root_, **freeholder_;
helix **freerecord_;
{
  struct LOC_sprintlist V;
  _TEXT pageheader;
  base bx;
  long x = 1, l = 1;
  long factor;
  helix *current;
  holder *currenth;
  boolean donerow, donebox;
  /*
*/
  boolean doneline;
  /*
*/
  /*
*/
  boolean printstate;

  V.root = root_;
  V.freeholder = freeholder_;
  V.freerecord = freerecord_;
  pageheader.f = NULL;
  *pageheader.name = '\0';
  makepageheader(&pageheader);
  while (x <= xlength) {
    if (l % pagelength == 0 || l == 1)
      printpageheader(&pageheader);

    bx = getbase(x, xseq);
    fprintf(kmlist.f, " %6ld ", inttopie(x, xseq));
    if (xscale == 1)
      fputc(basetochar(bx), kmlist.f);
    else
      putc(' ', kmlist.f);
    putc(' ', kmlist.f);

    if (*V.root == NULL)
      blankrow_(&V);
    else if (x + xscale - 1 < (*V.root)->x)
      blankrow_(&V);
    else {
      V.y = 1;
      donerow = false;
      currenth = *V.root;

      while (!donerow) {
	donebox = false;
	printstate = false;


	while (!donebox) {
	  if (currenth == NULL) {
	    donebox = true;
	    break;
	  }
	  if (currenth->x >= x + xscale) {
	    donebox = true;
	    break;
/* p2c: keymat.p: Note: Deleting unreachable code [255] */
	  }
	  current = currenth->across;
	  doneline = false;

	  /*
*/
	  while (!doneline) {
	    /*
*/
	    if (current == NULL) {
	      doneline = true;
	      break;
	    }
	    if (current->y3pos > V.y + yscale + xscale - current->x5pos + x - 2) {
	      /*
*/
	      doneline = true;
	      break;
/* p2c: keymat.p: Note: Deleting unreachable code [255] */
	    }
	    if (current->y3pos >= V.y + yscale) {
	      if (current->length >= current->y3pos - V.y - yscale + 2) {
		/*

*/
		/*


*/

/* p2c: keymat.p: Note: Eliminated unused assignment statement [338] */
		current = current->across;
	      }
	      /*
*/
	      else
		current = current->across;
	      continue;
	    }


	    printstate = true;
	    factor = x + xscale - current->x5pos;

	    current->x5pos += factor;
	    current->y3pos -= factor;
	    current->length -= factor;
	    /*

*/
	    freer_(&currenth, &current, &V);
	  }

	  /*



*/

	  if (currenth->across == NULL)
	    freeh_(V.root, &currenth, &V);

	  else {
	    currenth = currenth->down;

	  }
	}


	/*



*/

	if (printstate)
	  putc('*', kmlist.f);
	else
	  putc(nodot, kmlist.f);
	V.y += yscale;
	currenth = *V.root;
	if (V.y > ylength)
	  donerow = true;
      }
      putc('\n', kmlist.f);

      /*



*/
    }
    x += xscale;
    l++;
  }
  fprintf(kmlist.f, "        3%c\n", prime);
  if (pageheader.f != NULL)
    fclose(pageheader.f);
}



Static Void printmatrix(symbols, stops)
Char *symbols;
double *stops;
{
  /*

*/
  holder *root = NULL, *freeholder = NULL;
  helix *freerecord = NULL;
  _TEXT TEMP;


  checknames();
  makelist(&root, &freeholder, &freerecord, symbols, stops);
  if (debug) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showlist(root, &TEMP);
  }
  if (xscale > 1 || yscale > 1)
    sprintlist(&root, &freeholder, &freerecord);
  else
    printlist(root, freeholder, freerecord);
}



Static Void blank(symbols_, stops_)
Char *symbols_;
double *stops_;
{
  symbolholder symbols;
  stopholder stops;
  long i;

  memcpy(symbols, symbols_, sizeof(symbolholder));
  memcpy(stops, stops_, sizeof(stopholder));
  for (i = 0; i <= 36; i++) {
    symbols[i] = '?';
    stops[i] = 0.0;
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  keymatp.f = NULL;
  strcpy(keymatp.name, "keymatp");
  kmlist.f = NULL;
  strcpy(kmlist.name, "kmlist");
  hlist.f = NULL;
  strcpy(hlist.name, "hlist");
  ybook.f = NULL;
  strcpy(ybook.name, "ybook");
  xbook.f = NULL;
  strcpy(xbook.name, "xbook");
  initialize();

  if (gensymbol == 'e')
    dotable(&keymatp, &kmlist, &hlist, symbols, stops);
  else
    blank(symbols, stops);

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
      printmatrix(symbols, stops);
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
  if (kmlist.f != NULL)
    fclose(kmlist.f);
  if (keymatp.f != NULL)
    fclose(keymatp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
