/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "coscurve.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.05
/*

*/
#define updateversion   1.03



/*














































































































*/


Static _TEXT coscurvep;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


/* Local variables for themain: */
struct LOC_themain {
  long dec;
  double dx, dy, L;
  Char printcontrol;
  boolean printing;
  double twopi;
  long wid;
} ;

Local Void compute(intervals, LINK)
long intervals;
struct LOC_themain *LINK;
{
  long i;
  double x = 0.0;
  double y;
  double xold = 0.0;
  double yold;

  LINK->dx = LINK->twopi / intervals;
  if (LINK->printcontrol == 'p' || LINK->printcontrol == 'P')
    printf("dx = % .*E\n", P_max((int)LINK->wid - 7, 1), LINK->dx);
  LINK->L = 0.0;
  yold = cos(xold);
  for (i = 1; i <= intervals; i++) {
    x += LINK->dx;
    y = cos(x);
    LINK->dy = y - yold;
    LINK->L += sqrt(LINK->dx * LINK->dx + LINK->dy * LINK->dy);
    if (LINK->printing) {
      if (LINK->printcontrol == 'p' || LINK->printcontrol == 'P') {
	if (LINK->printcontrol == 'P') {
	  printf("  x = %*.*f", (int)LINK->wid, (int)LINK->dec, x);
	  printf("  y = %*.*f", (int)LINK->wid, (int)LINK->dec, y);
	  printf(" dy = %*.*f", (int)LINK->wid, (int)LINK->dec, LINK->dy);
	  printf("  L = %*.*f\n", (int)LINK->wid, (int)LINK->dec, LINK->L);
	}
	printf("  L/(2pi) = %*.*f\n",
	       (int)LINK->wid, (int)LINK->dec, LINK->L / LINK->twopi);
      }
    }
    xold = x;
    yold = y;
  }
}



Static Void themain(coscurvep)
_TEXT *coscurvep;
{
  struct LOC_themain V;
  long intervals, multiple;
  double parameterversion, pi = 4.0 * atan(1.0);
  long target;

  printf("coscurve %4.2f\n", version);
  if (*coscurvep->name != '\0') {
    if (coscurvep->f != NULL)
      coscurvep->f = freopen(coscurvep->name, "r", coscurvep->f);
    else
      coscurvep->f = fopen(coscurvep->name, "r");
  } else
    rewind(coscurvep->f);
  if (coscurvep->f == NULL)
    _EscIO2(FileNotFound, coscurvep->name);
  RESETBUF(coscurvep->f, Char);
  fscanf(coscurvep->f, "%lg%*[^\n]", &parameterversion);
  getc(coscurvep->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(coscurvep->f, "%ld%*[^\n]", &intervals);
  getc(coscurvep->f);
  fscanf(coscurvep->f, "%ld%*[^\n]", &V.wid);
  getc(coscurvep->f);

  V.printcontrol = getc(coscurvep->f);
  if (V.printcontrol == '\n')
    V.printcontrol = ' ';
  if (V.printcontrol == 'm') {
    fscanf(coscurvep->f, "%ld", &multiple);
    if (multiple <= 1) {
      printf("multiple must be >= 1 \n");
      halt();
    }
  } else
    multiple = 1;
  fscanf(coscurvep->f, "%*[^\n]");

  getc(coscurvep->f);
  if (V.printcontrol != '-' && V.printcontrol != 'm' &&
      V.printcontrol != 'p' && V.printcontrol != 'P') {
    printf("printcontrol must be one of: Ppm-\n");
    halt();
  }

  V.printing = (V.printcontrol != '-');
  V.dec = V.wid - 3;

  V.twopi = 2.0 * pi;
  printf("pi = %*.*f\n", (int)V.wid, (int)V.dec, pi);

  printf("intervals = %ld\n", intervals);

  if (V.printcontrol == 'm') {
    target = multiple;
    while (target <= intervals) {
      compute(target, &V);
      printf("%*ld, L/(2pi) = %*.*f\n",
	     (int)V.wid, target, (int)V.wid, (int)V.dec, V.L / V.twopi);
      target *= multiple;
    }
  } else
    compute(intervals, &V);

  printf("L = %*.*f\n", (int)V.wid, (int)V.dec, V.L);
  printf("L/(2pi) = %*.*f\n", (int)V.wid, (int)V.dec, V.L / V.twopi);
  printf("==========\n");

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  coscurvep.f = NULL;
  strcpy(coscurvep.name, "coscurvep");
  themain(&coscurvep);
_L1:
  if (coscurvep.f != NULL)
    fclose(coscurvep.f);
  exit(EXIT_SUCCESS);
}



/* End. */
