/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dem.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*



*/
#define updateversion   1.00



/*















































*/



/*

*/
#define errortolerance  1e-15


Static _TEXT demp, list;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static long countdigits(a, b, errortolerance_)
double a, b, errortolerance_;
{
  /*


*/
  double achop, bchop;
  long atrunc, btrunc, et;
  boolean equal = true;
  long pindex = 0;

  et = (long)floor(0.5 - log(errortolerance_) / log(10.0));
  achop = fabs(a);
  bchop = fabs(b);
  while (equal && pindex < et) {
    pindex++;
    achop = 10 * achop;
    bchop = 10 * bchop;
    atrunc = (long)achop;
    btrunc = (long)bchop;

    if (atrunc != btrunc)
      equal = false;
  }
  return (pindex - 1);
}



Static Void themain(demp, data)
_TEXT *demp, *data;
{
  double parameterversion, theoretical, measured, delta;
  long places;
  /*
*/
  long pindex;
  double identicaldigits;

  printf("dem %4.2f\n", version);
  if (*demp->name != '\0') {
    if (demp->f != NULL)
      demp->f = freopen(demp->name, "r", demp->f);
    else
      demp->f = fopen(demp->name, "r");
  } else
    rewind(demp->f);
  if (demp->f == NULL)
    _EscIO2(FileNotFound, demp->name);
  RESETBUF(demp->f, Char);
  fscanf(demp->f, "%lg%*[^\n]", &parameterversion);
  getc(demp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(demp->f, "%lg%*[^\n]", &theoretical);
  getc(demp->f);
  fscanf(demp->f, "%lg%*[^\n]", &measured);
  getc(demp->f);

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
  fprintf(data->f, "dem %4.2f\n", version);
  fprintf(data->f,
	  "theoretical    =  %10.8f (%6.4f) theoretical maximum efficiency\n",
	  theoretical, theoretical);

  fprintf(data->f, "measured       =  %10.8f (%6.4f) measured efficiency\n",
	  measured, measured);

  delta = measured - theoretical;
  fprintf(data->f, "delta          = ");
  if (delta >= 0)
    putc('+', data->f);
  fprintf(data->f, "%10.8f delta of efficiency\n", delta);
  fprintf(data->f, "                    ");

  places = countdigits(measured, theoretical, errortolerance);

  for (pindex = 1; pindex <= places; pindex++)
    putc('*', data->f);
  fprintf(data->f, "\nplaces identical:  %*c%ld\n", (int)places, ' ', places);
  if (fabs(delta) > 0)
    identicaldigits = fabs(log(fabs(delta)) / log(10.0));
  else
    identicaldigits = 0.0;
  fprintf(data->f, "identical_digits__1%*c%3.1f%9c   (to  1 decimal)\n",
	  (int)places, ' ', identicaldigits, ' ');
  fprintf(data->f, "identical_digits_10%*c%10.10f   (to 10 decimals)\n",
	  (int)places, ' ', identicaldigits);
  if (places == 0)
    fprintf(data->f, "no places match exactly\n");

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  list.f = NULL;
  strcpy(list.name, "list");
  demp.f = NULL;
  strcpy(demp.name, "demp");
  themain(&demp, &list);
_L1:
  if (demp.f != NULL)
    fclose(demp.f);
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
