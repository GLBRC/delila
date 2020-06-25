/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "shell.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         3.03
/*









*/
#define updateversion   1.00



/*





























































































































































*/


Static _TEXT afile, shellp, bfile;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(afile, shellp, bfile)
_TEXT *afile, *shellp, *bfile;
{
  double parameterversion;

  printf("shell %4.2f\n", version);
  if (*shellp->name != '\0') {
    if (shellp->f != NULL)
      shellp->f = freopen(shellp->name, "r", shellp->f);
    else
      shellp->f = fopen(shellp->name, "r");
  } else
    rewind(shellp->f);
  if (shellp->f == NULL)
    _EscIO2(FileNotFound, shellp->name);
  RESETBUF(shellp->f, Char);
  fscanf(shellp->f, "%lg%*[^\n]", &parameterversion);
  getc(shellp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  bfile.f = NULL;
  strcpy(bfile.name, "bfile");
  shellp.f = NULL;
  strcpy(shellp.name, "shellp");
  afile.f = NULL;
  strcpy(afile.name, "afile");
  themain(&afile, &shellp, &bfile);
_L1:
  if (afile.f != NULL)
    fclose(afile.f);
  if (shellp.f != NULL)
    fclose(shellp.f);
  if (bfile.f != NULL)
    fclose(bfile.f);
  exit(EXIT_SUCCESS);
}



/* End. */
