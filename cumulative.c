/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "cumulative.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.08


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


#define decimals        5
#define width           10



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  long count = 0;
  _TEXT hold;
  long lines = 0;

  /*

*/
  /*





















*/

  /*


*/


  hold.f = NULL;
  *hold.name = '\0';
  if (*hold.name != '\0') {
    if (hold.f != NULL)
      hold.f = freopen(hold.name, "w", hold.f);
    else
      hold.f = fopen(hold.name, "w");
  } else {
    if (hold.f != NULL)
      rewind(hold.f);
    else
      hold.f = tmpfile();
  }
  if (hold.f == NULL)
    _EscIO2(FileNotFound, hold.name);
  SETUPBUF(hold.f, Char);
  while (!BUFEOF(fin->f)) {
    lines++;
    copyaline(fin, &hold);
  }

  if (*hold.name != '\0') {
    if (hold.f != NULL)
      hold.f = freopen(hold.name, "r", hold.f);
    else
      hold.f = fopen(hold.name, "r");
  } else
    rewind(hold.f);
  if (hold.f == NULL)
    _EscIO2(FileNotFound, hold.name);
  RESETBUF(hold.f, Char);

  while (!BUFEOF(hold.f)) {
    count++;
    fprintf(fout->f, "%*ld %*.*f ",
	    width, count, width, decimals, (double)count / lines);
    copyaline(&hold, fout);
  }

  if (hold.f != NULL)
    fclose(hold.f);
}

#undef decimals
#undef width


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
