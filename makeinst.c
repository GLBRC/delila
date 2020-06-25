/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "makeinst.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*










*/



#define version         1.01
/*


*/
#define updateversion   1.00



/*


































































*/


Static _TEXT instA, instB, makeinstp, dcout;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(instA, instB, makeinstp, dcout)
_TEXT *instA, *instB, *makeinstp, *dcout;
{
  double parameterversion;
  long fromrange, torange, coordA, coordB, diff;
  Char c;

  printf("makeinst %4.2f\n", version);
  if (*makeinstp->name != '\0') {
    if (makeinstp->f != NULL)
      makeinstp->f = freopen(makeinstp->name, "r", makeinstp->f);
    else
      makeinstp->f = fopen(makeinstp->name, "r");
  } else
    rewind(makeinstp->f);
  if (makeinstp->f == NULL)
    _EscIO2(FileNotFound, makeinstp->name);
  RESETBUF(makeinstp->f, Char);
  fscanf(makeinstp->f, "%lg%*[^\n]", &parameterversion);
  getc(makeinstp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }
  fscanf(makeinstp->f, "%ld%ld%*[^\n]", &fromrange, &torange);
  getc(makeinstp->f);

  if (*dcout->name != '\0') {
    if (dcout->f != NULL)
      dcout->f = freopen(dcout->name, "r", dcout->f);
    else
      dcout->f = fopen(dcout->name, "r");
  } else
    rewind(dcout->f);
  if (dcout->f == NULL)
    _EscIO2(FileNotFound, dcout->name);
  RESETBUF(dcout->f, Char);
  if (*instA->name != '\0') {
    if (instA->f != NULL)
      instA->f = freopen(instA->name, "w", instA->f);
    else
      instA->f = fopen(instA->name, "w");
  } else {
    if (instA->f != NULL)
      rewind(instA->f);
    else
      instA->f = tmpfile();
  }
  if (instA->f == NULL)
    _EscIO2(FileNotFound, instA->name);
  SETUPBUF(instA->f, Char);
  if (*instB->name != '\0') {
    if (instB->f != NULL)
      instB->f = freopen(instB->name, "w", instB->f);
    else
      instB->f = fopen(instB->name, "w");
  } else {
    if (instB->f != NULL)
      rewind(instB->f);
    else
      instB->f = tmpfile();
  }
  if (instB->f == NULL)
    _EscIO2(FileNotFound, instB->name);
  SETUPBUF(instB->f, Char);
  while (!BUFEOF(dcout->f)) {
    c = getc(dcout->f);
    if (c == '\n')
      c = ' ';
    if (c == '*') {
      fscanf(dcout->f, "%*[^\n]");
      getc(dcout->f);
      continue;
    }
    fscanf(dcout->f, "%ld%ld%*[^\n]", &coordA, &coordB);
    getc(dcout->f);
    diff = coordA - coordB;
    if (diff >= 0) {
      fprintf(instA->f, "get from %ld %ld to same +%ld;\n",
	      coordA, fromrange, torange);
      fprintf(instB->f, "get from %ld %ld to same +%ld;\n",
	      coordB, fromrange, torange);
    }
    if (diff < 0) {
      fprintf(instA->f, "get from %ld +%ld to same -%ld;\n",
	      coordA, -fromrange, torange);
      fprintf(instB->f, "get from %ld +%ld to same -%ld;\n",
	      coordB, -fromrange, torange);
    }
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  dcout.f = NULL;
  strcpy(dcout.name, "dcout");
  makeinstp.f = NULL;
  strcpy(makeinstp.name, "makeinstp");
  instB.f = NULL;
  strcpy(instB.name, "instB");
  instA.f = NULL;
  strcpy(instA.name, "instA");
  themain(&instA, &instB, &makeinstp, &dcout);
_L1:
  if (instA.f != NULL)
    fclose(instA.f);
  if (instB.f != NULL)
    fclose(instB.f);
  if (makeinstp.f != NULL)
    fclose(makeinstp.f);
  if (dcout.f != NULL)
    fclose(dcout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
