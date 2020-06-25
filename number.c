/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "number.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.10


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



Static Void copyaline(fin, fout)
_TEXT *fin, *fout;
{
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
  putc('\n', fout->f);
}


/*

*/

#define decimals        1



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  long count = 0;

  /*


*/
  /*




















*/

  /*

*/
  while (!BUFEOF(fin->f)) {
    count++;
    fprintf(fout->f, "%ld ", count);
    copyaline(fin, fout);
  }

}

#undef decimals


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
