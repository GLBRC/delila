/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "worcharev.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00


/*
*/



/*
































*/


Static _TEXT reversed, worchap;


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



Static Void themain(worchap, reversed)
_TEXT *worchap, *reversed;
{
  _TEXT f;

  f.f = NULL;
  *f.name = '\0';
  printf("worcharev %4.2f\n", version);

  if (*worchap->name != '\0') {
    if (worchap->f != NULL)
      worchap->f = freopen(worchap->name, "r", worchap->f);
    else
      worchap->f = fopen(worchap->name, "r");
  } else
    rewind(worchap->f);
  if (worchap->f == NULL)
    _EscIO2(FileNotFound, worchap->name);
  RESETBUF(worchap->f, Char);
  if (*reversed->name != '\0') {
    if (reversed->f != NULL)
      reversed->f = freopen(reversed->name, "w", reversed->f);
    else
      reversed->f = fopen(reversed->name, "w");
  } else {
    if (reversed->f != NULL)
      rewind(reversed->f);
    else
      reversed->f = tmpfile();
  }
  if (reversed->f == NULL)
    _EscIO2(FileNotFound, reversed->name);
  SETUPBUF(reversed->f, Char);

  while (!BUFEOF(worchap->f)) {
    if (*f.name != '\0') {
      if (f.f != NULL)
	f.f = freopen(f.name, "w", f.f);
      else
	f.f = fopen(f.name, "w");
    } else {
      if (f.f != NULL)
	rewind(f.f);
      else
	f.f = tmpfile();
    }
    if (f.f == NULL)
      _EscIO2(FileNotFound, f.name);
    SETUPBUF(f.f, Char);
    copyaline(worchap, &f);

    if (BUFEOF(worchap->f)) {
      printf("error in worchap: there must be an even number of lines.\n");
      halt();
    }

    copyaline(worchap, reversed);
    if (*f.name != '\0') {
      if (f.f != NULL)
	f.f = freopen(f.name, "r", f.f);
      else
	f.f = fopen(f.name, "r");
    } else
      rewind(f.f);
    if (f.f == NULL)
      _EscIO2(FileNotFound, f.name);
    RESETBUF(f.f, Char);
    copyaline(&f, reversed);
  }

  if (f.f != NULL)
    fclose(f.f);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  worchap.f = NULL;
  strcpy(worchap.name, "worchap");
  reversed.f = NULL;
  strcpy(reversed.name, "reversed");
  themain(&worchap, &reversed);
_L1:
  if (reversed.f != NULL)
    fclose(reversed.f);
  if (worchap.f != NULL)
    fclose(worchap.f);
  exit(EXIT_SUCCESS);
}



/* End. */
