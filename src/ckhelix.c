/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "ckhelix.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.03


/*



*/



/*






































*/


Static _TEXT makelogop, wave, ckhelixp;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(makelogop, ckhelixp)
_TEXT *makelogop, *ckhelixp;
{
  long fromrange, torange, barlocation;
  double xcorner, ycorner;
  long rotation;
  double charwidth;


  double wavelocation, desiredcenter;

  double actualcenter;
  /*
*/
  double correction;
  Char thecorner;

  /*

*/

  printf("ckhelix %4.2f\n", version);

  if (*makelogop->name != '\0') {
    if (makelogop->f != NULL)
      makelogop->f = freopen(makelogop->name, "r", makelogop->f);
    else
      makelogop->f = fopen(makelogop->name, "r");
  } else
    rewind(makelogop->f);
  if (makelogop->f == NULL)
    _EscIO2(FileNotFound, makelogop->name);
  RESETBUF(makelogop->f, Char);
  fscanf(makelogop->f, "%ld%ld%*[^\n]", &fromrange, &torange);
  getc(makelogop->f);
  fscanf(makelogop->f, "%ld%*[^\n]", &barlocation);
  getc(makelogop->f);
  fscanf(makelogop->f, "%lg%lg%*[^\n]", &xcorner, &ycorner);
  getc(makelogop->f);
  fscanf(makelogop->f, "%ld%*[^\n]", &rotation);
  getc(makelogop->f);
  fscanf(makelogop->f, "%lg%*[^\n]", &charwidth);
  getc(makelogop->f);

  if (*ckhelixp->name != '\0') {
    if (ckhelixp->f != NULL)
      ckhelixp->f = freopen(ckhelixp->name, "r", ckhelixp->f);
    else
      ckhelixp->f = fopen(ckhelixp->name, "r");
  } else
    rewind(ckhelixp->f);
  if (ckhelixp->f == NULL)
    _EscIO2(FileNotFound, ckhelixp->name);
  RESETBUF(ckhelixp->f, Char);
  fscanf(ckhelixp->f, "%lg%*[^\n]", &wavelocation);
  getc(ckhelixp->f);
  fscanf(ckhelixp->f, "%lg%*[^\n]", &desiredcenter);
  getc(ckhelixp->f);

  if (rotation != 0 && rotation != -90)
    printf("WARNING: rotation must be 0 or -90 for sensible results!\n");

  if (rotation == -90)
    thecorner = 'y';
  else
    thecorner = 'x';

  if (thecorner == 'x')
    actualcenter = xcorner - (wavelocation - fromrange) * charwidth;
  else
    actualcenter = ycorner - (wavelocation - fromrange) * charwidth;

  printf(
    "          |wave    |         |         |         |         |         |  NEW\n");
  printf(
    "  %ccorner |location|fromrange|charwidth| center  | rotation| change  |  CORNER\n",
    thecorner);


  if (thecorner == 'x')
    printf(" %7.4f |", xcorner);
  else
    printf(" %7.4f |", ycorner);

  printf(" %7.4f |", wavelocation);
  printf(" %7ld |", fromrange);
  printf(" %7.4f |", charwidth);
  printf(" %7.4f |", actualcenter);
  printf(" %7ld |", rotation);
  correction = desiredcenter - actualcenter;
  if (correction != 0.0) {
    printf(" %7.4f |", correction);
    printf(" %7.4f |", ycorner + correction);
  } else
    printf(" (none)");

  putchar('\n');

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  ckhelixp.f = NULL;
  strcpy(ckhelixp.name, "ckhelixp");
  wave.f = NULL;
  *wave.name = '\0';
  makelogop.f = NULL;
  strcpy(makelogop.name, "makelogop");
  themain(&makelogop, &ckhelixp);
_L1:
  if (makelogop.f != NULL)
    fclose(makelogop.f);
  if (wave.f != NULL)
    fclose(wave.f);
  if (ckhelixp.f != NULL)
    fclose(ckhelixp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
