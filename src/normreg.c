/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "normreg.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.11


/*
*/



/*























































































































*/





Static _TEXT normregp, fresep;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


/* Local variables for themain: */
struct LOC_themain {
  long maxsequences;
} ;

Local long roundem(f, LINK)
double f;
struct LOC_themain *LINK;
{
  return ((long)floor(LINK->maxsequences * f + 0.5));


}



Static Void themain(normregp, fresep)
_TEXT *normregp, *fresep;
{
  struct LOC_themain V;
  double A, C, G, T, Afrequency, Cfrequency, Gfrequency, Tfrequency;
  long Aintegerbase, Ainteger, Cinteger, Ginteger, Tinteger, amount, sign,
       deviation;
  double expA, expC, expG, expT, expSum;
  long SumInteger;
  double ln2 = log(2.0);
  long position;
  double Rseq;

  printf("normreg %4.2f\n", version);
  if (*normregp->name != '\0') {
    if (normregp->f != NULL)
      normregp->f = freopen(normregp->name, "r", normregp->f);
    else
      normregp->f = fopen(normregp->name, "r");
  } else
    rewind(normregp->f);
  if (normregp->f == NULL)
    _EscIO2(FileNotFound, normregp->name);
  RESETBUF(normregp->f, Char);
  if (*fresep->name != '\0') {
    if (fresep->f != NULL)
      fresep->f = freopen(fresep->name, "w", fresep->f);
    else
      fresep->f = fopen(fresep->name, "w");
  } else {
    if (fresep->f != NULL)
      rewind(fresep->f);
    else
      fresep->f = tmpfile();
  }
  if (fresep->f == NULL)
    _EscIO2(FileNotFound, fresep->name);
  SETUPBUF(fresep->f, Char);


  fscanf(normregp->f, "%ld%*[^\n]", &V.maxsequences);
  getc(normregp->f);

  printf("information     Afrequency Cfrequency Gfrequency Tfrequency\n");
  while (!BUFEOF(normregp->f)) {
    fscanf(normregp->f, "%ld%lg%lg%lg%lg%*[^\n]", &position, &A, &C, &G, &T);
    getc(normregp->f);
    expA = exp(A);
    expC = exp(C);
    expG = exp(G);
    expT = exp(T);
    expSum = expA + expC + expG + expT;

    Afrequency = expA / expSum;
    Cfrequency = expC / expSum;
    Gfrequency = expG / expSum;
    Tfrequency = expT / expSum;

    Rseq = (Afrequency * log(Afrequency) + Cfrequency * log(Cfrequency) +
	    Gfrequency * log(Gfrequency) + Tfrequency * log(Tfrequency)) / ln2 - -2;

    printf(" %10.4f", Rseq);
    printf(" %10.4f", Afrequency);
    printf(" %10.4f", Cfrequency);
    printf(" %10.4f", Gfrequency);
    printf(" %10.4f\n", Tfrequency);

    Ainteger = roundem(Afrequency, &V);
    Cinteger = roundem(Cfrequency, &V);
    Ginteger = roundem(Gfrequency, &V);
    Tinteger = roundem(Tfrequency, &V);

    SumInteger = Ainteger + Cinteger + Ginteger + Tinteger;

    /*

*/
    amount = 1;
    sign = 1;
    deviation = 1;

    Aintegerbase = Ainteger;

    while (SumInteger != V.maxsequences) {
      printf("SumInteger = %ld maxsequences  = %ld\n",
	     SumInteger, V.maxsequences);

      Ainteger = Aintegerbase + amount;
      printf(
	"at position %ld, %ld was added to Ainteger to get them to sum properly\n",
	position, amount);
      SumInteger = Ainteger + Cinteger + Ginteger + Tinteger;
      sign = -sign;
      if (sign > 0)
	deviation++;
      amount = sign * deviation;
    }

    fprintf(fresep->f, " %10ld", position);
    fprintf(fresep->f, " %10ld", Ainteger);
    fprintf(fresep->f, " %10ld", Cinteger);
    fprintf(fresep->f, " %10ld", Ginteger);
    fprintf(fresep->f, " %10ld\n", Tinteger);

  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fresep.f = NULL;
  strcpy(fresep.name, "fresep");
  normregp.f = NULL;
  strcpy(normregp.name, "normregp");
  themain(&normregp, &fresep);
_L1:
  if (normregp.f != NULL)
    fclose(normregp.f);
  if (fresep.f != NULL)
    fclose(fresep.f);
  exit(EXIT_SUCCESS);
}



/* End. */
