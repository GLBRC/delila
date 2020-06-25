/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "xyplo.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         9.50
/*

















































































































*/



/*

































































































































































































































































































































































































































































































*/


/*





































*/


#define dsafety         5
/*
*/


#define YaxisXshift     (-0.127)
#define YaxisYshift     (-0.127)
#define Yticlength      0.127
#define XaxisXshift     0.000
#define XaxisYshift     (-0.508)
#define Xticlength      0.127
#define labelabovey     0.508
#define labelbelowx     1.143

/*
*/


#define defaultllx      7.10999
#define defaultlly      7.01995
#define defaulturx      588.15
#define defaultury      784.98


#define pwid            8
#define pdec            5

/*


*/

#define maxinterval     1000



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




typedef struct line {
  Char linetype;
  /*
*/
  double m, b, linesize;


  double sumx, sumy, sumxsqd, sumysqd, sumxy, ex, ey, varx, vary, covxy, r;
  long n;
} line;

typedef struct lines {
  line l;
  struct lines *next;
} lines;

typedef struct symbol {
  Char symboltoplot;
  /*



*/
  stringDelila symbolflag;
  double symbolxsize;
  /*
*/
  double symbolysize;

  boolean doconnection;
  Char connecttype;
  /*
*/
  double connectsize;
  boolean didlastpoint;
  /*

*/
  double oldx, oldy;

  boolean doline;
  line l;

  boolean symbolhasxyplomdef;
} symbol;

typedef struct symbols {
  symbol s;
  struct symbols *next;
} symbols;

typedef struct xyploparam {

  double parameterversion;

  /*
*/
  double llx, lly, urx, ury;

  Char edgecontrol;
  double edgeleft, edgelow, edgeright, edgehigh;

  boolean setx, sety;
  double xmin, xmax, ymin, ymax, xscale;
  /*
*/
  double yscale, xzero, yzero;
  /*
*/
  long xinterval, yinterval, xsubintervals, ysubintervals, xwidth, ywidth,
       xdecimal, ydecimal;
  double xsize, ysize;
  stringDelila xlabel, ylabel;
  boolean rotateylabel;
  /*
*/

  boolean crosshairs;
  /*
*/
  boolean doXaxis, doYaxis, doaxisline;

  boolean logxscale, logxnormal;
  double xbase, logxbase;
  /*
*/
  boolean logyscale, logynormal;
  double ybase, logybase;
  /*
*/

  long xcolumn, ycolumn, scolumn, xscolumn, yscolumn;
  /*
*/
  long hucolumn, sacolumn;
  /*
*/
  long brcolumn;
  /*
*/
  Char colorkind;
  /*
*/
  boolean needscolumn, needxscolumn, needyscolumn, needcocolumn;

  symbols *usersymbols;
  lines *userlines;

  double xrect, yrect;
  /*

*/
  boolean startrectangles;
  /*

*/

  boolean usermarks;
  /*
*/
  boolean warnedaboutusermarks;
  /*
*/

  Char fontcontrol;
  double ticscale, Xticscale, Yticscale, Xlabelshift, Ylabelshift;
} xyploparam;



Static _TEXT xyin, xyout, xyplom, xyplop, warnings;
Static long warningcount, datacount;
Static stringDelila uniquestring;
/*






*/



Static boolean inpicture;
/*
*/
Static double picxglobal, picyglobal, pictolerance;
/*
*/

Static double scale;

/*
*/
Static boolean inpath;

/*

*/
Static long segments;

Static double xsideold, ysideold;


Static jmp_buf _JL1;



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define tab             9






Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == tab);
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
  emptystring(ribbon);
  ribbon->next = NULL;
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




/*


*/

Static Void regress(control, x, y, sumx, sumy, sumxsqd, sumysqd, sumxy, ex,
		    ey, varx, vary, covxy, r, m, b, n)
Char control;
double x, y, *sumx, *sumy, *sumxsqd, *sumysqd, *sumxy, *ex, *ey, *varx, *vary,
       *covxy, *r, *m, *b;
long *n;
{




  /*









*/
  if (control != 'r' && control != 'e' && control != 'c') {
    printf(" linear regression control variable value, \"%c\" is not acceptable.\n",
	   control);
    printf(" it must be in [\"c\",\"e\",\"r\"]\n");
    halt();
    return;
  }

  switch (control) {

  case 'c':
    x = 0.0;
    y = 0.0;
    *sumx = 0.0;
    *sumy = 0.0;
    *sumxsqd = 0.0;
    *sumysqd = 0.0;
    *sumxy = 0.0;
    *ex = 0.0;
    *ey = 0.0;
    *varx = 0.0;
    *vary = 0.0;
    *covxy = 0.0;
    *r = 2.0;
    /*
*/

    *m = 0.0;
    *b = 0.0;
    *n = 0;
    break;

  case 'e':
    *sumx += x;
    *sumy += y;
    *sumxsqd += x * x;
    *sumysqd += y * y;
    *sumxy += x * y;
    (*n)++;
    break;

  case 'r':
    if (*n == 0) {
      printf("regress: n is 0; no samples recorded\n");
      halt();
    }
    if (*n == 1) {
      printf("regress: n is 1; regression impossible\n");
      halt();
    }

    *ex = *sumx / *n;
    *ey = *sumy / *n;

    *varx = *sumxsqd / *n - *ex * *ex;
    *vary = *sumysqd / *n - *ey * *ey;
    if (*varx == 0.0) {
      printf("regress: variance of x is zero; regression impossible\n");
      halt();
    }
    if (*vary == 0.0) {
      printf("regress: variance of y is zero; regression impossible\n");
      halt();
    }

    *covxy = *sumxy / *n - *ex * *ey;

    *r = *covxy / sqrt(*varx * *vary);

    *m = *covxy / *varx;
    *b = *ey - *m * *ex;
    break;

  }
}





Static Void await()
{
  /*
*/
  /*




*/
}



Static Void startpic(afile, setscale, x, y, thefont)
_TEXT *afile;
double setscale, x, y;
Char thefont;
{
  /*

*/
  fprintf(afile->f, "gsave\n");

  /*




*/

  scale = setscale;

  switch (thefont) {

  case 'c':
    fprintf(afile->f, "/Courier-Bold findfont\n");
    fprintf(afile->f, "%d scalefont\n", 10);
    fprintf(afile->f, "setfont\n");
    break;

  case 't':
    fprintf(afile->f, "/Times-Roman findfont\n");
    fprintf(afile->f, "%d scalefont\n", 12);
    fprintf(afile->f, "setfont\n");
    break;

  case 'n':
    fprintf(afile->f, "%% /Times-Roman findfont\n");
    fprintf(afile->f, "%%%d scalefont\n", 12);
    fprintf(afile->f, "%% setfont\n");
    break;
  }


  /*
























*/

  /*


*/


  /*

*/
  /*

*/

  /*

*/


  fprintf(afile->f, "%*.*f %*.*f translate\n\n",
	  picwidth, picdecim, x * scale, picwidth, picdecim, y * scale);

  fprintf(afile->f, "%% Define functions so the text produced is smaller\n");
  fprintf(afile->f, "/a {stroke newpath 0 0} def     %% special for arc\n");
  fprintf(afile->f, "/c {stroke 0 0 moveto} def      %% current point\n");
  fprintf(afile->f, "/f {findfont 10 scalefont setfont} def\n");
  fprintf(afile->f,
	  "   %% to set fonts simply use the f function.  Example:\n");
  fprintf(afile->f,
	  "   %%/Symbol f (\\142) /Courier-Bold f (-galactosidase\n");
  fprintf(afile->f, "/l {lineto} def\n");
  fprintf(afile->f, "/m {moveto} def\n");
  fprintf(afile->f, "/n {stroke newpath 0 0 moveto} def\n");
  fprintf(afile->f, "/rl {rlineto} def\n");
  fprintf(afile->f, "/rm {rmoveto} def\n");
  fprintf(afile->f, "/s {newpath 0 0 moveto} def     %% Start path \n");
  fprintf(afile->f, "/t {currentpoint translate} def %% translate \n");
  fprintf(afile->f, "/x {show} def                   %% show teXt \n\n");


  inpath = false;

  segments = 0;


  inpicture = true;
  picxglobal = 0.0;
  picyglobal = 0.0;
  pictolerance = (long)(exp(picwidth * log(10.0)) + 0.5);

}



Static Void stoppic(afile)
_TEXT *afile;
{
  if (inpath) {
    fprintf(afile->f, "stroke\n");
    inpath = false;
  }

  fprintf(afile->f, "showpage\n");
  fprintf(afile->f, "grestore\n");
  /*
*/

  await();

  inpicture = false;
}


#define buffer          10


Local Void checkseg(afile)
_TEXT *afile;
{
  /*
*/
  if (segments >= buffer) {
    fprintf(afile->f, "n\n");
    segments = 0;
  }

  else
    segments++;
}

#undef buffer



Static Void drawr(afile, dx, dy, visibility, spacing)
_TEXT *afile;
double dx, dy;
Char visibility;
double spacing;
{
  /*





*/
  double ddx, ddy, dr;
  boolean on;
  double y;
  long r;
  double ss, theta;
  long FORLIM;

  if (!inpath) {
    fprintf(afile->f, "s\n");
    inpath = true;
  }


  else
    checkseg(afile);
  /*


*/


  fprintf(afile->f, "%*.*f %*.*f",
	  picwidth, picdecim, dx * scale, picwidth, picdecim, dy * scale);
  switch (visibility) {

  case 'l':
  case 'i':
    switch (visibility) {

    case 'i':
      fprintf(afile->f, " m");
      break;

    case 'l':
      fprintf(afile->f, " l");
      break;
    }
    break;

  case '.':
  case '-':
    putc('\n', afile->f);
    if (spacing <= 0.0) {
      printf("drawr: spacing zero with . or - line\n");
      halt();
    }
    if (dx == 0.0) {
      ddx = 0.0;
      ddy = scale * spacing;
      if (dy < 0)
	ddy = -ddy;
      /*
*/
    } else {
      /*
*/
      theta = atan(fabs(dy / dx));

      ddx = scale * spacing * cos(theta);
      ddy = scale * spacing * sin(theta);


      if (dx < 0)
	ddx = -ddx;
      if (dy < 0)
	ddy = -ddy;
    }
    y = 0.0;
    switch (visibility) {

    case '.':
      ss = scale * dotfactor;
      break;

    case '-':
      on = true;
      break;
    }

    dr = sqrt(dx * dx + dy * dy);
    FORLIM = (long)floor(dr / spacing + 0.5);
    for (r = 1; r <= FORLIM; r++) {
      switch (visibility) {

      case '-':
	fprintf(afile->f, "%*.*f %*.*f",
		picwidth, picdecim, ddx, picwidth, picdecim, ddy);
	if (on)
	  fprintf(afile->f, " rl\n");
	else
	  fprintf(afile->f, " rm\n");
	on = !on;
	break;

      case '.':
	fprintf(afile->f, "%*.*f 0 rl", picwidth, picdecim, ss);
	fprintf(afile->f, " %*.*f 0 rl", picwidth, picdecim, -ss);
	fprintf(afile->f, " %*.*f %*.*f",
		picwidth, picdecim, ddx, picwidth, picdecim, ddy);
	fprintf(afile->f, " rm\n");
	break;

      }
    }

    fprintf(afile->f, " m\n");
    break;
  }

  /*
*/

  fprintf(afile->f, " t\n");

  picxglobal += dx;
  picyglobal += dy;
}



Static Void mover(afile, dx, dy)
_TEXT *afile;
double dx, dy;
{
  drawr(afile, dx, dy, 'i', 0.0);
}



Static Void liner(afile, dx, dy)
_TEXT *afile;
double dx, dy;
{
  drawr(afile, dx, dy, 'l', 0.0);
}




Static Void drawa(afile, x, y, visibility, spacing)
_TEXT *afile;
double x, y;
Char visibility;
double spacing;
{
  /*





*/
  double dx, dy;

  /*
*/
  dx = x - picxglobal;
  dy = y - picyglobal;

  drawr(afile, dx, dy, visibility, spacing);
}



Static Void movea(afile, x, y)
_TEXT *afile;
double x, y;
{
  drawa(afile, x, y, 'i', 0.0);
}



Static Void linea(afile, x, y)
_TEXT *afile;
double x, y;
{
  drawa(afile, x, y, 'l', 0.0);
}


Local Void postscriptprotect(afile, c)
_TEXT *afile;
Char c;
{
  /*





*/
}



Static Void graphstring(tofile, s, justification)
_TEXT *tofile;
stringDelila *s;
Char justification;
{
  /*










*/
  /*

*/
  long i;
  boolean quoted, skipping;
  long FORLIM;

  if (!(inpicture && s->length > 0))
    return;
  /*




*/
  if (s->length > 2) {
    if (s->letters[0] == '"' && s->letters[s->length - 1] == '"')
      quoted = true;
    else
      quoted = false;
  } else
    quoted = false;


  if (quoted)
    justification = 'c';


  if (justification != 'l')
    fprintf(tofile->f, "gsave ");



  fprintf(tofile->f, "c\n");

  if (justification == 'c') {
    if (s->letters[0] == ' ')
      skipping = true;
    else
      skipping = false;
  }

  else
    skipping = false;

  putc('(', tofile->f);
  if (quoted) {
    FORLIM = s->length;
    for (i = 2; i < FORLIM; i++) {
      if (s->letters[i-2] != '\\')
	postscriptprotect(tofile, s->letters[i-1]);
      putc(s->letters[i-1], tofile->f);
    }
  } else {
    FORLIM = s->length;
    for (i = 0; i < FORLIM; i++) {
      if (skipping) {
	if (s->letters[i] != ' ') {
	  skipping = false;
	  if (i == 0)
	    postscriptprotect(tofile, s->letters[0]);
	  else if (s->letters[i-1] != '\\')
	    postscriptprotect(tofile, s->letters[i]);
	  putc(s->letters[i], tofile->f);
	}

      } else {
	if (i == 0)
	  postscriptprotect(tofile, s->letters[0]);
	else if (s->letters[i-1] != '\\')
	  postscriptprotect(tofile, s->letters[i]);
	putc(s->letters[i], tofile->f);
      }
    }
  }
  putc(')', tofile->f);

  if (justification == 'c')
    fprintf(tofile->f, " dup stringwidth pop neg 2 div 0 rmoveto");
  else if (justification == 'r')
    fprintf(tofile->f, " dup stringwidth pop neg 0 rmoveto");

  fprintf(tofile->f, " x\n");
  inpath = false;

  if (justification != 'l')
    fprintf(tofile->f, "grestore ");
}



Static Void stringinteger(number, name, width, leadingzeros)
long number;
stringDelila *name;
long width;
boolean leadingzeros;
{
  /*

*/
  long bigdigit, dig, place, sign;

  if (number < 0) {
    sign = -1;
    name->length++;
    number = -number;
    if (leadingzeros)
      printf(
	"WARNING: stringinteger: the sign of a negative number with leading zeros is lost\n");
  } else
    sign = 1;

  /*


*/
  if (number > 9)
    dig = (long)(log(number + 0.1) / log(10.0)) + 1;
  else
    dig = 1;

  if (dig > width) {
    printf("stringinteger: number width too small\n");
    printf("%ld digit number (%ld)\n", dig, number);
    printf("does not fit in %ld characters\n", width);
    halt();
  }
  if (leadingzeros)
    bigdigit = name->length + 1;
  else {
    bigdigit = name->length + width - dig + 1;

    if (bigdigit <= name->length && sign < 0) {
      printf("stringinteger: no room for sign\n");
      halt();
    }
  }
  if (sign < 0)
    name->letters[bigdigit-2] = '-';

  for (place = name->length + width - 1; place >= bigdigit - 1; place--) {
    switch (number % 10) {

    case 0:
      name->letters[place] = '0';
      break;

    case 1:
      name->letters[place] = '1';
      break;

    case 2:
      name->letters[place] = '2';
      break;

    case 3:
      name->letters[place] = '3';
      break;

    case 4:
      name->letters[place] = '4';
      break;

    case 5:
      name->letters[place] = '5';
      break;

    case 6:
      name->letters[place] = '6';
      break;

    case 7:
      name->letters[place] = '7';
      break;

    case 8:
      name->letters[place] = '8';
      break;

    case 9:
      name->letters[place] = '9';
      break;
    }
    number /= 10;
  }
  name->length += width;
}



Static Void stringreal(number, name, width, decimal)
double number;
stringDelila *name;
long width, decimal;
{
  /*

*/
  /*





*/
  long abovezero;
  /*
*/
  long shift;
  /*
*/
  long sign, thedecimal, theupper, signspot;

  if (name->length + width > maxstring) {
    printf("real number =% .1E would exceed maxstring = %ld\n",
	   number, (long)maxstring);
    halt();
  }
  if (number < 0)
    sign = -1;
  else
    sign = 1;

  number = fabs(number);


  shift = (long)floor(exp(decimal * log(10.0)) + 0.5);
  abovezero = (long)floor(number * shift + 0.5);
  theupper = (long)((double)abovezero / shift);
  thedecimal = abovezero - shift * theupper;
  /*



*/






  signspot = name->length + 1;
  stringinteger(sign * theupper, name, width - decimal - 1, false);

  /*






*/
  if (sign < 1 && theupper == 0 &&
      (long)floor(exp(decimal * log(10.0)) * number + 0.5) != 0) {
    /*




*/
    /*
*/
    while (name->letters[signspot] == ' ')
      signspot++;
    name->letters[signspot-1] = '-';
  }

  /*





*/


  name->length++;
  name->letters[name->length - 1] = '.';
  stringinteger(thedecimal, name, decimal, true);
}



Static Void picnumber(afile, dx, dy, number, width, decimal, justification)
_TEXT *afile;
double dx, dy, number;
long width, decimal;
Char justification;
{
  /*





*/
  stringDelila name;

  if (width <= 0)
    return;
  mover(afile, dx, dy);

  clearstring(&name);

  if (decimal > 0)
    stringreal(number, &name, width, decimal);
  else
    stringinteger((long)floor(number + 0.5), &name, width, false);

  graphstring(afile, &name, justification);
  mover(afile, -dx, -dy);
}



Static Void xtic(afile, length, dx, dy, number, width, decimal, logxnormal,
		 logxbase)
_TEXT *afile;
double length, dx, dy, number;
long width, decimal;
boolean logxnormal;
double logxbase;
{
  /*






*/
  liner(afile, 0.0, -length);
  if (logxnormal)
    picnumber(afile, dx, dy, exp(number * logxbase), width, decimal, 'c');
  else
    picnumber(afile, dx, dy, number, width, decimal, 'c');
  mover(afile, 0.0, length);
}



Static Void ytic(afile, length, dx, dy, number, width, decimal, logynormal,
		 logybase)
_TEXT *afile;
double length, dx, dy, number;
long width, decimal;
boolean logynormal;
double logybase;
{
  /*






*/
  liner(afile, -length, 0.0);


  if (logynormal)
    number = exp(number * logybase);

  picnumber(afile, dx, dy, number, width, decimal, 'r');
  mover(afile, length, 0.0);
}



Static Void doaxis(afile, theaxis, doaxisline, alength, fromtic, interval,
		   totic, subintervals, length, dx, dy, width, decimal,
		   logscale, lognormal, logbase)
_TEXT *afile;
Char theaxis;
boolean doaxisline;
double alength, fromtic, interval, totic, subintervals, length, dx, dy;
long width, decimal;
boolean logscale, lognormal;
double logbase;
{
  /*












*/
  double half;
  /*

*/
  double jump;
  double jumpdistance = 0.0;
  /*

*/
  double tic;

  boolean dosubtics;
  double subtic, subinterval, subjump, halfsubinterval;

  double currentspot, oldspot, axisscale;

  fprintf(afile->f, "gsave\n");

  /*




*/

  if (theaxis == 'x') {
    if (doaxisline) {
      liner(afile, alength, 0.0);
      mover(afile, -alength, 0.0);
    }
  } else {
    if (doaxisline) {
      liner(afile, 0.0, alength);
      mover(afile, 0.0, -alength);
    }
  }

  if (totic == fromtic) {
    printf("doaxis: %c axis fromtic and totic cannot be equal\n", theaxis);
    halt();
  }

  if (alength == 0.0 || interval == 0.0) {
    printf("doaxis: neither %c axis length nor interval can be zero\n",
	   theaxis);

    halt();
  }

  axisscale = alength / (totic - fromtic);
  jump = axisscale * interval;
  half = interval / 2.0;

  if (subintervals > 1) {
    dosubtics = true;
    subinterval = interval / subintervals;
    halfsubinterval = subinterval / 2.0;
    subjump = jump / subintervals;
  } else {
    dosubtics = false;
    subinterval = 0.0;
    halfsubinterval = 0.0;
    subjump = 0.0;
  }
  /*




*/

  tic = fromtic;
  if (interval > 0.0) {
    while (tic <= totic + interval) {
      /*

*/
      /*

*/
      if (tic <= totic + interval / 2) {
	if (theaxis == 'x')
	  xtic(afile, length, dx, dy, tic, width, decimal, lognormal, logbase);
	else
	  ytic(afile, length, dx, dy, tic, width, decimal, lognormal, logbase);
      }

      /*
*/
      if (tic <= totic + interval) {
	/*






*/
	if (dosubtics) {
	  if (logscale) {
	    /*


*/
	    subtic = exp(tic * logbase);
	    /*
*/
	    /*

*/
	    subinterval = (exp((tic + interval) * logbase) - subtic) / subintervals;
	    halfsubinterval = subinterval / 2.0;

	    /*



*/

	    oldspot = axisscale * tic;
	    while (subtic < exp(logbase * (tic + interval)) - halfsubinterval) {
	      /*

*/

	      /*
*/
	      subtic += subinterval;


	      currentspot = axisscale * log(subtic) / logbase;
	      subjump = currentspot - oldspot;
	      /*






*/
	      oldspot = currentspot;


	      if (theaxis == 'x') {
		xtic(afile, length / 2, dx, dy, 0.0, 0L, 0L, lognormal,
		     logbase);
		mover(afile, subjump, 0.0);
	      } else {
		ytic(afile, length / 2, dx, dy, 0.0, 0L, 0L, lognormal,
		     logbase);
		mover(afile, 0.0, subjump);
	      }

	      jumpdistance += subjump;
	    }

	  }

	  else {
	    subtic = tic;
	    while (subtic < tic + interval - halfsubinterval) {
	      subtic += subinterval;

	      if (theaxis == 'x') {
		mover(afile, subjump, 0.0);
		if (subtic <= totic)
		  xtic(afile, length / 2, dx, dy, 0.0, 0L, 0L, lognormal,
		       logbase);
	      } else {
		mover(afile, 0.0, subjump);
		if (subtic <= totic)
		  ytic(afile, length / 2, dx, dy, 0.0, 0L, 0L, lognormal,
		       logbase);
	      }

	      jumpdistance += subjump;
	    }
	  }
	} else {
	  if (theaxis == 'x')
	    mover(afile, jump, 0.0);
	  else
	    mover(afile, 0.0, jump);
	  jumpdistance += jump;
	}
      }
      tic += interval;
    }
  } else if (interval < 0.0) {
    while (tic >= totic - half) {
      if (dosubtics)
	printf("Sorry, no subtics with negative scales\n");

      if (theaxis == 'x')
	xtic(afile, length, dx, dy, tic, width, decimal, lognormal, logbase);
      else
	ytic(afile, length, dx, dy, tic, width, decimal, lognormal, logbase);

      tic += interval;
      if (tic < totic - half)
	break;
      if (theaxis == 'x')
	mover(afile, jump, 0.0);
      else
	mover(afile, 0.0, jump);
      jumpdistance += jump;
    }
  }

  if (theaxis == 'x')
    mover(afile, -jumpdistance, 0.0);
  else
    mover(afile, 0.0, -jumpdistance);
  fprintf(afile->f, "grestore\n");
}



Static Void xaxis(afile, doaxisline, axlength, fromtic, interval, totic,
		  xsubintervals, length, dx, dy, width, decimal, logxscale,
		  logxnormal, logxbase)
_TEXT *afile;
boolean doaxisline;
double axlength, fromtic, interval, totic, xsubintervals, length, dx, dy;
long width, decimal;
boolean logxscale, logxnormal;
double logxbase;
{
  doaxis(afile, 'x', doaxisline, axlength, fromtic, interval, totic,
	 xsubintervals, length, dx, dy, width, decimal, logxscale, logxnormal,
	 logxbase);
}



Static Void yaxis(afile, doaxisline, aylength, fromtic, interval, totic,
		  ysubintervals, length, dx, dy, width, decimal, logyscale,
		  logynormal, logybase)
_TEXT *afile;
boolean doaxisline;
double aylength, fromtic, interval, totic, ysubintervals, length, dx, dy;
long width, decimal;
boolean logyscale, logynormal;
double logybase;
{
  doaxis(afile, 'y', doaxisline, aylength, fromtic, interval, totic,
	 ysubintervals, length, dx, dy, width, decimal, logyscale, logynormal,
	 logybase);
}





Static Void cboxr(afile, width, height)
_TEXT *afile;
double width, height;
{
  /*



*/
  double h2, w2;

  h2 = height / 2;
  w2 = width / 2;
  mover(afile, -w2, -h2);
  liner(afile, 0.0, height);
  liner(afile, width, 0.0);
  liner(afile, 0.0, -height);
  liner(afile, -width, 0.0);
  mover(afile, w2, h2);
}



Static Void skybox(afile, x, y, width, height)
_TEXT *afile;
double x, y, width, height;
{
  /*


*/
  double h2, w2;

  h2 = height / 2;
  w2 = width / 2;
  movea(afile, x - h2, 0.0);
  linea(afile, x + h2, 0.0);
  linea(afile, x + h2, y);
  linea(afile, x - h2, y);
  linea(afile, x - h2, 0.0);
}



Static Void dotr(afile)
_TEXT *afile;
{
  /*

*/
  drawr(afile, dotfactor, 0.0, 'l', 0.0);
}



Static double degtorad(angle)
double angle;
{
  return (angle / 360 * 2 * pi);
}



Static Void polrec(r, theta, x, y)
double r, theta, *x, *y;
{
  /*
*/
  *x = r * cos(theta);
  *y = r * sin(theta);
}



Static Void arc(thefile, angle1, angle2, radius, steps)
_TEXT *thefile;
double angle1, angle2, radius;
long steps;
{
  /*


*/
  double dtheta;

  double theta, x, y;

  /*


*/
  theta = degtorad(angle1);
  dtheta = degtorad(fabs(angle2 - angle1) / steps);
  polrec(radius, theta, &x, &y);
  /*



*/




  fprintf(thefile->f, "a %*.*f %*.*f %*.*f\n",
	  picwidth, picdecim, scale * radius, picwidth, picdecim, angle1,
	  picwidth, picdecim, angle2);
  fprintf(thefile->f, "arc");
  if (angle2 < angle1)
    putc('n', thefile->f);

  fprintf(thefile->f, " n\n");


  /*







*/
}



Static Void circler(afile, radius)
_TEXT *afile;
double radius;
{
  long steps;

  /*


*/
  if (radius < 1.0)
    steps = 25;
  else
    steps = (long)floor(radius * 25 + 0.5);

  arc(afile, 0.0, 360.0, radius, steps);
}



Static Void ibeam(afile, width, height)
_TEXT *afile;
double width, height;
{
  /*


*/
  double h2, w2, r;

  h2 = height / 2;
  w2 = width / 2;
  mover(afile, -w2, -h2);
  liner(afile, width, 0.0);
  mover(afile, -width, height);
  liner(afile, width, 0.0);
  mover(afile, -w2, 0.0);
  liner(afile, 0.0, -height);
  mover(afile, 0.0, h2);
  r = width / 8;
  /*

*/
  circler(afile, r);
}



Static Void xr(afile, width, height)
_TEXT *afile;
double width, height;
{
  /*



*/
  double h2, w2;

  h2 = height / 2;
  w2 = width / 2;
  mover(afile, -w2, -h2);
  liner(afile, width, height);
  mover(afile, 0.0, -height);
  liner(afile, -width, height);
  mover(afile, w2, -h2);
}



Static Void plusr(afile, width, height)
_TEXT *afile;
double width, height;
{
  /*



*/
  double h2, w2;

  h2 = height / 2;
  w2 = width / 2;
  mover(afile, -w2, 0.0);
  liner(afile, width, 0.0);
  mover(afile, -w2, h2);
  liner(afile, 0.0, -height);
  mover(afile, 0.0, h2);
}



Static Void rectinit(outfile)
_TEXT *outfile;
{
  /*
*/
  fprintf(outfile->f, "/rct\n");
  fprintf(outfile->f, "{gsave\n");
  fprintf(outfile->f, "  newpath\n");
  fprintf(outfile->f, "  0 0 moveto\n");
  fprintf(outfile->f, "  xs 0 lineto\n");
  fprintf(outfile->f, "  xs ys lineto\n");
  fprintf(outfile->f, "  0 ys lineto\n");
  fprintf(outfile->f, "  closepath fill\n");
  fprintf(outfile->f, "grestore} def\n");
}



Static Void rectsize(afile, xsideold, ysideold, xside, yside)
_TEXT *afile;
double *xsideold, *ysideold, *xside, *yside;
{
  /*

*/
  if (*xside != *xsideold) {
    fprintf(afile->f, "/xs %*.*f def\n", picwidth, picdecim, *xside * scale);
    *xsideold = *xside;
  }

  if (*yside != *ysideold) {
    fprintf(afile->f, "/ys %*.*f def\n", picwidth, picdecim, *yside * scale);
    *ysideold = *yside;
  }
}



Static Void rectdo(afile)
_TEXT *afile;
{
  /*

*/
  fprintf(afile->f, " rct\n");
}


#define colfield        8
#define colwidth        4



Static Void setgray(afile, brightness)
_TEXT *afile;
double brightness;
{
  /*
*/
  putc('n', afile->f);
  fprintf(afile->f, " %*.*f", colfield, colwidth, brightness);
  fprintf(afile->f, " setgray\n");
}

#undef colfield
#undef colwidth


#define colfield        8
#define colwidth        4

#define huefactor       0.84



Static Void setcolor(afile, colorkind, hue, saturation, brightness)
_TEXT *afile;
Char colorkind;
double hue, saturation, brightness;
{
  /*

























*/
  putc('n', afile->f);
  /*

*/
  if (colorkind == 'h') {
    fprintf(afile->f, " %*.*f",
	    colfield, colwidth, hue * huefactor + 1 - huefactor);
    fprintf(afile->f, " %*.*f", colfield, colwidth, saturation);
    fprintf(afile->f, " %*.*f", colfield, colwidth, brightness);
    fprintf(afile->f, " sethsbcolor\n");
    return;
  }
  fprintf(afile->f, " %*.*f", colfield, colwidth, hue);
  fprintf(afile->f, " %*.*f", colfield, colwidth, saturation);
  fprintf(afile->f, " %*.*f", colfield, colwidth, brightness);
  fprintf(afile->f, " setrgbcolor\n");
}

#undef colfield
#undef colwidth
#undef huefactor


/* Local variables for boxintercept: */
struct LOC_boxintercept {
  double xmin, ymin, xmax, ymax, m, b;
  boolean *intercept;
  double *x1, *y1, *x2, *y2;
  long count;
  boolean xlo, xhi, ylo, yhi;
} ;

/*
*/
Local double fny(x, LINK)
double x;
struct LOC_boxintercept *LINK;
{
  return (LINK->m * x + LINK->b);
}

Local double fnx(y, LINK)
double y;
struct LOC_boxintercept *LINK;
{
  return ((y - LINK->b) / LINK->m);
}

Local boolean between(a, b, c, LINK)
double a, b, c;
struct LOC_boxintercept *LINK;
{
  return (a <= b && b <= c);
}

Local Void normalcases(LINK)
struct LOC_boxintercept *LINK;
{
  /*











*/
  LINK->xlo = between(LINK->ymin, fny(LINK->xmin, LINK), LINK->ymax, LINK);
  LINK->xhi = between(LINK->ymin, fny(LINK->xmax, LINK), LINK->ymax, LINK);
  LINK->ylo = between(LINK->xmin, fnx(LINK->ymin, LINK), LINK->xmax, LINK);
  LINK->yhi = between(LINK->xmin, fnx(LINK->ymax, LINK), LINK->xmax, LINK);
  /*




*/

  *LINK->intercept = true;

  /*
*/
  LINK->count = 0;
  if (LINK->xlo)
    LINK->count++;
  if (LINK->xhi)
    LINK->count++;
  if (LINK->ylo)
    LINK->count++;
  if (LINK->yhi)
    LINK->count++;

  if (LINK->count > 2) {
    /*

*/
    if (LINK->xlo && LINK->xhi) {
      LINK->yhi = false;
      LINK->ylo = false;
    } else if (LINK->ylo && LINK->yhi) {
      LINK->xhi = false;
      LINK->xlo = false;
    } else {
      printf("error in between count!\n");
      halt();
    }
  }

  if (LINK->xlo && LINK->xhi) {
    *LINK->x1 = LINK->xmin;
    *LINK->x2 = LINK->xmax;
  } else if (LINK->xlo && LINK->ylo) {
    *LINK->x1 = LINK->xmin;
    *LINK->x2 = fnx(LINK->ymin, LINK);
  } else if (LINK->xlo && LINK->yhi) {
    *LINK->x1 = LINK->xmin;
    *LINK->x2 = fnx(LINK->ymax, LINK);
  } else if (LINK->xhi && LINK->ylo) {
    *LINK->x1 = LINK->xmax;
    *LINK->x2 = fnx(LINK->ymin, LINK);
  } else if (LINK->xhi && LINK->yhi) {
    *LINK->x1 = LINK->xmax;
    *LINK->x2 = fnx(LINK->ymax, LINK);
  } else if (LINK->ylo && LINK->yhi) {
    *LINK->x1 = fnx(LINK->ymin, LINK);
    *LINK->x2 = fnx(LINK->ymax, LINK);
  } else
    *LINK->intercept = false;

  if (*LINK->intercept) {
    *LINK->y1 = fny(*LINK->x1, LINK);
    *LINK->y2 = fny(*LINK->x2, LINK);
  }

}



Static Void boxintercept(xmin_, ymin_, xmax_, ymax_, m_, b_, intercept_, x1_,
			 y1_, x2_, y2_)
double xmin_, ymin_, xmax_, ymax_, m_, b_;
boolean *intercept_;
double *x1_, *y1_, *x2_, *y2_;
{
  /*

*/
  struct LOC_boxintercept V;

  V.xmin = xmin_;
  V.ymin = ymin_;
  V.xmax = xmax_;
  V.ymax = ymax_;
  V.m = m_;
  V.b = b_;
  V.intercept = intercept_;
  V.x1 = x1_;
  V.y1 = y1_;
  V.x2 = x2_;
  V.y2 = y2_;
  if (fabs(V.m) != 0.0) {
    normalcases(&V);
    return;
  }
  *V.intercept = between(V.ymin, V.b, V.ymax, &V);
  if (!*V.intercept)
    return;
  *V.x1 = V.xmin;
  *V.y1 = V.b;
  *V.x2 = V.xmax;
  *V.y2 = V.b;
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



Static Void tellwarning()
{
  /*
*/
  warningcount++;
  if (warningcount == 1) {
    if (*warnings.name != '\0') {
      if (warnings.f != NULL)
	warnings.f = freopen(warnings.name, "w", warnings.f);
      else
	warnings.f = fopen(warnings.name, "w");
    } else {
      if (warnings.f != NULL)
	rewind(warnings.f);
      else
	warnings.f = tmpfile();
    }
    if (warnings.f == NULL)
      _EscIO2(FileNotFound, warnings.name);
    SETUPBUF(warnings.f, Char);
    fprintf(warnings.f, "xyplo %4.2f\n", version);
  }
  fprintf(warnings.f, "\n%ld ", warningcount);
}



Static Void marksdo(afile, width, height, flagstring)
_TEXT *afile;
double width, height;
stringDelila flagstring;
{
  if (flagstring.length == 0) {
    printf("WARNING: user defined mark not used in xyin\n");
    return;
  }
  fprintf(afile->f, "\ngsave %% marksdo\n");
  fprintf(afile->f, "%*.*f %*.*f ",
	  picwidth, picdecim, scale * width, picwidth, picdecim,
	  scale * height);
  writestring(afile, &flagstring);
  fprintf(afile->f, "\ngrestore %% marksdo\n\n");
}



Static Void gettoken(infile, flag, tokenstring, gots)
_TEXT *infile;
Char *flag;
stringDelila *tokenstring;
boolean *gots;
{
  /*

*/
  boolean done = false;

  clearstring(tokenstring);
  while (!done) {
    if (P_eoln(infile->f)) {
      done = true;
      break;
    }
    if ((P_peek(infile->f) == ' ') | (P_peek(infile->f) == '\t')) {
      done = true;
      break;
    }
    tokenstring->length++;
    tokenstring->letters[tokenstring->length - 1] = getc(infile->f);
    if (tokenstring->letters[tokenstring->length - 1] == '\n')
      tokenstring->letters[tokenstring->length - 1] = ' ';
  }
  *gots = (tokenstring->length != 0);
  *flag = tokenstring->letters[0];
}



Static boolean equalstring(a, b)
stringDelila a, b;
{
  boolean notequal;
  long j;

  if (a.length != b.length)
    return false;
  else {
    j = 1;
    notequal = false;

    do {
      notequal = (a.letters[j-1] != b.letters[j-1]);
      j++;
    } while (!(j > a.length || notequal));

    return (!notequal && j > a.length);
  }
}



Static Void bar(f, c)
_TEXT *f;
Char c;
{
  long i;

  for (i = 1; i <= 50; i++)
    putc(c, f->f);
  putc('\n', f->f);
}


/* Local variables for readsymbol: */
struct LOC_readsymbol {
  _TEXT *f;
} ;

Local Void t(LINK)
struct LOC_readsymbol *LINK;
{
  if (BUFEOF(LINK->f->f)) {
    printf("missing symbol parameters\n");
    halt();
  }
}



Static Void readsymbol(f_, symb, p, s)
_TEXT *f_;
Char symb;
xyploparam *p;
symbol *s;
{
  struct LOC_readsymbol V;
  Char flag;
  boolean gotsymbol;
  line *WITH1;
  _TEXT TEMP;

  V.f = f_;
  s->symboltoplot = symb;
  WITH1 = &s->l;

  t(&V);
  gettoken(V.f, &flag, &s->symbolflag, &gotsymbol);
  if (!gotsymbol) {
    printf("Could not find symbol flag for symbol \"%c\"\n", symb);
    printf("while reading symbols in the xyplop file.\n");
    printf("Trying to read a symbol from this token:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &s->symbolflag);
    printf("\nThe rest of the file contains: \n");
    while (!BUFEOF(V.f->f)) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      copyaline(V.f, &TEMP);
    }
    halt();
  }
  fscanf(V.f->f, "%*[^\n]");

  getc(V.f->f);
  t(&V);
  fscanf(V.f->f, "%lg%*[^\n]", &s->symbolxsize);
  getc(V.f->f);
  if (s->symbolxsize <= 0 && !p->needxscolumn) {
    printf("While reading symbol \"%c\" with flag \"%c\" you asked that its\n",
	   symb, flag);
    printf("x size be determined from a column.\n");
    printf("unfortunately you did not specify a positive column!\n");
    halt();
  }



  t(&V);
  fscanf(V.f->f, "%lg%*[^\n]", &s->symbolysize);
  getc(V.f->f);
  if (s->symbolysize <= 0 && !p->needyscolumn) {
    printf("While reading symbol \"%c\" with flag \"%c\" you asked that its\n",
	   symb, flag);
    printf("y size be determined from a column.\n");
    printf("unfortunately you did not specify a positive column!\n");
    halt();
  }



  if (s->symbolxsize == 0.0 || s->symbolysize == 0.0) {
    printf("symbol size cannot be zero\n");
    halt();
  }


  if (P_peek(V.f->f) == 'c') {
    getc(V.f->f);
    if ((P_peek(V.f->f) == 'n') | (P_peek(V.f->f) == 'i'))
      s->doconnection = false;
    else {
      s->doconnection = true;
      s->connecttype = getc(V.f->f);
      if (s->connecttype == '\n')
	s->connecttype = ' ';
      if (s->connecttype == '.' || s->connecttype == '-') {
	skipnonblanks(V.f);
	fscanf(V.f->f, "%lg", &s->connectsize);
      } else
	s->connectsize = 0.05;
      s->didlastpoint = false;
    }
  } else
    s->doconnection = false;
  fscanf(V.f->f, "%*[^\n]");


  getc(V.f->f);
  if (!P_eoln(V.f->f)) {
    WITH1->linetype = getc(V.f->f);
    if (WITH1->linetype == '\n')
      WITH1->linetype = ' ';
    if (WITH1->linetype == 'l' || WITH1->linetype == '.' ||
	WITH1->linetype == '-' || WITH1->linetype == 'i')
      s->doline = true;
    else if (WITH1->linetype == 'n')
      s->doline = false;
    else {
      printf("for symbol %c line type must be one of \"lin.-\"\n",
	     s->symboltoplot);
      halt();
    }
    if (WITH1->linetype == '-' || WITH1->linetype == '.') {
      t(&V);
      skipnonblanks(V.f);
      fscanf(V.f->f, "%lg", &WITH1->linesize);
    } else
      WITH1->linesize = 0.05;
  }
  fscanf(V.f->f, "%*[^\n]");
  getc(V.f->f);
  s->symbolhasxyplomdef = false;
  if (symb == 'r' || symb == 'R')
    p->startrectangles = true;
}



Static Void copyfile(fin, fout)
_TEXT *fin, *fout;
{
  while (!BUFEOF(fin->f))
    copyaline(fin, fout);
}



Static Void upgrade1(xyplop, p)
_TEXT *xyplop;
xyploparam p;
{
  _TEXT internal;

  internal.f = NULL;
  *internal.name = '\0';
  if (*xyplop->name != '\0') {
    if (xyplop->f != NULL)
      xyplop->f = freopen(xyplop->name, "r", xyplop->f);
    else
      xyplop->f = fopen(xyplop->name, "r");
  } else
    rewind(xyplop->f);
  if (xyplop->f == NULL)
    _EscIO2(FileNotFound, xyplop->name);
  RESETBUF(xyplop->f, Char);
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


  copyfile(xyplop, &internal);


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
  if (*xyplop->name != '\0') {
    if (xyplop->f != NULL)
      xyplop->f = freopen(xyplop->name, "w", xyplop->f);
    else
      xyplop->f = fopen(xyplop->name, "w");
  } else {
    if (xyplop->f != NULL)
      rewind(xyplop->f);
    else
      xyplop->f = tmpfile();
  }
  if (xyplop->f == NULL)
    _EscIO2(FileNotFound, xyplop->name);
  SETUPBUF(xyplop->f, Char);
  copyfile(&internal, xyplop);


  fprintf(xyplop->f, "          **** more parameters\n");
  fprintf(xyplop->f, "p %4.2f %4.2f %4.2f %4.2f edgecontrol (p=page),",
	  p.edgeleft, p.edgeright, p.edgelow, p.edgehigh);
  fprintf(xyplop->f, " edgeleft, edgeright, edgelow, edgehigh in cm\n");
  fprintf(xyplop->f,
    "%4.2f version of xyplo that this parameter file is designed for.\n",
    version);

  if (*xyplop->name != '\0') {
    if (xyplop->f != NULL)
      xyplop->f = freopen(xyplop->name, "r", xyplop->f);
    else
      xyplop->f = fopen(xyplop->name, "r");
  } else
    rewind(xyplop->f);
  if (xyplop->f == NULL)
    _EscIO2(FileNotFound, xyplop->name);
  RESETBUF(xyplop->f, Char);
  if (internal.f != NULL)
    fclose(internal.f);
}


#define columns         8


/* Local variables for readparam: */
struct LOC_readparam {
  _TEXT *f;
  boolean checkout;
  long symbolcount;
} ;

Local Void helpem(LINK)
struct LOC_readparam *LINK;
{
  _TEXT TEMP;

  printf("\nTo help you locate the problem, here's the rest\n");
  printf("of the xyplop file:\n");
  printf(
    "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
  while (!BUFEOF(xyplop.f)) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    copyaline(&xyplop, &TEMP);
  }
  printf(
    "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n");
  halt();
}

Local Void cn(LINK)
struct LOC_readparam *LINK;
{
  LINK->checkout = checknumber(LINK->f);
  if (!LINK->checkout)
    helpem(LINK);
}

Local Void t_(LINK)
struct LOC_readparam *LINK;
{
  if (BUFEOF(LINK->f->f)) {
    printf("Missing parameters: unexpected end of xyplop\n");
    halt();
  }
}

Local Void num(f, LINK)
_TEXT *f;
struct LOC_readparam *LINK;
{
  skipblanks(f);
  if (P_peek(f->f) != '9' && P_peek(f->f) != '8' && P_peek(f->f) != '7' &&
      P_peek(f->f) != '6' && P_peek(f->f) != '5' && P_peek(f->f) != '4' &&
      P_peek(f->f) != '3' && P_peek(f->f) != '2' && P_peek(f->f) != '1' &&
      P_peek(f->f) != '0' && P_peek(f->f) != '+' && P_peek(f->f) != '-') {
    printf(
      "found this character: \"%c\" when expecting a number in the parameter file\n",
      P_peek(f->f));
    helpem(LINK);
  }
}

Local boolean softnumbertest(f, LINK)
_TEXT *f;
struct LOC_readparam *LINK;
{
  _TEXT TEMP;

  skipblanks(f);
  if (P_peek(f->f) != '9' && P_peek(f->f) != '8' && P_peek(f->f) != '7' &&
      P_peek(f->f) != '6' && P_peek(f->f) != '5' && P_peek(f->f) != '4' &&
      P_peek(f->f) != '3' && P_peek(f->f) != '2' && P_peek(f->f) != '1' &&
      P_peek(f->f) != '0' && P_peek(f->f) != '+' && P_peek(f->f) != '-') {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    bar(&TEMP, '-');
    printf("NOTE: in xyplop, another number is now allowed for this line:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    copyaline(f, &TEMP);
    return false;
  } else
    return true;
}

Local boolean notok(a, b)
long a, b;
{
  /*

*/
  if (a == b && a > 0)
    return true;
  else
    return false;
}

Local Void testcolumns(a, b, c, d, e, f, g, h, LINK)
long a, b, c, d, e, f, g, h;
struct LOC_readparam *LINK;
{
  /*
*/
  long all[8];
  boolean die = false;
  long x, y;

  all[0] = a;
  all[1] = b;
  all[2] = c;
  all[3] = d;
  all[4] = e;
  all[5] = f;
  all[6] = g;
  all[7] = h;

  for (x = 0; x <= columns - 2; x++) {
    for (y = x + 1; y < columns; y++) {
      if (notok(all[x], all[y])) {
	if (!die)
	  printf("no two columns can have the same positive position\n");
	printf("columns in conflict are numbered %ld\n", all[x]);
	die = true;
      }
    }
  }
  if (die)
    helpem(LINK);
}

#undef columns

/* Local variables for nextsection: */
struct LOC_nextsection {
  struct LOC_readparam *LINK;
  Char id;
} ;

Local Void nosection(LINK)
struct LOC_nextsection *LINK;
{
  printf("ERROR: SECTION DIVIDER NOT FOUND\n");
  printf("Section divider lines in xyplop may (but need not) begin with blanks\n");
  printf("and these must be followed by at least one asterisk (\"*\").\n");
  printf("Check that your xyplop matches the current documentation.\n");
  printf("The error was found while\n");
  printf("Xyplo was looking for the section divider before ");
  switch (LINK->id) {

  case 'c':
    printf("the COLUMN section.\n");
    break;

  case 's':
    printf("SYMBOL number %ld.\n", LINK->LINK->symbolcount);
    break;

  case 'l':
    printf("the LINE section.\n");
    break;

  case 'm':
    printf("the MORE PARAMETERS section.\n");
    break;
  }
  helpem(LINK->LINK);

}

Local Void nextsection(id_, LINK)
Char id_;
struct LOC_readparam *LINK;
{
  /*





*/
  struct LOC_nextsection V;

  V.LINK = LINK;
  V.id = id_;
  skipblanks(LINK->f);
  if (P_eoln(LINK->f->f))
    nosection(&V);
  if (P_peek(LINK->f->f) != '*')
    nosection(&V);
  fscanf(LINK->f->f, "%*[^\n]");
  getc(LINK->f->f);
}



Static Void readparam(f_, p)
_TEXT *f_;
xyploparam *p;
{
  struct LOC_readparam V;
  boolean moreparameters = false;
  /*
*/
  boolean gotten;
  Char symb;
  /*
*/
  lines *ul;
  symbols *us;
  _TEXT TEMP;
  line *WITH1;



  V.f = f_;
  V.checkout = true;
  if (*V.f->name != '\0') {
    if (V.f->f != NULL)
      V.f->f = freopen(V.f->name, "r", V.f->f);
    else
      V.f->f = fopen(V.f->name, "r");
  } else
    rewind(V.f->f);
  if (V.f->f == NULL)
    _EscIO2(FileNotFound, V.f->name);
  RESETBUF(V.f->f, Char);

  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%lg", &p->xzero);
  num(V.f, &V);
  fscanf(V.f->f, "%lg%*[^\n]", &p->yzero);
  getc(V.f->f);


  p->setx = false;
  t_(&V);
  if (!P_eoln(V.f->f)) {
    if (P_peek(V.f->f) == 'x') {
      p->setx = true;
      skipnonblanks(V.f);
      t_(&V);
      num(V.f, &V);
      fscanf(V.f->f, "%lg", &p->xmin);
      num(V.f, &V);
      fscanf(V.f->f, "%lg", &p->xmax);
      if (p->xmin >= p->xmax) {
	printf("xmin (%1.1f) cannot be >= xmax (%1.1f) in xyplop\n",
	       p->ymin, p->xmax);
	halt();
      }
    }
  }
  t_(&V);
  fscanf(V.f->f, "%*[^\n]");
  getc(V.f->f);
  if (!p->setx) {
    p->xmin = LONG_MAX;
    p->xmax = -LONG_MAX;
  }


  p->sety = false;
  t_(&V);
  if (!P_eoln(V.f->f)) {
    if (P_peek(V.f->f) == 'y') {
      p->sety = true;
      skipnonblanks(V.f);
      t_(&V);
      num(V.f, &V);
      fscanf(V.f->f, "%lg", &p->ymin);
      num(V.f, &V);
      fscanf(V.f->f, "%lg", &p->ymax);
      if (p->ymin >= p->ymax) {
	printf("ymin (%1.1f) cannot be >= ymax (%1.1f) in xyplop\n",
	       p->ymin, p->ymax);
	halt();
      }
    }
  }
  t_(&V);
  fscanf(V.f->f, "%*[^\n]");
  getc(V.f->f);
  if (!p->sety) {
    p->ymin = LONG_MAX;
    p->ymax = -LONG_MAX;
  }


  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->xinterval);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->yinterval);
  if (p->xinterval <= 0 || p->yinterval <= 0) {
    printf("x and y interval must be positive\n");
    halt();
  }

  if (p->xinterval >= maxinterval || p->yinterval >= maxinterval) {
    printf("x and y interval must be less than maxinterval (=%ld)\n",
	   (long)maxinterval);
    printf("You really can't read a graph with that many intervals!!\n");
    halt();
  }



  if (softnumbertest(V.f, &V)) {
    fscanf(V.f->f, "%ld", &p->xsubintervals);
    if (p->xsubintervals <= 0) {
      printf("xsubintervals must be positive\n");
      halt();
    }

    if (softnumbertest(V.f, &V)) {
      fscanf(V.f->f, "%ld", &p->ysubintervals);
      if (p->ysubintervals <= 0) {
	printf("ysubintervals must be positive\n");
	halt();
      }
      fscanf(V.f->f, "%*[^\n]");
      getc(V.f->f);
    } else {
      printf("You may now define the number of y subtic marks\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      bar(&TEMP, '-');
    }
  } else {
    printf("You may now define the number of x and y subtic marks\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    bar(&TEMP, '-');
  }

  if (p->xsubintervals >= maxinterval || p->ysubintervals >= maxinterval) {
    printf("x and y subintervals must be less than maxinterval (=%ld)\n",
	   (long)maxinterval);
    printf("You really can't read a graph with that many intervals!!\n");
    halt();
  }


  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->xwidth);
  num(V.f, &V);
  fscanf(V.f->f, "%ld%*[^\n]", &p->ywidth);
  getc(V.f->f);
  if (p->xwidth <= 0 || p->ywidth <= 0) {
    printf("x and y number widths must be positive\n");
    halt();
  }


  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->xdecimal);
  num(V.f, &V);
  fscanf(V.f->f, "%ld%*[^\n]", &p->ydecimal);
  getc(V.f->f);
  if (p->xdecimal < 0 || p->ydecimal < 0) {
    printf("x and y decimal places must be zero or positive\n");
    halt();
  }


  if (p->xdecimal > 0 && p->xwidth < p->xdecimal + 2) {
    printf("The width of numbers on the axis (xwidth) must be at least two larger\n");
    printf(
      "than the number of decimal places (xdecimal), to provide for the sign and\n");
    printf("decimal place.\n");
    halt();
  }


  if (p->ydecimal > 0 && p->ywidth < p->ydecimal + 2) {
    printf("The width of numbers on the axis (ywidth) must be at least two larger\n");
    printf(
      "than the number of decimal places (ydecimal), to provide for the sign and\n");
    printf("decimal place.\n");
    halt();
  }


  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%lg", &p->xsize);
  num(V.f, &V);
  fscanf(V.f->f, "%lg%*[^\n]", &p->ysize);
  getc(V.f->f);
  if (p->xsize < 0 || p->ysize <= 0) {
    printf("x and y size must be positive\n");
    halt();
  }


  getstring(V.f, &p->xlabel, &gotten);
  if (!gotten) {
    printf("cannot find xlabel\n");
    halt();
  }
  if (P_peek(V.f->f) == '^') {
    p->rotateylabel = false;
    getc(V.f->f);
  } else
    p->rotateylabel = true;
  getstring(V.f, &p->ylabel, &gotten);
  if (!gotten) {
    printf("cannot find ylabel\n");
    halt();
  }


  p->crosshairs = false;
  t_(&V);
  if (!P_eoln(V.f->f)) {
    if ((P_peek(V.f->f) == 'c') | (P_peek(V.f->f) == 'X') |
	(P_peek(V.f->f) == 'Y') | (P_peek(V.f->f) == 'N'))
      p->crosshairs = true;


    p->doXaxis = true;
    p->doYaxis = true;


    if ((P_peek(V.f->f) == 'x') | (P_peek(V.f->f) == 'X'))
      p->doYaxis = false;
    if ((P_peek(V.f->f) == 'y') | (P_peek(V.f->f) == 'Y'))
      p->doXaxis = false;
    if ((P_peek(V.f->f) == 'n') | (P_peek(V.f->f) == 'N')) {
      p->doXaxis = false;
      p->doYaxis = false;
    }
    if (P_peek(V.f->f) == 'i')
      p->doaxisline = false;
    else
      p->doaxisline = true;
  }
  fscanf(V.f->f, "%*[^\n]");


  getc(V.f->f);
  p->logxscale = false;
  p->logxnormal = false;
  t_(&V);
  if (!P_eoln(V.f->f)) {
    if ((P_peek(V.f->f) == 'l') | (P_peek(V.f->f) == 'L')) {
      p->logxscale = true;
      if (P_peek(V.f->f) == 'L')
	p->logxnormal = true;
      skipnonblanks(V.f);
      num(V.f, &V);
      fscanf(V.f->f, "%lg", &p->xbase);
      if (p->xbase <= 1.0) {
	printf("log x base must be > 1\n");
	halt();
      }
      p->logxbase = log(p->xbase);
    }
  }
  fscanf(V.f->f, "%*[^\n]");


  getc(V.f->f);
  p->logyscale = false;
  p->logynormal = false;
  t_(&V);
  if (!P_eoln(V.f->f)) {
    if ((P_peek(V.f->f) == 'l') | (P_peek(V.f->f) == 'L')) {
      p->logyscale = true;
      if (P_peek(V.f->f) == 'L')
	p->logynormal = true;
      skipnonblanks(V.f);
      num(V.f, &V);
      fscanf(V.f->f, "%lg", &p->ybase);
      if (p->ybase <= 1.0) {
	printf("log y base must be > 1\n");
	halt();
      }
      p->logybase = log(p->ybase);
    }
  }
  fscanf(V.f->f, "%*[^\n]");


  getc(V.f->f);
  nextsection('c', &V);



  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->xcolumn);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->ycolumn);
  fscanf(V.f->f, "%*[^\n]");

  getc(V.f->f);
  if (p->xcolumn <= 0 || p->ycolumn <= 0) {
    printf(
      " in xyplop, the defined locations of both x and y columns must be positive\n");
    halt();
  }

  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->scolumn);
  fscanf(V.f->f, "%*[^\n]");

  getc(V.f->f);
  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->xscolumn);

  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->yscolumn);
  fscanf(V.f->f, "%*[^\n]");


  getc(V.f->f);
  t_(&V);
  if (P_peek(V.f->f) == 'h' || P_peek(V.f->f) == '9' ||
      P_peek(V.f->f) == '8' || P_peek(V.f->f) == '7' ||
      P_peek(V.f->f) == '6' || P_peek(V.f->f) == '5' ||
      P_peek(V.f->f) == '4' || P_peek(V.f->f) == '3' ||
      P_peek(V.f->f) == '2' || P_peek(V.f->f) == '1' ||
      P_peek(V.f->f) == '0' || P_peek(V.f->f) == ' ')
    p->colorkind = 'h';
  else {
    if (P_peek(V.f->f) == 'r')
      p->colorkind = 'r';
    else {
      printf("colorkind must be: blank, digit, h or r\n");
      halt();
    }
  }
  t_(&V);
  if (P_peek(V.f->f) == 'r' || P_peek(V.f->f) == 'h')
    getc(V.f->f);
  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->hucolumn);
  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->sacolumn);
  t_(&V);
  num(V.f, &V);
  fscanf(V.f->f, "%ld", &p->brcolumn);
  fscanf(V.f->f, "%*[^\n]");

  getc(V.f->f);
  testcolumns(p->xcolumn, p->ycolumn, p->scolumn, p->xscolumn, p->yscolumn,
	      p->hucolumn, p->sacolumn, p->brcolumn, &V);

  p->needscolumn = (p->scolumn > 0);
  p->needxscolumn = (p->xscolumn > 0);
  p->needyscolumn = (p->yscolumn > 0);

  p->needcocolumn = (p->hucolumn > 0 || p->sacolumn > 0 || p->brcolumn > 0);

  /*

*/
  p->xrect = -LONG_MAX;
  p->yrect = -LONG_MAX;

  V.symbolcount = 1;

  nextsection('s', &V);


  t_(&V);
  fscanf(V.f->f, "%c%*[^\n]", &symb);
  getc(V.f->f);
  if (symb == '\n')
    symb = ' ';
  if (symb == '.') {
    printf("you must define at least one symbol\n");
    halt();
  }
  p->usersymbols = (symbols *)Malloc(sizeof(symbols));
  us = p->usersymbols;
  p->startrectangles = false;
  do {

    readsymbol(V.f, symb, p, &us->s);

    V.symbolcount++;

    nextsection('s', &V);

    t_(&V);
    fscanf(V.f->f, "%c%*[^\n]", &symb);
    getc(V.f->f);
    if (symb == '\n')
      symb = ' ';
    if (symb != '.') {
      us->next = (symbols *)Malloc(sizeof(symbols));
      us = us->next;
    } else
      us->next = NULL;
  } while (symb != '.');


  nextsection('l', &V);



  p->userlines = NULL;
  if (!BUFEOF(V.f->f)) {
    if (P_peek(V.f->f) == ' ')
      moreparameters = true;
    else {
      p->userlines = (lines *)Malloc(sizeof(lines));
      ul = p->userlines;

      while (!BUFEOF(V.f->f) && !moreparameters) {
	WITH1 = &ul->l;
	if (P_eoln(V.f->f)) {
	  fscanf(V.f->f, "%*[^\n]");
	  getc(V.f->f);
	  continue;
	}
	if (P_peek(V.f->f) == '*') {
	  fscanf(V.f->f, "%*[^\n]");
	  getc(V.f->f);
	  continue;
	}
	if (P_peek(V.f->f) == ' ') {
	  moreparameters = true;
	  break;
/* p2c: xyplo.p: Note: Deleting unreachable code [255] */
	}
	t_(&V);
	WITH1->linetype = getc(V.f->f);
	if (WITH1->linetype == '\n')
	  WITH1->linetype = ' ';
	if (WITH1->linetype != 'n' && WITH1->linetype != 'i' &&
	    WITH1->linetype != '-' &&
	    WITH1->linetype != '.' && WITH1->linetype != 'l') {
	  printf("user defined line type must be one of \"lin.-\"\n");
	  printf("you had \"%c\"\n", WITH1->linetype);
	  halt();
	}
	t_(&V);
	num(V.f, &V);
	fscanf(V.f->f, "%lg", &WITH1->m);
	t_(&V);
	num(V.f, &V);
	fscanf(V.f->f, "%lg", &WITH1->b);
	if (WITH1->linetype == '-' || WITH1->linetype == '.') {
	  t_(&V);
	  num(V.f, &V);
	  fscanf(V.f->f, "%lg", &WITH1->linesize);
	} else
	  WITH1->linesize = 0.0;
	fscanf(V.f->f, "%*[^\n]");

	getc(V.f->f);
	if ((!BUFEOF(V.f->f)) & (P_peek(V.f->f) != ' ')) {
	  if (!P_eoln(V.f->f)) {
	    if (P_peek(V.f->f) != '*') {
	      ul->next = (lines *)Malloc(sizeof(lines));
	      ul = ul->next;
	    }
	  }
	} else
	  ul->next = NULL;
      }
    }
  }






  p->llx = defaultllx;
  p->ury = defaultury;
  p->urx = defaulturx;
  p->lly = defaultlly;
  p->edgecontrol = 'p';
  p->edgeleft = 1.5;
  p->edgeright = 0.5;
  p->edgelow = 1.5;
  p->edgehigh = 1.0;
  p->parameterversion = 0.0;

  if (moreparameters) {
    nextsection('m', &V);

    p->edgecontrol = getc(V.f->f);
    if (p->edgecontrol == '\n')
      p->edgecontrol = ' ';
    if (p->edgecontrol == 'p') {
      cn(&V);
      fscanf(V.f->f, "%lg", &p->edgeleft);
      cn(&V);
      fscanf(V.f->f, "%lg", &p->edgeright);
      cn(&V);
      fscanf(V.f->f, "%lg", &p->edgelow);
      cn(&V);
      fscanf(V.f->f, "%lg", &p->edgehigh);

      /*



*/

      p->llx = (long)floor((p->xzero - p->edgeleft) * defscale + 0.5);
      p->lly = (long)floor((p->yzero - p->edgelow) * defscale + 0.5);
      p->urx = (long)floor((p->xzero + p->xsize + p->edgeright) * defscale + 0.5);
      p->ury = (long)floor((p->yzero + p->ysize + p->edgehigh) * defscale + 0.5);
    }
    fscanf(V.f->f, "%*[^\n]");

    getc(V.f->f);
    cn(&V);
    fscanf(V.f->f, "%lg%*[^\n]", &p->parameterversion);
    getc(V.f->f);
    return;
  }

  printf("*******************************************************\n");
  printf("* more parameters are now available, see              *\n");
  printf("* https://alum.mit.edu/www/toms/delila/xyplo.html *\n");
  printf("*******************************************************\n");
  printf(" They are being added to the parameter file ***********\n");

  upgrade1(&xyplop, *p);

}



Static Void loglabel(f, s, base, justification)
_TEXT *f;
stringDelila s;
double base;
Char justification;
{
  long i;
  stringDelila n;

  /*



*/
  clearstring(&n);
  n.letters[0] = 'l';
  n.letters[1] = 'o';
  n.letters[2] = 'g';
  n.length = 3;
  if (base < 10)
    stringinteger((long)base, &n, 1L, false);
  else
    stringinteger((long)base, &n, 2L, false);
  n.length++;
  n.letters[n.length - 1] = '{';
  for (i = 0; i < s.length; i++)
    n.letters[i + n.length] = s.letters[i];
  n.length += s.length + 1;
  n.letters[n.length - 1] = '}';
  graphstring(f, &n, justification);
}



Static Void logstring(f, s, base)
_TEXT *f;
stringDelila s;
double base;
{
  fprintf(f->f, "log%ld(", (long)base);
  writestring(f, &s);
  putc(')', f->f);
}



Static Void comment(f)
_TEXT *f;
{
  fprintf(f->f, "%% ");
}



Static Void writeparam(f, p)
_TEXT *f;
xyploparam p;
{
  symbols *us;
  lines *ul;
  symbol *WITH;
  line *WITH1;

  comment(f);
  fprintf(f->f, "user specified parameters:\n");
  comment(f);
  bar(f, '*');
  comment(f);
  bar(f, '*');
  comment(f);
  if (p.setx)
    putc('x', f->f);
  else
    putc('z', f->f);
  if (p.logxscale && !p.logxnormal)
    fprintf(f->f, " %*.*f  %*.*f minimum and maximum for x axis (log scale)",
	    (int)p.xwidth, (int)(p.xdecimal + 1), exp(p.logxbase * p.xmin),
	    (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	    exp(p.logxbase * p.xmax));
  else {
    fprintf(f->f, " %*.*f  %*.*f minimum and maximum for x axis",
	    (int)p.xwidth, (int)(p.xdecimal + 1), p.xmin,
	    (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	    p.xmax);
    if (p.logxnormal)
      fprintf(f->f, " (log scale)");
  }
  if (p.setx)
    fprintf(f->f, " set by user\n");
  else
    fprintf(f->f, " determined from data\n");

  comment(f);
  if (p.sety)
    putc('y', f->f);
  else
    putc('z', f->f);
  if (p.logyscale && !p.logynormal)
    fprintf(f->f, " %*.*f  %*.*f minimum and maximum for y axis (log scale) ",
	    (int)p.ywidth, (int)(p.ydecimal + 1), exp(p.logybase * p.ymin),
	    (int)(p.ywidth + dsafety), (int)(p.ydecimal + dsafety + 1),
	    exp(p.logybase * p.ymax));
  else {
    fprintf(f->f, " %*.*f  %*.*f minimum and maximum for y axis ",
	    (int)p.ywidth, (int)(p.ydecimal + 1), p.ymin,
	    (int)(p.ywidth + dsafety), (int)(p.ydecimal + dsafety + 1),
	    p.ymax);
    if (p.logynormal)
      fprintf(f->f, "(log scale)");
  }
  if (p.sety)
    fprintf(f->f, " set by user\n");
  else
    fprintf(f->f, " determined from data\n");

  comment(f);
  fprintf(f->f, "%10ld%10ld number of intervals on x and y to plot\n",
	  p.xinterval, p.yinterval);

  comment(f);
  fprintf(f->f, "%10ld%10ld width of numbers on graph in characters\n",
	  p.xwidth, p.ywidth);

  comment(f);
  fprintf(f->f, "%10ld%10ld number of decimal places for numbers\n",
	  p.xdecimal, p.ydecimal);

  comment(f);
  fprintf(f->f, "%10.3f%10.3f size of axes in cm\n", p.xsize, p.ysize);

  comment(f);
  writestring(f, &p.xlabel);
  fprintf(f->f, "    the x axis label\n");

  comment(f);
  writestring(f, &p.ylabel);
  fprintf(f->f, "    the y axis label\n");

  comment(f);
  if (!p.crosshairs)
    fprintf(f->f, "no ");
  fprintf(f->f, "cross hairs put on zero of x and y\n");

  comment(f);
  if (p.logxscale) {
    if (p.logxnormal)
      putc('L', f->f);
    else
      putc('l', f->f);
    fprintf(f->f, " %5.3f log scale on x axis", p.xbase);
    if (p.logxnormal)
      fprintf(f->f, " (normal numbers on scale)\n");
    else
      fprintf(f->f, " (log of numbers on scale)\n");
  } else
    fprintf(f->f, "no log scale on x axis\n");

  comment(f);
  if (p.logyscale) {
    if (p.logynormal)
      putc('L', f->f);
    else
      putc('l', f->f);
    fprintf(f->f, " %5.3f log scale on y axis", p.ybase);
    if (p.logynormal)
      fprintf(f->f, " (normal numbers on scale)\n");
    else
      fprintf(f->f, " (log of numbers on scale)\n");
  } else
    fprintf(f->f, "no log scale on y axis\n");

  /*



*/

  comment(f);
  fprintf(f->f, "* column choices: ");
  bar(f, '*');
  comment(f);
  fprintf(f->f, "%5ld%5ld columns of input chosen for x and y respectively\n",
	  p.xcolumn, p.ycolumn);
  comment(f);
  fprintf(f->f, "%5ld%5c column that determines symbols\n", p.scolumn, ' ');
  comment(f);
  fprintf(f->f, "%5ld%5ld columns that determine symbol size\n",
	  p.xscolumn, p.yscolumn);

  comment(f);
  fprintf(f->f, "%5ld%5ld%5ld columns that determine color\n",
	  p.hucolumn, p.sacolumn, p.brcolumn);

  comment(f);
  fprintf(f->f, "* user defined symbols: ");
  bar(f, '*');

  us = p.usersymbols;
  while (us != NULL) {
    WITH = &us->s;
    WITH1 = &us->s.l;
    comment(f);
    fprintf(f->f,
      "%c%12c symbol-to-plot:c(circle)bd(dotted box)x+Ifgpr(rectangle)\n",
      WITH->symboltoplot, ' ');
    comment(f);
    writestring(f, &WITH->symbolflag);
    fprintf(f->f, "%12c character string in xyin to indicate this symbol\n",
	    ' ');
    comment(f);
    fprintf(f->f, "%1.3f%8c symbol x size in cm\n", WITH->symbolxsize, ' ');
    comment(f);
    fprintf(f->f, "%1.3f%8c symbol y size in cm\n", WITH->symbolysize, ' ');

    comment(f);
    if (WITH->doconnection)
      fprintf(f->f, "connected %c %5.3f the symbols are connected by lines\n",
	      WITH->connecttype, WITH->connectsize);
    else
      fprintf(f->f, "no connected lines\n");

    comment(f);
    fprintf(f->f, " %c", WITH1->linetype);
    if (WITH1->linetype == '-' || WITH1->linetype == '.')
      fprintf(f->f, " %5.3f", WITH1->linesize);
    fprintf(f->f, " l=line .=dotted -=dashed i=invisible n=no line");
    if (WITH1->linetype == '-' || WITH1->linetype == '.')
      fprintf(f->f, "; spacing in cm");
    putc('\n', f->f);
    comment(f);
    bar(f, '*');
    us = us->next;
  }

  comment(f);
  fprintf(f->f, ".\n");
  comment(f);
  fprintf(f->f, "* user defined lines: ");
  bar(f, '*');

  ul = p.userlines;
  while (ul != NULL) {
    WITH1 = &ul->l;
    comment(f);
    fprintf(f->f, " %c", WITH1->linetype);
    fprintf(f->f, "  %*.*f",
	    (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	    WITH1->m);
    fprintf(f->f, "  %*.*f",
	    (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	    WITH1->b);
    fprintf(f->f, "  %1.3f", WITH1->linesize);
    fprintf(f->f, "   user defined line: linetype, m, b, linesize\n");
    ul = ul->next;
  }
  comment(f);
  if (p.userlines == NULL)
    fprintf(f->f, " (none)\n");
  comment(f);
  bar(f, '*');
  comment(f);
  bar(f, '*');

  comment(f);
  fprintf(f->f, " %c", p.edgecontrol);
  fprintf(f->f, " %*.*f", pwid, pdec, p.edgeleft);
  fprintf(f->f, " %*.*f", pwid, pdec, p.edgeright);
  fprintf(f->f, " %*.*f", pwid, pdec, p.edgelow);
  fprintf(f->f, " %*.*f", pwid, pdec, p.edgehigh);
  fprintf(f->f,
    " edgecontrol (p=page), edgeleft, edgeright, edgelow, edgehigh in cm\n");

  comment(f);
  fprintf(f->f, " %4.2f version of xyplop\n", p.parameterversion);

  comment(f);
  putc('\n', f->f);

}


Local double inverseFisher(z)
double z;
{
  /*

*/
  return ((exp(2 * z) - 1) / (exp(2 * z) + 1));
}



Static Void regressiondata(f, p)
_TEXT *f;
xyploparam p;
{
  symbols *us;
  double z;
  symbol *WITH;
  line *WITH1;

  /*
*/


  us = p.usersymbols;
  while (us != NULL) {
    WITH = &us->s;
    if (WITH->doline) {
      WITH1 = &WITH->l;

      comment(f);
      putc('\n', f->f);
      comment(f);
      fprintf(f->f, "Data on the regression line for symbol \"%c\" ",
	      WITH->symboltoplot);
      fprintf(f->f, "with flag \"");
      writestring(f, &WITH->symbolflag);
      fprintf(f->f, "\":\n");
      comment(f);
      fprintf(f->f, "%ld data points\n", WITH1->n);
      comment(f);
      fprintf(f->f, "mean     x = %*.*f and y = %*.*f\n",
	      (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	      WITH1->ex, (int)(p.xwidth + dsafety),
	      (int)(p.xdecimal + dsafety + 1), WITH1->ey);
      comment(f);
      fprintf(f->f, "variance x = %*.*f and y = %*.*f\n",
	      (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	      WITH1->varx, (int)(p.xwidth + dsafety),
	      (int)(p.xdecimal + dsafety + 1), WITH1->vary);
      comment(f);
      fprintf(f->f, "  r = %*.*f\n",
	      (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	      WITH1->r);
      comment(f);
      fprintf(f->f, "r^2 = %*.*f\n",
	      (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	      WITH1->r * WITH1->r);

      comment(f);
      fprintf(f->f, "Fisher's z' = ");
      if (WITH1->r > -1.0 && WITH1->r < 1.0) {
	z = 0.5 * (log(1 + WITH1->r) - log(1 - WITH1->r));
	fprintf(f->f, "%*.*f\n",
		(int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
		z);
	comment(f);
	if (WITH1->n > 3) {
	  fprintf(f->f, "95%% confidence limits on correlation r:");
	  fprintf(f->f, " %*.*f",
		  (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
		  inverseFisher(z - 1.96 / sqrt(WITH1->n - 3.0)));
	  fprintf(f->f, " to %*.*f",
		  (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
		  inverseFisher(z + 1.96 / sqrt(WITH1->n - 3.0)));
	} else
	  fprintf(f->f, "(no confidence limits, n < 3)");
	putc('\n', f->f);
      } else
	fprintf(f->f, " (undefined)\n");
      comment(f);
      fprintf(f->f, "covariance = %*.*f\n",
	      (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	      WITH1->covxy);
      comment(f);
      fprintf(f->f, "regression: ");
      if (p.logyscale)
	logstring(f, p.ylabel, p.ybase);
      else
	writestring(f, &p.ylabel);
      fprintf(f->f, " = %*.*f * ",
	      (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	      WITH1->m);
      if (p.logxscale)
	logstring(f, p.xlabel, p.xbase);

      else
	writestring(f, &p.xlabel);
      fprintf(f->f, " + %*.*f\n",
	      (int)(p.xwidth + dsafety), (int)(p.xdecimal + dsafety + 1),
	      WITH1->b);

      /*












*/


    }
    us = us->next;
  }
  /*


*/
}



Static Void skipcopy(infile, outfile, copy, linenumber)
_TEXT *infile, *outfile;
boolean copy;
long *linenumber;
{
  /*

*/
  boolean copying = true;

  *linenumber = 0;
  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else
    rewind(infile->f);
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);
  if (BUFEOF(infile->f)) {
    printf("skipcopy: xyin file is empty\n");
    halt();
  }
  while (copying) {
    if (BUFEOF(infile->f)) {
      copying = false;
      break;
    }
    if (P_eoln(infile->f)) {
      copying = false;
      break;
/* p2c: xyplo.p: Note: Deleting unreachable code [255] */
    }
    if (!((P_peek(infile->f) == '*') | (P_peek(infile->f) == '#'))) {
      copying = false;
      break;
    }
    if (copy) {
      fprintf(outfile->f, "%% ");
      copyaline(infile, outfile);
    } else {
      fscanf(infile->f, "%*[^\n]");
      getc(infile->f);
    }
    (*linenumber)++;
  }

}


/* Local variables for grabdata: */
struct LOC_grabdata {
  _TEXT *infile;
  xyploparam p;
  double *x, *y, *xs, *ys, *hue, *saturation, *brightness;
  Char *flag;
  stringDelila *flagstring;
  long *linenumber, column;
  boolean droppoint;
  /*
*/
  boolean gotx, goty, gotxs, gotys, gots, gothu, gotsa, gotbr, satisfied;
  /*
*/
  double xrawdata, yrawdata;
} ;

Local Void die(LINK)
struct LOC_grabdata *LINK;
{
  printf("at line %ld of data (INCLUDING * lines)\n", *LINK->linenumber);
  halt();
}

Local Void grab(LINK)
struct LOC_grabdata *LINK;
{
  long c;
  stringDelila *WITH;
  long FORLIM;


  LINK->droppoint = false;
  *LINK->hue = 0.0;
  *LINK->saturation = 0.0;
  *LINK->brightness = 0.0;
  LINK->column = 0;
  LINK->satisfied = false;
  LINK->gots = !LINK->p.needscolumn;
  LINK->gotx = false;
  LINK->goty = false;
  LINK->gotxs = !LINK->p.needxscolumn;
  LINK->gotys = !LINK->p.needyscolumn;
  LINK->gothu = !LINK->p.needcocolumn;
  LINK->gotsa = !LINK->p.needcocolumn;
  LINK->gotbr = !LINK->p.needcocolumn;
  *LINK->flag = ' ';


  clearstring(LINK->flagstring);


  while (!LINK->satisfied) {
    LINK->column++;
    skipblanks(LINK->infile);
    if (P_eoln(LINK->infile->f)) {
      printf("found end of line before all data columns were found\n");
      if (!LINK->gots)
	printf("missing symbol flag column\n");
      if (!LINK->gotx)
	printf("missing x data column\n");
      if (!LINK->goty)
	printf("missing y data column\n");
      if (!LINK->gotxs)
	printf("missing x symbol size column\n");
      if (!LINK->gotys)
	printf("missing y symbol size column\n");
      if (!LINK->gothu)
	printf("missing hue column\n");
      if (!LINK->gotsa)
	printf("missing saturation column\n");
      if (!LINK->gotbr)
	printf("missing brightness column\n");
      die(LINK);
    }



    if (LINK->column == LINK->p.xcolumn) {
      fscanf(LINK->infile->f, "%lg", LINK->x);
      if (LINK->p.logxscale) {
	if (*LINK->x <= 0.0) {
	  tellwarning();
	  fprintf(warnings.f,
		  "WARNING: all x values must be positive for logxscale\n");
	  fprintf(warnings.f, "The value you have is %10.5f\n", *LINK->x);
	  fprintf(warnings.f, "IT WAS DROPPED FROM THE DATA SET\n");
	  LINK->droppoint = true;
	} else {
	  LINK->xrawdata = *LINK->x;
	  *LINK->x = log(*LINK->x) / LINK->p.logxbase;
	  LINK->droppoint = false;
	}
      }
      LINK->gotx = true;
    } else if (LINK->column == LINK->p.ycolumn) {
      fscanf(LINK->infile->f, "%lg", LINK->y);
      if (LINK->p.logyscale) {
	if (*LINK->y <= 0.0) {
	  tellwarning();
	  fprintf(warnings.f,
		  "WARNING: all y values must be positive for logyscale\n");
	  fprintf(warnings.f, "At x = %10.5f", *LINK->x);
	  fprintf(warnings.f, " the value you have is y = %10.5f\n", *LINK->y);
	  fprintf(warnings.f, "IT WAS DROPPED FROM THE DATA SET\n");
	  LINK->droppoint = true;
	}

	else {
	  LINK->yrawdata = *LINK->y;
	  *LINK->y = log(*LINK->y) / LINK->p.logybase;
	  LINK->droppoint = false;
	}
      }
      LINK->goty = true;
    } else if (LINK->column == LINK->p.xscolumn) {
      if (LINK->p.needxscolumn) {
	fscanf(LINK->infile->f, "%lg", LINK->xs);
	if (LINK->p.logxscale) {
	  if (*LINK->xs <= 0.0) {
	    printf("all x values for symbol size must be positive for logxscale\n");
	    printf("The value you have is %10.5f\n", *LINK->xs);
	    die(LINK);
	  }
	  /*

*/
	}
	LINK->gotxs = true;
      } else
	skipnonblanks(LINK->infile);
    } else if (LINK->column == LINK->p.yscolumn) {
      if (LINK->p.needyscolumn) {
	fscanf(LINK->infile->f, "%lg", LINK->ys);
	if (LINK->p.logyscale) {
	  if (*LINK->ys <= 0.0) {
	    printf("all y values for symbol size must be positive for logyscale\n");
	    printf("The value you have is %10.5f\n", *LINK->ys);
	    die(LINK);
	  }

	  /*











*/
	}
	LINK->gotys = true;
      } else
	skipnonblanks(LINK->infile);
    }


    else if (LINK->column == LINK->p.hucolumn) {
      if (LINK->p.needcocolumn) {
	fscanf(LINK->infile->f, "%lg", LINK->hue);
	LINK->gothu = true;
      } else {
	*LINK->hue = 0.0;
	skipnonblanks(LINK->infile);
      }
    } else if (LINK->column == LINK->p.sacolumn) {
      if (LINK->p.needcocolumn) {
	fscanf(LINK->infile->f, "%lg", LINK->saturation);
	LINK->gotsa = true;
      } else {
	*LINK->saturation = 0.0;
	skipnonblanks(LINK->infile);
      }
    } else if (LINK->column == LINK->p.brcolumn) {
      if (LINK->p.needcocolumn) {
	fscanf(LINK->infile->f, "%lg", LINK->brightness);
	LINK->gotbr = true;
      } else {
	*LINK->brightness = 0.0;
	skipnonblanks(LINK->infile);
      }
    }

    else if (LINK->column == LINK->p.scolumn) {
      if (LINK->p.needscolumn) {
	/*



*/
	gettoken(LINK->infile, LINK->flag, LINK->flagstring, &LINK->gots);

	/*



*/
	/*
*/
	WITH = LINK->flagstring;
	FORLIM = WITH->length;
	for (c = 0; c < FORLIM; c++) {
	  if (WITH->letters[c] == '_')
	    WITH->letters[c] = ' ';
	}
      } else
	skipnonblanks(LINK->infile);
    } else
      skipnonblanks(LINK->infile);

    LINK->satisfied = (LINK->gotx && LINK->goty && LINK->gotxs &&
		       LINK->gotys && LINK->gots && LINK->gothu &&
		       LINK->gotsa && LINK->gotbr);
  }

  if (LINK->p.needxscolumn && LINK->p.logxscale)
    *LINK->xs /= LINK->xrawdata;
  if (LINK->p.needyscolumn && LINK->p.logyscale)
    *LINK->ys /= LINK->yrawdata;

  fscanf(LINK->infile->f, "%*[^\n]");


  getc(LINK->infile->f);
}



Static Void grabdata(infile_, p_, x_, y_, xs_, ys_, hue_, saturation_,
		     brightness_, flag_, flagstring_, linenumber_, gotdata)
_TEXT *infile_;
xyploparam p_;
double *x_, *y_, *xs_, *ys_, *hue_, *saturation_, *brightness_;
Char *flag_;
stringDelila *flagstring_;
long *linenumber_;
boolean *gotdata;
{
  /*






*/
  struct LOC_grabdata V;
  boolean done = false;

  V.infile = infile_;
  V.p = p_;
  V.x = x_;
  V.y = y_;
  V.xs = xs_;
  V.ys = ys_;
  V.hue = hue_;
  V.saturation = saturation_;
  V.brightness = brightness_;
  V.flag = flag_;
  V.flagstring = flagstring_;
  V.linenumber = linenumber_;
  *gotdata = false;
  while (!done) {
    if (BUFEOF(V.infile->f)) {
      done = true;
      break;
    }
    if (P_eoln(V.infile->f)) {
      fscanf(V.infile->f, "%*[^\n]");
      getc(V.infile->f);
      (*V.linenumber)++;
      continue;
    }
    if ((P_peek(V.infile->f) == '*') | (P_peek(V.infile->f) == '#')) {
      fscanf(V.infile->f, "%*[^\n]");
      getc(V.infile->f);
      (*V.linenumber)++;
    } else {
      grab(&V);
      *gotdata = true;
      done = true;
    }
  }
  if (V.droppoint)
    *gotdata = false;
  /*



*/
}



Static Void bounds(infile, outfile, p)
_TEXT *infile, *outfile;
xyploparam *p;
{
  /*



*/
  boolean done;

  long linenumber;
  symbols *us;

  boolean noregressions;
  /*

*/


  double x, y, xs, ys, hu, sa, br;
  Char flag;
  stringDelila flagstring;
  boolean gotdata;
  symbol *WITH;
  line *WITH1;
  _TEXT TEMP;

  skipcopy(infile, outfile, true, &linenumber);


  if (p->setx) {
    if (p->logxscale && !p->logxnormal) {
      if (p->xmin <= 0 || p->xmax <= 0) {
	printf("xmin and xmax must be positive for log scale\n");
	halt();
      }
      p->xmin = log(p->xmin) / p->logxbase;
      p->xmax = log(p->xmax) / p->logxbase;
    }
  }

  if (p->sety) {
    if (p->logyscale && !p->logynormal) {
      if (p->ymin <= 0 || p->ymax <= 0) {
	printf("ymin and ymax must be positive for log scale\n");
	halt();
      }
      p->ymin = log(p->ymin) / p->logybase;
      p->ymax = log(p->ymax) / p->logybase;
    }
  }
  /*


*/
  if (!p->needscolumn && p->usersymbols->s.doline)
    printf("regressing on first symbol only\n");


  us = p->usersymbols;
  while (us != NULL) {
    WITH = &us->s;
    if (WITH->doline) {
      WITH1 = &WITH->l;
      regress('c', x, y, &WITH1->sumx, &WITH1->sumy, &WITH1->sumxsqd,
	      &WITH1->sumysqd, &WITH1->sumxy, &WITH1->ex, &WITH1->ey,
	      &WITH1->varx, &WITH1->vary, &WITH1->covxy, &WITH1->r, &WITH1->m,
	      &WITH1->b, &WITH1->n);
    }
    us = us->next;
  }


  while (!BUFEOF(infile->f)) {
    grabdata(infile, *p, &x, &y, &xs, &ys, &hu, &sa, &br, &flag, &flagstring,
	     &linenumber, &gotdata);
    if (!gotdata)
      continue;


    if (!p->setx) {
      if (x < p->xmin)
	p->xmin = x;
      if (x > p->xmax)
	p->xmax = x;
    }
    if (!p->sety) {
      if (y < p->ymin)
	p->ymin = y;
      if (y > p->ymax)
	p->ymax = y;
    }

    if (!p->needscolumn) {
      WITH1 = &p->usersymbols->s.l;
      regress('e', x, y, &WITH1->sumx, &WITH1->sumy, &WITH1->sumxsqd,
	      &WITH1->sumysqd, &WITH1->sumxy, &WITH1->ex, &WITH1->ey,
	      &WITH1->varx, &WITH1->vary, &WITH1->covxy, &WITH1->r, &WITH1->m,
	      &WITH1->b, &WITH1->n);
      continue;
    }

    us = p->usersymbols;

    while (us != NULL) {
      noregressions = true;
      WITH = &us->s;
      if (WITH->doline) {
	if (equalstring(flagstring, WITH->symbolflag)) {
	  WITH1 = &WITH->l;
	  regress('e', x, y, &WITH1->sumx, &WITH1->sumy, &WITH1->sumxsqd,
		  &WITH1->sumysqd, &WITH1->sumxy, &WITH1->ex, &WITH1->ey,
		  &WITH1->varx, &WITH1->vary, &WITH1->covxy, &WITH1->r,
		  &WITH1->m, &WITH1->b, &WITH1->n);
	  noregressions = false;
	}
      }
      us = us->next;
    }
    if (!noregressions)
      continue;

    us = p->usersymbols;
    done = (us == NULL);
    while (!done) {
      done = (us->s.symboltoplot == 'g' || us->s.symboltoplot == 'G');
      if (done)
	break;
      us = us->next;
      done = (us == NULL);
    }
    if (us != NULL) {
      WITH = &us->s;
      if (WITH->doline) {
	WITH1 = &WITH->l;
	regress('e', x, y, &WITH1->sumx, &WITH1->sumy, &WITH1->sumxsqd,
		&WITH1->sumysqd, &WITH1->sumxy, &WITH1->ex, &WITH1->ey,
		&WITH1->varx, &WITH1->vary, &WITH1->covxy, &WITH1->r,
		&WITH1->m, &WITH1->b, &WITH1->n);
      }
    }
  }


  us = p->usersymbols;
  while (us != NULL) {
    WITH = &us->s;
    if (WITH->doline) {
      WITH1 = &WITH->l;
      if (WITH1->n == 0) {
	printf("no data found for symbol %c", WITH->symboltoplot);

	printf(" (which has flag ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &WITH->symbolflag);
	printf("):");

	printf(" no regression done\n");
	WITH->doline = false;
      } else if (WITH1->n == 1) {
	printf("only one data point found for symbol %c", WITH->symboltoplot);

	printf(" (which has flag ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &WITH->symbolflag);
	printf("):");

	printf(" no regression done\n");
	WITH->doline = false;
      } else
	regress('r', x, y, &WITH1->sumx, &WITH1->sumy, &WITH1->sumxsqd,
		&WITH1->sumysqd, &WITH1->sumxy, &WITH1->ex, &WITH1->ey,
		&WITH1->varx, &WITH1->vary, &WITH1->covxy, &WITH1->r,
		&WITH1->m, &WITH1->b, &WITH1->n);
    }
    us = us->next;
  }


  if (p->xmax == p->xmin) {
    printf(" no variation to x range of graph\n");
    p->xscale = 1.0;
  } else
    p->xscale = p->xsize / (p->xmax - p->xmin);
  if (p->ymax == p->ymin) {
    printf(" no variation to y range of graph\n");
    p->yscale = 1.0;
  } else
    p->yscale = p->ysize / (p->ymax - p->ymin);
}



Static double locatex(p, x)
xyploparam p;
double x;
{
  return ((x - p.xmin) * p.xscale);
}


Static double locatey(p, y)
xyploparam p;
double y;
{
  return ((y - p.ymin) * p.yscale);
}



Static Void crosshairs(outfile, p)
_TEXT *outfile;
xyploparam p;
{
  if (!p.crosshairs)
    return;

  if (p.xmin < 0.0 && p.xmax > 0.0) {
    movea(outfile, locatex(p, 0.0), locatey(p, p.ymin));
    yaxis(outfile, p.doaxisline, locatey(p, p.ymax) - locatey(p, p.ymin),
	  p.ymin, (p.ymax - p.ymin) / p.yinterval, p.ymax,
	  (double)p.ysubintervals, -0.05, -0.2, 0.0, 0L, 0L, p.logynormal,
	  p.logyscale, p.logybase);
    yaxis(outfile, p.doaxisline, p.ysize, p.ymin,
	  (p.ymax - p.ymin) / p.yinterval, p.ymax, (double)p.ysubintervals,
	  0.05, -0.2, 0.0, 0L, 0L, p.logynormal, p.logyscale, p.logybase);
  }

  if (p.ymin >= 0.0 || p.ymax <= 0.0)
    return;
  movea(outfile, locatex(p, p.xmin), locatey(p, 0.0));
  xaxis(outfile, p.doaxisline, locatex(p, p.xmax) - locatex(p, p.xmin),
	p.xmin, (p.xmax - p.xmin) / p.xinterval, p.xmax,
	(double)p.xsubintervals, -0.05, 0.0, -0.12, 0L, 0L, p.logxnormal,
	p.logxscale, p.logxbase);
  xaxis(outfile, p.doaxisline, p.xsize, p.xmin,
	(p.xmax - p.xmin) / p.xinterval, p.xmax, (double)p.xsubintervals,
	0.05, 0.0, -0.12, 0L, 0L, p.logxnormal, p.logxscale, p.logxbase);
}



Static Void makexlabel(outfile, p)
_TEXT *outfile;
xyploparam p;
{
  /*
*/
  movea(outfile, locatex(p, (p.xmax - p.xmin) / 2.0 + p.xmin),
	locatey(p, p.ymin) - labelbelowx -
	p.ticscale * p.Xticscale * Xticlength + p.Xlabelshift);
  if (!p.logxscale) {
    graphstring(outfile, &p.xlabel, 'c');
    return;
  }
  if (p.logxnormal) {
    graphstring(outfile, &p.xlabel, 'l');
    fprintf(outfile->f, "(, log scale) x\n");
  } else
    loglabel(outfile, p.xlabel, p.xbase, 'c');
}


#define ydebug          false



Static Void makeylabel(outfile, p)
_TEXT *outfile;
xyploparam p;
{
  /*
*/
  double shiftleft, number;
  long isminus, digits;
  _TEXT TEMP;

  if (ydebug)
    printf("********** debugging makeylabel\n");
  if (p.rotateylabel) {
    if (ydebug)
      printf("********** \"");
    if (ydebug) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &p.ylabel);
    }
    if (ydebug)
      printf("\" = %ld characters\n", p.ylabel.length);

    /*



*/
    shiftleft = 0.00;
    if (ydebug)
      printf("********** shiftleft = %1.1f\n", shiftleft);
    if (fabs(p.ymin) > fabs(p.ymax)) {
      number = fabs(p.ymin);
      if (ydebug)
	printf("********** ymin = %5.1f\n", p.ymin);
      if (p.ymin < 0)
	isminus = 1;
      else
	isminus = 0;
    } else {
      number = fabs(p.ymax);
      if (ydebug)
	printf("********** ymax = %5.1f\n", p.ymax);
      if (p.ymax < 0)
	isminus = 1;
      else
	isminus = 0;
    }
    if (ydebug)
      printf("********** number   = %5.1f\n", number);
    if (ydebug)
      printf("********** isminus  = %ld\n", isminus);

    if (ydebug)
      printf("********** ywidth   = %ld\n", p.ywidth);
    if (ydebug)
      printf("********** ydecimal = %ld\n", p.ydecimal);


    if (number > 9)
      digits = (long)(log(number + 0.1) / log(10.0)) + 1;
    else
      digits = 1;
    if (p.ydecimal > 0)
      digits += p.ydecimal + 1;
    if (ydebug)
      printf("********** digits = %ld\n", digits);


    shiftleft += p.ticscale * p.Yticscale * Yticlength;
    if (ydebug)
      printf("********** shiftleft = %1.1f\n", shiftleft);


    shiftleft += 0.3 * (digits + isminus + 1);
    if (ydebug)
      printf("********** shiftleft = %1.1f\n", shiftleft);

    /*



*/
    shiftleft += p.Ylabelshift;
    if (ydebug)
      printf("********** shiftleft = %1.1f\n", shiftleft);



    movea(outfile, locatex(p, p.xmin) - shiftleft,
	  locatey(p, (p.ymax - p.ymin) / 2.0 + p.ymin));
    /*





*/

    fprintf(outfile->f, "gsave\n");
    fprintf(outfile->f, "90 rotate\n");
    if (p.logyscale) {
      if (p.logynormal) {
	graphstring(outfile, &p.ylabel, 'c');
	fprintf(outfile->f, "(, log scale) x\n");
      } else
	loglabel(outfile, p.ylabel, p.ybase, 'c');
    } else
      graphstring(outfile, &p.ylabel, 'c');
    fprintf(outfile->f, "grestore\n");
    return;
  }


  movea(outfile, locatex(p, p.xmin), locatey(p, p.ymax) + labelabovey);
  if (!p.logyscale) {
    graphstring(outfile, &p.ylabel, 'l');

    return;
  }
  if (p.logynormal) {
    graphstring(outfile, &p.ylabel, 'l');
    fprintf(outfile->f, "(, log scale) x\n");
  } else
    loglabel(outfile, p.ylabel, p.ybase, 'l');
}

#undef ydebug



Static Void makeaxes(outfile, p)
_TEXT *outfile;
xyploparam p;
{
  movea(outfile, locatex(p, p.xmin), locatey(p, p.ymin));
  if (p.doXaxis)
    xaxis(outfile, p.doaxisline, locatex(p, p.xmax) - locatex(p, p.xmin),
	  p.xmin, (p.xmax - p.xmin) / p.xinterval, p.xmax,
	  (double)p.xsubintervals, p.ticscale * p.Xticscale * Xticlength,
	  XaxisXshift, XaxisYshift, p.xwidth, p.xdecimal, p.logxscale,
	  p.logxnormal, p.logxbase);


  if (p.doYaxis)
    yaxis(outfile, p.doaxisline, locatey(p, p.ymax) - locatey(p, p.ymin),
	  p.ymin, (p.ymax - p.ymin) / p.yinterval, p.ymax,
	  (double)p.ysubintervals, p.ticscale * p.Yticscale * Yticlength,
	  YaxisXshift, YaxisYshift, p.ywidth, p.ydecimal, p.logyscale,
	  p.logynormal, p.logybase);
}


#define maxbad          10


/* Local variables for plotdata: */
struct LOC_plotdata {
  _TEXT *outfile, *warnings;
  xyploparam p;
  symbols *us;


  double x, y, xs, ys, hu, sa, br;
  stringDelila flagstring;
  Char s;
} ;

Local Void findsymbol(LINK)
struct LOC_plotdata *LINK;
{
  boolean done = false;
  symbol *WITH;
  _TEXT TEMP;


  LINK->us = LINK->p.usersymbols;
  WITH = &LINK->us->s;
  if (!LINK->p.needscolumn)
    return;

  /*









*/

  /*
*/
  LINK->us = LINK->p.usersymbols;
  while (!equalstring(LINK->flagstring, LINK->us->s.symbolflag) &&
	 LINK->us->next != NULL)
    LINK->us = LINK->us->next;
  /*
*/


  if (!equalstring(LINK->flagstring, LINK->us->s.symbolflag))
    LINK->us = NULL;


  if (LINK->us != NULL)
    return;

  /*

*/
  LINK->us = LINK->p.usersymbols;

  while (!done) {
    /*


*/

    if (LINK->us == NULL) {
      done = true;
      break;
    }
    if (LINK->us->s.symboltoplot == 'g' || LINK->us->s.symboltoplot == 'G')
      done = true;
    else
      LINK->us = LINK->us->next;
  }


  if (LINK->us != NULL)
    return;
  printf("Could not find a symbol for the flagstring and there is no grabbag.\n");
  printf("The flagstring is: \"");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &LINK->flagstring);
  printf("\"\n");
  printf("The known symbols are: \n");
  LINK->us = LINK->p.usersymbols;
  while (LINK->us != NULL) {
    printf("  symbolflag=\"");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &LINK->us->s.symbolflag);
    putchar('"');

    printf(" | symboltoplot=\"%c\"", LINK->us->s.symboltoplot);
    printf("\"\n");
    LINK->us = LINK->us->next;
  }

  halt();
}

Local Void makesymbol(LINK)
struct LOC_plotdata *LINK;
{
  boolean docolor;
  symbol *WITH;
  _TEXT TEMP;

  LINK->s = LINK->us->s.symboltoplot;
  docolor = (LINK->s == 'C' || LINK->s == 'B' || LINK->s == 'M');

  WITH = &LINK->us->s;


  if (LINK->x >= LINK->p.xmin && LINK->x <= LINK->p.xmax &&
      LINK->y >= LINK->p.ymin && LINK->y <= LINK->p.ymax) {
    /*




*/
    if (WITH->doconnection) {
      /*

*/
      if (WITH->didlastpoint) {
	if (docolor)
	  setcolor(LINK->outfile, LINK->p.colorkind, LINK->hu, LINK->sa,
		   LINK->br);
	movea(LINK->outfile, locatex(LINK->p, WITH->oldx),
	      locatey(LINK->p, WITH->oldy));
	drawa(LINK->outfile, locatex(LINK->p, LINK->x),
	      locatey(LINK->p, LINK->y), WITH->connecttype,
	      WITH->connectsize);
	if (docolor)
	  setgray(LINK->outfile, 0.0);
      } else
	movea(LINK->outfile, locatex(LINK->p, LINK->x),
	      locatey(LINK->p, LINK->y));


      WITH->didlastpoint = true;
      WITH->oldx = LINK->x;
      WITH->oldy = LINK->y;
    } else
      movea(LINK->outfile, locatex(LINK->p, LINK->x),
	    locatey(LINK->p, LINK->y));
    /*

*/

    if (WITH->symbolxsize <= 0.0)
      LINK->xs = LINK->p.xscale * LINK->xs;
    else
      LINK->xs = WITH->symbolxsize;
    if (WITH->symbolysize <= 0.0)
      LINK->ys = LINK->p.yscale * LINK->ys;
    else
      LINK->ys = WITH->symbolysize;

    if (LINK->s == 'd' || LINK->s == 'b')
      fprintf(LINK->outfile->f, "2 setlinecap\n");


    if (LINK->s == 'b')
      cboxr(LINK->outfile, LINK->xs, LINK->ys);
    else if (LINK->s == 'B') {
      setcolor(LINK->outfile, LINK->p.colorkind, LINK->hu, LINK->sa, LINK->br);
      cboxr(LINK->outfile, LINK->xs, LINK->ys);
      setgray(LINK->outfile, 0.0);
    }

    else if (LINK->s == 'c') {

      circler(LINK->outfile, LINK->xs / 2);
    } else if (LINK->s == 'C') {
      setcolor(LINK->outfile, LINK->p.colorkind, LINK->hu, LINK->sa, LINK->br);
      circler(LINK->outfile, LINK->xs / 2);
      setgray(LINK->outfile, 0.0);
    }

    else if (LINK->s == 'd') {
      dotr(LINK->outfile);
      cboxr(LINK->outfile, LINK->xs, LINK->ys);
    } else if (LINK->s == 'p')
      dotr(LINK->outfile);
    else if (LINK->s == 'I')
      ibeam(LINK->outfile, LINK->xs, 2 * LINK->ys);
    else if (LINK->s == 'x')
      xr(LINK->outfile, LINK->xs, LINK->ys);
    else if (LINK->s == '+') {

      plusr(LINK->outfile, LINK->xs, LINK->ys);
    } else if (LINK->s == 'r') {
      setgray(LINK->outfile, LINK->br);
      rectsize(LINK->outfile, &LINK->p.xrect, &LINK->p.yrect, &LINK->xs,
	       &LINK->ys);
      rectdo(LINK->outfile);
      setgray(LINK->outfile, 0.0);
    }

    else if (LINK->s == 'R') {
      setcolor(LINK->outfile, LINK->p.colorkind, LINK->hu, LINK->sa, LINK->br);
      rectsize(LINK->outfile, &LINK->p.xrect, &LINK->p.yrect, &LINK->xs,
	       &LINK->ys);
      rectdo(LINK->outfile);
      setgray(LINK->outfile, 0.0);
    }

    /*

*/
    else if (LINK->s == 's') {

      skybox(LINK->outfile, locatex(LINK->p, LINK->x),
	     locatey(LINK->p, LINK->y), LINK->xs, LINK->ys);
    } else if (LINK->s == 'L') {
      setcolor(LINK->outfile, LINK->p.colorkind, LINK->hu, LINK->sa, LINK->br);

      fprintf(LINK->outfile->f, "%7.5f setlinewidth %% connectsize\n",
	      LINK->xs / (2.54 / 72));
    }

    else if (LINK->s == 'm' || LINK->s == 'M') {
      if (LINK->p.usermarks) {
	setcolor(LINK->outfile, LINK->p.colorkind, LINK->hu, LINK->sa,
		 LINK->br);
	if (!LINK->p.needscolumn) {
	  printf(
	    "user defined marks in xyplom require a (non-zero) symbol column to be defined in xyplop\n");
	  halt();
	}

	/*

*/
	if (WITH->symbolhasxyplomdef) {
	  marksdo(LINK->outfile, LINK->xs, LINK->ys, LINK->flagstring);
	  setgray(LINK->outfile, 0.0);
	}
      } else {
	if (!LINK->p.warnedaboutusermarks) {
	  printf("WARNING: NO USER MARKS DEFINED\n");
	  LINK->p.warnedaboutusermarks = true;
	}
      }
    }

    else if (LINK->s == 'f' || LINK->s == 'g' || LINK->s == 'F' ||
	     LINK->s == 'G') {
      if (LINK->s == 'f' || LINK->s == 'g')
	graphstring(LINK->outfile, &LINK->flagstring, 'c');
      else
	graphstring(LINK->outfile, &LINK->flagstring, 'l');
    } else if (!WITH->doconnection) {
      printf("unknown symbol \"%c\", with flag \"", LINK->s);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &LINK->flagstring);
      printf("\": no symbol plotted and points not connected\n");
    }

    if (LINK->s == 'd' || LINK->s == 'b')
      fprintf(LINK->outfile->f, "1 setlinecap\n");


    datacount++;
    return;
  }

  datacount++;
  tellwarning();
  fprintf(LINK->warnings->f, "skipping point # %ld (", datacount);

  if (LINK->p.xdecimal > 0)
    fprintf(LINK->warnings->f, "%*.*f",
	    (int)LINK->p.xwidth, (int)LINK->p.xdecimal, LINK->x);
  else
    fprintf(LINK->warnings->f, "%*ld",
	    (int)LINK->p.xwidth, (long)floor(LINK->x + 0.5));

  putc(',', LINK->warnings->f);

  if (LINK->p.ydecimal > 0)
    fprintf(LINK->warnings->f, "%*.*f",
	    (int)LINK->p.ywidth, (int)LINK->p.ydecimal, LINK->y);
  else
    fprintf(LINK->warnings->f, "%*ld",
	    (int)LINK->p.ywidth, (long)floor(LINK->y + 0.5));

  fprintf(LINK->warnings->f, ") because it is out of the plotting window\n");

  WITH->didlastpoint = false;
}



Static Void plotdata(infile, outfile_, warnings_, p_)
_TEXT *infile, *outfile_, *warnings_;
xyploparam p_;
{
  /*
*/
  struct LOC_plotdata V;
  long b;
  Char badlist[maxbad];
  /*
*/
  long badnumber = 0;
  boolean foundbad;
  long linenumber = 0;
  Char flag;
  boolean gotdata;

  V.outfile = outfile_;
  V.warnings = warnings_;
  V.p = p_;
  while (!BUFEOF(infile->f)) {
    grabdata(infile, V.p, &V.x, &V.y, &V.xs, &V.ys, &V.hu, &V.sa, &V.br,
	     &flag, &V.flagstring, &linenumber, &gotdata);
    if (!gotdata)
      continue;

    /*




*/

    findsymbol(&V);
    if (V.us != NULL) {
      makesymbol(&V);
      continue;
    }

    foundbad = false;
    b = 1;
    while (b <= badnumber && !foundbad) {
      if (badlist[b-1] == flag)
	foundbad = true;
      else
	b++;
    }

    if (foundbad)
      continue;
    if (badnumber < maxbad)
      badnumber++;
    badlist[badnumber-1] = flag;
    printf("flag \"%c\" has no symbol", flag);
    printf(" and there is no grab-bag:");
    printf(" this datum will not be plotted\n");
  }

  if (V.p.fontcontrol != 'n')
    fprintf(V.outfile->f, "1 setlinewidth %% in makesymbol\n");
}

#undef maxbad



Static Void plotuserlines(outfile, p)
_TEXT *outfile;
xyploparam p;
{
  lines *ul;
  boolean intercept;
  /*
*/
  double x1, y1, x2, y2;
  line *WITH;

  /*
*/


  ul = p.userlines;
  while (ul != NULL) {
    WITH = &ul->l;

    /*




*/
    boxintercept(p.xmin, p.ymin, p.xmax, p.ymax, WITH->m, WITH->b, &intercept,
		 &x1, &y1, &x2, &y2);
    /*




*/

    if (intercept) {
      /*









*/

      if (WITH->linetype != 'i' && WITH->linetype != 'n') {
	if (WITH->linetype != 'n' && WITH->linetype != 'l' &&
	    WITH->linetype != '-' && WITH->linetype != '.') {
	  printf("error in plotuserlines: \n");
	  printf("linetype (=%c) is not one of \"l.-in\"\n", WITH->linetype);

	  halt();
	}

	movea(outfile, locatex(p, x1), locatey(p, y1));
	drawa(outfile, locatex(p, x2), locatey(p, y2), WITH->linetype,
	      WITH->linesize);
      }
    }

    ul = ul->next;
  }
}



Static Void plotregressionlines(outfile, p)
_TEXT *outfile;
xyploparam p;
{
  symbols *us;
  boolean intercept;
  /*
*/
  double x1, y1, x2, y2;
  symbol *WITH;
  line *WITH1;

  /*
*/
  us = p.usersymbols;
  while (us != NULL) {
    WITH = &us->s;
    if (WITH->doline) {
      WITH1 = &WITH->l;
      boxintercept(p.xmin, p.ymin, p.xmax, p.ymax, WITH1->m, WITH1->b,
		   &intercept, &x1, &y1, &x2, &y2);
      if (intercept) {
	movea(outfile, locatex(p, x1), locatey(p, y1));
	drawa(outfile, locatex(p, x2), locatey(p, y2), WITH1->linetype,
	      WITH1->linesize);
      }
    }
    us = us->next;
  }
}



Static Void telllines(outfile, p)
_TEXT *outfile;
xyploparam p;
{
  lines *ul;
  symbols *us;

  boolean intercept;
  /*
*/
  double x1, y1, x2, y2;
  symbol *WITH;
  line *WITH1;

  /*
*/
  /*



*/



  us = p.usersymbols;
  while (us != NULL) {
    WITH = &us->s;
    if (WITH->doline) {
      WITH1 = &WITH->l;

      boxintercept(p.xmin, p.ymin, p.xmax, p.ymax, WITH1->m, WITH1->b,
		   &intercept, &x1, &y1, &x2, &y2);
      comment(outfile);
      fprintf(outfile->f, "The regression line for symbol \"%c",
	      WITH->symboltoplot);
      fprintf(outfile->f, "\" with flag \"");
      writestring(outfile, &WITH->symbolflag);
      fprintf(outfile->f, "\"\n");
      comment(outfile);
      if (intercept) {
	if (p.logxnormal) {
	  x1 = exp(x1 * p.logxbase);
	  x2 = exp(x2 * p.logxbase);
	}
	if (p.logynormal) {
	  y1 = exp(y1 * p.logybase);
	  y2 = exp(y2 * p.logybase);
	}
	fprintf(outfile->f,
	  "intercepts the display at points (%*.*f, %*.*f) and (%*.*f, %*.*f)\n",
	  (int)p.xwidth, (int)(p.xdecimal + 1), x1, (int)p.ywidth,
	  (int)(p.ydecimal + 1), y1, (int)p.xwidth, (int)(p.xdecimal + 1), x2,
	  (int)p.ywidth, (int)(p.ydecimal + 1), y2);
      } else {
	fprintf(outfile->f, " does not intercept the display\n");
	comment(outfile);
	printf("the regression line does not intercept the display\n");
      }
    }
    us = us->next;
  }
  ul = p.userlines;
  while (ul != NULL) {
    WITH1 = &ul->l;
    boxintercept(p.xmin, p.ymin, p.xmax, p.ymax, WITH1->m, WITH1->b,
		 &intercept, &x1, &y1, &x2, &y2);
    comment(outfile);
    fprintf(outfile->f,
      "the user defined line of slope = %10.5f and intercept = %10.5f\n",
      WITH1->m, WITH1->b);

    comment(outfile);
    if (intercept) {
      if (p.logxnormal) {
	x1 = exp(x1 * p.logxbase);
	x2 = exp(x2 * p.logxbase);
      }
      if (p.logynormal) {
	y1 = exp(y1 * p.logybase);
	y2 = exp(y2 * p.logybase);
      }
      fprintf(outfile->f,
	" intercepts the display at points (%*.*f, %*.*f) and (%*.*f, %*.*f)\n",
	(int)p.xwidth, (int)(p.xdecimal + 1), x1, (int)p.ywidth,
	(int)(p.ydecimal + 1), y1, (int)p.xwidth, (int)(p.xdecimal + 1), x2,
	(int)p.ywidth, (int)(p.ydecimal + 1), y2);
    } else
      fprintf(outfile->f, " does not intercept the display\n");
    ul = ul->next;
  }

  /*

*/
}



Static Void makeusermarks(xyplom, outfile, usermarks, warnedaboutusermarks,
			  fontcontrol, ticscale, Xticscale, Yticscale,
			  Xlabelshift, Ylabelshift, usersymbols)
_TEXT *xyplom, *outfile;
boolean *usermarks, *warnedaboutusermarks;
Char *fontcontrol;
double *ticscale, *Xticscale, *Yticscale, *Xlabelshift, *Ylabelshift;
symbols *usersymbols;
{
  /*
*/
  Char control;
  symbols *us;
  stringDelila psdef;
  boolean debug = true;
  _TEXT TEMP;

  debug = false;
  if (*xyplom->name != '\0') {
    if (xyplom->f != NULL)
      xyplom->f = freopen(xyplom->name, "r", xyplom->f);
    else
      xyplom->f = fopen(xyplom->name, "r");
  } else
    rewind(xyplom->f);
  if (xyplom->f == NULL)
    _EscIO2(FileNotFound, xyplom->name);
  RESETBUF(xyplom->f, Char);
  *warnedaboutusermarks = false;
  if (BUFEOF(xyplom->f)) {
    *usermarks = false;
    return;
  }
  if (*xyplom->name != '\0') {
    if (xyplom->f != NULL)
      xyplom->f = freopen(xyplom->name, "r", xyplom->f);
    else
      xyplom->f = fopen(xyplom->name, "r");
  } else
    rewind(xyplom->f);
  if (xyplom->f == NULL)
    _EscIO2(FileNotFound, xyplom->name);
  RESETBUF(xyplom->f, Char);
  while (!BUFEOF(xyplom->f)) {
    if (P_eoln(xyplom->f)) {
      fscanf(xyplom->f, "%*[^\n]");
      getc(xyplom->f);
      continue;
    }
    control = getc(xyplom->f);
    if (control == '\n')
      control = ' ';
    if (control != '/')
      continue;
    clearstring(&psdef);
    /*




*/
    while (!P_eoln(xyplom->f) && control != '%' && control != '}' &&
	   control != ' ') {
      control = getc(xyplom->f);
      if (control == '\n')
	control = ' ';
      /*


*/
      if (control != '%' && control != '{' && control != ' ') {
	psdef.length++;
	psdef.letters[psdef.length - 1] = control;
      }
    }
    if (debug) {
      printf("Checking for symbol defined in xyplom: \"/");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &psdef);
      printf("\"\n");
    }
    if (psdef.length <= 0)
      continue;
    us = usersymbols;
    while (us != NULL) {
      if (debug) {
	printf("  symbolflag=\"");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &us->s.symbolflag);
	putchar('"');
	printf(" | symbol=\"%c\"", us->s.symboltoplot);
	putchar('"');
      }

      if (debug) {
	printf("symbol: \"");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &psdef);
	printf("= psdef BUBBA00\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &us->s.symbolflag);
	printf("= us^.s.symbolflag BUBBA01\n");
      }
      if (equalstring(us->s.symbolflag, psdef)) {
	if (debug)
	  printf(" is it");

	*usermarks = true;
	us->s.symbolhasxyplomdef = true;
      } else {
	if (debug)
	  printf(" is not it");
      }
      if (debug)
	putchar('\n');
      us = us->next;
    }
  }


  if (*xyplom->name != '\0') {
    if (xyplom->f != NULL)
      xyplom->f = freopen(xyplom->name, "r", xyplom->f);
    else
      xyplom->f = fopen(xyplom->name, "r");
  } else
    rewind(xyplom->f);
  if (xyplom->f == NULL)
    _EscIO2(FileNotFound, xyplom->name);
  RESETBUF(xyplom->f, Char);
  while (!BUFEOF(xyplom->f)) {
    if (!P_eoln(xyplom->f)) {
      control = getc(xyplom->f);
      if (control == '\n')
	control = ' ';
      putc(control, outfile->f);
      if (control == '%') {
	if (!P_eoln(xyplom->f)) {
	  control = getc(xyplom->f);
	  if (control == '\n')
	    control = ' ';
	  putc(control, outfile->f);
	  if (control == 'F') {
	    *fontcontrol = 'n';
	    printf("Fonts are controlled from xyplom, ");
	    printf("user is responsible for correct form!\n");
	    printf("Example for xyplom -----\n");
	    printf("%%F %% font control from xyplom\n");
	    printf("/Helvetica-Bold findfont\n");
	    printf("16 scalefont\n");
	    printf("setfont\n");
	    printf("2 setlinewidth %% sets linewidth\n");
	    printf("%%T 1.5 %% tic mark scale factor\n");
	    printf("------------------------\n");
	  }

	  if (control == 'T') {
	    fscanf(xyplom->f, "%lg", ticscale);
	    fprintf(outfile->f, "%5.1f %%T tic mark scale factor\n",
		    *ticscale);
	    printf("%5.1f %%T tic mark scale factor\n", *ticscale);
	  }

	  if (control == 'X') {
	    fscanf(xyplom->f, "%lg", Xticscale);
	    fprintf(outfile->f, "%5.1f %%X Xtic mark scale factor\n",
		    *Xticscale);
	    printf("%5.1f %%X Xtic mark scale factor\n", *Xticscale);
	  }

	  if (control == 'Y') {
	    fscanf(xyplom->f, "%lg", Yticscale);
	    fprintf(outfile->f, "%5.1f %%Y Ytic mark scale factor\n",
		    *Yticscale);
	    printf("%5.1f %%Y Ytic mark scale factor\n", *Yticscale);
	  }

	  if (control == 'x') {
	    fscanf(xyplom->f, "%lg", Xlabelshift);
	    fprintf(outfile->f, "%5.1f %%x label shift\n", *Xlabelshift);
	    printf("%5.1f %%x label shift\n", *Xlabelshift);
	  }

	  if (control == 'y') {
	    fscanf(xyplom->f, "%lg", Ylabelshift);
	    fprintf(outfile->f, "%5.1f %%y label shift\n", *Ylabelshift);
	    printf("%5.1f %%y label shift\n", *Ylabelshift);
	  }

	  if (control == 'W') {
	    fprintf(outfile->f, "%% white background on bounding box:\n");
	    fprintf(outfile->f, "gsave\n");
	    fprintf(outfile->f, "llx lly moveto\n");
	    fprintf(outfile->f, "urx lly lineto\n");
	    fprintf(outfile->f, "urx ury lineto\n");
	    fprintf(outfile->f, "llx ury lineto\n");
	    fprintf(outfile->f, "llx lly lineto\n");
	    fprintf(outfile->f, "1 setgray\n");
	    fprintf(outfile->f, "%%1 1 0 setrgbcolor %% for testing\n");
	    fprintf(outfile->f, "fill\n");
	    fprintf(outfile->f, "grestore\n\n");
	  }
	  /*


*/
	  /*

*/

	}
      }
    }
    copyaline(xyplom, outfile);
    /*

*/
    /*
*/
    /*


*/
    /*











*/

    /*

*/
  }

}



Static Void generategraph(infile, outfile, warnings, p, fontcontrol)
_TEXT *infile, *outfile, *warnings;
xyploparam p;
Char fontcontrol;
{
  /*
*/
  long linenumber;

  skipcopy(infile, outfile, false, &linenumber);


  startpic(outfile, defscale, p.xzero, p.yzero, fontcontrol);
  if (p.startrectangles)
    rectinit(outfile);




  /*









*/

  /*


*/

  /*




*/
  plotdata(infile, outfile, warnings, p);

  /*

*/

  setgray(outfile, 0.0);
  makeaxes(outfile, p);

  makexlabel(outfile, p);
  makeylabel(outfile, p);

  crosshairs(outfile, p);
  plotuserlines(outfile, p);
  plotregressionlines(outfile, p);

  stoppic(outfile);
}



Static Void themain(infile, outfile, xyplop, xyplom, warnings)
_TEXT *infile, *outfile, *xyplop, *xyplom, *warnings;
{
  /*
*/
  xyploparam p;
  _TEXT TEMP;

  printf("xyplo %4.2f\n", version);

  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else
    rewind(infile->f);
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
  if (BUFEOF(infile->f)) {
    printf("xyin file is empty\n");
    halt();
  }
  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else
    rewind(infile->f);
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);

  if (*outfile->name != '\0') {
    if (outfile->f != NULL)
      outfile->f = freopen(outfile->name, "w", outfile->f);
    else
      outfile->f = fopen(outfile->name, "w");
  } else {
    if (outfile->f != NULL)
      rewind(outfile->f);
    else
      outfile->f = tmpfile();
  }
  if (outfile->f == NULL)
    _EscIO2(FileNotFound, outfile->name);
  SETUPBUF(outfile->f, Char);
  warningcount = 0;
  datacount = 0;

  readparam(xyplop, &p);

  clearstring(&uniquestring);
  uniquestring.length = 6;
  uniquestring.letters[0] = 'U';
  uniquestring.letters[1] = 'N';
  uniquestring.letters[2] = 'I';
  uniquestring.letters[3] = 'Q';
  uniquestring.letters[4] = 'U';
  uniquestring.letters[5] = 'E';
  /*



*/

  fprintf(outfile->f, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(outfile->f, "%%%%Title: xyplo %4.2f\n", version);
  fprintf(outfile->f, "%%%%Creator: Tom Schneider\n");
  /*


*/


  fprintf(outfile->f, "%%%%BoundingBox: %5ld %5ld %5ld %5ld\n",
	  (long)floor(p.llx + 0.5), (long)floor(p.lly + 0.5),
	  (long)floor(p.urx + 0.5), (long)floor(p.ury + 0.5));
  fprintf(outfile->f, "%%%%DocumentFonts:\n");
  fprintf(outfile->f, "%%%%EndComments\n");
  fprintf(outfile->f, "%%%%EndProlog\n\n");

  /*

*/

  fprintf(outfile->f, "/defaultllx %5.1f def\n", defaultllx);
  fprintf(outfile->f, "/defaultlly %5.1f def\n", defaultlly);
  fprintf(outfile->f, "/defaulturx %5.1f def\n", defaulturx);
  fprintf(outfile->f, "/defaultury %5.1f def\n", defaultury);
  fprintf(outfile->f, "/llx %5.1f def\n", p.llx);
  fprintf(outfile->f, "/lly %5.1f def\n", p.lly);
  fprintf(outfile->f, "/urx %5.1f def\n", p.urx);
  fprintf(outfile->f, "/ury %5.1f def\n", p.ury);

  fprintf(outfile->f,
	  "/cmfactor 72 2.54 div def %% defines points -> centimeters\n");
  fprintf(outfile->f, "/cm { cmfactor mul} def %% defines centimeters\n");
  fprintf(outfile->f, "/edgecontrol (%c) def\n", p.edgecontrol);
  fprintf(outfile->f, "/edgeleft  %*.*f cm def\n", pwid, pdec, p.edgeleft);
  fprintf(outfile->f, "/edgeright %*.*f cm def\n", pwid, pdec, p.edgeright);
  fprintf(outfile->f, "/edgelow   %*.*f cm def\n", pwid, pdec, p.edgelow);
  fprintf(outfile->f, "/edgehigh  %*.*f cm def\n", pwid, pdec, p.edgehigh);

  /*
*/
  bounds(infile, outfile, &p);

  writeparam(outfile, p);

  regressiondata(outfile, p);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  regressiondata(&TEMP, p);

  telllines(outfile, p);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  telllines(&TEMP, p);


  p.fontcontrol = 't';
  p.ticscale = 1.0;
  p.Xticscale = 1.0;
  p.Yticscale = 1.0;
  p.Xlabelshift = 0.0;
  p.Ylabelshift = 0.0;
  makeusermarks(xyplom, outfile, &p.usermarks, &p.warnedaboutusermarks,
		&p.fontcontrol, &p.ticscale, &p.Xticscale, &p.Yticscale,
		&p.Xlabelshift, &p.Ylabelshift, p.usersymbols);

  /*



*/
  warningcount = 0;
  datacount = 0;


  generategraph(infile, outfile, warnings, p, p.fontcontrol);

  if (warningcount > 0) {
    printf("**********");
    printf(" There are %ld WARNINGS in file warnings", warningcount);
    printf(" **********\n");
  }

  fprintf(outfile->f, "%%%%Trailer\n");
  fprintf(outfile->f, "%%%%Pages: 1\n");


}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  warnings.f = NULL;
  strcpy(warnings.name, "warnings");
  xyplop.f = NULL;
  strcpy(xyplop.name, "xyplop");
  xyplom.f = NULL;
  strcpy(xyplom.name, "xyplom");
  xyout.f = NULL;
  strcpy(xyout.name, "xyout");
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  themain(&xyin, &xyout, &xyplop, &xyplom, &warnings);
_L1:
  if (xyin.f != NULL)
    fclose(xyin.f);
  if (xyout.f != NULL)
    fclose(xyout.f);
  if (xyplom.f != NULL)
    fclose(xyplom.f);
  if (xyplop.f != NULL)
    fclose(xyplop.f);
  if (warnings.f != NULL)
    fclose(warnings.f);
  exit(EXIT_SUCCESS);
}

/*






























































*/



/* End. */
