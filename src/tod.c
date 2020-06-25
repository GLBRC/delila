/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "tod.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         3.01
/*




*/



/*
































































































*/



#define maxseq          1000
#define maxname         30
#define maxfillname     10
#define quicksequence   25



#define maxstring       150



#define fillermax       50



typedef Char fillname[maxfillname];

typedef struct name {
  Char letters[maxname];
  long length;
} name;

typedef struct sequence {
  Char bases[maxseq];
  long length, lanenumber;
  name plasmidname, primername, date;
  long start, stop;
  boolean editdeleted;
  /*
*/
  struct sequence *next;
} sequence;

typedef struct locations {
  long spot;
  struct locations *next;
} locations;



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


Static _TEXT abi, thedate, ssb, todp, results, summary, db;


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



Static Void fillstring(s, a)
stringDelila *s;
Char *a;
{
  /*
*/


  /*


*/
  long length = fillermax;
  long index;

  clearstring(s);
  while (length > 1 && a[length-1] == ' ')
    length--;
  if (length == 1 && a[length-1] == ' ') {
    printf("fillstring: the string is empty\n");
    halt();
  }

  for (index = 0; index < length; index++)
    s->letters[index] = a[index];
  s->length = length;
  s->current = 1;
}



Static Void filltrigger(t, a)
trigger *t;
Char *a;
{
  fillstring(&t->seek, a);
}



/*




*/

Static Void resettrigger(t)
trigger *t;
{
  t->state = 0;
  t->skip = false;
  t->found = false;
}


Static Void testfortrigger(ch, t)
Char ch;
trigger *t;
{
  /*




*/
  t->state++;
  /*



*/
  if (t->seek.letters[t->state - 1] == ch) {
    t->skip = false;
    if (t->state == t->seek.length)
      t->found = true;
    else
      t->found = false;
    return;
  }
  t->state = 0;
  t->skip = true;
  t->found = false;
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








Static Void bar(f, n, c)
_TEXT *f;
long n;
Char c;
{
  long i;

  for (i = 1; i <= n; i++)
    putc(c, f->f);
}



Static Void clearname(n, f)
name *n;
Char *f;
{
  long i;

  for (i = 0; i < maxfillname; i++)
    n->letters[i] = f[i];
  n->length = maxfillname;
}



Static Void writename(f, n)
_TEXT *f;
name n;
{
  long i;

  for (i = 0; i < n.length; i++)
    putc(n.letters[i], f->f);
}



Static Void oldreadname(f, n)
_TEXT *f;
name *n;
{
  /*
*/
  Char c = ' ';

  while ((P_peek(f->f) == 0) | (P_peek(f->f) == '/' || P_peek(f->f) == ' '))
    getc(f->f);
  n->length = 0;
  do {
    c = getc(f->f);
    if (c == '\n')
      c = ' ';
    if (c != '\0' && c != ' ' && c != '/') {
      n->length++;
      n->letters[n->length - 1] = c;
    }
  } while (!(((c == 0) | P_eoln(f->f)) || c == ' ' || c == '/'));
}



Static Void readname(f, n, location)
_TEXT *f;
name *n;
long *location;
{
  Char c;

  n->length = 0;

  /*

*/
  while (P_peek(f->f) == '/' || P_peek(f->f) == ' ') {
    getc(f->f);
    (*location)++;
  }
  /*





*/
  do {
    c = getc(f->f);
    if (c == '\n')
      c = ' ';
    (*location)++;
    if (c != '\0' && c != ' ' && c != '/') {
      n->length++;
      n->letters[n->length - 1] = c;
    }
  } while (!((c == '\0' || c == ' ' || c == '/') | P_eoln(f->f)));
}



Static Void copyname(n1, n2)
name n1, *n2;
{
  long i;

  n2->length = n1.length;
  for (i = 0; i < n1.length; i++)
    n2->letters[i] = n1.letters[i];
}



Static Void clearseq(seq)
sequence *seq;
{
  seq->length = 0;
  seq->lanenumber = 0;
  clearname(&seq->plasmidname, "plasmid?  ");
  clearname(&seq->primername, "primer?   ");
  clearname(&seq->date, "date?     ");
  seq->start = 0;
  seq->stop = 0;
  seq->editdeleted = false;
  seq->next = NULL;
}



Static Void joinname(d, add)
name *d, add;
{
  long i;

  if (d->length + add.length + 1 > maxname) {
    printf("name too long in joinname\n");
    halt();
  }
  d->length++;
  d->letters[d->length - 1] = ' ';
  for (i = 0; i < add.length; i++)
    d->letters[i + d->length] = add.letters[i];
  d->length += add.length;
}



Static Void readdate(results, date, d)
_TEXT *results, *date;
name *d;
{
  long location = 0;
  name year, month, day;

  if (*thedate.name != '\0') {
    if (thedate.f != NULL)
      thedate.f = freopen(thedate.name, "r", thedate.f);
    else
      thedate.f = fopen(thedate.name, "r");
  } else
    rewind(thedate.f);
  if (thedate.f == NULL)
    _EscIO2(FileNotFound, thedate.name);
  RESETBUF(thedate.f, Char);
  if (BUFEOF(date->f)) {
    printf("The file \"thedate\" is empty - run makedate.\n");
    /*

*/
    halt();
  } else {
    /*


*/
    readname(date, &year, &location);
    readname(date, &month, &location);
    readname(date, &day, &location);
    copyname(year, d);
    joinname(d, month);
    joinname(d, day);
    fscanf(date->f, "%*[^\n]");
    getc(date->f);
  }
  fprintf(results->f, "\nThe date found in file thedate is: \"");
  writename(results, *d);
  fprintf(results->f, "\"\n");
}


#define lanebot         131

#define namebot         163

#define lanelength      586


/* Local variables for readssblane: */
struct LOC_readssblane {
  _TEXT *ssb;
  long lanenumber, *location;
} ;

Local long spot(a, LINK)
long a;
struct LOC_readssblane *LINK;
{
  return (a + lanelength * (LINK->lanenumber - 1));
}

Local Void tolocation(a, LINK)
long a;
struct LOC_readssblane *LINK;
{
  if (*LINK->location > a) {
    if (*LINK->ssb->name != '\0') {
      if (LINK->ssb->f != NULL)
	LINK->ssb->f = freopen(LINK->ssb->name, "r", LINK->ssb->f);
      else
	LINK->ssb->f = fopen(LINK->ssb->name, "r");
    } else
      rewind(LINK->ssb->f);
    if (LINK->ssb->f == NULL)
      _EscIO2(FileNotFound, LINK->ssb->name);
    RESETBUF(LINK->ssb->f, Char);
    *LINK->location = 1;
  }
  while (*LINK->location < a) {
    getc(LINK->ssb->f);
    (*LINK->location)++;
  }

}



Static Void readssblane(ssb_, lanenumber_, plasmidname, primername, location_)
_TEXT *ssb_;
long lanenumber_;
name *plasmidname, *primername;
long *location_;
{
  /*




*/
  struct LOC_readssblane V;
  long numbers[3];
  Char number1, number2;

  V.ssb = ssb_;
  V.lanenumber = lanenumber_;
  V.location = location_;
  P_addset(P_expset(numbers, 0L), '0');
  P_addset(numbers, '1');
  P_addset(numbers, '2');
  P_addset(numbers, '3');
  P_addset(numbers, '4');
  P_addset(numbers, '5');
  P_addset(numbers, '6');
  P_addset(numbers, '7');
  P_addset(numbers, '8');
  P_addset(numbers, '9');
  tolocation(spot((long)lanebot, &V), &V);
  while (!P_inset(P_peek(V.ssb->f), numbers)) {
    getc(V.ssb->f);
    (*V.location)++;
  }
  number1 = getc(V.ssb->f);
  number2 = getc(V.ssb->f);
  if (number1 == '\n')
    number1 = ' ';
  if (number2 == '\n')
    number2 = ' ';
  *V.location += 2;
  if (!P_inset(number2, numbers)) {
    printf("readssblane: bad number: \"%c%c\"\n", number1, number2);
    printf("(ords: %d & %12d)\n", number1, number2);
    printf("for lane number %ld\n", V.lanenumber);
    halt();
  }

  if ((number1 - '0') * 10 + number2 - '0' != V.lanenumber) {
    printf("readssblane: trying to read lane number %ld but found %c%c\n",
	   V.lanenumber, number1, number2);
    halt();
  }

  tolocation(spot((long)namebot, &V), &V);

  if (P_peek(V.ssb->f) == 0) {
    plasmidname->length = 0;
    primername->length = 0;
  } else {
    readname(V.ssb, plasmidname, V.location);
    readname(V.ssb, primername, V.location);
  }
}

#undef lanebot
#undef namebot
#undef lanelength


/* Local variables for checkthedate: */
struct LOC_checkthedate {
  _TEXT *thedate, *ssb;
  boolean wasaspace;
} ;

/*
*/
Local Void position(LINK)
struct LOC_checkthedate *LINK;
{
  if (*LINK->ssb->name != '\0') {
    if (LINK->ssb->f != NULL)
      LINK->ssb->f = freopen(LINK->ssb->name, "r", LINK->ssb->f);
    else
      LINK->ssb->f = fopen(LINK->ssb->name, "r");
  } else
    rewind(LINK->ssb->f);
  if (LINK->ssb->f == NULL)
    _EscIO2(FileNotFound, LINK->ssb->name);
  RESETBUF(LINK->ssb->f, Char);
  getc(LINK->ssb->f);
  getc(LINK->ssb->f);
  if (*LINK->thedate->name != '\0') {
    if (LINK->thedate->f != NULL)
      LINK->thedate->f = freopen(LINK->thedate->name, "r", LINK->thedate->f);
    else
      LINK->thedate->f = fopen(LINK->thedate->name, "r");
  } else
    rewind(LINK->thedate->f);
  if (LINK->thedate->f == NULL)
    _EscIO2(FileNotFound, LINK->thedate->name);
  RESETBUF(LINK->thedate->f, Char);
  getc(LINK->thedate->f);
  getc(LINK->thedate->f);
}

Local Void step(LINK)
struct LOC_checkthedate *LINK;
{
  getc(LINK->ssb->f);
  LINK->wasaspace = (P_peek(LINK->thedate->f) == ' ');
  getc(LINK->thedate->f);
  if (LINK->wasaspace & (P_peek(LINK->thedate->f) == ' '))
    getc(LINK->thedate->f);
}



Static Void checkthedate(thedate_, ssb_)
_TEXT *thedate_, *ssb_;
{
  struct LOC_checkthedate V;
  boolean done;

  V.thedate = thedate_;
  V.ssb = ssb_;
  position(&V);
  while ((P_peek(V.ssb->f) != 0) & (!P_eoln(V.thedate->f))) {
    if (P_peek(V.ssb->f) != P_peek(V.thedate->f)) {
      printf("Sample Sheet name does not match date\n");
      position(&V);
      printf("thedate | ssb\n");
      done = false;
      do {
	printf("      %c | %c\n", P_peek(V.thedate->f), P_peek(V.ssb->f));
	done = (P_peek(V.ssb->f) != P_peek(V.thedate->f));
	if (!done)
	  step(&V);
      } while (!done);

      halt();
    }
    step(&V);
  }
}



Static Void readssb(ssb, now, data)
_TEXT *ssb;
name now;
sequence **data;
{
  sequence *d;
  long location = 1;
  long lane;

  clearseq(*data);
  d = *data;
  copyname(now, &d->date);
  if (*ssb->name != '\0') {
    if (ssb->f != NULL)
      ssb->f = freopen(ssb->name, "r", ssb->f);
    else
      ssb->f = fopen(ssb->name, "r");
  } else
    rewind(ssb->f);
  if (ssb->f == NULL)
    _EscIO2(FileNotFound, ssb->name);
  RESETBUF(ssb->f, Char);
  for (lane = 1; lane <= 24; lane++) {
    if (lane > 1) {
      d->next = (sequence *)Malloc(sizeof(sequence));
      d = d->next;
      clearseq(d);
      copyname(now, &d->date);
    }
    readssblane(ssb, lane, &d->plasmidname, &d->primername, &location);
    d->lanenumber = lane;
  }
}



Static Void readaseq(f, seq)
_TEXT *f;
sequence *seq;
{
  /*
*/
  Char c;

  if (seq->length > 0) {
    printf("* THERE IS A SECOND CASE OF SAMPLE %ld *\n", seq->lanenumber);
    if (*db.name != '\0') {
      if (db.f != NULL)
	db.f = freopen(db.name, "w", db.f);
      else
	db.f = fopen(db.name, "w");
    } else {
      if (db.f != NULL)
	rewind(db.f);
      else
	db.f = tmpfile();
    }
    if (db.f == NULL)
      _EscIO2(FileNotFound, db.name);
    SETUPBUF(db.f, Char);
    halt();
  }
  while ((!P_eoln(f->f)) & (P_peek(f->f) != ':')) {
    c = getc(f->f);
    if (c == '\n')
      c = ' ';
    if (c != ' ') {
      seq->length++;
      seq->bases[seq->length - 1] = c;
    }
  }
  seq->start = 1;
  seq->stop = seq->length;
  fscanf(f->f, "%*[^\n]");
  getc(f->f);
}



Static Void writeseq(f, seq)
_TEXT *f;
sequence *seq;
{
  long l, FORLIM;

  if (seq->start <= seq->stop && seq->start > 0 && seq->stop > 0) {
    FORLIM = seq->stop;
    for (l = seq->start - 1; l < FORLIM; l++)
      putc(seq->bases[l], f->f);
    return;
  }
  fprintf(f->f, "NO-SEQUENCE");
  if (seq->editdeleted)
    fprintf(f->f, " (DELETED BY EDITING)");
  else
    fprintf(f->f, " (NO SEQUENCE PROVIDED)");
}



Static Void reportlanes(results, data, expectsequence)
_TEXT *results;
sequence *data;
boolean expectsequence;
{
  /*
*/
  sequence *d;
  long l;
  sequence *WITH;

  putc('\n', results->f);
  bar(results, 80L, '*');
  fprintf(results->f, "\n*** Sample Sheet *");
  bar(results, 62L, '*');
  fprintf(results->f, "\n\n");
  d = data;
  while (d != NULL) {
    WITH = d;
    fprintf(results->f, "lane number: %2ld", WITH->lanenumber);
    if (WITH->plasmidname.length > 0) {
      fprintf(results->f, "  plasmid: ");
      writename(results, WITH->plasmidname);
      fprintf(results->f, "  primer: ");
      writename(results, WITH->primername);
    }

    if (expectsequence) {
      if (WITH->length > 0) {
	if (WITH->plasmidname.length <= 0)
	  fprintf(results->f, "  THAT'S ODD!  SEQUENCE WITHOUT A NAME:");
	putc(' ', results->f);
	l = 1;
	while (l <= WITH->length && l <= quicksequence) {
	  putc(WITH->bases[l-1], results->f);
	  l++;
	}
	if (l < WITH->length)
	  fprintf(results->f, "...");

      } else
	fprintf(results->f, " NO SEQUENCE DATA FOUND! ******");
    }

    putc('\n', results->f);
    d = d->next;
  }
  putc('\n', results->f);
}



Static Void writeedit(f, N, P1, P2, S)
_TEXT *f;
long N, P1, P2;
sequence S;
{
  fprintf(f->f, "Editing Lane N=%ld", N);
  fprintf(f->f, " from P1=%ld", P1);
  fprintf(f->f, " to P2=%ld", P2);
  fprintf(f->f, " with S=\"");
  writeseq(f, &S);
  fprintf(f->f, "\"\n");
}


#define seqline         80


/* Local variables for writeticseq: */
struct LOC_writeticseq {
  _TEXT *f;
  long l;
} ;

Local Void tic(number, LINK)
long number;
struct LOC_writeticseq *LINK;
{
  long j, k;

  putc('\n', LINK->f->f);
  for (j = 1; j <= number; j++) {
    for (k = 1; k <= 4; k++)
      putc(' ', LINK->f->f);
    putc('\'', LINK->f->f);
    for (k = 1; k <= 1; k++)
      putc(' ', LINK->f->f);
    fprintf(LINK->f->f, "%3ld^", j * 10 + LINK->l - seqline);
  }
  putc('\n', LINK->f->f);
}



Static Void writeticseq(f_, seq)
_TEXT *f_;
sequence *seq;
{
  /*
*/
  struct LOC_writeticseq V;

  V.f = f_;
  if (seq->start > seq->stop || seq->start <= 0 || seq->stop <= 0) {
    fprintf(V.f->f, "NO-SEQUENCE\n");
    return;
  }
  V.l = seq->start;
  while (V.l <= seq->stop) {
    putc(seq->bases[V.l-1], V.f->f);
    if (V.l % seqline == 0)
      tic(seqline / 10L, &V);
    V.l++;
  }


  V.l = (seq->stop + seqline) / seqline * seqline;
  tic(seqline / 10L, &V);
}

#undef seqline



Static Char capitalize(c)
Char c;
{
  long n = c;

  if (n >= 'a' && n <= 'z')
    c = _toupper(n);

  return c;
}



Static Char decapitalize(c)
Char c;
{
  long n = c;

  if (n >= 'A' && n <= 'Z')
    c = _tolower(n);
  else
    c = (Char)n;

  return c;
}



Static Void nxseq(seq)
sequence *seq;
{
  long l, FORLIM;

  FORLIM = seq->length;
  for (l = 0; l < FORLIM; l++) {
    if (seq->bases[l] == 'n')
      seq->bases[l] = 'x';
  }
}



Static Char basecomp(c)
Char c;
{
  /*
*/
  if (c == 'a')
    return 't';
  else if (c == 'c')
    return 'g';
  else if (c == 'g')
    return 'c';
  else if (c == 't')
    return 'a';
  else if (c == 'x')
    return 'x';
  else
    return c;
}



Static Void complement(seq)
sequence *seq;
{
  sequence hold;
  long lower, upper, FORLIM;

  FORLIM = seq->length;
  for (lower = 1; lower <= FORLIM; lower++) {
    upper = seq->length - lower + 1;
    hold.bases[upper-1] = basecomp(seq->bases[lower-1]);
  }
  FORLIM = seq->length;
  for (lower = 0; lower < FORLIM; lower++)
    seq->bases[lower] = hold.bases[lower];
}



Static boolean asymmetric(seq)
sequence *seq;
{
  long lower;
  boolean crooked = false;
  long upper, FORLIM;

  if (seq->length / 2 * 2 != seq->length)
    return true;
  FORLIM = seq->length / 2;
  for (lower = 1; lower <= FORLIM; lower++) {
    upper = seq->length - lower + 1;
    if (seq->bases[upper-1] != basecomp(seq->bases[lower-1]))
      crooked = true;
  }
  return crooked;

/* p2c: tod.p: Note: Eliminated unused assignment statement [338] */
}



Static Void upperseq(seq)
sequence *seq;
{
  long l, FORLIM;

  FORLIM = seq->length;
  for (l = 0; l < FORLIM; l++)
    seq->bases[l] = capitalize(seq->bases[l]);
}



Static Void lowerseq(seq)
sequence *seq;
{
  long l, FORLIM;

  FORLIM = seq->length;
  for (l = 0; l < FORLIM; l++)
    seq->bases[l] = decapitalize(seq->bases[l]);
}



Static Void locate(r, seq, l, finds)
trigger r;
sequence seq;
long *l, *finds;
{
  /*
*/
  long p;

  *l = -1;
  resettrigger(&r);
  *finds = 0;
  for (p = 1; p <= seq.length; p++) {
    putc(seq.bases[p-1], results.f);
    testfortrigger(seq.bases[p-1], &r);



    putc('_', results.f);
    if (r.skip)
      putc('S', results.f);
    else
      putc('s', results.f);
    if (r.found)
      putc('F', results.f);
    else
      putc('f', results.f);
    fprintf(results.f, "%ld_", r.state);

    if (r.found) {
      (*finds)++;
      *l = p;

      resettrigger(&r);
    }
  }
  fprintf(results.f, "   what we saw l=%ld finds = %ld\n", *l, *finds);
}



Static Void listlocations(f, l)
_TEXT *f;
locations *l;
{
  locations *location = l;

  while (location != NULL) {
    fprintf(f->f, " %ld", location->spot);
    location = location->next;
  }
}



Static Void find(r, seq, allspots, finds, direction)
sequence r, *seq;
locations **allspots;
long *finds, direction;
{
  /*



*/
  long d;
  locations *l, *last;
  Char lower;
  boolean matching;
  long p, j, FORLIM;

  /*



*/

  *allspots = (locations *)Malloc(sizeof(locations));
  (*allspots)->spot = -1;
  (*allspots)->next = NULL;
  /*



*/
  l = *allspots;
  last = l;
  /*

*/
  *finds = 0;
  FORLIM = seq->length;
  for (d = 1; d <= FORLIM; d++) {
    if (direction == -1)
      p = seq->length - d + 1;
    else
      p = d;
    matching = true;
    j = 0;
    /*

*/
    while (matching && j < r.length) {
      /*
*/
      lower = decapitalize(seq->bases[p + j - 1]);
      if (lower != r.bases[j])
	matching = false;
      /*




*/
      j++;
    }

    if (matching) {
      (*finds)++;
      l->spot = p;


      l->next = (locations *)Malloc(sizeof(locations));
      last = l;
      l = l->next;
      l->spot = -1;
      l->next = NULL;
      /*


*/

      for (j = -1; j <= r.length - 2; j++)
	seq->bases[d + j] = capitalize(seq->bases[d + j]);
    }

  }
  /*



*/
  if (last->next != NULL) {
    Free(last->next);
    last->next = NULL;
  }
  /*

*/

  /*


*/
}



Static Void readparameters(results, todp, r1, r2)
_TEXT *results, *todp;
sequence *r1, *r2;
{
  if (*todp->name != '\0') {
    if (todp->f != NULL)
      todp->f = freopen(todp->name, "r", todp->f);
    else
      todp->f = fopen(todp->name, "r");
  } else
    rewind(todp->f);
  if (todp->f == NULL)
    _EscIO2(FileNotFound, todp->name);
  RESETBUF(todp->f, Char);
  clearseq(r1);
  readaseq(todp, r1);
  clearseq(r2);
  readaseq(todp, r2);
  fprintf(results->f, "\nParameter (todp) says to grab sequences between ");
  upperseq(r1);
  writeseq(results, r1);
  fprintf(results->f, " and ");
  lowerseq(r1);
  upperseq(r2);
  writeseq(results, r2);
  putc('\n', results->f);
  lowerseq(r2);
}



Static Void coloncheck(abi)
_TEXT *abi;
{
  while (!P_eoln(abi->f)) {
    if (P_peek(abi->f) != ':') {
      printf("BAD RAW FILE:  MISSING COLONS LINE\n");
      printf("character found: %c\n", P_peek(abi->f));
      halt();
    } else
      getc(abi->f);
  }
  fscanf(abi->f, "%*[^\n]");
  getc(abi->f);
}



Static Void readsequences(results, abi, data)
_TEXT *results, *abi;
sequence **data;
{
  boolean found;
  long number = 0;

  sequence *seq;

  if (*abi->name != '\0') {
    if (abi->f != NULL)
      abi->f = freopen(abi->name, "r", abi->f);
    else
      abi->f = fopen(abi->name, "r");
  } else
    rewind(abi->f);
  if (abi->f == NULL)
    _EscIO2(FileNotFound, abi->name);
  RESETBUF(abi->f, Char);

  coloncheck(abi);
  while (!BUFEOF(abi->f)) {
    while (P_peek(abi->f) != '9' && P_peek(abi->f) != '8' &&
	   P_peek(abi->f) != '7' && P_peek(abi->f) != '6' &&
	   P_peek(abi->f) != '5' && P_peek(abi->f) != '4' &&
	   P_peek(abi->f) != '3' && P_peek(abi->f) != '2' &&
	   P_peek(abi->f) != '1' && P_peek(abi->f) != '0')
      getc(abi->f);
    fscanf(abi->f, "%ld%*[^\n]", &number);
    getc(abi->f);


    seq = *data;
    found = false;
    while (!found) {
      if (number == seq->lanenumber)
	break;
      /*
*/
/* p2c: tod.p: Note: Eliminated unused assignment statement [338] */
      if (seq->next != NULL) {
	seq = seq->next;
	continue;
      }
      fprintf(results->f,
	      "SEQUENCE %ld HAS NO CORRESPONDING DATA IN THE SAMPLE SHEET!\n",
	      number);
      fprintf(results->f, "Creating new lane, with empty names (etc)\n");
      seq->next = (sequence *)Malloc(sizeof(sequence));
      seq = seq->next;
      clearseq(seq);
      seq->lanenumber = number;
      found = true;
    }

    coloncheck(abi);
    readaseq(abi, seq);
  }


}


/* Local variables for edit: */
struct LOC_edit {
  _TEXT *results, *edits;
  sequence **data;
  long delta;
  boolean done;
  /*

*/
  long N;
  boolean ok;
  long P1, P2;
  sequence S, *seq;
} ;

Local Void copycomments(edits, results, LINK)
_TEXT *edits, *results;
struct LOC_edit *LINK;
{
  boolean havecomments = true;

  while (havecomments) {
    if (BUFEOF(edits->f)) {
      havecomments = false;
      break;
    }
    if (P_peek(edits->f) == '*')
      copyaline(edits, results);
    else
      havecomments = false;
  }
}

Local Void editoperation(LINK)
struct LOC_edit *LINK;
{
  long p;
  sequence *WITH;
  long FORLIM;

  fscanf(LINK->edits->f, "%ld%ld%ld", &LINK->N, &LINK->P1, &LINK->P2);
  clearseq(&LINK->S);
  readaseq(LINK->edits, &LINK->S);

  writeedit(LINK->results, LINK->N, LINK->P1, LINK->P2, LINK->S);
  copycomments(LINK->edits, LINK->results, LINK);


  LINK->ok = true;
  if (LINK->P1 > LINK->P2) {
    fprintf(LINK->results->f,
	    "Second position must be greater than the first,\n");
    fprintf(LINK->results->f, "but you said P1 = %ld > %ld = P2.\n",
	    LINK->P1, LINK->P2);
    LINK->ok = false;
  }


  if (LINK->S.bases[0] == 'd') {
    fprintf(LINK->results->f, "This will be a pure deletion.\n");
    LINK->S.length = 0;
  }


  LINK->seq = *LINK->data;
  LINK->done = false;
  while (LINK->seq != NULL && !LINK->done) {
    if (LINK->seq->lanenumber != LINK->N) {
      LINK->seq = LINK->seq->next;
      continue;
    }
    if (LINK->seq->length > 0) {
      LINK->done = true;
      break;
    }
    fprintf(LINK->results->f,
	    "Sample of Lane N =%ld exists, but has no sequence.\n", LINK->N);
    LINK->ok = false;
    LINK->done = true;
  }

  if (LINK->seq == NULL) {
    fprintf(LINK->results->f, "Sample of Lane N = %ld was not found.\n",
	    LINK->N);
    LINK->ok = false;
  }

  if (LINK->ok) {
    WITH = LINK->seq;

    fprintf(LINK->results->f, "The sequence to edit is length = %ld:\n",
	    WITH->length);
    writeticseq(LINK->results, LINK->seq);
    putc('\n', LINK->results->f);


    if (LINK->P1 > WITH->length) {
      LINK->P1 = WITH->length;
      LINK->P2 = LINK->P1 + 1;
    } else if (LINK->P2 > WITH->length) {
      LINK->P2 = WITH->length + 1;
      if (LINK->P1 < 1)
	LINK->P1 = 0;
    }
    if (LINK->P2 < 1) {
      LINK->P2 = 1;
      LINK->P1 = LINK->P2 - 1;
    } else if (LINK->P2 > 1 && LINK->P1 < 0)
      LINK->P1 = 0;




    if (LINK->P2 > LINK->P1 + 1) {
      fprintf(LINK->results->f, "The sequence being deleted is: \"");
      if (LINK->P2 - LINK->P1 > WITH->length) {
	fprintf(LINK->results->f, "(entire sequence)");
	WITH->editdeleted = true;
      } else {
	FORLIM = LINK->P2 - 2;
	for (p = LINK->P1; p <= FORLIM; p++)
	  putc(WITH->bases[p], LINK->results->f);
      }
      fprintf(LINK->results->f, "\"\n");

      LINK->delta = LINK->P1 - LINK->P2 + 1;
      FORLIM = WITH->length;
      for (p = LINK->P2 - 1; p < FORLIM; p++)
	WITH->bases[p + LINK->delta] = WITH->bases[p];
      WITH->length += LINK->delta;
      WITH->stop = WITH->length;
      fprintf(LINK->results->f,
	      "After the %ld base DELETION we have length = %ld:\n",
	      -LINK->delta, WITH->length);
      writeticseq(LINK->results, LINK->seq);
      putc('\n', LINK->results->f);
    }


    if (LINK->S.length > 0) {
      FORLIM = LINK->P1;
      for (p = WITH->length - 1; p >= FORLIM; p--)
	WITH->bases[p + LINK->S.length] = WITH->bases[p];

      FORLIM = LINK->S.length;
      for (p = 0; p < FORLIM; p++)
	WITH->bases[LINK->P1 + p] = LINK->S.bases[p];
      WITH->length += LINK->S.length;
      WITH->stop = WITH->length;
      fprintf(LINK->results->f,
	      "After the %ld base INSERTION we have length = %ld:\n",
	      LINK->S.length, WITH->length);
      writeticseq(LINK->results, LINK->seq);
      putc('\n', LINK->results->f);
    }

  } else
    fprintf(LINK->results->f, "THESE EDIT COMMANDS WERE IGNORED.\n");
  putc('\n', LINK->results->f);
}



Static Void edit(results_, edits_, data_)
_TEXT *results_, *edits_;
sequence **data_;
{
  struct LOC_edit V;

  V.results = results_;
  V.edits = edits_;
  V.data = data_;
  putc('\n', V.results->f);

  bar(V.results, 80L, '*');
  fprintf(V.results->f, "\n* EDITING THE SAMPLES ********");
  bar(V.results, 50L, '*');
  putc('\n', V.results->f);
  copycomments(V.edits, V.results, &V);

  while (!BUFEOF(V.edits->f))
    editoperation(&V);

  /*











*/
  fprintf(V.results->f, "DONE EDITING THE SAMPLES\n\n");
}


/* Local variables for process: */
struct LOC_process {
  _TEXT *results;
  sequence r1, r2;
  boolean chopout, flip1, flip2;
  long r1finds, r2finds;
  locations *r1location, *r2location;
  sequence *seq;
} ;

Local Void doasequence(LINK)
struct LOC_process *LINK;
{
  lowerseq(LINK->seq);
  nxseq(LINK->seq);

  find(LINK->r1, LINK->seq, &LINK->r1location, &LINK->r1finds, 1L);
  if (LINK->flip1) {
    if (LINK->r1finds == 0) {
      writeseq(LINK->results, &LINK->r1);
      fprintf(LINK->results->f, " not found; trying its complement\n");
      complement(&LINK->r1);
      find(LINK->r1, LINK->seq, &LINK->r1location, &LINK->r1finds, 1L);
    }
  }

  find(LINK->r2, LINK->seq, &LINK->r2location, &LINK->r2finds, 1L);
  if (LINK->flip2) {
    if (LINK->r2finds == 0) {
      writeseq(LINK->results, &LINK->r2);
      fprintf(LINK->results->f, " not found; trying its complement\n");
      complement(&LINK->r2);
      find(LINK->r2, LINK->seq, &LINK->r2location, &LINK->r2finds, 1L);
    }
  }

  fprintf(LINK->results->f, "- LOCATIONS OF MARKS ---------");
  bar(LINK->results, 50L, '-');
  putc('\n', LINK->results->f);
  writeticseq(LINK->results, LINK->seq);
  putc('\n', LINK->results->f);
  bar(LINK->results, 80L, '-');
  putc('\n', LINK->results->f);

  if (LINK->r1finds == 0) {
    fprintf(LINK->results->f, "NO copies of ");
    writeseq(LINK->results, &LINK->r1);
    fprintf(LINK->results->f, " found\n");
  } else {
    if (LINK->r1finds == 1) {
      writeseq(LINK->results, &LINK->r1);
      fprintf(LINK->results->f, " is");
    } else {
      fprintf(LINK->results->f, "%ld copies of ", LINK->r1finds);
      writeseq(LINK->results, &LINK->r1);
      fprintf(LINK->results->f, " found");
    }
    fprintf(LINK->results->f, " at");
    listlocations(LINK->results, LINK->r1location);
    putc('\n', LINK->results->f);
  }

  if (LINK->r2finds == 0) {
    fprintf(LINK->results->f, "NO copies of ");
    writeseq(LINK->results, &LINK->r2);
    fprintf(LINK->results->f, " found\n");
  } else {
    if (LINK->r2finds == 1) {
      writeseq(LINK->results, &LINK->r2);
      fprintf(LINK->results->f, " is");
    } else {
      fprintf(LINK->results->f, "%ld copies of ", LINK->r2finds);
      writeseq(LINK->results, &LINK->r2);
      fprintf(LINK->results->f, " found");
    }
    fprintf(LINK->results->f, " at");
    listlocations(LINK->results, LINK->r2location);
    putc('\n', LINK->results->f);
  }

  LINK->chopout = (LINK->r1finds == 1 && LINK->r2finds == 1);

  if (LINK->chopout) {
    if (LINK->r1location->spot > LINK->r2location->spot) {
      bar(LINK->results, 80L, '-');
      fprintf(LINK->results->f, "\nTaking the complement\n");
      lowerseq(LINK->seq);
      complement(LINK->seq);
      if (LINK->flip1)
	complement(&LINK->r1);
      if (LINK->flip2)
	complement(&LINK->r2);
      find(LINK->r1, LINK->seq, &LINK->r1location, &LINK->r1finds, 1L);
      find(LINK->r2, LINK->seq, &LINK->r2location, &LINK->r2finds, 1L);
      writeseq(LINK->results, LINK->seq);
      putc('\n', LINK->results->f);
      bar(LINK->results, 80L, '=');
      putc('\n', LINK->results->f);

      LINK->chopout = (LINK->r1finds == 1 && LINK->r2finds == 1);
      if (!LINK->chopout) {
	fprintf(LINK->results->f, "I can't find \n");
	writeseq(LINK->results, &LINK->r1);
	fprintf(LINK->results->f, " or ");
	writeseq(LINK->results, &LINK->r2);
	fprintf(LINK->results->f, " in the complement of the sequence!\n");
	halt();

	/*












*/

      }
    }

    if (LINK->chopout) {
      fprintf(LINK->results->f,
	"Both markers were found once so the middle will be chopped out\n");
      LINK->seq->start = LINK->r1location->spot;
      LINK->seq->stop = LINK->r2location->spot + LINK->r1.length - 1;

    }
  } else
    fprintf(LINK->results->f, "The middle will NOT be chopped out.\n");

  fprintf(LINK->results->f,
	  "=== THE SEQUENCE THAT WILL BE OUTPUT TO THE DB FILE IS: ==\n");
  writeseq(LINK->results, LINK->seq);
  fprintf(LINK->results->f, "\nOverall length: %ld\n",
	  LINK->seq->stop - LINK->seq->start + 1);
  if (LINK->chopout)
    fprintf(LINK->results->f, "length less R1 and R2: %ld\n",
      LINK->seq->stop - LINK->seq->start - LINK->r1.length - LINK->r2.length + 1);

  bar(LINK->results, 80L, '=');
  fprintf(LINK->results->f, "\n\n");
  lowerseq(LINK->seq);
}



Static Void process(results_, r1_, r2_, data)
_TEXT *results_;
sequence r1_, r2_, **data;
{
  /*
*/
  struct LOC_process V;
  _TEXT TEMP;

  V.results = results_;
  V.r1 = r1_;
  V.r2 = r2_;
  fprintf(V.results->f, "\n\n");
  bar(V.results, 80L, '*');
  fprintf(V.results->f, "\n* PROCESSING THE SEQUENCES ***");
  bar(V.results, 50L, '*');
  putc('\n', V.results->f);
  V.seq = *data;

  V.flip1 = asymmetric(&V.r1);
  V.flip2 = asymmetric(&V.r2);

  while (V.seq != NULL) {
    if (V.seq->plasmidname.length > 0) {
      fprintf(V.results->f, "Processing Lane Number %ld: ", V.seq->lanenumber);
      fprintf(V.results->f, "plasmid: ");
      writename(V.results, V.seq->plasmidname);
      fprintf(V.results->f, ", primer: ");
      writename(V.results, V.seq->primername);
      putc('\n', V.results->f);

      if (V.seq->length > 0) {
	fprintf(V.results->f, "- RAW SEQUENCE FILE ----------");
	bar(V.results, 50L, '-');
	putc('\n', V.results->f);
	writeticseq(V.results, V.seq);
	putc('\n', V.results->f);
	doasequence(&V);
      } else {
	if (V.seq->editdeleted)
	  fprintf(V.results->f, "(The sequence was deleted by editing.)\n\n");
	else {
	  bar(V.results, 80L, '*');
	  fprintf(V.results->f, "\n* MISSING SEQUENCE FILE! *****");
	  bar(V.results, 50L, '!');
	  putc('\n', V.results->f);
	  bar(V.results, 80L, '*');
	  fprintf(V.results->f, "\n\n");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';

	  bar(&TEMP, 80L, '*');
	  printf("\n* MISSING SEQUENCE FILE! *****");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  bar(&TEMP, 50L, '*');
	  putchar('\n');
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  bar(&TEMP, 80L, '*');
	  printf("\nWhile Processing Lane Number %ld: ", V.seq->lanenumber);
	  printf("plasmid: ");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writename(&TEMP, V.seq->plasmidname);
	  printf(", primer: ");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writename(&TEMP, V.seq->primername);
	  putchar('\n');
	}
      }
    }


    V.seq = V.seq->next;
  }
}



Static Void writesummary(db, data)
_TEXT *db;
sequence *data;
{
  sequence *datum = data;
  sequence *WITH;

  while (datum != NULL) {
    WITH = datum;
    fprintf(db->f, "%2ld", WITH->lanenumber);
    putc(' ', db->f);
    writename(db, WITH->plasmidname);
    putc(' ', db->f);
    writename(db, WITH->primername);
    putc(' ', db->f);
    writename(db, WITH->date);
    putc(' ', db->f);
    writeseq(db, datum);
    putc('\n', db->f);
    datum = datum->next;
  }
}



Static Void writedata(db, data)
_TEXT *db;
sequence *data;
{
  sequence *datum, *WITH;

  if (*db->name != '\0') {
    if (db->f != NULL)
      db->f = freopen(db->name, "w", db->f);
    else
      db->f = fopen(db->name, "w");
  } else {
    if (db->f != NULL)
      rewind(db->f);
    else
      db->f = tmpfile();
  }
  if (db->f == NULL)
    _EscIO2(FileNotFound, db->name);
  SETUPBUF(db->f, Char);
  datum = data;
  while (datum != NULL) {
    WITH = datum;
    if (WITH->length > 0) {
      writeseq(db, datum);
      putc(' ', db->f);
      writename(db, WITH->plasmidname);
      putc(' ', db->f);
      writename(db, WITH->primername);
      putc(' ', db->f);
      fprintf(db->f, "%ld", WITH->lanenumber);
      putc(' ', db->f);
      writename(db, WITH->date);
      putc('\n', db->f);
    }
    datum = datum->next;
  }
}



Static Void themain(abi, ssb, thedate, todp, results, summary, db)
_TEXT *abi, *ssb, *thedate, *todp, *results, *summary, *db;
{
  sequence *data, r1, r2;
  name now;
  _TEXT TEMP;

  printf("tod %4.2f\n", version);
  if (*results->name != '\0') {
    if (results->f != NULL)
      results->f = freopen(results->name, "w", results->f);
    else
      results->f = fopen(results->name, "w");
  } else {
    if (results->f != NULL)
      rewind(results->f);
    else
      results->f = tmpfile();
  }
  if (results->f == NULL)
    _EscIO2(FileNotFound, results->name);
  SETUPBUF(results->f, Char);
  fprintf(results->f, "tod %4.2f\n", version);


  readparameters(results, todp, &r1, &r2);


  readdate(results, thedate, &now);


  data = (sequence *)Malloc(sizeof(sequence));
  readssb(ssb, now, &data);


  readsequences(results, abi, &data);

  printf("\nDate/Name: ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writename(&TEMP, now);
  putchar('\n');
  reportlanes(results, data, true);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  reportlanes(&TEMP, data, true);

  checkthedate(thedate, ssb);


  edit(results, todp, &data);


  process(results, r1, r2, &data);


  writedata(db, data);


  if (*summary->name != '\0') {
    if (summary->f != NULL)
      summary->f = freopen(summary->name, "w", summary->f);
    else
      summary->f = fopen(summary->name, "w");
  } else {
    if (summary->f != NULL)
      rewind(summary->f);
    else
      summary->f = tmpfile();
  }
  if (summary->f == NULL)
    _EscIO2(FileNotFound, summary->name);
  SETUPBUF(summary->f, Char);
  fprintf(summary->f, "SUMMARY: \n");
  writesummary(summary, data);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  db.f = NULL;
  strcpy(db.name, "db");
  summary.f = NULL;
  strcpy(summary.name, "summary");
  results.f = NULL;
  strcpy(results.name, "results");
  todp.f = NULL;
  strcpy(todp.name, "todp");
  ssb.f = NULL;
  strcpy(ssb.name, "ssb");
  thedate.f = NULL;
  strcpy(thedate.name, "thedate");
  abi.f = NULL;
  strcpy(abi.name, "abi");
  themain(&abi, &ssb, &thedate, &todp, &results, &summary, &db);
_L1:
  if (abi.f != NULL)
    fclose(abi.f);
  if (thedate.f != NULL)
    fclose(thedate.f);
  if (ssb.f != NULL)
    fclose(ssb.f);
  if (todp.f != NULL)
    fclose(todp.f);
  if (results.f != NULL)
    fclose(results.f);
  if (summary.f != NULL)
    fclose(summary.f);
  if (db.f != NULL)
    fclose(db.f);
  exit(EXIT_SUCCESS);
}



/* End. */
