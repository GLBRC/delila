/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "riden.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.29
/*
*/



/*


































*/



#define binsmax         1000

#define Rmaximum        1.0


typedef long storage[binsmax + 1];


Static _TEXT color, ridenp, xyin;


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


Static double pd(d, r, twosigma2)
long d;
double r, twosigma2;
{
  if (r == 0.0)
    return 0.0;
  else
    return (exp(-(r * r / twosigma2)) * exp((d - 1) * log(r)));
}


Static Void themain(color, ridenp, xyin)
_TEXT *color, *ridenp, *xyin;
{
  long biggest = 0;
  long bins, D;
  double pdmax, r, rmax;
  long s;
  /*
*/
  long skipped = 0, stored = 0;
  storage store;
  double twosigma2;
  /*
*/
  double x, y;

  printf("riden %4.2f\n", version);

  if (*ridenp->name != '\0') {
    if (ridenp->f != NULL)
      ridenp->f = freopen(ridenp->name, "r", ridenp->f);
    else
      ridenp->f = fopen(ridenp->name, "r");
  } else
    rewind(ridenp->f);
  if (ridenp->f == NULL)
    _EscIO2(FileNotFound, ridenp->name);
  RESETBUF(ridenp->f, Char);
  fscanf(ridenp->f, "%lg%*[^\n]", &rmax);
  getc(ridenp->f);
  fscanf(ridenp->f, "%ld%*[^\n]", &bins);
  getc(ridenp->f);
  if (bins > binsmax) {
    printf("too many bins, increase binsmax\n");
    halt();
  }


  if (*color->name != '\0') {
    if (color->f != NULL)
      color->f = freopen(color->name, "r", color->f);
    else
      color->f = fopen(color->name, "r");
  } else
    rewind(color->f);
  if (color->f == NULL)
    _EscIO2(FileNotFound, color->name);
  RESETBUF(color->f, Char);
  for (s = 1; s <= 9; s++) {
    fscanf(color->f, "%*[^\n]");
    getc(color->f);
  }
  getc(color->f);
  fscanf(color->f, "%ld%*[^\n]", &D);
  getc(color->f);
  if (D < 2) {
    printf("D must be > 1\n");
    halt();
  }
  printf("dimensionality: D = %ld\n", D);

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
  fprintf(xyin->f, "* riden %4.2f\n", version);
  if (*color->name != '\0') {
    if (color->f != NULL)
      color->f = freopen(color->name, "r", color->f);
    else
      color->f = fopen(color->name, "r");
  } else
    rewind(color->f);
  if (color->f == NULL)
    _EscIO2(FileNotFound, color->name);
  RESETBUF(color->f, Char);
  while (P_peek(color->f) == '*')
    copyaline(color, xyin);


  for (s = 0; s <= bins; s++)
    store[s] = 0;


  while (!BUFEOF(color->f)) {
    if (P_peek(color->f) != 's') {
      fscanf(color->f, "%*[^\n]");
      getc(color->f);
      continue;
    }
    getc(color->f);
    fscanf(color->f, "%lg%lg%*[^\n]", &x, &y);
    getc(color->f);
    r = sqrt(x * x + y * y);
    s = (long)floor(bins / rmax * r + 0.5);

    if (s <= bins)
      store[s]++;
    else {
      skipped++;
      printf("skipping r=%10.8f\n", r);
    }
  }


  for (s = 0; s <= bins; s++) {
    if (store[s] > biggest)
      biggest = store[s];
    stored += store[s];
  }

  /*



*/

  twosigma2 = 2.0 / (D - 1);


  pdmax = pd(D, 1.0, twosigma2);


  fprintf(xyin->f,
    "* symbol, radius, normalized histogram, raw histogram, expected fD(r)\n");


  fprintf(xyin->f, "D=%ld %10.8f %10.8f %5d %10d\n", D, 2.0, 0.9, 0, 0);

  for (s = 0; s <= bins; s++) {
    r = rmax * s / bins;
    fprintf(xyin->f, "s %10.8f %10.8f %5ld %10.8f\n",
	    r, (double)store[s] / biggest, store[s],
	    pd(D, r, twosigma2) / pdmax);
  }


  for (s = 0; s <= bins; s++) {
    r = rmax * s / bins;
    fprintf(xyin->f, "e %10.8f %10.8f %5ld %10.8f\n",
	    r, pd(D, r, twosigma2) / pdmax, store[s],
	    (double)store[s] / biggest);
  }

  printf("number of points       skipped: %5ld\n", skipped);
  printf("number of points  histogrammed: %5ld\n", stored);
  printf("number of points in input file: %5ld\n", skipped + stored);

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
  ridenp.f = NULL;
  strcpy(ridenp.name, "ridenp");
  color.f = NULL;
  strcpy(color.name, "color");
  themain(&color, &ridenp, &xyin);
_L1:
  if (color.f != NULL)
    fclose(color.f);
  if (ridenp.f != NULL)
    fclose(ridenp.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
