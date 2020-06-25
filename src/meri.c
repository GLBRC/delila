/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "meri.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.15
/*

















*/
#define updateversion   1.00



/*


















































































*/



#define maxstring       2000



#define infofield       10
#define infodecim       6

#define nfield          8



#define minribl         (-2000)
#define maxribl         2000
#define defnegativeinfinity  (-1000)


/*
*/




typedef struct waveparam {

  Char extreme;
  double wavelocation, wavebit, waveamplitude, wavelength;
  /*




*/
  double dashon;
  /*
*/
  double dashoff, dashoffset, thickness;
  struct waveparam *next;
} waveparam;




typedef enum {
  a, c, g, t
} base;



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;



/*


*/
typedef double rblarray[(long)t - (long)a + 1][maxribl - minribl + 1];

typedef struct ribltype {
  stringDelila riblname, *riblheader;
  rblarray data;
  long numbers[(long)t - (long)a + 1][maxribl - minribl + 1];
  long frombase, tobase;
  double mean, stdev, consensus, anticonsensus, averageRi;
  long n;
  Char symmetry;
  waveparam *waves;
  double cmperbase, cmperbit, Ribound, Zbound, Pbound;
} ribltype;

/*
*/


Static _TEXT ribl5, ribl3, merip, ribl53;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define tab             9






Static boolean isblankDelila(c_)
Char c_;
{
  return (c_ == ' ' || c_ == tab);
}

#undef tab


Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while (isblankDelila(P_peek(thefile->f)) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((!isblankDelila(P_peek(thefile->f))) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}





Static Void emptystring(ribbon)
stringDelila *ribbon;
{
  /*

*/
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}


Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  /*



*/
  emptystring(ribbon);
  ribbon->previous = NULL;
  ribbon->next = NULL;
}


Static Void disposestring(ribbon)
stringDelila **ribbon;
{
  stringDelila *t_;

  t_ = *ribbon;

  while (t_->next != NULL)
    t_ = t_->next;

  while (t_->previous != NULL) {
    t_ = t_->previous;
    Free(t_->next);
  }
  Free(t_);
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*



*/
  printf("remove initializestring routine!\n");
  printf("replace it with clearstring routine!\n");
  halt();
  clearstring(ribbon);
  ribbon->next = NULL;
}



Static Void copystring(a_, b)
stringDelila a_, *b;
{
  /*
*/
  long l;

  b->length = a_.length;
  for (l = 0; l < a_.length; l++)
    b->letters[l] = a_.letters[l];
  b->current = a_.current;
  b->next = a_.next;
}



Static Void concatstring(a_, b, c_)
stringDelila *a_, *b, *c_;
{
  long abindex;
  long cindex = 0;
  long FORLIM;

  emptystring(c_);
  c_->length = a_->length + b->length;
  if (c_->length > maxstring) {
    printf("string too long in concatstring\n");
    halt();
  }

  FORLIM = a_->length;
  for (abindex = 0; abindex < FORLIM; abindex++) {
    cindex++;
    c_->letters[abindex] = a_->letters[abindex];
  }

  FORLIM = a_->length + b->length;
  for (abindex = a_->length; abindex < FORLIM; abindex++) {
    cindex++;
    c_->letters[abindex] = b->letters[abindex - a_->length];
  }
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



Static Void attachcharacterstring(c_, a_)
Char c_;
stringDelila *a_;
{
  if (a_->length > maxstring) {
    printf("attachcharacterstring: maxstring exceeded\n");
    halt();
  }
  a_->length++;
  a_->letters[a_->length - 1] = c_;
}



Static Void readstringline(afile, buffer)
_TEXT *afile;
stringDelila *buffer;
{
  long index = 0;

  clearstring(buffer);
  while (!P_eoln(afile->f) && index < maxstring) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
  }
  if (!P_eoln(afile->f)) {
    printf("readstringline: a line exceeds maximum string size (%ld)\n",
	   (long)maxstring);
    halt();
  }
  buffer->length = index;
  buffer->current = 1;
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
}


#define tabcharacter    9



Static Void detabstring(s)
stringDelila *s;
{
  /*

*/
  Char c_;
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++) {
    c_ = s->letters[i];
    /*


*/
    if (c_ == tabcharacter)
      c_ = ' ';
    s->letters[i] = c_;
  }
}

#undef tabcharacter


#define tabcharacter    9



Static Void readquotestring(afile, s)
_TEXT *afile;
stringDelila *s;
{
  /*
*/
  Char c_;
  long i, FORLIM;

  clearstring(s);
  skipblanks(afile);
  c_ = getc(afile->f);
  if (c_ == '\n')
    c_ = ' ';
  if (c_ != '"') {
    printf("quote string expected but \" missing\n");
    printf("character found instead:\"%c\"\n", c_);
    halt();
  }
  if (P_eoln(afile->f)) {
    printf("missing end of quote string \" string\n");
    halt();
  }
  do {
    c_ = getc(afile->f);
    if (c_ == '\n')
      c_ = ' ';
    if (c_ != '"') {
      s->length++;
      s->letters[s->length - 1] = c_;
    }
  } while (!(((c_ == '"') | P_eoln(afile->f)) || s->length == maxstring));
  if (s->length == maxstring) {
    printf("A quote string exceeded %ld characters.\n", (long)maxstring);
    printf("Use a shorter string or increase constant maxstring.\n");
    halt();
  }
  if (c_ != '"') {
    printf("The second \" mark of a string is missing.\n");
    halt();
  }

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++) {
    if (s->letters[i] == ' ')
      s->letters[i] = (Char)tabcharacter;
  }
}

#undef tabcharacter



Static Void writequotestring(afile, s)
_TEXT *afile;
stringDelila s;
{
  /*


*/
  detabstring(&s);
  putc('"', afile->f);
  writestring(afile, &s);
  putc('"', afile->f);
}



Static Void writestringlines(afile, l)
_TEXT *afile;
stringDelila *l;
{
  stringDelila *h;

  if (l == NULL)
    return;
  h = l;
  while (h != NULL) {
    writestring(afile, h);
    putc('\n', afile->f);
    h = h->next;
  }
}



Static boolean equalstring(a_, b)
stringDelila a_, b;
{
  /*





*/
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



Static Void readheader(afile, ch, theheader)
_TEXT *afile;
Char ch;
stringDelila **theheader;
{
  /*
*/
  stringDelila *h;

  if (BUFEOF(afile->f)) {
    clearstring(*theheader);
    return;
  }
  if (P_peek(afile->f) != ch) {
    *theheader = NULL;
    return;
  }
  *theheader = (stringDelila *)Malloc(sizeof(stringDelila));
  h = *theheader;
  while (P_peek(afile->f) == '*') {
    readstringline(afile, h);
    if (!BUFEOF(afile->f)) {
      if (P_peek(afile->f) == '*') {
	h->next = (stringDelila *)Malloc(sizeof(stringDelila));
	h = h->next;
      } else
	h->next = NULL;
    }
  }
}



Static Void grabtoken(thefile, thestring)
_TEXT *thefile;
stringDelila *thestring;
{
  Char c_;
  boolean done = false;

  skipblanks(thefile);
  thestring->length = 0;
  while (!done) {
    if (P_eoln(thefile->f)) {
      done = true;
      break;
    }
    c_ = getc(thefile->f);
    if (c_ == '\n')
      c_ = ' ';
    if (c_ == ' ')
      done = true;
    else {
      thestring->length++;
      thestring->letters[thestring->length - 1] = c_;
    }
  }
}



Static Void readawaveparameter(afile, wp)
_TEXT *afile;
waveparam *wp;
{
  /*

*/

  /*










*/
  fscanf(afile->f, "%c%*[^\n]", &wp->extreme);
  getc(afile->f);
  if (wp->extreme == '\n')
    wp->extreme = ' ';
  fscanf(afile->f, "%lg%*[^\n]", &wp->wavelocation);
  getc(afile->f);
  fscanf(afile->f, "%lg%*[^\n]", &wp->wavebit);
  getc(afile->f);
  fscanf(afile->f, "%lg%*[^\n]", &wp->waveamplitude);
  getc(afile->f);
  fscanf(afile->f, "%lg%*[^\n]", &wp->wavelength);
  getc(afile->f);
  if (wp->wavelength <= 0.0) {
    printf("wave parameters: wavelength must be positive\n");
    halt();
  }
  /*


*/
  if (BUFEOF(afile->f)) {
    wp->dashon = 0.0;
    wp->dashoff = 0.0;
    wp->dashoffset = 0.0;
    wp->thickness = 0.0;
    return;
  }
  /*
*/
  if (P_peek(afile->f) != 'd') {
    fscanf(afile->f, "%lg%*[^\n]", &wp->dashon);
    getc(afile->f);
    wp->dashoff = wp->dashon;
    wp->dashoffset = 0.0;
  } else {
    getc(afile->f);
    /*




*/
    fscanf(afile->f, "%lg%lg%lg%*[^\n]", &wp->dashon, &wp->dashoff,
	   &wp->dashoffset);
    getc(afile->f);

    /*






























*/

  }
  if (BUFEOF(afile->f))
    wp->thickness = 0.0;
  else {
    fscanf(afile->f, "%lg%*[^\n]", &wp->thickness);
    getc(afile->f);
  }
}


/* Local variables for readwaveparameters: */
struct LOC_readwaveparameters {
  _TEXT *afile;
  boolean done;
} ;

Local Void waystoend(LINK)
struct LOC_readwaveparameters *LINK;
{
  boolean clear = false;

  if (BUFEOF(LINK->afile->f))
    LINK->done = true;
  if (!LINK->done) {
    while (!clear && !LINK->done) {
      if (BUFEOF(LINK->afile->f)) {
	LINK->done = true;
	clear = true;
      } else if (P_peek(LINK->afile->f) == '*') {
	fscanf(LINK->afile->f, "%*[^\n]");
	getc(LINK->afile->f);
      } else
	clear = true;
    }
  }
  if (!LINK->done) {
    if (P_eoln(LINK->afile->f))
      LINK->done = true;
  }
  if (LINK->done)
    return;
  if (P_peek(LINK->afile->f) != '.')
    return;
  fscanf(LINK->afile->f, "%*[^\n]");
  getc(LINK->afile->f);
  LINK->done = true;
}


Static Void readwaveparameters(afile_, w)
_TEXT *afile_;
waveparam **w;
{
  /*

*/
  /*

*/
  struct LOC_readwaveparameters V;
  boolean morethanone = false;
  waveparam *p;

  V.afile = afile_;
  V.done = false;
  *w = NULL;
  waystoend(&V);
  if (V.done) {
    *w = NULL;
    return;
  }
  *w = (waveparam *)Malloc(sizeof(waveparam));
  p = *w;
  while (!V.done) {
    waystoend(&V);
    if (V.done) {
      p->next = NULL;
      continue;
    }
    if (P_peek(V.afile->f) == '*') {
      fscanf(V.afile->f, "%*[^\n]");
      getc(V.afile->f);
      continue;
    }
    if (morethanone) {
      p->next = (waveparam *)Malloc(sizeof(waveparam));
      p = p->next;
    }
    /*

*/
    readawaveparameter(V.afile, p);

    morethanone = true;
  }
}


#define wid             4



Static Void writewaveparameters(afile, w)
_TEXT *afile;
waveparam *w;
{
  waveparam *i = w;
  waveparam *WITH;

  while (i != NULL) {
    WITH = i;
    fprintf(afile->f,
      "%c  extreme: char; h or l, the high or low extreme to be defined\n",
      WITH->extreme);
    fprintf(afile->f,
      "%*.1f  wavelocation: real; the location in bases of the extreme\n",
      wid, WITH->wavelocation);
    fprintf(afile->f,
	    "%*.1f  wavebit: real; the location in bits of the extreme\n",
	    wid, WITH->wavebit);
    fprintf(afile->f,
	    "%*.1f  waveamplitude: real; the amplitude of the wave in bits\n",
	    wid, WITH->waveamplitude);
    fprintf(afile->f,
	    "%*.1f  wavelength: real; the wave length of the wave in bases\n",
	    wid, WITH->wavelength);
    /*







*/
    fprintf(afile->f,
	    "%*.1f %*.1f %*.1f dashon, dashoff, dashoffset (in bases)\n",
	    wid, WITH->dashon, wid, WITH->dashoff, wid, WITH->dashoffset);
    fprintf(afile->f,
      "%*.1f  thickness: real; the thickness of the cosine wave.  <=0 means default\n",
      wid, WITH->thickness);
    i = i->next;
  }
  fprintf(afile->f, ".\n");
}

#undef wid


#define clearvalue      5.0


/* Local variables for getriblmatrix: */
struct LOC_getriblmatrix {
  _TEXT *afile;
} ;

Local Void testforend(LINK)
struct LOC_getriblmatrix *LINK;
{
  if (BUFEOF(LINK->afile->f)) {
    printf("part or all of the ribl matrix is missing\n");
    halt();
  }
}

Local Void skip(LINK)
struct LOC_getriblmatrix *LINK;
{
  testforend(LINK);
  while (P_peek(LINK->afile->f) == '*') {
    fscanf(LINK->afile->f, "%*[^\n]");
    getc(LINK->afile->f);
    testforend(LINK);
  }
}



Static Void getriblmatrix(afile_, matrix)
_TEXT *afile_;
ribltype **matrix;
{
  /*

*/
  struct LOC_getriblmatrix V;
  /*
*/
  base b;
  long l, thel;

  stringDelila teststring, riblstart;
  ribltype *WITH;
  _TEXT TEMP;
  long FORLIM;

  V.afile = afile_;
  *matrix = (ribltype *)Malloc(sizeof(ribltype));
  WITH = *matrix;


  grabtoken(V.afile, &riblstart);
  teststring.letters[0] = 'r';
  teststring.letters[1] = 'i';
  teststring.letters[2] = 'b';
  teststring.letters[3] = 'l';
  teststring.length = 4;

  if (!equalstring(riblstart, teststring)) {
    printf("The ribl matrix must begin with 'ribl \"name\"'\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, riblstart);
    printf(" seen instead\n");
    halt();
  }

  skipblanks(V.afile);
  readquotestring(V.afile, &(*matrix)->riblname);
  fscanf(V.afile->f, "%*[^\n]");

  getc(V.afile->f);
  readheader(V.afile, '*', &WITH->riblheader);

  fscanf(V.afile->f, "%ld%ld%*[^\n]", &WITH->frombase, &WITH->tobase);
  getc(V.afile->f);

  if (WITH->frombase < minribl) {
    printf("Matrix is too large for program.\n");
    printf("frombase (=%ld)\n", WITH->frombase);
    printf("minribl (=%ld)\n", (long)minribl);
    printf("Decrease minribl or increase frombase\n");
    halt();
  }

  if (WITH->tobase > maxribl) {
    printf("Matrix is too large for program.\n");
    printf("tobase (=%ld)\n", WITH->tobase);
    printf("maxribl (=%ld)\n", (long)maxribl);
    printf("Increase maxribl or decrease tobase\n");
    halt();
  }


  FORLIM = WITH->frombase;
  for (l = minribl; l < FORLIM; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      (*matrix)->data[(long)b - (long)a][l - minribl] = clearvalue;
  }
  for (l = WITH->tobase + 1; l <= maxribl; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      (*matrix)->data[(long)b - (long)a][l - minribl] = clearvalue;
  }

  FORLIM = WITH->tobase;
  for (l = WITH->frombase; l <= FORLIM; l++) {
    testforend(&V);
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fscanf(V.afile->f, "%lg", &(*matrix)->data[(long)b - (long)a]
	     [l - minribl]);
    fscanf(V.afile->f, "%ld", &thel);
    if (thel != l) {
      printf("positions in ribl do not match from-to range\n");
      halt();
    }
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fscanf(V.afile->f, "%ld", &(*matrix)->numbers[(long)b - (long)a]
	     [l - minribl]);
    fscanf(V.afile->f, "%*[^\n]");
    getc(V.afile->f);
  }

  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->mean);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->stdev);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->consensus);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->anticonsensus);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->averageRi);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%ld%*[^\n]", &WITH->n);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%c%*[^\n]", &WITH->symmetry);
  getc(V.afile->f);
  if (WITH->symmetry == '\n')
    WITH->symmetry = ' ';
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->Ribound);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->Zbound);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->Pbound);
  getc(V.afile->f);
  /*



*/
  readwaveparameters(V.afile, &WITH->waves);
}

#undef clearvalue



Static Void putriblmatrix(afile, matrix)
_TEXT *afile;
ribltype *matrix;
{
  long lindex, FORLIM;

  if (matrix == NULL)
    return;
  fprintf(afile->f, "ribl");
  putc(' ', afile->f);
  writequotestring(afile, matrix->riblname);
  putc('\n', afile->f);

  writestringlines(afile, matrix->riblheader);
  fprintf(afile->f, "%*ld %*ld frombase, tobase\n",
	  nfield, matrix->frombase, nfield, matrix->tobase);
  FORLIM = matrix->tobase;
  for (lindex = matrix->frombase; lindex <= FORLIM; lindex++) {
    fprintf(afile->f, "  %*.*f %*.*f %*.*f %*.*f %*ld",
	    infofield, infodecim, matrix->data[0][lindex - minribl],
	    infofield, infodecim, matrix->data[(long)c - (long)a]
	    [lindex - minribl], infofield, infodecim,
	    matrix->data[(long)g - (long)a][lindex - minribl], infofield,
	    infodecim, matrix->data[(long)t - (long)a][lindex - minribl],
	    nfield, lindex);
    fprintf(afile->f, " %*ld %*ld %*ld %*ld\n",
	    nfield, matrix->numbers[0][lindex - minribl], nfield,
	    matrix->numbers[(long)c - (long)a][lindex - minribl], nfield,
	    matrix->numbers[(long)g - (long)a][lindex - minribl], nfield,
	    matrix->numbers[(long)t - (long)a][lindex - minribl]);
  }
  fprintf(afile->f, "*\n");
  fprintf(afile->f, "%*.*f bits = mean (Rsequence of selected region)\n",
	  infofield, infodecim, matrix->mean);
  fprintf(afile->f, "%*.*f bits = standard deviation\n",
	  infofield, infodecim, matrix->stdev);
  fprintf(afile->f, "*\n");
  fprintf(afile->f, "%*.*f bits = Ri of consensus sequence from %ld to %ld\n",
	  infofield, infodecim, matrix->consensus, matrix->frombase,
	  matrix->tobase);
  fprintf(afile->f,
	  "%*.*f bits = Ri of anticonsensus sequence from %ld to %ld\n",
	  infofield, infodecim, matrix->anticonsensus, matrix->frombase,
	  matrix->tobase);
  fprintf(afile->f, "*\n");
  fprintf(afile->f,
	  "%*.*f bits = average Ri for random sequence from %ld to %ld\n",
	  infofield, infodecim, matrix->averageRi, matrix->frombase,
	  matrix->tobase);
  fprintf(afile->f, "*\n");
  fprintf(afile->f, "%ld n, number of sequences used to create the matrix\n",
	  matrix->n);
  fprintf(afile->f, "*\n");
  switch (matrix->symmetry) {

  case 'a':
    fprintf(afile->f, "asymmetric");
    break;

  case 'e':
    fprintf(afile->f, "even");
    break;

  case 'o':
    fprintf(afile->f, "odd");
    break;
  }
  fprintf(afile->f, " symmetry of the matrix\n");
  fprintf(afile->f, "*\n");
  fprintf(afile->f, "%*.*f Ri bound: lower bound on Ri\n",
	  infofield, infodecim, matrix->Ribound);
  fprintf(afile->f, "%*.*f Z bound: lower bound on Z\n",
	  infofield, infodecim, matrix->Zbound);
  fprintf(afile->f, "%*.*f P bound: upper probability\n",
	  infofield, infodecim, matrix->Pbound);
  fprintf(afile->f, "*\n");
  writewaveparameters(afile, matrix->waves);
  putc('\n', afile->f);
}



Static boolean between(a_, b, c_)
double a_, b, c_;
{
  return (a_ <= b && b <= c_);
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


#define extrafield      10



Static Void themain(ribl5, ribl3, merip, ribl53)
_TEXT *ribl5, *ribl3, *merip, *ribl53;
{
  double parameterversion;
  stringDelila ribl53name;
  long ribl5from, ribl5to, ribl3from, ribl3to;
  ribltype *matrix5, *matrix3, *matrix53;
  long zero, l, lold;
  base b;
  stringDelila spare1, spare2;
  long lo, hi, va;
  base bl, bh;
  double Risum, nl, sum, sumRi2;
  _TEXT TEMP;
  long FORLIM;

  printf("meri %4.2f\n", version);
  if (*merip->name != '\0') {
    if (merip->f != NULL)
      merip->f = freopen(merip->name, "r", merip->f);
    else
      merip->f = fopen(merip->name, "r");
  } else
    rewind(merip->f);
  if (merip->f == NULL)
    _EscIO2(FileNotFound, merip->name);
  RESETBUF(merip->f, Char);
  fscanf(merip->f, "%lg%*[^\n]", &parameterversion);
  getc(merip->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  grabtoken(merip, &ribl53name);

  fscanf(merip->f, "%*[^\n]");
  getc(merip->f);
  fscanf(merip->f, "%ld%ld%*[^\n]", &ribl5from, &ribl5to);
  getc(merip->f);
  fscanf(merip->f, "%ld%ld%*[^\n]", &ribl3from, &ribl3to);
  getc(merip->f);
  fscanf(merip->f, "%ld%*[^\n]", &zero);
  getc(merip->f);


  if (ribl5from > ribl5to) {
    printf("ribl5from (=%ld) must not exceed ribl5to (=%ld) \n",
	   ribl5from, ribl5to);
    halt();
  }
  if (ribl3from > ribl3to) {
    printf("ribl3from (=%ld) must not exceed ribl3to (=%ld) \n",
	   ribl3from, ribl3to);
    halt();
  }


  if (*ribl5->name != '\0') {
    if (ribl5->f != NULL)
      ribl5->f = freopen(ribl5->name, "r", ribl5->f);
    else
      ribl5->f = fopen(ribl5->name, "r");
  } else
    rewind(ribl5->f);
  if (ribl5->f == NULL)
    _EscIO2(FileNotFound, ribl5->name);
  RESETBUF(ribl5->f, Char);
  getriblmatrix(ribl5, &matrix5);
  if (*ribl3->name != '\0') {
    if (ribl3->f != NULL)
      ribl3->f = freopen(ribl3->name, "r", ribl3->f);
    else
      ribl3->f = fopen(ribl3->name, "r");
  } else
    rewind(ribl3->f);
  if (ribl3->f == NULL)
    _EscIO2(FileNotFound, ribl3->name);
  RESETBUF(ribl3->f, Char);
  getriblmatrix(ribl3, &matrix3);

  printf("Fusing ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &matrix5->riblname);
  printf(" to ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &matrix3->riblname);
  putchar('\n');

  if (*ribl53->name != '\0') {
    if (ribl53->f != NULL)
      ribl53->f = freopen(ribl53->name, "w", ribl53->f);
    else
      ribl53->f = fopen(ribl53->name, "w");
  } else {
    if (ribl53->f != NULL)
      rewind(ribl53->f);
    else
      ribl53->f = tmpfile();
  }
  if (ribl53->f == NULL)
    _EscIO2(FileNotFound, ribl53->name);
  SETUPBUF(ribl53->f, Char);
  printf("new name: ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &ribl53name);
  putchar('\n');


  if (!between((double)matrix5->frombase, (double)ribl5from,
	       (double)matrix5->tobase)) {
    printf("ribl5from (=%ld) must be in the range of ", ribl5from);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &matrix5->riblname);
    printf(": from %ld", matrix5->frombase);
    printf(" to %ld\n", matrix5->tobase);
    halt();
  }
  if (!between((double)matrix5->frombase, (double)ribl5to,
	       (double)matrix5->tobase)) {
    printf("ribl5to (=%ld) must be in the range of ", ribl5to);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &matrix5->riblname);
    printf(": from %ld", matrix5->frombase);
    printf(" to %ld\n", matrix5->tobase);
    halt();
  }
  if (!between((double)matrix3->frombase, (double)ribl3from,
	       (double)matrix3->tobase)) {
    printf("ribl3from (=%ld) must be in the range of ", ribl3from);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &matrix3->riblname);
    printf(": from %ld", matrix3->frombase);
    printf(" to %ld\n", matrix3->tobase);
    halt();
  }
  if (!between((double)matrix3->frombase, (double)ribl3to,
	       (double)matrix3->tobase)) {
    printf("ribl3to (=%ld) must be in the range of ", ribl3to);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &matrix3->riblname);
    printf(": from %ld", matrix3->frombase);
    printf(" to %ld\n", matrix3->tobase);
    halt();
  }

  matrix53 = (ribltype *)Malloc(sizeof(ribltype));
  clearstring(&spare1);
  clearstring(&spare2);


  matrix53->riblheader = (stringDelila *)Malloc(sizeof(stringDelila));
  clearstring(matrix53->riblheader);
  attachcharacterstring('*', &spare1);
  attachcharacterstring(' ', &spare1);
  concatstring(&spare1, &matrix5->riblname, &spare2);
  attachcharacterstring('_', &spare2);
  concatstring(&spare2, &matrix3->riblname, matrix53->riblheader);
  printf("Header: \n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, matrix53->riblheader);
  putchar('\n');

  if (ribl53name.length > 0) {
    if (ribl53name.letters[0] != '+')
      copystring(ribl53name, &matrix53->riblname);
    else
      copystring(*matrix53->riblheader, &matrix53->riblname);
  } else
    copystring(*matrix53->riblheader, &matrix53->riblname);


  putchar('\n');
  /*
*/
  matrix53->frombase = 1 - zero;

  matrix53->tobase = matrix53->frombase + ribl5to - ribl5from +
		     ribl3to - ribl3from + 1;

  /*
*/
  printf("new range: %ld to %ld\n", matrix53->frombase, matrix53->tobase);
  printf("%*s%*s%*s\n",
	 nfield + extrafield, "5'_coordinate", nfield + extrafield,
	 "3'_coordinate", nfield + extrafield, "new_coordinate");
  FORLIM = matrix53->tobase;
  for (l = matrix53->frombase; l <= FORLIM; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      lold = l - matrix53->frombase + ribl5from;
      if (lold <= ribl5to) {
	if (b == a) {
	  printf("%*ld", nfield + extrafield, lold);
	  printf("%*c", nfield + extrafield, ' ');
	}

	matrix53->data[(long)b - (long)a]
	  [l - minribl] = matrix5->data[(long)b - (long)a][lold - minribl];
	matrix53->numbers[(long)b - (long)a]
	  [l - minribl] = matrix5->numbers[(long)b - (long)a][lold - minribl];
      } else {
	lold = l - matrix53->frombase + ribl3from + ribl5from;
	if (b == a) {
	  printf("%*c", nfield + extrafield, ' ');
	  printf("%*ld", nfield + extrafield, lold);
	}
	matrix53->data[(long)b - (long)a]
	  [l - minribl] = matrix3->data[(long)b - (long)a][lold - minribl];
	matrix53->numbers[(long)b - (long)a]
	  [l - minribl] = matrix3->numbers[(long)b - (long)a][lold - minribl];
      }
    }

    printf("%*ld\n", nfield + extrafield, l);
  }




  putchar('\n');
  matrix53->mean = 0.0;
  matrix53->stdev = 0.0;
  FORLIM = matrix53->tobase;
  for (l = matrix53->frombase; l <= FORLIM; l++) {
    nl = 0.0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      nl += matrix53->numbers[(long)b - (long)a][l - minribl];
    sum = 0.0;
    sumRi2 = 0.0;

    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      sum += matrix53->data[(long)b - (long)a]
	     [l - minribl] * (matrix53->numbers[(long)b - (long)a]
			      [l - minribl] / nl);
    matrix53->mean += sum;
  }
  printf("Mean is computed; standard deviation is set to zero.\n\n");




  /*








*/
  /*
























*/



  printf("%*s%*s%*s\n",
	 (int)(nfield + 3), "coordinate", (int)(nfield + 6), " anticonsensus",
	 (int)(nfield + 2), " consensus");
  matrix53->consensus = 0.0;
  matrix53->anticonsensus = 0.0;
  FORLIM = matrix53->tobase;
  for (l = matrix53->frombase; l <= FORLIM; l++) {
    printf("%*ld", (int)(nfield + 3), l);
    lo = LONG_MAX;
    hi = -LONG_MAX;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      if (matrix53->numbers[(long)b - (long)a][l - minribl] < lo) {
	lo = matrix53->numbers[(long)b - (long)a][l - minribl];
	va = lo;
	bl = b;
      }
      if (matrix53->numbers[(long)b - (long)a][l - minribl] > hi) {
	hi = matrix53->numbers[(long)b - (long)a][l - minribl];
	va = hi;
	bh = b;
      }
    }
    matrix53->consensus += matrix53->data[(long)bh - (long)a][l - minribl];
    matrix53->anticonsensus += matrix53->data[(long)bl - (long)a][l - minribl];
    printf("%*c%*c\n",
	   (int)(nfield + 6), basetochar(bl), (int)(nfield + 2),
	   basetochar(bh));
  }


  matrix53->averageRi = 0.0;
  FORLIM = matrix53->tobase;
  for (l = matrix53->frombase; l <= FORLIM; l++) {
    Risum = 0.0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      Risum += matrix53->data[(long)b - (long)a][l - minribl];
    matrix53->averageRi += Risum / 4;
  }


  printf("\nn for 5' ribl: %*ld\n", nfield, matrix5->n);
  printf("n for 3' ribl: %*ld\n", nfield, matrix3->n);
  if (matrix5->n > matrix3->n) {
    printf("Using 5' ribl n\n");
    matrix53->n = matrix5->n;
  } else {
    printf("Using 3' ribl n\n");
    matrix53->n = matrix3->n;
  }


  matrix53->symmetry = 'a';

  matrix53->waves = NULL;
  matrix53->cmperbase = 0.0;
  matrix53->cmperbit = 0.0;

  printf("\nUsing 5' ribl for Ribound, Zbound, Pbound\n");
  matrix53->Ribound = matrix5->Ribound;
  matrix53->Zbound = matrix5->Zbound;
  matrix53->Pbound = matrix5->Pbound;

  printf("\nOutput resulting fused matrix, ribl53\n");
  putriblmatrix(ribl53, matrix53);


}

#undef extrafield


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  ribl53.f = NULL;
  strcpy(ribl53.name, "ribl53");
  merip.f = NULL;
  strcpy(merip.name, "merip");
  ribl3.f = NULL;
  strcpy(ribl3.name, "ribl3");
  ribl5.f = NULL;
  strcpy(ribl5.name, "ribl5");
  themain(&ribl5, &ribl3, &merip, &ribl53);
_L1:
  if (ribl5.f != NULL)
    fclose(ribl5.f);
  if (ribl3.f != NULL)
    fclose(ribl3.f);
  if (merip.f != NULL)
    fclose(merip.f);
  if (ribl53.f != NULL)
    fclose(ribl53.f);
  exit(EXIT_SUCCESS);
}

/*



























*/



/* End. */
