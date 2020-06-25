/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "domod.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.42
/*




*/



/*








































*/



#define maxstring       150



#define fillermax       20



#define pi              3.14159265354

#define picfield        12
#define picwidth        9

#define charwidth       0.05
/*
*/

#define defscale        81


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
} stringDelila;



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;



/*


*/
typedef Char filler[fillermax];



Static boolean inpicture;
/*
*/
Static double picxglobal, picyglobal, pictolerance;
/*
*/
Static double scale;


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


























































































Static Void startpic(afile, setscale, x, y)
_TEXT *afile;
double setscale, x, y;
{
  /*
*/
  fprintf(afile->f, ".PS %*.*f %*.*f %*.*f\n",
	  picfield, picwidth, setscale, picfield, picwidth, x, picfield,
	  picwidth, y);
  scale = setscale;

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

  /*

*/
}



Static Void stoppic(afile)
_TEXT *afile;
{
  fprintf(afile->f, ".PE\n");
  inpicture = false;
}



Static Void drawr(afile, dx, dy, visibility, spacing)
_TEXT *afile;
double dx, dy;
Char visibility;
double spacing;
{
  /*





*/
  fprintf(afile->f, "drawr %*.*f %*.*f %c %*.*f\n",
	  picfield, picwidth, dx, picfield, picwidth, dy, visibility,
	  picfield, picwidth, spacing);
  picxglobal += dx;
  picyglobal += dy;
}



Static Void mover(afile, dx, dy)
_TEXT *afile;
double dx, dy;
{
  fprintf(afile->f, "mover %*.*f %*.*f\n",
	  picfield, picwidth, dx, picfield, picwidth, dy);
  picxglobal += dx;
  picyglobal += dy;
}



Static Void liner(afile, dx, dy)
_TEXT *afile;
double dx, dy;
{
  fprintf(afile->f, "liner %*.*f %*.*f\n",
	  picfield, picwidth, dx, picfield, picwidth, dy);
  picxglobal += dx;
  picyglobal += dy;
}




Static Void drawa(afile, x, y, visibility, spacing)
_TEXT *afile;
double x, y;
Char visibility;
double spacing;
{
  /*





*/
  fprintf(afile->f, "drawa %*.*f %*.*f %c %*.*f\n",
	  picfield, picwidth, x, picfield, picwidth, y, visibility, picfield,
	  picwidth, spacing);
  picxglobal = x;
  picyglobal = y;
}



Static Void movea(afile, x, y)
_TEXT *afile;
double x, y;
{
  fprintf(afile->f, "movea %*.*f %*.*f\n",
	  picfield, picwidth, x, picfield, picwidth, y);
  picxglobal = x;
  picyglobal = y;
}



Static Void linea(afile, x, y)
_TEXT *afile;
double x, y;
{
  fprintf(afile->f, "linea %*.*f %*.*f\n",
	  picfield, picwidth, x, picfield, picwidth, y);
  picxglobal = x;
  picyglobal = y;
}



Static Void graphstring(tofile, s, centered)
_TEXT *tofile;
stringDelila *s;
boolean centered;
{
  /*


*/
  long i;
  boolean quoted;
  stringDelila sq;
  long FORLIM;

  if (s->length > 2) {
    if (s->letters[0] == '"' && s->letters[s->length - 1] == '"')
      quoted = true;
    else
      quoted = false;
  } else
    quoted = false;

  if (!quoted && centered) {
    clearstring(&sq);
    sq.length = s->length + 2;
    sq.letters[0] = '"';
    FORLIM = s->length;
    for (i = 1; i <= FORLIM; i++)
      sq.letters[i] = s->letters[i-1];
    sq.letters[sq.length - 1] = '"';
    writestring(tofile, &sq);
  } else
    writestring(tofile, s);
  putc('\n', tofile->f);
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
  fprintf(afile->f, "picnumber %*.*f %*.*f %*.*f %2ld %2ld",
	  picfield, picwidth, dx, picfield, picwidth, dy, picfield, picwidth,
	  number, width, decimal);
  if (centered)
    fprintf(afile->f, " true");
  else
    fprintf(afile->f, " false");
  putc('\n', afile->f);
}



Static Void xtic(afile, length, dx, dy, number, width, decimal)
_TEXT *afile;
double length, dx, dy, number;
long width, decimal;
{
  /*




*/
  fprintf(afile->f, "xtic %*.*f %*.*f %*.*f %*.*f %*ld %*ld\n",
	  picfield, picwidth, length, picfield, picwidth, dx, picfield,
	  picwidth, dy, picfield, picwidth, number, picfield, width, picfield,
	  decimal);
}



Static Void ytic(afile, length, dx, dy, number, width, decimal)
_TEXT *afile;
double length, dx, dy, number;
long width, decimal;
{
  /*




*/
  fprintf(afile->f, "ytic %*.*f %*.*f %*.*f %*.*f %*ld %*ld\n",
	  picfield, picwidth, length, picfield, picwidth, dx, picfield,
	  picwidth, dy, picfield, picwidth, number, picfield, width, picfield,
	  decimal);
}



Static Void xaxis(afile, axlength, fromtic, interval, totic, length, dx, dy,
		  width, decimal)
_TEXT *afile;
double axlength, fromtic, interval, totic, length, dx, dy;
long width, decimal;
{
  /*





*/
  fprintf(afile->f,
	  "xaxis %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*ld %*ld\n",
	  picfield, picwidth, axlength, picfield, picwidth, fromtic, picfield,
	  picwidth, interval, picfield, picwidth, totic, picfield, picwidth,
	  length, picfield, picwidth, dx, picfield, picwidth, dy, picfield,
	  width, picfield, decimal);
}



Static Void yaxis(afile, aylength, fromtic, interval, totic, length, dx, dy,
		  width, decimal)
_TEXT *afile;
double aylength, fromtic, interval, totic, length, dx, dy;
long width, decimal;
{
  /*





*/
  fprintf(afile->f,
	  "yaxis %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*ld %*ld\n",
	  picfield, picwidth, aylength, picfield, picwidth, fromtic, picfield,
	  picwidth, interval, picfield, picwidth, totic, picfield, picwidth,
	  length, picfield, picwidth, dx, picfield, picwidth, dy, picfield,
	  width, picfield, decimal);
}





Static Void dotr(afile)
_TEXT *afile;
{
  fprintf(afile->f, "dotr\n");
}



Static Void boxr(afile, width, height)
_TEXT *afile;
double width, height;
{
  /*



*/
  fprintf(afile->f, "boxr %*.*f %*.*f\n",
	  picfield, picwidth, width, picfield, picwidth, height);
}



Static Void cboxr(afile, width, height)
_TEXT *afile;
double width, height;
{
  /*



*/
  fprintf(afile->f, "cboxr %*.*f %*.*f\n",
	  picfield, picwidth, width, picfield, picwidth, height);
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
  fprintf(afile->f, "spiral %*.*f %*ld %*.*f\n",
	  picfield, picwidth, thickness, picfield, steps, picfield, picwidth,
	  radius);
}



Static Void movepolar(afile, angle, distance)
_TEXT *afile;
double angle, distance;
{
  /*

*/
  double x, y;

  fprintf(afile->f, "movepolar %*.*f %*.*f\n",
	  picfield, picwidth, angle, picfield, picwidth, distance);
  polrec(distance, angle, &x, &y);
  picxglobal += x;
  picyglobal += y;
}


/* Local variables for boxintercept: */
struct LOC_boxintercept {
  double xmin, ymin, xmax, ymax, m, b;
  boolean *intercept;
  double *x1, *y1, *x2, *y2;
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

Local boolean inside(a, b, c, LINK)
double a, b, c;
struct LOC_boxintercept *LINK;
{
  return (a < b && b < c);
}

Local Void normalcases(LINK)
struct LOC_boxintercept *LINK;
{
  /*






*/
  LINK->xlo = between(LINK->ymin, fny(LINK->xmin, LINK), LINK->ymax, LINK);
  LINK->xhi = between(LINK->ymin, fny(LINK->xmax, LINK), LINK->ymax, LINK);
  LINK->ylo = inside(LINK->xmin, fnx(LINK->ymin, LINK), LINK->xmax, LINK);
  LINK->yhi = inside(LINK->xmin, fnx(LINK->ymax, LINK), LINK->xmax, LINK);

  /*

*/

  *LINK->intercept = true;


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
  /*

*/
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
  fprintf(afile->f, "plusr %*.*f %*.*f\n",
	  picfield, picwidth, width, picfield, picwidth, height);
}



Static Void xr(afile, width, height)
_TEXT *afile;
double width, height;
{
  /*



*/
  fprintf(afile->f, "xr %*.*f %*.*f\n",
	  picfield, picwidth, width, picfield, picwidth, height);
}



Static Void arc(afile, angle1, angle2, radius, steps)
_TEXT *afile;
double angle1, angle2, radius;
long steps;
{
  /*


*/
  fprintf(afile->f, "arc %*.*f %*.*f %*.*f %*ld\n",
	  picfield, picwidth, angle1, picfield, picwidth, angle2, picfield,
	  picwidth, radius, picfield, steps);
}



Static Void circler(afile, radius)
_TEXT *afile;
double radius;
{
  fprintf(afile->f, "circler %*.*f\n", picfield, picwidth, radius);
}



Static Void ibeam(afile, width, height)
_TEXT *afile;
double width, height;
{
  /*



*/
  fprintf(afile->f, "ibeam %*.*f %*.*f\n",
	  picfield, picwidth, width, picfield, picwidth, height);
}



Static Void rectinit(afile)
_TEXT *afile;
{
  fprintf(afile->f, "rectinit\n");
}



Static Void rectsize(afile, xsideold, ysideold, xside, yside)
_TEXT *afile;
double *xsideold, *ysideold, *xside, *yside;
{
  /*

*/
  fprintf(afile->f, "rectsize %*.*f %*.*f %*.*f %*.*f\n",
	  picfield, picwidth, *xsideold, picfield, picwidth, *ysideold,
	  picfield, picwidth, *xside, picfield, picwidth, *yside);
}



Static Void rectdo(afile)
_TEXT *afile;
{
  /*
*/
  fprintf(afile->f, "rectdo\n");
}



Static Void setgray(afile, brightness)
_TEXT *afile;
double brightness;
{
  fprintf(afile->f, "setgray %*.*f\n", picfield, picwidth, brightness);
}



Static Void setcolor(afile, hue, saturation, brightness)
_TEXT *afile;
double hue, saturation, brightness;
{
  fprintf(afile->f, "setcolor %*.*f %*.*f %*.*f\n",
	  picfield, picwidth, hue, picfield, picwidth, saturation, picfield,
	  picwidth, brightness);
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
  fprintf(afile->f, "test3d\n");
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
  fprintf(outfile->f, "domod ");
  halt();
}


/* Local variables for testblank: */
struct LOC_testblank {
  _TEXT *outfile;
} ;

Local Void die(LINK)
struct LOC_testblank *LINK;
{
  stoppic(LINK->outfile);
  fprintf(LINK->outfile->f, "badly formed instruction\n");
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
  startpic(outfile, scale, x, y);
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
/* p2c: domod.p: Note: Eliminated unused assignment statement [338] */
  getc(infile->f);
  picnumber(outfile, dx, dy, number, width, decimal, true);
}



Static Void mkxtic(infile, outfile)
_TEXT *infile, *outfile;
{
  double length, dx, dy, number;
  long width, decimal;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%lg%ld%ld%*[^\n]", &length, &dx, &dy, &number,
	 &width, &decimal);
  getc(infile->f);
  xtic(outfile, length, dx, dy, number, width, decimal);
}



Static Void mkytic(infile, outfile)
_TEXT *infile, *outfile;
{
  double length, dx, dy, number;
  long width, decimal;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%lg%ld%ld%*[^\n]", &length, &dx, &dy, &number,
	 &width, &decimal);
  getc(infile->f);
  ytic(outfile, length, dx, dy, number, width, decimal);
}



Static Void mkxaxis(infile, outfile)
_TEXT *infile, *outfile;
{
  double axlength, fromtic, interval, totic, length, dx, dy;
  long width, decimal;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%lg%lg%lg%lg%ld%ld%*[^\n]", &axlength, &fromtic,
	 &interval, &totic, &length, &dx, &dy, &width, &decimal);
  getc(infile->f);
  xaxis(outfile, axlength, fromtic, interval, totic, length, dx, dy, width,
	decimal);
}



Static Void mkyaxis(infile, outfile)
_TEXT *infile, *outfile;
{
  double aylength, fromtic, interval, totic, length, dx, dy;
  long width, decimal;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%lg%lg%lg%lg%lg%ld%ld%*[^\n]", &aylength, &fromtic,
	 &interval, &totic, &length, &dx, &dy, &width, &decimal);
  getc(infile->f);
  yaxis(outfile, aylength, fromtic, interval, totic, length, dx, dy, width,
	decimal);
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



Static Void mkrectinit(infile, outfile)
_TEXT *infile, *outfile;
{
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
  rectinit(outfile);
}



Static Void mkrectsize(infile, outfile)
_TEXT *infile, *outfile;
{
  double xside, yside;

  testblank(infile, outfile);
  fscanf(infile->f, "%lg%lg%*[^\n]", &xside, &yside);
  getc(infile->f);
  fprintf(outfile->f, "mkrectsize %*.*f %*.*f\n",
	  picfield, picwidth, xside, picfield, picwidth, yside);
}



Static Void mkrectdo(infile, outfile)
_TEXT *infile, *outfile;
{
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
  rectdo(outfile);
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



Static Void mktest3d(infile, outfile)
_TEXT *infile, *outfile;
{
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
  test3d(outfile);
}


/* Local variables for translate: */
struct LOC_translate {
  _TEXT *infile, *outfile;
  stringDelila buffer;
  Char ch;
  boolean go;
  trigger pe, ps;


  trigger halt, demo, drawr, mover, liner, drawa, movea, linea, picnumber,
	  xtic, ytic, xaxis, yaxis, dotr, boxr, cboxr, ibeam, circler, spiral,
	  movepolar, arc, plusr, xr, rectinit, rectsize, rectdo, setgray,
	  setcolor, test3d;
} ;

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
  filltrigger(&LINK->rectinit, "rectinit            ");
  filltrigger(&LINK->rectsize, "rectsize            ");
  filltrigger(&LINK->rectdo, "rectdo              ");
  filltrigger(&LINK->setgray, "setgray             ");
  filltrigger(&LINK->setcolor, "setcolor            ");
  filltrigger(&LINK->test3d, "test3d              ");
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
  resettrigger(&LINK->rectinit);
  resettrigger(&LINK->rectsize);
  resettrigger(&LINK->rectdo);
  resettrigger(&LINK->setgray);
  resettrigger(&LINK->setcolor);
  resettrigger(&LINK->test3d);
}

Local Void tests(LINK)
struct LOC_translate *LINK;
{
  /*

*/
  /*
*/
  if (inpicture) {
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->pe);
      if (LINK->pe.found) {
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
	printf("demo\n");
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
      testfortrigger(LINK->ch, &LINK->rectinit);
      if (LINK->rectinit.found) {
	mkrectinit(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->rectdo);
      if (LINK->rectdo.found) {
	mkrectdo(LINK->infile, LINK->outfile);
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
      testfortrigger(LINK->ch, &LINK->test3d);
      if (LINK->test3d.found) {
	mktest3d(LINK->infile, LINK->outfile);
	LINK->go = false;
      }
    }
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->ps);
      if (LINK->ps.found) {
	printf(".PS ignored: already in picture\n");
	LINK->go = false;
      }
    }
  } else {
    testfortrigger(LINK->ch, &LINK->ps);
    if (LINK->ps.found) {
      mkstartpic(LINK->infile, LINK->outfile);
      LINK->go = false;
    }
  }

  if (!LINK->go)
    return;
  if (LINK->ch != ' ')
    return;
  graphstring(LINK->outfile, &LINK->buffer, false);
  copyaline(LINK->infile, LINK->outfile);
  LINK->go = false;
}



Static Void translate(infile_, outfile_)
_TEXT *infile_, *outfile_;
{
  /*
*/
  struct LOC_translate V;
  long index;

  V.infile = infile_;
  V.outfile = outfile_;
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
	  graphstring(V.outfile, &V.buffer, false);
	else {
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



Static Void themain(fromfile, tofile)
_TEXT *fromfile, *tofile;
{
  translate(fromfile, tofile);
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
