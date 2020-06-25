/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "ordcut.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.02
/*



*/
#define updateversion   1.00



/*























































*/


/*

*/
Static _TEXT ordcutp;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(ordcutp)
_TEXT *ordcutp;
{
  /*

*/
  Char c;
  long lowerbound = -LONG_MAX, upperbound = LONG_MAX;

  /*







*/


  lowerbound = 32;
  upperbound = 126;
  /*
*/

  /*

*/

  while (!P_eof(stdin)) {
    if (P_eoln(stdin)) {
      scanf("%*[^\n]");
      getchar();
      putchar('\n');
      continue;
    }
    c = getchar();
    if (c == '\n')
      c = ' ';
    if (c >= lowerbound && c <= upperbound || c == 9 || c == 12 || c == 13)
      putchar(c);
    else
      putchar(' ');
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  ordcutp.f = NULL;
  strcpy(ordcutp.name, "ordcutp");
  themain(&ordcutp);
_L1:
  if (ordcutp.f != NULL)
    fclose(ordcutp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
