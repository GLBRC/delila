/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "smoothis.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.06
/*
*/
#define updateversion   1.00



/*















































*/



#define wid             10
#define dec             5
#define memmax          1000


Static _TEXT histog, smoothisp, smooth;


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


Local Void cycle(m, range)
long *m, range;
{
  (*m)++;
  if (*m > range)
    *m = 1;
}



Static Void themain(histog, smoothisp, smooth)
_TEXT *histog, *smoothisp, *smooth;
{
  /*
*/


  double average;
  long count, n;
  double memory[memmax];
  boolean memoryfull = false;
  double parameterversion, position;
  long range;
  double total = 0.0;

  printf("smoothis %4.2f\n", version);
  if (*smoothisp->name != '\0') {
    if (smoothisp->f != NULL)
      smoothisp->f = freopen(smoothisp->name, "r", smoothisp->f);
    else
      smoothisp->f = fopen(smoothisp->name, "r");
  } else
    rewind(smoothisp->f);
  if (smoothisp->f == NULL)
    _EscIO2(FileNotFound, smoothisp->name);
  RESETBUF(smoothisp->f, Char);
  fscanf(smoothisp->f, "%lg%*[^\n]", &parameterversion);
  getc(smoothisp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(smoothisp->f, "%ld%*[^\n]", &range);
  getc(smoothisp->f);

  if (range < 1) {
    printf("range must be positive\n");
    halt();
  }

  if (range > memmax) {
    printf("range must be less than memmax (%ld)\n", (long)memmax);
    halt();
  }

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
  if (*smooth->name != '\0') {
    if (smooth->f != NULL)
      smooth->f = freopen(smooth->name, "w", smooth->f);
    else
      smooth->f = fopen(smooth->name, "w");
  } else {
    if (smooth->f != NULL)
      rewind(smooth->f);
    else
      smooth->f = tmpfile();
  }
  if (smooth->f == NULL)
    _EscIO2(FileNotFound, smooth->name);
  SETUPBUF(smooth->f, Char);

  printf("range is %ld\n", range);

  for (n = 0; n < range; n++)
    memory[n] = 0.0;
  n = 1;

  if (debug)
    count = 0;

  while (!BUFEOF(histog->f)) {
    if (P_eoln(histog->f) | (P_peek(histog->f) == '*')) {
      copyaline(histog, smooth);
      continue;
    }
    total -= memory[n-1];


    fscanf(histog->f, "%lg%lg%*[^\n]", &position, &memory[n-1]);
    getc(histog->f);


    total += memory[n-1];

    if (debug)
      printf("%*.*f   unsmoothed:  %*.*f",
	     (int)(debugwid + 5), debugdec, position, debugwid, debugdec,
	     memory[n-1]);



    if (n == range)
      memoryfull = true;

    if (memoryfull) {
      average = total / range;
      fprintf(smooth->f, "%*.*f %*.*f\n",
	      wid, dec, position, wid, dec, average);

      if (debug)
	printf("   average: %*.*f   total: %*.*f",
	       debugwid, debugdec, average, debugwid, debugdec, total);


    }


    cycle(&n, range);

    if (debug)
      putchar('\n');


    if (debug)
      count++;
    if (debug) {
      if (count >= countmax) {
	halt();

      }
    }

  }

}

#undef debug
#undef debugwid
#undef debugdec
#undef countmax


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  smooth.f = NULL;
  strcpy(smooth.name, "smooth");
  smoothisp.f = NULL;
  strcpy(smoothisp.name, "smoothisp");
  histog.f = NULL;
  strcpy(histog.name, "histog");
  themain(&histog, &smoothisp, &smooth);
_L1:
  if (histog.f != NULL)
    fclose(histog.f);
  if (smoothisp.f != NULL)
    fclose(smoothisp.f);
  if (smooth.f != NULL)
    fclose(smooth.f);
  exit(EXIT_SUCCESS);
}































/* End. */
