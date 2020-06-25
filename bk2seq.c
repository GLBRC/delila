/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "bk2seq.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*








*/



#define version         1.13
/*



*/



/*







































*/


#define symbolsperline  70
/*
*/



#define dnamax          1024
#define namelength      100
#define linelength      200



#define pwid            8
#define pdec            5
#define pdecolor        4
/*
*/


#define fillermax       50



#define maxstring       2000





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

/* p2c: bk2seq.p, line 134:
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
  long state_;
  boolean skip, found;
} trigger;


Static _TEXT inst, book, bk2seqp, alprop, protseq;


Static piece *apiece;
Static long length_, alignedbase, fromparam, toparam;

Static long fromdo, todo;
Static boolean userrange;
Static Char paging;

Static long index_;

Static boolean orgopen;
Static Char nametype;
/*
*/
Static long namewidth;

Static Char b;
Static long column;
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
  t_->state_ = 0;
  t_->skip = false;
  t_->found = false;
}


Static Void testfortrigger(ch, t_)
Char ch;
trigger *t_;
{
  /*









*/
  t_->state_++;
  /*



*/
  if (t_->seek.letters[t_->state_ - 1] == ch) {
    t_->skip = false;
    if (t_->state_ == t_->seek.length)
      t_->found = true;
    else
      t_->found = false;
    return;
  }
  /*
*/
  if (t_->seek.letters[0] == ch) {
    t_->state_ = 1;
    t_->skip = false;
    t_->found = false;
    return;
  }
  t_->state_ = 0;
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

  boolean dotteddone;
  name *WITH;

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
    /*

*/
    /*

*/
    p = 1;
    dotteddone = false;
    while (!dotteddone) {
      if (P_eoln(V.inst->f)) {
	dotteddone = true;
	break;
      }
      V.ch = getc(V.inst->f);

      if (V.ch == '\n')
	V.ch = ' ';
      /*

*/
      if (V.ch == '.')
	dotteddone = true;
      if (WITH->letters[p-1] == '.')
	dotteddone = true;
      /*


*/

      if (WITH->letters[p-1] != V.ch && !dotteddone && V.ch != ';') {
	printf("The piece name in the book: \n");
/* p2c: bk2seq.p, line 1386: Note:
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
      p++;
      if (p > WITH->length) {
	dotteddone = true;

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
/* p2c: bk2seq.p, line 1433: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  printf(" piece name: %.*s\n", WITH->length, WITH->letters);
  printf(" piece number: %ld\n", number);
  printf(" aligned base is too far away... see the code\n");
  halt();
}

#undef maximumrange
#undef semicolon


#define maximumrange    500






Static Void maxminalignment(inst, book, theline, fromparam, toparam,
			    alignmenttype)
_TEXT *inst, *book;
long *theline, *fromparam, *toparam;
Char alignmenttype;
{
  /*





*/
  /*
*/
  long distance;
  piece *pie;
  long length, alignedbase;

  pie = (piece *)Malloc(sizeof(piece));

  *fromparam = LONG_MAX;
  *toparam = -LONG_MAX;

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
  while (!BUFEOF(book->f)) {
    switch (alignmenttype) {

    case 'i':
      align(inst, book, theline, &pie, &length, &alignedbase);
      break;

    case 'b':
    case 'f':
      getpiece(book, theline, &pie);
      length = piecelength(pie);
      break;
    }

    if (BUFEOF(book->f))
      break;
    switch (alignmenttype) {

    case 'f':
      alignedbase = 0;
      *fromparam = 1;
      distance = length - alignedbase;
      if (*toparam < distance)
	*toparam = distance;
      break;

    case 'i':
      distance = 1 - alignedbase;
      if (*fromparam > distance)
	*fromparam = distance;

      distance = length - alignedbase;
      if (*toparam < distance)
	*toparam = distance;
      break;

    case 'b':
      alignedbase = pietoint(0L, pie);
      distance = 1 - alignedbase;
      if (*fromparam > distance)
	*fromparam = distance;

      distance = length - alignedbase;
      if (*toparam < distance)
	*toparam = distance;
      break;
    }

    clearpiece(&pie);
  }

  if (*toparam - *fromparam > maximumrange) {
    printf(" WARNING: in procedure maxminalignment:\n");
    printf(" alignedbase = %ld\n", alignedbase);
    printf(" fromparameter = %ld\n", *fromparam);
    printf(" toparameter = %ld\n", *toparam);
    printf(" this exceeds the maximum range allowed (%ld)\n",
	   (long)maximumrange);
    printf(" see notes in the procedure. \n");

    /*



*/
  }


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
  Free(pie);
}

#undef maximumrange



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





Static Void pnumberbar(afile, spaces, firstnumber, lastnumber, linesused)
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
    putc('(', afile->f);
    for (spacecount = 1; spacecount <= spaces; spacecount++)
      putc(' ', afile->f);
    for (number = firstnumber; number <= lastnumber; number++)
      fputc(numberdigit(number, logplace), afile->f);
    fprintf(afile->f, ") sn\n");
  }
}


/* Local variables for getname: */
struct LOC_getname {
  _TEXT *namebook;
  Char *typefound;
  name *aname;
  long *theline;
  Char ch;
} ;

Local Void obtain(LINK)
struct LOC_getname *LINK;
{
  brname(LINK->namebook, LINK->theline, LINK->aname);
  *LINK->typefound = LINK->ch;
}


Static Void getname(namebook_, nametype, typefound_, aname_, orgopen,
		    theline_)
_TEXT *namebook_;
Char nametype, *typefound_;
name *aname_;
boolean *orgopen;
long *theline_;
{
  /*
*/
  /*


*/
  struct LOC_getname V;
  long SET[5];

  V.namebook = namebook_;
  V.typefound = typefound_;
  V.aname = aname_;
  V.theline = theline_;
  *V.typefound = '.';
  do {
    P_addset(P_expset(SET, 0L), 'o');
    V.ch = getto(V.namebook, V.theline, P_addset(SET, nametype));
    if (V.ch != ' ') {
      switch (V.ch) {

      case 'o':
	*orgopen = !*orgopen;
	if (*orgopen)
	  obtain(&V);
	break;

      case 'g':
      case 't':
      case 'p':
	if (V.ch == nametype)
	  obtain(&V);

	V.ch = getto(V.namebook, V.theline, P_addset(P_expset(SET, 0L), V.ch));
	break;
      }
    } else
      *V.typefound = V.ch;
  } while (*V.typefound != nametype && *V.typefound != 'o' &&
	   *V.typefound != ' ');
}


Static Void printname(namebook, nametype, namewidth, tofile, ctofile, orgopen,
		      linenumber, clinenumber, theline)
_TEXT *namebook;
Char nametype;
long namewidth;
_TEXT *tofile, *ctofile;
boolean *orgopen;
long *linenumber, *clinenumber, *theline;
{
  /*


*/
  name aname;
  Char typefound;
  long index;

  do {
    getname(namebook, nametype, &typefound, &aname, orgopen, theline);
    if (typefound != ' ') {
      switch (typefound) {

      case 'o':
	fprintf(tofile->f, "\n organism %.*s\n", namelength, aname.letters);
	fprintf(ctofile->f, "\n organism %.*s\n", namelength, aname.letters);
	*linenumber += 2;
	*clinenumber += 2;
	break;

      case 'g':
      case 't':
      case 'p':
	putc(' ', tofile->f);
	putc(' ', ctofile->f);
	for (index = 0; index < namewidth; index++)
	  putc(aname.letters[index], tofile->f);
	for (index = 0; index < namewidth; index++)
	  putc(aname.letters[index], ctofile->f);
	break;
      }
    }
  } while (typefound != nametype && typefound != ' ');
}


Static Void protectpostscript(afile, c_)
_TEXT *afile;
Char c_;
{
  /*




*/
  if (c_ == '\\' || c_ == '%' || c_ == ')' || c_ == '(')
    putc('\\', afile->f);
}



Static Void copynoreturn(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");

  getc(fin->f);
}


Static Void themain(inst, book, bk2seqp, alprop, protseq)
_TEXT *inst, *book, *bk2seqp, *alprop, *protseq;
{
  long alignpoint;
  /*
*/
  long index;
  Char outtype;
  /*
*/
  long theline;
  Char alignmenttype = 'i';
  long FORLIM;

  /*


*/
  printf("bk2seq %4.2f\n", version);
  if (*alprop->name != '\0') {
    if (alprop->f != NULL)
      alprop->f = freopen(alprop->name, "w", alprop->f);
    else
      alprop->f = fopen(alprop->name, "w");
  } else {
    if (alprop->f != NULL)
      rewind(alprop->f);
    else
      alprop->f = tmpfile();
  }
  if (alprop->f == NULL)
    _EscIO2(FileNotFound, alprop->name);
  SETUPBUF(alprop->f, Char);
  if (*protseq->name != '\0') {
    if (protseq->f != NULL)
      protseq->f = freopen(protseq->name, "w", protseq->f);
    else
      protseq->f = fopen(protseq->name, "w");
  } else {
    if (protseq->f != NULL)
      rewind(protseq->f);
    else
      protseq->f = tmpfile();
  }
  if (protseq->f == NULL)
    _EscIO2(FileNotFound, protseq->name);
  SETUPBUF(protseq->f, Char);

  if (*bk2seqp->name != '\0') {
    if (bk2seqp->f != NULL)
      bk2seqp->f = freopen(bk2seqp->name, "r", bk2seqp->f);
    else
      bk2seqp->f = fopen(bk2seqp->name, "r");
  } else
    rewind(bk2seqp->f);
  if (bk2seqp->f == NULL)
    _EscIO2(FileNotFound, bk2seqp->name);
  RESETBUF(bk2seqp->f, Char);
  if (BUFEOF(bk2seqp->f))
    outtype = 'p';
  else
    outtype = 'f';

  apiece = (piece *)Malloc(sizeof(piece));
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
  brinit(book, &theline);


  if (outtype == 'p') {
    fprintf(protseq->f, "* bk2seq %4.2f\n", version);
    fprintf(protseq->f, "* ");
  } else
    fprintf(protseq->f, "> bk2seq %4.2f", version);
  copyaline(book, protseq);
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

  maxminalignment(inst, book, &theline, &fromparam, &toparam, alignmenttype);

  fromdo = fromparam;
  todo = toparam;
  userrange = false;

  if (outtype == 'p') {
    fprintf(protseq->f, "* The book is from:   %ld to %ld\n",
	    fromparam, toparam);
    fprintf(protseq->f, "* Alist list is from: %ld to %ld\n", fromdo, todo);
  }

  alignpoint = 1 - fromparam;
  if (alignpoint < 1) {
    printf("WARNING\n");
    printf("Something may be wrong with the alignment of the book.\n");
    printf("The alignment is outside the pieces.\n");
  }

  fprintf(alprop->f, "%ld alignment point for the alpro program\n",
	  alignpoint);

  while (!BUFEOF(book->f)) {
    align(inst, book, &theline, &apiece, &length_, &alignedbase);
    if (BUFEOF(book->f))
      break;

    if (outtype == 'p')
      fprintf(protseq->f, "* ");
    else
      fprintf(protseq->f, "> ");

    /*


*/
    index = 1;
    while (apiece->key.hea.keynam.letters[index-1] != ' ' &&
	   index < namelength) {
      putc(apiece->key.hea.keynam.letters[index-1], protseq->f);
      index++;
    }
    fprintf(protseq->f, " %5ld", inttopie(alignedbase, apiece));
    if (numbered)
      fprintf(protseq->f, " %5ld ", number);
    putc('\n', protseq->f);

    column = 0;
    FORLIM = todo;
    for (index = fromdo; index <= FORLIM; index++) {
      if (withinalignment(index, alignedbase, length_)) {
	b = basetochar(getbase(index + alignedbase, apiece));
	putc(b, protseq->f);
      } else
	putc('-', protseq->f);
      column++;
      if (column % symbolsperline == 0) {
	column = 0;
	putc('\n', protseq->f);
      }
    }

    if (outtype == 'p')
      putc('.', protseq->f);


    putc('\n', protseq->f);
    clearpiece(&apiece);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  protseq.f = NULL;
  strcpy(protseq.name, "protseq");
  alprop.f = NULL;
  strcpy(alprop.name, "alprop");
  bk2seqp.f = NULL;
  strcpy(bk2seqp.name, "bk2seqp");
  book.f = NULL;
  strcpy(book.name, "book");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  themain(&inst, &book, &bk2seqp, &alprop, &protseq);
_L1:
  if (inst.f != NULL)
    fclose(inst.f);
  if (book.f != NULL)
    fclose(book.f);
  if (bk2seqp.f != NULL)
    fclose(bk2seqp.f);
  if (alprop.f != NULL)
    fclose(alprop.f);
  if (protseq.f != NULL)
    fclose(protseq.f);
  exit(EXIT_SUCCESS);
}



/* End. */
