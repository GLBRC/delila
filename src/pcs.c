/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "pcs.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*
*/



/*




























*/



#define wid             10
#define dec             5


Static _TEXT list, pcsp;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static double chi(observed, expected)
double observed, expected;
{
  double TEMP;

  TEMP = observed - expected;
  return (TEMP * TEMP / expected);
}



Static Void displaychi(f, name, number, partialchi, totalchi)
_TEXT *f;
Char name;
double number, partialchi, totalchi;
{
  fprintf(f->f, "   %c   %5ld %9.2f %5ld%%\n",
	  name, (long)floor(number + 0.5), partialchi,
	  (long)floor(100 * partialchi / totalchi + 0.5));
}



Static Void themain(pcsp, list)
_TEXT *pcsp, *list;
{
  double a, c, g, t, chisquared, chia, chic, chig, chit, expected, total;

  printf("pcs %4.2f\n", version);
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
  fprintf(list->f, "pcs %4.2f\n", version);
  if (*pcsp->name != '\0') {
    if (pcsp->f != NULL)
      pcsp->f = freopen(pcsp->name, "r", pcsp->f);
    else
      pcsp->f = fopen(pcsp->name, "r");
  } else
    rewind(pcsp->f);
  if (pcsp->f == NULL)
    _EscIO2(FileNotFound, pcsp->name);
  RESETBUF(pcsp->f, Char);

  while (!BUFEOF(pcsp->f)) {
    fscanf(pcsp->f, "%lg%lg%lg%lg%*[^\n]", &a, &c, &g, &t);
    getc(pcsp->f);
    total = a + c + g + t;
    expected = total / 4.0;

    chia = chi(a, expected);
    chic = chi(c, expected);
    chig = chi(g, expected);
    chit = chi(t, expected);

    chisquared = chia + chic + chig + chit;

    fprintf(list->f, "\nsymbol  number  partial  %% of\n");
    fprintf(list->f, "                chi^2    total\n");
    displaychi(list, 'a', a, chia, chisquared);
    displaychi(list, 'c', c, chic, chisquared);
    displaychi(list, 'g', g, chig, chisquared);
    displaychi(list, 't', t, chit, chisquared);
    fprintf(list->f, " Total %5ld %9.2f   100%%\n",
	    (long)floor(total + 0.5), chisquared);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  pcsp.f = NULL;
  strcpy(pcsp.name, "pcsp");
  list.f = NULL;
  strcpy(list.name, "list");
  themain(&pcsp, &list);
_L1:
  if (list.f != NULL)
    fclose(list.f);
  if (pcsp.f != NULL)
    fclose(pcsp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
