/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "memory.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.07
/*








*/
#define updateversion   1.07



/*






































































*/



#define maxstring       2000




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;



Static _TEXT memoryp, piechartp, pienumbers;


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


Local Void skipword(fin, fout)
_TEXT *fin, *fout;
{
  Char c;

  while (P_peek(fin->f) == ' ') {
    c = getc(fin->f);
    if (c == '\n')
      c = ' ';
    putc(c, fout->f);
  }
  while (P_peek(fin->f) != ' ') {
    c = getc(fin->f);
    if (c == '\n')
      c = ' ';
    putc(c, fout->f);
  }
}



Static Void themain(input, memoryp, pienumbers, piechartp)
_TEXT *input, *memoryp, *pienumbers, *piechartp;
{
  double parameterversion;
  _TEXT title;
  long fontsize;

  double wired, active, inactive, used, free;

  long wid = 9, dec = 3;
  double sum, freefraction, wiredfraction, activefraction, inactivefraction;
  long MperG = 1024;

  boolean testinput = true;
  Char c;
  _TEXT TEMP;

  title.f = NULL;
  *title.name = '\0';
  printf("memory %4.2f\n", version);
  if (*memoryp->name != '\0') {
    if (memoryp->f != NULL)
      memoryp->f = freopen(memoryp->name, "r", memoryp->f);
    else
      memoryp->f = fopen(memoryp->name, "r");
  } else
    rewind(memoryp->f);
  if (memoryp->f == NULL)
    _EscIO2(FileNotFound, memoryp->name);
  RESETBUF(memoryp->f, Char);
  fscanf(memoryp->f, "%lg%*[^\n]", &parameterversion);
  getc(memoryp->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }
  if (*title.name != '\0') {
    if (title.f != NULL)
      title.f = freopen(title.name, "w", title.f);
    else
      title.f = fopen(title.name, "w");
  } else {
    if (title.f != NULL)
      rewind(title.f);
    else
      title.f = tmpfile();
  }
  if (title.f == NULL)
    _EscIO2(FileNotFound, title.name);
  SETUPBUF(title.f, Char);
  copyaline(memoryp, &title);
  fscanf(memoryp->f, "%ld%*[^\n]", &fontsize);
  getc(memoryp->f);

  testinput = false;
  if (testinput) {
    while (!P_eoln(input->f)) {
      c = getc(input->f);
      if (c == '\n')
	c = ' ';
      putchar(c);
    }
    putchar('\n');
    halt();
  }

  TEMP.f = stdout;
  *TEMP.name = '\0';

  /*





*/

  /*











*/

  /*

*/

  skipword(input, &TEMP);

  fscanf(input->f, "%lg", &wired);
  printf(" %ld", (long)floor(wired + 0.5));
  c = getc(input->f);
  if (c == '\n')
    c = ' ';
  putchar(c);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  skipword(input, &TEMP);
  if (c == 'M')
    wired /= MperG;

  fscanf(input->f, "%lg", &active);
  printf(" %ld", (long)floor(active + 0.5));
  c = getc(input->f);
  if (c == '\n')
    c = ' ';
  putchar(c);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  skipword(input, &TEMP);
  if (c == 'M')
    active /= MperG;

  fscanf(input->f, "%lg", &inactive);
  printf(" %ld", (long)floor(inactive + 0.5));
  c = getc(input->f);
  if (c == '\n')
    c = ' ';
  putchar(c);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  skipword(input, &TEMP);
  if (c == 'M')
    inactive /= MperG;

  fscanf(input->f, "%lg", &used);
  printf(" %ld", (long)floor(used + 0.5));
  c = getc(input->f);
  if (c == '\n')
    c = ' ';
  putchar(c);
  /*





*/
  if (c == 'M')
    used /= MperG;
  TEMP.f = stdout;
  *TEMP.name = '\0';
  skipword(input, &TEMP);

  fscanf(input->f, "%lg", &free);
  printf(" %ld", (long)floor(free + 0.5));
  c = getc(input->f);
  if (c == '\n')
    c = ' ';
  putchar(c);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  skipword(input, &TEMP);
  if (c == 'M')
    free /= MperG;

  sum = wired + active + inactive + free;
  freefraction = free / sum;
  wiredfraction = wired / sum;
  activefraction = active / sum;
  inactivefraction = inactive / sum;

  printf("\n %*s KIND\n", (int)wid, "Gigabytes");
  printf(" %*.*f free     GREEN  \n", (int)wid, (int)dec, free);
  printf(" %*.*f wired    RED\n", (int)wid, (int)dec, wired);
  printf(" %*.*f active   YELLOW\n", (int)wid, (int)dec, active);
  printf(" %*.*f inactive BLUE\n", (int)wid, (int)dec, inactive);
  printf(" %*.*f used     (not part of sum)\n", (int)wid, (int)dec, used);
  printf(" %*s----\n", (int)wid, "------");
  printf(" %*.*f SUM\n", (int)wid, (int)dec, sum);
  printf(" %*.*f free fraction\n\n", (int)wid, (int)dec, freefraction);

  printf(" %*ld%% free percent\n",
	 (int)wid, (long)floor(freefraction * 100 + 0.5));
  printf(" %*ld%% wired percent\n",
	 (int)wid, (long)floor(wiredfraction * 100 + 0.5));
  printf(" %*ld%% active percent\n",
	 (int)wid, (long)floor(activefraction * 100 + 0.5));
  printf(" %*ld%% inactive percent\n",
	 (int)wid, (long)floor(inactivefraction * 100 + 0.5));

  if (*piechartp->name != '\0') {
    if (piechartp->f != NULL)
      piechartp->f = freopen(piechartp->name, "w", piechartp->f);
    else
      piechartp->f = fopen(piechartp->name, "w");
  } else {
    if (piechartp->f != NULL)
      rewind(piechartp->f);
    else
      piechartp->f = tmpfile();
  }
  if (piechartp->f == NULL)
    _EscIO2(FileNotFound, piechartp->name);
  SETUPBUF(piechartp->f, Char);
  fprintf(piechartp->f,
    "1.03  version of piechart that this parameter file is designed for.\n");
  fprintf(piechartp->f, "2.0   diameter of the chart (cm)\n");
  fprintf(piechartp->f, "90    angle to start\n");
  fprintf(piechartp->f, "-1    direction +1 or -1 to draw\n");
  if (*title.name != '\0') {
    if (title.f != NULL)
      title.f = freopen(title.name, "r", title.f);
    else
      title.f = fopen(title.name, "r");
  } else
    rewind(title.f);
  if (title.f == NULL)
    _EscIO2(FileNotFound, title.name);
  RESETBUF(title.f, Char);
  copyaline(&title, piechartp);
  fprintf(piechartp->f, "%ld    font size for the title\n", fontsize);

  /*




*/

  if (*pienumbers->name != '\0') {
    if (pienumbers->f != NULL)
      pienumbers->f = freopen(pienumbers->name, "w", pienumbers->f);
    else
      pienumbers->f = fopen(pienumbers->name, "w");
  } else {
    if (pienumbers->f != NULL)
      rewind(pienumbers->f);
    else
      pienumbers->f = tmpfile();
  }
  if (pienumbers->f == NULL)
    _EscIO2(FileNotFound, pienumbers->name);
  SETUPBUF(pienumbers->f, Char);
  fprintf(pienumbers->f, " %*.*f r 1 0 0 %% wired\n",
	  (int)wid, (int)dec, wired);
  fprintf(pienumbers->f, " %*.*f r 1 1 0 %% active\n",
	  (int)wid, (int)dec, active);
  fprintf(pienumbers->f, " %*.*f r 0 0 1 %% inactive\n",
	  (int)wid, (int)dec, inactive);
  fprintf(pienumbers->f, " %*.*f r 0 1 0 %% free\n", (int)wid, (int)dec, free);

  if (title.f != NULL)
    fclose(title.f);
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  pienumbers.f = NULL;
  strcpy(pienumbers.name, "pienumbers");
  piechartp.f = NULL;
  strcpy(piechartp.name, "piechartp");
  memoryp.f = NULL;
  strcpy(memoryp.name, "memoryp");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  themain(&TEMP, &memoryp, &pienumbers, &piechartp);
_L1:
  if (memoryp.f != NULL)
    fclose(memoryp.f);
  if (piechartp.f != NULL)
    fclose(piechartp.f);
  if (pienumbers.f != NULL)
    fclose(pienumbers.f);
  exit(EXIT_SUCCESS);
}



/* End. */
