/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "gap.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.08
/*
*/



/*


















































*/



#define pagelength      59




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

/* p2c: gap.p, line 121:
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




Static _TEXT inst, book, gapp, data;


Static piece *apiece;
Static long length_, alignedbase, fromparam, toparam;

Static long fromdo, todo;
Static boolean userrange;
Static Char displaylevel;
/*
*/

Static long index_;

Static boolean orgopen;

Static long gaps, gapminimum, modulus;






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
/* p2c: gap.p, line 862: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  printf(" piece name: %.*s\n", WITH->length, WITH->letters);
  printf(" piece number: %ld\n", number);
  printf(" aligned base is too far away... see the code\n");
  halt();
}

#undef maximumrange


#define maximumrange    500






Static Void maxminalignment(inst, book, fromparam, toparam)
_TEXT *inst, *book;
long *fromparam, *toparam;
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
    align(inst, book, &pie, &length, &alignedbase);
    if (BUFEOF(book->f))
      break;
    distance = 1 - alignedbase;
    if (*fromparam > distance)
      *fromparam = distance;

    distance = length - alignedbase;
    if (*toparam < distance)
      *toparam = distance;
    clearpiece(&pie);
  }

  if (*toparam - *fromparam > maximumrange) {
    printf(" in procedure maxminalignment:\n");
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



Static boolean withinalignment(alignedposition, alignedbase, length)
long alignedposition, alignedbase, length;
{
  /*
*/
  long p;

  p = alignedposition + alignedbase;
  return (p > 0 && p <= length);
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




main(argc, argv)
int argc;
Char *argv[];
{
  long FORLIM;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  data.f = NULL;
  strcpy(data.name, "data");
  gapp.f = NULL;
  strcpy(gapp.name, "gapp");
  book.f = NULL;
  strcpy(book.name, "book");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  printf("gap %4.2f\n", version);
  if (*data.name != '\0') {
    if (data.f != NULL)
      data.f = freopen(data.name, "w", data.f);
    else
      data.f = fopen(data.name, "w");
  } else {
    if (data.f != NULL)
      rewind(data.f);
    else
      data.f = tmpfile();
  }
  if (data.f == NULL)
    _EscIO2(FileNotFound, data.name);
  SETUPBUF(data.f, Char);
  apiece = (piece *)Malloc(sizeof(piece));
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
  brinit(&book);

  fprintf(data.f, "* gap %4.2f gaps in alignment of:\n", version);
  copyaline(&book, &data);

  orgopen = false;

  maxminalignment(&inst, &book, &fromparam, &toparam);

  if (*gapp.name != '\0') {
    if (gapp.f != NULL)
      gapp.f = freopen(gapp.name, "r", gapp.f);
    else
      gapp.f = fopen(gapp.name, "r");
  } else
    rewind(gapp.f);
  if (gapp.f == NULL)
    _EscIO2(FileNotFound, gapp.name);
  RESETBUF(gapp.f, Char);
  if (!BUFEOF(gapp.f)) {
    fscanf(gapp.f, "%ld%ld%*[^\n]", &fromdo, &todo);
    getc(gapp.f);
    userrange = true;

    if (!BUFEOF(gapp.f)) {
      fscanf(gapp.f, "%c%*[^\n]", &displaylevel);
      getc(gapp.f);
      if (displaylevel == '\n')
	displaylevel = ' ';
    } else
      displaylevel = ' ';

    if (!BUFEOF(gapp.f)) {
      fscanf(gapp.f, "%ld%*[^\n]", &gapminimum);
      getc(gapp.f);
    } else
      gapminimum = 1;

    if (!BUFEOF(gapp.f)) {
      fscanf(gapp.f, "%ld%*[^\n]", &modulus);
      getc(gapp.f);
    } else
      modulus = 1;
  } else {
    fromdo = fromparam;
    todo = toparam;
    userrange = false;
    displaylevel = ' ';
    gapminimum = 1;
    modulus = 1;
  }

  fprintf(data.f, "* The book is from:   %ld to %ld\n", fromparam, toparam);
  fprintf(data.f, "* gaps listed from: %ld to %ld\n", fromdo, todo);
  fprintf(data.f, "*\n");
  fprintf(data.f, "* gap length,");
  if (numbered)
    fprintf(data.f, " piece number,");
  fprintf(data.f, " piece name, zero coordinate\n");

  while (!BUFEOF(book.f)) {
    align(&inst, &book, &apiece, &length_, &alignedbase);
    if (BUFEOF(book.f))
      break;

    gaps = 0;

    FORLIM = todo;
    for (index_ = fromdo; index_ <= FORLIM; index_++) {
      if (!withinalignment(index_, alignedbase, length_))
	gaps++;
    }

    if (gaps >= gapminimum && gaps % modulus == 0) {
      fprintf(data.f, "%10ld", gaps);
      if (numbered)
	fprintf(data.f, " %5ld", number);

      if (displaylevel == 'p') {
	putc(' ', data.f);
	for (index_ = 1; index_ <= namelength; index_++)
	  putc(apiece->key.hea.keynam.letters[index_-1], data.f);
	fprintf(data.f, " %5ld", inttopie(alignedbase, apiece));
      }

      putc('\n', data.f);
    }

    clearpiece(&apiece);
  }
_L1:
  if (inst.f != NULL)
    fclose(inst.f);
  if (book.f != NULL)
    fclose(book.f);
  if (gapp.f != NULL)
    fclose(gapp.f);
  if (data.f != NULL)
    fclose(data.f);
  exit(EXIT_SUCCESS);
}



/* End. */
