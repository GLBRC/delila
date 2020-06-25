/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "diana.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*









*/



#define version         2.21
/*



























*/
#define updateversion   2.15
#define previousupdateversion  2.01
#define versionupperbound  10.00



/*




















































































































































































































*/








/*























*/

#define trimax          5151
#define infofield       6
#define infodecim       5




#define dnamax          1024
#define namelength      100
#define linelength      200



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

/* p2c: diana.p, line 390:
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





typedef struct trianglearray {
  long data[trimax + 1];
  long lower, upper, side, area;
} trianglearray;




typedef trianglearray trisquare[(long)t - (long)a + 1][(long)t - (long)a + 1];


typedef double realprism[(long)t - (long)a + 1][(long)t - (long)a + 1];


typedef long sumbase[(long)t - (long)a + 1];


typedef double basefreq[(long)t - (long)a + 1];


Static trisquare dataprism;
/*
*/






Static line *freeline;
Static dnastring *freedna;

Static boolean readnumber;
/*
*/
Static long number;
Static boolean numbered, skipunnum;
/*
*/






Static _TEXT book, inst, protseq, dianap, da;


Static jmp_buf _JL1;







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



Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) == ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) != ' ') & (!P_eoln(thefile->f)))
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
      if (V.ch == '.')
	dotteddone = true;
      if (WITH->letters[p-1] == '.')
	dotteddone = true;
      if (WITH->letters[p-1] != V.ch && !dotteddone && V.ch != ';') {
	printf("The piece name in the book: \n");
/* p2c: diana.p, line 1645: Note:
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
/* p2c: diana.p, line 1692: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  printf(" piece name: %.*s\n", WITH->length, WITH->letters);
  printf(" piece number: %ld\n", number);
  printf(" aligned base is too far away... see the code\n");
  halt();
}

#undef maximumrange
#undef semicolon






Static Void writepiename(thefile, pie, namespace)
_TEXT *thefile;
piece *pie;
long namespace;
{
  long index;

  for (index = 1; index <= namespace; index++) {
    if (index <= pie->key.hea.keynam.length)
      putc(pie->key.hea.keynam.letters[index-1], thefile->f);
    else
      putc(' ', thefile->f);
  }
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



Static boolean withinalignment(alignedposition, alignedbase, length)
long alignedposition, alignedbase, length;
{
  /*
*/
  long p;

  p = alignedposition + alignedbase;
  return (p > 0 && p <= length);
}


#define maximumrange    10000




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
    printf(" in procedure maxminalignment:\n");
    printf(" alignedbase = %ld\n", alignedbase);
    printf(" fromparameter = %ld\n", *fromparam);
    printf(" toparameter = %ld\n", *toparam);
    printf(" this exceeds the maximum range allowed (%ld)\n",
	   (long)maximumrange);
    printf(" see notes in the procedure. \n");
    /*



*/
    halt();
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






Static long tripos(tri, x, y)
trianglearray tri;
long x, y;
{
  long hold;

  if (x < y) {
    hold = x;
    x = y;
    y = hold;
  }
  if (x < tri.lower || y > tri.upper) {
    printf("array lower: %5ld upper: %5ld exceeded", tri.lower, tri.upper);
    printf(" out of bounds at (%ld,%ld)\n", x, y);
    halt();
  }

  y -= tri.lower;
  x -= tri.lower;
  /*
*/
  return (tri.side * y - y * (y + 1) / 2 + x);
}


Static Void cleartriangle(triangle, fromspot, tospot)
trianglearray *triangle;
long fromspot, tospot;
{
  /*
*/
  long loop, FORLIM;

  triangle->lower = fromspot;
  triangle->upper = tospot;
  triangle->side = triangle->upper - triangle->lower + 1;
  if (triangle->side <= 0) {
    printf("illegal triangle size; lower: %5ld", triangle->lower);
    printf(" upper: %5ld\n", triangle->upper);
    halt();
  }
  triangle->area = tripos(*triangle, triangle->upper, triangle->upper);
  if (triangle->area > trimax) {
    printf("requested triangle size too large for trimax:\n");
    printf("trimax is: %ld request was %ld\n", (long)trimax, triangle->area);
    halt();
  }
  FORLIM = triangle->area;
  for (loop = 0; loop <= FORLIM; loop++)
    triangle->data[loop] = 0;
}


Static Void puttriangle(tri, value, x, y)
trianglearray *tri;
long value, x, y;
{
  tri->data[tripos(*tri, x, y)] = value;
}


Static Void gettriangle(tri, value, x, y)
trianglearray *tri;
long *value, x, y;
{
  *value = tri->data[tripos(*tri, x, y)];
}


Static Void writetriangle(fout, tri)
_TEXT *fout;
trianglearray tri;
{
  long x, y, value, FORLIM, FORLIM1;

  FORLIM = tri.lower;
  for (y = tri.upper; y >= FORLIM; y--) {
    FORLIM1 = tri.upper;
    for (x = tri.lower; x <= FORLIM1; x++) {
      if (x >= y) {
	gettriangle(&tri, &value, x, y);
	fprintf(fout->f, " %5ld", value);
      } else
	fprintf(fout->f, " -----");
    }
    putc('\n', fout->f);
  }
}





Static Void protseqrange(protseq, fromparam, toparam)
_TEXT *protseq;
long *fromparam, *toparam;
{
  *fromparam = 1;
  while ((P_peek(protseq->f) == '*') | (P_peek(protseq->f) == '>')) {
    fscanf(protseq->f, "%*[^\n]");
    getc(protseq->f);
  }
  *toparam = 0;
  do {
    getc(protseq->f);
    (*toparam)++;
  } while (!((P_peek(protseq->f) == '.') | P_eoln(protseq->f)));

}


/* Local variables for filehead: */
struct LOC_filehead {
  long fromrange, torange, maxdistance, maxsequence, pairs, side;
} ;

Local Void showdata(tofile, c_, LINK)
_TEXT *tofile;
Char c_;
struct LOC_filehead *LINK;
{
  fprintf(tofile->f, "%c %ld %ld is the from-to range used\n",
	  c_, LINK->fromrange, LINK->torange);
  fprintf(tofile->f, "%c %ld bases is the side of the data triangle\n",
	  c_, LINK->side);
  fprintf(tofile->f, "%c %ld pairs will be analyzed [=side*(side+1)/2]\n",
	  c_, LINK->pairs);
  fprintf(tofile->f, "%c %ld maximum distance written\n",
	  c_, LINK->maxdistance);
  fprintf(tofile->f, "%c %ld maximum sequences\n", c_, LINK->maxsequence);
}







Static Void filehead(fin, fout, fromrange_, torange_, maxdistance_,
		     maxsequence_)
_TEXT *fin, *fout;
long fromrange_, torange_, maxdistance_, maxsequence_;
{
  struct LOC_filehead V;
  long copynumber = 1;
  _TEXT TEMP;

  V.fromrange = fromrange_;
  V.torange = torange_;
  V.maxdistance = maxdistance_;
  V.maxsequence = maxsequence_;
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


  fprintf(fout->f, "* diana %4.2f\n", version);


  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  RESETBUF(fin->f, Char);
  fprintf(fout->f, "* ");
  if (copylines(fin, fout, copynumber) != copynumber) {
    fprintf(fout->f, "inst does not have enough header lines\n");
    halt();
  }

  V.side = V.torange - V.fromrange + 1;
  V.pairs = (long)floor(V.side * (V.side + 1.0) / 2 + 0.5);
  showdata(fout, '*', &V);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  showdata(&TEMP, ' ', &V);


  /*






*/

  fprintf(fout->f, "*\n");
  fprintf(fout->f, "* Definition of columns:\n");
  fprintf(fout->f, "*  1: n (normal), d (diagonal), i (information)\n");
  fprintf(fout->f,
	  "*  2: dinucleotide, in (info, normal), id (info, diagonal)\n");
  fprintf(fout->f, "*  3: x coordinate\n");
  fprintf(fout->f, "*  4: y coordinate\n");
  fprintf(fout->f, "*  5: 1, use for xscolumn for xyplop\n");
  fprintf(fout->f, "*  6: 1, use for yscolumn for xyplop\n");
  fprintf(fout->f, "*  7: sum.data[position]\n");
  fprintf(fout->f,
	  "*  8: Rcorrelation (with small sample correction), (bits)\n");
  fprintf(fout->f, "*  9: chisquare\n");
  fprintf(fout->f, "* 10: degrees of freedom\n");
  fprintf(fout->f, "* 11: 1, use for hue for xyplop\n");
  fprintf(fout->f, "* 12: 0, use for saturation for xyplop\n");
  fprintf(fout->f, "* 13: Rnormalized = Rcorrelation/4 or 1-(Rsequence/2)\n");
  fprintf(fout->f, "* 14: 1-Rnormalized\n");
  fprintf(fout->f, "* 15: correlation coefficient for x to y\n");
  fprintf(fout->f, "* 16: 1, use for saturation for xyplop\n");

  /*


*/

}







Static Void addpair(xbase, ybase, xcoord, ycoord, dataprism)
base xbase, ybase;
long xcoord, ycoord;
trianglearray (*dataprism)[(long)t - (long)a + 1];
{
  long distance;
  trianglearray *WITH;

  distance = tripos(dataprism[(long)xbase - (long)a]
		    [(long)ybase - (long)a], xcoord, ycoord);
  WITH = &dataprism[(long)xbase - (long)a][(long)ybase - (long)a];

  /*


*/
  WITH->data[distance]++;
}




Static Void readbook(apiece, book, theline, dataprism, fromparam, toparam,
		     fromrequest, torequest, alignmenttype, maxsequences,
		     maxdistance)
piece **apiece;
_TEXT *book;
long theline;
trianglearray (*dataprism)[(long)t - (long)a + 1];
long fromparam, toparam, fromrequest, torequest;
Char alignmenttype;
long maxsequences, maxdistance;
{
  /*




*/
  long alignedbase, length;
  base xbase;
  long x;
  long xcoord = 0;
  long y;
  base ybase;
  long ycoord = 0;
  _TEXT empty;
  long count = 0;
  Char bk = '\b';

  empty.f = NULL;
  *empty.name = '\0';


  for (xbase = a; (long)xbase <= (long)t; xbase = (base)((long)xbase + 1)) {
    for (ybase = a; (long)ybase <= (long)t; ybase = (base)((long)ybase + 1))
      cleartriangle(&dataprism[(long)xbase - (long)a]
		    [(long)ybase - (long)a], fromrequest, torequest);
  }



  if (*empty.name != '\0') {
    if (empty.f != NULL)
      empty.f = freopen(empty.name, "w", empty.f);
    else
      empty.f = fopen(empty.name, "w");
  } else {
    if (empty.f != NULL)
      rewind(empty.f);
    else
      empty.f = tmpfile();
  }
  if (empty.f == NULL)
    _EscIO2(FileNotFound, empty.name);
  SETUPBUF(empty.f, Char);
  if (*empty.name != '\0') {
    if (empty.f != NULL)
      empty.f = freopen(empty.name, "r", empty.f);
    else
      empty.f = fopen(empty.name, "r");
  } else
    rewind(empty.f);
  if (empty.f == NULL)
    _EscIO2(FileNotFound, empty.name);
  RESETBUF(empty.f, Char);
  printf(" inserting sequence: ");
  while (!BUFEOF(book->f) && count <= maxsequences) {
    switch (alignmenttype) {

    case 'i':
      align(&inst, book, &theline, apiece, &length, &alignedbase);
      break;

    case 'b':
      align(&empty, book, &theline, apiece, &length, &alignedbase);
      break;
    }

    if (!BUFEOF(book->f)) {
      count++;

      printf(" %7ld%c%c%c%c%c%c%c%c", count, bk, bk, bk, bk, bk, bk, bk, bk);
      /*






*/
      for (xcoord = fromrequest; xcoord <= torequest; xcoord++) {
	x = xcoord + alignedbase;
	if (x >= 1 && x <= length) {
	  xbase = getbase(x, *apiece);
	  /*

*/
	  addpair(xbase, xbase, xcoord, xcoord, dataprism);
	  for (ycoord = xcoord + 1; ycoord <= torequest; ycoord++) {
	    y = ycoord + alignedbase;

	    /*

*/
	    if (y - x <= maxdistance) {
	      if (y >= 1 && y <= length) {
		ybase = getbase(y, *apiece);
		/*

*/
		addpair(xbase, ybase, xcoord, ycoord, dataprism);
	      }
	    }
	  }
	}
      }

    }
    clearpiece(apiece);
  }


  if (empty.f != NULL)
    fclose(empty.f);
}


#define debugging       false



Static Void readprotseq(protseq, dataprism, fromparam, toparam, fromrequest,
			torequest, maxsequences)
_TEXT *protseq;
trianglearray (*dataprism)[(long)t - (long)a + 1];
long fromparam, toparam, fromrequest, torequest, maxsequences;
{
  /*



*/
  Char letter;
  long alignedbase, length;
  Char sequence[trimax];
  base xbase;
  long xcoord = 0;
  base ybase;
  long ycoord = 0;
  boolean done;
  long count = 0;

  alignedbase = fromrequest - fromparam + 1;


  for (xbase = a; (long)xbase <= (long)t; xbase = (base)((long)xbase + 1)) {
    for (ybase = a; (long)ybase <= (long)t; ybase = (base)((long)ybase + 1))
      cleartriangle(&dataprism[(long)xbase - (long)a]
		    [(long)ybase - (long)a], fromrequest, torequest);
  }


  if (*protseq->name != '\0') {
    if (protseq->f != NULL)
      protseq->f = freopen(protseq->name, "r", protseq->f);
    else
      protseq->f = fopen(protseq->name, "r");
  } else
    rewind(protseq->f);
  if (protseq->f == NULL)
    _EscIO2(FileNotFound, protseq->name);
  RESETBUF(protseq->f, Char);
  while (!BUFEOF(protseq->f) && count <= maxsequences) {
    while (P_peek(protseq->f) == '*') {
      fscanf(protseq->f, "%*[^\n]");
      getc(protseq->f);
    }
    while (P_peek(protseq->f) == '>') {
      fscanf(protseq->f, "%*[^\n]");
      getc(protseq->f);
    }
    length = 0;
    done = false;
    count++;
    do {
      if (BUFEOF(protseq->f))
	done = true;
      else {
	letter = getc(protseq->f);
	if (letter == '\n')
	  letter = ' ';
	if (P_peek(protseq->f) == '.' || P_peek(protseq->f) == '>' ||
	    P_peek(protseq->f) == '*')
	  done = true;
	else {
	  if (letter == 'A')
	    letter = 'a';
	  if (letter == 'C')
	    letter = 'c';
	  if (letter == 'G')
	    letter = 'g';
	  if (letter == 'T')
	    letter = 't';
	  if (letter == 'U')
	    letter = 't';
	  if (letter == '-' || letter == 't' || letter == 'g' ||
	      letter == 'c' || letter == 'a') {
	    sequence[length-1] = letter;
	    length++;
	  }
	}
	if (P_eoln(protseq->f)) {
	  fscanf(protseq->f, "%*[^\n]");
	  getc(protseq->f);
	}
      }
    } while (!done);

    if (BUFEOF(protseq->f))
      break;
    for (xcoord = fromrequest; xcoord <= torequest; xcoord++) {
      letter = sequence[xcoord + alignedbase - 1];
      if (letter != '-') {
	xbase = chartobase(letter);
	addpair(xbase, xbase, xcoord, xcoord, dataprism);
	for (ycoord = xcoord + 1; ycoord <= torequest; ycoord++) {
	  if (sequence[ycoord + alignedbase - 1] != '-') {
	    ybase = chartobase(sequence[ycoord + alignedbase - 1]);
	    addpair(xbase, ybase, xcoord, ycoord, dataprism);
	  }
	}

      }
    }
  }

}

#undef debugging




Static Void totalprism(dataprism, totalsum, fromparam, toparam)
trianglearray (*dataprism)[(long)t - (long)a + 1];
trianglearray *totalsum;
long fromparam, toparam;
{
  long position;
  base xindex, yindex;
  long FORLIM;
  trianglearray *WITH;

  cleartriangle(totalsum, 1L, toparam - fromparam + 1);


  FORLIM = dataprism[0][0].area;
  for (position = 0; position <= FORLIM; position++) {
    for (xindex = a; (long)xindex <= (long)t; xindex = (base)((long)xindex + 1)) {
      for (yindex = a; (long)yindex <= (long)t; yindex = (base)((long)yindex + 1)) {
	WITH = &dataprism[(long)xindex - (long)a][(long)yindex - (long)a];
	totalsum->data[position] += WITH->data[position];
      }
    }
  }
}


#define bigoutput       false




Static Void writeprism(dianalysis, dataprism, fromparam, toparam, sum,
		       dicontrol, colorslope, colorconstant, rangelower,
		       rangeupper, maxdistance)
_TEXT *dianalysis;
trianglearray (*dataprism)[(long)t - (long)a + 1];
long fromparam, toparam;
trianglearray sum;
Char dicontrol;
double colorslope, colorconstant, rangelower, rangeupper;
long maxdistance;
{
  /*


*/
  base baseindex;
  double chisquare, correlation;
  boolean dodata, doinfo;
  double expected;
  long freedom;
  double Hx, Hy, Hxy;
  realprism freq;
  double ln2 = log(2.0);
  double observed;
  long position;
  double Rcorrelation;
  double Rcorrelationsum = 0.0;
  double Rc, Rnormalized, Rx, Ry, Rxy, ssc4 = 3 / (2 * log(2.0));
  /*
*/
  double ssc16 = 15 / (2 * log(2.0));
  /*
*/
  base xbase;
  long xfreedom;
  basefreq xfreq;
  long xindex;
  sumbase xsum;
  base ybase;
  long yfreedom;
  basefreq yfreq;
  long yindex;
  sumbase ysum;
  trianglearray *WITH;
  double TEMP;

  if (dicontrol == 'b') {
    dodata = true;
    doinfo = true;
  } else if (dicontrol == 'd') {
    dodata = true;
    doinfo = false;
  } else if (dicontrol == 'i') {
    dodata = false;
    doinfo = true;
  }

  for (xindex = fromparam; xindex <= toparam; xindex++) {
    /*

*/
    for (yindex = xindex; yindex <= toparam; yindex++) {
      if (yindex - xindex <= maxdistance) {
	Hxy = 0.0;
	Hx = 0.0;
	Hy = 0.0;
	Rxy = 0.0;
	Rx = 0.0;
	Ry = 0.0;
	for (baseindex = a;
	     (long)baseindex <= (long)t;
	     baseindex = (base)((long)baseindex + 1)) {
	  xsum[(long)baseindex - (long)a] = 0;
	  ysum[(long)baseindex - (long)a] = 0;
	}


	for (xbase = a; (long)xbase <= (long)t; xbase = (base)((long)xbase + 1)) {
	  for (ybase = a; (long)ybase <= (long)t; ybase = (base)((long)ybase + 1)) {
	    position = tripos(dataprism[(long)xbase - (long)a]
			      [(long)ybase - (long)a], xindex, yindex);


	    WITH = &dataprism[(long)xbase - (long)a][(long)ybase - (long)a];


	    xsum[(long)xbase - (long)a] += WITH->data[position];
	    ysum[(long)ybase - (long)a] += WITH->data[position];


	    /*



*/
	    /*








*/



	    if (sum.data[position] > 0) {
	      freq[(long)xbase - (long)a][(long)ybase - (long)a] =
		(double)WITH->data[position] / sum.data[position];
	      if (freq[(long)xbase - (long)a][(long)ybase - (long)a] != 0.0)
		Hxy -= freq[(long)xbase - (long)a]
		    [(long)ybase - (long)a] * log(freq[(long)xbase - (long)a]
						  [(long)ybase - (long)a]);
	    }

	    if (dodata) {
	      if (xindex != yindex)
		putc('n', dianalysis->f);
	      else
		putc('d', dianalysis->f);
	      fprintf(dianalysis->f, " %c%c",
		      basetochar(xbase), basetochar(ybase));
	      fprintf(dianalysis->f, " %ld", xindex);
	      fprintf(dianalysis->f, " %ld", yindex);
	      fprintf(dianalysis->f, " 1");
	      fprintf(dianalysis->f, " 1");
	      fprintf(dianalysis->f, " %ld",
		      dataprism[(long)xbase - (long)a]
		      [(long)ybase - (long)a].data[position]);
	      fprintf(dianalysis->f, " %12.10f",
		      freq[(long)xbase - (long)a][(long)ybase - (long)a]);
	      fprintf(dianalysis->f, " %12.10f",
		      1 - freq[(long)xbase - (long)a][(long)ybase - (long)a]);
	      fprintf(dianalysis->f, " 1");
	      fprintf(dianalysis->f, " 1");
	      fprintf(dianalysis->f, " 0");
	      fprintf(dianalysis->f, " 0");
	      fprintf(dianalysis->f, " 0");
	      fprintf(dianalysis->f, " 0");
	      fprintf(dianalysis->f, " 1\n");
	    }
	  }

	}


	xfreedom = 0;
	yfreedom = 0;
	freedom = 0;



	/*







*/



	for (baseindex = a;
	     (long)baseindex <= (long)t;
	     baseindex = (base)((long)baseindex + 1)) {
	  if (sum.data[position] > 0) {
	    xfreq[(long)baseindex - (long)a] =
	      (double)xsum[(long)baseindex - (long)a] / sum.data[position];
	    if (xfreq[(long)baseindex - (long)a] > 0) {
	      Hx -= xfreq[(long)baseindex - (long)a] *
		    log(xfreq[(long)baseindex - (long)a]);
	      xfreedom++;
	    }


	    yfreq[(long)baseindex - (long)a] =
	      (double)ysum[(long)baseindex - (long)a] / sum.data[position];
	    if (yfreq[(long)baseindex - (long)a] > 0) {
	      Hy -= yfreq[(long)baseindex - (long)a] *
		    log(yfreq[(long)baseindex - (long)a]);
	      yfreedom++;
	    }
	  }


	}




	freedom = (xfreedom - 1) * (yfreedom - 1);


	/*




*/
	Rcorrelation = (Hx + Hy - Hxy) / ln2;

	/*










*/



	chisquare = 0.0;
	for (xbase = a; (long)xbase <= (long)t; xbase = (base)((long)xbase + 1)) {
	  for (ybase = a; (long)ybase <= (long)t; ybase = (base)((long)ybase + 1)) {
	    expected = xfreq[(long)xbase - (long)a] *
		       yfreq[(long)ybase - (long)a] * sum.data[position];
	    observed = dataprism[(long)xbase - (long)a]
		       [(long)ybase - (long)a].data[position];
	    if (expected != 0) {
	      TEMP = observed - expected;
	      chisquare += TEMP * TEMP / expected;
	    }
	  }
	}
	correlation = 0.0;

	if (doinfo) {
	  fprintf(dianalysis->f, "i ");
	  putc('i', dianalysis->f);

	  if (bigoutput) {
	    printf("* n= %ld", sum.data[position]);
	    printf("* Rcorrelation = %*.*f",
		   infofield, infodecim, Rcorrelation);
	  }

	  if (xindex == yindex) {
	    /*
*/
	    Rnormalized = 1.0 - Rcorrelation / 2.0;
	    putc('d', dianalysis->f);


	    if (sum.data[position] > 0) {
	      Rc = Rcorrelation - ssc4 / sum.data[position];
	      Rcorrelationsum += Rc;
	    } else
	      Rc = 0.0;

	    if (bigoutput) {
	      printf("*  ssc4/n = %*.*f",
		     infofield, infodecim, ssc4 / sum.data[position]);
	      printf("* Rcorrelation -  ssc4/n = %*.*f",
		     infofield, infodecim, Rc);
	    }

	  } else {
	    Rnormalized = Rcorrelation / 4.0;
	    putc('n', dianalysis->f);

	    if (sum.data[position] > 0) {
	      Rc = Rcorrelation - ssc16 / sum.data[position];
	      Rcorrelationsum += Rc;
	    } else
	      Rc = 0.0;

	    /*







*/


	    if (bigoutput) {
	      printf("* ssc16/n = %*.*f",
		     infofield, infodecim, ssc16 / sum.data[position]);
	      printf("* Rcorrelation - ssc16/n = %*.*f",
		     infofield, infodecim, Rc);
	    }

	  }

	  if (bigoutput)
	    putchar('\n');

	  Rnormalized = colorslope * Rnormalized + colorconstant;
	  fprintf(dianalysis->f, " %ld", xindex);
	  fprintf(dianalysis->f, " %ld", yindex);
	  fprintf(dianalysis->f, " 1");
	  fprintf(dianalysis->f, " 1");
	  fprintf(dianalysis->f, " %ld", sum.data[position]);
	  fprintf(dianalysis->f, " %*.*f", infofield, infodecim, Rc);
	  fprintf(dianalysis->f, " %8.5f", chisquare);
	  fprintf(dianalysis->f, " %ld", freedom);
	  fprintf(dianalysis->f, " 1");
	  fprintf(dianalysis->f, " 0");
	  fprintf(dianalysis->f, " %*.*f", infofield, infodecim, Rnormalized);
	  fprintf(dianalysis->f, " %*.*f",
		  infofield, infodecim, 1 - Rnormalized);
	  fprintf(dianalysis->f, " %*.*f", infofield, infodecim, correlation);
	  fprintf(dianalysis->f, " 1");

	  if (Rc >= rangelower && Rc <= rangeupper)
	    fprintf(dianalysis->f, " INN");

	  else
	    fprintf(dianalysis->f, " OUT");
	  putc('\n', dianalysis->f);
	  /*




*/
	}

      }



    }
  }
  printf("sum of Rcorrelation %10.8f bits\n", Rcorrelationsum);
  fprintf(dianalysis->f, "* sum of Rcorrelation %10.8f bits\n",
	  Rcorrelationsum);
}

#undef bigoutput


/* Local variables for checknumber: */
struct LOC_checknumber {
  _TEXT *afile;
  boolean ok;
} ;

Local Void conclude(LINK)
struct LOC_checknumber *LINK;
{
  _TEXT TEMP;

  printf("Including this character, the rest of the data line is:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  copyaline(LINK->afile, &TEMP);
  LINK->ok = false;
}







Static boolean checknumber(afile_)
_TEXT *afile_;
{
  /*
*/
  struct LOC_checknumber V;

  V.afile = afile_;
  V.ok = true;
  if (BUFEOF(V.afile->f)) {
    V.ok = false;
    printf("A number was expected on a data line, but");
    printf(" the end of the file was found instead.\n");
    return false;
  }
  skipblanks(V.afile);
  if (P_eoln(V.afile->f)) {
    printf("A number was expected on a data line, but");
    printf(" the end of the line was found instead.\n");
    conclude(&V);
  }
  if (P_peek(V.afile->f) == '+' || P_peek(V.afile->f) == '-' ||
      P_peek(V.afile->f) == '.' || P_peek(V.afile->f) == '9' ||
      P_peek(V.afile->f) == '8' || P_peek(V.afile->f) == '7' ||
      P_peek(V.afile->f) == '6' || P_peek(V.afile->f) == '5' ||
      P_peek(V.afile->f) == '4' || P_peek(V.afile->f) == '3' ||
      P_peek(V.afile->f) == '2' || P_peek(V.afile->f) == '1' ||
      P_peek(V.afile->f) == '0')
    return V.ok;
  printf("A number was expected on a data line, but");
  printf(" the character \"%c\" was found instead.\n", P_peek(V.afile->f));
  conclude(&V);
  return V.ok;
}



Static Void copyfile(fin, fout)
_TEXT *fin, *fout;
{
  while (!BUFEOF(fin->f))
    copyaline(fin, fout);
}


#define copylines_      4



Static Void upgradeto201(dianap)
_TEXT *dianap;
{
  /*











*/
  _TEXT internal;
  long line_;
  double parameterversion = 2.01;

  internal.f = NULL;
  *internal.name = '\0';
  printf("upgrading to version %4.2f ...\n", parameterversion);


  if (*dianap->name != '\0') {
    if (dianap->f != NULL)
      dianap->f = freopen(dianap->name, "r", dianap->f);
    else
      dianap->f = fopen(dianap->name, "r");
  } else
    rewind(dianap->f);
  if (dianap->f == NULL)
    _EscIO2(FileNotFound, dianap->name);
  RESETBUF(dianap->f, Char);
  /*


*/
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  for (line_ = 1; line_ <= copylines_; line_++)
    copyaline(dianap, &internal);


  fprintf(internal.f,
	  "i           f: first base, i: inst, b: book alignment\n");


  /*

*/


  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  if (*dianap->name != '\0') {
    if (dianap->f != NULL)
      dianap->f = freopen(dianap->name, "w", dianap->f);
    else
      dianap->f = fopen(dianap->name, "w");
  } else {
    if (dianap->f != NULL)
      rewind(dianap->f);
    else
      dianap->f = tmpfile();
  }
  if (dianap->f == NULL)
    _EscIO2(FileNotFound, dianap->name);
  SETUPBUF(dianap->f, Char);
  fprintf(dianap->f,
    "%4.2f        version of dianap that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, dianap);




  if (*dianap->name != '\0') {
    if (dianap->f != NULL)
      dianap->f = freopen(dianap->name, "r", dianap->f);
    else
      dianap->f = fopen(dianap->name, "r");
  } else
    rewind(dianap->f);
  if (dianap->f == NULL)
    _EscIO2(FileNotFound, dianap->name);
  RESETBUF(dianap->f, Char);

  if (internal.f != NULL)
    fclose(internal.f);
}

#undef copylines_


#define copylines_      5



Static Void upgradeto215(dianap)
_TEXT *dianap;
{
  /*











*/
  _TEXT internal;
  long line_;
  double parameterversion = 2.15;

  internal.f = NULL;
  *internal.name = '\0';
  printf("upgrading to version %4.2f ...\n", parameterversion);


  if (*dianap->name != '\0') {
    if (dianap->f != NULL)
      dianap->f = freopen(dianap->name, "r", dianap->f);
    else
      dianap->f = fopen(dianap->name, "r");
  } else
    rewind(dianap->f);
  if (dianap->f == NULL)
    _EscIO2(FileNotFound, dianap->name);
  RESETBUF(dianap->f, Char);
  fscanf(dianap->f, "%*[^\n]");
  getc(dianap->f);
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  for (line_ = 1; line_ <= copylines_; line_++)
    copyaline(dianap, &internal);


  fprintf(internal.f, "r 0 100   range of Rcorrelation to plot\n");
  fprintf(internal.f,
	  "20        maxsequences - maxmum number of sequences to read\n");
  fprintf(internal.f,
	  "2         maxsequences - maxmum number of sequences to read\n");


  /*

*/


  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  if (*dianap->name != '\0') {
    if (dianap->f != NULL)
      dianap->f = freopen(dianap->name, "w", dianap->f);
    else
      dianap->f = fopen(dianap->name, "w");
  } else {
    if (dianap->f != NULL)
      rewind(dianap->f);
    else
      dianap->f = tmpfile();
  }
  if (dianap->f == NULL)
    _EscIO2(FileNotFound, dianap->name);
  SETUPBUF(dianap->f, Char);
  fprintf(dianap->f,
    "%4.2f        version of dianap that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, dianap);




  if (*dianap->name != '\0') {
    if (dianap->f != NULL)
      dianap->f = freopen(dianap->name, "r", dianap->f);
    else
      dianap->f = fopen(dianap->name, "r");
  } else
    rewind(dianap->f);
  if (dianap->f == NULL)
    _EscIO2(FileNotFound, dianap->name);
  RESETBUF(dianap->f, Char);

  if (internal.f != NULL)
    fclose(internal.f);
}

#undef copylines_



Static Void upgradeparameters(dianap)
_TEXT *dianap;
{
  double parameterversion;

  fscanf(dianap->f, "%lg%*[^\n]", &parameterversion);
  getc(dianap->f);

  if ((long)floor(100 * parameterversion + 0.5) >=
      (long)floor(100 * updateversion + 0.5) &&
      (long)floor(100 * parameterversion + 0.5) <=
      (long)floor(100 * versionupperbound + 0.5))
    return;
  /*

*/

  printf("^GYou have an old parameter file!, version %4.2f!\n",
	 parameterversion);
  printf("          version = %4.2f\n", version);
  printf("    updateversion = %4.2f\n", updateversion);
  printf(" parameterversion = %4.2f\n", parameterversion);
  printf("versionupperbound = %4.2f\n", versionupperbound);

  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100 * 1.88 + 0.5))
    upgradeto201(dianap);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100 * 2.15 + 0.5))
    upgradeto215(dianap);

  if (*dianap->name != '\0') {
    if (dianap->f != NULL)
      dianap->f = freopen(dianap->name, "r", dianap->f);
    else
      dianap->f = fopen(dianap->name, "r");
  } else
    rewind(dianap->f);
  if (dianap->f == NULL)
    _EscIO2(FileNotFound, dianap->name);
  RESETBUF(dianap->f, Char);
  fscanf(dianap->f, "%lg%*[^\n]", &parameterversion);
  getc(dianap->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    printf("Sorry! I am unable to fully upgrade your parameter file\n");
    printf("from version %4.2f to version %4.2f!\n",
	   parameterversion, updateversion);
    printf("Start from a fresh copy or edit this one.\n");
    halt();
  }

  else
    printf("... upgrade successful!\n");

  printf("See this page for the new documentation:\n");
  printf("https://alum.mit.edu/www/toms/delila/diana.html\n");
  printf("----------\n");
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *dianap;
  Char alignmenttype, rangecontrol;
  double rangelower, rangeupper;
  long maxsequences, maxdistance;

  Char bookorprotseq;
  /*
*/
  double colorslope, colorconstant;
  /*


*/
  Char dicontrol;
  long fromparam, fromrequest, fromprotseq, toparam, torequest, toprotseq;
} ;

/* Local variables for readparameters: */
struct LOC_readparameters {
  struct LOC_themain *LINK;
  boolean checkout;
} ;

Local Void cn(LINK)
struct LOC_readparameters *LINK;
{
  LINK->checkout = checknumber(LINK->LINK->dianap);
  if (!LINK->checkout)
    halt();
}

Local Void readparameters(LINK)
struct LOC_themain *LINK;
{
  /*
*/
  struct LOC_readparameters V;

  V.LINK = LINK;
  V.checkout = true;
  if (*LINK->dianap->name != '\0') {
    if (LINK->dianap->f != NULL)
      LINK->dianap->f = freopen(LINK->dianap->name, "r", LINK->dianap->f);
    else
      LINK->dianap->f = fopen(LINK->dianap->name, "r");
  } else
    rewind(LINK->dianap->f);
  if (LINK->dianap->f == NULL)
    _EscIO2(FileNotFound, LINK->dianap->name);
  RESETBUF(LINK->dianap->f, Char);

  if (!BUFEOF(LINK->dianap->f)) {
    cn(&V);
    upgradeparameters(LINK->dianap);

    cn(&V);
    fscanf(LINK->dianap->f, "%ld%ld%*[^\n]", &LINK->fromrequest,
	   &LINK->torequest);
    getc(LINK->dianap->f);
    cn(&V);
    fscanf(LINK->dianap->f, "%ld%ld%*[^\n]", &LINK->fromprotseq,
	   &LINK->toprotseq);
    getc(LINK->dianap->f);
    fscanf(LINK->dianap->f, "%c%*[^\n]", &LINK->dicontrol);
    getc(LINK->dianap->f);
    if (LINK->dicontrol == '\n')
      LINK->dicontrol = ' ';
    cn(&V);
    fscanf(LINK->dianap->f, "%lg%lg%*[^\n]", &LINK->colorslope,
	   &LINK->colorconstant);
    getc(LINK->dianap->f);
    fscanf(LINK->dianap->f, "%c%*[^\n]", &LINK->alignmenttype);
    getc(LINK->dianap->f);

    if (LINK->alignmenttype == '\n')
      LINK->alignmenttype = ' ';
    LINK->rangecontrol = getc(LINK->dianap->f);
    if (LINK->rangecontrol == '\n')
      LINK->rangecontrol = ' ';
    if (LINK->rangecontrol == 'r')
      fscanf(LINK->dianap->f, "%lg%lg", &LINK->rangelower, &LINK->rangeupper);
    else {
      LINK->rangelower = -LONG_MAX;
      LINK->rangeupper = LONG_MAX;
    }
    fscanf(LINK->dianap->f, "%*[^\n]");
    getc(LINK->dianap->f);
    cn(&V);
    fscanf(LINK->dianap->f, "%ld%*[^\n]", &LINK->maxsequences);
    getc(LINK->dianap->f);
    cn(&V);
    fscanf(LINK->dianap->f, "%ld%*[^\n]", &LINK->maxdistance);
    getc(LINK->dianap->f);
  } else {
    LINK->fromrequest = LINK->fromparam;
    LINK->torequest = LINK->toparam;
    LINK->fromprotseq = LINK->fromparam;
    LINK->toprotseq = LINK->toparam;
    LINK->dicontrol = 'b';
    LINK->colorslope = 0.84;
    LINK->colorconstant = 0.16;
    LINK->alignmenttype = 'i';
    LINK->rangecontrol = '-';
    LINK->rangelower = -LONG_MAX;
    LINK->rangeupper = LONG_MAX;
    LINK->maxsequences = LONG_MAX;
    LINK->maxdistance = LONG_MAX;

  }


  if (LINK->fromrequest > LINK->torequest) {
    printf("fromrequest must be <= torequest\n");
    halt();
  }

  if (LINK->bookorprotseq == 'p') {
    LINK->fromparam = LINK->fromprotseq;
    LINK->toparam = LINK->toprotseq;
  }


}



Static Void themain(book, inst, protseq, dianap_, da)
_TEXT *book, *inst, *protseq, *dianap_, *da;
{
  struct LOC_themain V;
  piece *apiece;
  long theline = 0;
  trianglearray totalsum;


  V.dianap = dianap_;
  printf(" diana %4.2f\n", version);

  readparameters(&V);


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
  if (*protseq->name != '\0') {
    if (protseq->f != NULL)
      protseq->f = freopen(protseq->name, "r", protseq->f);
    else
      protseq->f = fopen(protseq->name, "r");
  } else
    rewind(protseq->f);
  if (protseq->f == NULL)
    _EscIO2(FileNotFound, protseq->name);
  RESETBUF(protseq->f, Char);

  /*
*/
  fscanf(book->f, "%*[^\n]");
  getc(book->f);
  fscanf(protseq->f, "%*[^\n]");

  getc(protseq->f);

  if (BUFEOF(book->f) & BUFEOF(protseq->f)) {
    printf("either book or protseq must contain sequence data\n");
    halt();
  }

  if ((!BUFEOF(book->f)) & (!BUFEOF(protseq->f))) {
    printf("One of book or protseq must be empty\n");
    printf("eof(book)%s\n", BUFEOF(book->f) ? " TRUE" : "FALSE");
    printf("eof(protseq)%s\n", BUFEOF(protseq->f) ? " TRUE" : "FALSE");
    halt();
  }

  if (!BUFEOF(book->f)) {
    brinit(book, &theline);
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
    apiece = (piece *)Malloc(sizeof(piece));


    printf(" Using book\n");
    printf(" getting the range from the book.\n");
    maxminalignment(inst, book, &theline, &V.fromparam, &V.toparam,
		    V.alignmenttype);
    printf(" Book is from: %ld to: %ld\n", V.fromparam, V.toparam);
    V.bookorprotseq = 'b';
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

  if (*protseq->name != '\0') {
    if (protseq->f != NULL)
      protseq->f = freopen(protseq->name, "r", protseq->f);
    else
      protseq->f = fopen(protseq->name, "r");
  } else
    rewind(protseq->f);
  if (protseq->f == NULL)
    _EscIO2(FileNotFound, protseq->name);
  RESETBUF(protseq->f, Char);
  fscanf(protseq->f, "%*[^\n]");
  getc(protseq->f);
  if (!BUFEOF(protseq->f)) {
    printf(" Using protseq\n");
    printf(" Getting the range from the protseq.\n");
    protseqrange(protseq, &V.fromparam, &V.toparam);
    /*

*/
    V.bookorprotseq = 'p';
  }


  if (V.fromrequest < V.fromparam) {
    printf("************** WARNING **************\n");
    printf("Requested FROM parameter (%ld)\n", V.fromrequest);
    printf(
      "cannot be lower than the sequence FROM limit (%ld).  Using sequence limit: %ld\n",
      V.fromparam, V.fromparam);
    V.fromrequest = V.fromparam;
  }
  if (V.torequest > V.toparam) {
    printf("requested TO parameter (%ld)\n", V.torequest);
    printf(
      "cannot be lower than the sequence TO limit (%ld).  Using sequence limit: %ld\n",
      V.toparam, V.toparam);
    V.torequest = V.toparam;
  }


  if (*da->name != '\0') {
    if (da->f != NULL)
      da->f = freopen(da->name, "w", da->f);
    else
      da->f = fopen(da->name, "w");
  } else {
    if (da->f != NULL)
      rewind(da->f);
    else
      da->f = tmpfile();
  }
  if (da->f == NULL)
    _EscIO2(FileNotFound, da->name);
  SETUPBUF(da->f, Char);
  switch (V.bookorprotseq) {

  case 'b':
    filehead(book, da, V.fromrequest, V.torequest, V.maxdistance,
	     V.maxsequences);
    break;

  case 'p':
    filehead(protseq, da, V.fromrequest, V.torequest, V.maxdistance,
	     V.maxsequences);
    break;
  }

  printf(" creating the dataprism\n");


  switch (V.bookorprotseq) {

  case 'b':
    readbook(&apiece, book, theline, dataprism, V.fromparam, V.toparam,
	     V.fromrequest, V.torequest, V.alignmenttype, V.maxsequences,
	     V.maxdistance);
    break;

  case 'p':
    readprotseq(protseq, dataprism, V.fromparam, V.toparam, V.fromrequest,
		V.torequest, V.maxsequences);
    break;

  }


  printf(" totaling the dataprism\n");
  totalprism(dataprism, &totalsum, V.fromrequest, V.torequest);


  printf(" calculating and writing the dataprism to da\n");
  writeprism(da, dataprism, V.fromrequest, V.torequest, totalsum, V.dicontrol,
	     V.colorslope, V.colorconstant, V.rangelower, V.rangeupper,
	     V.maxdistance);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  da.f = NULL;
  strcpy(da.name, "da");
  dianap.f = NULL;
  strcpy(dianap.name, "dianap");
  protseq.f = NULL;
  strcpy(protseq.name, "protseq");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  book.f = NULL;
  strcpy(book.name, "book");
  themain(&book, &inst, &protseq, &dianap, &da);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (inst.f != NULL)
    fclose(inst.f);
  if (protseq.f != NULL)
    fclose(protseq.f);
  if (dianap.f != NULL)
    fclose(dianap.f);
  if (da.f != NULL)
    fclose(da.f);
  exit(EXIT_SUCCESS);
}



/* End. */
