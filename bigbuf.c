/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "bigbuf.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*


*/
#define updateversion   1.00



/*












































































*/


Static _TEXT afile, bigbufp, bfile;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define buffermax       10000



Static Void themain(afile, bigbufp, bfile)
_TEXT *afile, *bigbufp, *bfile;
{
  double parameterversion;
  Char buffer[buffermax];
  long buffersize, bufferstart;
  long c = 0;
  Char ch;
  long l = 0, s = 0;


  printf("bigbuf %4.2f\n", version);
  if (*bigbufp->name != '\0') {
    if (bigbufp->f != NULL)
      bigbufp->f = freopen(bigbufp->name, "r", bigbufp->f);
    else
      bigbufp->f = fopen(bigbufp->name, "r");
  } else
    rewind(bigbufp->f);
  if (bigbufp->f == NULL)
    _EscIO2(FileNotFound, bigbufp->name);
  RESETBUF(bigbufp->f, Char);
  fscanf(bigbufp->f, "%lg%*[^\n]", &parameterversion);
  getc(bigbufp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(bigbufp->f, "%ld%*[^\n]", &buffersize);
  getc(bigbufp->f);
  if (buffersize < 2) {
    printf("minimum buffer allowed is 1\n");
    halt();
  }
  if (buffersize > buffermax) {
    printf("maximum buffer allowed is %ld\n", (long)buffermax);
    halt();
  }

  fscanf(bigbufp->f, "%ld%*[^\n]", &bufferstart);
  getc(bigbufp->f);
  if (bufferstart < 2) {
    printf("minimum buffer start allowed is 1\n");
    halt();
  }
  if (bufferstart > buffermax) {
    printf("maximum buffer start allowed is %ld\n", (long)buffermax);
    halt();
  }

  if (*afile->name != '\0') {
    if (afile->f != NULL)
      afile->f = freopen(afile->name, "r", afile->f);
    else
      afile->f = fopen(afile->name, "r");
  } else
    rewind(afile->f);
  if (afile->f == NULL)
    _EscIO2(FileNotFound, afile->name);
  RESETBUF(afile->f, Char);
  while (!BUFEOF(afile->f)) {
    if (P_eoln(afile->f)) {
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      l++;
      continue;
    }
    while (!P_eoln(afile->f)) {
      ch = getc(afile->f);
      if (ch == '\n')
	ch = ' ';
      c++;
      s++;
      if (s > buffersize)
	s = 1;
      buffer[s-1] = ch;
    }
  }

  if (*bfile->name != '\0') {
    if (bfile->f != NULL)
      bfile->f = freopen(bfile->name, "w", bfile->f);
    else
      bfile->f = fopen(bfile->name, "w");
  } else {
    if (bfile->f != NULL)
      rewind(bfile->f);
    else
      bfile->f = tmpfile();
  }
  if (bfile->f == NULL)
    _EscIO2(FileNotFound, bfile->name);
  SETUPBUF(bfile->f, Char);
  s = 0;
  /*

*/
  while (s < bufferstart) {
    s++;
    putchar(buffer[s-1]);
    putc(buffer[s-1], bfile->f);
  }
  putchar('\n');
  putc('\n', bfile->f);

}

#undef buffermax


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  bfile.f = NULL;
  strcpy(bfile.name, "bfile");
  bigbufp.f = NULL;
  strcpy(bigbufp.name, "bigbufp");
  afile.f = NULL;
  strcpy(afile.name, "afile");
  themain(&afile, &bigbufp, &bfile);
_L1:
  if (afile.f != NULL)
    fclose(afile.f);
  if (bigbufp.f != NULL)
    fclose(bigbufp.f);
  if (bfile.f != NULL)
    fclose(bfile.f);
  exit(EXIT_SUCCESS);
}



/* End. */
