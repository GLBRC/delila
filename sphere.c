/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "sphere.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.48
/*









*/



/*




































































































*/



#define wid             10
#define dec             8

#define Rmaximum        1.0


Static _TEXT spherep, sigma, xyin;


#define tolerance       5e-2


Static boolean near_(n, spot)
double n, spot;
{
  return (n > spot - tolerance && n < spot + tolerance);
}

#undef tolerance


Static double pd(d, r, twosigma2)
long d;
double r, twosigma2;
{
  /*
*/
  return (exp((d - 1) * log(r)) * exp(-(r * r / twosigma2)));
}


Static double lnpd(d, r, twosigma2)
double d, r, twosigma2;
{
  /*
*/
  /*
*/
  return ((d - 1) * log(r) - r * r / twosigma2);
}


Static Void themain(spherep, sigma, xyin)
_TEXT *spherep, *sigma, *xyin;
{
  double D, Dx, Dy;
  long decimals;
  double exphalf = exp(-1.0 / 2);
  long interval;
  /*



*/
  double lastvalue, lnpdmax, lnvalue, maxr, pdmax, r, sigmaplus, sigmaminus,
	 step, twosigma2;
  /*
*/
  double value;

  printf("sphere %4.2f\n", version);
  if (*sigma->name != '\0') {
    if (sigma->f != NULL)
      sigma->f = freopen(sigma->name, "w", sigma->f);
    else
      sigma->f = fopen(sigma->name, "w");
  } else {
    if (sigma->f != NULL)
      rewind(sigma->f);
    else
      sigma->f = tmpfile();
  }
  if (sigma->f == NULL)
    _EscIO2(FileNotFound, sigma->name);
  SETUPBUF(sigma->f, Char);
  fprintf(sigma->f, "* sphere %4.2f\n", version);
  fprintf(sigma->f, "* D: dimension\n");
  fprintf(sigma->f, "* sigma-: deviation to inside of sphere\n");
  fprintf(sigma->f, "* sigma+: deviation to outside of sphere\n");
  fprintf(sigma->f, "* average: average of absolute values of the sigmas\n");
  fprintf(sigma->f, "* estimate: of sigma (1/sqrt(2*(D-1))\n");
  fprintf(sigma->f, "*\n");

  if (*spherep->name != '\0') {
    if (spherep->f != NULL)
      spherep->f = freopen(spherep->name, "r", spherep->f);
    else
      spherep->f = fopen(spherep->name, "r");
  } else
    rewind(spherep->f);
  if (spherep->f == NULL)
    _EscIO2(FileNotFound, spherep->name);
  RESETBUF(spherep->f, Char);
  fscanf(spherep->f, "%lg%*[^\n]", &step);
  getc(spherep->f);
  if (step <= 0) {
    step = 0.2;
    printf("step must be positive, using %*.*f\n", wid, dec, step);
  }
  fscanf(spherep->f, "%lg%*[^\n]", &maxr);
  getc(spherep->f);

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
  fprintf(xyin->f, "* sphere %4.2f\n", version);
  fprintf(xyin->f, "* step: %*.*f\n", wid, dec, step);
  fprintf(xyin->f, "* maximum r: %*.*f\n", wid, dec, maxr);
  fprintf(xyin->f, "* kind (d=dimension numeral, c = curve) r, pd(r)\n");

  while (!BUFEOF(spherep->f)) {
    fscanf(spherep->f, "%lg", &D);
    if (D < 0) {
      fscanf(spherep->f, "%lg%lg", &Dx, &Dy);
      D = fabs(D);
      if (D == (long)floor(D + 0.5))
	decimals = 0;
      else
	fscanf(spherep->f, "%ld", &decimals);
      fscanf(spherep->f, "%*[^\n]");

      getc(spherep->f);
      if (D == (long)floor(D + 0.5)) {
	fprintf(xyin->f, "D=%ld %*.*f %*.*f\n",
		(long)floor(D + 0.5), wid, dec, Dx, wid, dec, Dy);
	printf("D = %ld plotted at %*.*f %*.*f\n",
	       (long)floor(D + 0.5), wid, dec, Dx, wid, dec, Dy);
      } else {
	fprintf(xyin->f, "D=%1.*f %*.*f %*.*f\n",
		(int)decimals, D, wid, dec, Dx, wid, dec, Dy);
	printf("D = %1.*f plotted at %*.*f %*.*f\n",
	       (int)decimals, D, wid, dec, Dx, wid, dec, Dy);
      }
    } else {
      fscanf(spherep->f, "%*[^\n]");


      /*







*/

      getc(spherep->f);
      if (D == (long)floor(D + 0.5))
	printf("D = % .*E\n", P_max(wid - 7, 1), D);
      else
	printf("D = %*.*f\n", wid, dec, D);
    }

    fprintf(sigma->f, "D:% .1E ", D);
    if (D > 1) {
      twosigma2 = 2.0 / (D - 1);

      lnpdmax = lnpd(D, 1.0, twosigma2);
    } else {
      twosigma2 = 2.0;
      pdmax = 1.0;
      lnpdmax = log(pdmax);
    }

    sigmaplus = 0.0;
    sigmaminus = 0.0;
    interval = 1;
    lastvalue = -LONG_MAX;
    r = step;
    while (r <= maxr) {
      /*
*/
      lnvalue = lnpd(D, r, twosigma2) - lnpdmax;

      /*



*/
      if (lnvalue >= -25) {
	value = exp(lnvalue);
	fprintf(xyin->f, "c %*.*f %*.*f\n", wid, dec, r, wid, dec, value);

	switch (interval) {

	case 1:
	  if (value > exphalf) {
	    sigmaminus = r - step + step * (exphalf - lastvalue) /
				    (value - lastvalue) - Rmaximum;
	    lastvalue = LONG_MAX;
	    interval++;
	  } else
	    lastvalue = value;
	  break;

	case 2:
	  if (r >= Rmaximum)
	    interval++;
	  break;

	case 3:
	  if (value < exphalf) {
	    sigmaplus = r + step + step * (value - exphalf) /
				   (lastvalue - value) - Rmaximum;
	    fprintf(sigma->f,
		    "  sigma-: %5.3f  sigma+: %5.3f  average: %5.3f",
		    sigmaminus, sigmaplus, (sigmaplus - sigmaminus) / 2.0);
	    if (D > 1)
	      fprintf(sigma->f, "  estimate: %5.3f\n", 1 / sqrt(2 * (D - 1)));
	    else
	      fprintf(sigma->f, "  exactly!: %5.3f\n", exphalf);
	    interval++;
	  } else
	    lastvalue = value;
	  break;

	case 4:
	  /* blank case */
	  break;
	}
      }
      r += step;
    }


    fprintf(xyin->f, "c %*.*f %*.*f", wid, dec, 2 * maxr, wid, dec, 0.0);
    fprintf(xyin->f, " move to axis\n");


    fprintf(xyin->f, "c %*.*f %*.*f", wid, dec, 0.0, wid, dec, 0.0);
    fprintf(xyin->f, " move to origin\n");
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  sigma.f = NULL;
  strcpy(sigma.name, "sigma");
  spherep.f = NULL;
  strcpy(spherep.name, "spherep");
  themain(&spherep, &sigma, &xyin);
_L1:
  if (spherep.f != NULL)
    fclose(spherep.f);
  if (sigma.f != NULL)
    fclose(sigma.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
