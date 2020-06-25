/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "cap.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.09
/*


*/



/*
























*/



#define single          '\''
#define double_         '"'


Static _TEXT fin, fout;


Static jmp_buf _JL1;



Static Void halt()
{
  longjmp(_JL1, 1);
}



Static Void docomment(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  Char ch;
  boolean done = false, endcomment = false;

  while (!BUFEOF(fin->f) && !done) {
    if (P_eoln(fin->f)) {
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      putc('\n', fout->f);
      endcomment = false;
      continue;
    }
    ch = getc(fin->f);
    if (ch == '\n')
      ch = ' ';
    putc(ch, fout->f);

    if (ch == ')' && endcomment)
      done = true;
    else
      endcomment = (ch == '*');
  }
}



Static Void doquote(fin, fout, endquote, diff)
_TEXT *fin, *fout;
Char endquote;
long diff;
{
  /*

*/
  Char ch = ' ';

  while (!BUFEOF(fin->f) && ch != endquote) {
    if (P_eoln(fin->f)) {
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      putc('\n', fout->f);
      continue;
    }
    ch = getc(fin->f);
    if (ch == '\n')
      ch = ' ';
    if (islower(ch))
      putc((Char)(ch - diff), fout->f);
    else
      putc(ch, fout->f);
  }
}



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  Char ch;
  long diff = 'a' - 'A';
  /*


*/
  boolean startcomment = false;

  printf(" cap %4.2f\n", version);

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

  while (!BUFEOF(fin->f)) {
    if (P_eoln(fin->f)) {
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      putc('\n', fout->f);
      startcomment = false;
      continue;
    }
    ch = getc(fin->f);
    if (ch == '\n')
      ch = ' ';
    putc(ch, fout->f);

    if (ch == '*' && startcomment) {
      docomment(fin, fout);
      continue;
    }
/* p2c: cap.p: Note: Eliminated unused assignment statement [338] */
    if (ch == '(')
      startcomment = true;
    else {
      startcomment = false;
      if (ch == single || ch == double_)
	doquote(fin, fout, ch, diff);
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
  exit(EXIT_SUCCESS);
}



/* End. */
