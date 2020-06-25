/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dotmat.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         3.08



/*













































*/



#define nodot           ':'
/*

*/
#define prime           '\''

#define pagelength      48




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

/* p2c: dotmat.p, line 123:
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



Static _TEXT xbook, ybook, mlist, dotmatp;

Static piece *xseq, *yseq;
Static long xnumber, ynumber;
Static boolean guallowed;

Static long xlength, ylength;







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



Static boolean basepair(xseq, yseq, x, y, guallowed)
piece *xseq, *yseq;
long x, y;
boolean guallowed;
{
  /*

*/
  base bx, by;

  bx = getbase(x, xseq);
  by = getbase(y, yseq);

  if (bx == complement(by))
    return true;
  else if (guallowed) {
    if (bx == g && by == t)
      return true;
    else if (bx == t && by == g)
      return true;
    else
      return false;
  } else
    return false;
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



Static Void unlimitln(afile)
_TEXT *afile;
{
  /*


*/
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




Static Void initialize()
{
  printf(" dotmat %4.2f\n", version);
  brinit(&xbook);
  brinit(&ybook);
  if (*mlist.name != '\0') {
    if (mlist.f != NULL)
      mlist.f = freopen(mlist.name, "w", mlist.f);
    else
      mlist.f = fopen(mlist.name, "w");
  } else {
    if (mlist.f != NULL)
      rewind(mlist.f);
    else
      mlist.f = tmpfile();
  }
  if (mlist.f == NULL)
    _EscIO2(FileNotFound, mlist.name);
  SETUPBUF(mlist.f, Char);
  unlimitln(&mlist);

  xseq = (piece *)Malloc(sizeof(piece));
  yseq = (piece *)Malloc(sizeof(piece));


  if (*dotmatp.name != '\0') {
    if (dotmatp.f != NULL)
      dotmatp.f = freopen(dotmatp.name, "r", dotmatp.f);
    else
      dotmatp.f = fopen(dotmatp.name, "r");
  } else
    rewind(dotmatp.f);
  if (dotmatp.f == NULL)
    _EscIO2(FileNotFound, dotmatp.name);
  RESETBUF(dotmatp.f, Char);
  if (BUFEOF(dotmatp.f))
    guallowed = false;
  else if (P_peek(dotmatp.f) == 'g')
    guallowed = true;
  else
    guallowed = false;


  fprintf(mlist.f, " dotmat %4.2f from:\n", version);
  putc(' ', mlist.f);
  copyaline(&xbook, &mlist);
  putc(' ', mlist.f);
  copyaline(&ybook, &mlist);

  fprintf(mlist.f, "\n gu pairs are");
  if (!guallowed)
    fprintf(mlist.f, " not");
  fprintf(mlist.f, " allowed.\n\n");

  fprintf(mlist.f, " symbols used:\n");
  fprintf(mlist.f, " %c no complementary bases between these points\n", nodot);
  fprintf(mlist.f, " 1 gt base pair\n");
  fprintf(mlist.f, " 2 at base pair\n");
  fprintf(mlist.f, " 3 gc base pair\n");
}




Static Void piecebar(afile, spaces, pie, firstnumber, lastnumber, linesused)
_TEXT *afile;
long spaces;
piece *pie;
long firstnumber, lastnumber, *linesused;
{
  /*





*/
  long logpower, spacecount, number;
  struct {
    FILE *f;
    FILEBUFNC(f,long);
    Char name[_FNSIZE];
  } numbers;
  long width, index;

  *numbers.name = '\0';
  *linesused = 0;
  if (*numbers.name != '\0')
    numbers.f = fopen(numbers.name, "wb");
  else
    numbers.f = tmpfile();
  if (numbers.f == NULL)
    _EscIO2(FileNotFound, numbers.name);
  SETUPBUF(numbers.f, long);
  for (index = firstnumber; index <= lastnumber; index++) {
    number = inttopie(index, pie);
    fwrite(&number, sizeof(long), 1, numbers.f);
    width = numbersize(number);
    if (width > *linesused)
      *linesused = width;
  }

  for (logpower = *linesused - 1; logpower >= 0; logpower--) {
    for (spacecount = 1; spacecount <= spaces; spacecount++)
      putc(' ', afile->f);
    if (*numbers.name != '\0')
      numbers.f = freopen(numbers.name, "rb", numbers.f);
    else
      rewind(numbers.f);
    if (numbers.f == NULL)
      _EscIO2(FileNotFound, numbers.name);
    RESETBUF(numbers.f, long);
    while (!BUFEOF(numbers.f)) {
      fread(&number, sizeof(long), 1, numbers.f);
      fputc(numberdigit(number, logpower), afile->f);
    }
    putc('\n', afile->f);
  }
  fclose(numbers.f);
}



Static Void makedna(afile, spaces, pie, firstnumber, lastnumber)
_TEXT *afile;
long spaces;
piece *pie;
long firstnumber, lastnumber;
{
  long spacecount, index;

  for (spacecount = 1; spacecount <= spaces - 3; spacecount++)
    putc(' ', afile->f);
  fprintf(afile->f, "5%c ", prime);
  for (index = firstnumber; index <= lastnumber; index++)
    fputc(basetochar(getbase(index, pie)), afile->f);
  fprintf(afile->f, " 3%c\n", prime);
}



Static Void makepageheader(pageheader)
_TEXT *pageheader;
{
  long lines;

  if (*pageheader->name != '\0') {
    if (pageheader->f != NULL)
      pageheader->f = freopen(pageheader->name, "w", pageheader->f);
    else
      pageheader->f = fopen(pageheader->name, "w");
  } else {
    if (pageheader->f != NULL)
      rewind(pageheader->f);
    else
      pageheader->f = tmpfile();
  }
  if (pageheader->f == NULL)
    _EscIO2(FileNotFound, pageheader->name);
  SETUPBUF(pageheader->f, Char);
  fprintf(pageheader->f, "\f");
  fprintf(pageheader->f, " matrix,  x down: %ld %.*s y across: %ld %.*s\n\n",
	  xnumber, namelength, xseq->key.hea.keynam.letters, ynumber,
	  namelength, yseq->key.hea.keynam.letters);
  piecebar(pageheader, 10L, yseq, 1L, ylength, &lines);
  makedna(pageheader, 10L, yseq, 1L, ylength);
  fprintf(pageheader->f, "        5%c\n", prime);
}



Static Void printpageheader(pageheader)
_TEXT *pageheader;
{
  if (*pageheader->name != '\0') {
    if (pageheader->f != NULL)
      pageheader->f = freopen(pageheader->name, "r", pageheader->f);
    else
      pageheader->f = fopen(pageheader->name, "r");
  } else
    rewind(pageheader->f);
  if (pageheader->f == NULL)
    _EscIO2(FileNotFound, pageheader->name);
  RESETBUF(pageheader->f, Char);
  while (!BUFEOF(pageheader->f)) {
    while (!P_eoln(pageheader->f)) {
      putc(P_peek(pageheader->f), mlist.f);
      getc(pageheader->f);
    }
    fscanf(pageheader->f, "%*[^\n]");
    getc(pageheader->f);
    putc('\n', mlist.f);
  }
}



Static Void printarray()
{
  _TEXT pageheader;
  long x, y;
  base bx, by;
  long FORLIM, FORLIM1;

  pageheader.f = NULL;
  *pageheader.name = '\0';
  makepageheader(&pageheader);
  FORLIM = xlength;
  for (x = 1; x <= FORLIM; x++) {
    if (x % pagelength == 0 || x == 1)
      printpageheader(&pageheader);
    bx = getbase(x, xseq);
    fprintf(mlist.f, " %6ld %c ", inttopie(x, xseq), basetochar(bx));
    FORLIM1 = ylength;
    for (y = 1; y <= FORLIM1; y++) {
      if (basepair(xseq, yseq, x, y, guallowed)) {
	by = getbase(y, yseq);
	if (bx == complement(by)) {
	  switch (bx) {

	  case a:
	    putc('2', mlist.f);
	    break;

	  case c:
	    putc('3', mlist.f);
	    break;

	  case g:
	    putc('3', mlist.f);
	    break;

	  case t:
	    putc('2', mlist.f);
	    break;
	  }
	} else
	  putc('1', mlist.f);
      } else
	putc(nodot, mlist.f);
    }
    putc('\n', mlist.f);
  }
  fprintf(mlist.f, "        3%c\n", prime);
  if (pageheader.f != NULL)
    fclose(pageheader.f);
}


Static Void printdotmat()
{
  printarray();
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  dotmatp.f = NULL;
  strcpy(dotmatp.name, "dotmatp");
  mlist.f = NULL;
  strcpy(mlist.name, "mlist");
  ybook.f = NULL;
  strcpy(ybook.name, "ybook");
  xbook.f = NULL;
  strcpy(xbook.name, "xbook");
  initialize();
  while (!BUFEOF(xbook.f)) {
    getpiece(&xbook, &xseq);
    xnumber = number;
    xlength = piecelength(xseq);
    if (BUFEOF(xbook.f))
      break;
    if (*ybook.name != '\0') {
      if (ybook.f != NULL)
	ybook.f = freopen(ybook.name, "r", ybook.f);
      else
	ybook.f = fopen(ybook.name, "r");
    } else
      rewind(ybook.f);
    if (ybook.f == NULL)
      _EscIO2(FileNotFound, ybook.name);
    RESETBUF(ybook.f, Char);
    while (!BUFEOF(ybook.f)) {
      getpiece(&ybook, &yseq);
      ynumber = number;
      ylength = piecelength(yseq);
      if (BUFEOF(ybook.f))
	break;
      printdotmat();
      clearpiece(&yseq);
    }
    clearpiece(&xseq);
  }
_L1:
  if (xbook.f != NULL)
    fclose(xbook.f);
  if (ybook.f != NULL)
    fclose(ybook.f);
  if (mlist.f != NULL)
    fclose(mlist.f);
  if (dotmatp.f != NULL)
    fclose(dotmatp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
