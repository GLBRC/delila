/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "expgraph.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.07


/*
*/

main(argc, argv)
int argc;
Char *argv[];
{
  /*


*/



  /*




























































































































*/



  long d, divisions;
  double ln2 = log(2.0);
  double x, y;
  long rangeofx;
  double tolerance;
  long FORLIM;

  PASCAL_MAIN(argc, argv);
  printf("* expgraph %4.2f\n", version);

  printf("* number of divisions: \n");
  scanf("%ld%*[^\n]", &divisions);
  getchar();
  printf("* %ld\n", divisions);
  if (divisions < 2)
    divisions = 2;
  tolerance = 0.5 / divisions;

  printf("* range of x: \n");
  scanf("%ld%*[^\n]", &rangeofx);
  getchar();
  printf("* %ld\n", rangeofx);

  FORLIM = divisions * rangeofx;
  for (d = 0; d <= FORLIM; d++) {
    x = (double)d / divisions;

    y = exp(x * ln2);

    printf("n %10.8f %10.8f\n", x, y);

    /*
*/
    if ((long)(1 + x - tolerance) < (long)(1 + x))
      printf("p %10.8f %10.8f\n", x, y);
  }
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
