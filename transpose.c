/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "transpose.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.05
/*






*/
#define updateversion   1.05



/*






















































*/


#define maxstore        1000


Static _TEXT fin, transposep, fout;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define tab             9






Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == tab);
}

#undef tab


Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while (isblankDelila(P_peek(thefile->f)) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((!isblankDelila(P_peek(thefile->f))) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
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


/* Local variables for themain: */
struct LOC_themain {
  double store[4][maxstore];
  long count, wid, dec, zerobase;
} ;

Local Void writeout(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  long base, c, FORLIM;

  FORLIM = LINK->count;
  for (c = 0; c < FORLIM; c++) {
    fprintf(f->f, "%4ld", c - LINK->zerobase + 1);
    for (base = 0; base <= 3; base++) {
      if (LINK->dec > 0)
	fprintf(f->f, " %*.*f",
		(int)LINK->wid, (int)LINK->dec, LINK->store[base][c]);
      else
	fprintf(f->f, " %*ld",
		(int)LINK->wid, (long)floor(LINK->store[base][c] + 0.5));
    }
    putc('\n', f->f);
  }
}



Static Void themain(fin, transposep, fout)
_TEXT *fin, *transposep, *fout;
{
  struct LOC_themain V;
  double parameterversion;
  long base, c, FORLIM1;
  _TEXT TEMP;

  printf("transpose %4.2f\n", version);

  if (*transposep->name != '\0') {
    if (transposep->f != NULL)
      transposep->f = freopen(transposep->name, "r", transposep->f);
    else
      transposep->f = fopen(transposep->name, "r");
  } else
    rewind(transposep->f);
  if (transposep->f == NULL)
    _EscIO2(FileNotFound, transposep->name);
  RESETBUF(transposep->f, Char);
  fscanf(transposep->f, "%lg%*[^\n]", &parameterversion);
  getc(transposep->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(transposep->f, "%ld%*[^\n]", &V.wid);
  getc(transposep->f);
  fscanf(transposep->f, "%ld%*[^\n]", &V.dec);
  getc(transposep->f);
  fscanf(transposep->f, "%ld%*[^\n]", &V.zerobase);
  getc(transposep->f);

  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  RESETBUF(fin->f, Char);
  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);


  copyaline(fin, fout);


  printf("Reading data. \"+\" = accepted number, \"-\" = not a number\n");
  for (base = 0; base <= 3; base++) {
    V.count = 0;
    while (!P_eoln(fin->f)) {
      skipblanks(fin);
      if (P_peek(fin->f) == '9' || P_peek(fin->f) == '8' ||
	  P_peek(fin->f) == '7' || P_peek(fin->f) == '6' ||
	  P_peek(fin->f) == '5' || P_peek(fin->f) == '4' ||
	  P_peek(fin->f) == '3' || P_peek(fin->f) == '2' ||
	  P_peek(fin->f) == '1' || P_peek(fin->f) == '0') {
	V.count++;
	fscanf(fin->f, "%lg", &V.store[base][V.count-1]);
	putchar('+');
      } else {
	putchar('-');
	skipcolumn(fin);
      }
    }
    printf(" base %ld\n", base + 1);
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
  }

  printf("\nnumber of columns read: %ld\n", V.count);

  for (base = 1; base <= 4; base++) {
    printf("base %ld |", base);
    FORLIM1 = V.count;
    for (c = 0; c < FORLIM1; c++)
      printf(" % .1E", V.store[base-1][c]);
    putchar('\n');
  }

  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeout(&TEMP, &V);
  writeout(fout, &V);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fout.f = NULL;
  strcpy(fout.name, "fout");
  transposep.f = NULL;
  strcpy(transposep.name, "transposep");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &transposep, &fout);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (transposep.f != NULL)
    fclose(transposep.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
