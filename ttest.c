/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "ttest.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.12


/*




*/



/*

































































































































































*/


Static _TEXT ttestp, list;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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
  printf(" t = %*.*f df = % .*E P = %*.*f\n",
	 wid, dec, t, P_max(wid - 7, 1), df, wid, dec,
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


#define dec             15
#define wid             24



Static Void themain(ttestp, list)
_TEXT *ttestp, *list;
{
  long df;
  double m1, m2;
  long n1, n2;
  double p, s1, s2, t, sigmad;
  long sdf;

  printf("ttest %4.2f\n", version);
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
  fprintf(list->f, "ttest %4.2f\n", version);

  if (*ttestp->name != '\0') {
    if (ttestp->f != NULL)
      ttestp->f = freopen(ttestp->name, "r", ttestp->f);
    else
      ttestp->f = fopen(ttestp->name, "r");
  } else
    rewind(ttestp->f);
  if (ttestp->f == NULL)
    _EscIO2(FileNotFound, ttestp->name);
  RESETBUF(ttestp->f, Char);
  fscanf(ttestp->f, "%ld%*[^\n]", &n1);
  getc(ttestp->f);
  fscanf(ttestp->f, "%lg%*[^\n]", &m1);
  getc(ttestp->f);
  fscanf(ttestp->f, "%lg%*[^\n]", &s1);
  getc(ttestp->f);
  fscanf(ttestp->f, "%ld%*[^\n]", &n2);
  getc(ttestp->f);
  fscanf(ttestp->f, "%lg%*[^\n]", &m2);
  getc(ttestp->f);
  fscanf(ttestp->f, "%lg%*[^\n]", &s2);
  getc(ttestp->f);
  fscanf(ttestp->f, "%ld%*[^\n]", &sdf);
  getc(ttestp->f);

  if (sdf > 1) {
    fprintf(list->f, "sample division by a factor of %ld\n", sdf);
    m1 /= sdf;
    s1 /= sqrt((double)sdf);

    m2 /= sdf;
    s2 /= sqrt((double)sdf);
  }
  /*

*/

  df = n1 + n2 - 2;
  sigmad = sqrt(s1 * s1 / n1 + s2 * s2 / n2);
  t = (m1 - m2) / sigmad;

  fprintf(list->f, "              ");
  fprintf(list->f, "%*s", wid, "distribution 1");
  fprintf(list->f, " | ");
  fprintf(list->f, "%*s\n", wid, "distribution 2");

  fprintf(list->f, "number        ");
  fprintf(list->f, "%*ld", wid, n1);
  fprintf(list->f, " | ");
  fprintf(list->f, "%*ld\n", wid, n2);

  fprintf(list->f, "mean          ");
  fprintf(list->f, "%*.*f", wid, dec, m1);
  fprintf(list->f, " | ");
  fprintf(list->f, "%*.*f\n", wid, dec, m2);

  fprintf(list->f, "standard dev. ");
  fprintf(list->f, "%*.*f", wid, dec, s1);
  fprintf(list->f, " | ");
  fprintf(list->f, "%*.*f\n\n", wid, dec, s2);

  fprintf(list->f, "sigma-D = %*.*f\n", wid, dec, sigmad);
  fprintf(list->f, "degrees of freedom = %ld\n", df);
  fprintf(list->f, "  t = %*.*f\n", wid, dec, t);

  p = ttestprobability(t, (double)df);
  fprintf(list->f, "  p = %*.*f (probability that the means differ)\n",
	  wid, dec, p);
  fprintf(list->f, "1-p = %*.*f (probability that the means are the same)\n",
	  wid, dec, 1 - p);
  fprintf(list->f, "1-p = % .2E\n", 1 - p);



  /*









*/

}

#undef dec
#undef wid


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  list.f = NULL;
  strcpy(list.name, "list");
  ttestp.f = NULL;
  strcpy(ttestp.name, "ttestp");
  themain(&ttestp, &list);
_L1:
  if (ttestp.f != NULL)
    fclose(ttestp.f);
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}












/* End. */
