/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "normal.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         3.17


/*
*/



/*


















































*/



Static _TEXT normalp, data, genhisp;


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


#define pi              3.1415926539

#define xnumber         20



Static Void themain(data, genhisp)
_TEXT *data, *genhisp;
{
  long total, indexfortotal;
  double r, seed, theta, x, y;

  printf("normal %4.2f\n", version);

  if (*normalp.name != '\0') {
    if (normalp.f != NULL)
      normalp.f = freopen(normalp.name, "r", normalp.f);
    else
      normalp.f = fopen(normalp.name, "r");
  } else
    rewind(normalp.f);
  if (normalp.f == NULL)
    _EscIO2(FileNotFound, normalp.name);
  RESETBUF(normalp.f, Char);
  fscanf(normalp.f, "%lg%*[^\n]", &seed);
  getc(normalp.f);
  fscanf(normalp.f, "%ld%*[^\n]", &total);
  getc(normalp.f);

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
  fprintf(data->f, "* normal %4.2f\n", version);
  fprintf(data->f, "* %10.9f random seed\n", seed);
  fprintf(data->f, "* %ld numbers\n", total);

  for (indexfortotal = 1; indexfortotal <= total; indexfortotal++) {
    do {
      randomDelila(&seed);
      theta = 2.0 * pi * seed;
      randomDelila(&seed);


      if (seed != 0)
	r = sqrt(-2 * log(seed));
    } while (seed == 0.0);

    polrec(r, theta, &x, &y);
    fprintf(data->f, "%10.8f\n", x);
    fprintf(data->f, "%10.8f\n", y);
  }
}

#undef pi
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
  normalp.f = NULL;
  strcpy(normalp.name, "normalp");
  themain(&data, &genhisp);
_L1:
  if (normalp.f != NULL)
    fclose(normalp.f);
  if (data.f != NULL)
    fclose(data.f);
  if (genhisp.f != NULL)
    fclose(genhisp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
