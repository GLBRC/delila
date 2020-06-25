/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "instal.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*



*/



#define version         1.49
/*
*/



/*
































































*/



#define absshift        50
/*
*/
#define fillermax       3



#define maxstring       150





#define dnamax          3000
#define namelength      20
#define linelength      80




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



/*


*/
typedef Char filler[fillermax];



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;





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

/* p2c: instal.p, line 169:
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




Static _TEXT xbook, ybook, shlist, inst, rinst, sinst, list;







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






Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
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



/*




*/

Static Void resettrigger(t_)
trigger *t_;
{
  t_->state = 0;
  t_->skip = false;
  t_->found = false;
}


Static Void testfortrigger(ch, t_)
Char ch;
trigger *t_;
{
  /*




*/
  t_->state++;
  /*



*/
  if (t_->seek.letters[t_->state - 1] == ch) {
    t_->skip = false;
    if (t_->state == t_->seek.length)
      t_->found = true;
    else
      t_->found = false;
    return;
  }
  t_->state = 0;
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





Static Void readc(inst, rinst, sinst, c_, copying)
_TEXT *inst, *rinst, *sinst;
Char *c_;
boolean copying;
{
  /*
*/
  if (BUFEOF(inst->f)) {
    printf("unexpected end of inst file\n");
    halt();
  }
  if (P_eoln(inst->f)) {
    fscanf(inst->f, "%*[^\n]");


    getc(inst->f);
    putc('\n', rinst->f);
    putc('\n', sinst->f);
    return;
  }
  *c_ = getc(inst->f);
  if (*c_ == '\n')
    *c_ = ' ';
  if (copying) {
    putc(*c_, rinst->f);
    putc(*c_, sinst->f);
  }
}



Static Void token(inst, rinst, sinst)
_TEXT *inst, *rinst, *sinst;
{
  /*


*/
  Char c_;

  do {
    readc(inst, rinst, sinst, &c_, true);
  } while ((c_ == ' ') | P_eoln(inst->f));

  do {
    readc(inst, rinst, sinst, &c_, true);
  } while (!((c_ == ' ') | P_eoln(inst->f)));
}



Static Void getalignment(inst, rinst, sinst, alignment)
_TEXT *inst, *rinst, *sinst;
long *alignment;
{
  /*

*/
  token(inst, rinst, sinst);
  fscanf(inst->f, "%ld", alignment);
}



Static Void copytoget(inst, rinst, sinst, begcom, endcom, getinst)
_TEXT *inst, *rinst, *sinst;
trigger begcom, endcom, getinst;
{
  /*



*/
  Char c_;
  long level = 0;
  long l;
  /*

*/
  boolean isnoteoln;

  /*

*/
  resettrigger(&begcom);
  resettrigger(&getinst);
  while (!getinst.found) {
    isnoteoln = !P_eoln(inst->f);
    readc(inst, rinst, sinst, &c_, false);
    testfortrigger(c_, &getinst);


    if (getinst.state == 0) {
      if (level != 0) {
	for (l = 0; l < level; l++) {
	  putc(getinst.seek.letters[l], rinst->f);
	  putc(getinst.seek.letters[l], sinst->f);
	}
      }
      if (isnoteoln) {
	putc(c_, rinst->f);
	putc(c_, sinst->f);
      }
    }

    level = getinst.state;

    if (getinst.found)
      break;
    testfortrigger(c_, &begcom);
    if (!begcom.found)
      continue;

    resettrigger(&endcom);
    while (!endcom.found) {
      if (BUFEOF(inst->f)) {
	printf("no end to a comment in inst\n");
	halt();
      }
      readc(inst, rinst, sinst, &c_, true);
      testfortrigger(c_, &endcom);
    }

    resettrigger(&begcom);
  }


}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *xbook, *ybook, *shlist, *inst, *rinst, *sinst, *list;
  trigger begcom, endcom, getinst;
  piece *xpiece, *ypiece;
  long alignment, newalignment;


  long x, y, length;

  long sets;


  long shift;

  long distribution[absshift + absshift + 1];
  long minshift, maxshift;
  /*
*/
  long shiftedsets;
} ;

/*
*/
Local Void conversion(LINK)
struct LOC_themain *LINK;
{
  /*









*/
  long internal;

  /*
*/
  internal = pietoint(LINK->y, LINK->ypiece) + LINK->length +
	     pietoint(LINK->x, LINK->xpiece) - 2;
  LINK->shift = internal - pietoint(LINK->alignment, LINK->ypiece);
  LINK->newalignment = inttopie(internal, LINK->ypiece);


  if (labs(LINK->shift) > absshift) {
    printf(" shift outside range -maxshift to +maxshift:\n");
    printf(" %ld is outside %ld to %ld\n",
	   LINK->shift, -LINK->maxshift, LINK->maxshift);

    /*






*/
    return;
  }
  LINK->distribution[LINK->shift + absshift]++;
  if (LINK->shift < LINK->minshift)
    LINK->minshift = LINK->shift;
  if (LINK->shift > LINK->maxshift)
    LINK->maxshift = LINK->shift;
}

Local Void initialize(LINK)
struct LOC_themain *LINK;
{
  long shift, FORLIM;

  printf("instal %4.2f\n", version);
  if (*LINK->shlist->name != '\0') {
    if (LINK->shlist->f != NULL)
      LINK->shlist->f = freopen(LINK->shlist->name, "r", LINK->shlist->f);
    else
      LINK->shlist->f = fopen(LINK->shlist->name, "r");
  } else
    rewind(LINK->shlist->f);
  if (LINK->shlist->f == NULL)
    _EscIO2(FileNotFound, LINK->shlist->name);
  RESETBUF(LINK->shlist->f, Char);
  if (*LINK->inst->name != '\0') {
    if (LINK->inst->f != NULL)
      LINK->inst->f = freopen(LINK->inst->name, "r", LINK->inst->f);
    else
      LINK->inst->f = fopen(LINK->inst->name, "r");
  } else
    rewind(LINK->inst->f);
  if (LINK->inst->f == NULL)
    _EscIO2(FileNotFound, LINK->inst->name);
  RESETBUF(LINK->inst->f, Char);
  if (*LINK->rinst->name != '\0') {
    if (LINK->rinst->f != NULL)
      LINK->rinst->f = freopen(LINK->rinst->name, "w", LINK->rinst->f);
    else
      LINK->rinst->f = fopen(LINK->rinst->name, "w");
  } else {
    if (LINK->rinst->f != NULL)
      rewind(LINK->rinst->f);
    else
      LINK->rinst->f = tmpfile();
  }
  if (LINK->rinst->f == NULL)
    _EscIO2(FileNotFound, LINK->rinst->name);
  SETUPBUF(LINK->rinst->f, Char);
  if (*LINK->sinst->name != '\0') {
    if (LINK->sinst->f != NULL)
      LINK->sinst->f = freopen(LINK->sinst->name, "w", LINK->sinst->f);
    else
      LINK->sinst->f = fopen(LINK->sinst->name, "w");
  } else {
    if (LINK->sinst->f != NULL)
      rewind(LINK->sinst->f);
    else
      LINK->sinst->f = tmpfile();
  }
  if (LINK->sinst->f == NULL)
    _EscIO2(FileNotFound, LINK->sinst->name);
  SETUPBUF(LINK->sinst->f, Char);
  if (*LINK->list->name != '\0') {
    if (LINK->list->f != NULL)
      LINK->list->f = freopen(LINK->list->name, "w", LINK->list->f);
    else
      LINK->list->f = fopen(LINK->list->name, "w");
  } else {
    if (LINK->list->f != NULL)
      rewind(LINK->list->f);
    else
      LINK->list->f = tmpfile();
  }
  if (LINK->list->f == NULL)
    _EscIO2(FileNotFound, LINK->list->name);
  SETUPBUF(LINK->list->f, Char);

  fillstring(&LINK->begcom.seek, "(* ");
  fillstring(&LINK->endcom.seek, "*) ");
  fillstring(&LINK->getinst.seek, "get");

  /*
*/
  /*









*/


  brinit(LINK->xbook);
  LINK->xpiece = (piece *)Malloc(sizeof(piece));
  getpiece(LINK->xbook, &LINK->xpiece);


  brinit(LINK->ybook);
  LINK->ypiece = (piece *)Malloc(sizeof(piece));


  fprintf(LINK->list->f, " instal %4.2f instruction alignment\n\n", version);
  fprintf(LINK->list->f, " shlist: \n");
  if (copylines(LINK->shlist, LINK->list, 7L) != 7) {
    printf("incomplete header of shlist\n");
    halt();
  }
  fscanf(LINK->shlist->f, "%*[^\n]");


  getc(LINK->shlist->f);
  fprintf(LINK->list->f, "\n inst: \n");
  putc(' ', LINK->list->f);
  if (copylines(LINK->inst, LINK->list, 1L) != 1) {
    printf("empty inst file\n");
    halt();
  }
  if (*LINK->inst->name != '\0') {
    if (LINK->inst->f != NULL)
      LINK->inst->f = freopen(LINK->inst->name, "r", LINK->inst->f);
    else
      LINK->inst->f = fopen(LINK->inst->name, "r");
  } else
    rewind(LINK->inst->f);
  if (LINK->inst->f == NULL)
    _EscIO2(FileNotFound, LINK->inst->name);
  RESETBUF(LINK->inst->f, Char);
  putc('\n', LINK->list->f);

  LINK->sets = 0;
  FORLIM = LINK->maxshift;
  for (shift = -LINK->maxshift; shift <= FORLIM; shift++)
    LINK->distribution[shift + absshift] = 0;
  LINK->minshift = LONG_MAX;
  LINK->maxshift = -LONG_MAX;
  LINK->shiftedsets = 0;
}



Static Void themain(xbook_, ybook_, shlist_, inst_, rinst_, sinst_, list_)
_TEXT *xbook_, *ybook_, *shlist_, *inst_, *rinst_, *sinst_, *list_;
{
  struct LOC_themain V;
  long secondalignment;
  double energy;
  boolean done;
  /*


*/
  Char c_;
  double frequency, information;
  /*
*/
  double ln2 = log(2.0);
  /*
*/
  double totalinformation = 0.0;
  long FORLIM;

  V.xbook = xbook_;
  V.ybook = ybook_;
  V.shlist = shlist_;
  V.inst = inst_;
  V.rinst = rinst_;
  V.sinst = sinst_;
  V.list = list_;
  initialize(&V);
  while (!BUFEOF(V.shlist->f)) {
    gethelix(V.shlist, &V.x, &V.y, &V.length, false, &energy, &done);
    V.sets++;
    fprintf(V.list->f, " set %4ld ", V.sets);

    if (done) {
      if (!BUFEOF(V.shlist->f)) {
	fprintf(V.list->f, "skip");


	copytoget(V.inst, V.rinst, V.sinst, V.begcom, V.endcom, V.getinst);
	fprintf(V.sinst->f, "(* get");
	fprintf(V.rinst->f, "(* get");
	c_ = ' ';
	while (c_ != ';')
	  readc(V.inst, V.rinst, V.sinst, &c_, true);
	fprintf(V.sinst->f, " *)\n");
	fprintf(V.rinst->f, " *)\n");


	fprintf(V.sinst->f, "default numbering %ld;\n", V.sets + 1);
	fprintf(V.rinst->f, "default numbering %ld;\n", V.sets + 1);

	fscanf(V.shlist->f, "%*[^\n]");


	getc(V.shlist->f);
	getpiece(V.ybook, &V.ypiece);
	clearpiece(&V.ypiece);
      }
    } else {
      fprintf(V.list->f, "realign");
      V.shiftedsets++;


      copytoget(V.inst, V.rinst, V.sinst, V.begcom, V.endcom, V.getinst);
      fprintf(V.rinst->f, "get");
      fprintf(V.sinst->f, "get");
      getalignment(V.inst, V.rinst, V.sinst, &V.alignment);


      fprintf(V.rinst->f, "%ld", V.alignment);


      getpiece(V.ybook, &V.ypiece);


      conversion(&V);
      fprintf(V.sinst->f, "%ld", V.newalignment);


      token(V.inst, V.rinst, V.sinst);
      getalignment(V.inst, V.rinst, V.sinst, &secondalignment);
      if (V.alignment != secondalignment) {
	printf("instruction %ld from and to alignments are not identical\n",
	       V.sets);
	halt();
      }


      fprintf(V.rinst->f, "%ld", V.alignment);
      fprintf(V.sinst->f, "%ld", V.newalignment);
      fprintf(V.list->f, " %6ld to %6ld", V.alignment, V.newalignment);
      fprintf(V.list->f, " shift is %3ld", V.shift);


      clearpiece(&V.ypiece);


      gethelix(V.shlist, &V.x, &V.y, &V.length, false, &energy, &done);
      if (done) {
	if (!BUFEOF(V.shlist->f)) {
	  fscanf(V.shlist->f, "%*[^\n]");
	  getc(V.shlist->f);
	}
      } else {
	printf(" there are at least two helixes in set %ld, use the program sorth\n",
	       V.sets);
	printf(" to take only the top helix of each set\n");
	halt();

      }
    }
    putc('\n', V.list->f);
  }


  while (!BUFEOF(V.inst->f))
    readc(V.inst, V.rinst, V.sinst, &c_, true);


  clearpiece(&V.xpiece);
  getpiece(V.xbook, &V.xpiece);
  if (!BUFEOF(V.xbook->f))
    printf("warning: only the first piece of xbook was used\n");


  putc('\n', V.list->f);
  printf(" %ld instructions shifted out of %ld instructions (sets)\n",
	 V.shiftedsets, V.sets);
  fprintf(V.list->f,
	  " %ld instructions shifted out of %ld instructions (sets)\n\n",
	  V.shiftedsets, V.sets);
  fprintf(V.list->f, " the distribution of shifts is: \n");
  fprintf(V.list->f,
	  " shift  number  frequency information total information\n");
  FORLIM = V.maxshift;
  for (V.shift = V.minshift; V.shift <= FORLIM; V.shift++) {
    fprintf(V.list->f, " %5ld  %6ld",
	    V.shift, V.distribution[V.shift + absshift]);
    frequency = (double)V.distribution[V.shift + absshift] / V.shiftedsets;

    if (frequency <= 0.0 || frequency >= 1.0)
      information = 0.0;
    else
      information = -(frequency * log(frequency) / ln2);

    totalinformation += information;
    fprintf(V.list->f, " %10.5f  %10.5f %10.5f\n",
	    frequency, information, totalinformation);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  list.f = NULL;
  strcpy(list.name, "list");
  sinst.f = NULL;
  strcpy(sinst.name, "sinst");
  rinst.f = NULL;
  strcpy(rinst.name, "rinst");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  shlist.f = NULL;
  strcpy(shlist.name, "shlist");
  ybook.f = NULL;
  strcpy(ybook.name, "ybook");
  xbook.f = NULL;
  strcpy(xbook.name, "xbook");
  themain(&xbook, &ybook, &shlist, &inst, &rinst, &sinst, &list);
_L1:
  if (xbook.f != NULL)
    fclose(xbook.f);
  if (ybook.f != NULL)
    fclose(ybook.f);
  if (shlist.f != NULL)
    fclose(shlist.f);
  if (inst.f != NULL)
    fclose(inst.f);
  if (rinst.f != NULL)
    fclose(rinst.f);
  if (sinst.f != NULL)
    fclose(sinst.f);
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
