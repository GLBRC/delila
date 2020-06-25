/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "prgmod.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*








*/



#define version         5.67
/*























































































































































*/



/*


































*/


#define superdebug      false
/*
*/
#define globaldebug     true
/*
*/



/*

*/






























#define maxstring       2000



#define fillermax       50




#define defaultllx      10.08
#define defaultlly      8.91
#define defaulturx      588.06
#define defaultury      779.85
/*





*/

#define pwid            8
#define pdec            5



#define minribl         (-10001)
#define maxribl         10001
#define defnegativeinfinity  (-1000)
/*
*/



#define infofield       10
#define infodecim       6

#define nfield          8







typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;



/*


*/
typedef Char filler[fillermax];



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;



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





typedef struct petaltype {

  long el;
  Char c_;

  double eh, es, eb;

  double fh, fs, fb;

  double spare;
} petaltype;

typedef struct definetype {
  /*

*/
  stringDelila nametag, background, negparts, posparts;
  double locations[maxstring];
  /*
*/


  long marks;
  double min, max;
  long definitionnumber;
  ribltype *matrix;
  /*



*/
  double RiboundFeature, ZboundFeature, PboundFeature;
  petaltype petal;
  boolean isgap;
  /*
*/
  boolean istotal;
  /*
*/
  boolean ismulti;
  /*
*/
  struct definetype *next;
} definetype;


typedef struct featurecarrier {
  /*
*/
  struct featuretype *afeature;
  struct featurecarrier *next;
} featurecarrier;

typedef struct featuretype {
  /*




*/
  stringDelila id;
  long featurenumber;
  double coordinate;
  long orientation;
  stringDelila nametag, othertag;

  definetype *definition;

  /*
*/
  double Ri, Z, probability;

  /*

*/
  Char Tparam;
  /*
*/
  double Aparam, Bparam, Cparam, Dparam, relcoo;
  /*

















*/

  double evencoordinate;
  /*

*/


  boolean unsatisfied;
  /*
*/
  double fromrange, torange;
  long desiredline;
  /*

*/
  /*







*/
  featurecarrier *others, *mothers;

  /*
*/
  struct featuretype *duplicateprevious;
  /*



*/

  boolean usedInGroup;

  struct featuretype *next;
} featuretype;



/*

*/

typedef short maptype[maxstring];


Static _TEXT list;

Static stringDelila buffer;
Static boolean newline;
Static stringDelila atoken;
Static long ourplace;
Static maptype map;
Static Char control;
Static boolean gotten;
Static long spaces, first, last, lines;
Static double radius;



Static long groupnumber;


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



Static boolean between(a_, b, c_)
double a_, b, c_;
{
  return (a_ <= b && b <= c_);
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


/*













*/




Static boolean complines(a_, b)
_TEXT *a_, *b;
{
  /*







*/
  boolean Result;
  Char chara, charb;
  boolean done = false;

  if (BUFEOF(a_->f) & BUFEOF(b->f))
    return true;
  if (BUFEOF(a_->f) | BUFEOF(b->f))
    return false;
  while (!done) {
    if (P_eoln(a_->f))
      chara = ' ';
    else {
      chara = getc(a_->f);
      if (chara == '\n')
	chara = ' ';
    }

    if (P_eoln(b->f))
      charb = ' ';
    else {
      charb = getc(b->f);
      if (charb == '\n')
	charb = ' ';
    }

    if (P_eoln(a_->f) & P_eoln(b->f)) {
      done = true;
      Result = true;
    } else if (chara != charb) {
      done = true;
      Result = false;
    }
  }
  fscanf(a_->f, "%*[^\n]");
  getc(a_->f);
  fscanf(b->f, "%*[^\n]");
  getc(b->f);
  return Result;
}







Static Void plural(thefile, number)
_TEXT *thefile;
long number;
{
  if (number != 1)
    putc('s', thefile->f);
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



Static Void copyfile(fin, fout)
_TEXT *fin, *fout;
{
  while (!BUFEOF(fin->f))
    copyaline(fin, fout);
}


/* Local variables for numberdigit: */
struct LOC_numberdigit {
  long number, place, myabsolute;
  Char acharacter;
} ;

Local Void digit(LINK)
struct LOC_numberdigit *LINK;
{
  long tenplace, z, d;

  tenplace = LINK->place * 10;
  z = LINK->myabsolute - LINK->myabsolute / tenplace * tenplace;
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







/*























*/



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
  V.myabsolute = labs(V.number);
  if (V.myabsolute < V.place / 10) {
    V.acharacter = ' ';
    return V.acharacter;
  }
  if (V.myabsolute >= V.place)
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



Static long firstlastmax(firstnumber, lastnumber)
long firstnumber, lastnumber;
{
  /*
*/
  long firstlines, lastlines;

  firstlines = numbersize(firstnumber);
  if (firstnumber > 0)
    firstlines++;
  lastlines = numbersize(lastnumber);
  if (lastnumber > 0)
    lastlines++;
  if (firstlines > lastlines)
    return firstlines;
  else
    return lastlines;
}


Static Void numberbar(afile, spaces, firstnumber, lastnumber, linesused)
_TEXT *afile;
long spaces, firstnumber, lastnumber, *linesused;
{
  /*
*/
  long logplace, number, spacecount;

  /*






*/
  *linesused = firstlastmax(firstnumber, lastnumber);
  /*






*/
  for (logplace = *linesused - 1; logplace >= 0; logplace--) {
    for (spacecount = 1; spacecount <= spaces; spacecount++)
      putc(' ', afile->f);
    for (number = firstnumber; number <= lastnumber; number++)
      fputc(numberdigit(number, logplace), afile->f);
    putc('\n', afile->f);
  }
}


/* Local variables for pluckdigit: */
struct LOC_pluckdigit {
  long place, myabsolute;
  Char acharacter;
} ;

Local Void digit_(LINK)
struct LOC_pluckdigit *LINK;
{
  long tenplace, z, d;

  tenplace = LINK->place * 10;
  z = LINK->myabsolute - LINK->myabsolute / tenplace * tenplace;
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



Static Char pluckdigit(number, logplace)
long number, logplace;
{
  /*








*/
  struct LOC_pluckdigit V;
  long count;

  V.place = 1;
  for (count = 1; count <= logplace; count++)
    V.place *= 10;
  if (number == 0) {
    V.acharacter = '0';
    return V.acharacter;
  }
  V.myabsolute = number;
  if (V.myabsolute >= V.place)
    digit_(&V);
  else
    V.acharacter = '0';
  return V.acharacter;
}








/*












*/




Static Void prompt(afile)
_TEXT *afile;
{
  /*



*/
  if (BUFEOF(afile->f)) {
    if (*afile->name != '\0') {
      if (afile->f != NULL)
	afile->f = freopen(afile->name, "r", afile->f);
      else
	afile->f = fopen(afile->name, "r");
    } else
      rewind(afile->f);
    if (afile->f == NULL)
      _EscIO2(FileNotFound, afile->name);
    RESETBUF(afile->f, Char);
  }
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
}



Static Void readchar(afile, ch)
_TEXT *afile;
Char *ch;
{
  if (BUFEOF(afile->f))
    prompt(afile);
  *ch = getc(afile->f);

  if (*ch == '\n')
    *ch = ' ';
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



Static Void readstring(afile, line)
_TEXT *afile;
stringDelila *line;
{
  long index;
  Char cha;
  boolean done, acceptable;

  do {
    clearstring(line);
    prompt(afile);
    index = 0;
    while (!P_eoln(afile->f) && index < maxstring) {
      index++;
      readchar(afile, &cha);
      line->letters[index-1] = cha;
    }
    if (!P_eoln(afile->f)) {
      printf("type lines shorter than %ld characters.  please retype the line...\n",
	     maxstring + 1L);
      acceptable = false;
    } else
      acceptable = true;
  } while (!acceptable);

  line->length = index;
  if (line->length > 0) {
    done = false;
    do {
      /*

*/
      if (line->letters[line->length - 1] == ' ')
	line->length--;
      else
	done = true;

      if (line->length == 0)
	done = true;
    } while (!done);
  }

  if (line->length > 0)
    line->current = 1;
  else
    line->current = 0;
}


/* Local variables for figurestring: */
struct LOC_figurestring {
  stringDelila *line;
  long power;
} ;

Local long figureinteger(first, last, LINK)
long first, last;
struct LOC_figurestring *LINK;
{
  long i;
  long sum = 0;
  long increment;

  LINK->power = 1;
  for (i = last - 1; i >= first - 1; i--) {
    switch (LINK->line->letters[i]) {

    case '0':
      increment = 0;
      break;

    case '1':
      increment = 1;
      break;

    case '2':
      increment = 2;
      break;

    case '3':
      increment = 3;
      break;

    case '4':
      increment = 4;
      break;

    case '5':
      increment = 5;
      break;

    case '6':
      increment = 6;
      break;

    case '7':
      increment = 7;
      break;

    case '8':
      increment = 8;
      break;

    case '9':
      increment = 9;
      break;
    }
    sum += LINK->power * increment;
    LINK->power *= 10;
  }
  return sum;
}



Static Void figurestring(line_, first, last, whzat, c_, i, r)
stringDelila *line_;
long *first, *last;
Char *whzat, *c_;
long *i;
double *r;
{
  /*













*/
  struct LOC_figurestring V;
  long numbers[3];
  long sign, numberstart;
  /*
*/
  long point = 0;

  long l;
  stringDelila *WITH;
  long FORLIM;


  V.line = line_;
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
  *whzat = '.';


  WITH = V.line;

  if (WITH->length == 0 || WITH->current < 1 || WITH->current > WITH->length)
    *whzat = ' ';
  else {
    *first = V.line->current;
    while (V.line->letters[*first - 1] == ' ' && *first < V.line->length)
      (*first)++;
    if (*first == V.line->length && V.line->letters[*first - 1] == ' ')
      *whzat = ' ';
  }

  if (*whzat == ' ')
    return;
  *last = *first;
  while (V.line->letters[*last - 1] != ' ' && *last < V.line->length)
    (*last)++;
  if (V.line->letters[*last - 1] == ' ')
    (*last)--;



  *c_ = V.line->letters[*first - 1];
  if (P_inset(*c_, numbers) || *c_ == '-' || *c_ == '+') {
    if (*c_ == '-' || *c_ == '+') {
      switch (*c_) {

      case '+':
	sign = 1;
	break;

      case '-':
	sign = -1;
	break;
      }
      numberstart = *first + 1;
    } else {
      sign = 1;
      numberstart = *first;
    }

    *whzat = 'i';
    FORLIM = *last;
    for (l = numberstart; l <= FORLIM; l++) {
      if (!P_inset(V.line->letters[l-1], numbers)) {
	if (V.line->letters[l-1] == '.') {
	  if (*whzat == 'i') {
	    *whzat = 'r';
	    point = l;
	  } else
	    *whzat = 'g';
	} else
	  *whzat = 'g';
      }
    }




    if (*whzat == 'r' && point == *last)
      *whzat = 'i';

    if (*whzat == 'i') {
      if (point == *last)
	*i = sign * figureinteger(numberstart, *last - 1, &V);
      else
	*i = sign * figureinteger(numberstart, *last, &V);
      *r = *i;
    } else if (*whzat == 'r') {
      *i = figureinteger(numberstart, point - 1, &V);
      *r = sign * (*i + (double)figureinteger(point + 1, *last, &V) / V.power);
      *i *= sign;
    }
  } else
    *whzat = 'c';


  V.line->current = *last + 1;
}


/* Local variables for nostring: */
struct LOC_nostring {
  stringDelila *buffer;
  boolean answer;
} ;

Local Void kill(LINK)
struct LOC_nostring *LINK;
{
  LINK->answer = true;
  clearstring(LINK->buffer);
}




/*




























*/



Static boolean nostring(buffer_)
stringDelila *buffer_;
{
  /*

*/
  struct LOC_nostring V;
  stringDelila *WITH;

  V.buffer = buffer_;
  WITH = V.buffer;
  if (WITH->length <= 0) {
    kill(&V);
    return V.answer;
  }
  if (WITH->length < maxstring) {
    while (WITH->letters[WITH->current - 1] == ' ' &&
	   WITH->current < WITH->length)
      WITH->current++;
  }

  if (WITH->current > maxstring) {
    kill(&V);
    return V.answer;
  }
  if (WITH->letters[WITH->current - 1] == ' ')
    kill(&V);
  else
    V.answer = false;
  return V.answer;
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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}



Static Void writeNOTABstring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  /*
*/
  stringDelila notabs;

  copystring(*s, &notabs);
  detabstring(&notabs);
  writestring(tofile, &notabs);
}



Static Void flagstring(afile, buffer)
_TEXT *afile;
stringDelila *buffer;
{
  buffer->length = buffer->current;
  writestring(afile, buffer);
  fprintf(afile->f, "? ");
  clearstring(buffer);
}



Static Void getinteger(afile, buffer, int_, gotten)
_TEXT *afile;
stringDelila *buffer;
long *int_;
boolean *gotten;
{
  long first, last;
  Char what, cha;
  double rea;
  _TEXT TEMP;

  if (buffer->length == 0) {
    *gotten = false;
    readstring(afile, buffer);
    return;
  }
  figurestring(buffer, &first, &last, &what, &cha, int_, &rea);

  if (what == 'i') {
    *gotten = true;
    return;
  }
  TEMP.f = stdout;
  *TEMP.name = '\0';
  flagstring(&TEMP, buffer);
  printf(" please type an integer\n");
  *gotten = false;
}



Static Void getreal(afile, buffer, rea, gotten)
_TEXT *afile;
stringDelila *buffer;
double *rea;
boolean *gotten;
{
  /*
*/
  long first, last;
  Char what, cha;
  long int_;
  _TEXT TEMP;

  if (buffer->length == 0) {
    *gotten = false;
    readstring(afile, buffer);
  } else {
    figurestring(buffer, &first, &last, &what, &cha, &int_, rea);

    if (what != 'i' && what != 'r') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      flagstring(&TEMP, buffer);
      printf(" please type a real number\n");
      *gotten = false;
    } else
      *gotten = true;
  }


  if (what == 'i')
    *rea = int_;
}



Static Void getchar_(afile, buffer, cha, gotten)
_TEXT *afile;
stringDelila *buffer;
Char *cha;
boolean *gotten;
{
  /*

*/
  long first, last;
  Char what;
  long int_;
  double rea;

  if (buffer->length == 0) {
    *gotten = false;
    readstring(afile, buffer);
  } else {
    figurestring(buffer, &first, &last, &what, cha, &int_, &rea);
    *gotten = (what != ' ');
  }
}



Static Void token(buffer, atoken, gotten)
stringDelila *buffer, *atoken;
boolean *gotten;
{
  long first, last;
  Char what, cha;
  long int_;
  double rea;
  long index;

  figurestring(buffer, &first, &last, &what, &cha, &int_, &rea);

  if (what == ' ') {
    *gotten = false;
    return;
  }
  clearstring(atoken);
  for (index = first; index <= last; index++)
    atoken->letters[index - first] = buffer->letters[index-1];
  atoken->length = last - first + 1;
  atoken->current = 1;

  *gotten = true;
}



Static Void gettoken(afile, buffer, atoken, gotten)
_TEXT *afile;
stringDelila *buffer, *atoken;
boolean *gotten;
{
  if (buffer->length == 0) {
    *gotten = false;
    readstring(afile, buffer);
  } else
    token(buffer, atoken, gotten);
}


#define tabcharacter    9



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
  emptystring(buffer);
  if (BUFEOF(afile->f)) {
    *gotten = false;
    return;
  }
  while (!P_eoln(afile->f) && index < maxstring && !done) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
    if (buffer->letters[index-1] == ' ' ||
	buffer->letters[index-1] == tabcharacter) {
      done = true;
      index--;
    }
  }
  buffer->length = index;
  buffer->current = 1;
  *gotten = true;
}

#undef tabcharacter



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




Static Void skipstringblanks(line)
stringDelila *line;
{
  while (line->letters[line->current - 1] == ' ' && line->current < maxstring)
    line->current++;
}


Static Void skipstringnonblanks(line)
stringDelila *line;
{
  while (line->letters[line->current - 1] != ' ' && line->current < maxstring)
    line->current++;
}


Static Void skipstringcolumn(line)
stringDelila *line;
{
  skipstringblanks(line);
  skipstringnonblanks(line);
}


/*

*/

#define debug           false


Static Void stringcolumn(line, column, datum)
stringDelila line;
long column;
stringDelila *datum;
{
  /*
*/
  long c_;
  long columnnumber = 0;
  boolean done = false;
  long positionbeg, positionend;
  _TEXT TEMP;
  long FORLIM;

  if (debug) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &line);
    putchar('\n');
  }
  while (!done) {
    if (columnnumber == column || line.current >= line.length) {
      done = true;
      break;
    }
    skipstringblanks(&line);
    positionbeg = line.current;
    if (debug)
      printf("%*c %ld", (int)line.current, '|', line.current);
    if (debug)
      printf("\"%c\"", line.letters[positionbeg-1]);
    if (debug)
      putchar('\n');

    skipstringnonblanks(&line);
    positionend = line.current - 1;
    if (debug)
      printf("%*c %ld", (int)line.current, '|', line.current);
    if (debug)
      printf("\"%c\"", line.letters[positionend-1]);
    if (debug)
      putchar('\n');

    columnnumber++;
  }
  clearstring(datum);
  if (columnnumber == column) {
    datum->length = positionend - positionbeg + 1;
    FORLIM = datum->length;
    for (c_ = 1; c_ <= FORLIM; c_++) {
      if (debug)
	putchar(line.letters[positionbeg + c_ - 2]);
      datum->letters[c_-1] = line.letters[positionbeg + c_ - 2];
    }
  }
  if (debug)
    putchar('\n');
  /*

*/
}

#undef debug




Static Void stringtoreal(datum, rea, gotten)
stringDelila datum;
double *rea;
boolean *gotten;
{
  /*
*/
  long first, last;
  Char what, cha;
  long int_;

  if (datum.length == 0) {
    *gotten = false;
    *rea = LONG_MAX;
    return;
  }
  datum.current = 1;
  figurestring(&datum, &first, &last, &what, &cha, &int_, rea);
  if (what != 'i' && what != 'r') {
    *gotten = false;
    return;
  }
  *gotten = true;
  if (what == 'i')
    *rea = int_;
}







Static Void parsestring(a_, tokens)
stringDelila a_, **tokens;
{
  /*



*/
  long ia = 1;
  long ib;
  long tokennumber = 0;
  stringDelila *token;

  if (*tokens == NULL)
    *tokens = (stringDelila *)Malloc(sizeof(stringDelila));
  else {
    disposestring(tokens);
    *tokens = (stringDelila *)Malloc(sizeof(stringDelila));
  }
  clearstring(*tokens);
  token = *tokens;
  while (ia <= a_.length) {
    while (isblankDelila(a_.letters[ia-1]) && ia < a_.length)
      ia++;
    if (ia > a_.length)
      break;
    ib = 1;
    tokennumber++;
    token->length = 0;
    while (!isblankDelila(a_.letters[ia-1]) && ia <= a_.length) {
      token->length++;
      token->letters[ib-1] = a_.letters[ia-1];
      ib++;
      ia++;
    }
    token->next = (stringDelila *)Malloc(sizeof(stringDelila));
    emptystring(token->next);
    token->next->previous = token;
    token = token->next;
  }


  if (token->length != 0) {
    printf("parsestring end token error\n");
    halt();
    return;
  }
  if (token->previous != NULL) {
    token->previous->next = NULL;
    Free(token);
  }
}



Static Void stringtointeger(token, result)
stringDelila token;
long *result;
{
  /*
*/
  long i;
  long power = 1, sum = 0;
  long increment;

  for (i = token.length - 1; i >= 0; i--) {
    switch (token.letters[i]) {

    case '0':
      increment = 0;
      break;

    case '1':
      increment = 1;
      break;

    case '2':
      increment = 2;
      break;

    case '3':
      increment = 3;
      break;

    case '4':
      increment = 4;
      break;

    case '5':
      increment = 5;
      break;

    case '6':
      increment = 6;
      break;

    case '7':
      increment = 7;
      break;

    case '8':
      increment = 8;
      break;

    case '9':
      increment = 9;
      break;
    }
    sum += power * increment;
    power *= 10;
  }
  *result = sum;
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



Static boolean lessstring(a_, b)
stringDelila a_, b;
{
  boolean Result;
  boolean done = false;
  long l = 0;


  do {
    l++;

    if (l > a_.length) {
      done = true;
      Result = false;
    } else if (b.letters[l-1] != a_.letters[l-1]) {
      done = true;
      Result = (b.letters[l-1] > a_.letters[l-1]);
    }
  } while (!done);
  return Result;
}



Static Void makenumber(name, number, found)
stringDelila name;
long *number;
boolean *found;
{
  /*
*/
  long l;

  *found = false;
  *number = 0;
  for (l = 0; l < name.length; l++) {
    if (name.letters[l] == '9' || name.letters[l] == '8' ||
	name.letters[l] == '7' || name.letters[l] == '6' ||
	name.letters[l] == '5' || name.letters[l] == '4' ||
	name.letters[l] == '3' || name.letters[l] == '2' ||
	name.letters[l] == '1' || name.letters[l] == '0') {
      *found = true;
      *number *= 10;
      switch (name.letters[l]) {

      case '0':
	/* blank case */
	break;

      case '1':
	(*number)++;
	break;

      case '2':
	*number += 2;
	break;

      case '3':
	*number += 3;
	break;

      case '4':
	*number += 4;
	break;

      case '5':
	*number += 5;
	break;

      case '6':
	*number += 6;
	break;

      case '7':
	*number += 7;
	break;

      case '8':
	*number += 8;
	break;

      case '9':
	*number += 9;
	break;
      }
    }
  }
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



Static Void attachstring(a_, b)
stringDelila *a_, b;
{
  stringDelila hold;

  copystring(*a_, &hold);
  concatstring(&hold, &b, a_);
}



Static Void attachcharacterstring(c_, a_)
Char c_;
stringDelila *a_;
{
  if (a_->length == maxstring) {
    printf("attachcharacterstring: maxstring exceeded\n");
    halt();
  }
  a_->length++;
  a_->letters[a_->length - 1] = c_;
}



Static Void joinstring(parts, target)
stringDelila *parts, *target;
{
  /*

*/
  stringDelila *p = parts;

  while (p != NULL) {
    attachstring(target, *p);
    p = p->next;
  }
}



Static Void numberstring(anumber, astring)
long anumber;
stringDelila *astring;
{
  /*
*/
  long d, size;

  clearstring(astring);
  size = numbersize(anumber) - 1;
  for (d = 0; d <= size; d++)
    astring->letters[size - d] = numberdigit(anumber, d);
  astring->length = size + 1;
}



Static Void onenumberstringtest(f, i)
_TEXT *f;
long i;
{
  /*
*/
  stringDelila n;
  long size;

  clearstring(&n);
  fprintf(f->f, "i= %6ld", i);
  size = numbersize(i);
  fprintf(f->f, " (size = %ld) ", size);
  fprintf(f->f, " n = \"");
  numberstring(i, &n);
  writestring(f, &n);
  fprintf(f->f, "\"\n");
}


#define range           15


Static Void numberstringtest(f)
_TEXT *f;
{
  /*

*/
  long i;

  fprintf(f->f, "* nu %4.2f\n", version);
  onenumberstringtest(f, -10000L);
  onenumberstringtest(f, -1000L);
  onenumberstringtest(f, -100L);
  for (i = -range; i <= range; i++)
    onenumberstringtest(f, i);
  onenumberstringtest(f, 100L);
  onenumberstringtest(f, 1000L);
  onenumberstringtest(f, 10000L);
}

#undef range



Static Void numberrealstring(s, n, thewidth, thedecimal)
stringDelila *s;
double n;
long thewidth, thedecimal;
{
  /*









*/
  /*


*/
  long decimalplace, d, digits;
  /*


*/
  long i, power;
  boolean minus;
  long newwidth, sign;
  Char symbol;
  long FORLIM;
  _TEXT TEMP;

  /*
*/
  emptystring(s);
  if (thewidth > 0) {
    power = 1;
    for (i = 1; i <= thedecimal; i++)
      power *= 10;
    digits = (long)floor(power * fabs(n) + 0.5);
    if (thedecimal > 0)
      decimalplace = 1;
    else
      decimalplace = 0;
    if (n < 0)
      sign = 1;
    else
      sign = 0;
    newwidth = numbersize(digits) + thedecimal + sign;

    /*


*/
    if (fabs(n) < 1.0)
      newwidth++;


    if (thewidth > newwidth)
      newwidth = thewidth;

    /*













*/

    for (d = newwidth - decimalplace - 1; d >= 0; d--) {
      symbol = numberdigit(digits, d);
      /*





*/
      if (symbol == '-')
	symbol = '0';
      if (symbol == '+')
	symbol = '0';
      if (symbol == ' ')
	symbol = '0';
      s->length++;
      s->letters[s->length - 1] = symbol;
      if (d == thedecimal) {
	if (thedecimal > 0) {
	  /*




*/
	  s->length++;
	  s->letters[s->length - 1] = '.';
	}
      }
    }
    /*



*/


    s->current = 1;
    while (s->letters[s->current - 1] == '0' && s->current < s->length) {
      if (s->letters[s->current] != '.')
	s->letters[s->current - 1] = ' ';
      s->current++;
      /*



*/

    }

    if (n < 0) {
      s->current = 1;
      minus = false;
      while (s->letters[s->current - 1] == ' ') {
	if (s->letters[s->current] != ' ') {
	  s->letters[s->current - 1] = '-';
	  minus = true;
	}
	s->current++;
	if (s->current < maxstring)
	  continue;
	printf("numberrealstring:");
	printf(" cannot place minus sign in string\n");
	printf("placing number at %ld\n", s->current);
	printf("the number is n =%*.*f\n", (int)newwidth, (int)thedecimal, n);
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, s);
	halt();
      }
      if (!minus) {
	printf("numberrealstring ERROR: missing minus sign for number %*.*f\n",
	       (int)newwidth, (int)thedecimal, n);
	printf("Increase one of the number widths.\n");
	halt();
      }
    }
  }

  /*

*/
  if (s->length <= thewidth)
    return;

  /*





*/

  while (s->letters[0] == ' ') {
    FORLIM = s->length;
    for (i = 1; i < FORLIM; i++)
      s->letters[i-1] = s->letters[i];
    s->length--;
  }
}


Local Void test(s, r)
stringDelila *s;
double r;
{
  _TEXT TEMP;

  numberrealstring(s, r, 1L, 1L);
  printf("for number = \"%8.5f\"", r);
  printf(" = \"%1.1f\",", r);
  printf(" string=\"");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, s);
  printf("\"\n");
}



Static Void testnumberrealstring(afile)
_TEXT *afile;
{
  /*

*/
  stringDelila s;

  printf("testnumberrealstring \n");

  clearstring(&s);
  test(&s, 1.0);
  test(&s, 1.7);
  test(&s, 1.8);
  test(&s, 1.9);
  test(&s, 1.99);
  test(&s, 2.0);
  test(&s, 0.975901);
  test(&s, 0.97);
  printf("Special cases\n\n");
  test(&s, -3.97);
  test(&s, -2.97);
  test(&s, -1.97);
  test(&s, -0.97);
  test(&s, 0.97);
  test(&s, 1.97);
  test(&s, 2.97);
  test(&s, 3.97);
  printf("It is a special case!\n\n");
  test(&s, -3.07);
  test(&s, -2.07);
  test(&s, -1.07);
  test(&s, -0.07);
  test(&s, 0.07);
  test(&s, 1.07);
  test(&s, 2.07);
  test(&s, 3.07);
  printf("It is a special case!\n");
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



Static Void decapstring(t_)
stringDelila *t_;
{
  long i, FORLIM;

  FORLIM = t_->length;
  for (i = 0; i < FORLIM; i++)
    t_->letters[i] = decapitalize(t_->letters[i]);
}



Static Void instring(buffer, bufferstart, tag, gottag)
stringDelila buffer;
long bufferstart;
stringDelila tag;
long *gottag;
{
  /*
*/
  long b = bufferstart, t_ = 1;
  boolean tracking = false;

  if (bufferstart < 1) {
    printf("instring: bufferstart must be positive, it is %ld\n", bufferstart);
    halt();
  }
  *gottag = 0;
  while (b <= buffer.length && *gottag == 0) {
    if (decapitalize(buffer.letters[b-1]) == tag.letters[t_-1])
      tracking = true;
    if (tracking) {
      if (decapitalize(buffer.letters[b-1]) == tag.letters[t_-1]) {
	if (t_ == tag.length)
	  *gottag = b;
      }
      t_++;
    }
    b++;
  }
}



Static boolean alphabetic(c_)
Char *c_;
{
  long n;

  n = *c_;
  if (n >= 'A' && n <= 'Z' || n >= 'a' && n <= 'z')
    return true;
  else
    return false;
}



Static Void whatisthere(t_, charsok)
stringDelila t_;
boolean *charsok;
{
  /*
*/
  long i, FORLIM;

  *charsok = true;
  FORLIM = t_.length;

  /*
*/
  for (i = 0; i < FORLIM; i++)
    *charsok &= alphabetic(&t_.letters[i]);
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








Static Void setdemo(buffer, map)
stringDelila buffer;
short *map;
{
  long index;

  for (index = 1; index <= buffer.length; index++)
    map[index-1] = index;
}


Static Void print(tofile, buffer, map)
_TEXT *tofile;
stringDelila buffer;
short *map;
{
  long index;

  for (index = 0; index < buffer.length; index++)
    putc(buffer.letters[map[index] - 1], tofile->f);
  putc('\n', tofile->f);
}


Static boolean lessthan(a_, b)
long a_, b;
{
  return (buffer.letters[map[a_-1] - 1] < buffer.letters[map[b-1] - 1]);
}


Static Void swap_(a_, b)
long a_, b;
{
  long hold;

  hold = map[a_-1];
  map[a_-1] = map[b-1];
  map[b-1] = hold;

}





Static Void quicksort(left, right)
long left, right;
{
  /*












*/
  long lower = left;
  long upper, center;

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



Static Void writeadefinition(afile, d)
_TEXT *afile;
definetype *d;
{
  long i, FORLIM;

  fprintf(afile->f, "define");
  putc(' ', afile->f);
  writequotestring(afile, d->nametag);
  putc(' ', afile->f);
  writequotestring(afile, d->background);
  putc(' ', afile->f);
  writequotestring(afile, d->negparts);
  putc(' ', afile->f);
  writequotestring(afile, d->posparts);
  FORLIM = d->marks;
  for (i = 0; i < FORLIM; i++)
    fprintf(afile->f, " %1.1f", d->locations[i]);
  putc('\n', afile->f);
  /*





*/
}


#define pwid_           8
#define pdec_           5



Static Void showadefinition(afile, d)
_TEXT *afile;
definetype *d;
{
  long i, FORLIM;
  petaltype *WITH1;

  fprintf(afile->f, "DEFINITION: \n");
  fprintf(afile->f, "nametag: ");
  writequotestring(afile, d->nametag);
  fprintf(afile->f, "\nbackground: ");

  writequotestring(afile, d->background);
  fprintf(afile->f, "\nnegative parts: ");

  writequotestring(afile, d->negparts);
  fprintf(afile->f, "\npositive parts: ");

  writequotestring(afile, d->posparts);
  fprintf(afile->f, "\nlocations: ");

  FORLIM = d->marks;
  for (i = 0; i < FORLIM; i++)
    fprintf(afile->f, " %1.1f", d->locations[i]);
  fprintf(afile->f, "\nmarks: %ld\n", d->marks);

  fprintf(afile->f, "min: %5.1f\n", d->min);
  fprintf(afile->f, "max: %5.1f\n", d->max);

  fprintf(afile->f, "petal: ");
  WITH1 = &d->petal;
  fprintf(afile->f, "%ld", WITH1->el);
  fprintf(afile->f, " %c", WITH1->c_);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->eh);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->es);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->eb);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->fh);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->fs);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->fb);
  fprintf(afile->f, " %*.*f\n", pwid_, pdec_, WITH1->spare);

  /*






*/


}

#undef pwid_
#undef pdec_



Static Void showdefinitions(afile, definitionlist)
_TEXT *afile;
definetype *definitionlist;
{
  definetype *f = definitionlist;

  while (f != NULL) {
/* p2c: prgmod.p: Note: Eliminated unused assignment statement [338] */
    showadefinition(afile, f);
    putc('\n', afile->f);
    f = f->next;
  }
}



Static Void writedefinitions(afile, definitionlist)
_TEXT *afile;
definetype *definitionlist;
{
  definetype *f = definitionlist;

  while (f != NULL) {
/* p2c: prgmod.p: Note: Eliminated unused assignment statement [338] */
    writeadefinition(afile, f);
    f = f->next;
  }
}



Static Void cleardefinition(d)
definetype **d;
{
  long l;
  definetype *WITH;
  petaltype *WITH1;

  WITH = *d;
  clearstring(&WITH->nametag);
  clearstring(&WITH->background);
  clearstring(&WITH->negparts);
  clearstring(&WITH->posparts);
  for (l = 0; l < maxstring; l++)
    WITH->locations[l] = 0.0;
  WITH->marks = 0;
  WITH->min = 0.0;
  WITH->max = 0.0;
  WITH->definitionnumber = 0;
  WITH->matrix = NULL;
  WITH->RiboundFeature = 0.0;
  WITH->ZboundFeature = 0.0;
  WITH->PboundFeature = 0.0;

  WITH1 = &WITH->petal;

  WITH1->el = 0;
  WITH1->c_ = ' ';
  WITH1->eh = 0.0;
  WITH1->es = 0.0;
  WITH1->eb = 0.0;
  WITH1->fh = 0.0;
  WITH1->fs = 0.0;
  WITH1->fb = 0.0;
  WITH1->spare = 0.0;
  WITH->isgap = false;
  WITH->istotal = false;
  WITH->ismulti = false;
  WITH->next = NULL;
}



Static boolean equaldefinition(a_, b)
definetype *a_, *b;
{
  boolean same = true;
  long i, FORLIM;

  if (!equalstring(a_->nametag, b->nametag))
    return false;
  if (!equalstring(a_->background, b->background))
    return false;
  if (!equalstring(a_->negparts, b->negparts))
    return false;

  if (a_->min != b->min)
    return false;
  if (a_->max != b->max)
    return false;
  /*

*/
  if (a_->marks != b->marks)
    return true;
  FORLIM = a_->marks;
  for (i = 0; i < FORLIM; i++) {
    if (a_->locations[i] != b->locations[i]) {
      same = false;
/* p2c: prgmod.p: Note: Eliminated unused assignment statement [338] */
/* p2c: prgmod.p: Note: Eliminated unused assignment statement [338] */
/* p2c: prgmod.p: Note: Eliminated unused assignment statement [338] */
    }
  }
/* p2c: prgmod.p: Note: Eliminated unused assignment statement [338] */
/* p2c: prgmod.p: Note: Eliminated unused assignment statement [338] */
  return same;
  /*






*/
}


#define tab             9

#define debugging       false


Local Void IsGapOrIsTotal(d)
definetype **d;
{
  /*
*/
  stringDelila totstring, gapstring;
  long deflocation, gaplocation, totlocation;
  boolean done = false;
  _TEXT TEMP;

  if (debugging) {
    printf("IsGapOrIsTotal test of this definition:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeadefinition(&TEMP, *d);
  }

  clearstring(&totstring);

  totstring.letters[0] = (Char)tab;
  totstring.letters[1] = 't';
  totstring.letters[2] = 'o';
  totstring.letters[3] = 't';
  totstring.letters[4] = 'a';
  totstring.letters[5] = 'l';
  totstring.length = 6;
  clearstring(&gapstring);

  /*

*/


  gapstring.letters[0] = (Char)tab;
  gapstring.letters[1] = 'G';
  gapstring.letters[2] = 'a';
  gapstring.letters[3] = 'p';
  gapstring.length = 4;
  (*d)->isgap = true;
  deflocation = (*d)->nametag.length;
  gaplocation = gapstring.length;
  while (!done) {
    /*




*/
    if ((*d)->nametag.letters[deflocation-1] !=
	gapstring.letters[gaplocation-1]) {
      (*d)->isgap = false;
      done = true;
    }
    deflocation--;
    gaplocation--;
    /*


*/
    if (deflocation < 1)
      done = true;
    if (gaplocation < 1)
      done = true;
  }

  /*




















*/


  (*d)->istotal = true;
  done = false;

  deflocation = (*d)->nametag.length;
  while (!done) {
    /*

*/
    if ((*d)->nametag.letters[deflocation-1] == (Char)tab)
      done = true;
    deflocation--;
    if (deflocation < 1) {
      done = true;
      (*d)->istotal = false;
    }
  }
  /*

*/
  if ((*d)->istotal == true) {
    /*

*/
    totlocation = totstring.length;
    done = false;
    /*


*/
    while (!done) {
      /*




*/
      if ((*d)->nametag.letters[deflocation-1] !=
	  totstring.letters[totlocation-1]) {
	(*d)->istotal = false;
	done = true;
      }
      deflocation--;
      totlocation--;
      /*


*/
      if (deflocation < 1)
	done = true;
      if (totlocation < 1)
	done = true;
    }
  }


  if ((*d)->isgap || (*d)->istotal)
    (*d)->ismulti = true;
  else
    (*d)->ismulti = false;

  if (!debugging)
    return;


  TEMP.f = stdout;
  *TEMP.name = '\0';
  /*

*/
  showadefinition(&TEMP, *d);
  /*




*/
  if ((*d)->isgap)
    printf("Is a gap\n");
  else
    printf("Is NOT a gap\n");
  if ((*d)->istotal)
    printf("Is a total\n");
  else
    printf("Is NOT a total\n");
  printf("IsGapOrIsTotal: end &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
}

#undef tab
#undef debugging



Static Void readadefinition(afile, d)
_TEXT *afile;
definetype **d;
{
  long i;
  double location;
  definetype *WITH;
  _TEXT TEMP;
  long FORLIM;


  skipnonblanks(afile);
  skipblanks(afile);
  WITH = *d;
  /*



*/
  readquotestring(afile, &WITH->nametag);


  readquotestring(afile, &WITH->background);
  if (WITH->background.length < 1 || WITH->background.length > 2) {
    printf("Background string must contain only 1 or 2 characters.\n");
    printf("Instead, it contained: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->background);
    putchar('\n');
    halt();
  }

  readquotestring(afile, &WITH->negparts);
  readquotestring(afile, &WITH->posparts);
  if (WITH->negparts.length != WITH->posparts.length) {
    printf("While reading definition of ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->nametag);
    printf("\nthe negative part ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->negparts);
    printf(" and\n");
    printf("the positive part ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->posparts);
    printf(" are not the same length.\n");
    printf("(%ld<>%ld)\n", WITH->negparts.length, WITH->posparts.length);
    halt();
  }

  WITH->marks = WITH->negparts.length;
  if (WITH->marks == 0)
    WITH->marks = 2;

  FORLIM = WITH->marks;

  for (i = 0; i < FORLIM; i++)
    fscanf(afile->f, "%lg", &WITH->locations[i]);

  WITH->matrix = NULL;
  WITH->RiboundFeature = -LONG_MAX;
  WITH->ZboundFeature = LONG_MAX;
  WITH->PboundFeature = 0.0;

  WITH->ismulti = false;

  WITH->next = NULL;




  WITH->min = LONG_MAX;
  WITH->max = -LONG_MAX;
  FORLIM = WITH->marks;
  for (i = 0; i < FORLIM; i++) {
    location = WITH->locations[i];
    if (location < WITH->min)
      WITH->min = location;
    if (location > WITH->max)
      WITH->max = location;
  }
  if (WITH->min == LONG_MAX) {
    printf("readadefinition: program error at min\n");
    halt();
  }
  if (WITH->max == -LONG_MAX) {
    printf("readadefinition: program error at max\n");
    halt();
  }


  IsGapOrIsTotal(d);

}



Static Void clearfeature(f)
featuretype **f;
{
  featuretype *WITH;

  WITH = *f;
  clearstring(&WITH->id);
  WITH->coordinate = 0.0;
  WITH->orientation = 1;
  clearstring(&WITH->nametag);
  clearstring(&WITH->othertag);
  WITH->definition = NULL;
  WITH->duplicateprevious = NULL;
  WITH->Ri = 0.0;
  WITH->Z = 0.0;
  WITH->probability = 0.0;
  WITH->Tparam = ' ';
  WITH->Aparam = 0.0;
  WITH->Bparam = 0.0;
  WITH->Cparam = 0.0;
  WITH->Dparam = 0.0;
  WITH->relcoo = 0.0;
  WITH->evencoordinate = 0.0;
  WITH->unsatisfied = true;
  WITH->fromrange = 0.0;
  WITH->torange = 0.0;
  WITH->featurenumber = 0;
  WITH->desiredline = 0;
  WITH->others = NULL;
  WITH->mothers = NULL;
  WITH->usedInGroup = false;
  WITH->next = NULL;
}



Static Void writeafeature(afile, f)
_TEXT *afile;
featuretype *f;
{
  /*




*/
  featurecarrier *o;

  /*
*/
  if (f == NULL) {
    printf("writeafeature: nil feature pointer!\n");
    halt();
  }
  fprintf(afile->f, "@ ");
  writeNOTABstring(afile, &f->id);
  fprintf(afile->f, " %1.1f", f->coordinate);
  putc(' ', afile->f);
  if (f->orientation > 0)
    fprintf(afile->f, "+%ld", f->orientation);
  else
    fprintf(afile->f, "%ld", f->orientation);
  putc(' ', afile->f);
  writequotestring(afile, f->nametag);
  putc(' ', afile->f);
  writequotestring(afile, f->othertag);
  if (f->definition == NULL) {
    fprintf(afile->f, " %*.*f", infofield, infodecim, 0.0);
    fprintf(afile->f, " %*.*f", infofield, infodecim, 0.0);
    fprintf(afile->f, " %*.*f", infofield, infodecim, 0.0);
    fprintf(afile->f, " %3.1f", 0.0);
  }
  /*









*/
  else {
    if (f->definition->matrix != NULL) {
      fprintf(afile->f, " %1.*f", infodecim, f->Ri);
      fprintf(afile->f, " %1.*f", infodecim, f->Z);
      fprintf(afile->f, " %1.*f", infodecim, f->probability);
    }
    /*



*/
    else {
      fprintf(afile->f, " %c", f->Tparam);
      /*




*/
      fprintf(afile->f, " %1.*f", infodecim, f->Aparam);
      fprintf(afile->f, " %1.*f", infodecim, f->Bparam);
      fprintf(afile->f, " %1.*f", infodecim, f->Cparam);
      fprintf(afile->f, " %1.*f", infodecim, f->Dparam);
    }
    fprintf(afile->f, " %1.1f", f->relcoo);
    fprintf(afile->f, " #%ld", f->featurenumber);
  }
  if (f->others == NULL)
    return;
  if (f->others->afeature != NULL)
    fprintf(afile->f, " > ");
  o = f->others;
  while (o != NULL) {
    if (o != f->others)
      fprintf(afile->f, ", ");
    if (o->afeature != NULL) {
      putc('[', afile->f);
      writeNOTABstring(afile, &o->afeature->nametag);
      putc(']', afile->f);
    }
    o = o->next;
  }
}


/* Local variables for readafeature: */
struct LOC_readafeature {
  featuretype **f;
} ;

Local Void reportandhalt(LINK)
struct LOC_readafeature *LINK;
{
  _TEXT TEMP;

  printf("Feature details:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeafeature(&TEMP, *LINK->f);
  putchar('\n');
  halt();
}




Static Void readafeature(afile, definitionlist, f_)
_TEXT *afile;
definetype *definitionlist;
featuretype **f_;
{
  /*




*/
  struct LOC_readafeature V;
  definetype *d;
  featuretype *WITH;
  _TEXT TEMP;

  V.f = f_;
  WITH = *V.f;
  skipnonblanks(afile);
  skipblanks(afile);
  grabtoken(afile, &WITH->id);
  fscanf(afile->f, "%lg", &WITH->coordinate);
  fscanf(afile->f, "%ld", &WITH->orientation);
  readquotestring(afile, &WITH->nametag);
  readquotestring(afile, &(*V.f)->othertag);


  if (WITH->orientation != -1 && WITH->orientation != 1) {
    printf("while reading feature id ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->id);
    printf("\nfeature orientation must be -1 or +1\n");
    printf("It is %ld\n", WITH->orientation);
    reportandhalt(&V);
  }



  d = definitionlist;
  while (d != NULL) {
    if (equalstring(d->nametag, (*V.f)->nametag))
      WITH->definition = d;
    d = d->next;
  }
  if (WITH->definition == NULL) {
    printf("No definition was found for feature ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->nametag);
    putchar('\n');
    reportandhalt(&V);
  }
  d = WITH->definition;


  if (d->matrix != NULL) {
    fscanf(afile->f, "%lg%lg%lg", &WITH->Ri, &WITH->Z, &WITH->probability);
    if (!P_eoln(afile->f))
      fscanf(afile->f, "%lg", &WITH->relcoo);
    else
      WITH->relcoo = 0.0;
  } else {
    WITH->Ri = -LONG_MAX;
    WITH->Z = LONG_MAX;
    WITH->probability = 1.0;


    /*

*/

    skipblanks(afile);
    if (P_eoln(afile->f)) {
      WITH->Tparam = ' ';
      WITH->Aparam = 0.0;
      WITH->Bparam = 0.0;
      WITH->Cparam = 0.0;
      WITH->Dparam = 0.0;
      WITH->relcoo = 0.0;
    } else {
      if (P_peek(afile->f) == '9' || P_peek(afile->f) == '8' ||
	  P_peek(afile->f) == '7' || P_peek(afile->f) == '6' ||
	  P_peek(afile->f) == '5' || P_peek(afile->f) == '4' ||
	  P_peek(afile->f) == '3' || P_peek(afile->f) == '2' ||
	  P_peek(afile->f) == '1' || P_peek(afile->f) == '0' ||
	  P_peek(afile->f) == '+' || P_peek(afile->f) == '-') {
	fscanf(afile->f, "%lg", &WITH->Aparam);
	fscanf(afile->f, "%lg", &WITH->Bparam);
	fscanf(afile->f, "%lg", &WITH->Cparam);
	WITH->Dparam = 0.0;
      }

      else {
	WITH->Tparam = getc(afile->f);
	if (WITH->Tparam == '\n')
	  WITH->Tparam = ' ';
	fscanf(afile->f, "%lg", &WITH->Aparam);
	fscanf(afile->f, "%lg", &WITH->Bparam);
	fscanf(afile->f, "%lg", &WITH->Cparam);
	fscanf(afile->f, "%lg", &WITH->Dparam);
      }
      if (!P_eoln(afile->f))
	fscanf(afile->f, "%lg", &WITH->relcoo);
      else
	WITH->relcoo = 0.0;
    }
  }


  /*



*/

  WITH->unsatisfied = true;
  WITH->fromrange = WITH->definition->min;
  WITH->torange = WITH->definition->max;

  /*

*/
  WITH->desiredline = 0;
  WITH->next = NULL;

  /*


*/
  WITH->others = NULL;
  WITH->mothers = NULL;

}




Static Void showafeature(afile, f)
_TEXT *afile;
featuretype *f;
{
  featurecarrier *o;
  long totals = 0, gaps = 0;

  fprintf(afile->f, "FEATURE #%ld:\n", f->featurenumber);
  fprintf(afile->f, "id: ");
  writequotestring(afile, f->id);
  fprintf(afile->f, "\ncoordinate: %1.1f\n", f->coordinate);


  fprintf(afile->f, "orientation: ");
  if (f->orientation > 0)
    fprintf(afile->f, "+%ld\n", f->orientation);
  else
    fprintf(afile->f, "%ld\n", f->orientation);

  fprintf(afile->f, "nametag: ");
  writequotestring(afile, f->nametag);
  fprintf(afile->f, "\nothertag: ");

  writequotestring(afile, f->othertag);
  fprintf(afile->f, "\nunsatisfied: ");

  if (f->unsatisfied)
    fprintf(afile->f, "true\n");
  else
    fprintf(afile->f, "false\n");

  fprintf(afile->f, "fromrange: %5.1f\n", f->fromrange);
  fprintf(afile->f, "  torange: %5.1f\n", f->torange);
  fprintf(afile->f, "desiredline: %5ld\n", f->desiredline);
  if (f->duplicateprevious == NULL)
    fprintf(afile->f, "NOT a duplicate feature\n");
  else
    fprintf(afile->f, "DUPLICATE of feature #%ld\n",
	    f->duplicateprevious->featurenumber);

  if (f->others != NULL) {
    fprintf(afile->f, "This feature points at these OTHERS:\n");
    fprintf(afile->f, "feature -> ");
    o = f->others;
    while (o != NULL) {
      if (o != f->others)
	fprintf(afile->f, ", ");
      if (o->afeature != NULL) {
	putc('[', afile->f);
	writeNOTABstring(afile, &o->afeature->nametag);
	putc(']', afile->f);
      } else
	fprintf(afile->f, "[EMPTY]");
      o = o->next;
    }
    /*





*/

    putc('\n', afile->f);
    o = f->others;
    while (o != NULL) {
      if (o->afeature != NULL) {
	fprintf(afile->f, "= ");
	writeafeature(afile, o->afeature);
	putc('\n', afile->f);
      }
      o = o->next;
    }
  } else
    fprintf(afile->f, "This feature does not point to any OTHERS.\n");

  if (f->mothers != NULL) {
    fprintf(afile->f,
	    "The MOTHERS of this feature (that point to this feature) are:\n");
    o = f->mothers;
    while (o != NULL) {
      if (o->afeature != NULL) {
	fprintf(afile->f, "feature <- \"");
	writeNOTABstring(afile, &o->afeature->nametag);
	fprintf(afile->f, "\" #%ld\n", o->afeature->featurenumber);
	/*

*/
	if (o->afeature->definition->istotal)
	  totals++;
	if (o->afeature->definition->isgap)
	  gaps++;
      }

      o = o->next;
    }
    o = f->mothers;

    while (o != NULL) {
      if (o->afeature != NULL) {
	fprintf(afile->f, "= ");
	writeafeature(afile, o->afeature);
	putc('\n', afile->f);
      }
      o = o->next;
    }

  } else
    fprintf(afile->f, "This feature has no MOTHERS.\n");


  fprintf(afile->f, "%ld gap", gaps);
  if (gaps != 1)
    putc('s', afile->f);
  fprintf(afile->f, "\n%ld total", totals);
  if (totals != 1)
    putc('s', afile->f);
  putc('\n', afile->f);
  if (totals > 1) {
    printf("showafeature: more than one total!\n");
    halt();
  }
  if (gaps > 1 && totals < 1) {
    printf("showafeature: %ld gaps but no total!\n", gaps);

    halt();
  }
}



Static Void showfeatures(afile, featurelist, paging)
_TEXT *afile;
featuretype *featurelist;
boolean paging;
{
  /*

*/
  featuretype *f = featurelist;

  while (f != NULL) {
/* p2c: prgmod.p: Note: Eliminated unused assignment statement [338] */
    showafeature(afile, f);
    f = f->next;
    if (f == NULL)
      break;

    if (paging)
      fprintf(afile->f, "\f");
    else
      putc('\n', afile->f);
  }
}



Static Void writefeatures(afile, featurelist)
_TEXT *afile;
featuretype *featurelist;
{
  featuretype *f = featurelist;

  while (f != NULL) {
/* p2c: prgmod.p: Note: Eliminated unused assignment statement [338] */
    writeafeature(afile, f);
    putc('\n', afile->f);
    f = f->next;
  }
}


/* Local variables for featureinlist: */
struct LOC_featureinlist {
  featuretype *newfeature, *f;
  boolean found;
} ;

Local Void checkorpos(LINK)
struct LOC_featureinlist *LINK;
{
  /*

*/
  LINK->found = (LINK->newfeature->orientation == LINK->f->orientation &&
		 LINK->newfeature->coordinate == LINK->f->coordinate);
}



Static boolean featureinlist(featurelist, newfeature_, oldfeature)
featuretype *featurelist, *newfeature_, **oldfeature;
{
  /*

*/
  struct LOC_featureinlist V;
  /*


*/
  boolean done = false;

  V.newfeature = newfeature_;
  /*







*/

  V.found = false;
  V.f = featurelist;
  *oldfeature = NULL;
  /*



*/
  while (!done) {
    /*



*/

    if (equalstring(V.newfeature->id, V.f->id) &
	equalstring(V.newfeature->nametag, V.f->nametag)) {
      if (V.f->definition == NULL)
	checkorpos(&V);
      else if (V.f->definition->matrix == NULL)
	checkorpos(&V);
      else {
	V.found = (V.newfeature->Ri == V.f->Ri &&
		   ((V.f->definition->matrix->symmetry == 'o' &&
		     V.newfeature->coordinate == V.f->coordinate) ||
		    (V.f->definition->matrix->symmetry == 'a' &&
		     V.newfeature->orientation == V.f->orientation &&
		     V.newfeature->coordinate == V.f->coordinate) ||
		    (V.f->definition->matrix->symmetry == 'e' &&
		     1 == fabs(V.newfeature->coordinate - V.f->coordinate) &&
		     V.newfeature->orientation != V.f->orientation) ||
		    (V.f->definition->matrix->symmetry == 'e' &&
		     V.newfeature->coordinate == V.f->coordinate &&
		     V.newfeature->orientation == V.f->orientation)));

	/*
*/
	/*




*/









	/*


*/







      }
    }

    if (V.found) {
      *oldfeature = V.f;
      done = true;
      /*









*/
    } else if (V.f->next == NULL)
      done = true;
    else
      V.f = V.f->next;
  }

  /*



*/
  return V.found;
}



Static Void locatedefinition(aname, definitionlist, thedefinition)
stringDelila aname;
definetype *definitionlist, **thedefinition;
{
  /*
*/
  boolean done = false;

  *thedefinition = definitionlist;
  do {
    if (*thedefinition == NULL)
      done = true;
    else {
      if (equalstring((*thedefinition)->nametag, aname))
	done = true;
      else {
	*thedefinition = (*thedefinition)->next;

      }
    }
  } while (!done);
}



Static Void locatefeature(aname, coo, featurelist, thefeature, StopAtTotal)
stringDelila aname;
double coo;
featuretype *featurelist, **thefeature;
boolean StopAtTotal;
{

  /*





*/
  boolean done = false;

  *thefeature = featurelist;
  do {
    if (*thefeature == NULL)
      done = true;
    else {
      /*





*/
      if (equalstring((*thefeature)->nametag, aname)) {
	if ((*thefeature)->coordinate == coo)
	  done = true;
	else
	  *thefeature = (*thefeature)->next;
      } else if ((*thefeature)->definition->istotal && StopAtTotal) {

	*thefeature = NULL;
      } else {
	*thefeature = (*thefeature)->next;

      }
    }
  } while (!done);
}


#define debugSplitstring  true


#define countmarkdebug  false


/* Local variables for readdefinitionsandfeatures: */
struct LOC_readdefinitionsandfeatures {
  definetype **definitionlist;
  featuretype **featurelist, *f;


  /*



*/
  boolean ingroup;
  /*
*/
  featuretype *groupfeature, *aGapFeature, *groupTotalFeature;
  featurecarrier *groupGapCarrier, *groupTotalCarrier;
  featuretype *leftgap, *rightgap, *ff;
  /*
*/
  featuretype *gg;
  /*
*/
  stringDelila *nameparts, *anamepart, leftstring, gapstring, rightstring;
  long gap;
  Char aletter;
} ;

Local Void yell(LINK)
struct LOC_readdefinitionsandfeatures *LINK;
{
  printf("ERROR in reading definitions and features\n");
}

Local Void barout(LINK)
struct LOC_readdefinitionsandfeatures *LINK;
{
  printf("**********************************************************************\n");
}

Local Void splitstring(s, break_, terminator, parts, LINK)
stringDelila s;
Char break_, terminator;
stringDelila **parts;
struct LOC_readdefinitionsandfeatures *LINK;
{
  /*








*/
  long l = 1;
  boolean done = false;
  stringDelila *spointer;
  _TEXT TEMP;

  if (superdebug) {
    if (debugSplitstring) {
      printf("IN splitstring the string to split is:\n");
      putchar('"');
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &s);
      printf("\"\n");
      printf("======> break character is: \"%c\"\n", break_);
    }
  }
  disposestring(parts);
  *parts = (stringDelila *)Malloc(sizeof(stringDelila));
  spointer = *parts;
  clearstring(spointer);
  while (!done) {
    if (s.letters[l-1] == terminator)
      done = true;
    else {
      if (s.letters[l-1] == break_) {
	if (superdebug) {
	  if (debugSplitstring) {
	    printf("debugSplitstring: \"");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writestring(&TEMP, spointer);
	    printf("\"\n");
	  }
	}
	spointer->next = (stringDelila *)Malloc(sizeof(stringDelila));
	spointer = spointer->next;
	clearstring(spointer);
      } else {
	spointer->length++;
	spointer->letters[spointer->length - 1] = s.letters[l-1];
	if (superdebug) {
	  if (debugSplitstring) {
	    if (s.letters[l-1] == ' ')
	      printf(
		"debugSplitstring: splitstring write a space $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	    if (s.letters[l-1] == '\t')
	      printf(
		"debugSplitstring: splitstring write a TAB  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	    if (false)
	      printf("WRITING: ord(%c)=%d\n", s.letters[l-1], s.letters[l-1]);
	  }
	}
      }
      l++;
      if (l > maxstring)
	done = true;
    }
    if (l == maxstring) {
      printf("splitstring: maxed out! maxstring = l = %ld\n", l);
      halt();
    }
  }
  if (!superdebug)
    return;
  if (!debugSplitstring)
    return;
  printf("debugSplitstring: splitstring result: \"");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, spointer);
  printf("\"\n");
  printf("OUT splitstring\n");
}

#undef debugSplitstring

/* Local variables for threadgroup: */
struct LOC_threadgroup {
  struct LOC_readdefinitionsandfeatures *LINK;
} ;

/* Local variables for countmarks: */
struct LOC_countmarks {
  struct LOC_threadgroup *LINK;
  featuretype *f;
  long requiredcount;
} ;

Local Void countmarksprint(LINK)
struct LOC_countmarks *LINK;
{
  _TEXT TEMP;

  printf("++++++++ countmarks ++++++++");
  if (LINK->requiredcount < 0)
    printf(" final check\n");
  else
    printf(" looking for %ld marks\n", LINK->requiredcount);
  printf(
    "++++++++ Counting marks for this total feature in group number: %ld FAILED: \n",
    groupnumber);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  showadefinition(&TEMP, LINK->f->definition);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  showafeature(&TEMP, LINK->f);
  printf("%ld marks are in this feature\n", LINK->f->definition->marks);

  if (true)
    return;

  printf("\n******* countmarks readdefinitionsandfeatures: *******\n");
  printf("******* DEFINITIONS:\n");
  if (false) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showdefinitions(&TEMP, *LINK->LINK->LINK->definitionlist);
  }
  printf("\n******* FEATURES:\n");
  if (true) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showfeatures(&TEMP, *LINK->LINK->LINK->featurelist, true);
  }
  printf("\n******* countmarks readdefinitionsandfeatures: *******\n");
  /*

*/
}


Local Void countmarks(f_, requiredcount_, LINK)
featuretype *f_;
long requiredcount_;
struct LOC_threadgroup *LINK;
{
  /*





*/
  struct LOC_countmarks V;
  long expectedcount, observedcount;
  featurecarrier *otherptr;
  _TEXT TEMP;

  V.LINK = LINK;
  V.f = f_;
  V.requiredcount = requiredcount_;
  if (countmarkdebug)
    printf("+++++++++++++++++++++ BEGIN countmarks\n");
  if (V.f->definition->istotal) {
    if (V.f->others != NULL) {
      otherptr = V.f->others;
      observedcount = 0;
      while (otherptr != NULL) {
	if (countmarkdebug) {
	  putchar('\n');
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writeafeature(&TEMP, otherptr->afeature);
	  printf("\nloop observedcount=%ld\n", observedcount);
	  printf("isgap:   %s\n",
		 otherptr->afeature->definition->isgap ? " TRUE" : "FALSE");
	  printf("istotal: %s\n",
		 otherptr->afeature->definition->istotal ? " TRUE" : "FALSE");
	}
	if (!otherptr->afeature->definition->isgap &&
	    !otherptr->afeature->definition->istotal)
	  observedcount++;
	otherptr = otherptr->next;
      }
      if (countmarkdebug)
	printf("**** final observedcount=%ld\n", observedcount);

      if (V.requiredcount == -1)
	expectedcount = V.f->definition->marks;
      else
	expectedcount = V.requiredcount;
      if (observedcount != expectedcount) {
	countmarksprint(&V);
	printf("countmarks: expected: %ld observed: %ld\n",
	       expectedcount, observedcount);
	printf(
	  "**** Wrong number of marks found  in linked list for this total feature. ****\n");
	halt();
      }

    } else {
      if (countmarkdebug)
	printf("countmarks no others\n");
    }
  } else {
    printf("countmarks feature is not a total, cannot count marks\n");
    halt();
  }
  if (countmarkdebug)
    printf("+++++++++++++++++++++ END countmarks\n");
}

#undef countmarkdebug


Local Void addmotherlinks(agap, thefeature, LINK)
featuretype **agap, **thefeature;
struct LOC_threadgroup *LINK;
{
  /*
*/
  featurecarrier *mom, *previousMom;
  /*
*/
  featurecarrier *gapthing;
  boolean done = false;
  _TEXT TEMP;

  /*





*/
  if ((*thefeature)->mothers == NULL) {
    if (false) {
      printf("************ ALL FEATURES: *************\n");
      printf("**** (also listed in the list file) ****\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      showfeatures(&TEMP, *LINK->LINK->featurelist, false);
      printf("****************************************\n");
      printf("**** addmotherlinks: no mothers found for the feature:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      showafeature(&TEMP, *thefeature);
      /*


*/
    }


    /*
*/
    if (!((*thefeature)->definition->isgap ||
	  (*thefeature)->definition->istotal))
      halt();
    return;
  }
  mom = (*thefeature)->mothers;
  previousMom = mom;
  while (!done) {
    if (mom->afeature == *thefeature) {
      done = true;
      break;
    }
    if (mom->afeature->definition->istotal) {
      gapthing = (featurecarrier *)Malloc(sizeof(featurecarrier));
      if (mom == previousMom) {
	gapthing->next = (*thefeature)->mothers;
	(*thefeature)->mothers = gapthing;
      } else {
	previousMom->next = gapthing;
	gapthing->next = mom;
      }
      gapthing->afeature = *agap;
      done = true;
      if (mom->next != NULL) {
	printf("addmotherlinks: total points to something!\n");
	halt();
      }

      /*


*/
      continue;
    }
    /*
*/

    previousMom = mom;
    mom = mom->next;
    if (mom == NULL) {
      printf("addmotherlinks: failed to find total\n");
      halt();
    }
  }
  /*


*/
  /*

*/
}

Local Void threadgroup(LINK)
struct LOC_readdefinitionsandfeatures *LINK;
{
  /*
*/
  struct LOC_threadgroup V;
  long m, FORLIM;
  _TEXT TEMP;

  V.LINK = LINK;
  if (!LINK->ingroup)
    return;
  if (LINK->groupfeature == NULL) {
    LINK->groupfeature = LINK->f;

    return;
  }
  /*


*/

  if (!LINK->f->definition->istotal)
    return;
  /*



*/
  LINK->groupTotalFeature = LINK->f;
  /*







*/



  /*



*/
  /*






*/

  /*
*/
  LINK->groupTotalCarrier = NULL;


  /*



*/
  splitstring(LINK->groupTotalFeature->nametag, '-', '\t', &LINK->nameparts,
	      LINK);


  /*



*/

  LINK->anamepart = LINK->nameparts;
  FORLIM = LINK->groupTotalFeature->definition->marks;
  for (m = 1; m <= FORLIM; m++) {
    LINK->gg = LINK->groupfeature;

    while (LINK->gg != NULL) {
      /*














*/

      /*

*/
      if (equalstring(*LINK->anamepart, LINK->gg->nametag)) {
	if (LINK->gg->definition->isgap || LINK->gg->definition->istotal) {
	  printf("*** PROGRAM SHOULD NEVER BE HERE!\n");
	  printf("*** Total found matching gap or total\n");
	  halt();
	}


	else {
	  if (LINK->groupTotalCarrier == NULL) {
	    LINK->groupTotalFeature->others =
	      (featurecarrier *)Malloc(sizeof(featurecarrier));
	    LINK->groupTotalCarrier = LINK->groupTotalFeature->others;
	    LINK->groupTotalCarrier->next = NULL;
	  }
	  /*

*/
	  else {
	    LINK->groupTotalCarrier->next = (featurecarrier *)
					    Malloc(sizeof(featurecarrier));
	    LINK->groupTotalCarrier = LINK->groupTotalCarrier->next;
	    LINK->groupTotalCarrier->next = NULL;
	  }
	  /*

*/
	  LINK->groupTotalCarrier->afeature = LINK->gg;


	  if (LINK->gg->mothers == NULL) {
	    LINK->gg->mothers = (featurecarrier *)Malloc(sizeof(featurecarrier));
	    LINK->gg->mothers->next = NULL;
	    LINK->gg->mothers->afeature = LINK->groupTotalFeature;


	  }

	  countmarks(LINK->groupTotalFeature, m, &V);
	}
      }
      /*

*/

      LINK->gg = LINK->gg->next;
    }




    LINK->anamepart = LINK->anamepart->next;
  }
  /*



*/
  /*

*/


  if (LINK->anamepart != NULL) {
    printf("total names not all used\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showafeature(&TEMP, LINK->f);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showadefinition(&TEMP, LINK->f->definition);
    halt();
  }
  countmarks(LINK->groupTotalFeature, -1L, &V);


  /*



*/

  /*

*/
  LINK->gg = LINK->groupfeature;
  while (!LINK->gg->definition->istotal && LINK->gg != NULL) {
    if (LINK->gg->definition->isgap) {
      if (superdebug) {
	printf("BUBBA gg^.number=%ld ++++++++++++++++++++\n",
	       LINK->gg->featurenumber);
	printf("BUBBA gg is: \n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, LINK->gg);
	putchar('\n');
      }

      splitstring(LINK->gg->nametag, '-', '\t', &LINK->nameparts, LINK);
      /*



*/

      /*









*/

      LINK->leftstring = *LINK->nameparts;
      LINK->gapstring = *LINK->nameparts->next;

      LINK->rightstring = *LINK->nameparts->next->next;


      LINK->gap = 0;
      if (LINK->gapstring.letters[0] != '(' &&
	  LINK->gapstring.letters[LINK->gapstring.length - 1] != ')') {
	printf("Gap string found without surrounding \"()\"\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showafeature(&TEMP, LINK->gg);
	halt();
      } else {
	LINK->gapstring.current = 2;
	while (LINK->gapstring.current < LINK->gapstring.length) {
	  LINK->aletter = LINK->gapstring.letters[LINK->gapstring.current - 1];
	  if (LINK->aletter != '9' && LINK->aletter != '8' &&
	      LINK->aletter != '7' && LINK->aletter != '6' &&
	      LINK->aletter != '5' && LINK->aletter != '4' &&
	      LINK->aletter != '3' && LINK->aletter != '2' &&
	      LINK->aletter != '1' && LINK->aletter != '0') {
	    printf("Gap contains non-digit: \"%c\"\n", LINK->aletter);
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    showafeature(&TEMP, LINK->gg);
	    halt();
	  }
	  LINK->gap = LINK->gap * 10 + LINK->aletter - '0';
	  LINK->gapstring.current++;
	}
      }

      if (superdebug) {
	printf("BUBBA gap is %ld\n", LINK->gap);
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &LINK->leftstring);
	printf("<-  leftstring BUBBA\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &LINK->rightstring);
	printf("<- rightstring BUBBA\n");
      }
      if (LINK->gap != LINK->gg->torange - LINK->gg->fromrange) {
	printf("Gap in nametag does not match range:\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showafeature(&TEMP, LINK->gg);
	halt();
      }
      LINK->aGapFeature = LINK->gg;



      LINK->leftgap = NULL;
      LINK->rightgap = NULL;
      LINK->ff = LINK->groupfeature;
      while (LINK->ff != LINK->groupTotalFeature &&
	     (LINK->leftgap == NULL || LINK->rightgap == NULL)) {
	if (!(LINK->ff->definition->isgap || LINK->ff->definition->istotal)) {
	  if ((long)floor(LINK->ff->relcoo + 0.5) ==
	      LINK->aGapFeature->fromrange) {
	    if (equalstring(LINK->leftstring, LINK->ff->nametag))
	      LINK->leftgap = LINK->ff;

	  }

	  if ((long)floor(LINK->ff->relcoo + 0.5) == LINK->aGapFeature->torange) {
	    if (equalstring(LINK->rightstring, LINK->ff->nametag))
	      LINK->rightgap = LINK->ff;


	  }

	}

	LINK->ff = LINK->ff->next;

      }

      if (superdebug) {
	printf("BUBBA leftgap: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, LINK->leftgap);
	printf("\nBUBBA rightgap: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, LINK->rightgap);
	putchar('\n');
      }


      if (LINK->leftgap == NULL || LINK->rightgap == NULL) {
	printf("gap ends not found:\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showafeature(&TEMP, LINK->aGapFeature);
	halt();
      }
      if (LINK->leftgap->mothers == NULL) {
	printf("ERROR: missing total for leftgap\n");
	halt();
      }
      if (LINK->rightgap->mothers == NULL) {
	printf("ERROR: missing total for rightgap\n");
	halt();
      }

      /*
*/

      /*
*/

      if (LINK->aGapFeature->others != NULL) {
	printf("ERROR: aGapFeature not nil!\n");
	halt();
      }
      LINK->aGapFeature->others = (featurecarrier *)Malloc(sizeof(featurecarrier));
      LINK->groupGapCarrier = LINK->aGapFeature->others;
      LINK->groupGapCarrier->next = NULL;
      LINK->groupGapCarrier->afeature = LINK->leftgap;

      LINK->groupGapCarrier->next = (featurecarrier *)Malloc(sizeof(featurecarrier));
      LINK->groupGapCarrier = LINK->groupGapCarrier->next;
      LINK->groupGapCarrier->next = NULL;
      LINK->groupGapCarrier->afeature = LINK->rightgap;

      /*
*/

      addmotherlinks(&LINK->aGapFeature, &LINK->rightgap, &V);
      addmotherlinks(&LINK->aGapFeature, &LINK->leftgap, &V);
      if (superdebug) {
	printf("BUBBA final aGapFeature is: ========\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showafeature(&TEMP, LINK->aGapFeature);
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, LINK->aGapFeature);
	putchar('\n');
	/*





*/
      }

    }
    /*



*/

    LINK->gg = LINK->gg->next;

    /*

*/

  }

  if (superdebug) {
    printf("BUBBA: show AFTER threading:\n");
    LINK->gg = LINK->groupfeature;
    while (LINK->gg != NULL) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeafeature(&TEMP, LINK->gg);
      putchar('\n');
      LINK->gg = LINK->gg->next;
    }
  }



  /*

*/
  LINK->gg = LINK->groupfeature;
  while (LINK->gg != NULL) {
    if (LINK->gg->duplicateprevious != NULL && !LINK->gg->definition->isgap) {
      addmotherlinks(&LINK->aGapFeature, &LINK->gg->duplicateprevious, &V);
      /*




*/
      /*
*/
      /*


*/

      if (!LINK->aGapFeature->definition->isgap)
	LINK->aGapFeature->others->afeature = LINK->gg->duplicateprevious;
      /*






*/
    }

    /*

*/
    LINK->gg = LINK->gg->next;
  }
  /*

*/
  /*

*/


  if (superdebug) {
    printf("BUBBA: show AFTER motherlinks:========\n");
    LINK->gg = LINK->groupfeature;
    while (LINK->gg != NULL) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeafeature(&TEMP, LINK->gg);
      putchar('\n');
      LINK->gg = LINK->gg->next;
    }
  }

  LINK->groupTotalFeature = NULL;
  LINK->ingroup = false;

  LINK->groupfeature = NULL;
  /*
*/


}




Static Void readdefinitionsandfeatures(afile, KeepDuplicates, definitionlist_,
				       featurelist_)
_TEXT *afile;
boolean *KeepDuplicates;
definetype **definitionlist_;
featuretype **featurelist_;
{
  /*











































*/
  struct LOC_readdefinitionsandfeatures V;
  stringDelila aname;
  long countdefinitions = 0, countfeatures = 0, countribls = 0,
       countboundaries = 0, countpetals = 0;
  definetype *d;
  boolean done;
  definetype *dprevious, *dtest;
  boolean emptyafile;
  boolean firstfeature = true;

  Char identifier;
  ribltype *matrix;
  boolean missingmatrix;
  featuretype *newfeature;
  /*
*/
  featuretype *oldfeature;
  definetype *thedefinition;
  /*
*/
  long uniquedefinitions = 0, uniquefeatures = 0;
  _TEXT TEMP;
  petaltype *WITH;

  V.definitionlist = definitionlist_;
  V.featurelist = featurelist_;
  if (*afile->name != '\0') {
    if (afile->f != NULL)
      afile->f = freopen(afile->name, "r", afile->f);
    else
      afile->f = fopen(afile->name, "r");
  } else
    rewind(afile->f);
  if (afile->f == NULL)
    _EscIO2(FileNotFound, afile->name);
  RESETBUF(afile->f, Char);
  emptyafile = BUFEOF(afile->f);
  *V.definitionlist = NULL;
  *V.featurelist = (featuretype *)Malloc(sizeof(featuretype));
  clearfeature(V.featurelist);
  d = *V.definitionlist;
  V.f = *V.featurelist;
  /*


*/


  V.groupfeature = NULL;
  V.ingroup = false;
  V.nameparts = (stringDelila *)Malloc(sizeof(stringDelila));
  clearstring(V.nameparts);
  while (!BUFEOF(afile->f)) {
    if ((P_peek(afile->f) == '*') | P_eoln(afile->f)) {
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      continue;
    }
    skipblanks(afile);


    identifier = P_peek(afile->f);

    if (identifier == 'd') {
      if (*V.definitionlist == NULL) {
	*V.definitionlist = (definetype *)Malloc(sizeof(definetype));
	cleardefinition(V.definitionlist);
	d = *V.definitionlist;
	dprevious = d;
	/*

*/
      } else {
	dprevious = d;
	d->next = (definetype *)Malloc(sizeof(definetype));
	d = d->next;
	cleardefinition(&d);
      }
      readadefinition(afile, &d);
      /*

*/
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      countdefinitions++;
      uniquedefinitions++;
      d->definitionnumber = uniquedefinitions;


      dtest = *V.definitionlist;
      done = false;
      while (!done) {
	if (dtest == NULL) {
	  done = true;
	  break;
	}
	if (equaldefinition(dtest, d) &&
	    dtest->definitionnumber != d->definitionnumber) {
	  /*

*/

	  done = true;
	  /*
*/
	  barout(&V);
	  printf("*** WARNING: A duplicate definition was found at definition %ld:\n",
		 countdefinitions);
	  printf("*** ");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writequotestring(&TEMP, d->nametag);
	  printf(".\n");
	  printf("*** The first definition (number %ld) will be used.\n",
		 dtest->definitionnumber);

	  /*


















*/
	  barout(&V);

	  /*













*/



	  dprevious->next = NULL;
	  Free(d);
	  done = true;
	  /*
*/

	  d = dprevious;
	  /*



*/

	  /*



*/

	  /*


*/

	  uniquedefinitions--;
	}
	/*

*/

	if (!done)
	  dtest = dtest->next;
      }
      /*



*/
      continue;
    }


    if (identifier == 'r') {
      getriblmatrix(afile, &matrix);

      locatedefinition(matrix->riblname, *V.definitionlist, &thedefinition);
      if (thedefinition == NULL) {
	printf("A definition for Ribl matrix ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, matrix->riblname);
	printf(" was not found in the features.\n");
	halt();
      }

      /*
*/
      if (thedefinition->min < matrix->frombase ||
	  thedefinition->max > matrix->tobase) {
	printf("Definition:\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeadefinition(&TEMP, thedefinition);
	printf("\nViolates The From-To Bounds of its Matrix:\n");
	printf("matrix frombase: %ld\n", matrix->frombase);
	printf("matrix   tobase: %ld\n", matrix->tobase);
	halt();
      }


      thedefinition->matrix = matrix;
      countribls++;
      continue;
    }


    if (identifier == 'b') {
      skipnonblanks(afile);
      skipblanks(afile);
      readquotestring(afile, &aname);
      locatedefinition(aname, *V.definitionlist, &thedefinition);
      if (thedefinition == NULL) {
	printf("WARNING: A definition for the boundaries: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf("          was not found in the features.\n");
      } else {
	fscanf(afile->f, "%lg%lg%lg%*[^\n]", &thedefinition->RiboundFeature,
	       &thedefinition->ZboundFeature, &thedefinition->PboundFeature);
	getc(afile->f);
	countboundaries++;
      }
      continue;
    }


    if (identifier == 'p') {
      skipnonblanks(afile);
      skipblanks(afile);
      readquotestring(afile, &aname);
      locatedefinition(aname, *V.definitionlist, &thedefinition);
      if (thedefinition == NULL) {
	printf("WARNING: A definition for the color rectangle: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf("\n         was not found in the features.\n");
	fscanf(afile->f, "%*[^\n]");
	getc(afile->f);
	continue;
      }
      WITH = &thedefinition->petal;
      fscanf(afile->f, "%ld", &WITH->el);
      /*

*/
      skipblanks(afile);
      WITH->c_ = getc(afile->f);
      if (WITH->c_ == '\n')
	WITH->c_ = ' ';
      if (WITH->c_ != 'r' && WITH->c_ != 'h') {
	yell(&V);
	printf(" feature\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf(": color type must be r or h, not \"%c\"\n", WITH->c_);
	halt();
      }
      fscanf(afile->f, "%lg%lg%lg%lg%lg%lg%lg%*[^\n]", &WITH->eh, &WITH->es,
	     &WITH->eb, &WITH->fh, &WITH->fs, &WITH->fb, &WITH->spare);
      getc(afile->f);

      /*















*/

      if (WITH->eh > 1 || WITH->es > 1 || WITH->eb > 1 || WITH->fh > 1 ||
	  WITH->fs > 1 || WITH->fb > 1) {
	yell(&V);
	printf(" feature\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf(": eh, es, eb, fh, fs, and fb \n");
	printf("must have values less than 1\n");
	halt();
      }

      /*











*/

      countpetals++;
      continue;
    }




    if (identifier == '@') {
      countfeatures++;

      if (firstfeature) {
	firstfeature = false;
	readafeature(afile, *V.definitionlist, &V.f);
	fscanf(afile->f, "%*[^\n]");
	getc(afile->f);
	/*
*/
	V.f->featurenumber = countfeatures;
	uniquefeatures++;
      }

      else {
	newfeature = (featuretype *)Malloc(sizeof(featuretype));
	clearfeature(&newfeature);
	readafeature(afile, *V.definitionlist, &newfeature);
	fscanf(afile->f, "%*[^\n]");
	getc(afile->f);
	/*
*/
	if (!featureinlist(*V.featurelist, newfeature, &oldfeature)) {
	  V.f->next = newfeature;
	  V.f = V.f->next;
	  V.f->featurenumber = countfeatures;
	  uniquefeatures++;
	}


	else {
	  printf("This feature was found more than once:\n");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writeafeature(&TEMP, newfeature);
	  putchar('\n');

	  /*











*/




	  V.f->next = newfeature;
	  V.f = V.f->next;
	  V.f->featurenumber = countfeatures;

	  if (*KeepDuplicates) {
	    printf("It is being kept.\n");
	    V.f->duplicateprevious = NULL;
	    /*

*/
	  }

	  else {
	    printf("It will be ignored.\n");
	    V.f->duplicateprevious = oldfeature;
	    /*


*/

	  }
	}
      }

      threadgroup(&V);

      /*



*/
      continue;
    }

    if (identifier == 'g') {
      groupnumber++;
      V.ingroup = true;
      fscanf(afile->f, "%*[^\n]");
      /*


*/
      getc(afile->f);
    }

    else {
      printf(
	"features file lines must be blank or begin with one of \"*d@grbp\", not \"%c\".\n",
	identifier);
      halt();
    }
  }

  /*

*/
  if (*V.featurelist != NULL) {
    if ((*V.featurelist)->id.length == 0)
      emptyafile = true;
  }

  if (!emptyafile) {
    /*
*/
    if (*V.definitionlist != NULL) {
      d = *V.definitionlist;
      missingmatrix = false;
      while (d != NULL) {
	/*



*/
	if (d->background.length == 2) {
	  if (d->background.letters[1] == 'I' ||
	      d->background.letters[1] == 'i' ||
	      d->background.letters[1] == 'L' ||
	      d->background.letters[1] == 'l' ||
	      d->background.letters[1] == 'W' ||
	      d->background.letters[1] == 'w') {
	    if (d->matrix == NULL) {
	      missingmatrix = true;
	      printf("Definition:\n");
	      TEMP.f = stdout;
	      *TEMP.name = '\0';
	      writeadefinition(&TEMP, d);
	      printf("\nis missing a Ribl weight matrix\n");
	    }
	  }
	}
	d = d->next;
      }
      if (missingmatrix) {
	printf("readdefinitionsandfeatures: missingmatrix is true\n");
	halt();
      }
    }
  }

  printf("**************************************************\n");
  if (emptyafile) {
    printf("** Empty Features File ***************************\n");
    printf("** The features file may have characters in it, **\n");
    printf("** but it contains no features.                 **\n");
    printf("**************************************************\n");

    Free(*V.featurelist);
    *V.featurelist = NULL;
  } else {
    printf("%ld definition", countdefinitions);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    plural(&TEMP, countdefinitions);
    printf(" read in\n");

    printf("%ld unique definition", uniquedefinitions);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    plural(&TEMP, uniquedefinitions);
    printf(" found\n");

    printf("%ld definition", countribls);
    if (countribls == 1)
      printf(" has an Ribl matrix\n");
    else
      printf("s have Ribl matricies\n");

    printf("%ld definition", countboundaries);
    if (countboundaries == 1)
      printf(" boundry defined\n");
    else
      printf(" boundries defined\n");

    printf("%ld", countpetals);
    if (countpetals == 1)
      printf(" petal (color rectangle behind a walker) defined\n");
    else
      printf(" petals (color rectangles behind walkers) defined\n");

    printf("%ld feature", countfeatures);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    plural(&TEMP, countfeatures);
    printf(" read in\n");

    printf("%ld unique feature", uniquefeatures);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    plural(&TEMP, uniquefeatures);
    printf(" found\n");

    /*






*/



    if (false) {
      printf("\n******* @ end of readdefinitionsandfeatures: *******\n");
      printf("******* DEFINITIONS:\n");
      if (false) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showdefinitions(&TEMP, *V.definitionlist);
      }
      printf("\n******* FEATURES:\n");
      if (true) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showfeatures(&TEMP, *V.featurelist, true);
      }
      printf("\n******* @ end of readdefinitionsandfeatures: *******\n");
      /*

*/
    }

  }
  printf("**************************************************\n");
}







Static Void protectcharacter(c_, protectioncharacter, needed)
Char c_, *protectioncharacter;
boolean *needed;
{
  /*





*/
  if (c_ == '\\' || c_ == '%' || c_ == ')' || c_ == '(') {
    *protectioncharacter = '\\';
    *needed = true;
  } else {
    *protectioncharacter = ' ';
    *needed = false;
  }
}



Static Void protectpostscript(afile, c_)
_TEXT *afile;
Char c_;
{
  /*




*/
  boolean needed;
  Char protectionchar;

  protectcharacter(c_, &protectionchar, &needed);
  if (needed)
    putc(protectionchar, afile->f);
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



Static Void markup(logo, thecharacterwidth, cmperbit, position, marks,
		   marksymbol, marktype, markbase1, markbits1, markbase2,
		   markbits2, markscale, nextmark, waitfornextpiece,
		   marksorder)
_TEXT *logo;
double thecharacterwidth, cmperbit;
long position;
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

  while (position == *nextmark && !*waitfornextpiece) {
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



Static Void cosine(afile, amplitude, phase, wavelength, base_, xmin, ymin,
		   xmax, ymax, step, dashon, dashoff, dashoffset, thickness)
_TEXT *afile;
double amplitude, phase, wavelength, base_, xmin, ymin, xmax, ymax;
long step;
double dashon, dashoff, dashoffset, thickness;
{
  fprintf(afile->f, "%% amplitude  phase  wavelength  base:\n");
  fprintf(afile->f, " %*.*f cm %*.*f cm %*.*f cm %*.*f cm\n",
	  pwid, pdec, amplitude, pwid, pdec, phase, pwid, pdec, wavelength,
	  pwid, pdec, base_);
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
			 stacknumber, position, walkerdirection)
_TEXT *logo;
waveparam *wp;
double charwidth, cmperbit, barheight;
long stacknumber, position, walkerdirection;
{
  /*
*/
  /*

*/
  double xscale, yscale, xmin, ymin, xmax, ymax;
  waveparam *thiswave = wp;
  waveparam *WITH;

  while (thiswave != NULL) {
    WITH = thiswave;
    xscale = charwidth;
    yscale = cmperbit;
    xmin = (position - stacknumber + 1) * xscale;
    xmax = (position + 1) * xscale;
    ymin = 0.0;
    ymax = WITH->waveamplitude + barheight * yscale;

    fprintf(logo->f, "gsave\n");
    fprintf(logo->f, " %*.*f cm 0 translate\n",
	    pwid, pdec, -(position + 1) * charwidth);
    if (WITH->extreme == 'h') {
      /*

*/
      cosine(logo, yscale * WITH->waveamplitude,
	     walkerdirection * xscale * (WITH->wavelocation + 0.5),
	     xscale * WITH->wavelength,
	     yscale * (WITH->wavebit - WITH->waveamplitude), xmin, ymin, xmax,
	     ymax, 1L, WITH->dashon, WITH->dashoff, WITH->dashoffset,
	     WITH->thickness);
    } else
      cosine(logo, -yscale * WITH->waveamplitude,
	     walkerdirection * xscale * (WITH->wavelocation + 0.5),
	     xscale * WITH->wavelength,
	     yscale * (WITH->wavebit + WITH->waveamplitude), xmin, ymin, xmax,
	     ymax, 1L, WITH->dashon, WITH->dashoff, WITH->dashoffset,
	     WITH->thickness);

    fprintf(logo->f, "grestore\n");
    thiswave = thiswave->next;
  }
}











Static Void postscriptheader(a_, title, creator, llx, lly, urx, ury)
_TEXT *a_;
stringDelila title, creator;
double llx, lly, urx, ury;
{
  /*
*/
  fprintf(a_->f, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(a_->f, "%%%%Title: ");
  writestring(a_, &title);
  fprintf(a_->f, " %4.2f\n", version);
  fprintf(a_->f, "%%%%Creator: ");
  writestring(a_, &creator);
  fprintf(a_->f, "\n%%%%BoundingBox: %5ld %5ld %5ld %5ld\n",
	  (long)floor(llx + 0.5), (long)floor(lly + 0.5),
	  (long)floor(urx + 0.5), (long)floor(ury + 0.5));
  /*





*/
  fprintf(a_->f, "%%%%DocumentFonts:\n");
  fprintf(a_->f, "%%%%EndComments\n");

  /*



*/
  /*





*/

  fprintf(a_->f, "/defaultllx %5.1f def\n", defaultllx);
  fprintf(a_->f, "/defaultlly %5.1f def\n", defaultlly);
  fprintf(a_->f, "/defaulturx %5.1f def\n", defaulturx);
  fprintf(a_->f, "/defaultury %5.1f def\n", defaultury);
  fprintf(a_->f, "/llx %5.1f def\n", llx);
  fprintf(a_->f, "/lly %5.1f def\n", lly);
  fprintf(a_->f, "/urx %5.1f def\n", urx);
  fprintf(a_->f, "/ury %5.1f def\n", ury);
}



Static Void makepageedges(a_)
_TEXT *a_;
{
  /*


*/
  fprintf(a_->f, "/makepageedges {%%\n");
  fprintf(a_->f,
	  "%% draw a red line showing page edges based on the current\n");
  fprintf(a_->f, "%% page boundaries defined by\n");
  fprintf(a_->f, "%% defaultllx, defaultlly, defaulturx and defaultury\n\n");
  fprintf(a_->f, "/Xscreenupper 600 def\n");
  fprintf(a_->f, "/Yscreenupper 800 def\n\n");
  fprintf(a_->f, "/newarea{\n");
  fprintf(a_->f, "%% make a clip path around the area\n");
  fprintf(a_->f, "0 0 moveto\n");
  fprintf(a_->f, "defaultllx defaultlly lineto\n");
  fprintf(a_->f, "defaulturx defaultlly lineto\n");
  fprintf(a_->f, "defaulturx defaultury lineto\n");
  fprintf(a_->f, "defaultllx defaultury lineto\n");
  fprintf(a_->f, "defaultllx defaultlly lineto\n");
  fprintf(a_->f, "0 0 lineto\n");
  fprintf(a_->f, "0 Yscreenupper lineto\n");
  fprintf(a_->f, "Xscreenupper Yscreenupper lineto\n");
  fprintf(a_->f, "Xscreenupper 0 lineto\n");
  fprintf(a_->f, "0 0 lineto\n");
  fprintf(a_->f, "clip\n");
  fprintf(a_->f, "} def\n\n");
  fprintf(a_->f, "/printedge 2 def\n\n");
  fprintf(a_->f, "/bigger{\n");
  fprintf(a_->f, "/defaultllx defaultllx printedge sub def\n");
  fprintf(a_->f, "/defaultlly defaultlly printedge sub def\n");
  fprintf(a_->f, "/defaulturx defaulturx printedge add def\n");
  fprintf(a_->f, "/defaultury defaultury printedge add def\n");
  fprintf(a_->f, "} def\n\n");
  fprintf(a_->f, "gsave\n");
  fprintf(a_->f, "  newarea\n");
  fprintf(a_->f, "  1 0 0 setrgbcolor\n");
  fprintf(a_->f, "  fill\n");
  fprintf(a_->f, "grestore\n\n");
  fprintf(a_->f, "gsave\n");
  fprintf(a_->f, "  bigger\n");
  fprintf(a_->f, "  newarea\n");
  fprintf(a_->f, "  1 setgray\n");
  fprintf(a_->f, "  fill\n");
  fprintf(a_->f, "grestore\n\n");
  fprintf(a_->f, "} def\n");
}


#define ordControlH     8



Static Void digitcounter(afile, counter, previous)
_TEXT *afile;
long *counter, *previous;
{
  /*





*/
  /*

*/
  long digits, digitcount;

  if (*previous > 0) {
    /*

*/
    digits = (long)(log(*previous + 0.5) / log(10.0) + 1);
    /*


*/
    for (digitcount = 1; digitcount <= digits; digitcount++)
      putchar((Char)ordControlH);
    /*

*/
  }

  printf("%ld", *counter);
  *previous = *counter;
}

#undef ordControlH



Static Void TESTdigitcounter(maxcounter)
long maxcounter;
{
  long counter = 0;
  long previouscounter;
  _TEXT TEMP;

  printf("counter number: ");
  while (counter < maxcounter) {
    counter++;
    TEMP.f = stdout;
    *TEMP.name = '\0';
    digitcounter(&TEMP, &counter, &previouscounter);
  }
  printf(" Voila!\n");
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
    "* https://alum.mit.edu/www/toms/Source_Code_Use_License_Schneider.txt *\n");
  fprintf(V.f->f,
    "* Contact: https://alum.mit.edu/www/toms/contacts.html                *\n");
  makebar(&V);
}






Static Void help()
{
  printf("* help for prgmod\n");
  printf("* prgmod is a library of modules.  each module does\n");
  printf("* a particular thing that is useful for programming\n");
  printf("* in various circumstances.  (hence the name prgmod.)\n");
  printf("*\n");
  printf("* prgmod is also a program, set up to test the modules\n");
  printf("* and to give you an idea of what the modules do.\n");
  printf("* there are four things to test and play with:\n");
  printf("* 1. interactive input and output.  you may type any\n");
  printf("*    sentence you want to, and prgmod will find\n");
  printf("*    out the words in the sentence and show them to\n");
  printf("*    you.  then it will do step 2:\n");
  printf("* 2. the characters on the line will be sorted with a\n");
  printf("*    very fast algorithm called a quicksort.  this\n");
  printf("*    sort procedure is useful for sorting arrays.\n");
  printf("* 3. if any word that you type begins with the letter n,\n");
  printf("*    then the next two words are expected to be integers\n");
  printf("*    for the beginning and end of a \"numbar\": a set of\n");
  printf("*    numbers written vertically.  these are quite handy\n");
  printf("*    for labeling the axis of a graph.  you may ask for\n");
  printf("*    several numbars at once.  for example, type:\n");
  printf("   numbar -5 15 n 90 105\n");
  printf("*    to see that the program detects errors, try:\n");
  printf("   not so good\n");
  printf("*    and also:\n");
  printf("   n 5 -5\n");
  printf("* 4. if a word begins with a c, then you will be asked\n");
  printf("*    for the radius of a circle - for testing real numbers\n");
  printf("* 5. if # is given, the numberstring routine is tested\n");
  printf("* 6. if p is given, the patent message is given.\n");
  printf("* 7. if k 1000 is given, a counter will count to 1000.\n");
  printf("*\n");
  printf("* the only other commands are help and quit the program.\n");
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  list.f = NULL;
  strcpy(list.name, "list");
  printf("* prgmod %4.2f\n", version);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  PatentMessage(&TEMP);
  printf("* Start interaction by typing a Return.\n");
  printf("* Type \"help\" for instructions.\n");

  clearstring(&buffer);
  do {
    newline = false;
    do {
      if (nostring(&buffer)) {
	printf("* prgmod\n");
	newline = true;
      }
      TEMP.f = stdin;
      *TEMP.name = '\0';
      getchar_(&TEMP, &buffer, &control, &gotten);
    } while (!gotten);

    if (newline) {
      printf("* %ld characters\n", buffer.length);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &buffer);
      putchar('\n');



      ourplace = buffer.current;
      buffer.current = 1;
      printf("* tokens on the line are:\n");
      gotten = true;
      while (buffer.current <= buffer.length && gotten == true) {
	TEMP.f = stdin;
	*TEMP.name = '\0';
	gettoken(&TEMP, &buffer, &atoken, &gotten);
	if (!gotten)
	  break;
	putchar('(');
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &atoken);
	putchar(')');
      }

      putchar('\n');
      buffer.current = ourplace;


      /*
*/
      printf("* sort the line:\n");
      setdemo(buffer, map);
      quicksort(1L, buffer.length);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      print(&TEMP, buffer, map);
    }

    if (control == 'n') {
      do {
	if (nostring(&buffer))
	  printf("* numbar: type the first number\n");
	TEMP.f = stdin;
	*TEMP.name = '\0';
	getinteger(&TEMP, &buffer, &first, &gotten);
      } while (!gotten);
      do {
	if (nostring(&buffer))
	  printf("* numbar: type the second number\n");
	TEMP.f = stdin;
	*TEMP.name = '\0';
	getinteger(&TEMP, &buffer, &last, &gotten);
	if (gotten) {
	  if (last < first) {
	    gotten = false;
	    printf("the second number must be greater than");
	    printf(" or equal to the first (%ld).\n", first);
	  }
	}
      } while (!gotten);


      spaces = 0;
      printf("\n* the numbar between %ld and %ld is:\n", first, last);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      numberbar(&TEMP, spaces, first, last, &lines);
      printf("* %ld lines used to print the numbar.\n", lines);
    }


    if (control == 'h')
      help();

    if (control == 'c') {
      do {
	if (nostring(&buffer))
	  printf("radius:\n");
	TEMP.f = stdin;
	*TEMP.name = '\0';
	getreal(&TEMP, &buffer, &radius, &gotten);
      } while (!gotten);
      if (radius < 0)
	printf("a negative radius is silly.\n");
      else
	printf("the area of a circle of radius %5.2f is %5.2f\n",
	       radius, 3.14159 * radius * radius);
    }

    if (control == '#') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      numberstringtest(&TEMP);
    }

    if (control == 'p') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      PatentMessage(&TEMP);
    }

    if (control == 'k') {
      do {
	if (nostring(&buffer))
	  printf("* counter: type the a number\n");
	TEMP.f = stdin;
	*TEMP.name = '\0';
	getinteger(&TEMP, &buffer, &first, &gotten);
      } while (!gotten);

      TESTdigitcounter(first);
    }


  } while (control != 'q');
  printf("quit\n");
_L1:
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
