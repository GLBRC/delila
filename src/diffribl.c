/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "diffribl.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*














*/



#define version         1.24
/*





















*/
#define updateversion   1.00



/*


























































































































































































































































































*/



#define minribl         (-10001)
#define maxribl         10001
#define defnegativeinfinity  (-1000)
/*
*/




#define maxstring       2000





typedef struct box {
  double range, occur;
  /*

*/
  double probdist;
  boolean isitzero;
} box;

typedef box map[1000];




typedef enum {
  a, c, g, t
} base;



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


Static _TEXT ribla, riblb, diffriblp, posdiff, drxyin;


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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
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


#define tabcharacter    9



Static Void readquotestring(afile, s)
_TEXT *afile;
stringDelila *s;
{
  /*
*/
  Char c_;
  long i, FORLIM;

  emptystring(s);
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






Static Void cleanmatrix(matrix, frombase, tobase)
ribltype **matrix;
long frombase, tobase;
{
  long x = minribl;



  while (x < frombase) {
    (*matrix)->data[0][x - minribl] = 0.0;
    (*matrix)->data[(long)c - (long)a][x - minribl] = 0.0;
    (*matrix)->data[(long)g - (long)a][x - minribl] = 0.0;
    (*matrix)->data[(long)t - (long)a][x - minribl] = 0.0;
    x++;
  }

  while (x >= frombase && x <= tobase)
    x++;

  while (x > tobase && x <= maxribl) {
    (*matrix)->data[0][x - minribl] = 0.0;
    (*matrix)->data[(long)c - (long)a][x - minribl] = 0.0;
    (*matrix)->data[(long)g - (long)a][x - minribl] = 0.0;
    (*matrix)->data[(long)t - (long)a][x - minribl] = 0.0;
    x++;
  }

}





Static Void subribl(matrixA, matrixB, matrixC, posdiff, drxyin, space,
		    rangebool, userfrom, userto)
ribltype *matrixA, *matrixB, **matrixC;
_TEXT *posdiff, *drxyin;
long space;
boolean rangebool;
long userfrom, userto;
{
  long x;
  double sum;
  double totalsum = 0.0;
  long FORLIM;


  if (rangebool == false) {
    FORLIM = matrixA->tobase + labs(space);
    for (x = matrixA->frombase - labs(space); x <= FORLIM; x++) {
      (*matrixC)->data[0]
	[x - minribl] = matrixA->data[0][x - minribl] - matrixB->data[0]
			[x - space - minribl];
      (*matrixC)->data[(long)c - (long)a]
	[x - minribl] = matrixA->data[(long)c - (long)a]
			[x - minribl] - matrixB->data[(long)c - (long)a]
			[x - space - minribl];
      (*matrixC)->data[(long)g - (long)a]
	[x - minribl] = matrixA->data[(long)g - (long)a]
			[x - minribl] - matrixB->data[(long)g - (long)a]
			[x - space - minribl];
      (*matrixC)->data[(long)t - (long)a]
	[x - minribl] = matrixA->data[(long)t - (long)a]
			[x - minribl] - matrixB->data[(long)t - (long)a]
			[x - space - minribl];
      sum = (*matrixC)->data[0]
	    [x - minribl] + (*matrixC)->data[(long)c - (long)a]
	    [x - minribl] + (*matrixC)->data[(long)g - (long)a]
	    [x - minribl] + (*matrixC)->data[(long)t - (long)a][x - minribl];
      fprintf(posdiff->f, "%10ld", x);
      fprintf(posdiff->f, "%15.6f\n", sum);

      totalsum += sum;
    }
    fprintf(posdiff->f, "*\n");
    fprintf(posdiff->f, "* the sum of all the differences is: %15.6f bits\n",
	    totalsum);

    fprintf(drxyin->f, "%10ld%15.6f\n", space, totalsum);

    if (space == 0)
      printf("The sum of all distances is: %10.6f bits\n", totalsum);

  }

  if (!rangebool)
    return;

  FORLIM = userto + labs(space);
  for (x = userfrom - labs(space); x <= FORLIM; x++) {
    (*matrixC)->data[0]
      [x - minribl] = matrixA->data[0][x - minribl] - matrixB->data[0]
		      [x - space - minribl];
    (*matrixC)->data[(long)c - (long)a]
      [x - minribl] = matrixA->data[(long)c - (long)a]
		      [x - minribl] - matrixB->data[(long)c - (long)a]
		      [x - space - minribl];
    (*matrixC)->data[(long)g - (long)a]
      [x - minribl] = matrixA->data[(long)g - (long)a]
		      [x - minribl] - matrixB->data[(long)g - (long)a]
		      [x - space - minribl];
    (*matrixC)->data[(long)t - (long)a]
      [x - minribl] = matrixA->data[(long)t - (long)a]
		      [x - minribl] - matrixB->data[(long)t - (long)a]
		      [x - space - minribl];
    sum = (*matrixC)->data[0]
	  [x - minribl] + (*matrixC)->data[(long)c - (long)a]
	  [x - minribl] + (*matrixC)->data[(long)g - (long)a]
	  [x - minribl] + (*matrixC)->data[(long)t - (long)a][x - minribl];
    fprintf(posdiff->f, "%10ld", x);
    fprintf(posdiff->f, "%15.6f\n", sum);

    totalsum += sum;
  }
  fprintf(posdiff->f, "*\n");
  fprintf(posdiff->f, "* the sum of all the differences is: %15.6fbits\n",
	  totalsum);

  fprintf(drxyin->f, "%10ld%15.6f\n", space, totalsum);

  if (space == 0) {
    printf("The sum of all distances is: %10.6fbits\n", totalsum);

  }

}



/*

*/


Static double rifreq(ri)
double ri;
{
  /*









*/
  return exp((ri - 2.0) * log(2.0));
}



Static Void scanribl(matrixA, matrixB, matrixC, posdiff, drxyin, space,
		     rangebool, userfrom, userto)
ribltype *matrixA, *matrixB, **matrixC;
_TEXT *posdiff, *drxyin;
long space;
boolean rangebool;
long userfrom, userto;
{
  /*
*/
  base b;
  long x;
  double sum;
  double totalsum = 0.0;
  long thefrom, theto, nl, FORLIM;


  /*

*/


  if (rangebool) {
    thefrom = matrixA->frombase;
    theto = matrixA->tobase;
  } else {
    thefrom = userfrom;
    theto = userto;
  }
  FORLIM = theto + labs(space);

  /*


*/

  for (x = thefrom - labs(space); x <= FORLIM; x++) {
    nl = 0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      nl += matrixB->numbers[(long)b - (long)a][x - space - minribl];
      /*


*/
    }

    sum = 0.0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      /*


*/
      if (matrixB->numbers[(long)b - (long)a][x - space - minribl] != 0) {
	(*matrixC)->data[(long)b - (long)a]
	  [x - minribl] = matrixA->data[(long)b - (long)a]
			  [x - minribl] * matrixB->numbers[(long)b - (long)a]
			  [x - space - minribl] / nl;

	/*


*/

	sum += (*matrixC)->data[(long)b - (long)a][x - minribl];
      }

    }
    /*

*/

    fprintf(posdiff->f, "%10ld", x);
    fprintf(posdiff->f, "%15.6f\n", sum);

    totalsum += sum;
  }
  fprintf(posdiff->f, "* at a spacing of %ld, the average is: %15.6f bits\n",
	  space, totalsum);
  fprintf(posdiff->f, "*\n");

  fprintf(drxyin->f, "%10ld%15.6f\n", space, totalsum);

  if (space == 0)
    printf("At zero spacing, the average is: %10.6f bits\n", totalsum);

}




/*

*/



Static Void euribl(matrixA, matrixB, matrixC, posdiff, drxyin, space,
		   rangebool, userfrom, userto)
ribltype *matrixA, *matrixB, **matrixC;
_TEXT *posdiff, *drxyin;
long space;
boolean rangebool;
long userfrom, userto;
{
  long x;
  double sum;
  double totalsum = 0.0;
  long FORLIM;
  double TEMP;



  if (rangebool == false) {
    FORLIM = matrixA->tobase + labs(space);
    for (x = matrixA->frombase - labs(space); x <= FORLIM; x++) {
      TEMP = matrixA->data[0][x - minribl] - matrixB->data[0]
	     [x - space - minribl];
      (*matrixC)->data[0][x - minribl] = TEMP * TEMP;
      TEMP = matrixA->data[(long)c - (long)a]
	     [x - minribl] - matrixB->data[(long)c - (long)a]
	     [x - space - minribl];
      (*matrixC)->data[(long)c - (long)a][x - minribl] = TEMP * TEMP;
      TEMP = matrixA->data[(long)g - (long)a]
	     [x - minribl] - matrixB->data[(long)g - (long)a]
	     [x - space - minribl];
      (*matrixC)->data[(long)g - (long)a][x - minribl] = TEMP * TEMP;
      TEMP = matrixA->data[(long)t - (long)a]
	     [x - minribl] - matrixB->data[(long)t - (long)a]
	     [x - space - minribl];
      (*matrixC)->data[(long)t - (long)a][x - minribl] = TEMP * TEMP;
      sum = sqrt((*matrixC)->data[0]
		 [x - minribl] + (*matrixC)->data[(long)c - (long)a]
		 [x - minribl] + (*matrixC)->data[(long)g - (long)a]
		 [x - minribl] + (*matrixC)->data[(long)t - (long)a]
		 [x - minribl]);
      fprintf(posdiff->f, "%10ld", x);
      fprintf(posdiff->f, "%15.6f\n", sum);

      totalsum += sum;

    }
    fprintf(posdiff->f, "*\n");
    fprintf(posdiff->f,
	    "* the sum of all the Euclidean distances is: %15.6fbits\n",
	    totalsum);

    fprintf(drxyin->f, "%10ld%15.6f\n", space, totalsum);

    if (space == 0) {
      fprintf(posdiff->f, "*\n");
      printf("The sum of all distances is: %10.6fbits\n", totalsum);
    }

  }

  if (!rangebool)
    return;

  FORLIM = userto + labs(space);
  for (x = userfrom - labs(space); x <= FORLIM; x++) {
    TEMP = matrixA->data[0][x - minribl] - matrixB->data[0]
	   [x - space - minribl];
    (*matrixC)->data[0][x - minribl] = TEMP * TEMP;
    TEMP = matrixA->data[(long)c - (long)a]
	   [x - minribl] - matrixB->data[(long)c - (long)a]
	   [x - space - minribl];
    (*matrixC)->data[(long)c - (long)a][x - minribl] = TEMP * TEMP;
    TEMP = matrixA->data[(long)g - (long)a]
	   [x - minribl] - matrixB->data[(long)g - (long)a]
	   [x - space - minribl];
    (*matrixC)->data[(long)g - (long)a][x - minribl] = TEMP * TEMP;
    TEMP = matrixA->data[(long)t - (long)a]
	   [x - minribl] - matrixB->data[(long)t - (long)a]
	   [x - space - minribl];
    (*matrixC)->data[(long)t - (long)a][x - minribl] = TEMP * TEMP;
    sum = sqrt((*matrixC)->data[0]
	       [x - minribl] + (*matrixC)->data[(long)c - (long)a]
	       [x - minribl] + (*matrixC)->data[(long)g - (long)a]
	       [x - minribl] + (*matrixC)->data[(long)t - (long)a]
	       [x - minribl]);
    fprintf(posdiff->f, "%10ld", x);
    fprintf(posdiff->f, "%15.6f\n", sum);

    totalsum += sum;
  }
  fprintf(posdiff->f, "*\n");
  fprintf(posdiff->f,
	  "* the sum of all the Euclidean distances is: %15.6f bits\n",
	  totalsum);

  fprintf(drxyin->f, "%10ld%15.6f\n", space, totalsum);

  if (space == 0)
    printf("The sum of all distances is: %10.6f bits\n", totalsum);
}






Static Void onepointribl(matrixA, matrixB, matrixC, posdiff, drxyin, space,
			 rangebool, userfrom, userto)
ribltype *matrixA, *matrixB, **matrixC;
_TEXT *posdiff, *drxyin;
long space;
boolean rangebool;
long userfrom, userto;
{
  long x;
  double sum;
  double totalsum = 0.0;
  double onedist, ln2 = log(2.0);
  long FORLIM;
  double TEMP;



  if (rangebool == false) {
    FORLIM = matrixA->tobase + labs(space);
    for (x = matrixA->frombase - labs(space); x <= FORLIM; x++) {
      TEMP = matrixA->data[0][x - minribl] - matrixB->data[0]
	     [x - space - minribl];
      (*matrixC)->data[0][x - minribl] = TEMP * TEMP;
      TEMP = matrixA->data[(long)g - (long)a]
	     [x - minribl] - matrixB->data[(long)g - (long)a]
	     [x - space - minribl];
      (*matrixC)->data[(long)g - (long)a][x - minribl] = TEMP * TEMP;
      TEMP = matrixA->data[(long)t - (long)a]
	     [x - minribl] - matrixB->data[(long)t - (long)a]
	     [x - space - minribl];
      (*matrixC)->data[(long)t - (long)a][x - minribl] = TEMP * TEMP;
      TEMP = matrixA->data[(long)c - (long)a]
	     [x - minribl] - matrixB->data[(long)c - (long)a]
	     [x - space - minribl];
      (*matrixC)->data[(long)c - (long)a][x - minribl] = TEMP * TEMP;
      sum = (*matrixC)->data[0]
	    [x - minribl] + (*matrixC)->data[(long)c - (long)a]
	    [x - minribl] + (*matrixC)->data[(long)g - (long)a]
	    [x - minribl] + (*matrixC)->data[(long)t - (long)a][x - minribl];
      fprintf(posdiff->f, "%10ld", x);
      fprintf(posdiff->f, "%15.6f\n", sqrt(sum));

      totalsum += sum;

    }
    onedist = sqrt(totalsum);
    fprintf(posdiff->f, "*\n");
    fprintf(posdiff->f,
	    "* the sum of all the Euclidean distances is: %15.6fbits\n",
	    onedist);

    fprintf(drxyin->f, "%10ld%15.6f\n", space, onedist);

    if (space == 0)
      printf("The sum of all distances is: %10.6f bits\n", onedist);

  }

  if (!rangebool)
    return;
  FORLIM = userto + labs(space);
  for (x = userfrom - labs(space); x <= FORLIM; x++) {
    TEMP = matrixA->data[0][x - minribl] - matrixB->data[0]
	   [x - space - minribl];
    (*matrixC)->data[0][x - minribl] = TEMP * TEMP;
    TEMP = matrixA->data[(long)c - (long)a]
	   [x - minribl] - matrixB->data[(long)c - (long)a]
	   [x - space - minribl];
    (*matrixC)->data[(long)c - (long)a][x - minribl] = TEMP * TEMP;
    TEMP = matrixA->data[(long)g - (long)a]
	   [x - minribl] - matrixB->data[(long)g - (long)a]
	   [x - space - minribl];
    (*matrixC)->data[(long)g - (long)a][x - minribl] = TEMP * TEMP;
    TEMP = matrixA->data[(long)t - (long)a]
	   [x - minribl] - matrixB->data[(long)t - (long)a]
	   [x - space - minribl];
    (*matrixC)->data[(long)t - (long)a][x - minribl] = TEMP * TEMP;
    sum = (*matrixC)->data[0]
	  [x - minribl] + (*matrixC)->data[(long)c - (long)a]
	  [x - minribl] + (*matrixC)->data[(long)g - (long)a]
	  [x - minribl] + (*matrixC)->data[(long)t - (long)a][x - minribl];
    fprintf(posdiff->f, "%10ld", x);
    fprintf(posdiff->f, "%15.6f\n", sum);

    totalsum += sum;
  }
  onedist = sqrt(totalsum);
  fprintf(posdiff->f, "*\n");
  fprintf(posdiff->f,
	  "* the sum of all the Euclidean distances is: %15.6f bits\n",
	  onedist);

  fprintf(drxyin->f, "%10ld%15.6f\n", space, onedist);

  if (space == 0)
    printf("The sum of all distances is: %10.6f bits\n", onedist);
}






Static Void overlapribl(matrixA, matrixB, matrixC, posdiff, drxyin, space,
			rangebool, userfrom, userto)
ribltype *matrixA, *matrixB, **matrixC;
_TEXT *posdiff, *drxyin;
long space;
boolean rangebool;
long userfrom, userto;
{
  long x;
  double sum;
  double totalsum = 0.0;
  long newfrom, newto;
  double TEMP;



  if (space <= 0) {
    if (rangebool == false) {
      newfrom = matrixA->frombase;
      newto = matrixA->tobase + space;
    }

    if (rangebool) {
      newfrom = userfrom;
      newto = userto + space;
    }

  }

  if (space > 0) {
    if (rangebool == false) {
      newfrom = matrixA->frombase + space;
      newto = matrixA->tobase;
    }

    if (rangebool) {
      newfrom = userfrom + space;
      newto = userto;
    }

  }


  for (x = newfrom; x <= newto; x++) {
    TEMP = matrixA->data[0][x - minribl] - matrixB->data[0]
	   [x - space - minribl];
    (*matrixC)->data[0][x - minribl] = TEMP * TEMP;
    TEMP = matrixA->data[(long)c - (long)a]
	   [x - minribl] - matrixB->data[(long)c - (long)a]
	   [x - space - minribl];
    (*matrixC)->data[(long)c - (long)a][x - minribl] = TEMP * TEMP;
    TEMP = matrixA->data[(long)g - (long)a]
	   [x - minribl] - matrixB->data[(long)g - (long)a]
	   [x - space - minribl];
    (*matrixC)->data[(long)g - (long)a][x - minribl] = TEMP * TEMP;
    TEMP = matrixA->data[(long)t - (long)a]
	   [x - minribl] - matrixB->data[(long)t - (long)a]
	   [x - space - minribl];
    (*matrixC)->data[(long)t - (long)a][x - minribl] = TEMP * TEMP;
    sum = sqrt((*matrixC)->data[0]
	       [x - minribl] + (*matrixC)->data[(long)c - (long)a]
	       [x - minribl] + (*matrixC)->data[(long)g - (long)a]
	       [x - minribl] + (*matrixC)->data[(long)t - (long)a]
	       [x - minribl]);
    fprintf(posdiff->f, "%10ld", x);
    fprintf(posdiff->f, "%15.6f\n", sum);

    totalsum += sum;
  }

  fprintf(posdiff->f, "*\n");
  fprintf(posdiff->f,
	  "* the sum of all the Euclidean distances is: %15.6f bits\n",
	  totalsum);

  fprintf(drxyin->f, "%10ld%15.6f\n", space, totalsum);
}






Static Void zhang3ribl(matrixA, matrixB, matrixC, posdiff, drxyin, space,
		       rangebool, userfrom, userto, normalize)
ribltype *matrixA, *matrixB, **matrixC;
_TEXT *posdiff, *drxyin;
long space;
boolean rangebool;
long userfrom, userto;
boolean normalize;
{
  long x;
  double sum;
  double totalsum = 0.0;
  long newfrom, newto;
  double maxfactor;
  long numA, numB;
  base b;
  double piAa = 0.0, piAg = 0.0, piAc = 0.0, piAt = 0.0, piBa = 0.0,
	 piBg = 0.0, piBc = 0.0, piBt = 0.0, piCa = 0.0, piCc = 0.0,
	 piCg = 0.0, piCt = 0.0;
  double TEMP;



  if (space <= 0) {
    if (rangebool == false) {
      newfrom = matrixA->frombase;
      newto = matrixA->tobase + space;
    }

    if (rangebool) {
      newfrom = userfrom;
      newto = userto + space;
    }

  }

  if (space > 0) {
    if (rangebool == false) {
      newfrom = matrixA->frombase + space;
      newto = matrixA->tobase;
    }

    if (rangebool) {
      newfrom = userfrom + space;
      newto = userto;
    }

  }

  if (normalize)
    maxfactor = (newto - newfrom - 1) * sqrt(3.0 / 2);
  else
    maxfactor = 1.00;

  for (x = newfrom; x <= newto; x++) {
    numA = 0;
    numB = 0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      numA += matrixA->numbers[(long)b - (long)a][x - minribl];
      numB += matrixB->numbers[(long)b - (long)a][x - minribl];
    }

    piAa = (double)matrixA->numbers[0][x - minribl] / numA;
    piAc = (double)matrixA->numbers[(long)c - (long)a][x - minribl] / numA;
    piAg = (double)matrixA->numbers[(long)g - (long)a][x - minribl] / numA;
    piAt = (double)matrixA->numbers[(long)t - (long)a][x - minribl] / numA;
    piBa = (double)matrixB->numbers[0][x - minribl] / numB;
    piBc = (double)matrixB->numbers[(long)c - (long)a][x - minribl] / numB;
    piBg = (double)matrixB->numbers[(long)g - (long)a][x - minribl] / numB;
    piBt = (double)matrixB->numbers[(long)t - (long)a][x - minribl] / numB;
    TEMP = piAa - piBa;
    piCa = TEMP * TEMP;
    TEMP = piAc - piBc;
    piCc = TEMP * TEMP;
    TEMP = piAg - piBg;
    piCg = TEMP * TEMP;
    TEMP = piAt - piBt;
    piCt = TEMP * TEMP;

    sum = sqrt(3 * (piCa + piCc + piCg + piCt)) / 2;
    fprintf(posdiff->f, "%10ld", x);
    if (normalize)
      fprintf(posdiff->f, "%15.6f\n", sum / maxfactor);
    else
      fprintf(posdiff->f, "%15.6f\n", sum);

    totalsum += sum;
  }

  fprintf(posdiff->f, "*\n");

  if (normalize)
    fprintf(posdiff->f,
      "* NORMALIZED the sum of all the Euclidean distances is: %15.6f\n",
      totalsum / maxfactor);
  else
    fprintf(posdiff->f,
	    "* the sum of all the Euclidean distances is: %15.6f\n",
	    totalsum);

  if (normalize)
    fprintf(drxyin->f, "%10ld%15.6f\n", space, totalsum / maxfactor);
  else {
    fprintf(drxyin->f, "%10ld%15.6f\n", space, totalsum);

  }
}



/*








































































*/




Static Void dlapribl(matrixA, matrixB, matrixC, posdiff, drxyin, space,
		     rangebool, userfrom, userto)
ribltype *matrixA, *matrixB, **matrixC;
_TEXT *posdiff, *drxyin;
long space;
boolean rangebool;
long userfrom, userto;
{
  long x;
  double sum;
  double totalsum = 0.0;
  long newfrom, newto;



  if (space <= 0) {
    if (rangebool == false) {
      newfrom = matrixA->frombase;
      newto = matrixA->tobase + space;
    }

    if (rangebool) {
      newfrom = userfrom;
      newto = userto + space;
    }

  }

  if (space > 0) {
    if (rangebool == false) {
      newfrom = matrixA->frombase + space;
      newto = matrixA->tobase;
    }

    if (rangebool) {
      newfrom = userfrom + space;
      newto = userto;
    }

  }


  for (x = newfrom; x <= newto; x++) {
    (*matrixC)->data[0]
      [x - minribl] = matrixA->data[0][x - minribl] - matrixB->data[0]
		      [x - space - minribl];
    (*matrixC)->data[(long)c - (long)a]
      [x - minribl] = matrixA->data[(long)c - (long)a]
		      [x - minribl] - matrixB->data[(long)c - (long)a]
		      [x - space - minribl];
    (*matrixC)->data[(long)g - (long)a]
      [x - minribl] = matrixA->data[(long)g - (long)a]
		      [x - minribl] - matrixB->data[(long)g - (long)a]
		      [x - space - minribl];
    (*matrixC)->data[(long)t - (long)a]
      [x - minribl] = matrixA->data[(long)t - (long)a]
		      [x - minribl] - matrixB->data[(long)t - (long)a]
		      [x - space - minribl];
    sum = (*matrixC)->data[0]
	  [x - minribl] + (*matrixC)->data[(long)c - (long)a]
	  [x - minribl] + (*matrixC)->data[(long)g - (long)a]
	  [x - minribl] + (*matrixC)->data[(long)t - (long)a][x - minribl];
    fprintf(posdiff->f, "%10ld", x);
    fprintf(posdiff->f, "%15.6f\n", sum);

    totalsum += sum;
  }
  fprintf(posdiff->f, "*\n");
  fprintf(posdiff->f, "* the sum of all the Euclidean distances is: %15.6f\n",
	  totalsum);

  fprintf(drxyin->f, "%10ld%15.6f\n", space, totalsum);
}





Static Void readdrparam(diffriblp, parameterversion, rangechar, spacechar,
			userfrom, userto, spacefrom, spaceto, rangebool,
			spacebool, calctype)
_TEXT *diffriblp;
double *parameterversion;
Char *rangechar, *spacechar;
long *userfrom, *userto, *spacefrom, *spaceto;
boolean *rangebool, *spacebool;
Char *calctype;
{
  if (*diffriblp->name != '\0') {
    if (diffriblp->f != NULL)
      diffriblp->f = freopen(diffriblp->name, "r", diffriblp->f);
    else
      diffriblp->f = fopen(diffriblp->name, "r");
  } else
    rewind(diffriblp->f);
  if (diffriblp->f == NULL)
    _EscIO2(FileNotFound, diffriblp->name);
  RESETBUF(diffriblp->f, Char);
  fscanf(diffriblp->f, "%lg%*[^\n]", parameterversion);
  getc(diffriblp->f);
  if (*parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  *rangechar = getc(diffriblp->f);
  if (*rangechar == '\n')
    *rangechar = ' ';
  *rangebool = false;
  *spacebool = false;

  if (*rangechar == 'r') {
    fscanf(diffriblp->f, "%*[^\n]");
    getc(diffriblp->f);
  }

  if (*rangechar == 'u') {
    *rangebool = true;
    fscanf(diffriblp->f, "%ld", userfrom);
    fscanf(diffriblp->f, "%ld%*[^\n]", userto);
    getc(diffriblp->f);
  }

  if (*rangechar != 'r' && *rangechar != 'u') {
    printf("the range needs to specified with an \"r\" or \"u\"\n");
    halt();
  }

  *spacechar = getc(diffriblp->f);

  if (*spacechar == '\n')
    *spacechar = ' ';
  if (*spacechar == 'n') {
    fscanf(diffriblp->f, "%*[^\n]");
    getc(diffriblp->f);
  }

  if (*spacechar == 'v') {
    *spacebool = true;
    fscanf(diffriblp->f, "%ld", spacefrom);
    fscanf(diffriblp->f, "%ld%*[^\n]", spaceto);
    getc(diffriblp->f);
  }

  if (*spacechar != 'n' && *spacechar != 'v') {
    printf("the variability range needs to specified with an \"n\" or \"v\"\n");
    halt();
  }

  *calctype = getc(diffriblp->f);

  if (*calctype == '\n')
    *calctype = ' ';
  fscanf(diffriblp->f, "%*[^\n]");

  getc(diffriblp->f);
  if (*calctype != 'y' && *calctype != 'z' && *calctype != 's' &&
      *calctype != 'd' && *calctype != 'o' && *calctype != 'e') {
    printf("calculation type must be specified by one of \"eodszy\"\n");
    halt();
  }

}



Static Void writedrparam(f, parameterversion, rangechar, spacechar, userfrom,
			 userto, spacefrom, spaceto, rangebool, spacebool,
			 calctype)
_TEXT *f;
double parameterversion;
Char rangechar, spacechar;
long userfrom, userto, spacefrom, spaceto;
boolean rangebool, spacebool;
Char calctype;
{
  fprintf(f->f, "* %c ", calctype);

  switch (calctype) {

  case 'e':
    fprintf(f->f, "Euclidean distance\n");
    break;

  case 'o':
    fprintf(f->f, "Euclidean distance, 4 dimensional\n");
    break;

  case 'd':
    fprintf(f->f, "Difference distance\n");
    break;

  case 's':
    fprintf(f->f, "scan average\n");
    break;

  case 'z':
    fprintf(f->f, "Zhang, position-by-position\n");
    break;

  case 'Z':
    fprintf(f->f, "Zhang 4 dimensional\n");
    break;

  case 'y':
    fprintf(f->f, "Zhang, position-by-position, normalized by sqrt(3/2)L\n");
    break;
  }

}


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




Static Void themain(ribla, riblb, diffriblp, posdiff, drxyin)
_TEXT *ribla, *riblb, *diffriblp, *posdiff, *drxyin;
{
  double parameterversion;
  Char rangechar, spacechar;
  long userfrom, userto, spacefrom, spaceto;

  boolean rangebool, spacebool;

  Char calctype;

  ribltype *ribltypeptrA, *ribltypeptrB, *ribltypeptrC;

  long space;
  _TEXT TEMP;

  printf("diffribl%4.2f\n", version);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  PatentMessage(&TEMP);


  readdrparam(diffriblp, &parameterversion, &rangechar, &spacechar, &userfrom,
	      &userto, &spacefrom, &spaceto, &rangebool, &spacebool,
	      &calctype);

  if (*ribla->name != '\0') {
    if (ribla->f != NULL)
      ribla->f = freopen(ribla->name, "r", ribla->f);
    else
      ribla->f = fopen(ribla->name, "r");
  } else
    rewind(ribla->f);
  if (ribla->f == NULL)
    _EscIO2(FileNotFound, ribla->name);
  RESETBUF(ribla->f, Char);
  if (*riblb->name != '\0') {
    if (riblb->f != NULL)
      riblb->f = freopen(riblb->name, "r", riblb->f);
    else
      riblb->f = fopen(riblb->name, "r");
  } else
    rewind(riblb->f);
  if (riblb->f == NULL)
    _EscIO2(FileNotFound, riblb->name);
  RESETBUF(riblb->f, Char);


  getriblmatrix(ribla, &ribltypeptrA);
  getriblmatrix(riblb, &ribltypeptrB);


  if (*ribla->name != '\0') {
    if (ribla->f != NULL)
      ribla->f = freopen(ribla->name, "r", ribla->f);
    else
      ribla->f = fopen(ribla->name, "r");
  } else
    rewind(ribla->f);
  if (ribla->f == NULL)
    _EscIO2(FileNotFound, ribla->name);
  RESETBUF(ribla->f, Char);
  getriblmatrix(ribla, &ribltypeptrC);

  if (rangechar == 'r') {
    userfrom = ribltypeptrA->frombase;
    userto = ribltypeptrA->tobase;
    printf("range is taken from the A matrix as: %ld %ld\n", userfrom, userto);
  }



  if (rangebool) {
    if (userfrom < ribltypeptrA->frombase || userto > ribltypeptrA->tobase) {
      printf("userfrom userto range must be smaller than ribl size\n");
      halt();
    }
  }


  if (rangebool == false) {
    cleanmatrix(&ribltypeptrA, ribltypeptrA->frombase, ribltypeptrA->tobase);
    cleanmatrix(&ribltypeptrB, ribltypeptrB->frombase, ribltypeptrB->tobase);
  }

  if (rangebool) {
    cleanmatrix(&ribltypeptrA, userfrom, userto);
    cleanmatrix(&ribltypeptrB, userfrom, userto);
  }

  if (*drxyin->name != '\0') {
    if (drxyin->f != NULL)
      drxyin->f = freopen(drxyin->name, "w", drxyin->f);
    else
      drxyin->f = fopen(drxyin->name, "w");
  } else {
    if (drxyin->f != NULL)
      rewind(drxyin->f);
    else
      drxyin->f = tmpfile();
  }
  if (drxyin->f == NULL)
    _EscIO2(FileNotFound, drxyin->name);
  SETUPBUF(drxyin->f, Char);

  if (*posdiff->name != '\0') {
    if (posdiff->f != NULL)
      posdiff->f = freopen(posdiff->name, "w", posdiff->f);
    else
      posdiff->f = fopen(posdiff->name, "w");
  } else {
    if (posdiff->f != NULL)
      rewind(posdiff->f);
    else
      posdiff->f = tmpfile();
  }
  if (posdiff->f == NULL)
    _EscIO2(FileNotFound, posdiff->name);
  SETUPBUF(posdiff->f, Char);
  fprintf(posdiff->f, "* diffribl %4.2f\n", version);

  writedrparam(posdiff, parameterversion, rangechar, spacechar, userfrom,
	       userto, spacefrom, spaceto, rangebool, spacebool, calctype);

  fprintf(posdiff->f, "* Matrix A: ");
  writestring(posdiff, &ribltypeptrA->riblname);
  fprintf(posdiff->f, "\n* Matrix B: ");

  writestring(posdiff, &ribltypeptrB->riblname);
  fprintf(posdiff->f, "\n*\n");

  fprintf(posdiff->f, "* column 1: position\n");
  fprintf(posdiff->f,
    "* column 2: square root of the sum of squared differences or distances\n");
  fprintf(posdiff->f, "*\n");



  if (spacebool == false) {
    spacefrom = 0;
    spaceto = 0;
  }

  for (space = spacefrom; space <= spaceto; space++) {
    switch (calctype) {

    case 'e':
      overlapribl(ribltypeptrA, ribltypeptrB, &ribltypeptrC, posdiff, drxyin,
		  space, rangebool, userfrom, userto);
      break;

    case 'o':
      onepointribl(ribltypeptrA, ribltypeptrB, &ribltypeptrC, posdiff, drxyin,
		   space, rangebool, userfrom, userto);
      break;

    case 'd':
      dlapribl(ribltypeptrA, ribltypeptrB, &ribltypeptrC, posdiff, drxyin,
	       space, rangebool, userfrom, userto);
      break;

    case 's':
      scanribl(ribltypeptrA, ribltypeptrB, &ribltypeptrC, posdiff, drxyin,
	       space, rangebool, userfrom, userto);
      break;

    case 'z':
      zhang3ribl(ribltypeptrA, ribltypeptrB, &ribltypeptrC, posdiff, drxyin,
		 space, rangebool, userfrom, userto, false);
      break;

    case 'y':
      zhang3ribl(ribltypeptrA, ribltypeptrB, &ribltypeptrC, posdiff, drxyin,
		 space, rangebool, userfrom, userto, true);
      break;

      /*



















*/

    }
  }


}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  drxyin.f = NULL;
  strcpy(drxyin.name, "drxyin");
  posdiff.f = NULL;
  strcpy(posdiff.name, "posdiff");
  diffriblp.f = NULL;
  strcpy(diffriblp.name, "diffriblp");
  riblb.f = NULL;
  strcpy(riblb.name, "riblb");
  ribla.f = NULL;
  strcpy(ribla.name, "ribla");
  themain(&ribla, &riblb, &diffriblp, &posdiff, &drxyin);
_L1:
  if (ribla.f != NULL)
    fclose(ribla.f);
  if (riblb.f != NULL)
    fclose(riblb.f);
  if (diffriblp.f != NULL)
    fclose(diffriblp.f);
  if (posdiff.f != NULL)
    fclose(posdiff.f);
  if (drxyin.f != NULL)
    fclose(drxyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
