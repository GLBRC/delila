/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "gauss.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"




#define version         1.03


/*


*/



/*


















































*/



Static _TEXT gaussp, data, genhisparameters;


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



Static Void polrec(r, theta, x, y)
double r, theta, *x, *y;
{
  /*
*/
  *x = r * cos(theta);
  *y = r * sin(theta);
}


#define twopi           6.283185307179586476925



Static Void gaussian(seed, haveone, x, y)
double *seed;
boolean *haveone;
double *x, *y;
{
  /*





*/
  double radius, angle;

  if (*haveone)
    *x = *y;
  else {
    randomDelila(seed);
    angle = *seed * twopi;
    randomDelila(seed);
    radius = sqrt(-2.0 * log(*seed));
    polrec(radius, angle, x, y);
  }
  *haveone = !*haveone;
}

#undef twopi


#define xnumber         20



Static Void themain(data, genhisparameters)
_TEXT *data, *genhisparameters;
{
  boolean haveone = false;
  /*
*/
  long indexfortotal, total;
  double seed, x, y;

  printf("gauss %4.2f\n", version);

  if (*gaussp.name != '\0') {
    if (gaussp.f != NULL)
      gaussp.f = freopen(gaussp.name, "r", gaussp.f);
    else
      gaussp.f = fopen(gaussp.name, "r");
  } else
    rewind(gaussp.f);
  if (gaussp.f == NULL)
    _EscIO2(FileNotFound, gaussp.name);
  RESETBUF(gaussp.f, Char);
  fscanf(gaussp.f, "%lg%*[^\n]", &seed);
  getc(gaussp.f);
  fscanf(gaussp.f, "%ld%*[^\n]", &total);
  getc(gaussp.f);

  if (*genhisparameters->name != '\0') {
    if (genhisparameters->f != NULL)
      genhisparameters->f = freopen(genhisparameters->name, "w",
				    genhisparameters->f);
    else
      genhisparameters->f = fopen(genhisparameters->name, "w");
  } else {
    if (genhisparameters->f != NULL)
      rewind(genhisparameters->f);
    else
      genhisparameters->f = tmpfile();
  }
  if (genhisparameters->f == NULL)
    _EscIO2(FileNotFound, genhisparameters->name);
  SETUPBUF(genhisparameters->f, Char);

  fprintf(genhisparameters->f, "x n %ld\n", (long)xnumber);
  fprintf(genhisparameters->f, "p g\n");

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
  fprintf(data->f, "* gauss %4.2f\n", version);
  fprintf(data->f, "* %10.9f random seed\n", seed);
  fprintf(data->f, "* %ld numbers, each the sum of\n", total);

  randomDelila(&seed);
  for (indexfortotal = 1; indexfortotal <= total; indexfortotal++) {
    gaussian(&seed, &haveone, &x, &y);
    fprintf(data->f, "% .5E\n", x);
  }
}

#undef xnumber


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  genhisparameters.f = NULL;
  strcpy(genhisparameters.name, "genhisparameters");
  data.f = NULL;
  strcpy(data.name, "data");
  gaussp.f = NULL;
  strcpy(gaussp.name, "gaussp");
  themain(&data, &genhisparameters);
_L1:
  if (gaussp.f != NULL)
    fclose(gaussp.f);
  if (data.f != NULL)
    fclose(data.f);
  if (genhisparameters.f != NULL)
    fclose(genhisparameters.f);
  exit(EXIT_SUCCESS);
}

/*






















































































































































































*/



/* End. */
