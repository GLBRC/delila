/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "winrs.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.17
/*







*/



/*












































































*/



/*
*/

#define dnamax          400000L

#define namelength      20
#define linelength      80

#define LongestWindowAllowed  5000
#define infofield       8
#define infodecim       5






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
typedef long dnarange;

/* p2c: winrs.p, line 160:
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


Static _TEXT book, cmp, winrsp, xyin;






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



Static Void copyaline(fin, data)
_TEXT *fin, *data;
{
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), data->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
  putc('\n', data->f);
}





Static Void skipoligo(thefile)
_TEXT *thefile;
{
  /*


*/
  Char ch;

  do {
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != 't' && ch != 'g' && ch != 'c' && ch != 'a');
  do {
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != ' ');
}


/*


*/

typedef struct list {
  compnode *item;
  struct list *next;
} list;


/* Local variables for readcomp: */
struct LOC_readcomp {
  list *freeitem;
} ;

/*

*/
Local Void getitem(l, LINK)
list **l;
struct LOC_readcomp *LINK;
{
  if (LINK->freeitem != NULL) {
    *l = LINK->freeitem;
    LINK->freeitem = LINK->freeitem->next;
  } else
    *l = (list *)Malloc(sizeof(list));
  (*l)->next = NULL;
}


Local Void clearitem(l, LINK)
list **l;
struct LOC_readcomp *LINK;
{
  list *lptr;

  if (*l == NULL)
    return;
  lptr = *l;
  *l = (*l)->next;
  lptr->next = LINK->freeitem;
  LINK->freeitem = lptr;
}


Static Void readcomp(comp, compmax, readmax, root, monocomptotal)
_TEXT *comp;
long *compmax, readmax;
compnode **root;
comptotal **monocomptotal;
{
  /*






*/
  struct LOC_readcomp V;
  list *listitem, *first, *last;
  comptotal *comptot, *newcomptot;
  long detcomp, level, number;
  Char ch;
  base ba;

  if (*comp->name != '\0') {
    if (comp->f != NULL)
      comp->f = freopen(comp->name, "r", comp->f);
    else
      comp->f = fopen(comp->name, "r");
  } else
    rewind(comp->f);
  if (comp->f == NULL)
    _EscIO2(FileNotFound, comp->name);
  RESETBUF(comp->f, Char);
  if (BUFEOF(comp->f)) {
    printf(" error: no composition file provided\n");
    halt();
  }
  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%ld%*[^\n]", &detcomp);
  getc(comp->f);
  fscanf(comp->f, "%*[^\n]");


  getc(comp->f);
  if (readmax < 1) {
    printf("\n warning: 0 or negative oligo length requested\n");
    printf(" composition used is depth %ld\n\n", detcomp);
    *compmax = detcomp;
  } else if (readmax > detcomp) {
    printf("\n warning: requested composition oligo length (%ld)\n", readmax);
    printf(" is larger than the determined composition oligo length (%ld).\n",
	   detcomp);
    printf(" composition used is to depth %ld\n\n", detcomp);
    *compmax = detcomp;
  } else
    *compmax = readmax;

  *root = (compnode *)Malloc(sizeof(compnode));
  *monocomptotal = (comptotal *)Malloc(sizeof(comptotal));
  comptot = (comptotal *)Malloc(sizeof(comptotal));
  first = (list *)Malloc(sizeof(list));
  Malloc(sizeof(list));
/* p2c: winrs.p: Note: Eliminated unused assignment statement [338] */
  first->item = *root;
  first->next = NULL;
  last = first;
  V.freeitem = (list *)Malloc(sizeof(list));
  V.freeitem->next = NULL;


  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%ld%*[^\n]", &number);
  getc(comp->f);
  (*root)->count = number;

  do {
    ch = getc(comp->f);
    if (ch == '\n')
      ch = ' ';
    ch = getc(comp->f);
    if (ch == '\n')
      ch = ' ';
    if (ch == '*') {
      fscanf(comp->f, "%*[^\n]");
      getc(comp->f);
      fscanf(comp->f, "%ld%*[^\n]", &level);
      getc(comp->f);
      if (level == 1)
	*monocomptotal = comptot;
      else {
	newcomptot = (comptotal *)Malloc(sizeof(comptotal));
	comptot->next = newcomptot;
	comptot = newcomptot;
      }
      comptot->count = 0;
      comptot->next = NULL;
    } else {
      for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1)) {
	skipoligo(comp);
	fscanf(comp->f, "%ld", &number);
	if (number != 0) {
	  first->item->son[(long)ba] = (compnode *)Malloc(sizeof(compnode));
	  first->item->son[(long)ba]->count = number;
	  comptot->count += number;
	  getitem(&listitem, &V);
	  last->next = listitem;
	  last = listitem;
	  last->next = NULL;
	  last->item = first->item->son[(long)ba];
	} else
	  first->item->son[(long)ba] = NULL;
      }
      clearitem(&first, &V);
      fscanf(comp->f, "%*[^\n]");
      getc(comp->f);
    }
  } while (level != *compmax);
  /*

*/
  do {
    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1)) {
      skipoligo(comp);
      fscanf(comp->f, "%ld", &number);
      if (number != 0) {
	first->item->son[(long)ba] = (compnode *)Malloc(sizeof(compnode));
	first->item->son[(long)ba]->count = number;
	comptot->count += number;
      } else
	first->item->son[(long)ba] = NULL;
    }
    clearitem(&first, &V);
    fscanf(comp->f, "%*[^\n]");
    getc(comp->f);
  } while (first != NULL);
}


Static long getcount(root, start)
compnode *root;
path *start;
{
  /*



*/
  compnode *place = root;
  path *point = start;

  while (place != NULL && point != NULL) {
    place = place->son[(long)point->bas];
    point = point->next;
  }
  if (place == NULL)
    return 0;
  else
    return (place->count);
}




Static Void getmonocomposition(cmp, gna, gnc, gng, gnt, data, equalmono)
_TEXT *cmp;
long *gna, *gnc, *gng, *gnt;
_TEXT *data;
boolean *equalmono;
{
  /*

*/
  long compmax;
  compnode *root;
  comptotal *monocomptotal;
  path *start;
  base b1;
  long count;

  if (*cmp->name != '\0') {
    if (cmp->f != NULL)
      cmp->f = freopen(cmp->name, "r", cmp->f);
    else
      cmp->f = fopen(cmp->name, "r");
  } else
    rewind(cmp->f);
  if (cmp->f == NULL)
    _EscIO2(FileNotFound, cmp->name);
  RESETBUF(cmp->f, Char);
  if (BUFEOF(cmp->f)) {
    fprintf(data->f, "* the composition file is empty.\n");
    fprintf(data->f, "* (equal mononucleotides assumed)\n");
    *gna = 1;
    *gnc = 1;
    *gng = 1;
    *gnt = 1;
    *equalmono = true;
    return;
  }
  putc('*', data->f);
  copyaline(cmp, data);
  putc('*', data->f);
  copyaline(cmp, data);

  readcomp(cmp, &compmax, 1L, &root, &monocomptotal);


  start = (path *)Malloc(sizeof(path));
  start->next = NULL;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    start->bas = b1;
    count = getcount(root, start);
    switch (b1) {

    case a:
      *gna = count;
      break;

    case c:
      *gnc = count;
      break;

    case g:
      *gng = count;
      break;

    case t:
      *gnt = count;
      break;
    }
  }
  Free(start);
  *equalmono = false;
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


#define debugging       false

#define kickover        50


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *book, *cmp, *winrsp, *xyin;
  /*

*/
  piece *apiece;
  long bases[(long)t - (long)a + 1];
  double Ehnb;
  boolean equalmono;
  /*
*/
  double f;
  double flogf[LongestWindowAllowed + 1];

  long gna, gnc, gng, gnt;
  double hg;
  long length;
  double ln2;
  long MinimumWindowSize, MaximumWindowSize, NumberOfWindows,
       PositionOnSequence;
  double Rs, SD;
  long SpaceBetweenWindows, StartCoordinate;
  double varhnb;
} ;

Local Void die(LINK)
struct LOC_themain *LINK;
{
  /*
*/
  if (*LINK->xyin->name != '\0') {
    if (LINK->xyin->f != NULL)
      LINK->xyin->f = freopen(LINK->xyin->name, "w", LINK->xyin->f);
    else
      LINK->xyin->f = fopen(LINK->xyin->name, "w");
  } else {
    if (LINK->xyin->f != NULL)
      rewind(LINK->xyin->f);
    else
      LINK->xyin->f = tmpfile();
  }
  if (LINK->xyin->f == NULL)
    _EscIO2(FileNotFound, LINK->xyin->name);
  SETUPBUF(LINK->xyin->f, Char);
  halt();
}

Local Void ReadParametersIn(LINK)
struct LOC_themain *LINK;
{
  if (*LINK->winrsp->name != '\0') {
    if (LINK->winrsp->f != NULL)
      LINK->winrsp->f = freopen(LINK->winrsp->name, "r", LINK->winrsp->f);
    else
      LINK->winrsp->f = fopen(LINK->winrsp->name, "r");
  } else
    rewind(LINK->winrsp->f);
  if (LINK->winrsp->f == NULL)
    _EscIO2(FileNotFound, LINK->winrsp->name);
  RESETBUF(LINK->winrsp->f, Char);
  fscanf(LINK->winrsp->f, "%ld%*[^\n]", &LINK->StartCoordinate);
  getc(LINK->winrsp->f);
  fscanf(LINK->winrsp->f, "%ld%*[^\n]", &LINK->MinimumWindowSize);
  getc(LINK->winrsp->f);
  fscanf(LINK->winrsp->f, "%ld%*[^\n]", &LINK->MaximumWindowSize);
  getc(LINK->winrsp->f);
  fscanf(LINK->winrsp->f, "%ld%*[^\n]", &LINK->SpaceBetweenWindows);
  getc(LINK->winrsp->f);
  fscanf(LINK->winrsp->f, "%ld%*[^\n]", &LINK->NumberOfWindows);
  getc(LINK->winrsp->f);



  if (LINK->StartCoordinate < 1) {
    printf("StartCoordinate cannot be less than 1\n");
    die(LINK);
  }
  /*
*/

  if (LINK->MinimumWindowSize < 1) {
    printf("MinimumWindowSize cannot be less than 1\n");
    die(LINK);
  }

  if (LINK->MaximumWindowSize < 1) {
    printf("MaximumWindowSize cannot be less than 1\n");
    die(LINK);
  }

  if (LINK->MaximumWindowSize > LongestWindowAllowed) {
    printf("MaximumWindowSize cannot exceed LongestWindowAllowed = %ld\n",
	   (long)LongestWindowAllowed);
    die(LINK);
  }

  if (LINK->MinimumWindowSize > LINK->MaximumWindowSize) {
    printf("No computation: MinimumWindowSize exceeds MaximumWindowSize\n");
    die(LINK);
  }

  if (LINK->NumberOfWindows < 1) {
    printf("No computation: NumberOfWindows must be > 0\n");
    die(LINK);
  }

}

Local Void WriteParametersOut(LINK)
struct LOC_themain *LINK;
{
  if (*LINK->xyin->name != '\0') {
    if (LINK->xyin->f != NULL)
      LINK->xyin->f = freopen(LINK->xyin->name, "w", LINK->xyin->f);
    else
      LINK->xyin->f = fopen(LINK->xyin->name, "w");
  } else {
    if (LINK->xyin->f != NULL)
      rewind(LINK->xyin->f);
    else
      LINK->xyin->f = tmpfile();
  }
  if (LINK->xyin->f == NULL)
    _EscIO2(FileNotFound, LINK->xyin->name);
  SETUPBUF(LINK->xyin->f, Char);
  fprintf(LINK->xyin->f, "* winscan %4.2f\n", version);
  fprintf(LINK->xyin->f, "*\n");
  fprintf(LINK->xyin->f, "* paremeters:\n");
  fprintf(LINK->xyin->f, "* %10ld Start Coordinate\n", LINK->StartCoordinate);
  fprintf(LINK->xyin->f, "* %10ld Minimum Window Size\n",
	  LINK->MinimumWindowSize);
  fprintf(LINK->xyin->f, "* %10ld Maximum Window Size\n",
	  LINK->MaximumWindowSize);
  fprintf(LINK->xyin->f, "* %10ld Space Between Windows\n",
	  LINK->SpaceBetweenWindows);
  fprintf(LINK->xyin->f, "* %10ld Number Of Windows\n", LINK->NumberOfWindows);

  fprintf(LINK->xyin->f, "*\n");
  if (LINK->SpaceBetweenWindows < 0) {
    fprintf(LINK->xyin->f, "*\n");
    printf("WINDOWS OVERLAP\n");
    fprintf(LINK->xyin->f, "* WINDOWS OVERLAP\n");
  } else {
    printf("windows do not overlap\n");
    fprintf(LINK->xyin->f, "* windows do not overlap\n");
  }

  fprintf(LINK->xyin->f, "*\n");
  fprintf(LINK->xyin->f, "* book:\n");
  if (*LINK->book->name != '\0') {
    if (LINK->book->f != NULL)
      LINK->book->f = freopen(LINK->book->name, "r", LINK->book->f);
    else
      LINK->book->f = fopen(LINK->book->name, "r");
  } else
    rewind(LINK->book->f);
  if (LINK->book->f == NULL)
    _EscIO2(FileNotFound, LINK->book->name);
  RESETBUF(LINK->book->f, Char);
  if (BUFEOF(LINK->book->f)) {
    printf("book is empty\n");
    die(LINK);
  }
  copyaline(LINK->book, LINK->xyin);
}

Local Void ReadInThePiece(LINK)
struct LOC_themain *LINK;
{
  brinit(LINK->book);
  LINK->apiece = (piece *)Malloc(sizeof(piece));
  getpiece(LINK->book, &LINK->apiece);
  LINK->length = piecelength(LINK->apiece);
  printf("Sequence length is %ld bases.\n", LINK->length);
  if (LINK->length > dnamax) {
    printf("The first piece in the book has a length of %ld bases.\n",
	   LINK->length);
    printf("This is larger than can be stored, dnamax is %ld bases.\n",
	   dnamax);
    die(LINK);
  }
  fprintf(LINK->xyin->f, "* %.*s %6ld bases\n",
	  namelength, LINK->apiece->key.hea.keynam.letters, LINK->length);
}

Local Void DoMoreChecksNowThatWeHaveTheLength(LINK)
struct LOC_themain *LINK;
{
  if (LINK->StartCoordinate > LINK->length) {
    printf("StartCoordinate cannot exceed sequence length\n");
    die(LINK);
  }

  if (LINK->StartCoordinate < 0) {
    printf("StartCoordinate must be positive.\n");
    die(LINK);
  }

  if (LINK->StartCoordinate + (LINK->MaximumWindowSize + LINK->
	    SpaceBetweenWindows) * LINK->NumberOfWindows - 1 <= LINK->length)
    return;
  printf("Windows cannot extend outside to the right.\n");
  printf("(StartCoordinate[=%ld] - 1)\n", LINK->StartCoordinate);
  printf(
    "+ (MaximumWindowSize[=%ld] + SpaceBetweenWindows[=%ld]) * (NumberOfWindows[=%ld])\n",
    LINK->MaximumWindowSize, LINK->SpaceBetweenWindows,
    LINK->NumberOfWindows);
  printf(" must not exceed the sequence length[=%ld].\n", LINK->length);
  die(LINK);
}

Local Void BuildPrecomputedTable(LINK)
struct LOC_themain *LINK;
{
  long n;
  _TEXT TEMP;
  long FORLIM;


  TEMP.f = stdout;
  *TEMP.name = '\0';
  getmonocomposition(LINK->cmp, &LINK->gna, &LINK->gnc, &LINK->gng,
		     &LINK->gnt, &TEMP, &LINK->equalmono);


  if (LINK->NumberOfWindows <= kickover)
    calehnb(LINK->NumberOfWindows, LINK->gna, LINK->gnc, LINK->gng, LINK->gnt,
	    &LINK->hg, &LINK->Ehnb, &LINK->varhnb);
  else
    calaehnb(LINK->NumberOfWindows, LINK->gna, LINK->gnc, LINK->gng,
	     LINK->gnt, &LINK->hg, &LINK->Ehnb, &LINK->varhnb);

  fprintf(LINK->xyin->f, "* Ehnb = %*.*f bits\n",
	  infofield, infodecim, LINK->Ehnb);
  fprintf(LINK->xyin->f, "* Hg   = %*.*f bits\n",
	  infofield, infodecim, LINK->hg);
  fprintf(LINK->xyin->f, "* 2-Hg = %*.*f bits per base\n",
	  infofield, infodecim, 2 - LINK->hg);

  LINK->ln2 = log(2.0);
  LINK->flogf[0] = 0.0;
  FORLIM = LINK->NumberOfWindows;
  for (n = 1; n < FORLIM; n++) {
    LINK->f = (double)n / LINK->NumberOfWindows;
    LINK->flogf[n] = -(LINK->f * log(LINK->f) / LINK->ln2);
  }
  LINK->flogf[LINK->NumberOfWindows] = 0.0;

  if (debugging) {
    FORLIM = LINK->NumberOfWindows;
    for (n = 0; n <= FORLIM; n++)
      fprintf(LINK->xyin->f, "* flogf[%ld]=%*.*f\n",
	      n, infofield, infodecim, LINK->flogf[n]);
  }

}

Local Void ReportDataColumnsOfXyin(LINK)
struct LOC_themain *LINK;
{
  fprintf(LINK->xyin->f, "*\n");
  fprintf(LINK->xyin->f, "* data columns:\n");
  fprintf(LINK->xyin->f, "* 1: Window Size\n");
  fprintf(LINK->xyin->f, "* 2: Rs, Rsequence, bits\n");
  fprintf(LINK->xyin->f, "* 3: SD, Standard Deviation of Rs, bits\n");
  fprintf(LINK->xyin->f, "* 4: Rs/(Window Size), bits/base\n");
  fprintf(LINK->xyin->f, "* 5: SD/(Window Size), bits/base\n");
}

Local Void DoTheCrunchAndMakeTheXyin(LINK)
struct LOC_themain *LINK;
{
  base b;
  long WindowSize, PositionInWindow, Window, FORLIM, FORLIM1, FORLIM2;

  FORLIM = LINK->MaximumWindowSize;
  for (WindowSize = LINK->MinimumWindowSize; WindowSize <= FORLIM; WindowSize++) {
    if (debugging)
      fprintf(LINK->xyin->f, "* WindowSize: %3ld\n", WindowSize);

    LINK->SD = sqrt(WindowSize * LINK->varhnb);
    LINK->Rs = LINK->Ehnb * WindowSize;

    FORLIM1 = LINK->StartCoordinate + WindowSize;
    for (PositionInWindow = LINK->StartCoordinate;
	 PositionInWindow < FORLIM1;
	 PositionInWindow++) {
      for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
	LINK->bases[(long)b - (long)a] = 0;
      LINK->PositionOnSequence = PositionInWindow;
      if (debugging)
	fprintf(LINK->xyin->f, "* position=%3ld ", PositionInWindow);

      FORLIM2 = LINK->NumberOfWindows;
      for (Window = 1; Window <= FORLIM2; Window++) {
	if (LINK->PositionOnSequence > LINK->length) {
	  printf("PROGRAM ERROR:\n");
	  printf("PositionOnSequence = %ld\n", LINK->PositionOnSequence);
	  printf("This is past the end of the sequence.\n");
	  halt();
	}

	b = (base)P_getbits_UB(LINK->apiece->dna->part,
			       LINK->PositionOnSequence - 1, 1, 3);

	if (debugging)
	  fputc(basetochar(b), LINK->xyin->f);

	LINK->bases[(long)b - (long)a]++;
	LINK->PositionOnSequence += WindowSize + LINK->SpaceBetweenWindows;

      }

      if (debugging) {
	for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
	  fprintf(LINK->xyin->f, " %c=%ld",
		  basetochar(b), LINK->bases[(long)b - (long)a]);
	putc('\n', LINK->xyin->f);
      }

      for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
	LINK->Rs -= LINK->flogf[LINK->bases[(long)b - (long)a]];
    }


    fprintf(LINK->xyin->f, "%*ld %*.*f %*.*f %*.*f %*.*f\n",
	    infodecim, WindowSize, infofield, infodecim, LINK->Rs, infofield,
	    infodecim, LINK->SD, infofield, infodecim, LINK->Rs / WindowSize,
	    infofield, infodecim, LINK->SD / WindowSize);
  }
}



Static Void themain(book_, cmp_, winrsp_, xyin_)
_TEXT *book_, *cmp_, *winrsp_, *xyin_;
{
  struct LOC_themain V;
  /*
*/
  long PositionInWindow;

  V.book = book_;
  V.cmp = cmp_;
  V.winrsp = winrsp_;
  V.xyin = xyin_;
  printf("winrs %4.2f\n", version);
  ReadParametersIn(&V);
  WriteParametersOut(&V);
  ReadInThePiece(&V);
  DoMoreChecksNowThatWeHaveTheLength(&V);
  BuildPrecomputedTable(&V);
  ReportDataColumnsOfXyin(&V);
  DoTheCrunchAndMakeTheXyin(&V);
}

#undef debugging
#undef kickover


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  winrsp.f = NULL;
  strcpy(winrsp.name, "winrsp");
  cmp.f = NULL;
  strcpy(cmp.name, "cmp");
  book.f = NULL;
  strcpy(book.name, "book");
  themain(&book, &cmp, &winrsp, &xyin);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (cmp.f != NULL)
    fclose(cmp.f);
  if (winrsp.f != NULL)
    fclose(winrsp.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
