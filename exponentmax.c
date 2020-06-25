/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "exponentmax.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00
/*

*/
#define updateversion   1.00



/*











































*/



Static Void themain()
{
  long x;

  printf("exponentmax %4.2f\n", version);

  for (x = 1; x <= 1000; x++)
    printf("x = %4ld, exp(-x) = % .3E\n", x, exp((double)(-x)));

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  themain();
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
