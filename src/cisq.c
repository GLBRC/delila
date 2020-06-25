/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "cisq.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.43
/*
*/



/*







































































































































































































*/



#define wid             12
#define dec             8
#define firstwid        3
#define firstdec        1


Static _TEXT cisqp, xyin;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void writeparameters(parameters, mlo, mhi, mstep, reffective, steps,
			    spinfactor)
_TEXT *parameters;
double mlo, mhi, mstep, reffective, steps, spinfactor;
{
  fprintf(parameters->f, "* cisq %4.2f\n", version);
  fprintf(parameters->f, "*\n");
  fprintf(parameters->f, "* parameters:\n");
  fprintf(parameters->f, "* mlo        = %*.*f\n", wid, dec, mlo);
  fprintf(parameters->f, "* mhi        = %*.*f\n", wid, dec, mhi);
  fprintf(parameters->f, "* mstep      = %*.*f\n", wid, dec, mstep);
  fprintf(parameters->f, "* reffective = %*.*f\n", wid, dec, reffective);
  fprintf(parameters->f, "* steps      = %*.*f\n", wid, dec, steps);
  fprintf(parameters->f, "* spinfactor = %*.*f\n", wid, dec, spinfactor);
  fprintf(parameters->f, "*\n");
}


/* Local variables for readparameters: */
struct LOC_readparameters {
  double *mlo, *mhi, *mstep, *reffective, *steps, *spinfactor;
} ;

Local Void nope(LINK)
struct LOC_readparameters *LINK;
{
  _TEXT TEMP;

  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeparameters(&TEMP, *LINK->mlo, *LINK->mhi, *LINK->mstep,
		  *LINK->reffective, *LINK->steps, *LINK->spinfactor);
  halt();
}



Static Void readparameters(parameters, mlo_, mhi_, mstep_, reffective_,
			   steps_, spinfactor_)
_TEXT *parameters;
double *mlo_, *mhi_, *mstep_, *reffective_, *steps_, *spinfactor_;
{
  struct LOC_readparameters V;

  V.mlo = mlo_;
  V.mhi = mhi_;
  V.mstep = mstep_;
  V.reffective = reffective_;
  V.steps = steps_;
  V.spinfactor = spinfactor_;
  if (*parameters->name != '\0') {
    if (parameters->f != NULL)
      parameters->f = freopen(parameters->name, "r", parameters->f);
    else
      parameters->f = fopen(parameters->name, "r");
  } else
    rewind(parameters->f);
  if (parameters->f == NULL)
    _EscIO2(FileNotFound, parameters->name);
  RESETBUF(parameters->f, Char);

  fscanf(parameters->f, "%lg%*[^\n]", V.mlo);
  getc(parameters->f);

  fscanf(parameters->f, "%lg%*[^\n]", V.mhi);
  getc(parameters->f);
  if (*V.mlo > *V.mhi) {
    printf("mlo must be bigger than mhi:\n");
    nope(&V);
  }

  fscanf(parameters->f, "%lg%*[^\n]", V.mstep);
  getc(parameters->f);
  fscanf(parameters->f, "%lg%*[^\n]", V.reffective);
  getc(parameters->f);
  fscanf(parameters->f, "%lg%*[^\n]", V.steps);
  getc(parameters->f);
  fscanf(parameters->f, "%lg%*[^\n]", V.spinfactor);
  getc(parameters->f);

  if (*V.mlo <= 0 || *V.mhi <= 0 || *V.reffective <= 0 || *V.steps <= 0 ||
      *V.spinfactor <= 0 || *V.mhi <= 0) {
    printf("all parameters must be positive:\n");
    nope(&V);
  }
}



Static Void polrec(r, theta, x, y)
double r, theta, *x, *y;
{
  /*
*/
  *x = r * cos(theta);
  *y = r * sin(theta);
}


#define pi              3.1415926535



Static Void themain(cisqp, xyin)
_TEXT *cisqp, *xyin;
{
  double abscostheta, dtheta;
  boolean isinteger;
  double m, mhi, mlo, mstep, r, reffective, sinmstep;
  /*






*/
  double abssintheta, spinfactor, steps, theta, twopi = 2 * pi;
  double x, y;
  _TEXT TEMP;

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

  readparameters(cisqp, &mlo, &mhi, &mstep, &reffective, &steps, &spinfactor);
  writeparameters(xyin, mlo, mhi, mstep, reffective, steps, spinfactor);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeparameters(&TEMP, mlo, mhi, mstep, reffective, steps, spinfactor);

  dtheta = twopi / steps;

  m = mlo;

  sinmstep = sin(pi * mstep / 2);

  while (m <= mhi) {
    theta = 0.0;

    if (fabs(sin(pi * m)) < sinmstep)
      isinteger = true;
    else
      isinteger = false;


    fprintf(xyin->f, "* m = %*.*f", wid, dec, m);
    if (isinteger)
      fprintf(xyin->f, ", an integer\n");
    else
      putc('\n', xyin->f);

    if (isinteger || m == mlo) {
      /*

*/
      x = reffective / exp(log(2.0) / m);
      if (m == mlo)
	fprintf(xyin->f, "m=%*.*f", firstwid, firstdec, m);
      else
	fprintf(xyin->f, "m=%ld", (long)floor(m + 0.5));
      fprintf(xyin->f, " %*.*f%*.*f %*.*f%*.*f\n",
	      wid, dec, x, wid, dec, x, wid, dec, 1.0, wid, dec, 1.0);
    }

    while (theta <= spinfactor * twopi) {
      abscostheta = fabs(cos(theta));
      abssintheta = fabs(sin(theta));

      /*
*/
      if (abscostheta == 0.0 || abssintheta == 0.0)
	r = reffective;
      else
	r = reffective * exp(
	      log(1 / (exp(m * log(abscostheta)) + exp(m * log(abssintheta)))) / m);

      polrec(r, theta / spinfactor, &x, &y);

      if (isinteger)
	putc('i', xyin->f);
      else
	putc('r', xyin->f);
      fprintf(xyin->f, " %*.*f %*.*f %*.*f %*.*f\n",
	      wid, dec, x, wid, dec, y, wid, dec, r, wid, dec, theta);

      theta += dtheta / spinfactor;
    }

    m += mstep;
  }
}

#undef pi


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  cisqp.f = NULL;
  strcpy(cisqp.name, "cisqp");
  themain(&cisqp, &xyin);
_L1:
  if (cisqp.f != NULL)
    fclose(cisqp.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}
























/* End. */
