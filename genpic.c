/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "genpic.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/




#define version         2.62
/*






















































*/
#define updateversion   2.50
#define bugversion226   2.26
/*

*/



/*



































































































































































































*/




#define dx              1.0
#define dy              3.0

#define drotation       0

#define dgraphheight    5.0
#define dboxwidth       0.15
#define dintervalsize   1.25
#define dhistogramvalue  1.25
#define dboxshift       (-0.15)
/*
*/

#define difield         8
#define didecimal       2
#define dnfield         5
#define dmodulo         1

#define dmodulomultiplier  1.0


#define defaultllx      10.08
#define defaultlly      8.91
#define defaulturx      588.06
#define defaultury      779.85




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




#define maxstring       2000




#define fillermax       50



#define maxslots        1000
#define defslots        100
#define pageheight      105

#define dch             '+'
#define sch             ':'
#define bch             '*'



typedef enum {
  none, gaussian, poisson
} plots;
/*
*/
typedef enum {
  start, stop
} endpoints;

typedef struct rhistarray {
  /*

*/

  double numbers[maxslots];
  double range[2];
  double interval;
  long slots;
} rhistarray;




typedef struct histdata {
  /*
*/
  long column, entries;
  double minimum, maximum, mean, stdev, sem, variance, uncertainty,
	 computeduncertainty;
  /*
*/
  double start_, stop_, xinterval;
  long slots, ymaximum;
  double yaxisscale;
  Char plot;
} histdata;




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



typedef struct title {
  long titlenamesize;
  double titlex, titley;
  stringDelila letters;
  struct title *next;
} title;





Static _TEXT histog, genpicp, picout;

Static double parameterversion;



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


#define pi_             3.14159265

#define Zmax            10





Static Void gaushist(entries, ave, vari, stanarray)
long entries;
double ave, vari;
rhistarray *stanarray;
{
  /*









*/
  /*
*/
  /*
*/
  double sd, d1, d2, x, ex;
  long i;
  double Z;
  long FORLIM;

  sd = sqrt(vari);
  d1 = 2 * vari;
  d2 = sd * sqrt(2 * pi_);
  FORLIM = stanarray->slots;
  for (i = 0; i < FORLIM; i++) {
    x = stanarray->range[(long)start] + (i + 0.5) * stanarray->interval;
    /*

*/
    Z = (ave - x) * (x - ave) / d1;
    if (-Z < Zmax)
      ex = exp(Z) / d2 * stanarray->interval;
    else
      ex = 0.0;
    /*

*/

    stanarray->numbers[i] = ex * entries;
  }
}

#undef pi_
#undef Zmax


#define pi_             3.14159265



Static Void poishist(entries, ave, stanarray)
long entries;
double ave;
rhistarray *stanarray;
{
  /*
















*/
  double x, lnex, ex;
  long i, FORLIM;

  if (ave <= 0) {
    printf(
      " warning: poisson not defined for ave <= 0, procedure poishist called but not used\n");
    return;
  }
  FORLIM = stanarray->slots;
  for (i = 0; i < FORLIM; i++) {
    x = stanarray->range[(long)start] + (i + 0.5) * stanarray->interval;

    if (x < 0) {
      ex = 0.0;

    } else if (x <= 1)
      ex = exp(-ave);
    else {
      lnex = x - ave + x * log(ave / x) - log(2 * pi_ * x) / 2;
      ex = exp(lnex) * stanarray->interval;
    }
    stanarray->numbers[i] = (long)floor(ex * entries + 0.5);
  }

}

#undef pi_






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
    break;

  case 't':
    fprintf(afile->f, "/Times-Roman findfont\n");
    fprintf(afile->f, "%d scalefont\n", 12);
    break;
  }

  fprintf(afile->f, "setfont\n");

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



Static Void drawr(afile, dx_, dy_, visibility, spacing)
_TEXT *afile;
double dx_, dy_;
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
	  picwidth, picdecim, dx_ * scale, picwidth, picdecim, dy_ * scale);
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
    if (dx_ == 0.0) {
      ddx = 0.0;
      ddy = scale * spacing;
      if (dy_ < 0)
	ddy = -ddy;
      /*
*/
    } else {
      /*
*/
      theta = atan(fabs(dy_ / dx_));

      ddx = scale * spacing * cos(theta);
      ddy = scale * spacing * sin(theta);


      if (dx_ < 0)
	ddx = -ddx;
      if (dy_ < 0)
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

    dr = sqrt(dx_ * dx_ + dy_ * dy_);
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

  picxglobal += dx_;
  picyglobal += dy_;
}



Static Void mover(afile, dx_, dy_)
_TEXT *afile;
double dx_, dy_;
{
  drawr(afile, dx_, dy_, 'i', 0.0);
}



Static Void liner(afile, dx_, dy_)
_TEXT *afile;
double dx_, dy_;
{
  drawr(afile, dx_, dy_, 'l', 0.0);
}




Static Void drawa(afile, x, y, visibility, spacing)
_TEXT *afile;
double x, y;
Char visibility;
double spacing;
{
  /*





*/
  double dx_, dy_;

  /*
*/
  dx_ = x - picxglobal;
  dy_ = y - picyglobal;

  drawr(afile, dx_, dy_, visibility, spacing);
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
    FORLIM = s->length - 2;
    for (i = 1; i <= FORLIM; i++)
      putc(s->letters[i], tofile->f);
  } else {
    FORLIM = s->length;
    for (i = 0; i < FORLIM; i++) {
      if (skipping) {
	if (s->letters[i] != ' ') {
	  skipping = false;
	  putc(s->letters[i], tofile->f);
	}

      } else
	putc(s->letters[i], tofile->f);
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



Static Void picnumber(afile, dx_, dy_, number, width, decimal, justification)
_TEXT *afile;
double dx_, dy_, number;
long width, decimal;
Char justification;
{
  /*





*/
  stringDelila name;

  if (width <= 0)
    return;
  mover(afile, dx_, dy_);

  clearstring(&name);

  if (decimal > 0)
    stringreal(number, &name, width, decimal);
  else
    stringinteger((long)floor(number + 0.5), &name, width, false);

  graphstring(afile, &name, justification);
  mover(afile, -dx_, -dy_);
}



Static Void xtic(afile, length, dx_, dy_, number, width, decimal, logxnormal,
		 logxbase)
_TEXT *afile;
double length, dx_, dy_, number;
long width, decimal;
boolean logxnormal;
double logxbase;
{
  /*






*/
  liner(afile, 0.0, -length);
  if (logxnormal)
    picnumber(afile, dx_, dy_, exp(number * logxbase), width, decimal, 'c');
  else
    picnumber(afile, dx_, dy_, number, width, decimal, 'c');
  mover(afile, 0.0, length);
}



Static Void ytic(afile, length, dx_, dy_, number, width, decimal, logynormal,
		 logybase)
_TEXT *afile;
double length, dx_, dy_, number;
long width, decimal;
boolean logynormal;
double logybase;
{
  /*






*/
  liner(afile, -length, 0.0);


  if (logynormal)
    number = exp(number * logybase);

  picnumber(afile, dx_, dy_, number, width, decimal, 'r');
  mover(afile, length, 0.0);
}



Static Void doaxis(afile, theaxis, doaxisline, alength, fromtic, interval,
		   totic, subintervals, length, dx_, dy_, width, decimal,
		   logscale, lognormal, logbase)
_TEXT *afile;
Char theaxis;
boolean doaxisline;
double alength, fromtic, interval, totic, subintervals, length, dx_, dy_;
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
      if (tic <= totic) {
	if (theaxis == 'x')
	  xtic(afile, length, dx_, dy_, tic, width, decimal, lognormal,
	       logbase);
	else
	  ytic(afile, length, dx_, dy_, tic, width, decimal, lognormal,
	       logbase);
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
		xtic(afile, length / 2, dx_, dy_, 0.0, 0L, 0L, lognormal,
		     logbase);
		mover(afile, subjump, 0.0);
	      } else {
		ytic(afile, length / 2, dx_, dy_, 0.0, 0L, 0L, lognormal,
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
		  xtic(afile, length / 2, dx_, dy_, 0.0, 0L, 0L, lognormal,
		       logbase);
	      } else {
		mover(afile, 0.0, subjump);
		if (subtic <= totic)
		  ytic(afile, length / 2, dx_, dy_, 0.0, 0L, 0L, lognormal,
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
	xtic(afile, length, dx_, dy_, tic, width, decimal, lognormal, logbase);
      else
	ytic(afile, length, dx_, dy_, tic, width, decimal, lognormal, logbase);

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
		  xsubintervals, length, dx_, dy_, width, decimal, logxscale,
		  logxnormal, logxbase)
_TEXT *afile;
boolean doaxisline;
double axlength, fromtic, interval, totic, xsubintervals, length, dx_, dy_;
long width, decimal;
boolean logxscale, logxnormal;
double logxbase;
{
  doaxis(afile, 'x', doaxisline, axlength, fromtic, interval, totic,
	 xsubintervals, length, dx_, dy_, width, decimal, logxscale,
	 logxnormal, logxbase);
}



Static Void yaxis(afile, doaxisline, aylength, fromtic, interval, totic,
		  ysubintervals, length, dx_, dy_, width, decimal, logyscale,
		  logynormal, logybase)
_TEXT *afile;
boolean doaxisline;
double aylength, fromtic, interval, totic, ysubintervals, length, dx_, dy_;
long width, decimal;
boolean logyscale, logynormal;
double logybase;
{
  doaxis(afile, 'y', doaxisline, aylength, fromtic, interval, totic,
	 ysubintervals, length, dx_, dy_, width, decimal, logyscale,
	 logynormal, logybase);
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


/* Local variables for getdata: */
struct LOC_getdata {
  _TEXT *histog;
} ;

Local Void t(LINK)
struct LOC_getdata *LINK;
{
  if (BUFEOF(LINK->histog->f)) {
    printf("missing lines of histog\n");
    halt();
  }
}





Static Void getdata(histog_, data)
_TEXT *histog_;
histdata *data;
{
  struct LOC_getdata V;
  Char ch;
  boolean done = false;
  trigger s;

  V.histog = histog_;
  if (*V.histog->name != '\0') {
    if (V.histog->f != NULL)
      V.histog->f = freopen(V.histog->name, "r", V.histog->f);
    else
      V.histog->f = fopen(V.histog->name, "r");
  } else
    rewind(V.histog->f);
  if (V.histog->f == NULL)
    _EscIO2(FileNotFound, V.histog->name);
  RESETBUF(V.histog->f, Char);


  filltrigger(&s, "parameters:                                       ");
  resettrigger(&s);

  while (!done) {
    if (BUFEOF(V.histog->f))
      t(&V);
    testfortrigger(P_peek(V.histog->f), &s);
    if (P_eoln(V.histog->f)) {
      fscanf(V.histog->f, "%*[^\n]");
      getc(V.histog->f);
    } else
      getc(V.histog->f);
    if (s.found)
      done = true;
  }

  fscanf(V.histog->f, "%*[^\n]");

  getc(V.histog->f);
  t(&V);
  fscanf(V.histog->f, "%c%ld%*[^\n]", &ch, &data->column);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%ld%*[^\n]", &ch, &data->entries);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->minimum);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->maximum);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->mean);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->stdev);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->sem);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->variance);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->uncertainty);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->computeduncertainty);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%*[^\n]");
  getc(V.histog->f);
  t(&V);
  fscanf(V.histog->f, "%c%lg", &ch, &data->start_);
  if (ch == '\n')
    ch = ' ';
  skipblanks(V.histog);
  skipnonblanks(V.histog);
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->stop_);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->xinterval);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%ld%*[^\n]", &ch, &data->slots);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%ld%*[^\n]", &ch, &data->ymaximum);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%c%lg%*[^\n]", &ch, &data->yaxisscale);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  t(&V);
  fscanf(V.histog->f, "%*[^\n]");
  getc(V.histog->f);
  t(&V);
  fscanf(V.histog->f, "%c%c%c%*[^\n]", &ch, &ch, &data->plot);
  getc(V.histog->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == '\n')
    ch = ' ';
  if (data->plot == '\n')
    data->plot = ' ';
}



Static Void putdata(f, data)
_TEXT *f;
histdata *data;
{
  fprintf(f->f, "%% %10ld column\n", data->column);
  fprintf(f->f, "%% %10ld entries\n", data->entries);
  fprintf(f->f, "%% %10.5f minimum\n", data->minimum);
  fprintf(f->f, "%% %10.5f maximum\n", data->maximum);
  fprintf(f->f, "%% %10.5f mean\n", data->mean);
  fprintf(f->f, "%% %10.5f st dev\n", data->stdev);
  fprintf(f->f, "%% %10.5f SEM\n", data->sem);
  fprintf(f->f, "%% %10.5f variance\n", data->variance);
  fprintf(f->f, "%% %10.5f uncertainty\n", data->uncertainty);
  fprintf(f->f, "%% %10.5f computeduncertainty\n\n",
	  data->computeduncertainty);
  fprintf(f->f, "%% %10.5f start\n", data->start_);
  fprintf(f->f, "%% %10.5f stop\n", data->stop_);
  fprintf(f->f, "%% %10.5f x interval\n", data->xinterval);
  fprintf(f->f, "%% %10ld slots\n", data->slots);
  fprintf(f->f, "%% %10ld y maximum\n", data->ymaximum);
  fprintf(f->f, "%% %10.5f y axis scale\n\n", data->yaxisscale);
  fprintf(f->f, "%% %c%c plot\n", data->plot, data->plot);

  /*
















*/
}



Static Void comment(f)
_TEXT *f;
{
  fprintf(f->f, "%% ");
}



Static Void copystart(histog, picout, boxwidth, downshift)
_TEXT *histog, *picout;
double boxwidth, *downshift;
{
  /*

*/
  boolean done = false, gotten = true;
  stringDelila line;
  long lines;
  trigger t;

  if (*histog->name != '\0') {
    if (histog->f != NULL)
      histog->f = freopen(histog->name, "r", histog->f);
    else
      histog->f = fopen(histog->name, "r");
  } else
    rewind(histog->f);
  if (histog->f == NULL)
    _EscIO2(FileNotFound, histog->name);
  RESETBUF(histog->f, Char);


  /*

*/

  filltrigger(&t, "beginning | number                                ");
  resettrigger(&t);


  if (parameterversion <= bugversion226)
    lines = 4;
  else
    lines = 0;

  mover(picout, 0.0, -lines * boxwidth);

  while (gotten && !done) {
    getstring(histog, &line, &gotten);
    if (!gotten)
      break;

    /*
*/

    /*







*/
    if (parameterversion <= bugversion226)
      mover(picout, 0.0, -boxwidth);

    while (line.current <= line.length && !done) {
      testfortrigger(line.letters[line.current - 1], &t);
      if (t.found)
	done = true;
      line.current++;
    }
    lines++;
  }
  if (parameterversion <= bugversion226)
    *downshift = lines * boxwidth;
  else
    *downshift = 0 * boxwidth;

  /*



*/


  if (!BUFEOF(histog->f)) {
    fscanf(histog->f, "%*[^\n]");
    getc(histog->f);
  } else {
    printf("missing data in histog file\n");
    /*

*/
  }
}



Static Void readvalue(histog, b, v)
_TEXT *histog;
double *b, *v;
{
  /*
*/
  fscanf(histog->f, "%lg%lg%*[^\n]", b, v);
  getc(histog->f);
}



Static Void copyfile(fin, fout)
_TEXT *fin, *fout;
{
  while (!BUFEOF(fin->f))
    copyaline(fin, fout);
}


#define copylines       14



Static Void upgradeto226(genpicp)
_TEXT *genpicp;
{
  /*


*/
  _TEXT internal, titlesize;
  long line;
  double parameterversion = 2.26;

  titlesize.f = NULL;
  *titlesize.name = '\0';
  internal.f = NULL;
  *internal.name = '\0';
  printf("upgrading to version %4.2f ...\n", parameterversion);
  /*





































*/


  if (*genpicp->name != '\0') {
    if (genpicp->f != NULL)
      genpicp->f = freopen(genpicp->name, "r", genpicp->f);
    else
      genpicp->f = fopen(genpicp->name, "r");
  } else
    rewind(genpicp->f);
  if (genpicp->f == NULL)
    _EscIO2(FileNotFound, genpicp->name);
  RESETBUF(genpicp->f, Char);
  fscanf(genpicp->f, "%*[^\n]");
  getc(genpicp->f);
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
  for (line = 2; line <= copylines; line++)
    copyaline(genpicp, &internal);


  if (*titlesize.name != '\0') {
    if (titlesize.f != NULL)
      titlesize.f = freopen(titlesize.name, "w", titlesize.f);
    else
      titlesize.f = fopen(titlesize.name, "w");
  } else {
    if (titlesize.f != NULL)
      rewind(titlesize.f);
    else
      titlesize.f = tmpfile();
  }
  if (titlesize.f == NULL)
    _EscIO2(FileNotFound, titlesize.name);
  SETUPBUF(titlesize.f, Char);
  copyaline(genpicp, &titlesize);

  if (*titlesize.name != '\0') {
    if (titlesize.f != NULL)
      titlesize.f = freopen(titlesize.name, "r", titlesize.f);
    else
      titlesize.f = fopen(titlesize.name, "r");
  } else
    rewind(titlesize.f);
  if (titlesize.f == NULL)
    _EscIO2(FileNotFound, titlesize.name);
  RESETBUF(titlesize.f, Char);
  copyaline(&titlesize, &internal);


  for (line = 16; line <= 18; line++)
    copyaline(genpicp, &internal);


  if (*titlesize.name != '\0') {
    if (titlesize.f != NULL)
      titlesize.f = freopen(titlesize.name, "r", titlesize.f);
    else
      titlesize.f = fopen(titlesize.name, "r");
  } else
    rewind(titlesize.f);
  if (titlesize.f == NULL)
    _EscIO2(FileNotFound, titlesize.name);
  RESETBUF(titlesize.f, Char);
  copyaline(&titlesize, &internal);


  copyfile(genpicp, &internal);


  fprintf(internal.f,
    "n 0 0 0 0   edgecontrol (p=page), edgeleft, edgeright, edgelow, edgehigh in cm\n");


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
  if (*genpicp->name != '\0') {
    if (genpicp->f != NULL)
      genpicp->f = freopen(genpicp->name, "w", genpicp->f);
    else
      genpicp->f = fopen(genpicp->name, "w");
  } else {
    if (genpicp->f != NULL)
      rewind(genpicp->f);
    else
      genpicp->f = tmpfile();
  }
  if (genpicp->f == NULL)
    _EscIO2(FileNotFound, genpicp->name);
  SETUPBUF(genpicp->f, Char);
  fprintf(genpicp->f,
    "%4.2f  version of genpicp that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, genpicp);




  if (*genpicp->name != '\0') {
    if (genpicp->f != NULL)
      genpicp->f = freopen(genpicp->name, "r", genpicp->f);
    else
      genpicp->f = fopen(genpicp->name, "r");
  } else
    rewind(genpicp->f);
  if (genpicp->f == NULL)
    _EscIO2(FileNotFound, genpicp->name);
  RESETBUF(genpicp->f, Char);

  if (internal.f != NULL)
    fclose(internal.f);
  if (titlesize.f != NULL)
    fclose(titlesize.f);
}

#undef copylines


#define copylines       14



Static Void upgradeto250(genpicp)
_TEXT *genpicp;
{
  /*

*/
  _TEXT internal, titlesize;
  long line;
  double parameterversion = 2.50;

  titlesize.f = NULL;
  *titlesize.name = '\0';
  internal.f = NULL;
  *internal.name = '\0';
  printf("upgrading to version %4.2f ...\n", parameterversion);
  /*





































*/


  if (*genpicp->name != '\0') {
    if (genpicp->f != NULL)
      genpicp->f = freopen(genpicp->name, "r", genpicp->f);
    else
      genpicp->f = fopen(genpicp->name, "r");
  } else
    rewind(genpicp->f);
  if (genpicp->f == NULL)
    _EscIO2(FileNotFound, genpicp->name);
  RESETBUF(genpicp->f, Char);
  fscanf(genpicp->f, "%*[^\n]");
  getc(genpicp->f);
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
  for (line = 2; line <= copylines; line++)
    copyaline(genpicp, &internal);


  if (*titlesize.name != '\0') {
    if (titlesize.f != NULL)
      titlesize.f = freopen(titlesize.name, "w", titlesize.f);
    else
      titlesize.f = fopen(titlesize.name, "w");
  } else {
    if (titlesize.f != NULL)
      rewind(titlesize.f);
    else
      titlesize.f = tmpfile();
  }
  if (titlesize.f == NULL)
    _EscIO2(FileNotFound, titlesize.name);
  SETUPBUF(titlesize.f, Char);
  copyaline(genpicp, &titlesize);

  if (*titlesize.name != '\0') {
    if (titlesize.f != NULL)
      titlesize.f = freopen(titlesize.name, "r", titlesize.f);
    else
      titlesize.f = fopen(titlesize.name, "r");
  } else
    rewind(titlesize.f);
  if (titlesize.f == NULL)
    _EscIO2(FileNotFound, titlesize.name);
  RESETBUF(titlesize.f, Char);
  copyaline(&titlesize, &internal);


  for (line = 16; line <= 18; line++)
    copyaline(genpicp, &internal);


  copyfile(genpicp, &internal);


  fprintf(internal.f,
    "x 17 5 5 0.1 -0.00 -0.35 xaxis: control max int subint ticlength ticdx ticdy\n");
  fprintf(internal.f,
    "y 15 5 5 0.1 -0.12 -0.12 yaxis: control max int subint ticlength ticdx ticdy\n");


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
  if (*genpicp->name != '\0') {
    if (genpicp->f != NULL)
      genpicp->f = freopen(genpicp->name, "w", genpicp->f);
    else
      genpicp->f = fopen(genpicp->name, "w");
  } else {
    if (genpicp->f != NULL)
      rewind(genpicp->f);
    else
      genpicp->f = tmpfile();
  }
  if (genpicp->f == NULL)
    _EscIO2(FileNotFound, genpicp->name);
  SETUPBUF(genpicp->f, Char);
  fprintf(genpicp->f,
    "%4.2f  version of genpicp that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, genpicp);




  if (*genpicp->name != '\0') {
    if (genpicp->f != NULL)
      genpicp->f = freopen(genpicp->name, "r", genpicp->f);
    else
      genpicp->f = fopen(genpicp->name, "r");
  } else
    rewind(genpicp->f);
  if (genpicp->f == NULL)
    _EscIO2(FileNotFound, genpicp->name);
  RESETBUF(genpicp->f, Char);

  if (internal.f != NULL)
    fclose(internal.f);
  if (titlesize.f != NULL)
    fclose(titlesize.f);
}

#undef copylines



Static Void writetitle(afile, t)
_TEXT *afile;
title *t;
{
  if (t == NULL)
    return;
  fprintf(afile->f, "%% %ld size in points\n", t->titlenamesize);
  fprintf(afile->f, "%% %*.*f relative x adjustment\n",
	  difield, didecimal, t->titlex);
  fprintf(afile->f, "%% %*.*f relative y adjustment\n",
	  difield, didecimal, t->titley);
  fprintf(afile->f, "%% \"");
  writestring(afile, &t->letters);
  fprintf(afile->f, "\"\n");
}



Static Void readtitle(genpicp, t)
_TEXT *genpicp;
title **t;
{
  boolean gotten;
  title *WITH;

  WITH = *t;
  /*


*/
  fscanf(genpicp->f, "%ld%*[^\n]", &WITH->titlenamesize);
  getc(genpicp->f);
  fscanf(genpicp->f, "%lg%*[^\n]", &WITH->titlex);
  getc(genpicp->f);
  fscanf(genpicp->f, "%lg%*[^\n]", &WITH->titley);
  getc(genpicp->f);
  getstring(genpicp, &WITH->letters, &gotten);
  if (!gotten) {
    printf("could not find title while reading genpicp\n");
    halt();
  }
}


/* Local variables for readparam: */
struct LOC_readparam {
  _TEXT *afile;
  boolean checkout;
  /*

*/
  long paramnumber;
} ;

Local Void cn(LINK)
struct LOC_readparam *LINK;
{
  LINK->checkout = checknumber(LINK->afile);
  if (!LINK->checkout)
    halt();
}

Local Void bomb(LINK)
struct LOC_readparam *LINK;
{
  LINK->paramnumber++;
  if (BUFEOF(LINK->afile->f)) {
    printf("genpic: readparam: missing parameter number%ld\n",
	   LINK->paramnumber);
    halt();
  }
}



Static Void readparam(afile_, x, y, rotation, graphheight, boxwidth,
  intervalsize, histogramvalue, boxshift, ifield, idecimal, nfield, modulo,
  modulomultiplier, barlocation, title1, title2, title3, othertitles, titles,
  edgecontrol, edgeleft, edgeright, edgelow, edgehigh, xaxiscontrol, xaxismax,
  xaxisintervals, xaxissubintervals, xaxisticlength, xaxisticdx, xaxisticdy,
  yaxiscontrol, yaxismax, yaxisintervals, yaxissubintervals, yaxisticlength,
  yaxisticdx, yaxisticdy, dobugreport)
/* p2c: genpic.p, line 2454: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 2726 [251] */
_TEXT *afile_;
double *x, *y, *rotation, *graphheight, *boxwidth, *intervalsize,
       *histogramvalue, *boxshift;
long *ifield, *idecimal, *nfield, *modulo;
double *modulomultiplier;
long *barlocation;
title **title1, **title2, **title3;
long *othertitles;
title **titles;
Char *edgecontrol;
double *edgeleft, *edgeright, *edgelow, *edgehigh;
Char *xaxiscontrol;
long *xaxismax, *xaxisintervals, *xaxissubintervals;
double *xaxisticlength, *xaxisticdx, *xaxisticdy;
Char *yaxiscontrol;
long *yaxismax, *yaxisintervals, *yaxissubintervals;
double *yaxisticlength, *yaxisticdx, *yaxisticdy;
boolean *dobugreport;
{



  /*
*/


  /*
*/

  /*
*/
  struct LOC_readparam V;
  long o;
  title *t;
  long FORLIM;

  V.afile = afile_;
  if (*V.afile->name != '\0') {
    if (V.afile->f != NULL)
      V.afile->f = freopen(V.afile->name, "r", V.afile->f);
    else
      V.afile->f = fopen(V.afile->name, "r");
  } else
    rewind(V.afile->f);
  if (V.afile->f == NULL)
    _EscIO2(FileNotFound, V.afile->name);
  RESETBUF(V.afile->f, Char);

  *dobugreport = false;

  if (BUFEOF(V.afile->f)) {
    *x = dx;
    *y = dy;
    *rotation = drotation;
    *graphheight = dgraphheight;
    *boxwidth = dboxwidth;
    *intervalsize = dintervalsize;
    *histogramvalue = dhistogramvalue;
    *boxshift = dboxshift;
    *ifield = difield;
    *idecimal = didecimal;
    *nfield = dnfield;
    *modulo = dmodulo;
    *modulomultiplier = dmodulomultiplier;
    *barlocation = 0;
    return;
  }
  fscanf(V.afile->f, "%lg%*[^\n]", &parameterversion);
  getc(V.afile->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    printf("You have an old parameter file!\n");
    printf("parameterversion is %4.2f\n", parameterversion);
    printf("   updateversion is %4.2f\n", updateversion);

    if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100 * 2.26 + 0.5))
      upgradeto226(&genpicp);
    else {
      if (*V.afile->name != '\0') {
	if (V.afile->f != NULL)
	  V.afile->f = freopen(V.afile->name, "r", V.afile->f);
	else
	  V.afile->f = fopen(V.afile->name, "r");
      } else
	rewind(V.afile->f);
      if (V.afile->f == NULL)
	_EscIO2(FileNotFound, V.afile->name);
      RESETBUF(V.afile->f, Char);
    }

    if ((long)floor(100 * parameterversion + 0.5) <=
	(long)floor(100 * bugversion226 + 0.5)) {
      printf("******************************************\n");
      printf("*********** WARNING **********************\n");
      printf("******************************************\n");
      printf("* You have an old parameter file, with a *\n");
      printf("* version <= 2.26, that has a bug in it. *\n");
      printf("* To fix the bug,                        *\n");
      printf("* update the version number to:          *\n");
      printf("*      2.32                              *\n");
      printf("* Then you will find that your graph     *\n");
      printf("* shifts to the left (negative x value). *\n");
      printf("* Correct this by hand.                  *\n");
      /*

*/
      printf("******************************************\n");

      *dobugreport = true;

    }


    if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100 * 2.50 + 0.5))
      upgradeto250(&genpicp);
    else {
      if (*V.afile->name != '\0') {
	if (V.afile->f != NULL)
	  V.afile->f = freopen(V.afile->name, "r", V.afile->f);
	else
	  V.afile->f = fopen(V.afile->name, "r");
      } else
	rewind(V.afile->f);
      if (V.afile->f == NULL)
	_EscIO2(FileNotFound, V.afile->name);
      RESETBUF(V.afile->f, Char);
    }

    fscanf(V.afile->f, "%lg%*[^\n]", &parameterversion);
    getc(V.afile->f);
  }

  V.paramnumber = 1;

  bomb(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", x);
  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", y);
  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", rotation);
  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", graphheight);
  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", boxwidth);
  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", intervalsize);
  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", histogramvalue);
  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", boxshift);
  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%ld%*[^\n]", ifield);
  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%ld%*[^\n]", idecimal);
  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%ld%*[^\n]", nfield);
  getc(V.afile->f);

  bomb(&V);
  /*

*/
  fscanf(V.afile->f, "%ld", modulo);
  skipblanks(V.afile);
  if (!P_eoln(V.afile->f)) {
    if (P_peek(V.afile->f) == '9' || P_peek(V.afile->f) == '8' ||
	P_peek(V.afile->f) == '7' || P_peek(V.afile->f) == '6' ||
	P_peek(V.afile->f) == '5' || P_peek(V.afile->f) == '4' ||
	P_peek(V.afile->f) == '3' || P_peek(V.afile->f) == '2' ||
	P_peek(V.afile->f) == '1' || P_peek(V.afile->f) == '0')
      fscanf(V.afile->f, "%lg", modulomultiplier);
    else
      *modulomultiplier = dmodulomultiplier;
  } else
    *modulomultiplier = dmodulomultiplier;
  fscanf(V.afile->f, "%*[^\n]");
  /*

*/

  getc(V.afile->f);
  bomb(&V);
  fscanf(V.afile->f, "%ld%*[^\n]", barlocation);
  getc(V.afile->f);

  *title1 = (title *)Malloc(sizeof(title));
  readtitle(&genpicp, title1);
  (*title1)->next = NULL;
  *title2 = (title *)Malloc(sizeof(title));
  readtitle(&genpicp, title2);
  (*title2)->next = NULL;
  *title3 = (title *)Malloc(sizeof(title));
  readtitle(&genpicp, title3);
  (*title3)->next = NULL;


  fscanf(genpicp.f, "%ld%*[^\n]", othertitles);
  getc(genpicp.f);
  printf("reading in %ld other titles\n", *othertitles);

  if (*othertitles > 0) {
    *titles = (title *)Malloc(sizeof(title));
    t = *titles;
    FORLIM = *othertitles;
    for (o = 1; o <= FORLIM; o++) {
      if (BUFEOF(V.afile->f)) {
	printf("found end of genpicp when reading in titles\n");
	halt();
      }
      if (o > 1) {
	t->next = (title *)Malloc(sizeof(title));
	t = t->next;
	t->next = NULL;
      }
      readtitle(&genpicp, &t);
    }
  } else
    *titles = NULL;
  *edgecontrol = getc(genpicp.f);
  if (*edgecontrol == '\n') {

    *edgecontrol = ' ';
  }
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%lg", edgeleft);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%lg", edgeright);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%lg", edgelow);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%lg", edgehigh);
  fscanf(genpicp.f, "%*[^\n]");

  getc(genpicp.f);
  *xaxiscontrol = getc(genpicp.f);
  if (*xaxiscontrol == '\n')
    *xaxiscontrol = ' ';
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%ld", xaxismax);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%ld", xaxisintervals);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%ld", xaxissubintervals);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%lg", xaxisticlength);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%lg", xaxisticdx);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%lg", xaxisticdy);
  fscanf(genpicp.f, "%*[^\n]");

  getc(genpicp.f);
  *yaxiscontrol = getc(genpicp.f);
  if (*yaxiscontrol == '\n')
    *yaxiscontrol = ' ';
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%ld", yaxismax);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%ld", yaxisintervals);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%ld", yaxissubintervals);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%lg", yaxisticlength);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%lg", yaxisticdx);
  bomb(&V);
  cn(&V);
  fscanf(genpicp.f, "%lg", yaxisticdy);
  fscanf(genpicp.f, "%*[^\n]");

  getc(genpicp.f);
  if (*yaxismax <= 0) {
    printf("yaxismax must be positive\n");
    halt();
  }

}



Static Void writeparam(afile, fc, x, y, rotation, graphheight, boxwidth,
  intervalsize, histogramvalue, boxshift, ifield, idecimal, nfield, modulo,
  modulomultiplier, barlocation, title1, title2, title3, othertitles, titles,
  edgecontrol, edgeleft, edgeright, edgelow, edgehigh, xaxiscontrol, xaxismax,
  xaxisintervals, xaxissubintervals, xaxisticlength, xaxisticdx, xaxisticdy,
  yaxiscontrol, yaxismax, yaxisintervals, yaxissubintervals, yaxisticlength,
  yaxisticdx, yaxisticdy)
/* p2c: genpic.p, line 2537: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 3040 [251] */
_TEXT *afile;
Char fc;
double x, y, rotation, graphheight, boxwidth, intervalsize, histogramvalue,
       boxshift;
long ifield, idecimal, nfield, modulo;
double modulomultiplier;
long barlocation;
title *title1, *title2, *title3;
long othertitles;
title *titles;
Char edgecontrol;
double edgeleft, edgeright, edgelow, edgehigh;
Char xaxiscontrol;
long xaxismax, xaxisintervals, xaxissubintervals;
double *xaxisticlength, *xaxisticdx, *xaxisticdy;
Char yaxiscontrol;
long yaxismax, yaxisintervals, yaxissubintervals;
double *yaxisticlength, *yaxisticdx, *yaxisticdy;
{


  /*
*/


  /*
*/

  /*
*/
  long o;
  title *t;

  fprintf(afile->f, "%c the parameters used are:\n", fc);
  fprintf(afile->f, "%c%10.5f x\n", fc, x);
  fprintf(afile->f, "%c%10.5f y\n", fc, y);
  fprintf(afile->f, "%c%10.5f rotation\n", fc, rotation);
  fprintf(afile->f, "%c%10.5f graphheight\n", fc, graphheight);
  fprintf(afile->f, "%c%10.5f boxwidth\n", fc, boxwidth);
  fprintf(afile->f, "%c%10.5f interval size\n", fc, intervalsize);
  fprintf(afile->f, "%c%10.5f histogram value\n", fc, histogramvalue);
  fprintf(afile->f, "%c%10.5f box shift\n", fc, boxshift);
  fprintf(afile->f, "%c%5ld field width of interval numbers\n", fc, ifield);
  fprintf(afile->f, "%c%5ld decimals of interval numbers\n", fc, idecimal);
  fprintf(afile->f, "%c%5ld field width of number data column\n", fc, nfield);
  fprintf(afile->f,
	  "%c%5ld %5.1f numbers modulo this are plotted and multiplier\n",
	  fc, modulo, modulomultiplier);
  fprintf(afile->f, "%c%5ld barlocation: where to put a vertical bar\n",
	  fc, barlocation);
  fprintf(afile->f, "%c%5ld number of other titles\n", fc, othertitles);


  writetitle(afile, title1);
  writetitle(afile, title2);
  writetitle(afile, title3);

  t = titles;
  for (o = 1; o <= othertitles; o++) {
    writetitle(afile, t);
    t = t->next;
  }

  fprintf(afile->f,
    "%c %c %1.3f %1.3f %1.3f %1.3f edgecontrol (p=page), edgeleft, edgeright, edgelow, edgehigh in cm\n",
    fc, edgecontrol, edgeleft, edgeright, edgelow, edgehigh);

  fprintf(afile->f,
    "%c %c %ld %ld %ld xaxiscontrol (x=axis), xaxismax, xaxisintervals\n",
    fc, xaxiscontrol, xaxismax, xaxisintervals, xaxissubintervals);

  fprintf(afile->f,
    "%c %c %ld %ld %ld yaxiscontrol (y=axis), yaxismax, yaxisintervals\n",
    fc, yaxiscontrol, yaxismax, yaxisintervals, yaxissubintervals);

}


/* Local variables for buildpic: */
struct LOC_buildpic {
  _TEXT *picout;
  double halfsmallbox;
  /*
*/
  double smallbox;
} ;

Local Void dosmallbox(LINK)
struct LOC_buildpic *LINK;
{
  mover(LINK->picout, -LINK->halfsmallbox, -LINK->halfsmallbox);
  boxr(LINK->picout, LINK->smallbox, LINK->smallbox);
  mover(LINK->picout, LINK->halfsmallbox, LINK->halfsmallbox);
}



Static Void buildpic(hdata, histog, picout_, graphheight, boxwidth,
		     intervalsize, histogramvalue, boxshift, ifield, idecimal,
		     nfield, modulo, modulomultiplier, barlocation, downshift,
		     title1, title2, title3, othertitles, titles,
		     xaxiscontrol, yaxiscontrol)
histdata hdata;
_TEXT *histog, *picout_;
double graphheight, boxwidth, intervalsize, histogramvalue, boxshift;
long ifield, idecimal, nfield, modulo;
double modulomultiplier;
long barlocation;
double downshift;
title *title1, *title2, *title3;
long othertitles;
title *titles;
Char xaxiscontrol, yaxiscontrol;
{
  /*
*/
  struct LOC_buildpic V;
  double b;
  long index = 0;
  long o;
  double returnx;
  /*
*/
  double returny;
  /*
*/
  double scale;
  rhistarray stanarray;

  double v, scalev, shiftup;
  /*
*/
  title *t, *WITH;


  V.picout = picout_;
  if (hdata.plot != 'n') {
    stanarray.range[(long)start] = hdata.start_;
    stanarray.range[(long)stop] = hdata.stop_;
    stanarray.interval = hdata.xinterval;
    stanarray.slots = hdata.slots;

    if (hdata.plot == 'g')
      gaushist(hdata.entries, hdata.mean, hdata.variance, &stanarray);
    else if (hdata.plot == 'p')
      poishist(hdata.entries, hdata.mean, &stanarray);
    else {
      printf("pic: build: unknown plot type ,%c\n", hdata.plot);
      halt();
    }
  }


  returnx = -intervalsize - histogramvalue;
  returny = -boxwidth - boxshift;
  scale = graphheight / hdata.ymaximum;
  V.smallbox = boxwidth / 4;
  V.halfsmallbox = V.smallbox / 2;

  /*


*/
  while (!BUFEOF(histog->f)) {
    readvalue(histog, &b, &v);
    index++;


    if (xaxiscontrol != 'x') {
      if ((long)floor(modulomultiplier * b + 0.5) % modulo == 0)
	picnumber(V.picout, 0.0, 0.0, b, ifield, idecimal, 'r');
      /*






*/

      mover(V.picout, intervalsize, 0.0);
      if ((long)floor(modulomultiplier * b + 0.5) % modulo == 0)
	picnumber(V.picout, 0.0, 0.0, (double)((long)floor(v + 0.5)), nfield,
		  0L, 'r');
    }

    else
      mover(V.picout, intervalsize, 0.0);

    mover(V.picout, histogramvalue, boxshift);

    scalev = scale * v;

    if (b == barlocation) {
      if (hdata.plot != 'n') {
	if (stanarray.numbers[index-1] > v)
	  shiftup = scale * stanarray.numbers[index-1];
	else {
	  shiftup = scalev;
	  /*



*/
	}
      } else
	shiftup = scalev;
      shiftup += 4 * V.smallbox;
      mover(V.picout, shiftup, boxwidth / 2);
      liner(V.picout, graphheight - shiftup, 0.0);
      mover(V.picout, shiftup - graphheight, 0.0);
      mover(V.picout, -shiftup, boxwidth / -2);

      /*



*/

    }


    boxr(V.picout, scalev, boxwidth);

    mover(V.picout, returnx, returny);
  }


  mover(V.picout, returnx, returny);

  fprintf(V.picout->f, "%% now create the names\n");


  fprintf(V.picout->f, "gsave\n");
  fprintf(V.picout->f, " currentpoint translate\n");
  fprintf(V.picout->f, " -90 rotate\n");
  fprintf(V.picout->f, " gsave\n");
  fprintf(V.picout->f, "  /columnnamesize %ld def\n", title1->titlenamesize);
  fprintf(V.picout->f,
	  "  /Courier-Bold findfont columnnamesize scalefont setfont\n");
  fprintf(V.picout->f, "  0 %*.*f cm rmoveto\n",
	  picwidth, picdecim, intervalsize);
  fprintf(V.picout->f, "  /col1x %*.*f def\n",
	  picwidth, picdecim, title1->titlex);
  fprintf(V.picout->f, "  /col1y %*.*f def\n",
	  picwidth, picdecim, title1->titley);
  fprintf(V.picout->f, "  col1x cm col1y cm rmoveto\n");
  fprintf(V.picout->f, "  (");
  writestring(V.picout, &title1->letters);
  fprintf(V.picout->f, ") show\n");
  fprintf(V.picout->f, " grestore\n");


  fprintf(V.picout->f, " gsave\n");
  fprintf(V.picout->f, "  /columnnamesize %ld def\n", title2->titlenamesize);
  fprintf(V.picout->f,
	  "  /Courier-Bold findfont columnnamesize scalefont setfont\n");
  fprintf(V.picout->f, "  0 %*.*f cm rmoveto\n",
	  picwidth, picdecim, intervalsize);
  fprintf(V.picout->f, "  0 %*.*f cm rmoveto\n",
	  picwidth, picdecim, histogramvalue);
  fprintf(V.picout->f, "  /col2x %*.*f def\n",
	  picwidth, picdecim, title2->titlex);
  fprintf(V.picout->f, "  /col2y %*.*f def\n",
	  picwidth, picdecim, title2->titley);
  fprintf(V.picout->f, "  col2x cm col2y cm rmoveto\n");
  fprintf(V.picout->f, "  (");
  writestring(V.picout, &title2->letters);
  fprintf(V.picout->f, ") show\n");
  fprintf(V.picout->f, " grestore\n");
  fprintf(V.picout->f, "grestore\n");

  fprintf(V.picout->f, "gsave\n");

  fprintf(V.picout->f, " currentpoint translate\n");
  fprintf(V.picout->f, " -90 rotate\n");
  fprintf(V.picout->f, "%*.*f cm 0 cm rmoveto\n",
	  picwidth, picdecim, hdata.slots * boxwidth / -2);
  fprintf(V.picout->f, " /titlenamesize %3ld def\n", title3->titlenamesize);
  fprintf(V.picout->f, " /titlex %*.*f def\n",
	  picwidth, picdecim, title3->titlex);
  fprintf(V.picout->f, " /titley %*.*f def\n",
	  picwidth, picdecim, title3->titley);
  fprintf(V.picout->f, " titlex cm titley cm rmoveto\n");
  fprintf(V.picout->f,
	  " /Courier-Bold findfont titlenamesize scalefont setfont\n");
  fprintf(V.picout->f, " (");
  writestring(V.picout, &title3->letters);
  fprintf(V.picout->f, ") show\n");
  fprintf(V.picout->f, "grestore\n");

  if (othertitles > 0) {
    t = titles;
    for (o = 1; o <= othertitles; o++) {
      WITH = t;
      fprintf(V.picout->f, "gsave\n");
      fprintf(V.picout->f, " currentpoint translate\n");
      fprintf(V.picout->f, " -90 rotate\n");
      fprintf(V.picout->f, "%*.*f cm 0 cm rmoveto\n",
	      picwidth, picdecim, -(hdata.slots + 2) * boxwidth);
      fprintf(V.picout->f, " /titlenamesize %ld def\n", t->titlenamesize);
      fprintf(V.picout->f, " /titlex %*.*f def\n",
	      picwidth, picdecim, t->titlex);
      fprintf(V.picout->f, " /titley %*.*f def\n",
	      picwidth, picdecim, t->titley);
      fprintf(V.picout->f, " titlex cm titley cm rmoveto\n");
      fprintf(V.picout->f,
	      " /Courier-Bold findfont titlenamesize scalefont setfont\n");
      fprintf(V.picout->f, " (");
      writestring(V.picout, &t->letters);
      fprintf(V.picout->f, ") show\n");
      fprintf(V.picout->f, "grestore\n");
      t = t->next;
    }
    if (t != NULL) {
      printf("buildpic: extra titles on list not accounted for\n");
      halt();
    }
  }


  if (hdata.plot != 'n') {
    movea(V.picout, scale * stanarray.numbers[0] - returnx,
	  boxshift - downshift + boxwidth / 2);

    /*


*/

    dosmallbox(&V);
    for (index = 2; index <= hdata.slots; index++) {
      liner(V.picout,
	    scale * (stanarray.numbers[index-1] - stanarray.numbers[index-2]),
	    -boxwidth);
      dosmallbox(&V);
    }
  }




  stoppic(V.picout);
}



Static Void postscriptheader(a, title_, creator, llx, lly, urx, ury)
_TEXT *a;
stringDelila title_, creator;
double llx, lly, urx, ury;
{
  /*
*/
  fprintf(a->f, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(a->f, "%%%%Title: ");
  writestring(a, &title_);
  fprintf(a->f, " %4.2f\n", version);
  fprintf(a->f, "%%%%Creator: ");
  writestring(a, &creator);
  fprintf(a->f, "\n%%%%BoundingBox: %5ld %5ld %5ld %5ld\n",
	  (long)floor(llx + 0.5), (long)floor(lly + 0.5),
	  (long)floor(urx + 0.5), (long)floor(ury + 0.5));
  /*





*/
  fprintf(a->f, "%%%%DocumentFonts:\n");
  fprintf(a->f, "%%%%EndComments\n");

  /*



*/
  fprintf(a->f, "%%%%BeginDefaults\n");
  fprintf(a->f, "%%%%PageOrientation: Portrait\n");
  fprintf(a->f, "%%%%EndDefaults\n");
  fprintf(a->f, "%%%%Orientation: Portrait\n");

  fprintf(a->f, "/defaultllx %5.1f def\n", defaultllx);
  fprintf(a->f, "/defaultlly %5.1f def\n", defaultlly);
  fprintf(a->f, "/defaulturx %5.1f def\n", defaulturx);
  fprintf(a->f, "/defaultury %5.1f def\n", defaultury);
  fprintf(a->f, "/llx %5.1f def\n", llx);
  fprintf(a->f, "/lly %5.1f def\n", lly);
  fprintf(a->f, "/urx %5.1f def\n", urx);
  fprintf(a->f, "/ury %5.1f def\n", ury);
}



Static Void initpostscript(title_, creator, x, y, graphheight, boxwidth,
			   title1, title2, title3, llx, lly, urx, ury,
			   edgecontrol, edgeleft, edgeright, edgelow,
			   edgehigh)
stringDelila *title_, *creator;
double x, y, graphheight, boxwidth;
title *title1, *title2, *title3;
double *llx, *lly, *urx, *ury;
Char edgecontrol;
double edgeleft, edgeright, edgelow, edgehigh;
{
  /*
*/
  double cmfactor = 72 / 2.54;



  clearstring(title_);
  title_->letters[0] = 'g';
  title_->letters[1] = 'e';
  title_->letters[2] = 'n';
  title_->letters[3] = 'p';
  title_->letters[4] = 'i';
  title_->letters[5] = 'c';
  title_->length = 6;

  clearstring(creator);
  creator->letters[0] = 'T';
  creator->letters[1] = 'o';
  creator->letters[2] = 'm';
  creator->letters[3] = ' ';
  creator->letters[4] = 'S';
  creator->letters[5] = 'c';
  creator->letters[6] = 'h';
  creator->letters[7] = 'n';
  creator->letters[8] = 'e';
  creator->letters[9] = 'i';
  creator->letters[10] = 'd';
  creator->letters[11] = 'e';
  creator->letters[12] = 'r';
  creator->length = 13;


  if (edgecontrol == 'p') {
    /*
*/
    *llx = defaultllx;
    *ury = defaultury;
    *urx = defaulturx;
    *lly = defaultlly;
  } else {
    *llx = defaultllx;
    *ury = defaultury;
    *urx = defaulturx;
    *lly = defaultlly;

    /*



*/
    *llx = defaultllx;

    *lly = defaultlly +
	(y - title1->titley - title2->titley - title3->titley - 2) * cmfactor;
    /*



*/
    *urx = defaulturx;

    *ury = defaultlly + (y + graphheight + 2) * cmfactor;

  }


  *llx -= edgeleft * cmfactor;
  *urx += edgeright * cmfactor;
  *lly -= edgelow * cmfactor;
  *ury += edgehigh * cmfactor;

}


/* Local variables for themain: */
struct LOC_themain {
  double boxwidth, downshift, x;
} ;

/*
*/
Local Void bugreport(LINK)
struct LOC_themain *LINK;
{
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  printf("@ Here is how to fix the bug: @\n");
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

  printf("change your x coordinate from %1.4f cm", LINK->x);
  printf(" to %1.4f cm\n", LINK->x - LINK->downshift);

  printf("downshift: %8.2f\n", LINK->downshift);
  printf("boxwidth: %8.2f\n", LINK->boxwidth);
  printf("lines = downshift/boxwidth: %8.2f\n",
	 LINK->downshift / LINK->boxwidth);
}



Static Void themain(histog, genpicp, picout)
_TEXT *histog, *genpicp, *picout;
{
  struct LOC_themain V;
  histdata data;


  long barlocation;
  double graphheight, boxheight, intervalsize, histogramvalue, boxshift;
  /*
*/
  long ifield, idecimal, nfield, modulo;
  double modulomultiplier, rotation;
  stringDelila title_, creator;
  double y;

  title *title1, *title2, *title3;

  long othertitles;
  title *titles;


  double llx, lly, urx, ury;

  Char edgecontrol;
  double edgeleft, edgeright, edgelow, edgehigh;

  Char yaxiscontrol;
  long yaxismax, yaxisintervals, yaxissubintervals;
  double yaxisticlength, yaxisticdx, yaxisticdy;


  Char xaxiscontrol;
  long xaxismax, xaxisintervals, xaxissubintervals;
  double xaxisticlength, xaxisticdx, xaxisticdy;


  boolean dobugreport;
  _TEXT TEMP;

  printf(" genpic %4.2f\n", version);

  getdata(histog, &data);

  readparam(genpicp, &V.x, &y, &rotation, &graphheight, &V.boxwidth,
    &intervalsize, &histogramvalue, &boxshift, &ifield, &idecimal, &nfield,
    &modulo, &modulomultiplier, &barlocation, &title1, &title2, &title3,
    &othertitles, &titles, &edgecontrol, &edgeleft, &edgeright, &edgelow,
    &edgehigh, &xaxiscontrol, &xaxismax, &xaxisintervals, &xaxissubintervals,
    &xaxisticlength, &xaxisticdx, &xaxisticdy, &yaxiscontrol, &yaxismax,
    &yaxisintervals, &yaxissubintervals, &yaxisticlength, &yaxisticdx,
    &yaxisticdy, &dobugreport);
/* p2c: genpic.p, line 3223: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 3599 [251] */

  initpostscript(&title_, &creator, V.x, y, graphheight, V.boxwidth, title1,
		 title2, title3, &llx, &lly, &urx, &ury, edgecontrol,
		 edgeleft, edgeright, edgelow, edgehigh);

  if (*picout->name != '\0') {
    if (picout->f != NULL)
      picout->f = freopen(picout->name, "w", picout->f);
    else
      picout->f = fopen(picout->name, "w");
  } else {
    if (picout->f != NULL)
      rewind(picout->f);
    else
      picout->f = tmpfile();
  }
  if (picout->f == NULL)
    _EscIO2(FileNotFound, picout->name);
  SETUPBUF(picout->f, Char);
  postscriptheader(picout, title_, creator, llx, lly, urx, ury);

  writeparam(picout, '%', V.x, y, rotation, graphheight, V.boxwidth,
    intervalsize, histogramvalue, boxshift, ifield, idecimal, nfield, modulo,
    modulomultiplier, barlocation, title1, title2, title3, othertitles,
    titles, edgecontrol, edgeleft, edgeright, edgelow, edgehigh, xaxiscontrol,
    xaxismax, xaxisintervals, xaxissubintervals, &xaxisticlength, &xaxisticdx,
    &xaxisticdy, yaxiscontrol, yaxismax, yaxisintervals, yaxissubintervals,
    &yaxisticlength, &yaxisticdx, &yaxisticdy);
/* p2c: genpic.p, line 3223: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 3629 [251] */


  startpic(picout, defscale, V.x, y, 'c');

  fprintf(picout->f,
	  "/cmfactor 72 2.54 div def %% defines points -> centimeters\n");
  fprintf(picout->f, "/cm { cmfactor mul} def %% defines centimeters\n");

  fprintf(picout->f, "2 setlinecap\n");

  /*


*/

  if (idecimal == 0)
    fprintf(picout->f, "%*ld", (int)ifield, (long)floor(45 + rotation + 0.5));
  else
    fprintf(picout->f, "%*.*f", (int)ifield, (int)idecimal, 45 + rotation);
  fprintf(picout->f, " rotate\n");
  fprintf(picout->f, "45 rotate\n\n");


  fprintf(picout->f, "%%%%EndProlog\n");
  fprintf(picout->f, "%%%%Page: 1\n");

  putdata(picout, &data);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  putdata(&TEMP, &data);

  copystart(histog, picout, V.boxwidth, &V.downshift);

  /*







*/

  if (xaxiscontrol == 'x') {
    fprintf(picout->f, "gsave\n");
    fprintf(picout->f, "-90 rotate\n");

    fprintf(picout->f, "%*.*f cm %*.*f cm translate\n",
	    picwidth, picdecim, V.boxwidth / -2, picwidth, picdecim,
	    intervalsize + histogramvalue);
    fprintf(picout->f, "0 0 moveto\n");

    /*




*/

    /*









*/
    /*


































*/


    xaxis(picout, true,
	  (xaxismax - data.start_) / data.xinterval * V.boxwidth, data.start_,
	  (double)xaxisintervals, (double)xaxismax, (double)xaxissubintervals,
	  xaxisticlength, xaxisticdx, xaxisticdy, 20L, 0L, false, false, 2.0);

    fprintf(picout->f, "grestore\n");
  }



  if (yaxiscontrol == 'y') {
    fprintf(picout->f, "gsave\n");
    fprintf(picout->f, "-90 rotate\n");

    fprintf(picout->f, "%*.*f cm %*.*f cm translate\n",
	    picwidth, picdecim, -V.boxwidth, picwidth, picdecim,
	    intervalsize + histogramvalue);
    fprintf(picout->f, "0 0 moveto\n");


    yaxis(picout, true, graphheight, 0.0, (double)yaxisintervals,
	  (double)yaxismax, (double)yaxissubintervals, yaxisticlength,
	  yaxisticdx, yaxisticdy, 20L, 0L, false, false, 2.0);
    fprintf(picout->f, "grestore\n");
    boxheight = graphheight * ((double)data.ymaximum / yaxismax);
  }

  /*


















*/

  else
    boxheight = graphheight;

  /*



*/

  buildpic(data, histog, picout, boxheight, V.boxwidth, intervalsize,
	   histogramvalue, boxshift, ifield, idecimal, nfield, modulo,
	   modulomultiplier, barlocation, V.downshift, title1, title2, title3,
	   othertitles, titles, xaxiscontrol, yaxiscontrol);

  fprintf(picout->f, "%%%%Trailer\n");
  fprintf(picout->f, "%%%%Pages: 1\n");

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  picout.f = NULL;
  strcpy(picout.name, "picout");
  genpicp.f = NULL;
  strcpy(genpicp.name, "genpicp");
  histog.f = NULL;
  strcpy(histog.name, "histog");
  themain(&histog, &genpicp, &picout);
_L1:
  if (histog.f != NULL)
    fclose(histog.f);
  if (genpicp.f != NULL)
    fclose(genpicp.f);
  if (picout.f != NULL)
    fclose(picout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
