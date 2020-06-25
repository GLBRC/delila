/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "emin.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.36
/*

*/



/*





























*/



#define pi              3.14159265354

#define picwidth        8
#define picdecim        5

#define charwidth       0.0625
/*
*/
/*
*/
#define dotfactor       0.00625

#define defscale        72

/*
*/



#define maxstring       150



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



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



Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
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

  fprintf(afile->f, "%% initgraphics\n");
  /*

*/

  scale = setscale;
  fprintf(afile->f, "%% clear erasepage\n");

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
  long sign, thedecimal, theupper;

  if (number < 0)
    sign = -1;
  else
    sign = 1;

  number = fabs(number);


  shift = (long)floor(exp(decimal * log(10.0)) + 0.5);
  abovezero = (long)floor(number * shift + 0.5);
  theupper = (long)((double)abovezero / shift);
  thedecimal = abovezero - shift * theupper;



  stringinteger(sign * theupper, name, width - decimal - 1, false);
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



Static Void doaxis(afile, theaxis, alength, fromtic, interval, totic,
		   subintervals, length, dx, dy, width, decimal, logscale,
		   lognormal, logbase)
_TEXT *afile;
Char theaxis;
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


  /*




*/

  if (theaxis == 'x') {
    liner(afile, alength, 0.0);
    mover(afile, -alength, 0.0);
  } else {
    liner(afile, 0.0, alength);
    mover(afile, 0.0, -alength);
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

  tic = fromtic;
  if (interval > 0.0) {
    while (tic <= totic + half) {
      if (theaxis == 'x')
	xtic(afile, length, dx, dy, tic, width, decimal, lognormal, logbase);
      else
	ytic(afile, length, dx, dy, tic, width, decimal, lognormal, logbase);

      tic += interval;
      if (tic > totic + half)
	break;
      /*



*/
      if (!dosubtics) {
	if (theaxis == 'x')
	  mover(afile, jump, 0.0);
	else
	  mover(afile, 0.0, jump);
	jumpdistance += jump;
	continue;
      }
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
	    xtic(afile, length / 2, dx, dy, 0.0, 0L, 0L, lognormal, logbase);
	    mover(afile, subjump, 0.0);
	  } else {
	    ytic(afile, length / 2, dx, dy, 0.0, 0L, 0L, lognormal, logbase);
	    mover(afile, 0.0, subjump);
	  }

	  jumpdistance += subjump;
	}

	continue;
      }

      subtic = tic;
      while (subtic < tic + interval - halfsubinterval) {
	subtic += subinterval;

	if (theaxis == 'x') {
	  mover(afile, subjump, 0.0);
	  xtic(afile, length / 2, dx, dy, 0.0, 0L, 0L, lognormal, logbase);
	} else {
	  mover(afile, 0.0, subjump);
	  ytic(afile, length / 2, dx, dy, 0.0, 0L, 0L, lognormal, logbase);
	}

	jumpdistance += subjump;
      }
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
}



Static Void xaxis(afile, axlength, fromtic, interval, totic, xsubintervals,
		  length, dx, dy, width, decimal, logxscale, logxnormal,
		  logxbase)
_TEXT *afile;
double axlength, fromtic, interval, totic, xsubintervals, length, dx, dy;
long width, decimal;
boolean logxscale, logxnormal;
double logxbase;
{
  doaxis(afile, 'x', axlength, fromtic, interval, totic, xsubintervals,
	 length, dx, dy, width, decimal, logxscale, logxnormal, logxbase);
}



Static Void yaxis(afile, aylength, fromtic, interval, totic, ysubintervals,
		  length, dx, dy, width, decimal, logyscale, logynormal,
		  logybase)
_TEXT *afile;
double aylength, fromtic, interval, totic, ysubintervals, length, dx, dy;
long width, decimal;
boolean logyscale, logynormal;
double logybase;
{
  doaxis(afile, 'y', aylength, fromtic, interval, totic, ysubintervals,
	 length, dx, dy, width, decimal, logyscale, logynormal, logybase);
}





Static Void eeminsymbol(f)
_TEXT *f;
{
  /*



















*/
  /*















































































































































































































































































*/


  fprintf(f->f, "/eemin save def\n");
  fprintf(f->f, "%%!PS-Adobe-2.0\n");






  /*

*/

  fprintf(f->f,
    "/TeXDict 250 dict def TeXDict begin /N{def}def /B{bind def}N /S{exch}N /X{S N}\n");
  fprintf(f->f,
    "B /TR{translate}N /isls false N /vsize 11 72 mul N /@rigin{isls{[0 -1 1 0 0 0]\n");
  fprintf(f->f,
    "concat}if 72 Resolution div 72 VResolution div neg scale isls{Resolution hsize\n");
  fprintf(f->f,
    "-72 div mul 0 TR}if Resolution VResolution vsize -72 div 1 add mul TR matrix\n");
  fprintf(f->f,
    "currentmatrix dup dup 4 get round 4 exch put dup dup 5 get round 5 exch put\n");
  fprintf(f->f,
    "setmatrix}N /@landscape{/isls true N}B /@manualfeed{statusdict /manualfeed\n");
  fprintf(f->f,
    "true put}B /@copies{/#copies X}B /FMat[1 0 0 -1 0 0]N /FBB[0 0 0 0]N /nn 0 N\n");
  fprintf(f->f,
    "/IE 0 N /ctr 0 N /df-tail{/nn 8 dict N nn begin /FontType 3 N /FontMatrix\n");
  fprintf(f->f,
    "fntrx N /FontBBox FBB N string /base X array /BitMaps X /BuildChar{\n");
  fprintf(f->f,
    "CharBuilder}N /Encoding IE N end dup{/foo setfont}2 array copy cvx N load 0 nn\n");
  fprintf(f->f,
    "put /ctr 0 N[}B /df{/sf 1 N /fntrx FMat N df-tail}B /dfs{div /sf X /fntrx[sf 0\n");
  fprintf(f->f,
    "0 sf neg 0 0]N df-tail}B /E{pop nn dup definefont setfont}B /ch-width{ch-data\n");
  fprintf(f->f,
    "dup length 5 sub get}B /ch-height{ch-data dup length 4 sub get}B /ch-xoff{128\n");
  fprintf(f->f,
    "ch-data dup length 3 sub get sub}B /ch-yoff{ch-data dup length 2 sub get 127\n");
  fprintf(f->f,
    "sub}B /ch-dx{ch-data dup length 1 sub get}B /ch-image{ch-data dup type\n");
  fprintf(f->f,
    "/stringtype ne{ctr get /ctr ctr 1 add N}if}B /id 0 N /rw 0 N /rc 0 N /gp 0 N\n");
  fprintf(f->f,
    "/cp 0 N /G 0 N /sf 0 N /CharBuilder{save 3 1 roll S dup /base get 2 index get\n");
  fprintf(f->f,
    "S /BitMaps get S get /ch-data X pop /ctr 0 N ch-dx 0 ch-xoff ch-yoff ch-height\n");
  fprintf(f->f,
    "sub ch-xoff ch-width add ch-yoff setcachedevice ch-width ch-height true[1 0 0\n");
  fprintf(f->f,
    "-1 -.1 ch-xoff sub ch-yoff .1 add]{ch-image}imagemask restore}B /D{/cc X dup\n");
  fprintf(f->f,
    "type /stringtype ne{]}if nn /base get cc ctr put nn /BitMaps get S ctr S sf 1\n");
  fprintf(f->f,
    "ne{dup dup length 1 sub dup 2 index S get sf div put}if put /ctr ctr 1 add N}\n");
  fprintf(f->f,
    "B /I{cc 1 add D}B /bop{userdict /bop-hook known{bop-hook}if /SI save N @rigin\n");
  fprintf(f->f,
    "0 0 moveto /V matrix currentmatrix dup 1 get dup mul exch 0 get dup mul add\n");
  fprintf(f->f,
    ".99 lt{/FV}{/RV}ifelse load def pop pop}N /eop{SI restore userdict\n");
  fprintf(f->f,
    "/eop-hook known{eop-hook}if}N /@start{userdict /start-hook known{start-hook}\n");
  fprintf(f->f,
    "if pop /VResolution X /Resolution X 1000 div /DVImag X /IE 256 array N 0 1 255\n");
  fprintf(f->f,
    "{IE S 1 string dup 0 3 index put cvn put}for 65781.76 div /vsize X 65781.76\n");
  fprintf(f->f,
    "div /hsize X}N /p{show}N /RMat[1 0 0 -1 0 0]N /BDot 260 string N /rulex 0 N\n");
  fprintf(f->f,
    "/ruley 0 N /v{/ruley X /rulex X V}B /V{}B /RV statusdict begin /product where{\n");
  fprintf(f->f,
    "pop product dup length 7 ge{0 7 getinterval dup(Display)eq exch 0 4\n");
  fprintf(f->f,
    "getinterval(NeXT)eq or}{pop false}ifelse}{false}ifelse end{{gsave TR -.1 -.1\n");
  fprintf(f->f,
    "TR 1 1 scale rulex ruley false RMat{BDot}imagemask grestore}}{{gsave TR -.1\n");
  fprintf(f->f,
    "-.1 TR rulex ruley scale 1 1 false RMat{BDot}imagemask grestore}}ifelse B /FV{\n");
  fprintf(f->f,
    "gsave transform round exch round exch itransform moveto rulex 0 rlineto 0\n");
  fprintf(f->f,
    "ruley neg rlineto rulex neg 0 rlineto fill grestore}B /a{moveto}B /delta 0 N\n");
  fprintf(f->f,
    "/tail{dup /delta X 0 rmoveto}B /M{S p delta add tail}B /b{S p tail}B /c{-4 M}\n");
  fprintf(f->f,
    "B /d{-3 M}B /e{-2 M}B /f{-1 M}B /g{0 M}B /h{1 M}B /i{2 M}B /j{3 M}B /k{4 M}B\n");
  fprintf(f->f,
    "/w{0 rmoveto}B /l{p -4 w}B /m{p -3 w}B /n{p -2 w}B /o{p -1 w}B /q{p 1 w}B /r{\n");
  fprintf(f->f,
    "p 2 w}B /s{p 3 w}B /t{p 4 w}B /x{0 S rmoveto}B /y{3 2 roll p a}B /bos{/SS save\n");
  fprintf(f->f, "N}B /eos{SS restore}B end\n");

  fprintf(f->f, "TeXDict begin 40258431 52099146 1000 300 300\n");
  fprintf(f->f,
	  "(/home/kaylor/toms/paper/edmm/fig/eemin.dvi) @start /Fa 3 111\n");
  fprintf(f->f,
    "df<01C003C003C001800000000000000000000000001C00270047004700870087000E000E001C\n");
  fprintf(f->f,
    "001C001C003800388038807080710032001C000A1C7E9B0E>105 D<381F81F04E20C6184640E8\n");
  fprintf(f->f,
    "1C4680F01C8F00F01C8E00E01C0E00E01C0E00E01C1C01C0381C01C0381C01C0381C01C0703803\n");
  fprintf(f->f,
	  "807138038071380380E1380380E2700700643003003820127E9124>109\n");
  fprintf(f->f,
    "D<381F004E61804681C04701C08F01C08E01C00E01C00E01C01C03801C03801C03801C07003807\n");
  fprintf(f->f,
    "10380710380E10380E2070064030038014127E9119>I E /Fb 1 62 df<00000C00001C00001C\n");
  fprintf(f->f,
    "0000380000380000380000700000700000700000E00000E00000E00001C00001C00001C0000380\n");
  fprintf(f->f,
    "000380000380000700000700000700000E00000E00000E00001C00001C00001C00003800003800\n");
  fprintf(f->f,
    "00380000700000700000700000E00000E00000E00001C00001C00001C000038000038000038000\n");
  fprintf(f->f,
    "0700000700000700000E00000E00000E00001C00001C00001C0000380000380000380000700000\n");
  fprintf(f->f,
    "700000700000E00000E00000C00000163C7DAC1D>61 D E /Fc 1 70 df<000007F000003FFC00\n");
  fprintf(f->f,
    "00FFFE000181FE0006007E000C003E0018003C003800380070003000F0004000F0000001F00000\n");
  fprintf(f->f,
    "01F0000001F8000001F8000001FC000000FF000000FFC000007FFC00001FF0000007E000001C00\n");
  fprintf(f->f,
    "000020000000C000000180000003000000060000000E0000001C0000003C000000380000007800\n");
  fprintf(f->f,
    "000078000020F80000C0F8000180F8000300FC000600FE000C00FF0018007FC060003FFFC0001F\n");
  fprintf(f->f, "FF000007F800001F2B7EA920>69 D E end\n");



  fprintf(f->f, "TeXDict begin\n\n");


  fprintf(f->f,
	  "1 0 bop 350 241 a Fc(E)5 b Fb(=)p Fc(E)448 250 y Fa(min)p eop\n");

  fprintf(f->f, "end\n");
  fprintf(f->f, "userdict /end-hook known{end-hook}if\n");

  fprintf(f->f, "eemin restore\n");

}





Static Void pynysymbol(f)
_TEXT *f;
{
  /*








*/
  fprintf(f->f, "gsave\n");

  fprintf(f->f, "250 -260 translate\n");
  fprintf(f->f, "%%!PS-Adobe-2.0\n");






  fprintf(f->f,
    "/TeXDict 200 dict def TeXDict begin /N /def load def /B{bind def}N /S /exch\n");
  fprintf(f->f,
    "load def /X{S N}B /TR /translate load N /isls false N /vsize 10 N /@rigin{\n");
  fprintf(f->f,
    "isls{[0 1 -1 0 0 0]concat}if 72 Resolution div 72 VResolution div neg scale\n");
  fprintf(f->f,
    "Resolution VResolution vsize neg mul TR matrix currentmatrix dup dup 4 get\n");
  fprintf(f->f,
    "round 4 exch put dup dup 5 get round 5 exch put setmatrix}B /@letter{/vsize\n");
  fprintf(f->f, "10\n");
  fprintf(f->f,
    "N}B /@landscape{/isls true N /vsize -1 N}B /@a4{/vsize 10.6929133858 N}B\n");
  fprintf(f->f, "/@a3{\n");
  fprintf(f->f,
    "/vsize 15.5531 N}B /@ledger{/vsize 16 N}B /@legal{/vsize 13 N}B /@manualfeed{\n");
  fprintf(f->f,
    "statusdict /manualfeed true put}B /@copies{/#copies X}B /FMat[1 0 0 -1 0\n");
  fprintf(f->f, "0]N\n");
  fprintf(f->f,
    "/FBB[0 0 0 0]N /df{/sf 1 N /fntrx FMat N df-tail}B /dfs{div /sf X /fntrx[sf\n");
  fprintf(f->f, "0\n");
  fprintf(f->f,
    "0 sf neg 0 0]N df-tail}B /df-tail{/nn 8 dict N nn begin /FontType 3 N\n");
  fprintf(f->f,
    "/FontMatrix fntrx N /FontBBox FBB N string /base X array /BitMaps X /BuildChar\n");
  fprintf(f->f,
    "{CharBuilder}N /Encoding IE N end dup{/foo setfont}2 array copy cvx N load\n");
  fprintf(f->f, "0\n");
  fprintf(f->f,
    "nn put /ctr 0 N[}B /E{pop nn dup definefont setfont}B /ch-image{ch-data\n");
  fprintf(f->f, "dup\n");
  fprintf(f->f,
    "type /stringtype ne{ctr get /ctr ctr 1 add N}if}B /ch-width{ch-data dup\n");
  fprintf(f->f, "length\n");
  fprintf(f->f,
    "5 sub get}B /ch-height{ch-data dup length 4 sub get}B /ch-xoff{128 ch-data\n");
  fprintf(f->f, "dup\n");
  fprintf(f->f,
    "length 3 sub get sub}B /ch-yoff{ch-data dup length 2 sub get 127 sub}B /ch-dx{\n");
  fprintf(f->f,
    "ch-data dup length 1 sub get}B /ctr 0 N /CharBuilder{save 3 1 roll S dup\n");
  fprintf(f->f, "/base\n");
  fprintf(f->f,
    "get 2 index get S /BitMaps get S get /ch-data X pop /ctr 0 N ch-dx 0 ch-xoff\n");
  fprintf(f->f,
    "ch-yoff ch-height sub ch-xoff ch-width add ch-yoff setcachedevice ch-width\n");
  fprintf(f->f,
    "ch-height true[1 0 0 -1 -.1 ch-xoff sub ch-yoff .1 add]{ch-image}imagemask\n");
  fprintf(f->f,
    "restore}B /D{/cc X dup type /stringtype ne{]}if nn /base get cc ctr put\n");
  fprintf(f->f, "nn\n");
  fprintf(f->f,
    "/BitMaps get S ctr S sf 1 ne{dup dup length 1 sub dup 2 index S get sf div\n");
  fprintf(f->f, "put\n");
  fprintf(f->f,
    "}if put /ctr ctr 1 add N}B /I{cc 1 add D}B /bop{userdict /bop-hook known{\n");
  fprintf(f->f,
    "bop-hook}if /SI save N @rigin 0 0 moveto}B /eop{clear SI restore\n");

  fprintf(f->f,
    "userdict /eop-hook known{eop-hook}if}B /@start{userdict /start-hook known{\n");
  fprintf(f->f,
    "start-hook}if /VResolution X /Resolution X 1000 div /DVImag X /IE 256 array\n");
  fprintf(f->f, "N\n");
  fprintf(f->f,
    "0 1 255{IE S 1 string dup 0 3 index put cvn put}for}B /p /show load N /RMat[1\n");
  fprintf(f->f,
    "0 0 -1 0 0]N /BDot 260 string N /v{/ruley X /rulex X V}B /V{gsave TR -.1\n");
  fprintf(f->f, "-.1\n");
  fprintf(f->f,
    "TR 1 ruley scale rulex 1 false RMat{BDot}imagemask grestore}B /a{moveto}B\n");
  fprintf(f->f,
    "/delta 0 N /tail{dup /delta X 0 rmoveto}B /M{S p delta add tail}B /b{S p\n");
  fprintf(f->f, "tail}\n");
  fprintf(f->f,
    "B /c{-4 M}B /d{-3 M}B /e{-2 M}B /f{-1 M}B /g{0 M}B /h{1 M}B /i{2 M}B /j{3\n");
  fprintf(f->f, "M}B\n");
  fprintf(f->f,
    "/k{4 M}B /l{p -4 w}B /m{p -3 w}B /n{p -2 w}B /o{p -1 w}B /q{p 1 w}B /r{p\n");
  fprintf(f->f, "2 w}\n");
  fprintf(f->f,
    "B /s{p 3 w}B /t{p 4 w}B /w{0 rmoveto}B /x{0 S rmoveto}B /y{3 2 roll p a}B\n");
  fprintf(f->f, "/bos\n");
  fprintf(f->f, "{/SS save N}B /eos{clear SS restore}B end\n");
  /*
*/
  fprintf(f->f,
    "TeXDict begin 1000 300 300 @start /Fa 1 122 df<1C00C02701C04701C04701C0870380\n");
  fprintf(f->f,
    "8703800E03800E03801C07001C07001C07001C0700180E00180E00180E001C1E000C3C0007DC00\n");
  fprintf(f->f,
	  "001C00001800603800F03000F06000E0C0004180003E0000121A7E9114>121\n");
  fprintf(f->f,
    "D E /Fb 3 81 df<00000600000E00000E00001C00001C00001C00003800003800003800007000\n");
  fprintf(f->f,
    "00700000E00000E00000E00001C00001C00001C000038000038000038000070000070000070000\n");
  fprintf(f->f,
    "0E00000E00000E00001C00001C00001C0000380000380000700000700000700000E00000E00000\n");
  fprintf(f->f,
    "E00001C00001C00001C0000380000380000380000700000700000700000E00000E00000E00001C\n");
  fprintf(f->f,
    "00001C0000380000380000380000700000700000700000E00000E00000C00000173C7DAC1E>61\n");
  fprintf(f->f,
    "D<003FF80007FFC0003FFC0007FFC00000FC00007C0000017E0000300000013E0000200000013E\n");
  fprintf(f->f,
    "0000200000011F0000200000021F0000400000020F8000400000020F8000400000020FC0004000\n");
  fprintf(f->f,
    "000407C0008000000407C0008000000403E0008000000403E0008000000803F0010000000801F0\n");
  fprintf(f->f,
    "010000000801F8010000000800F8010000001000F80200000010007C0200000010007C02000000\n");
  fprintf(f->f,
    "10007E0200000020003E0400000020003F0400000020001F0400000020001F0400000040000F88\n");
  fprintf(f->f,
    "00000040000F8800000040000FC8000000400007C8000000800007F0000000800003F000000080\n");
  fprintf(f->f,
    "0003F0000000800001F0000001000001E0000001000001E0000003000000E000000FC00000E000\n");
  fprintf(f->f,
    "00FFFC0000400000FFF8000040000032297EA830>78 D<003FFFFFE000003FFFFFFC000001F800\n");
  fprintf(f->f,
    "7F000001F0001F800001F0000F800001F00007C00001F00007C00003E00007E00003E00007E000\n");
  fprintf(f->f,
    "03E00007E00003E00007E00007C0000FC00007C0000FC00007C0000F800007C0001F80000F8000\n");
  fprintf(f->f,
    "1F00000F80003E00000F80007C00000F8001F000001F0007E000001FFFFF0000001F0000000000\n");
  fprintf(f->f,
    "1F00000000003E00000000003E00000000003E00000000003E00000000007C00000000007C0000\n");
  fprintf(f->f,
    "0000007C00000000007C0000000000F80000000000F80000000000F80000000000F80000000001\n");
  fprintf(f->f,
    "F00000000001F00000000001F00000000003F000000000FFFFC0000000FFFF800000002B297EA8\n");
  fprintf(f->f, "26>80 D E end\n");



  fprintf(f->f, "TeXDict begin \n");


  fprintf(f->f,
	  "bop 365 257 a Fb(P)403 266 y Fa(y)427 257 y Fb(=)m(N)502 266\n");
  fprintf(f->f, "y Fa(y)p eop\n");

  fprintf(f->f, "end\n");
  fprintf(f->f, "userdict /end-hook known{end-hook}if\n");


  fprintf(f->f, "grestore\n");
}


#define dashlength      0.1


/* Local variables for etcurve: */
struct LOC_etcurve {
  _TEXT *afile;
  double scalex, scaley;
} ;

Local double et(pn, LINK)
double pn;
struct LOC_etcurve *LINK;
{
  if (pn != 0.0)
    return (pn / log(1 + pn));
  else
    return 1.0;
}

Local Void special(pn, LINK)
double pn;
struct LOC_etcurve *LINK;
{
  double e, xloc, yloc;

  /*
*/
  e = et(pn, LINK);
  xloc = pn * LINK->scalex;
  yloc = e * LINK->scaley;
  movea(LINK->afile, 0.0, yloc);
  ytic(LINK->afile, 0.05, -0.2, -0.02, e, 4L, 2L, false, 2.0);
  drawa(LINK->afile, xloc, yloc, '-', dashlength);
  drawa(LINK->afile, xloc, 0.0, '-', dashlength);
}

#undef dashlength



Static Void etcurve(afile_, height, width, range)
_TEXT *afile_;
double height, width, range;
{
  /*

*/
  struct LOC_etcurve V;
  double dpn = 0.01, pn = 0.0;

  V.afile = afile_;
  V.scalex = width / range;
  V.scaley = height / et(range, &V);


  xaxis(V.afile, width, 0.0, 1.0, range, 1.0, 0.05, 0.0, -0.12, 1L, 0L, false,
	false, 2.0);
  movea(V.afile, 0.0, 0.0);
  linea(V.afile, 0.0, height);



  /*


*/


  movea(V.afile, 0.0, 0.0);
  ytic(V.afile, 0.05, -0.2, 0.0, 0.0, 4L, 2L, false, 2.0);

  movea(V.afile, 0.0, et(0.0, &V) * V.scaley);
  ytic(V.afile, 0.05, -0.2, 0.0, et(0.0, &V), 4L, 2L, false, 2.0);

  /*


*/


  special(1.0, &V);
  special(2.0, &V);
  special(3.0, &V);
  special(4.0, &V);


  movea(V.afile, pn * V.scalex, et(pn, &V));
  while (pn <= range) {
    linea(V.afile, pn * V.scalex, et(pn, &V) * V.scaley);
    pn += dpn;
  }
}



Static Void themain(afile)
_TEXT *afile;
{
  fprintf(afile->f, "%%! emin %4.2f\n", version);

  fprintf(afile->f, "%%***************************************************\n");
  fprintf(afile->f, "%% produce PyNy symbol\n");
  pynysymbol(afile);
  fprintf(afile->f, "%% end of PyNy symbol\n");
  fprintf(afile->f,
	  "%%***************************************************\n\n");

  fprintf(afile->f, "%%***************************************************\n");
  fprintf(afile->f, "%% produce Emin/E symbol\n");
  eeminsymbol(afile);
  fprintf(afile->f, "%% end of Emin/E symbol\n");
  fprintf(afile->f,
	  "%%***************************************************\n\n");

  startpic(afile, 81.0, 2.0, 5.0, 't');
  etcurve(afile, 3.0, 3.0, 4.3);
  movea(afile, -2.0, 0.0);
  stoppic(afile);
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  TEMP.f = stdout;
  *TEMP.name = '\0';
  themain(&TEMP);
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
