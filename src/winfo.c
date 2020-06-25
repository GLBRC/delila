/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "winfo.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.08
/*
*/



/*
































*/



#define maxwin          1000


Static _TEXT data, winfop, xyin;


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



Static Void themain(data, winfop, xyin)
_TEXT *data, *winfop, *xyin;
{
  long a, c, g, t, currentR;
  long howmany = 0;
  long l;
  long oldestR = 0;
  double R;
  double rvalues[maxwin];
  double total = 0.0;
  long windowsize;

  printf("winfo %4.2f\n", version);

  if (*winfop->name != '\0') {
    if (winfop->f != NULL)
      winfop->f = freopen(winfop->name, "r", winfop->f);
    else
      winfop->f = fopen(winfop->name, "r");
  } else
    rewind(winfop->f);
  if (winfop->f == NULL)
    _EscIO2(FileNotFound, winfop->name);
  RESETBUF(winfop->f, Char);
  if (BUFEOF(winfop->f)) {
    printf("empty parameter file winfop\n");
    halt();
  }
  fscanf(winfop->f, "%ld%*[^\n]", &windowsize);
  getc(winfop->f);
  if (windowsize > maxwin) {
    printf("windowsize (%ld) is bigger than maxwin (%ld, increase constant maxwin.\n",
	   windowsize, (long)maxwin);
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
  fprintf(xyin->f, "* winfo %4.2f\n", version);

  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "r", data->f);
    else
      data->f = fopen(data->name, "r");
  } else
    rewind(data->f);
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  RESETBUF(data->f, Char);
  while (P_peek(data->f) == '*')
    copyaline(data, xyin);

  while (!BUFEOF(data->f) && howmany < windowsize) {
    fscanf(data->f, "%ld%ld%ld%ld%ld%lg%*[^\n]", &l, &a, &c, &g, &t, &R);
    getc(data->f);

    howmany++;
    rvalues[howmany-1] = R;
    total += R;
  }

  currentR = windowsize;
  fprintf(xyin->f, "* window: %ld\n", windowsize);
  fprintf(xyin->f, "* symbol | l | average R in window | R | current total\n");
  while (!BUFEOF(data->f)) {
    if (P_peek(data->f) == '*') {
      fscanf(data->f, "%*[^\n]");
      getc(data->f);
      continue;
    }
    if (l % 10 == 0)
      fprintf(xyin->f, "c%5ld %10.5f %10.5f %10.5f\n",
	      l, total / windowsize, R, total);

    fprintf(xyin->f, "d%5ld %10.5f %10.5f %10.5f\n",
	    l, total / windowsize, R, total);

    oldestR++;
    if (oldestR > windowsize)
      oldestR = 1;

    total -= rvalues[oldestR-1];

    fscanf(data->f, "%ld%ld%ld%ld%ld%lg%*[^\n]", &l, &a, &c, &g, &t, &R);
    getc(data->f);

    currentR++;
    if (currentR > windowsize)
      currentR = 1;

    rvalues[currentR-1] = R;
    total += R;
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  winfop.f = NULL;
  strcpy(winfop.name, "winfop");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &winfop, &xyin);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (winfop.f != NULL)
    fclose(winfop.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}
























/* End. */
