/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "makelogo.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         9.66
/*









































































*/



/*




































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































*/




#define infofield       8
#define infodecim       5

#define nfield          4
#define mapmax          26

#define pwid            8
#define pdec            5
#define pdecolor        4
/*
*/

#define pnum            100000L

#define protecting      true
/*
*/

#define gooddisplay     false


/*


*/


#define llx             10.08
#define lly             8.91
#define urx             588.06
#define ury             779.85

/*





*/

#define edgemargindefault  2

#define centertrigger   (-1000)
/*
*/

#define outlinethinness  32
/*
*/

#define subticsBigdefault  2
#define subticsSmalldefault  10

/*
*/
#define minimumStackSize  13



#define squashspraydefault  1.00
/*
*/





#define maxstring       150






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





typedef char position;

typedef struct place {
  Char b;
  long n;
} place;


typedef struct strings {
  stringDelila astring;
  double x, y, s;
  struct strings *next;
} strings;

typedef struct parameters {
  double Ibeamfraction;
  /*

*/
  long bar;
  double barbits;
  /*
*/
  Char barends;
  /*
*/
  double barheight, barwidth;
  boolean caps;
  double charwidth;
  Char edgecontrol;
  double edgeleft, edgeright, edgelow, edgehigh;
  long highest;
  double linemove;
  long linesperpage, lowest;
  boolean numbering, outline;
  double rotation;
  Char showingbox, ShowEnds;
  double shrink;
  boolean shrinking;
  double squashspray;
  long stacksperline, subticsBig, subticsSmall;
  strings *thestrings;
  Char ticcommand;
  double xcorner, ycorner;

  boolean HalfWhiteIbeam;
  /*
*/
  waveparam *wp;


  double cmperbit;
  long symbols;

  Char formcontrol;
  /*





*/
  double stackheight, symbolheight;
  boolean showletter;
} parameters;




Static _TEXT symvec, makelogop, colors, marks, wave, logo;


Static place map[mapmax];


Static jmp_buf _JL1;




Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Char capitalize(c)
Char c;
{
  long n = c;

  if (n >= 'a' && n <= 'z')
    c = _toupper(n);
  return c;
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




Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
  clearstring(ribbon);
  ribbon->next = NULL;
}




Static Void getstring(afile, buffer, gotten)
_TEXT *afile;
stringDelila *buffer;
boolean *gotten;
{
  /*
*/
  long index = 0;

  clearstring(buffer);
  if (BUFEOF(afile->f)) {
    *gotten = false;
    return;
  }
  while (!P_eoln(afile->f) && index < maxstring) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
  }
  if (!P_eoln(afile->f)) {
    printf(" getstring: a line exceeds maximum string size (%ld)\n",
	   (long)maxstring);
    halt();
  }
  buffer->length = index;
  buffer->current = 1;
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  *gotten = true;
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




Static Void copystring(a, b)
stringDelila a, *b;
{
  long l;

  b->length = a.length;
  for (l = 0; l < a.length; l++)
    b->letters[l] = a.letters[l];
}


Static boolean lessthan(a, b)
position a, b;
{
  /*
*/
  return (labs(map[a-1].n) < labs(map[b-1].n));
}


Static Void swap_(a, b)
position a, b;
{
  Char holdb;
  long holdn;

  holdn = map[a-1].n;
  map[a-1].n = map[b-1].n;
  map[b-1].n = holdn;

  holdb = map[a-1].b;
  map[a-1].b = map[b-1].b;
  map[b-1].b = holdb;


}



Static Void quicksort(left, right)
position left, right;
{
  /*












*/
  position lower = left;
  position upper, center;

  center = (left + right) / 2;
  upper = right;

  do {
    while (lessthan(lower, center))
      lower++;
    while (lessthan(center, upper))
      upper--;

    if (lower <= upper) {
      if (lower == center)
	center = upper;
      else if (upper == center)
	center = lower;
      swap_(lower, upper);
      lower++;
      upper--;
    }

  } while (lower <= upper);

  if (left < upper)
    quicksort(left, upper);
  if (lower < right)
    quicksort(lower, right);
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


/* Local variables for numberdigit: */
struct LOC_numberdigit {
  long number, place_, myabsolute;
  Char acharacter;
} ;

Local Void digit(LINK)
struct LOC_numberdigit *LINK;
{
  long tenplace, z, d;

  tenplace = LINK->place_ * 10;
  z = LINK->myabsolute - LINK->myabsolute / tenplace * tenplace;
  if (LINK->place_ == 1)
    d = z;
  else
    d = z / LINK->place_;
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
  V.place_ = 1;
  for (count = 1; count <= logplace; count++)
    V.place_ *= 10;

  if (V.number == 0) {
    if (V.place_ == 1)
      V.acharacter = '0';
    else
      V.acharacter = ' ';
    return V.acharacter;
  }
  V.myabsolute = labs(V.number);
  if (V.myabsolute < V.place_ / 10) {
    V.acharacter = ' ';
    return V.acharacter;
  }
  if (V.myabsolute >= V.place_)
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
  long size;

  if (n == 0)
    return 1;
  else {
    size = (long)(log((double)labs(n)) / ln10 + epsilon) + 1;

    /*


*/
    if (n < 0)
      size++;
    return size;
  }
}

#undef ln10
#undef epsilon





Static Void protectcharacter(c, protectioncharacter, needed)
Char c, *protectioncharacter;
boolean *needed;
{
  /*





*/
  if (c == '\\' || c == '%' || c == ')' || c == '(') {
    *protectioncharacter = '\\';
    *needed = true;
  } else {
    *protectioncharacter = ' ';
    *needed = false;
  }
}



Static Void protectpostscript(afile, c)
_TEXT *afile;
Char c;
{
  /*




*/
  boolean needed;
  Char protectionchar;

  protectcharacter(c, &protectionchar, &needed);
  if (needed)
    putc(protectionchar, afile->f);
}



Static Void processsymvec(symvec)
_TEXT *symvec;
{
  boolean done = false;

  while (!done) {
    if (BUFEOF(symvec->f)) {
      printf("symvec has a blank line at the end, this is not allowed\n");
      halt();
      continue;
    }
    if (P_peek(symvec->f) == '*') {
      fscanf(symvec->f, "%*[^\n]");
      getc(symvec->f);
      continue;
    }
    if (P_peek(symvec->f) == ' ')
      getc(symvec->f);
    else if (P_eoln(symvec->f)) {
      fscanf(symvec->f, "%*[^\n]");
      getc(symvec->f);
    } else
      done = true;
  }
}


#define escape          '\\'


/*

*/

#define debug           false


/* Local variables for postscriptstring: */
struct LOC_postscriptstring {
  stringDelila *instring, *outstring, *rawstring;
  _TEXT *symvec;
  long lowest, highest;
  double rs, sd;
  long n;
  boolean havers, havesd;
  Char curr;
  long decimals, i;
  boolean italic, insidestring;
  Char prev;
} ;

Local Void getchar_(LINK)
struct LOC_postscriptstring *LINK;
{
  LINK->i++;
  LINK->prev = LINK->curr;
  LINK->curr = LINK->instring->letters[LINK->i-1];
}

Local Void putchar_(c, LINK)
Char c;
struct LOC_postscriptstring *LINK;
{
  LINK->outstring->length++;
  if (LINK->outstring->length > maxstring) {
    printf("postscriptstring: label too long, increase maxstring\n");
    halt();
  }
  LINK->outstring->letters[LINK->outstring->length - 1] = c;

  /*
*/
  if (LINK->insidestring) {
    LINK->rawstring->length++;
    LINK->rawstring->letters[LINK->rawstring->length - 1] = c;
    /*



*/
  }
}

Local Void badstring(LINK)
struct LOC_postscriptstring *LINK;
{
  _TEXT TEMP;

  printf("This string:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, LINK->instring);
  printf("\nis bad because parenthesis must be paired like this:\n");
  printf("\"\\)\"  stuff \"\\(\"\n");
  printf("I am emptying the logo file.\n");
  if (*logo.name != '\0') {
    if (logo.f != NULL)
      logo.f = freopen(logo.name, "w", logo.f);
    else
      logo.f = fopen(logo.name, "w");
  } else {
    if (logo.f != NULL)
      rewind(logo.f);
    else
      logo.f = tmpfile();
  }
  if (logo.f == NULL)
    _EscIO2(FileNotFound, logo.name);
  SETUPBUF(logo.f, Char);
  halt();
}

Local Void doitalics(LINK)
struct LOC_postscriptstring *LINK;
{
  /*


*/
  LINK->insidestring = false;
  if (LINK->italic) {
    putchar_(')', LINK);
    putchar_(' ', LINK);
    putchar_('I', LINK);
    putchar_('T', LINK);
    putchar_(' ', LINK);
    putchar_('(', LINK);
  } else {
    putchar_(')', LINK);
    putchar_(' ', LINK);
    putchar_('(', LINK);
  }
  LINK->insidestring = true;
  LINK->italic = !LINK->italic;
}

Local Void dosymbol(LINK)
struct LOC_postscriptstring *LINK;
{
  /*



*/
  LINK->insidestring = false;
  putchar_(')', LINK);


  LINK->insidestring = true;
  putchar_(' ', LINK);
  LINK->insidestring = false;

  putchar_('(', LINK);
  putchar_('\\', LINK);
  putchar_(LINK->curr, LINK);

  getchar_(LINK);
  putchar_(LINK->curr, LINK);
  getchar_(LINK);
  putchar_(LINK->curr, LINK);
  putchar_(')', LINK);
  putchar_(' ', LINK);
  putchar_('S', LINK);
  putchar_('Y', LINK);
  putchar_(' ', LINK);
  putchar_('(', LINK);
  LINK->insidestring = true;
}

Local Void getnum(num, LINK)
long *num;
struct LOC_postscriptstring *LINK;
{
  boolean done = false, numberstarted = false;
  long sign = 1;
  long firsti;
  _TEXT TEMP;

  /*



*/
  firsti = LINK->i;
  *num = 0;
  while (!done) {
    LINK->i++;
    if (LINK->i > LINK->instring->length) {
      done = true;
      break;
    }
    LINK->curr = LINK->instring->letters[LINK->i-1];
    /*

*/
    if (LINK->curr == '-') {
      if (sign == -1) {
	printf("strings can have only one \"-\" in \\n numbers\n");
	halt();
      }
      sign = -1;
      numberstarted = true;
      continue;
    }
    if (LINK->curr == '+') {
      if (sign == -1) {
	printf("you cannot have both + and - signs in \\n numbers\n");
	halt();
      }
      continue;
    }
    if (LINK->curr == '9' || LINK->curr == '8' || LINK->curr == '7' ||
	LINK->curr == '6' || LINK->curr == '5' || LINK->curr == '4' ||
	LINK->curr == '3' || LINK->curr == '2' || LINK->curr == '1' ||
	LINK->curr == '0') {
      *num = *num * 10 + LINK->curr - '0';
      /*

*/
      numberstarted = true;
    } else if (LINK->curr != ' ' || numberstarted) {

      done = true;
    }
  }
  *num *= sign;
  /*

*/


  LINK->i--;
  LINK->prev = ' ';
  LINK->curr = LINK->instring->letters[LINK->i-1];

  if (numberstarted)
    return;

  printf("WARNING: a number was not found in this string:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, LINK->instring);
  printf("\nzero is being used\n");
  *num = 0;
  LINK->i = firsti;
}

Local Void don(LINK)
struct LOC_postscriptstring *LINK;
{
  Char c;
  long coo = -LONG_MAX;
  long num, maxcoo = -LONG_MAX, maxnum = -LONG_MAX;
  double sd, rs;
  long desiredcoo, symbols, s;

  getnum(&desiredcoo, LINK);


  if (*LINK->symvec->name != '\0') {
    if (LINK->symvec->f != NULL)
      LINK->symvec->f = freopen(LINK->symvec->name, "r", LINK->symvec->f);
    else
      LINK->symvec->f = fopen(LINK->symvec->name, "r");
  } else {


    rewind(LINK->symvec->f);
  }
  if (LINK->symvec->f == NULL)
    _EscIO2(FileNotFound, LINK->symvec->name);
  RESETBUF(LINK->symvec->f, Char);
  if (!BUFEOF(LINK->symvec->f)) {
    processsymvec(LINK->symvec);
    fscanf(LINK->symvec->f, "%ld%*[^\n]", &symbols);
    getc(LINK->symvec->f);
  }

  while (!BUFEOF(LINK->symvec->f) && coo != desiredcoo) {
    processsymvec(LINK->symvec);
    fscanf(LINK->symvec->f, "%ld%ld%lg%lg%*[^\n]", &coo, &num, &rs, &sd);
    getc(LINK->symvec->f);
    if (num >= maxnum) {
      maxnum = num;
      maxcoo = coo;
    }
    for (s = 1; s <= symbols; s++) {
      fscanf(LINK->symvec->f, "%*[^\n]");
      getc(LINK->symvec->f);
    }
  }

  if (BUFEOF(LINK->symvec->f)) {
    if (*LINK->symvec->name != '\0') {
      if (LINK->symvec->f != NULL)
	LINK->symvec->f = freopen(LINK->symvec->name, "r", LINK->symvec->f);
      else
	LINK->symvec->f = fopen(LINK->symvec->name, "r");
    } else
      rewind(LINK->symvec->f);
    if (LINK->symvec->f == NULL)
      _EscIO2(FileNotFound, LINK->symvec->name);
    RESETBUF(LINK->symvec->f, Char);
    if (BUFEOF(LINK->symvec->f)) {
      coo = 0;
      num = 0;
      printf("* The symvec is empty.\n");
      printf("  Using %ld for the coordinate and %ld for the number.\n",
	     coo, num);
    } else {
      printf("* Coordinate %ld is not in the symvec:\n", desiredcoo);
      printf("  the maximum value (%ld) at coordinate %ld will be used instead.\n",
	     maxnum, maxcoo);
      coo = maxcoo;
      num = maxnum;
    }
  }
  /*

*/


  for (s = numbersize(num); s >= 0; s--) {
    c = numberdigit(num, s);
    if (c != ' ' && c != '+')
      putchar_(c, LINK);
  }

  LINK->n = num;
  printf("* %ld sequences found at coordinate %ld\n", LINK->n, coo);
}

Local Void getrssd(LINK)
struct LOC_postscriptstring *LINK;
{
  long b, nl, position_;
  double rsl, rsvar;
  double rstotal = 0.0;
  long symbols;
  double varhnb = 0.0;

  if (*LINK->symvec->name != '\0') {
    if (LINK->symvec->f != NULL)
      LINK->symvec->f = freopen(LINK->symvec->name, "r", LINK->symvec->f);
    else
      LINK->symvec->f = fopen(LINK->symvec->name, "r");
  } else
    rewind(LINK->symvec->f);
  if (LINK->symvec->f == NULL)
    _EscIO2(FileNotFound, LINK->symvec->name);
  RESETBUF(LINK->symvec->f, Char);
  if (BUFEOF(LINK->symvec->f)) {
    printf("symvec is empty: can't do \\r\n");
    halt();
  }

  processsymvec(LINK->symvec);
  fscanf(LINK->symvec->f, "%ld%*[^\n]", &symbols);
  getc(LINK->symvec->f);

  position_ = LINK->lowest;
  while (!BUFEOF(LINK->symvec->f) && position_ <= LINK->highest) {
    processsymvec(LINK->symvec);
    fscanf(LINK->symvec->f, "%ld%ld%lg%lg%*[^\n]", &position_, &nl, &rsl,
	   &rsvar);
    getc(LINK->symvec->f);

    if (position_ >= LINK->lowest && position_ <= LINK->highest) {
      rstotal += rsl;
      varhnb += rsvar;
    }


    for (b = 1; b <= symbols; b++) {
      processsymvec(LINK->symvec);
      fscanf(LINK->symvec->f, "%*[^\n]");
      getc(LINK->symvec->f);
    }
  }

  LINK->rs = rstotal;
  if (varhnb >= 0)
    LINK->sd = sqrt(varhnb);
  else
    LINK->sd = 0.0;

  LINK->havers = true;
  LINK->havesd = true;

}

Local Void putreal(r, dec, LINK)
double r;
long dec;
struct LOC_postscriptstring *LINK;
{
  /*




*/
  Char c;
  long m, num, s;

  printf("******* putreal r = ,%5.3f d = %ld\n", r, dec);
  if (r < 0) {
    c = '-';
    putchar_(c, LINK);
    r = fabs(r);
  }
  if (dec > 0) {
    /*








*/
    if (debug)
      printf("BB r initial   = %10.5f\n", r);


    for (m = 1; m <= dec; m++)
      r *= 10;

    r = (long)floor(r + 0.5);
    if (debug)
      printf("BB r rounded   = %10.5f\n", r);

    for (m = 1; m <= dec; m++)
      r /= 10;
    if (debug)
      printf("BB r divided   = %10.5f\n", r);

    num = (long)r;
    if (debug)
      printf("BB r final     = %10.5f\n", r);
    if (debug)
      printf("BB num = %ld\n", num);

  }

  else
    num = (long)floor(r + 0.5);


  for (s = numbersize(num); s >= 0; s--) {
    c = numberdigit(num, s);
    if (c != ' ' && c != '+')
      putchar_(c, LINK);
  }


  if (dec <= 0)
    return;

  c = '.';
  putchar_(c, LINK);

  r -= num;
  if (debug)
    printf("BL r rm num    = %10.5f\n", r);


  for (m = 1; m <= dec; m++)
    r = 10 * r;
  if (debug)
    printf("BL r above     = %10.5f\n", r);
  num = (long)floor(r + 0.5);
  if (debug)
    printf("BL num         = %10ld\n", num);
  /*


*/
  if (debug)
    printf("BL numbersize(num) = %10ld\n", numbersize(num));
  if (debug)
    printf("BL dec             = %10ld\n", dec);
  for (s = dec; s >= 0; s--) {
    c = numberdigit(num, s);
    /*


*/
    if (c != ' ' || dec != 1) {
      /*
*/
      if (c == ' ')
	c = '0';
      if (c != '+')
	putchar_(c, LINK);
    }
    /*


*/
  }
}

#undef debug

Local Void dors(LINK)
struct LOC_postscriptstring *LINK;
{
  if (!LINK->havers)
    getrssd(LINK);
  putreal(LINK->rs, LINK->decimals, LINK);
  printf("* Rsequence for logo:%*.*f\n",
	 infofield, (int)LINK->decimals, LINK->rs);
}

Local Void dosd(LINK)
struct LOC_postscriptstring *LINK;
{
  if (!LINK->havesd)
    getrssd(LINK);
  putreal(LINK->sd, LINK->decimals, LINK);
  printf("* SD of Rsequence for logo:%*.*f\n",
	 infofield, (int)LINK->decimals, LINK->sd);
}

Local Void dodecimal(LINK)
struct LOC_postscriptstring *LINK;
{
  getnum(&LINK->decimals, LINK);
  printf("* decimal places set to: %ld\n", LINK->decimals);
}

Local Void badescapewarning(f, fc, LINK)
_TEXT *f;
Char fc;
struct LOC_postscriptstring *LINK;
{
  /*
*/
  fprintf(f->f, "%c************************************************\n", fc);
  fprintf(f->f, "%c** UNKNOWN ESCAPE CHARACTER IN TITLE STRING  ***\n", fc);
  fprintf(f->f, "%c** The character \"%c\" is not allowed          ***\n",
	  fc, LINK->curr);
  fprintf(f->f, "%c** following an escape backslash (\"\\\")       ***\n", fc);
  fprintf(f->f, "%c** in the title:                             ***\n", fc);
  fprintf(f->f, "%c** \"", fc);
  writestring(f, LINK->instring);
  fprintf(f->f, "\"\n");
  if (LINK->italic)
    fprintf(f->f, "%c** Italics string was not finished with \"\\i\" ***\n",
	    fc);
  fprintf(f->f, "%c************************************************\n", fc);
}



Static Void postscriptstring(instring_, outstring_, rawstring_, symvec_,
			     lowest_, highest_, rs_, sd_, n_, havers_,
			     havesd_)
stringDelila *instring_, *outstring_, *rawstring_;
_TEXT *symvec_;
long lowest_, highest_;
double rs_, sd_;
long n_;
boolean havers_, havesd_;
{
  /*






























*/
  struct LOC_postscriptstring V;
  boolean needed;
  Char protectionchar;
  _TEXT TEMP;


  /*












*/

  V.instring = instring_;
  V.outstring = outstring_;
  V.rawstring = rawstring_;
  V.symvec = symvec_;
  V.lowest = lowest_;
  V.highest = highest_;
  V.rs = rs_;
  V.sd = sd_;
  V.n = n_;
  V.havers = havers_;
  V.havesd = havesd_;
  if (!protecting) {
    copystring(*V.instring, V.outstring);
    copystring(*V.instring, V.rawstring);
    return;
  }
  clearstring(V.outstring);
  clearstring(V.rawstring);
  V.curr = ' ';
  V.decimals = infodecim;
  V.italic = false;
  V.insidestring = false;
  putchar_('(', &V);
  V.insidestring = true;

  V.i = 0;
  while (V.i <= V.instring->length) {
    getchar_(&V);


    if (V.prev == escape && V.curr == ')') {
      if (!V.insidestring)
	badstring(&V);
      V.insidestring = false;
    }


    if (V.curr == escape) {
      if (V.prev == escape) {
	putchar_(V.prev, &V);
	putchar_(V.curr, &V);
      }
    } else {
      if (V.prev != escape) {
	protectcharacter(V.curr, &protectionchar, &needed);
	if (needed)
	  putchar_(protectionchar, &V);
      }

      if (V.prev == escape && V.curr == 'i')
	doitalics(&V);

      else if (V.prev == escape && V.curr == 'n') {

	don(&V);
      } else if (V.prev == escape &&
		 (V.curr == '7' || V.curr == '6' || V.curr == '5' ||
		  V.curr == '4' || V.curr == '3' || V.curr == '2' ||
		  V.curr == '1' || V.curr == '0')) {

	dosymbol(&V);
      } else if (V.prev == escape && V.curr == 'r') {

	dors(&V);
      } else if (V.prev == escape && V.curr == 's') {

	dosd(&V);
      } else if (V.prev == escape && V.curr == 'd') {

	dodecimal(&V);
      } else if (V.prev == escape) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	badescapewarning(&TEMP, '*', &V);
	badescapewarning(&logo, '%', &V);
	halt();
      } else
	putchar_(V.curr, &V);
    }



    if (V.prev == escape && V.curr == '(') {
      if (V.insidestring)
	badstring(&V);
      V.insidestring = true;
    }
  }
  if (V.italic) {
    printf("*** Italics string was not finished with \"\\i\" ***\n");
    fprintf(logo.f, "%%** Italics string was not finished with \"\\i\" ***\n");
    halt();
  }

  if (V.insidestring)
    V.insidestring = false;
  else {
    printf("This string:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, V.instring);
    printf("\nis bad because it needs a final \"\\(\".\n");
    printf("I am emptying the logo file.\n");
    if (*logo.name != '\0') {
      if (logo.f != NULL)
	logo.f = freopen(logo.name, "w", logo.f);
      else
	logo.f = fopen(logo.name, "w");
    } else {
      if (logo.f != NULL)
	rewind(logo.f);
      else
	logo.f = tmpfile();
    }
    if (logo.f == NULL)
      _EscIO2(FileNotFound, logo.name);
    SETUPBUF(logo.f, Char);
    halt();
  }


  if (V.italic)
    doitalics(&V);

  putchar_(')', &V);
}

#undef escape






Static Void truth(f, b)
_TEXT *f;
boolean b;
{
  if (b)
    fprintf(f->f, "true");
  else
    fprintf(f->f, "false");
}



Static Void p1(l, params)
_TEXT *l;
parameters params;
{
  fprintf(l->f, "%%\n");
  fprintf(l->f, "%% logo from %ld to %ld\n\n", params.lowest, params.highest);


  if (params.formcontrol == 'e')
    params.barbits = 1.0;
  if (params.formcontrol == 'e')
    params.subticsBig = 10;

  fprintf(l->f,
	  "/cmfactor 72 2.54 div def %% defines points -> cm conversion\n");
  fprintf(l->f, "/cm {cmfactor mul} bind def %% defines centimeters\n\n");

  fprintf(l->f, "%% user defined parameters\n");
  fprintf(l->f, "/lowest %ld def\n", params.lowest);
  fprintf(l->f, "/highest %ld def\n", params.highest);
  fprintf(l->f, "/bar %ld def\n", params.bar);
  fprintf(l->f, "/xcorner %*.*f cm def\n", pwid, pdec, params.xcorner);
  fprintf(l->f, "/ycorner %*.*f cm def\n", pwid, pdec, params.ycorner);
  fprintf(l->f, "/rotation %*.*f def %% degrees\n",
	  pwid, pdec, params.rotation);
  fprintf(l->f, "/charwidth %*.*f cm def\n", pwid, pdec, params.charwidth);
  fprintf(l->f, "/charwidth2m charwidth 2 mul def\n");
  fprintf(l->f, "/barheight %*.*f cm def\n", pwid, pdec, params.barheight);
  fprintf(l->f, "/barwidth %*.*f cm def\n", pwid, pdec, params.barwidth);
  fprintf(l->f, "/barbits %*.*f def %% bits\n", pwid, pdec, params.barbits);
  fprintf(l->f, "/Ibeamfraction %*.*f def\n",
	  pwid, pdec, params.Ibeamfraction);
  fprintf(l->f, "/barends (%c) def\n", params.barends);
  fprintf(l->f, "/subticsBig %ld def %% sub-tic interval size (1/bits)\n",
	  params.subticsBig);
  fprintf(l->f, "/subticsSmall %ld def %% sub-tic interval size (1/bits)\n",
	  params.subticsSmall);
  fprintf(l->f, "/showingbox (%c) def\n", params.showingbox);
  fprintf(l->f, "/outline ");
  truth(l, params.outline);
  fprintf(l->f, " def\n");
  fprintf(l->f, "/caps ");
  truth(l, params.caps);
  fprintf(l->f, " def\n");
  fprintf(l->f, "/stacksperline %ld def\n", params.stacksperline);
  fprintf(l->f, "/linesperpage %ld def\n", params.linesperpage);
  fprintf(l->f, "/linemove %*.*f def\n", pwid, pdec, params.linemove);
  fprintf(l->f, "/numbering ");
  truth(l, params.numbering);
  fprintf(l->f, " def\n");
  fprintf(l->f, "/shrinking ");
  truth(l, params.shrinking);
  fprintf(l->f, " def\n");
  fprintf(l->f, "/edgecontrol (%c) def\n", params.edgecontrol);
  fprintf(l->f, "/edgeleft %*.*f def\n", pwid, pdec, params.edgeleft);
  fprintf(l->f, "/edgeright %*.*f def\n", pwid, pdec, params.edgeright);
  fprintf(l->f, "/edgelow %*.*f def\n", pwid, pdec, params.edgelow);
  fprintf(l->f, "/edgehigh %*.*f def\n", pwid, pdec, params.edgehigh);
  fprintf(l->f, "/shrink %*.*f def\n", pwid, pdec, params.shrink);
  fprintf(l->f, "/ShowEnds (%c) def %% d: DNA, p: PROTEIN, -: none\n",
	  params.ShowEnds);
  fprintf(l->f, "/HalfWhiteIbeam ");
  truth(l, params.HalfWhiteIbeam);
  fprintf(l->f, " def\n");
  fprintf(l->f, "/squashspray %*.*f def\n\n", pwid, pdec, params.squashspray);

  fprintf(l->f, "/knirhs 1 shrink sub 2 div def\n");
  fprintf(l->f, "/charwidth4 charwidth 4 div def\n");
  fprintf(l->f, "/charwidth2 charwidth 2 div def\n\n");

  /*






*/

  fprintf(l->f, "/outlinewidth {charwidth %ld div} def\n",
	  (long)outlinethinness);
  fprintf(l->f, "/setthelinewidth {%% set the linewidth\n");
  fprintf(l->f, "  outline\n");
  fprintf(l->f, "    {outlinewidth setlinewidth}\n");
  fprintf(l->f, "    {1 setlinewidth}\n");
  fprintf(l->f, "  ifelse\n");
  fprintf(l->f, "} def\n");

  fprintf(l->f, "/toggleoutline { %% switch the state of outlineing\n");
  fprintf(l->f, "pop pop pop pop\n");
  fprintf(l->f, "/outline outline not def\n");
  fprintf(l->f, "setthelinewidth\n");
  fprintf(l->f, "} def\n\n");


  fprintf(l->f, "%% define fonts\n");
  fprintf(l->f, "/ffss {findfont fontsize scalefont setfont} def\n");
  /*

*/
  fprintf(l->f, "/FontForStringRegular {/Helvetica-Bold       ffss} def\n");
  fprintf(l->f, "/FontForStringItalic  {/Times-BoldItalic ffss} def\n");
  fprintf(l->f, "/FontForLogo          {/Helvetica-Bold   ffss} def\n");
  fprintf(l->f, "/FontForPrime         {/Symbol           ffss} def\n");
  fprintf(l->f, "/FontForSymbol        {/Symbol           ffss} def\n\n");

  fprintf(l->f, "%% Set up the font size for the graphics\n");
  fprintf(l->f, "/fontsize charwidth def\n\n");

  fprintf(l->f, "%% movements to place 5' and 3' symbols\n");
  fprintf(l->f, "/fivemovex {0} def\n");
  fprintf(l->f, "/fivemovey {(0) charparams lx ux sub 3 mul} def\n");
  fprintf(l->f, "/threemovex {(0) stringwidth pop 0.5 mul} def\n");
  fprintf(l->f, "/threemovey {fivemovey} def\n");

  fprintf(l->f,
	  "/prime {FontForPrime (\\242) show FontForStringRegular} def\n\n");

  fprintf(l->f, "%% make italics possible in titles\n");
  fprintf(l->f, "/IT {%% TRstring ITstring IT -\n");
  fprintf(l->f, "  exch show\n");
  fprintf(l->f, "  FontForStringItalic\n");
  fprintf(l->f, "  show\n");
  fprintf(l->f, "  FontForStringRegular\n");
  fprintf(l->f, "} def\n\n\n");

  fprintf(l->f, "%% make symbols possible in titles\n");
  fprintf(l->f, "/SY {%% TRstring SYstring SY -\n");
  fprintf(l->f, "  exch show\n");
  fprintf(l->f, "  FontForSymbol\n");
  fprintf(l->f, "  show\n");
  fprintf(l->f, "  FontForStringRegular\n");
  fprintf(l->f, "} def\n\n");

}



Static Void p2(l, colors, params)
_TEXT *l, *colors;
parameters params;
{
  Char symbol;
  double red, green, blue;

  putc('%', l->f);
  fprintf(l->f, "(*[[ This special comment allows deletion of the repeated\n");
  fprintf(l->f,
	  "%% procedures when several logos are concatenated together\n");
  fprintf(l->f, "%% See the censor program.\n\n");

  fprintf(l->f, "/charparams { %% char charparams => uy ux ly lx\n");
  fprintf(l->f,
	  "%% takes a single character and returns the coordinates that\n");
  fprintf(l->f, "%% defines the outer bounds of where the ink goes\n");
  fprintf(l->f, "  gsave\n");
  fprintf(l->f, "    newpath\n");
  fprintf(l->f, "    0 0 moveto\n");
  fprintf(l->f, "    %% take the character off the stack and use it here:\n");
  fprintf(l->f, "    true charpath \n");
  fprintf(l->f, "    flattenpath \n");
  fprintf(l->f,
    "    pathbbox %% compute bounding box of 1 pt. char => lx ly ux uy\n");
  fprintf(l->f, "    %% the path is here, but toss it away ...\n");
  fprintf(l->f, "  grestore\n");
  fprintf(l->f, "  /uy exch def\n");
  fprintf(l->f, "  /ux exch def\n");
  fprintf(l->f, "  /ly exch def\n");
  fprintf(l->f, "  /lx exch def\n");
  /*



*/
  fprintf(l->f, "} bind def\n\n");

  fprintf(l->f, "/dashbox { %% xsize ysize dashbox -\n");
  fprintf(l->f, "%% draw a dashed box of xsize by ysize (in points)\n");
  fprintf(l->f, "  /ysize exch def %% the y size of the box\n");
  fprintf(l->f, "  /xsize exch def %% the x size of the box\n");
  fprintf(l->f, "  1 setlinewidth\n");
  fprintf(l->f, "  gsave\n");
  fprintf(l->f, "    %% Define the width of the dashed lines for boxes:\n");
  fprintf(l->f, "    newpath\n");
  fprintf(l->f, "    0 0 moveto\n");
  fprintf(l->f, "    xsize 0 lineto\n");
  fprintf(l->f, "    xsize ysize lineto\n");
  fprintf(l->f, "    0 ysize lineto\n");
  fprintf(l->f, "    0 0 lineto\n");
  fprintf(l->f, "    [3] 0 setdash\n");
  fprintf(l->f, "    stroke\n");
  fprintf(l->f, "  grestore\n");
  fprintf(l->f, "  setthelinewidth\n");
  fprintf(l->f, "} bind def\n\n");

  fprintf(l->f, "/boxshow { %% xsize ysize char boxshow\n");
  fprintf(l->f,
	  "%% show the character with a box around it, sizes in points\n");
  fprintf(l->f, "gsave\n");
  fprintf(l->f, "  /tc exch def %% define the character\n");
  fprintf(l->f, "  /ysize exch def %% the y size of the character\n");
  fprintf(l->f, "  /xsize exch def %% the x size of the character\n");
  fprintf(l->f, "  /xmulfactor 1 def /ymulfactor 1 def\n\n");

  fprintf(l->f, "  %% if ysize is negative, make everything upside down!\n");
  fprintf(l->f, "  ysize 0 lt {\n");
  fprintf(l->f, "    %% put ysize normal in this orientation\n");
  fprintf(l->f, "    /ysize ysize abs def\n");
  fprintf(l->f, "    xsize ysize translate\n");
  fprintf(l->f, "    180 rotate\n");
  fprintf(l->f, "  } if\n\n");

  fprintf(l->f, "  shrinking {\n");
  fprintf(l->f, "    xsize knirhs mul ysize knirhs mul translate\n");
  fprintf(l->f, "    shrink shrink scale\n");
  fprintf(l->f, "  } if\n\n");

  fprintf(l->f, "  2 {\n");
  fprintf(l->f, "    gsave\n");
  fprintf(l->f, "    xmulfactor ymulfactor scale\n");
  fprintf(l->f, "    tc charparams\n");
  fprintf(l->f, "    grestore\n\n");

  /*


*/

  fprintf(l->f, "    ysize %% desired size of character in points\n");
  fprintf(l->f, "    uy ly sub %% height of character in points\n");
  fprintf(l->f, "    dup 0.0 ne {\n");
  fprintf(l->f, "      div %% factor by which to scale up the character\n");
  fprintf(l->f, "      /ymulfactor exch def\n");
  fprintf(l->f, "    } %% end if\n");
  fprintf(l->f, "    {pop pop}\n");
  fprintf(l->f, "    ifelse\n\n");
  fprintf(l->f, "    xsize %% desired size of character in points\n");
  fprintf(l->f, "    ux lx sub %% width of character in points\n");
  fprintf(l->f, "    dup 0.0 ne {\n");
  fprintf(l->f, "      div %% factor by which to scale up the character\n");
  fprintf(l->f, "      /xmulfactor exch def\n");
  fprintf(l->f, "    } %% end if\n");
  fprintf(l->f, "    {pop pop}\n");
  fprintf(l->f, "    ifelse\n");
  fprintf(l->f, "  } repeat\n\n");

  /*

*/
  fprintf(l->f, "  %% Adjust horizontal position if the symbol is an I\n");
  fprintf(l->f,
    "  tc (I) eq {charwidth 2 div %% half of requested character width\n");
  fprintf(l->f,
	  "             ux lx sub 2 div %% half of the actual character\n");
  fprintf(l->f, "                sub      0 translate} if\n");
  fprintf(l->f, "  %% Avoid x scaling for I\n");
  fprintf(l->f, "  tc (I) eq {/xmulfactor 1 def} if\n\n");

  fprintf(l->f, "  /xmove xmulfactor lx mul neg def\n");
  fprintf(l->f, "  /ymove ymulfactor ly mul neg def\n\n");

  fprintf(l->f, "  newpath\n");
  fprintf(l->f, "  xmove ymove moveto\n");
  fprintf(l->f, "  xmulfactor ymulfactor scale\n\n");

  fprintf(l->f, "  outline {  %% outline characters:\n");

  fprintf(l->f, "setthelinewidth\n");
  fprintf(l->f, "    tc true charpath\n");

  fprintf(l->f, "    gsave 1 setgray fill grestore\n");
  /*



*/
  fprintf(l->f, "    clip stroke\n");
  fprintf(l->f, "}\n");

  fprintf(l->f, "  { %% regular characters\n");
  fprintf(l->f, "    tc show\n");
  fprintf(l->f, "  }\n");
  fprintf(l->f, "  ifelse\n");

  fprintf(l->f, "grestore\n");
  fprintf(l->f, "} def\n\n");

  fprintf(l->f, "/numchar{ %% charheight character numchar\n");
  fprintf(l->f, "%% Make a character of given height in cm,\n");
  fprintf(l->f, "%% then move vertically by that amount\n");
  fprintf(l->f, "  gsave\n");
  fprintf(l->f, "    /char exch def\n");
  fprintf(l->f, "    /charheight exch cm def\n");


  if (*colors->name != '\0') {
    if (colors->f != NULL)
      colors->f = freopen(colors->name, "r", colors->f);
    else
      colors->f = fopen(colors->name, "r");
  } else
    rewind(colors->f);
  if (colors->f == NULL)
    _EscIO2(FileNotFound, colors->name);
  RESETBUF(colors->f, Char);
  fprintf(l->f, "    /visible true def %% most characters are visible\n");
  while (!BUFEOF(colors->f)) {
    if (!((P_peek(colors->f) != '*') & (!P_eoln(colors->f)))) {
      fscanf(colors->f, "%*[^\n]");
      getc(colors->f);
      continue;
    }

    if (P_peek(colors->f) == '\\')
      getc(colors->f);
    fscanf(colors->f, "%c%lg%lg%lg%*[^\n]", &symbol, &red, &green, &blue);
    getc(colors->f);

    if (symbol == '\n')
      symbol = ' ';
    if (!(((params.symbols == 4) & (capitalize(symbol) != symbol)) ||
	  params.symbols != 4))
      continue;


    if (params.symbols == 4) {
      /*

*/
      if (params.caps) {
	if (capitalize(symbol) != symbol)
	  symbol = capitalize(symbol);
      }
    }

    fprintf(l->f, "    char (");
    protectpostscript(l, symbol);
    fprintf(l->f, "%c) eq {", symbol);
    if (red < 0 || green < 0 || blue < 0) {
      printf("Character %c will be invisible\n", symbol);
      fprintf(l->f, "0 0 0 setrgbcolor /visible false def } if\n");
      continue;
    }
    if (red == 1.0 || red == 0.0)
      fprintf(l->f, "%ld", (long)floor(red + 0.5));
    else
      fprintf(l->f, "%*.*f", pwid, pdecolor, red);
    putc(' ', l->f);
    if (green == 1.0 || green == 0.0)
      fprintf(l->f, "%ld", (long)floor(green + 0.5));
    else
      fprintf(l->f, "%*.*f", pwid, pdecolor, green);
    putc(' ', l->f);
    if (blue == 1.0 || blue == 0.0)
      fprintf(l->f, "%ld", (long)floor(blue + 0.5));
    else
      fprintf(l->f, "%*.*f", pwid, pdecolor, blue);
    fprintf(l->f, " setrgbcolor} if\n");
  }


  fprintf(l->f, "     %% (* 2018 Jul 25 squash-spray test *)\n");
  /*

*/
  fprintf(l->f, "     charheight squashspray le {\n");
  fprintf(l->f,
	  "        /visible false def %% low height characters dropped\n");
  fprintf(l->f, "        %% Show the character as a rectangle if it would\n");
  fprintf(l->f, "        %% break the postscript display program.\n");
  fprintf(l->f, "        ly lx\n");
  fprintf(l->f, "        ly charwidth add\n");
  fprintf(l->f, "        lx charheight add\n");
  fprintf(l->f, "        boxsymbol\n");
  fprintf(l->f, "        clip\n");
  fprintf(l->f, "        fill\n");
  fprintf(l->f, "     } if\n");

  fprintf(l->f, "     visible {\n");
  fprintf(l->f, "       %% implement boxes, fill and characters:\n");
  fprintf(l->f, "       showingbox (s) eq\n");
  fprintf(l->f, "       showingbox (f) eq\n");
  fprintf(l->f, "       or\n");
  fprintf(l->f, "       {gsave\n");

  fprintf(l->f, "           ly lx\n");
  fprintf(l->f, "           ly charwidth add\n");
  fprintf(l->f, "           lx charheight add\n");
  fprintf(l->f, "           boxsymbol\n");
  fprintf(l->f, "           clip\n");

  fprintf(l->f, "           showingbox (f) eq\n");
  fprintf(l->f, "           {fill}  \n");
  fprintf(l->f, "           {gsave 0 setgray stroke grestore  \n");
  fprintf(l->f, "            charwidth charheight char boxshow\n");
  fprintf(l->f, "           }\n");
  fprintf(l->f, "           ifelse\n");
  fprintf(l->f, "       grestore\n");
  fprintf(l->f, "       }\n");
  fprintf(l->f, "       {charwidth charheight char boxshow}\n");
  fprintf(l->f, "       ifelse\n");
  fprintf(l->f, "     } if %% visibility control\n");

  fprintf(l->f, "  grestore\n");

  /*



*/

  /*


*/

  /*



*/

  if (gooddisplay)
    fprintf(l->f, "  0 charheight abs translate\n");
  else
    fprintf(l->f, "  charheight abs 1 gt {0 charheight abs translate} if\n");

  fprintf(l->f, "} bind def\n\n");

}



Static Void p3(l, params)
_TEXT *l;
parameters params;
{
  fprintf(l->f, "/Ibar{\n");
  fprintf(l->f, "%% make a horizontal bar\n");
  fprintf(l->f, "gsave\n");
  fprintf(l->f, "  newpath\n");
  fprintf(l->f, "    charwidth4 neg 0 moveto\n");
  fprintf(l->f, "    charwidth4 0 lineto\n");
  fprintf(l->f, "  stroke\n");
  fprintf(l->f, "grestore\n");
  fprintf(l->f, "} bind def\n\n");

  fprintf(l->f, "/Ibeam{ %% height Ibeam\n");
  fprintf(l->f, "%% Make an Ibeam of twice the given height, in cm\n");
  fprintf(l->f, "  /height exch cm def\n");
  fprintf(l->f, "  /heightDRAW height Ibeamfraction mul def\n");
  fprintf(l->f, "  1 setlinewidth\n");
  fprintf(l->f, "     HalfWhiteIbeam outline not and\n");
  fprintf(l->f, "     {0.75 setgray} %% grey on bottom\n");
  fprintf(l->f, "     {0 setgray} %% black on bottom\n");
  fprintf(l->f, "  ifelse\n");
  fprintf(l->f, "  gsave\n");
  fprintf(l->f, "    charwidth2 height neg translate\n");
  fprintf(l->f, "    Ibar\n");
  fprintf(l->f, "    newpath\n");
  fprintf(l->f, "      0 0 moveto\n");
  fprintf(l->f, "      0 heightDRAW rlineto\n");
  fprintf(l->f, "    stroke\n");
  fprintf(l->f, "    0 setgray %% black on top\n");
  fprintf(l->f, "    newpath\n");
  fprintf(l->f, "      0 height moveto\n");
  fprintf(l->f, "      0 height rmoveto\n");
  fprintf(l->f, "      currentpoint translate\n");
  fprintf(l->f, "    Ibar\n");
  fprintf(l->f, "    newpath\n");
  fprintf(l->f, "      0 0 moveto\n");
  fprintf(l->f, "      0 heightDRAW neg rlineto\n");
  fprintf(l->f, "      currentpoint translate\n");
  fprintf(l->f, "    stroke\n");
  fprintf(l->f, "  grestore\n");
  fprintf(l->f, "  setthelinewidth\n");
  fprintf(l->f, "} bind def\n\n");

  /*


















*/

  fprintf(l->f, "/makenumber { %% number makenumber\n");
  fprintf(l->f, "%% make the number\n");
  fprintf(l->f, "gsave\n");
  fprintf(l->f, "  shift %% shift to the other side of the stack\n");
  fprintf(l->f, "  90 rotate %% rotate so the number fits\n");
  fprintf(l->f, "  dup stringwidth pop %% find the length of the number\n");
  fprintf(l->f, "  neg %% prepare for move\n");
  fprintf(l->f, "  charwidth (0) charparams uy ly sub %% height of numbers\n");
  fprintf(l->f, "  sub 2 div %%\n");
  fprintf(l->f, "  moveto %% move back to provide space\n");
  fprintf(l->f, "  show\n");
  fprintf(l->f, "grestore\n");
  fprintf(l->f, "} bind def\n\n");

  fprintf(l->f, "/shift{ %% move to the next horizontal position\n");
  fprintf(l->f, "charwidth 0 translate\n");
  fprintf(l->f, "} bind def\n\n");

  /*





*/

  fprintf(l->f, "/bar2 barwidth 2 div def\n");
  fprintf(l->f, "/bar2n bar2 neg def\n");
  fprintf(l->f, "/makebar { %% make a vertical bar at the current location\n");
  fprintf(l->f, "gsave\n");
  fprintf(l->f, "   bar2n 0 moveto\n");
  fprintf(l->f, "   barwidth 0 rlineto\n");
  fprintf(l->f, "   0 barheight rlineto\n");
  fprintf(l->f, "   barwidth neg 0 rlineto\n");
  fprintf(l->f, "   closepath\n");
  fprintf(l->f, "   fill\n");
  fprintf(l->f, "grestore\n");
  fprintf(l->f, "} def\n\n");

  fprintf(l->f, "%% definitions for maketic\n");
  fprintf(l->f, "/str 10 string def %% string to hold number\n");
  fprintf(l->f, "%% points of movement between tic marks:\n");
  fprintf(l->f, "%% (abs protects against barbits being negative)\n");
  fprintf(l->f, "/ticmovement barheight barbits abs div def\n\n");
  fprintf(l->f, "/maketic { %% make tic marks and numbers\n");
  fprintf(l->f, "%% define tic mark to be the width of the number 4:\n");
  fprintf(l->f, "(4) stringwidth pop\n");
  fprintf(l->f, "/ticwidth exch def %% width of tic (as a dash) to show\n");
  fprintf(l->f, "gsave\n");
  fprintf(l->f, "  %% initial increment limit proc for\n");
  fprintf(l->f, "  0 1 barbits abs cvi\n");
  fprintf(l->f, "  {/loopnumber exch def\n\n");
  fprintf(l->f,
	  "    %% convert the number coming from the loop to a string\n");
  fprintf(l->f, "    %% and find its width\n");
  fprintf(l->f, "    loopnumber 10 str cvrs\n");
  fprintf(l->f,
	  "    /stringnumber exch def %% string representing the number\n\n");
  fprintf(l->f, "    stringnumber stringwidth pop\n");
  fprintf(l->f, "    /numberwidth exch def %% width of number to show\n\n");
  fprintf(l->f, "    /halfnumberheight\n");
  fprintf(l->f, "      stringnumber charparams %% capture sizes\n");
  fprintf(l->f, "      uy ly sub 2 div\n");
  fprintf(l->f, "    def\n\n\n");
  /*



*/
  fprintf(l->f, "    numberwidth %% move back width of number\n");
  fprintf(l->f, "    neg loopnumber ticmovement mul %% shift on y axis\n");
  fprintf(l->f, "    halfnumberheight sub %% down half the digit\n\n");
  fprintf(l->f, "    moveto %% move back the width of the string\n\n");
  fprintf(l->f,
	  "    ticwidth neg 0 rmoveto %% move back the width of the tic\n\n");
  fprintf(l->f, "    stringnumber show\n\n");
  fprintf(l->f, "    %% now show the tic mark\n");
  fprintf(l->f, "    0 halfnumberheight rmoveto %% shift up again\n");
  fprintf(l->f, "    ticwidth 0 rlineto\n");
  fprintf(l->f, "    stroke\n");
  /*
*/
  fprintf(l->f, "  }");
  fprintf(l->f, " for\n");
  fprintf(l->f, "grestore\n\n");

  fprintf(l->f, "%% do additional BIG tic marks.  subtics is user defined\n");
  fprintf(l->f, "  %% initial increment limit proc for\n");
  fprintf(l->f, "gsave\n");
  /*

*/
  fprintf(l->f, "  0 1 barbits subticsBig mul abs cvi\n");
  fprintf(l->f, "  {/bitnumber exch subticsBig div subticsBig div def\n");
  fprintf(l->f, "    0\n");
  fprintf(l->f,
    "    neg bitnumber ticmovement mul subticsBig mul %% shift on y axis\n");
  fprintf(l->f, "    moveto\n");
  fprintf(l->f, "    ticwidth neg 0 rlineto\n");
  fprintf(l->f, "    stroke\n");
  fprintf(l->f, "  }");
  fprintf(l->f, " for\n");

  fprintf(l->f, "/subticsBig %ld def %% sub-tic interval size (1/bits)\n",
	  params.subticsBig);

  fprintf(l->f,
	  "%% do additional SMALL tic marks.  subticsSmall is user defined\n");
  fprintf(l->f, "/ticwidth ticwidth 2 div def %% halve the ticwidth\n");
  fprintf(l->f, "  %% initial increment limit proc for\n");
  fprintf(l->f, "gsave\n");
  /*

*/
  fprintf(l->f, "  0 1 barbits subticsSmall mul abs cvi\n");
  fprintf(l->f, "  {/bitnumber exch subticsSmall div subticsSmall div def\n");
  fprintf(l->f, "    0\n");
  fprintf(l->f,
    "    neg bitnumber ticmovement mul subticsSmall mul %% shift on y axis\n");
  fprintf(l->f, "    moveto\n");
  fprintf(l->f, "    ticwidth neg 0 rlineto\n");
  fprintf(l->f, "    stroke\n");
  fprintf(l->f, "  }");
  fprintf(l->f, " for\n");

  fprintf(l->f, "grestore\n");

  /*

*/

  fprintf(l->f, "gsave\n");


  if (params.formcontrol == 'e')
    fprintf(l->f, "  /labelstring (frequency) def\n");
  else
    fprintf(l->f, "  /labelstring (bits) def\n");

  fprintf(l->f, "  numberwidth neg 2.5 mul\n");
  fprintf(l->f, "  barheight\n");
  fprintf(l->f, "  labelstring stringwidth pop\n");
  fprintf(l->f, "  sub 2 div\n");
  fprintf(l->f, "  translate\n");
  fprintf(l->f, "  90 rotate\n");
  fprintf(l->f, "  0 0 moveto\n");
  fprintf(l->f, "  labelstring show\n");
  fprintf(l->f, "grestore\n");

  fprintf(l->f, "} def\n");

}



Static Void startcosine(l)
_TEXT *l;
{

  /*





*/
  fprintf(l->f, "\n/degpercycle 360 def\n");
  fprintf(l->f, " \n");
  fprintf(l->f, "/drawcosine {%% amplitude  phase  wavelength  base\n");
  fprintf(l->f, "%%              xmin ymin xmax ymax step\n");
  fprintf(l->f, "%%              dashon dashoff dashoffset thickness\n");
  fprintf(l->f, "%%              cosine -\n");
  fprintf(l->f, "%% draws a cosine wave with the given parameters:\n");
  fprintf(l->f, "%% amplitude (points): height of the wave\n");
  fprintf(l->f, "%% phase (points): starting point of the wave\n");
  fprintf(l->f, "%% wavelength (points): length from crest to crest\n");
  fprintf(l->f, "%% base (points): lowest point of the curve\n");
  fprintf(l->f, "%% xmin ymin xmax ymax (points): region in which to draw\n");
  fprintf(l->f, "%% step steps for drawing a cosine wave\n");
  fprintf(l->f,
    "%% dashon if greater than zero, size of dashes of the wave (points)\n");
  fprintf(l->f, "%% dashon dashing on interval (points)\n");
  fprintf(l->f, "%% dashoff dashing off interval (points)\n");
  fprintf(l->f, "%% dashoffset offset for dashing (points)\n");
  fprintf(l->f,
	  "%% thickness if greater than zero, thickness of wave (points)\n");
  fprintf(l->f,
    "%% use dashon and dashoff as blank and dashoffset as 0 for solid line\n");
  fprintf(l->f,
    "%% See PostScrirt Language Reference Manual 2nd ed p. 500 on dash.\n\n");
  fprintf(l->f, "  /thickness exch def\n");
  fprintf(l->f, "  /dashoffset exch def\n");
  fprintf(l->f, "  /dashoff exch def\n");
  fprintf(l->f, "  /dashon exch def\n");
  fprintf(l->f, "  /step exch def\n");
  fprintf(l->f, "  /ymax exch def\n");
  fprintf(l->f, "  /xmax exch def\n");
  fprintf(l->f, "  /ymin exch def\n");
  fprintf(l->f, "  /xmin exch def\n");
  fprintf(l->f, "  /base exch def\n");
  fprintf(l->f, "  /wavelength exch def\n");
  fprintf(l->f, "  /phase exch def\n");
  fprintf(l->f, "  /amplitude exch def\n");
  fprintf(l->f,
	  "  %% fun := amplitude*cos( ((-y-phase)/wavelength)*360) + base\n");
  fprintf(l->f, "  /fun {phase sub wavelength div degpercycle mul cos\n");
  fprintf(l->f, "           amplitude mul base add} def\n\n");
  fprintf(l->f, "  gsave\n");
  fprintf(l->f, "    /originallinewidth currentlinewidth def\n");
  fprintf(l->f, "    thickness 0 gt {thickness setlinewidth} if\n\n");
  fprintf(l->f, "    %% Force the curve to fit into the region specified:\n");
  fprintf(l->f, "    newpath\n");
  fprintf(l->f, "    xmin ymin moveto\n");
  fprintf(l->f, "    xmax ymin lineto\n");
  fprintf(l->f, "    xmax ymax lineto\n");
  fprintf(l->f, "    xmin ymax lineto\n");
  fprintf(l->f, "    closepath\n");
  fprintf(l->f, "    clip\n\n");
  fprintf(l->f, "    newpath\n");
  fprintf(l->f, "    xmin dup fun moveto\n");
  fprintf(l->f,
	  "    %% go to xmin-1 and xmax+1 to make sure we overlap the\n");
  fprintf(l->f,
    "    %% next wave if there is one.  The clip above ensures that it\n");
  fprintf(l->f, "    %% goes no further than requested. \n");
  fprintf(l->f, "    %% loop from xmin-1 to xmax+1 by step:\n");
  fprintf(l->f, "    xmin 1 sub step xmax 1 add {dup fun lineto} for\n");
  fprintf(l->f, "    %% turn dash on if dashon is positive\n");
  fprintf(l->f,
    "    dashon 0 gt {[dashon cvi dashoff cvi] dashoffset setdash} if\n");
  fprintf(l->f, "    stroke\n\n");
  fprintf(l->f, "    originallinewidth setlinewidth\n");
  fprintf(l->f, "  grestore\n");
  fprintf(l->f, "} bind def\n\n");
}



Static Void p4(l)
_TEXT *l;
{
  startcosine(l);
}



Static Void p4a(l, params)
_TEXT *l;
parameters params;
{
  fprintf(l->f,
    "%% The following special comment allows deletion of the repeated\n");
  fprintf(l->f,
	  "%% procedures when several logos are concatenated together\n");
  fprintf(l->f, "%% See the censor program.\n");
  fprintf(l->f, "%%]]%%*)\n\n");

  fprintf(l->f, "/startpage { %% start a page\n");
  fprintf(l->f, "  save %% [ startpage\n");
  fprintf(l->f, "  %% set the font used in the title strings\n");
  fprintf(l->f, "  FontForStringRegular\n");
  fprintf(l->f, "  gsave %% [ startpage\n");
  fprintf(l->f, "  xcorner ycorner translate\n");
  fprintf(l->f, "  rotation rotate\n");
}



Static Void p5(l, symvec, params)
_TEXT *l, *symvec;
parameters params;
{
  strings *stringspot;
  long stringnumber = 0;
  stringDelila printstring, rawstring;
  /*
*/
  double rs = 0.0, sd = 0.0;
  long n = 0;
  boolean havers = false, havesd = false;
  _TEXT TEMP;

  if (params.thestrings == NULL)
    return;

  fprintf(l->f, "  %% create the user defined strings\n");
  stringspot = params.thestrings;


  while (stringspot != NULL) {
    fprintf(l->f, "  gsave\n");
    postscriptstring(&stringspot->astring, &printstring, &rawstring, symvec,
		     params.lowest, params.highest, rs, sd, n, havers,
		     havesd);
    if (stringspot->x <= centertrigger) {
      if (params.stacksperline > (params.highest - params.lowest) * 2) {
	printf("\nWARNING: if you don't see centered string number %ld:\n",
	       stringnumber);
	printf("    \"");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &stringspot->astring);
	printf("\"\n");
	printf(
	  "it may be off the page because stacksperline is large (%ld) relative to the\n",
	  params.stacksperline);
	printf(
	  "FROM-TO range (%ld to %ld) and centering is based on stacks-per-line.  To\n",
	  params.lowest, params.highest);
	printf("solve this, reduce parameter stacksperline to FROM-TO+1 = %ld\n\n",
	       params.highest - params.lowest + 1);
      }

      stringnumber++;
      fprintf(l->f, "    %% string number %ld\n", stringnumber);
      fprintf(l->f, "    %% center the string\n");
      fprintf(l->f, "    /stringscale %*.*f def\n", pwid, pdec, stringspot->s);

      /*























*/

      /*



*/

      fprintf(l->f, "    (");
      writestring(l, &rawstring);
      fprintf(l->f, ")\n");
      fprintf(l->f, "    stringwidth pop\n");
      fprintf(l->f, "    stringscale mul neg\n");
      fprintf(l->f, "    stacksperline charwidth mul\n");
      fprintf(l->f, "    add 2 div\n");
      fprintf(l->f, "    %*.*f cm moveto\n", pwid, pdec, stringspot->y);

    }


    else {
      fprintf(l->f, "    /stringscale %*.*f def\n", pwid, pdec, stringspot->s);
      fprintf(l->f, "    %*.*f cm %*.*f cm moveto\n",
	      pwid, pdec, stringspot->x, pwid, pdec, stringspot->y);
    }
    fprintf(l->f, "    stringscale stringscale scale\n");
    fprintf(l->f, "    ");
    writestring(l, &printstring);
    fprintf(l->f, "\n    show\n");
    stringspot = stringspot->next;
    fprintf(l->f, "  grestore\n");
  }
}



Static Void p6(l, params)
_TEXT *l;
parameters params;
{
  fprintf(l->f, "  %% now move up to the top of the top line:\n");
  fprintf(l->f, "  0 linesperpage linemove barheight mul mul translate\n\n");
  fprintf(l->f, "  %% set the font used in the logos\n");
  fprintf(l->f, "  FontForLogo\n");
  fprintf(l->f, "} def\n\n");

  fprintf(l->f,
	  "%%(*[[ This special comment allows deletion of the repeated\n");
  fprintf(l->f,
	  "%% procedures when several logos are concatenated together\n");
  fprintf(l->f, "%% See the censor program.\n\n");

  fprintf(l->f, "/endpage { %% end a page\n");
  fprintf(l->f, "  grestore %% ] endpage\n");
  fprintf(l->f, "  showpage %% REMOVE FOR PACKAGING INTO ANOTHER FIGURE\n");
  fprintf(l->f, "  restore %% ] endpage\n");
  fprintf(l->f, "} def\n\n");

  fprintf(l->f, "/showleftend {\n");
  fprintf(l->f, "gsave\n");
  fprintf(l->f, " charwidth neg 0 translate\n");
  fprintf(l->f, " fivemovex fivemovey moveto");
  fprintf(l->f, " ShowEnds (d) eq {(5) show prime} if\n");
  fprintf(l->f, " ShowEnds (p) eq {(N) show} if\n");
  fprintf(l->f, "grestore\n");
  fprintf(l->f, "} def\n\n");

  fprintf(l->f, "/showrightend {\n");
  fprintf(l->f, "gsave\n");
  fprintf(l->f, " threemovex threemovey moveto");
  fprintf(l->f, " ShowEnds (d) eq {(3) show prime} if\n");
  fprintf(l->f, " ShowEnds (p) eq {(C) show} if\n");
  fprintf(l->f, "grestore\n");
  fprintf(l->f, "} def\n\n");

  fprintf(l->f, "/startline{ %% start a line\n");
  fprintf(l->f, "%% move down to the bottom of the line:\n");
  fprintf(l->f, "  0 linemove barheight mul neg translate\n");
  fprintf(l->f, "  gsave %% [ startline\n");

  fprintf(l->f, "  %% put a bar on the left side:\n");
  fprintf(l->f, "  barends (b) eq barends (l) eq or {\n");
  fprintf(l->f, "    maketic %% maketic.startline\n");
  fprintf(l->f, "    gsave\n");
  fprintf(l->f, "      bar2n 0 translate %% makebar.startline\n");
  fprintf(l->f, "      makebar %% makebar.startline\n");
  fprintf(l->f, "    grestore\n");
  fprintf(l->f, "  } if\n");
  fprintf(l->f, "  showleftend\n");
  fprintf(l->f, "} def\n\n");

  fprintf(l->f, "/endline{ %% end a line\n");
  fprintf(l->f, "  showrightend\n");
  fprintf(l->f, "  %% put a bar on the right side:\n");
  fprintf(l->f, "  barends (b) eq barends (r) eq or {\n");
  fprintf(l->f, "    gsave\n");
  fprintf(l->f, "      bar2 0 translate %% makebar.endline\n");
  fprintf(l->f, "      makebar %% makebar.endline\n");
  fprintf(l->f, "    grestore\n");
  fprintf(l->f, "  } if\n");
  fprintf(l->f, "  grestore %% ] startline\n");
  fprintf(l->f, "} def\n\n");

  fprintf(l->f,
	  "%% @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  fprintf(l->f,
	  "%% @@@@@@@@@@@@@@@@@@@@ End of procedures @@@@@@@@@@@@@@@@@@@\n");
  fprintf(l->f,
	  "%% @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n");

  fprintf(l->f,
    "%% The following special comment allows deletion of the repeated\n");
  fprintf(l->f,
	  "%% procedures when several logos are concatenated together\n");
  fprintf(l->f, "%% See the censor program.\n");
  fprintf(l->f, "%%]]%%*)\n\n");

}




/*
*/


Static Void getmark(marks, logo, marksymbol, marktype, markbase1, markbits1,
		    markbase2, markbits2, markscale, nextmark,
		    waitfornextpiece, marksorder)
_TEXT *marks, *logo;
Char *marksymbol, *marktype;
double *markbase1, *markbits1, *markbase2, *markbits2, *markscale;
long *nextmark;
boolean *waitfornextpiece;
Char *marksorder;
{
  /*


*/
  boolean done = false;

  if (*waitfornextpiece)
    return;
  do {
    if (BUFEOF(marks->f)) {
      *nextmark = LONG_MAX;
      *marksymbol = ' ';
      done = true;
    } else {
      if (P_eoln(marks->f) | (P_peek(marks->f) == ' ')) {
	fscanf(marks->f, "%*[^\n]");
	getc(marks->f);
	/*


*/
	*marksymbol = '!';
      } else {
	*marksymbol = getc(marks->f);
	if (*marksymbol == '\n')
	  *marksymbol = ' ';
	if (*marksymbol != 'o' && *marksymbol != 'p' && *marksymbol != 'U' &&
	    *marksymbol != 'u' && *marksymbol != '*' && *marksymbol != 't' &&
	    *marksymbol != 's' && *marksymbol != 'l' && *marksymbol != 'b' &&
	    *marksymbol != 'c') {
	  printf("mark symbol in file marks must be one of: \"cblst*uUpo\"\n");
	  printf("\"%c\" is not allowed\n", *marksymbol);
	  halt();
	}

	if (*marksymbol != '*' && *marksymbol != 'u' && *marksymbol != 'U' &&
	    *marksymbol != 'p' && *marksymbol != 'o') {
	  *marktype = getc(marks->f);
	  if (*marktype == '\n')
	    *marktype = ' ';
	  if (*marktype != 'd' && *marktype != 'f' && *marktype != 's') {
	    printf("mark type in file marks must be one of: \"sfd\"\n");
	    printf("\"%c\" is not allowed\n", *marktype);
	    printf("ascii character: %d\n", *marktype);
	    halt();
	  }
	}

	switch (*marksymbol) {

	case '*':
	  fscanf(marks->f, "%*[^\n]");
	  getc(marks->f);
	  break;

	case 'c':
	case 't':
	case 's':
	  fscanf(marks->f, "%lg%lg%lg%*[^\n]", markbase1, markbits1,
		 markscale);
	  getc(marks->f);
	  *markbase2 = 0.0;
	  *markbits2 = 0.0;
	  if (*markscale <= 0.0) {
	    printf("mark scale must be positive\n");
	    halt();
	  }
	  *nextmark = (long)(*markbase1);
	  done = true;
	  break;

	case 'l':
	case 'b':
	  fscanf(marks->f, "%lg%lg%lg%lg%*[^\n]", markbase1, markbits1,
		 markbase2, markbits2);
	  getc(marks->f);
	  *nextmark = (long)(*markbase1);
	  done = true;
	  break;

	case 'u':
	  while (!done) {
	    if (BUFEOF(marks->f)) {
	      printf(
		"The \"!\" to mark the end of a user defined symbol in the marks file is missing.\n");
	      halt();
	    } else if (P_peek(marks->f) != '!')
	      copyaline(marks, logo);
	    else
	      done = true;
	  }
	  fscanf(marks->f, "%*[^\n]");
	  getc(marks->f);
	  done = false;
	  break;

	case 'U':
	  fscanf(marks->f, "%lg%lg%lg%lg", markbase1, markbits1, markbase2,
		 markbits2);
	  *nextmark = (long)(*markbase1);
	  done = true;
	  break;

	/*


*/
	case 'p':
	  *waitfornextpiece = true;
	  fscanf(marks->f, "%*[^\n]");
	  getc(marks->f);
	  break;

	case 'o':
	  if (P_eoln(marks->f)) {
	    printf("The \"o\" command must be followed by a character, bNADF\n");
	    halt();
	  }
	  fscanf(marks->f, "%c%*[^\n]", marksorder);
	  getc(marks->f);
	  if (*marksorder == '\n')
	    *marksorder = ' ';
	  break;
	}
      }
    }
  } while (!(done || *waitfornextpiece));
}



Static Void makemarktype(f, marktype)
_TEXT *f;
Char marktype;
{
  switch (marktype) {

  /*
*/
  case 'f':
    fprintf(f->f, " gsave stroke grestore fill\n");
    break;

  case 's':
    fprintf(f->f, " stroke\n");
    break;

  case 'd':
    fprintf(f->f, " [3] 0 setdash stroke\n");
    break;
  }
}



Static Void circlesymbol(f, x, y, radius, marktype)
_TEXT *f;
double x, y, radius;
Char marktype;
{
  /*
*/
  fprintf(f->f, " %*.*f cm %*.*f cm %*.*f cm circlesymbol\n",
	  pwid, pdec, x, pwid, pdec, y, pwid, pdec, radius);
  makemarktype(f, marktype);
}



Static Void trianglesymbol(f, x, y, radius, marktype)
_TEXT *f;
double x, y, radius;
Char marktype;
{
  /*
*/
  fprintf(f->f, " %*.*f cm %*.*f cm %*.*f cm trianglesymbol\n",
	  pwid, pdec, x, pwid, pdec, y, pwid, pdec, radius);
  makemarktype(f, marktype);
}



Static Void squaresymbol(f, x, y, side, marktype)
_TEXT *f;
double x, y, side;
Char marktype;
{
  /*
*/
  fprintf(f->f, " %*.*f cm %*.*f cm %*.*f cm squaresymbol\n",
	  pwid, pdec, x, pwid, pdec, y, pwid, pdec, side);
  makemarktype(f, marktype);
}



Static Void linesymbol(f, x1, y1, x2, y2, marktype)
_TEXT *f;
double x1, y1, x2, y2;
Char marktype;
{
  /*
*/
  fprintf(f->f, " %*.*f cm %*.*f cm %*.*f cm %*.*f cm linesymbol\n",
	  pwid, pdec, x1, pwid, pdec, y1, pwid, pdec, x2, pwid, pdec, y2);
  makemarktype(f, marktype);
}



Static Void boxsymbol(f, x1, y1, x2, y2, marktype)
_TEXT *f;
double x1, y1, x2, y2;
Char marktype;
{
  /*
*/
  fprintf(f->f, " %*.*f cm %*.*f cm %*.*f cm %*.*f cm boxsymbol\n",
	  pwid, pdec, x1, pwid, pdec, y1, pwid, pdec, x2, pwid, pdec, y2);
  makemarktype(f, marktype);
}



Static Void usersymbol(f, x1, y1, x2, y2, marks)
_TEXT *f;
double x1, y1, x2, y2;
_TEXT *marks;
{
  fprintf(f->f, " %*.*f cm %*.*f cm %*.*f cm %*.*f cm \n",
	  pwid, pdec, x1, pwid, pdec, y1, pwid, pdec, x2, pwid, pdec, y2);
  copyaline(marks, f);
}



Static Void markup(logo, thecharacterwidth, cmperbit, position_, marks,
		   marksymbol, marktype, markbase1, markbits1, markbase2,
		   markbits2, markscale, nextmark, waitfornextpiece,
		   marksorder)
_TEXT *logo;
double thecharacterwidth, cmperbit;
long position_;
_TEXT *marks;
Char *marksymbol, *marktype;
double *markbase1, *markbits1, *markbase2, *markbits2, *markscale;
long *nextmark;
boolean *waitfornextpiece;
Char *marksorder;
{
  /*
*/
  double radius;
  /*
*/
  double x1, y1, x2, y2;

  while (position_ == *nextmark && !*waitfornextpiece) {
    fprintf(logo->f, "gsave\n");

    switch (*marksymbol) {

    case 'c':
    case 't':
    case 's':
      x1 = (*markbase1 - *nextmark + 0.5) * thecharacterwidth;
      y1 = *markbits1 * cmperbit;
      radius = *markscale * thecharacterwidth / 2.0;
      switch (*marksymbol) {

      case 'c':
	circlesymbol(logo, x1, y1, radius, *marktype);
	break;

      case 't':
	trianglesymbol(logo, x1, y1, radius, *marktype);
	break;

      case 's':
	squaresymbol(logo, x1, y1, radius, *marktype);
	break;
      }
      break;

    case 'l':
    case 'b':
    case 'U':
      x1 = (*markbase1 - *nextmark + 0.5) * thecharacterwidth;
      y1 = *markbits1 * cmperbit;
      x2 = (*markbase2 - *nextmark + 0.5) * thecharacterwidth;
      y2 = *markbits2 * cmperbit;
      switch (*marksymbol) {

      case 'b':
	boxsymbol(logo, x1, y1, x2, y2, *marktype);
	break;

      case 'l':
	linesymbol(logo, x1, y1, x2, y2, *marktype);
	break;

      case 'U':
	usersymbol(logo, x1, y1, x2, y2, marks);
	break;
      }
      break;
    }

    fprintf(logo->f, "grestore\n");


    getmark(marks, logo, marksymbol, marktype, markbase1, markbits1,
	    markbase2, markbits2, markscale, nextmark, waitfornextpiece,
	    marksorder);
  }

  /*



*/
}



Static Void marksymbols(l)
_TEXT *l;
{
  fprintf(l->f, "/circlesymbol { %% x y radius circlesymbol - (path)\n");
  fprintf(l->f, "newpath 0 360 arc closepath} bind def\n\n");

  /*




















*/

  fprintf(l->f, "/sqrt3 3 sqrt def\n");
  fprintf(l->f, "/trianglesymbol { %% x y radius trianglesymbol - (path)\n");
  fprintf(l->f, "/r exch def\n");
  fprintf(l->f, "/sqrt3r sqrt3 r mul def\n");
  fprintf(l->f, "translate\n");

  fprintf(l->f, "120 rotate\n");

  fprintf(l->f, "0 r translate\n");
  fprintf(l->f, "-120 rotate\n");
  fprintf(l->f, "newpath\n");
  fprintf(l->f, "0 0 moveto\n");
  fprintf(l->f, "sqrt3r 0 lineto\n");
  fprintf(l->f, "-300 rotate\n");

  fprintf(l->f, "sqrt3r 0 lineto\n");
  fprintf(l->f, "closepath} bind def\n\n");

  fprintf(l->f, "/squaresymbol { %% x y side squaresymbol - (path)\n");
  fprintf(l->f, "/side exch def\n");
  fprintf(l->f, "translate\n");
  fprintf(l->f, "side 2 div neg dup translate\n");
  fprintf(l->f, "newpath\n");
  fprintf(l->f, "0 0 moveto\n");
  fprintf(l->f, "0 side lineto\n");
  fprintf(l->f, "side side lineto\n");
  fprintf(l->f, "side 0 lineto\n");
  fprintf(l->f, "closepath} bind def\n\n");

  fprintf(l->f, "/linesymbol { %% x1 y1 x2 y2 linesymbol - (path)\n");
  fprintf(l->f, "/y2 exch def\n");
  fprintf(l->f, "/x2 exch def\n");
  fprintf(l->f, "/y1 exch def\n");
  fprintf(l->f, "/x1 exch def\n");
  fprintf(l->f, "newpath\n");
  fprintf(l->f, "x1 y1 moveto\n");
  fprintf(l->f, "x2 y2 lineto\n");
  fprintf(l->f, "} bind def\n\n");

  fprintf(l->f, "/boxsymbol { %% x1 y1 x2 y2 boxsymbol - (path)\n");
  fprintf(l->f, "/y2 exch def\n");
  fprintf(l->f, "/x2 exch def\n");
  fprintf(l->f, "/y1 exch def\n");
  fprintf(l->f, "/x1 exch def\n");
  fprintf(l->f, "newpath\n");
  fprintf(l->f, "x1 y1 moveto\n");
  fprintf(l->f, "x2 y1 lineto\n");
  fprintf(l->f, "x2 y2 lineto\n");
  fprintf(l->f, "x1 y2 lineto\n");
  fprintf(l->f, "closepath\n");
  fprintf(l->f, "} bind def\n\n");
}






Static Void startpostscript(l, colors, symvec, params)
_TEXT *l, *colors, *symvec;
parameters params;
{
  /*



*/
  p1(l, params);
  p2(l, colors, params);
  p3(l, params);
  p4(l);
  marksymbols(l);
  p4a(l, params);
  p5(l, symvec, params);
  p6(l, params);
}






Static boolean softnumbertest(f)
_TEXT *f;
{
  skipblanks(f);
  if (P_peek(f->f) != '9' && P_peek(f->f) != '8' && P_peek(f->f) != '7' &&
      P_peek(f->f) != '6' && P_peek(f->f) != '5' && P_peek(f->f) != '4' &&
      P_peek(f->f) != '3' && P_peek(f->f) != '2' && P_peek(f->f) != '1' &&
      P_peek(f->f) != '0' && P_peek(f->f) != '+' && P_peek(f->f) != '-') {
    /*




*/
    return false;
  } else
    return true;
}


/* Local variables for checknumber: */
struct LOC_checknumber {
  _TEXT *afile;
  boolean ok;
} ;

Local Void conclude(LINK)
struct LOC_checknumber *LINK;
{
  _TEXT TEMP;

  printf("Including this character, the rest of the data line is:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  copyaline(LINK->afile, &TEMP);
  LINK->ok = false;
}



Static boolean checknumber(afile_)
_TEXT *afile_;
{
  /*
*/
  struct LOC_checknumber V;

  V.afile = afile_;
  V.ok = true;
  if (BUFEOF(V.afile->f)) {
    V.ok = false;
    printf("A number was expected on a data line, but");
    printf(" the end of the file was found instead.\n");
    return false;
  }
  skipblanks(V.afile);
  if (P_eoln(V.afile->f)) {
    printf("A number was expected on a data line, but");
    printf(" the end of the line was found instead.\n");
    conclude(&V);
  }
  if (P_peek(V.afile->f) == '+' || P_peek(V.afile->f) == '-' ||
      P_peek(V.afile->f) == '.' || P_peek(V.afile->f) == '9' ||
      P_peek(V.afile->f) == '8' || P_peek(V.afile->f) == '7' ||
      P_peek(V.afile->f) == '6' || P_peek(V.afile->f) == '5' ||
      P_peek(V.afile->f) == '4' || P_peek(V.afile->f) == '3' ||
      P_peek(V.afile->f) == '2' || P_peek(V.afile->f) == '1' ||
      P_peek(V.afile->f) == '0')
    return V.ok;
  printf("A number was expected on a data line, but");
  printf(" the character \"%c\" was found instead.\n", P_peek(V.afile->f));
  conclude(&V);
  return V.ok;
}


/* Local variables for readparameters: */
struct LOC_readparameters {
  _TEXT *theplace;
} ;

Local Void cn(LINK)
struct LOC_readparameters *LINK;
{
  boolean checkout;

  checkout = checknumber(LINK->theplace);
  if (!checkout)
    halt();
}



Static Void readparameters(theplace_, params)
_TEXT *theplace_;
parameters *params;
{
  struct LOC_readparameters V;
  boolean gotten, gottoresetcorner;
  /*
*/
  double graphshift;
  /*
*/
  long numberstrings, n, spl;
  strings *stringspot;
  double zerobase;
  _TEXT TEMP;

  V.theplace = theplace_;
  if (*V.theplace->name != '\0') {
    if (V.theplace->f != NULL)
      V.theplace->f = freopen(V.theplace->name, "r", V.theplace->f);
    else
      V.theplace->f = fopen(V.theplace->name, "r");
  } else
    rewind(V.theplace->f);
  if (V.theplace->f == NULL)
    _EscIO2(FileNotFound, V.theplace->name);
  RESETBUF(V.theplace->f, Char);
  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%ld%ld%*[^\n]", &params->lowest, &params->highest);
    getc(V.theplace->f);
  } else {
    printf("missing all parameters\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%ld%*[^\n]", &params->bar);
    getc(V.theplace->f);
  } else {
    printf("missing bar coordinate parameter\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    if (P_peek(V.theplace->f) != 'z') {
      cn(&V);
      fscanf(V.theplace->f, "%lg%lg%*[^\n]", &params->xcorner,
	     &params->ycorner);
      getc(V.theplace->f);
      gottoresetcorner = false;
    } else {
      getc(V.theplace->f);
      cn(&V);
      fscanf(V.theplace->f, "%lg%lg%lg%*[^\n]", &params->xcorner,
	     &params->ycorner, &zerobase);
      getc(V.theplace->f);
      gottoresetcorner = true;
    }
  } else {
    printf("missing corner coordinate parameters\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%lg%*[^\n]", &params->rotation);
    getc(V.theplace->f);
  } else {
    printf("missing rotation parameter\n");
    halt();
  }
  if ((long)floor(params->rotation + 0.5) % 90 != 0) {
    printf("\nWARNING: rotations that are not multiples of\n");
    printf("of 90 degrees are likely to be scaled\n");
    printf("incorrectly, due to a PostScript limitation.\n");
    printf("See the description of the pathbbox function\n");
    printf("in the Red book.\n\n");
  }

  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%lg", &params->charwidth);

    params->squashspray = squashspraydefault;

    skipblanks(V.theplace);
    if (!P_eoln(V.theplace->f)) {
      if (P_peek(V.theplace->f) == '9' || P_peek(V.theplace->f) == '8' ||
	  P_peek(V.theplace->f) == '7' || P_peek(V.theplace->f) == '6' ||
	  P_peek(V.theplace->f) == '5' || P_peek(V.theplace->f) == '4' ||
	  P_peek(V.theplace->f) == '3' || P_peek(V.theplace->f) == '2' ||
	  P_peek(V.theplace->f) == '1' || P_peek(V.theplace->f) == '0')
	fscanf(V.theplace->f, "%lg", &params->squashspray);
    }
    fscanf(V.theplace->f, "%*[^\n]");
    getc(V.theplace->f);
  } else {
    printf("missing charwidth parameter\n");
    halt();
  }
  if (params->charwidth <= 0.0) {
    printf("charwidth parameter must be > 0 cm\n");
    halt();
  }


  if (gottoresetcorner) {
    /*
*/
    graphshift = params->charwidth * (params->lowest - zerobase - 0.5);
    /*






*/
    if (fabs(params->rotation) > 135)
      params->xcorner += graphshift;
    else if (params->rotation > 0)
      params->xcorner -= graphshift;
    else if (params->rotation < 0)
      params->ycorner += graphshift;
    else
      params->xcorner += graphshift;
  }

  /*

*/
  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%lg%lg%*[^\n]", &params->barheight,
	   &params->barwidth);
    getc(V.theplace->f);
  } else {
    printf("missing barheight and barwidth parameters\n");
    halt();
  }
  if (params->barheight <= 0.0) {
    printf("barheight parameter must be > 0 cm\n");
    halt();
  }
  if (params->barwidth <= 0.0) {
    printf("barwidth parameter must be > 0 cm\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%lg%*[^\n]", &params->barbits);
    getc(V.theplace->f);
  } else {
    printf("missing barbits parameter\n");
    halt();
  }
  if (params->barbits < 0.0) {
    printf(
      "\nWARNING: are you SURE you don't want to show the variation due to a small\n");
    printf("number of samples (i.e. the I-beam error bars)?\n\n");
  } else {
    printf("* Reminder: the I-beam error bars show the\n");
    printf("  variation of the entire stack not just the highest\n");
    printf("  symbol, even though it may look like that.\n");
  }
  if (params->barbits == 0.0) {
    printf("barbits parameter must not be zero\n");
    halt();
  }

  /*

*/
  if (softnumbertest(V.theplace)) {
    fscanf(V.theplace->f, "%lg%*[^\n]", &params->Ibeamfraction);
    getc(V.theplace->f);
    /*
*/
  } else
    params->Ibeamfraction = 1.0;

  if (!BUFEOF(V.theplace->f)) {
    params->barends = getc(V.theplace->f);
    if (params->barends == '\n')
      params->barends = ' ';
    params->subticsBig = subticsBigdefault;
    params->subticsSmall = subticsSmalldefault;
    if (!P_eoln(V.theplace->f)) {
      params->ticcommand = getc(V.theplace->f);
      if (params->ticcommand == '\n')
	params->ticcommand = ' ';
      if (params->ticcommand == 't') {
	fscanf(V.theplace->f, "%ld%ld", &params->subticsBig,
	       &params->subticsSmall);
	if (params->subticsBig <= 0) {
	  printf("subticsBig is %ld but it must be positive\n",
		 params->subticsBig);
	  halt();
	}
	if (params->subticsSmall <= 0) {
	  printf("subticsSmall is %ld but it must be positive\n",
		 params->subticsSmall);
	  halt();
	}
      }
    }
    fscanf(V.theplace->f, "%*[^\n]");
    getc(V.theplace->f);
  } else {
    printf("barends parameter is missing\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    fscanf(V.theplace->f, "%c%*[^\n]", &params->showingbox);
    getc(V.theplace->f);
    if (params->showingbox == '\n')
      params->showingbox = ' ';
  } else {
    printf("showingbox parameter is missing\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    if (P_peek(V.theplace->f) == 'o')
      params->outline = true;
    else
      params->outline = false;
    fscanf(V.theplace->f, "%*[^\n]");
    getc(V.theplace->f);
  } else {
    printf("outline parameter is missing\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    if (P_peek(V.theplace->f) == 'c')
      params->caps = true;
    else
      params->caps = false;
    fscanf(V.theplace->f, "%*[^\n]");
    getc(V.theplace->f);
  } else {
    printf("caps parameter is missing\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%ld%*[^\n]", &params->stacksperline);
    getc(V.theplace->f);
  } else {
    printf("stacksperline parameter is missing\n");
    halt();
  }
  if (params->stacksperline <= 0) {
    printf("stacksperline parameter must be positive\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%ld%*[^\n]", &params->linesperpage);
    getc(V.theplace->f);
  } else {
    printf("linesperpage parameter is missing\n");
    halt();
  }
  if (params->linesperpage <= 0) {
    printf("linesperpage parameter must be positive\n");
    halt();
  }

  if (params->linesperpage == 1) {
    spl = params->highest - params->lowest + 1;
    if (params->stacksperline != spl) {
      printf("* NOTE: Since there is only one line per page\n");
      printf("  requested stacks per line (%ld)\n", params->stacksperline);
      printf("  will be replaced with computed stacks per line (%ld)\n", spl);
      printf("  to make centered strings work correctly\n");
      params->stacksperline = spl;
    }
  }

  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%lg%*[^\n]", &params->linemove);
    getc(V.theplace->f);
  } else {
    printf("linemove parameter is missing\n");
    halt();
  }
  if (params->linemove <= 0) {
    printf("linemove parameter must be positive\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    if (P_peek(V.theplace->f) == 'n')
      params->numbering = true;
    else
      params->numbering = false;
    fscanf(V.theplace->f, "%*[^\n]");
    getc(V.theplace->f);
  } else {
    printf("numbering parameters are missing\n");
    halt();
  }

  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%lg%*[^\n]", &params->shrink);
    getc(V.theplace->f);
  } else {
    printf("shrink parameter is missing\n");
    halt();
  }
  if (params->shrink > 1.0) {
    printf("Shrink parameter must be less than or equal to 1.\n");
    printf("This way, you won't create a misleading logo\n");
    printf("which has an apparent height larger than it should.\n");
    halt();
  }
  if (params->shrink > 0.0 && params->shrink < 1.0 &&
      params->showingbox != 'b') {
    printf("Shrinking can only be done when showing the box.\n");
    printf("This way, you won't create a misleading logo\n");
    printf("which has an apparent height smaller than it should.\n");
    halt();
  }
  if (params->showingbox != 'b' || params->shrink <= 0.0 ||
      params->shrink >= 1.0)
    params->shrinking = false;
  else
    params->shrinking = true;

  if (!BUFEOF(V.theplace->f)) {
    cn(&V);
    fscanf(V.theplace->f, "%ld%*[^\n]", &numberstrings);
    getc(V.theplace->f);
  } else {
    printf("number of user defined strings must be given\n");
    halt();
  }
  if (numberstrings > 0) {
    params->thestrings = (strings *)Malloc(sizeof(strings));
    stringspot = params->thestrings;
    for (n = 1; n <= numberstrings; n++) {
      if (BUFEOF(V.theplace->f)) {
	printf("missing a user defined string coordinate\n");
	halt();
      }
      fscanf(V.theplace->f, "%lg%lg%lg%*[^\n]", &stringspot->x,
	     &stringspot->y, &stringspot->s);
      getc(V.theplace->f);
      getstring(V.theplace, &stringspot->astring, &gotten);

      putchar('"');
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &stringspot->astring);
      printf("\"\n");
      if (!gotten) {
	printf("missing a user defined string\n");
	halt();
      }
      /*



*/


      if (n < numberstrings) {
	stringspot->next = (strings *)Malloc(sizeof(strings));
	stringspot = stringspot->next;
      } else
	stringspot->next = NULL;
    }
  } else
    params->thestrings = NULL;

  if (!BUFEOF(V.theplace->f)) {
    params->edgecontrol = getc(V.theplace->f);
    if (params->edgecontrol == '\n')
      params->edgecontrol = ' ';
    if (params->edgecontrol != 'p') {
      cn(&V);
      fscanf(V.theplace->f, "%lg%lg%lg%lg", &params->edgeleft,
	     &params->edgeright, &params->edgelow, &params->edgehigh);
    } else {
      params->edgeleft = edgemargindefault;
      params->edgeright = edgemargindefault;
      params->edgelow = edgemargindefault;
      params->edgehigh = edgemargindefault;
    }
    fscanf(V.theplace->f, "%*[^\n]");
    getc(V.theplace->f);
  } else {
    params->edgecontrol = ' ';
    params->edgeleft = edgemargindefault;
    params->edgeright = edgemargindefault;
    params->edgelow = edgemargindefault;
    params->edgehigh = edgemargindefault;
    printf("NOTE:  you can now define the edges, see the documentation.\n");
    printf("Edges are all defaulting to %ld cm.\n", (long)edgemargindefault);
  }

  if (!BUFEOF(V.theplace->f)) {
    fscanf(V.theplace->f, "%c%*[^\n]", &params->ShowEnds);
    getc(V.theplace->f);
    if (params->ShowEnds == '\n')
      params->ShowEnds = ' ';
    if (params->ShowEnds != 'd' && params->ShowEnds != 'p')
      params->ShowEnds = '-';
  } else
    params->ShowEnds = '-';

  if (BUFEOF(V.theplace->f)) {
    params->formcontrol = 'n';
    return;
  }
  params->formcontrol = getc(V.theplace->f);

  if (params->formcontrol == '\n')
    params->formcontrol = ' ';
  if (params->formcontrol != 'R' && params->formcontrol != 'r' &&
      params->formcontrol != 'v' && params->formcontrol != 'e')
    params->formcontrol = 'n';
}



Static Void warnifsitesused(symvec, fout, sitesused)
_TEXT *symvec, *fout;
boolean *sitesused;
{
  /*
*/
  Char a, b, c, d, e, f, g;

  *sitesused = false;
  if (*symvec->name != '\0') {
    if (symvec->f != NULL)
      symvec->f = freopen(symvec->name, "r", symvec->f);
    else
      symvec->f = fopen(symvec->name, "r");
  } else
    rewind(symvec->f);
  if (symvec->f == NULL)
    _EscIO2(FileNotFound, symvec->name);
  RESETBUF(symvec->f, Char);
  if (BUFEOF(symvec->f))
    return;
  fscanf(symvec->f, "%*[^\n]");
  getc(symvec->f);
  if (BUFEOF(symvec->f))
    return;


  a = getc(symvec->f);
  b = getc(symvec->f);
  c = getc(symvec->f);
  d = getc(symvec->f);
  e = getc(symvec->f);
  f = getc(symvec->f);
  g = getc(symvec->f);
  if (a == '\n')
    a = ' ';
  if (b == '\n')
    b = ' ';
  if (c == '\n')
    c = ' ';
  if (d == '\n')
    d = ' ';
  if (e == '\n')
    e = ' ';
  if (f == '\n')
    f = ' ';
  if (g == '\n')
    g = ' ';
  if (a != '*' || b != ' ' || c != 's' || d != 'i' || e != 't' || f != 'e' ||
      g != 's')
    return;
  fprintf(fout->f,
    "******************************************************************************\n");
  fprintf(fout->f,
    "* NOTE: since the data come from the sites program, the error bar is not     *\n");
  fprintf(fout->f,
    "* printed on the sequence logo.                                              *\n");
  fprintf(fout->f,
    "******************************************************************************\n");
  *sitesused = true;
}


#define threshold       0.00001



Static Void makesymbol(afile, height, caps, c)
_TEXT *afile;
double height;
boolean caps;
Char c;
{
  /*
*/
  if (fabs(height) <= threshold)
    return;
  fprintf(afile->f, "%*.*f (", pwid, pdec, height);
  protectpostscript(afile, c);
  if (caps)
    fputc(capitalize(c), afile->f);
  else
    putc(c, afile->f);
  fprintf(afile->f, ") numchar\n");
}

#undef threshold



Static Void Ibeam(afile, height)
_TEXT *afile;
double height;
{
  /*
*/
  fprintf(afile->f, "%*.*f Ibeam\n", pwid, pdec, height);
}



Static Void summary(f, c, rstotal, varhnb, thefrom, theto, gaps, density)
_TEXT *f;
Char c;
double rstotal, varhnb;
long thefrom, theto, gaps;
double density;
{
  if (c != ' ')
    fprintf(f->f, "%c ", c);
  fprintf(f->f, "%ld gaps in the sequences\n", gaps);
  if (c != ' ')
    fprintf(f->f, "%c ", c);
  fprintf(f->f, "Rs total is %*.*f", infofield, infodecim, rstotal);
  if (varhnb >= 0)
    fprintf(f->f, " +/- %*.*f bits", infofield, infodecim, sqrt(varhnb));
  else
    fprintf(f->f, " bits (sample error not known)");
  fprintf(f->f, " in the range from %ld to %ld", thefrom, theto);

  fprintf(f->f, " d= %1.*f\n", infodecim, density);
}



Static Void makenumber(l, number)
_TEXT *l;
long number;
{
  fprintf(logo.f, "numbering {");
  fprintf(l->f, "(%ld) makenumber", number);
  fprintf(logo.f, "} if\n");
}



Static Void cosine(afile, amplitude, phase, wavelength, base, xmin, ymin,
		   xmax, ymax, step, dashon, dashoff, dashoffset, thickness)
_TEXT *afile;
double amplitude, phase, wavelength, base, xmin, ymin, xmax, ymax;
long step;
double dashon, dashoff, dashoffset, thickness;
{
  fprintf(afile->f, "%% amplitude  phase  wavelength  base:\n");
  fprintf(afile->f, " %*.*f cm %*.*f cm %*.*f cm %*.*f cm\n",
	  pwid, pdec, amplitude, pwid, pdec, phase, pwid, pdec, wavelength,
	  pwid, pdec, base);
  fprintf(afile->f, "%% xmin ymin xmax ymax step:\n");
  fprintf(afile->f, " %*.*f cm %*.*f cm %*.*f cm %*.*f cm %ld\n",
	  pwid, pdec, xmin, pwid, pdec, ymin, pwid, pdec, xmax, pwid, pdec,
	  ymax, step);
  fprintf(afile->f, "%% dash settings:\n");
  fprintf(afile->f, " %*.*f cm", pwid, pdec, dashon);
  fprintf(afile->f, " %*.*f cm", pwid, pdec, dashoff);
  fprintf(afile->f, " %*.*f cm", pwid, pdec, dashoffset);
  fprintf(afile->f, " %*.*f cm drawcosine\n", pwid, pdec, thickness);
}



Static Void docosinewave(logo, wp, charwidth, cmperbit, barheight,
			 stacknumber, position_)
_TEXT *logo;
waveparam *wp;
double charwidth, cmperbit, barheight;
long stacknumber, position_;
{
  /*
*/
  double xscale, yscale, xmin, ymin, xmax, ymax;
  waveparam *thiswave = wp;
  waveparam *WITH;

  while (thiswave != NULL) {
    WITH = thiswave;
    xscale = charwidth;
    yscale = cmperbit;
    xmin = (position_ - stacknumber + 1) * xscale;
    xmax = (position_ + 1) * xscale;
    ymin = 0.0;
    ymax = WITH->waveamplitude + barheight * yscale;

    fprintf(logo->f, "gsave\n");
    fprintf(logo->f, " %*.*f cm 0 translate\n",
	    pwid, pdec, -(position_ + 1) * charwidth);
    if (WITH->extreme == 'h') {
      /*

*/
      cosine(logo, yscale * WITH->waveamplitude,
	     xscale * (WITH->wavelocation + 0.5), xscale * WITH->wavelength,
	     yscale * (WITH->wavebit - WITH->waveamplitude), xmin, ymin, xmax,
	     ymax, 1L, WITH->dashon, WITH->dashoff, WITH->dashoffset,
	     WITH->thickness);
    } else
      cosine(logo, -yscale * WITH->waveamplitude,
	     xscale * (WITH->wavelocation + 0.5), xscale * WITH->wavelength,
	     yscale * (WITH->wavebit + WITH->waveamplitude), xmin, ymin, xmax,
	     ymax, 1L, WITH->dashon, WITH->dashoff, WITH->dashoffset,
	     WITH->thickness);
    fprintf(logo->f, "grestore\n");
    thiswave = thiswave->next;
  }
}


#define warnchar        '*'

#define warnwidth       70


Local Void showbar(n, c)
long n;
Char c;
{
  long count;

  for (count = 1; count <= n; count++)
    putchar(c);
  putchar('\n');
}



Static Void setboundingbox(logo, params)
_TEXT *logo;
parameters params;
{
  double cmfactor;
  /*
*/
  double BaseAxis, BitAxis, llxhere, llyhere, urxhere, uryhere;
  boolean srotation = false, pagewarning = false;

  /*



*/

  if (params.highest - params.lowest + 1 > params.stacksperline)
    BaseAxis = params.charwidth * params.stacksperline;
  else
    BaseAxis = params.charwidth * (params.highest - params.lowest + 1);

  BitAxis = params.barheight * params.linesperpage * params.linemove;


  if (params.edgecontrol != 'p') {
    cmfactor = 72 / 2.54;
    if ((long)floor(params.rotation + 0.5) == -90) {
      llxhere = params.xcorner;
      llyhere = params.ycorner - BaseAxis;
      urxhere = params.xcorner + BitAxis;
      uryhere = params.ycorner;


      llxhere = (llxhere - params.edgelow) * cmfactor;
      llyhere = (llyhere - params.edgeright) * cmfactor;
      urxhere = (urxhere + params.edgehigh) * cmfactor;
      uryhere = (uryhere + params.edgeleft) * cmfactor;

      srotation = true;
    } else if ((long)floor(params.rotation + 0.5) == 0) {
      llxhere = params.xcorner;
      llyhere = params.ycorner;
      urxhere = params.xcorner + BaseAxis;
      uryhere = params.ycorner + BitAxis;


      llxhere = (llxhere - params.edgeleft) * cmfactor;
      llyhere = (llyhere - params.edgelow) * cmfactor;
      urxhere = (urxhere + params.edgeright) * cmfactor;
      uryhere = (uryhere + params.edgehigh) * cmfactor;

      srotation = true;
    } else if ((long)floor(params.rotation + 0.5) == 90) {
      llxhere = params.xcorner - BitAxis;
      llyhere = params.ycorner;
      urxhere = params.xcorner;
      uryhere = params.ycorner + BaseAxis;


      llxhere = (llxhere - params.edgehigh) * cmfactor;
      llyhere = (llyhere - params.edgeleft) * cmfactor;
      urxhere = (urxhere + params.edgelow) * cmfactor;
      uryhere = (uryhere + params.edgeright) * cmfactor;

      srotation = true;
    }

    /*





*/

    if (!srotation) {
      printf("* NOTE: The angle of rotation is not a multiple of 90 degrees,\n");
      printf("so bounding box is being set to page size.\n");
    }
  }

  if (!srotation) {
    llxhere = llx;
    llyhere = lly;
    urxhere = urx;
    uryhere = ury;
  }



  if (llxhere < llx) {
    showbar((long)warnwidth, warnchar);
    printf("* WARNING: left side goes off page\n");
    pagewarning = true;
  }
  if (llyhere < lly) {
    showbar((long)warnwidth, warnchar);
    printf("* WARNING: lower side goes off page\n");
    pagewarning = true;
  }
  if (urxhere > urx) {
    showbar((long)warnwidth, warnchar);
    printf("* WARNING: right side goes off page\n");
    pagewarning = true;
  }
  if (uryhere > ury) {
    showbar((long)warnwidth, warnchar);
    printf("* WARNING: upper side goes off page\n");
    pagewarning = true;
  }

  if (llxhere >= urxhere) {
    showbar((long)warnwidth, warnchar);
    printf("* WARNING: horizontal bounds of the page are inverted\n");
    pagewarning = true;
  }

  if (llyhere >= uryhere) {
    showbar((long)warnwidth, warnchar);
    printf("* WARNING: vertical bounds of the page are inverted\n");
    pagewarning = true;
  }

  if (pagewarning) {
    printf("* You have set weird bounds to the standard page limits.\n");
    printf("* These limits are defined in the module makelogo.const.\n");
    printf("* They are called llx, lly urx and ury, as in PostScript.\n");
    printf("* ll is the lower left corner of the display device.\n");
    printf("* ur is the upper right corner of the display device.\n");
    printf("* See: https://alum.mit.edu/www/toms/postscript.html\n");
    printf("* for information about PostScript.  That page describes\n");
    printf("* the postscript program printerarea.ps:\n");
    printf("* https://alum.mit.edu/www/toms/ftp/printerarea.ps\n");
    printf("* That program will report to you the correct values to use\n");
    printf("* for your printer.\n");
    showbar((long)warnwidth, warnchar);
  }


  fprintf(logo->f, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(logo->f, "%%%%Title: makelogo %4.2f\n", version);
  fprintf(logo->f, "%%%%Creator: Tom Schneider, toms@alum.mit.edu\n");
  /*
*/
  fprintf(logo->f, "%%%%BoundingBox: %5ld %5ld %5ld %5ld\n",
	  (long)floor(llxhere + 0.5), (long)floor(llyhere + 0.5),
	  (long)floor(urxhere + 0.5), (long)floor(uryhere + 0.5));
  fprintf(logo->f, "%%%%Pages: atend\n");
  fprintf(logo->f, "%%%%DocumentFonts:\n");
  fprintf(logo->f, "%%%%EndComments\n");

  fprintf(logo->f, "/llx %5.1f def\n", llxhere);
  fprintf(logo->f, "/lly %5.1f def\n", llyhere);
  fprintf(logo->f, "/urx %5.1f def\n", urxhere);
  fprintf(logo->f, "/ury %5.1f def\n", uryhere);
}

#undef warnchar
#undef warnwidth



Static Void themain(symvec, makelogop, colors, marks, wave, logo)
_TEXT *symvec, *makelogop, *colors, *marks, *wave, *logo;
{
  parameters params;

  long actualsymbols;
  /*
*/
  long b;
  boolean chilogo;
  double density;
  boolean dobeam, dostack;
  double fbl;
  long gaps = 0;
  Char letter;
  long l, linenumber;

  double markbase1, markbase2, markbits1, markbits2, markscale;
  Char marksymbol, marktype;
  Char marksorder = 'b';
  long nextmark = LONG_MAX;
  boolean waitfornextpiece = false;
  /*
*/
  /*
*/

  long nl;
  long pagenumber = 0;
  long position_, previousposition;
  double rsl, rsvar, rstotal, varhnb;
  boolean sitesused;
  /*

*/
  long stacknumber;
  /*

*/

  /*


*/

  Char mapbb;
  long mapbn;
  _TEXT TEMP;
  long FORLIM;

  printf("makelogo %4.2f\n", version);
  /*
*/
  if (*symvec->name != '\0') {
    if (symvec->f != NULL)
      symvec->f = freopen(symvec->name, "r", symvec->f);
    else
      symvec->f = fopen(symvec->name, "r");
  } else
    rewind(symvec->f);
  if (symvec->f == NULL)
    _EscIO2(FileNotFound, symvec->name);
  RESETBUF(symvec->f, Char);
  if (*makelogop->name != '\0') {
    if (makelogop->f != NULL)
      makelogop->f = freopen(makelogop->name, "r", makelogop->f);
    else
      makelogop->f = fopen(makelogop->name, "r");
  } else
    rewind(makelogop->f);
  if (makelogop->f == NULL)
    _EscIO2(FileNotFound, makelogop->name);
  RESETBUF(makelogop->f, Char);
  if (*colors->name != '\0') {
    if (colors->f != NULL)
      colors->f = freopen(colors->name, "r", colors->f);
    else
      colors->f = fopen(colors->name, "r");
  } else
    rewind(colors->f);
  if (colors->f == NULL)
    _EscIO2(FileNotFound, colors->name);
  RESETBUF(colors->f, Char);
  if (*marks->name != '\0') {
    if (marks->f != NULL)
      marks->f = freopen(marks->name, "r", marks->f);
    else
      marks->f = fopen(marks->name, "r");
  } else
    rewind(marks->f);
  if (marks->f == NULL)
    _EscIO2(FileNotFound, marks->name);
  RESETBUF(marks->f, Char);
  if (*wave->name != '\0') {
    if (wave->f != NULL)
      wave->f = freopen(wave->name, "r", wave->f);
    else
      wave->f = fopen(wave->name, "r");
  } else
    rewind(wave->f);
  if (wave->f == NULL)
    _EscIO2(FileNotFound, wave->name);
  RESETBUF(wave->f, Char);

  readparameters(makelogop, &params);

  params.cmperbit = params.barheight / fabs(params.barbits);

  if (*wave->name != '\0') {
    if (wave->f != NULL)
      wave->f = freopen(wave->name, "r", wave->f);
    else
      wave->f = fopen(wave->name, "r");
  } else
    rewind(wave->f);
  if (wave->f == NULL)
    _EscIO2(FileNotFound, wave->name);
  RESETBUF(wave->f, Char);
  if (BUFEOF(wave->f))
    params.wp = NULL;
  else
    readwaveparameters(wave, &params.wp);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  /*

*/


  warnifsitesused(symvec, &TEMP, &sitesused);

  if (BUFEOF(colors->f) && !params.outline) {
    printf("Half-White Ibeams are used because the symbols are solid black.\n");
    params.HalfWhiteIbeam = true;
  } else
    params.HalfWhiteIbeam = false;
  if (*logo->name != '\0') {
    if (logo->f != NULL)
      logo->f = freopen(logo->name, "w", logo->f);
    else
      logo->f = fopen(logo->name, "w");
  } else {
    if (logo->f != NULL) {

      rewind(logo->f);
    } else
      logo->f = tmpfile();
  }
  if (logo->f == NULL)
    _EscIO2(FileNotFound, logo->name);
  SETUPBUF(logo->f, Char);

  setboundingbox(logo, params);

  if (*symvec->name != '\0') {
    if (symvec->f != NULL)
      symvec->f = freopen(symvec->name, "r", symvec->f);
    else
      symvec->f = fopen(symvec->name, "r");
  } else
    rewind(symvec->f);
  if (symvec->f == NULL)
    _EscIO2(FileNotFound, symvec->name);
  RESETBUF(symvec->f, Char);
  if (!BUFEOF(symvec->f)) {
    while (P_peek(symvec->f) == '*') {
      fprintf(logo->f, "%% ");
      copyaline(symvec, logo);
    }
    fscanf(symvec->f, "%ld%*[^\n]", &params.symbols);
    getc(symvec->f);
    printf("* %ld symbols.\n", params.symbols);
  }
  startpostscript(logo, colors, symvec, params);

  getmark(marks, logo, &marksymbol, &marktype, &markbase1, &markbits1,
	  &markbase2, &markbits2, &markscale, &nextmark, &waitfornextpiece,
	  &marksorder);

  if (*symvec->name != '\0') {
    if (symvec->f != NULL)
      symvec->f = freopen(symvec->name, "r", symvec->f);
    else
      symvec->f = fopen(symvec->name, "r");
  } else
    rewind(symvec->f);
  if (symvec->f == NULL)
    _EscIO2(FileNotFound, symvec->name);
  RESETBUF(symvec->f, Char);
  if (BUFEOF(symvec->f)) {
    printf("* Alphabet Logo created because symvec is empty.\n");
    fprintf(logo->f, "%% Alphabet Logo\n");
    fprintf(logo->f, "startpage startline\n");
    if (params.barends == 'b' || params.barends == 'l' ||
	params.barends == 'r')
      fprintf(logo->f, "makebar\n");
    fprintf(logo->f, "%% find out how high the bar is in centimeters:\n");
    fprintf(logo->f, "/barincm barheight cmfactor div def\n");
    for (b = 'A'; b <= 'Z'; b++) {
      position_ = b + 1 - 'A';
      if (position_ == params.bar)
	fprintf(logo->f, "makebar\n");
      if (position_ >= params.lowest && position_ <= params.highest) {
	makenumber(logo, position_);
	if (!params.caps)
	  letter = _tolower(b);
	else
	  letter = (Char)b;
	fprintf(logo->f, "  gsave barincm (%c) numchar grestore shift\n",
		letter);
      }
    }
    if (params.barends == 'b' || params.barends == 'l' ||
	params.barends == 'r')
      fprintf(logo->f, "makebar\n");
    fprintf(logo->f, "endline endpage\n");
  } else {
    processsymvec(symvec);
    fscanf(symvec->f, "%*[^\n]");

    getc(symvec->f);
    rstotal = 0.0;
    varhnb = 0.0;
    linenumber = 0;
    l = params.lowest;
    position_ = params.lowest;
    chilogo = false;
    while (!BUFEOF(symvec->f)) {
      previousposition = position_;
      processsymvec(symvec);

      fscanf(symvec->f, "%ld%ld%lg%lg%*[^\n]", &position_, &nl, &rsl, &rsvar);
      getc(symvec->f);
      if (nl == 0)
	gaps++;


      /*


*/


      /*
*/
      actualsymbols = 0;
      for (b = 0; b < params.symbols; b++) {
	processsymvec(symvec);

	/*



*/
	fscanf(symvec->f, "%c%ld%*[^\n]", &mapbb, &mapbn);
	getc(symvec->f);
	if (mapbb == '\n')
	  mapbb = ' ';
	map[b].b = mapbb;
	map[b].n = mapbn;

	if (map[b].n < 0) {
	  if (!chilogo) {
	    chilogo = true;
	    printf("This is a Chilogo! - Upside down letterswill be produced\n");
	  }
	}
	if (chilogo)
	  actualsymbols += labs(map[b].n);
	else
	  actualsymbols += map[b].n;
      }
      if (!chilogo && nl != actualsymbols ||
	  chilogo && labs(nl - actualsymbols) > 1.5) {
	printf("\nTHE SYMVEC FILE IS BAD!\n");
	printf("At position %ld the sum of symbols found is %ld\n",
	       position_, actualsymbols);
	printf("But the noted number is %ld\n", nl);
	halt();
      }
      /*
*/


      if (position_ < params.lowest || position_ > params.highest)
	continue;

      rstotal += rsl;
      varhnb += rsvar;


      quicksort(1, (int)params.symbols);


      if (linenumber == 0) {
	pagenumber++;
	if (pagenumber == 1)
	  fprintf(logo->f, "%%%%EndProlog\n");
	else
	  printf("writing page %ld\n", pagenumber);

	fprintf(logo->f, "\n%%%%Page: %ld %ld\n", pagenumber, pagenumber);
	fprintf(logo->f, "startpage %% [\n");

	stacknumber = 0;
      }


      if (stacknumber == 0) {
	linenumber++;
	fprintf(logo->f, "startline %% line number %ld\n", linenumber);
      }
      stacknumber++;

      if (position_ == params.bar) {
	if (params.barends == 'b' || params.barends == 'l' ||
	    params.barends == 'r' || stacknumber != 1) {
	  fprintf(logo->f, "%% before coordinate %ld: make the bar\n",
		  position_);
	  fprintf(logo->f, "makebar\n");
	}
      }

      fprintf(logo->f, "%% at coordinate %ld\n", position_);
      makenumber(logo, position_);

      /*
*/

      /*



*/
      if ((long)floor(pnum * rsl * params.cmperbit + 0.5) > minimumStackSize)
	dostack = true;
      else
	dostack = false;

      /*
*/
      dobeam = false;
      if (rsvar >= 0.0) {
	if (params.barbits > 0.0)
	  dobeam = true;
      } else if (!sitesused) {
	printf("WARNING: a negative Rs variation was\n");
	printf("found in symvec, but the data are not from\n");
	printf(" the sites program.\n");
	printf("There may be a program or data error.\n");
      }

      if (params.formcontrol == 'R' || params.formcontrol == 'e') {
	dostack = true;
	dobeam = false;
      }


      if (dostack || dobeam)
	fprintf(logo->f, "gsave\n");
      if (dostack) {
	FORLIM = params.symbols;
	for (b = 0; b < FORLIM; b++) {
	  fbl = (double)map[b].n / nl;
	  params.showletter = true;
	  /*



*/
	  switch (params.formcontrol) {

	  case 'n':
	    params.stackheight = rsl;
	    params.symbolheight = fbl;
	    break;

	  case 'v':
	    params.stackheight = log((double)params.symbols) / log(2.0) - rsl;
	    params.symbolheight = fbl;
	    break;

	  case 'e':
	    params.stackheight = 1.0;
	    params.symbolheight = fbl * log((double)params.symbols) / log(2.0);
	    /*



*/
	    /*




*/
	    break;

	  case 'r':
	    params.stackheight = rsl;
	    params.symbolheight = (1 - fbl) / (params.symbols - 1);
	    params.showletter = (fbl > 0.0);
	    break;

	  case 'R':
	    params.stackheight = log((double)params.symbols) / log(2.0);
	    params.symbolheight = (1 - fbl) / (params.symbols - 1);
	    params.showletter = (fbl > 0.0);
	    break;
	  }
	  if (params.showletter)
	    makesymbol(logo, (double)((long)floor(pnum * params.symbolheight *
						  params.stackheight *
						  params.cmperbit + 0.5)) /
			     ((pnum < 0) ? -pnum : pnum), params.caps,
		       map[b].b);
/* p2c: makelogo.p, line 5349: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 5669 [251] */
	  else
	    makesymbol(logo, (double)((long)floor(pnum * params.symbolheight *
						  params.stackheight *
						  params.cmperbit + 0.5)) /
			     ((pnum < 0) ? -pnum : pnum), params.caps, '.');
/* p2c: makelogo.p, line 5349: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 5676 [251] */
	}

      }

      /*

*/
      if (dobeam && !sitesused)
	Ibeam(logo, sqrt(rsvar) * params.cmperbit);
      if (dostack || dobeam)
	fprintf(logo->f, "grestore\n");


      /*



*/
      if (nextmark < position_)
	nextmark = position_;
      markup(logo, params.charwidth, params.cmperbit, position_, marks,
	     &marksymbol, &marktype, &markbase1, &markbits1, &markbase2,
	     &markbits2, &markscale, &nextmark, &waitfornextpiece,
	     &marksorder);

      fprintf(logo->f, "shift\n");
      if (stacknumber == params.stacksperline || position_ == params.highest)
	docosinewave(logo, params.wp, params.charwidth, params.cmperbit,
		     params.barheight, stacknumber, position_);

      if (linenumber == params.linesperpage &&
	  stacknumber == params.stacksperline) {
	if (params.highest == params.bar - 1) {
	  if (params.barends == 'b' || params.barends == 'l' ||
	      params.barends == 'r') {
	    fprintf(logo->f,
	      "%% bar at end of logo: make the bar before position %ld\n",
	      position_);
	    fprintf(logo->f, "makebar\n");
	  }

	}

	fprintf(logo->f, "endline\n");
	stacknumber = 0;
	fprintf(logo->f, "endpage %% ]\n");
	linenumber = 0;
      }


      if (stacknumber == params.stacksperline) {
	fprintf(logo->f, "endline\n");
	stacknumber = 0;
      }


      /*
*/
      while (l != position_ && l <= params.highest) {
	printf("\nWARNING: symvec does not contain data for position %ld\n",
	       l);
	if (l == 0 && params.lowest < 0 && params.highest > 0 &&
	    position_ == 1 && previousposition == -1) {
	  printf("It seems that you don't have a zero coordinate.\n");
	  printf(
	    "\"... the invention, probably by the Hindus, of the digit zero has been\n");
	  printf(
	    "described as one of the greatest importance in the history of mathematics.\"\n");
	  printf("--- Encyclopaedia Britannica (1:1175, 1982)\n\n");
	  printf("Please use a zero!\n\n");
	  printf("A zero coordinate is necessary for\n");
	  printf("identifying the location of a binding site\n");
	  printf("with a walker (NAR 25:4408-4415 (1997).\n");
	  printf("If you remove the zero it will make life\n");
	  printf("much more difficult later.\n\n");
	  if (*logo->name != '\0') {
	    if (logo->f != NULL)
	      logo->f = freopen(logo->name, "w", logo->f);
	    else
	      logo->f = fopen(logo->name, "w");
	  } else {
	    if (logo->f != NULL)
	      rewind(logo->f);
	    else
	      logo->f = tmpfile();
	  }
	  if (logo->f == NULL)
	    _EscIO2(FileNotFound, logo->name);
	  SETUPBUF(logo->f, Char);
	  halt();
	}

	putchar('\n');
	l++;
      }
      l++;
    }

    if (position_ < params.highest) {
      printf("\nWARNING: Highest position requested was not found in symvec\n\n");

      if (stacknumber != 0)
	docosinewave(logo, params.wp, params.charwidth, params.cmperbit,
		     params.barheight, stacknumber, position_);

    }


    if (stacknumber != 0)
      fprintf(logo->f, "endline\n");
    if (linenumber != 0)
      fprintf(logo->f, "endpage %% ]\n");

    density = rstotal / ((params.highest - params.lowest - gaps + 1) * 2);
    summary(logo, '%', rstotal, varhnb, params.lowest, params.highest, gaps,
	    density);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    summary(&TEMP, ' ', rstotal, varhnb, params.lowest, params.highest, gaps,
	    density);

  }
  /*




*/

  fprintf(logo->f, "%%%%Trailer\n");
  fprintf(logo->f, "%%%%Pages: %ld\n", pagenumber);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  logo.f = NULL;
  strcpy(logo.name, "logo");
  wave.f = NULL;
  strcpy(wave.name, "wave");
  marks.f = NULL;
  strcpy(marks.name, "marks");
  colors.f = NULL;
  strcpy(colors.name, "colors");
  makelogop.f = NULL;
  strcpy(makelogop.name, "makelogop");
  symvec.f = NULL;
  strcpy(symvec.name, "symvec");
  themain(&symvec, &makelogop, &colors, &marks, &wave, &logo);
_L1:
  if (symvec.f != NULL)
    fclose(symvec.f);
  if (makelogop.f != NULL)
    fclose(makelogop.f);
  if (colors.f != NULL)
    fclose(colors.f);
  if (marks.f != NULL)
    fclose(marks.f);
  if (wave.f != NULL)
    fclose(wave.f);
  if (logo.f != NULL)
    fclose(logo.f);
  exit(EXIT_SUCCESS);
}



/* End. */
