/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "tstrnd.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.07
/*
*/



/*




























*/



#define debugging       false


Static jmp_buf _JL1;



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define pow14           16384

#define pow15           32768L
#define pow22           4194304L
#define pow23           8388608L



Static Void randomDelila(seed)
double *seed;
{
  /*

*/

  /*











*/
  long iseed, i, nrep;

  iseed = (long)floor(*seed * pow23 + 0.5);
  if (iseed < 1 || iseed >= pow23)
    iseed = 1;
  nrep = (iseed & 7) + 4;
  for (i = 1; i <= nrep; i++) {
    if ((iseed & 1) == ((iseed & (pow15 - 1)) >= pow14))
      iseed /= 2;
    else
      iseed = iseed / 2 + pow22;
  }

  *seed = (double)iseed / pow23;
}

#undef pow14
#undef pow15
#undef pow22
#undef pow23


#define n               10000
#define tests           20

#define seed            4.0



Static Void randomtest(table)
_TEXT *table;
{
  /*


*/
  long test, call;
  double x = seed;
  double sumx, sumxsqd, mean, std;

  putc('\n', table->f);

  fprintf(table->f, " test of the random number generator:\n");
  fprintf(table->f, " std. dev is 1/sqrt(12); found by integrating:\n");
  fprintf(table->f, " from 0 to 1 of (x- mean)^2 dx, where the mean is 0.5\n");
  fprintf(table->f, " \n");
  fprintf(table->f, " \n\n");
  fprintf(table->f, "     mean     std dev\n");
  fprintf(table->f, "    (0.5000)  (0.2887)  (expected values)\n");
  fprintf(table->f, " ===================== %ld calls per test\n", (long)n);

  for (test = 1; test <= tests; test++) {
    sumx = 0.0;
    sumxsqd = 0.0;

    for (call = 1; call <= n; call++) {
      randomDelila(&x);
      sumx += x;
      sumxsqd += x * x;
    }

    mean = sumx / n;
    std = sqrt(n / (n - 1.0) * (sumxsqd / n - mean * mean));
    fprintf(table->f, " %10.4f%10.4f\n", mean, std);
  }

}

#undef n
#undef tests
#undef seed


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  printf(" test new random generator %4.2f\n", version);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  randomtest(&TEMP);

_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
