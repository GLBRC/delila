/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "digrab.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.04


/*
*/



/*































*/


Static _TEXT ii, xyin, xyplop;


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



Static Void makexyplop(xyplop, distance)
_TEXT *xyplop;
long distance;
{
  fprintf(xyplop->f,
	  "2 2       zerox zeroy         graph coordinate center\n");
  fprintf(xyplop->f,
    "x -50 50  zx min max          (char, real, real) if zx=x then set xaxis\n");
  fprintf(xyplop->f,
    "y 0 0.1   zy min max          (char, real, real) if zy=y then set yaxis\n");
  fprintf(xyplop->f,
    "10 10     xinterval yinterval number of intervals on axes to plot\n");
  fprintf(xyplop->f,
	  "4 6       xwidth    ywidth    width of numbers in characters\n");
  fprintf(xyplop->f,
	  "0 2       xdecimal  ydecimal  number of decimal places\n");
  fprintf(xyplop->f, "5 5       xsize     ysize     size of axes in inches\n");
  fprintf(xyplop->f, "position x in (x,x+%ld)\n", distance);
  fprintf(xyplop->f, "bits\n");
  fprintf(xyplop->f, "a         zc               c crosshairs, axXyYnN\n");
  fprintf(xyplop->f,
    "n 2       zxl base         if zxl=l then make x axis log to the given base\n");
  fprintf(xyplop->f,
    "n 2       zyl base         if zyl=l then make y axis log to the given base\n");
  fprintf(xyplop->f,
    "          ******************************************************************\n");
  fprintf(xyplop->f,
    "3 8       xcolumn   ycolumn   columns of xyin that determine plot location\n");
  fprintf(xyplop->f,
    "0         symbol column       the xyin column to read symbols from\n");
  fprintf(xyplop->f,
    "0  0      xscolumn  yscolumn  columns of xyin that determine the symbol size\n");
  fprintf(xyplop->f,
    "0 0 0     hue saturation brightness   columns for color manipulation\n");
  fprintf(xyplop->f,
    "          ******************************************************************\n");
  fprintf(xyplop->f,
    "c         symbol-to-plot    c(circle)bd(dotted box)x+Ifgpr(rectangle)\n");
  fprintf(xyplop->f, "0         symbol-flag       character in xyin\n");
  fprintf(xyplop->f,
    "0.05      symbol sizex      side in inches on the x axis of the symbol.\n");
  fprintf(xyplop->f,
    "0.05      symbol sizey      as for the x axis, get size from yscolumn\n");
  fprintf(xyplop->f, "cl 0.05   connection\n");
  fprintf(xyplop->f,
    "n  0.05   linetype  size    linetype l.-in and size of dashes or dots\n");
  fprintf(xyplop->f,
    "          ******************************************************************\n");
  fprintf(xyplop->f, ".\n");
  fprintf(xyplop->f,
    "          ******************************************************************\n");
}



Static Void themain(ii, xyin, xyplop)
_TEXT *ii, *xyin, *xyplop;
{
  Char a, b, c, d;
  long distance, x, y;

  printf("digrab %4.2f\n", version);

  printf("Give distance from the first base that you want the second base:\n");
  scanf("%ld%*[^\n]", &distance);
  getchar();

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
  fprintf(xyin->f, "* digrab %4.2f\n", version);
  fprintf(xyin->f, "* distance between positions x and y: %ld\n", distance);

  if (*ii->name != '\0') {
    if (ii->f != NULL)
      ii->f = freopen(ii->name, "r", ii->f);
    else
      ii->f = fopen(ii->name, "r");
  } else
    rewind(ii->f);
  if (ii->f == NULL)
    _EscIO2(FileNotFound, ii->name);
  RESETBUF(ii->f, Char);
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
  makexyplop(xyplop, distance);

  while (P_peek(ii->f) == '*')
    copyaline(ii, xyin);

  while (!BUFEOF(ii->f)) {
    a = getc(ii->f);
    b = getc(ii->f);
    c = getc(ii->f);
    d = getc(ii->f);


    if (a == '\n')
      a = ' ';
    if (b == '\n')
      b = ' ';
    if (c == '\n')
      c = ' ';
    if (d == '\n')
      d = ' ';
    fscanf(ii->f, "%ld%ld", &x, &y);
    if (y == x + distance) {
      fprintf(xyin->f, "%c%c%c%c %ld %ld", a, b, c, d, x, y);
      while (!P_eoln(ii->f)) {
	c = getc(ii->f);
	if (c == '\n')
	  c = ' ';
	putc(c, xyin->f);
      }
      putc('\n', xyin->f);
    }
    fscanf(ii->f, "%*[^\n]");
    getc(ii->f);
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyplop.f = NULL;
  strcpy(xyplop.name, "xyplop");
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  ii.f = NULL;
  strcpy(ii.name, "ii");
  themain(&ii, &xyin, &xyplop);
_L1:
  if (ii.f != NULL)
    fclose(ii.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  if (xyplop.f != NULL)
    fclose(xyplop.f);
  exit(EXIT_SUCCESS);
}



/* End. */
