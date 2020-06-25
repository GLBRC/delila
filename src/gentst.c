/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "gentst.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         3.13


/*
*/



/*












































*/



Static _TEXT gentstp, data, genhisp;


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


#define xnumber         20



Static Void themain(data, genhisp)
_TEXT *data, *genhisp;
{
  long total, components, indexfortotal, indexforcomponents;
  double seed, sum;

  printf("gentst %4.2f\n", version);

  if (*gentstp.name != '\0') {
    if (gentstp.f != NULL)
      gentstp.f = freopen(gentstp.name, "r", gentstp.f);
    else
      gentstp.f = fopen(gentstp.name, "r");
  } else
    rewind(gentstp.f);
  if (gentstp.f == NULL)
    _EscIO2(FileNotFound, gentstp.name);
  RESETBUF(gentstp.f, Char);
  fscanf(gentstp.f, "%lg%*[^\n]", &seed);
  getc(gentstp.f);
  fscanf(gentstp.f, "%ld%*[^\n]", &total);
  getc(gentstp.f);
  fscanf(gentstp.f, "%ld%*[^\n]", &components);
  getc(gentstp.f);

  if (*genhisp->name != '\0') {
    if (genhisp->f != NULL)
      genhisp->f = freopen(genhisp->name, "w", genhisp->f);
    else
      genhisp->f = fopen(genhisp->name, "w");
  } else {
    if (genhisp->f != NULL)
      rewind(genhisp->f);
    else
      genhisp->f = tmpfile();
  }
  if (genhisp->f == NULL)
    _EscIO2(FileNotFound, genhisp->name);
  SETUPBUF(genhisp->f, Char);

  fprintf(genhisp->f, "x n %ld\n", (long)xnumber);
  fprintf(genhisp->f, "p g\n");

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
  fprintf(data->f, "* gentst %4.2f\n", version);
  fprintf(data->f, "* %10.9f random seed\n", seed);
  fprintf(data->f, "* %ld numbers, each the sum of\n", total);
  fprintf(data->f, "* %ld random numbers\n", components);

  for (indexfortotal = 1; indexfortotal <= total; indexfortotal++) {
    sum = 0.0;
    for (indexforcomponents = 1;
	 indexforcomponents <= components;
	 indexforcomponents++) {
      randomDelila(&seed);
      sum += seed;
    }
    fprintf(data->f, "% .5E\n",
	    (sum - components / 2.0) * sqrt(12.0 / components));
  }
}

#undef xnumber


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  genhisp.f = NULL;
  strcpy(genhisp.name, "genhisp");
  data.f = NULL;
  strcpy(data.name, "data");
  gentstp.f = NULL;
  strcpy(gentstp.name, "gentstp");
  themain(&data, &genhisp);
_L1:
  if (gentstp.f != NULL)
    fclose(gentstp.f);
  if (data.f != NULL)
    fclose(data.f);
  if (genhisp.f != NULL)
    fclose(genhisp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
