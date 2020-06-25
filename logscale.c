/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "logscale.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*



*/
#define updateversion   1.00


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



Static Void themain()
{
  double x;
  long b, n, m;
  double z;
  long w, d;
  double result;

  scanf("%lg%ld%ld%ld%ld%ld%*[^\n]", &x, &b, &n, &m, &w, &d);
  getchar();

  if (b < 1)
    printf("base of scale must be greater than 1\n");

  if (w < 0)
    printf("width must be positive\n");

  if (d < 0)
    printf("decimals must be positive\n");


  if (n < 0) {
    printf("logscale %4.2f\n", version);
    return;
  }
  /*

*/
  z = exp(log((double)b) / n);


  result = x * exp(m * log(z));

  printf("%*.*f\n", (int)w, (int)d, result);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  themain();
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
