/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "aran.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/



#define version         1.16
/*
*/



/*














































*/



#define seqmax          100





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

/* p2c: aran.p, line 120:
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




typedef struct seqmatrix {
  double data[(long)t - (long)a + 1][seqmax];
  long length;
} seqmatrix;


Static _TEXT book, list, sequ, aranp;






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






Static base stepbase(startdna, dna, d)
dnastring *startdna, **dna;
dnarange *d;
{
  /*




*/
  long TEMP;

  if (*d != dnamax && *d != (*dna)->length) {
    (*d)++;
    TEMP = *d - 1;
    return ((base)P_getbits_UB((*dna)->part, TEMP, 1, 3));
  }
  *d = 1;
  *dna = (*dna)->next;
  if (*dna == NULL)
    *dna = startdna;
  TEMP = *d - 1;
  return ((base)P_getbits_UB((*dna)->part, TEMP, 1, 3));
}



Static Void initnextbase(book, pie, lastbase, endofbook)
_TEXT *book;
piece **pie;
boolean *lastbase, *endofbook;
{
  /*




*/
  piece *WITH;
  header *WITH1;

  brinit(book);

  *pie = (piece *)Malloc(sizeof(piece));
  WITH = *pie;

  WITH1 = &WITH->key.hea;
  WITH1->fulnam = NULL;
  WITH1->note = NULL;
  WITH->dna = NULL;
  *lastbase = true;
  if (BUFEOF(book->f))
    *endofbook = true;
  else
    *endofbook = false;
}


Static base nextbase(book, pie, dnalink, dnalinkspot, dnaspot, length,
		     lastbase, endofbook)
_TEXT *book;
piece **pie;
dnastring **dnalink;
dnarange *dnalinkspot;
long *dnaspot, *length;
boolean *lastbase, *endofbook;
{


  /*
*/
  /*
*/
  /*






*/
  base Result;

  if (!*lastbase) {
    Result = stepbase((*pie)->dna, dnalink, dnalinkspot);
    (*dnaspot)++;
    if (*dnaspot == *length)
      *lastbase = true;
    return Result;
  } else {
    clearpiece(pie);
    getpiece(book, pie);
    if (!BUFEOF(book->f)) {
      *dnalink = (*pie)->dna;
      *dnalinkspot = 0;
      *dnaspot = 0;
      *length = piecelength(*pie);
      *lastbase = false;
      *endofbook = false;
      return (nextbase(book, pie, dnalink, dnalinkspot, dnaspot, length,
		       lastbase, endofbook));
    } else {
      *endofbook = true;
      *lastbase = false;
      return a;
    }
  }
  return Result;
}


#define pow14           16384

#define pow15           32768L
#define pow22           4194304L
#define pow23           8388608L






Static Void randomDelila(seed)
double *seed;
{
  /*



*/

  /*











*/
  long iseed, i, nrep;

  iseed = (long)floor(*seed * pow23 + 0.5);
  if (iseed < 1 || iseed >= pow23)
    iseed = 1;
  nrep = (iseed & 7) + 4;
  for (i = 1; i <= nrep; i++) {
    if ((iseed & 1) == ((iseed & (pow15 - 1)) >= pow14))
      iseed /= 2;
    else
      iseed = iseed / 2 + pow22;
  }

  *seed = (double)iseed / pow23;
}

#undef pow14
#undef pow15
#undef pow22
#undef pow23



Static Void readparameters(aranp, fraction, number)
_TEXT *aranp;
double *fraction;
long *number;
{
  if (*aranp->name != '\0') {
    if (aranp->f != NULL)
      aranp->f = freopen(aranp->name, "r", aranp->f);
    else
      aranp->f = fopen(aranp->name, "r");
  } else
    rewind(aranp->f);
  if (aranp->f == NULL)
    _EscIO2(FileNotFound, aranp->name);
  RESETBUF(aranp->f, Char);
  if (BUFEOF(aranp->f)) {
    printf("aranp must contain two parmeters\n");
    halt();
  }
  fscanf(aranp->f, "%lg%*[^\n]", fraction);
  getc(aranp->f);

  if ((unsigned)(*fraction) > 1.0) {
    printf("The fraction given in aranp must be between 0 and 1\n");
    halt();
  }

  if (BUFEOF(aranp->f)) {
    printf("aranp must contain the number of sequences to generate\n");
    halt();
  }

  fscanf(aranp->f, "%ld%*[^\n]", number);
  getc(aranp->f);

  if (*number < 1) {
    printf("at least one sequence must be generated\n");
    halt();
  }
}



Static Void getseqmatrix(book, fout, conserved, unconserved, sm)
_TEXT *book, *fout;
double conserved, unconserved;
seqmatrix *sm;
{
  /*
*/
  base b;
  long l;


  piece *pie;
  dnastring *dnalink;
  dnarange dnalinkspot;
  long dnaspot, length;
  boolean lastbase, endofbook;

  /*
*/
  Char character;
  long FORLIM;

  for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
    for (l = 0; l < seqmax; l++)
      sm->data[(long)b - (long)a][l] = unconserved;
  }

  l = 0;

  initnextbase(book, &pie, &lastbase, &endofbook);
  do {
    b = nextbase(book, &pie, &dnalink, &dnalinkspot, &dnaspot, &length,
		 &lastbase, &endofbook);
    if (length > seqmax) {
      printf("sequence length (%ld) exceeds seqmax constant (%ld)\n",
	     length, (long)seqmax);
      halt();
    }
    l++;
    sm->data[(long)b - (long)a][l-1] = conserved;

    character = basetochar(b);
    putc(character, fout->f);
    /*





*/
  } while (!lastbase);

  sm->length = l;
  fprintf(fout->f, ".\n");
  fprintf(fout->f, "%ld bases\n", sm->length);

  fprintf(fout->f, "----\n");

  fprintf(fout->f, "The frequency matrix:\n");
  FORLIM = sm->length;
  for (l = 0; l < FORLIM; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fprintf(fout->f, " %4.2f", sm->data[(long)b - (long)a][l]);
    putc('\n', fout->f);
  }
  fprintf(fout->f, "----\n");


  fprintf(fout->f, "Sum across the matrix for faster sequence generation:\n");
  FORLIM = sm->length;
  for (l = 0; l < FORLIM; l++) {
    for (b = c; (long)b <= (long)t; b = (base)((long)b + 1))
      sm->data[(long)b - (long)a][l] += sm->data[(long)b - (long)a - 1][l];
  }

  FORLIM = sm->length;
  for (l = 0; l < FORLIM; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fprintf(fout->f, " %4.2f", sm->data[(long)b - (long)a][l]);
    putc('\n', fout->f);
  }
  fprintf(fout->f, "----\n");
}


#define pagewidth       60



Static Void generate(seed, sm, number, sequ)
double *seed;
seqmatrix sm;
long number;
_TEXT *sequ;
{
  /*
*/
  long l, n;

  if (*sequ->name != '\0') {
    if (sequ->f != NULL)
      sequ->f = freopen(sequ->name, "w", sequ->f);
    else
      sequ->f = fopen(sequ->name, "w");
  } else {
    if (sequ->f != NULL)
      rewind(sequ->f);
    else
      sequ->f = tmpfile();
  }
  if (sequ->f == NULL)
    _EscIO2(FileNotFound, sequ->name);
  SETUPBUF(sequ->f, Char);
  for (n = 1; n <= number; n++) {
    for (l = 1; l <= sm.length; l++) {
      if (l % pagewidth == 0)
	putc('\n', sequ->f);
      randomDelila(seed);
      if (sm.data[0][l-1] > *seed)
	putc('a', sequ->f);
      else if (sm.data[(long)c - (long)a][l-1] > *seed)
	putc('c', sequ->f);
      else if (sm.data[(long)g - (long)a][l-1] > *seed)
	putc('g', sequ->f);
      else
	putc('t', sequ->f);
    }
    fprintf(sequ->f, ".\n");
  }
}

#undef pagewidth



Static Void themain(book, list, sequ, aranp)
_TEXT *book, *list, *sequ, *aranp;
{
  double conserved;
  long number;
  double unconserved;
  double seed = 0.5;
  seqmatrix sm;

  printf("aran %4.2f\n", version);
  if (*sequ->name != '\0') {
    if (sequ->f != NULL)
      sequ->f = freopen(sequ->name, "w", sequ->f);
    else
      sequ->f = fopen(sequ->name, "w");
  } else {
    if (sequ->f != NULL)
      rewind(sequ->f);
    else
      sequ->f = tmpfile();
  }
  if (sequ->f == NULL)
    _EscIO2(FileNotFound, sequ->name);
  SETUPBUF(sequ->f, Char);
  readparameters(aranp, &conserved, &number);
  unconserved = (1.0 - conserved) / 3.0;

  if (*list->name != '\0') {
    if (list->f != NULL)
      list->f = freopen(list->name, "w", list->f);
    else
      list->f = fopen(list->name, "w");
  } else {
    if (list->f != NULL)
      rewind(list->f);
    else
      list->f = tmpfile();
  }
  if (list->f == NULL)
    _EscIO2(FileNotFound, list->name);
  SETUPBUF(list->f, Char);
  fprintf(list->f, "aran %4.2f\n", version);
  fprintf(list->f, "  Conserved fraction: %4.2f\n", conserved);
  fprintf(list->f, "unConserved fraction: %4.2f\n", unconserved);
  getseqmatrix(book, list, conserved, unconserved, &sm);
  fprintf(list->f, "%ld sequences generated\n", number);

  generate(&seed, sm, number, sequ);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  aranp.f = NULL;
  strcpy(aranp.name, "aranp");
  sequ.f = NULL;
  strcpy(sequ.name, "sequ");
  list.f = NULL;
  strcpy(list.name, "list");
  book.f = NULL;
  strcpy(book.name, "book");
  themain(&book, &list, &sequ, &aranp);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (list.f != NULL)
    fclose(list.f);
  if (sequ.f != NULL)
    fclose(sequ.f);
  if (aranp.f != NULL)
    fclose(aranp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
