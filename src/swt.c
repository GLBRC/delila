/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "swt.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.00
/*
*/
#define updateversion   1.00



/*






























































*/


Static _TEXT swtp, list;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Static double doone(pa, pb)
double pa, pb;
{
  double iab;

  iab = pb * log(pb / pa) / log(2.0) +
	(1 - pb) * log((1 - pb) / (1 - pa)) / log(2.0);
  return iab;
}


Static Void themain(swtp, list)
_TEXT *swtp, *list;
{
  double parameterversion, pa, pb, pc, iab, ibc, iac, icb;

  printf("swt %4.2f\n", version);
  if (*swtp->name != '\0') {
    if (swtp->f != NULL)
      swtp->f = freopen(swtp->name, "r", swtp->f);
    else
      swtp->f = fopen(swtp->name, "r");
  } else
    rewind(swtp->f);
  if (swtp->f == NULL)
    _EscIO2(FileNotFound, swtp->name);
  RESETBUF(swtp->f, Char);
  fscanf(swtp->f, "%lg%*[^\n]", &parameterversion);
  getc(swtp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(swtp->f, "%lg%lg%lg%*[^\n]", &pa, &pb, &pc);
  getc(swtp->f);

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
  fprintf(list->f, "swt %4.2f\n", version);
  fprintf(list->f, " pa = %10.5f\n", pa);
  fprintf(list->f, " pb = %10.5f\n", pb);
  fprintf(list->f, " pc = %10.5f\n\n", pc);

  fprintf(list->f, "For a->b->c we have: \n");
  fprintf(list->f, "Iab = sum pb log pb/pa\n");
  fprintf(list->f, "Ibc = sum pc log pc/pb\n");

  iab = doone(pa, pb);
  ibc = doone(pb, pc);

  fprintf(list->f, "iab + ibc = %10.5f \"bits\"\n\n", iab + ibc);

  fprintf(list->f, "For a->c->b we have:\n");
  fprintf(list->f, "Iac = sum pc log pc/pa\n");
  fprintf(list->f, "Icb = sum pb log pb/pc\n");

  iac = doone(pa, pc);
  icb = doone(pc, pb);
  fprintf(list->f, "iac + icb = %10.5f \"bits\"\n", iac + icb);

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
  swtp.f = NULL;
  strcpy(swtp.name, "swtp");
  themain(&swtp, &list);
_L1:
  if (swtp.f != NULL)
    fclose(swtp.f);
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
