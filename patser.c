/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "patser.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         2.32
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

/* p2c: patser.p, line 116:
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


Static _TEXT patserp;
/*
*/
Static _TEXT inst;
/*
*/
Static _TEXT book, values, scale, pattern;
Static long beginning, width, printmin, maxafter, maxaligned;
/*
*/
Static matrix wmatrix;
Static long scaleup;
Static piece *thispie;

Static orgkey org;
Static chrkey chr;
Static boolean orgchange, orgopen, chrchange, chropen, pieopen;






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



Static Void unlimitln(afile)
_TEXT *afile;
{
  /*


*/
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






Static Void brorgkey(thefile, org)
_TEXT *thefile;
orgkey *org;
{
  brheader(thefile, &org->hea);
  getlineDelila(&org->mapunit);
  brline(thefile, &org->mapunit);
}



Static Void brchrkey(thefile, chr)
_TEXT *thefile;
chrkey *chr;
{
  brheader(thefile, &chr->hea);
  brreanum(thefile, &chr->mapbeg);
  brreanum(thefile, &chr->mapend);
}



Static Void getocp(thefile, org, orgchange, orgopen, chr, chrchange, chropen,
		   pie, pieopen)
_TEXT *thefile;
orgkey *org;
boolean *orgchange, *orgopen;
chrkey *chr;
boolean *chrchange, *chropen;
piece **pie;
boolean *pieopen;
{
  /*








*/
  Char ch = 'a';
  long SET[5];

  *orgchange = false;
  *chrchange = false;
  while (ch != 'p' && ch != ' ') {
    P_addset(P_expset(SET, 0L), 'o');
    P_addset(SET, 'c');
    ch = getto(thefile, P_addset(SET, 'p'));
    if (ch == ' ')
      break;
    switch (ch) {

    case 'o':
      if (*orgopen)
	*orgopen = false;
      else {
	brorgkey(thefile, org);
	*orgopen = true;
	*orgchange = true;
      }
      break;

    case 'c':
      if (*chropen)
	*chropen = false;
      else {
	brchrkey(thefile, chr);
	*chropen = true;
	*chrchange = true;
      }
      break;

    case 'p':
      if (*pieopen) {
	*pieopen = false;
	ch = 'a';
      } else {
	brpiece(thefile, pie);
	*pieopen = true;
      }
      break;
    }
  }
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





Static Void iwcombk(book, afile)
_TEXT *book, *afile;
{
  fprintf(afile->f, "(* ");
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
  if (copylines(book, afile, 1L) == 0) {
    printf(" book is empty, can not write comment for instructions\n");
    halt();
  }
  fprintf(afile->f, "*)\n");
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



Static Void iworgchr(afile, org, orgchange, chr, chrchange)
_TEXT *afile;
orgkey org;
boolean orgchange;
chrkey chr;
boolean chrchange;
{
  /*
*/
  if (orgchange)
    iworg(afile, org);
  if (orgchange && chrchange)
    putc(' ', afile->f);
  if (chrchange)
    iwchr(afile, chr);
  if (orgchange || chrchange)
    putc('\n', afile->f);
}


/* Local variables for iwget: */
struct LOC_iwget {
  _TEXT *afile;
  piece *pie;
  long pieceplace;
} ;

/* Local variables for iwplace: */
struct LOC_iwplace {
  struct LOC_iwget *LINK;
} ;

Local Void iwrelative(relative, LINK)
long relative;
struct LOC_iwplace *LINK;
{
  if (relative >= 0)
    fprintf(LINK->LINK->afile->f, " +%ld", relative);
  else if (relative < 0)
    fprintf(LINK->LINK->afile->f, " %ld", relative);
}

Local Void iwplace(relative, LINK)
long relative;
struct LOC_iwget *LINK;
{
  struct LOC_iwplace V;

  V.LINK = LINK;
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



Static Void iwget(afile_, pie_, fromplace, pieceplace_, toplace, flip)
_TEXT *afile_;
piece *pie_;
long fromplace, pieceplace_, toplace;
boolean flip;
{
  /*


*/
  struct LOC_iwget V;

  V.afile = afile_;
  V.pie = pie_;
  V.pieceplace = pieceplace_;
  fprintf(V.afile->f, "get from");
  iwplace(fromplace, &V);
  fprintf(V.afile->f, " to");
  iwplace(toplace, &V);

  fprintf(V.afile->f, " direction");
  switch (V.pie->key.piedir) {

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

  fprintf(V.afile->f, ";\n");
}






Static Void valuheader()
{
  fprintf(values.f, "* patser %4.2f; evaluations of sequences in \n", version);
  copyaline(&book, &values);
  fprintf(values.f, "* by the matrix from ");
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
  fprintf(values.f, "*\n");
}


Static Void instheader()
{
  fprintf(inst.f,
    "(* delila instructions from patser %4.2f evaluations of sequences in \n",
    version);
  if (*book.name != '\0') {
    if (book.f != NULL)
      book.f = freopen(book.name, "r", book.f);
    else
      book.f = fopen(book.name, "r");
  } else
    rewind(book.f);
  if (book.f == NULL)
    _EscIO2(FileNotFound, book.name);
  RESETBUF(book.f, Char);
  copyaline(&book, &inst);
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
  fprintf(inst.f, "by the matrix from ");
  fscanf(pattern.f, "%*[^\n]");
  getc(pattern.f);
  copyaline(&pattern, &inst);
  fprintf(inst.f, " *)\n");
  fprintf(inst.f, "  \n");
  fprintf(inst.f, "default out-of-range reduce-range;\n");
  fprintf(inst.f, "default numbering piece;\n");
  fprintf(inst.f, "default key note off;\n");
  fprintf(inst.f, " \n");
}


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


Static Void printingminimum()
{
  /*

*/
  if (!BUFEOF(patserp.f))
    fscanf(patserp.f, "%ld", &printmin);
  else {
    findlearnend(&pattern);
    getcolon(&pattern);
    fscanf(pattern.f, "%ld%*[^\n]", &printmin);
    getc(pattern.f);
  }
  fprintf(values.f, "* sequences identified have a minimum value of %4ld\n",
	  printmin);
  fprintf(values.f, "*\n");
  /*

*/
  fprintf(values.f, "* sequence #, position, value\n");
}


/* Local variables for evaluate: */
struct LOC_evaluate {
  long valu;
} ;

/*
*/

Local Void getvalue(thispie, seqpos, matpos, LINK)
piece *thispie;
long seqpos, matpos;
struct LOC_evaluate *LINK;
{
  /*
*/
  LINK->valu += wmatrix[(long)getbase(seqpos, thispie)][matpos-1];
}


Static Void evaluate()
{
  /*



*/
  struct LOC_evaluate V;
  long firstpos;
  /*

*/
  long lastpos;
  /*

*/
  long alignedbase;
  /*
*/
  long length, i;
  boolean offend;
  long FORLIM1;

  getocp(&book, &org, &orgchange, &orgopen, &chr, &chrchange, &chropen,
	 &thispie, &pieopen);
  if (BUFEOF(book.f))
    return;
  iworgchr(&inst, org, orgchange, chr, chrchange);
  iwpie(&inst, thispie->key);
  length = piecelength(thispie);
  for (alignedbase = 1; alignedbase <= length; alignedbase++) {
    V.valu = 0;
    offend = false;
    firstpos = alignedbase - maxaligned + 1;
    lastpos = alignedbase + maxafter;
    if (firstpos > 0) {
      if (lastpos <= length) {
	for (i = firstpos; i <= lastpos; i++)
	  getvalue(thispie, i, i - firstpos + 1, &V);
      } else {
	for (i = firstpos; i <= length; i++)
	  getvalue(thispie, i, i - firstpos + 1, &V);
	if (thispie->key.piecon == circular) {
	  FORLIM1 = lastpos - length;
	  for (i = 1; i <= FORLIM1; i++)
	    getvalue(thispie, i, width + i - lastpos + length, &V);
	}

	else
	  offend = true;
      }
    } else {
      if (lastpos <= length) {
	for (i = 1; i <= lastpos; i++)
	  getvalue(thispie, i, i - firstpos + 1, &V);
      } else {
	for (i = 1; i <= length; i++)
	  getvalue(thispie, i, i - firstpos + 1, &V);
	if (thispie->key.piecon == circular) {
	  FORLIM1 = lastpos - length;
	  for (i = 1; i <= FORLIM1; i++)
	    getvalue(thispie, i, width + i - lastpos + length, &V);
	}

      }
      if (thispie->key.piecon == circular) {
	for (i = length + firstpos; i <= length; i++)
	  getvalue(thispie, i, i - length - firstpos + 1, &V);
      }

      else
	offend = true;
    }
    if (V.valu >= printmin && !offend) {
      fprintf(values.f, "%8ld%12ld%20.10f\n",
	      number, inttopie(alignedbase, thispie),
	      (double)V.valu / scaleup);
      /*



*/

      iwget(&inst, thispie, 1 - maxaligned, inttopie(alignedbase, thispie),
	    maxafter, false);
    }
  }
  clearpiece(&thispie);
}


Static Void initialize()
{
  brinit(&book);
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
  if (*patserp.name != '\0') {
    if (patserp.f != NULL)
      patserp.f = freopen(patserp.name, "r", patserp.f);
    else
      patserp.f = fopen(patserp.name, "r");
  } else
    rewind(patserp.f);
  if (patserp.f == NULL)
    _EscIO2(FileNotFound, patserp.name);
  RESETBUF(patserp.f, Char);
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
  if (*inst.name != '\0') {
    if (inst.f != NULL)
      inst.f = freopen(inst.name, "w", inst.f);
    else
      inst.f = fopen(inst.name, "w");
  } else {
    if (inst.f != NULL)
      rewind(inst.f);
    else
      inst.f = tmpfile();
  }
  if (inst.f == NULL)
    _EscIO2(FileNotFound, inst.name);
  SETUPBUF(inst.f, Char);


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

  skipunnum = true;
  thispie = (piece *)Malloc(sizeof(piece));

  orgopen = false;
  chropen = false;
  pieopen = false;
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
  book.f = NULL;
  strcpy(book.name, "book");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  patserp.f = NULL;
  strcpy(patserp.name, "patserp");
  printf("patser %4.2f\n", version);
  initialize();
  readmatrix(&pattern, wmatrix, &beginning, &width);


  valuheader();
  instheader();
  printingminimum();
  maxaligned = 1 - beginning;
  maxafter = width - maxaligned;

  while (!BUFEOF(book.f))
    evaluate();
_L1:
  if (patserp.f != NULL)
    fclose(patserp.f);
  if (inst.f != NULL)
    fclose(inst.f);
  if (book.f != NULL)
    fclose(book.f);
  if (values.f != NULL)
    fclose(values.f);
  if (scale.f != NULL)
    fclose(scale.f);
  if (pattern.f != NULL)
    fclose(pattern.f);
  exit(EXIT_SUCCESS);
}



/* End. */
