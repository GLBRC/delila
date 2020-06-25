/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "curvafea.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.06
/*







*/
#define updateversion   1.00



/*






































































































































































*/



#define shiftdown       (-0.350)
/*
*/
#define saturation      0.5




#define dnamax          1024
#define namelength      100
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

/* p2c: curvafea.p, line 256:
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



typedef struct parameters {

  double parameterversion;
  long firstbase, lastbase, zerobase;
  double wavelength, wavevertical, waveheight;
  Char basephase;
  /*
*/
} parameters;


Static _TEXT book, curvafeap, curvaout, curvafeamarks, curvaxyin;






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





Static Void readparameters(curvafeap, p)
_TEXT *curvafeap;
parameters *p;
{
  if (*curvafeap->name != '\0') {
    if (curvafeap->f != NULL)
      curvafeap->f = freopen(curvafeap->name, "r", curvafeap->f);
    else
      curvafeap->f = fopen(curvafeap->name, "r");
  } else
    rewind(curvafeap->f);
  if (curvafeap->f == NULL)
    _EscIO2(FileNotFound, curvafeap->name);
  RESETBUF(curvafeap->f, Char);
  fscanf(curvafeap->f, "%lg%*[^\n]", &p->parameterversion);
  getc(curvafeap->f);
  if (p->parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if ((P_peek(curvafeap->f) == 'f') | (P_peek(curvafeap->f) == 'l')) {
    fscanf(curvafeap->f, "%c%ld%*[^\n]", &p->basephase, &p->zerobase);
    getc(curvafeap->f);
    if (p->basephase == '\n')
      p->basephase = ' ';
  } else {
    p->basephase = ' ';
    fscanf(curvafeap->f, "%ld%*[^\n]", &p->zerobase);
    getc(curvafeap->f);
  }

  fscanf(curvafeap->f, "%lg%*[^\n]", &p->wavelength);
  getc(curvafeap->f);
  fscanf(curvafeap->f, "%lg%*[^\n]", &p->wavevertical);
  getc(curvafeap->f);
  fscanf(curvafeap->f, "%lg%*[^\n]", &p->waveheight);
  getc(curvafeap->f);





  if (p->wavelength <= 0) {
    printf("wavelength must be positive\n");
    halt();
  }

  if (p->waveheight <= 0) {
    printf("waveheight must be positive\n");
    halt();
  }

}


#define wid             10
#define dec             5


Static Void writeparameters(f, p)
_TEXT *f;
parameters *p;
{
  fprintf(f->f, "* %*.2f parameterverion\n", wid, p->parameterversion);
  fprintf(f->f, "* %*ld firstbase\n", wid, p->firstbase);
  fprintf(f->f, "* %*ld lastbase\n", wid, p->lastbase);
  /*

*/
  switch (p->basephase) {

  case 'f':
    fprintf(f->f, "* first base + %*ld is zerobase\n", wid, p->zerobase);
    break;

  case 'l':
    fprintf(f->f, "* last base + %*ld is zerobase\n", wid, p->zerobase);
    break;

  case ' ':
    fprintf(f->f, "* %*ld zerobase\n", wid, p->zerobase);
    break;
  }
  fprintf(f->f, "* %*.*f wavelength\n", wid, dec, p->wavelength);
  fprintf(f->f, "* %*.*f wavevertical\n", wid, dec, p->wavevertical);
  fprintf(f->f, "* %*.*f waveheight\n", wid, dec, p->waveheight);
}

#undef wid
#undef dec


Static Void makecurvafeamarks(curvaout, f, p)
_TEXT *curvaout, *f;
parameters p;
{
  if (*f->name != '\0') {
    if (f->f != NULL)
      f->f = freopen(f->name, "w", f->f);
    else
      f->f = fopen(f->name, "w");
  } else {
    if (f->f != NULL)
      rewind(f->f);
    else
      f->f = tmpfile();
  }
  if (f->f == NULL)
    _EscIO2(FileNotFound, f->name);
  SETUPBUF(f->f, Char);
  fprintf(f->f, "* curvafea %4.2f, create curve features\n\n", version);
  writeparameters(f, &p);

  fprintf(f->f, "\nu\n");
  fprintf(f->f, "%% fs is the font size vertically\n");
  fprintf(f->f,
    "%% charwidth is assumed to exist (it does in makelogo and lister)\n");
  fprintf(f->f, "%% as the width of the characters horizontally.\n");
  fprintf(f->f, "%% define font size as a unit of measure:\n");
  fprintf(f->f, "/fs {fontsize mul} def\n\n");

  /*




















*/

  fprintf(f->f, "/wavebox { %% TailX TailY HeadX HeadY wbcolor wavebox -\n");
  fprintf(f->f,
	  "%% make a box for the wave n charcters vertically from zero\n");
  fprintf(f->f, "/wbcolor exch def\n");
  fprintf(f->f, "/HeadY exch def\n");
  fprintf(f->f, "/HeadX exch def\n");
  fprintf(f->f, "/TailY exch def\n");
  fprintf(f->f, "/TailX exch def\n");
  fprintf(f->f, "gsave\n");
  fprintf(f->f, "%% shift left 1 character:\n");
  fprintf(f->f, "charwidth neg 0 translate\n");

  fprintf(f->f, "TailX TailY moveto\n");
  fprintf(f->f, "HeadX TailY lineto\n");
  fprintf(f->f, "HeadX HeadY lineto\n");
  fprintf(f->f, "TailX HeadY lineto\n");
  fprintf(f->f, "TailX TailY lineto\n");
  fprintf(f->f, "wbcolor %5.3f 1 sethsbcolor\n", saturation);
  fprintf(f->f, "fill\n");
  fprintf(f->f, "grestore\n");
  /*


*/
  fprintf(f->f, "} def\n");
  fprintf(f->f, "!\n\n");

}


Static Void markat(curvafeamarks, x, y, waveheight, color)
_TEXT *curvafeamarks;
double x, y, waveheight, color;
{
  /*





*/
  /*


*/
  fprintf(curvafeamarks->f, "U %10.5f %10.5f %10.5f %10.5f %10.5f wavebox\n",
	  x, y, x + 1, y + waveheight, color);
}


Static double setcolor(thezero, internal, wavelength, huge_)
long thezero, internal;
double wavelength;
long huge_;
{
  /*













*/
  double thecolor, x, y;

  x = (internal - thezero) / wavelength;
  y = x + huge_ - (long)(x + huge_);

  thecolor = 0.85 * y;
  return thecolor;

  /*


*/
  /*



*/
  /*





*/

}


Static Void nobookloop(thezero, cooexternal, p, huge_)
long thezero, cooexternal;
parameters p;
long huge_;
{
  double thecolor;

  thecolor = setcolor(thezero, cooexternal, p.wavelength, huge_);

  markat(&curvafeamarks, (double)cooexternal, shiftdown + p.wavevertical,
	 p.waveheight, thecolor);
}


#define wid             10
#define dec             5

#define saturation_     1.0
#define brightness      1.0


Static Void bookloop(thezero, internal, pie, p, huge_, cinternal, curvaout,
		     curvaxyin)
long thezero, internal;
piece *pie;
parameters p;
long huge_, *cinternal;
_TEXT *curvaout, *curvaxyin;
{
  /*

*/
  double hue, curvature;
  long piecoordinate, sign;
  piekey *WITH;

  WITH = &pie->key;
  /*


*/


  if (WITH->piedir == WITH->coodir)
    sign = 1;
  else
    sign = -1;
  /*

*/

  if (!((*cinternal <= internal) & (!BUFEOF(curvaout->f))))
    return;
  if (*cinternal < internal) {
    if (P_eoln(curvaout->f)) {
      fscanf(curvaout->f, "%*[^\n]");
      getc(curvaout->f);
    } else if (P_peek(curvaout->f) == '*') {
      fscanf(curvaout->f, "%*[^\n]");
      getc(curvaout->f);
    } else {
      fscanf(curvaout->f, "%ld%lg%*[^\n]", cinternal, &curvature);
      getc(curvaout->f);
    }
  }

  /*


*/

  /*




*/

  if (internal != *cinternal)
    return;
  /*


*/


  if (curvature <= 1)
    hue = 0.84 * curvature + 0.16;
  else
    hue = 1.0;

  piecoordinate = inttopie(internal, pie);
  markat(&curvafeamarks, (double)piecoordinate, shiftdown + p.wavevertical,
	 p.waveheight, hue);

  fprintf(curvaxyin->f, "%*ld %*.*f %*.*f %*.*f %*.*f %*ld\n",
	  wid, internal, wid, dec, curvature, wid, dec, hue, wid, dec,
	  saturation_, wid, dec, brightness, wid, sign * piecoordinate);

  /*
*/

  /*

*/
  /*

*/
}

#undef wid
#undef dec
#undef saturation_
#undef brightness



Static Void themain(book, curvafeap, curvaout, curvafeamarks, curvaxyin)
_TEXT *book, *curvafeap, *curvaout, *curvafeamarks, *curvaxyin;
{
  long internal, inthezero, l;
  parameters p;
  long thezero, huge_;


  long theline;
  orgkey org;
  boolean orgchange = false, orgopen = false;
  chrkey chr;
  boolean chrchange = false, chropen = false;
  piece *pie = NULL;
  boolean piechange = false, pieopen = false;

  long cinternal = 0;
  long FORLIM;

  /*
*/

  printf("curvafea %4.2f\n", version);
  readparameters(curvafeap, &p);
  makecurvafeamarks(curvaout, curvafeamarks, p);

  if (*curvaout->name != '\0') {
    if (curvaout->f != NULL)
      curvaout->f = freopen(curvaout->name, "r", curvaout->f);
    else
      curvaout->f = fopen(curvaout->name, "r");
  } else
    rewind(curvaout->f);
  if (curvaout->f == NULL)
    _EscIO2(FileNotFound, curvaout->name);
  RESETBUF(curvaout->f, Char);
  if (*curvaxyin->name != '\0') {
    if (curvaxyin->f != NULL)
      curvaxyin->f = freopen(curvaxyin->name, "w", curvaxyin->f);
    else
      curvaxyin->f = fopen(curvaxyin->name, "w");
  } else {
    if (curvaxyin->f != NULL) {

      rewind(curvaxyin->f);
    } else
      curvaxyin->f = tmpfile();
  }
  if (curvaxyin->f == NULL)
    _EscIO2(FileNotFound, curvaxyin->name);
  SETUPBUF(curvaxyin->f, Char);
  fprintf(curvaxyin->f, "* curvafea %4.2f\n", version);
  fprintf(curvaxyin->f, "* columns:\n");
  fprintf(curvaxyin->f, "* 1 internal coordinate\n");
  fprintf(curvaxyin->f,
	  "* 2 curvature units (relative to nucleosome bending)\n");
  fprintf(curvaxyin->f, "* 3 hue for color\n");
  fprintf(curvaxyin->f, "* 4 saturation for color\n");
  fprintf(curvaxyin->f, "* 5 brightness for color\n");
  fprintf(curvaxyin->f, "* 6 piecoordinate: coordinate on the book\n");

  if (!BUFEOF(curvafeap->f)) {
    printf("Warning: not using book; coordinates may not be correct:\n");
    printf("assuming 1 to n coordinates\n");
    huge_ = (long)(LONG_MAX / 2.0);
    while (!BUFEOF(curvafeap->f)) {
      fscanf(curvafeap->f, "%ld%ld%*[^\n]", &p.firstbase, &p.lastbase);
      getc(curvafeap->f);
      switch (p.basephase) {

      case 'f':
	thezero = p.firstbase + p.zerobase;
	break;

      case 'l':
	thezero = p.firstbase + p.zerobase;
	break;

      case ' ':
	thezero = p.zerobase;
	break;
      }
      inthezero = thezero;
      if (p.firstbase <= p.lastbase) {
	for (l = p.firstbase; l <= p.lastbase; l++)
	  nobookloop(inthezero, l, p, huge_);
      } else {
	for (l = p.firstbase; l >= p.lastbase; l--)
	  nobookloop(inthezero, l, p, huge_);
      }
    }
    return;
  }
  brinit(book, &theline);
  do {
    getocp(book, &theline, &org, &orgchange, &orgopen, &chr, &chrchange,
	   &chropen, &pie, &piechange, &pieopen);
    if (pieopen) {
      if (number > 1)
	fprintf(curvafeamarks->f, "p - curvafea skip to piece %ld\n\n",
		number);
      fprintf(curvafeamarks->f, "* piece %ld\n", number);
      /*


*/
      switch (p.basephase) {

      case 'f':
	thezero = pietoint(pie->key.piebeg, pie) + p.zerobase;
	/*

*/
	inthezero = thezero;
	break;

      case 'l':
	thezero = pietoint(pie->key.pieend, pie) + p.zerobase;
	/*

*/
	inthezero = thezero;
	break;

      case ' ':
	thezero = p.zerobase;
	inthezero = thezero;
	break;
      }
      /*

*/
      huge_ = piecelength(pie) * 2;
      FORLIM = piecelength(pie);
      for (internal = 1; internal <= FORLIM; internal++) {
	bookloop(inthezero, internal, pie, p, huge_, &cinternal, curvaout,
		 curvaxyin);

      }
    }
  } while (pieopen);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  curvaxyin.f = NULL;
  strcpy(curvaxyin.name, "curvaxyin");
  curvafeamarks.f = NULL;
  strcpy(curvafeamarks.name, "curvafeamarks");
  curvaout.f = NULL;
  strcpy(curvaout.name, "curvaout");
  curvafeap.f = NULL;
  strcpy(curvafeap.name, "curvafeap");
  book.f = NULL;
  strcpy(book.name, "book");
  themain(&book, &curvafeap, &curvaout, &curvafeamarks, &curvaxyin);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (curvafeap.f != NULL)
    fclose(curvafeap.f);
  if (curvaout.f != NULL)
    fclose(curvaout.f);
  if (curvafeamarks.f != NULL)
    fclose(curvafeamarks.f);
  if (curvaxyin.f != NULL)
    fclose(curvaxyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
