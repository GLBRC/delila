/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "evenco.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.12


Static jmp_buf _JL1;


/*


*/



/*






































*/



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



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{

  /*



*/
  long count = 0;
  long decimals;
  _TEXT internal;
  /*
*/
  long lines, i;
  long maxi = 0;

  *internal.name = '\0';
  if (*internal.name != '\0')
    internal.f = fopen(internal.name, "w");
  else
    internal.f = tmpfile();
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  while (!BUFEOF(fin->f)) {
    count++;
    fscanf(fin->f, "%ld%*[^\n]", &i);
    getc(fin->f);
    fprintf(internal.f, "%ld\n", i);
    if (maxi < i)
      maxi = i;
  }
  lines = count;

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
  if (lines > 0) {
    decimals = (long)(log((double)maxi) / log(10.0)) + 1;
    if (*internal.name != '\0')
      internal.f = freopen(internal.name, "r", internal.f);
    else
      rewind(internal.f);
    if (internal.f == NULL)
      _EscIO2(FileNotFound, internal.name);
    RESETBUF(internal.f, Char);
    for (count = 1; count <= lines; count++) {
      fscanf(internal.f, "%ld%*[^\n]", &i);
      getc(internal.f);
      fprintf(fout->f, "%*ld\n", (int)decimals, i);
    }
  }


  fclose(internal.f);
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
_L1:
  exit(EXIT_SUCCESS);
}

/*



















*/



/* End. */
