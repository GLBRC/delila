/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "bndrec.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.05
/*
*/
#define updateversion   1.00



/*





























































*/


Static _TEXT bndrecp, bndmarks;


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


#define dec             2
#define wid             10

#define debug           false



Static Void themain(bndrecp, bndmarks)
_TEXT *bndrecp, *bndmarks;
{
  double bits;
  double bitheight = 1.0;
  long counter;
  double displacement = 0.0;
  double left, parameterversion, right;
  Char skip;

  printf("bndrec %4.2f\n", version);
  if (*bndrecp->name != '\0') {
    if (bndrecp->f != NULL)
      bndrecp->f = freopen(bndrecp->name, "r", bndrecp->f);
    else
      bndrecp->f = fopen(bndrecp->name, "r");
  } else
    rewind(bndrecp->f);
  if (bndrecp->f == NULL)
    _EscIO2(FileNotFound, bndrecp->name);
  RESETBUF(bndrecp->f, Char);
  fscanf(bndrecp->f, "%lg%*[^\n]", &parameterversion);
  getc(bndrecp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*bndmarks->name != '\0') {
    if (bndmarks->f != NULL)
      bndmarks->f = freopen(bndmarks->name, "w", bndmarks->f);
    else
      bndmarks->f = fopen(bndmarks->name, "w");
  } else {
    if (bndmarks->f != NULL) {

      rewind(bndmarks->f);
    } else
      bndmarks->f = tmpfile();
  }
  if (bndmarks->f == NULL)
    _EscIO2(FileNotFound, bndmarks->name);
  SETUPBUF(bndmarks->f, Char);
  fprintf(bndmarks->f, "* bndrec %4.2f\n", version);

  if (debug)
    counter = 0;
  while (!BUFEOF(bndrecp->f)) {
    if (debug)
      counter++;
    if (debug)
      printf("%ld\n", counter);
    if (debug)
      printf("bndrecp^ = \"%c\"\n", P_peek(bndrecp->f));
    if (P_eoln(bndrecp->f)) {
      fscanf(bndrecp->f, "%*[^\n]");
      getc(bndrecp->f);
      continue;
    }

    if (P_peek(bndrecp->f) == 'd') {
      fscanf(bndrecp->f, "%c%lg%*[^\n]", &skip, &displacement);
      getc(bndrecp->f);
      if (skip == '\n')
	skip = ' ';
      fprintf(bndmarks->f, "* displacement: %*.*f\n", wid, dec, displacement);
      continue;
    }

    if (P_peek(bndrecp->f) == 'b') {
      fscanf(bndrecp->f, "%c%lg%*[^\n]", &skip, &bitheight);
      getc(bndrecp->f);
      if (skip == '\n')
	skip = ' ';
      fprintf(bndmarks->f, "* bitheight: %*.*f\n", wid, dec, bitheight);
      continue;
    }

    if (P_peek(bndrecp->f) == 'u') {
      do {
	copyaline(bndrecp, bndmarks);
	if (BUFEOF(bndrecp->f)) {
	  printf("no end to user definition (u) found.\n");
	  printf("you need to end it with a '!'\n");
	  halt();
	}
	if (debug)
	  printf("copying\n");
      } while (P_peek(bndrecp->f) != '!');
      copyaline(bndrecp, bndmarks);
      continue;
    }

    if (P_peek(bndrecp->f) == '*') {
      copyaline(bndrecp, bndmarks);
      continue;
    }

    if (debug)
      printf("reading left ...\n");

    fscanf(bndrecp->f, "%lg%lg%lg%*[^\n]", &left, &right, &bits);
    getc(bndrecp->f);
    if (right < left) {
      printf("all right coordinates must be greater than left\n");
      printf("*  left:%*.*f\n", wid, dec, left);
      printf("* right:%*.*f\n", wid, dec, right);
      halt();
    }
    fprintf(bndmarks->f, "\n*  left:%*.*f\n", wid, dec, left);
    fprintf(bndmarks->f, "* right:%*.*f\n", wid, dec, right);

    fprintf(bndmarks->f, "U %*.*f %*.*f %*.*f %*.*f %*.*f boundrectangle\n",
	    wid, dec, displacement, wid, dec, bits, wid, dec,
	    displacement + right - left, wid, dec, bits + bitheight, wid, dec,
	    2 * (left - displacement));

    displacement += 1.0;

  }

  /*


*/

}

#undef dec
#undef wid
#undef debug


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  bndmarks.f = NULL;
  strcpy(bndmarks.name, "bndmarks");
  bndrecp.f = NULL;
  strcpy(bndrecp.name, "bndrecp");
  themain(&bndrecp, &bndmarks);
_L1:
  if (bndrecp.f != NULL)
    fclose(bndrecp.f);
  if (bndmarks.f != NULL)
    fclose(bndmarks.f);
  exit(EXIT_SUCCESS);
}



/* End. */
