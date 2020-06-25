/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "hist.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/



#define version         4.27
/*
*/



/*













































*/





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

/* p2c: hist.p, line 114:
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



typedef struct alpiece {
  long length, alignedbase;
  dnastring *dna;
  struct alpiece *next;
} alpiece;






Static line *freeline;
Static dnastring *freedna;

Static boolean readnumber;
/*
*/
Static long number;
Static boolean numbered, skipunnum;
/*
*/




Static _TEXT hst, histp, book, inst;
Static long oligomin, oligomax, oligolength, seqnumb, maxaligned, maxafter;
Static alpiece *alpie, *first;
Static piece *apiece;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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


#define maximumrange    500



Static Void align(inst, book, pie, length, alignedbase)
_TEXT *inst, *book;
piece **pie;
long *length, *alignedbase;
{
  /*
*/
  /*


*/
  Char ch;
  boolean comment;
  boolean done = false;
  long thebase;
  name *WITH;

  if (BUFEOF(book->f))
    return;
  getpiece(book, pie);
  if (BUFEOF(book->f))
    return;
  *length = pietoint((*pie)->key.pieend, *pie);


  while (!done) {
    if (BUFEOF(inst->f)) {
      *alignedbase = 1;
      done = true;
      break;
    }
    if (P_peek(inst->f) == '(') {
      getc(inst->f);
      if (P_peek(inst->f) == '*')
	comment = true;

      while (comment) {
	if (BUFEOF(inst->f)) {
	  printf(" in procedure align:\n");
	  printf(" an instruction comment does not end!\n");
	  halt();
	}

	getc(inst->f);
	if (P_peek(inst->f) == '*') {
	  getc(inst->f);

	  if (P_peek(inst->f) == ')')
	    comment = false;
	}
      }
    }
    if (P_peek(inst->f) == 'g') {
      getc(inst->f);
      if (P_peek(inst->f) == 'e') {
	getc(inst->f);
	if (P_peek(inst->f) == 't') {
	  getc(inst->f);
	  if (P_peek(inst->f) == ' ') {
	    findnonblank(inst, &ch);
	    findblank(inst);
	    fscanf(inst->f, "%ld", &thebase);

	    *alignedbase = pietoint(thebase, *pie);
	    done = true;
	  }
	}
      }
    }
    getc(inst->f);
  }

  if (*alignedbase > -maximumrange && *alignedbase <= *length + maximumrange)
    return;
  printf(" in procedure align:\n");
  printf(" read in base was %ld\n", thebase);
  printf(" in internal coordinates: %ld\n", *alignedbase);
  printf(" maximum range was %ld\n", (long)maximumrange);
  printf(" piece length was %ld\n", *length);
  WITH = &(*pie)->key.hea.keynam;
/* p2c: hist.p, line 866: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  printf(" piece name: %.*s\n", WITH->length, WITH->letters);
  printf(" piece number: %ld\n", number);
  printf(" aligned base is too far away... see the code\n");
  halt();
}

#undef maximumrange


Static Void histogramwidth()
{
  /*
*/
  if (maxaligned < alpie->alignedbase)
    maxaligned = alpie->alignedbase;
  if (maxafter < alpie->length - alpie->alignedbase)
    maxafter = alpie->length - alpie->alignedbase;
  if (maxaligned + maxafter > dnamax) {
    printf("dnamax too small");
    halt();
  }
}


Static base getalbase(position, pie)
long position;
alpiece *pie;
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


/* Local variables for makehist: */
struct LOC_makehist {
  long histwidth, histfield;
  base oligoarray[4];
  long oligolength;
} ;

/* Local variables for getnextletter: */
struct LOC_getnextletter {
  struct LOC_makehist *LINK;
} ;

/* Local variables for scan: */
struct LOC_scan {
  struct LOC_getnextletter *LINK;
  long tablearray[dnamax];
} ;

Local Void tableout(LINK)
struct LOC_scan *LINK;
{
  /*
*/
  long i, FORLIM;

  FORLIM = oligomax - LINK->LINK->LINK->oligolength;
  for (i = 0; i <= FORLIM; i++)
    putc(' ', hst.f);
  FORLIM = oligomax;
  for (i = oligomax - LINK->LINK->LINK->oligolength; i < FORLIM; i++)
    fputc(basetochar(LINK->LINK->LINK->oligoarray[i]), hst.f);
  putc(' ', hst.f);
  FORLIM = LINK->LINK->LINK->histwidth;
  for (i = 0; i < FORLIM; i++)
    fprintf(hst.f, " %*ld",
	    (int)LINK->LINK->LINK->histfield, LINK->tablearray[i]);
  putc('\n', hst.f);
  /*
*/
  if (LINK->LINK->LINK->oligoarray[oligomax-1] == t ||
      LINK->LINK->LINK->oligolength == 0)
    putc('\n', hst.f);
}


Local Void scan(LINK)
struct LOC_getnextletter *LINK;
{
  /*
*/
  struct LOC_scan V;
  long i, j, k, l;
  boolean match;
  long FORLIM, FORLIM1;

  V.LINK = LINK;
  FORLIM = LINK->LINK->histwidth;
  for (i = 0; i < FORLIM; i++)
    V.tablearray[i] = 0;
  alpie = first;
  FORLIM = seqnumb;
  for (j = 1; j <= FORLIM; j++) {
    FORLIM1 = alpie->length - LINK->LINK->oligolength + 1;
    for (l = 1; l <= FORLIM1; l++) {
      k = 0;
      match = true;
      while (k < LINK->LINK->oligolength && match) {
	if (getalbase(k + l, alpie) ==
	    LINK->LINK->oligoarray[oligomax - LINK->LINK->oligolength + k])
	  k++;
	else
	  match = false;
      }
      if (match)
	V.tablearray[maxaligned - alpie->alignedbase + l - 1]++;
    }
    alpie = alpie->next;
  }
  tableout(&V);
}

Local Void getnextletter(k, LINK)
long k;
struct LOC_makehist *LINK;
{
  /*
*/
  struct LOC_getnextletter V;
  base ba;

  V.LINK = LINK;
  if (k == 0) {
    scan(&V);
    return;
  }
  for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1)) {
    LINK->oligoarray[oligomax - k] = ba;
    getnextletter(k - 1, LINK);
  }
}


Static Void makehist()
{
  /*
*/
  struct LOC_makehist V;
  long i, FORLIM;


  V.histwidth = maxafter + maxaligned;
  /*

*/
  V.histfield = numbersize(seqnumb);
  if (V.histfield < numbersize(1 - maxaligned))
    V.histfield = numbersize(1 - maxaligned);
  if (V.histfield < numbersize(maxafter))
    V.histfield = numbersize(maxafter);


  fprintf(hst.f, "%4ld is the shortest oligo searched for\n", oligomin);
  fprintf(hst.f, "%4ld is the longest oligo searched for\n", oligomax);
  fprintf(hst.f, "%4ld is the number of sequences in the book\n", seqnumb);
  fprintf(hst.f, "%4ld is the width (in bases) of the histogram\n\n",
	  V.histwidth);
  FORLIM = oligomax + 1;

  for (i = 0; i <= FORLIM; i++)
    putc(' ', hst.f);
  FORLIM = maxafter;
  for (i = 1 - maxaligned; i <= FORLIM; i++)
    fprintf(hst.f, " %*ld", (int)V.histfield, i);
  putc('\n', hst.f);

  FORLIM = oligomax + 1;
  for (i = 0; i <= FORLIM; i++)
    putc(' ', hst.f);
  FORLIM = V.histwidth * (V.histfield + 1);
  for (i = 1; i <= FORLIM; i++)
    putc('-', hst.f);
  putc('\n', hst.f);

  FORLIM = oligomax;
  for (V.oligolength = oligomin; V.oligolength <= FORLIM; V.oligolength++)
    getnextletter(V.oligolength, &V);
}


Static Void setparam()
{
  if (BUFEOF(histp.f)) {
    oligomin = 0;
    oligomax = 2;
    return;
  }
  fscanf(histp.f, "%ld%*[^\n]", &oligomin);
  getc(histp.f);
  if (BUFEOF(histp.f)) {
    printf("there must be a second parameter in histp\n");
    halt();
  }
  fscanf(histp.f, "%ld%*[^\n]", &oligomax);
  getc(histp.f);

  if (oligomin > oligomax) {
    printf("oligomin cannot be greater than oligomax\n");
    halt();
  }
  if (oligomin > 4 || oligomax > 4) {
    printf("oligomin and oligomax must be no larger than 4\n");
    halt();
  }
  if (oligomin < 0) {
    printf("oligomin must be positive or zero\n");
    halt();
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  inst.f = NULL;
  strcpy(inst.name, "inst");
  book.f = NULL;
  strcpy(book.name, "book");
  histp.f = NULL;
  strcpy(histp.name, "histp");
  hst.f = NULL;
  strcpy(hst.name, "hst");
  printf("hist %4.2f\n", version);

  brinit(&book);
  if (*inst.name != '\0') {
    if (inst.f != NULL)
      inst.f = freopen(inst.name, "r", inst.f);
    else
      inst.f = fopen(inst.name, "r");
  } else
    rewind(inst.f);
  if (inst.f == NULL)
    _EscIO2(FileNotFound, inst.name);
  RESETBUF(inst.f, Char);
  if (*histp.name != '\0') {
    if (histp.f != NULL)
      histp.f = freopen(histp.name, "r", histp.f);
    else
      histp.f = fopen(histp.name, "r");
  } else
    rewind(histp.f);
  if (histp.f == NULL)
    _EscIO2(FileNotFound, histp.name);
  RESETBUF(histp.f, Char);
  setparam();
  if (*hst.name != '\0') {
    if (hst.f != NULL)
      hst.f = freopen(hst.name, "w", hst.f);
    else
      hst.f = fopen(hst.name, "w");
  } else {
    if (hst.f != NULL)
      rewind(hst.f);
    else
      hst.f = tmpfile();
  }
  if (hst.f == NULL)
    _EscIO2(FileNotFound, hst.name);
  SETUPBUF(hst.f, Char);
  fprintf(hst.f, "* hist %4.2f histogram of \n", version);
  copyaline(&book, &hst);
  apiece = (piece *)Malloc(sizeof(piece));

  maxaligned = 0;
  maxafter = 0;
  seqnumb = 0;
  first = NULL;

  while (!BUFEOF(book.f)) {
    alpie = (alpiece *)Malloc(sizeof(alpiece));
    align(&inst, &book, &apiece, &alpie->length, &alpie->alignedbase);
    if (BUFEOF(book.f))
      break;
    seqnumb++;
    alpie->dna = apiece->dna;
    apiece->dna = NULL;
    alpie->next = first;
    first = alpie;
    clearpiece(&apiece);
    histogramwidth();
  }

  makehist();
  /*

*/
_L1:
  if (hst.f != NULL)
    fclose(hst.f);
  if (histp.f != NULL)
    fclose(histp.f);
  if (book.f != NULL)
    fclose(book.f);
  if (inst.f != NULL)
    fclose(inst.f);
  exit(EXIT_SUCCESS);
}



/* End. */
