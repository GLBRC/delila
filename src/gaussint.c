/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "gaussint.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*














*/



#define version         1.70


/*



*/



/*


































*/


Static _TEXT gout;


Static jmp_buf _JL1;



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}




Static Void intgauss(lower, upper, answer, tol)
double lower, upper, *answer, *tol;
{
  /*





*/
  double deltax;
  /*
*/
  double endcor, endsum;
  double evensum = 0.0;
  /*
*/
  long i;
  double oddsum, pi = 4.0 * atan(1.0);
  long pieces = 2;
  double prevsum;
  /*

*/
  double temp;
  /*
*/
  double sum = 0.0;
  double val;
  /*

*/
  double x;

  /*
*/
  val = 1 / sqrt(2 * pi);
  *tol = 1.0 / LONG_MAX;
  if (upper < lower) {
    temp = lower;
    lower = upper;
    upper = temp;
  }
  deltax = (upper - lower) / pieces;
  x = lower + deltax;
  oddsum = val * exp(-0.5 * x * x);
  endsum = val * exp(-0.5 * lower * lower) + val * exp(-0.5 * upper * upper);
  endcor = upper * val * exp(-0.5 * upper * upper) -
	   lower * val * exp(-0.5 * lower * lower);
  sum = (endsum + 4.0 * oddsum) * deltax / 3.0;

  do {
    pieces *= 2;
    prevsum = sum;
    deltax = (upper - lower) / pieces;
    evensum += oddsum;
    oddsum = 0.0;
    for (i = 1; i <= pieces / 2; i++) {
      x = lower + deltax * (2.0 * i - 1.0);
      oddsum += val * exp(-0.5 * x * x);
    }
    sum = (7.0 * endsum + 14.0 * evensum + 16.0 * oddsum + endcor * deltax) *
	  deltax / 15.0;
  } while (fabs(sum - prevsum) > fabs(*tol * sum));
  *answer = sum;
}




Static Void probgauss(zvalue, onetail, tol)
double zvalue, *onetail, *tol;
{
  /*




*/
  intgauss(0.0, zvalue, onetail, tol);
  *onetail = 0.5 - *onetail;
}



Static Void gausstest(gout)
_TEXT *gout;
{
  double tol, onetail, upper;
  long i, j;

  if (*gout->name != '\0') {
    if (gout->f != NULL)
      gout->f = freopen(gout->name, "w", gout->f);
    else
      gout->f = fopen(gout->name, "w");
  } else {
    if (gout->f != NULL)
      rewind(gout->f);
    else
      gout->f = tmpfile();
  }
  if (gout->f == NULL)
    _EscIO2(FileNotFound, gout->name);
  SETUPBUF(gout->f, Char);
  fprintf(gout->f, "gaussint %4.2f\n", version);
  printf("gaussint %4.2f\n", version);
  fprintf(gout->f,
	  "\nValues of One-Tail integration of the Normal Distribution\n");
  fprintf(gout->f, "Integral is from z to infinity\n\n\n");
  fprintf(gout->f, " z |   0");
  for (i = 1; i <= 9; i++)
    fprintf(gout->f, "%7ld", i);
  putc('\n', gout->f);
  for (i = 1; i <= 74; i++)
    putc('-', gout->f);
  putc('\n', gout->f);

  i = 300;
  do {
    fprintf(gout->f, "%3.1f| ", i / 100.0);
    for (j = 0; j <= 9; j++) {
      upper = (i + j) / 100.0;
      probgauss(upper, &onetail, &tol);
      fprintf(gout->f, "%6.4f ", onetail);
    }
    putc('\n', gout->f);
    i -= 10;
  } while (i >= 0);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  gout.f = NULL;
  strcpy(gout.name, "gout");
  gausstest(&gout);
  printf("done\n");
_L1:
  if (gout.f != NULL)
    fclose(gout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
