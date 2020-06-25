/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "decimals.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*


*/
#define updateversion   1.00



/*




































*/


Static _TEXT decimalsp;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(decimalsp)
_TEXT *decimalsp;
{
  double parameterversion;
  double x = 1.0;

  printf("decimals %4.2f\n", version);
  if (*decimalsp->name != '\0') {
    if (decimalsp->f != NULL)
      decimalsp->f = freopen(decimalsp->name, "r", decimalsp->f);
    else
      decimalsp->f = fopen(decimalsp->name, "r");
  } else
    rewind(decimalsp->f);
  if (decimalsp->f == NULL)
    _EscIO2(FileNotFound, decimalsp->name);
  RESETBUF(decimalsp->f, Char);
  fscanf(decimalsp->f, "%lg%*[^\n]", &parameterversion);
  getc(decimalsp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  while (x > 0) {
    printf("% .1E\n", x);
    x /= 2.0;
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  decimalsp.f = NULL;
  strcpy(decimalsp.name, "decimalsp");
  themain(&decimalsp);
_L1:
  if (decimalsp.f != NULL)
    fclose(decimalsp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
