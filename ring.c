/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "ring.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         3.00
/*
*/



/*
















































































































































*/



#define wid             6
#define dec             4

#define Rmaximum        1.0



#define overlap         1.10
/*
*/
#define rangemax        2.5



Static _TEXT data, ringp, color;

Static double pi;


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



Static Void readparameters(f, D, n, steps, partial, idstring)
_TEXT *f;
long *D, *n, *steps;
double *partial;
Char *idstring;
{
  if (*f->name != '\0') {
    if (f->f != NULL)
      f->f = freopen(f->name, "r", f->f);
    else
      f->f = fopen(f->name, "r");
  } else
    rewind(f->f);
  if (f->f == NULL)
    _EscIO2(FileNotFound, f->name);
  RESETBUF(f->f, Char);
  if (BUFEOF(f->f)) {
    printf("missing parameter: D\n");
    halt();
  }
  fscanf(f->f, "%ld%*[^\n]", D);
  getc(f->f);

  if (BUFEOF(f->f)) {
    printf("missing parameter: n\n");
    halt();
  }
  fscanf(f->f, "%ld%*[^\n]", n);
  getc(f->f);

  if (BUFEOF(f->f)) {
    printf("missing parameter: steps\n");
    halt();
  }
  fscanf(f->f, "%ld%*[^\n]", steps);
  getc(f->f);

  if (BUFEOF(f->f)) {
    printf("missing parameter: partial\n");
    halt();
  }
  fscanf(f->f, "%lg%*[^\n]", partial);
  getc(f->f);

  if (BUFEOF(f->f)) {
    printf("missing parameter: idstring\n");
    halt();
  }
  fscanf(f->f, "%c%*[^\n]", idstring);
  getc(f->f);
  if (*idstring == '\n')
    *idstring = ' ';
  if (*idstring != 'p' && *idstring != 'd' && *idstring != 'a' &&
      *idstring != 'n')
    *idstring = 'n';
}



Static Void writeparameters(f, D, n, steps, partial, idstring)
_TEXT *f;
long D, n, steps;
double partial;
Char idstring;
{
  fprintf(f->f, "* \n");
  fprintf(f->f, "* Parameters:\n");
  fprintf(f->f, "* %10ld total dimensionality, D\n", D);
  fprintf(f->f, "* %10ld requested number of points to plot\n", n);
  fprintf(f->f, "* %10ld steps for plotting smooth fD(r) graph\n", steps);
  fprintf(f->f, "* %10.2f partial value used for plotting\n", partial);
  fprintf(f->f, "* %10c parameter data on plot: ", idstring);
  if (idstring == 'd')
    fprintf(f->f, "dimension\n");
  if (idstring == 'p')
    fprintf(f->f, "dimension + point\n");
  if (idstring == 'a')
    fprintf(f->f, "all\n");
  if (idstring == 'n')
    fprintf(f->f, "none\n");
  fprintf(f->f, "* \n");
}



Static Void polrec(r, theta, x, y)
double r, theta, *x, *y;
{
  /*
*/
  *x = r * cos(theta);
  *y = r * sin(theta);
}



Static boolean getpoint(data, D, plotx, ploty)
_TEXT *data;
long D;
double *plotx, *ploty;
{
  /*



*/
  long dindex = 0;
  double partialsqd = 0.0;
  double Gaussian;
  boolean gotpoint;
  double rz, x, y, Znormalize, Zrsqd;

  /*
*/
/* p2c: ring.p: Note: Eliminated unused assignment statement [338] */
  if (D > 2) {
    while (!BUFEOF(data->f) && dindex < D - 2) {
      fscanf(data->f, "%lg%*[^\n]", &Gaussian);
      getc(data->f);
      partialsqd += Gaussian * Gaussian;
      dindex++;
    }
  }


  if (dindex >= D - 2 || D <= 2) {
    if (!BUFEOF(data->f)) {
      fscanf(data->f, "%lg%*[^\n]", &x);
      getc(data->f);

      if (!BUFEOF(data->f)) {
	fscanf(data->f, "%lg%*[^\n]", &y);
	getc(data->f);
	gotpoint = true;
      }
    }
  }

  if (gotpoint) {
    Zrsqd = x * x + y * y;
    rz = sqrt((partialsqd + Zrsqd) / (D - 1));
    Znormalize = rz / sqrt(Zrsqd);

    *plotx = x * Znormalize;
    *ploty = y * Znormalize;

    return true;
  } else {
    *plotx = 0.0;
    *ploty = 0.0;
    return false;
  }
}





Static double pd(d, r, twosigma2)
long d;
double r, twosigma2;
{
  return (exp(-(r * r / twosigma2)) * exp((d - 1) * log(r)));
}


Static double lnpd(d, r, twosigma2)
long d;
double r, twosigma2;
{
  /*
*/
  return ((d - 1) * log(r) - r * r / twosigma2);
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *color;
  double boxwidth, halfbox, partial;
  /*
*/

  double density;
} ;

Local Void dofdr(xplot, yplot, LINK)
double xplot, yplot;
struct LOC_themain *LINK;
{
  fprintf(LINK->color->f,
	  "f %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f 1 0 1 0 %4.2f\n",
	  wid, dec, xplot - LINK->halfbox, wid, dec, yplot - LINK->halfbox,
	  wid, dec, LINK->boxwidth, wid, dec, LINK->boxwidth, wid, dec,
	  LINK->density * LINK->partial, wid, dec,
	  1.0 - LINK->density * LINK->partial, LINK->partial);
}



Static Void themain(data, ringp, color_)
_TEXT *data, *ringp, *color_;
{
  struct LOC_themain V;
  /*
*/
  Char idstring;
  /*


*/
  long D;
  double halfjump, jump;
  long n;
  long nindex = 0;
  long steps;
  double xplot, yplot;


  double lnpdmax;

  double twosigma2, lndensity, r;
  _TEXT TEMP;

  V.color = color_;
  printf("ring %4.2f\n", version);
  pi = 4.0 * atan(1.0);


  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "r", data->f);
    else
      data->f = fopen(data->name, "r");
  } else
    rewind(data->f);
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  RESETBUF(data->f, Char);
  if (*V.color->name != '\0') {
    if (V.color->f != NULL)
      V.color->f = freopen(V.color->name, "w", V.color->f);
    else
      V.color->f = fopen(V.color->name, "w");
  } else {
    if (V.color->f != NULL)
      rewind(V.color->f);
    else
      V.color->f = tmpfile();
  }
  if (V.color->f == NULL)
    _EscIO2(FileNotFound, V.color->name);
  SETUPBUF(V.color->f, Char);
  fprintf(V.color->f, "* ring %4.2f\n", version);
  fprintf(V.color->f, "*\n");


  fprintf(V.color->f, "* Gaussians from:\n");
  while (P_peek(data->f) == '*')
    copyaline(data, V.color);

  readparameters(ringp, &D, &n, &steps, &V.partial, &idstring);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeparameters(&TEMP, D, n, steps, V.partial, idstring);
  if (D < 2) {
    printf("dimensionality must be greater than 1\n");
    halt();
  }

  writeparameters(V.color, D, n, steps, V.partial, idstring);


  if (D > 1) {
    twosigma2 = 2.0 / (D - 1);

    lnpdmax = lnpd(D, 1.0, twosigma2);
  } else {
    twosigma2 = 2.0;

    lnpdmax = log(1.0);
  }

  fprintf(V.color->f, "* xyin columns are:\n");
  fprintf(V.color->f,
    "* symbols: b=background rectangle, f=from fD(r), s = simulated point\n");
  fprintf(V.color->f,
    "* symbols| x| y| xwidth| ywidth| density| 1-density| max| min| max| min| partial\n");
  fprintf(V.color->f,
    "*       1| 2| 3|      4|      5|       6|         7|   8|   9|  10|  11|      12\n");
  fprintf(V.color->f, "*\n");



  fprintf(V.color->f, "b %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f 1 0 1 0 %4.2f\n",
	  wid, dec, -rangemax, wid, dec, -rangemax, wid, dec, 2 * rangemax,
	  wid, dec, 2 * rangemax, wid, dec, 0.0, wid, dec, 1.0, V.partial);


  /*











*/

  jump = rangemax / (steps + (overlap - 1) / 2);
  V.boxwidth = overlap * jump;
  V.halfbox = V.boxwidth / 2.0;
  halfjump = jump / 2.0;


  xplot = halfjump;
  yplot = halfjump;


  if (steps > 0) {
    while (xplot < rangemax) {
      r = sqrt(xplot * xplot + yplot * yplot);
      if (r != 0.0)
	lndensity = lnpd(D, r, twosigma2) - lnpdmax;
      else
	lndensity = -25.0;

      /*


*/
      if (lndensity >= -25) {
	V.density = exp(lndensity);


	dofdr(xplot, yplot, &V);
	dofdr(yplot, xplot, &V);

	dofdr(yplot, -xplot, &V);
	dofdr(-xplot, yplot, &V);

	dofdr(-xplot, -yplot, &V);
	dofdr(-yplot, -xplot, &V);

	dofdr(-yplot, xplot, &V);
	dofdr(xplot, -yplot, &V);

      }

      yplot += jump;
      if (yplot > xplot) {
	yplot = halfjump;
	xplot += jump;
      }
    }
  }


  fprintf(V.color->f, "b %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f 0 1 0 1 %4.2f\n",
	  wid, dec, 0.0, wid, dec, 0.0, wid, dec, 0.0, wid, dec, 0.0, wid,
	  dec, 0.0, wid, dec, 0.0, V.partial);


  while ((nindex < n) & (!BUFEOF(data->f))) {
    if (getpoint(data, D, &xplot, &yplot)) {
      nindex++;


      fprintf(V.color->f, "s %*.*f %*.*f %*.*f %*.*f 0 1 1 0 1 0 %4.2f\n",
	      wid, dec, xplot, wid, dec, yplot, wid, dec, V.boxwidth, wid,
	      dec, V.boxwidth, V.partial);
    }
  }


  if (idstring == 'a' || idstring == 'p' || idstring == 'd') {
    fprintf(V.color->f, "D=%ld", D);
    if (idstring == 'a' || idstring == 'p') {
      fprintf(V.color->f, "__%ld_points", nindex);
      if (idstring == 'a')
	fprintf(V.color->f, "__%ld_steps__%4.2f_partial", steps, V.partial);
    }

    fprintf(V.color->f, " %*.*f %*.*f %d %d 0 1 1 0 1 0 %4.2f\n",
	    wid, dec, -1.3, wid, dec, 2.5, 1, 1, V.partial);
  }

  if (idstring == 'a')
    fprintf(V.color->f, "ring_%4.2f %*.*f %*.*f %d %d 0 1 1 0 1 0 %4.2f\n",
	    version, wid, dec, 2.0, wid, dec, 2.5, 1, 1, V.partial);

  if (nindex == n)
    printf(" %ld points done\n", nindex);
  else
    printf(" Did not reach %ld data points, Only %ld points done\n",
	   n, nindex);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  color.f = NULL;
  strcpy(color.name, "color");
  ringp.f = NULL;
  strcpy(ringp.name, "ringp");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &ringp, &color);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (ringp.f != NULL)
    fclose(ringp.f);
  if (color.f != NULL)
    fclose(color.f);
  exit(EXIT_SUCCESS);
}



/* End. */
