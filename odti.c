/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "odti.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.03


/*
*/



/*





























*/


Static _TEXT od, time, odtime;


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



Static long copylines(fin, fout, n)
_TEXT *fin, *fout;
long n;
{
  /*
*/
  long index = 0;

  while (!BUFEOF(fin->f) && index < n) {
    copyaline(fin, fout);
    index++;
  }

  return index;
}


Static Void themain(od, time, odtime)
_TEXT *od, *time, *odtime;
{
  /*
*/
  long indexc, indexr;
  double number;

  printf("odti %4.2f\n", version);
  if (*od->name != '\0') {
    if (od->f != NULL)
      od->f = freopen(od->name, "r", od->f);
    else
      od->f = fopen(od->name, "r");
  } else
    rewind(od->f);
  if (od->f == NULL)
    _EscIO2(FileNotFound, od->name);
  RESETBUF(od->f, Char);
  if (*time->name != '\0') {
    if (time->f != NULL)
      time->f = freopen(time->name, "r", time->f);
    else
      time->f = fopen(time->name, "r");
  } else
    rewind(time->f);
  if (time->f == NULL)
    _EscIO2(FileNotFound, time->name);
  RESETBUF(time->f, Char);
  if (*odtime->name != '\0') {
    if (odtime->f != NULL)
      odtime->f = freopen(odtime->name, "w", odtime->f);
    else
      odtime->f = fopen(odtime->name, "w");
  } else {
    if (odtime->f != NULL)
      rewind(odtime->f);
    else
      odtime->f = tmpfile();
  }
  if (odtime->f == NULL)
    _EscIO2(FileNotFound, odtime->name);
  SETUPBUF(odtime->f, Char);

  if (copylines(od, odtime, 4L) != 4) {
    printf("od file is not long enough\n");
    halt();
  }

  if (copylines(time, odtime, 6L) != 6) {
    printf("time file is not long enough\n");
    halt();
  }

  fscanf(od->f, "%*[^\n]");
  getc(od->f);
  fscanf(time->f, "%*[^\n]");

  getc(time->f);
  fscanf(od->f, "%*[^\n]");
  getc(od->f);
  fscanf(time->f, "%*[^\n]");

  getc(time->f);
  for (indexr = 2; indexr <= 12; indexr++) {
    fscanf(od->f, "%lg", &number);
    fscanf(time->f, "%lg", &number);

    for (indexc = 1; indexc <= 8; indexc++) {
      fscanf(time->f, "%lg", &number);
      fprintf(odtime->f, "%6.3f", number);
      fscanf(od->f, "%lg", &number);
      fprintf(odtime->f, "%6.3f\n", number);
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
  odtime.f = NULL;
  strcpy(odtime.name, "odtime");
  time.f = NULL;
  strcpy(time.name, "time");
  od.f = NULL;
  strcpy(od.name, "od");
  themain(&od, &time, &odtime);
_L1:
  if (od.f != NULL)
    fclose(od.f);
  if (time.f != NULL)
    fclose(time.f);
  if (odtime.f != NULL)
    fclose(odtime.f);
  exit(EXIT_SUCCESS);
}



/* End. */
