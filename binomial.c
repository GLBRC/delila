/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "binomial.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.50


/*



*/



/*




















































































*/


Static _TEXT xyin, xyplop, binomialp;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Local double combination(black, white)
long black, white;
{
  /*
*/
  long index;
  double sum = 0.0;

  /*
*/
  for (index = black + 1; index <= black + white; index++)
    sum += log((double)index);

  for (index = 1; index <= white; index++)
    sum -= log((double)index);
  return sum;
}

Local double power(black, white, fraction)
long black, white;
double fraction;
{
  /*
*/
  if (fraction >= 1.0)
    return 0.0;
  else
    return (black * log(fraction) + white * log(1 - fraction));
}



Static double pbinomial(black, white, fraction)
long black, white;
double fraction;
{
  /*






*/
  double sum;
  long total;

  total = black + white;
  sum = combination(black, white) + power(black, white, fraction);
  /*
*/
  if (sum < -50.0)
    return 0.0;
  else
    return exp(sum);
}



Static Void tpbinom(black, white, fraction)
long black, white;
double fraction;
{
  long index;
  double sum = 0.0;
  long total;

  printf("testing pbinomial procedure\n");
  total = black + white;
  for (index = 0; index <= total; index++)
    sum += pbinomial(index, total - index, fraction);
  printf("sum of p(b,w|fraction) for %ld balls is %10.8f\n", total, sum);
}



Static Void makexyplop(x, maxwhite)
_TEXT *x;
long maxwhite;
{
  long xintervals;

  if (maxwhite > 20)
    xintervals = 10;
  else
    xintervals = maxwhite;
  fprintf(x->f, "3 6       zerox zeroy\n");
  fprintf(x->f, "zx 0 %ld     zx min max          \n", maxwhite);
  fprintf(x->f, "zy 0 1 zy min max          \n");
  fprintf(x->f, "%ld 10 1 1 xinterval yinterval \n", xintervals);
  fprintf(x->f, "6 6       xwidth    ywidth    \n");
  fprintf(x->f, "0 3       xdecimal  ydecimal  \n");
  fprintf(x->f, "15 15   xsize     ysize     \n");
  fprintf(x->f, "number of white\n");
  fprintf(x->f, "p(number of white)\n");

  fprintf(x->f, "a         no cross hairs\n");
  fprintf(x->f, "n 2       no log\n");
  fprintf(x->f, "n 2       no log\n");
  fprintf(x->f, " *-\n");
  fprintf(x->f, "1 2       xcolumn   ycolumn\n");
  fprintf(x->f, "0         symbol column\n");
  fprintf(x->f, "0 0       xscolumn   yscolumn\n");
  fprintf(x->f,
    "0 0 0    hue saturation brightness columns for color manipulation\n");
  fprintf(x->f, " *-\n");
  fprintf(x->f, "c         symbol-to-plot\n");
  fprintf(x->f, "o         symbol flag\n");
  fprintf(x->f, "0.250     symbol x size\n");
  fprintf(x->f, "0.250     symbol y size\n");
  fprintf(x->f, "cl 0.05   connected line  \n");
  fprintf(x->f, "n 0.05    no regression-line  \n");
  fprintf(x->f, " *-\n");
  fprintf(x->f, ".\n");
  fprintf(x->f, " *-\n");
  fprintf(x->f, " *-\n");
  fprintf(x->f,
    "p 1.50 0.50 1.50 1.00 edgecontrol (p=page), edgeleft, edgeright, edgelow, edgehigh in cm\n");
  fprintf(x->f,
	  "9.01 version of xyplo that this parameter file is designed for.\n");
}



Static Void themain(xyin, xyplop, binomialp)
_TEXT *xyin, *xyplop, *binomialp;
{
  double cumulative = 0.0;
  long black, white;
  double p, q;
  long maxwhite;
  double mean, pbin, sigma;
  long n;

  printf("binomial %4.2f\n", version);

  if (*binomialp->name != '\0') {
    if (binomialp->f != NULL)
      binomialp->f = freopen(binomialp->name, "r", binomialp->f);
    else
      binomialp->f = fopen(binomialp->name, "r");
  } else
    rewind(binomialp->f);
  if (binomialp->f == NULL)
    _EscIO2(FileNotFound, binomialp->name);
  RESETBUF(binomialp->f, Char);
  fscanf(binomialp->f, "%ld%*[^\n]", &n);
  getc(binomialp->f);
  fscanf(binomialp->f, "%lg%*[^\n]", &p);
  getc(binomialp->f);
  fscanf(binomialp->f, "%ld%*[^\n]", &maxwhite);
  getc(binomialp->f);

  printf("n = %ld\n", n);
  printf("p = %10.8f\n", p);
  printf("maxwhite = %ld\n", maxwhite);

  if ((unsigned)p > 1) {
    printf("probability must be between 0 and 1.\n");
    halt();
  }

  if (black < 0) {
    printf("there cannot be negative black balls\n");
    halt();
  }

  if (white < 0) {
    printf("there cannot be negative white balls\n");
    halt();
  }

  /*


*/

  if (n < 0) {
    printf("there must be a positive number of balls\n");
    halt();
  }

  q = 1 - p;
  mean = n * p;
  sigma = sqrt(n * p * q);
  if (maxwhite > n)
    maxwhite = n;

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
  makexyplop(xyplop, maxwhite);

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
  fprintf(xyin->f, "* binomial %4.2f\n", version);
  fprintf(xyin->f, "* n=%ld\n", n);
  fprintf(xyin->f, "* probability of white = %5.3f\n", p);
  fprintf(xyin->f, "* highest white value  = %4ld\n", maxwhite);
  fprintf(xyin->f, "* -----\n");
  fprintf(xyin->f, "* mean  = %4.2f\n", mean);
  fprintf(xyin->f, "* sigma = %4.2f\n", sigma);
  fprintf(xyin->f, "* column 1: number of white balls\n");
  fprintf(xyin->f,
    "* column 2: probability of withdrawing that number white balls in\n");
  fprintf(xyin->f, "*           %ld black and white balls total\n", n);
  fprintf(xyin->f,
    "*           given a probability of %5.3f white balls in the population\n",
    p);
  fprintf(xyin->f, "* column 3: cumulative sum of column 2\n");

  printf("                  mean = %5.2f\n", mean);
  printf("                 sigma = %5.2f\n", sigma);
  printf("sharpness = mean/sigma = %5.2f\n", mean / sigma);

  for (white = 0; white <= maxwhite; white++) {
    black = n - white;
    pbin = pbinomial(black, white, p);
    cumulative = pbin + cumulative;
    fprintf(xyin->f, "%8ld %15.13f %15.13f\n", white, pbin, cumulative);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  binomialp.f = NULL;
  strcpy(binomialp.name, "binomialp");
  xyplop.f = NULL;
  strcpy(xyplop.name, "xyplop");
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  themain(&xyin, &xyplop, &binomialp);
_L1:
  if (xyin.f != NULL)
    fclose(xyin.f);
  if (xyplop.f != NULL)
    fclose(xyplop.f);
  if (binomialp.f != NULL)
    fclose(binomialp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
