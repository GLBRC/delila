/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "xyda.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00
/*

*/
#define updateversion   1.00



/*

























































*/


Static _TEXT predata, xydap, postdata;


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



Static Void themain(predata, xydap, postdata)
_TEXT *predata, *xydap, *postdata;
{
  double parameterversion, position;
  long number, n;

  printf("xyda %4.2f\n", version);
  if (*xydap->name != '\0') {
    if (xydap->f != NULL)
      xydap->f = freopen(xydap->name, "r", xydap->f);
    else
      xydap->f = fopen(xydap->name, "r");
  } else
    rewind(xydap->f);
  if (xydap->f == NULL)
    _EscIO2(FileNotFound, xydap->name);
  RESETBUF(xydap->f, Char);
  fscanf(xydap->f, "%lg%*[^\n]", &parameterversion);
  getc(xydap->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*postdata->name != '\0') {
    if (postdata->f != NULL)
      postdata->f = freopen(postdata->name, "w", postdata->f);
    else
      postdata->f = fopen(postdata->name, "w");
  } else {
    if (postdata->f != NULL)
      rewind(postdata->f);
    else
      postdata->f = tmpfile();
  }
  if (postdata->f == NULL)
    _EscIO2(FileNotFound, postdata->name);
  SETUPBUF(postdata->f, Char);
  if (*predata->name != '\0') {
    if (predata->f != NULL)
      predata->f = freopen(predata->name, "r", predata->f);
    else
      predata->f = fopen(predata->name, "r");
  } else
    rewind(predata->f);
  if (predata->f == NULL)
    _EscIO2(FileNotFound, predata->name);
  RESETBUF(predata->f, Char);

  while (!BUFEOF(predata->f)) {
    if ((P_peek(predata->f) == '*') | P_eoln(predata->f)) {
      copyaline(predata, postdata);
      continue;
    }
    fscanf(predata->f, "%lg%ld%*[^\n]", &position, &number);
    getc(predata->f);
    printf("%15.8f %5ld\n", position, number);
    if (number < 0) {
      printf("Number is less than zero - bad predata file?\n");
      halt();
    }
    for (n = 1; n <= number; n++)
      fprintf(postdata->f, "%15.8f\n", position);
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  postdata.f = NULL;
  strcpy(postdata.name, "postdata");
  xydap.f = NULL;
  strcpy(xydap.name, "xydap");
  predata.f = NULL;
  strcpy(predata.name, "predata");
  themain(&predata, &xydap, &postdata);
_L1:
  if (predata.f != NULL)
    fclose(predata.f);
  if (xydap.f != NULL)
    fclose(xydap.f);
  if (postdata.f != NULL)
    fclose(postdata.f);
  exit(EXIT_SUCCESS);
}



/* End. */
