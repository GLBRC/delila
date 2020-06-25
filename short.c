/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "short.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/



#define version         1.01


/*
*/



/*





























*/



Static _TEXT fin, fout, shortp;


Static jmp_buf _JL1;



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(fin, fout, shortp)
_TEXT *fin, *fout, *shortp;
{
  long count, detection;
  long line = 0;

  printf(" short %4.2f\n", version);
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
  fprintf(fout->f, " short %4.2f\n", version);
  if (*shortp->name != '\0') {
    if (shortp->f != NULL)
      shortp->f = freopen(shortp->name, "r", shortp->f);
    else
      shortp->f = fopen(shortp->name, "r");
  } else
    rewind(shortp->f);
  if (shortp->f == NULL)
    _EscIO2(FileNotFound, shortp->name);
  RESETBUF(shortp->f, Char);

  if (BUFEOF(shortp->f)) {
    printf(" shortp is empty\n");
    halt();
  } else {
    fscanf(shortp->f, "%ld%*[^\n]", &detection);
    getc(shortp->f);

  }
  while (!BUFEOF(fin->f)) {
    count = 0;
    while (!P_eoln(fin->f)) {
      getc(fin->f);
      count++;
    }
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
    line++;
    if (count <= detection)
      fprintf(fout->f, " at %ld count is %ld\n", line, count);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  shortp.f = NULL;
  strcpy(shortp.name, "shortp");
  fout.f = NULL;
  strcpy(fout.name, "fout");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &fout, &shortp);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  if (shortp.f != NULL)
    fclose(shortp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
