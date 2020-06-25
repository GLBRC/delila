/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "multtest.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.21
/*




*/



/*



















































































































































*/



#define defrwidth       9
#define defrdecimal     2
#define zbound          38
/*
*/

#define pmstring1       " \\pm "
#define pmstring2       " +/- "


typedef struct range {
  double estimate, error;
} range;


Static _TEXT distributions, multtestp, list, xyin, key;


Static boolean showingerrors;
/*

*/
Static Char pmstring[5];

Static boolean fromto;


Static jmp_buf _JL1;


/*
*/


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


/* Local variables for writerange: */
struct LOC_writerange {
  _TEXT *fout;
  range r;
  long width, decimal;
  boolean exponential, zerodecimal;
} ;

Local Void makeplusminus(LINK)
struct LOC_writerange *LINK;
{
  if (LINK->exponential) {
    fprintf(LINK->fout->f, "% .*E%.5s% .*E",
	    P_max((int)LINK->width - 7, 1), LINK->r.estimate, pmstring,
	    P_max((int)LINK->width - 7, 1), LINK->r.error);
    return;
  }
  if (LINK->zerodecimal)
    fprintf(LINK->fout->f, "%*ld%.5s%*ld",
	    (int)LINK->width, (long)floor(LINK->r.estimate + 0.5), pmstring,
	    (int)LINK->width, (long)floor(LINK->r.error + 0.5));
  else {
    fprintf(LINK->fout->f, "%*.*f%.5s%*.*f",
	    (int)LINK->width, (int)LINK->decimal, LINK->r.estimate, pmstring,
	    (int)LINK->width, (int)LINK->decimal, LINK->r.error);
    /*




*/
  }
}

Local Void makefromto(LINK)
struct LOC_writerange *LINK;
{
  putc('[', LINK->fout->f);

  if (LINK->exponential)
    fprintf(LINK->fout->f, "% .*E to % .*E",
	    P_max((int)LINK->width - 7, 1), LINK->r.estimate - LINK->r.error,
	    P_max((int)LINK->width - 7, 1), LINK->r.estimate + LINK->r.error);
  else if (LINK->zerodecimal)
    fprintf(LINK->fout->f, "%*ld to %*ld",
	    (int)LINK->width,
	    (long)floor(LINK->r.estimate - LINK->r.error + 0.5),
	    (int)LINK->width,
	    (long)floor(LINK->r.estimate + LINK->r.error + 0.5));
  else
    fprintf(LINK->fout->f, "%*.*f to %*.*f",
	    (int)LINK->width, (int)LINK->decimal,
	    LINK->r.estimate - LINK->r.error, (int)LINK->width,
	    (int)LINK->decimal, LINK->r.estimate + LINK->r.error);

  putc(']', LINK->fout->f);
}



Static Void writerange(fout_, r_, width_, decimal_, exponential_)
_TEXT *fout_;
range r_;
long width_, decimal_;
boolean exponential_;
{
  /*
*/
  struct LOC_writerange V;

  V.fout = fout_;
  V.r = r_;
  V.width = width_;
  V.decimal = decimal_;
  V.exponential = exponential_;
  V.zerodecimal = (V.decimal <= 0);

  if (showingerrors) {
    if (fromto)
      makefromto(&V);
    else
      makeplusminus(&V);
    return;
  }
  if (V.exponential) {
    fprintf(V.fout->f, "% .*E", P_max((int)V.width - 7, 1), V.r.estimate);
    return;
  }
  if (V.zerodecimal)
    fprintf(V.fout->f, "%*ld", (int)V.width, (long)floor(V.r.estimate + 0.5));
  else
    fprintf(V.fout->f, "%*.*f", (int)V.width, (int)V.decimal, V.r.estimate);
}



Static Void timesrange(a, b, atimesb)
range a, b, *atimesb;
{
  double TEMP, TEMP1;

  atimesb->estimate = a.estimate * b.estimate;
  if (a.estimate != 0 && b.estimate != 0) {
    TEMP = a.error / a.estimate;
    TEMP1 = b.error / b.estimate;
    atimesb->error = fabs(atimesb->estimate) * sqrt(TEMP * TEMP + TEMP1 * TEMP1);
    return;
  }
  printf("WARNING: one of the estimates to be multiplied is zero\n");
  printf("so the error will be set to zero\n");
  atimesb->error = 0.0;
}



Static Void dividerange(a, b, aoverb)
range a, b, *aoverb;
{
  double TEMP, TEMP1;

  aoverb->estimate = a.estimate / b.estimate;
  if (a.estimate != 0) {
    TEMP = a.error / a.estimate;
    TEMP1 = b.error / b.estimate;
    aoverb->error = fabs(aoverb->estimate) * sqrt(TEMP * TEMP + TEMP1 * TEMP1);
    return;
  }
  printf("WARNING: one of the estimates to be divided is zero\n");
  printf("so the error will be set to zero\n");
  aoverb->error = 0.0;
}



Static Void logrange(a, base, loga)
range a;
double base;
range *loga;
{
  loga->estimate = log(a.estimate) / log(base);
  loga->error = a.error / a.estimate;
}



Static Void simpson(upper, answer, tol)
double upper, *answer, *tol;
{
  /*













*/
  long i;

  double x, x1, x2;
  /*
*/
  double pi = 4.0 * atan(1.0);
  double val;
  /*

*/
  double deltax;
  double evensum = 0.0;
  double oddsum, endsum, endcor, sum1;
  /*

*/
  long pieces = 2;
  double lower = 0.0;
  double sum;

  val = 1 / sqrt(2 * pi);

  upper = fabs(upper);
  /*

*/
  *tol = 1.0e-6;
  deltax = (upper - lower) / pieces;
  x = lower + deltax;
  oddsum = val * exp(-0.5 * x * x);
  x1 = lower;
  x2 = upper;
  endsum = val * exp(-0.5 * x1 * x1) + val * exp(-0.5 * x2 * x2);
  endcor = x2 * val * exp(-0.5 * x2 * x2) - x1 * val * exp(-0.5 * x1 * x1);
  sum = (endsum + 4.0 * oddsum) * deltax / 3.0;

  do {
    pieces *= 2;
    sum1 = sum;
    deltax = (upper - lower) / pieces;
    evensum += oddsum;
    oddsum = 0.0;
    for (i = 1; i <= pieces / 2; i++) {
      x = lower + deltax * (2.0 * i - 1.0);
      oddsum += val * exp(-0.5 * x * x);
    }
    sum = (7.0 * endsum + 14.0 * evensum + 16.0 * oddsum + endcor * deltax) *
	  deltax / 15.0;
  } while (fabs(sum - sum1) > fabs(*tol * sum));
  *answer = 0.5 - sum;
  if (*answer < 0.0)
    *answer = 0.0;
  /*

*/
}


/* Local variables for ttestprobability: */
struct LOC_ttestprobability {
  double pj2;
} ;

Local double zip(q, i, j, b, LINK)
double q, i, j, b;
struct LOC_ttestprobability *LINK;
{
  double k;
  double zz = 1.0;
  double z;

  z = zz;
  k = i;
  while (k <= j) {
    zz = zz * q * k / (k - b);
    z += zz;
    k += 2;
  }
  return z;
}

Local double buzz(t, n, LINK)
double t, n;
struct LOC_ttestprobability *LINK;
{
  double rt, fk, ek, dk;

  t = fabs(t);
  rt = t / sqrt(n);
  fk = atan(rt);
  if (n == 1)
    return (1 - fk / LINK->pj2);
  else {
    ek = sin(fk);
    dk = cos(fk);
    if ((((long)floor(n + 0.5)) & 1) == 1)
      return (1 - (fk + ek * dk * zip(dk * dk, 2.0, n - 3, -1.0, LINK)) / LINK->pj2);
    else
      return (1 - ek * zip(dk * dk, 1.0, n - 3, -1.0, LINK));
  }
}





Static double ttestprobability(t, df)
double t, df;
{
  /*
























*/
  struct LOC_ttestprobability V;
  double pi = 4.0 * atan(1.0);
  double pj4, pi2, e = exp(1.0);
  double dgr;

  /*

















*/

  V.pj2 = pi / 2;
  pj4 = pi / 4;
  pi2 = 2 * pi;
  dgr = 180 / pi;
  return (1.0 - buzz(t, df, &V) / 2.0);
}


#define wid             6
#define dec             3


Local Void try_(t, df)
double t, df;
{
  /*
*/
  printf(" t = %*.*f df = %*ld P = %*.*f\n",
	 wid, dec, t, wid, (long)floor(df + 0.5), wid, dec,
	 ttestprobability(t, df));
}


/*



















































































*/



Static Void testttestprobability()
{
  printf("Test t-test to probability routine\n\n");
  printf("Use Table 3 page 441 of this source:\n\n");
  printf("@book{Papoulis1990,\n");
  printf("author = \"Athanasios Papoulis\",\n");
  printf("title = \"Probability \\& Statistics\",\n");
  printf("publisher = \"Prentice Hall\",\n");
  printf("address = \"Englewood Cliffs, NJ\",\n");
  printf("comment = \"Englewood Cliffs NJ 07632\n");
  printf("QA273.P197 1990 / ISBN 0-13-711698-5\",\n");
  printf("year = \"1990\"}\n\n");

  printf("Values should be near 0.90:\n");
  try_(3.08, 1.0);
  try_(1.89, 2.0);
  try_(1.64, 3.0);
  try_(1.35, 14.0);
  try_(1.31, 28.0);
  try_(1.31, 30.0);
  printf("\nValues should be near 0.95:\n");

  try_(6.31, 1.0);
  try_(2.92, 2.0);
  try_(2.35, 3.0);
  try_(1.76, 14.0);
  try_(1.70, 28.0);
  try_(1.70, 30.0);
  printf("\nValues should be near 0.975:\n");

  try_(12.7, 1.0);
  try_(4.30, 2.0);
  try_(3.18, 3.0);
  try_(2.15, 14.0);
  try_(2.05, 28.0);
  try_(2.05, 30.0);
  printf("\nValues should be near 0.99:\n");

  try_(31.8, 1.0);
  try_(6.97, 2.0);
  try_(4.54, 3.0);
  try_(2.62, 14.0);
  try_(2.47, 28.0);
  try_(2.46, 30.0);
  printf("\nValues should be near 0.995:\n");

  try_(63.7, 1.0);
  try_(9.93, 2.0);
  try_(5.84, 3.0);
  try_(2.98, 14.0);
  try_(2.76, 28.0);
  try_(2.75, 30.0);
  putchar('\n');

}

#undef wid
#undef dec


/*





*/

#define color1          0.0
#define color2          0.3
#define color3          0.4
#define color4          0.6
#define color5          1.0
/*



*/
#define colorshift      (-0.40)

#define keysmooth       100
/*
*/

#define dec             5
#define wid             14
#define maxdistrib      1000


/*



*/

#define lowerhue        0.12
#define upperhue        0.90


#define baseofnaturallogs  2.718281828459045


/* Local variables for themain: */
struct LOC_themain {
  double brightness;
  Char colortype;
  long d1, d2, df;
  double hue;
  long intervals;
  double m[maxdistrib];
  long n[maxdistrib];
  double p;
  double s[maxdistrib];
  double t, saturation, sigmad;
  Char symbol;


  range r1, r2, r1overr2, logr1overr2, energyfactor, deltadeltaG;
} ;


Local Void setcolor(LINK)
struct LOC_themain *LINK;
{
  /*



*/
  double p1, p2;

  p1 = LINK->p * (upperhue - lowerhue) + lowerhue;

  p2 = p1 / 0.85;

  switch (LINK->colortype) {

  case 'n':
    LINK->hue = p2;
    break;

  case 'i':
    LINK->hue = (double)((long)(LINK->intervals * p2)) / LINK->intervals;
    break;

  case 's':
    if (p2 < 0.50)
      LINK->hue = color1;
    else if (p2 < 0.90)
      LINK->hue = color2;
    else if (p2 < 0.95)
      LINK->hue = color3;
    else if (p2 < 0.99)
      LINK->hue = color4;
    else {
      LINK->hue = color5;

      /*














*/

      /*





*/

    }
    break;

  }

  /*
































































*/


  if (LINK->p > 1 - 0.95) {
    LINK->saturation = 1.0;
    LINK->brightness = 1.0;
    LINK->symbol = 'c';
    return;
  }

  LINK->saturation = 0.0;
  LINK->brightness = 0.0;
  LINK->symbol = 'b';


}

#undef lowerhue
#undef upperhue

Local Void showcolor(xyin, d1, d2, symbol, LINK)
_TEXT *xyin;
double d1, d2;
Char symbol;
struct LOC_themain *LINK;
{
  fprintf(xyin->f, "%*.*f %*.*f %*.*f %*.*f %*.*f",
	  wid, dec, d1 + colorshift, wid, dec, d2 + colorshift, wid, dec,
	  LINK->hue, wid, dec, LINK->saturation, wid, dec, LINK->brightness);

  if (symbol == 'c')
    fprintf(xyin->f, " %c", symbol);
  else
    fprintf(xyin->f, " hollowsquare");

  putc('\n', xyin->f);

  /*












*/

}

Local Void compute(LINK)
struct LOC_themain *LINK;
{
  double TEMP, TEMP1;

  LINK->df = LINK->n[LINK->d1-1] + LINK->n[LINK->d2-1] - 2;
  TEMP = LINK->s[LINK->d1-1];
  TEMP1 = LINK->s[LINK->d2-1];
  LINK->sigmad = sqrt(TEMP * TEMP / LINK->n[LINK->d1-1] +
		      TEMP1 * TEMP1 / LINK->n[LINK->d2-1]);
  LINK->t = fabs(LINK->m[LINK->d1-1] - LINK->m[LINK->d2-1]) / LINK->sigmad;
  LINK->p = 2.0 * (1.0 - ttestprobability(LINK->t, (double)LINK->df));
}

Local Void computetheenergy(afile, LINK)
_TEXT *afile;
struct LOC_themain *LINK;
{
  double Z, p, tol;

  showingerrors = true;
  memcpy(pmstring, pmstring1, 5L);
  LINK->r1.estimate = LINK->m[LINK->d1-1];
  LINK->r1.error = LINK->s[LINK->d1-1];
  LINK->r2.estimate = LINK->m[LINK->d2-1];
  LINK->r2.error = LINK->s[LINK->d2-1];
  dividerange(LINK->r1, LINK->r2, &LINK->r1overr2);
  logrange(LINK->r1overr2, baseofnaturallogs, &LINK->logr1overr2);

  LINK->energyfactor.estimate = 1.0 / (log(2.0) * 2.3424);
  LINK->energyfactor.error = 0.0;
  timesrange(LINK->logr1overr2, LINK->energyfactor, &LINK->deltadeltaG);
  fprintf(afile->f, "deltadeltaG energy, deltaG[%ld] - deltaG[%ld]: $",
	  LINK->d1, LINK->d2);


  writerange(afile, LINK->deltadeltaG, 1L, 2L, false);
  fprintf(afile->f, "$ kcal/mole\n");





  Z = LINK->deltadeltaG.estimate / LINK->deltadeltaG.error;
  simpson(Z, &p, &tol);

  fprintf(afile->f, "Z score for energy deviation from zero: %1.2f\n", Z);
  fprintf(afile->f, "probability that this deltadeltaG is zero: %1.2f\n", p);



  /*








*/


}

#undef baseofnaturallogs






Static Void themain(distributions, multtestp, list, xyin, key)
_TEXT *distributions, *multtestp, *list, *xyin, *key;
{
  struct LOC_themain V;
  Char controlchar;
  long d = 0;
  boolean dolog = false;
  double fudgefactor = 1.0;
  long keyrange = 10;
  _TEXT TEMP;
  long FORLIM;

  printf("multtest %4.2f\n", version);
  if (*list->name != '\0') {
    if (list->f != NULL)
      list->f = freopen(list->name, "w", list->f);
    else
      list->f = fopen(list->name, "w");
  } else {
    if (list->f != NULL)
      rewind(list->f);
    else
      list->f = tmpfile();
  }
  if (list->f == NULL)
    _EscIO2(FileNotFound, list->name);
  SETUPBUF(list->f, Char);
  fprintf(list->f, "multtest %4.2f\n", version);


  if (*distributions->name != '\0') {
    if (distributions->f != NULL)
      distributions->f = freopen(distributions->name, "r", distributions->f);
    else
      distributions->f = fopen(distributions->name, "r");
  } else
    rewind(distributions->f);
  if (distributions->f == NULL)
    _EscIO2(FileNotFound, distributions->name);
  RESETBUF(distributions->f, Char);
  while (!BUFEOF(distributions->f)) {
    if (P_peek(distributions->f) == ' ' || P_peek(distributions->f) == '9' ||
	P_peek(distributions->f) == '8' || P_peek(distributions->f) == '7' ||
	P_peek(distributions->f) == '6' || P_peek(distributions->f) == '5' ||
	P_peek(distributions->f) == '4' || P_peek(distributions->f) == '3' ||
	P_peek(distributions->f) == '2' || P_peek(distributions->f) == '1' ||
	P_peek(distributions->f) == '0') {
      d++;
      if (d > maxdistrib) {
	printf("cannot have more than %ld distributions\n", (long)maxdistrib);
	halt();
      }
      fscanf(distributions->f, "%ld%lg%lg%*[^\n]", &V.n[d-1], &V.m[d-1],
	     &V.s[d-1]);
      getc(distributions->f);
      V.s[d-1] = fudgefactor * V.s[d-1];
      continue;
    }
    if (P_peek(distributions->f) == '*') {
      fscanf(distributions->f, "%*[^\n]");
      getc(distributions->f);
      continue;
    }
    if (P_peek(distributions->f) != 'f') {
      printf("distributions can only contain numbers or f(udge)\n");
      halt();
    }
    getc(distributions->f);
    fscanf(distributions->f, "%lg%*[^\n]", &fudgefactor);
    getc(distributions->f);
    printf("Fudge factor is set to: %*.*f\n", wid, dec, fudgefactor);
  }

  V.colortype = 'n';
  V.intervals = 20;
  if (*multtestp->name != '\0') {
    if (multtestp->f != NULL)
      multtestp->f = freopen(multtestp->name, "r", multtestp->f);
    else
      multtestp->f = fopen(multtestp->name, "r");
  } else
    rewind(multtestp->f);
  if (multtestp->f == NULL)
    _EscIO2(FileNotFound, multtestp->name);
  RESETBUF(multtestp->f, Char);
  while (!BUFEOF(multtestp->f)) {
    if (P_peek(multtestp->f) == ' ' || P_peek(multtestp->f) == '9' ||
	P_peek(multtestp->f) == '8' || P_peek(multtestp->f) == '7' ||
	P_peek(multtestp->f) == '6' || P_peek(multtestp->f) == '5' ||
	P_peek(multtestp->f) == '4' || P_peek(multtestp->f) == '3' ||
	P_peek(multtestp->f) == '2' || P_peek(multtestp->f) == '1' ||
	P_peek(multtestp->f) == '0') {
      fscanf(multtestp->f, "%ld%ld%*[^\n]", &V.d1, &V.d2);
      getc(multtestp->f);

      if (V.d1 > d || V.d2 > d || V.d1 < 1 || V.d2 < 1) {
	printf(
	  "d1 (=%ld) or d2 (=%ld) is out of the range of distributions, 1 to %ld\n",
	  V.d1, V.d2, d);
	halt();
      }

      compute(&V);

      /*

*/
      printf("d1 = %2ld: %*ld %*.*f %*.*f\n",
	     V.d1, wid, V.n[V.d1-1], wid, dec, V.m[V.d1-1], wid, dec,
	     V.s[V.d1-1]);
      printf("d2 = %2ld: %*ld %*.*f %*.*f\n",
	     V.d2, wid, V.n[V.d2-1], wid, dec, V.m[V.d2-1], wid, dec,
	     V.s[V.d2-1]);

      printf("t = %*.*f\n", wid, dec, V.t);
      printf("p(%ld=%ld) = %*.*f\n", V.d1, V.d2, wid, dec, V.p);

      fprintf(list->f, "              ");
      fprintf(list->f, "distribution %ld", V.d1);
      fprintf(list->f, " | ");
      fprintf(list->f, "distribution %ld\n", V.d2);

      fprintf(list->f, "number        ");
      fprintf(list->f, "%*ld", wid, V.n[V.d1-1]);
      fprintf(list->f, " | ");
      fprintf(list->f, "%*ld\n", wid, V.n[V.d2-1]);

      fprintf(list->f, "mean          ");
      fprintf(list->f, "%*.*f", wid, dec, V.m[V.d1-1]);
      fprintf(list->f, " | ");
      fprintf(list->f, "%*.*f\n", wid, dec, V.m[V.d2-1]);

      fprintf(list->f, "standard dev. ");
      fprintf(list->f, "%*.*f", wid, dec, V.s[V.d1-1]);
      fprintf(list->f, " | ");
      fprintf(list->f, "%*.*f\n", wid, dec, V.s[V.d2-1]);

      fprintf(list->f, "sigma-D = %*.*f\n", wid, dec, V.sigmad);
      fprintf(list->f, "degrees of freedom = %ld\n", V.df);
      fprintf(list->f, "t = %*.*f\n", wid, dec, V.t);

      fprintf(list->f, "p(%ld=%ld) = %*.*f\n", V.d1, V.d2, wid, dec, V.p);
      fprintf(list->f, "This is the probability that they are the same.\n");

      TEMP.f = stdout;
      *TEMP.name = '\0';
      computetheenergy(&TEMP, &V);
      printf("------------------------------------\n");

      computetheenergy(list, &V);
      fprintf(list->f, "------------------------------------\n\n");

      continue;
    }
    controlchar = getc(multtestp->f);

    if (controlchar == '\n')
      controlchar = ' ';
    if (controlchar != 'c') {
      if (controlchar == 'l')
	dolog = true;
      else {
	printf("unknown control character in multtestp: %c\n", controlchar);
	halt();

      }
      continue;
    }
    V.colortype = getc(multtestp->f);
    if (V.colortype == '\n')
      V.colortype = ' ';
    if (V.colortype != 'f' && V.colortype != 's' && V.colortype != 'i' &&
	V.colortype != 'n') {
      printf("colortype in multtestp must be one of: nisf\n");
      halt();
    }
    if (V.colortype == 'i') {
      fscanf(multtestp->f, "%ld%*[^\n]", &V.intervals);
      getc(multtestp->f);
      continue;
    }
    if (V.colortype == 'f') {
      fscanf(multtestp->f, "%lg%*[^\n]", &fudgefactor);
      getc(multtestp->f);
    } else {
      fscanf(multtestp->f, "%*[^\n]");
      getc(multtestp->f);
    }
  }



  if (dolog) {
    printf("USING LOGS\n");
    FORLIM = d;
    for (d = 0; d < FORLIM; d++) {
      printf("d= %2ld n[d] = %ld m[d] = %*.*f s[d] = %*.*f",
	     d + 1, V.n[d], wid, dec, V.m[d], wid, dec, V.s[d]);

      V.s[d] /= V.m[d];
      /*

*/
      V.m[d] = log(V.m[d]);
      printf(" m'[d] = %*.*f s'[d] = %*.*f\n",
	     wid, dec, V.m[d], wid, dec, V.s[d]);
    }
  } else {
    /*








*/
    printf("NOT USING LOGS\n");
  }
  if (*xyin->name != '\0') {
    if (xyin->f != NULL)
      xyin->f = freopen(xyin->name, "w", xyin->f);
    else
      xyin->f = fopen(xyin->name, "w");
  } else {
    if (xyin->f != NULL) {


      rewind(xyin->f);
    } else
      xyin->f = tmpfile();
  }
  if (xyin->f == NULL)
    _EscIO2(FileNotFound, xyin->name);
  SETUPBUF(xyin->f, Char);
  fprintf(xyin->f, "* multtest %4.2f\n", version);
  fprintf(xyin->f, "* color graph \n");
  V.saturation = 0.5;
  V.brightness = 1.0;
  for (V.d1 = 1; V.d1 <= d; V.d1++) {
    for (V.d2 = 1; V.d2 <= d; V.d2++) {
      compute(&V);
      setcolor(&V);
      showcolor(xyin, (double)V.d1, (double)V.d2, V.symbol, &V);
    }
  }


  if (*key->name != '\0') {
    if (key->f != NULL)
      key->f = freopen(key->name, "w", key->f);
    else
      key->f = fopen(key->name, "w");
  } else {
    if (key->f != NULL)
      rewind(key->f);
    else
      key->f = tmpfile();
  }
  if (key->f == NULL)
    _EscIO2(FileNotFound, key->name);
  SETUPBUF(key->f, Char);
  fprintf(key->f, "* multtest %4.2f\n", version);
  fprintf(key->f, "* color key\n");
  V.d2 = 0;
  /*
*/

  V.p = 0.0;
  setcolor(&V);
  showcolor(key, 0.4, 0.5, 'b', &V);
  V.symbol = 'c';
  for (V.d1 = keyrange * 6; V.d1 <= keysmooth * keyrange; V.d1++) {
    V.p = (double)V.d1 / (keysmooth * keyrange);
    setcolor(&V);
    showcolor(key, (double)V.d1 / keysmooth / 10 - colorshift, 0.5, V.symbol,
	      &V);
  }


  /*












*/

  /*








*/

  /*









*/

}

#undef color1
#undef color2
#undef color3
#undef color4
#undef color5
#undef colorshift
#undef keysmooth
#undef dec
#undef wid
#undef maxdistrib


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  key.f = NULL;
  strcpy(key.name, "key");
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  list.f = NULL;
  strcpy(list.name, "list");
  multtestp.f = NULL;
  strcpy(multtestp.name, "multtestp");
  distributions.f = NULL;
  strcpy(distributions.name, "distributions");
  /*

*/
  themain(&distributions, &multtestp, &list, &xyin, &key);
_L1:
  if (distributions.f != NULL)
    fclose(distributions.f);
  if (multtestp.f != NULL)
    fclose(multtestp.f);
  if (list.f != NULL)
    fclose(list.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  if (key.f != NULL)
    fclose(key.f);
  exit(EXIT_SUCCESS);
}



/* End. */
