/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rsconv.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.19
/*
*/
#define updateversion   1.08



/*














































































*/



#define infodec         5
#define infowid         10

#define maxposition     1000


typedef struct dataarray {
  /*





*/
  long rangefrom, rangeto, rangesize;
  double rsl[maxposition];
  double rs;
  /*
*/
} dataarray;




Static _TEXT rsdata1, rsdata2, rsconvp, rsconvmax, xyin;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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



Static Void cleararray(a)
dataarray *a;
{
  long position;

  for (position = 0; position < maxposition; position++)
    a->rsl[position] = -111111.0;
}



Static double maxarray(a)
dataarray a;
{
  long position;
  double max;

  for (position = 0; position < maxposition; position++) {
    if (max < a.rsl[position])
      max = a.rsl[position];
  }
  return max;
}


#define debug           false

#define width           20



Static Void findpeak(a, peaklocation, peakvalue)
dataarray a;
long *peaklocation;
double *peakvalue;
{
  /*

*/
  long position, test;
  boolean done = false;
  boolean changed;
  long FORLIM;

  position = 1 - a.rangefrom;
  *peakvalue = a.rsl[position-1];

  if (debug) {
    *peaklocation = position + a.rangefrom - 1;
    printf("initial peak location: %ld", *peaklocation);
    printf(" initial peak value: %*.*f\n", infowid, infodec, *peakvalue);
  }

  while (!done) {
    changed = false;

    FORLIM = position + width;
    for (test = position - width; test <= FORLIM; test++) {
      if (debug) {
	printf("   test position: %ld", test + a.rangefrom - 1);
	printf("   test value: %*.*f\n", infowid, infodec, a.rsl[test-1]);
      }
      if (a.rsl[test-1] > *peakvalue) {
	position = test;
	*peakvalue = a.rsl[position-1];
	changed = true;
      }
    }

    if (!changed)
      done = true;

    if (debug) {
      *peaklocation = position + a.rangefrom - 1;
      printf("peaklocation: %ld", *peaklocation);
      printf("   peakvalue: %*.*f\n", infowid, infodec, *peakvalue);
    }
  }

  *peaklocation = position + a.rangefrom - 1;
}

#undef debug
#undef width



Static Void readrsdata(r, anarray)
_TEXT *r;
dataarray *anarray;
{
  /*

*/
  boolean notstarted = true;
  long l, a, c, g, t;
  long position = 0;

  if (*r->name != '\0') {
    if (r->f != NULL)
      r->f = freopen(r->name, "r", r->f);
    else
      r->f = fopen(r->name, "r");
  } else
    rewind(r->f);
  if (r->f == NULL)
    _EscIO2(FileNotFound, r->name);
  RESETBUF(r->f, Char);
  cleararray(anarray);
  while (!BUFEOF(r->f)) {
    if (P_peek(r->f) == '*') {
      fscanf(r->f, "%*[^\n]");
      getc(r->f);
      continue;
    }
    position++;
    fscanf(r->f, "%ld%ld%ld%ld%ld%lg%*[^\n]", &l, &a, &c, &g, &t,
	   &anarray->rsl[position-1]);
    /*
*/
    getc(r->f);
    if (notstarted) {
      anarray->rangefrom = l;
      notstarted = false;
    }
    anarray->rs += anarray->rsl[position-1];
  }
  anarray->rangeto = l;
  anarray->rangesize = position;
  if (anarray->rangesize == anarray->rangeto - anarray->rangefrom + 1)
    return;
  printf("range size does not equal from-to size\n");
  printf("rangesize = %ld\n", anarray->rangesize);
  printf("rangefrom = %ld\n", anarray->rangefrom);
  printf("rangeto = %ld\n", anarray->rangeto);
}



Static Void writearray(r, startcharacter, anarray)
_TEXT *r;
Char startcharacter;
dataarray anarray;
{
  /*
*/
  long position, l;

  l = anarray.rangefrom;
  for (position = 0; position < anarray.rangesize; position++) {
    fprintf(r->f, "%c ", startcharacter);
    fprintf(r->f, "%ld %*.*f\n", l, infowid, infodec, anarray.rsl[position]);
    l++;
  }
}



Static double magnitude(anarray)
dataarray anarray;
{
  long position;
  double sum = 0.0;
  double TEMP;

  for (position = 0; position < anarray.rangesize; position++) {
    TEMP = anarray.rsl[position];
    sum += TEMP * TEMP;
  }
  return sqrt(sum);
}



Static long minimum(a, b)
long a, b;
{
  if (a < b)
    return a;
  else
    return b;
}



Static long maximum(a, b)
long a, b;
{
  if (a > b)
    return a;
  else
    return b;
}


#define debug           false



Static Void convolve(convolutionmethod, a, b, c)
Char convolutionmethod;
dataarray a, b, *c;
{
  /*
*/
  long aposition, bposition, cposition, offset, position;
  double ars, brs;
  /*
*/


  double x, y, sumx, sumy, sumxsqd, sumysqd, sumxy;

  double ex, ey, varx, vary, covxy;

  double r;

  double m, bintercept;

  long n, FORLIM, FORLIM1;

  /*


































*/

  c->rangefrom = a.rangefrom - b.rangeto;
  c->rangeto = a.rangeto - b.rangefrom;
  c->rangesize = c->rangeto - c->rangefrom + 1;

  if (debug || c->rangesize > maxposition) {
    printf("a.range from/to/size:%10ld%10ld%10ld\n",
	   a.rangefrom, a.rangeto, a.rangesize);
    printf("b.range from/to/size:%10ld%10ld%10ld\n",
	   b.rangefrom, b.rangeto, b.rangesize);
    printf("c.range from/to/size:%10ld%10ld%10ld\n",
	   c->rangefrom, c->rangeto, c->rangesize);
    if (c->rangesize > maxposition) {
      printf("Convolution results exceed available space.\n");
      printf("Increase constant maxposition from\n");
      printf("maxposition = %ld\n", (long)maxposition);
      printf("to at least %ld\n", c->rangesize);
      halt();
    }
  }

  FORLIM = c->rangeto;
  for (offset = c->rangefrom; offset <= FORLIM; offset++) {
    cposition = offset - c->rangefrom + 1;

    if (convolutionmethod == 'c') {
      regress('c', x, y, &sumx, &sumy, &sumxsqd, &sumysqd, &sumxy, &ex, &ey,
	      &varx, &vary, &covxy, &r, &m, &bintercept, &n);
      x = 0.0;
      y = 0.0;
    }

    if (debug)
      printf("offset = %5ld | ", offset);
    c->rs = 0.0;
    ars = 0.0;
    brs = 0.0;
    FORLIM1 = minimum(a.rangeto, offset + b.rangeto);
    for (position = maximum(a.rangefrom, offset + b.rangefrom); position <=
	   FORLIM1; position++) {
      if (debug)
	printf("%3ld", position);
      aposition = position - a.rangefrom + 1;
      bposition = position - offset - b.rangefrom + 1;

      c->rs += a.rsl[aposition-1] * b.rsl[bposition-1];

      ars += a.rsl[aposition-1] * a.rsl[aposition-1];

      brs += b.rsl[bposition-1] * b.rsl[bposition-1];

      if (convolutionmethod == 'c')
	regress('e', a.rsl[aposition-1], b.rsl[bposition-1], &sumx, &sumy,
		&sumxsqd, &sumysqd, &sumxy, &ex, &ey, &varx, &vary, &covxy,
		&r, &m, &bintercept, &n);
    }
/* p2c: rsconv.p, line 566:
 * Note: Evaluating FOR loop limit before initial value [315] */

    if (convolutionmethod == 'c') {
      if (n > 2)
	regress('r', a.rsl[aposition-1], b.rsl[bposition-1], &sumx, &sumy,
		&sumxsqd, &sumysqd, &sumxy, &ex, &ey, &varx, &vary, &covxy,
		&r, &m, &bintercept, &n);
      else
	r = 0.0;
      c->rsl[cposition-1] = r;
    } else
      c->rsl[cposition-1] = c->rs / sqrt(ars * brs);

    if (debug)
      putchar('\n');

  }
/* p2c: rsconv.p, line 582: Internal
 * error in checkvarchangedexpr: Too few arguments for EK_FUNCTION [267] */
/* p2c: rsconv.p, line 582: Internal
 * error in checkvarchangedexpr: Too few arguments for EK_FUNCTION [267] */
/* p2c: rsconv.p, line 582: Internal
 * error in checkvarchangedexpr: Too few arguments for EK_FUNCTION [267] */
}

#undef debug



Static Void themain(rsdata1, rsdata2, rsconvp, xyin, rsconvmax)
_TEXT *rsdata1, *rsdata2, *rsconvp, *xyin, *rsconvmax;
{
  Char convolutionmethod;
  double parameterversion;
  long peaklocation;
  double peakvalue;
  dataarray arrayA, arrayB, arrayC;

  printf("rsconv %4.2f\n", version);
  if (*rsconvp->name != '\0') {
    if (rsconvp->f != NULL)
      rsconvp->f = freopen(rsconvp->name, "r", rsconvp->f);
    else
      rsconvp->f = fopen(rsconvp->name, "r");
  } else
    rewind(rsconvp->f);
  if (rsconvp->f == NULL)
    _EscIO2(FileNotFound, rsconvp->name);
  RESETBUF(rsconvp->f, Char);
  fscanf(rsconvp->f, "%lg%*[^\n]", &parameterversion);
  getc(rsconvp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(rsconvp->f, "%c%*[^\n]", &convolutionmethod);
  getc(rsconvp->f);

  if (convolutionmethod == '\n')
    convolutionmethod = ' ';
  if (*xyin->name != '\0') {
    if (xyin->f != NULL)
      xyin->f = freopen(xyin->name, "w", xyin->f);
    else
      xyin->f = fopen(xyin->name, "w");
  } else {
    if (xyin->f != NULL)
      rewind(xyin->f);
    else
      xyin->f = tmpfile();
  }
  if (xyin->f == NULL)
    _EscIO2(FileNotFound, xyin->name);
  SETUPBUF(xyin->f, Char);
  fprintf(xyin->f, "* rsconv %4.2f\n", version);
  fprintf(xyin->f, "* %c: convolution method\n", convolutionmethod);
  fprintf(xyin->f, "* columns:\n");
  fprintf(xyin->f, "* 1: point definition: d(ata) p(eak)\n");
  fprintf(xyin->f, "* 2: coordinate\n");
  fprintf(xyin->f, "* 3: convolution value\n");

  readrsdata(rsdata1, &arrayA);
  readrsdata(rsdata2, &arrayB);

  cleararray(&arrayC);

  convolve(convolutionmethod, arrayA, arrayB, &arrayC);

  writearray(xyin, 'd', arrayC);

  findpeak(arrayC, &peaklocation, &peakvalue);

  if (*rsconvmax->name != '\0') {
    if (rsconvmax->f != NULL)
      rsconvmax->f = freopen(rsconvmax->name, "w", rsconvmax->f);
    else
      rsconvmax->f = fopen(rsconvmax->name, "w");
  } else {
    if (rsconvmax->f != NULL)
      rewind(rsconvmax->f);
    else
      rsconvmax->f = tmpfile();
  }
  if (rsconvmax->f == NULL)
    _EscIO2(FileNotFound, rsconvmax->name);
  SETUPBUF(rsconvmax->f, Char);
  fprintf(rsconvmax->f, "%*.*f %*ld\n",
	  infowid, infodec, peakvalue, infowid, peaklocation);
  /*

*/


  fprintf(xyin->f, "p %ld %*.*f\n", peaklocation, infowid, infodec, peakvalue);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  rsconvmax.f = NULL;
  strcpy(rsconvmax.name, "rsconvmax");
  rsconvp.f = NULL;
  strcpy(rsconvp.name, "rsconvp");
  rsdata2.f = NULL;
  strcpy(rsdata2.name, "rsdata2");
  rsdata1.f = NULL;
  strcpy(rsdata1.name, "rsdata1");
  themain(&rsdata1, &rsdata2, &rsconvp, &xyin, &rsconvmax);
_L1:
  if (rsdata1.f != NULL)
    fclose(rsdata1.f);
  if (rsdata2.f != NULL)
    fclose(rsdata2.f);
  if (rsconvp.f != NULL)
    fclose(rsconvp.f);
  if (rsconvmax.f != NULL)
    fclose(rsconvmax.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
