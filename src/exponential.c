/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "exponential.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.03
/*




*/
#define updateversion   1.00



/*

























































*/


Static _TEXT exponentialp, exponentialdata;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(exponentialp, exponentialdata)
_TEXT *exponentialp, *exponentialdata;
{
  double c;
  long d, G, maxdist, n;
  double p, parameterversion, q;

  printf("exponential %4.2f\n", version);
  if (*exponentialp->name != '\0') {
    if (exponentialp->f != NULL)
      exponentialp->f = freopen(exponentialp->name, "r", exponentialp->f);
    else
      exponentialp->f = fopen(exponentialp->name, "r");
  } else
    rewind(exponentialp->f);
  if (exponentialp->f == NULL)
    _EscIO2(FileNotFound, exponentialp->name);
  RESETBUF(exponentialp->f, Char);
  fscanf(exponentialp->f, "%lg%*[^\n]", &parameterversion);
  getc(exponentialp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(exponentialp->f, "%ld%*[^\n]", &G);
  getc(exponentialp->f);
  fscanf(exponentialp->f, "%ld%*[^\n]", &n);
  getc(exponentialp->f);
  fscanf(exponentialp->f, "%ld%*[^\n]", &maxdist);
  getc(exponentialp->f);

  if (*exponentialdata->name != '\0') {
    if (exponentialdata->f != NULL)
      exponentialdata->f = freopen(exponentialdata->name, "w",
				   exponentialdata->f);
    else
      exponentialdata->f = fopen(exponentialdata->name, "w");
  } else {
    if (exponentialdata->f != NULL)
      rewind(exponentialdata->f);
    else
      exponentialdata->f = tmpfile();
  }
  if (exponentialdata->f == NULL)
    _EscIO2(FileNotFound, exponentialdata->name);
  SETUPBUF(exponentialdata->f, Char);
  fprintf(exponentialdata->f, "* exponential %4.2f\n", version);
  fprintf(exponentialdata->f, "* %4ld size of genome:  G\n", G);
  fprintf(exponentialdata->f, "* %4ld number of sites: n\n", n);
  fprintf(exponentialdata->f, "* %4ld maximum distance computed: maxdist\n",
	  maxdist);

  p = (double)n / G;
  q = 1 - p;
  c = p * p * G;

  printf("***************\n");
  printf("n     %8ld\n", n);
  printf("G     %8ld\n", G);
  printf("p     %8.5f\n", p);
  printf("q     %8.5f\n", q);
  printf("q*p*G %8.5f\n", q * p * G);
  printf("c     %8.5f\n", c);
  printf("p*p*G %8.5f\n", p * p * G);
  printf("***************\n");

  for (d = 0; d <= maxdist; d++) {
    if (d > 0)
      c *= q;
    printf("for d = %2ld c = (1-p)^d: %8.5f\n", d, c);
    fprintf(exponentialdata->f, "%5ld %5ld\n", d, (long)floor(c + 0.5));
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  exponentialdata.f = NULL;
  strcpy(exponentialdata.name, "exponentialdata");
  exponentialp.f = NULL;
  strcpy(exponentialp.name, "exponentialp");
  themain(&exponentialp, &exponentialdata);
_L1:
  if (exponentialp.f != NULL)
    fclose(exponentialp.f);
  if (exponentialdata.f != NULL)
    fclose(exponentialdata.f);
  exit(EXIT_SUCCESS);
}



/* End. */
