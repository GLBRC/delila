/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "round.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.01
/*


*/
#define updateversion   1.00



/*






































*/


Static _TEXT fin, roundp, fout;


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



Static Void themain(fin, roundp, fout)
_TEXT *fin, *roundp, *fout;
{
  long decimals;
  double number, parameterversion;

  printf("round %4.2f\n", version);
  if (*roundp->name != '\0') {
    if (roundp->f != NULL)
      roundp->f = freopen(roundp->name, "r", roundp->f);
    else
      roundp->f = fopen(roundp->name, "r");
  } else
    rewind(roundp->f);
  if (roundp->f == NULL)
    _EscIO2(FileNotFound, roundp->name);
  RESETBUF(roundp->f, Char);
  fscanf(roundp->f, "%lg%*[^\n]", &parameterversion);
  getc(roundp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(roundp->f, "%ld%*[^\n]", &decimals);
  getc(roundp->f);

  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  RESETBUF(fin->f, Char);
  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);
  while (!BUFEOF(fin->f)) {
    fscanf(fin->f, "%lg", &number);
    fprintf(fout->f, "%*.*f ", (int)(decimals + 2), (int)decimals, number);
    copyaline(fin, fout);
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fout.f = NULL;
  strcpy(fout.name, "fout");
  roundp.f = NULL;
  strcpy(roundp.name, "roundp");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &roundp, &fout);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (roundp.f != NULL)
    fclose(roundp.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
