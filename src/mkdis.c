/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "mkdis.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*
*/
#define updateversion   1.00



/*











































































*/


Static _TEXT distribution, mkdisp, data;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(distribution, mkdisp, data)
_TEXT *distribution, *mkdisp, *data;
{
  double parameterversion;
  long d, dindex, position;

  printf("mkdis %4.2f\n", version);
  if (*mkdisp->name != '\0') {
    if (mkdisp->f != NULL)
      mkdisp->f = freopen(mkdisp->name, "r", mkdisp->f);
    else
      mkdisp->f = fopen(mkdisp->name, "r");
  } else
    rewind(mkdisp->f);
  if (mkdisp->f == NULL)
    _EscIO2(FileNotFound, mkdisp->name);
  RESETBUF(mkdisp->f, Char);
  fscanf(mkdisp->f, "%lg%*[^\n]", &parameterversion);
  getc(mkdisp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*distribution->name != '\0') {
    if (distribution->f != NULL)
      distribution->f = freopen(distribution->name, "r", distribution->f);
    else
      distribution->f = fopen(distribution->name, "r");
  } else
    rewind(distribution->f);
  if (distribution->f == NULL)
    _EscIO2(FileNotFound, distribution->name);
  RESETBUF(distribution->f, Char);
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


  while ((P_peek(distribution->f) == '*') | P_eoln(distribution->f)) {
    fscanf(distribution->f, "%*[^\n]");
    getc(distribution->f);
  }
  fscanf(distribution->f, "%ld%*[^\n]", &position);
  getc(distribution->f);

  while (!BUFEOF(distribution->f)) {
    if ((P_peek(distribution->f) == '*') | P_eoln(distribution->f)) {
      fscanf(distribution->f, "%*[^\n]");
      getc(distribution->f);
      continue;
    }
    fscanf(distribution->f, "%ld%*[^\n]", &d);
    getc(distribution->f);
    for (dindex = 1; dindex <= d; dindex++)
      fprintf(data->f, "%12ld\n", position);
    position++;
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  data.f = NULL;
  strcpy(data.name, "data");
  mkdisp.f = NULL;
  strcpy(mkdisp.name, "mkdisp");
  distribution.f = NULL;
  strcpy(distribution.name, "distribution");
  themain(&distribution, &mkdisp, &data);
_L1:
  if (distribution.f != NULL)
    fclose(distribution.f);
  if (mkdisp.f != NULL)
    fclose(mkdisp.f);
  if (data.f != NULL)
    fclose(data.f);
  exit(EXIT_SUCCESS);
}



/* End. */
