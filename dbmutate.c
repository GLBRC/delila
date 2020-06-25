/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dbmutate.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.91
/*













*/
#define updateversion   1.00



/*















































































































































































*/



#define sequencemax     500000L

#define deffromrange    (-50)
#define deftorange      50





#define dnamax          10000000L

#define namelength      100
#define linelength      80





#define changesetmax    20
#define insertmax       100



#define insertupperbits  (-0.1)
#define insertlowerbits  (-1.3)
#define deleteupperbits  (-0.1)
#define deletelowerbits  (-1.3)
#define changeupperbits  (-1.3)
#define changelowerbits  (-11.3)

#define displacement    0



#define maxstring       150



#define fillermax       50




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



/*


*/
typedef Char filler[fillermax];



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;



typedef struct changedata {
  Char changetype, baseold, basenew;
  double basecoo1, basecoo2;
  long inserts;
  Char insert[insertmax];
} changedata;

typedef struct changeset {
  changedata data[changesetmax];
  long number;
} changeset;





typedef struct cystem {

  double lower, upper;
  struct cystem *next;
} cystem;


/*

*/




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
typedef long dnarange;

/* p2c: dbmutate.p, line 351:
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



typedef struct asequence {
  long length;
  Char sequence[sequencemax];
} asequence;


Static _TEXT dbin, dbout, dbmutatep, markspots, minst;

Static Char cha;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}






Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}



Static Void fillstring(s, a_)
stringDelila *s;
Char *a_;
{
  /*
*/


  /*


*/
  long length = fillermax;
  long index;

  clearstring(s);
  while (length > 1 && a_[length-1] == ' ')
    length--;
  if (length == 1 && a_[length-1] == ' ') {
    printf("fillstring: the string is empty\n");
    halt();
  }

  for (index = 0; index < length; index++)
    s->letters[index] = a_[index];
  s->length = length;
  s->current = 1;
}



Static Void filltrigger(t_, a_)
trigger *t_;
Char *a_;
{
  fillstring(&t_->seek, a_);
}



/*




*/

Static Void resettrigger(t_)
trigger *t_;
{
  t_->state = 0;
  t_->skip = false;
  t_->found = false;
}


Static Void testfortrigger(ch, t_)
Char ch;
trigger *t_;
{
  /*









*/
  t_->state++;
  /*



*/
  if (t_->seek.letters[t_->state - 1] == ch) {
    t_->skip = false;
    if (t_->state == t_->seek.length)
      t_->found = true;
    else
      t_->found = false;
    return;
  }
  /*
*/
  if (t_->seek.letters[0] == ch) {
    t_->state = 1;
    t_->skip = false;
    t_->found = false;
    return;
  }
  t_->state = 0;
  t_->skip = true;
  t_->found = false;
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


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}






Static boolean equalstring(a_, b)
stringDelila a_, b;
{
  long index;
  boolean equal;

  if (a_.length == b.length) {
    index = 1;
    do {
      equal = (a_.letters[index-1] == b.letters[index-1]);
      index++;
    } while (equal && index <= a_.length);
    return equal;
  } else
    return false;
}



Static Void onetoken(afile, buffer, gotten)
_TEXT *afile;
stringDelila *buffer;
boolean *gotten;
{
  /*
*/
  long index = 0;
  boolean done = false;

  skipblanks(afile);
  clearstring(buffer);
  if (BUFEOF(afile->f)) {
    *gotten = false;
    return;
  }
  while (!P_eoln(afile->f) && index < maxstring && !done) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
    if (buffer->letters[index-1] == ' ') {
      done = true;
      index--;
    }
  }

  buffer->length = index;
  buffer->current = 1;
  *gotten = true;
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



Static Void copyline(fin, fout)
_TEXT *fin, *fout;
{
  /*

*/
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
}









/*











*/



Static Void clearcystem(l, freecystem)
cystem **l, *freecystem;
{
  cystem *lptr;

  if (*l == NULL)
    return;
  lptr = *l;
  (*l)->lower = 0.0;
  (*l)->upper = 0.0;
  *l = (*l)->next;
  lptr->next = freecystem;
  freecystem = lptr;
}


Static Void disposecystem(l)
cystem **l;
{
  cystem *hold;

  while (*l != NULL) {
    hold = (*l)->next;
    Free(*l);
    *l = hold;
  }
}


Static Void getcystem(l, freecystem)
cystem **l, *freecystem;
{
  if (freecystem != NULL) {
    *l = freecystem;
    freecystem = freecystem->next;
  } else
    *l = (cystem *)Malloc(sizeof(cystem));
  (*l)->lower = 0.0;
  (*l)->upper = 0.0;
  (*l)->next = NULL;
}


Static Void startcystem(c_, l, u, freecystem)
cystem **c_;
long l, u;
cystem **freecystem;
{
  cystem *WITH;

  getcystem(c_, *freecystem);
  WITH = *c_;
  WITH->lower = l;
  WITH->upper = u;
}


Static Void showcystem(f, r)
_TEXT *f;
double r;
{
  if (r - (long)r > 0)
    fprintf(f->f, "%1.1f", r);
  else
    fprintf(f->f, "%ld", (long)floor(r + 0.5));
}


Static Void writecystem(f, c_)
_TEXT *f;
cystem *c_;
{
  cystem *p = c_;

  while (p != NULL) {
    putc('(', f->f);
    if (p->lower == p->upper)
      showcystem(f, p->lower);
    else {
      showcystem(f, p->lower);
      putc(' ', f->f);
      showcystem(f, p->upper);
    }
    putc(')', f->f);
    /*





*/

    p = p->next;
    if (p == c_) {
      printf("circular\n");
      halt();
    }
  }
}


Static boolean insidecystem(l, c_)
double l;
cystem *c_;
{
  return (c_->lower <= l && l <= c_->upper);
}


Static cystem *locatecystem(c_, l)
cystem **c_;
long l;
{
  boolean done = false;
  cystem *p;
  _TEXT TEMP;

  /*

*/
  p = *c_;
  while (!done) {
    if (insidecystem((double)l, p)) {
      done = true;
      break;
    }
    p = p->next;
    if (p->next == NULL)
      done = true;
  }
  if (p != NULL)
    return p;
  printf("cannot locate coordinate %ld in ", l);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writecystem(&TEMP, *c_);
  putchar('\n');
  halt();
  return p;
}


Static Void findcystem(c_, l, k, location)
cystem **c_;
long l;
Char *k;
cystem **location;
{
  /*





*/
  boolean done = false;

  *k = '?';
  if (l < (*c_)->lower) {
    *k = 'b';
    *location = NULL;
    return;
  }
  *k = 'f';
  *location = *c_;
  while (!done) {
    if (insidecystem((double)l, *location))
      break;
    if ((*location)->next != NULL) {
      *location = (*location)->next;
/* p2c: dbmutate.p: Note: Eliminated unused assignment statement [338] */

      continue;
    }
    if (l > (*location)->upper)
      *k = 'a';
    else {
      *k = 'm';
      *location = NULL;
    }
    done = true;
  }
}


Static Void deletecystem(c_, l, u, freecystem)
cystem **c_;
long l, u;
cystem **freecystem;
{
  cystem *p, *plower, *pupper, *pnewer, *holder;
  _TEXT TEMP;

  printf("deleting from ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  showcystem(&TEMP, (double)l);
  printf(" to ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  showcystem(&TEMP, (double)u);
  printf(": ");

  plower = locatecystem(c_, l);
  pupper = locatecystem(c_, u);

  if (plower == pupper) {
    if (plower->lower == l && pupper->upper == u) {
      printf(" = =!");

      if (plower == *c_) {
	holder = *c_;
	*c_ = (*c_)->next;
	clearcystem(&holder, *freecystem);
	return;
      }
      p = *c_;
      while (p->next->lower != l)
	p = p->next;
      holder = p->next;
      p->next = p->next->next;
      clearcystem(&holder, *freecystem);
      return;
    }
    if (plower->lower != l && pupper->upper == u) {
      printf("<> = ");
      pupper->upper = l - 1.0;
      return;
    }
    if (plower->lower == l && pupper->upper != u) {
      printf(" =<>!");
      return;
    }
    if (plower->lower == l || pupper->upper == u)
      return;

    /*












*/
    printf("<><> ");

    getcystem(&pnewer, *freecystem);
    pnewer->next = pupper->next;
    plower->next = pnewer;
    pnewer->lower = u + 1.0;
    pnewer->upper = pupper->upper;
    plower->upper = l - 1.0;
    return;
  }
  printf("     ");

  p = plower->next;
  while (p != pupper) {
    holder = p;
    p = p->next;
    clearcystem(&holder, *freecystem);
  }
  plower->upper = l - 1.0;
  pupper->lower = u + 1.0;
}


Static Void checkcystem(c_, spot)
cystem *c_;
long spot;
{
  Char k;
  cystem *location;
  _TEXT TEMP;

  findcystem(&c_, spot, &k, &location);
  printf("@ %ld k=%c ", spot, k);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  /*

*/
  writecystem(&TEMP, location);
  putchar('\n');
}



Static Void testcystem(f)
_TEXT *f;
{
  /*


*/
  cystem *freecystem = NULL, *c_ = NULL;

  fprintf(f->f, "test of cystem -------------------------------------\n");
  startcystem(&c_, 10L, 99L, &freecystem);
  writecystem(f, c_);
  putc('\n', f->f);
  deletecystem(&c_, 11L, 29L, &freecystem);
  writecystem(f, c_);
  putc('\n', f->f);
  deletecystem(&c_, 51L, 59L, &freecystem);
  writecystem(f, c_);
  putc('\n', f->f);
  deletecystem(&c_, 71L, 79L, &freecystem);
  writecystem(f, c_);
  putc('\n', f->f);
  deletecystem(&c_, 66L, 89L, &freecystem);
  writecystem(f, c_);
  putc('\n', f->f);
  deletecystem(&c_, 41L, 50L, &freecystem);
  writecystem(f, c_);
  putc('\n', f->f);
  deletecystem(&c_, 30L, 40L, &freecystem);
  writecystem(f, c_);
  putc('\n', f->f);
  checkcystem(c_, 9L);
  checkcystem(c_, 10L);
  checkcystem(c_, 11L);
  checkcystem(c_, 100L);
  checkcystem(c_, 65L);
  checkcystem(c_, 66L);
  disposecystem(&c_);
  disposecystem(&freecystem);

  /*




*/
  fprintf(f->f, "test of cystem DONE --------------------------------\n");

  /*

*/
}








/*
*/
Static long pietoint(p, pie)
long p;
piece *pie;
{
  piece *dummy = pie;

  return p;
}


Static long inttopie(i, pie)
long i;
piece *pie;
{
  piece *dummy = pie;

  return i;
}


Static long piecelength(pie)
piece *pie;
{
  return 0;
}



Static Void nextnonblank(f)
_TEXT *f;
{
  boolean done = false;

  while (!done) {
    skipblanks(f);


    if (P_eoln(f->f)) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
    } else
      done = true;

    if (BUFEOF(f->f)) {
      printf("unexpected end of file found in dbmutatep\n");
      printf("you are missing part of an instruction\n");
      halt();
    }
  }
}


Static Void readchanges(f, c_)
_TEXT *f;
changeset *c_;
{
  changedata *WITH;

  WITH = &c_->data[c_->number - 1];
  WITH->changetype = 'c';
  nextnonblank(f);
  WITH->baseold = getc(f->f);
  if (WITH->baseold == '\n')
    WITH->baseold = ' ';
  nextnonblank(f);
  fscanf(f->f, "%lg", &WITH->basecoo1);
  WITH->basecoo2 = WITH->basecoo1;
  if (P_eoln(f->f)) {
    printf(": to make a change, a new base is required\n");
    halt();
  }

  nextnonblank(f);
  WITH->basenew = getc(f->f);

  if (WITH->basenew == '\n')
    WITH->basenew = ' ';
  if (WITH->baseold != 't' && WITH->baseold != 'g' && WITH->baseold != 'c' &&
      WITH->baseold != 'a')
    printf("\nWARNING: old base usually should be a, c, g, t\n");
  if (WITH->basenew != 't' && WITH->basenew != 'g' && WITH->basenew != 'c' &&
      WITH->basenew != 'a')
    printf("\n WARNING: new base usually should be a, c, g, t\n");
  WITH->inserts = 0;
}


Static Void readinsertion(f, c_)
_TEXT *f;
changeset *c_;
{
  Char comma;
  changedata *WITH;

  WITH = &c_->data[c_->number - 1];
  WITH->changetype = getc(f->f);
  if (WITH->changetype == '\n')
    WITH->changetype = ' ';
  fscanf(f->f, "%lg", &WITH->basecoo1);
  skipblanks(f);
  comma = getc(f->f);
  if (comma == '\n')
    comma = ' ';
  if (comma != ',') {
    printf(" comma expected between coordinates for insertion\n");
    halt();
  }
  fscanf(f->f, "%lg", &WITH->basecoo2);
  if (WITH->basecoo1 < 0)
    WITH->basecoo1 = 0.0;
  if (WITH->basecoo2 < 1)
    WITH->basecoo2 = 1.0;
  WITH->inserts = 0;
  while (P_peek(f->f) == 't' || P_peek(f->f) == 'g' || P_peek(f->f) == 'c' ||
	 P_peek(f->f) == 'a') {
    WITH->inserts++;
    if (WITH->inserts > insertmax) {
      printf(
	" no more than %ld insertion bases allowed, increase constant insertmax\n",
	(long)insertmax);
      halt();
    }
    WITH->insert[WITH->inserts - 1] = getc(f->f);
    if (WITH->insert[WITH->inserts - 1] == '\n')
      WITH->insert[WITH->inserts - 1] = ' ';
  }
  /*










*/
}


Static Void readdeletion(f, c_)
_TEXT *f;
changeset *c_;
{
  Char comma;
  changedata *WITH;

  WITH = &c_->data[c_->number - 1];
  WITH->changetype = getc(f->f);
  if (WITH->changetype == '\n')
    WITH->changetype = ' ';
  fscanf(f->f, "%lg", &WITH->basecoo1);
  skipblanks(f);
  comma = getc(f->f);
  if (comma == '\n')
    comma = ' ';
  if (comma != ',') {
    printf("comma expected between coordinates for deletion\n");
    halt();
  }
  fscanf(f->f, "%lg", &WITH->basecoo2);
  if (WITH->basecoo1 < 1)
    WITH->basecoo1 = 1.0;
  if (WITH->basecoo2 < 1)
    WITH->basecoo2 = 1.0;
  WITH->inserts = 0;
  if (WITH->basecoo1 > WITH->basecoo2) {
    printf(
      " the first base, %ld, must be less thanor equal to the second base, %ld for deletion\n",
      (long)floor(WITH->basecoo1 + 0.5), (long)floor(WITH->basecoo2 + 0.5));
    halt();
  }
}


Static Void readchangeset(changes, c_)
_TEXT *changes;
changeset *c_;
{
  boolean done = false;

  c_->number = 0;
  while (!P_eoln(changes->f) && !done) {
    skipblanks(changes);
    if (P_peek(changes->f) == ';')
      done = true;
    else if (P_peek(changes->f) == '.') {

      getc(changes->f);
    } else {
      c_->number++;
      if (c_->number > changesetmax) {
	printf("Too many changes requested, increase changesetmax.");
	halt();
      }
      if (P_peek(changes->f) == 't' || P_peek(changes->f) == 'g' ||
	  P_peek(changes->f) == 'c' || P_peek(changes->f) == 'a')
	readchanges(changes, c_);
      else if (P_peek(changes->f) == 'i')
	readinsertion(changes, c_);
      else if (P_peek(changes->f) == 'd')
	readdeletion(changes, c_);
      else {
	printf(" change must be identified by one of: acgtdi\n");
	printf(" the illegal change character \"%c\" was found\n",
	       P_peek(changes->f));
	halt();
      }
    }
    skipblanks(changes);
  }
}


Static Void writechangeset(f, changes)
_TEXT *f;
changeset changes;
{
  long i, n, FORLIM;
  changedata *WITH;
  long FORLIM1;

  FORLIM = changes.number;
  for (n = 1; n <= FORLIM; n++) {
    if (n > 1)
      putc('.', f->f);
    WITH = &changes.data[n-1];
    switch (WITH->changetype) {

    case 'c':
      fprintf(f->f, "%c%ld%c",
	      WITH->baseold, (long)floor(WITH->basecoo1 + 0.5),
	      WITH->basenew);
      break;

    case 'i':
      fprintf(f->f, "i%ld,%ld",
	      (long)floor(WITH->basecoo1 + 0.5),
	      (long)floor(WITH->basecoo2 + 0.5));
      FORLIM1 = WITH->inserts;
      for (i = 0; i < FORLIM1; i++)
	putc(WITH->insert[i], f->f);
      break;

    case 'd':
      fprintf(f->f, "d%ld,%ld",
	      (long)floor(WITH->basecoo1 + 0.5),
	      (long)floor(WITH->basecoo2 + 0.5));
      break;
    }
  }
}


Static Void describechangeset(f, changes)
_TEXT *f;
changeset changes;
{
  long i, n, FORLIM;
  changedata *WITH;
  long FORLIM1;

  if (changes.number == 0) {
    fprintf(f->f, "no changes");
    return;
  }
  FORLIM = changes.number;
  for (n = 1; n <= FORLIM; n++) {
    if (n > 1)
      fprintf(f->f, ", ");
    WITH = &changes.data[n-1];
    switch (WITH->changetype) {

    case 'c':
      fprintf(f->f, "at %ld %c->%c",
	      (long)floor(WITH->basecoo1 + 0.5), WITH->baseold,
	      WITH->basenew);
      break;

    case 'i':
      fprintf(f->f, "insert ");
      FORLIM1 = WITH->inserts;
      for (i = 0; i < FORLIM1; i++)
	putc(WITH->insert[i], f->f);
      fprintf(f->f, " between %ld and %ld",
	      (long)floor(WITH->basecoo1 + 0.5),
	      (long)floor(WITH->basecoo2 + 0.5));
      break;

    case 'd':
      fprintf(f->f, "delete %ld to %ld",
	      (long)floor(WITH->basecoo1 + 0.5),
	      (long)floor(WITH->basecoo2 + 0.5));
      break;
    }
  }
}




Static Void marksautomate(markspots)
_TEXT *markspots;
{
  fprintf(markspots->f,
	  "* markspots: define markings for the lister program\n");
  fprintf(markspots->f,
	  "* The standard marks.arrow must be used prior to this file.\n\n");
  fprintf(markspots->f, "u\n");
  fprintf(markspots->f, "/setmarkspotarrow{\n");
  fprintf(markspots->f, "/bodycolor {black} def\n");
  fprintf(markspots->f, "/strokecolor {black} def\n");
  fprintf(markspots->f, "/BodyThick     0.30 fs def\n");
  fprintf(markspots->f, "/HeadWidth     0.90 fs def\n");
  fprintf(markspots->f, "/HeadLength    1.50 fs def\n");
  fprintf(markspots->f, "} def\n");
  fprintf(markspots->f, "setmarkspotarrow\n\n");
  fprintf(markspots->f,
	  "/change   {%% tailx taily headx heady shift change\n");
  fprintf(markspots->f, "%% the head of an arrow\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "setmarkspotarrow\n");
  fprintf(markspots->f, "fixedarrow\n");
  fprintf(markspots->f, "} def\n\n");
  fprintf(markspots->f,
	  "/changeworra{%% tailx taily headx heady shift changeworra\n");
  fprintf(markspots->f,
	  "%% the tail of an arrow is a 'worra' (spelling backards)\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "setmarkspotarrow\n");
  fprintf(markspots->f, "fixedworra\n");
  fprintf(markspots->f, "} def\n\n");
  fprintf(markspots->f,
	  "/insertion{%% tailx taily headx heady shift insertion\n");
  fprintf(markspots->f, "%% an insertion is a green rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "/bodycolor {lightgreen} def\n");
  fprintf(markspots->f, "boundrectangle\n");
  fprintf(markspots->f, "} def\n\n");
  fprintf(markspots->f,
	  "/deletion {%% tailx taily headx heady shift deletion\n");
  fprintf(markspots->f, "%% a deletion is a red rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "/bodycolor {lightred} def\n");
  fprintf(markspots->f, "boundrectangle\n");
  fprintf(markspots->f, "} def\n\n");

  fprintf(markspots->f, "/doubleY{%% tailx taily headx heady shift doubleY\n");
  fprintf(markspots->f,
	  "%% Two Y's connected at their bases indicate insertion\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "/bodycolor {lightgreen} def\n");
  fprintf(markspots->f, "fixeddoubleY\n");
  fprintf(markspots->f, "%% NOT IMPLEMENTED\n");
  fprintf(markspots->f, "} def\n\n");
  fprintf(markspots->f, "!\n\n");
}


typedef long position;


/* Local variables for sortchanges: */
struct LOC_sortchanges {
  changeset *sorted;
} ;

Local boolean lessthan(a_, b, LINK)
long a_, b;
struct LOC_sortchanges *LINK;
{
  return (LINK->sorted->data[a_-1].basecoo1 < LINK->sorted->data[b-1].basecoo1);
}

Local Void swap_(a_, b, LINK)
long a_, b;
struct LOC_sortchanges *LINK;
{
  changedata hold;

  hold = LINK->sorted->data[a_-1];
  LINK->sorted->data[a_-1] = LINK->sorted->data[b-1];
  LINK->sorted->data[b-1] = hold;

}

Local Void quicksort(left, right, LINK)
position left, right;
struct LOC_sortchanges *LINK;
{
  /*











*/
  position lower = left;
  position upper, center;

  center = (left + right) / 2;
  upper = right;
  do {
    while (lessthan(lower, center, LINK))
      lower++;
    while (lessthan(center, upper, LINK))
      upper--;
    if (lower <= upper) {
      if (lower == center)
	center = upper;
      else if (upper == center)
	center = lower;
      swap_(lower, upper, LINK);
      lower++;
      upper--;
    }

  } while (lower <= upper);
  if (left < upper)
    quicksort(left, upper, LINK);
  if (lower < right)
    quicksort(lower, right, LINK);
}



Static Void sortchanges(unsorted, sorted_)
changeset unsorted, *sorted_;
{
  /*

*/
  struct LOC_sortchanges V;

  V.sorted = sorted_;
  *V.sorted = unsorted;
  quicksort(1L, V.sorted->number, &V);
}


/* Local variables for propagatechanges: */
struct LOC_propagatechanges {
  piece *pie;
  long shift;
} ;

Local Void shiftit(x, LINK)
changedata *x;
struct LOC_propagatechanges *LINK;
{
  x->basecoo1 = inttopie(
      pietoint((long)floor(x->basecoo1 + 0.5), LINK->pie) + LINK->shift,
      LINK->pie);
  x->basecoo2 = inttopie(
      pietoint((long)floor(x->basecoo2 + 0.5), LINK->pie) + LINK->shift,
      LINK->pie);


}



Static Void propagatechanges(cin, cout, wildtype, pie_)
changeset cin, *cout;
boolean wildtype;
piece *pie_;
{
  /*


*/
  struct LOC_propagatechanges V;
  long m, n, location, FORLIM;
  changedata *WITH1;
  long FORLIM1;
  changedata *WITH2;

  V.pie = pie_;
  /*
*/
  *cout = cin;
  FORLIM = cout->number;
  for (m = 1; m <= FORLIM; m++) {
    location = pietoint((long)floor(cout->data[m-1].basecoo1 + 0.5), V.pie);


    WITH1 = &cout->data[m-1];
    switch (WITH1->changetype) {

    case 'c':
      break;

    case 'i':
      V.shift = pietoint((long)floor(WITH1->basecoo2 + 0.5), V.pie) - pietoint(
		  (long)floor(WITH1->basecoo1 + 0.5),
		  V.pie) - WITH1->inserts - 1;

      if (wildtype) {
	FORLIM1 = cout->number;
	for (n = m; n < FORLIM1; n++) {
	  WITH2 = &cout->data[n];
	  if (location < pietoint((long)floor(cout->data[n].basecoo1 + 0.5),
				  V.pie))
	    shiftit(&cout->data[n], &V);

	}
      } else {
	V.shift = -V.shift;
	for (n = 0; n <= m - 2; n++) {
	  WITH2 = &cout->data[n];
	  if (location < pietoint((long)floor(cout->data[n].basecoo1 + 0.5),
				  V.pie))
	    shiftit(&cout->data[n], &V);
	}
      }
      break;

    case 'd':
      V.shift = pietoint((long)floor(WITH1->basecoo2 + 0.5), V.pie) -
		pietoint((long)floor(WITH1->basecoo1 + 0.5), V.pie) + 1;

      if (wildtype) {
	FORLIM1 = cout->number;
	for (n = m; n < FORLIM1; n++) {
	  WITH2 = &cout->data[n];
	  if (location <= pietoint((long)floor(cout->data[n].basecoo1 + 0.5),
				   V.pie))
	    shiftit(&cout->data[n], &V);

	}
      }

      else {
	V.shift = -V.shift;
	for (n = 0; n <= m - 2; n++) {
	  WITH2 = &cout->data[n];
	  if (location <= pietoint((long)floor(cout->data[n].basecoo1 + 0.5),
				   V.pie))
	    shiftit(&cout->data[n], &V);

	}
      }
      break;

    }
  }
}



Static long nwpietoint(p, pie)
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
    i = p - WITH->piebeg + 1;
    break;

  case minus:
    i = WITH->piebeg - p + 1;
    break;
  }
  /*







*/

  return i;
}


#define decbase         2
#define widbase         6
#define decbits         2
#define widbits         6

#define blackbar        0.05

#define shiftdown       1



Static Void writemarks(markspots, changes, insertupperbits_, insertlowerbits_,
		       deleteupperbits_, deletelowerbits_, changeupperbits_,
		       changelowerbits_, displacement_, pie, thenumber)
_TEXT *markspots;
changeset changes;
double insertupperbits_, insertlowerbits_, deleteupperbits_, deletelowerbits_,
       changeupperbits_, changelowerbits_, displacement_;
piece *pie;
long thenumber;
{
  /*
*/
  /*






*/
  long i, n;
  double markplace, protection;
  /*

*/
  changeset sorted;
  /*

*/
  changeset unsorted;
  long FORLIM;
  changedata *WITH;
  long FORLIM1;

  fprintf(markspots->f, "\n* piece #%ld ", thenumber);
  writechangeset(markspots, changes);
  putc('\n', markspots->f);


  propagatechanges(changes, &unsorted, true, pie);

  FORLIM = unsorted.number;

  for (n = 0; n < FORLIM; n++) {
    WITH = &unsorted.data[n];
    switch (WITH->changetype) {

    case 'c':
      /* blank case */
      break;

    case 'i':
      protection = blackbar;
      WITH->basecoo1 += 0.5 - protection;
      WITH->basecoo2 += protection - 0.5;
      break;

    case 'd':
      /*
*/
      WITH->basecoo1 -= 0.5;
      WITH->basecoo2 += 0.5;
      break;
    }
  }
  sortchanges(unsorted, &sorted);


  FORLIM = sorted.number;

  for (n = 0; n < FORLIM; n++) {
    WITH = &sorted.data[n];
    switch (WITH->changetype) {

    case 'c':
      markplace = WITH->basecoo1 - displacement_;
      WITH->basecoo1 = markplace;
      fprintf(markspots->f,
	      "U %*.*f %*.*f %*.*f %*.*f %*.*f (%c->%c) changeworra \n",
	      widbits, decbits, WITH->basecoo1, widbits, decbits,
	      changeupperbits_ - shiftdown, widbits, decbits, WITH->basecoo2,
	      widbits, decbits, changelowerbits_ - shiftdown, widbits,
	      decbits, displacement_, WITH->baseold, WITH->basenew);
      break;

    case 'i':
      /*
*/
      if (nwpietoint((long)WITH->basecoo1, pie) == 0) {
	WITH->basecoo1 += 1.0;
	WITH->basecoo2 += 1.0;
	displacement_ = -1.8;

      }

      if (nwpietoint((long)WITH->basecoo1, pie) > piecelength(pie)) {
	WITH->basecoo1 -= 1.0;
	WITH->basecoo2 -= 1.0;
	displacement_ = 0.0;

      }


      fprintf(markspots->f,
	      "U %*.*f %*.*f %*.*f %*.*f %*.*f (insert) deletion\n",
	      widbase, decbase, WITH->basecoo1, widbits, decbits,
	      deleteupperbits_, widbase, decbase, WITH->basecoo2, widbits,
	      decbits, deletelowerbits_, widbits, decbits, displacement_);
      break;
      /*
*/

    case 'd':
      fprintf(markspots->f, "U %*.*f %*.*f %*.*f %*.*f %*.*f (deletion",
	      widbase, decbase, WITH->basecoo1, widbits, decbits,
	      deleteupperbits_, widbase, decbase, WITH->basecoo2, widbits,
	      decbits, deletelowerbits_, widbits, decbits, displacement_);
      fprintf(markspots->f, ") deletion \n");
      break;
      /*
*/
    }
  }
  fprintf(markspots->f, "p - skip ahead to mutated piece");
  fprintf(markspots->f, " #%ld\n", thenumber + 1);

  propagatechanges(changes, &unsorted, false, pie);

  FORLIM = unsorted.number;

  for (n = 0; n < FORLIM; n++) {
    WITH = &unsorted.data[n];
    switch (WITH->changetype) {

    case 'c':
      markplace = WITH->basecoo1 - displacement_;
      WITH->basecoo1 = markplace;
      WITH->basecoo2 = markplace;
      break;

    case 'i':
      /*


*/
      if (WITH->inserts <= 0)
	protection = blackbar;
      else
	protection = 0.0;
      markplace = WITH->basecoo1 + 0.5;
      WITH->basecoo1 = markplace - protection;
      WITH->basecoo2 = markplace + WITH->inserts + protection;
      break;

    case 'd':
      markplace = WITH->basecoo1;
      protection = blackbar;
      WITH->basecoo1 = markplace - 0.5 - protection;
      WITH->basecoo2 = markplace - 0.5 + protection;
      break;
    }
  }
  sortchanges(unsorted, &sorted);

  FORLIM = sorted.number;

  for (n = 0; n < FORLIM; n++) {
    WITH = &sorted.data[n];
    switch (WITH->changetype) {

    case 'c':
      fprintf(markspots->f,
	      "U %*.*f %*.*f %*.*f %*.*f %*.*f (%c->%c) change \n",
	      widbits, decbits, WITH->basecoo1, widbits, decbits,
	      changeupperbits_, widbits, decbits, WITH->basecoo2, widbits,
	      decbits, changelowerbits_, widbits, decbits, displacement_,
	      WITH->baseold, WITH->basenew);
      break;

    case 'i':
      /*
*/
      if (nwpietoint((long)WITH->basecoo1, pie) == 0) {
	WITH->basecoo1 += 1.0;
	WITH->basecoo2 += 1.0;
	displacement_ = -1.8;

      }

      if (nwpietoint((long)WITH->basecoo1, pie) > piecelength(pie)) {
	printf("nwpietoint(trunc(basecoo1),pie) = %ld\n",
	       nwpietoint((long)WITH->basecoo1, pie));
	printf("nwpietoint(trunc(basecoo2),pie) = %ld\n",
	       nwpietoint((long)WITH->basecoo2, pie));
	printf("piecelength(pie) = %ld\n", piecelength(pie));
	WITH->basecoo1 -= 1.0;
	WITH->basecoo2 -= 1.0;
	displacement_ = 0.0;

      }


      fprintf(markspots->f, "U %*.*f %*.*f %*.*f %*.*f %*.*f (",
	      widbase, decbase, WITH->basecoo1, widbits, decbits,
	      insertupperbits_, widbase, decbase, WITH->basecoo2, widbits,
	      decbits, insertlowerbits_, widbits, decbits, displacement_);
      /*
*/
      if (WITH->inserts == 0)
	putc(' ', markspots->f);
      else {
	FORLIM1 = WITH->inserts;
	for (i = 0; i < FORLIM1; i++)
	  putc(WITH->insert[i], markspots->f);
      }
      fprintf(markspots->f, ") insertion\n");
      break;
      /*


*/

    case 'd':
      fprintf(markspots->f,
	      "U %*.*f %*.*f %*.*f %*.*f %*.*f (delete) deletion\n",
	      widbase, decbase, WITH->basecoo1, widbits, decbits,
	      deleteupperbits_, widbase, decbase, WITH->basecoo2, widbits,
	      decbits, deletelowerbits_, widbits, decbits, displacement_);
      break;
    }
  }
  fprintf(markspots->f, "p - skip ahead to next piece\n");
}

#undef decbase
#undef widbase
#undef decbits
#undef widbits
#undef blackbar
#undef shiftdown



Static Void processcommand(dbmutatep, fromrange, torange)
_TEXT *dbmutatep;
long *fromrange, *torange;
{
  Char command;

  command = getc(dbmutatep->f);
  if (command == '\n')
    command = ' ';
  skipblanks(dbmutatep);
  command = getc(dbmutatep->f);
  if (command == '\n')
    command = ' ';
  skipnonblanks(dbmutatep);
  printf("@ command: ");
  if (command == 't' || command == 'f') {
    switch (command) {

    case 'f':
      fscanf(dbmutatep->f, "%ld", fromrange);
      printf("fromrange set to %ld", *fromrange);
      break;

    case 't':
      fscanf(dbmutatep->f, "%ld", torange);
      printf("torange set to %ld", *torange);
      break;
    }
  } else
    printf("unidentifed command, \",command,\", ignored");
  putchar('\n');
  fscanf(dbmutatep->f, "%*[^\n]");
  getc(dbmutatep->f);
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *minst;
  trigger accession;
  changeset changes;
  trigger entryname, entryend;
  boolean gotten;
  trigger locus, origin, organism;
} ;

Local Void readsequence(f, s, LINK)
_TEXT *f;
asequence *s;
struct LOC_themain *LINK;
{
  Char c_;
  boolean done = false;
  long sindex = 0;
  _TEXT TEMP;

  resettrigger(&LINK->entryend);
  while (!done) {
    if (BUFEOF(f->f)) {
      printf("No end to entry ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &LINK->entryname.seek);
      printf("!\n");
      halt();
    }
    if (P_eoln(f->f)) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
      continue;
    }
    c_ = getc(f->f);
    if (c_ == '\n')
      c_ = ' ';
    testfortrigger(c_, &LINK->entryend);
    if (LINK->entryend.found) {
      done = true;
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
      continue;
    }
    /*

*/

    if (c_ < 'a' || c_ > 'z')
      continue;
    /*

*/
    sindex++;
    if (sindex > sequencemax) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &LINK->entryname.seek);
      printf(" sequence length is larger than the allowed %ld.\n",
	     sequencemax);
      printf("Increase constant sequencemax.\n");
      halt();
    }
    s->sequence[sindex-1] = c_;

  }
  s->length = sindex;
}

Local Void writesequence(f, s, LINK)
_TEXT *f;
asequence s;
struct LOC_themain *LINK;
{
  long sindex = 0;



  while (sindex < s.length) {
    sindex++;
    if (sindex % 60 == 1)
      fprintf(f->f, "%9ld", sindex);
    if (sindex % 10 == 1)
      putc(' ', f->f);
    putc(s.sequence[sindex-1], f->f);
    /*






*/
    if (sindex % 60 == 0)
      putc('\n', f->f);
  }
  if (sindex % 60 != 1) {
    putc('\n', f->f);
    /*


*/
  }
  fprintf(f->f, "//\n\n");
}

Local Void changesequence(changes, thesequence, LINK)
changeset changes;
asequence *thesequence;
struct LOC_themain *LINK;
{
  long i, n, shift, FORLIM;
  changedata *WITH1;
  long FORLIM1;

  FORLIM = changes.number;
  for (n = 0; n < FORLIM; n++) {
    WITH1 = &changes.data[n];
    /*











*/

    switch (WITH1->changetype) {

    case 'c':
      if (WITH1->baseold == WITH1->basenew) {
	printf("The initial and final bases are the same,\n");
	printf("so you did not request any change!\n");
	halt();
      }
      if (WITH1->baseold !=
	  thesequence->sequence[(long)floor(WITH1->basecoo1 + 0.5) - 1]) {
	printf("The old base at %ld is NOT %c!  It is %c.\n",
	       (long)floor(WITH1->basecoo1 + 0.5), WITH1->baseold,
	       thesequence->sequence[(long)floor(WITH1->basecoo1 + 0.5) - 1]);
	halt();
      }
      thesequence->sequence[(long)floor(WITH1->basecoo1 + 0.5) - 1] = WITH1->
								      basenew;
      break;

    case 'i':
      if (WITH1->basecoo1 > thesequence->length)
	WITH1->basecoo1 = thesequence->length;
      if (WITH1->basecoo2 > thesequence->length)
	WITH1->basecoo2 = thesequence->length + 1.0;


      shift = WITH1->inserts - (long)floor(WITH1->basecoo2 + 0.5) +
	      (long)floor(WITH1->basecoo1 + 0.5) + 1;
      /*


*/
      thesequence->length += shift;



      if (shift > 0) {
	if (thesequence->length > sequencemax) {
	  printf(
	    " Insertion of %ld bases would cause the sequence to exceed constant sequencemax (%ld)\n",
	    WITH1->inserts, sequencemax);
	  halt();
	}

	FORLIM1 = (long)floor(WITH1->basecoo2 + 0.5) - 1;
	for (i = thesequence->length - 1; i >= FORLIM1; i--)
	  thesequence->sequence[i] = thesequence->sequence[i - shift];
      }



      else if (shift < 0) {


	FORLIM1 = thesequence->length;
	for (i = (long)floor(WITH1->basecoo2 + 0.5) + shift - 1; i < FORLIM1; i++)
	  thesequence->sequence[i] = thesequence->sequence[i - shift];
      }
      FORLIM1 = WITH1->inserts;


      for (i = 0; i < FORLIM1; i++)
	thesequence->sequence[(long)floor(WITH1->basecoo1 + 0.5) + i] =
	  WITH1->insert[i];

      /*



*/


      break;

    case 'd':
      /*



*/
      if (WITH1->basecoo1 < 0)
	WITH1->basecoo1 = 1.0;
      if (WITH1->basecoo2 < 0)
	WITH1->basecoo2 = 1.0;
      if (WITH1->basecoo1 > thesequence->length)
	WITH1->basecoo1 = thesequence->length + 1.0;
      if (WITH1->basecoo2 > thesequence->length)
	WITH1->basecoo2 = thesequence->length + 1.0;
      shift = (long)floor(WITH1->basecoo2 + 0.5) -
	      (long)floor(WITH1->basecoo1 + 0.5) + 1;
      thesequence->length -= shift;
      FORLIM1 = thesequence->length;
      for (i = (long)floor(WITH1->basecoo1 + 0.5) - 1; i < FORLIM1; i++)
	thesequence->sequence[i] = thesequence->sequence[i + shift];
      break;

    }
  }
}

Local Void startinst(minst, version_, LINK)
_TEXT *minst;
double version_;
struct LOC_themain *LINK;
{
  if (*minst->name != '\0') {
    if (minst->f != NULL)
      minst->f = freopen(minst->name, "w", minst->f);
    else
      minst->f = fopen(minst->name, "w");
  } else {
    if (minst->f != NULL)
      rewind(minst->f);
    else
      minst->f = tmpfile();
  }
  if (minst->f == NULL)
    _EscIO2(FileNotFound, minst->name);
  SETUPBUF(minst->f, Char);
  fprintf(minst->f, "title \"dbmutate %4.2f\";\n", version_);
  printf("title \"dbmutate %4.2f\";\n", version_);
}

/* Local variables for instorgchr: */
struct LOC_instorgchr {
  struct LOC_themain *LINK;
  _TEXT *minst;
  stringDelila organismgenus, organismspecies;
} ;

Local Void givename(LINK)
struct LOC_instorgchr *LINK;
{
  putc(LINK->organismgenus.letters[0], LINK->minst->f);
  putc('.', LINK->minst->f);
  writestring(LINK->minst, &LINK->organismspecies);
  fprintf(LINK->minst->f, ";\n");
}

Local Void instorgchr(minst_, organismgenus_, organismspecies_, LINK)
_TEXT *minst_;
stringDelila organismgenus_, organismspecies_;
struct LOC_themain *LINK;
{
  struct LOC_instorgchr V;

  V.LINK = LINK;
  V.minst = minst_;
  V.organismgenus = organismgenus_;
  V.organismspecies = organismspecies_;
  if (LINK->changes.number <= 0)
    return;
  fprintf(V.minst->f, "\norganism ");
  givename(&V);
  fprintf(V.minst->f, "chromosome ");
  givename(&V);
  putc('\n', V.minst->f);
}

/* Local variables for instget: */
struct LOC_instget {
  struct LOC_themain *LINK;
  _TEXT *minst;
  changeset changes;
  long fromrange, torange;
} ;

Local Void sign(minst, range, LINK)
_TEXT *minst;
long range;
struct LOC_instget *LINK;
{
  if (range > 0)
    putc('+', minst->f);
}

Local Void dotheget(LINK)
struct LOC_instget *LINK;
{
  fprintf(LINK->minst->f, "get from % .1E ", LINK->changes.data[0].basecoo1);
  sign(LINK->minst, LINK->fromrange, LINK);
  fprintf(LINK->minst->f, "%ld to same ", LINK->fromrange);
  sign(LINK->minst, LINK->torange, LINK);
  fprintf(LINK->minst->f, "%ld;\n", LINK->torange);
}

Local Void instget(minst_, piecename, changes_, fromrange_, torange_, LINK)
_TEXT *minst_;
stringDelila piecename;
changeset changes_;
long fromrange_, torange_;
struct LOC_themain *LINK;
{
  /*

*/
  struct LOC_instget V;

  V.LINK = LINK;
  V.minst = minst_;
  V.changes = changes_;
  V.fromrange = fromrange_;
  V.torange = torange_;
  if (V.changes.number <= 0)
    return;
  fprintf(V.minst->f, "(* ");
  describechangeset(V.minst, V.changes);
  fprintf(V.minst->f, " *)\n");

  fprintf(V.minst->f, "name \"wildtype\";\n");

  fprintf(V.minst->f, "piece ");
  writestring(V.minst, &piecename);
  fprintf(V.minst->f, "; (* wild type *)\n");

  dotheget(&V);

  fprintf(V.minst->f, "name \"");
  writestring(V.minst, &piecename);
  putc(' ', V.minst->f);
  describechangeset(V.minst, V.changes);
  fprintf(V.minst->f, "\";\n");

  fprintf(V.minst->f, "piece ");
  writestring(V.minst, &piecename);
  putc('.', V.minst->f);
  writechangeset(V.minst, V.changes);
  fprintf(V.minst->f, "; (* mutant *)\n");

  dotheget(&V);
}

Local Void dolocus(dbin, dbout, markspots, entryname, changes, found,
		   fromrange, torange, thenumber, LINK)
_TEXT *dbin, *dbout, *markspots;
trigger entryname;
changeset changes;
boolean *found;
long *fromrange, *torange, *thenumber;
struct LOC_themain *LINK;
{
  /*
*/
  stringDelila anaccessionname, alocusname;
  Char c_;
  long entrylength;
  _TEXT holdfile;
  stringDelila organismgenus, organismspecies;
  boolean done = false;
  asequence sequence;
  changeset sortedchanges;
  piece *pie, *dummy;
  _TEXT TEMP;

  holdfile.f = NULL;
  *holdfile.name = '\0';
  if (*holdfile.name != '\0') {
    if (holdfile.f != NULL)
      holdfile.f = freopen(holdfile.name, "w", holdfile.f);
    else
      holdfile.f = fopen(holdfile.name, "w");
  } else {
    if (holdfile.f != NULL)
      rewind(holdfile.f);
    else
      holdfile.f = tmpfile();
  }
  if (holdfile.f == NULL)
    _EscIO2(FileNotFound, holdfile.name);
  SETUPBUF(holdfile.f, Char);


  onetoken(dbin, &alocusname, &LINK->gotten);
  fscanf(dbin->f, "%ld", &entrylength);

  clearstring(&organismgenus);
  clearstring(&organismspecies);


  resettrigger(&LINK->entryend);
  *found = false;
  while (!done) {
    if (P_eoln(dbin->f)) {
      fscanf(dbin->f, "%*[^\n]");
      getc(dbin->f);
      putc('\n', holdfile.f);
      if (BUFEOF(dbin->f))
	done = true;

    } else {
      c_ = getc(dbin->f);
      if (c_ == '\n')
	c_ = ' ';
      putc(c_, holdfile.f);


      testfortrigger(c_, &LINK->entryend);
      if (LINK->entryend.found)
	done = true;

      testfortrigger(c_, &LINK->accession);
      if (LINK->accession.found) {
	onetoken(dbin, &anaccessionname, &LINK->gotten);
	if (LINK->gotten) {
	  /*


*/
	  done = true;
	  if (equalstring(anaccessionname, entryname.seek)) {
	    *found = true;

	    fprintf(holdfile.f, "   ");
	    writestring(&holdfile, &entryname.seek);

	    if (changes.number > 0)
	      putc('.', holdfile.f);
	    writechangeset(&holdfile, changes);

	    if (changes.number > 0) {
	      fprintf(markspots->f, "\n* dbmutatep requested piece %ld\n",
		      *thenumber);
	      fprintf(markspots->f, "* final display pieces %ld and %ld\n",
		      *thenumber * 2 - 3, *thenumber * 2 - 2);
	      fprintf(markspots->f, "* ");
	      writestring(markspots, &entryname.seek);
	      putc('\n', markspots->f);
	    } else {
	      fprintf(markspots->f, "\n* The wild type piece in dbmutatep, ");
	      writestring(markspots, &entryname.seek);
	      fprintf(markspots->f, " does not have marks in markspots\n");
	    }

	    pie = NULL;
	    dummy = pie;
	    writemarks(markspots, sortedchanges, insertupperbits,
		       insertlowerbits, deleteupperbits, deletelowerbits,
		       changeupperbits, changelowerbits, (double)displacement,
		       pie, *thenumber);


	    putc(' ', holdfile.f);
	    copyaline(dbin, &holdfile);


	    resettrigger(&LINK->organism);
	    done = false;
	    while (!done) {
	      if (BUFEOF(dbin->f)) {
		printf("No ");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writestring(&TEMP, &LINK->organism.seek);
		printf(" to entry ");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writestring(&TEMP, &entryname.seek);
		printf("!\n");
		halt();
	      }
	      if (P_eoln(dbin->f)) {
		fscanf(dbin->f, "%*[^\n]");
		getc(dbin->f);
		putc('\n', holdfile.f);
		continue;
	      }
	      c_ = getc(dbin->f);
	      if (c_ == '\n')
		c_ = ' ';
	      putc(c_, holdfile.f);
	      testfortrigger(c_, &LINK->organism);
	      if (!LINK->organism.found)
		continue;
	      done = true;
	      onetoken(dbin, &organismgenus, &LINK->gotten);
	      if (!LINK->gotten) {
		copyaline(dbin, &holdfile);
		continue;
	      }
	      onetoken(dbin, &organismspecies, &LINK->gotten);
	      if (!LINK->gotten)
		continue;
	      /*





*/
	      fprintf(holdfile.f, "  ");
	      writestring(&holdfile, &organismgenus);
	      putc(' ', holdfile.f);
	      writestring(&holdfile, &organismspecies);
	      instorgchr(LINK->minst, organismgenus, organismspecies, LINK);

	    }

	    instget(LINK->minst, entryname.seek, changes, *fromrange,
		    *torange, LINK);


	    resettrigger(&LINK->origin);
	    done = false;
	    while (!done) {
	      if (BUFEOF(dbin->f)) {
		printf("No ");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writestring(&TEMP, &LINK->origin.seek);
		printf(" to entry ");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writestring(&TEMP, &entryname.seek);
		printf("!\n");
		halt();
	      }
	      if (P_eoln(dbin->f)) {
		fscanf(dbin->f, "%*[^\n]");
		getc(dbin->f);
		putc('\n', holdfile.f);
		continue;
	      }
	      c_ = getc(dbin->f);
	      if (c_ == '\n')
		c_ = ' ';
	      putc(c_, holdfile.f);
	      testfortrigger(c_, &LINK->origin);
	      if (LINK->origin.found) {
		done = true;
		copyaline(dbin, &holdfile);
	      }
	    }

	    readsequence(dbin, &sequence, LINK);
	    changesequence(changes, &sequence, LINK);




	    writestring(dbout, &LINK->locus.seek);
	    fprintf(dbout->f, "       ");
	    writestring(dbout, &alocusname);
	    fprintf(dbout->f, " %6ld", sequence.length);

	    if (*holdfile.name != '\0') {
	      if (holdfile.f != NULL)
		holdfile.f = freopen(holdfile.name, "r", holdfile.f);
	      else
		holdfile.f = fopen(holdfile.name, "r");
	    } else
	      rewind(holdfile.f);
	    if (holdfile.f == NULL)
	      _EscIO2(FileNotFound, holdfile.name);
	    RESETBUF(holdfile.f, Char);
	    while (!BUFEOF(holdfile.f))
	      copyaline(&holdfile, dbout);


	    writesequence(dbout, sequence, LINK);

	  }
	}
	/*



*/

      }
    }

    resettrigger(&entryname);
    testfortrigger(c_, &entryname);

    if (BUFEOF(dbin->f))
      done = true;
  }

  if (!*found) {
    /*
*/
    resettrigger(&LINK->entryend);
    done = false;
    while (!done) {
      if (BUFEOF(dbin->f)) {
	printf("No end to entry ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &entryname.seek);
	printf("!\n");
	halt();
      }
      if (P_eoln(dbin->f)) {
	fscanf(dbin->f, "%*[^\n]");
	getc(dbin->f);
	continue;
      }
      c_ = getc(dbin->f);
      if (c_ == '\n')
	c_ = ' ';
      testfortrigger(c_, &LINK->entryend);
      if (LINK->entryend.found) {
	done = true;
	fscanf(dbin->f, "%*[^\n]");
	getc(dbin->f);
      }
    }
  }
  if (holdfile.f != NULL)
    fclose(holdfile.f);
}

Local Void locateentry(dbin, dbout, markspots, entryname, changes, fromrange,
		       torange, thenumber, LINK)
_TEXT *dbin, *dbout, *markspots;
trigger entryname;
changeset changes;
long *fromrange, *torange, *thenumber;
struct LOC_themain *LINK;
{
  boolean done = false;
  Char c_;
  _TEXT TEMP;

  if (*dbin->name != '\0') {
    if (dbin->f != NULL)
      dbin->f = freopen(dbin->name, "r", dbin->f);
    else
      dbin->f = fopen(dbin->name, "r");
  } else
    rewind(dbin->f);
  if (dbin->f == NULL)
    _EscIO2(FileNotFound, dbin->name);
  RESETBUF(dbin->f, Char);
  resettrigger(&LINK->locus);
  while (!done) {
    if (BUFEOF(dbin->f)) {
      printf("Could not locate entry ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &entryname.seek);
      printf("!\n");
      halt();
    }
    if (P_eoln(dbin->f)) {
      fscanf(dbin->f, "%*[^\n]");
      getc(dbin->f);
      continue;
    }
    c_ = getc(dbin->f);

    if (c_ == '\n')
      c_ = ' ';
    testfortrigger(c_, &LINK->locus);
    if (LINK->locus.found)
      dolocus(dbin, dbout, markspots, entryname, changes, &done, fromrange,
	      torange, thenumber, LINK);
  }

}



Static Void themain(dbin, dbout, dbmutatep, markspots, minst_)
_TEXT *dbin, *dbout, *dbmutatep, *markspots, *minst_;
{
  struct LOC_themain V;
  double parameterversion;
  long piecesoutput = 0, fromrange = deffromrange, torange = deftorange;
  _TEXT TEMP;

  V.minst = minst_;
  printf("dbmutate %4.2f\n", version);

  if (*dbmutatep->name != '\0') {
    if (dbmutatep->f != NULL)
      dbmutatep->f = freopen(dbmutatep->name, "r", dbmutatep->f);
    else
      dbmutatep->f = fopen(dbmutatep->name, "r");
  } else
    rewind(dbmutatep->f);
  if (dbmutatep->f == NULL)
    _EscIO2(FileNotFound, dbmutatep->name);
  RESETBUF(dbmutatep->f, Char);
  fscanf(dbmutatep->f, "%lg%*[^\n]", &parameterversion);
  getc(dbmutatep->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }



  filltrigger(&V.locus, "LOCUS                                             ");
  filltrigger(&V.accession,
	      "ACCESSION                                         ");
  filltrigger(&V.origin, "ORIGIN                                            ");
  filltrigger(&V.organism,
	      "ORGANISM                                          ");
  filltrigger(&V.entryend,
	      "//                                                ");

  if (*dbout->name != '\0') {
    if (dbout->f != NULL)
      dbout->f = freopen(dbout->name, "w", dbout->f);
    else
      dbout->f = fopen(dbout->name, "w");
  } else {
    if (dbout->f != NULL)
      rewind(dbout->f);
    else
      dbout->f = tmpfile();
  }
  if (dbout->f == NULL)
    _EscIO2(FileNotFound, dbout->name);
  SETUPBUF(dbout->f, Char);

  if (*markspots->name != '\0') {
    if (markspots->f != NULL)
      markspots->f = freopen(markspots->name, "w", markspots->f);
    else
      markspots->f = fopen(markspots->name, "w");
  } else {
    if (markspots->f != NULL)
      rewind(markspots->f);
    else
      markspots->f = tmpfile();
  }
  if (markspots->f == NULL)
    _EscIO2(FileNotFound, markspots->name);
  SETUPBUF(markspots->f, Char);
  fprintf(markspots->f, "* dbmutate %4.2f\n", version);
  marksautomate(markspots);
  startinst(V.minst, version, &V);


  while (!BUFEOF(dbmutatep->f)) {
    skipblanks(dbmutatep);
    if ((P_peek(dbmutatep->f) == '*') | P_eoln(dbmutatep->f)) {
      fscanf(dbmutatep->f, "%*[^\n]");
      getc(dbmutatep->f);
      continue;
    }
    if (P_peek(dbmutatep->f) == '@') {
      processcommand(dbmutatep, &fromrange, &torange);
      continue;
    }
    onetoken(dbmutatep, &V.entryname.seek, &V.gotten);
    if (!V.gotten)
      continue;
    piecesoutput++;

    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &V.entryname.seek);
    readchangeset(dbmutatep, &V.changes);
    putchar(' ');
    TEMP.f = stdout;
    *TEMP.name = '\0';
    describechangeset(&TEMP, V.changes);
    fscanf(dbmutatep->f, "%*[^\n]");
    getc(dbmutatep->f);
    putchar('\n');

    locateentry(dbin, dbout, markspots, V.entryname, V.changes, &fromrange,
		&torange, &piecesoutput, &V);
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  minst.f = NULL;
  strcpy(minst.name, "minst");
  markspots.f = NULL;
  strcpy(markspots.name, "markspots");
  dbmutatep.f = NULL;
  strcpy(dbmutatep.name, "dbmutatep");
  dbout.f = NULL;
  strcpy(dbout.name, "dbout");
  dbin.f = NULL;
  strcpy(dbin.name, "dbin");
  /*

*/
  themain(&dbin, &dbout, &dbmutatep, &markspots, &minst);
_L1:
  if (dbin.f != NULL)
    fclose(dbin.f);
  if (dbout.f != NULL)
    fclose(dbout.f);
  if (dbmutatep.f != NULL)
    fclose(dbmutatep.f);
  if (markspots.f != NULL)
    fclose(markspots.f);
  if (minst.f != NULL)
    fclose(minst.f);
  exit(EXIT_SUCCESS);
}



/* End. */
