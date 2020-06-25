/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "gedo.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.08
/*









*/
#define updateversion   1.00



/*

























































































































*/


Static _TEXT gedop, result;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void copyaline(fin, fout)
_TEXT *fin, *fout;
{
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
  putc('\n', fout->f);
}



Static Void copytoeolnwidth(fin, fout, thewidth)
_TEXT *fin, *fout;
long thewidth;
{
  /*
*/
  long c = 0;

  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
    c++;
  }
  while (c < thewidth) {
    putc(' ', fout->f);
    c++;
  }
}


#define namewidth       11



Static Void themain(gedop, result)
_TEXT *gedop, *result;
{
  double C, D;
  /*
*/
  long dec;
  double f;
  /*
*/
  double fprime;
  /*




*/
  double g, k, minutes, oriC, parameterversion;
  boolean skipsum;
  double x;
  double xsum = 0.0;
  long wid;

  printf("gedo %4.2f\n", version);
  if (*gedop->name != '\0') {
    if (gedop->f != NULL)
      gedop->f = freopen(gedop->name, "r", gedop->f);
    else
      gedop->f = fopen(gedop->name, "r");
  } else
    rewind(gedop->f);
  if (gedop->f == NULL)
    _EscIO2(FileNotFound, gedop->name);
  RESETBUF(gedop->f, Char);
  fscanf(gedop->f, "%lg%*[^\n]", &parameterversion);
  getc(gedop->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*result->name != '\0') {
    if (result->f != NULL)
      result->f = freopen(result->name, "w", result->f);
    else
      result->f = fopen(result->name, "w");
  } else {
    if (result->f != NULL)
      rewind(result->f);
    else
      result->f = tmpfile();
  }
  if (result->f == NULL)
    _EscIO2(FileNotFound, result->name);
  SETUPBUF(result->f, Char);
  fprintf(result->f, "* gedo %4.2f\n", version);


  fscanf(gedop->f, "%ld%ld%*[^\n]", &wid, &dec);
  getc(gedop->f);
  fscanf(gedop->f, "%lg%*[^\n]", &g);
  getc(gedop->f);
  fprintf(result->f, "* %4.1f doubling time (min)\n", g);
  fprintf(result->f, "* ");
  fscanf(gedop->f, "%lg", &oriC);
  fprintf(result->f, "%4.1f", oriC);
  copyaline(gedop, result);


  k = 60 * log(2.0) / g;
  fprintf(result->f, "*%5.2f the specific growth rate in $h^{-1}$\n", k);
  fprintf(result->f, "*\n");
  fprintf(result->f, "* min, minutes on the E. coli map\n");
  fprintf(result->f, "* x, the number of copies of a gene per cell\n");
  fprintf(result->f,
	  "* f, is the fractional distance from the origin of replication;\n");
  fprintf(result->f,
    "  i.e. the distance in either direction from oriC divided by 50.\n");

  fprintf(result->f, "* C, the time taken to replicate the chromosome\n");
  fprintf(result->f,
    "* D, the time between completion of chromosomal replication and cell division\n");
  fprintf(result->f, "*\n");

  /*

*/

  fprintf(result->f, "*%*s %*c  %*s %*c %*c %*c\n",
	  (int)(wid - 1), "min", (int)wid, 'x', namewidth, "name", (int)wid,
	  'f', (int)wid, 'C', (int)wid, 'D');

  while (!BUFEOF(gedop->f)) {
    if (P_peek(gedop->f) == '*') {
      fscanf(gedop->f, "%*[^\n]");
      getc(gedop->f);
      continue;
    }
    if (P_peek(gedop->f) == 'n') {
      skipsum = true;
      getc(gedop->f);
    } else
      skipsum = false;

    fscanf(gedop->f, "%lg", &minutes);
    fprintf(result->f, "%*.*f", (int)wid, (int)dec, minutes);

    /*

*/

    if (oriC - minutes > 50)
      f = (100 - oriC + minutes) / 50;
    else
      f = fabs(oriC - minutes) / 50;


    if ((unsigned)minutes < 36)
      fprime = (minutes + 16) / 50;
    if (minutes >= 36 && minutes < oriC)
      fprime = (oriC - minutes) / 50;
    if (minutes >= oriC && minutes <= 100)
      fprime = (minutes - oriC) / 50;
    if (f != fprime) {
      printf("Warning: f <> fprime\n");
      printf("Warning: %*.*f <> %*.*f\n",
	     (int)wid, (int)dec, f, (int)wid, (int)dec, fprime);
      printf("Warning: %*.*f minutes \n", (int)wid, (int)dec, minutes);
    }

    D = 21 + 4 / k;
    C = 40.5 + 4.5 / (k * k);

    x = exp((C * (1 - f) + D) / g * log(2.0));
    if (!skipsum)
      xsum = x + xsum;

    fprintf(result->f, " %*.*f", (int)wid, (int)dec, x);
    putc(' ', result->f);
    copytoeolnwidth(gedop, result, (long)namewidth);
    fscanf(gedop->f, "%*[^\n]");

    getc(gedop->f);
    fprintf(result->f, " %*.*f", (int)wid, (int)dec, f);
    fprintf(result->f, " %*.*f", (int)wid, (int)dec, C);
    fprintf(result->f, " %*.*f", (int)wid, (int)dec, D);

    if (skipsum)
      fprintf(result->f, " - skipped in sum");
    putc('\n', result->f);
  }
  fprintf(result->f, "* %*.*f sum of x\n", (int)wid, (int)dec, xsum);

}

#undef namewidth


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  result.f = NULL;
  strcpy(result.name, "result");
  gedop.f = NULL;
  strcpy(gedop.name, "gedop");
  themain(&gedop, &result);
_L1:
  if (gedop.f != NULL)
    fclose(gedop.f);
  if (result.f != NULL)
    fclose(result.f);
  exit(EXIT_SUCCESS);
}



/* End. */
