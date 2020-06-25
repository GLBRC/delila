/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "tkod.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.13


/*
*/



/*




























*/

Static _TEXT xyin, xyplop;


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



Static Void header(infile, xyin)
_TEXT *infile, *xyin;
{
  long x;

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
  fprintf(xyin->f, "* tkod %4.2f\n", version);
  for (x = 1; x <= 3; x++) {
    fprintf(xyin->f, "* ");
    copyaline(infile, xyin);
  }
  fprintf(xyin->f, "* data are:\n");
  fprintf(xyin->f, "* name, time (sec), od, exp(time/3600)\n");

}



Static Void rowcol(outfile, row, column)
_TEXT *outfile;
long row, column;
{
  /*
*/
  switch (column) {

  case 1:
    putc('a', outfile->f);
    break;

  case 2:
    putc('b', outfile->f);
    break;

  case 3:
    putc('c', outfile->f);
    break;

  case 4:
    putc('d', outfile->f);
    break;

  case 5:
    putc('e', outfile->f);
    break;

  case 6:
    putc('f', outfile->f);
    break;

  case 7:
    putc('g', outfile->f);
    break;

  case 8:
    putc('h', outfile->f);
    break;
  }
  fprintf(outfile->f, "%ld", row);
}




Static Void movedata(infile, outfile)
_TEXT *infile, *outfile;
{
  Char c;
  long column;
  double datum, etime;
  /*
*/
  long ro, row, time;

  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
  fscanf(infile->f, "%*[^\n]");


  getc(infile->f);
  fscanf(infile->f, "%c%ld%*[^\n]", &c, &time);
  getc(infile->f);

  if (c == '\n')
    c = ' ';
  /*
*/
  etime = exp(time / 3600.0);


  fscanf(infile->f, "%*[^\n]");


  getc(infile->f);
  for (row = 1; row <= 12; row++) {
    fscanf(infile->f, "%ld", &ro);
    if (ro != row) {
      printf(" bad row number: %ld seen where %ld expected\n", ro, row);
      _Escape(0);
    }

    for (column = 1; column <= 8; column++) {
      fscanf(infile->f, "%lg", &datum);


      rowcol(outfile, row, column);

      fprintf(outfile->f, " %10ld %6.3f %10.8f\n", time, datum, etime);
    }
    fscanf(infile->f, "%*[^\n]");
    getc(infile->f);
  }


  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
}



Static Void makexyplop(xyplop)
_TEXT *xyplop;
{
  long column, row;

  if (*xyplop->name != '\0') {
    if (xyplop->f != NULL)
      xyplop->f = freopen(xyplop->name, "w", xyplop->f);
    else
      xyplop->f = fopen(xyplop->name, "w");
  } else {
    if (xyplop->f != NULL)
      rewind(xyplop->f);
    else
      xyplop->f = tmpfile();
  }
  if (xyplop->f == NULL)
    _EscIO2(FileNotFound, xyplop->name);
  SETUPBUF(xyplop->f, Char);
  fprintf(xyplop->f, " x 0 5000  zx min max         \n");
  fprintf(xyplop->f, " y -0.1 0.1 zy min max        \n");
  fprintf(xyplop->f, "10 10     xinterval yinterval \n");
  fprintf(xyplop->f, "4 4       xwidth    ywidth    \n");
  fprintf(xyplop->f, "0 3       xdecimal  ydecimal  \n");
  fprintf(xyplop->f, "6 8       xsize     ysize     \n");
  fprintf(xyplop->f, "exp(time) (hours)\n");
  fprintf(xyplop->f, "OD 620\n");
  fprintf(xyplop->f, "n         zc                  \n");
  fprintf(xyplop->f, "n 2       zxl base            \n");
  fprintf(xyplop->f, "n 2       zyl base            \n");
  fprintf(xyplop->f, "          --------------------\n");
  fprintf(xyplop->f, "4 3       xcolumn   ycolumn   \n");
  fprintf(xyplop->f, "1         symbol column       \n");
  fprintf(xyplop->f, "0  0      xscolumn  yscolumn  \n");
  fprintf(xyplop->f, "          --------------------\n");

  for (row = 1; row <= 12; row++) {
    for (column = 1; column <= 8; column++) {
      fprintf(xyplop->f, "f         symbol to plot      \n");
      rowcol(xyplop, row, column);
      fprintf(xyplop->f, "        symbol flag         \n");
      fprintf(xyplop->f, "0.05      symbol sizex        \n");
      fprintf(xyplop->f, "0.05      symbol sizey        \n");
      fprintf(xyplop->f, "nl 0.05   no connection (examp\n");
      fprintf(xyplop->f, "l 0.05    linetype  size      \n");
      fprintf(xyplop->f, "          --------------------\n");
    }
  }
  fprintf(xyplop->f, ".\n");
  fprintf(xyplop->f, "          --------------------\n");
}



Static Void themain(xyin, xyplop)
_TEXT *xyin, *xyplop;
{
  _TEXT TEMP;

  printf("tkod %4.2f\n", version);

  TEMP.f = stdin;
  *TEMP.name = '\0';
  header(&TEMP, xyin);
  while (!P_eof(stdin)) {
    TEMP.f = stdin;
    *TEMP.name = '\0';
    movedata(&TEMP, xyin);
  }

  makexyplop(xyplop);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  xyplop.f = NULL;
  strcpy(xyplop.name, "xyplop");
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  themain(&xyin, &xyplop);
_L1:
  if (xyin.f != NULL)
    fclose(xyin.f);
  if (xyplop.f != NULL)
    fclose(xyplop.f);
  exit(EXIT_SUCCESS);
}



/* End. */
