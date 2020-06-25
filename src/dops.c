/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dops.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         2.80
/*









*/



/*
























































































*/




#define maxstring       2000




#define fillermax       20



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





typedef double threevector[3];

typedef double tbtarray[3][3];

typedef struct screen {
  threevector a, b, c, v, g;
  double smag, range;
} screen;




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;




typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;



/*


*/
typedef Char filler[fillermax];



Static _TEXT demo;



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
  /*
*/
  if (t->seek.letters[0] == ch) {
    t->state = 1;
    t->skip = false;
    t->found = false;
    return;
  }
  t->state = 0;
  t->skip = true;
  t->found = false;
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



Static Void await()
{
  /*
*/
  /*




*/
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





Static Void dotr(afile)
_TEXT *afile;
{
  /*

*/
  drawr(afile, dotfactor, 0.0, 'l', 0.0);
}



Static Void boxr(afile, width, height)
_TEXT *afile;
double width, height;
{
  /*



*/
  liner(afile, 0.0, height);
  liner(afile, width, 0.0);
  liner(afile, 0.0, -height);
  liner(afile, -width, 0.0);
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



Static Void polrec(r, theta, x, y)
double r, theta, *x, *y;
{
  /*
*/
  *x = r * cos(theta);
  *y = r * sin(theta);
}



Static double degtorad(angle)
double angle;
{
  return (angle / 360 * 2 * pi);
}



Static Void spiral(afile, thickness, steps, radius)
_TEXT *afile;
double thickness;
long steps;
double radius;
{
  /*



*/
  double dr, dtheta;
  double r = 0.0, theta = 0.0;
  double x, xpos, y, ypos;

  if (steps == 0)
    return;
  xpos = picxglobal;
  ypos = picyglobal;
  dr = fabs(thickness / steps);
  dtheta = 2 * pi / steps;

  while (r < fabs(radius)) {
    r += dr;
    theta += dtheta;
    polrec(r, theta, &x, &y);
    linea(afile, x + xpos, y + ypos);
  }
  movea(afile, xpos, ypos);
}



Static Void movepolar(afile, angle, distance)
_TEXT *afile;
double angle, distance;
{
  /*

*/
  double dx, dy;

  polrec(distance, degtorad(angle), &dx, &dy);
  mover(afile, dx, dy);
}


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

#define huefactor       0.85



Static Void setcolor(afile, hue, saturation, brightness)
_TEXT *afile;
double hue, saturation, brightness;
{
  /*




















*/
  putc('n', afile->f);
  fprintf(afile->f, " %*.*f", colfield, colwidth, hue * huefactor);
  fprintf(afile->f, " %*.*f", colfield, colwidth, saturation);
  fprintf(afile->f, " %*.*f", colfield, colwidth, brightness);
  fprintf(afile->f, " sethsbcolor\n");
}

#undef colfield
#undef colwidth
#undef huefactor



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



Static Void rectdo(afile)
_TEXT *afile;
{
  /*

*/
  fprintf(afile->f, " rct\n");
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







Static double determinant(a)
double (*a)[3];
{
  return (a[0][0] * (a[1][1] * a[2][2] - a[2][1] * a[1][2]) + a[0]
	    [1] * (a[2][0] * a[1][2] - a[1][0] * a[2][2]) + a[0]
	    [2] * (a[1][0] * a[2][1] - a[2][0] * a[1][1]));
}



Static Void d32(o, a, b, c, v, xloc, yloc)
double *o, *a, *b, *c, *v;
double *xloc, *yloc;
{
  /*













*/
  double ov, oa;
  long j;
  tbtarray d, x, y;

  for (j = 0; j <= 2; j++) {
    ov = o[j] - v[j];
    d[j][0] = b[j];
    d[j][1] = c[j];
    d[j][2] = ov;

    oa = o[j] - a[j];
    x[j][0] = oa;
    x[j][1] = c[j];
    x[j][2] = ov;

    y[j][0] = b[j];
    y[j][1] = oa;
    y[j][2] = ov;
  }


  *xloc = determinant(x) / determinant(d);
  *yloc = determinant(y) / determinant(d);
}



Static Void view(v, gaze, smag, a, b, c)
double *v, *gaze;
double smag;
double *a, *b, *c;
{
  /*












*/
  double db, dgaze;
  long j;

  dgaze = sqrt(gaze[0] * gaze[0] + gaze[1] * gaze[1] + gaze[2] * gaze[2]);
  if (smag == 0.0) {
    printf("screen magnitude cannot be zero\n");
    halt();
  }
  if (dgaze <= 0.001) {
    printf("gaze magnitude (%5.3f) is too small\n", dgaze);
    halt();
  }

  /*
*/
  for (j = 0; j <= 2; j++) {
    gaze[j] /= dgaze;
    a[j] = v[j] + gaze[j];
  }

  /*
*/
  b[0] = gaze[1];
  b[1] = -gaze[0];
  b[2] = 0.0;
  db = sqrt(b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);


  if (db == 0.0) {
    db = 1.0;
    b[0] = 1.0;
    b[1] = 0.0;

  } else {
    for (j = 0; j <= 2; j++)
      b[j] /= db;
  }

  /*


*/
  c[0] = b[1] * gaze[2] - gaze[1] * b[2];
  c[1] = gaze[0] * b[2] - b[0] * gaze[2];
  c[2] = b[0] * gaze[1] - gaze[0] * b[1];


  for (j = 0; j <= 2; j++) {
    b[j] /= smag;
    c[j] /= smag;
  }
}



Static Void makescreen(vx, vy, vz, gx, gy, gz, smagnitude, s)
double vx, vy, vz, gx, gy, gz, smagnitude;
screen *s;
{
  /*

*/

  /*
*/
  s->v[0] = vx;
  s->v[1] = vy;
  s->v[2] = vz;
  s->g[0] = gx;
  s->g[1] = gy;
  s->g[2] = gz;
  view(s->v, s->g, smagnitude, s->a, s->b, s->c);
  s->smag = smagnitude;
  s->range = 1 / smagnitude;
}



Static Void project3d(x, y, z, s, xscreen, yscreen)
double x, y, z;
screen s;
double *xscreen, *yscreen;
{
  /*
*/
  threevector o;

  o[0] = x;
  o[1] = y;
  o[2] = z;
  d32(o, s.a, s.b, s.c, s.v, xscreen, yscreen);
}



Static double fun(r)
double r;
{
  return (3 / (1 + r * r / 2));
}



Static Void test3d(afile)
_TEXT *afile;
{
  screen s;

  double xscreen, yscreen;
  /*
*/
  double xold, yold;


  double dr, dtheta;
  double r = 0.0, radius = 2.0, theta = 0.0, thickness = 0.1, steps = 15.0;

  double x = 0.0, y = 0.0;
  double z;

  makescreen(5.0, 5.0, 5.0, -1.0, -1.0, -1.0, 5.0, &s);

  dr = thickness / steps;
  dtheta = 2 * pi / steps;

  z = fun(r);
  project3d(x, y, z, s, &xold, &yold);
  mover(afile, xold, yold);

  while (r < radius) {
    r += dr;
    theta += dtheta;
    polrec(r, theta, &x, &y);

    z = fun(r);
    project3d(x, y, z, s, &xscreen, &yscreen);


    liner(afile, xscreen - xold, yscreen - yold);
    xold = xscreen;
    yold = yscreen;
  }
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




Static Void readchar(a, c)
_TEXT *a;
Char *c;
{
  /*
*/
  skipblanks(a);
  *c = getc(a->f);
  if (*c == '\n')
    *c = ' ';
  skipnonblanks(a);
}




Static Void mkhalt(outfile)
_TEXT *outfile;
{
  fprintf(outfile->f, "dops ");
  halt();
}


/* Local variables for testblank: */
struct LOC_testblank {
  _TEXT *outfile;
} ;

Local Void die(LINK)
struct LOC_testblank *LINK;
{
  fprintf(LINK->outfile->f, "(badly formed graphics instruction) show\n");
  stoppic(LINK->outfile);
  mkhalt(LINK->outfile);
}



Static Void testblank(infile, outfile_)
_TEXT *infile, *outfile_;
{
  /*


*/
  struct LOC_testblank V;

  V.outfile = outfile_;
  if (P_eoln(infile->f))
    die(&V);
  else if (P_peek(infile->f) != ' ')
    die(&V);
}



Static Void mkstartpic(infile, outfile)
_TEXT *infile, *outfile;
{
  double scale, x, y;

  fscanf(infile->f, "%lg%lg%lg%*[^\n]", &scale, &x, &y);
  getc(infile->f);
  startpic(outfile, scale, x, y, 't');
}



Static Void mkstoppic(infile, outfile)
_TEXT *infile, *outfile;
{
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
  stoppic(outfile);
}



Static Void mkdrawr(infile, outfile)
_TEXT *infile, *outfile;
{
  double dx, dy;
  Char visibility;
  double spacing;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg", &dx, &dy);
  readchar(infile, &visibility);
  fscanf(infile->f, "%lg%*[^\n]", &spacing);
  getc(infile->f);
  drawr(outfile, dx, dy, visibility, spacing);
}



Static Void mkmover(infile, outfile)
_TEXT *infile, *outfile;
{
  double dx, dy;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%*[^\n]", &dx, &dy);
  getc(infile->f);
  mover(outfile, dx, dy);
}



Static Void mkliner(infile, outfile)
_TEXT *infile, *outfile;
{
  double dx, dy;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%*[^\n]", &dx, &dy);
  getc(infile->f);
  liner(outfile, dx, dy);
}



Static Void mkdrawa(infile, outfile)
_TEXT *infile, *outfile;
{
  double x, y;
  Char visibility;
  double spacing;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg", &x, &y);
  readchar(infile, &visibility);
  fscanf(infile->f, "%lg%*[^\n]", &spacing);
  getc(infile->f);
  drawa(outfile, x, y, visibility, spacing);
}



Static Void mkmovea(infile, outfile)
_TEXT *infile, *outfile;
{
  double x, y;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%*[^\n]", &x, &y);
  getc(infile->f);
  movea(outfile, x, y);
}



Static Void mklinea(infile, outfile)
_TEXT *infile, *outfile;
{
  double x, y;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%*[^\n]", &x, &y);
  getc(infile->f);
  linea(outfile, x, y);
}



Static Void mkdotr(infile, outfile)
_TEXT *infile, *outfile;
{
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
  dotr(outfile);
}



Static Void mkpicnumber(infile, outfile)
_TEXT *infile, *outfile;
{
  double dx, dy, number;
  long width, decimal;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%ld%ld", &dx, &dy, &number, &width, &decimal);
  skipblanks(infile);
  P_peek(infile->f);
  fscanf(infile->f, "%*[^\n]");
/* p2c: dops.p: Note: Eliminated unused assignment statement [338] */
  getc(infile->f);
  picnumber(outfile, dx, dy, number, width, decimal, 'c');
}



Static Void mkxtic(infile, outfile)
_TEXT *infile, *outfile;
{
  double length, dx, dy, number;
  long width, decimal;
  boolean logxnormal;
  Char clogxnormal;
  double logxbase;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%lg%ld%ld%c%lg%*[^\n]", &length, &dx, &dy,
	 &number, &width, &decimal, &clogxnormal, &logxbase);
  getc(infile->f);
  if (clogxnormal == '\n')
    clogxnormal = ' ';
  logxnormal = (clogxnormal == 't');
  xtic(outfile, length, dx, dy, number, width, decimal, logxnormal, logxbase);
  /*




*/
}



Static Void mkytic(infile, outfile)
_TEXT *infile, *outfile;
{
  double length, dx, dy, number;
  long width, decimal;
  boolean logynormal;
  Char clogynormal;
  double logybase;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%lg%ld%ld%c%lg%*[^\n]", &length, &dx, &dy,
	 &number, &width, &decimal, &clogynormal, &logybase);
  getc(infile->f);
  if (clogynormal == '\n')
    clogynormal = ' ';
  logynormal = (clogynormal == 't');
  ytic(outfile, length, dx, dy, number, width, decimal, logynormal, logybase);
}



Static Void mkxaxis(infile, outfile)
_TEXT *infile, *outfile;
{
  boolean doaxisline;
  double axlength, fromtic, interval, totic, xsubintervals, length, dx, dy;
  long width, decimal;
  boolean logxscale, logxnormal;
  Char clogxscale, clogxnormal;
  double logxbase;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%lg%lg%lg%lg%lg%ld%ld%c%c%lg%*[^\n]", &axlength,
	 &fromtic, &interval, &totic, &xsubintervals, &length, &dx, &dy,
	 &width, &decimal, &clogxscale, &clogxnormal, &logxbase);
  getc(infile->f);
  if (clogxscale == '\n')
    clogxscale = ' ';
  if (clogxnormal == '\n')
    clogxnormal = ' ';
  logxnormal = (clogxnormal == 't');
  logxscale = (clogxscale == 't');
/* p2c: dops.p: Note: Eliminated unused assignment statement [338] */
  xaxis(outfile, true, axlength, fromtic, interval, totic, xsubintervals,
	length, dx, dy, width, decimal, logxscale, logxnormal, logxbase);
}



Static Void mkyaxis(infile, outfile)
_TEXT *infile, *outfile;
{
  boolean doaxisline;
  double aylength, fromtic, interval, totic, ysubintervals, length, dx, dy;
  long width, decimal;
  boolean logyscale, logynormal;
  Char clogyscale, clogynormal;
  double logybase;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%lg%lg%lg%lg%lg%ld%ld%c%c%lg%*[^\n]", &aylength,
	 &fromtic, &interval, &totic, &ysubintervals, &length, &dx, &dy,
	 &width, &decimal, &clogyscale, &clogynormal, &logybase);
  getc(infile->f);
  if (clogyscale == '\n')
    clogyscale = ' ';
  if (clogynormal == '\n')
    clogynormal = ' ';
  logynormal = (clogynormal == 't');
  logyscale = (clogyscale == 't');
/* p2c: dops.p: Note: Eliminated unused assignment statement [338] */
  yaxis(outfile, true, aylength, fromtic, interval, totic, ysubintervals,
	length, dx, dy, width, decimal, logyscale, logynormal, logybase);
}



Static Void mkboxr(infile, outfile)
_TEXT *infile, *outfile;
{
  double width, height;

  testblank(infile, outfile);

  fscanf(infile->f, "%lg%lg%*[^\n]", &width, &height);
  getc(infile->f);
  boxr(outfile, width, height);
}



Static Void mkcboxr(infile, outfile)
_TEXT *infile, *outfile;
{
  double width, height;

  testblank(infile, outfile);

  fscanf(infile->f, "%lg%lg%*[^\n]", &width, &height);
  getc(infile->f);

  cboxr(outfile, width, height);
}



Static Void mkibeam(infile, outfile)
_TEXT *infile, *outfile;
{
  double width, height;

  testblank(infile, outfile);

  fscanf(infile->f, "%lg%lg%*[^\n]", &width, &height);
  getc(infile->f);

  ibeam(outfile, width, height);
}



Static Void mkcircler(infile, outfile)
_TEXT *infile, *outfile;
{
  double radius;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%*[^\n]", &radius);
  getc(infile->f);
  circler(outfile, radius);
}



Static Void mkspiral(infile, outfile)
_TEXT *infile, *outfile;
{
  double thickness;
  long steps;
  double radius;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%ld%lg%*[^\n]", &thickness, &steps, &radius);
  getc(infile->f);
  spiral(outfile, thickness, steps, radius);
}



Static Void mkmovepolar(infile, outfile)
_TEXT *infile, *outfile;
{
  double angle, distance;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%*[^\n]", &angle, &distance);
  getc(infile->f);
  movepolar(outfile, angle, distance);
}



Static Void mkarc(infile, outfile)
_TEXT *infile, *outfile;
{
  double angle1, angle2, radius;
  long steps;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%ld%*[^\n]", &angle1, &angle2, &radius, &steps);
  getc(infile->f);
  arc(outfile, angle1, angle2, radius, steps);
}



Static Void mkplusr(infile, outfile)
_TEXT *infile, *outfile;
{
  double width, height;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%*[^\n]", &width, &height);
  getc(infile->f);
  plusr(outfile, width, height);
}



Static Void mkxr(infile, outfile)
_TEXT *infile, *outfile;
{
  double width, height;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%*[^\n]", &width, &height);
  getc(infile->f);
  xr(outfile, width, height);
}



Static Void mktest3d(outfile)
_TEXT *outfile;
{
  test3d(outfile);
}



Static Void mksetgray(infile, outfile)
_TEXT *infile, *outfile;
{
  double brightness;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%*[^\n]", &brightness);
  getc(infile->f);
  setgray(outfile, brightness);
}



Static Void mksetcolor(infile, outfile)
_TEXT *infile, *outfile;
{
  double hue, saturation, brightness;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%*[^\n]", &hue, &saturation, &brightness);
  getc(infile->f);
  setcolor(outfile, hue, saturation, brightness);
}



Static Void mkrectinit(outfile)
_TEXT *outfile;
{
  xsideold = 0.0;
  ysideold = 0.0;
  rectinit(outfile);
}



Static Void mkrectdo(outfile)
_TEXT *outfile;
{
  rectdo(outfile);
}



Static Void mkrectsize(infile, outfile)
_TEXT *infile, *outfile;
{
  double xside, yside;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%*[^\n]", &xside, &yside);
  getc(infile->f);
  rectsize(outfile, &xsideold, &ysideold, &xside, &yside);
}


Static Void translate PP((_TEXT *demofile, _TEXT *infile, _TEXT *outfile,
			  long depth));

/* Local variables for translate: */
struct LOC_translate {
  _TEXT *demofile, *infile, *outfile;
  long depth;
  stringDelila buffer;
  Char ch;
  boolean go;
  trigger pe, ps;


  trigger halt, demo, drawr, mover, liner, drawa, movea, linea, picnumber,
	  xtic, ytic, xaxis, yaxis, dotr, boxr, cboxr, ibeam, circler, spiral,
	  movepolar, arc, plusr, xr, test3d, setgray, setcolor, rectinit,
	  rectsize, rectdo;
} ;

/*




*/

Local Void fill(LINK)
struct LOC_translate *LINK;
{
  filltrigger(&LINK->ps, ".PS                 ");
  filltrigger(&LINK->pe, ".PE                 ");

  filltrigger(&LINK->halt, "halt                ");
  filltrigger(&LINK->demo, "demo                ");
  filltrigger(&LINK->drawr, "drawr               ");
  filltrigger(&LINK->mover, "mover               ");
  filltrigger(&LINK->liner, "liner               ");
  filltrigger(&LINK->drawa, "drawa               ");
  filltrigger(&LINK->movea, "movea               ");
  filltrigger(&LINK->linea, "linea               ");
  filltrigger(&LINK->dotr, "dotr                ");
  filltrigger(&LINK->picnumber, "picnumber           ");
  filltrigger(&LINK->xtic, "xtic                ");
  filltrigger(&LINK->ytic, "ytic                ");
  filltrigger(&LINK->xaxis, "xaxis               ");
  filltrigger(&LINK->yaxis, "yaxis               ");
  filltrigger(&LINK->boxr, "boxr                ");
  filltrigger(&LINK->cboxr, "cboxr               ");
  filltrigger(&LINK->ibeam, "ibeam               ");
  filltrigger(&LINK->circler, "circler             ");
  filltrigger(&LINK->spiral, "spiral              ");
  filltrigger(&LINK->movepolar, "movepolar           ");
  filltrigger(&LINK->arc, "arc                 ");
  filltrigger(&LINK->plusr, "plusr               ");
  filltrigger(&LINK->xr, "xr                  ");
  filltrigger(&LINK->test3d, "test3d              ");
  filltrigger(&LINK->setgray, "setgray             ");
  filltrigger(&LINK->setcolor, "setcolor            ");
  filltrigger(&LINK->rectinit, "rectinit            ");
  filltrigger(&LINK->rectsize, "rectsize            ");
  filltrigger(&LINK->rectdo, "rectdo              ");
}

Local Void resetall(LINK)
struct LOC_translate *LINK;
{
  resettrigger(&LINK->ps);
  resettrigger(&LINK->pe);

  resettrigger(&LINK->halt);
  resettrigger(&LINK->demo);
  resettrigger(&LINK->drawr);
  resettrigger(&LINK->mover);
  resettrigger(&LINK->liner);
  resettrigger(&LINK->drawa);
  resettrigger(&LINK->movea);
  resettrigger(&LINK->linea);
  resettrigger(&LINK->dotr);
  resettrigger(&LINK->picnumber);
  resettrigger(&LINK->xtic);
  resettrigger(&LINK->ytic);
  resettrigger(&LINK->xaxis);
  resettrigger(&LINK->yaxis);
  resettrigger(&LINK->boxr);
  resettrigger(&LINK->cboxr);
  resettrigger(&LINK->ibeam);
  resettrigger(&LINK->circler);
  resettrigger(&LINK->spiral);
  resettrigger(&LINK->movepolar);
  resettrigger(&LINK->arc);
  resettrigger(&LINK->plusr);
  resettrigger(&LINK->xr);
  resettrigger(&LINK->test3d);
  resettrigger(&LINK->setgray);
  resettrigger(&LINK->setcolor);
  resettrigger(&LINK->rectinit);
  resettrigger(&LINK->rectsize);
  resettrigger(&LINK->rectdo);
}

Local Void tests(LINK)
struct LOC_translate *LINK;
{
  /*

*/
  if (inpicture) {
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->pe);
      if (LINK->pe.found) {
	if (LINK->depth > 0) {
	  printf("%% .PE ignored in demo file\n");
	  while (!BUFEOF(LINK->demofile->f)) {
	    fscanf(LINK->demofile->f, "%*[^\n]");
	    getc(LINK->demofile->f);
	  }
	} else if (!inpicture)
	  printf("%% .PE ignored: not in picture\n");
	else
	  mkstoppic(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }

    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->halt);
      if (LINK->halt.found) {
	mkhalt(LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->demo);
      if (LINK->demo.found) {
	if (LINK->depth >= 1)
	  printf("%% RECURSIVE DEMONSTRATION REFUSED\n");
	else {
	  printf("%% DEMONSTRATION BEGINS\n");
	  if (*LINK->demofile->name != '\0') {
	    if (LINK->demofile->f != NULL)
	      LINK->demofile->f = freopen(LINK->demofile->name, "r",
					  LINK->demofile->f);
	    else
	      LINK->demofile->f = fopen(LINK->demofile->name, "r");
	  } else
	    rewind(LINK->demofile->f);
	  if (LINK->demofile->f == NULL)
	    _EscIO2(FileNotFound, LINK->demofile->name);
	  RESETBUF(LINK->demofile->f, Char);
	  if (inpicture) {
	    /*

*/
	    resettrigger(&LINK->ps);
	    while ((!LINK->ps.found) & (!BUFEOF(LINK->demofile->f))) {
	      LINK->ch = getc(LINK->demofile->f);
	      if (LINK->ch == '\n')
		LINK->ch = ' ';
	      testfortrigger(LINK->ch, &LINK->ps);
	      while (P_eoln(LINK->demofile->f) & (!BUFEOF(LINK->demofile->f))) {
		fscanf(LINK->demofile->f, "%*[^\n]");
		getc(LINK->demofile->f);
	      }
	    }
	  }
	  translate(LINK->demofile, LINK->demofile, LINK->outfile,
		    LINK->depth + 1);
	  if (!inpicture)
	    mkstartpic(LINK->infile, LINK->outfile);
	  printf("%% DEMONSTRATION ENDS\n");
	}
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->drawr);
      if (LINK->drawr.found) {
	mkdrawr(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->mover);
      if (LINK->mover.found) {
	mkmover(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->liner);
      if (LINK->liner.found) {
	mkliner(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->drawa);
      if (LINK->drawa.found) {
	mkdrawa(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->movea);
      if (LINK->movea.found) {
	mkmovea(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->linea);
      if (LINK->linea.found) {
	mklinea(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->dotr);
      if (LINK->dotr.found) {
	mkdotr(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->picnumber);
      if (LINK->picnumber.found) {
	mkpicnumber(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->xtic);
      if (LINK->xtic.found) {
	mkxtic(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->ytic);
      if (LINK->ytic.found) {
	mkytic(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->xaxis);
      if (LINK->xaxis.found) {
	mkxaxis(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->yaxis);
      if (LINK->yaxis.found) {
	mkyaxis(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->cboxr);
      if (LINK->cboxr.found) {
	mkcboxr(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->ibeam);
      if (LINK->ibeam.found) {
	mkibeam(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      /*


*/
      testfortrigger(LINK->ch, &LINK->boxr);
      if (LINK->boxr.found) {
	mkboxr(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->circler);
      if (LINK->circler.found) {
	mkcircler(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->spiral);
      if (LINK->spiral.found) {
	mkspiral(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->movepolar);
      if (LINK->movepolar.found) {
	mkmovepolar(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->arc);
      if (LINK->arc.found) {
	mkarc(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->plusr);
      if (LINK->plusr.found) {
	mkplusr(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->xr);
      if (LINK->xr.found) {
	mkxr(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->test3d);
      if (LINK->test3d.found) {
	mktest3d(LINK->outfile);
	LINK->go = false;
      }
    }

    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->setgray);
      if (LINK->setgray.found) {
	mksetgray(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }

    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->setcolor);
      if (LINK->setcolor.found) {
	mksetcolor(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }

    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->rectinit);
      if (LINK->rectinit.found) {
	mkrectinit(LINK->outfile);
	LINK->go = false;
      }
    }

    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->rectdo);
      if (LINK->rectdo.found) {
	mkrectdo(LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->rectsize);
      if (LINK->rectsize.found) {
	mkrectsize(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }

    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->ps);
      if (LINK->ps.found) {
	printf("%% .PS ignored: already in picture\n");
	LINK->go = false;
      }
    }

    if (!LINK->go)
      return;
    if (LINK->ch != ' ')
      return;
    if (!P_eoln(LINK->infile->f)) {
      while (!P_eoln(LINK->infile->f) && LINK->buffer.length < maxstring) {
	LINK->buffer.length++;
	LINK->buffer.letters[LINK->buffer.length - 1] = getc(LINK->infile->f);
	if (LINK->buffer.letters[LINK->buffer.length - 1] == '\n')
	  LINK->buffer.letters[LINK->buffer.length - 1] = ' ';
      }
      graphstring(LINK->outfile, &LINK->buffer, 'l');
    }
    LINK->go = false;
    return;
  }
  testfortrigger(LINK->ch, &LINK->ps);
  if (LINK->ps.found) {
    mkstartpic(LINK->infile, LINK->outfile);
    LINK->go = false;
  }
}



Static Void translate(demofile_, infile_, outfile_, depth_)
_TEXT *demofile_, *infile_, *outfile_;
long depth_;
{
  /*


*/
  struct LOC_translate V;
  long index;

  V.demofile = demofile_;
  V.infile = infile_;
  V.outfile = outfile_;
  V.depth = depth_;
  fill(&V);


  while (!BUFEOF(V.infile->f)) {
    resetall(&V);
    clearstring(&V.buffer);
    index = 0;
    V.go = true;
    while (V.go) {
      if (P_eoln(V.infile->f)) {
	fscanf(V.infile->f, "%*[^\n]");
	getc(V.infile->f);
	if (inpicture)
	  graphstring(V.outfile, &V.buffer, 'l');
	else {
	  putc('%', V.outfile->f);
	  writestring(V.outfile, &V.buffer);
	  putc('\n', V.outfile->f);
	}

	V.go = false;
	continue;
      }

      if (index >= maxstring) {
	fprintf(V.outfile->f, "\ntranslate: line too long\n");
	mkhalt(V.outfile);
	continue;
      }
      V.ch = getc(V.infile->f);

      if (V.ch == '\n')
	V.ch = ' ';
      index++;
      V.buffer.letters[index-1] = V.ch;
      V.buffer.length = index;

      tests(&V);
    }
  }
}



Static Void themain(demo, fromfile, tofile)
_TEXT *demo, *fromfile, *tofile;
{
  printf("%%!\n");
  printf("%% dops %4.2f\n", version);

  /*


*/
  translate(demo, fromfile, tofile, 0L);
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  demo.f = NULL;
  strcpy(demo.name, "demo");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&demo, &TEMP, &TEMP1);
_L1:
  if (demo.f != NULL)
    fclose(demo.f);
  exit(EXIT_SUCCESS);
}



/* End. */
