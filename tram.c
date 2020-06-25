/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "tram.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*
*/
#define updateversion   1.00



/*








































*/


Static _TEXT fin, tramp, fout;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void copynoreturn(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  /*

*/

}


Static double plogp(f)
double f;
{
  if (f <= 0.0)
    return 0.0;
  else
    return (-(f * log(f) / log(2.0)));
}


#define maxcount        2000
#define wid             4
#define dec             1


Static Void themain(fin, tramp, fout)
_TEXT *fin, *tramp, *fout;
{
  long count = 0;
  boolean done = false;
  double H;
  Char id[2];
  long i;
  double na, nc, ng, nt, n, parameterversion, R;
  long FORLIM;

  printf("tram %4.2f\n", version);
  if (*tramp->name != '\0') {
    if (tramp->f != NULL)
      tramp->f = freopen(tramp->name, "r", tramp->f);
    else
      tramp->f = fopen(tramp->name, "r");
  } else
    rewind(tramp->f);
  if (tramp->f == NULL)
    _EscIO2(FileNotFound, tramp->name);
  RESETBUF(tramp->f, Char);
  fscanf(tramp->f, "%lg%*[^\n]", &parameterversion);
  getc(tramp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  RESETBUF(fin->f, Char);
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

  while (!done) {
    if (BUFEOF(fin->f) || count >= maxcount) {
      done = true;
      break;
    }
    P_readpaoc(fin->f, id, 2);
    /*

*/
    if (!strncmp(id, "NA", 2)) {
      fprintf(fout->f, "count = %ld", count);
      copynoreturn(fin, fout);
      putc('\n', fout->f);
    }
    fscanf(fin->f, "%*[^\n]");

    getc(fin->f);
    if (strncmp(id, "P0", 2))
      continue;
    count++;
    /*

*/
    while (strncmp(id, "XX", 2)) {
      P_readpaoc(fin->f, id, 2);
      /*

*/
      if (strncmp(id, "XX", 2)) {
	fscanf(fin->f, "%lg%lg%lg%lg", &na, &nc, &ng, &nt);
	fprintf(fout->f, " %*.*f %*.*f %*.*f %*.*f",
		wid, dec, na, wid, dec, nc, wid, dec, ng, wid, dec, nt);

	copynoreturn(fin, fout);
	fprintf(fout->f, " | ");

	n = na + nc + ng + nt;
	H = plogp(na / n) + plogp(nc / n) + plogp(ng / n) + plogp(nt / n);
	R = 2 - H;

	/*

*/
	fprintf(fout->f, " R %5.2f", R);
	fprintf(fout->f, " | ");
	FORLIM = (long)floor(20 * R + 0.5);
	for (i = 0; i <= FORLIM; i++)
	  putc('*', fout->f);
	fprintf(fout->f, "|\n");

      }
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
    }
  }

}

#undef maxcount
#undef wid
#undef dec


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fout.f = NULL;
  strcpy(fout.name, "fout");
  tramp.f = NULL;
  strcpy(tramp.name, "tramp");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &tramp, &fout);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (tramp.f != NULL)
    fclose(tramp.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}






























/* End. */
