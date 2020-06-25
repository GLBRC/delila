/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "fitgauss.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.09
/*





*/
#define updateversion   1.00



/*
















































*/



#define wid             10
#define dec             5
#define memmax          10000


Static _TEXT histog, fitgaussp, gfit;


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


#define debug           false

#define debugwid        5
#define debugdec        2
#define countmax        10
/*
*/

#define pi              3.14159265358974


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *gfit;

  double mean;
  long points;
  struct {
    double position[memmax];
    double value[memmax];
    double gauss[memmax];
  } memory;
  double stdev;



  double d1, d2, ex, interval, entries;
} ;



Local Void vary(p, fraction, LINK)
double *p, fraction;
struct LOC_themain *LINK;
{
  double seed, variation;

  randomDelila(&seed);
  variation = 2.0 * (seed - 0.5) * fraction;
  /*


*/
  *p += *p * variation;
}

Local Void computegauss(LINK)
struct LOC_themain *LINK;
{
  long p, FORLIM;

  LINK->d1 = 2 * LINK->stdev * LINK->stdev;
  LINK->d2 = LINK->stdev * sqrt(2 * pi);
  LINK->interval = LINK->memory.position[1] - LINK->memory.position[0];
  FORLIM = LINK->points;
  for (p = 0; p < FORLIM; p++) {
    LINK->ex = LINK->interval * exp((LINK->mean - LINK->memory.position[p]) *
	    (LINK->memory.position[p] - LINK->mean) / LINK->d1) / LINK->d2;
    LINK->memory.gauss[p] = LINK->ex * LINK->entries;
  }
}

Local Void printgauss(LINK)
struct LOC_themain *LINK;
{
  long p, FORLIM;

  FORLIM = LINK->points;
  for (p = 0; p < FORLIM; p++)
    fprintf(LINK->gfit->f, "%*.*f %*.*f %*.*f\n",
	    wid, dec, LINK->memory.position[p], wid, dec,
	    LINK->memory.gauss[p], wid, dec, LINK->memory.value[p]);
}

Local Void computedeviation(deviation, LINK)
double *deviation;
struct LOC_themain *LINK;
{
  /*
*/
  long p;
  double diff;
  long FORLIM;

  *deviation = 0.0;
  FORLIM = LINK->points;
  for (p = 0; p < FORLIM; p++) {
    diff = LINK->memory.value[p] - LINK->memory.gauss[p];
    *deviation += diff * diff;
  }
  /*

*/
}



Static Void themain(histog, fitgaussp, gfit_)
_TEXT *histog, *fitgaussp, *gfit_;
{
  struct LOC_themain V;
  long count, p;
  double parameterversion;
  double total = 0.0, sumsquare = 0.0;

  double sumrange = 0.0, sumsqrange = 0.0;
  double weighted;

  double s;

  double number;

  boolean printing;

  boolean done = false;

  double deviation, newdeviation;

  double currentmean, currentstdev, currententries;

  double fractionstep = 0.05;

  boolean watchprogress = false;

  double tolerance = 0.00005;
  double delta;
  long maxsteps = 10000;
  long FORLIM;



  V.gfit = gfit_;
  printf("fitgauss %4.2f\n", version);
  if (*fitgaussp->name != '\0') {
    if (fitgaussp->f != NULL)
      fitgaussp->f = freopen(fitgaussp->name, "r", fitgaussp->f);
    else
      fitgaussp->f = fopen(fitgaussp->name, "r");
  } else
    rewind(fitgaussp->f);
  if (fitgaussp->f == NULL)
    _EscIO2(FileNotFound, fitgaussp->name);
  RESETBUF(fitgaussp->f, Char);
  fscanf(fitgaussp->f, "%lg%*[^\n]", &parameterversion);
  getc(fitgaussp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  /*











*/

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
  if (*V.gfit->name != '\0') {
    if (V.gfit->f != NULL)
      V.gfit->f = freopen(V.gfit->name, "w", V.gfit->f);
    else
      V.gfit->f = fopen(V.gfit->name, "w");
  } else {
    if (V.gfit->f != NULL)
      rewind(V.gfit->f);
    else
      V.gfit->f = tmpfile();
  }
  if (V.gfit->f == NULL)
    _EscIO2(FileNotFound, V.gfit->name);
  SETUPBUF(V.gfit->f, Char);
  fprintf(V.gfit->f, "* fitgauss %4.2f\n", version);

  /*

*/

  V.points = 0;

  if (debug)
    count = 0;


  while (!BUFEOF(histog->f)) {
    if (P_eoln(histog->f) | (P_peek(histog->f) == '*')) {
      copyaline(histog, V.gfit);
      continue;
    }
    V.points++;
    if (V.points > memmax) {
      printf("histogram must be shorter than memmax (%ld)\n", (long)memmax);
      halt();
    }

    fscanf(histog->f, "%lg%lg%*[^\n]", &V.memory.position[V.points-1],
	   &V.memory.value[V.points-1]);
    getc(histog->f);


    weighted = V.memory.value[V.points-1] * V.memory.position[V.points-1];
    total += weighted;
    V.entries += V.memory.value[V.points-1];
    sumrange += V.memory.value[V.points-1];

    /*


*/

    if (debug)
      printf("%*.*f %*.*f\n",
	     wid, dec, V.memory.position[V.points-1], wid, dec,
	     V.memory.value[V.points-1]);


    if (debug)
      printf("   nverage: %*.*f   total: %*.*f",
	     debugwid, debugdec, V.mean, debugwid, debugdec, total);



    if (debug)
      putchar('\n');


    if (debug)
      count++;
    if (debug) {
      if (count >= countmax)
	halt();
    }

  }


  V.mean = total / sumrange;
  /*

*/

  printf("mean  = %*.*f\n", wid, dec, V.mean);
  printf("entries  = %*.*f\n", wid, dec, V.entries);
  /*

*/

  /*
*/
  sumsquare = 0.0;
  FORLIM = V.points;
  for (p = 0; p < FORLIM; p++) {
    s = V.memory.value[p] * V.memory.position[p] - V.mean;
    sumsquare += s;
  }
  V.stdev = sumsquare / sumrange;
  printf("stdev = %*.*f\n", wid, dec, V.stdev);


  computegauss(&V);

  if (false)
    printgauss(&V);

  /*
*/
  currentmean = V.mean;
  currentstdev = V.stdev;
  currententries = V.entries;

  count = 0;
  computedeviation(&deviation, &V);
  while (!done) {
    vary(&V.mean, fractionstep, &V);
    vary(&V.stdev, fractionstep, &V);
    vary(&V.entries, fractionstep, &V);
    computegauss(&V);
    computedeviation(&newdeviation, &V);

    if (newdeviation < deviation) {
      delta = (deviation - newdeviation) / deviation;
      if (delta < tolerance) {
	done = true;
	printf("reached tolerance: %*.*f\n", wid, dec, delta);
      }

      /*

*/
      currentmean = V.mean;
      currentstdev = V.stdev;
      currententries = V.entries;
      deviation = newdeviation;
      if (watchprogress)
	printgauss(&V);

    }

    else {
      V.mean = currentmean;
      V.stdev = currentstdev;
      V.entries = currententries;
    }
    /*

*/


    count++;
    /*

*/
    if (count > maxsteps)
      done = true;
  }

  if (!watchprogress)
    printgauss(&V);

}

#undef debug
#undef debugwid
#undef debugdec
#undef countmax
#undef pi


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  gfit.f = NULL;
  strcpy(gfit.name, "gfit");
  fitgaussp.f = NULL;
  strcpy(fitgaussp.name, "fitgaussp");
  histog.f = NULL;
  strcpy(histog.name, "histog");
  themain(&histog, &fitgaussp, &gfit);
_L1:
  if (histog.f != NULL)
    fclose(histog.f);
  if (fitgaussp.f != NULL)
    fclose(fitgaussp.f);
  if (gfit.f != NULL)
    fclose(gfit.f);
  exit(EXIT_SUCCESS);
}































/* End. */
