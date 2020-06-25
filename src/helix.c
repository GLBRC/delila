/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "helix.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*




*/



#define version         3.24
/*
*/



/*




































































*/


/*


*/


#define absminlength    2
/*
*/
#define defminlenmaxene  5

#define defguallowed    false
#define defgivefreeenergy  false

#define prime           '\''

#define debugging       true




#define dnamax          3000
#define namelength      20
#define linelength      80






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

/* p2c: helix.p, line 155:
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



Static _TEXT xbook, ybook, hlist, helixp;

Static piece *xseq, *yseq;
Static long xnumber, ynumber;
Static double minlenmaxene;
/*
*/
Static boolean guallowed, givefreeenergy;
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



Static boolean basepair(xseq, yseq, x, y, guallowed)
piece *xseq, *yseq;
long x, y;
boolean guallowed;
{
  /*

*/
  base bx, by;

  bx = getbase(x, xseq);
  by = getbase(y, yseq);

  if (bx == complement(by))
    return true;
  else if (guallowed) {
    if (bx == g && by == t)
      return true;
    else if (bx == t && by == g)
      return true;
    else
      return false;
  } else
    return false;
}




/*





*/

Static Void moveendsin(x5bound, x3bound, xpiece, y5bound, y3bound, ypiece)
long *x5bound, *x3bound;
piece *xpiece;
long *y5bound, *y3bound;
piece *ypiece;
{
  /*
*/
  /*

*/
  /*


*/

  while (((getbase(*x3bound, xpiece) != complement(getbase(*y5bound, ypiece))) |
	  (getbase(*x3bound - 1, xpiece) !=
	   complement(getbase(*y5bound + 1, ypiece)))) &&
	 *x3bound > *x5bound + 1) {
    (*x3bound)--;
    (*y5bound)++;
  }

  /*

*/
  while (((getbase(*x5bound, xpiece) != complement(getbase(*y3bound, ypiece))) |
	  (getbase(*x5bound + 1, xpiece) !=
	   complement(getbase(*y3bound - 1, ypiece)))) &&
	 *x5bound < *x3bound) {
    (*x5bound)++;
    (*y3bound)--;
  }
}


/* Local variables for freeenergy: */
struct LOC_freeenergy {
  piece *xpiece, *ypiece;
  long x5, y3;
  base x5b, x3b;
  /*
*/
  double energy;
  long bulgebases;
  boolean bulging;
} ;

Local Void readofftable(LINK)
struct LOC_freeenergy *LINK;
{
  /*

*/
  double deltag;

  switch (LINK->x5b) {

  case a:
    switch (LINK->x3b) {

    case a:
      deltag = -0.9;
      break;

    case c:
      deltag = -2.1;
      break;

    case g:
      deltag = -1.7;
      break;

    case t:
      deltag = -0.9;
      break;
    }
    break;

  case c:
    switch (LINK->x3b) {

    case a:
      deltag = -1.8;
      break;

    case c:
      deltag = -2.9;
      break;

    case g:
      deltag = -2.0;
      break;

    case t:
      deltag = -1.7;
      break;
    }
    break;

  case g:
    switch (LINK->x3b) {

    case a:
      deltag = -2.3;
      break;

    case c:
      deltag = -3.4;
      break;

    case g:
      deltag = -2.9;
      break;

    case t:
      deltag = -2.1;
      break;
    }
    break;

  case t:
    switch (LINK->x3b) {

    case a:
      deltag = -1.1;
      break;

    case c:
      deltag = -2.3;
      break;

    case g:
      deltag = -1.8;
      break;

    case t:
      deltag = -0.9;
      break;
    }
    break;
  }
  LINK->energy += deltag;
}

Local Void gupair(LINK)
struct LOC_freeenergy *LINK;
{
  LINK->energy -= 0.4;
}

Local Void continuebulge(LINK)
struct LOC_freeenergy *LINK;
{
  LINK->bulgebases += 2;
}

Local Void beginbulge(LINK)
struct LOC_freeenergy *LINK;
{
  if (!LINK->bulging) {
    LINK->bulgebases = 2;
    LINK->bulging = true;
  } else {
    /*
*/
    continuebulge(LINK);
  }
}

Local Void endbulge(LINK)
struct LOC_freeenergy *LINK;
{
  if (getbase(LINK->x5 - 1, LINK->xpiece) !=
      complement(getbase(LINK->y3 + 1, LINK->ypiece))) {
    continuebulge(LINK);
    return;
  }
  if (2 <= LINK->bulgebases && LINK->bulgebases <= 6)
    LINK->energy += 2.0;
  else if (7 <= LINK->bulgebases && LINK->bulgebases <= 20)
    LINK->energy += 3.0;
  else
    LINK->energy += 1.0 + 2.0 * log((double)LINK->bulgebases) / log(10.0);
  LINK->bulging = false;
}


Static double freeenergy(x5start, x3start, xpiece_, y5start, y3start, ypiece_)
long x5start, x3start;
piece *xpiece_;
long y5start, y3start;
piece *ypiece_;
{
  /*







*/
  struct LOC_freeenergy V;
  long x3, y5;
  base y5b, y3b;
  long x5bound, x3bound, y5bound, y3bound;


  V.xpiece = xpiece_;
  V.ypiece = ypiece_;
  if (x5start - x3start != y5start - y3start) {
    printf(" function freeenergy:\n");
    printf(" the supplied ends are not consistent with a helix.\n");
    halt();
  }

  x5bound = x5start;
  x3bound = x3start;
  y5bound = y5start;
  y3bound = y3start;

  moveendsin(&x5bound, &x3bound, V.xpiece, &y5bound, &y3bound, V.ypiece);


  V.energy = 0.0;


  V.bulging = false;

  if (x5bound >= x3bound)
    return V.energy;

  x3 = x3bound;
  y5 = y5bound;
  V.x5 = x3 - 1;
  V.y3 = y5 + 1;

  /*

*/
  do {

    V.x5b = getbase(V.x5, V.xpiece);
    V.x3b = getbase(x3, V.xpiece);
    y5b = getbase(y5, V.ypiece);
    y3b = getbase(V.y3, V.ypiece);

    if ((V.x3b == complement(y5b)) & (V.x5b == complement(y3b)))
      readofftable(&V);
    else if (y5b == g && y3b == t && V.x3b == t && V.x5b == g)
      gupair(&V);
    else if ((V.x5b != complement(y3b)) & (V.x3b != complement(y5b)))
      continuebulge(&V);
    else if (V.x3b == complement(y5b))
      beginbulge(&V);
    else if (V.x5b == complement(y3b))
      endbulge(&V);
    else
      halt();

    x3--;
    y5++;
    V.x5--;
    V.y3++;

  } while (V.x5 >= x5bound);
  return V.energy;
}



Static Void unlimitln(afile)
_TEXT *afile;
{
  /*


*/
}


Static Void initialize()
{
  printf(" helix %4.2f\n", version);
  brinit(&xbook);
  brinit(&ybook);
  unlimitln(&hlist);
  if (*hlist.name != '\0') {
    if (hlist.f != NULL)
      hlist.f = freopen(hlist.name, "w", hlist.f);
    else
      hlist.f = fopen(hlist.name, "w");
  } else {
    if (hlist.f != NULL)
      rewind(hlist.f);
    else
      hlist.f = tmpfile();
  }
  if (hlist.f == NULL)
    _EscIO2(FileNotFound, hlist.name);
  SETUPBUF(hlist.f, Char);

  xseq = (piece *)Malloc(sizeof(piece));
  yseq = (piece *)Malloc(sizeof(piece));


  if (*helixp.name != '\0') {
    if (helixp.f != NULL)
      helixp.f = freopen(helixp.name, "r", helixp.f);
    else
      helixp.f = fopen(helixp.name, "r");
  } else
    rewind(helixp.f);
  if (helixp.f == NULL)
    _EscIO2(FileNotFound, helixp.name);
  RESETBUF(helixp.f, Char);
  if (BUFEOF(helixp.f)) {
    minlenmaxene = defminlenmaxene;
    guallowed = defguallowed;
    givefreeenergy = defgivefreeenergy;
  } else {
    fscanf(helixp.f, "%lg%*[^\n]", &minlenmaxene);
    getc(helixp.f);

    if (BUFEOF(helixp.f)) {
      guallowed = defguallowed;
      givefreeenergy = defgivefreeenergy;
    } else {
      if (P_peek(helixp.f) == 'g')
	guallowed = true;
      else
	guallowed = false;
      fscanf(helixp.f, "%*[^\n]");

      getc(helixp.f);
      if (BUFEOF(helixp.f))
	givefreeenergy = defgivefreeenergy;
      else if (P_peek(helixp.f) == 'e')
	givefreeenergy = true;
      else
	givefreeenergy = false;
    }
  }

  if (minlenmaxene < absminlength && minlenmaxene > 0) {
    minlenmaxene = absminlength;
    printf(" warning: the minimum length helix printed is %ld\n",
	   (long)minlenmaxene);
  }


  fprintf(hlist.f, " helix %4.2f\n", version);

  fprintf(hlist.f, " x: ");
  copyaline(&xbook, &hlist);

  fprintf(hlist.f, " y: ");
  copyaline(&ybook, &hlist);

  if (minlenmaxene > 0)
    fprintf(hlist.f, " %ld base pairs is the minimum length helix recorded\n",
	    (long)minlenmaxene);
  else
    fprintf(hlist.f,
	    " %4.2f kcal is the maximum energy of a helix recorded\n",
	    minlenmaxene);

  putc(' ', hlist.f);
  if (!guallowed)
    fprintf(hlist.f, "no ");
  fprintf(hlist.f, "gu pairs allowed\n");

  putc(' ', hlist.f);
  if (!givefreeenergy)
    fprintf(hlist.f, "no ");
  fprintf(hlist.f, "energies calculated\n\n");
}


/*
*/

Static Void nextsequences(x, y, xnumber, ynumber)
piece *x, *y;
long xnumber, ynumber;
{
  fprintf(hlist.f, " x piece: %ld %.*s y piece: %ld %.*s\n",
	  xnumber, namelength, x->key.hea.keynam.letters, ynumber, namelength,
	  y->key.hea.keynam.letters);
}


Static Void hrecord(x5, y3, xsequence, ysequence, length)
long x5, y3;
piece *xsequence, *ysequence;
long length;
{
  /*




*/
  long x3, y5;
  double deltag;

  y5 = y3 - length + 1;

  if (givefreeenergy) {
    x3 = x5 + length - 1;
    deltag = freeenergy(x5, x3, xsequence, y5, y3, ysequence);
  } else
    deltag = 0.0;

  if ((minlenmaxene <= 0.0 || length < minlenmaxene) &&
      (minlenmaxene > 0.0 || deltag > minlenmaxene))
    return;
  fprintf(hlist.f, "    x5%c: %6ld    y5%c: %6ld    length: %3ld",
	  prime, inttopie(x5, xsequence), prime, inttopie(y5, ysequence),
	  length);
  if (givefreeenergy)
    fprintf(hlist.f, "  %7.2f kcal", deltag);
  putc('\n', hlist.f);
}


Static Void discani(xmin, ymin, x, y, xmax, ymax, x0, endofdiagonal,
		    endofscan)
long xmin, ymin, *x, *y, xmax, ymax, *x0;
boolean *endofdiagonal, *endofscan;
{
  /*
*/
  boolean xdone, ydone;

  *x0 = xmin;
  *x = *x0;
  *y = ymin;

  xdone = (*x == xmin);
  ydone = (*y == ymax);

  *endofdiagonal = (xdone || ydone);
  *endofscan = (*x == xmax && ydone);
}


Static Void discans(xmin, ymin, x, y, xmax, ymax, x0, endofdiagonal,
		    endofscan)
long xmin, ymin, *x, *y, xmax, ymax, *x0;
boolean *endofdiagonal, *endofscan;
{
  /*













*/
  boolean xdone, ydone;

  if (*endofdiagonal) {
    if (*endofscan)
      halt();
    (*x0)++;

    if (*x0 <= xmax) {
      *x = *x0;
      *y = ymin;
    } else {
      *x = xmax;
      *y = ymin + *x0 - xmax;
    }
  } else {
    (*x)--;
    (*y)++;
  }

  xdone = (*x == xmin);
  ydone = (*y == ymax);

  *endofdiagonal = (xdone || ydone);
  *endofscan = (*x == xmax && ydone);
  /*





*/
}


/* Local variables for scan: */
struct LOC_scan {
  piece *xseq, *yseq;
  boolean guallowed;
  long x, y, length;
  boolean eod;
} ;

Local Void terminate(x, y, LINK)
long x, y;
struct LOC_scan *LINK;
{
  if (LINK->length >= absminlength)
    hrecord(x, y, LINK->xseq, LINK->yseq, LINK->length);
  LINK->length = 0;
}

Local Void look(LINK)
struct LOC_scan *LINK;
{
  if (!basepair(LINK->xseq, LINK->yseq, LINK->x, LINK->y, LINK->guallowed)) {
    terminate(LINK->x + 1, LINK->y - 1, LINK);
    return;
  }
  LINK->length++;
  if (LINK->eod)
    terminate(LINK->x, LINK->y, LINK);
}


Static Void scan(xseq_, yseq_, minlenmaxene, guallowed_)
piece *xseq_, *yseq_;
double minlenmaxene;
boolean guallowed_;
{
  /*











*/
  struct LOC_scan V;
  long xlength, ylength, x0;
  boolean eos;

  V.xseq = xseq_;
  V.yseq = yseq_;
  V.guallowed = guallowed_;
  nextsequences(V.xseq, V.yseq, xnumber, ynumber);
  xlength = piecelength(V.xseq);
  ylength = piecelength(V.yseq);
  V.length = 0;

  discani(1L, 1L, &V.x, &V.y, xlength, ylength, &x0, &V.eod, &eos);
  look(&V);
  while (!eos) {
    discans(1L, 1L, &V.x, &V.y, xlength, ylength, &x0, &V.eod, &eos);
    look(&V);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  helixp.f = NULL;
  strcpy(helixp.name, "helixp");
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
      if (BUFEOF(ybook.f))
	break;
      scan(xseq, yseq, minlenmaxene, guallowed);
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
  if (helixp.f != NULL)
    fclose(helixp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
