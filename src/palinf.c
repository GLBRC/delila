/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "palinf.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         2.37
/*









*/



/*










































































































































































































*/



#define fearraymax      100
/*
*/




#define dnamax          1024
#define namelength      100
#define linelength      200





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

/* p2c: palinf.p, line 284:
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



Static _TEXT palinfp, book, fout, palinfeatures;






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



Static Void missparam(param)
_TEXT *param;
{
  /*

*/
  if (BUFEOF(param->f)) {
    printf(" missing parameter\n");
    halt();
  }
}


Static Void initialize(palinfp, book, fout, palinfeatures, sig, boundary,
		       mmode, xmode, apiece, theline)
_TEXT *palinfp, *book, *fout, *palinfeatures;
double *sig;
long *boundary;
boolean *mmode, *xmode;
piece **apiece;
long theline;
{
  long maxsize;

  printf("palinf %4.2f\n", version);
  theline = 0;

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
  fprintf(fout->f,
	  "palinf %4.2f palindrome search based on information theory\n",
	  version);
  brinit(book, &theline);
  putc(' ', fout->f);
  copyaline(book, fout);
  putc('\n', fout->f);

  if (*palinfeatures->name != '\0') {
    if (palinfeatures->f != NULL)
      palinfeatures->f = freopen(palinfeatures->name, "w", palinfeatures->f);
    else
      palinfeatures->f = fopen(palinfeatures->name, "w");
  } else {
    if (palinfeatures->f != NULL)
      rewind(palinfeatures->f);
    else
      palinfeatures->f = tmpfile();
  }
  if (palinfeatures->f == NULL)
    _EscIO2(FileNotFound, palinfeatures->name);
  SETUPBUF(palinfeatures->f, Char);
  fprintf(palinfeatures->f,
	  "* palinf %4.2f palindrome search based on information theory\n",
	  version);
  brinit(book, &theline);
  copyaline(book, palinfeatures);


  if (*palinfp->name != '\0') {
    if (palinfp->f != NULL)
      palinfp->f = freopen(palinfp->name, "r", palinfp->f);
    else
      palinfp->f = fopen(palinfp->name, "r");
  } else
    rewind(palinfp->f);
  if (palinfp->f == NULL)
    _EscIO2(FileNotFound, palinfp->name);
  RESETBUF(palinfp->f, Char);
  missparam(palinfp);
  fscanf(palinfp->f, "%lg%*[^\n]", sig);
  getc(palinfp->f);

  if (BUFEOF(palinfp->f))
    *boundary = LONG_MAX;
  else {
    fscanf(palinfp->f, "%ld%*[^\n]", &maxsize);
    getc(palinfp->f);

    if (maxsize < 1) {
      printf("Maximum size must be positive\n");
      halt();
    }

    switch (maxsize & 1) {

    case 0:
      *boundary = (long)floor(maxsize / 2.0 + 0.5);
      break;

    case 1:
      *boundary = (long)floor((maxsize - 1.0) / 2 + 0.5);
      break;
    }

    *mmode = false;
    *xmode = false;
    if (!BUFEOF(palinfp->f)) {
      if (P_peek(palinfp->f) == 'm')
	*mmode = true;
      else if (P_peek(palinfp->f) == 'x')
	*xmode = true;
    }
  }


  if (*sig >= 0.0)
    fprintf(fout->f, " %8.2f bits is the lowest rsequence detected\n", *sig);
  else {
    fprintf(fout->f, " %8.5f palindromes of this significance\n", *sig);
    fprintf(fout->f, "    (in standard deviations) or more are found\n");
  }
  fprintf(fout->f, " %8ld largest palindrome found (bases)\n",
	  *boundary * 2 + 1);

  if (!(*mmode || *xmode))
    fprintf(fout->f, " not in a graphic mode.\n");
  else if (*mmode)
    fprintf(fout->f,
	    " m versus rsequence is plotted for each detected palindrome\n");
  else if (*xmode)
    fprintf(fout->f, " x versus rsequence is plotted for all positions (x)\n");
  putc('\n', fout->f);

  if (*mmode)
    fprintf(fout->f,
	    " m is the distance from the center of the odd palindrome.\n\n");

  if (*mmode || *xmode) {
    fprintf(fout->f,
      " the numbers on the plot are standard deviations of e(hnb) for\n");
    fprintf(fout->f, " comparing the palindrome to expected values.\n\n");
  }

  fprintf(fout->f, " warning:\n");
  fprintf(fout->f, " the information content is found by summing from\n");
  fprintf(fout->f, " the center to the edges of the palindrome.\n");
  fprintf(fout->f, " since the highest sum found is used for detection,\n");
  fprintf(fout->f, " palindromes may appear more significant than\n");
  fprintf(fout->f, " they really are.\n\n");

  fprintf(fout->f, " an odd palindrome is centered on the base x;\n");
  fprintf(fout->f,
    " while an even one is centered just before the base x, between bases.\n");

  *apiece = (piece *)Malloc(sizeof(piece));
}


Static Void showpiece(fout, apiece)
_TEXT *fout;
piece *apiece;
{
  name *WITH;
  line *WITH1;

  putc('\n', fout->f);
  if (number > 0)
    fprintf(fout->f, " piece number: %ld", number);

  WITH = &apiece->key.hea.keynam;
/* p2c: palinf.p, line 1256: Note:
 * Format for packed-array-of-char will work only if width < length [321] */

  fprintf(fout->f, "  name: %.*s", WITH->length, WITH->letters);
  WITH1 = apiece->key.hea.fulnam;
/* p2c: palinf.p, line 1259: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  fprintf(fout->f, "  full name: %.*s\n", WITH1->length, WITH1->letters);
}


#define infofield       4
#define infodecim       1
#define defmaxr         30


/* Local variables for palsearch: */
struct LOC_palsearch {
  _TEXT *fout;
  piece *pie;
  long boundary;
  double varhnb, equal, unequal, halfunequal, reven, rodd, maxreven, maxrodd,
	 minrs;
  long maxr, n, x, m;
  /*

*/
  long xm;
  base b1, b2, b3;
  /*

*/
  long twomaxr;

  long evenmatch[fearraymax];
  /*
*/
  long evennumber;

  long oddmatch[fearraymax];
  /*
*/
  long oddnumber;
} ;

Local Void graph10s(fout, maxr, LINK)
_TEXT *fout;
long maxr;
struct LOC_palsearch *LINK;
{
  long r;

  for (r = 1; r <= maxr / 10; r++)
    fprintf(fout->f, "%19c%ld", ' ', r);
  putc('\n', fout->f);
}

Local Void graph1s(fout, maxr, LINK)
_TEXT *fout;
long maxr;
struct LOC_palsearch *LINK;
{
  /*
*/
  long r;

  fprintf(fout->f, "<0");
  for (r = 1; r <= maxr; r++)
    fprintf(fout->f, ".%ld", r % 10);
  putc('\n', fout->f);
}

Local Void graphline(fout, reven, rodd, stdev, twomaxr, LINK)
_TEXT *fout;
double reven, rodd, stdev;
long twomaxr;
struct LOC_palsearch *LINK;
{
  /*

*/
  long r, tworeven, tworodd;
  double twostdev;

  if (reven < 0 || rodd < 0) {
    if (reven < 0 && rodd < 0)
      putc('=', fout->f);
    else if (rodd < 0)
      putc('o', fout->f);
    else
      putc('e', fout->f);
  } else
    putc(' ', fout->f);

  tworeven = (long)floor(2 * reven + 0.5);
  if (tworeven > twomaxr)
    tworeven = twomaxr;
  tworodd = (long)floor(2 * (rodd - LINK->halfunequal) + 0.5);
  if (tworodd > twomaxr)
    tworodd = twomaxr;

  twostdev = (long)floor(2 * stdev + 0.5);

  for (r = 0; r <= twomaxr; r++) {
    if (tworeven == r && tworodd == r)
      putc('=', fout->f);
    else if (tworeven == r)
      putc('e', fout->f);
    else if (tworodd == r)
      putc('o', fout->f);
    else if (twostdev == r)
      putc('1', fout->f);
    else if (2 * twostdev == r)
      putc('2', fout->f);
    else if (3 * twostdev == r)
      putc('3', fout->f);
    else if (4 * twostdev == r)
      putc('4', fout->f);
    else if (5 * twostdev == r)
      putc('5', fout->f);
    else if (((long)floor(r / 2.0 + 0.5) % 10 == 0 ||
	      (long)floor(r / 2.0 + 0.5) % 5 == 0) && !(r & 1))
      putc('.', fout->f);
    else
      putc(' ', fout->f);
  }
  putc('\n', fout->f);
}

Local Void space(f, s, LINK)
_TEXT *f;
long s;
struct LOC_palsearch *LINK;
{
  /*
*/
  long count;

  if (s > 0) {
    for (count = 1; count <= s; count++)
      putc(' ', f->f);
  }
}

Local Void mrgraphheader(LINK)
struct LOC_palsearch *LINK;
{
  fprintf(LINK->fout->f, " at position %5ld\n", inttopie(LINK->x, LINK->pie));


  space(LINK->fout, infofield * 2L + 8, LINK);


  graph10s(LINK->fout, LINK->maxr, LINK);


  space(LINK->fout, 3L, LINK);
  putc('m', LINK->fout->f);
  space(LINK->fout, infofield - 3L, LINK);
  fprintf(LINK->fout->f, "even");
  space(LINK->fout, infofield - 2L, LINK);
  fprintf(LINK->fout->f, "odd");


  graph1s(LINK->fout, LINK->maxr, LINK);
}

Local Void mrgraphic(LINK)
struct LOC_palsearch *LINK;
{
  fprintf(LINK->fout->f, " %3ld", LINK->m);
  fprintf(LINK->fout->f, " %*.*f", infofield, infodecim, LINK->reven);
  fprintf(LINK->fout->f, " %*.*f", infofield, infodecim, LINK->rodd);
  graphline(LINK->fout, LINK->reven, LINK->rodd,
	    2 * sqrt(LINK->varhnb * LINK->m), LINK->twomaxr, LINK);
}

Local Void xrgraphheader(fout, maxr, LINK)
_TEXT *fout;
long maxr;
struct LOC_palsearch *LINK;
{
  space(fout, infofield * 2L + 10, LINK);


  graph10s(fout, maxr, LINK);


  space(fout, 4L, LINK);
  putc('x', fout->f);
  fprintf(fout->f, " bp");
  space(fout, infofield - 3L, LINK);
  fprintf(fout->f, "even");
  space(fout, infofield - 2L, LINK);
  fprintf(fout->f, "odd");


  graph1s(fout, maxr, LINK);
}

Local Void xrgraphic(fout, x, b, maxreven, maxrodd, twomaxr, LINK)
_TEXT *fout;
long x;
Char b;
double maxreven, maxrodd;
long twomaxr;
struct LOC_palsearch *LINK;
{
  /*



*/
  fprintf(fout->f, " %5ld", inttopie(x, LINK->pie));
  fprintf(fout->f, " %c", b);
  fprintf(fout->f, " %*.*f", infofield, infodecim, maxreven);
  fprintf(fout->f, " %*.*f", infofield, infodecim, maxrodd);
  graphline(fout, maxreven, maxrodd, 2 * sqrt(LINK->varhnb * LINK->m),
	    twomaxr, LINK);
}

Local Void maximize(r, maxr, LINK)
double r, *maxr;
struct LOC_palsearch *LINK;
{
  if (r > *maxr)
    *maxr = r;
}

Local Void match(evenodd, r, left, right, LINK)
Char evenodd;
double *r;
base left, right;
struct LOC_palsearch *LINK;
{
  /*

*/
  if (left != complement(right)) {
    *r += LINK->unequal;
    return;
  }
  *r += LINK->equal;


  switch (evenodd) {

  case 'e':
    LINK->evennumber++;
    if (LINK->evennumber > fearraymax) {
      printf("An even palendrome has more than %ld matches.\n",
	     (long)fearraymax);
      printf("increase constant fearraymax\n");
      halt();
    }
    LINK->evenmatch[LINK->evennumber-1] = LINK->m;
    break;

  case 'o':
    LINK->oddnumber++;
    if (LINK->oddnumber > fearraymax) {
      printf("An odd palendrome has more than %ld matches.\n",
	     (long)fearraymax);
      printf("increase constant fearraymax\n");
      halt();
    }
    LINK->oddmatch[LINK->oddnumber-1] = LINK->m;
    break;
  }
}

Local boolean within(x, m, n, LINK)
long x, m, n;
struct LOC_palsearch *LINK;
{
  return (x - m >= 1 && x + m <= n);
}

Local Void dolinear(showit, LINK)
boolean showit;
struct LOC_palsearch *LINK;
{
  while (within(LINK->x, LINK->m, LINK->n, LINK) && LINK->m <= LINK->boundary) {
    LINK->xm = LINK->x + LINK->m;

    LINK->b1 = getbase(LINK->x - LINK->m, LINK->pie);
    LINK->b2 = getbase(LINK->xm - 1, LINK->pie);
    LINK->b3 = getbase(LINK->xm, LINK->pie);

    match('e', &LINK->reven, LINK->b1, LINK->b2, LINK);
    match('o', &LINK->rodd, LINK->b1, LINK->b3, LINK);
    maximize(LINK->reven, &LINK->maxreven, LINK);
    maximize(LINK->rodd, &LINK->maxrodd, LINK);
    if (showit)
      mrgraphic(LINK);
    LINK->m++;
  }
}

Local long wrap(p, n, LINK)
long p, n;
struct LOC_palsearch *LINK;
{
  /*
*/
  if (p < 1)
    return (n + p);
  else if (p > n) {
    return ((p - 1) % n + 1);
/* p2c: palinf.p, line 1522:
 * Note: Using % for possibly-negative arguments [317] */
  } else
    return p;
}

Local Void docircular(showit, LINK)
boolean showit;
struct LOC_palsearch *LINK;
{
  while (LINK->m <= LINK->boundary) {
    LINK->xm = LINK->x + LINK->m;

    LINK->b1 = getbase(wrap(LINK->x - LINK->m, LINK->n, LINK), LINK->pie);
    LINK->b2 = getbase(wrap(LINK->xm - 1, LINK->n, LINK), LINK->pie);
    LINK->b3 = getbase(wrap(LINK->xm, LINK->n, LINK), LINK->pie);

    match('e', &LINK->reven, LINK->b1, LINK->b2, LINK);
    match('o', &LINK->rodd, LINK->b1, LINK->b3, LINK);
    maximize(LINK->reven, &LINK->maxreven, LINK);
    maximize(LINK->rodd, &LINK->maxrodd, LINK);
    if (showit)
      mrgraphic(LINK);
    LINK->m++;
  }
}

Local Void search(showit, LINK)
boolean showit;
struct LOC_palsearch *LINK;
{
  /*
*/
  LINK->reven = 0.0;
  LINK->rodd = LINK->halfunequal;
  /*

*/
  LINK->maxreven = -LONG_MAX;
  /*
*/
  LINK->maxrodd = -LONG_MAX;
  LINK->m = 1;
  LINK->evennumber = 0;
  LINK->oddnumber = 0;

  if (showit)
    mrgraphheader(LINK);


  switch (LINK->pie->key.piecon) {

  case linear:
    dolinear(showit, LINK);
    break;

  case circular:
    docircular(showit, LINK);
    break;
  }
}


Local Void recordpalinfeatures(pf, apiece, LINK)
_TEXT *pf;
piece *apiece;
struct LOC_palsearch *LINK;
{
  long mindex, rpt;
  name *WITH;
  long FORLIM, FORLIM1;

  /*














*/

  if (LINK->maxreven >= LINK->minrs) {
    fprintf(pf->f, "define \"even%ld.", inttopie(LINK->x, LINK->pie));
    WITH = &apiece->key.hea.keynam;
/* p2c: palinf.p, line 1614: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
    fprintf(pf->f, "%.*s", WITH->length, WITH->letters);
    fprintf(pf->f, "\" \"-\"");
    for (rpt = 1; rpt <= 2; rpt++) {
      fprintf(pf->f, " \"");
      FORLIM1 = LINK->evennumber;
      for (mindex = 1; mindex <= FORLIM1; mindex++)
	putc('(', pf->f);
      FORLIM1 = LINK->evennumber;
      for (mindex = 1; mindex <= FORLIM1; mindex++)
	putc(')', pf->f);
      putc('"', pf->f);
    }
    for (mindex = LINK->evennumber - 1; mindex >= 0; mindex--)
      fprintf(pf->f, " %ld", -LINK->evenmatch[mindex]);

    FORLIM1 = LINK->evennumber;
    for (mindex = 0; mindex < FORLIM1; mindex++)
      fprintf(pf->f, " %ld", LINK->evenmatch[mindex] - 1);
    fprintf(pf->f, "\n@ ");


    WITH = &apiece->key.hea.keynam;
/* p2c: palinf.p, line 1630: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
    fprintf(pf->f, "%.*s", WITH->length, WITH->letters);
    fprintf(pf->f, " %ld", inttopie(LINK->x, LINK->pie));
    /*




*/
    fprintf(pf->f, " +1 \"even%ld.", inttopie(LINK->x, LINK->pie));
    WITH = &apiece->key.hea.keynam;
/* p2c: palinf.p, line 1640: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
    fprintf(pf->f, "%.*s", WITH->length, WITH->letters);
    fprintf(pf->f, "\" \"%*.*f bits\"\n",
	    infofield, infodecim, LINK->maxreven);
  }
  /*







*/



  /*



*/

  if (LINK->maxrodd < LINK->minrs)
    return;

  fprintf(pf->f, "\ndefine \"odd%ld.", inttopie(LINK->x, LINK->pie));
  /*






*/


  /*



*/
  WITH = &apiece->key.hea.keynam;
/* p2c: palinf.p, line 1664: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  fprintf(pf->f, "%.*s", WITH->length, WITH->letters);
  fprintf(pf->f, "\" \"-\"");
  for (rpt = 1; rpt <= 2; rpt++) {
    fprintf(pf->f, " \"");
    FORLIM = LINK->oddnumber;
    for (mindex = 1; mindex <= FORLIM; mindex++)
      putc('(', pf->f);
    putc('|', pf->f);
    FORLIM = LINK->oddnumber;
    for (mindex = 1; mindex <= FORLIM; mindex++)
      putc(')', pf->f);
    putc('"', pf->f);
  }

  for (mindex = LINK->oddnumber - 1; mindex >= 0; mindex--)
    fprintf(pf->f, " %ld", -LINK->oddmatch[mindex]);
  fprintf(pf->f, " %d", 0);
  FORLIM1 = LINK->oddnumber;
  for (mindex = 0; mindex < FORLIM1; mindex++)
    fprintf(pf->f, " %ld", LINK->oddmatch[mindex]);
  fprintf(pf->f, "\n@ ");


  WITH = &apiece->key.hea.keynam;
/* p2c: palinf.p, line 1682: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  fprintf(pf->f, "%.*s", WITH->length, WITH->letters);
  fprintf(pf->f, " %ld", inttopie(LINK->x, LINK->pie));
  /*




*/
  fprintf(pf->f, " +1 \"odd%ld.", inttopie(LINK->x, LINK->pie));
  WITH = &apiece->key.hea.keynam;
/* p2c: palinf.p, line 1692: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  fprintf(pf->f, "%.*s", WITH->length, WITH->letters);
  fprintf(pf->f, "\" \"%*.*f bits\"\n", infofield, infodecim, LINK->maxrodd);

}


Static Void palsearch(fout_, palinfeatures, pie_, sig, boundary_, mmode,
		      xmode)
_TEXT *fout_, *palinfeatures;
piece *pie_;
double sig;
long boundary_;
boolean mmode, xmode;
{
  /*
*/
  struct LOC_palsearch V;
  double ehnb = 0.75;
  boolean graphics;
  long FORLIM;

  V.fout = fout_;
  V.pie = pie_;
  V.boundary = boundary_;
  V.varhnb = 0.1875;

  /*


*/
  V.equal = 2 * ehnb;
  V.unequal = 2 * (ehnb - 1.0);
  V.halfunequal = 0.0;
  /*

*/

  if (sig >= 0) {
    V.minrs = sig;
    fprintf(V.fout->f,
      " minimum rsequence = %*.*f the minimum information content to display\n",
      infofield, infodecim, V.minrs);
  } else
    V.minrs = LONG_MAX;

  putc('\n', V.fout->f);
  graphics = (mmode || xmode);
  if (!graphics)
    fprintf(V.fout->f, "%*ceven%*codd  palindromes\n",
	    (int)(infofield + 7), ' ', (int)(infofield + 2), ' ');

  V.n = piecelength(V.pie);


  if (xmode) {
    V.maxr = defmaxr - 2;
    /*

*/
    xrgraphheader(V.fout, V.maxr, &V);
  } else
    V.maxr = defmaxr;
  V.twomaxr = V.maxr * 2;

  FORLIM = V.n;
  /*
*/
  for (V.x = 2; V.x < FORLIM; V.x++) {
    search(false, &V);


    if (xmode)
      xrgraphic(V.fout, V.x, basetochar(getbase(V.x, V.pie)), V.maxreven,
		V.maxrodd, V.twomaxr, &V);
    else {
      if (sig < 0)
	V.minrs = sig * -2 * sqrt(V.varhnb * V.m);
      /*
*/

      if (V.maxreven >= V.minrs || V.maxrodd >= V.minrs) {
	if (mmode)
	  search(true, &V);

	fprintf(V.fout->f, " at %5ld ", inttopie(V.x, V.pie));

	if (V.maxreven >= V.minrs)
	  fprintf(V.fout->f, "%*.*f bits", infofield, infodecim, V.maxreven);
	else
	  fprintf(V.fout->f, "%*c", (int)(infofield + 5), ' ');

	if (V.maxrodd >= V.minrs)
	  fprintf(V.fout->f, " %*.*f bits", infofield, infodecim, V.maxrodd);
	if (mmode)
	  putc('\n', V.fout->f);
	putc('\n', V.fout->f);

	recordpalinfeatures(palinfeatures, V.pie, &V);

      }


    }
  }
}

#undef infofield
#undef infodecim
#undef defmaxr


Static Void themain(palinfp, book, fout, palinfeatures)
_TEXT *palinfp, *book, *fout, *palinfeatures;
{
  /*
*/
  /*

*/
  double sig;
  long boundary;
  /*
*/
  boolean mmode, xmode;
  piece *apiece;
  long theline;

  initialize(palinfp, book, fout, palinfeatures, &sig, &boundary, &mmode,
	     &xmode, &apiece, theline);


  while (!BUFEOF(book->f)) {
    getpiece(book, &theline, &apiece);
    if (BUFEOF(book->f))
      break;
    showpiece(fout, apiece);
    palsearch(fout, palinfeatures, apiece, sig, boundary, mmode, xmode);
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
  palinfeatures.f = NULL;
  strcpy(palinfeatures.name, "palinfeatures");
  fout.f = NULL;
  strcpy(fout.name, "fout");
  book.f = NULL;
  strcpy(book.name, "book");
  palinfp.f = NULL;
  strcpy(palinfp.name, "palinfp");
  themain(&palinfp, &book, &fout, &palinfeatures);
_L1:
  if (palinfp.f != NULL)
    fclose(palinfp.f);
  if (book.f != NULL)
    fclose(book.f);
  if (fout.f != NULL)
    fclose(fout.f);
  if (palinfeatures.f != NULL)
    fclose(palinfeatures.f);
  exit(EXIT_SUCCESS);
}



/* End. */
