/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "derive.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*



*/
#define updateversion   1.00



/*















































*/


Static _TEXT xyin, derivep, dxyin;


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



Static Void getcolumns(xyin, column1, column2, x, y)
_TEXT *xyin;
long column1, column2;
double *x, *y;
{
  if (column1 != 1 && column2 != 2) {
    printf("COLUMNS NOT BEING READ PROPERLY YET\n");
    halt();
  }
  fscanf(xyin->f, "%lg%lg%*[^\n]", x, y);
  getc(xyin->f);
}



Static Void themain(xyin, derivep, dxyin)
_TEXT *xyin, *derivep, *dxyin;
{
  double parameterversion;
  long columnx = 1, columny = 2;
  double x0, y0, x1, y1, slope;
  boolean initializing = true;
  long xwidth = 15, xdecimal = 13, ywidth = 15, ydecimal = 13, swidth = 15,
       sdecimal = 13;

  printf("derive %4.2f\n", version);
  if (*derivep->name != '\0') {
    if (derivep->f != NULL)
      derivep->f = freopen(derivep->name, "r", derivep->f);
    else
      derivep->f = fopen(derivep->name, "r");
  } else
    rewind(derivep->f);
  if (derivep->f == NULL) {

    _EscIO2(FileNotFound, derivep->name);
  }
  RESETBUF(derivep->f, Char);
  fscanf(derivep->f, "%lg%*[^\n]", &parameterversion);
  getc(derivep->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*xyin->name != '\0') {
    if (xyin->f != NULL)
      xyin->f = freopen(xyin->name, "r", xyin->f);
    else
      xyin->f = fopen(xyin->name, "r");
  } else {

    rewind(xyin->f);
  }
  if (xyin->f == NULL)
    _EscIO2(FileNotFound, xyin->name);
  RESETBUF(xyin->f, Char);
  if (*dxyin->name != '\0') {
    if (dxyin->f != NULL)
      dxyin->f = freopen(dxyin->name, "w", dxyin->f);
    else
      dxyin->f = fopen(dxyin->name, "w");
  } else {
    if (dxyin->f != NULL)
      rewind(dxyin->f);
    else
      dxyin->f = tmpfile();
  }
  if (dxyin->f == NULL)
    _EscIO2(FileNotFound, dxyin->name);
  SETUPBUF(dxyin->f, Char);
  fprintf(dxyin->f, "* derive %4.2f\n", version);

  while (!BUFEOF(xyin->f)) {
    if (P_eoln(xyin->f)) {
      fscanf(xyin->f, "%*[^\n]");
      getc(xyin->f);
      putc('\n', dxyin->f);
      continue;
    }
    if (P_peek(xyin->f) == '*') {
      copyaline(xyin, dxyin);
      continue;
    }
    if (initializing) {
      getcolumns(xyin, columnx, columny, &x0, &y0);

      /*





*/
      initializing = false;
      fprintf(dxyin->f, "\n* dxyin columns: x, y, slope\n");
      continue;
    }
    getcolumns(xyin, columnx, columny, &x1, &y1);

    if (x1 - x0 == 0) {
      printf("x1 = x0 = %5.2f\n", x1);
      halt();
    }

    /*

*/
    slope = (log(y1) - log(y0)) / (log(x1) - log(x0));
    fprintf(dxyin->f, "%*.*f %*.*f %*.*f\n",
	    (int)xwidth, (int)xdecimal, x0, (int)ywidth, (int)ydecimal, y0,
	    (int)swidth, (int)sdecimal, slope);


    x0 = x1;
    y0 = y1;

  }


}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  dxyin.f = NULL;
  strcpy(dxyin.name, "dxyin");
  derivep.f = NULL;
  strcpy(derivep.name, "derivep");
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  themain(&xyin, &derivep, &dxyin);
_L1:
  if (xyin.f != NULL)
    fclose(xyin.f);
  if (derivep.f != NULL)
    fclose(derivep.f);
  if (dxyin.f != NULL)
    fclose(dxyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
