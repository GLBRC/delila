/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "denplo.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         2.22
/*





























































































































*/
#define updateversion   2.16



/*






















































































































































































































































































































































































*/



#define minValue        0
#define maxValue        1000


#define defaultllx      7.10999
#define defaultlly      7.01995
#define defaulturx      588.15
#define defaultury      784.98

#define komment         '#'




#define maxstring       300



#define pi              3.14159265354

#define picwidth        8
#define picdecim        5

#define charwidth       0.15875
/*
*/
/*
*/
#define dotfactor       0.015875
/*

*/
#define defscale        28.35


/*
*/





typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;



/*
*/
typedef long binsarray[maxValue - minValue + 1][maxValue - minValue + 1];


Static _TEXT data, denplop, denploxyin, denploxyplom, denploxyplop;


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






Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == '\t');
}


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


#define escape          '\\'

#define infofield       8
#define infodecim       5

#define protecting      true


#define debug           false


/* Local variables for postscriptstring: */
struct LOC_postscriptstring {
  stringDelila *instring, *outstring, *rawstring;
  double rs, sd;
  long n;
  /*
*/

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
  /*


*/
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
  /*

*/
  long num, maxcoo = -LONG_MAX, maxnum = -LONG_MAX;
  /*


*/
  long desiredcoo;
  /*

*/
  long s;

  getnum(&desiredcoo, LINK);



  /*







































*/

  num = LINK->n;


  for (s = numbersize(num); s >= 0; s--) {
    c = numberdigit(num, s);
    if (c != ' ' && c != '+')
      putchar_(c, LINK);
  }

  LINK->n = num;
  /*

*/
  printf("* %ld sequences found at coordinate ,coo:1\n", LINK->n);
}

/*



















































*/

Local Void putreal(r, dec, LINK)
double r;
long dec;
struct LOC_postscriptstring *LINK;
{
  /*




*/
  Char c;
  long m, num, s;

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
    /*
*/
    if (c == ' ')
      c = '0';
    if (c != '+')
      putchar_(c, LINK);
  }
}

#undef debug

Local Void dors(LINK)
struct LOC_postscriptstring *LINK;
{
  /*

*/
  putreal(LINK->rs, LINK->decimals, LINK);
  printf("* Rsequence for logo:%*.*f\n",
	 infofield, (int)LINK->decimals, LINK->rs);
}

Local Void dosd(LINK)
struct LOC_postscriptstring *LINK;
{
  /*

*/
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







Static Void postscriptstring(instring_, outstring_, rawstring_, rs_, sd_, n_)
stringDelila *instring_, *outstring_, *rawstring_;
double rs_, sd_;
long n_;
{
  /*


*/
  /*
*/
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
  V.rs = rs_;
  V.sd = sd_;
  V.n = n_;
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
  /*




*/
  V.insidestring = true;

  V.i = 0;
  while (V.i < V.instring->length) {
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
      } else
	putchar_(V.curr, &V);
    }



    if (V.prev == escape && V.curr == '(') {
      if (V.insidestring)
	badstring(&V);
      V.insidestring = true;
    }
  }

  if (V.insidestring)
    V.insidestring = false;
  else {
    printf("This string:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, V.instring);
    printf("\nis bad because it needs a final \"\\(\".\n");
    /*


*/
    halt();
  }


  if (V.italic)
    doitalics(&V);

  /*

*/
}

#undef escape
#undef infofield
#undef infodecim
#undef protecting


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


#define copylines       27


#define copylines_      28


#define copylines__     29


#define copylines___    31


/*

*/

#define debugging       true


#define debugging_      false


#define debugging__     true


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *data;
  Char background;
  double grey;

  Char cutofftype;
  /*
*/
  double cutoffvaluemin, cutoffvaluemax;

  /*


*/
  long (*bins)[maxValue - minValue + 1],
       (*smoothingbins)[maxValue - minValue + 1],
       (*swapbins)[maxValue - minValue + 1];
  double ysums[maxValue - minValue + 1];
  long ycounts[maxValue - minValue + 1];

  long numbertoprocess;

  double parameterversion;

  long NuWi, NuDe;

  double XminValue, XmaxValue, Xrange;
  long Xintervals;

  double YminValue, YmaxValue, Yrange;
  long Yintervals;

  long Xcolumn, Ycolumn;
  boolean xlessthany;

  double Xsize, Ysize;

  double Xcorner, Ycorner;

  long XDisplayIntervals, YDisplayIntervals;
  /*
*/

  long XDisplaySubIntervals, YDisplaySubIntervals;
  /*
*/

  long xwidth, ywidth, xdecimal, ydecimal;

  double Xvalue, Yvalue;

  long dataCount;

  long inCount, outCount;

  long XinCount, XoutCount;

  long YinCount, YoutCount;

  boolean Xin, Yin;

  long Xbin, Ybin;

  long Xlo, Ylo;

  long Xhi, Yhi;

  double SymbolSizeX, SymbolSizeY, shrinkfactorX, shrinkfactorY;

  double jumpX, jumpY;

  double shiftX, shiftY;
  /*
*/

  stringDelila XaxisLabel, YaxisLabel;

  Char cog;

  long BinMax;


  Char keytype;
  double keyX, keyY, keyXsize, keyYsize;
  long keydecimals, keyintervals;
  double keyshrinkfactor, keyfontsize, thelinewidth;

  /*
*/
  double llx, lly, urx, ury;

  Char edgecontrol;
  double edgeleft, edgelow, edgeright, edgehigh;

  Char findpeak;
  double startX, startY;
  long SearchRange;
  /*
*/


  Char yaverage;
  double shrinkYav;
  /*
*/



  boolean logxscale, logxnormal;
  double xbase, logxbase;
  /*
*/
  boolean logyscale, logynormal;
  double ybase, logybase;
  /*
*/

  Char smoothkind;
  double smoothsize;
  long smoothrange;
} ;

/*
*/


Local Void upgradefrom166to188(denplop, LINK)
_TEXT *denplop;
struct LOC_themain *LINK;
{
  /*

*/
  _TEXT internal;
  long line;
  double parameterversion = 1.88;

  internal.f = NULL;
  *internal.name = '\0';
  printf("upgrading to version %4.2f ...\n", parameterversion);


  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "r", denplop->f);
    else
      denplop->f = fopen(denplop->name, "r");
  } else
    rewind(denplop->f);
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  RESETBUF(denplop->f, Char);
  fscanf(denplop->f, "%*[^\n]");
  getc(denplop->f);
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  for (line = 1; line <= copylines; line++)
    copyaline(denplop, &internal);


  fprintf(internal.f,
	  "- 0 0 1 28. findpeak(l), startX, startY, SearchRange\n");



  copyfile(denplop, &internal);


  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "w", denplop->f);
    else
      denplop->f = fopen(denplop->name, "w");
  } else {
    if (denplop->f != NULL)
      rewind(denplop->f);
    else
      denplop->f = tmpfile();
  }
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  SETUPBUF(denplop->f, Char);
  fprintf(denplop->f,
    "%4.2f        version of denplop that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, denplop);




  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "r", denplop->f);
    else
      denplop->f = fopen(denplop->name, "r");
  } else
    rewind(denplop->f);
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  RESETBUF(denplop->f, Char);

  if (internal.f != NULL)
    fclose(internal.f);
}

#undef copylines



Local Void upgradefrom188to201(denplop, LINK)
_TEXT *denplop;
struct LOC_themain *LINK;
{
  /*

*/
  _TEXT internal;
  long line;
  double parameterversion = 2.01;

  internal.f = NULL;
  *internal.name = '\0';
  printf("upgrading to version %4.2f ...\n", parameterversion);


  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "r", denplop->f);
    else
      denplop->f = fopen(denplop->name, "r");
  } else
    rewind(denplop->f);
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  RESETBUF(denplop->f, Char);
  fscanf(denplop->f, "%*[^\n]");
  getc(denplop->f);
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  for (line = 1; line <= copylines_; line++)
    copyaline(denplop, &internal);


  /*

*/



  copyfile(denplop, &internal);


  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "w", denplop->f);
    else
      denplop->f = fopen(denplop->name, "w");
  } else {
    if (denplop->f != NULL)
      rewind(denplop->f);
    else
      denplop->f = tmpfile();
  }
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  SETUPBUF(denplop->f, Char);
  fprintf(denplop->f,
    "%4.2f        version of denplop that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, denplop);

  /*




*/


  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "r", denplop->f);
    else
      denplop->f = fopen(denplop->name, "r");
  } else
    rewind(denplop->f);
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  RESETBUF(denplop->f, Char);

  if (internal.f != NULL)
    fclose(internal.f);
}

#undef copylines_




Local Void upgradefrom201to210(denplop, LINK)
_TEXT *denplop;
struct LOC_themain *LINK;
{
  /*

*/
  _TEXT internal;
  long line;
  double parameterversion = 2.10;

  internal.f = NULL;
  *internal.name = '\0';
  printf("upgrading to version %4.2f ...\n", parameterversion);


  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "r", denplop->f);
    else
      denplop->f = fopen(denplop->name, "r");
  } else
    rewind(denplop->f);
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  RESETBUF(denplop->f, Char);
  fscanf(denplop->f, "%*[^\n]");
  getc(denplop->f);
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  for (line = 1; line <= copylines__; line++)
    copyaline(denplop, &internal);


  /*

*/



  copyfile(denplop, &internal);


  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "w", denplop->f);
    else
      denplop->f = fopen(denplop->name, "w");
  } else {
    if (denplop->f != NULL)
      rewind(denplop->f);
    else
      denplop->f = tmpfile();
  }
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  SETUPBUF(denplop->f, Char);
  fprintf(denplop->f,
    "%4.2f        version of denplop that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, denplop);


  fprintf(denplop->f, "n 2     30. zxl (char), xbase (integer)\n");
  fprintf(denplop->f, "n 2     31. zyl (char), ybase (integer)\n");

  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "r", denplop->f);
    else
      denplop->f = fopen(denplop->name, "r");
  } else
    rewind(denplop->f);
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  RESETBUF(denplop->f, Char);

  if (internal.f != NULL)
    fclose(internal.f);
}

#undef copylines__



Local Void upgradefrom210to216(denplop, LINK)
_TEXT *denplop;
struct LOC_themain *LINK;
{
  /*

*/
  _TEXT internal;
  long line;
  double parameterversion = 2.16;

  internal.f = NULL;
  *internal.name = '\0';
  printf("upgrading to version %4.2f ...\n", parameterversion);


  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "r", denplop->f);
    else
      denplop->f = fopen(denplop->name, "r");
  } else
    rewind(denplop->f);
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  RESETBUF(denplop->f, Char);
  fscanf(denplop->f, "%*[^\n]");
  getc(denplop->f);
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  for (line = 1; line <= copylines___; line++)
    copyaline(denplop, &internal);


  /*

*/



  copyfile(denplop, &internal);


  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "w", denplop->f);
    else
      denplop->f = fopen(denplop->name, "w");
  } else {
    if (denplop->f != NULL)
      rewind(denplop->f);
    else
      denplop->f = tmpfile();
  }
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  SETUPBUF(denplop->f, Char);
  fprintf(denplop->f,
    "%4.2f        version of denplop that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, denplop);


  fprintf(denplop->f,
	  "- 2     32. smoothkind (char) \"-s\", smoothsize (integer)\n");

  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "r", denplop->f);
    else
      denplop->f = fopen(denplop->name, "r");
  } else
    rewind(denplop->f);
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  RESETBUF(denplop->f, Char);

  if (internal.f != NULL)
    fclose(internal.f);
}

#undef copylines___

/*


*/

Local long r100(a)
double a;
{
  return ((long)floor(100 * a + 0.5));
}

Local boolean equalversion(a, b)
double a, b;
{
  if (r100(a) == r100(b))
    return true;
  else
    return false;
}

Local boolean lessthan(a, b)
double a, b;
{
  if (r100(a) < r100(b))
    return true;
  else
    return false;
}

Local boolean greaterthan(a, b)
double a, b;
{
  if (r100(a) > r100(b))
    return true;
  else
    return false;
}



Local Void upgradeparameters(denplop, LINK)
_TEXT *denplop;
struct LOC_themain *LINK;
{
  double parameterversion;

  fscanf(denplop->f, "%lg%*[^\n]", &parameterversion);
  getc(denplop->f);
  if (!lessthan(parameterversion, updateversion))
    return;
  /*

*/

  printf("^GYou have an old parameter file!, version %4.2f!\n",
	 parameterversion);
  printf("          version = %4.2f\n", version);
  printf("    updateversion = %4.2f\n", updateversion);
  printf(" parameterversion = %4.2f\n", parameterversion);
  /*

*/

  if (equalversion(parameterversion, 1.66))
    upgradefrom166to188(denplop, LINK);
  if (lessthan(parameterversion, 2.01))
    upgradefrom188to201(denplop, LINK);

  if (lessthan(parameterversion, 2.10))
    upgradefrom201to210(denplop, LINK);

  if (lessthan(parameterversion, 2.11))
    upgradefrom210to216(denplop, LINK);


  if (*denplop->name != '\0') {
    if (denplop->f != NULL)
      denplop->f = freopen(denplop->name, "r", denplop->f);
    else
      denplop->f = fopen(denplop->name, "r");
  } else
    rewind(denplop->f);
  if (denplop->f == NULL)
    _EscIO2(FileNotFound, denplop->name);
  RESETBUF(denplop->f, Char);
  fscanf(denplop->f, "%lg%*[^\n]", &parameterversion);
  getc(denplop->f);
  if (lessthan(parameterversion, updateversion)) {
    printf("Sorry! I am unable to fully upgrade your parameter file\n");
    printf("from version %4.2f to version %4.2f!\n",
	   parameterversion, updateversion);
    printf("Start from a fresh copy or edit this one.\n");
    halt();
  } else
    printf("... upgrade successful!\n");

  printf("See this page for the new documentation:\n");
  printf("https://alum.mit.edu/www/toms/delila/denplo.html\n");
}

/* Local variables for readparameters: */
struct LOC_readparameters {
  struct LOC_themain *LINK;
  _TEXT *denplop;
  boolean checkout;
} ;

Local Void cn(LINK)
struct LOC_readparameters *LINK;
{
  LINK->checkout = checknumber(LINK->denplop);
  if (LINK->checkout)
    return;
  printf("denplo readparameters: halt to avoid snoballing\n");
  printf("A number was expected but not found.\n");
  if (BUFEOF(LINK->denplop->f))
    printf("Unexpected End of File\n");
  else if (P_eoln(LINK->denplop->f))
    printf("Unexpected End Of Line\n");
  else
    printf("The bad character in denplop is: \"%c\"\n",
	   P_peek(LINK->denplop->f));
  halt();
}

Local Void t(LINK)
struct LOC_readparameters *LINK;
{
  if (BUFEOF(LINK->denplop->f)) {
    printf("Missing parameters: unexpected end of denplop\n");
    halt();
  }
}


Local Void readparameters(denplop_, LINK)
_TEXT *denplop_;
struct LOC_themain *LINK;
{
  struct LOC_readparameters V;
  boolean gotten;

  V.LINK = LINK;
  V.denplop = denplop_;
  if (*V.denplop->name != '\0') {
    if (V.denplop->f != NULL)
      V.denplop->f = freopen(V.denplop->name, "r", V.denplop->f);
    else
      V.denplop->f = fopen(V.denplop->name, "r");
  } else
    rewind(V.denplop->f);
  if (V.denplop->f == NULL)
    _EscIO2(FileNotFound, V.denplop->name);
  RESETBUF(V.denplop->f, Char);
  cn(&V);
  upgradeparameters(V.denplop, LINK);

  cn(&V);
  fscanf(V.denplop->f, "%ld%*[^\n]", &LINK->numbertoprocess);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%lg%ld%*[^\n]", &LINK->XminValue, &LINK->XmaxValue,
	 &LINK->Xintervals);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%lg%ld%*[^\n]", &LINK->YminValue, &LINK->YmaxValue,
	 &LINK->Yintervals);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%ld%ld%*[^\n]", &LINK->NuWi, &LINK->NuDe);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%ld%ld%*[^\n]", &LINK->Xcolumn, &LINK->Ycolumn);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%lg%*[^\n]", &LINK->Xsize, &LINK->Ysize);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%lg%*[^\n]", &LINK->Xcorner, &LINK->Ycorner);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%ld%ld%ld%ld%*[^\n]", &LINK->XDisplayIntervals,
	 &LINK->YDisplayIntervals, &LINK->XDisplaySubIntervals,
	 &LINK->YDisplaySubIntervals);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%ld%ld%*[^\n]", &LINK->xwidth, &LINK->ywidth);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%ld%ld%*[^\n]", &LINK->xdecimal, &LINK->ydecimal);
  getc(V.denplop->f);

  LINK->background = getc(V.denplop->f);
  if (LINK->background == '\n')
    LINK->background = ' ';
  if (LINK->background == 'g')
    fscanf(V.denplop->f, "%lg", &LINK->grey);
  else
    LINK->grey = 0.9;
  fscanf(V.denplop->f, "%*[^\n]");

  getc(V.denplop->f);
  fscanf(V.denplop->f, "%c%lg", &LINK->cutofftype, &LINK->cutoffvaluemin);
  if (LINK->cutofftype == '\n')
    LINK->cutofftype = ' ';
  if (LINK->cutofftype == 'C' || LINK->cutofftype == 'F')
    fscanf(V.denplop->f, "%lg", &LINK->cutoffvaluemax);
  else
    LINK->cutoffvaluemax = LONG_MAX;
  fscanf(V.denplop->f, "%*[^\n]");

  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%lg%*[^\n]", &LINK->shrinkfactorX,
	 &LINK->shrinkfactorY);
  getc(V.denplop->f);

  getstring(V.denplop, &LINK->XaxisLabel, &gotten);
  if (!gotten) {
    printf("end of file found when reading X axis label\n");
    halt();
  }

  getstring(V.denplop, &LINK->YaxisLabel, &gotten);
  if (!gotten) {
    printf("end of file found when reading X axis label\n");
    halt();
  }

  fscanf(V.denplop->f, "%c%*[^\n]", &LINK->cog);
  getc(V.denplop->f);

  if (LINK->cog == '\n')
    LINK->cog = ' ';
  cn(&V);
  fscanf(V.denplop->f, "%ld%*[^\n]", &LINK->keyintervals);
  getc(V.denplop->f);
  fscanf(V.denplop->f, "%c%*[^\n]", &LINK->keytype);
  getc(V.denplop->f);
  if (LINK->keytype == '\n')
    LINK->keytype = ' ';
  cn(&V);
  fscanf(V.denplop->f, "%lg%*[^\n]", &LINK->keyX);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%*[^\n]", &LINK->keyY);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%*[^\n]", &LINK->keyXsize);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%*[^\n]", &LINK->keyYsize);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%*[^\n]", &LINK->keyshrinkfactor);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%*[^\n]", &LINK->thelinewidth);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%ld%*[^\n]", &LINK->keydecimals);
  getc(V.denplop->f);
  cn(&V);
  fscanf(V.denplop->f, "%lg%*[^\n]", &LINK->keyfontsize);
  getc(V.denplop->f);


  LINK->llx = defaultllx;
  LINK->ury = defaultury;
  LINK->urx = defaulturx;
  LINK->lly = defaultlly;

  LINK->edgecontrol = getc(V.denplop->f);
  if (LINK->edgecontrol == '\n')
    LINK->edgecontrol = ' ';
  if (LINK->edgecontrol == 'p') {
    cn(&V);
    fscanf(V.denplop->f, "%lg", &LINK->edgeleft);
    cn(&V);
    fscanf(V.denplop->f, "%lg", &LINK->edgeright);
    cn(&V);
    fscanf(V.denplop->f, "%lg", &LINK->edgelow);
    cn(&V);
    fscanf(V.denplop->f, "%lg", &LINK->edgehigh);


    /*



*/


    LINK->llx = (long)floor((LINK->Xcorner - LINK->edgeleft) * defscale + 0.5);
    LINK->lly = (long)floor((LINK->Ycorner - LINK->edgelow) * defscale + 0.5);
    LINK->urx = (long)floor(
	  (LINK->Xcorner + LINK->Xsize + LINK->edgeright) * defscale + 0.5);
    LINK->ury = (long)floor(
	  (LINK->Ycorner + LINK->Xsize + LINK->edgehigh) * defscale + 0.5);
  }
  fscanf(V.denplop->f, "%*[^\n]");


  getc(V.denplop->f);
  LINK->findpeak = getc(V.denplop->f);
  if (LINK->findpeak == '\n')
    LINK->findpeak = ' ';
  if (LINK->findpeak == 'l') {
    cn(&V);
    fscanf(V.denplop->f, "%lg", &LINK->startX);
    cn(&V);
    fscanf(V.denplop->f, "%lg", &LINK->startY);
    cn(&V);
    fscanf(V.denplop->f, "%ld", &LINK->SearchRange);
  } else if (LINK->findpeak == 'm') {
    cn(&V);
    fscanf(V.denplop->f, "%ld", &LINK->BinMax);
    LINK->startX = 0.0;
    LINK->startY = 0.0;
    LINK->SearchRange = 0;
  } else {
    LINK->startX = 0.0;
    LINK->startY = 0.0;
    LINK->SearchRange = 0;
  }
  fscanf(V.denplop->f, "%*[^\n]");



  getc(V.denplop->f);
  fscanf(V.denplop->f, "%c%lg%*[^\n]", &LINK->yaverage, &LINK->shrinkYav);
  getc(V.denplop->f);



  if (LINK->yaverage == '\n')
    LINK->yaverage = ' ';
  LINK->logxscale = false;
  LINK->logxnormal = false;
  LINK->xbase = 2.0;
  LINK->logxbase = log(LINK->xbase);
  t(&V);
  if (!P_eoln(V.denplop->f)) {
    if ((P_peek(V.denplop->f) == 'l') | (P_peek(V.denplop->f) == 'L')) {
      LINK->logxscale = true;
      if (P_peek(V.denplop->f) == 'L')
	LINK->logxnormal = true;
      skipnonblanks(V.denplop);
      cn(&V);
      fscanf(V.denplop->f, "%lg", &LINK->xbase);
      if (LINK->xbase <= 1.0) {
	printf("log x base must be > 1\n");
	halt();
      }
      LINK->logxbase = log(LINK->xbase);
    }
  }
  fscanf(V.denplop->f, "%*[^\n]");


  getc(V.denplop->f);
  LINK->logyscale = false;
  LINK->logynormal = false;
  LINK->ybase = 2.0;
  LINK->logybase = log(LINK->ybase);
  t(&V);
  if (!P_eoln(V.denplop->f)) {
    if ((P_peek(V.denplop->f) == 'l') | (P_peek(V.denplop->f) == 'L')) {
      LINK->logyscale = true;
      if (P_peek(V.denplop->f) == 'L')
	LINK->logynormal = true;
      skipnonblanks(V.denplop);
      cn(&V);
      fscanf(V.denplop->f, "%lg", &LINK->ybase);
      if (LINK->ybase <= 1.0) {
	printf("log y base must be > 1\n");
	halt();
      }
      LINK->logybase = log(LINK->ybase);
    }
  }
  fscanf(V.denplop->f, "%*[^\n]");

  getc(V.denplop->f);
  LINK->smoothkind = getc(V.denplop->f);
  if (LINK->smoothkind == '\n')
    LINK->smoothkind = ' ';
  cn(&V);
  fscanf(V.denplop->f, "%lg%*[^\n]", &LINK->smoothsize);
  getc(V.denplop->f);



  if (LINK->XmaxValue <= LINK->XminValue) {
    printf("XmaxValue (%*.*f) cannot be less than XminValue (%*.*f)\n",
	   (int)LINK->NuWi, (int)LINK->NuDe, LINK->XmaxValue, (int)LINK->NuWi,
	   (int)LINK->NuDe, LINK->XminValue);
    halt();
  }

  if (LINK->YmaxValue <= LINK->YminValue) {
    printf("YmaxValue (%*.*f) cannot be less than YminValue (%*.*f)\n",
	   (int)LINK->NuWi, (int)LINK->NuDe, LINK->YmaxValue, (int)LINK->NuWi,
	   (int)LINK->NuDe, LINK->YminValue);
    halt();
  }

  if (LINK->Xintervals < 1) {
    printf("Xintervals (=%ld) cannot be less than 2\n", LINK->Xintervals);
    halt();
  }

  if (LINK->Yintervals < 1) {
    printf("Yintervals (=%ld) cannot be less than 2\n", LINK->Yintervals);
    halt();
  }

  if (LINK->NuWi < 1) {
    printf("NuWi must be positive\n");
    halt();
  }

  if (LINK->NuDe < 1) {
    printf("NuDe must be positive\n");
    halt();
  }

  /*





*/

  if (LINK->NuDe < 5) {
    printf("NuDe must be at least 5 decimal places\n");
    halt();
  }

  if (LINK->Xcolumn < 1) {
    printf("Xcolumn must be positive\n");
    halt();
  }

  if (LINK->Ycolumn < 1) {
    printf("Ycolumn must be positive\n");
    halt();
  }

  if (LINK->Xcolumn == LINK->Ycolumn) {
    printf("X column cannot equal Ycolumn\n");
    halt();
  }

  if (LINK->Xsize <= 0.0) {
    printf("Xsize must be positive\n");
    halt();
  }

  if (LINK->Ysize <= 0.0) {
    printf("Ysize must be positive\n");
    halt();
  }

  if (LINK->XDisplayIntervals < 0) {
    printf("XDisplayIntervals must be zero or positive it is %ld\n",
	   LINK->XDisplayIntervals);
    halt();
  }

  if (LINK->YDisplayIntervals < 0) {
    printf("YDisplayIntervals must be zero or positive it is %ld\n",
	   LINK->YDisplayIntervals);
    halt();
  }

  if (LINK->XDisplaySubIntervals <= 0) {
    printf("XDisplaySubIntervals must be positive\n");
    halt();
  }

  if (LINK->YDisplaySubIntervals <= 0) {
    printf("YDisplaySubIntervals must be positive\n");
    halt();
  }

  if (LINK->xwidth <= 1) {
    printf("xwidth must be positive\n");
    halt();
  }

  if (LINK->ywidth <= 1) {
    printf("ywidth must be positive\n");
    halt();
  }

  if (LINK->xdecimal < 0) {
    printf("xdecimal must be positive or zero\n");
    halt();
  }

  if (LINK->ydecimal < 0) {
    printf("ydecimal must be positive or zero\n");
    halt();
  }

  if (LINK->background != 'c' && LINK->background != 'w' &&
      LINK->background != 'g' && LINK->background != 'b') {
    printf("background must be one of bgwc\n");
    halt();
  }

  if ((unsigned)LINK->grey > 1.0) {
    printf("grey must be between 0 and 1\n");
    halt();
  }

  if (LINK->cutofftype != 'F' && LINK->cutofftype != 'C' &&
      LINK->cutofftype != 'f' && LINK->cutofftype != 'c') {
    printf("cutofftype must be one of \"cfCF\"\n");
    halt();
  }

  if (LINK->cutoffvaluemin < 0) {
    printf("cutoffvaluemin (%1.1f) must be positive or zero\n",
	   LINK->cutoffvaluemin);
    halt();
  }

  if (LINK->cutoffvaluemax < 0) {
    printf("cutoffvaluemax (%1.1f) must be positive or zero\n",
	   LINK->cutoffvaluemax);
    halt();
  }

  if (LINK->cutofftype == 'f' || LINK->cutofftype == 'F') {
    if ((unsigned)LINK->cutoffvaluemin > 1.00) {
      printf("for cutofftype = \"f\", cutoffvaluemin must be between 0 and 1\n");
      halt();
    }
  }

  if (LINK->cutofftype == 'f' || LINK->cutofftype == 'F') {
    if ((unsigned)LINK->cutoffvaluemax > 1.00) {
      printf("for cutofftype = \"f\", cutoffvaluemax must be between 0 and 1\n");
      halt();
    }
  }

  if (LINK->cutofftype == 'C' || LINK->cutofftype == 'F') {
    if (LINK->cutoffvaluemin > LINK->cutoffvaluemax) {
      printf("for cutoffvaluemin must be less than or equal to cutoffvaluemax \n");
      halt();
    }
  }

  /*








*/


  if (LINK->shrinkfactorX <= 0) {
    printf("shrinkfactorX must be > 0\n");
    halt();
  }

  if (LINK->shrinkfactorY <= 0) {
    printf("shrinkfactorY must be > 0\n");
    halt();
  }

  if (LINK->cog != 'g' && LINK->cog != 'c') {
    printf("cog (color or grey) must be one of \"cg\"\n");
    halt();
  }

  if (LINK->keyintervals < 0)
    printf("No color key will be displayed\n");

  if (LINK->keytype != 'n' && LINK->keytype != 'f') {
    printf("keytype must be one of \"fn\"\n");
    halt();
  }

  /*







*/

  if (LINK->keyXsize <= 0.0) {
    printf("keyXsize must be positive, it was % .1E\n", LINK->keyXsize);
    halt();
  }

  if (LINK->keyYsize <= 0.0) {
    printf("keyYsize must be positive, it was % .1E\n", LINK->keyYsize);
    halt();
  }

  /*





*/

  if (LINK->keytype == 'f' && LINK->keydecimals < 1)
    printf("keydecimals must be positive for fractions. It was %ld\n",
	   LINK->keydecimals);

  /*




*/
  /*




*/

  if (LINK->shrinkYav <= 0) {
    printf("shrinkYav must be positive\n");
    halt();
  }

  if (LINK->smoothkind != 's' && LINK->smoothkind != '-') {
    printf("smoothkind is \"%c\" BUT\n", LINK->smoothkind);
    printf("smoothkind must be one of \"-s\"\n");
    halt();
  }
  if (LINK->smoothsize < 0) {
    printf("smoothsize (=%5.2f)must be one positive\n", LINK->smoothsize);
    halt();
  }

}

Local Void writeparameters(out, k, LINK)
_TEXT *out;
Char k;
struct LOC_themain *LINK;
{
  /*
*/
  fprintf(out->f, "%c denplo %4.2f\n", k, version);
  fprintf(out->f, "%c %9.2f parameterversion for denplo\n",
	  k, LINK->parameterversion);
  fprintf(out->f, "%c %9ld numbertoprocess\n", k, LINK->numbertoprocess);

  fprintf(out->f, "%c %*.*f %*.*f %*ld Xminvalue, XmaxValue, Xintervals\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->XminValue,
	  (int)LINK->NuWi, (int)LINK->NuDe, LINK->XmaxValue, (int)LINK->NuWi,
	  LINK->Xintervals);

  fprintf(out->f, "%c %*.*f %*.*f %*ld Yminvalue, YmaxValue, Yintervals\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->YminValue,
	  (int)LINK->NuWi, (int)LINK->NuDe, LINK->YmaxValue, (int)LINK->NuWi,
	  LINK->Yintervals);

  fprintf(out->f, "%c %ld %ld NuWi, NuDe\n", k, LINK->NuWi, LINK->NuDe);

  fprintf(out->f, "%c %ld %ld Xcolumn, Ycolumn\n",
	  k, LINK->Xcolumn, LINK->Ycolumn);
  fprintf(out->f, "%c %*.*f %*.*f Xsize, Ysize (cm)\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->Xsize, (int)LINK->NuWi,
	  (int)LINK->NuDe, LINK->Ysize);
  fprintf(out->f, "%c %*.*f %*.*f Xcorner, Ycorner (cm)\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->Xcorner, (int)LINK->NuWi,
	  (int)LINK->NuDe, LINK->Ycorner);
  /*




*/
  fprintf(out->f,
    "%c %ld %ld %ld %ld (X,Y)DisplayIntervals, (X,Y)DisplaySubIntervals\n",
    k, LINK->XDisplayIntervals, LINK->XDisplayIntervals,
    LINK->XDisplaySubIntervals, LINK->YDisplaySubIntervals);
  fprintf(out->f, "%c %ld %ld xwidth, ywidth\n",
	  k, LINK->xwidth, LINK->ywidth);
  fprintf(out->f, "%c %ld %ld xdecimal, ydecimal\n",
	  k, LINK->xdecimal, LINK->ydecimal);
  fprintf(out->f, "%c %c %*.*f background\n",
	  k, LINK->background, (int)LINK->NuWi, (int)LINK->NuDe, LINK->grey);
  fprintf(out->f,
	  "%c %c %*.*f %*.*f cutofftype cutoffvaluemin cutoffvaluemax\n",
	  k, LINK->cutofftype, (int)LINK->NuWi, (int)LINK->NuDe,
	  LINK->cutoffvaluemin, (int)LINK->NuWi, (int)LINK->NuDe,
	  LINK->cutoffvaluemax);
  fprintf(out->f, "%c %*.*f %*.*f shrinkfactorX, shrinkfactorY\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->shrinkfactorX,
	  (int)LINK->NuWi, (int)LINK->NuDe, LINK->shrinkfactorY);

  fprintf(out->f, "%c ", k);
  writestring(out, &LINK->XaxisLabel);
  fprintf(out->f, "\n%c ", k);
  writestring(out, &LINK->YaxisLabel);
  fprintf(out->f, "\n%c %c cog: color or grey plotting\n", k, LINK->cog);

  fprintf(out->f, "%c %ld keyintervals\n", k, LINK->keyintervals);
  fprintf(out->f, "%c %c keytype\n", k, LINK->keytype);
  fprintf(out->f, "%c %*.*f keyX\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->keyX);
  fprintf(out->f, "%c %*.*f keyY\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->keyY);
  fprintf(out->f, "%c %*.*f keyXsize\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->keyXsize);
  fprintf(out->f, "%c %*.*f keyYsize\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->keyYsize);
  fprintf(out->f, "%c %*.*f keyshrinkfactor\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->keyshrinkfactor);
  fprintf(out->f, "%c %*.*f thelinewidth\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->thelinewidth);
  fprintf(out->f, "%c %*ld keydecimals\n",
	  k, (int)LINK->NuWi, LINK->keydecimals);
  fprintf(out->f, "%c %*.*f keyfontsize\n",
	  k, (int)LINK->NuWi, (int)LINK->NuDe, LINK->keyfontsize);

  fprintf(out->f, "%c %c", k, LINK->edgecontrol);
  fprintf(out->f, " %1.2f", LINK->edgeleft);
  fprintf(out->f, " %1.2f", LINK->edgeright);
  fprintf(out->f, " %1.2f", LINK->edgelow);
  fprintf(out->f, " %1.2f", LINK->edgehigh);
  fprintf(out->f, " edgecontrol, edge(left, right, low, high)\n");

  fprintf(out->f, "%c %c", k, LINK->findpeak);
  fprintf(out->f, " %1.*f", (int)LINK->NuDe, LINK->startX);
  fprintf(out->f, " %1.*f", (int)LINK->NuDe, LINK->startY);
  fprintf(out->f, " %*ld", (int)LINK->NuWi, LINK->SearchRange);
  fprintf(out->f, " findpeak, startX, startY, SearchRange\n\n");

}

Local Void buildxyplom(l, LINK)
_TEXT *l;
struct LOC_themain *LINK;
{
  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "w", l->f);
    else
      l->f = fopen(l->name, "w");
  } else {
    if (l->f != NULL)
      rewind(l->f);
    else
      l->f = tmpfile();
  }
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  SETUPBUF(l->f, Char);

  fprintf(l->f, "\n/charwidth 12 def\n");
  fprintf(l->f, "/fontsize charwidth def\n\n");

  fprintf(l->f, "%% define fonts\n");
  fprintf(l->f, "/ffss {findfont fontsize scalefont setfont} def\n");
  fprintf(l->f, "/FontForStringRegular {/Times-Bold       ffss} def\n");
  fprintf(l->f, "/FontForStringItalic  {/Times-BoldItalic ffss} def\n");
  fprintf(l->f, "/FontForLogo          {/Helvetica-Bold   ffss} def\n");
  fprintf(l->f, "/FontForPrime         {/Symbol           ffss} def\n");
  fprintf(l->f, "/FontForSymbol        {/Symbol           ffss} def\n\n");

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
  fprintf(l->f, "} def\n\n\n");

  fprintf(l->f, "%% make a bolder display\n");
  fprintf(l->f, "2 setlinewidth\n");
  fprintf(l->f, "%%F\n");
  fprintf(l->f, "/Helvetica-Bold findfont\n");
  fprintf(l->f, "16 scalefont\n");
  fprintf(l->f, "setfont\n");
  fprintf(l->f, "%%T 1.5\n\n");

}



Local Void makecolor(cog, r, hue, saturation, brightness, LINK)
Char cog;
double r, *hue, *saturation, *brightness;
struct LOC_themain *LINK;
{
  /*

*/
  switch (cog) {

  case 'c':
    /*

*/
    /*

*/
    *hue = r;

    *saturation = 1.0;
    *brightness = 1.0;
    break;

  case 'g':
    *hue = 0.0;
    *saturation = 0.0;
    *brightness = 1 - r;
    break;
  }
}

Local Void dobackground(background, Xbin, Ybin, r, hue, saturation,
			brightness, LINK)
Char background;
long Xbin, Ybin;
double *r, *hue, *saturation, *brightness;
struct LOC_themain *LINK;
{
  *r = 0.0;
  switch (background) {

  case 'b':
    *hue = 0.0;
    *brightness = 0.0;
    *saturation = 0.0;
    break;

  case 'g':
    *hue = 0.0;
    *brightness = LINK->grey;
    *saturation = 0.0;
    break;

  case 'w':
    *hue = 0.0;
    *brightness = 1.0;
    *saturation = 0.0;
    break;

  case 'c':
    *r = ((double)Xbin / LINK->Xintervals + (double)Ybin / LINK->Yintervals) / 2;
    if (Xbin > Ybin)
      *r = 1 - *r;

    /*

*/
    *hue = *r;
    *saturation = 0.5;

    if (Xbin + (Ybin & 1) == 0)
      *brightness = 1.0;
    else {
      *brightness = 0.9;

    }
    break;



  }
}

/* Local variables for determinecolors: */
struct LOC_determinecolors {
  struct LOC_themain *LINK;
  Char cog;
  long b, BinMax;
  double *r;
  Char cutofftype;
  double cutoffvaluemin, cutoffvaluemax;
  long Xbin, Ybin;
  double *hue, *saturation, *brightness;
} ;

Local Void normal(LINK)
struct LOC_determinecolors *LINK;
{
  if ((LINK->cutofftype == 'f' || LINK->cutofftype == 'F') &&
      ((double)LINK->b / LINK->BinMax < LINK->cutoffvaluemin ||
       (double)LINK->b / LINK->BinMax > LINK->cutoffvaluemax))
    dobackground(LINK->LINK->background, LINK->Xbin, LINK->Ybin, LINK->r,
		 LINK->hue, LINK->saturation, LINK->brightness, LINK->LINK);
  else
    makecolor(LINK->cog, *LINK->r, LINK->hue, LINK->saturation,
	      LINK->brightness, LINK->LINK);
}

Local Void determinecolors(cog_, b_, BinMax_, r_, cutofftype_,
			   cutoffvaluemin_, cutoffvaluemax_, Xbin_, Ybin_,
			   hue_, saturation_, brightness_, LINK)
Char cog_;
long b_, BinMax_;
double *r_;
Char cutofftype_;
double cutoffvaluemin_, cutoffvaluemax_;
long Xbin_, Ybin_;
double *hue_, *saturation_, *brightness_;
struct LOC_themain *LINK;
{
  /*








*/
  struct LOC_determinecolors V;

  V.LINK = LINK;
  /*
*/
  V.cog = cog_;
  V.b = b_;
  V.BinMax = BinMax_;
  V.r = r_;
  V.cutofftype = cutofftype_;
  V.cutoffvaluemin = cutoffvaluemin_;
  V.cutoffvaluemax = cutoffvaluemax_;
  V.Xbin = Xbin_;
  V.Ybin = Ybin_;
  V.hue = hue_;
  V.saturation = saturation_;
  V.brightness = brightness_;
  if (V.b == 0) {
    dobackground(LINK->background, V.Xbin, V.Ybin, V.r, V.hue, V.saturation,
		 V.brightness, LINK);
    return;
  }
  if ((V.cutofftype == 'c' || V.cutofftype == 'C') &&
      (V.b < V.cutoffvaluemin || V.b > V.cutoffvaluemax)) {
    dobackground(LINK->background, V.Xbin, V.Ybin, V.r, V.hue, V.saturation,
		 V.brightness, LINK);
    return;
  }
  *V.r = (double)V.b / V.BinMax;
  if (LINK->findpeak != 'l') {
    normal(&V);
    return;
  }
  if (*V.r <= 1.0) {
    normal(&V);
    return;
  }
  /*



*/
  *V.hue = 0.0;
  *V.brightness = 0.0;
  *V.saturation = 0.0;
}

Local Void colorkey(l, keytype, BinMax, cog, keyX, keyY, keyXsize, keyYsize,
		    keydecimals, keyintervals, keyshrinkfactor, keyfontsize,
		    cutofftype, cutoffvaluemin, cutoffvaluemax, thelinewidth,
		    LINK)
_TEXT *l;
Char keytype;
long BinMax;
Char cog;
double keyX, keyY, keyXsize, keyYsize;
long keydecimals, keyintervals;
double keyshrinkfactor, keyfontsize;
Char cutofftype;
double cutoffvaluemin, cutoffvaluemax, thelinewidth;
struct LOC_themain *LINK;
{
  /*








*/
  double hue, saturation, brightness;
  long i;
  double r;
  long Xbin = 0, Ybin = 0;
  long BinValue, keywidth;

  fprintf(l->f, "%% Color Key\n\n");


  fprintf(l->f,
	  "/cmfactor 72 2.54 div def %% defines points -> centimeters\n");
  fprintf(l->f, "/cm { cmfactor mul} def %% defines centimeters\n");

  fprintf(l->f, "/keyX %*.*f cm def\n",
	  (int)LINK->NuWi, (int)LINK->NuDe, keyX);
  fprintf(l->f, "/keyY %*.*f cm def\n",
	  (int)LINK->NuWi, (int)LINK->NuDe, keyY);
  fprintf(l->f, "/keyXsize %*.*f cm def\n",
	  (int)LINK->NuWi, (int)LINK->NuDe, keyXsize);
  fprintf(l->f, "/keyYsize %*.*f cm def\n",
	  (int)LINK->NuWi, (int)LINK->NuDe, keyYsize);
  fprintf(l->f, "/keyshrinkfactor %*.*f def\n",
	  (int)LINK->NuWi, (int)LINK->NuDe, keyshrinkfactor);
  fprintf(l->f, "/keyfontsize % .*E def\n",
	  P_max((int)LINK->NuWi - 7, 1), keyfontsize);
  fprintf(l->f, "/thelinewidth %*.*f cm def\n",
	  (int)LINK->NuWi, (int)LINK->NuDe, thelinewidth);
  fprintf(l->f, "/Courier-Bold findfont keyfontsize scalefont setfont\n");

  fprintf(l->f, "gsave\n");
  fprintf(l->f, "keyX keyY translate\n");

  fprintf(l->f, "/keybox { %% make the key box \n");
  fprintf(l->f, "sethsbcolor\n");
  fprintf(l->f, "gsave\n");
  fprintf(l->f, "keyXsize keyYsize scale\n");
  fprintf(l->f, "/s keyshrinkfactor def\n");

  fprintf(l->f, "0 0 moveto\n");
  fprintf(l->f, "0 s lineto\n");
  fprintf(l->f, "s s lineto\n");
  fprintf(l->f, "s 0 lineto\n");
  fprintf(l->f, "closepath\n");
  fprintf(l->f, "thelinewidth 0 ge {\n");
  fprintf(l->f, "   gsave\n");
  fprintf(l->f, "     thelinewidth setlinewidth\n");
  fprintf(l->f, "      0 setgray\n");
  fprintf(l->f, "      stroke\n");
  fprintf(l->f, "   grestore\n");
  fprintf(l->f, "} if\n");
  fprintf(l->f, "fill\n");
  fprintf(l->f, "grestore\n");
  fprintf(l->f, "} def\n");

  fprintf(l->f, "%% columns: hue saturation  brightness sethsbcolor %% r\n");

  if (BinMax <= 0)
    keywidth = 1;
  else {
    if (keytype == 'f') {
      keywidth = keydecimals + 2;

    } else {
      keywidth = (long)(log((double)BinMax) / log(10.0) + 1 + 0.2);
      /*
*/
    }
  }

  if (keyintervals > BinMax) {
    if (BinMax > 0)
      keyintervals = BinMax;
    else
      keyintervals = 1;
  }

  if (keyintervals <= 0)
    keyintervals = 1;
  for (i = 0; i <= keyintervals; i++) {
    /*

*/
    BinValue = (long)(BinMax * ((double)i / keyintervals) + 0.5);

    determinecolors(cog, BinValue, BinMax, &r, cutofftype, cutoffvaluemin,
		    cutoffvaluemax, Xbin, Ybin, &hue, &saturation,
		    &brightness, LINK);

    /*

*/
    hue = 0.84 * hue + 0.16;


    fprintf(l->f, " %*.*f %*.*f %*.*f keybox %% %ld\n",
	    (int)LINK->NuWi, (int)LINK->NuDe, hue, (int)LINK->NuWi,
	    (int)LINK->NuDe, saturation, (int)LINK->NuWi, (int)LINK->NuDe,
	    brightness, i);

    fprintf(l->f, "gsave");
    fprintf(l->f, " keyXsize 0 moveto");
    fprintf(l->f, " 0 setgray");
    switch (keytype) {

    case 'n':
      fprintf(l->f, " (%*ld) show", (int)keywidth, BinValue);
      break;

    case 'f':
      fprintf(l->f, " (%*.*f) show",
	      (int)keywidth, (int)keydecimals, (double)i / keyintervals);
      break;
    }
    fprintf(l->f, " grestore\n");

    fprintf(l->f, "0 keyYsize translate\n");

  }

  fprintf(l->f, "grestore\n");

}





Local Void setvariables(LINK)
struct LOC_themain *LINK;
{
  long X, Y;

  printf("clearing bins ...\n");
  for (X = minValue; X <= maxValue; X++) {
    for (Y = minValue; Y <= maxValue; Y++)
      LINK->bins[X - minValue][Y - minValue] = 0;
  }

  if (LINK->smoothkind != '-') {
    LINK->smoothingbins = (long(*)[maxValue - minValue + 1])
			  Malloc(sizeof(binsarray));
    printf("clearing smoothing bins ...\n");
    for (X = minValue; X <= maxValue; X++) {
      for (Y = minValue; Y <= maxValue; Y++)
	LINK->smoothingbins[X - minValue][Y - minValue] = 0;
    }
  }


  for (X = minValue; X <= maxValue; X++) {
    LINK->ysums[X - minValue] = 0.0;
    LINK->ycounts[X - minValue] = 0;
  }
  printf("cleared\n");

  LINK->dataCount = 0;
  LINK->inCount = 0;
  LINK->outCount = 0;

  LINK->XinCount = 0;
  LINK->XoutCount = 0;

  LINK->YinCount = 0;
  LINK->YoutCount = 0;

  LINK->Xlo = LONG_MAX;
  LINK->Xhi = -LONG_MAX;
  LINK->Ylo = LONG_MAX;
  LINK->Yhi = -LONG_MAX;



  LINK->xlessthany = (LINK->Xcolumn < LINK->Ycolumn);

  LINK->Xrange = LINK->XmaxValue - LINK->XminValue;
  LINK->Yrange = LINK->YmaxValue - LINK->YminValue;

  LINK->jumpX = LINK->Xrange / LINK->Xintervals;
  LINK->jumpY = LINK->Yrange / LINK->Yintervals;

  LINK->SymbolSizeX = LINK->shrinkfactorX * (LINK->Xrange / LINK->Xintervals);
  LINK->SymbolSizeY = LINK->shrinkfactorY * (LINK->Yrange / LINK->Yintervals);

  LINK->shiftX = (1 - LINK->shrinkfactorX) * LINK->jumpX / 2;
  LINK->shiftY = (1 - LINK->shrinkfactorY) * LINK->jumpY / 2;

  if (debugging) {
    printf("==================================================\n");

    printf("  XminValue: %10.5f \n", LINK->XminValue);
    printf("  XmaxValue: %10.5f \n", LINK->XmaxValue);
    printf("     Xrange: %10.5f \n", LINK->Xrange);

    printf("  YminValue: %10.5f \n", LINK->YminValue);
    printf("  YmaxValue: %10.5f \n", LINK->YmaxValue);
    printf("     Yrange: %10.5f \n", LINK->Yrange);

    printf(" Xintervals: %10ld \n", LINK->Xintervals);
    printf(" Yintervals: %10ld \n", LINK->Yintervals);

    printf("      jumpX: %10.5f \n", LINK->jumpX);
    printf("      jumpY: %10.5f \n", LINK->jumpY);

    printf("SymbolSizeX: %10.5f \n", LINK->SymbolSizeX);
    printf("SymbolSizeY: %10.5f \n", LINK->SymbolSizeY);

    printf("shiftX:      %10.5f \n", LINK->shiftX);
    printf("shiftY:      %10.5f \n", LINK->shiftY);

    printf("==================================================\n");
  }

  if (LINK->SymbolSizeX <= 0) {
    printf("SymbolSizeX: %10.5f is negative!\n", LINK->SymbolSizeX);
    printf("Program error!\n");
    halt();
  }

  if (LINK->SymbolSizeY > 0)
    return;
  /*

*/

  printf("SymbolSizeY: %10.5f is negative!\n", LINK->SymbolSizeY);
  printf("Program error!\n");
  halt();
}

#undef debugging

Local Void grab(Xvalue, Xcolumn, Yvalue, Ycolumn, LINK)
double *Xvalue;
long Xcolumn;
double *Yvalue;
long Ycolumn;
struct LOC_themain *LINK;
{
  long column = 1;

  /*


*/
  while (column < Xcolumn) {
    /*

*/
    skipcolumn(LINK->data);
    column++;
  }
  fscanf(LINK->data->f, "%lg", Xvalue);
  /*


*/
  column++;
  while (column < Ycolumn) {
    skipcolumn(LINK->data);
    column++;
  }
  fscanf(LINK->data->f, "%lg", Yvalue);
  fscanf(LINK->data->f, "%*[^\n]");
  /*



*/
  getc(LINK->data->f);
}

Local Void fail()
{
  printf("PROGRAM ERROR: This datum is outside the acceptable range.\n");
  halt();
}

Local Void fillonedatum(LINK)
struct LOC_themain *LINK;
{
  /*

*/
  if ((P_peek(LINK->data->f) == '*') | (P_peek(LINK->data->f) == '#') |
      P_eoln(LINK->data->f)) {
    fscanf(LINK->data->f, "%*[^\n]");
    getc(LINK->data->f);
    return;
  }
  LINK->dataCount++;


  if (LINK->xlessthany)
    grab(&LINK->Xvalue, LINK->Xcolumn, &LINK->Yvalue, LINK->Ycolumn, LINK);
  else
    grab(&LINK->Yvalue, LINK->Ycolumn, &LINK->Xvalue, LINK->Xcolumn, LINK);

  if (LINK->XminValue <= LINK->Xvalue && LINK->Xvalue < LINK->XmaxValue) {
    /*



*/
    LINK->Xbin = (long)(LINK->Xintervals * (LINK->Xvalue - LINK->XminValue) /
			LINK->Xrange);


    if (LINK->Xbin < minValue) {
      printf("fillonedatum: X (= %ld) < minValue = %ld\n",
	     LINK->Xbin, (long)minValue);
      /*







*/
      fail();
    }
    if (LINK->Xbin > maxValue) {
      printf("fillonedatum: Xbin (= %ld) > maxValue = %ld\n",
	     LINK->Xbin, (long)maxValue);
      fail();
    }
    LINK->XinCount++;
    LINK->Xin = true;
    if (LINK->Xbin < LINK->Xlo)
      LINK->Xlo = LINK->Xbin;
    if (LINK->Xbin > LINK->Xhi)
      LINK->Xhi = LINK->Xbin;
  }

  else {
    LINK->Xbin = LONG_MAX;
    LINK->XoutCount++;
    LINK->Xin = false;
  }

  if (LINK->Xin) {
    LINK->ysums[LINK->Xbin - minValue] += LINK->Yvalue;
    LINK->ycounts[LINK->Xbin - minValue]++;
  }

  if (LINK->YminValue <= LINK->Yvalue && LINK->Yvalue < LINK->YmaxValue) {
    /*

*/
    LINK->Ybin = (long)(LINK->Yintervals * (LINK->Yvalue - LINK->YminValue) /
			LINK->Yrange);


    if (LINK->Ybin < minValue) {
      printf("fillonedatum: Ybin (= %ld) < minValue = %ld\n",
	     LINK->Ybin, (long)minValue);
      fail();
    }
    if (LINK->Ybin > maxValue) {
      printf("fillonedatum: Ybin (= %ld) > maxValue = %ld\n",
	     LINK->Ybin, (long)maxValue);
      fail();
    }
    LINK->YinCount++;
    LINK->Yin = true;
    if (LINK->Ybin < LINK->Ylo)
      LINK->Ylo = LINK->Ybin;
    if (LINK->Ybin > LINK->Yhi)
      LINK->Yhi = LINK->Ybin;
  }

  else {
    LINK->Ybin = LONG_MAX;
    LINK->YoutCount++;
    LINK->Yin = false;
  }

  if (LINK->Yin && LINK->Xin) {
    LINK->bins[LINK->Xbin - minValue][LINK->Ybin - minValue]++;
    LINK->inCount++;
  } else
    LINK->outCount++;
}

Local Void fillbins(LINK)
struct LOC_themain *LINK;
{
  printf("filling bins ...\n");
  if (*LINK->data->name != '\0') {
    if (LINK->data->f != NULL)
      LINK->data->f = freopen(LINK->data->name, "r", LINK->data->f);
    else
      LINK->data->f = fopen(LINK->data->name, "r");
  } else
    rewind(LINK->data->f);
  if (LINK->data->f == NULL)
    _EscIO2(FileNotFound, LINK->data->name);
  RESETBUF(LINK->data->f, Char);
  if (LINK->numbertoprocess < 0) {
    while (!BUFEOF(LINK->data->f))
      fillonedatum(LINK);
  } else {
    while (!BUFEOF(LINK->data->f) && LINK->dataCount < LINK->numbertoprocess)
      fillonedatum(LINK);
  }
  printf("filled, dataCount = %ld\n", LINK->dataCount);
}

/* Local variables for findbinmax: */
struct LOC_findbinmax {
  struct LOC_themain *LINK;
  long b, Xbin, Ybin;
} ;

Local Void showlocation(X, Y, LINK)
long X, Y;
struct LOC_findbinmax *LINK;
{
  LINK->LINK->Xvalue = LINK->LINK->Xrange * ((double)X / LINK->LINK->Xintervals) +
		       LINK->LINK->XminValue;
  LINK->LINK->Yvalue = LINK->LINK->Yrange * ((double)Y / LINK->LINK->Yintervals) +
		       LINK->LINK->YminValue;

  if (debugging_)
    printf("[%ld, %*ld]=>", X, (int)LINK->LINK->NuWi, Y);
  printf("at (%1.*f, %1.*f)\n",
	 (int)LINK->LINK->NuWi, LINK->LINK->Xvalue, (int)LINK->LINK->NuWi,
	 LINK->LINK->Yvalue);
  /*


*/
}

Local boolean IsOutsideData(X, Y, LINK)
long X, Y;
struct LOC_findbinmax *LINK;
{
  if (X < LINK->LINK->Xlo || X > LINK->LINK->Xhi || Y < LINK->LINK->Ylo ||
      Y > LINK->LINK->Yhi)
    return true;
  else
    return false;
}

Local Void around(SearchRange, LINK)
long SearchRange;
struct LOC_findbinmax *LINK;
{
  long tryX, tryY;
  long loops = 0;
  long FORLIM, FORLIM1;

  FORLIM = LINK->Ybin - SearchRange;
  /*


*/

  for (tryY = LINK->Ybin + SearchRange; tryY >= FORLIM; tryY--) {
    FORLIM1 = LINK->Xbin + SearchRange;
    for (tryX = LINK->Xbin - SearchRange; tryX <= FORLIM1; tryX++) {
      if (IsOutsideData(tryX, tryY, LINK))
	printf(" +");
      else {
	LINK->b = LINK->LINK->bins[tryX - minValue][tryY - minValue];
	if (tryX == LINK->Xbin && tryY == LINK->Ybin)
	  putchar('(');
	else if (tryX - 1 == LINK->Xbin && tryY == LINK->Ybin)
	  putchar(')');
	else
	  putchar(' ');
	printf("%ld", LINK->b);
      }
      loops++;
    }
    printf(" %2ld\n", tryY);
    /*

*/
  }
}

Local Void findbinmax(denploxyin, BinMax, LINK)
_TEXT *denploxyin;
long *BinMax;
struct LOC_themain *LINK;
{
  /*



*/
  struct LOC_findbinmax V;
  boolean done;
  long nextX, nextY, newBinMax, tryX, tryY, step, loops, FORLIM, FORLIM1;

  /*


*/

  V.LINK = LINK;
  if (LINK->findpeak != 'l') {
    printf("Finding GLOBAL MAXIMUM for the entire plot\n");
    *BinMax = 0;
    FORLIM = LINK->Xhi;
    for (V.Xbin = LINK->Xlo; V.Xbin <= FORLIM; V.Xbin++) {
      FORLIM1 = LINK->Yhi;
      for (V.Ybin = LINK->Ylo; V.Ybin <= FORLIM1; V.Ybin++) {
	V.b = LINK->bins[V.Xbin - minValue][V.Ybin - minValue];
	if (V.b > *BinMax) {
	  *BinMax = V.b;
	  nextX = V.Xbin;
	  nextY = V.Ybin;
	}
      }
    }
    V.Xbin = nextX;
    V.Ybin = nextY;
    if (debugging_)
      around(2L, &V);
    if (debugging_)
      showlocation(V.Xbin, V.Ybin, &V);
  } else {
    printf("Finding LOCAL MAXIMUM starting from");
    printf(" (%1.*f, %1.*f)\n",
	   (int)LINK->NuWi, LINK->startX, (int)LINK->NuWi, LINK->startY);
    printf("with a SearchRange of %ld\n", LINK->SearchRange);

    /*



*/

    /*
*/
    V.Xbin = (long)(0.5 + LINK->Xintervals *
			  ((LINK->startX - LINK->XminValue) / LINK->Xrange));
    V.Ybin = (long)(0.5 + LINK->Yintervals *
			  ((LINK->startY - LINK->YminValue) / LINK->Yrange));

    /*

*/
    if (debugging_)
      showlocation(V.Xbin, V.Ybin, &V);


    if (IsOutsideData(V.Xbin, V.Ybin, &V)) {
      printf("WARNING: Point (%1.*f, %1.*f) is outside the data!\n",
	     (int)LINK->NuWi, LINK->startX, (int)LINK->NuWi, LINK->startY);
      showlocation(V.Xbin, V.Ybin, &V);
    } else
      printf("Good, that point is inside the graph!\n");






    if (debugging_)
      LINK->SearchRange = 3;
    if (debugging_)
      around(LINK->SearchRange, &V);

    /*

*/
    *BinMax = 0;
    newBinMax = 0;
    step = 0;
    loops = 0;
    done = false;
    nextX = V.Xbin;
    nextY = V.Ybin;
    showlocation(V.Xbin, V.Ybin, &V);
    while (!done) {
      /*
*/
      if (debugging_)
	printf("BinMax=%ld\n", *BinMax);
      FORLIM = V.Xbin + LINK->SearchRange;
      for (tryX = V.Xbin - LINK->SearchRange; tryX <= FORLIM; tryX++) {
	FORLIM1 = V.Ybin + LINK->SearchRange;
	for (tryY = V.Ybin - LINK->SearchRange; tryY <= FORLIM1; tryY++) {
	  if (!IsOutsideData(tryX, tryY, &V)) {
	    V.b = LINK->bins[tryX - minValue][tryY - minValue];
	    if (debugging_)
	      printf("b=%ld ", V.b);
	    if (debugging_)
	      showlocation(tryX, tryY, &V);
	    if (V.b > newBinMax) {
	      newBinMax = V.b;
	      if (debugging_)
		printf("grab newBinMax=%ld\n", newBinMax);
	      nextX = tryX;
	      nextY = tryY;
	      step++;
	    }
	  } else {
	    if (debugging_)
	      printf("outside of data\n");
	    if (debugging_)
	      halt();
	  }
	}
      }

      if (debugging_)
	printf("after square: newBinMax=%ld\n", newBinMax);
      if (debugging_)
	printf("after square:    BinMax=%ld\n", *BinMax);
      if (newBinMax == *BinMax) {
	if (debugging_)
	  printf("NO MOVE\n");

	done = true;
      } else {
	if (debugging_)
	  printf("MOVE\n");

	V.Xbin = nextX;
	V.Ybin = nextY;
	*BinMax = newBinMax;
	if (debugging_)
	  printf("NEW location: \n");
	printf("%ld ", LINK->bins[V.Xbin - minValue][V.Ybin - minValue]);
	showlocation(V.Xbin, V.Ybin, &V);
      }

      if (debugging_)
	around(LINK->SearchRange, &V);
      loops++;
      if (debugging_)
	printf("loops: %ld------------------------------------\n", loops);
      if (debugging_) {
	if (loops > 4)
	  halt();
      }

    }
  }

  printf("The highest number of points in a bin is: %ld ", *BinMax);
  showlocation(V.Xbin, V.Ybin, &V);
}

#undef debugging_

Local Void smoothsquare(LINK)
struct LOC_themain *LINK;
{
  /*
*/
  long Xbin, Ybin;
  /*

*/
  long sum, low, high, dx, dy, FORLIM, FORLIM1;

  printf("Smoothing in square with side %ld\n", LINK->smoothrange);



  if ((long)(LINK->smoothsize / 2) * 2 == (long)floor(LINK->smoothsize + 0.5)) {
    printf("smoothsize = %ld is even\n", (long)floor(LINK->smoothsize + 0.5));
    low = 1 - (long)(LINK->smoothsize / 2);
    high = (long)(LINK->smoothsize / 2);
  } else {
    printf("smoothsize = %ld is odd\n", (long)floor(LINK->smoothsize + 0.5));
    low = -(long)(LINK->smoothsize / 2);
    high = (long)(LINK->smoothsize / 2);
  }
  printf("low =  %ld\n", low);
  printf("high = %ld\n", high);

  if (high - low + 1 != (long)floor(LINK->smoothsize + 0.5)) {
    printf("high - low + 1 =%ld <> round(smoothsize) = %ld\n",
	   high - low + 1, (long)floor(LINK->smoothsize + 0.5));
    halt();
  }

  FORLIM = LINK->Xintervals - high;
  /*


*/
  for (Xbin = -low; Xbin < FORLIM; Xbin++) {
    FORLIM1 = LINK->Yintervals - high;
    for (Ybin = -low; Ybin < FORLIM1; Ybin++) {
      /*








*/
      sum = 0;
      for (dx = low; dx <= high; dx++) {
	for (dy = low; dy <= high; dy++)
	  sum += LINK->bins[Xbin + dx - minValue][Ybin + dy - minValue];
      }
      LINK->smoothingbins[Xbin - minValue][Ybin - minValue] = sum;
    }
  }

  LINK->swapbins = LINK->smoothingbins;
  LINK->bins = LINK->smoothingbins;
  LINK->smoothingbins = LINK->swapbins;
}

#undef debugging__

Local Void writedenploxyin(denploxyin, BinMax, k, yaverage, LINK)
_TEXT *denploxyin;
long BinMax;
Char k, yaverage;
struct LOC_themain *LINK;
{
  /*



*/
  long b;
  double r, hue, saturation, brightness;
  long Xbin, Ybin, FORLIM, FORLIM1;

  printf("writing denplopxyin ...\n");
  fprintf(denploxyin->f, "\n%c definition of data columns:\n", k);

  fprintf(denploxyin->f, "%c  1: X coordinate\n", k);
  fprintf(denploxyin->f, "%c  2: Y coordinate\n", k);
  fprintf(denploxyin->f, "%c  3: total counts at this position\n", k);
  fprintf(denploxyin->f,
	  "%c  4: (counts at this bit-position) / (maximum bin count)\n", k);
  fprintf(denploxyin->f, "%c  5: X plotting coordinate\n", k);
  fprintf(denploxyin->f, "%c  6: Y plotting coordinate\n", k);
  fprintf(denploxyin->f, "%c  7: SymbolSizeX: X column size control\n", k);
  fprintf(denploxyin->f, "%c  8: SymbolSizeY: Y column size control\n", k);
  fprintf(denploxyin->f, "%c  9: hue\n", k);
  fprintf(denploxyin->f, "%c 10: saturation\n", k);
  fprintf(denploxyin->f, "%c 11: brightness\n\n", k);

  fprintf(denploxyin->f, "%c maximum bin count, Binmax: %ld\n\n", k, BinMax);
  FORLIM = LINK->Xintervals;

  /*














*/

  for (Xbin = 0; Xbin < FORLIM; Xbin++) {
    putc('\n', denploxyin->f);
    FORLIM1 = LINK->Yintervals;
    /*
*/
    for (Ybin = 0; Ybin < FORLIM1; Ybin++) {
      b = LINK->bins[Xbin - minValue][Ybin - minValue];
      /*




*/

      determinecolors(LINK->cog, b, BinMax, &r, LINK->cutofftype,
		      LINK->cutoffvaluemin, LINK->cutoffvaluemax, Xbin, Ybin,
		      &hue, &saturation, &brightness, LINK);

      if (LINK->background != 'w' || b != 0) {
	/*




*/

	/*


*/
	LINK->Xvalue = LINK->Xrange * ((double)Xbin / LINK->Xintervals) +
		       LINK->XminValue;
	LINK->Yvalue = LINK->Yrange * ((double)Ybin / LINK->Yintervals) +
		       LINK->YminValue;


	/*











*/

	fprintf(denploxyin->f,
	  "%*.*f %*.*f %*ld %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f",
	  (int)LINK->NuWi, (int)LINK->NuDe, LINK->Xvalue, (int)LINK->NuWi,
	  (int)LINK->NuDe, LINK->Yvalue, (int)LINK->NuWi, b, (int)LINK->NuWi,
	  (int)LINK->NuDe, r, (int)LINK->NuWi, (int)LINK->NuDe,
	  LINK->Xvalue + LINK->shiftX, (int)LINK->NuWi, (int)LINK->NuDe,
	  LINK->Yvalue + LINK->shiftY, (int)LINK->NuWi, (int)LINK->NuDe,
	  LINK->SymbolSizeX, (int)LINK->NuWi, (int)LINK->NuDe,
	  LINK->SymbolSizeY, (int)LINK->NuWi, (int)LINK->NuDe, hue,
	  (int)LINK->NuWi, (int)LINK->NuDe, saturation, (int)LINK->NuWi,
	  (int)LINK->NuDe, brightness);
/* p2c: denplo.p, line 3788: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 4073 [251] */
	if (yaverage == 'y')
	  fprintf(denploxyin->f, " -");
	putc('\n', denploxyin->f);

      }

    }

    if (yaverage == 'y') {
      if (LINK->ycounts[Xbin - minValue] > 0) {
	LINK->Yvalue = LINK->ysums[Xbin - minValue] /
		       LINK->ycounts[Xbin - minValue];
	/*




*/


	fprintf(denploxyin->f,
	  "%*.*f %*.*f %*ld %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f +",
	  (int)LINK->NuWi, (int)LINK->NuDe, LINK->Xvalue, (int)LINK->NuWi,
	  (int)LINK->NuDe, LINK->Yvalue, (int)LINK->NuWi, b, (int)LINK->NuWi,
	  (int)LINK->NuDe, r, (int)LINK->NuWi, (int)LINK->NuDe,
	  LINK->Xvalue + LINK->shiftX, (int)LINK->NuWi, (int)LINK->NuDe,
	  LINK->Yvalue - LINK->jumpY / (2 * LINK->shrinkYav), (int)LINK->NuWi,
	  (int)LINK->NuDe, LINK->SymbolSizeX, (int)LINK->NuWi,
	  (int)LINK->NuDe, LINK->jumpY / LINK->shrinkYav, (int)LINK->NuWi,
	  (int)LINK->NuDe, hue, (int)LINK->NuWi, (int)LINK->NuDe, saturation,
	  (int)LINK->NuWi, (int)LINK->NuDe, 0.0);
/* p2c: denplo.p, line 3788: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 4106 [251] */
      }
      /*









*/

    }


  }
  putc('\n', denploxyin->f);
  printf("finished writing denplopxyin\n");
}


Local Void mkdenploxyplop(denploxyplop, LINK)
_TEXT *denploxyplop;
struct LOC_themain *LINK;
{
  long graphXintervals, graphYintervals;
  Char crosshairsymbol;

  /*
*/

  /*


*/

  /*










*/



  if (LINK->XminValue <= 0.0 && LINK->XmaxValue >= 0.0 ||
      LINK->YminValue <= 0.0 && LINK->YmaxValue >= 0.0) {
/* p2c: denplo.p: Note: Eliminated unused assignment statement [338] */
    crosshairsymbol = 'c';
  } else {
/* p2c: denplo.p: Note: Eliminated unused assignment statement [338] */
    crosshairsymbol = 'a';
  }

  /*







*/

  if (LINK->XDisplayIntervals > 0)
    graphXintervals = LINK->XDisplayIntervals;
  else
    graphXintervals = LINK->Xintervals;

  if (LINK->YDisplayIntervals > 0)
    graphYintervals = LINK->YDisplayIntervals;
  else
    graphYintervals = LINK->Yintervals;
  if (*denploxyplop->name != '\0') {
    if (denploxyplop->f != NULL)
      denploxyplop->f = freopen(denploxyplop->name, "w", denploxyplop->f);
    else
      denploxyplop->f = fopen(denploxyplop->name, "w");
  } else {
    if (denploxyplop->f != NULL) {

      rewind(denploxyplop->f);
    } else
      denploxyplop->f = tmpfile();
  }
  if (denploxyplop->f == NULL)
    _EscIO2(FileNotFound, denploxyplop->name);
  SETUPBUF(denploxyplop->f, Char);
  fprintf(denploxyplop->f, "%*.*f %*.*f zerox zeroy\n",
	  (int)LINK->NuWi, (int)LINK->NuDe, LINK->Xcorner, (int)LINK->NuWi,
	  (int)LINK->NuDe, LINK->Ycorner);

  fprintf(denploxyplop->f, "x %*.*f %*.*f zx min max\n",
	  (int)LINK->NuWi, (int)LINK->NuDe, LINK->XminValue, (int)LINK->NuWi,
	  (int)LINK->NuDe, LINK->XmaxValue);
  fprintf(denploxyplop->f, "y %*.*f %*.*f zy min max\n",
	  (int)LINK->NuWi, (int)LINK->NuDe, LINK->YminValue, (int)LINK->NuWi,
	  (int)LINK->NuDe, LINK->YmaxValue);

  fprintf(denploxyplop->f,
    "%ld %ld %ld %ld Display Intervals (X,Y) Display SubIntervals (X,Y)\n",
    graphXintervals, graphYintervals, LINK->XDisplaySubIntervals,
    LINK->YDisplaySubIntervals);
  /*



*/

  fprintf(denploxyplop->f, "%ld %ld xwidth    ywidth    \n",
	  LINK->xwidth, LINK->ywidth);
  fprintf(denploxyplop->f, "%ld %ld xdecimal  ydecimal  \n",
	  LINK->xdecimal, LINK->ydecimal);
  fprintf(denploxyplop->f, "%*.*f %*.*f     xsize     ysize     \n",
	  (int)LINK->NuWi, (int)LINK->NuDe, LINK->Xsize, (int)LINK->NuWi,
	  (int)LINK->NuDe, LINK->Ysize);

  /*

*/



  /*
*/
  writestring(denploxyplop, &LINK->XaxisLabel);
  putc('\n', denploxyplop->f);
  writestring(denploxyplop, &LINK->YaxisLabel);
  fprintf(denploxyplop->f, "\n%c         zc                  \n",
	  crosshairsymbol);

  /*












*/

  /*







*/




  /*




*/
  if (LINK->logxscale)
    putc('l', denploxyplop->f);
  else
    putc('n', denploxyplop->f);
  fprintf(denploxyplop->f, " %8.5f      zxl base            \n", LINK->xbase);
  if (LINK->logyscale)
    putc('l', denploxyplop->f);
  else
    putc('n', denploxyplop->f);
  fprintf(denploxyplop->f, " %8.5f      zyl base            \n", LINK->ybase);

  fprintf(denploxyplop->f, "          *******");
  fprintf(denploxyplop->f, " This xyplop was produced by denplo %4.2f\n",
	  version);
  fprintf(denploxyplop->f, "5 6       xcolumn   ycolumn   \n");
  fprintf(denploxyplop->f, "0         symbol column       \n");
  fprintf(denploxyplop->f, "7  8      xscolumn  yscolumn  \n");
  fprintf(denploxyplop->f, "9 10 11   hue saturation brightness\n");
  fprintf(denploxyplop->f, "          ********************\n");
  fprintf(denploxyplop->f, "R         symbol-to-plot      \n");
  fprintf(denploxyplop->f, "i         symbol-flag         \n");
  fprintf(denploxyplop->f, "-1.0      symbol sizex        \n");
  fprintf(denploxyplop->f, "-1.0      symbol sizey        \n");
  fprintf(denploxyplop->f, "n         connection size     \n");
  fprintf(denploxyplop->f, "n  0.125  linetype  size      \n");
  fprintf(denploxyplop->f, "          ********************\n");
  fprintf(denploxyplop->f, ".\n");
  fprintf(denploxyplop->f, "          ********************\n");

  fprintf(denploxyplop->f, "          **** more parameters\n");

  fprintf(denploxyplop->f,
    "%c %4.2f %4.2f %4.2f %4.2f edgecontrol (p=page), edge: left,right,low,high in cm\n",
    LINK->edgecontrol, LINK->edgeleft, LINK->edgeright, LINK->edgelow,
    LINK->edgehigh);

  /*


*/

  /*






*/


  fprintf(denploxyplop->f,
	  "8.91 version of xyplo that this parameter file is designed for.\n");

}


Local Void report(f, k, LINK)
_TEXT *f;
Char k;
struct LOC_themain *LINK;
{
  fprintf(f->f, "%c %*ld data points were INSIDE the plot\n",
	  k, (int)LINK->NuWi, LINK->inCount);
  fprintf(f->f, "%c %*ld data points were OUTSIDE the plot\n",
	  k, (int)LINK->NuWi, LINK->outCount);
  fprintf(f->f, "%c %*ld total data points\n",
	  k, (int)LINK->NuWi, LINK->outCount + LINK->inCount);
  /*




*/
}



Static Void themain(data_, denplop, denploxyin, denploxyplom, denploxyplop)
_TEXT *data_, *denplop, *denploxyin, *denploxyplom, *denploxyplop;
{
  struct LOC_themain V;
  _TEXT TEMP;


  V.data = data_;
  printf("denplo %4.2f\n", version);

  readparameters(denplop, &V);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeparameters(&TEMP, ' ', &V);

  buildxyplom(denploxyplom, &V);

  V.bins = (long(*)[maxValue - minValue + 1])Malloc(sizeof(binsarray));


  if (V.smoothkind != '-')
    V.smoothrange = (long)V.smoothsize;

  setvariables(&V);

  if (*V.data->name != '\0') {
    if (V.data->f != NULL)
      V.data->f = freopen(V.data->name, "r", V.data->f);
    else
      V.data->f = fopen(V.data->name, "r");
  } else
    rewind(V.data->f);
  if (V.data->f == NULL)
    _EscIO2(FileNotFound, V.data->name);
  RESETBUF(V.data->f, Char);
  fillbins(&V);

  if (V.smoothkind != '-') {
    if (V.smoothkind == 's')
      smoothsquare(&V);
  }

  if (*denploxyin->name != '\0') {
    if (denploxyin->f != NULL)
      denploxyin->f = freopen(denploxyin->name, "w", denploxyin->f);
    else
      denploxyin->f = fopen(denploxyin->name, "w");
  } else {
    if (denploxyin->f != NULL)
      rewind(denploxyin->f);
    else
      denploxyin->f = tmpfile();
  }
  if (denploxyin->f == NULL)
    _EscIO2(FileNotFound, denploxyin->name);
  SETUPBUF(denploxyin->f, Char);
  writeparameters(denploxyin, komment, &V);


  if (V.findpeak != 'm')
    findbinmax(denploxyin, &V.BinMax, &V);
  writedenploxyin(denploxyin, V.BinMax, komment, V.yaverage, &V);

  mkdenploxyplop(denploxyplop, &V);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  report(&TEMP, ' ', &V);
  report(denploxyin, komment, &V);

  /*











*/

  if (V.keyintervals > 0)
    colorkey(denploxyplom, V.keytype, V.BinMax, V.cog, V.keyX + V.Xcorner,
	     V.keyY + V.Ycorner, V.keyXsize, V.keyYsize, V.keydecimals,
	     V.keyintervals, V.keyshrinkfactor, V.keyfontsize, V.cutofftype,
	     V.cutoffvaluemin, V.cutoffvaluemax, V.thelinewidth, &V);

  printf("denplo %4.2f is DONE\n", version);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  denploxyplop.f = NULL;
  strcpy(denploxyplop.name, "denploxyplop");
  denploxyplom.f = NULL;
  strcpy(denploxyplom.name, "denploxyplom");
  denploxyin.f = NULL;
  strcpy(denploxyin.name, "denploxyin");
  denplop.f = NULL;
  strcpy(denplop.name, "denplop");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &denplop, &denploxyin, &denploxyplom, &denploxyplop);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (denplop.f != NULL)
    fclose(denplop.f);
  if (denploxyin.f != NULL)
    fclose(denploxyin.f);
  if (denploxyplom.f != NULL)
    fclose(denploxyplom.f);
  if (denploxyplop.f != NULL)
    fclose(denploxyplop.f);
  exit(EXIT_SUCCESS);
}



/* End. */
