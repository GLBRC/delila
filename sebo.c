/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "sebo.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.47
/*
























*/
#define updateversion   1.21



/*





























































































































































































































































*/


/*








































































*/



/*

*/

#define dnamax          5000000L
/*
*/

#define namelength      100
#define linelength      80



#define maxstring       2000



#define fillermax       50




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
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



typedef long dnarange;

/* p2c: sebo.p, line 468:
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



Static _TEXT inst, book, target, sebop, list, instout;






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










Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
  clearstring(ribbon);
  ribbon->next = NULL;
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



Static Void filltrigger(t_, a_)
trigger *t_;
Char *a_;
{
  fillstring(&t_->seek, a_);
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
  /*
*/
  if (t_->seek.letters[0] == ch) {
    t_->state = 1;
    t_->skip = false;
    t_->found = false;
    return;
  }
  t_->state = 0;
  t_->skip = true;
  t_->found = false;
}






Static boolean isblankDelila(c_)
Char c_;
{
  return (c_ == ' ' || c_ == '\t');
}


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
    printf(", length = %ld\n", l->length);
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






Static Void findblank(afile)
_TEXT *afile;
{
  Char ch;

  do {
    ch = getc(afile->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != ' ');
}



Static Void findnonblank(afile, ch)
_TEXT *afile;
Char *ch;
{
  *ch = ' ';
  while (!BUFEOF(afile->f) && *ch == ' ') {
    *ch = getc(afile->f);
    if (*ch == '\n')
      *ch = ' ';
    if (P_eoln(afile->f)) {
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
    }
  }
}


#define maximumrange    10000
/*


*/

#define semicolon       ';'


/* Local variables for align: */
struct LOC_align {
  _TEXT *inst;
  Char ch;
  trigger endcomment, endcurly;
} ;

/*
















*/

Local Void skipcomment(f, LINK)
_TEXT *f;
struct LOC_align *LINK;
{
  boolean comment = true;

  resettrigger(&LINK->endcomment);
  while (comment) {
    if (BUFEOF(f->f)) {
      printf("A comment does not end!\n");
      halt();
    }
    if (P_eoln(f->f)) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
      continue;
    }

    LINK->ch = getc(f->f);
    if (LINK->ch == '\n')
      LINK->ch = ' ';
    testfortrigger(LINK->ch, &LINK->endcomment);
    if (LINK->endcomment.found) {
      comment = false;
      /*

*/
    }
  }
}

Local Void skipcurly(f, LINK)
_TEXT *f;
struct LOC_align *LINK;
{
  boolean comment = true;

  resettrigger(&LINK->endcurly);
  while (comment) {
    if (BUFEOF(f->f)) {
      printf("A comment does not end!\n");
      halt();
    }
    if (P_eoln(f->f)) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
      continue;
    }

    LINK->ch = getc(f->f);
    if (LINK->ch == '\n')
      LINK->ch = ' ';
    testfortrigger(LINK->ch, &LINK->endcurly);
    if (LINK->endcurly.found) {
      comment = false;
      /*

*/
    }
  }
}

Local Void skipquote(quote, LINK)
trigger quote;
struct LOC_align *LINK;
{
  Char kind;

  kind = quote.seek.letters[0];

  do {
    findnonblank(LINK->inst, &LINK->ch);
  } while (!((LINK->ch == kind) | BUFEOF(LINK->inst->f)));
  if (LINK->ch != kind) {
    printf("end of quote starting with %c not found\n", kind);
    halt();
  }
}



Static Void align(inst_, book, theline, pie, length, alignedbase)
_TEXT *inst_, *book;
long *theline;
piece **pie;
long *length, *alignedbase;
{
  /*




*/
  struct LOC_align V;
  long p, p1;
  boolean done = false;
  long thebase;
  boolean indefault = false;
  /*
*/

  trigger gettrigger, defaulttrigger, nametrigger, piecetrigger, settrigger;

  trigger begincomment, begincurly;

  trigger quote1trigger, quote2trigger;
  name *WITH;
  long FORLIM;

  V.inst = inst_;
  filltrigger(&defaulttrigger,
	      "default                                           ");
  filltrigger(&gettrigger,
	      "get                                               ");
  filltrigger(&nametrigger,
	      "name                                              ");
  filltrigger(&piecetrigger,
	      "piece                                             ");
  filltrigger(&settrigger,
	      "set                                               ");
  filltrigger(&begincomment,
	      "(*                                                ");
  filltrigger(&V.endcomment,
	      "*)                                                ");
  filltrigger(&begincurly,
	      "{                                                 ");
  filltrigger(&V.endcurly,
	      "}                                                 ");
  filltrigger(&quote1trigger,
	      "'                                                 ");
  filltrigger(&quote2trigger,
	      "\"                                                 ");

  resettrigger(&defaulttrigger);
  resettrigger(&gettrigger);
  resettrigger(&nametrigger);
  resettrigger(&piecetrigger);
  resettrigger(&settrigger);

  resettrigger(&begincomment);
  resettrigger(&begincurly);
  resettrigger(&quote1trigger);
  resettrigger(&quote2trigger);


  if (BUFEOF(book->f))
    return;
  getpiece(book, theline, pie);
  if (BUFEOF(book->f))
    return;
  *length = pietoint((*pie)->key.pieend, *pie);


  while (!done) {
    if (BUFEOF(V.inst->f)) {
      *alignedbase = 1;
      done = true;
      break;
    }
    if (P_eoln(V.inst->f)) {
      fscanf(V.inst->f, "%*[^\n]");
      getc(V.inst->f);
      continue;
    }

    V.ch = getc(V.inst->f);

    if (V.ch == '\n')
      V.ch = ' ';
    testfortrigger(V.ch, &begincomment);
    testfortrigger(V.ch, &begincurly);
    if (begincomment.found || begincurly.found) {
      if (V.ch == '*') {
	skipcomment(V.inst, &V);
	resettrigger(&begincomment);
      } else {
	resettrigger(&begincurly);
	skipcurly(V.inst, &V);
      }
      continue;
    }
    testfortrigger(V.ch, &gettrigger);
    if (gettrigger.found) {
      findnonblank(V.inst, &V.ch);
      findblank(V.inst);
      fscanf(V.inst->f, "%ld", &thebase);

      *alignedbase = pietoint(thebase, *pie);

      done = true;
    }

    testfortrigger(V.ch, &quote1trigger);
    if (quote1trigger.found)
      skipquote(quote1trigger, &V);

    testfortrigger(V.ch, &quote2trigger);
    if (quote2trigger.found)
      skipquote(quote2trigger, &V);

    testfortrigger(V.ch, &defaulttrigger);
    if (defaulttrigger.found) {
      indefault = true;
      resettrigger(&defaulttrigger);
    }
    if (V.ch == semicolon)
      indefault = false;

    testfortrigger(V.ch, &settrigger);
    if (settrigger.found) {
      indefault = true;
      resettrigger(&settrigger);
    }
    if (V.ch == semicolon)
      indefault = false;


    testfortrigger(V.ch, &piecetrigger);
    if (indefault)
      continue;

    if (!piecetrigger.found)
      continue;
    skipblanks(V.inst);
    WITH = &(*pie)->key.hea.keynam;
    FORLIM = WITH->length;
    for (p = 1; p <= FORLIM; p++) {
      V.ch = getc(V.inst->f);
      if (V.ch == '\n')
	V.ch = ' ';
      if (WITH->letters[p-1] != V.ch) {
	printf("The piece name in the book: \n");
/* p2c: sebo.p, line 1656: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
	printf("%.*s\n", WITH->length, WITH->letters);
	printf("does not match the inst file piece name:\n");

	for (p1 = 0; p1 <= p - 2; p1++)
	  putchar(WITH->letters[p1]);

	putchar(V.ch);

	done = P_eoln(V.inst->f);
	while (!done) {
	  done = P_eoln(V.inst->f);
	  if (done)
	    break;
	  V.ch = getc(V.inst->f);
	  if (V.ch == '\n')
	    V.ch = ' ';
	  if (V.ch == ' ' || V.ch == ';')
	    done = true;
	  if (!done)
	    putchar(V.ch);
	}
	putchar('\n');
	/*
*/
	for (p1 = 1; p1 < p; p1++)
	  putchar(' ');
	printf("^\n");
	halt();

      }
    }
  }


  if (*alignedbase > -maximumrange && *alignedbase <= *length + maximumrange)
    return;
  printf(" In procedure align:\n");
  printf(" read in base was %ld\n", thebase);
  printf(" in internal coordinates: %ld\n", *alignedbase);
  printf(" maximum range was %ld\n", (long)maximumrange);
  printf(" piece length was %ld\n", *length);
  WITH = &(*pie)->key.hea.keynam;
/* p2c: sebo.p, line 1700: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  printf(" piece name: %.*s\n", WITH->length, WITH->letters);
  printf(" piece number: %ld\n", number);
  printf(" aligned base is too far away... see the code\n");
  halt();
}

#undef maximumrange
#undef semicolon








Static boolean withinalignment(alignedposition, alignedbase, length)
long alignedposition, alignedbase, length;
{
  /*
*/
  long p;

  p = alignedposition + alignedbase;
  return (p > 0 && p <= length);
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
      printf("error in getbase, spot (=%ld) must be less than length (=%ld)\n",
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



Static Void clearname(n)
name *n;
{
  long i;

  n->length = 0;
  for (i = 0; i < namelength; i++)
    n->letters[i] = ' ';
}


Static Void writename(f, n)
_TEXT *f;
name n;
{
  long i;

  for (i = 0; i < n.length; i++)
    putc(n.letters[i], f->f);
}


Static Void copyname(a_, b)
name a_, *b;
{
  long i;

  for (i = 0; i < a_.length; i++)
    b->letters[i] = a_.letters[i];
  b->length = a_.length;
}


Static boolean equalname(a_, b)
name a_, b;
{
  long i = 1;
  boolean same = true;

  if (b.length != a_.length)
    return false;
  while (same && i <= a_.length) {
    same = (b.letters[i-1] == a_.letters[i-1]);
    i++;
/* p2c: sebo.p: Note: Eliminated unused assignment statement [338] */
  }
  return same;
}



Static Void brorgkey(thefile, theline, org)
_TEXT *thefile;
long *theline;
orgkey *org;
{
  brheader(thefile, theline, &org->hea);
  getlineDelila(&org->mapunit);
  brline(thefile, theline, &org->mapunit);
}



Static Void brchrkey(thefile, theline, chr)
_TEXT *thefile;
long *theline;
chrkey *chr;
{
  brheader(thefile, theline, &chr->hea);
  brreanum(thefile, theline, &chr->mapbeg);
  brreanum(thefile, theline, &chr->mapend);
}



Static Void getocp(thefile, theline, org, orgchange, orgopen, chr, chrchange,
		   chropen, pie, piechange, pieopen)
_TEXT *thefile;
long *theline;
orgkey *org;
boolean *orgchange, *orgopen;
chrkey *chr;
boolean *chrchange, *chropen;
piece **pie;
boolean *piechange, *pieopen;
{
  /*




























*/
  Char ch = 'a';
  chrkey newchr;
  orgkey neworg;
  piece *newpie;
  long SET[5];

  while (ch != 'p' && ch != ' ') {
    P_addset(P_expset(SET, 0L), 'o');
    P_addset(SET, 'c');
    ch = getto(thefile, theline, P_addset(SET, 'p'));
    if (ch == ' ') {
      *pieopen = false;
      break;
    }
    switch (ch) {

    case 'o':
      if (*orgopen) {
	fscanf(thefile->f, "%*[^\n]");
	getc(thefile->f);
	/*
*/
	*orgopen = false;
      } else {
	brorgkey(thefile, theline, &neworg);
	if (strncmp(neworg.hea.keynam.letters, org->hea.keynam.letters,
		    sizeof(alpha)) &&
	    neworg.hea.keynam.length != org->hea.keynam.length) {
	  *orgchange = true;
	  copyheader(neworg.hea, &org->hea);

	  org->mapunit = neworg.mapunit;
	  clearline(&neworg.mapunit);

	}
	/*





*/

	else
	  *orgchange = false;
	*orgopen = true;
      }
      break;

    case 'c':
      if (*chropen) {
	fscanf(thefile->f, "%*[^\n]");
	getc(thefile->f);
	/*
*/
	*chropen = false;
      } else {
	brchrkey(thefile, theline, &newchr);
	if (strncmp(newchr.hea.keynam.letters, chr->hea.keynam.letters,
		    sizeof(alpha)) &&
	    newchr.hea.keynam.length != chr->hea.keynam.length) {
	  /*





*/
	  *chrchange = true;
	  copyheader(newchr.hea, &chr->hea);

	  chr->mapbeg = newchr.mapbeg;
	  chr->mapend = newchr.mapend;
	} else
	  *chrchange = false;
	*chropen = true;
      }
      break;

    case 'p':
      if (*pieopen) {
	*pieopen = false;
	ch = 'a';
      } else {
	newpie = (piece *)Malloc(sizeof(piece));
	brpiece(thefile, theline, &newpie);
	if (*pie == NULL)
	  *piechange = true;
	else {
	  if (strncmp(newpie->key.hea.keynam.letters,
		      (*pie)->key.hea.keynam.letters, sizeof(alpha)) &&
	      newpie->key.hea.keynam.length != (*pie)->key.hea.keynam.length)
	    *piechange = true;
	  else
	    *piechange = false;
	}
	*pieopen = true;
	/*




*/
	if (*pie != NULL) {
	  clearpiece(pie);
	  Free(*pie);
	}
	*pie = newpie;
      }
      break;
    }
  }
}








Static Void iwname(thefile, thename)
_TEXT *thefile;
name thename;
{
  long c_;

  for (c_ = 0; c_ < thename.length; c_++)
    putc(thename.letters[c_], thefile->f);
}



Static Void iworg(afile, org)
_TEXT *afile;
orgkey org;
{
  /*
*/
  fprintf(afile->f, "organism ");
  iwname(afile, org.hea.keynam);
  putc(';', afile->f);
}



Static Void iwchr(afile, chr)
_TEXT *afile;
chrkey chr;
{
  /*
*/
  fprintf(afile->f, "chromosome ");
  iwname(afile, chr.hea.keynam);
  putc(';', afile->f);
}



Static Void iwpie(afile, pie)
_TEXT *afile;
piekey pie;
{
  fprintf(afile->f, "piece ");
  iwname(afile, pie.hea.keynam);
  fprintf(afile->f, ";\n");
}



Static Void iworgchr(afile, org, orgchange, orgopen, chr, chrchange, chropen)
_TEXT *afile;
orgkey org;
boolean orgchange, orgopen;
chrkey chr;
boolean chrchange, chropen;
{
  /*


*/
  if (orgchange && orgopen)
    iworg(afile, org);
  if (orgchange && chrchange && orgopen && chropen)
    putc(' ', afile->f);
  if (chrchange && chropen)
    iwchr(afile, chr);
  if (orgchange && orgopen || chrchange && chropen)
    putc('\n', afile->f);
}


/* Local variables for iwget: */
struct LOC_iwget {
  _TEXT *afile;
  piece *pie;
  long pieceplace, insttype;
} ;

/* Local variables for iwposition: */
struct LOC_iwposition {
  struct LOC_iwget *LINK;
} ;

Local Void iwrelative(relative, LINK)
long relative;
struct LOC_iwposition *LINK;
{
  if (relative >= 0)
    fprintf(LINK->LINK->afile->f, " +%ld", relative);
  else if (relative < 0)
    fprintf(LINK->LINK->afile->f, " %ld", relative);
}

Local Void iwposition(relative, sameallowed, LINK)
long relative;
boolean sameallowed;
struct LOC_iwget *LINK;
{
  struct LOC_iwposition V;

  V.LINK = LINK;
  if (LINK->insttype == 1 && sameallowed)
    fprintf(LINK->afile->f, " same");
  else
    fprintf(LINK->afile->f, " %ld", LINK->pieceplace);
  switch (LINK->pie->key.piedir) {

  case plus:
    iwrelative(relative, &V);
    break;

  case minus:
    iwrelative(-relative, &V);
    break;
  }
}



Static Void iwget(afile_, pie_, fromplace, pieceplace_, toplace, flip,
		  insttype_, carriagereturn)
_TEXT *afile_;
piece *pie_;
long fromplace, pieceplace_, toplace;
boolean flip;
long insttype_;
boolean carriagereturn;
{
  /*








*/
  struct LOC_iwget V;

  V.afile = afile_;
  V.pie = pie_;
  V.pieceplace = pieceplace_;
  V.insttype = insttype_;
  fprintf(V.afile->f, "get from");
  iwposition(fromplace, false, &V);
  fprintf(V.afile->f, " to");
  iwposition(toplace, true, &V);
  fprintf(V.afile->f, " direction");
  switch (V.pie->key.piedir) {

  case dirhomologous:
  case plus:
    switch (flip) {

    case false:
      fprintf(V.afile->f, " +");
      break;

    case true:
      fprintf(V.afile->f, " -");
      break;
    }
    break;

  case dircomplement:
  case minus:
    switch (flip) {

    case false:
      fprintf(V.afile->f, " -");
      break;

    case true:
      fprintf(V.afile->f, " +");
      break;
    }
    break;
  }
  putc(';', V.afile->f);
  if (carriagereturn)
    putc('\n', V.afile->f);
}



Static Void iwgetsimple(afile, pie, fromplace, toplace, flip, carriagereturn)
_TEXT *afile;
piece *pie;
long fromplace, toplace;
boolean flip, carriagereturn;
{
  /*








*/
  fprintf(afile->f, "get from");
  fprintf(afile->f, " %ld", fromplace);
  fprintf(afile->f, " to");
  fprintf(afile->f, " %ld", toplace);
  fprintf(afile->f, " direction");
  switch (pie->key.piedir) {

  case dirhomologous:
  case plus:
    switch (flip) {

    case false:
      fprintf(afile->f, " +");
      break;

    case true:
      fprintf(afile->f, " -");
      break;
    }
    break;

  case dircomplement:
  case minus:
    switch (flip) {

    case false:
      fprintf(afile->f, " -");
      break;

    case true:
      fprintf(afile->f, " +");
      break;
    }
    break;
  }
  putc(';', afile->f);
  if (carriagereturn)
    putc('\n', afile->f);
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



Static long firstlastmax(firstnumber, lastnumber)
long firstnumber, lastnumber;
{
  /*
*/
  long firstlines, lastlines;

  firstlines = numbersize(firstnumber);
  if (firstnumber > 0)
    firstlines++;
  lastlines = numbersize(lastnumber);
  if (lastnumber > 0)
    lastlines++;
  if (firstlines > lastlines)
    return firstlines;
  else
    return lastlines;
}


Static Void numberbar(afile, spaces, firstnumber, lastnumber, linesused)
_TEXT *afile;
long spaces, firstnumber, lastnumber, *linesused;
{
  /*
*/
  long logplace, number, spacecount;

  /*






*/
  *linesused = firstlastmax(firstnumber, lastnumber);
  /*






*/
  for (logplace = *linesused - 1; logplace >= 0; logplace--) {
    for (spacecount = 1; spacecount <= spaces; spacecount++)
      putc(' ', afile->f);
    for (number = firstnumber; number <= lastnumber; number++)
      fputc(numberdigit(number, logplace), afile->f);
    putc('\n', afile->f);
  }
}





Static Void space(afile, spaces)
_TEXT *afile;
long spaces;
{
  long spacecount;

  for (spacecount = 1; spacecount <= spaces; spacecount++)
    putc(' ', afile->f);
}



Static Void readparameters(sebop, parameterversion, alignmenttype, fromdo,
			   todo, rangecontrol, frominst, toinst, mismatches,
			   alertinterval)
_TEXT *sebop;
double *parameterversion;
Char *alignmenttype;
long *fromdo, *todo;
Char *rangecontrol;
long *frominst, *toinst, *mismatches, *alertinterval;
{
  if (*sebop->name != '\0') {
    if (sebop->f != NULL)
      sebop->f = freopen(sebop->name, "r", sebop->f);
    else
      sebop->f = fopen(sebop->name, "r");
  } else
    rewind(sebop->f);
  if (sebop->f == NULL)
    _EscIO2(FileNotFound, sebop->name);
  RESETBUF(sebop->f, Char);
  fscanf(sebop->f, "%lg%*[^\n]", parameterversion);
  getc(sebop->f);
  /*


*/
  if ((long)floor(100 * *parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(sebop->f, "%c%*[^\n]", alignmenttype);
  getc(sebop->f);

  if (*alignmenttype == '\n')
    *alignmenttype = ' ';
  fscanf(sebop->f, "%ld%ld%*[^\n]", fromdo, todo);
  getc(sebop->f);
  if (*fromdo > *todo) {
    printf("fromdo (%ld) must be less than todo (%ld)\n", *fromdo, *todo);
    halt();
  }

  if (P_peek(sebop->f) == 'r' || P_peek(sebop->f) == 'a') {
    fscanf(sebop->f, "%c%*[^\n]", rangecontrol);
    getc(sebop->f);
    if (*rangecontrol == '\n')
      *rangecontrol = ' ';
    *frominst = 0;
    *toinst = 0;
  } else {
    fscanf(sebop->f, "%ld%ld%*[^\n]", frominst, toinst);
    getc(sebop->f);
    if (*frominst > *toinst) {
      printf("frominst (%ld) must be less than toinst (%ld)\n",
	     *frominst, *toinst);
      halt();
    }
  }

  fscanf(sebop->f, "%ld%*[^\n]", mismatches);
  getc(sebop->f);
  if (*mismatches < 0) {
    printf("mismatches must be non-negative\n");
    halt();
  }

  fscanf(sebop->f, "%ld%*[^\n]", alertinterval);
  getc(sebop->f);

}


#define wid             10




Static Void writeparameters(afile, alignmenttype, fromdo, todo, rangecontrol,
			    frominst, toinst, mismatches, alertinterval)
_TEXT *afile;
Char alignmenttype;
long fromdo, todo;
Char rangecontrol;
long frominst, toinst, mismatches, alertinterval;
{
  switch (alignmenttype) {

  case 'f':
    fprintf(afile->f, "f: force alignment on first base\n");
    break;

  case 'i':
    fprintf(afile->f, "i: use the alignedbase from the book\n");
    break;

  case 'b':
    fprintf(afile->f, "b: use the internal book\n");
    break;
  }
  fprintf(afile->f, "%*ld %*ld range of book alignment used for search\n",
	  wid, fromdo, wid, todo);
  if (rangecontrol != 'r' && rangecontrol != 'a')
    fprintf(afile->f, "%*ld %*ld range of resulting output instructions\n",
	    wid, frominst, wid, toinst);
  else {
    fprintf(afile->f, "%c%*c%*c", rangecontrol, wid, ' ', wid, ' ');
    if (rangecontrol == 'r')
      fprintf(afile->f, " RELATIVE");
    if (rangecontrol == 'a')
      fprintf(afile->f, " ABSOLUTE");
    fprintf(afile->f, " instructions\n");
  }
  fprintf(afile->f, "%*ld %*c mismatches allowed\n",
	  wid, mismatches, wid, ' ');
  fprintf(afile->f, "%*ld %*c alert interval\n", wid, alertinterval, wid, ' ');
}

#undef wid



Static Void identifypiece(afile, Ppie, carriagereturn)
_TEXT *afile;
piece **Ppie;
boolean carriagereturn;
{
  /*




*/
  boolean Pflip;
  header *WITH;

  WITH = &(*Ppie)->key.hea;
  if (WITH->fulnam != NULL) {
    if (WITH->fulnam->length > 0) {
      fprintf(afile->f, "name \"");
      writeline(afile, WITH->fulnam, false);
      fprintf(afile->f, "\"; ");
    }
  }
  Pflip = ((*Ppie)->key.piedir != (*Ppie)->key.coodir);
  fprintf(afile->f, "piece ");
  iwname(afile, (*Ppie)->key.hea.keynam);

  fprintf(afile->f, "; ");
  if (Pflip)
    iwgetsimple(afile, *Ppie, inttopie(1L, *Ppie),
		inttopie(piecelength(*Ppie), *Ppie), !Pflip, false);
  else
    iwgetsimple(afile, *Ppie, inttopie(1L, *Ppie),
		inttopie(piecelength(*Ppie), *Ppie), Pflip, false);
  if (carriagereturn)
    putc('\n', afile->f);
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *instout, *list;

  long alertinterval;
  /*
*/
  long alignedbase;
  Char alignmenttype;
  /*
*/
  boolean done, found;
  long fromdo, todo, frominst, toinst, hits, indent, insttype, l, linesused,
       mismatches, misses, probenumber;
  Char rangecontrol;
  /*
*/
  long targetnumber;
  line *thefulnam;
  /*
*/
  long totalprobes;
  piece *Bpie;
  long Blength;
  base Bbase;
  long Bposition;
  piece *Tpie;
  long Tlength;
  base Tbase;
  long Tposition;
} ;

Local Void showalignment(fout, t_, LINK)
_TEXT *fout;
long t_;
struct LOC_themain *LINK;
{
  long index, FORLIM;


  putc('\n', fout->f);
  numberbar(fout, LINK->indent, LINK->fromdo, LINK->todo, &LINK->linesused);
  space(fout, LINK->indent);
  FORLIM = LINK->todo;
  for (index = LINK->fromdo; index <= FORLIM; index++) {
    if (withinalignment(index, LINK->alignedbase, LINK->Blength)) {
      LINK->Bbase = getbase(index + LINK->alignedbase, LINK->Bpie);
      fputc(basetochar(LINK->Bbase), fout->f);
    } else
      putc(' ', fout->f);
  }
  fprintf(fout->f, " probe\n");


  if (LINK->misses > 0) {
    space(fout, LINK->indent);
    FORLIM = LINK->todo;
    for (index = LINK->fromdo; index <= FORLIM; index++) {
      if (withinalignment(index, LINK->alignedbase, LINK->Blength) &
	  withinalignment(index, t_, LINK->Tlength)) {
	if (getbase(index + LINK->alignedbase, LINK->Bpie) ==
	    getbase(index + t_, LINK->Tpie))
	  putc(' ', fout->f);
	else
	  putc('X', fout->f);
      } else
	putc(' ', fout->f);
    }
    fprintf(fout->f, " differences\n");
  }


  if (LINK->misses > 0) {
    space(fout, LINK->indent);
    FORLIM = LINK->todo;
    for (index = LINK->fromdo; index <= FORLIM; index++) {
      if (withinalignment(index, t_, LINK->Tlength)) {
	LINK->Tbase = getbase(index + t_, LINK->Tpie);
	fputc(basetochar(LINK->Tbase), fout->f);
      } else
	putc(' ', fout->f);
    }
    fprintf(fout->f, " target\n");
  }


  space(fout, LINK->indent);
  space(fout, labs(LINK->fromdo));
  fprintf(fout->f, "^ found at %ld", inttopie(t_, LINK->Tpie));
  fprintf(fout->f, " on the target\n\n");


}

Local Void copyline(a_, b, LINK)
line *a_, **b;
struct LOC_themain *LINK;
{
  long index, FORLIM;

  FORLIM = a_->length;
  for (index = 0; index < FORLIM; index++)
    (*b)->letters[index] = a_->letters[index];
  (*b)->length = a_->length;
}

Local Void writeinstructions(instout, t_, LINK)
_TEXT *instout;
long t_;
struct LOC_themain *LINK;
{
  fprintf(instout->f, "\n{Searching target %ld with probe %ld: \n",
	  LINK->targetnumber, LINK->probenumber);

  /*


*/
  /*


















*/


  identifypiece(instout, &LINK->Bpie, false);
  fprintf(instout->f, " }\n");

  fprintf(instout->f, "{ final target instructions: }\n");

  if (LINK->Bpie->key.hea.fulnam->length != 0) {
    fprintf(instout->f, "name \"");
    writeline(instout, LINK->Bpie->key.hea.fulnam, false);

    fprintf(instout->f, "\"; ");
  } else {
    fprintf(instout->f, "name \"");
    iwname(instout, LINK->Bpie->key.hea.keynam);
    fprintf(instout->f, "\"; ");
  }

  /*




*/

  if (LINK->rangecontrol != 'r' && LINK->rangecontrol != 'a') {
    iwget(instout, LINK->Tpie, LINK->frominst, inttopie(t_, LINK->Tpie),
	  LINK->toinst, false, LINK->insttype, true);
    return;
  }
  switch (LINK->rangecontrol) {

  case 'a':
    iwget(instout, LINK->Tpie, LINK->frominst, inttopie(t_, LINK->Tpie),
	  LINK->frominst + piecelength(LINK->Bpie) - 1, false, 2L, true);
    break;

  case 'r':
    iwget(instout, LINK->Tpie, 0L, inttopie(t_, LINK->Tpie),
	  piecelength(LINK->Bpie) - 1, false, LINK->insttype, true);
    /*













*/

    break;
  }
}

Local Void announceTarget(fout, LINK)
_TEXT *fout;
struct LOC_themain *LINK;
{
  fprintf(fout->f, "\nTarget sequence: \n");
  identifypiece(fout, &LINK->Tpie, false);
  fprintf(fout->f, " { %ld bases }\n", LINK->Tlength);
}

Local Void announceProbe(fout, LINK)
_TEXT *fout;
struct LOC_themain *LINK;
{
  space(fout, LINK->indent);
  fprintf(fout->f, "Searching target %ld with probe %ld: \n",
	  LINK->targetnumber, LINK->probenumber);

  identifypiece(fout, &LINK->Bpie, true);
}

Local Void usingpreviousname(fout, LINK)
_TEXT *fout;
struct LOC_themain *LINK;
{
  fprintf(fout->f, "{  Using a previous probe full name: \"");
  writeline(fout, LINK->thefulnam, false);
  fprintf(fout->f, "\" as the full name for the next probe. }\n");
}

Local Void reporthits(fout, LINK)
_TEXT *fout;
struct LOC_themain *LINK;
{
  if (LINK->hits == 0) {
    fprintf(fout->f, "{ probe");

    if (LINK->thefulnam->length != 0) {
      fprintf(fout->f, " \"");
      writeline(fout, LINK->thefulnam, false);
      putc('"', fout->f);
    }

    fprintf(fout->f, " was not found in the target. }\n");
    return;
  }
  if (LINK->hits <= 1)
    return;
  fprintf(fout->f, "{  probe \"");
  writeline(fout, LINK->thefulnam, false);
  fprintf(fout->f, "\" was found %ld times in the target. }\n", LINK->hits);
}

Local Void dosearch(LINK)
struct LOC_themain *LINK;
{
  long t_, FORLIM;
  _TEXT TEMP;

  switch (LINK->alignmenttype) {

  case 'f':
    LINK->alignedbase = 1;
    break;

  case 'i':
    /* blank case */
    break;

  case 'b':
    LINK->alignedbase = pietoint(0L, LINK->Bpie);
    break;
  }

  LINK->hits = 0;
  FORLIM = LINK->Tlength;
  /*



*/
  for (t_ = 1; t_ <= FORLIM; t_++) {
    if (LINK->alertinterval > 0) {
      if (t_ % LINK->alertinterval == 0)
	printf("... %ld\n", inttopie(t_, LINK->Tpie));
    }

    LINK->done = false;
    LINK->found = true;
    LINK->l = LINK->fromdo;
    LINK->misses = 0;
    while (LINK->l <= LINK->todo && LINK->found) {
      LINK->Bposition = LINK->l + LINK->alignedbase;


      if (LINK->Bposition >= 1 && LINK->Bposition <= LINK->Blength) {
	LINK->Tposition = t_ + LINK->l;


	if (LINK->Tposition < 1)
	  LINK->Tposition += LINK->Tlength;
	if (LINK->Tposition > LINK->Tlength)
	  LINK->Tposition -= LINK->Tlength;

	LINK->Tbase = getbase(LINK->Tposition, LINK->Tpie);
	LINK->Bbase = getbase(LINK->Bposition, LINK->Bpie);

	if (LINK->Bbase != LINK->Tbase) {
	  LINK->misses++;
	  if (LINK->misses > LINK->mismatches)
	    LINK->found = false;
	}
      }



      LINK->l++;
    }



    if (LINK->found) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      showalignment(&TEMP, t_, LINK);
      showalignment(LINK->list, t_, LINK);
      writeinstructions(LINK->instout, t_, LINK);
      LINK->hits++;
      LINK->totalprobes += LINK->hits;
    }

  }
  TEMP.f = stdout;
  *TEMP.name = '\0';
  reporthits(&TEMP, LINK);
  reporthits(LINK->list, LINK);
  reporthits(LINK->instout, LINK);

}



Static Void themain(inst, book, target, sebop, instout_, list_)
_TEXT *inst, *book, *target, *sebop, *instout_, *list_;
{
  struct LOC_themain V;
  double parameterversion;
  boolean Bflip, Tflip;


  long Btheline;
  orgkey Borg;
  boolean Borgchange = false, Borgopen = false;
  chrkey Bchr;
  boolean Bchrchange = false, Bchropen = false, Bpiechange = false,
	  Bpieopen = false;


  long Ttheline;
  orgkey Torg;
  boolean Torgchange = false, Torgopen = false;
  chrkey Tchr;
  boolean Tchrchange = false, Tchropen = false, Tpiechange = false,
	  Tpieopen = false;
  _TEXT TEMP;

  V.instout = instout_;
  V.list = list_;
  printf("sebo %4.2f\n", version);
  readparameters(sebop, &parameterversion, &V.alignmenttype, &V.fromdo,
		 &V.todo, &V.rangecontrol, &V.frominst, &V.toinst,
		 &V.mismatches, &V.alertinterval);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeparameters(&TEMP, V.alignmenttype, V.fromdo, V.todo, V.rangecontrol,
		  V.frominst, V.toinst, V.mismatches, V.alertinterval);


  if (V.alignmenttype == 'i') {
    if (*inst->name != '\0') {
      if (inst->f != NULL)
	inst->f = freopen(inst->name, "r", inst->f);
      else
	inst->f = fopen(inst->name, "r");
    } else
      rewind(inst->f);
    if (inst->f == NULL)
      _EscIO2(FileNotFound, inst->name);
    RESETBUF(inst->f, Char);
    if (BUFEOF(inst->f)) {
      printf("The sebop requests instructions to be used\n");
      printf("but the inst file is empty!\n");
      halt();
    }
  }

  if (*V.instout->name != '\0') {
    if (V.instout->f != NULL)
      V.instout->f = freopen(V.instout->name, "w", V.instout->f);
    else
      V.instout->f = fopen(V.instout->name, "w");
  } else {
    if (V.instout->f != NULL)
      rewind(V.instout->f);
    else
      V.instout->f = tmpfile();
  }
  if (V.instout->f == NULL)
    _EscIO2(FileNotFound, V.instout->name);
  SETUPBUF(V.instout->f, Char);
  fprintf(V.instout->f, "title \"sebo %4.2f search with book\";\n", version);
  fprintf(V.instout->f, "{ sebop parameters:\n");
  writeparameters(V.instout, V.alignmenttype, V.fromdo, V.todo,
		  V.rangecontrol, V.frominst, V.toinst, V.mismatches,
		  V.alertinterval);
  fprintf(V.instout->f, "}\n");
  V.insttype = 1;

  if (*V.list->name != '\0') {
    if (V.list->f != NULL)
      V.list->f = freopen(V.list->name, "w", V.list->f);
    else
      V.list->f = fopen(V.list->name, "w");
  } else {
    if (V.list->f != NULL)
      rewind(V.list->f);
    else
      V.list->f = tmpfile();
  }
  if (V.list->f == NULL)
    _EscIO2(FileNotFound, V.list->name);
  SETUPBUF(V.list->f, Char);
  fprintf(V.list->f, "sebo %4.2f search with book\";\n", version);
  fprintf(V.list->f, "sebop parameters:\n");
  writeparameters(V.list, V.alignmenttype, V.fromdo, V.todo, V.rangecontrol,
		  V.frominst, V.toinst, V.mismatches, V.alertinterval);

  brinit(target, &Ttheline);

  V.Tpie = (piece *)Malloc(sizeof(piece));

  V.Bpie = (piece *)Malloc(sizeof(piece));

  V.linesused = 0;
  V.indent = 3;
  getlineDelila(&V.thefulnam);
  V.targetnumber = 0;

  V.totalprobes = 0;

  while (!BUFEOF(target->f)) {
    printf("\nreading in target book ...\n");
    getocp(target, &Ttheline, &Torg, &Torgchange, &Torgopen, &Tchr,
	   &Tchrchange, &Tchropen, &V.Tpie, &Tpiechange, &Tpieopen);
    printf("... done\n");
    iworgchr(V.instout, Torg, Torgchange, Torgopen, Tchr, Tchrchange,
	     Tchropen);

    if (BUFEOF(target->f)) {
      printf("No further targets.\n");
      continue;
    }
    V.targetnumber++;
    V.Tlength = piecelength(V.Tpie);

    fprintf(V.instout->f, "piece ");
    writename(V.instout, V.Tpie->key.hea.keynam);
    fprintf(V.instout->f, "; { the target sequence is %ld bases }\n",
	    V.Tlength);

    TEMP.f = stdout;
    *TEMP.name = '\0';
    announceTarget(&TEMP, &V);
    announceTarget(V.list, &V);

    if (*inst->name != '\0') {
      if (inst->f != NULL)
	inst->f = freopen(inst->name, "r", inst->f);
      else
	inst->f = fopen(inst->name, "r");
    } else
      rewind(inst->f);
    if (inst->f == NULL)
      _EscIO2(FileNotFound, inst->name);
    RESETBUF(inst->f, Char);
    brinit(book, &Btheline);
    V.probenumber = 0;
    while (!BUFEOF(book->f)) {
      switch (V.alignmenttype) {

      case 'i':
	align(inst, book, &Btheline, &V.Bpie, &V.Blength, &V.alignedbase);
	break;

      case 'b':
      case 'f':
	getocp(book, &Btheline, &Borg, &Borgchange, &Borgopen, &Bchr,
	       &Bchrchange, &Bchropen, &V.Bpie, &Bpiechange, &Bpieopen);
	V.Blength = piecelength(V.Bpie);
	break;

      }

      if (BUFEOF(book->f))
	break;
      V.probenumber++;

      /*



*/
      Tflip = (V.Tpie->key.piedir != V.Tpie->key.coodir);
      Bflip = (V.Bpie->key.piedir != V.Bpie->key.coodir);

      /*







*/

      /*






*/


      if (V.Bpie->key.hea.fulnam->length != 0)
	copyline(V.Bpie->key.hea.fulnam, &V.thefulnam, &V);
      else if (V.thefulnam->length != 0) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	usingpreviousname(&TEMP, &V);
	usingpreviousname(V.list, &V);
	usingpreviousname(V.instout, &V);
	copyline(V.thefulnam, &V.Bpie->key.hea.fulnam, &V);
      }
      TEMP.f = stdout;
      *TEMP.name = '\0';


      announceProbe(&TEMP, &V);
      announceProbe(V.list, &V);

      if (V.Tlength < V.Blength) {
	printf("Target length is less than probe in book.\n");
	/*

*/
      } else
	dosearch(&V);


      clearpiece(&V.Bpie);

    }

  }



  printf("\nprobes:       %ld\n", V.probenumber);
  printf("total probes found: %ld\n", V.totalprobes);
  fprintf(V.list->f, "total probes found: %ld\n", V.totalprobes);
  fprintf(V.instout->f, "{total probes found: %ld}\n", V.totalprobes);
  printf("targets: %ld\n", V.targetnumber);
  printf("sebo search completed\n");

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  instout.f = NULL;
  strcpy(instout.name, "instout");
  list.f = NULL;
  strcpy(list.name, "list");
  sebop.f = NULL;
  strcpy(sebop.name, "sebop");
  target.f = NULL;
  strcpy(target.name, "target");
  book.f = NULL;
  strcpy(book.name, "book");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  themain(&inst, &book, &target, &sebop, &instout, &list);
_L1:
  if (inst.f != NULL)
    fclose(inst.f);
  if (book.f != NULL)
    fclose(book.f);
  if (target.f != NULL)
    fclose(target.f);
  if (sebop.f != NULL)
    fclose(sebop.f);
  if (list.f != NULL)
    fclose(list.f);
  if (instout.f != NULL)
    fclose(instout.f);
  exit(EXIT_SUCCESS);
}


























/* End. */
