/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "truncround.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*


*/
#define updateversion   1.00



/*


































































































*/


Static _TEXT afile, truncroundp, xyin;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define wid             5
#define dec             2



Static Void themain(afile, truncroundp, xyin)
_TEXT *afile, *truncroundp, *xyin;
{
  double parameterversion, x, xmin, xmax, yr, yrd, yt, ytd, yz, yzd, zxd;

  printf("truncround %4.2f\n", version);
  if (*truncroundp->name != '\0') {
    if (truncroundp->f != NULL)
      truncroundp->f = freopen(truncroundp->name, "r", truncroundp->f);
    else
      truncroundp->f = fopen(truncroundp->name, "r");
  } else
    rewind(truncroundp->f);
  if (truncroundp->f == NULL)
    _EscIO2(FileNotFound, truncroundp->name);
  RESETBUF(truncroundp->f, Char);
  fscanf(truncroundp->f, "%lg%*[^\n]", &parameterversion);
  getc(truncroundp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*xyin->name != '\0') {
    if (xyin->f != NULL)
      xyin->f = freopen(xyin->name, "w", xyin->f);
    else
      xyin->f = fopen(xyin->name, "w");
  } else {
    if (xyin->f != NULL)
      rewind(xyin->f);
    else
      xyin->f = tmpfile();
  }
  if (xyin->f == NULL)
    _EscIO2(FileNotFound, xyin->name);
  SETUPBUF(xyin->f, Char);
  fprintf(xyin->f, "* truncround %4.2f\n", version);

  fscanf(truncroundp->f, "%lg%*[^\n]", &xmin);
  getc(truncroundp->f);
  fscanf(truncroundp->f, "%lg%*[^\n]", &xmax);
  getc(truncroundp->f);
  fscanf(truncroundp->f, "%lg%*[^\n]", &yrd);
  getc(truncroundp->f);
  fscanf(truncroundp->f, "%lg%*[^\n]", &ytd);
  getc(truncroundp->f);
  fscanf(truncroundp->f, "%lg%*[^\n]", &yzd);
  getc(truncroundp->f);
  fscanf(truncroundp->f, "%lg%*[^\n]", &zxd);
  getc(truncroundp->f);

  /*






*/

  x = xmin;
  while (x < xmax) {
    yt = (long)x + ytd;
    yr = (long)floor(x + 0.5) + yrd;
    yz = (long)floor(x + zxd + 0.5) - 1.0 + yzd;

    fprintf(xyin->f, "%*.*f %*.*f t\n", wid, dec, x, wid, dec, yt);
    fprintf(xyin->f, "%*.*f %*.*f r\n", wid, dec, x, wid, dec, yr);
    fprintf(xyin->f, "%*.*f %*.*f z\n", wid, dec, x, wid, dec, yz);
    x += 0.1;
  }


}

#undef wid
#undef dec


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  truncroundp.f = NULL;
  strcpy(truncroundp.name, "truncroundp");
  afile.f = NULL;
  strcpy(afile.name, "afile");
  themain(&afile, &truncroundp, &xyin);
_L1:
  if (afile.f != NULL)
    fclose(afile.f);
  if (truncroundp.f != NULL)
    fclose(truncroundp.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
