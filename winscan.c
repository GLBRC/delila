/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "winscan.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02


typedef struct nametype {
  Char letters[50];
  long length;
} nametype;


/*
*/



/*










































*/


Static _TEXT winscanp, lidelp, fin;


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


Static Void readname(afile, name)
_TEXT *afile;
nametype *name;
{
  name->length = 0;
  while ((!P_eoln(afile->f)) & (P_peek(afile->f) != ' ')) {
    name->length++;
    name->letters[name->length - 1] = getc(afile->f);
    if (name->letters[name->length - 1] == '\n')
      name->letters[name->length - 1] = ' ';
  }
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
}


Static Void writename(afile, name)
_TEXT *afile;
nametype name;
{
  long p;

  for (p = 0; p < name.length; p++)
    putc(name.letters[p], afile->f);
}


Static Void themain(winscanp, lidelp, fin)
_TEXT *winscanp, *lidelp, *fin;
{
  nametype PieceName, OrganismName, ChromosomeName;
  long StartCoordinate, IncrementBetweenWindows, NumberOfWindows, w,
       WindowFrom, WindowTo, WindowCoordinate;

  printf("winscan %4.2f\n", version);


  if (*winscanp->name != '\0') {
    if (winscanp->f != NULL)
      winscanp->f = freopen(winscanp->name, "r", winscanp->f);
    else
      winscanp->f = fopen(winscanp->name, "r");
  } else
    rewind(winscanp->f);
  if (winscanp->f == NULL)
    _EscIO2(FileNotFound, winscanp->name);
  RESETBUF(winscanp->f, Char);
  readname(winscanp, &OrganismName);
  readname(winscanp, &ChromosomeName);
  readname(winscanp, &PieceName);
  fscanf(winscanp->f, "%ld%*[^\n]", &StartCoordinate);
  getc(winscanp->f);
  fscanf(winscanp->f, "%ld%*[^\n]", &NumberOfWindows);
  getc(winscanp->f);
  fscanf(winscanp->f, "%ld%*[^\n]", &IncrementBetweenWindows);
  getc(winscanp->f);
  fscanf(winscanp->f, "%ld%*[^\n]", &WindowFrom);
  getc(winscanp->f);
  fscanf(winscanp->f, "%ld%*[^\n]", &WindowTo);
  getc(winscanp->f);


  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "w", fin->f);
    else
      fin->f = fopen(fin->name, "w");
  } else {
    if (fin->f != NULL)
      rewind(fin->f);
    else
      fin->f = tmpfile();
  }
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  SETUPBUF(fin->f, Char);
  fprintf(fin->f, "* winscan %4.2f\n", version);
  fprintf(fin->f, "* ");
  writename(fin, OrganismName);
  fprintf(fin->f, " (organism)\n");
  fprintf(fin->f, "* ");
  writename(fin, ChromosomeName);
  fprintf(fin->f, " (chromosome)\n");
  fprintf(fin->f, "* ");
  writename(fin, PieceName);
  fprintf(fin->f, " (piece)\n");
  fprintf(fin->f, "* %ld start coordinate\n", StartCoordinate);
  fprintf(fin->f, "* %ld number of windows\n", NumberOfWindows);
  fprintf(fin->f, "* %ld increment between windows\n",
	  IncrementBetweenWindows);
  fprintf(fin->f, "* %ld window from\n", WindowFrom);
  fprintf(fin->f, "* %ld window to\n", WindowTo);


  if (*lidelp->name != '\0') {
    if (lidelp->f != NULL)
      lidelp->f = freopen(lidelp->name, "w", lidelp->f);
    else
      lidelp->f = fopen(lidelp->name, "w");
  } else {
    if (lidelp->f != NULL)
      rewind(lidelp->f);
    else
      lidelp->f = tmpfile();
  }
  if (lidelp->f == NULL)
    _EscIO2(FileNotFound, lidelp->name);
  SETUPBUF(lidelp->f, Char);
  fprintf(lidelp->f, "%ld %ld fromrange torange\n", WindowFrom, WindowTo);
  fprintf(lidelp->f, "1 column number of piece name\n");
  fprintf(lidelp->f, "2 column number of coordinate data\n");
  fprintf(lidelp->f, "CDS (fake value)\n");
  writename(lidelp, OrganismName);
  putc('\n', lidelp->f);
  writename(lidelp, ChromosomeName);
  fprintf(lidelp->f, "\no   symmetry (e)ven or (o)dd, (l)eft or (r)ight\n");
  fprintf(lidelp->f, "1   number of instructions to write to inst\n");
  fprintf(lidelp->f, "parameters for lidel.p created by winscan %4.2f\n",
	  version);


  if (WindowTo - WindowFrom + 1 > IncrementBetweenWindows) {
    printf("WINDOWS OVERLAP\n");
    fprintf(fin->f, "* WINDOWS OVERLAP\n");
  } else {
    printf("windows do not overlap\n");
    fprintf(fin->f, "* windows do not overlap\n");
  }



  WindowCoordinate = StartCoordinate;
  for (w = 1; w <= NumberOfWindows; w++) {
    writename(fin, PieceName);
    fprintf(fin->f, " %ld\n", WindowCoordinate);
    WindowCoordinate += IncrementBetweenWindows;
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fin.f = NULL;
  strcpy(fin.name, "fin");
  lidelp.f = NULL;
  strcpy(lidelp.name, "lidelp");
  winscanp.f = NULL;
  strcpy(winscanp.name, "winscanp");
  themain(&winscanp, &lidelp, &fin);
_L1:
  if (winscanp.f != NULL)
    fclose(winscanp.f);
  if (lidelp.f != NULL)
    fclose(lidelp.f);
  if (fin.f != NULL)
    fclose(fin.f);
  exit(EXIT_SUCCESS);
}
































/* End. */
