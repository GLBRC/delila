/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "frag.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.04
/*





*/
#define updateversion   1.00



/*


























































*/




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

/* p2c: frag.p, line 137:
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



Static _TEXT book, fragp, fraginst, fragfeatures;






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






Static Void writename(afile, s)
_TEXT *afile;
name s;
{
  long i;

  for (i = 0; i < s.length; i++)
    putc(s.letters[i], afile->f);
}



Static Void themain(book, fragp, fraginst, fragfeatures)
_TEXT *book, *fragp, *fraginst, *fragfeatures;
{
  long coordinate, fromrange;
  long fcount = 0;
  long length;
  long pcount = 0;
  double parameterversion;
  long torange, number, skip, moreskip;


  orgkey org;
  boolean orgchange = false, orgopen = false;
  chrkey chr;
  boolean chrchange = false, chropen = false;
  piece *apiece;
  boolean piechange = false, pieopen = false;
  long theline;

  printf("frag %4.2f\n", version);
  if (*fragp->name != '\0') {
    if (fragp->f != NULL)
      fragp->f = freopen(fragp->name, "r", fragp->f);
    else
      fragp->f = fopen(fragp->name, "r");
  } else
    rewind(fragp->f);
  if (fragp->f == NULL)
    _EscIO2(FileNotFound, fragp->name);
  RESETBUF(fragp->f, Char);
  fscanf(fragp->f, "%lg%*[^\n]", &parameterversion);
  getc(fragp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(fragp->f, "%ld%ld%*[^\n]", &fromrange, &torange);
  getc(fragp->f);
  fscanf(fragp->f, "%ld%*[^\n]", &number);
  getc(fragp->f);
  fscanf(fragp->f, "%ld%*[^\n]", &moreskip);
  getc(fragp->f);

  if (fromrange > 0) {
    printf("fromrange must be negative\n");
    halt();
  }

  if (torange < 0) {
    printf("torange must be positive\n");
    halt();
  }

  if (*fraginst->name != '\0') {
    if (fraginst->f != NULL)
      fraginst->f = freopen(fraginst->name, "w", fraginst->f);
    else
      fraginst->f = fopen(fraginst->name, "w");
  } else {
    if (fraginst->f != NULL)
      rewind(fraginst->f);
    else
      fraginst->f = tmpfile();
  }
  if (fraginst->f == NULL)
    _EscIO2(FileNotFound, fraginst->name);
  SETUPBUF(fraginst->f, Char);
  fprintf(fraginst->f, "title \"frag %4.2f\";\n", version);

  if (*fragfeatures->name != '\0') {
    if (fragfeatures->f != NULL)
      fragfeatures->f = freopen(fragfeatures->name, "w", fragfeatures->f);
    else
      fragfeatures->f = fopen(fragfeatures->name, "w");
  } else {
    if (fragfeatures->f != NULL)
      rewind(fragfeatures->f);
    else
      fragfeatures->f = tmpfile();
  }
  if (fragfeatures->f == NULL)
    _EscIO2(FileNotFound, fragfeatures->name);
  SETUPBUF(fragfeatures->f, Char);
  fprintf(fragfeatures->f, "* frag %4.2f\n", version);



  fprintf(fraginst->f, "(*\n");
  fprintf(fraginst->f, "* Fragmentation of book:\n");
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
  copyaline(book, fraginst);
  fprintf(fraginst->f, "fromrange: %5ld\n", fromrange);
  fprintf(fraginst->f, "  torange: %5ld\n", torange);
  fprintf(fraginst->f, "   number: %5ld\n", number);
  fprintf(fraginst->f, "*)\n\n");
  fprintf(fraginst->f, "default out-of-range reduce-range;\n");
  fprintf(fraginst->f, "default numbering piece;\n\n");

  brinit(book, &theline);


  skip = torange - fromrange + moreskip + 1;
  while (!BUFEOF(book->f)) {
    getocp(book, &theline, &org, &orgchange, &orgopen, &chr, &chrchange,
	   &chropen, &apiece, &piechange, &pieopen);
    if (BUFEOF(book->f))
      break;
    pcount++;
    length = piecelength(apiece);
    printf("writing fraginst for piece %ld, length %ld bases\n",
	   pcount, length);


    if (orgchange) {
      fprintf(fraginst->f, "organism ");
      writename(fraginst, org.hea.keynam);
      putc(';', fraginst->f);

      chrchange = true;
    }
    if (chrchange) {
      if (orgchange)
	fprintf(fraginst->f, " \n");
      fprintf(fraginst->f, "chromosome ");
      writename(fraginst, chr.hea.keynam);
      putc(';', fraginst->f);
    }
    if (chrchange || chrchange)
      putc('\n', fraginst->f);

    fprintf(fraginst->f, "piece ");
    writename(fraginst, apiece->key.hea.keynam);
    fprintf(fraginst->f, ";\n");

    /*


*/
    fprintf(fragfeatures->f,
	    "define \"fragment\" \"-\" \"<0>\" \"<0>\" %ld %d %ld\n",
	    fromrange, 0, torange);

    coordinate = 1 - fromrange;
    while (coordinate + torange < length && fcount < number) {
      fcount++;
      fprintf(fraginst->f, "get from %ld %ld to same +%ld; (* %ld *)\n",
	      coordinate, fromrange, torange, fcount);
      fprintf(fragfeatures->f, "@ ");
      writename(fragfeatures, apiece->key.hea.keynam);
      fprintf(fragfeatures->f, " %ld", coordinate);
      fprintf(fragfeatures->f, " +1");
      fprintf(fragfeatures->f, " \"fragment\"");
      fprintf(fragfeatures->f, " \"%ld\"\n", fcount);

      coordinate += skip;
    }



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
  fragfeatures.f = NULL;
  strcpy(fragfeatures.name, "fragfeatures");
  fraginst.f = NULL;
  strcpy(fraginst.name, "fraginst");
  fragp.f = NULL;
  strcpy(fragp.name, "fragp");
  book.f = NULL;
  strcpy(book.name, "book");
  themain(&book, &fragp, &fraginst, &fragfeatures);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (fragp.f != NULL)
    fclose(fragp.f);
  if (fraginst.f != NULL)
    fclose(fraginst.f);
  if (fragfeatures.f != NULL)
    fclose(fragfeatures.f);
  exit(EXIT_SUCCESS);
}



/* End. */
