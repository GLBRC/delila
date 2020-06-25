/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "bookshift.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.14
/*
















*/
#define updateversion   1.00



/*



































































































*/




#define dnamax          1024
#define namelength      100
#define linelength      80




#define maxstring       150




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



typedef short dnarange;

/* p2c: bookshift.p, line 229:
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



Static _TEXT book, inst, bookshiftp, bookout;






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










Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
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





/*
*/



Static Void bwstartline(book)
_TEXT *book;
{
  fprintf(book->f, "* ");
}


Static Void bwline(book, l)
_TEXT *book;
line *l;
{
  long i, FORLIM;

  if (l == NULL)
    return;
  bwstartline(book);
  if (l->length != 0) {
    FORLIM = l->length;
    for (i = 0; i < FORLIM; i++)
      putc(l->letters[i], book->f);
  }
  putc('\n', book->f);
}


Static Void bwtext(book, lines)
_TEXT *book;
line *lines;
{
  line *l = lines;

  while (l != NULL) {
    bwline(book, l);
    l = l->next;
  }
}


Static Void bwnote(book, note)
_TEXT *book;
line *note;
{
  if (note == NULL)
    return;
  fprintf(book->f, "note\n");
  bwtext(book, note);
  fprintf(book->f, "note\n");
}


Static Void bwnumber(book, num)
_TEXT *book;
long num;
{
  bwstartline(book);
  fprintf(book->f, "%ld\n", num);
}


Static Void bwreanum(book, reanum)
_TEXT *book;
double reanum;
{
  bwstartline(book);
  fprintf(book->f, "%1.2f\n", reanum);
}


Static Void bwstate(book, sta)
_TEXT *book;
state sta;
{
  bwstartline(book);
  switch (sta) {

  case on:
    fprintf(book->f, "on\n");
    break;

  case off:
    fprintf(book->f, "off\n");
    break;
  }
}


Static Void bwname(book, nam)
_TEXT *book;
name nam;
{
  long i;

  bwstartline(book);
  for (i = 0; i < nam.length; i++)
    putc(nam.letters[i], book->f);
  putc('\n', book->f);
}


Static Void bwdirect(book, direct)
_TEXT *book;
direction direct;
{
  bwstartline(book);
  switch (direct) {

  case plus:
    fprintf(book->f, "+\n");
    break;

  case minus:
    fprintf(book->f, "-\n");
    break;
  }
}


Static Void bwconfig(book, config)
_TEXT *book;
configuration config;
{
  bwstartline(book);
  switch (config) {

  case linear:
    fprintf(book->f, "linear\n");
    break;

  case circular:
    fprintf(book->f, "circular\n");
    break;
  }
}


Static Void bwheader(book, hea)
_TEXT *book;
header hea;
{
  bwname(book, hea.keynam);
  bwline(book, hea.fulnam);
  bwnote(book, hea.note);
}


Static Void bworgkey(book, org)
_TEXT *book;
orgkey org;
{
  bwheader(book, org.hea);
  bwline(book, org.mapunit);
}


Static Void bwchrkey(book, chr)
_TEXT *book;
chrkey chr;
{
  bwheader(book, chr.hea);
  bwreanum(book, chr.mapbeg);
  bwreanum(book, chr.mapend);
}




Static Void bworg(thefile, org, chropen, orgopen)
_TEXT *thefile;
orgkey org;
boolean *chropen, *orgopen;
{
  /*

*/
  if (*chropen) {
    fprintf(thefile->f, "chromosome\n");
    *chropen = false;
  }
  if (*orgopen)
    fprintf(thefile->f, "organism\n");
  fprintf(thefile->f, "organism\n");
  bworgkey(thefile, org);
  *orgopen = true;
}



Static Void bwchr(thefile, chr, chropen)
_TEXT *thefile;
chrkey chr;
boolean *chropen;
{
  /*
*/
  if (*chropen)
    fprintf(thefile->f, "chromosome\n");
  fprintf(thefile->f, "chromosome\n");
  bwchrkey(thefile, chr);
  *chropen = true;
}



Static Void bwdna(thefile, d)
_TEXT *thefile;
dnastring *d;
{
  long i, l;
  boolean newline = true;
  long FORLIM;

  fprintf(thefile->f, "dna\n");
  while (d != NULL) {
    FORLIM = d->length;
    for (i = 1; i <= FORLIM; i++) {
      if (newline) {
	bwstartline(thefile);
	l = 0;
	newline = false;
      }
      fputc(basetochar((base)P_getbits_UB(d->part, i - 1, 1, 3)), thefile->f);
      l++;
      if (l % 60 == 0 || i == d->length && d->next == NULL) {
	putc('\n', thefile->f);
	newline = true;
      }
    }
    d = d->next;
  }
  if (!newline)
    putc('\n', thefile->f);
  fprintf(thefile->f, "dna\n");
}



Static Void bwpie(thefile, pie)
_TEXT *thefile;
piece *pie;
{
  fprintf(thefile->f, "piece\n");
  bwheader(thefile, pie->key.hea);

  bwstartline(thefile);
  fprintf(thefile->f, "%1.2f\n", pie->key.mapbeg);

  bwstartline(thefile);
  if (pie->key.coocon == circular)
    fprintf(thefile->f, "circular\n");
  else
    fprintf(thefile->f, "linear\n");

  bwstartline(thefile);
  if (pie->key.coodir == plus)
    fprintf(thefile->f, "+\n");
  else
    fprintf(thefile->f, "-\n");

  bwstartline(thefile);
  fprintf(thefile->f, "%ld\n", pie->key.coobeg);

  bwstartline(thefile);
  fprintf(thefile->f, "%ld\n", pie->key.cooend);

  bwstartline(thefile);
  if (pie->key.piecon == circular)
    fprintf(thefile->f, "circular\n");
  else
    fprintf(thefile->f, "linear\n");

  bwstartline(thefile);
  if (pie->key.piedir == plus)
    fprintf(thefile->f, "+\n");
  else
    fprintf(thefile->f, "-\n");

  bwstartline(thefile);
  fprintf(thefile->f, "%ld\n", pie->key.piebeg);

  bwstartline(thefile);
  fprintf(thefile->f, "%ld\n", pie->key.pieend);
  bwdna(thefile, pie->dna);
  fprintf(thefile->f, "piece\n");
}



Static Void bwref(book, ref)
_TEXT *book;
reference ref;
{
  bwname(book, ref.pienam);
  bwreanum(book, ref.mapbeg);
  bwdirect(book, ref.refdir);
  bwnumber(book, ref.refbeg);
  bwnumber(book, ref.refend);
}



Static Void bwgen(thefile, gene)
_TEXT *thefile;
genkey gene;
{
  /*
*/
  fprintf(thefile->f, "gene\n");
  bwheader(thefile, gene.hea);
  bwref(thefile, gene.ref);
  fprintf(thefile->f, "gene\n");
}



Static Void bwtra(thefile, trans)
_TEXT *thefile;
trakey trans;
{
  /*
*/
  fprintf(thefile->f, "transcript\n");
  bwheader(thefile, trans.hea);
  bwref(thefile, trans.ref);
  fprintf(thefile->f, "transcript\n");
}



Static Void bwmar(thefile, mark)
_TEXT *thefile;
marker mark;
{
  /*
*/
  long i, FORLIM;

  fprintf(thefile->f, "marker\n");
  bwheader(thefile, mark.key.hea);
  bwref(thefile, mark.key.ref);
  bwstate(thefile, mark.key.sta);
  bwstartline(thefile);
  FORLIM = mark.key.phenotype->length;
  for (i = 0; i < FORLIM; i++)
    putc(mark.key.phenotype->letters[i], thefile->f);
  putc('\n', thefile->f);
  bwdna(thefile, mark.dna);
  fprintf(thefile->f, "marker\n");
}


#define maximumrange    10000
/*


*/

#define semicolon       ';'


/* Local variables for alignwrite: */
struct LOC_alignwrite {
  _TEXT *inst;
  Char ch;
  trigger endcomment, endcurly;
} ;

/*
















*/

Local Void skipcomment(f, LINK)
_TEXT *f;
struct LOC_alignwrite *LINK;
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
struct LOC_alignwrite *LINK;
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
struct LOC_alignwrite *LINK;
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









Static Void alignwrite(inst_, book, theline, length, alignedbase, org,
		       orgchange, orgopen, chr, chrchange, chropen, pie,
		       piechange, pieopen)
_TEXT *inst_, *book;
long *theline, *length, *alignedbase;
orgkey *org;
boolean *orgchange, *orgopen;
chrkey *chr;
boolean *chrchange, *chropen;
piece **pie;
boolean *piechange, *pieopen;
{
  /*




*/
  struct LOC_alignwrite V;
  long p, p1;
  boolean done = false;
  long thebase;
  boolean indefault = false;
  /*
*/

  trigger gettrigger, defaulttrigger, nametrigger, piecetrigger, settrigger;

  trigger begincomment, begincurly;

  trigger quote1trigger, quote2trigger;
  _TEXT TEMP;
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
  printf("before getocp line %ld\n", *theline);
  getocp(book, theline, org, orgchange, orgopen, chr, chrchange, chropen, pie,
	 piechange, pieopen);
  printf("after getocp line %ld\n", *theline);
  /*

*/

  if (*orgopen)
    printf("orgopen at line %ld\n", *theline);
  else
    printf("NO orgopen at line %ld\n", *theline);
  if (*chropen)
    printf("chropen at line %ld\n", *theline);
  else
    printf("NO chropen at line %ld\n", *theline);
  if (*orgchange)
    printf("orgchange at line %ld\n", *theline);
  else
    printf("NO orgchange at line %ld\n", *theline);
  if (*chrchange)
    printf("chrchange at line %ld\n", *theline);
  else
    printf("NO chrchange at line %ld\n", *theline);

  if (*orgchange) {
    printf("WRITE ORG\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    bworg(&TEMP, *org, chropen, orgopen);

    bworg(&bookout, *org, chropen, orgopen);
    printf("WRITE ORG DONE\n");
    halt();
  }

  if (*chrchange) {
    printf("WRITE CHR\n");
    /*

*/
    *chropen = false;
    bwchr(&bookout, *chr, chropen);
    printf("WRITE CHR DONE\n");
  }


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
/* p2c: bookshift.p, line 1926: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
	printf("%.*s\n", WITH->length, WITH->letters);
	printf("does not match the inst file name:\n");

	for (p1 = 1; p1 < p; p1++)
	  putchar(WITH->letters[p-1]);

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
/* p2c: bookshift.p, line 1967: Note:
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
      /* blank case */
      break;

    /*

*/
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


#define maximumrange    500


/*


*/


Static Void realignbook(inst, book, bookout, fromparam, toparam,
			alignmenttype)
_TEXT *inst, *book, *bookout;
long *fromparam, *toparam;
Char alignmenttype;
{
  /*


*/
  /*
*/
  long distance;
  /*

*/
  long length, alignedbase;
  long theline = 0;

  orgkey org;
  boolean orgchange = false, orgopen = false;
  chrkey chr;
  boolean chrchange = false, chropen = false;
  piece *pie;
  boolean piechange = false, pieopen = false;
  _TEXT TEMP;

  pie = (piece *)Malloc(sizeof(piece));

  *fromparam = LONG_MAX;
  *toparam = -LONG_MAX;


  /*

*/
  pie = (piece *)Malloc(sizeof(piece));
  if (*book->name != '\0') {
    if (book->f != NULL)
      book->f = freopen(book->name, "r", book->f);
    else
      book->f = fopen(book->name, "r");
  } else {

    rewind(book->f);
  }
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
  if (*bookout->name != '\0') {
    if (bookout->f != NULL)
      bookout->f = freopen(bookout->name, "w", bookout->f);
    else
      bookout->f = fopen(bookout->name, "w");
  } else {
    if (bookout->f != NULL)
      rewind(bookout->f);
    else
      bookout->f = tmpfile();
  }
  if (bookout->f == NULL)
    _EscIO2(FileNotFound, bookout->name);
  SETUPBUF(bookout->f, Char);

  copyaline(book, bookout);
  theline = 1;

  /*

*/
  do {
    printf("HERE GOES!!!!!!!\n");
    switch (alignmenttype) {

    case 'i':
      alignwrite(inst, book, &theline, &length, &alignedbase, &org,
		 &orgchange, &orgopen, &chr, &chrchange, &chropen, &pie,
		 &piechange, &pieopen);
      break;

    case 'b':
    case 'f':
      getpiece(book, &theline, &pie);

      length = piecelength(pie);
      break;
    }
    if (BUFEOF(book->f))
      pieopen = false;
    printf("DONE ***************\n");

    /*


*/
    if (pieopen) {
      switch (alignmenttype) {

      case 'f':
	alignedbase = 1;
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


      TEMP.f = stdout;
      *TEMP.name = '\0';
      bwpie(&TEMP, pie);
      bwpie(bookout, pie);
      clearpiece(&pie);

      halt();
    }

  } while (pieopen);

  /*








*/

  /*



*/
  /*

*/

  if (*toparam - *fromparam > maximumrange) {
    printf(" in procedure realignbook:\n");
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


#define debug           false


/* Local variables for themain: */
struct LOC_themain {


  piece *apiece;
  long zerobase;
} ;

Local Void notbetween(LINK)
struct LOC_themain *LINK;
{
  /*
*/
  piekey *WITH;
  _TEXT TEMP;

  WITH = &LINK->apiece->key;
  printf(" zero base is not between piece ends!\n");
  printf("   pieend: %ld\n", WITH->pieend);
  printf(" zerobase: %ld\n", LINK->zerobase);
  printf("   piebeg: %ld\n", WITH->piebeg);
  printf("piece: \n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  bwpie(&TEMP, LINK->apiece);
  halt();
}




Static Void themain(book, inst, bookshiftp, bookout)
_TEXT *book, *inst, *bookshiftp, *bookout;
{
  struct LOC_themain V;
  double parameterversion;
  /*


*/

  Char alignmenttype = 'i';
  /*
*/

  trigger gettrigger;
  Char ch;
  long theline = 1;
  boolean done;
  long ORIGINALpiebeg, ORIGINALpieend;

  long thelength;
  _TEXT TEMP;
  piekey *WITH;

  printf("bookshift %4.2f\n", version);
  if (*bookshiftp->name != '\0') {
    if (bookshiftp->f != NULL)
      bookshiftp->f = freopen(bookshiftp->name, "r", bookshiftp->f);
    else
      bookshiftp->f = fopen(bookshiftp->name, "r");
  } else
    rewind(bookshiftp->f);
  if (bookshiftp->f == NULL)
    _EscIO2(FileNotFound, bookshiftp->name);
  RESETBUF(bookshiftp->f, Char);
  fscanf(bookshiftp->f, "%lg%*[^\n]", &parameterversion);
  getc(bookshiftp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  V.apiece = (piece *)Malloc(sizeof(piece));

  if (!BUFEOF(bookshiftp->f)) {
    if (!BUFEOF(bookshiftp->f)) {
      fscanf(bookshiftp->f, "%c%*[^\n]", &alignmenttype);
      getc(bookshiftp->f);
      if (alignmenttype == '\n')
	alignmenttype = ' ';
      if (alignmenttype != 'b' && alignmenttype != 'i' && alignmenttype != 'f') {
	printf("alignment type must be f, b, or i\n");
	printf("alignmenttype was \"%c\"\n", alignmenttype);
	halt();
      }
    }
  }

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
    if (alignmenttype == 'i') {
      printf("forcing alignment to be 1 to n because there are no instructions\n");
      alignmenttype = 'f';
    }
  }
  if (alignmenttype == 'f')
    printf("alignment by first base: 1 to n\n");

  /*







*/

  if (*bookout->name != '\0') {
    if (bookout->f != NULL)
      bookout->f = freopen(bookout->name, "w", bookout->f);
    else
      bookout->f = fopen(bookout->name, "w");
  } else {
    if (bookout->f != NULL)
      rewind(bookout->f);
    else
      bookout->f = tmpfile();
  }
  if (bookout->f == NULL)
    _EscIO2(FileNotFound, bookout->name);
  SETUPBUF(bookout->f, Char);

  /*


*/

  filltrigger(&gettrigger,
	      "get from                                          ");
  resettrigger(&gettrigger);
  if (*book->name != '\0') {
    if (book->f != NULL)
      book->f = freopen(book->name, "r", book->f);
    else
      book->f = fopen(book->name, "r");
  } else {

    rewind(book->f);
  }
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
    /*

*/
    if (P_peek(book->f) != 'p') {
      copyaline(book, bookout);
      theline++;
      continue;
    }
    /*


*/
    if (debug)
      printf("Piece at line %ld\n", theline);
    brpiece(book, &theline, &V.apiece);

    if (alignmenttype != 'f') {
      done = false;
      while (!done) {
	if (BUFEOF(inst->f)) {
	  printf("ERROR: No get found for this piece ");
	  printf(" at line %ld of the book:\n", theline);
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  bwpie(&TEMP, V.apiece);
	  halt();
	}
	if (P_eoln(inst->f)) {
	  fscanf(inst->f, "%*[^\n]");
	  /*

*/
	  getc(inst->f);
	  continue;
	}
	ch = getc(inst->f);
	if (ch == '\n')
	  ch = ' ';
	/*

*/
	testfortrigger(ch, &gettrigger);
	if (!gettrigger.found)
	  continue;
	WITH = &V.apiece->key;
	fscanf(inst->f, "%ld", &V.zerobase);
	if (debug) {
	  printf("  zero base: %ld\n", V.zerobase);
	  printf("  from: %ld\n", WITH->piebeg);
	  printf("    to: %ld\n", WITH->pieend);
	}

	ORIGINALpiebeg = WITH->piebeg;
	ORIGINALpieend = WITH->pieend;

	if (V.zerobase >= WITH->piebeg) {
	  if (V.zerobase > WITH->pieend)
	    notbetween(&V);
	  WITH->piebeg = ORIGINALpiebeg - V.zerobase;
	  WITH->pieend = ORIGINALpieend - V.zerobase;
	} else {
	  if (V.zerobase < WITH->pieend)
	    notbetween(&V);
	  WITH->piebeg = ORIGINALpieend - V.zerobase;
	  WITH->pieend = ORIGINALpiebeg - V.zerobase;
	}


	WITH->piedir = plus;
	WITH->coodir = plus;
	WITH->coobeg = WITH->piebeg;
	WITH->cooend = WITH->pieend;

	fscanf(inst->f, "%*[^\n]");
	getc(inst->f);
	resettrigger(&gettrigger);
	done = true;
      }
    }
    /*


*/

    else {
      WITH = &V.apiece->key;


      thelength = piecelength(V.apiece);
      WITH->coodir = plus;
      WITH->coobeg = 1;
      WITH->cooend = thelength;
      WITH->piedir = plus;
      WITH->piebeg = 1;
      WITH->pieend = thelength;
    }

    bwpie(bookout, V.apiece);
  }

}

#undef debug


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  bookout.f = NULL;
  strcpy(bookout.name, "bookout");
  bookshiftp.f = NULL;
  strcpy(bookshiftp.name, "bookshiftp");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  book.f = NULL;
  strcpy(book.name, "book");
  themain(&book, &inst, &bookshiftp, &bookout);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (inst.f != NULL)
    fclose(inst.f);
  if (bookshiftp.f != NULL)
    fclose(bookshiftp.f);
  if (bookout.f != NULL)
    fclose(bookout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
