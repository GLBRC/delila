/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "binom.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.49


/*
*/



/*





































*/


Static _TEXT xyin, xyplop, binomp;


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

Local double power(black, white, P)
long black, white;
double P;
{
  /*
*/
  if (P >= 1.0)
    return 0.0;
  else
    return (black * log(P) + white * log(1 - P));
}



Static double pbinomial(black, white, P)
long black, white;
double P;
{
  /*







*/
  double sum;
  long total;

  total = black + white;
  sum = combination(black, white) + power(black, white, P);
  /*
*/
  if (sum < -50.0)
    return 0.0;
  else
    return exp(sum);
}



Static Void tpbinom(black, white, P)
long black, white;
double P;
{
  long index;
  double sum = 0.0;
  long total;

  printf("testing pbinomial procedure\n");
  total = black + white;
  for (index = 0; index <= total; index++)
    sum += pbinomial(index, total - index, P);
  printf("sum of p(b,w|P) for %ld balls is %10.8f\n", total, sum);
}



Static Void makexyplop(x, maxblack)
_TEXT *x;
long maxblack;
{
  fprintf(x->f, "1 2       zerox zeroy\n");
  fprintf(x->f, "x 0 %ld     zx min max          \n", maxblack);
  fprintf(x->f, "y 0 1 zy min max          \n");
  fprintf(x->f, "20 10 1  1  xinterval yinterval \n");
  fprintf(x->f, "4 5       xwidth    ywidth    \n");
  fprintf(x->f, "0 3       xdecimal  ydecimal  \n");
  fprintf(x->f, "6 6     xsize     ysize     \n");
  fprintf(x->f, "number of blacks\n");
  fprintf(x->f, "p(number of blacks)\n");

  fprintf(x->f, "a         no cross hairs\n");
  fprintf(x->f, "n         no log\n");
  fprintf(x->f, "n         no log\n");
  fprintf(x->f, " **********************************\n");
  fprintf(x->f, "1 2       xcolumn   ycolumn\n");
  fprintf(x->f, "0         symbol column\n");
  fprintf(x->f, "0 0       xscolumn   yscolumn\n");
  fprintf(x->f,
    "0 0 0     hue saturation brightness columns for color manipulation\n");
  fprintf(x->f, " **********************************\n");
  fprintf(x->f, "+         symbol-to-plot\n");
  fprintf(x->f, "+         symbol flag\n");
  fprintf(x->f, "0.01      symbol x size\n");
  fprintf(x->f, "0.01      symbol y size\n");
  fprintf(x->f, "cl 0.05   connected line  \n");
  fprintf(x->f, "n 0.05    no regression-line  \n");
  fprintf(x->f, " **********************************\n");
  fprintf(x->f, ".\n");
  fprintf(x->f, " **********************************\n");
}



Static Void themain(xyin, xyplop, binomp)
_TEXT *xyin, *xyplop, *binomp;
{
  long black, white;
  double P, Q, mean, pbin, sigma;
  long total;

  printf("binom %4.2f\n", version);

  if (*binomp->name != '\0') {
    if (binomp->f != NULL)
      binomp->f = freopen(binomp->name, "r", binomp->f);
    else
      binomp->f = fopen(binomp->name, "r");
  } else
    rewind(binomp->f);
  if (binomp->f == NULL)
    _EscIO2(FileNotFound, binomp->name);
  RESETBUF(binomp->f, Char);
  fscanf(binomp->f, "%ld%ld%*[^\n]", &black, &white);
  getc(binomp->f);
  fscanf(binomp->f, "%lg%*[^\n]", &P);
  getc(binomp->f);

  if (P < 0) {
    printf("probability P must be positive\n");
    halt();
  }

  if (P > 1) {
    printf("probability P must be less than 1.0\n");
    halt();
  }

  total = black + white;
  Q = 1 - P;
  mean = total * P;
  sigma = sqrt(total * P * Q);

  /*

*/

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

  fprintf(xyin->f, "* binom %4.2f\n", version);
  fprintf(xyin->f, "* black=%ld: white=%ld\n", black, white);
  fprintf(xyin->f, "* probability of black =%5.3f\n", P);
  fprintf(xyin->f, "* mean  = %4.2f\n", mean);
  fprintf(xyin->f, "* sigma = %4.2f\n", sigma);
  fprintf(xyin->f, "* Zblack = %4.2f\n", (black - mean) / sigma);

  /*



*/

  pbin = pbinomial(black, white, P);

  fprintf(xyin->f, "probability of this black/white combination: %10.8f\n",
	  pbin);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  binomp.f = NULL;
  strcpy(binomp.name, "binomp");
  xyplop.f = NULL;
  strcpy(xyplop.name, "xyplop");
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  themain(&xyin, &xyplop, &binomp);
_L1:
  if (xyin.f != NULL)
    fclose(xyin.f);
  if (xyplop.f != NULL)
    fclose(xyplop.f);
  if (binomp.f != NULL)
    fclose(binomp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
