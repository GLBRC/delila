/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "derivative.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.03
/*
*/
#define updateversion   1.00



/*




























































*/


Static _TEXT xyin, derivativep, data;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void getxy(xyin, x, y, gotten)
_TEXT *xyin;
double *x, *y;
boolean *gotten;
{
  /*
*/
  boolean done = false;

  *gotten = false;
  while (!done) {
    if (BUFEOF(xyin->f)) {
      done = true;
      break;
    }
    if (P_eoln(xyin->f)) {
      fscanf(xyin->f, "%*[^\n]");
      getc(xyin->f);
      continue;
    }
    if (P_peek(xyin->f) == '*') {
      fscanf(xyin->f, "%*[^\n]");
      getc(xyin->f);
    } else {
      fscanf(xyin->f, "%lg%lg%*[^\n]", x, y);
      getc(xyin->f);
      *gotten = true;
      done = true;
      /*



*/
    }
  }
}


/* Local variables for themain: */
struct LOC_themain {
  double ln2;
} ;

Local double log2Delila(x, LINK)
double x;
struct LOC_themain *LINK;
{
  return (log(x) / LINK->ln2);
}



Static Void themain(xyin, derivativep, data)
_TEXT *xyin, *derivativep, *data;
{
  struct LOC_themain V;
  double deltax, deltay;
  boolean gotten;
  double parameterversion;
  long outwid, outdec;
  double previousx, previousy;
  Char scale;
  double slope, trigger, x, y;

  printf("derivative %4.2f\n", version);
  if (*derivativep->name != '\0') {
    if (derivativep->f != NULL)
      derivativep->f = freopen(derivativep->name, "r", derivativep->f);
    else
      derivativep->f = fopen(derivativep->name, "r");
  } else
    rewind(derivativep->f);
  if (derivativep->f == NULL)
    _EscIO2(FileNotFound, derivativep->name);
  RESETBUF(derivativep->f, Char);
  fscanf(derivativep->f, "%lg%*[^\n]", &parameterversion);
  getc(derivativep->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(derivativep->f, "%ld%ld%*[^\n]", &outwid, &outdec);
  getc(derivativep->f);
  fscanf(derivativep->f, "%c%*[^\n]", &scale);
  getc(derivativep->f);
  if (scale == '\n')
    scale = ' ';
  fscanf(derivativep->f, "%lg%*[^\n]", &trigger);
  getc(derivativep->f);

  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "w", data->f);
    else
      data->f = fopen(data->name, "w");
  } else {
    if (data->f != NULL)
      rewind(data->f);
    else
      data->f = tmpfile();
  }
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  SETUPBUF(data->f, Char);
  fprintf(data->f, "* derivative %4.2f\n", version);

  V.ln2 = log(2.0);
  if (*xyin->name != '\0') {
    if (xyin->f != NULL)
      xyin->f = freopen(xyin->name, "r", xyin->f);
    else
      xyin->f = fopen(xyin->name, "r");
  } else
    rewind(xyin->f);
  if (xyin->f == NULL)
    _EscIO2(FileNotFound, xyin->name);
  RESETBUF(xyin->f, Char);

  getxy(xyin, &previousx, &previousy, &gotten);

  if (!gotten) {
    printf("NO DATA\n");
    halt();
  }

  while (!BUFEOF(xyin->f)) {
    getxy(xyin, &x, &y, &gotten);
    if (!gotten)
      continue;
    if (scale == 'l') {
      deltax = log2Delila(x, &V) - log2Delila(previousx, &V);
      deltay = log2Delila(y, &V) - log2Delila(previousy, &V);
    } else {
      deltax = x - previousx;
      deltay = y - previousy;
    }
    if (deltax == 0) {
      printf("DIVISION BY ZERO\n");
      printf("DIVISION BY ZERO\n");
      printf("x = %1.1f", x);
      printf("previousx = %1.1f\n", previousx);
      halt();
    }
    slope = deltay / deltax;
    fprintf(data->f, "%*.*f", (int)outwid, (int)outdec, slope);
    fprintf(data->f, " %*.*f", (int)outwid, (int)outdec, x);
    fprintf(data->f, " %*.*f", (int)outwid, (int)outdec, y);
    if (slope < trigger)
      fprintf(data->f, " 0");
    else
      fprintf(data->f, " 1");
    putc('\n', data->f);
    previousx = x;
    previousy = y;
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  data.f = NULL;
  strcpy(data.name, "data");
  derivativep.f = NULL;
  strcpy(derivativep.name, "derivativep");
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  themain(&xyin, &derivativep, &data);
_L1:
  if (xyin.f != NULL)
    fclose(xyin.f);
  if (derivativep.f != NULL)
    fclose(derivativep.f);
  if (data.f != NULL)
    fclose(data.f);
  exit(EXIT_SUCCESS);
}



/* End. */
