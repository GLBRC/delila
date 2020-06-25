/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "mutseq.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.03
/*




*/
#define updateversion   1.00



#define seedwid         20
#define seeddec         20
#define maxsequ         100



/*




























































*/


Static _TEXT sequseed, mutseqp;


Static jmp_buf _JL1;



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define pi              3.141592653589



Static Void randomDelila(seed)
double *seed;
{
  /*

























*/
  double intermediate;

  intermediate = *seed + pi;
  intermediate = exp(5.0 * log(intermediate));
  *seed = intermediate - (long)intermediate;
}

#undef pi



Static Void themain(sequseed, mutseqp)
_TEXT *sequseed, *mutseqp;
{
  Char b, newb;
  boolean verbose;
  double parameterversion;
  long length = 0;
  long l;
  double seed;
  Char sequ[maxsequ];

  printf("mutseq %4.2f\n", version);
  if (*mutseqp->name != '\0') {
    if (mutseqp->f != NULL)
      mutseqp->f = freopen(mutseqp->name, "r", mutseqp->f);
    else
      mutseqp->f = fopen(mutseqp->name, "r");
  } else
    rewind(mutseqp->f);
  if (mutseqp->f == NULL)
    _EscIO2(FileNotFound, mutseqp->name);
  RESETBUF(mutseqp->f, Char);
  fscanf(mutseqp->f, "%lg%*[^\n]", &parameterversion);
  getc(mutseqp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (P_eoln(mutseqp->f))
    verbose = false;
  else {
    verbose = (P_peek(mutseqp->f) == 'v');
    fscanf(mutseqp->f, "%*[^\n]");
    getc(mutseqp->f);
  }
  if (*sequseed->name != '\0') {
    if (sequseed->f != NULL)
      sequseed->f = freopen(sequseed->name, "r", sequseed->f);
    else
      sequseed->f = fopen(sequseed->name, "r");
  } else {



    rewind(sequseed->f);
  }
  if (sequseed->f == NULL)
    _EscIO2(FileNotFound, sequseed->name);
  RESETBUF(sequseed->f, Char);

  while ((length <= maxsequ) & (!P_eoln(sequseed->f))) {
    length++;
    sequ[length-1] = getc(sequseed->f);
    if (sequ[length-1] == '\n')
      sequ[length-1] = ' ';
    if (sequ[length-1] == 't' || sequ[length-1] == 'g' ||
	sequ[length-1] == 'c' || sequ[length-1] == 'a')
      continue;
    if (verbose)
      printf("bad letter at position %ld\n", length);
    sequ[length-1] = 'a';
    if (verbose)
      printf("using %c instead\n", sequ[length-1]);
  }

  fscanf(sequseed->f, "%lg%*[^\n]", &seed);
  getc(sequseed->f);
  if ((unsigned)seed > 1) {
    if (verbose)
      printf(" WARNING: the seed was out of range, %3.1f\n", seed);
    seed = 0.5;
    if (verbose)
      printf(" WARNING: setting seed to %3.1f\n", seed);
  }

  for (l = 0; l < length; l++) {
    if (verbose)
      putchar(sequ[l]);
  }
  if (verbose)
    putchar('\n');
  if (verbose)
    printf(" %*.*f \n", seedwid, seeddec, seed);

  if (verbose)
    printf(" %ld bp\n", length);


  randomDelila(&seed);
  if (verbose)
    printf(" %*.*f new seed\n", seedwid, seeddec, seed);

  l = (long)floor(seed * length + 0.5) + 1;
  if (verbose)
    printf(" %ld position to change\n", l);

  b = sequ[l-1];
  if (verbose)
    printf(" \"%c\" old base\n", b);

  newb = b;
  while (newb == b) {
    randomDelila(&seed);
    if (verbose)
      printf(" %*.*f new seed\n", seedwid, seeddec, seed);

    if (seed < 0.25) {
      newb = 'a';
      break;
    }
    if (seed < 0.5) {
      newb = 'c';
      break;
    }
    if (seed < 0.75)
      newb = 'g';
    else
      newb = 't';
  }


  if (verbose)
    printf(" \"%c\" new base at position %ld\n", newb, l);


  sequ[l-1] = newb;

  if (*sequseed->name != '\0') {
    if (sequseed->f != NULL)
      sequseed->f = freopen(sequseed->name, "w", sequseed->f);
    else
      sequseed->f = fopen(sequseed->name, "w");
  } else {
    if (sequseed->f != NULL)
      rewind(sequseed->f);
    else
      sequseed->f = tmpfile();
  }
  if (sequseed->f == NULL)
    _EscIO2(FileNotFound, sequseed->name);
  SETUPBUF(sequseed->f, Char);
  for (l = 0; l < length; l++)
    putc(sequ[l], sequseed->f);
  fprintf(sequseed->f, "\n %*.*f \n", seedwid, seeddec, seed);


}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  mutseqp.f = NULL;
  strcpy(mutseqp.name, "mutseqp");
  sequseed.f = NULL;
  strcpy(sequseed.name, "sequseed");
  themain(&sequseed, &mutseqp);
_L1:
  if (sequseed.f != NULL)
    fclose(sequseed.f);
  if (mutseqp.f != NULL)
    fclose(mutseqp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
