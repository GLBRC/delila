/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "spec.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.10
/*
*/



/*






























































*/



#define correctionfactor  100.00
/*
*/

#define cswid           10
#define csdec           5


Static _TEXT csdata, baseline, xyin;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void skip(f, n)
_TEXT *f;
long n;
{
  long i;

  for (i = 1; i <= n; i++)
    getc(f->f);
  /*



*/
}



Static Void tostart(f, interval, low, high)
_TEXT *f;
double *interval;
long *low, *high;
{
  /*
*/
  boolean done = false;

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
  while (!done) {
    if (BUFEOF(f->f)) {
      printf("no spectrum found!\n");
      halt();
    }
    if (!P_eoln(f->f)) {
      if (P_peek(f->f) == 'S') {
	getc(f->f);
	if (P_peek(f->f) == 'c') {
	  getc(f->f);
	  if (P_peek(f->f) == 'a') {
	    getc(f->f);
	    if (P_peek(f->f) == 'n') {
	      /*


*/
	      skip(f, 4L);
	      fscanf(f->f, "%lg", interval);
	      skip(f, 14L);
	      fscanf(f->f, "%ld%ld", low, high);
	      done = true;
	    }
	  }
	}
      }
    }
    fscanf(f->f, "%*[^\n]");
    getc(f->f);
  }

  fscanf(f->f, "%*[^\n]");
  getc(f->f);
}



Static Void showdata(f, interval, low, high)
_TEXT *f;
double *interval;
long *low, *high;
{
  fprintf(f->f, "* %3.1f interval\n", *interval);
  fprintf(f->f, "* %3ld low nm\n", *low);
  fprintf(f->f, "* %3ld high nm\n", *high);
  fprintf(f->f, "* \n");
}



Static Void readdata(f, nm, data)
_TEXT *f;
double *nm, *data;
{
  Char c;

  fscanf(f->f, "%lg%c%lg%*[^\n]", nm, &c, data);
  getc(f->f);
  if (c == '\n')
    c = ' ';
  if (c != ',') {
    printf("Misread: \"%c\" should have been a comma.\n", c);
    halt();
  }
  *data /= correctionfactor;
}



Static Void themain(csdata, baseline, xyin)
_TEXT *csdata, *baseline, *xyin;
{
  double bdata;
  long bhigh;
  double binterval;
  long blow;
  double bnm;

  double ddata;
  long dhigh;
  double dinterval;
  long dlow;
  double dnm;

  boolean done = false;

  boolean zerobaseline;

  /*
*/
  printf("spec %4.2f\n", version);

  if (*csdata->name != '\0') {
    if (csdata->f != NULL)
      csdata->f = freopen(csdata->name, "r", csdata->f);
    else
      csdata->f = fopen(csdata->name, "r");
  } else
    rewind(csdata->f);
  if (csdata->f == NULL)
    _EscIO2(FileNotFound, csdata->name);
  RESETBUF(csdata->f, Char);
  if (*baseline->name != '\0') {
    if (baseline->f != NULL)
      baseline->f = freopen(baseline->name, "r", baseline->f);
    else
      baseline->f = fopen(baseline->name, "r");
  } else
    rewind(baseline->f);
  if (baseline->f == NULL)
    _EscIO2(FileNotFound, baseline->name);
  RESETBUF(baseline->f, Char);
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

  tostart(csdata, &dinterval, &dlow, &dhigh);

  if (BUFEOF(baseline->f)) {
    zerobaseline = true;
    binterval = dinterval;
    blow = dlow;
    bhigh = dhigh;
  } else {
    zerobaseline = false;
    tostart(baseline, &binterval, &blow, &bhigh);
  }

  if (binterval != dinterval || blow != dlow || bhigh != dhigh) {
    printf("intervals and range of spectrum must match\n");
    halt();
  }

  fprintf(xyin->f, "* spec %4.2f\n", version);
  if (zerobaseline)
    fprintf(xyin->f, "* baseline: set to zero\n");
  else {
    fprintf(xyin->f, "* baseline:\n");
    showdata(xyin, &binterval, &blow, &bhigh);
  }
  fprintf(xyin->f, "* csdata:\n");
  showdata(xyin, &dinterval, &dlow, &dhigh);

  fprintf(xyin->f, "*\n");
  fprintf(xyin->f, "* columns: nm, data-baseline, baseline, data\n");
  fprintf(xyin->f, "*\n");


  fprintf(xyin->f, "%5ld %*ld %*ld %*d\n",
	  blow, cswid, -LONG_MAX, cswid, LONG_MAX, cswid, 0);

  while (!done) {
    if (BUFEOF(baseline->f) && !zerobaseline) {
      printf("unexpected end of baseline file");
      halt();
    }

    if (BUFEOF(csdata->f)) {
      printf("unexpected end of csdata file");
      halt();
    }

    if (P_peek(baseline->f) == 'S' && !zerobaseline) {
      if (P_peek(baseline->f) != 'S') {
	printf("baseline ended early\n");
	halt();
      }
      done = true;
    }

    if (P_peek(csdata->f) == 'S') {
      if (P_peek(csdata->f) != 'S') {
	printf("csdata file ended early\n");
	halt();
      }
      done = true;
      continue;
    }

    readdata(csdata, &dnm, &ddata);
    if (zerobaseline) {
      bnm = dnm;
      bdata = 0.0;
    } else
      readdata(baseline, &bnm, &bdata);

    if (bnm != dnm) {
      printf("misread:  baseline is at %5.1f but  data is at %5.1f\n",
	     bnm, dnm);
      halt();
    }
    fprintf(xyin->f, "%5.1f %*.*f %*.*f %*.*f\n",
	    dnm, cswid, csdec, ddata - bdata, cswid, csdec, bdata, cswid,
	    csdec, ddata);
  }



  fprintf(xyin->f, "%5ld %*ld %*ld %*d\n",
	  bhigh, cswid, -LONG_MAX, cswid, LONG_MAX, cswid, 0);
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
  baseline.f = NULL;
  strcpy(baseline.name, "baseline");
  csdata.f = NULL;
  strcpy(csdata.name, "csdata");
  themain(&csdata, &baseline, &xyin);
_L1:
  if (csdata.f != NULL)
    fclose(csdata.f);
  if (baseline.f != NULL)
    fclose(baseline.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
