/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rila.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.43
/*














*/



/*































































































































*/



#define minribl         (-2000)
#define maxribl         2000
#define defnegativeinfinity  (-1000)
/*
*/



#define maxstring       2000




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;



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



Static _TEXT ribl, headlatex, labelatex, sequlatex, senulatex, freqlatex,
	     ribllatex, taillatex, rilap;


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



Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
  ribbon->next = NULL;
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
  clearstring(ribbon);
  ribbon->next = NULL;
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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}


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



Static Char capitalize(c_)
Char c_;
{
  long n = c_;

  if (n >= 'a' && n <= 'z')
    c_ = _toupper(n);
  return c_;
}



Static Char decapitalize(c_)
Char c_;
{
  long n = c_;

  if (n >= 'A' && n <= 'Z')
    c_ = _tolower(n);
  else
    c_ = (Char)n;
  return c_;
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


/* Local variables for PatentMessage: */
struct LOC_PatentMessage {
  _TEXT *f;
} ;

Local Void makebar(LINK)
struct LOC_PatentMessage *LINK;
{
  fprintf(LINK->f->f,
    "**********************************************************************\n");
}



Static Void PatentMessage(f_)
_TEXT *f_;
{
  /*

*/
  struct LOC_PatentMessage V;

  V.f = f_;
  makebar(&V);
  fprintf(V.f->f,
    "* By downloading this code you agree to the Source Code Use License: *\n");
  fprintf(V.f->f,
    "* https://alum.mit.edu/www/toms/Source_Code_Use_License.txt           *\n");
  fprintf(V.f->f,
    "* Contact: https://alum.mit.edu/www/toms/contacts.html                *\n");
  makebar(&V);
}


#define wid             1


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *ribl, *rilap, *headlatex, *labelatex, *sequlatex, *senulatex,
	*freqlatex, *ribllatex, *taillatex;
  long askedfrom, askedto;
  boolean caps;
  long decimals;
  boolean dobox, dobold, hline;
  long length;
  ribltype *matrix;
  double negativeinfinity;
  Char sequence[20];
} ;

Local Void startfiles(LINK)
struct LOC_themain *LINK;
{
  _TEXT TEMP;

  printf("rila %4.2f\n", version);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  PatentMessage(&TEMP);

  if (*LINK->headlatex->name != '\0') {
    if (LINK->headlatex->f != NULL)
      LINK->headlatex->f = freopen(LINK->headlatex->name, "w",
				   LINK->headlatex->f);
    else
      LINK->headlatex->f = fopen(LINK->headlatex->name, "w");
  } else {
    if (LINK->headlatex->f != NULL)
      rewind(LINK->headlatex->f);
    else
      LINK->headlatex->f = tmpfile();
  }
  if (LINK->headlatex->f == NULL)
    _EscIO2(FileNotFound, LINK->headlatex->name);
  SETUPBUF(LINK->headlatex->f, Char);
  if (*LINK->labelatex->name != '\0') {
    if (LINK->labelatex->f != NULL)
      LINK->labelatex->f = freopen(LINK->labelatex->name, "w",
				   LINK->labelatex->f);
    else
      LINK->labelatex->f = fopen(LINK->labelatex->name, "w");
  } else {
    if (LINK->labelatex->f != NULL)
      rewind(LINK->labelatex->f);
    else
      LINK->labelatex->f = tmpfile();
  }
  if (LINK->labelatex->f == NULL)
    _EscIO2(FileNotFound, LINK->labelatex->name);
  SETUPBUF(LINK->labelatex->f, Char);
  if (*LINK->sequlatex->name != '\0') {
    if (LINK->sequlatex->f != NULL)
      LINK->sequlatex->f = freopen(LINK->sequlatex->name, "w",
				   LINK->sequlatex->f);
    else
      LINK->sequlatex->f = fopen(LINK->sequlatex->name, "w");
  } else {
    if (LINK->sequlatex->f != NULL)
      rewind(LINK->sequlatex->f);
    else
      LINK->sequlatex->f = tmpfile();
  }
  if (LINK->sequlatex->f == NULL)
    _EscIO2(FileNotFound, LINK->sequlatex->name);
  SETUPBUF(LINK->sequlatex->f, Char);
  if (*LINK->senulatex->name != '\0') {
    if (LINK->senulatex->f != NULL)
      LINK->senulatex->f = freopen(LINK->senulatex->name, "w",
				   LINK->senulatex->f);
    else
      LINK->senulatex->f = fopen(LINK->senulatex->name, "w");
  } else {
    if (LINK->senulatex->f != NULL)
      rewind(LINK->senulatex->f);
    else
      LINK->senulatex->f = tmpfile();
  }
  if (LINK->senulatex->f == NULL)
    _EscIO2(FileNotFound, LINK->senulatex->name);
  SETUPBUF(LINK->senulatex->f, Char);
  if (*LINK->freqlatex->name != '\0') {
    if (LINK->freqlatex->f != NULL)
      LINK->freqlatex->f = freopen(LINK->freqlatex->name, "w",
				   LINK->freqlatex->f);
    else
      LINK->freqlatex->f = fopen(LINK->freqlatex->name, "w");
  } else {
    if (LINK->freqlatex->f != NULL)
      rewind(LINK->freqlatex->f);
    else
      LINK->freqlatex->f = tmpfile();
  }
  if (LINK->freqlatex->f == NULL)
    _EscIO2(FileNotFound, LINK->freqlatex->name);
  SETUPBUF(LINK->freqlatex->f, Char);
  if (*LINK->ribllatex->name != '\0') {
    if (LINK->ribllatex->f != NULL)
      LINK->ribllatex->f = freopen(LINK->ribllatex->name, "w",
				   LINK->ribllatex->f);
    else
      LINK->ribllatex->f = fopen(LINK->ribllatex->name, "w");
  } else {
    if (LINK->ribllatex->f != NULL)
      rewind(LINK->ribllatex->f);
    else
      LINK->ribllatex->f = tmpfile();
  }
  if (LINK->ribllatex->f == NULL)
    _EscIO2(FileNotFound, LINK->ribllatex->name);
  SETUPBUF(LINK->ribllatex->f, Char);
  if (*LINK->taillatex->name != '\0') {
    if (LINK->taillatex->f != NULL)
      LINK->taillatex->f = freopen(LINK->taillatex->name, "w",
				   LINK->taillatex->f);
    else
      LINK->taillatex->f = fopen(LINK->taillatex->name, "w");
  } else {
    if (LINK->taillatex->f != NULL)
      rewind(LINK->taillatex->f);
    else
      LINK->taillatex->f = tmpfile();
  }
  if (LINK->taillatex->f == NULL)
    _EscIO2(FileNotFound, LINK->taillatex->name);
  SETUPBUF(LINK->taillatex->f, Char);
  fprintf(LINK->headlatex->f, "%% rila %4.2f head\n", version);
  fprintf(LINK->labelatex->f, "%% rila %4.2f label\n", version);
  fprintf(LINK->sequlatex->f, "%% rila %4.2f sequence as letters\n", version);
  fprintf(LINK->senulatex->f, "%% rila %4.2f sequence as numbers\n", version);
  fprintf(LINK->freqlatex->f, "%% rila %4.2f f(b,l) matrix\n", version);
  fprintf(LINK->ribllatex->f, "%% rila %4.2f Ri(b,l) matrix\n", version);
  fprintf(LINK->taillatex->f, "%% rila %4.2f tail\n", version);
}

Local Void readparameters(LINK)
struct LOC_themain *LINK;
{
  long p, FORLIM;

  if (*LINK->rilap->name != '\0') {
    if (LINK->rilap->f != NULL)
      LINK->rilap->f = freopen(LINK->rilap->name, "r", LINK->rilap->f);
    else
      LINK->rilap->f = fopen(LINK->rilap->name, "r");
  } else
    rewind(LINK->rilap->f);
  if (LINK->rilap->f == NULL)
    _EscIO2(FileNotFound, LINK->rilap->name);
  RESETBUF(LINK->rilap->f, Char);
  if (BUFEOF(LINK->rilap->f)) {
    printf("Missing data in rilap\n");
    halt();
  }

  fscanf(LINK->rilap->f, "%ld%ld%*[^\n]", &LINK->askedfrom, &LINK->askedto);
  getc(LINK->rilap->f);
  if (LINK->askedto < minribl || LINK->askedfrom > maxribl) {
    printf(
      "cannot handle this much data; use smaller range or increase constants minribl or maxribl\n");
    halt();
  }

  fscanf(LINK->rilap->f, "%lg%*[^\n]", &LINK->negativeinfinity);
  getc(LINK->rilap->f);


  if (!BUFEOF(LINK->rilap->f)) {
    LINK->length = 0;
    while (!P_eoln(LINK->rilap->f)) {
      LINK->length++;
      LINK->sequence[LINK->length-1] = getc(LINK->rilap->f);
      if (LINK->sequence[LINK->length-1] == '\n')
	LINK->sequence[LINK->length-1] = ' ';
      LINK->sequence[LINK->length-1] = decapitalize(LINK->sequence[LINK->length-1]);
      putchar(LINK->sequence[LINK->length-1]);
      if (LINK->sequence[LINK->length-1] != 't' &&
	  LINK->sequence[LINK->length-1] != 'g' &&
	  LINK->sequence[LINK->length-1] != 'c' &&
	  LINK->sequence[LINK->length-1] != 'a') {
	printf("sequence must be characters acgt only\n");
	halt();
      }
    }
    putchar('\n');
  }
  if (LINK->length > 0) {
    fprintf(LINK->labelatex->f, "%% sequence read in: ");
    FORLIM = LINK->length;
    for (p = 0; p < FORLIM; p++)
      putc(LINK->sequence[p], LINK->labelatex->f);
    fprintf(LINK->labelatex->f, " is of length %ld\n", LINK->length);
  }

  fscanf(LINK->rilap->f, "%ld%*[^\n]", &LINK->decimals);
  getc(LINK->rilap->f);

  if ((P_peek(LINK->rilap->f) == 'c') | (P_peek(LINK->rilap->f) == 'u'))
    LINK->caps = true;
  else
    LINK->caps = false;
  fscanf(LINK->rilap->f, "%*[^\n]");

  getc(LINK->rilap->f);
  if (P_peek(LINK->rilap->f) == 'h')
    LINK->hline = true;
  else
    LINK->hline = false;
  fscanf(LINK->rilap->f, "%*[^\n]");

  getc(LINK->rilap->f);
  if (P_peek(LINK->rilap->f) == 'b')
    LINK->dobox = true;
  else
    LINK->dobox = false;
  fscanf(LINK->rilap->f, "%*[^\n]");

  getc(LINK->rilap->f);
  LINK->dobold = false;
  if (BUFEOF(LINK->rilap->f))
    return;

  if (P_eoln(LINK->rilap->f))
    return;
  if (P_peek(LINK->rilap->f) == 'b')
    LINK->dobold = true;
  fscanf(LINK->rilap->f, "%*[^\n]");
  getc(LINK->rilap->f);
}

Local Void db(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  if (LINK->dobold)
    fprintf(f->f, "\\textbf{");
}

Local Void de(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  if (LINK->dobold)
    putc('}', f->f);
}

Local Void dm(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  if (LINK->dobold)
    fprintf(f->f, "\\mathbf{");
}

Local Void setupheaders(LINK)
struct LOC_themain *LINK;
{
  long position;
  _TEXT TEMP;
  long FORLIM;

  fprintf(LINK->labelatex->f, "%% \n");
  fprintf(LINK->labelatex->f, "%% Ri(b,l) TABLE: -------------------------\n");


  if (*LINK->ribl->name != '\0') {
    if (LINK->ribl->f != NULL)
      LINK->ribl->f = freopen(LINK->ribl->name, "r", LINK->ribl->f);
    else
      LINK->ribl->f = fopen(LINK->ribl->name, "r");
  } else
    rewind(LINK->ribl->f);
  if (LINK->ribl->f == NULL)
    _EscIO2(FileNotFound, LINK->ribl->name);
  RESETBUF(LINK->ribl->f, Char);
  fprintf(LINK->labelatex->f, "%% ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  copyaline(LINK->ribl, &TEMP);
  while (P_peek(LINK->ribl->f) == '*') {
    fprintf(LINK->labelatex->f, "%% ");
    copyaline(LINK->ribl, LINK->labelatex);
  }

  if (LINK->askedfrom < LINK->matrix->frombase) {
    printf("requested from, %ld, is out of range of the ribl%ld to %ld\n",
	   LINK->askedfrom, LINK->matrix->frombase, LINK->matrix->tobase);
    halt();
  }

  if (LINK->askedto > LINK->matrix->tobase) {
    printf("requested to, %ld, is out of range of the ribl%ld to %ld\n",
	   LINK->askedto, LINK->matrix->frombase, LINK->matrix->tobase);
    halt();
  }

  fprintf(LINK->labelatex->f, "%% ----------------------------------------\n");
  fprintf(LINK->labelatex->f, "%% \n");

  fprintf(LINK->labelatex->f, "%% from %ld to %ld\n",
	  LINK->askedfrom, LINK->askedto);

  /*


*/
  db(LINK->labelatex, LINK);
  fprintf(LINK->labelatex->f, "base ");
  de(LINK->labelatex, LINK);
  putc('$', LINK->labelatex->f);
  dm(LINK->labelatex, LINK);
  putc('b', LINK->labelatex->f);
  de(LINK->labelatex, LINK);
  putc('$', LINK->labelatex->f);
  fprintf(LINK->labelatex->f, " & \\multicolumn{%ld}{|c|}{", LINK->length);
  db(LINK->labelatex, LINK);
  fprintf(LINK->labelatex->f, "position ");
  de(LINK->labelatex, LINK);
  putc('$', LINK->labelatex->f);
  dm(LINK->labelatex, LINK);
  putc('l', LINK->labelatex->f);
  de(LINK->labelatex, LINK);
  fprintf(LINK->labelatex->f, "$} \\\\\n");

  fprintf(LINK->headlatex->f, "\\begin{tabular}{|c|");
  FORLIM = LINK->length;
  for (position = 1; position <= FORLIM; position++)
    putc('r', LINK->headlatex->f);
  fprintf(LINK->headlatex->f, "|}\n");

  FORLIM = LINK->length;
  for (position = 0; position < FORLIM; position++) {
    fprintf(LINK->sequlatex->f, " & ");
    db(LINK->sequlatex, LINK);
    if (LINK->caps)
      fputc(capitalize(LINK->sequence[position]), LINK->sequlatex->f);
    else
      fputc(decapitalize(LINK->sequence[position]), LINK->sequlatex->f);
    de(LINK->sequlatex, LINK);
  }
  fprintf(LINK->sequlatex->f, " \\\\\n");

}



Static Void themain(ribl_, rilap_, headlatex_, labelatex_, sequlatex_,
		    senulatex_, freqlatex_, ribllatex_, taillatex_)
_TEXT *ribl_, *rilap_, *headlatex_, *labelatex_, *sequlatex_, *senulatex_,
      *freqlatex_, *ribllatex_, *taillatex_;
{
  struct LOC_themain V;
  base b;
  boolean closebox;
  long column;
  /*
*/
  long count = 0;
  long l;
  Char thebase;

  long wraplimit = 100;
  /*
*/

  long sum;
  base bsum;
  long FORLIM, FORLIM1;


  V.ribl = ribl_;
  V.rilap = rilap_;
  V.headlatex = headlatex_;
  V.labelatex = labelatex_;
  V.sequlatex = sequlatex_;
  V.senulatex = senulatex_;
  V.freqlatex = freqlatex_;
  V.ribllatex = ribllatex_;
  V.taillatex = taillatex_;
  startfiles(&V);
  readparameters(&V);
  if (V.hline)
    fprintf(V.labelatex->f, "\\hline\n");

  if (*V.ribl->name != '\0') {
    if (V.ribl->f != NULL)
      V.ribl->f = freopen(V.ribl->name, "r", V.ribl->f);
    else
      V.ribl->f = fopen(V.ribl->name, "r");
  } else
    rewind(V.ribl->f);
  if (V.ribl->f == NULL)
    _EscIO2(FileNotFound, V.ribl->name);
  RESETBUF(V.ribl->f, Char);
  getriblmatrix(V.ribl, &V.matrix);

  setupheaders(&V);
  /*


*/




  putc(' ', V.freqlatex->f);
  putc(' ', V.ribllatex->f);
  putc(' ', V.senulatex->f);
  FORLIM = V.askedto;

  for (l = V.askedfrom; l <= FORLIM; l++) {
    fprintf(V.freqlatex->f, " & $ ");
    dm(V.freqlatex, &V);
    fprintf(V.freqlatex->f, "%ld", l);
    de(V.freqlatex, &V);
    putc('$', V.freqlatex->f);
    fprintf(V.freqlatex->f, "\\hspace{\\fboxsep}");

    fprintf(V.ribllatex->f, " & $ ");
    dm(V.ribllatex, &V);
    fprintf(V.ribllatex->f, "%ld", l);
    de(V.ribllatex, &V);
    putc('$', V.ribllatex->f);
    /*



*/
    fprintf(V.ribllatex->f, "\\hspace{\\fboxsep}");

    fprintf(V.senulatex->f, " & $ ");
    dm(V.senulatex, &V);
    fprintf(V.senulatex->f, "%ld", l);
    de(V.senulatex, &V);
    putc('$', V.senulatex->f);

    count++;
  }

  fprintf(V.freqlatex->f, " \\\\\n");
  fprintf(V.ribllatex->f, " \\\\\n");
  fprintf(V.senulatex->f, " \\\\\n");


/* p2c: rila.p: Note: Eliminated unused assignment statement [338] */
  if (V.hline)
    fprintf(V.senulatex->f, "\\hline\n");
  if (V.hline)
    fprintf(V.freqlatex->f, "\\hline\n");
  if (V.hline)
    fprintf(V.ribllatex->f, "\\hline\n");
  for (column = 1; column <= 4; column++) {
    switch (column) {

    case 1:
      thebase = 'a';
      break;

    case 2:
      thebase = 'c';
      break;

    case 3:
      thebase = 'g';
      break;

    case 4:
      thebase = 't';
      break;
    }

    switch (column) {

    case 1:
      b = a;
      break;

    case 2:
      b = c;
      break;

    case 3:
      b = g;
      break;

    case 4:
      b = t;
      break;
    }

    if (V.caps)
      thebase = capitalize(thebase);
    else
      thebase = decapitalize(thebase);
    db(V.senulatex, &V);
    fprintf(V.senulatex->f, "%c\n", thebase);
    de(V.senulatex, &V);
    db(V.senulatex, &V);
    fprintf(V.freqlatex->f, "%c\n", thebase);
    fprintf(V.ribllatex->f, "%c\n", thebase);
    de(V.senulatex, &V);
    count = 0;

    FORLIM1 = V.askedto;
    for (l = V.askedfrom; l <= FORLIM1; l++) {
      count++;
      fprintf(V.freqlatex->f, " & ");
      fprintf(V.ribllatex->f, " & ");
      fprintf(V.senulatex->f, " & ");

      if (((count <= V.length) & (V.sequence[count-1] == decapitalize(thebase))) &&
	  V.dobox) {
	fprintf(V.freqlatex->f, "\\fbox{");
	fprintf(V.ribllatex->f, "\\fbox{");
	closebox = true;
      } else {
	fprintf(V.freqlatex->f, "\\mbox{");
	fprintf(V.ribllatex->f, "\\mbox{");
	closebox = false;
      }

      putc('$', V.freqlatex->f);
      dm(V.freqlatex, &V);
      sum = 0;
      for (bsum = a; (long)bsum <= (long)t; bsum = (base)((long)bsum + 1))
	sum += V.matrix->numbers[(long)bsum - (long)a][l - minribl];
      fprintf(V.freqlatex->f, "%*.*f",
	      wid,
	      (int)V.decimals, (double)V.matrix->numbers[(long)b - (long)a]
			       [l - minribl] / sum);
      /*
*/
      de(V.freqlatex, &V);
      putc('$', V.freqlatex->f);

      putc('$', V.ribllatex->f);
      dm(V.ribllatex, &V);
      if (V.matrix->data[(long)b - (long)a][l - minribl] <= V.negativeinfinity)
	fprintf(V.ribllatex->f, "-\\infty");
      else if (V.matrix->data[(long)b - (long)a][l - minribl] < 0)
	fprintf(V.ribllatex->f, "%*.*f",
		wid, (int)V.decimals, V.matrix->data[(long)b - (long)a]
		[l - minribl]);
      else
	fprintf(V.ribllatex->f, "+%*.*f",
		wid, (int)V.decimals, V.matrix->data[(long)b - (long)a]
		[l - minribl]);
      de(V.ribllatex, &V);
      putc('$', V.ribllatex->f);

      if (closebox)
	putc('}', V.freqlatex->f);
      else
	fprintf(V.freqlatex->f, "\\hspace{\\fboxsep}}");

      if (closebox)
	putc('}', V.ribllatex->f);
      else
	fprintf(V.ribllatex->f, "\\hspace{\\fboxsep}}");

      putc('$', V.senulatex->f);
      dm(V.senulatex, &V);
      if ((count <= V.length) & (V.sequence[count-1] == decapitalize(thebase)))
	putc('1', V.senulatex->f);
      else
	putc('0', V.senulatex->f);
      de(V.senulatex, &V);
      putc('$', V.senulatex->f);

    }
    fprintf(V.freqlatex->f, " \\\\\n");
    fprintf(V.ribllatex->f, " \\\\\n");
    fprintf(V.senulatex->f, " \\\\\n");
  }
  if (V.hline)
    fprintf(V.senulatex->f, "\\hline\n");
  if (V.hline)
    fprintf(V.freqlatex->f, "\\hline\n");
  if (V.hline)
    fprintf(V.ribllatex->f, "\\hline\n");
  if (V.hline)
    fprintf(V.labelatex->f, "\\hline\n");

  fprintf(V.taillatex->f, "\\end{tabular}\n");
}

#undef wid


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  rilap.f = NULL;
  strcpy(rilap.name, "rilap");
  taillatex.f = NULL;
  strcpy(taillatex.name, "taillatex");
  ribllatex.f = NULL;
  strcpy(ribllatex.name, "ribllatex");
  freqlatex.f = NULL;
  strcpy(freqlatex.name, "freqlatex");
  senulatex.f = NULL;
  strcpy(senulatex.name, "senulatex");
  sequlatex.f = NULL;
  strcpy(sequlatex.name, "sequlatex");
  labelatex.f = NULL;
  strcpy(labelatex.name, "labelatex");
  headlatex.f = NULL;
  strcpy(headlatex.name, "headlatex");
  ribl.f = NULL;
  strcpy(ribl.name, "ribl");
  themain(&ribl, &rilap, &headlatex, &labelatex, &sequlatex, &senulatex,
	  &freqlatex, &ribllatex, &taillatex);
_L1:
  if (ribl.f != NULL)
    fclose(ribl.f);
  if (headlatex.f != NULL)
    fclose(headlatex.f);
  if (labelatex.f != NULL)
    fclose(labelatex.f);
  if (sequlatex.f != NULL)
    fclose(sequlatex.f);
  if (senulatex.f != NULL)
    fclose(senulatex.f);
  if (freqlatex.f != NULL)
    fclose(freqlatex.f);
  if (ribllatex.f != NULL)
    fclose(ribllatex.f);
  if (taillatex.f != NULL)
    fclose(taillatex.f);
  if (rilap.f != NULL)
    fclose(rilap.f);
  exit(EXIT_SUCCESS);
}



/* End. */
