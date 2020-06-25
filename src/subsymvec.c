/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "subsymvec.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.01
/*

*/
#define updateversion   1.00



/*










































*/


/*

*/

#define infofield       8
#define infodecim       5

#define nfield          4
#define mapmax          26

#define pwid            8
#define pdec            5

#define pnum            100000L

#define cxmove          30


Static _TEXT symvecin, subsymvecp, symvecout;


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



Static Void themain(symvecin, subsymvecp, symvecout)
_TEXT *symvecin, *subsymvecp, *symvecout;
{
  double parameterversion;
  long s, sindex;

  long fromrange, torange;

  double r, v;
  long n, l;

  boolean startup = true;

  printf("subsymvec %4.2f\n", version);
  if (*subsymvecp->name != '\0') {
    if (subsymvecp->f != NULL)
      subsymvecp->f = freopen(subsymvecp->name, "r", subsymvecp->f);
    else
      subsymvecp->f = fopen(subsymvecp->name, "r");
  } else
    rewind(subsymvecp->f);
  if (subsymvecp->f == NULL) {

    _EscIO2(FileNotFound, subsymvecp->name);
  }
  RESETBUF(subsymvecp->f, Char);
  fscanf(subsymvecp->f, "%lg%*[^\n]", &parameterversion);
  getc(subsymvecp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }
  fscanf(subsymvecp->f, "%ld%ld%*[^\n]", &fromrange, &torange);
  getc(subsymvecp->f);
  printf("* extraction from: %ld", fromrange);
  printf(" to: %ld\n", torange);

  if (*symvecin->name != '\0') {
    if (symvecin->f != NULL)
      symvecin->f = freopen(symvecin->name, "r", symvecin->f);
    else
      symvecin->f = fopen(symvecin->name, "r");
  } else
    rewind(symvecin->f);
  if (symvecin->f == NULL)
    _EscIO2(FileNotFound, symvecin->name);
  RESETBUF(symvecin->f, Char);
  if (*symvecout->name != '\0') {
    if (symvecout->f != NULL)
      symvecout->f = freopen(symvecout->name, "w", symvecout->f);
    else
      symvecout->f = fopen(symvecout->name, "w");
  } else {
    if (symvecout->f != NULL)
      rewind(symvecout->f);
    else
      symvecout->f = tmpfile();
  }
  if (symvecout->f == NULL)
    _EscIO2(FileNotFound, symvecout->name);
  SETUPBUF(symvecout->f, Char);
  fprintf(symvecout->f, "* subsymvec %4.2f\n", version);
  fprintf(symvecout->f, "* extraction from: %ld", fromrange);
  fprintf(symvecout->f, " to: %ld\n", torange);

  while (!BUFEOF(symvecin->f)) {
    if (P_peek(symvecin->f) == '*') {
      copyaline(symvecin, symvecout);
      continue;
    }
    if (startup) {
      fscanf(symvecin->f, "%ld%*[^\n]", &s);
      getc(symvecin->f);
      printf("%ld number of symbols\n", s);
      fprintf(symvecout->f, "%ld number of symbols\n", s);
      startup = false;
      continue;
    }
    fscanf(symvecin->f, "%ld%ld%lg%lg%*[^\n]", &l, &n, &r, &v);
    getc(symvecin->f);

    /*




*/

    if (l >= fromrange && l <= torange) {
      fprintf(symvecout->f, "%*ld %*ld %*.*f % .*E\n",
	      nfield, l, infofield, n, infofield, infodecim, r,
	      P_max((int)(infofield + 3) - 7, 1), v);

      for (sindex = 1; sindex <= s; sindex++)
	copyaline(symvecin, symvecout);
    }


    else {
      for (sindex = 1; sindex <= s; sindex++) {
	fscanf(symvecin->f, "%*[^\n]");
	/*

*/
	getc(symvecin->f);
      }
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
  symvecout.f = NULL;
  strcpy(symvecout.name, "symvecout");
  subsymvecp.f = NULL;
  strcpy(subsymvecp.name, "subsymvecp");
  symvecin.f = NULL;
  strcpy(symvecin.name, "symvecin");
  themain(&symvecin, &subsymvecp, &symvecout);
_L1:
  if (symvecin.f != NULL)
    fclose(symvecin.f);
  if (subsymvecp.f != NULL)
    fclose(subsymvecp.f);
  if (symvecout.f != NULL)
    fclose(symvecout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
