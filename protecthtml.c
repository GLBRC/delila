/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "protecthtml.p" */


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



Static Void themain(afile, bfile)
_TEXT *afile, *bfile;
{
  Char c;

  while (!BUFEOF(afile->f)) {
    if (P_eoln(afile->f)) {
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      putc('\n', bfile->f);
      continue;
    }
    c = getc(afile->f);
    if (c == '\n')
      c = ' ';
    if (c == '<') {
      fprintf(bfile->f, "&lt;");
      continue;
    }
    if (c == '>')
      fprintf(bfile->f, "&gt;");
    else if (c == '&')
      fprintf(bfile->f, "&amp;");
    else
      putc(c, bfile->f);
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
