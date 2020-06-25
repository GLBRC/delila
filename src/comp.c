/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "comp.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         5.27
/*


*/



/*















































































*/



#define defcompmax      2





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

/* p2c: comp.p, line 153:
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





typedef struct compnode {
  long count;
  struct compnode *son[4];
  /*
*/
} compnode;


typedef struct spider {
  /*


*/
  long depth;
  compnode *place;
  struct spider *next;
} spider;

/*
*/

typedef struct comptotal {
  long count;
  struct comptotal *next;
} comptotal;


typedef struct path {
  base bas;
  struct path *next;
} path;



Static _TEXT book, cmp, compp;
Static long compmax;
/*
*/
Static piece *pie;
Static compnode *root;
Static spider *firstspider;






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




Static Void makespiders()
{
  /*

*/
  long i;
  spider *aspider;
  long FORLIM;

  firstspider = (spider *)Malloc(sizeof(spider));
  firstspider->depth = 0;
  firstspider->place = root;
  firstspider->next = NULL;
  FORLIM = compmax;
  for (i = 1; i < FORLIM; i++) {
    aspider = (spider *)Malloc(sizeof(spider));
    aspider->next = firstspider;
    firstspider = aspider;
  }
}


Static Void initialize()
{
  base ba;

  printf("comp %4.2f\n", version);
  brinit(&book);
  if (*cmp.name != '\0') {
    if (cmp.f != NULL)
      cmp.f = freopen(cmp.name, "w", cmp.f);
    else
      cmp.f = fopen(cmp.name, "w");
  } else {
    if (cmp.f != NULL)
      rewind(cmp.f);
    else
      cmp.f = tmpfile();
  }
  if (cmp.f == NULL)
    _EscIO2(FileNotFound, cmp.name);
  SETUPBUF(cmp.f, Char);
  fprintf(cmp.f, " comp %4.2f: composition of \n", version);
  putc(' ', cmp.f);
  copyaline(&book, &cmp);


  if (*compp.name != '\0') {
    if (compp.f != NULL)
      compp.f = freopen(compp.name, "r", compp.f);
    else
      compp.f = fopen(compp.name, "r");
  } else
    rewind(compp.f);
  if (compp.f == NULL)
    _EscIO2(FileNotFound, compp.name);
  RESETBUF(compp.f, Char);
  if (!BUFEOF(compp.f)) {
    fscanf(compp.f, "%ld%*[^\n]", &compmax);
    getc(compp.f);
  } else
    compmax = defcompmax;
  if (compmax <= 0) {
    printf(" composition maximum must be positive\n");
    halt();
  }
  fprintf(cmp.f, " %ld is the longest oligo counted\n\n", compmax);


  root = (compnode *)Malloc(sizeof(compnode));
  root->count = 0;
  for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1))
    root->son[(long)ba] = NULL;

  makespiders();
  pie = (piece *)Malloc(sizeof(piece));
}


Static Void increment(s, ba)
spider **s;
base *ba;
{
  /*
*/
  base bai;
  spider *WITH;
  compnode *WITH1;

  WITH = *s;
  if (WITH->depth == 0)
    WITH->place->count++;
  WITH->depth++;
  if (WITH->place->son[(long)(*ba)] == NULL) {
    WITH->place->son[(long)(*ba)] = (compnode *)Malloc(sizeof(compnode));
    WITH->place = WITH->place->son[(long)(*ba)];
    WITH1 = WITH->place;
    WITH1->count = 1;
    for (bai = a; (long)bai <= (long)t; bai = (base)((long)bai + 1))
      WITH1->son[(long)bai] = NULL;
  } else {
    WITH->place = WITH->place->son[(long)(*ba)];
    WITH->place->count++;
  }
  if (WITH->depth == compmax) {
    WITH->depth = 0;
    WITH->place = root;
  }
}


Static Void maketree(pie, root, firstspider)
piece *pie;
compnode *root;
spider *firstspider;
{
  /*
*/
  spider *active;
  long i, j, length;
  dnarange pos = 0;
  dnastring *thedna;
  base ba;
  long FORLIM, FORLIM1;

  thedna = pie->dna;
  active = firstspider;

  FORLIM = compmax;
  for (i = 1; i <= FORLIM; i++) {
    active->depth = 0;
    active->place = root;
    active = active->next;
  }
  length = piecelength(pie);
  if (length >= compmax) {
    FORLIM = compmax;
    for (i = 1; i <= FORLIM; i++) {
      active = firstspider;
      ba = stepbase(pie->dna, &thedna, &pos);
      for (j = 1; j <= i; j++) {
	increment(&active, &ba);
	active = active->next;
      }
    }

    for (i = compmax + 1; i <= length; i++) {
      active = firstspider;
      ba = stepbase(pie->dna, &thedna, &pos);
      FORLIM1 = compmax;
      for (j = 1; j <= FORLIM1; j++) {
	increment(&active, &ba);
	active = active->next;
      }
    }
    if (pie->key.piecon != circular)
      return;

    FORLIM = length + compmax;
    for (i = length + 1; i < FORLIM; i++) {
      active = firstspider;
      ba = stepbase(pie->dna, &thedna, &pos);
      FORLIM1 = compmax;
      for (j = 1; j <= FORLIM1; j++) {
	if (active->depth >= i - length)
	  increment(&active, &ba);
	active = active->next;
      }
    }
    return;
  }

  for (i = 1; i <= length; i++) {
    active = firstspider;
    ba = stepbase(pie->dna, &thedna, &pos);
    for (j = 1; j <= i; j++) {
      increment(&active, &ba);
      active = active->next;
    }
  }
  if (pie->key.piecon != circular)
    return;
  FORLIM = compmax;
  for (i = length + 1; i <= FORLIM; i++) {
    active = firstspider;
    ba = stepbase(pie->dna, &thedna, &pos);
    FORLIM1 = compmax;
    for (j = 1; j <= FORLIM1; j++) {
      if (active->depth >= i - length)
	increment(&active, &ba);
      active = active->next;
    }
  }
}


/* Local variables for writetree: */
struct LOC_writetree {
  _TEXT *thefile;
  path *start;
  long printdepth;
} ;

/* Local variables for advancetonode: */
struct LOC_advancetonode {
  struct LOC_writetree *LINK;
  long depth;
} ;


Local Void writecomp(point, count, LINK)
path *point;
long count;
struct LOC_advancetonode *LINK;
{
  long i, FORLIM;

  fprintf(LINK->LINK->thefile->f, "%6c", ' ');

  fputc(basetochar(point->bas), LINK->LINK->thefile->f);
  FORLIM = LINK->depth;
  for (i = 2; i <= FORLIM; i++) {
    point = point->next;
    fputc(basetochar(point->bas), LINK->LINK->thefile->f);
  }
  fprintf(LINK->LINK->thefile->f, " %*ld", (int)(10 - LINK->depth), count);
  if (point->bas == t)
    putc('\n', LINK->LINK->thefile->f);
}


Local Void advancetonode(depth_, thenode, point, LINK)
long depth_;
compnode *thenode;
path *point;
struct LOC_writetree *LINK;
{
  /*


*/
  struct LOC_advancetonode V;
  base ba;

  V.LINK = LINK;
  V.depth = depth_;
  if (V.depth == LINK->printdepth) {
    if (thenode != NULL)
      writecomp(LINK->start, thenode->count, &V);
    else
      writecomp(LINK->start, 0L, &V);
    return;
  }
  if (thenode == NULL)
    return;
  for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1)) {
    point->bas = ba;
    switch (ba) {

    case a:
      advancetonode(V.depth + 1, thenode->son[(long)a], point->next, LINK);
      break;

    case c:
      advancetonode(V.depth + 1, thenode->son[(long)c], point->next, LINK);
      break;

    case g:
      advancetonode(V.depth + 1, thenode->son[(long)g], point->next, LINK);
      break;

    case t:
      advancetonode(V.depth + 1, thenode->son[(long)t], point->next, LINK);
      break;
    }
  }
}



Static Void writetree(thefile_, root)
_TEXT *thefile_;
compnode *root;
{
  /*

*/
  struct LOC_writetree V;
  path *point;
  long i, FORLIM;


  V.thefile = thefile_;
  V.start = (path *)Malloc(sizeof(path));
  V.start->next = NULL;
  FORLIM = compmax;
  for (i = 1; i < FORLIM; i++) {
    point = (path *)Malloc(sizeof(path));
    point->next = V.start;
    V.start = point;
  }

  fprintf(V.thefile->f, " *\n");
  fprintf(V.thefile->f, " 0-long oligos (the total number of bases)\n");
  fprintf(V.thefile->f, " %16ld\n", root->count);

  FORLIM = compmax;
  for (V.printdepth = 1; V.printdepth <= FORLIM; V.printdepth++) {
    fprintf(V.thefile->f, " *\n");
    fprintf(V.thefile->f, " %ld-long oligos\n", V.printdepth);
    advancetonode(0L, root, V.start, &V);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  compp.f = NULL;
  strcpy(compp.name, "compp");
  cmp.f = NULL;
  strcpy(cmp.name, "cmp");
  book.f = NULL;
  strcpy(book.name, "book");
  initialize();
  while (!BUFEOF(book.f)) {
    getpiece(&book, &pie);
    if (BUFEOF(book.f))
      break;
    maketree(pie, root, firstspider);
    clearpiece(&pie);
  }
  writetree(&cmp, root);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (cmp.f != NULL)
    fclose(cmp.f);
  if (compp.f != NULL)
    fclose(compp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
