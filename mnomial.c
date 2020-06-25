/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "mnomial.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.19
/*
*/



/*































*/



#define tolerance       0.001

#define maxtable        100
#define wid             10
#define dec             8



typedef double lnfactarray[maxtable + 1];


Static _TEXT mnomialp, list;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Local double lnpower(p, n)
double p;
long n;
{
  /*
*/
  if (p > 0)
    return (n * log(p));
  else
    return 0.0;
}



Static double pmultinomial(n, na, nc, ng, nt, p, pa, pc, pg, pt, lnfact)
long *n, *na, *nc, *ng, *nt;
double *p, *pa, *pc, *pg, *pt;
double *lnfact;
{
  /*








*/
  *p = *pa + *pc + *pg + *pt;
  if (*p < 1.0 - tolerance || *p > 1.0 + tolerance) {
    printf("p is not 1.0 within the tolerance\n");
    halt();
  }

  *n = *na + *nc + *ng + *nt;
  if (*n > maxtable) {
    printf(" n too big for program, increase maxtable\n");
    halt();
  }

  return exp(lnfact[*n] - lnfact[*na] - lnfact[*nc] - lnfact[*ng] -
	     lnfact[*nt] + lnpower(*pa, *na) + lnpower(*pc, *nc) +
	     lnpower(*pg, *ng) + lnpower(*pt, *nt));
}



Static Void makelnfact(lnfact, n)
double *lnfact;
long n;
{
  long i;

  lnfact[0] = 0.0;
  for (i = 1; i <= n; i++)
    lnfact[i] = lnfact[i-1] + log((double)i);
}




Static Void themain(mnomialp, list)
_TEXT *mnomialp, *list;
{
  lnfactarray lnfact;
  long n, na, nc, ng, nt;
  double p, pa, pc, pg, pt, pmul;
  double sum = 0.0;

  printf("mnomial %4.2f\n", version);
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
  fprintf(list->f, "mnomial %4.2f\n", version);

  makelnfact(lnfact, (long)maxtable);

  if (*mnomialp->name != '\0') {
    if (mnomialp->f != NULL)
      mnomialp->f = freopen(mnomialp->name, "r", mnomialp->f);
    else
      mnomialp->f = fopen(mnomialp->name, "r");
  } else
    rewind(mnomialp->f);
  if (mnomialp->f == NULL)
    _EscIO2(FileNotFound, mnomialp->name);
  RESETBUF(mnomialp->f, Char);
  while (!BUFEOF(mnomialp->f)) {
    fscanf(mnomialp->f, "%ld%ld%ld%ld%*[^\n]", &na, &nc, &ng, &nt);
    getc(mnomialp->f);
    fscanf(mnomialp->f, "%lg%lg%lg%lg%*[^\n]", &pa, &pc, &pg, &pt);
    getc(mnomialp->f);

    pmul = pmultinomial(&n, &na, &nc, &ng, &nt, &p, &pa, &pc, &pg, &pt,
			lnfact);
    sum += pmul;

    fprintf(list->f, "-----\n");
    if (ng == 0 && nt == 0) {
      fprintf(list->f, " Binomial: \n");
      fprintf(list->f, "    n  = %*ld\n", wid, n);
      fprintf(list->f, "    pa = %*.*f\n", wid, dec, pa);
      fprintf(list->f, "    mean = n*pa = %*.*f\n", wid, dec, n * pa);
      fprintf(list->f, "    sigma = sqrt(n*pa*(1-pa)) = %*.*f\n\n",
	      wid, dec, sqrt(n * pa * (1 - pa)));
    }
    fprintf(list->f, " na= %*ld nc= %*ld ng= %*ld nt= %*ld\n",
	    wid, na, wid, nc, wid, ng, wid, nt);
    fprintf(list->f, " pa= %*.*f pc= %*.*f pg= %*.*f pt= %*.*f\n\n",
	    wid, dec, pa, wid, dec, pc, wid, dec, pg, wid, dec, pt);
    fprintf(list->f, " p(na,nc,ng,nt|pa,pc,pg,pt)=%*.*f\n", wid, dec, pmul);
    fprintf(list->f, "              running sum = %*.*f\n", wid, dec, sum);
  }
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
  mnomialp.f = NULL;
  strcpy(mnomialp.name, "mnomialp");
  themain(&mnomialp, &list);
_L1:
  if (mnomialp.f != NULL)
    fclose(mnomialp.f);
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
