/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "binplo.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.29



/*


















































*/


Static _TEXT xyin, xyplop, binplop;


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
  return (black * log(fraction) + white * log(1 - fraction));
}



Static double pbinomial(black, white, fraction)
long black, white;
double fraction;
{
  /*






*/
  long total;

  total = black + white;
  return exp(combination(black, white) + power(black, white, fraction));
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



Static Void makexyplop(x, black, white)
_TEXT *x;
long black, white;
{
  fprintf(x->f, "x 0 1     zx min max          \n");
  fprintf(x->f, "y 0 %10.8f zy min max          \n",
	  pbinomial(black, white, (double)black / (black + white)));
  fprintf(x->f, "20 20     xinterval yinterval \n");
  fprintf(x->f, "4 5       xwidth    ywidth    \n");
  fprintf(x->f, "2 3       xdecimal  ydecimal  \n");
  fprintf(x->f, "5.5 4     xsize     ysize     \n");
  fprintf(x->f, "fraction of black\n");
  fprintf(x->f, "p(b=%ld: w=%ld | fraction b)\n", black, white);
  fprintf(x->f, "n         no cross hairs\n");
  fprintf(x->f, "n         no log\n");
  fprintf(x->f, " --\n");
  fprintf(x->f, "1 2       xcolumn   ycolumn\n");
  fprintf(x->f, "0         symbol column\n");
  fprintf(x->f, "0 0       xscolumn   yscolumn\n");
  fprintf(x->f, " --\n");
  fprintf(x->f, "+         symbol-to-plot\n");
  fprintf(x->f, "+         symbol flag\n");
  fprintf(x->f, "0.01      symbol x size\n");
  fprintf(x->f, "0.01      symbol y size\n");
  fprintf(x->f, "cl 0.05   connected line  \n");
  fprintf(x->f, "n 0.05    no regression-line  \n");
  fprintf(x->f, " --\n");
  fprintf(x->f, ".\n");
  fprintf(x->f, " --\n");
}



Static Void themain(xyin, xyplop, binplop)
_TEXT *xyin, *xyplop, *binplop;
{
  long black, white;
  double fraction = 0.0;
  long index;
  double interval;
  long points;
  double p;

  printf("binplo %4.2f\n", version);

  if (*binplop->name != '\0') {
    if (binplop->f != NULL)
      binplop->f = freopen(binplop->name, "r", binplop->f);
    else
      binplop->f = fopen(binplop->name, "r");
  } else
    rewind(binplop->f);
  if (binplop->f == NULL)
    _EscIO2(FileNotFound, binplop->name);
  RESETBUF(binplop->f, Char);
  fscanf(binplop->f, "%ld%ld%*[^\n]", &black, &white);
  getc(binplop->f);
  fscanf(binplop->f, "%ld%*[^\n]", &points);
  getc(binplop->f);

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
  makexyplop(xyplop, black, white);

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
  fprintf(xyin->f, "* binplo %4.2f\n", version);
  fprintf(xyin->f, "* %ld intervals plotted\n", points);
  fprintf(xyin->f, "* black=%ld: white=%ld\n", black, white);

  if (points <= 0) {
    if (points == 0)
      tpbinom(black, white, (double)black / (black + white));
    else {
      printf(" you cannot have a negative number of points\n");
      halt();
    }
    return;
  }
  interval = 1.0 / points;
  for (index = 1; index <= points; index++) {
    fraction += interval;
    p = pbinomial(black, white, fraction);
    if ((long)(p * 1e8) > 0)
      fprintf(xyin->f, "%10.8f %10.8f\n", fraction, p);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  binplop.f = NULL;
  strcpy(binplop.name, "binplop");
  xyplop.f = NULL;
  strcpy(xyplop.name, "xyplop");
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  themain(&xyin, &xyplop, &binplop);
_L1:
  if (xyin.f != NULL)
    fclose(xyin.f);
  if (xyplop.f != NULL)
    fclose(xyplop.f);
  if (binplop.f != NULL)
    fclose(binplop.f);
  exit(EXIT_SUCCESS);
}



/* End. */
