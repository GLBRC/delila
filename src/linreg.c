/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "linreg.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         2.00


/*

*/



/*





















*/



/*


*/

Static Void regress(control, x, y, sumx, sumy, sumxsqd, sumysqd, sumxy, ex,
		    ey, varx, vary, covxy, r, m, b, n)
Char control;
double x, y, *sumx, *sumy, *sumxsqd, *sumysqd, *sumxy, *ex, *ey, *varx, *vary,
       *covxy, *r, *m, *b;
long *n;
{




  /*









*/
  if (control != 'r' && control != 'e' && control != 'c') {
    printf(" linear regression control variable value, \"%c\" is not acceptable.\n",
	   control);
    printf(" it must be in [\"c\",\"e\",\"r\"]\n");
    _Escape(0);
  }

  switch (control) {

  case 'c':
    x = 0.0;
    y = 0.0;
    *sumx = 0.0;
    *sumy = 0.0;
    *sumxsqd = 0.0;
    *sumysqd = 0.0;
    *sumxy = 0.0;
    *ex = 0.0;
    *ey = 0.0;
    *varx = 0.0;
    *vary = 0.0;
    *covxy = 0.0;
    *r = 2.0;
    /*
*/

    *m = 0.0;
    *b = 0.0;
    *n = 0;
    break;

  case 'e':
    *sumx += x;
    *sumy += y;
    *sumxsqd += x * x;
    *sumysqd += y * y;
    *sumxy += x * y;
    (*n)++;
    break;

  case 'r':
    *ex = *sumx / *n;
    *ey = *sumy / *n;

    *varx = *sumxsqd / *n - *ex * *ex;
    *vary = *sumysqd / *n - *ey * *ey;

    *covxy = *sumxy / *n - *ex * *ey;

    *r = *covxy / sqrt(*varx * *vary);

    *m = *covxy / *varx;
    *b = *ey - *m * *ex;
    break;
  }
}



Static Void themain(infile, outfile)
_TEXT *infile, *outfile;
{
  /*
*/
  long colx, coly;
  double datum;
  long column;
  boolean satisfied;
  /*
*/
  boolean xsatisfied, ysatisfied;


  double x, y;

  double sumx, sumy, sumxsqd, sumysqd, sumxy;

  double ex, ey, varx, vary, covxy;

  double r;

  double m, b;

  long n;

  printf("linreg %4.2f\n", version);
  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else
    rewind(infile->f);
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);
  fscanf(infile->f, "%ld%ld%*[^\n]", &colx, &coly);
  getc(infile->f);
  regress('c', x, y, &sumx, &sumy, &sumxsqd, &sumysqd, &sumxy, &ex, &ey,
	  &varx, &vary, &covxy, &r, &m, &b, &n);

  while (!BUFEOF(infile->f)) {
    column = 0;
    satisfied = false;
    xsatisfied = false;
    ysatisfied = false;

    while (!satisfied) {
      fscanf(infile->f, "%lg", &datum);
      column++;
      if (column == colx) {
	x = datum;
	xsatisfied = true;
      }
      if (column == coly) {
	y = datum;
	ysatisfied = true;
      }
      satisfied = (xsatisfied && ysatisfied);
    }
    fscanf(infile->f, "%*[^\n]");

    getc(infile->f);
    regress('e', x, y, &sumx, &sumy, &sumxsqd, &sumysqd, &sumxy, &ex, &ey,
	    &varx, &vary, &covxy, &r, &m, &b, &n);
  }


  regress('r', x, y, &sumx, &sumy, &sumxsqd, &sumysqd, &sumxy, &ex, &ey,
	  &varx, &vary, &covxy, &r, &m, &b, &n);


  fprintf(outfile->f, "columns  x=%5ld and y=%5ld\n", colx, coly);
  fprintf(outfile->f, "mean     x=%5.3f and y=%5.3f\n", ex, ey);
  fprintf(outfile->f, "variance x=%5.3f and y=%5.3f\n", varx, vary);
  fprintf(outfile->f, "covariance = %5.3f\n", covxy);
  fprintf(outfile->f, "r = %5.3f\n", r);
  fprintf(outfile->f, "m = %5.3f\n", m);
  fprintf(outfile->f, "b = %5.3f\n", b);
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
