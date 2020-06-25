/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "column.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.06


/*



*/



/*








































*/


Static _TEXT columnp;


Static jmp_buf _JL1;


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



Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) == ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) != ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}



Static Void copynonblanks(infile, outfile)
_TEXT *infile, *outfile;
{
  skipblanks(infile);
  while ((P_peek(infile->f) != ' ') & (!P_eoln(infile->f))) {
    putc(P_peek(infile->f), outfile->f);
    getc(infile->f);
  }
}



Static Void themain(fin, columnp, fout)
_TEXT *fin, *columnp, *fout;
{
  long c, whichone;

  /*

*/
  if (*columnp->name != '\0') {
    if (columnp->f != NULL)
      columnp->f = freopen(columnp->name, "r", columnp->f);
    else
      columnp->f = fopen(columnp->name, "r");
  } else
    rewind(columnp->f);
  if (columnp->f == NULL)
    _EscIO2(FileNotFound, columnp->name);
  RESETBUF(columnp->f, Char);
  fscanf(columnp->f, "%ld%*[^\n]", &whichone);
  getc(columnp->f);
  /*





*/

  while (!BUFEOF(fin->f)) {
    if (P_peek(fin->f) == '*') {
      copyaline(fin, fout);
      continue;
    }
    c = 1;
    skipblanks(fin);
    while (c < whichone) {
      skipnonblanks(fin);
      skipblanks(fin);
      c++;
    }
    /*


*/
    copynonblanks(fin, fout);
    putc('\n', fout->f);
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
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
  columnp.f = NULL;
  strcpy(columnp.name, "columnp");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &columnp, &TEMP1);
_L1:
  if (columnp.f != NULL)
    fclose(columnp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
