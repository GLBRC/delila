/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "fdr.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"



#define version         1.04
/*

*/



/*

















































*/





#define k               1.380662e-23

#define maxval          50
#define maxpass         100
#define maxpassp        101

#define big             1000000.0
#define machep          (-500.0)


typedef struct singlept {
  double start, histval;
} singlept;

typedef struct recdata {
  singlept allpts[maxval];
  double sig, D, err;
} recdata;

typedef double rdata[maxpassp + 1];


Static _TEXT histog, fdrp, outfile;



Static double expo_(a, b)
double a, b;
{
  if (a == 0.0)
    return 0.0;
  else if (b * log(a) < machep)
    return 0.0;
  else
    return exp(b * log(a));
}


#define stp             2.50662827465



Static double gammaDelila(xx)
double xx;
{
  /*
*/
  double x, tmp, ser, gammaln;

  if (xx < 1)
    x = xx;
  else
    x = xx - 1.0;
  tmp = x + 5.5;
  tmp = (x + 0.5) * log(tmp) - tmp;
  ser = 1.0 + 76.18009173 / (x + 1.0) - 86.50532033 / (x + 2.0) +
	24.01409822 / (x + 3.0) - 1.231739516 / (x + 4.0) +
	0.120858003e-2 / (x + 5.0) - 0.536382e-5 / (x + 6.0);
  gammaln = tmp + log(stp * ser);
  if (xx < 1)
    return (exp(gammaln) / xx);
  else
    return exp(gammaln);
}

#undef stp



Static double f(r, sigma, dim)
double r, sigma, dim;
{
  double g, h;

  if (-1 * expo_(r, 2.0) / (2 * expo_(sigma, 2.0)) < machep)
    g = 0.0;
  else
    g = exp(-1 * expo_(r, 2.0) / (2 * expo_(sigma, 2.0)));
  h = gammaDelila(dim / 2) * expo_(sigma, dim) * expo_(2.0, dim / 2 - 1);
  return (expo_(r, dim - 1) * g / h);
}



Static Void readdata(histog, datareal, Rseq, Rstart, Rstep)
_TEXT *histog;
recdata *datareal;
double *Rseq, *Rstart, *Rstep;
{
  double total = 0.0;
  long dummy;
  long count1 = 0;
  long count2;

  if (*histog->name != '\0') {
    if (histog->f != NULL)
      histog->f = freopen(histog->name, "r", histog->f);
    else
      histog->f = fopen(histog->name, "r");
  } else
    rewind(histog->f);
  if (histog->f == NULL)
    _EscIO2(FileNotFound, histog->name);
  RESETBUF(histog->f, Char);
  while (!BUFEOF(histog->f)) {
    if (P_peek(histog->f) == '*') {
      fscanf(histog->f, "%*[^\n]");
      getc(histog->f);
      continue;
    }
    count1++;
    fscanf(histog->f, "%lg%ld%*[^\n]", &datareal->allpts[count1-1].start,
	   &dummy);
    getc(histog->f);
    datareal->allpts[count1-1].histval = dummy;
    total = dummy + total;
  }
  for (count2 = 0; count2 < count1; count2++) {
    *Rseq = datareal->allpts[count2].histval * datareal->allpts[count2].start + *Rseq;
    datareal->allpts[count2].histval /= total;
  }
  *Rseq /= total;
  *Rstart = datareal->allpts[0].start;
  *Rstep = datareal->allpts[1].start - datareal->allpts[0].start;
  for (count2 = count1; count2 < maxval; count2++) {
    datareal->allpts[count2].start = *Rstart + count2 * *Rstep;
    datareal->allpts[count2].histval = 0.0;
  }
}



Static Void readpar(fdrp, sigma, sigmastep, dim, dimstep, temp)
_TEXT *fdrp;
double *sigma, *sigmastep, *dim, *dimstep, *temp;
{
  if (*fdrp->name != '\0') {
    if (fdrp->f != NULL)
      fdrp->f = freopen(fdrp->name, "r", fdrp->f);
    else
      fdrp->f = fopen(fdrp->name, "r");
  } else
    rewind(fdrp->f);
  if (fdrp->f == NULL)
    _EscIO2(FileNotFound, fdrp->name);
  RESETBUF(fdrp->f, Char);
  fscanf(fdrp->f, "%lg%lg%*[^\n]", sigma, sigmastep);
  getc(fdrp->f);
  fscanf(fdrp->f, "%lg%lg%*[^\n]", dim, dimstep);
  getc(fdrp->f);
  fscanf(fdrp->f, "%lg%*[^\n]", temp);
  getc(fdrp->f);
}



Static Void computeerror(databest, error, datareal, sigma, dim, kT, Rstart,
			 Rstep, sigmastep, dimstep, pass)
recdata *databest;
double *error;
recdata datareal;
double sigma, dim, kT, Rstart, Rstep, sigmastep, dimstep;
long pass;
{
  recdata datameas;
  double r, diff, err;
  long count;

  datameas.sig = sigma;
  datameas.D = dim;
  for (count = 0; count < maxval; count++) {
    r = Rstart + count * Rstep;
    datameas.allpts[count].start = r;
    datameas.allpts[count].histval = f(r, sigma, dim) / kT;
    diff = datareal.allpts[count].histval - datameas.allpts[count].histval;
    if (diff < 0)
      diff = -1 * diff;
    err = expo_(diff, 2.0) + err;
  }
  datameas.err = err / maxval;

  printf("% .5E% .5E% .5E%12d%12d% .5E% .5E\n",
	 sigma, dim, datameas.err / 225 * 0.84 + 0.16, 1, 1, sigmastep,
	 dimstep);

  if (datameas.err < error[pass])
    error[pass] = datameas.err;
  if (datameas.err >= databest->err)
    return;
  for (count = 0; count < maxval; count++) {
    databest->allpts[count].start = datameas.allpts[count].start;
    databest->allpts[count].histval = datameas.allpts[count].histval;
  }
  databest->sig = datameas.sig;
  databest->D = datameas.D;
  databest->err = datameas.err;
}



Static Void writefile(outfile, databest, sigmastep, dimstep, pass)
_TEXT *outfile;
recdata databest;
double sigmastep, dimstep;
long pass;
{
  long count;

  fprintf(outfile->f, "* Sigma:  % .5E +/-% .5E\n", databest.sig, sigmastep);
  fprintf(outfile->f, "* Dimension:  % .5E +/-% .5E\n", databest.D, dimstep);
  fprintf(outfile->f, "* Std. Deviation:  % .5E\n", expo_(databest.err, 0.5));
  fprintf(outfile->f, "* in %12ld passes\n", pass);
  for (count = 0; count < maxval; count++) {
    fprintf(outfile->f, "% .5E", databest.allpts[count].start);
    fprintf(outfile->f, "% .5E\n", databest.allpts[count].histval);
  }
}



Static Void themain(histog, fdrp, outfile)
_TEXT *histog, *fdrp, *outfile;
{
  recdata datareal, databest;
  rdata error;
  double Rseq, Rstart, Rstep, sigma, sigmastep, dim, dimstep, dime, temp, kT;
  long pass = 1;
  long count;
  boolean result = false;
  long FORLIM;

  if (*outfile->name != '\0') {
    if (outfile->f != NULL)
      outfile->f = freopen(outfile->name, "w", outfile->f);
    else
      outfile->f = fopen(outfile->name, "w");
  } else {
    if (outfile->f != NULL)
      rewind(outfile->f);
    else
      outfile->f = tmpfile();
  }
  if (outfile->f == NULL)
    _EscIO2(FileNotFound, outfile->name);
  SETUPBUF(outfile->f, Char);
  readdata(histog, &datareal, &Rseq, &Rstart, &Rstep);
  databest.err = big;
  error[0] = big;
  do {
    if (pass == 1) {
      readpar(fdrp, &sigma, &sigmastep, &dim, &dimstep, &temp);
      kT = 1.0;
    }
    dime = dim;
    sigma += sigmastep;
    FORLIM = (long)floor(Rseq + 0.5) * 3;
    for (count = 0; count <= FORLIM; count++) {
      dime += dimstep;
      error[pass] = big;
      computeerror(&databest, error, datareal, sigma, dime, kT, Rstart, Rstep,
		   sigmastep, dimstep, pass);
    }
    if (error[pass] < error[pass-1] && pass > 1 || pass > maxpass) {
      writefile(outfile, databest, sigmastep, dimstep, pass);
      result = true;
    }
    pass++;
  } while (result != true);
}



main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  outfile.f = NULL;
  strcpy(outfile.name, "outfile");
  fdrp.f = NULL;
  strcpy(fdrp.name, "fdrp");
  histog.f = NULL;
  strcpy(histog.name, "histog");
  themain(&histog, &fdrp, &outfile);
  if (histog.f != NULL)
    fclose(histog.f);
  if (fdrp.f != NULL)
    fclose(fdrp.f);
  if (outfile.f != NULL)
    fclose(outfile.f);
  exit(EXIT_SUCCESS);
}




/* End. */
