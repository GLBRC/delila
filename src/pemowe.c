/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "pemowe.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/



#define version         2.20
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

/* p2c: pemowe.p, line 111:
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




typedef Char aminoacid[3];


Static _TEXT book, list;






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



Static Void getaminoacid(position, pie, aa)
long position;
piece *pie;
Char *aa;
{
  /*




*/
  base b0, b1, b2;
  long length;
  boolean done = false;

  length = pietoint(pie->key.pieend, pie);


  if (position < 1 || position > length - 2) {
    switch (pie->key.piecon) {

    case linear:
      memcpy(aa, "   ", sizeof(aminoacid));
      done = true;
      break;

    case circular:
      /*
*/
      while (position < 1)
	position += length;
      while (position > length)
	position -= length;

      b0 = getbase(position, pie);


      position++;
      if (position > length)
	position = 1;
      b1 = getbase(position, pie);

      position++;
      if (position > length)
	position = 1;
      b2 = getbase(position, pie);
      break;
    }
  } else {
    b0 = getbase(position, pie);
    b1 = getbase(position + 1, pie);
    b2 = getbase(position + 2, pie);
  }

  if (done)
    return;
  switch (b0) {

  case t:
    switch (b1) {

    case t:
      switch (b2) {

      case t:
	memcpy(aa, "phe", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "phe", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "leu", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "leu", sizeof(aminoacid));
	break;
      }
      break;

    case c:
      switch (b2) {

      case t:
	memcpy(aa, "ser", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "ser", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "ser", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "ser", sizeof(aminoacid));
	break;
      }
      break;

    case a:
      switch (b2) {

      case t:
	memcpy(aa, "tyr", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "tyr", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "och", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "amb", sizeof(aminoacid));
	break;
      }
      break;

    case g:
      switch (b2) {

      case t:
	memcpy(aa, "cys", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "cys", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "opa", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "trp", sizeof(aminoacid));
	break;
      }
      break;
    }
    break;

  case c:
    switch (b1) {

    case t:
      switch (b2) {

      case t:
	memcpy(aa, "leu", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "leu", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "leu", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "leu", sizeof(aminoacid));
	break;
      }
      break;

    case c:
      switch (b2) {

      case t:
	memcpy(aa, "pro", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "pro", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "pro", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "pro", sizeof(aminoacid));
	break;
      }
      break;

    case a:
      switch (b2) {

      case t:
	memcpy(aa, "his", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "his", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "gln", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "gln", sizeof(aminoacid));
	break;
      }
      break;

    case g:
      switch (b2) {

      case t:
	memcpy(aa, "arg", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "arg", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "arg", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "arg", sizeof(aminoacid));
	break;
      }
      break;
    }
    break;

  case a:
    switch (b1) {

    case t:
      switch (b2) {

      case t:
	memcpy(aa, "ile", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "ile", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "ile", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "met", sizeof(aminoacid));
	break;
      }
      break;

    case c:
      switch (b2) {

      case t:
	memcpy(aa, "thr", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "thr", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "thr", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "thr", sizeof(aminoacid));
	break;
      }
      break;

    case a:
      switch (b2) {

      case t:
	memcpy(aa, "asn", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "asn", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "lys", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "lys", sizeof(aminoacid));
	break;
      }
      break;

    case g:
      switch (b2) {

      case t:
	memcpy(aa, "ser", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "ser", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "arg", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "arg", sizeof(aminoacid));
	break;
      }
      break;
    }
    break;

  case g:
    switch (b1) {

    case t:
      switch (b2) {

      case t:
	memcpy(aa, "val", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "val", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "val", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "val", sizeof(aminoacid));
	break;
      }
      break;

    case c:
      switch (b2) {

      case t:
	memcpy(aa, "ala", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "ala", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "ala", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "ala", sizeof(aminoacid));
	break;
      }
      break;

    case a:
      switch (b2) {

      case t:
	memcpy(aa, "asp", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "asp", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "glu", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "glu", sizeof(aminoacid));
	break;
      }
      break;

    case g:
      switch (b2) {

      case t:
	memcpy(aa, "gly", sizeof(aminoacid));
	break;

      case c:
	memcpy(aa, "gly", sizeof(aminoacid));
	break;

      case a:
	memcpy(aa, "gly", sizeof(aminoacid));
	break;

      case g:
	memcpy(aa, "gly", sizeof(aminoacid));
	break;
      }
      break;
    }
    break;
  }
}



Static double aamw(aa)
Char *aa;
{
  /*


*/
  if (!strncmp(aa, "gly", sizeof(aminoacid)))
    return 75.07;
  if (!strncmp(aa, "ala", sizeof(aminoacid)))
    return 89.10;
  if (!strncmp(aa, "val", sizeof(aminoacid)))
    return 117.15;
  if (!strncmp(aa, "leu", sizeof(aminoacid)))
    return 131.18;
  if (!strncmp(aa, "ile", sizeof(aminoacid)))
    return 131.18;
  if (!strncmp(aa, "ser", sizeof(aminoacid)))
    return 105.10;
  if (!strncmp(aa, "thr", sizeof(aminoacid)))
    return 119.12;
  if (!strncmp(aa, "cys", sizeof(aminoacid)))
    return 121.16;
  if (!strncmp(aa, "met", sizeof(aminoacid)))
    return 149.21;
  if (!strncmp(aa, "asp", sizeof(aminoacid)))
    return 132.11;
  if (!strncmp(aa, "asn", sizeof(aminoacid)))
    return 132.13;
  if (!strncmp(aa, "glu", sizeof(aminoacid)))
    return 146.13;
  if (!strncmp(aa, "gln", sizeof(aminoacid)))
    return 146.15;
  if (!strncmp(aa, "lys", sizeof(aminoacid)))
    return 146.19;
  if (!strncmp(aa, "arg", sizeof(aminoacid)))
    return 174.20;
  if (!strncmp(aa, "his", sizeof(aminoacid)))
    return 155.16;
  if (!strncmp(aa, "phe", sizeof(aminoacid)))
    return 165.19;
  if (!strncmp(aa, "tyr", sizeof(aminoacid)))
    return 181.19;
  if (!strncmp(aa, "trp", sizeof(aminoacid)))
    return 204.23;
  if (!strncmp(aa, "pro", sizeof(aminoacid)))
    return 115.13;
  if (!strncmp(aa, "amb", sizeof(aminoacid)))
    return 0.0;
  if (!strncmp(aa, "och", sizeof(aminoacid)))
    return 0.0;
  if (!strncmp(aa, "opa", sizeof(aminoacid)))
    return 0.0;
  if (!strncmp(aa, "   ", sizeof(aminoacid)))
    return 0.0;
}


#define backbone        9


Static long aaatoms(aa)
Char *aa;
{
  /*









*/
  if (!strncmp(aa, "gly", sizeof(aminoacid)))
    return (backbone + 1);
  if (!strncmp(aa, "ala", sizeof(aminoacid)))
    return (backbone + 4);
  if (!strncmp(aa, "val", sizeof(aminoacid)))
    return (backbone + 10);
  if (!strncmp(aa, "leu", sizeof(aminoacid)))
    return (backbone + 13);
  if (!strncmp(aa, "ile", sizeof(aminoacid)))
    return (backbone + 13);
  if (!strncmp(aa, "ser", sizeof(aminoacid)))
    return (backbone + 5);
  if (!strncmp(aa, "thr", sizeof(aminoacid)))
    return (backbone + 8);
  if (!strncmp(aa, "cys", sizeof(aminoacid)))
    return (backbone + 5);
  if (!strncmp(aa, "met", sizeof(aminoacid)))
    return (backbone + 11);
  if (!strncmp(aa, "asp", sizeof(aminoacid)))
    return (backbone + 6);
  if (!strncmp(aa, "asn", sizeof(aminoacid)))
    return (backbone + 8);
  if (!strncmp(aa, "glu", sizeof(aminoacid)))
    return (backbone + 9);
  if (!strncmp(aa, "gln", sizeof(aminoacid)))
    return (backbone + 11);
  if (!strncmp(aa, "lys", sizeof(aminoacid)))
    return (backbone + 16);
  if (!strncmp(aa, "arg", sizeof(aminoacid)))
    return (backbone + 18);
  if (!strncmp(aa, "his", sizeof(aminoacid)))
    return (backbone + 11);
  if (!strncmp(aa, "phe", sizeof(aminoacid)))
    return (backbone + 14);
  if (!strncmp(aa, "tyr", sizeof(aminoacid)))
    return (backbone + 15);
  if (!strncmp(aa, "trp", sizeof(aminoacid)))
    return (backbone + 18);
  if (!strncmp(aa, "pro", sizeof(aminoacid)))
    return (backbone + 8);
  if (!strncmp(aa, "amb", sizeof(aminoacid)))
    return 0;
  if (!strncmp(aa, "och", sizeof(aminoacid)))
    return 0;
  if (!strncmp(aa, "opa", sizeof(aminoacid)))
    return 0;
  if (!strncmp(aa, "   ", sizeof(aminoacid)))
    return 0;
}

#undef backbone


#define wateratoms      3


Static long peptideatoms(pie, position)
piece *pie;
long position;
{
  /*

*/
  long index = position, totalatoms = 0;
  aminoacid aa;
  long atoms;
  boolean done = false;

  while (!done) {
    getaminoacid(index, pie, aa);
    atoms = aaatoms(aa);
    if (atoms > 0.0) {
      if (totalatoms > 0.0)
	totalatoms -= wateratoms;

      totalatoms += atoms;


      if (pie->key.piecon == circular) {
	if (index > piecelength(pie))
	  index -= piecelength(pie);

	if (index == position)
	  done = true;
      }

    }

    else
      done = true;
    index += 3;
  }
  return totalatoms;
}

#undef wateratoms


#define watermw         18.01534


Static Void peptidemw(pie, position, totalmw, aminoacids)
piece *pie;
long position;
double *totalmw;
long *aminoacids;
{
  /*

*/
  long index = position;
  aminoacid aa;
  double mw;
  boolean done = false;

  *totalmw = 0.0;
  *aminoacids = 0;
  while (!done) {
    getaminoacid(index, pie, aa);
    mw = aamw(aa);
    if (mw > 0.0) {
      if (*totalmw > 0.0)
	*totalmw -= watermw;

      *totalmw += mw;
      (*aminoacids)++;


      if (pie->key.piecon == circular) {
	if (index > piecelength(pie))
	  index -= piecelength(pie);

	if (index == position)
	  done = true;
      }

    }

    else
      done = true;
    index += 3;
  }
}

#undef watermw


#define watermw         18.01534

#define wateratoms      3


Static Void themain()
{
  long aminoacids;
  double mw;
  long atoms;
  double totalmw = watermw;
  long totalatoms = wateratoms;
  piece *pie;

  printf("pemowe %4.2f\n", version);
  brinit(&book);
  pie = (piece *)Malloc(sizeof(piece));

  if (*list.name != '\0') {
    if (list.f != NULL)
      list.f = freopen(list.name, "w", list.f);
    else
      list.f = fopen(list.name, "w");
  } else {
    if (list.f != NULL)
      rewind(list.f);
    else
      list.f = tmpfile();
  }
  if (list.f == NULL)
    _EscIO2(FileNotFound, list.name);
  SETUPBUF(list.f, Char);
  fprintf(list.f, " pemowe %4.2f: peptide molecular weights\n", version);
  putc(' ', list.f);
  copyaline(&book, &list);

  fprintf(list.f,
    "\n only mw from the peptide starting at piece beginning is given.\n");
  fprintf(list.f, " fmet is not used for initial aug or gug codons.\n");
  fprintf(list.f, " source: crc 60th ed. 1980.\n\n");

  while (!BUFEOF(book.f)) {
    getpiece(&book, &pie);
    if (BUFEOF(book.f))
      break;
    peptidemw(pie, 1L, &mw, &aminoacids);
    totalmw += mw - watermw;

    atoms = peptideatoms(pie, 1L);
    totalatoms += atoms - wateratoms;

    fprintf(list.f, " number %4ld (%.*s)  %8.2f dalton",
	    number, namelength, pie->key.hea.keynam.letters, mw);
    fprintf(list.f, ", %8ld atoms %ld amino acids\n", atoms, aminoacids);
    clearpiece(&pie);
  }
  fprintf(list.f, "\n\n %8.2f dalton", totalmw);

  fprintf(list.f, ", %8ld atoms\n", totalatoms);
}

#undef watermw
#undef wateratoms


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  list.f = NULL;
  strcpy(list.name, "list");
  book.f = NULL;
  strcpy(book.name, "book");
  themain();
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
