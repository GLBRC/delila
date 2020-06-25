/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "patval.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         2.24
/*
*/



/*





























*/






#define dnamax          3000
#define namelength      20
#define linelength      80




#define maxmatrix       120





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

/* p2c: patval.p, line 102:
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



typedef long matrix[4][maxmatrix];







Static line *freeline;
Static dnastring *freedna;

Static boolean readnumber;
/*
*/
Static long number;
Static boolean numbered, skipunnum;
/*
*/



Static _TEXT book, inst, values, scale, pattern;
Static long beginning, width, length_, alignedbase;
Static matrix wmatrix;
Static long scaleup;
Static piece *thispie;


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






Static base getbase(position, pie)
long position;
piece *pie;
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
/* p2c: patval.p, line 885: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  printf(" piece name: %.*s\n", WITH->length, WITH->letters);
  printf(" piece number: %ld\n", number);
  printf(" aligned base is too far away... see the code\n");
  halt();
}

#undef maximumrange


#define learnmax        16






Static Void findlearnend(pattern)
_TEXT *pattern;
{
  Char learnend[learnmax];
  long state_ = 1;

  if (*pattern->name != '\0') {
    if (pattern->f != NULL)
      pattern->f = freopen(pattern->name, "r", pattern->f);
    else
      pattern->f = fopen(pattern->name, "r");
  } else
    rewind(pattern->f);
  if (pattern->f == NULL)
    _EscIO2(FileNotFound, pattern->name);
  RESETBUF(pattern->f, Char);
  memcpy(learnend, "end of learning.", (long)learnmax);
  while (!BUFEOF(pattern->f) && state_ < learnmax) {
    if (learnend[state_-1] == P_peek(pattern->f))
      state_++;
    else
      state_ = 1;
    if (P_eoln(pattern->f)) {
      fscanf(pattern->f, "%*[^\n]");
      getc(pattern->f);
    } else
      getc(pattern->f);
  }
  if (!BUFEOF(pattern->f))
    return;
  printf("pattern matrix does not contain \"");
  printf("%.*s\" signal\n", learnmax, learnend);
  halt();
}

#undef learnmax


Static Void getcolon(f)
_TEXT *f;
{
  while (P_peek(f->f) != ':') {
    getc(f->f);
    if (BUFEOF(f->f)) {
      printf("pattern is missing colons\n");
      halt();
    }
  }
  getc(f->f);
}


Static Void readmatrix(thefile, wmatrix, beginning, width)
_TEXT *thefile;
long (*wmatrix)[maxmatrix];
long *beginning, *width;
{
  /*
*/
  long i;
  base ba;
  long FORLIM;

  findlearnend(thefile);


  getcolon(thefile);
  getcolon(thefile);

  getcolon(thefile);
  fscanf(thefile->f, "%ld", beginning);

  getcolon(thefile);
  fscanf(thefile->f, "%ld", width);

  if (*width > maxmatrix) {
    printf("input matrix too large\n");
    halt();
  }

  FORLIM = *width;
  for (i = 0; i < FORLIM; i++) {
    getcolon(thefile);
    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1))
      fscanf(thefile->f, "%ld", &wmatrix[(long)ba][i]);
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
  }

  for (i = *width; i < maxmatrix; i++) {
    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1))
      wmatrix[(long)ba][i] = 0;
  }
}


Static Void valheader()
{
  fprintf(values.f, "* patval %4.2f evaluations of sequences in:\n", version);
  copyaline(&book, &values);
  fprintf(values.f, "* by the matrix from \n");
  if (*pattern.name != '\0') {
    if (pattern.f != NULL)
      pattern.f = freopen(pattern.name, "r", pattern.f);
    else
      pattern.f = fopen(pattern.name, "r");
  } else
    rewind(pattern.f);
  if (pattern.f == NULL)
    _EscIO2(FileNotFound, pattern.name);
  RESETBUF(pattern.f, Char);
  fscanf(pattern.f, "%*[^\n]");
  getc(pattern.f);
  putc('*', values.f);
  copyaline(&pattern, &values);
  fprintf(values.f, "* value, sequence #\n");
}


Static Void evaluate()
{
  /*





*/
  long valu = 0;
  long seqpos, matpos;

  align(&inst, &book, &thispie, &length_, &alignedbase);
  if (BUFEOF(book.f))
    return;

  if (beginning + alignedbase < 1) {
    seqpos = 1;
    matpos = 2 - beginning - alignedbase;
  } else {
    matpos = 1;
    seqpos = alignedbase + beginning;
  }

  while (seqpos <= length_ && matpos <= width) {
    valu += wmatrix[(long)getbase(seqpos, thispie)][matpos-1];
    seqpos++;
    matpos++;
  }
  fprintf(values.f, "%20.10f %4ld\n", (double)valu / scaleup, number);
  clearpiece(&thispie);
}


Static Void initialize()
{
  printf("patval %4.2f\n", version);
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


  if (*scale.name != '\0') {
    if (scale.f != NULL)
      scale.f = freopen(scale.name, "r", scale.f);
    else
      scale.f = fopen(scale.name, "r");
  } else
    rewind(scale.f);
  if (scale.f == NULL)
    _EscIO2(FileNotFound, scale.name);
  RESETBUF(scale.f, Char);
  scaleup = 0;
  if (!BUFEOF(scale.f))
    fscanf(scale.f, "%ld", &scaleup);
  if (scaleup <= 0)
    scaleup = 1;

  if (*values.name != '\0') {
    if (values.f != NULL)
      values.f = freopen(values.name, "w", values.f);
    else
      values.f = fopen(values.name, "w");
  } else {
    if (values.f != NULL)
      rewind(values.f);
    else
      values.f = tmpfile();
  }
  if (values.f == NULL)
    _EscIO2(FileNotFound, values.name);
  SETUPBUF(values.f, Char);
  skipunnum = true;
  thispie = (piece *)Malloc(sizeof(piece));
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  pattern.f = NULL;
  strcpy(pattern.name, "pattern");
  scale.f = NULL;
  strcpy(scale.name, "scale");
  values.f = NULL;
  strcpy(values.name, "values");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  book.f = NULL;
  strcpy(book.name, "book");
  initialize();
  readmatrix(&pattern, wmatrix, &beginning, &width);
  valheader();
  while (!BUFEOF(book.f))
    evaluate();
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (inst.f != NULL)
    fclose(inst.f);
  if (values.f != NULL)
    fclose(values.f);
  if (scale.f != NULL)
    fclose(scale.f);
  if (pattern.f != NULL)
    fclose(pattern.f);
  exit(EXIT_SUCCESS);
}



/* End. */
