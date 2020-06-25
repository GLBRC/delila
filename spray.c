/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "spray.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.02
/*



*/
#define updateversion   1.00



/*

















































*/


Static _TEXT sprayp, drops;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define pow14           16384

#define pow15           32768L
#define pow22           4194304L
#define pow23           8388608L



Static Void randomDelila(seed)
double *seed;
{
  /*

*/

  /*











*/
  long iseed, i, nrep;

  iseed = (long)floor(*seed * pow23 + 0.5);
  if (iseed < 1 || iseed >= pow23)
    iseed = 1;
  nrep = (iseed & 7) + 4;
  for (i = 1; i <= nrep; i++) {
    if ((iseed & 1) == ((iseed & (pow15 - 1)) >= pow14))
      iseed /= 2;
    else
      iseed = iseed / 2 + pow22;
  }

  *seed = (double)iseed / pow23;
}

#undef pow14
#undef pow15
#undef pow22
#undef pow23


/*

*/

#define debugging       false



Static Void themain(sprayp, drops)
_TEXT *sprayp, *drops;
{
  double parameterversion;
  long d, G, l, n;
  double r = 0.5;

  printf("spray %4.2f\n", version);
  if (*sprayp->name != '\0') {
    if (sprayp->f != NULL)
      sprayp->f = freopen(sprayp->name, "r", sprayp->f);
    else
      sprayp->f = fopen(sprayp->name, "r");
  } else
    rewind(sprayp->f);
  if (sprayp->f == NULL)
    _EscIO2(FileNotFound, sprayp->name);
  RESETBUF(sprayp->f, Char);
  fscanf(sprayp->f, "%lg%*[^\n]", &parameterversion);
  getc(sprayp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(sprayp->f, "%ld%*[^\n]", &G);
  getc(sprayp->f);
  fscanf(sprayp->f, "%ld%*[^\n]", &n);
  getc(sprayp->f);

  if (*drops->name != '\0') {
    if (drops->f != NULL)
      drops->f = freopen(drops->name, "w", drops->f);
    else
      drops->f = fopen(drops->name, "w");
  } else {
    if (drops->f != NULL)
      rewind(drops->f);
    else
      drops->f = tmpfile();
  }
  if (drops->f == NULL)
    _EscIO2(FileNotFound, drops->name);
  SETUPBUF(drops->f, Char);
  fprintf(drops->f, "* spray %4.2f\n", version);
  fprintf(drops->f, "* size of genome:  G %4ld\n", G);
  fprintf(drops->f, "* number of sites: n %4ld\n", n);

  if (debugging)
    n = 10;

  printf("Starting generation of drops ...\n");

  for (d = 1; d <= n; d++) {
    randomDelila(&r);
    l = (long)(r * G);
    fprintf(drops->f, "%10ld\n", l);

    if (debugging) {
      printf("* random number r %8.5f\n", r);
      printf("%10ld\n", (long)floor(r * G + 0.5));
    }

  }
  /*



*/

  printf("... end of generation of drops\n");

}

#undef debugging


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  drops.f = NULL;
  strcpy(drops.name, "drops");
  sprayp.f = NULL;
  strcpy(sprayp.name, "sprayp");
  themain(&sprayp, &drops);
_L1:
  if (sprayp.f != NULL)
    fclose(sprayp.f);
  if (drops.f != NULL)
    fclose(drops.f);
  exit(EXIT_SUCCESS);
}



/* End. */
