/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "mergemarks.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.32
/*















*/
#define updateversion   1.00



/*

















































































*/



#define bignumber       1e15
/*
*/




#define maxstring       2000





#define dnamax          1024
#define namelength      100
#define linelength      200




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;






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

/* p2c: mergemarks.p, line 193:
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



Static _TEXT book, marks1, marks2, mergemarksp, marksout;






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

  /*
















*/

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



Static Void getstring(afile, buffer, gotten)
_TEXT *afile;
stringDelila *buffer;
boolean *gotten;
{
  /*
*/
  long index = 0;

  clearstring(buffer);
  if (BUFEOF(afile->f)) {
    *gotten = false;
    return;
  }
  while (!P_eoln(afile->f) && index < maxstring) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
  }

  if (!P_eoln(afile->f)) {
    printf(" getstring: a line exceeds maximum string size (%ld)\n",
	   (long)maxstring);
    halt();
  }

  buffer->length = index;
  buffer->current = 1;
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  *gotten = true;
}


/* Local variables for figurestring: */
struct LOC_figurestring {
  stringDelila *line_;
  long power;
} ;

Local long figureinteger(first, last, LINK)
long first, last;
struct LOC_figurestring *LINK;
{
  long i;
  long sum = 0;
  long increment;

  LINK->power = 1;
  for (i = last - 1; i >= first - 1; i--) {
    switch (LINK->line_->letters[i]) {

    case '0':
      increment = 0;
      break;

    case '1':
      increment = 1;
      break;

    case '2':
      increment = 2;
      break;

    case '3':
      increment = 3;
      break;

    case '4':
      increment = 4;
      break;

    case '5':
      increment = 5;
      break;

    case '6':
      increment = 6;
      break;

    case '7':
      increment = 7;
      break;

    case '8':
      increment = 8;
      break;

    case '9':
      increment = 9;
      break;
    }
    sum += LINK->power * increment;
    LINK->power *= 10;
  }
  return sum;
}



Static Void figurestring(line__, first, last, whzat, c_, i, r)
stringDelila *line__;
long *first, *last;
Char *whzat, *c_;
long *i;
double *r;
{
  /*













*/
  struct LOC_figurestring V;
  long numbers[3];
  long sign, numberstart;
  /*
*/
  long point = 0;

  long l;
  stringDelila *WITH;
  long FORLIM;


  V.line_ = line__;
  P_addset(P_expset(numbers, 0L), '0');
  P_addset(numbers, '1');
  P_addset(numbers, '2');
  P_addset(numbers, '3');
  P_addset(numbers, '4');
  P_addset(numbers, '5');
  P_addset(numbers, '6');
  P_addset(numbers, '7');
  P_addset(numbers, '8');
  P_addset(numbers, '9');
  *whzat = '.';


  WITH = V.line_;

  if (WITH->length == 0 || WITH->current < 1 || WITH->current > WITH->length)
    *whzat = ' ';
  else {
    *first = V.line_->current;
    while (V.line_->letters[*first - 1] == ' ' && *first < V.line_->length)
      (*first)++;
    if (*first == V.line_->length && V.line_->letters[*first - 1] == ' ')
      *whzat = ' ';
  }

  if (*whzat == ' ')
    return;
  *last = *first;
  while (V.line_->letters[*last - 1] != ' ' && *last < V.line_->length)
    (*last)++;
  if (V.line_->letters[*last - 1] == ' ')
    (*last)--;



  *c_ = V.line_->letters[*first - 1];
  if (P_inset(*c_, numbers) || *c_ == '-' || *c_ == '+') {
    if (*c_ == '-' || *c_ == '+') {
      switch (*c_) {

      case '+':
	sign = 1;
	break;

      case '-':
	sign = -1;
	break;
      }
      numberstart = *first + 1;
    } else {
      sign = 1;
      numberstart = *first;
    }

    *whzat = 'i';
    FORLIM = *last;
    for (l = numberstart; l <= FORLIM; l++) {
      if (!P_inset(V.line_->letters[l-1], numbers)) {
	if (V.line_->letters[l-1] == '.') {
	  if (*whzat == 'i') {
	    *whzat = 'r';
	    point = l;
	  } else
	    *whzat = 'g';
	} else
	  *whzat = 'g';
      }
    }




    if (*whzat == 'r' && point == *last)
      *whzat = 'i';

    if (*whzat == 'i') {
      if (point == *last)
	*i = sign * figureinteger(numberstart, *last - 1, &V);
      else
	*i = sign * figureinteger(numberstart, *last, &V);
      *r = *i;
    } else if (*whzat == 'r') {
      *i = figureinteger(numberstart, point - 1, &V);
      *r = sign * (*i + (double)figureinteger(point + 1, *last, &V) / V.power);
      *i *= sign;
    }
  } else
    *whzat = 'c';


  V.line_->current = *last + 1;
}







Static Void parsestring(marks, s, m, c1, c2, done)
_TEXT *marks;
stringDelila *s;
double *m;
Char *c1, *c2;
boolean *done;
{
  /*
*/
  boolean gotten;

  long first, last;
  Char whzat, c_;
  long i;
  double r;
  _TEXT TEMP;

  /*


*/
  figurestring(s, &first, &last, &whzat, &c_, &i, &r);
  /*

*/
  *c1 = c_;
  /*
*/
  if (first != last)
    *c2 = s->letters[1];
  else
    *c2 = ' ';
  /*

*/

  if (*c1 == '*' || *c1 == '%' || *c1 == 'o') {
    writestring(&marksout, s);
    putc('\n', marksout.f);
    return;
  }


  if (*c1 == 'p') {
    *done = true;
    *c2 = 'P';
    *m = bignumber;
    /*



*/
    return;
  }



  /*









*/

  if (*c1 == 'u') {
    writestring(&marksout, s);
    putc('\n', marksout.f);
    gotten = true;
    while (gotten && s->letters[0] != '!') {
      getstring(marks, s, &gotten);
      if (!gotten)
	break;
      writestring(&marksout, s);
      putc('\n', marksout.f);
    }
    return;
  }


  /*

*/

  figurestring(s, &first, &last, &whzat, &c_, &i, &r);
  /*

*/
  if (whzat != 'r' && whzat != 'i') {
    printf("parsestring: could not find a real number or integer.\n");
    printf("The string is:\n");
    putchar('"');
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, s);
    printf("\"\n");
    for (i = 1; i <= first; i++)
      putchar(' ');
    printf("^ this is not the beginning of an integer\n");
    /*


*/
    printf("c1    = %c\n", *c1);
    printf("c2    = %c\n", *c2);
    *done = true;
  }
  *m = r;

  *done = true;

}



Static Void markget(marks, marksout, m, c1, c2, s, gotten)
_TEXT *marks, *marksout;
double *m;
Char *c1, *c2;
stringDelila *s;
boolean *gotten;
{
  /*

*/
  boolean done = false;

  /*

*/
  *gotten = false;
  while (!done) {
    /*

*/
    if (BUFEOF(marks->f)) {
      done = true;
      *gotten = false;
      continue;
    }
    if (P_eoln(marks->f)) {
      fscanf(marks->f, "%*[^\n]");
      getc(marks->f);
      putc('\n', marksout->f);
    }

    else {
      getstring(marks, s, gotten);
      /*



*/
      if (*gotten) {
	parsestring(marks, s, m, c1, c2, &done);

      }
    }
  }
}



Static Void markput(marksout, m, c1, c2, s)
_TEXT *marksout;
double *m;
Char *c1, *c2;
stringDelila *s;
{
  /*

*/
  if (*c1 == 'p')
    fprintf(marksout->f, "* p finished synchronization %ld\n",
	    (long)floor(*m + 0.5));
  /*
*/
  else if (*c1 != ' ') {
    writestring(marksout, s);
    putc('\n', marksout->f);
  }
  *m = bignumber;
  *c1 = ' ';
  *c2 = ' ';
  clearstring(s);
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *marks1, *marks2, *marksout;
  boolean gotten;
  double m1, m2;
  Char m1c1, m1c2, m2c1, m2c2;
  /*

*/
  stringDelila s1, s2;
  boolean waitfor1, waitfor2;
  piece *pie;
  /*
*/

  boolean pieceisdone;
} ;

/* Local variables for markcheck: */
struct LOC_markcheck {
  struct LOC_themain *LINK;
  long position, position1, position2;
  /*


*/
  double d1, d2;
  boolean onefirst;
  long twopiecelength;
} ;

Local Void doifchain(LINK)
struct LOC_markcheck *LINK;
{
  if (LINK->position1 == LINK->position2 && LINK->position1 == LINK->position) {
    LINK->twopiecelength = piecelength(LINK->LINK->pie) * 2;
    switch (LINK->LINK->pie->key.piedir) {

    case minus:
      LINK->d1 = (long)(LINK->LINK->m1 + LINK->twopiecelength) -
		 LINK->LINK->m1 - LINK->twopiecelength;
      LINK->d2 = (long)(LINK->LINK->m2 + LINK->twopiecelength) -
		 LINK->LINK->m2 - LINK->twopiecelength;
      LINK->onefirst = (LINK->d1 < LINK->d2);
      break;

    case plus:
      LINK->d1 = (long)(LINK->LINK->m1 + LINK->twopiecelength) -
		 LINK->LINK->m1 - LINK->twopiecelength;
      LINK->d2 = (long)(LINK->LINK->m2 + LINK->twopiecelength) -
		 LINK->LINK->m2 - LINK->twopiecelength;
      LINK->onefirst = (LINK->d1 >= LINK->d2);
      break;
    }

    /*


*/
    /*
*/
    if (LINK->d1 == LINK->d2)
      LINK->onefirst = true;

    if (LINK->onefirst) {
      markput(LINK->LINK->marksout, &LINK->LINK->m1, &LINK->LINK->m1c1,
	      &LINK->LINK->m1c2, &LINK->LINK->s1);
      markget(LINK->LINK->marks1, LINK->LINK->marksout, &LINK->LINK->m1,
	      &LINK->LINK->m1c1, &LINK->LINK->m1c2, &LINK->LINK->s1,
	      &LINK->LINK->gotten);
      markput(LINK->LINK->marksout, &LINK->LINK->m2, &LINK->LINK->m2c1,
	      &LINK->LINK->m2c2, &LINK->LINK->s2);
      markget(LINK->LINK->marks2, LINK->LINK->marksout, &LINK->LINK->m2,
	      &LINK->LINK->m2c1, &LINK->LINK->m2c2, &LINK->LINK->s2,
	      &LINK->LINK->gotten);
      return;
    }
    markput(LINK->LINK->marksout, &LINK->LINK->m2, &LINK->LINK->m2c1,
	    &LINK->LINK->m2c2, &LINK->LINK->s2);
    markget(LINK->LINK->marks2, LINK->LINK->marksout, &LINK->LINK->m2,
	    &LINK->LINK->m2c1, &LINK->LINK->m2c2, &LINK->LINK->s2,
	    &LINK->LINK->gotten);
    markput(LINK->LINK->marksout, &LINK->LINK->m1, &LINK->LINK->m1c1,
	    &LINK->LINK->m1c2, &LINK->LINK->s1);
    markget(LINK->LINK->marks1, LINK->LINK->marksout, &LINK->LINK->m1,
	    &LINK->LINK->m1c1, &LINK->LINK->m1c2, &LINK->LINK->s1,
	    &LINK->LINK->gotten);
    return;
  }
  /*



*/
  /*
*/
  /*

*/


  if (LINK->position1 == LINK->position) {
    fprintf(LINK->LINK->marksout->f,
	    "* position1 = position  (* do just 1 *)\n");
    markput(LINK->LINK->marksout, &LINK->LINK->m1, &LINK->LINK->m1c1,
	    &LINK->LINK->m1c2, &LINK->LINK->s1);
    markget(LINK->LINK->marks1, LINK->LINK->marksout, &LINK->LINK->m1,
	    &LINK->LINK->m1c1, &LINK->LINK->m1c2, &LINK->LINK->s1,
	    &LINK->LINK->gotten);
    return;
  }

  if (LINK->position2 != LINK->position)
    return;
  fprintf(LINK->LINK->marksout->f,
	  "* position2 = position  (* do just 2 *)\n");
  markput(LINK->LINK->marksout, &LINK->LINK->m2, &LINK->LINK->m2c1,
	  &LINK->LINK->m2c2, &LINK->LINK->s2);
  markget(LINK->LINK->marks2, LINK->LINK->marksout, &LINK->LINK->m2,
	  &LINK->LINK->m2c1, &LINK->LINK->m2c2, &LINK->LINK->s2,
	  &LINK->LINK->gotten);
}

/*


*/

Local Void markcheck(position_, decreasing, LINK)
long position_;
boolean decreasing;
struct LOC_themain *LINK;
{
  /*
*/
  struct LOC_markcheck V;

  V.LINK = LINK;

  /*


*/
  V.position = position_;
  if (LINK->m1c1 == 'p') {
    /*

*/
    LINK->waitfor2 = true;
  }

  if (LINK->m2c1 == 'p') {
    /*

*/
    LINK->waitfor1 = true;
  }

  if (LINK->waitfor1 && LINK->waitfor2) {
    fprintf(LINK->marksout->f, "p - SYNCHRONOUS FIND\n");
    LINK->pieceisdone = true;
    fprintf(LINK->marksout->f, "* pieceisdone ----> waitfor1 and waitfor2\n");

    markget(LINK->marks1, LINK->marksout, &LINK->m1, &LINK->m1c1, &LINK->m1c2,
	    &LINK->s1, &LINK->gotten);
    /*




*/

    markget(LINK->marks2, LINK->marksout, &LINK->m2, &LINK->m2c1, &LINK->m2c2,
	    &LINK->s2, &LINK->gotten);
    /*


*/

    /*




*/

    LINK->waitfor1 = false;
    LINK->waitfor2 = false;
    return;
  }


  if (LINK->waitfor1) {
    markput(LINK->marksout, &LINK->m1, &LINK->m1c1, &LINK->m1c2, &LINK->s1);
    markget(LINK->marks1, LINK->marksout, &LINK->m1, &LINK->m1c1, &LINK->m1c2,
	    &LINK->s1, &LINK->gotten);
    if (LINK->m1c1 != 'p')
      return;
    fprintf(LINK->marksout->f, "p - in mark1 finally found\n");
    LINK->pieceisdone = true;
    fprintf(LINK->marksout->f, "* pieceisdone ----> waitfor1\n");
    markget(LINK->marks2, LINK->marksout, &LINK->m2, &LINK->m2c1, &LINK->m2c2,
	    &LINK->s2, &LINK->gotten);
    markget(LINK->marks1, LINK->marksout, &LINK->m1, &LINK->m1c1, &LINK->m1c2,
	    &LINK->s1, &LINK->gotten);
    LINK->waitfor1 = false;
    return;
  }

  if (LINK->waitfor2) {
    markput(LINK->marksout, &LINK->m2, &LINK->m2c1, &LINK->m2c2, &LINK->s2);
    markget(LINK->marks2, LINK->marksout, &LINK->m2, &LINK->m2c1, &LINK->m2c2,
	    &LINK->s2, &LINK->gotten);
    if (LINK->m2c1 != 'p')
      return;
    fprintf(LINK->marksout->f, "p - in mark2 finally found\n");
    LINK->pieceisdone = true;
    fprintf(LINK->marksout->f, "* pieceisdone ----> waitfor2\n");
    markget(LINK->marks1, LINK->marksout, &LINK->m1, &LINK->m1c1, &LINK->m1c2,
	    &LINK->s1, &LINK->gotten);
    markget(LINK->marks2, LINK->marksout, &LINK->m2, &LINK->m2c1, &LINK->m2c2,
	    &LINK->s2, &LINK->gotten);
    LINK->waitfor2 = false;
    return;
  }

  if (false) {
    printf("HUNH??  This case is supposed to be impossible!\n");
    halt();
    return;
  }
  /*




*/

  /*











*/

  if (LINK->m1 != bignumber)
    V.position1 = pietoint((long)floor(LINK->m1 + 0.5), LINK->pie);
  else
    V.position1 = -100;

  if (LINK->m2 != bignumber)
    V.position2 = pietoint((long)floor(LINK->m2 + 0.5), LINK->pie);
  else
    V.position2 = -200;
  if (V.position1 > 1000) {
    fprintf(LINK->marksout->f, "* position  = %ld\n", V.position);
    fprintf(LINK->marksout->f, "* position1 = %ld\n", V.position1);
    fprintf(LINK->marksout->f, "* position2 = %ld\n", V.position2);
    fprintf(LINK->marksout->f, "* number = %ld\n", number);
    fprintf(LINK->marksout->f, "* m1 = %1.1f\n", LINK->m1);
    fprintf(LINK->marksout->f, "* m2 = %1.1f\n", LINK->m2);
    halt();
  }
  /*

*/
  if (V.position1 != -100 || V.position2 != -200)
    doifchain(&V);

}

Local Void checkending(marks, marksout, m, c1, c2, s, gotten, number, LINK)
_TEXT *marks, *marksout;
double *m;
Char *c1, *c2;
stringDelila *s;
boolean *gotten;
long number;
struct LOC_themain *LINK;
{
  boolean warned = false;
  _TEXT TEMP;

  /*







*/
  if (BUFEOF(marks->f))
    return;
  while (!BUFEOF(marks->f)) {
    markget(marks, marksout, m, c1, c2, s, gotten);
    if (!*gotten)
      continue;
    if (!warned) {
      printf("\nWARNING: marks%ld is not at end-of-file when the book is finished!\n",
	     number);
      printf("The extra lines are:\n");
      warned = true;
    }
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, s);
    putchar('\n');
    markput(marksout, m, c1, c2, s);
  }
}



Static Void themain(book, marks1_, marks2_, mergemarksp, marksout_)
_TEXT *book, *marks1_, *marks2_, *mergemarksp, *marksout_;
{
  struct LOC_themain V;
  boolean done = false;
  boolean decreasing;
  double parameterversion;
  long theline;
  /*

*/


  orgkey org;
  boolean orgchange = false, orgopen = false;
  chrkey chr;
  boolean chrchange = false, chropen = false, piechange = false,
	  pieopen = false;
  long position;

  long piecelengthpie;
  piekey *WITH;

  V.marks1 = marks1_;
  V.marks2 = marks2_;
  V.marksout = marksout_;
  printf("mergemarks %4.2f\n", version);
  if (*mergemarksp->name != '\0') {
    if (mergemarksp->f != NULL)
      mergemarksp->f = freopen(mergemarksp->name, "r", mergemarksp->f);
    else
      mergemarksp->f = fopen(mergemarksp->name, "r");
  } else
    rewind(mergemarksp->f);
  if (mergemarksp->f == NULL)
    _EscIO2(FileNotFound, mergemarksp->name);
  RESETBUF(mergemarksp->f, Char);
  fscanf(mergemarksp->f, "%lg%*[^\n]", &parameterversion);
  getc(mergemarksp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  brinit(book, &theline);
  V.pie = NULL;

  if (*V.marks1->name != '\0') {
    if (V.marks1->f != NULL)
      V.marks1->f = freopen(V.marks1->name, "r", V.marks1->f);
    else
      V.marks1->f = fopen(V.marks1->name, "r");
  } else
    rewind(V.marks1->f);
  if (V.marks1->f == NULL)
    _EscIO2(FileNotFound, V.marks1->name);
  RESETBUF(V.marks1->f, Char);
  if (*V.marks2->name != '\0') {
    if (V.marks2->f != NULL)
      V.marks2->f = freopen(V.marks2->name, "r", V.marks2->f);
    else
      V.marks2->f = fopen(V.marks2->name, "r");
  } else
    rewind(V.marks2->f);
  if (V.marks2->f == NULL)
    _EscIO2(FileNotFound, V.marks2->name);
  RESETBUF(V.marks2->f, Char);
  if (*V.marksout->name != '\0') {
    if (V.marksout->f != NULL)
      V.marksout->f = freopen(V.marksout->name, "w", V.marksout->f);
    else
      V.marksout->f = fopen(V.marksout->name, "w");
  } else {
    if (V.marksout->f != NULL)
      rewind(V.marksout->f);
    else
      V.marksout->f = tmpfile();
  }
  if (V.marksout->f == NULL)
    _EscIO2(FileNotFound, V.marksout->name);
  SETUPBUF(V.marksout->f, Char);
  fprintf(V.marksout->f, "* mergemarks %4.2f\n", version);
  V.m1 = bignumber;
  V.m1c1 = ' ';
  V.m1c2 = ' ';
  V.m2 = bignumber;
  V.m2c1 = ' ';
  V.m2c2 = ' ';
  clearstring(&V.s1);
  clearstring(&V.s2);

  V.waitfor1 = false;
  V.waitfor2 = false;

  markget(V.marks1, V.marksout, &V.m1, &V.m1c1, &V.m1c2, &V.s1, &V.gotten);
  if (V.m1 == bignumber)
    printf("No marks to use in marks1\n");

  else
    printf("Found the first mark at %10.1f in marks1, type: %c%c\n",
	   V.m1, V.m1c1, V.m1c2);
  markget(V.marks2, V.marksout, &V.m2, &V.m2c1, &V.m2c2, &V.s2, &V.gotten);
  if (V.m2 == bignumber) {
    printf("No marks to use in marks2\n");
    /*

*/
  }

  else {
    printf("Found the first mark at %10.1f in marks2, type: %c%c\n",
	   V.m2, V.m2c1, V.m2c2);



  }
  while (!done) {
    getocp(book, &theline, &org, &orgchange, &orgopen, &chr, &chrchange,
	   &chropen, &V.pie, &piechange, &pieopen);

    WITH = &V.pie->key;

    decreasing = (WITH->piedir != WITH->coodir);
    if (!pieopen) {
      done = true;
      break;
    }
    fprintf(V.marksout->f,
	    "* piece %ld ------------------------------------\n", number);
    position = 1;
    V.pieceisdone = false;
    piecelengthpie = piecelength(V.pie);
    while (position <= piecelengthpie && !V.pieceisdone) {
      markcheck(position, decreasing, &V);
      position++;
    }
    putc('\n', V.marksout->f);



  }


  checkending(V.marks1, V.marksout, &V.m1, &V.m1c1, &V.m1c2, &V.s1, &V.gotten,
	      1L, &V);
  checkending(V.marks2, V.marksout, &V.m2, &V.m2c1, &V.m2c2, &V.s2, &V.gotten,
	      2L, &V);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  marksout.f = NULL;
  strcpy(marksout.name, "marksout");
  mergemarksp.f = NULL;
  strcpy(mergemarksp.name, "mergemarksp");
  marks2.f = NULL;
  strcpy(marks2.name, "marks2");
  marks1.f = NULL;
  strcpy(marks1.name, "marks1");
  book.f = NULL;
  strcpy(book.name, "book");
  themain(&book, &marks1, &marks2, &mergemarksp, &marksout);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (marks1.f != NULL)
    fclose(marks1.f);
  if (marks2.f != NULL)
    fclose(marks2.f);
  if (mergemarksp.f != NULL)
    fclose(mergemarksp.f);
  if (marksout.f != NULL)
    fclose(marksout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
