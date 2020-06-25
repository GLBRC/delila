/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "ic.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00


Static jmp_buf _JL1;


/*
*/



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
  double cm, inches;

  printf("ic %4.2f\n", version);
  printf("give inch values to get cm\n");
  while (!P_eoln(stdin)) {
    scanf("%lg%*[^\n]", &inches);
    getchar();
    cm = inches * 2.54;
    printf("for inches = %8.5f cm =%8.5f\n", inches, cm);
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
