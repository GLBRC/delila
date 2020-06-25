/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dirty.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         2.39


/*
*/



/*































































































































*/


Static double b;
Static _TEXT dirtyp, distribution;
Static double g, o, m;
Static long n;
Static double p, r;
Static long randoms;
Static double sum;
Static _TEXT xyin;
/*
*/
Static double x;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Static double raise(tothe)
long tothe;
{
  return exp(tothe * log(p));
}


Static double calcgiven(p)
double p;
{
  /*
*/
  long i;
  double m = 1.0;
  long FORLIM;

  FORLIM = randoms;
  for (i = randoms - n + 1; i <= FORLIM; i++)
    m *= i;
  FORLIM = n;
  for (i = 2; i <= FORLIM; i++)
    m /= i;
  return (m * raise(randoms - n) * exp(n * log(1 - p)));
}


Static Void header(f)
_TEXT *f;
{
  if (*f->name != '\0') {
    if (f->f != NULL)
      f->f = freopen(f->name, "w", f->f);
    else
      f->f = fopen(f->name, "w");
  } else {
    if (f->f != NULL)
      rewind(f->f);
    else
      f->f = tmpfile();
  }
  if (f->f == NULL)
    _EscIO2(FileNotFound, f->name);
  SETUPBUF(f->f, Char);
  fprintf(f->f, "* dirty %4.2f\n", version);
  fprintf(f->f, "* %ld random positions\n", randoms);
  fprintf(f->f, "* independent variable, p = probability of correct base\n");
  fprintf(f->f, "* dependendent variables: o, m\n");
  fprintf(f->f,
	  "* o = probability of only one base changed, as %ld(1-p)p^%ld\n",
	  randoms, randoms - 1);
  fprintf(f->f, "* m = probability of one or more bases changed, 1 - p^%ld\n",
	  randoms);
  fprintf(f->f,
    "* n = probability of n (=%ld) bases changed calculated from binomial\n",
    n);
}


main(argc, argv)
int argc;
Char *argv[];
{
  long FORLIM;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  distribution.f = NULL;
  strcpy(distribution.name, "distribution");
  dirtyp.f = NULL;
  strcpy(dirtyp.name, "dirtyp");
  printf("dirty %4.2f\n", version);

  if (*dirtyp.name != '\0')
    dirtyp.f = fopen(dirtyp.name, "r");
  else
    rewind(dirtyp.f);
  if (dirtyp.f == NULL)
    _EscIO2(FileNotFound, dirtyp.name);
  RESETBUF(dirtyp.f, Char);
  if (BUFEOF(dirtyp.f)) {
    printf("empty parameter file\n");
    halt();
  }
  fscanf(dirtyp.f, "%ld%*[^\n]", &randoms);
  getc(dirtyp.f);
  if (randoms < 1) {
    printf("there must be at least 1 random position\n");
    halt();
  }
  printf("%ld random positions\n", randoms);
  fscanf(dirtyp.f, "%ld%*[^\n]", &n);
  getc(dirtyp.f);
  if (n <= 0) {
    printf("n must be positive\n");
    halt();
  }
  if (n > randoms) {
    printf("n must less than or equal to the number of random positions\n");
    halt();
  }

  header(&xyin);
  p = 0.01;
  while (p < 1.0) {
    o = randoms * raise(randoms - 1) * (1 - p);
    m = 1 - raise(randoms);
    g = calcgiven(p);

    fprintf(xyin.f, "o  %10.8f %10.8f\n", p, o);
    fprintf(xyin.f, "n  %10.8f %10.8f\n", p, g);
    fprintf(xyin.f, "m  %10.8f %10.8f o/m ratio: %10.8f\n", p, m, o / m);
    p += 0.01;
  }

  p = 1 - (double)n / randoms;
  r = (1 - p) * 4 / 3;
  b = 1 - r;
  x = b / r;

  if (*distribution.name != '\0') {
    if (distribution.f != NULL)
      distribution.f = freopen(distribution.name, "w", distribution.f);
    else
      distribution.f = fopen(distribution.name, "w");
  } else {
    if (distribution.f != NULL)
      rewind(distribution.f);
    else
      distribution.f = tmpfile();
  }
  if (distribution.f == NULL)
    _EscIO2(FileNotFound, distribution.name);
  SETUPBUF(distribution.f, Char);
  fprintf(distribution.f, "* dirty %4.2f\n", version);
  fprintf(distribution.f,
	  "* distribution of number of changes calculated from binomial\n");
  fprintf(distribution.f, "* %ld random positions\n", randoms);
  fprintf(distribution.f, "* %2ld average number of bases changed\n", n);

  if (r < 1.0) {
    fprintf(distribution.f, "* p = probability of correct base = %10.8f\n", p);
    fprintf(distribution.f, "* fraction of [base] : %10.8f\n", b);
    fprintf(distribution.f, "* fraction of [random n] : %10.8f\n", r);
    fprintf(distribution.f, "*\n");
    fprintf(distribution.f, "* ratio of [base] to [random N]: %10.8f\n", x);
    fprintf(distribution.f, "*\n");

    fprintf(distribution.f, "* TO DO THE SYNTHESIS, \n");
    fprintf(distribution.f,
	    "* add one part of an equimolar mixture of the 4 bases\n");
    fprintf(distribution.f, "* to %10.8f parts of the \"wild type\" base\n",
	    x);

    fprintf(distribution.f, "*\n");
    fprintf(distribution.f, "* In the following table,\n");
    fprintf(distribution.f, "*    n = number of changes\n");
    fprintf(distribution.f, "*    f = frequency of n changes\n");
    fprintf(distribution.f,
	    "*    s = running sum of frequencies f (should approach 1.0)\n");
    fprintf(distribution.f,
      "* In the first row, where n=0, f is the frequency of wild type sequences\n");
    fprintf(distribution.f, "*\n");

    sum = 0.0;
    FORLIM = randoms;
    for (n = 0; n <= FORLIM; n++) {
      g = calcgiven(p);
      sum += g;
      fprintf(distribution.f, "n =%2ld   f = %10.8f   s = %10.8f\n",
	      n, g, sum);
    }
  } else
    fprintf(distribution.f,
	    "* USE PURE EQUIPROBABLY RANDOM BASES FOR THE SYNTHESIS.\n");
_L1:
  if (dirtyp.f != NULL)
    fclose(dirtyp.f);
  if (distribution.f != NULL)
    fclose(distribution.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
