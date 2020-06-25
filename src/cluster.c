/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "cluster.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/



#define version         5.06
/*
*/



/*

















































































































































*/







#define dnamax          101
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
typedef char dnarange;

/* p2c: cluster.p, line 215:
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







typedef struct bag {
  long leftcoord, rightcoord;
  boolean leftmatch, rightmatch;
  struct bag *nextbag;
} bag;


typedef struct entries {
  long piece_, inst, coordinate;
  struct entries *nextentry;
} entries;


typedef struct numberlist {
  long number;
  struct numberlist *nextlist;
} numberlist;


typedef struct numberpair {
  long leftpiece, rightpiece;
  numberlist *leftseq, *rightseq;
  bag *seqbag;
  struct numberpair *nextpair;
} numberpair;


typedef struct rangerecord {
  long instnumber, singlescount;
  struct rangerecord *nextrange;
} rangerecord;




typedef struct clump {
  numberlist *piecelist;
  numberpair *pairlist;
  struct clump *nextclump;
} clump;








Static line *freeline;
Static dnastring *freedna;

Static boolean readnumber;
/*
*/
Static long number;
Static boolean numbered, skipunnum;
/*
*/





Static _TEXT book, clumps, clusterp;
Static boolean debugging;
Static _TEXT pairs, subind;
Static bag *freebag;
Static clump *freeclump;
Static entries *freeentry;
Static numberpair *freepair;
Static numberlist *freesequence;
Static _TEXT inst;
Static long matchparameter;
Static boolean showfragments, writepairs, writeclumps;


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
/* p2c: cluster.p, line 1072: Note:
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










Static Void writepiename(thefile, pie, namespace)
_TEXT *thefile;
piece *pie;
long namespace;
{
  long index;

  for (index = 1; index <= namespace; index++) {
    if (index <= pie->key.hea.keynam.length)
      putc(pie->key.hea.keynam.letters[index-1], thefile->f);
    else
      putc(' ', thefile->f);
  }
}


#define namespace       10
#define width           10








Static Void writeseqline(fout, apiece, length, alignedbase, fromparam,
			 toparam, number)
_TEXT *fout;
piece *apiece;
long length, alignedbase, fromparam, toparam, number;
{
  long index;

  fprintf(fout->f, "%5ld ", number);


  writepiename(fout, apiece, (long)namespace);
  putc(' ', fout->f);


  fprintf(fout->f, " %*ld ", width, inttopie(1L, apiece));


  for (index = fromparam; index <= toparam; index++) {
    if (withinalignment(index, alignedbase, length))
      fputc(basetochar(getbase(index + alignedbase, apiece)), fout->f);
    else
      putc(' ', fout->f);
  }


  fprintf(fout->f, " %ld\n", inttopie(toparam - fromparam + 1, apiece));


}

#undef namespace
#undef width




Static Void dosequence(apiece, book, inst, piecenumber, alignedbase, length)
piece **apiece;
_TEXT *book, *inst;
long piecenumber, *alignedbase, *length;
{
  boolean foundpiece = false;
  long linenumber;

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


  while (!foundpiece) {
    clearpiece(apiece);
    align(inst, book, apiece, length, alignedbase);
    if (BUFEOF(book->f))
      continue;
    if (numbered) {
      if (number != piecenumber)
	linenumber++;
      else
	foundpiece = true;
      continue;
    }
    if (false)
      break;
    if (piecenumber > number || piecenumber < 1) {
      fprintf(clumps.f, "The requested sequence is not in book range.\n");
      foundpiece = true;
    }
  }
}








Static Void readparameters(clusterp)
_TEXT *clusterp;
{
  Char letter1;

  if (*clusterp->name != '\0') {
    if (clusterp->f != NULL)
      clusterp->f = freopen(clusterp->name, "r", clusterp->f);
    else
      clusterp->f = fopen(clusterp->name, "r");
  } else
    rewind(clusterp->f);
  if (clusterp->f == NULL)
    _EscIO2(FileNotFound, clusterp->name);
  RESETBUF(clusterp->f, Char);
  if (!BUFEOF(clusterp->f)) {
    fscanf(clusterp->f, "%c%*[^\n]", &letter1);
    getc(clusterp->f);
    if (letter1 == '\n')
      letter1 = ' ';
    if (letter1 == 'y')
      debugging = true;
    else
      debugging = false;
    fscanf(clusterp->f, "%c%*[^\n]", &letter1);
    getc(clusterp->f);
    if (letter1 == '\n') {


      letter1 = ' ';
    }
    if (letter1 == 'y')
      showfragments = true;
    else
      showfragments = false;
    fscanf(clusterp->f, "%c%*[^\n]", &letter1);
    getc(clusterp->f);
    if (letter1 == '\n') {


      letter1 = ' ';
    }
    if (letter1 == 'y')
      writepairs = true;
    else
      writepairs = false;
    fscanf(clusterp->f, "%c%*[^\n]", &letter1);
    getc(clusterp->f);
    if (letter1 == '\n') {


      letter1 = ' ';
    }
    if (letter1 == 'y')
      writeclumps = true;
    else
      writeclumps = false;


    fscanf(clusterp->f, "%ld%*[^\n]", &matchparameter);
    getc(clusterp->f);

    return;
  }

  printf("The parameter file clusterp is empty.\n");
  printf("Program halt.\n");
  halt();
}


/* Local variables for writeparameters: */
struct LOC_writeparameters {
  _TEXT *fout;
} ;

Local Void onoff(toggle, LINK)
boolean toggle;
struct LOC_writeparameters *LINK;
{
  if (toggle)
    fprintf(LINK->fout->f, "on\n");
  else
    fprintf(LINK->fout->f, "off\n");
}




Static Void writeparameters(fout_)
_TEXT *fout_;
{
  struct LOC_writeparameters V;


  V.fout = fout_;
  fprintf(V.fout->f, "* debugging ");
  onoff(debugging, &V);


  fprintf(V.fout->f, "* showfragments ");
  onoff(showfragments, &V);


  fprintf(V.fout->f, "* writepairs ");
  onoff(writepairs, &V);


  fprintf(V.fout->f, "* writeclumps ");
  onoff(writeclumps, &V);


  if (matchparameter < 0)
    matchparameter = 0;
  fprintf(V.fout->f, "* matchparameter is set at %ld\n\n", matchparameter);


}







Static Void getwindowsize(fin, windowsize)
_TEXT *fin;
long *windowsize;
{
  Char acharacter;

  do {
    acharacter = getc(fin->f);
    if (acharacter == '\n')
      acharacter = ' ';
  } while (acharacter != '[');

  fscanf(fin->f, "%ld", windowsize);
}




Static Void filehead(fin, fout)
_TEXT *fin, *fout;
{
  long copynumber = 12;

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


  fprintf(fout->f, "cluster %4.2f\n", version);


  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  RESETBUF(fin->f, Char);
  if (copylines(fin, fout, copynumber) != copynumber) {
    fprintf(fout->f, "subind does not have enough header lines\n");
    halt();
  }


  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
}




Static Void readdata(fin, seqnumber, coord, lastchar)
_TEXT *fin;
long *seqnumber, *coord;
Char *lastchar;
{
  skipblanks(fin);


  skipnonblanks(fin);
  skipblanks(fin);



  fscanf(fin->f, "%ld", seqnumber);
  fscanf(fin->f, "%ld", coord);


  while (!P_eoln(fin->f)) {
    *lastchar = getc(fin->f);
    if (*lastchar == '\n')
      *lastchar = ' ';
  }


  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
}








Static Void clearclump(a_)
clump **a_;
{
  clump *WITH;

  WITH = *a_;
  WITH->piecelist = NULL;
  WITH->pairlist = NULL;
  WITH->nextclump = NULL;
}




Static Void clearpair(a_)
numberpair **a_;
{
  numberpair *WITH;

  WITH = *a_;
  WITH->leftpiece = 0;
  WITH->rightpiece = 0;
  WITH->leftseq = NULL;
  WITH->rightseq = NULL;
  WITH->seqbag = NULL;
  WITH->nextpair = NULL;
}




Static Void clearbag(a_)
bag **a_;
{
  bag *WITH;

  WITH = *a_;
  WITH->leftcoord = 0;
  WITH->rightcoord = 0;
  WITH->leftmatch = false;
  WITH->rightmatch = false;
  WITH->nextbag = NULL;
}




Static Void clearentry(a_)
entries **a_;
{
  entries *WITH;

  WITH = *a_;
  WITH->piece_ = 0;
  number = 0;
  WITH->coordinate = 0;
  WITH->nextentry = NULL;
}




Static Void clearrange(a_)
rangerecord **a_;
{
  rangerecord *WITH;

  WITH = *a_;
  number = 0;
  WITH->singlescount = 0;
  WITH->nextrange = NULL;
}




Static Void clearnumberlist(a_)
numberlist **a_;
{
  numberlist *WITH;

  WITH = *a_;
  WITH->number = 0;
  WITH->nextlist = NULL;
}




Static Void getclump(a_)
clump **a_;
{
  if (freeclump == NULL)
    *a_ = (clump *)Malloc(sizeof(clump));
  else {
    *a_ = freeclump;
    freeclump = freeclump->nextclump;
  }
  clearclump(a_);
}




Static Void getpair(a_)
numberpair **a_;
{
  if (freepair == NULL)
    *a_ = (numberpair *)Malloc(sizeof(numberpair));
  else {
    *a_ = freepair;
    freepair = freepair->nextpair;
  }
  clearpair(a_);
}




Static Void getbag(a_)
bag **a_;
{
  if (freebag == NULL)
    *a_ = (bag *)Malloc(sizeof(bag));
  else {
    *a_ = freebag;
    freebag = freebag->nextbag;
  }
  clearbag(a_);
}




Static Void getentry(a_)
entries **a_;
{
  if (freeentry == NULL)
    *a_ = (entries *)Malloc(sizeof(entries));
  else {
    *a_ = freeentry;
    freeentry = freeentry->nextentry;
  }
  clearentry(a_);
}



Static Void getnumberlist(a_)
numberlist **a_;
{
  if (freesequence == NULL)
    *a_ = (numberlist *)Malloc(sizeof(numberlist));
  else {
    *a_ = freesequence;
    freesequence = freesequence->nextlist;
  }
  clearnumberlist(a_);
}




Static Void putclump(a_)
clump *a_;
{
  clearclump(&a_);
  a_->nextclump = freeclump;
  freeclump = a_;
}




Static Void putpair(a_)
numberpair *a_;
{
  clearpair(&a_);
  a_->nextpair = freepair;
  freepair = a_;
}




Static Void putbag(a_)
bag *a_;
{
  clearbag(&a_);
  a_->nextbag = freebag;
  freebag = a_;
}




Static Void putentry(a_)
entries *a_;
{
  clearentry(&a_);
  a_->nextentry = freeentry;
  freeentry = a_;
}




Static Void putnumberlist(a_)
numberlist *a_;
{
  clearnumberlist(&a_);
  a_->nextlist = freesequence;
  freesequence = a_;
}








Static long findranges(instnumber, list)
long instnumber;
rangerecord *list;
{
  long Result;
  rangerecord *current = list;
  boolean foundpiecenumber = false;
  long instcounter = 0, piececounter = 1;

  do {
    if (instnumber <= current->instnumber) {
      foundpiecenumber = true;
      if (instnumber > instcounter + current->singlescount)
	Result = piececounter;
      else
	Result = piececounter - current->singlescount +
		 instnumber - instcounter - 1;
    } else {
      instcounter = current->instnumber;
      current = current->nextrange;
      if (current != NULL)
	piececounter += current->singlescount + 1;
      else
	printf("(would have bombed!)\n");
    }
  } while (!(foundpiecenumber || current == NULL));
  return Result;
}




Static Void addrange(number, piecenumber, numberlist_)
long number, piecenumber;
rangerecord **numberlist_;
{
  rangerecord *current, *last, *next;
  long piececounter = 0;

  if (*numberlist_ == NULL) {
    *numberlist_ = (rangerecord *)Malloc(sizeof(rangerecord));
    clearrange(numberlist_);
    (*numberlist_)->instnumber = number;
    return;
  }
  last = *numberlist_;
  current = *numberlist_;
  next = *numberlist_;


  while (next != NULL) {
    last = current;
    current = next;
    piececounter += next->singlescount + 1;
    next = next->nextrange;
  }


  if (piecenumber == piececounter) {
    current->instnumber = number;
    return;
  }
  if (number - last->instnumber - current->singlescount == 2) {
    current->instnumber = number;
    current->singlescount++;

    return;
  }
  next = (rangerecord *)Malloc(sizeof(rangerecord));
  clearrange(&next);
  next->instnumber = number;
  current->nextrange = next;
}




Static Void createrangelist(book, numberlist_, apiece)
_TEXT *book;
rangerecord **numberlist_;
piece *apiece;
{
  name lastname;
  long piececounter = 0, piecenumber = 0;

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


  while (!BUFEOF(book->f)) {
    getpiece(book, &apiece);
    if (BUFEOF(book->f))
      break;

    if (strncmp(apiece->key.hea.keynam.letters, lastname.letters,
		sizeof(alpha)))
      piecenumber++;

    addrange(number, piecenumber, numberlist_);
    lastname = apiece->key.hea.keynam;
    clearpiece(&apiece);
  }

}








Static boolean overmatched(currentpair, totallength, matchlength)
numberpair *currentpair;
long totallength, matchlength;
{
  long count = 0;
  bag *currentbag;
  numberlist *index;
  long insttotal = 0;
  long matchesperinst, maxmatch;
  bag *WITH;



  index = currentpair->leftseq;
  while (index != NULL) {
    insttotal++;
    index = index->nextlist;
  }


  if (matchparameter > 0)
    matchesperinst = matchparameter;
  else
    matchesperinst = totallength / matchlength;
  maxmatch = insttotal * matchesperinst;


  currentbag = currentpair->seqbag;
  while (currentbag != NULL) {
    WITH = currentbag;
    if (WITH->nextbag != NULL) {
      if (WITH->rightmatch && WITH->nextbag->leftmatch)
	count++;
    }
    if (!(WITH->leftmatch || WITH->rightmatch))
      count++;
    currentbag = currentbag->nextbag;
  }
  return (count > maxmatch);
}




Static Void emptybag(baglist)
bag **baglist;
{
  bag *trashbag;

  while (*baglist != NULL) {
    trashbag = *baglist;
    *baglist = (*baglist)->nextbag;
    trashbag->nextbag = NULL;
    putbag(trashbag);
  }
}








Static Void showRAWbag(fout, pairs)
_TEXT *fout;
numberpair *pairs;
{
  /*
*/
  bag *bagview;
  long leftprev = 0, rightprev = 0;
  bag *WITH;

  bagview = pairs->seqbag;
  while (bagview != NULL) {
    WITH = bagview;

    if (WITH->leftmatch)
      fprintf(fout->f, " L ");
    else
      fprintf(fout->f, " - ");
    fprintf(fout->f, " (%5ld,%5ld)  ", WITH->leftcoord, WITH->rightcoord);
    if (WITH->rightmatch)
      fprintf(fout->f, " R ");
    else
      fprintf(fout->f, " - ");


    if (leftprev != 0)
      fprintf(fout->f, " {%ld} ", WITH->leftcoord - leftprev + 1);
    if (rightprev != 0)
      fprintf(fout->f, " {%ld} ", WITH->rightcoord - rightprev + 1);
    putc('\n', fout->f);


    bagview = bagview->nextbag;
    leftprev = WITH->leftcoord;
    rightprev = WITH->rightcoord;
  }
}




Static Void multi(f, c_, n)
_TEXT *f;
Char c_;
long n;
{
  long i;

  for (i = 1; i <= n; i++)
    putc(c_, f->f);
}


#define width           10


/* Local variables for showbag: */
struct LOC_showbag {
  _TEXT *fout;
  numberpair *pairs;
  bag *abag;
  long half;
} ;


Local Void bar(LINK)
struct LOC_showbag *LINK;
{
  bag *WITH;

  LINK->abag = LINK->pairs->seqbag;
  while (LINK->abag != NULL) {
    WITH = LINK->abag;
    if (!(WITH->leftmatch || WITH->rightmatch))
      multi(LINK->fout, '*', (long)width);
    multi(LINK->fout, '*', (long)width);
    LINK->abag = LINK->abag->nextbag;
  }

  multi(LINK->fout, '*', LINK->half);
  putc('*', LINK->fout->f);
}

Local Void writemid(afile, number, LINK)
_TEXT *afile;
long number;
struct LOC_showbag *LINK;
{
  /*
*/
  long half, numdigits;

  if (number != 0)
    numdigits = (long)(log((double)labs(number)) / log(10.0)) + 1;
  else
    numdigits = 1;
  if (number < 0)
    numdigits++;
  half = (long)((width - numdigits) / 2.0);
  multi(afile, ' ', half);
  fprintf(afile->f, "%*ld", (int)numdigits, number);
  multi(afile, ' ', half);
  multi(afile, ' ', width - half * 2 - numdigits);
}




Static Void showbag(fout_, windowsize, pairs_, apiece)
_TEXT *fout_;
long windowsize;
numberpair *pairs_;
piece *apiece;
{
  struct LOC_showbag V;
  long alignedbase, leftprev, length, rightprev;
  long seqlength = 0;
  bag *WITH;



  V.fout = fout_;
  V.pairs = pairs_;

/* p2c: cluster.p: Note: Eliminated unused assignment statement [338] */
/* p2c: cluster.p: Note: Eliminated unused assignment statement [338] */
  V.half = width / 2;
  bar(&V);
  multi(V.fout, '*', 11L);
  putc('\n', V.fout->f);


  multi(V.fout, ' ', (long)namelength);
  putc('|', V.fout->f);
  V.abag = V.pairs->seqbag;
  while (V.abag != NULL) {
    WITH = V.abag;
    if (WITH->rightmatch)
      writemid(V.fout, V.abag->leftcoord, &V);
    if (WITH->nextbag != NULL) {
      if (WITH->nextbag->leftmatch) {
	seqlength = windowsize + WITH->nextbag->leftcoord - WITH->leftcoord;
	writemid(V.fout, WITH->leftcoord + seqlength - 1, &V);
      }
    }



    if (!(WITH->leftmatch || WITH->rightmatch)) {
      writemid(V.fout, V.abag->leftcoord, &V);
      seqlength = windowsize;
      writemid(V.fout, WITH->leftcoord + seqlength - 1, &V);
    }

    V.abag = V.abag->nextbag;
  }
  putc('\n', V.fout->f);


  dosequence(&apiece, &book, &inst, V.pairs->leftseq->number, &alignedbase,
	     &length);
  fprintf(V.fout->f, "%.*s", namelength, apiece->key.hea.keynam.letters);
  putc('|', V.fout->f);
  multi(V.fout, ' ', V.half);
  V.abag = V.pairs->seqbag;
  while (V.abag != NULL) {
    WITH = V.abag;
    if (WITH->leftmatch) {
      seqlength = windowsize + WITH->leftcoord - leftprev;
      multi(V.fout, '-', (long)width);
    } else if (leftprev != 0)
      writemid(V.fout, WITH->leftcoord - leftprev - seqlength, &V);



    if (!(WITH->leftmatch || WITH->rightmatch)) {
      multi(V.fout, '-', (long)width);
      if (WITH->nextbag != NULL)
	writemid(V.fout,
		 WITH->nextbag->leftcoord - WITH->leftcoord - windowsize, &V);
    }

    if (WITH->rightmatch)
      leftprev = WITH->leftcoord;
    V.abag = V.abag->nextbag;
  }
  putc('\n', V.fout->f);


  multi(V.fout, ' ', (long)namelength);
  putc('|', V.fout->f);
  multi(V.fout, ' ', V.half);
  leftprev = 0;
  if (V.pairs->seqbag == NULL)
    fprintf(V.fout->f, " WARNING: sequence pairs have too many matches.");
  V.abag = V.pairs->seqbag;
  while (V.abag != NULL) {
    WITH = V.abag;
    if (WITH->leftmatch) {
      seqlength = windowsize + WITH->rightcoord - rightprev;
      writemid(V.fout, seqlength, &V);
    } else if (leftprev != 0)
      multi(V.fout, ' ', (long)width);



    if (!(WITH->leftmatch || WITH->rightmatch)) {
      seqlength = windowsize;
      writemid(V.fout, seqlength, &V);
    }


    if (WITH->leftcoord - leftprev != WITH->rightcoord - rightprev &&
	WITH->leftmatch) {
      fprintf(V.fout->f, "(!");
      fprintf(V.fout->f, " %*ld", width, WITH->leftcoord - leftprev + 1);
      fprintf(V.fout->f, " %*ld", width, WITH->rightcoord - rightprev + 1);
      fprintf(V.fout->f, "!)");
    }

    leftprev = WITH->leftcoord;
    rightprev = WITH->rightcoord;
    V.abag = V.abag->nextbag;
  }
  putc('\n', V.fout->f);


  dosequence(&apiece, &book, &inst, V.pairs->rightseq->number, &alignedbase,
	     &length);
  fprintf(V.fout->f, "%.*s", namelength, apiece->key.hea.keynam.letters);

  putc('|', V.fout->f);
  multi(V.fout, ' ', V.half);
  rightprev = 0;
  V.abag = V.pairs->seqbag;
  while (V.abag != NULL) {
    WITH = V.abag;
    if (WITH->leftmatch) {
      seqlength = windowsize + WITH->rightcoord - rightprev;
      multi(V.fout, '-', (long)width);
    } else if (rightprev != 0)
      writemid(V.fout, WITH->rightcoord - rightprev - seqlength, &V);



    if (!(WITH->leftmatch || WITH->rightmatch)) {
      multi(V.fout, '-', (long)width);
      if (WITH->nextbag != NULL)
	writemid(V.fout,
		 WITH->nextbag->rightcoord - WITH->rightcoord - windowsize,
		 &V);
    }

    if (WITH->rightmatch)
      rightprev = WITH->rightcoord;
    V.abag = V.abag->nextbag;
  }
  putc('\n', V.fout->f);


  multi(V.fout, ' ', (long)namelength);
  putc('|', V.fout->f);
  V.abag = V.pairs->seqbag;
  while (V.abag != NULL) {
    WITH = V.abag;
    if (WITH->rightmatch)
      writemid(V.fout, WITH->rightcoord, &V);
    if (WITH->nextbag != NULL) {
      if (WITH->nextbag->leftmatch) {
	seqlength = windowsize + WITH->nextbag->rightcoord - WITH->rightcoord;
	writemid(V.fout, WITH->rightcoord + seqlength - 1, &V);
      }
    }



    if (!(WITH->leftmatch || WITH->rightmatch)) {
      writemid(V.fout, WITH->rightcoord, &V);
      seqlength = windowsize;
      writemid(V.fout, WITH->rightcoord + seqlength - 1, &V);
    }

    V.abag = V.abag->nextbag;
  }
  putc('\n', V.fout->f);

  bar(&V);
  multi(V.fout, '*', 11L);
  fprintf(V.fout->f, "\n\n");
}

#undef width




Static Void showlist(fout, windowsize, totallength, rootpair, apiece)
_TEXT *fout;
long windowsize, totallength;
numberpair **rootpair;
piece *apiece;
{
  numberpair *pair, *WITH;

  pair = *rootpair;
  while (pair != NULL) {
    WITH = pair;
    if ((!showfragments) & overmatched(pair, totallength, windowsize))
      emptybag(&pair->seqbag);
    if (debugging)
      showRAWbag(fout, pair);
    showbag(fout, windowsize, pair, apiece);
    pair = pair->nextpair;
  }
}




Static Void printsequences(apiece, book, inst, fout, alignedbase, length,
			   fromparam, toparam, list1, list2)
piece *apiece;
_TEXT *book, *inst, *fout;
long *alignedbase, *length, *fromparam, *toparam;
numberlist *list1, *list2;
{
  numberlist *current1 = list1, *current2 = list2;

  while (current1 != NULL) {
    dosequence(&apiece, book, inst, current1->number, alignedbase, length);
    writeseqline(fout, apiece, *length, *alignedbase, *fromparam, *toparam,
		 current1->number);
    dosequence(&apiece, book, inst, current2->number, alignedbase, length);
    writeseqline(fout, apiece, *length, *alignedbase, *fromparam, *toparam,
		 current2->number);

    /*
*/
    multi(fout, ' ', *alignedbase + 28);

    fprintf(fout->f, "^\n\n");
    current1 = current1->nextlist;
    current2 = current2->nextlist;
  }
}




Static Void showclumpedlist(fout, clumproot, windowsize, totallength,
			    fromparam, toparam, apiece)
_TEXT *fout;
clump **clumproot;
long windowsize, totallength, fromparam, toparam;
piece *apiece;
{
  /*
*/
  long alignedbase;

  clump *clump_;
  numberlist *clumpseq;
  numberpair *clumppair;

  long length;
  clump *WITH;

  clump_ = *clumproot;
  while (clump_ != NULL) {
    WITH = clump_;

    multi(fout, '#', 70L);
    putc('\n', fout->f);
    multi(fout, '#', 70L);
    fprintf(fout->f, "\nThe related pieces in this clump are: \n");


    clumpseq = WITH->piecelist;
    while (clumpseq != NULL) {
      fprintf(fout->f, "%ld", clumpseq->number);
      if (clumpseq->nextlist != NULL)
	putc(',', fout->f);
      clumpseq = clumpseq->nextlist;
    }
    fprintf(fout->f, "\nTheir pairings are as follows: \n\n");


    clumppair = clump_->pairlist;
    while (clumppair != NULL) {
      if ((!showfragments) & overmatched(clumppair, totallength, windowsize))
	emptybag(&clumppair->seqbag);
      showbag(fout, windowsize, clumppair, apiece);


      printsequences(apiece, &book, &inst, fout, &alignedbase, &length,
		     &fromparam, &toparam, clumppair->leftseq,
		     clumppair->rightseq);

      clumppair = clumppair->nextpair;
    }


    putc('\n', fout->f);
    multi(fout, '#', 70L);
    putc('\n', fout->f);
    multi(fout, '#', 70L);
    fprintf(fout->f, "\n\n\n");


    clump_ = clump_->nextclump;
  }
}




Static Void fullinsert(pair, clumproot)
numberpair **pair;
clump **clumproot;
{
  /*
*/
  clump *newclump;
  numberlist *newpiece;

  getclump(&newclump);


  getnumberlist(&newpiece);
  getnumberlist(&newpiece->nextlist);
  newpiece->number = (*pair)->leftpiece;
  newpiece->nextlist->number = (*pair)->rightpiece;
  newclump->piecelist = newpiece;


  newclump->pairlist = *pair;


  newclump->nextclump = *clumproot;
  *clumproot = newclump;
}




Static Void insertclump(pair, currentclump, leftfound, rightfound)
numberpair **pair;
clump **currentclump;
boolean leftfound, rightfound;
{
  numberlist *newpiece;

  if (leftfound == rightfound)
    return;

  getnumberlist(&newpiece);
  if (leftfound)
    newpiece->number = (*pair)->rightpiece;
  if (rightfound)
    newpiece->number = (*pair)->leftpiece;
  newpiece->nextlist = (*currentclump)->piecelist;
  (*currentclump)->piecelist = newpiece;


  (*pair)->nextpair = (*currentclump)->pairlist;
  (*currentclump)->pairlist = *pair;
}




Static Void clumppairs(pair, root)
numberpair **pair;
clump **root;
{
  boolean clumpdone;
  clump *currentclump;
  numberlist *currentpiece;

  boolean leftfound = false;

  boolean rightfound = false;

  boolean piecedone;
  numberlist *WITH;

  currentclump = *root;
  clumpdone = (*root == NULL);
  while (!clumpdone) {
    leftfound = false;
    rightfound = false;
    piecedone = false;
    currentpiece = currentclump->piecelist;
    while (!piecedone) {
      WITH = currentpiece;
      if ((*pair)->leftpiece == WITH->number)
	leftfound = true;
      if ((*pair)->rightpiece == WITH->number)
	rightfound = true;
      piecedone = (WITH->nextlist == NULL || leftfound && rightfound);
      if (!piecedone)
	currentpiece = currentpiece->nextlist;
    }
    insertclump(pair, &currentclump, leftfound, rightfound);
    currentclump = currentclump->nextclump;
    clumpdone = (currentclump == NULL || leftfound || rightfound);
  }
  if (currentclump == NULL && !(leftfound || rightfound))
    fullinsert(pair, root);
}




Static Void removebag(prior, defendent)
bag **prior, **defendent;
{
  /*
*/
  if ((*defendent)->leftcoord == (*prior)->leftcoord + 1 &&
      (*defendent)->rightcoord == (*prior)->rightcoord + 1) {
    (*prior)->rightmatch = true;
    (*defendent)->leftmatch = true;
  }


  if (!((*defendent)->leftmatch && (*defendent)->rightmatch))
    return;
  (*prior)->nextbag = (*defendent)->nextbag;
  (*defendent)->nextbag = NULL;
  putbag(*defendent);
}




Static Void baginsert(root, coord1, coord2)
bag **root;
long *coord1, *coord2;
{
  bag *currentbag;

  boolean done = false;

  bag *lastlastbag;

  bag *lastbag;

  bag *newbag;

  getbag(&newbag);


  newbag->leftcoord = *coord1;
  newbag->rightcoord = *coord2;
  currentbag = *root;
  lastbag = *root;
  lastlastbag = *root;
  while (!done) {
    if (currentbag == NULL) {
      if (*root == NULL) {
	*root = newbag;
	lastbag = *root;
      } else
	lastbag->nextbag = newbag;
      continue;
    }
/* p2c: cluster.p: Note: Eliminated unused assignment statement [338] */
    if (currentbag->leftcoord < *coord1 && currentbag->rightcoord < *coord2) {
      lastlastbag = lastbag;
      lastbag = currentbag;
      currentbag = currentbag->nextbag;
      continue;
    }
    newbag->nextbag = currentbag;
    if (currentbag == *root) {
      *root = newbag;
      lastbag = *root;
    } else
      lastbag->nextbag = newbag;
    done = true;
  }


  if (currentbag != NULL)
    removebag(&newbag, &currentbag);
  if (newbag != *root)
    removebag(&lastbag, &newbag);
  if (lastbag != *root)
    removebag(&lastlastbag, &lastbag);
}




Static Void insertinst(apair, firstinst, secondinst)
numberpair **apair;
long firstinst, secondinst;
{
  boolean insert;
  boolean done = false;
  numberlist *lastleft, *lastright, *left, *newlist1, *newlist2, *right;

  left = (*apair)->leftseq;
  right = (*apair)->rightseq;
  lastleft = (*apair)->leftseq;
  lastright = (*apair)->rightseq;


  while (!done) {
    insert = (left == NULL);
    if (!insert)
      insert = (firstinst <= left->number);
    if (!insert) {
      lastleft = left;
      lastright = right;
      left = left->nextlist;
      right = right->nextlist;

      continue;
    }

    getnumberlist(&newlist1);
    newlist1->number = firstinst;
    getnumberlist(&newlist2);
    newlist2->number = secondinst;

    if (left == NULL) {
      if ((*apair)->leftseq != NULL) {
	lastleft->nextlist = newlist1;
	lastright->nextlist = newlist2;
      }

      else {
	(*apair)->leftseq = newlist1;
	(*apair)->rightseq = newlist2;
      }
    } else if (firstinst != left->number && secondinst != right->number) {


      if (left != lastleft) {
	newlist1->nextlist = lastleft->nextlist;
	lastleft->nextlist = newlist1;
	newlist2->nextlist = lastright->nextlist;
	lastright->nextlist = newlist2;
      }

      else {
	newlist1->nextlist = (*apair)->leftseq;
	(*apair)->leftseq = newlist1;
	newlist2->nextlist = (*apair)->rightseq;
	(*apair)->rightseq = newlist2;
      }
    } else {

      putnumberlist(newlist1);
      putnumberlist(newlist2);

    }
    done = true;
  }

}




Static Void pairinsert(root, first, second)
numberpair **root;
entries **first, **second;
{
  numberpair *current;
  boolean foundone = false;
  numberpair *WITH;

  if ((*first)->piece_ == (*second)->piece_)
    return;


  current = *root;
  while (!foundone) {
    if (current != NULL) {
      if (current->leftpiece == (*first)->piece_ &&
	  current->rightpiece == (*second)->piece_)
	foundone = true;
      else
	current = current->nextpair;
      continue;
    }

    getpair(&current);
    if (*root != NULL)
      current->nextpair = *root;
    *root = current;
    foundone = true;
  }


  WITH = current;

  WITH->leftpiece = (*first)->piece_;
  WITH->rightpiece = (*second)->piece_;


  if (current->leftseq == NULL)
    insertinst(&current, (*first)->inst, (*second)->inst);
  else if (writeclumps)
    insertinst(&current, (*first)->inst, (*second)->inst);


  baginsert(&WITH->seqbag, &(*first)->coordinate, &(*second)->coordinate);
}




Static Void completelist(piecenumber, pieceinst, instcoord, lastchar,
			 entryroot, pairroot)
long piecenumber, pieceinst, instcoord;
Char lastchar;
entries **entryroot;
numberpair **pairroot;
{
  entries *current, *lastentry, *newentry;

  getentry(&newentry);


  newentry->piece_ = piecenumber;
  newentry->inst = pieceinst;
  newentry->coordinate = instcoord;
  newentry->nextentry = NULL;
  if (lastchar != '*') {
    while (*entryroot != NULL) {
      lastentry = *entryroot;
      *entryroot = (*entryroot)->nextentry;
      putentry(lastentry);
    }
  }


  current = *entryroot;
  while (current != NULL) {
    lastentry = current;


    if (lastentry->piece_ < newentry->piece_)
      pairinsert(pairroot, &lastentry, &newentry);
    else
      pairinsert(pairroot, &newentry, &lastentry);

    current = current->nextentry;
  }
  if (*entryroot != NULL)
    lastentry->nextentry = newentry;
  else
    *entryroot = newentry;
}




Static Void pairclumps(pairroot, clumproot)
numberpair **pairroot;
clump **clumproot;
{
  numberpair *pair;

  while (*pairroot != NULL) {
    pair = *pairroot;
    *pairroot = pair->nextpair;
    pair->nextpair = NULL;


    clumppairs(&pair, clumproot);
  }


}




Static Void themain(clusterp, subind, inst, book, pairs, clumps)
_TEXT *clusterp, *subind, *inst, *book, *pairs, *clumps;
{
  piece *apiece;

  clump *clumproot = NULL;
  long coord = 0;

  entries *entryroot = NULL;

  long fromparam = 0;

  long instnumber = 0;

  Char lastchar = ' ';
  rangerecord *numberlist_ = NULL;

  numberpair *pairroot = NULL;

  long toparam = 0, totallength = 0;

  long windowsize = 0;
  _TEXT TEMP;


  printf("cluster %4.2f\n", version);


  brinit(book);
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
  apiece = (piece *)Malloc(sizeof(piece));


  freebag = NULL;
  freeclump = NULL;
  freeentry = NULL;
  freepair = NULL;
  freesequence = NULL;
  matchparameter = 0;


  if (*subind->name != '\0') {
    if (subind->f != NULL)
      subind->f = freopen(subind->name, "r", subind->f);
    else
      subind->f = fopen(subind->name, "r");
  } else
    rewind(subind->f);
  if (subind->f == NULL)
    _EscIO2(FileNotFound, subind->name);
  RESETBUF(subind->f, Char);
  getwindowsize(subind, &windowsize);


  createrangelist(book, &numberlist_, apiece);


  /*


*/
  readparameters(clusterp);


  if (writepairs)
    filehead(subind, pairs);
  if (writeclumps)
    filehead(subind, clumps);


  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeparameters(&TEMP);
  if (writepairs)
    writeparameters(pairs);
  if (writeclumps)
    writeparameters(clumps);


  maxminalignment(inst, book, &fromparam, &toparam);
  totallength = toparam - fromparam + 1;


  while (!BUFEOF(subind->f)) {
    readdata(subind, &instnumber, &coord, &lastchar);


    completelist(findranges(instnumber, numberlist_), instnumber, coord,
		 lastchar, &entryroot, &pairroot);
  }





  if (writepairs) {
    printf("* about to print output file pairs\n");
    showlist(pairs, windowsize, totallength, &pairroot, apiece);
  }


  if (writeclumps)
    pairclumps(&pairroot, &clumproot);


  if (writeclumps) {
    printf("* about to print output file clumps\n");
    showclumpedlist(clumps, &clumproot, windowsize, totallength, fromparam,
		    toparam, apiece);
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
  subind.f = NULL;
  strcpy(subind.name, "subind");
  pairs.f = NULL;
  strcpy(pairs.name, "pairs");
  clusterp.f = NULL;
  strcpy(clusterp.name, "clusterp");
  clumps.f = NULL;
  strcpy(clumps.name, "clumps");
  book.f = NULL;
  strcpy(book.name, "book");
  themain(&clusterp, &subind, &inst, &book, &pairs, &clumps);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (clumps.f != NULL)
    fclose(clumps.f);
  if (clusterp.f != NULL)
    fclose(clusterp.f);
  if (pairs.f != NULL)
    fclose(pairs.f);
  if (subind.f != NULL)
    fclose(subind.f);
  if (inst.f != NULL)
    fclose(inst.f);
  exit(EXIT_SUCCESS);
}



/* End. */
