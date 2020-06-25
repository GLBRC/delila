/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "doodle.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.97
/*


*/



/*

































*/



#define pi              3.14159265354

#define picfield        8
#define picwidth        5



#define maxstring       150



#define fillermax       20





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



Static double picxglobal, picyglobal, pictolerance;


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














































































Static Void startpic(afile)
_TEXT *afile;
{
  fprintf(afile->f, ".KF\n");
  fprintf(afile->f, ".PS\n");
  picxglobal = 0.0;
  picyglobal = 0.0;
  pictolerance = (long)(exp(picwidth * log(10.0)) + 0.5);

}



Static Void stoppic(afile)
_TEXT *afile;
{
  fprintf(afile->f, ".PE\n");
  fprintf(afile->f, ".KE\n");
}


Local boolean notzero(x)
double x;
{
  /*








*/
  return ((long)(fabs(x) * pictolerance) != 0);
  /*









*/
}



Static Void drawr(afile, dx, dy, visibility, spacing)
_TEXT *afile;
double dx, dy;
Char visibility;
double spacing;
{
  /*





*/
  boolean dxnotzero, dynotzero;

  dxnotzero = notzero(dx);
  dynotzero = notzero(dy);
  if (!(dxnotzero || dynotzero))
    return;
  if (visibility == 'i')
    fprintf(afile->f, "move");
  else
    fprintf(afile->f, "line");

  if (dynotzero) {
    fprintf(afile->f, " up %*.*fi", picfield, picwidth, dy);
    picyglobal += dy;
  }

  if (dxnotzero) {
    fprintf(afile->f, " right %*.*fi", picfield, picwidth, dx);
    picxglobal += dx;
  }

  if (visibility == '-')
    fprintf(afile->f, " dashed %*.*fi", picfield, picwidth, spacing);
  else if (visibility == '.')
    fprintf(afile->f, " dotted %*.*fi", picfield, picwidth, spacing);

  putc('\n', afile->f);
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



Static Void picnumber(afile, dx, dy, number, width, decimal)
_TEXT *afile;
double dx, dy, number;
long width, decimal;
{
  /*



*/
  if (width <= 0)
    return;
  mover(afile, dx, dy);
  if (decimal <= 0)
    fprintf(afile->f, "\"%*ld\"\n", (int)width, (long)floor(number + 0.5));
  else
    fprintf(afile->f, "\"%*.*f\"\n", (int)width, (int)decimal, number);
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
  picnumber(afile, dx, dy, number, width, decimal);
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
  picnumber(afile, dx, dy, number, width, decimal);
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
  double tic;

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

  tic = fromtic;
  if (interval > 0.0) {
    while (tic <= totic) {
      xtic(afile, length, dx, dy, tic, width, decimal);
      tic += interval;
      if (tic > totic)
	break;
      mover(afile, jump, 0.0);
      jumpdistance += jump;
    }
  } else if (interval < 0.0) {
    while (tic >= totic) {
      xtic(afile, length, dx, dy, tic, width, decimal);
      tic += interval;
      if (tic < totic)
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

  tic = fromtic;
  if (interval > 0.0) {
    while (tic <= totic) {
      ytic(afile, length, dx, dy, tic, width, decimal);
      tic += interval;
      if (tic > totic)
	break;
      mover(afile, 0.0, jump);
      jumpdistance += jump;
    }
  } else if (interval < 0.0) {
    while (tic >= totic) {
      ytic(afile, length, dx, dy, tic, width, decimal);
      tic += interval;
      if (tic < totic)
	break;
      mover(afile, 0.0, jump);
      jumpdistance += jump;
    }
  }

  mover(afile, 0.0, -jumpdistance);
}





Static Void dotr(afile)
_TEXT *afile;
{
  mover(afile, -0.0025, 0.0);
  drawr(afile, 0.0050, 0.0, 'l', 0.0);
  mover(afile, -0.0025, 0.0);
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



Static Void circler(afile, radius)
_TEXT *afile;
double radius;
{
  /*




*/
  double x, y;

  x = picxglobal;
  y = picyglobal;
  mover(afile, -radius, 0.0);
  fprintf(afile->f, "circle radius %*.*fi\n", picfield, picwidth, radius);
  mover(afile, -radius, 0.0);
  picxglobal = x;
  picyglobal = y;
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
  if (r < 0.025)
    r = 0.025;
  circler(afile, r);
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

  xpos = picxglobal;
  ypos = picyglobal;
  dr = thickness / steps;
  dtheta = 2 * pi / steps;

  while (r < radius) {
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
  LINK->xlo = between(LINK->ymin, fny(LINK->xmin, LINK), LINK->ymax, LINK);
  LINK->xhi = between(LINK->ymin, fny(LINK->xmax, LINK), LINK->ymax, LINK);
  LINK->ylo = between(LINK->xmin, fnx(LINK->ymin, LINK), LINK->xmax, LINK);
  LINK->yhi = between(LINK->xmin, fnx(LINK->ymax, LINK), LINK->xmax, LINK);

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
  long s;
  double theta, x, y, zerox, zeroy;

  zerox = picxglobal;
  zeroy = picyglobal;
  theta = degtorad(angle1);
  dtheta = degtorad(fabs(angle2 - angle1) / steps);
  polrec(radius, theta, &x, &y);
  movea(thefile, zerox + x, zeroy + y);

  for (s = 1; s <= steps; s++) {
    theta += dtheta;
    polrec(radius, theta, &x, &y);
    linea(thefile, zerox + x, zeroy + y);
  }

  movea(thefile, zerox, zeroy);
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


  double dr, dtheta;
  double r = 0.0, radius = 2.0, theta = 0.0, thickness = 0.1, steps = 15.0;

  double x = 0.0, y = 0.0;
  double z;

  makescreen(5.0, 5.0, 5.0, -1.0, -1.0, -1.0, 5.0, &s);

  dr = thickness / steps;
  dtheta = 2 * pi / steps;

  z = fun(r);
  project3d(x, y, z, s, &xscreen, &yscreen);
  movea(afile, xscreen, yscreen);

  while (r < radius) {
    r += dr;
    theta += dtheta;
    polrec(r, theta, &x, &y);

    z = fun(r);
    project3d(x, y, z, s, &xscreen, &yscreen);
    linea(afile, xscreen, yscreen);
  }
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
  fprintf(outfile->f, "doodle ");
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
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
  startpic(outfile);
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
  fscanf(infile->f, "%lg%lg%lg%ld%ld%*[^\n]", &dx, &dy, &number, &width,
	 &decimal);
  getc(infile->f);
  picnumber(outfile, dx, dy, number, width, decimal);
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



Static Void mktest3d(outfile)
_TEXT *outfile;
{
  test3d(outfile);
}


/* Local variables for translate: */
struct LOC_translate {
  _TEXT *infile, *outfile;
  stringDelila buffer;
  Char ch;
  boolean go;
  trigger pe;
  boolean picture;
  trigger ps;


  trigger halt, drawr, mover, liner, drawa, movea, linea, picnumber, xtic,
	  ytic, xaxis, yaxis, dotr, boxr, cboxr, ibeam, circler, spiral,
	  movepolar, arc, plusr, xr, test3d;
} ;

Local Void fill(LINK)
struct LOC_translate *LINK;
{
  filltrigger(&LINK->ps, ".PS                 ");
  filltrigger(&LINK->pe, ".PE                 ");

  filltrigger(&LINK->halt, "halt                ");
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
}

Local Void reset(LINK)
struct LOC_translate *LINK;
{
  resettrigger(&LINK->ps);
  resettrigger(&LINK->pe);

  resettrigger(&LINK->halt);
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
}

Local Void tests(LINK)
struct LOC_translate *LINK;
{
  /*

*/
  /*
*/
  if (LINK->picture) {
    if (LINK->go) {
      testfortrigger(LINK->ch, &LINK->pe);
      if (LINK->pe.found) {
	LINK->picture = false;
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
  }

  else {
    testfortrigger(LINK->ch, &LINK->ps);
    if (LINK->ps.found) {
      LINK->picture = true;
      mkstartpic(LINK->infile, LINK->outfile);
      LINK->go = false;
    }
  }

  if (!LINK->go)
    return;
  if (LINK->ch != ' ')
    return;
  writestring(LINK->outfile, &LINK->buffer);
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
  V.picture = false;


  while (!BUFEOF(V.infile->f)) {
    reset(&V);
    clearstring(&V.buffer);
    index = 0;
    V.go = true;
    while (V.go) {
      if (P_eoln(V.infile->f)) {
	fscanf(V.infile->f, "%*[^\n]");
	getc(V.infile->f);
	writestring(V.outfile, &V.buffer);
	putc('\n', V.outfile->f);
	V.go = false;
	continue;
      }

      if (index >= maxstring) {
	writestring(V.outfile, &V.buffer);
	clearstring(&V.buffer);
	index = 0;
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
  fprintf(tofile->f, ".\\\" doodle %4.2f\n", version);

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
