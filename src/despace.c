/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "despace.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.03
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



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  Char previous = '-';

  /*

*/

  while (!BUFEOF(fin->f)) {
    while ((!P_eoln(fin->f)) & (!BUFEOF(fin->f))) {
      if (!((previous == ' ') & (P_peek(fin->f) == ' ')))
	putc(P_peek(fin->f), fout->f);
      previous = getc(fin->f);
    }
    putc('\n', fout->f);
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
    previous = '-';
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
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
