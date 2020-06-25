/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "power.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00
/*

*/
#define updateversion   1.00



/*































































*/


Static _TEXT powerp;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(input, powerp, bfile)
_TEXT *input, *powerp, *bfile;
{
  double parameterversion, base;
  long wid, dec;
  double exponent;

  /*

*/
  if (*powerp->name != '\0') {
    if (powerp->f != NULL)
      powerp->f = freopen(powerp->name, "r", powerp->f);
    else
      powerp->f = fopen(powerp->name, "r");
  } else
    rewind(powerp->f);
  if (powerp->f == NULL)
    _EscIO2(FileNotFound, powerp->name);
  RESETBUF(powerp->f, Char);
  fscanf(powerp->f, "%lg%*[^\n]", &parameterversion);
  getc(powerp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }
  fscanf(powerp->f, "%lg%*[^\n]", &base);
  getc(powerp->f);
  fscanf(powerp->f, "%ld%*[^\n]", &wid);
  getc(powerp->f);
  fscanf(powerp->f, "%ld%*[^\n]", &dec);
  getc(powerp->f);

  if (base <= 0) {
    printf("The base to compute a power must be positive.\n");
    halt();
  }

  if (*input->name != '\0') {
    if (input->f != NULL)
      input->f = freopen(input->name, "r", input->f);
    else
      input->f = fopen(input->name, "r");
  } else
    rewind(input->f);
  if (input->f == NULL)
    _EscIO2(FileNotFound, input->name);
  RESETBUF(input->f, Char);
/* p2c: power.p, line 137: Note: REWRITE does not specify a name [181] */
/* p2c: power.p, line 137:
 * Note: RESET/REWRITE ignored for file OUTPUT [319] */
  if (stdout == NULL)
    _EscIO(FileNotFound);

  while (!P_eoln(input->f)) {
    fscanf(input->f, "%lg%*[^\n]", &exponent);
    getc(input->f);
    printf("%*.*f\n", (int)wid, (int)dec, exp(exponent * log(base)));
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  powerp.f = NULL;
  strcpy(powerp.name, "powerp");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &powerp, &TEMP1);
_L1:
  if (powerp.f != NULL)
    fclose(powerp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
