/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "sqz.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.14
/*
*/



/*

































*/



#define defaultendofline  '#'

#define breakpoint      80


Static _TEXT fin, fout, sqzp;


Static jmp_buf _JL1;


/*
*/


Static Void halt()
{
  printf("program halt\n");
  longjmp(_JL1, 1);
}



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  long count;
  /*
*/
  Char endofline;

  printf("sqz %4.2f\n", version);
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
  if (*sqzp.name != '\0') {
    if (sqzp.f != NULL)
      sqzp.f = freopen(sqzp.name, "r", sqzp.f);
    else
      sqzp.f = fopen(sqzp.name, "r");
  } else
    rewind(sqzp.f);
  if (sqzp.f == NULL)
    _EscIO2(FileNotFound, sqzp.name);
  RESETBUF(sqzp.f, Char);
  if (!BUFEOF(sqzp.f)) {
    endofline = getc(sqzp.f);
    if (endofline == '\n')
      endofline = ' ';
  } else
    endofline = defaultendofline;
  putc(endofline, fout->f);
  fprintf(fout->f, " squeezed file from sqz %4.2f\n", version);

  while (!BUFEOF(fin->f)) {
    count = 0;
    while (!P_eoln(fin->f)) {
      if (P_peek(fin->f) == endofline) {
	printf("the character %c must not appear in the file\n", endofline);
	printf("the fout file is being destroyed.\n");
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
	halt();
      }
      if (count == breakpoint - 1) {
	fprintf(fout->f, "%c\n", endofline);
	count = 0;
      }
      putc(P_peek(fin->f), fout->f);
      getc(fin->f);
      count++;
    }
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
    putc('\n', fout->f);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  sqzp.f = NULL;
  strcpy(sqzp.name, "sqzp");
  fout.f = NULL;
  strcpy(fout.name, "fout");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &fout);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  if (sqzp.f != NULL)
    fclose(sqzp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
