/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "stirling.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/


#define version         1.07


/*
*/



/*



























*/



Static double lnnf(n)
long n;
{
  long i;
  double s = 0.0;

  for (i = 2; i <= n; i++)
    s += log((double)i);
  return s;
}



Static double stirling(n)
long n;
{
  return (n * log((double)n) - n);
}


Static double beststirling(n)
long n;
{
  double pi = 4.0 * atan(1.0);

  return ((n + 1.0 / 2) * log((double)n) - n + 1.0 / 2 * log(2 * pi));
}


Static double reduce(a, n)
double a;
long n;
{
  return ((log((double)n) - a / n) / log(2.0));
}


Static Void themain(fout)
_TEXT *fout;
{
  double a, b, s;
  long n;

  printf("stirling %4.2f\n", version);

  fprintf(fout->f, "* Stirling's approximation %4.2f\n", version);
  fprintf(fout->f, "* 3 columns: n, flag, value\n");
  fprintf(fout->f, "* flag = a means ln(n!)\n");
  fprintf(fout->f, "* flag = b means (n + 1/2)*ln(n) - n + (1/2)* ln(2*pi)\n");
  fprintf(fout->f, "* flag = s means n*ln(n)-n (the approximation)\n");
  fprintf(fout->f, "* flag = d means ln(n!) - [n*ln(n)-n]\n");

  for (n = 1; n <= 53; n++) {
    a = lnnf(n);
    b = beststirling(n);
    s = stirling(n);

    fprintf(fout->f, "%5ld", n);
    fprintf(fout->f, " a%15.5f\n", a);

    fprintf(fout->f, "%5ld", n);
    fprintf(fout->f, " b%15.5f\n", b);

    fprintf(fout->f, "%5ld", n);
    fprintf(fout->f, " s%15.5f\n", s);

    fprintf(fout->f, "%5ld", n);
    fprintf(fout->f, " d%15.5f\n", a - s);

  }


  /*























*/


}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  themain(&TEMP);
  exit(EXIT_SUCCESS);
}



/* End. */
