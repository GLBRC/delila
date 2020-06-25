/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "log.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.02
/*



*/
#define updateversion   1.00



/*





























































*/




#define maxslots        20000
#define defslots        100
#define pageheight      53
/*


*/

#define dch             '+'
#define sch             ':'
#define bch             '*'

#define wid             10
#define dec             5


Static _TEXT histog, logp, xyin;


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



Static Void themain(histog, logp, xyin)
_TEXT *histog, *logp, *xyin;
{
  double logvalue, ln2 = log(2.0);
  double parameterversion, position, value;

  printf("* log %4.2f\n", version);
  if (*logp->name != '\0') {
    if (logp->f != NULL)
      logp->f = freopen(logp->name, "r", logp->f);
    else
      logp->f = fopen(logp->name, "r");
  } else
    rewind(logp->f);
  if (logp->f == NULL)
    _EscIO2(FileNotFound, logp->name);
  RESETBUF(logp->f, Char);
  fscanf(logp->f, "%lg%*[^\n]", &parameterversion);
  getc(logp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*histog->name != '\0') {
    if (histog->f != NULL)
      histog->f = freopen(histog->name, "r", histog->f);
    else
      histog->f = fopen(histog->name, "r");
  } else {

    rewind(histog->f);
  }
  if (histog->f == NULL)
    _EscIO2(FileNotFound, histog->name);
  RESETBUF(histog->f, Char);
  if (*xyin->name != '\0') {
    if (xyin->f != NULL)
      xyin->f = freopen(xyin->name, "w", xyin->f);
    else
      xyin->f = fopen(xyin->name, "w");
  } else {
    if (xyin->f != NULL)
      rewind(xyin->f);
    else
      xyin->f = tmpfile();
  }
  if (xyin->f == NULL)
    _EscIO2(FileNotFound, xyin->name);
  SETUPBUF(xyin->f, Char);
  fprintf(xyin->f, "* log %4.2f\n", version);

  while (!BUFEOF(histog->f)) {
    if (P_peek(histog->f) == '*') {
      copyaline(histog, xyin);
      continue;
    }

    fscanf(histog->f, "%lg%lg", &position, &value);

    if (value > 0)
      logvalue = log(value) / ln2;
    else
      logvalue = -LONG_MAX;

    /*



*/

    /*



*/

    fprintf(xyin->f, "%*.*f %*.*f %*.*f",
	    wid, dec, position, wid, dec, logvalue, wid, dec, value);

    /*







*/

    copyaline(histog, xyin);

    /*

*/

    /*


*/

  }


}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  logp.f = NULL;
  strcpy(logp.name, "logp");
  histog.f = NULL;
  strcpy(histog.name, "histog");
  themain(&histog, &logp, &xyin);
_L1:
  if (histog.f != NULL)
    fclose(histog.f);
  if (logp.f != NULL)
    fclose(logp.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}




























/* End. */
