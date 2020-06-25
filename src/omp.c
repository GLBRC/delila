/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "omp.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00
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
  double p;

  /*







*/

  rewind(stdin);
  while (!P_eof(stdin)) {
    if (P_peek(stdin) == '*') {
      scanf("%*[^\n]");
      getchar();
    } else {
      scanf("%lg%*[^\n]", &p);
      getchar();
      printf("%10.8f\n", 1.0 - p);
    }
  }

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
