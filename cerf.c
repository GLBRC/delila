/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "cerf.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.06


/*
*/



/*































*/


Static _TEXT list;


#define sqrtpi          1.7724538
#define tol             1.0e-8


Static double computeerf(x)
double x;
{
  double x2, sum, sum1, term;
  long i = 0;

  x2 = x * x;
  sum = x;
  term = x;
  do {
    i++;
    sum1 = sum;
    term = 2.0 * term * x2 / (1.0 + 2.0 * i);
    sum = term + sum1;
  } while (term >= tol * sum);
  return (2.0 * sum * exp(-x2) / sqrtpi);
}

#undef sqrtpi
#undef tol


#define sqrtpi          1.7724538

#define terms           12



Static double computeerfc(x)
double x;
{
  double x2, u, v, sum;
  long i;

  x2 = x * x;
  v = 1.0 / (2.0 * x2);
  u = 1.0 + v * (terms + 1.0);
  for (i = terms; i >= 1; i--) {
    sum = 1.0 + i * v / u;
    u = sum;
  }
  return (exp(-x2) / (x * sum * sqrtpi));
}

#undef sqrtpi
#undef terms



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  double x, er, ec;
  boolean done = false;

  printf("cerf %4.2f\n", version);
  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);
  fprintf(fout->f, "cerf %4.2f\n", version);

  do {
    printf("z: ");
    fscanf(fin->f, "%lg%*[^\n]", &x);
    getc(fin->f);

    if (x < 0.0)
      done = true;
    else {
      if (x == 0.0) {
	er = 0.0;
	ec = 1.0;
      } else if (x < 1.5) {
	er = computeerf(x);
	ec = 1.0 - er;
      } else {
	ec = computeerfc(x);
	er = 1.0 - ec;
      }
      printf("z = %5.2f, Erf(z) = %7.4f, Erfc(z) = % .5E\n", x, er, ec);
      fprintf(fout->f, "z = %5.2f, Erf(z) = %7.4f, Erfc(z) = % .5E\n",
	      x, er, ec);
    }
  } while (!done);
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  list.f = NULL;
  strcpy(list.name, "list");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  themain(&TEMP, &list);
_L1:
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
