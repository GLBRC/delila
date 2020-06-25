/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "asciidna.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.09


/*
*/



/*















































































*/


Static _TEXT asciidnap, plot;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define pi              3.1415926535

#define xmax            80
#define ymax            30


/* Local variables for themain: */
struct LOC_themain {
  double amplitude, multiplier;
} ;

Local long outer(x, LINK)
double x;
struct LOC_themain *LINK;
{
  return ((long)floor(LINK->amplitude * (x + 1) / 2 + 0.5));
}

Local double inner(x, phase, LINK)
long x;
double phase;
struct LOC_themain *LINK;
{
  return (LINK->multiplier * (x + phase));
}



Static Void themain(asciidnap, plot)
_TEXT *asciidnap, *plot;
{
  struct LOC_themain V;
  Char barmajor, barminor;
  long bargap;
  Char field[xmax + 1][xmax + 1];
  double phase1, phase2;
  long x, y, y1, y2, z1, z2, linewidth;
  double wavelength;
  Char Watson, Crick;
  long FORLIM1;

  printf("asciidna %4.2f\n", version);
  if (*plot->name != '\0') {
    if (plot->f != NULL)
      plot->f = freopen(plot->name, "w", plot->f);
    else
      plot->f = fopen(plot->name, "w");
  } else {
    if (plot->f != NULL)
      rewind(plot->f);
    else
      plot->f = tmpfile();
  }
  if (plot->f == NULL)
    _EscIO2(FileNotFound, plot->name);
  SETUPBUF(plot->f, Char);
  fprintf(plot->f, " asciidna %4.2f\n", version);

  if (*asciidnap->name != '\0') {
    if (asciidnap->f != NULL)
      asciidnap->f = freopen(asciidnap->name, "r", asciidnap->f);
    else
      asciidnap->f = fopen(asciidnap->name, "r");
  } else
    rewind(asciidnap->f);
  if (asciidnap->f == NULL)
    _EscIO2(FileNotFound, asciidnap->name);
  RESETBUF(asciidnap->f, Char);
  fscanf(asciidnap->f, "%ld%*[^\n]", &linewidth);
  getc(asciidnap->f);
  fscanf(asciidnap->f, "%lg%*[^\n]", &wavelength);
  getc(asciidnap->f);
  fscanf(asciidnap->f, "%lg%*[^\n]", &V.amplitude);
  getc(asciidnap->f);
  fscanf(asciidnap->f, "%lg%*[^\n]", &phase1);
  getc(asciidnap->f);
  fscanf(asciidnap->f, "%lg%*[^\n]", &phase2);
  getc(asciidnap->f);
  fscanf(asciidnap->f, "%c%*[^\n]", &Watson);
  getc(asciidnap->f);
  if (Watson == '\n')
    Watson = ' ';
  fscanf(asciidnap->f, "%c%*[^\n]", &Crick);
  getc(asciidnap->f);
  if (Crick == '\n')
    Crick = ' ';
  fscanf(asciidnap->f, "%c%*[^\n]", &barmajor);
  getc(asciidnap->f);
  if (barmajor == '\n')
    barmajor = ' ';
  fscanf(asciidnap->f, "%c%*[^\n]", &barminor);
  getc(asciidnap->f);
  if (barminor == '\n')
    barminor = ' ';
  fscanf(asciidnap->f, "%ld%*[^\n]", &bargap);
  getc(asciidnap->f);

  if (linewidth > xmax) {
    printf("linewidth > %ld\n", (long)xmax);
    halt();
  }

  if (V.amplitude > ymax) {
    printf("amplitude > %ld\n", (long)ymax);
    halt();
  }

  V.multiplier = 2 * pi / wavelength;
  linewidth--;
  for (x = 0; x <= linewidth; x++) {
    FORLIM1 = (long)floor(V.amplitude + 0.5);
    for (y = 0; y <= FORLIM1; y++)
      field[x][y] = ' ';
  }

  for (x = 0; x <= linewidth; x++) {
    y1 = outer(sin(inner(x, phase1, &V)), &V);
    y2 = outer(sin(inner(x, phase2, &V)), &V);
    z1 = outer(cos(inner(x, phase1, &V)), &V);
    z2 = outer(cos(inner(x, phase2, &V)), &V);


    if (x % bargap == 0) {
      if (y1 < y2) {
	for (y = y1; y <= y2; y++)
	  field[x][y] = barmajor;
      } else {
	for (y = y2; y <= y1; y++)
	  field[x][y] = barminor;
      }
    }


    if (z1 > z2) {
      field[x][y2] = Crick;
      field[x][y1] = Watson;
    } else {
      field[x][y1] = Watson;
      field[x][y2] = Crick;
    }
  }
  /*




*/



  FORLIM1 = (long)floor(V.amplitude + 0.5);

  for (y = 0; y <= FORLIM1; y++) {
    for (x = 0; x <= linewidth; x++)
      putc(field[x][y], plot->f);
    putc('\n', plot->f);
  }

}

#undef pi
#undef xmax
#undef ymax


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  plot.f = NULL;
  strcpy(plot.name, "plot");
  asciidnap.f = NULL;
  strcpy(asciidnap.name, "asciidnap");
  themain(&asciidnap, &plot);
_L1:
  if (asciidnap.f != NULL)
    fclose(asciidnap.f);
  if (plot.f != NULL)
    fclose(plot.f);
  exit(EXIT_SUCCESS);
}



/* End. */
