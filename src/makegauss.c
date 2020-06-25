/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "makegauss.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*

*/
#define updateversion   1.00



/*




















































*/



#define wid             10
#define dec             5
#define memmax          10000


Static _TEXT histog, makegaussp, gfit;


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



  double d1, d2, ex, interval;

  double maximum;
} ;



Local Void vary(p, fraction, LINK)
double *p, fraction;
struct LOC_themain *LINK;
{
  double seed, variation;

  /*

*/
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
  /*

*/
  LINK->interval = LINK->maximum;
  FORLIM = LINK->points;
  for (p = 0; p < FORLIM; p++) {
    LINK->ex = LINK->interval * exp((LINK->mean - LINK->memory.position[p]) *
		   (LINK->memory.position[p] - LINK->mean) / LINK->d1);
    /*


*/
    LINK->memory.gauss[p] = LINK->ex;
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



Static Void themain(histog, makegaussp, gfit_)
_TEXT *histog, *makegaussp, *gfit_;
{
  struct LOC_themain V;
  long count;
  double parameterversion;
  double total = 0.0, sumsquare = 0.0;

  double sumrange = 0.0, sumsqrange = 0.0;
  double weighted;

  double s;

  double number, entries;

  boolean printing;

  boolean done;

  double deviation;

  double currentmean;

  double fractionstep = 0.05;

  boolean watchprogress;

  double tolerance = 0.00005;
  long maxsteps = 10000;



  V.gfit = gfit_;
  printf("makegauss %4.2f\n", version);
  if (*makegaussp->name != '\0') {
    if (makegaussp->f != NULL)
      makegaussp->f = freopen(makegaussp->name, "r", makegaussp->f);
    else
      makegaussp->f = fopen(makegaussp->name, "r");
  } else
    rewind(makegaussp->f);
  if (makegaussp->f == NULL)
    _EscIO2(FileNotFound, makegaussp->name);
  RESETBUF(makegaussp->f, Char);
  fscanf(makegaussp->f, "%lg%*[^\n]", &parameterversion);
  getc(makegaussp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  /*











*/

  fscanf(makegaussp->f, "%lg%*[^\n]", &V.mean);
  getc(makegaussp->f);
  fscanf(makegaussp->f, "%lg%*[^\n]", &V.stdev);
  getc(makegaussp->f);
  fscanf(makegaussp->f, "%lg%*[^\n]", &V.maximum);
  getc(makegaussp->f);

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
  fprintf(V.gfit->f, "* makegauss %4.2f\n", version);

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
    entries += V.memory.value[V.points-1];
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

  /*

*/
  /*

*/


  printf("mean  = %*.*f\n", wid, dec, V.mean);
  printf("entries  = %*.*f\n", wid, dec, entries);
  /*

*/

  /*








*/
  printf("stdev = %*.*f\n", wid, dec, V.stdev);

  printf("maximum = %*.*f\n", wid, dec, V.maximum);

/* p2c: makegauss.p: Note: Eliminated unused assignment statement [338] */

  computegauss(&V);

  /*





















































*/

  if (true)
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
  makegaussp.f = NULL;
  strcpy(makegaussp.name, "makegaussp");
  histog.f = NULL;
  strcpy(histog.name, "histog");
  themain(&histog, &makegaussp, &gfit);
_L1:
  if (histog.f != NULL)
    fclose(histog.f);
  if (makegaussp.f != NULL)
    fclose(makegaussp.f);
  if (gfit.f != NULL)
    fclose(gfit.f);
  exit(EXIT_SUCCESS);
}































/* End. */
