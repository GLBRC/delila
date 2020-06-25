/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "da3d.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.23
/*
*/



/*

























































*/



#define mingrid         (-100)
#define maxgrid         100



#define maxstring       150



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






typedef double threevector[3];

typedef double tbtarray[3][3];

typedef struct screen {
  threevector a, b, c, v, g;
  double smag, range;
} screen;



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;




typedef double datagrid[maxgrid - mingrid + 1][maxgrid - mingrid + 1];


Static _TEXT da, da3dp, scene;


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

  fprintf(afile->f, "initgraphics\n");
  /*

*/

  scale = setscale;
  fprintf(afile->f, "clear erasepage\n");

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





Static Void polrec(r, theta, x, y)
double r, theta, *x, *y;
{
  /*
*/
  *x = r * cos(theta);
  *y = r * sin(theta);
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
  double oldxscreen, oldyscreen;


  double dr, dtheta;
  double r = 0.0, radius = 2.0, theta = 0.0, thickness = 0.1, steps = 15.0;

  double x = 0.0, y = 0.0;
  double z;

  makescreen(5.0, 5.0, 5.0, -1.0, -1.0, -1.0, 5.0, &s);

  dr = thickness / steps;
  dtheta = 2 * pi / steps;

  z = fun(r);
  project3d(x, y, z, s, &oldxscreen, &oldyscreen);
  mover(afile, oldxscreen, oldyscreen);

  while (r < radius) {
    r += dr;
    theta += dtheta;
    polrec(r, theta, &x, &y);

    z = fun(r);
    project3d(x, y, z, s, &xscreen, &yscreen);


    liner(afile, xscreen - oldxscreen, yscreen - oldyscreen);
    oldxscreen = xscreen;
    oldyscreen = yscreen;
  }
}



Static Void getfromto(da, f, t)
_TEXT *da;
long *f, *t;
{
  Char c;

  if (*da->name != '\0') {
    if (da->f != NULL)
      da->f = freopen(da->name, "r", da->f);
    else
      da->f = fopen(da->name, "r");
  } else
    rewind(da->f);
  if (da->f == NULL)
    _EscIO2(FileNotFound, da->name);
  RESETBUF(da->f, Char);
  fscanf(da->f, "%*[^\n]");
  getc(da->f);
  fscanf(da->f, "%*[^\n]");
  getc(da->f);
  fscanf(da->f, "%c%ld%ld%*[^\n]", &c, f, t);
  getc(da->f);
  if (c == '\n')
    c = ' ';
  printf("%ld %ld\n", *f, *t);
}



Static Void readdata(da, datacolumn, fromrange, torange, data)
_TEXT *da;
long datacolumn, *fromrange, *torange;
double (*data)[maxgrid - mingrid + 1];
{
  Char idcharacter, skipc;
  long column, x, y;
  double z;

  getfromto(da, fromrange, torange);

  if (*torange - *fromrange > maxgrid - mingrid) {
    printf(
      "range (from %ld to %ld) is bigger than allowed by constant maxgrid (%ld)\n",
      *fromrange, *torange, (long)maxgrid);
    halt();
  }


  for (x = mingrid; x <= maxgrid; x++) {
    for (y = mingrid; y <= maxgrid; y++)
      data[x - mingrid][y - mingrid] = -LONG_MAX;
  }

  while (!BUFEOF(da->f)) {
    idcharacter = getc(da->f);
    if (idcharacter == '\n')
      idcharacter = ' ';
    if (idcharacter != 'i') {
      fscanf(da->f, "%*[^\n]");
      getc(da->f);
      continue;
    }
    skipc = getc(da->f);
    skipc = getc(da->f);
    skipc = getc(da->f);
    if (skipc == '\n')
      skipc = ' ';
    if (skipc == '\n')
      skipc = ' ';
    if (skipc == '\n')
      skipc = ' ';
    fscanf(da->f, "%ld%ld", &x, &y);

    for (column = 5; column <= datacolumn; column++)
      fscanf(da->f, "%lg", &z);
    fscanf(da->f, "%*[^\n]");

    getc(da->f);
    if (x < mingrid || x > maxgrid || y < mingrid || y > maxgrid) {
      printf("point (%ld,%ld) is out of range%ld to %ld\n",
	     x, y, (long)mingrid, (long)maxgrid);
      halt();
    }


    data[x - mingrid][y - mingrid] = z;
  }
}


/* Local variables for grid: */
struct LOC_grid {
  _TEXT *afile;
  datagrid data;
  boolean dodiagonal;
  double xmagnify, ymagnify, zmagnify;
  screen s;

  double xscreen, yscreen;
  /*
*/
  double oldxscreen, oldyscreen;

  long x, y;
  double z, xold, yold;

  boolean changed;
  /*
*/

  double xhold, yhold;
} ;

Local Void drawit(LINK)
struct LOC_grid *LINK;
{
  LINK->z = LINK->data[LINK->x - mingrid][LINK->y - mingrid];
  if (LINK->z <= -LONG_MAX)
    return;
  if (!(LINK->dodiagonal && LINK->x == LINK->y || LINK->x != LINK->y))
    return;
  fprintf(LINK->afile->f, "%% (%3ld, %3ld, %10.5f)\n",
	  LINK->x, LINK->y, LINK->z);


  if (fabs(LINK->x - LINK->xold) > 1 || fabs(LINK->y - LINK->yold) > 1)
    LINK->changed = true;
  else
    LINK->changed = false;

  if (LINK->changed) {
    project3d(LINK->x * LINK->xmagnify, LINK->y * LINK->ymagnify, 0.0,
	      LINK->s, &LINK->xhold, &LINK->yhold);
    movea(LINK->afile, LINK->xhold, LINK->yhold);
    project3d(LINK->x * LINK->xmagnify, LINK->y * LINK->ymagnify, -1.0,
	      LINK->s, &LINK->xhold, &LINK->yhold);
    linea(LINK->afile, LINK->xhold, LINK->yhold);
    fprintf(LINK->afile->f, "gsave s (%ld) x grestore\n", LINK->x);
  }

  project3d(LINK->x * LINK->xmagnify, LINK->y * LINK->ymagnify,
	    LINK->z * LINK->zmagnify, LINK->s, &LINK->xscreen,
	    &LINK->yscreen);

  if (LINK->changed)
    movea(LINK->afile, LINK->xscreen, LINK->yscreen);
  else
    liner(LINK->afile, LINK->xscreen - LINK->oldxscreen,
	  LINK->yscreen - LINK->oldyscreen);

  LINK->oldxscreen = LINK->xscreen;
  LINK->oldyscreen = LINK->yscreen;
  LINK->xold = LINK->x;
  LINK->yold = LINK->y;
  LINK->changed = false;
}



Static Void grid(afile_, data_, dodiagonal_, fromrange, torange, xlocation,
		 ylocation, zlocation, magnify, xmagnify_, ymagnify_,
		 zmagnify_)
_TEXT *afile_;
double (*data_)[maxgrid - mingrid + 1];
boolean dodiagonal_;
double fromrange, torange, xlocation, ylocation, zlocation, magnify,
       xmagnify_, ymagnify_, zmagnify_;
{
  struct LOC_grid V;


  V.afile = afile_;
  memcpy(V.data, data_, sizeof(datagrid));
  V.dodiagonal = dodiagonal_;
  V.xmagnify = xmagnify_;
  V.ymagnify = ymagnify_;
  V.zmagnify = zmagnify_;
  makescreen(xlocation, ylocation, zlocation, -xlocation, -ylocation,
	     -zlocation, magnify, &V.s);


  project3d(fromrange * V.xmagnify, fromrange * V.ymagnify, 0.0, V.s,
	    &V.xscreen, &V.yscreen);
  movea(V.afile, V.xscreen, V.yscreen);

  project3d(fromrange * V.xmagnify, torange * V.ymagnify, 0.0, V.s,
	    &V.xscreen, &V.yscreen);
  linea(V.afile, V.xscreen, V.yscreen);

  project3d(fromrange * V.xmagnify, torange * V.ymagnify, 2.0, V.s, &V.xhold,
	    &V.yhold);
  linea(V.afile, V.xhold, V.yhold);
  fprintf(V.afile->f, "gsave s (%4.2f bits) x grestore\n",
	  2 / (V.zmagnify * magnify));
  linea(V.afile, V.xscreen, V.yscreen);

  project3d(torange * V.xmagnify, torange * V.ymagnify, 0.0, V.s, &V.xscreen,
	    &V.yscreen);
  linea(V.afile, V.xscreen, V.yscreen);

  project3d(fromrange * V.xmagnify, fromrange * V.ymagnify, 0.0, V.s,
	    &V.xscreen, &V.yscreen);
  linea(V.afile, V.xscreen, V.yscreen);

  V.xold = -LONG_MAX;
  V.yold = -LONG_MAX;

  for (V.x = mingrid; V.x <= maxgrid; V.x++) {
    for (V.y = mingrid; V.y <= maxgrid; V.y++)
      drawit(&V);
  }

  V.changed = true;

  for (V.y = mingrid; V.y <= maxgrid; V.y++) {
    for (V.x = mingrid; V.x <= maxgrid; V.x++)
      drawit(&V);
  }

}


/* Local variables for themain: */
struct LOC_themain {
  Char dd;
  boolean dodiagonal;
  double magnify, xmagnify, ymagnify, zmagnify, xlocation, ylocation,
	 zlocation;
  long datacolumn;
  double horizontal, vertical;
} ;

Local Void readda3d(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  fscanf(f->f, "%lg%*[^\n]", &LINK->horizontal);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->vertical);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->xlocation);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->ylocation);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->zlocation);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->magnify);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->xmagnify);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->ymagnify);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->zmagnify);
  getc(f->f);
  fscanf(f->f, "%ld%*[^\n]", &LINK->datacolumn);
  getc(f->f);
  if (LINK->datacolumn <= 4 || LINK->datacolumn > 15) {
    printf("data column must be >4 and < 16\n");
    halt();
  }
  fscanf(f->f, "%c%*[^\n]", &LINK->dd);
  getc(f->f);
  if (LINK->dd == '\n')
    LINK->dd = ' ';
  LINK->dodiagonal = (LINK->dd == 't');
}

Local Void writeda3d(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  fprintf(f->f, "%*.*f horizontal adjust (cm)\n",
	  picfield, picwidth, LINK->horizontal);
  fprintf(f->f, "%*.*f vertical adjust (cm)\n",
	  picfield, picwidth, LINK->vertical);
  fprintf(f->f, "% .*E xlocation (bases)\n",
	  P_max(picfield - 7, 1), LINK->xlocation);
  fprintf(f->f, "% .*E ylocation (bases)\n",
	  P_max(picfield - 7, 1), LINK->ylocation);
  fprintf(f->f, "% .*E zlocation (bases)\n",
	  P_max(picfield - 7, 1), LINK->zlocation);
  fprintf(f->f, "%*.*f magnify\n", picfield, picwidth, LINK->magnify);
  fprintf(f->f, "%*.*f xmagnify\n", picfield, picwidth, LINK->xmagnify);
  fprintf(f->f, "%*.*f ymagnify\n", picfield, picwidth, LINK->ymagnify);
  fprintf(f->f, "%*.*f zmagnify\n", picfield, picwidth, LINK->zmagnify);
  fprintf(f->f, "%*ld data column\n", picfield, LINK->datacolumn);
  if (LINK->datacolumn <= 4 || LINK->datacolumn > 15) {
    printf("data column must be >4 and < 16\n");
    halt();
  }
  fprintf(f->f, "%c dodiagonal\n", LINK->dd);
}





Static Void themain(da, da3dp, scene)
_TEXT *da, *da3dp, *scene;
{
  struct LOC_themain V;
  datagrid data;
  long fromrange, torange;
  double cmperinch;
  _TEXT TEMP;

  printf("da3d %4.2f\n", version);

  if (*scene->name != '\0') {
    if (scene->f != NULL)
      scene->f = freopen(scene->name, "w", scene->f);
    else
      scene->f = fopen(scene->name, "w");
  } else {
    if (scene->f != NULL)
      rewind(scene->f);
    else
      scene->f = tmpfile();
  }
  if (scene->f == NULL)
    _EscIO2(FileNotFound, scene->name);
  SETUPBUF(scene->f, Char);
  fprintf(scene->f, "%%! da3d %4.2f\n\n", version);
  if (*da3dp->name != '\0') {
    if (da3dp->f != NULL)
      da3dp->f = freopen(da3dp->name, "r", da3dp->f);
    else
      da3dp->f = fopen(da3dp->name, "r");
  } else {

    rewind(da3dp->f);
  }
  if (da3dp->f == NULL)
    _EscIO2(FileNotFound, da3dp->name);
  RESETBUF(da3dp->f, Char);
  if (BUFEOF(da3dp->f)) {
    test3d(scene);
    startpic(scene, 81.0, 3.0, 5.0, 'c');
  } else {
    readda3d(da3dp, &V);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeda3d(&TEMP, &V);

    cmperinch = 2.54;
    startpic(scene, 72.0, V.horizontal / cmperinch, V.vertical / cmperinch,
	     'c');

    readdata(da, V.datacolumn, &fromrange, &torange, data);

    grid(scene, data, V.dodiagonal, (double)fromrange, (double)torange,
	 V.xlocation, V.ylocation, V.zlocation, V.magnify, V.xmagnify,
	 V.ymagnify, V.zmagnify);

  }

  stoppic(scene);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  scene.f = NULL;
  strcpy(scene.name, "scene");
  da3dp.f = NULL;
  strcpy(da3dp.name, "da3dp");
  da.f = NULL;
  strcpy(da.name, "da");
  themain(&da, &da3dp, &scene);
_L1:
  if (da.f != NULL)
    fclose(da.f);
  if (da3dp.f != NULL)
    fclose(da3dp.f);
  if (scene.f != NULL)
    fclose(scene.f);
  exit(EXIT_SUCCESS);
}



/* End. */
