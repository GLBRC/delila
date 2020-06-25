/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rsgra.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         5.08
/*











*/



/*
























































*/



#define nfield          4



#define pi              3.14159265354

#define picfield        8
#define picwidth        5

#define charwidth       0.0625
/*
*/
/*
*/
#define dotfactor       0.00625

#define defscale        81




#define maxstring       2000




#define fillermax       21




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




typedef struct rstype {
  long rstart, rstop, l, nal, ncl, ngl, ntl;
  double rsl, rs, varhnb, sumvar;
  long nl;
  double ehnb;
  Char flag;
} rstype;




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



Static _TEXT rsdata, picture, rsgrap, wave, marks;



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

  fprintf(afile->f,
	  "clear erasepage %% REMOVE FOR PACKAGING INTO ANOTHER FIGURE\n");

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
	  picfield, picwidth, x * scale, picfield, picwidth, y * scale);

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

  fprintf(afile->f, "showpage %% REMOVE FOR PACKAGING INTO ANOTHER FIGURE\n");
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
	  picwidth, picfield, dx * scale, picwidth, picfield, dy * scale);
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
		picwidth, picfield, ddx, picwidth, picfield, ddy);
	if (on)
	  fprintf(afile->f, " rl\n");
	else
	  fprintf(afile->f, " rm\n");
	on = !on;
	break;

      case '.':
	fprintf(afile->f, "%*.*f 0 rl", picwidth, picfield, ss);
	fprintf(afile->f, " %*.*f 0 rl", picwidth, picfield, -ss);
	fprintf(afile->f, " %*.*f %*.*f",
		picwidth, picfield, ddx, picwidth, picfield, ddy);
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



Static Void graphstring(tofile, s, centered)
_TEXT *tofile;
stringDelila *s;
boolean centered;
{
  /*


*/
  long i;
  double mv;
  boolean quoted;
  long FORLIM;

  if (!(inpicture && s->length > 0))
    return;
  if (s->length > 2) {
    if (s->letters[0] == '"' && s->letters[s->length - 1] == '"')
      quoted = true;
    else
      quoted = false;
  } else
    quoted = false;


  if (quoted)
    centered = true;

  if (centered) {
    /*
*/
    if (quoted)
      i = s->length - 2;
    else
      i = s->length;
    mv = i * charwidth / 2.0;
    mover(tofile, -mv, 0.0);
  }




  fprintf(tofile->f, "c\n");

  putc('(', tofile->f);
  if (quoted) {
    FORLIM = s->length - 2;
    for (i = 1; i <= FORLIM; i++)
      putc(s->letters[i], tofile->f);
  } else {
    FORLIM = s->length;
    for (i = 0; i < FORLIM; i++)
      putc(s->letters[i], tofile->f);
  }
  putc(')', tofile->f);
  fprintf(tofile->f, " x\n");
  inpath = false;

  if (centered) {
    mover(tofile, mv, 0.0);
    /*

*/
  }

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



Static Void picnumber(afile, dx, dy, number, width, decimal, centered)
_TEXT *afile;
double dx, dy, number;
long width, decimal;
boolean centered;
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

  graphstring(afile, &name, centered);
  mover(afile, -dx, -dy);
}



Static Void xtic(afile, length, dx, dy, number, width, decimal)
_TEXT *afile;
double length, dx, dy, number;
long width, decimal;
{
  /*




*/
  liner(afile, 0.0, -length);
  picnumber(afile, dx, dy, number, width, decimal, true);
  mover(afile, 0.0, length);
}



Static Void ytic(afile, length, dx, dy, number, width, decimal)
_TEXT *afile;
double length, dx, dy, number;
long width, decimal;
{
  /*




*/
  liner(afile, -length, 0.0);
  picnumber(afile, dx, dy, number, width, decimal, true);
  mover(afile, length, 0.0);
}



Static Void xaxis(afile, axlength, fromtic, interval, totic, length, dx, dy,
		  width, decimal)
_TEXT *afile;
double axlength, fromtic, interval, totic, length, dx, dy;
long width, decimal;
{
  /*





*/
  double jump;
  double jumpdistance = 0.0;
  /*

*/
  double tic, totici2;

  liner(afile, axlength, 0.0);
  mover(afile, -axlength, 0.0);
  if (totic == fromtic) {
    printf("xaxis: fromtic and totic cannot be equal\n");
    halt();
  }
  if (axlength == 0.0 || interval == 0.0) {
    printf("xaxis: neither axlength nor interval can be zero\n");
    halt();
  }
  jump = axlength * interval / (totic - fromtic);

  /*

*/
  tic = fromtic;
  if (interval > 0.0) {
    totici2 = totic + interval / 2.0;
    while (tic <= totici2) {
      xtic(afile, length, dx, dy, tic, width, decimal);
      tic += interval;
      if (tic > totici2)
	break;
      mover(afile, jump, 0.0);
      jumpdistance += jump;
    }
  } else if (interval < 0.0) {
    totici2 = totic - interval / 2.0;
    while (tic >= totici2) {
      xtic(afile, length, dx, dy, tic, width, decimal);
      tic += interval;
      if (tic < totici2)
	break;
      mover(afile, jump, 0.0);
      jumpdistance += jump;
    }
  }

  mover(afile, -jumpdistance, 0.0);
}



Static Void yaxis(afile, aylength, fromtic, interval, totic, length, dx, dy,
		  width, decimal)
_TEXT *afile;
double aylength, fromtic, interval, totic, length, dx, dy;
long width, decimal;
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

  liner(afile, 0.0, aylength);
  mover(afile, 0.0, -aylength);
  if (totic == fromtic) {
    printf("yaxis: fromtic and totic cannot be equal\n");
    halt();
  }
  if (aylength == 0.0 || interval == 0.0) {
    printf("yaxis: neither aylength nor interval can be zero\n");
    halt();
  }
  jump = aylength * interval / (totic - fromtic);
  half = interval / 2.0;

  tic = fromtic;
  if (interval > 0.0) {
    while (tic <= totic + half) {
      ytic(afile, length, dx, dy, tic, width, decimal);
      tic += interval;
      if (tic > totic + half)
	break;
      mover(afile, 0.0, jump);
      jumpdistance += jump;
    }
  } else if (interval < 0.0) {
    while (tic >= totic - half) {
      ytic(afile, length, dx, dy, tic, width, decimal);
      tic += interval;
      if (tic < totic - half)
	break;
      mover(afile, 0.0, jump);
      jumpdistance += jump;
    }
  }

  mover(afile, 0.0, -jumpdistance);
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
	  picfield, picwidth, scale * radius, picfield, picwidth, angle1,
	  picfield, picwidth, angle2);
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



/*





















*/


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



Static Void header(infile, marks, outfile)
_TEXT *infile, *marks, *outfile;
{
  long index;

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

  /*


*/

  fprintf(outfile->f, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(outfile->f, "%%%%Title: rsgra %4.2f\n", version);
  fprintf(outfile->f, "%%%%Creator: Tom Schneider\n");
  fprintf(outfile->f, "%%%%BoundingBox:    0   0   630 900\n");
  fprintf(outfile->f, "%%%%DocumentFonts:\n");
  fprintf(outfile->f, "%%%%EndComments\n");
  fprintf(outfile->f, "%%%%EndProlog\n");


  for (index = 1; index <= 3; index++) {
    fprintf(outfile->f, "%% ");
    copyaline(infile, outfile);
  }

  fprintf(outfile->f, "\n%% MARKS: \n");


  while (!BUFEOF(marks->f)) {
    fprintf(outfile->f, "%% ");
    copyaline(marks, outfile);
  }

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
}


#define step            0.01


/* Local variables for cosine: */
struct LOC_cosine {
  double amplitude, phase, wavelength, base;
} ;

Local double c(y, LINK)
double y;
struct LOC_cosine *LINK;
{
  return (LINK->amplitude *
	  cos((-y - LINK->phase) / LINK->wavelength * 2 * pi) + LINK->base);
}

Local boolean between(a, b, c, LINK)
double a, b, c;
struct LOC_cosine *LINK;
{
  return (a <= b && b <= c);
}



Static Void cosine(afile, amplitude_, phase_, wavelength_, base_, xmin, ymin,
		   xmax, ymax)
_TEXT *afile;
double amplitude_, phase_, wavelength_, base_, xmin, ymin, xmax, ymax;
{
  /*



*/
  struct LOC_cosine V;
  boolean inside1, inside2;
  double x1, y1, x2, y2;

  V.amplitude = amplitude_;
  V.phase = phase_;
  V.wavelength = wavelength_;
  V.base = base_;
  y1 = ymax;
  x1 = c(y1, &V);
  inside1 = between(ymin, y1, ymax, &V) & between(xmin, x1, xmax, &V);

  while (y1 >= ymin) {
    x2 = x1;
    y2 = y1;
    inside2 = inside1;

    y1 -= step;
    x1 = c(y1, &V);
    inside1 = between(ymin, y1, ymax, &V) & between(xmin, x1, xmax, &V);

    if (inside1 && inside2) {
      movea(afile, x1, y1);
      linea(afile, x2, y2);
    }
  }
}

#undef step






Static Void readrsrange(rsdata, r)
_TEXT *rsdata;
rstype *r;
{
  /*
*/
  long index;
  Char skip;

  for (index = 1; index <= 11; index++) {
    fscanf(rsdata->f, "%*[^\n]");
    getc(rsdata->f);
  }
  fscanf(rsdata->f, "%c%ld%ld%*[^\n]", &skip, &r->rstart, &r->rstop);
  getc(rsdata->f);

  if (skip == '\n')
    skip = ' ';
}



Static Void getrsbegin(infile)
_TEXT *infile;
{
  /*
*/
  Char ch;
  trigger begindata;


  filltrigger(&begindata, "l    a    c    g    t");
  resettrigger(&begindata);

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
  while (!begindata.found) {
    if (P_eoln(infile->f)) {
      fscanf(infile->f, "%*[^\n]");
      getc(infile->f);
    }
    if (BUFEOF(infile->f)) {
      printf("beginning of data not found\n");
      halt();
    }
    ch = getc(infile->f);
    if (ch == '\n')
      ch = ' ';
    testfortrigger(ch, &begindata);
  }
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
}



Static Void readrsdata(rsdata, rdata)
_TEXT *rsdata;
rstype *rdata;
{
  fscanf(rsdata->f, "%ld%ld%ld%ld%ld%lg%lg%lg%lg%ld%lg", &rdata->l,
	 &rdata->nal, &rdata->ncl, &rdata->ngl, &rdata->ntl, &rdata->rsl,
	 &rdata->rs, &rdata->varhnb, &rdata->sumvar, &rdata->nl,
	 &rdata->ehnb);

  while (P_peek(rsdata->f) == ' ')
    getc(rsdata->f);
  fscanf(rsdata->f, "%c%*[^\n]", &rdata->flag);
  getc(rsdata->f);
  /*
*/
  if (rdata->flag == '\n')
    rdata->flag = ' ';
}


#define duplicates      2

#define halfdatawidth   2.4
/*

*/
#define leftbound       3.0
/*
*/
#define spacing         0.01
/*
*/
#define xscale          2.00
#define yscale          0.1595






Static Void themain(infile, rsgrap, wave, marks, outfile)
_TEXT *infile, *rsgrap, *wave, *marks, *outfile;
{
  /*
*/
  /*


*/
  double bar;
  long barposition, basetomark;
  double dataposition;
  /*
*/
  double dx, dy;
  long frombase, tobase, line;
  double linedx, linedy;
  /*
*/
  double lowpoint = LONG_MAX;
  boolean notfirstpoint = false;
  /*
*/
  /*

*/
  waveparam *params;
  double point;
  long position;
  rstype rdata;
  double xmin, ymin, xmax = 2.0 * xscale, ymax = 0.0;
  double x1, y1, x2, y2, ybar;
  long FORLIM;

  printf("rsgra %4.2f\n", version);


  header(infile, marks, outfile);

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
  readwaveparameters(wave, &params);

  if (*rsgrap->name != '\0') {
    if (rsgrap->f != NULL)
      rsgrap->f = freopen(rsgrap->name, "r", rsgrap->f);
    else
      rsgrap->f = fopen(rsgrap->name, "r");
  } else
    rewind(rsgrap->f);
  if (rsgrap->f == NULL)
    _EscIO2(FileNotFound, rsgrap->name);
  RESETBUF(rsgrap->f, Char);
  if (BUFEOF(rsgrap->f)) {
    frombase = -LONG_MAX;
    tobase = LONG_MAX;
  } else {
    fscanf(rsgrap->f, "%ld%ld%*[^\n]", &frombase, &tobase);
    getc(rsgrap->f);
  }


  if (BUFEOF(marks->f))
    barposition = -LONG_MAX;
  else {
    if (P_peek(marks->f) == 'b') {
      getc(marks->f);
      fscanf(marks->f, "%ld%*[^\n]", &barposition);
      getc(marks->f);
      printf("bar placed at position %ld\n", barposition);
    }
  }


  if (!BUFEOF(marks->f)) {
    fscanf(marks->f, "%ld%*[^\n]", &basetomark);
    getc(marks->f);
  } else
    basetomark = -LONG_MAX;
  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else {


    rewind(infile->f);
  }
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);
  readrsrange(infile, &rdata);
  /*

*/
  if (params != NULL)
    fprintf(outfile->f, "%% cosine wavelength is %4.2f bases\n",
	    params->wavelength);


  if (frombase < rdata.rstart)
    frombase = rdata.rstart;
  if (tobase > rdata.rstop)
    tobase = rdata.rstop;
  getrsbegin(infile);
  FORLIM = rdata.rstop;
  for (position = rdata.rstart; position <= FORLIM; position++) {
    if (P_peek(infile->f) != '*') {
      readrsdata(infile, &rdata);
      point = rdata.rsl - sqrt(rdata.varhnb);
      if (point < lowpoint)
	lowpoint = point;
    } else {
      fscanf(infile->f, "%*[^\n]");
      getc(infile->f);
    }
  }

  /*
*/
  if (lowpoint >= 0.0)
    lowpoint = -0.1;
  else
    lowpoint = (long)(1 - 10 * lowpoint) / -10.0;
  dataposition = halfdatawidth - lowpoint * xscale;




  xmin = lowpoint * xscale;
  ymin = (frombase - tobase - 2) * yscale;
  /*

*/
  startpic(outfile, (double)defscale, 0.7, 5.0, 'c');


  fprintf(outfile->f, "45 rotate\n");
  fprintf(outfile->f, "45 rotate\n");
  /*
*/
  movea(outfile, -leftbound, 0.0);
  movea(outfile, -dataposition, 0.0);

  fprintf(outfile->f, "gsave 1 2 scale\n");
  fprintf(outfile->f, "(   l    a    c    g    t  Rs(l)) x\n");
  fprintf(outfile->f, "grestore\n");


  movea(outfile, 0.25 * xscale, 3 * yscale);
  fprintf(outfile->f, "gsave 1 2 scale\n");
  fprintf(outfile->f, "(Rs(l) = Rsequence(l), Information in bits) x\n");
  fprintf(outfile->f, "grestore\n");


  /*


*/
  movea(outfile, -1.60 * xscale, ((frombase - tobase) / 2.0 + 8.5) * yscale);
  fprintf(outfile->f, "gsave 2 2 scale\n");

  /*





*/
  fprintf(outfile->f, "-45 rotate\n");
  fprintf(outfile->f, "-45 rotate\n");

  mover(outfile, 0.0, -0.60);
  fprintf(outfile->f, "(Position L (in bases)) x\n");
  mover(outfile, 0.0, 0.60);

  fprintf(outfile->f, "grestore\n");


  /*
*/


  movea(outfile, -2 * dataposition / 2, 4 * yscale - 0.2);
  putc('(', outfile->f);

  fprintf(outfile->f, "rsgra %4.2f ", version);
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
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
  while (!P_eoln(infile->f)) {
    putc(P_peek(infile->f), outfile->f);
    getc(infile->f);
  }
  fprintf(outfile->f, ") x %% NOHEADER FOR PACKAGING INTO ANOTHER FIGURE\n");


  movea(outfile, 0.0, 0.0);
  xaxis(outfile, xmax, 0.0, 1.0, 2.0, -0.1, 0.0, 0.1, 3L, 1L);

  mover(outfile, lowpoint * xscale, 0.0);
  xaxis(outfile, (2.0 - lowpoint) * xscale, lowpoint, 0.1, 2.0, -0.1, 0.0,
	0.0, 0L, 1L);


  movea(outfile, 0.0, 0.0);
  drawa(outfile, 0.0, ymin, '-', 0.03);


  xaxis(outfile, xmax, 0.0, 1.0, 2.0, 0.1, 0.0, -0.1, 0L, 1L);

  mover(outfile, xmin, 0.0);
  xaxis(outfile, (2.0 - lowpoint) * xscale, lowpoint, 0.1, 2.0, 0.1, 0.0, 0.0,
	0L, 1L);
  /*
*/

  /*
*/


  getrsbegin(infile);


  FORLIM = rdata.rstop;
  for (position = rdata.rstart; position <= FORLIM; position++) {
    if (P_peek(infile->f) != '*') {
      readrsdata(infile, &rdata);

      if (position >= frombase && position <= tobase) {


	if (position == barposition) {
	  /*
*/
	  /*
*/
	  ybar = (frombase - position - 1.5) * yscale;
	  /*

*/
	  movea(outfile, -dataposition, ybar);
	  linea(outfile, 2 * xscale, ybar);
	}


	/*
*/
	movea(outfile, -dataposition, (frombase - position - 1.2) * yscale);
	fprintf(outfile->f, "(%*ld %*ld %*ld %*ld %*ld%7.2f) x\n",
		nfield, rdata.l, nfield, rdata.nal, nfield, rdata.ncl, nfield,
		rdata.ngl, nfield, rdata.ntl, rdata.rsl);

	if (notfirstpoint) {
	  x2 = x1;
	  y2 = y1;
	  x1 = rdata.rsl * xscale;
	  y1 = (frombase - position - 1) * yscale;

	  /*





*/
	  polrec(spacing, atan((x1 - x2) / (y2 - y1)), &dx, &dy);

	  for (line = -duplicates; line <= duplicates; line++) {
	    linedx = line * dx;
	    linedy = line * dy;
	    movea(outfile, x2 + linedx, y2 + linedy);
	    linea(outfile, x1 + linedx, y1 + linedy);
	  }
	}

	else {
	  x1 = rdata.rsl * xscale;
	  y1 = (frombase - position - 1) * yscale;
	  notfirstpoint = true;
	}



	movea(outfile, x1, y1);
	bar = sqrt(rdata.varhnb) * xscale;
	mover(outfile, bar, 0.0);
	liner(outfile, -bar, 0.0);
	circler(outfile, yscale / 3);
	liner(outfile, -bar, 0.0);


	if (basetomark == rdata.l) {
	  movea(outfile, lowpoint, (frombase - position - 1) * yscale);
	  circler(outfile, yscale / 3);
	  printf("marking base %ld\n", rdata.l);
	  if (!BUFEOF(marks->f)) {
	    fscanf(marks->f, "%ld%*[^\n]", &basetomark);
	    getc(marks->f);
	  } else
	    basetomark = -LONG_MAX;
	}

      }
    } else {
      fscanf(infile->f, "%*[^\n]");
      getc(infile->f);
    }
  }



  /*

*/
  if (params != NULL) {
    if (params->extreme == 'h')
      cosine(outfile, xscale * params->waveamplitude,
	     yscale * (params->wavelocation - frombase + 1),
	     yscale * params->wavelength,
	     xscale * (params->wavebit - params->waveamplitude), xmin, ymin,
	     xmax, ymax);
    else
      cosine(outfile, -xscale * params->waveamplitude,
	     yscale * (params->wavelocation - frombase + 1),
	     yscale * params->wavelength,
	     xscale * (params->wavebit + params->waveamplitude), xmin, ymin,
	     xmax, ymax);
  }
  stoppic(outfile);
}

#undef duplicates
#undef halfdatawidth
#undef leftbound
#undef spacing
#undef xscale
#undef yscale


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  marks.f = NULL;
  strcpy(marks.name, "marks");
  wave.f = NULL;
  strcpy(wave.name, "wave");
  rsgrap.f = NULL;
  strcpy(rsgrap.name, "rsgrap");
  picture.f = NULL;
  strcpy(picture.name, "picture");
  rsdata.f = NULL;
  strcpy(rsdata.name, "rsdata");
  themain(&rsdata, &rsgrap, &wave, &marks, &picture);
_L1:
  if (rsdata.f != NULL)
    fclose(rsdata.f);
  if (picture.f != NULL)
    fclose(picture.f);
  if (rsgrap.f != NULL)
    fclose(rsgrap.f);
  if (wave.f != NULL)
    fclose(wave.f);
  if (marks.f != NULL)
    fclose(marks.f);
  exit(EXIT_SUCCESS);
}



/* End. */
