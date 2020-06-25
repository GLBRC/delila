/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "flag78.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/



#define version         1.15
/*
*/



/*



































*/



#define maxline         78
#define maxstring       3000


typedef struct stringDelila {
  Char letters[maxstring];
  long length;
} stringDelila;


Static _TEXT fin, fout;
Static long countlines, errors;
Static stringDelila buffer;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Static Void checkin(thefile)
_TEXT *thefile;
{
  if (!BUFEOF(thefile->f))
    return;
  printf("i am frightfully sorry old bean but if you want me to\n");
  printf("function properly you must not feed me empty files.\n\n");
  halt();
}


Static Void readline(thefile, aline)
_TEXT *thefile;
stringDelila *aline;
{
  aline->length = 0;
  while (!P_eoln(thefile->f)) {
    aline->length++;
    if (aline->length > maxstring) {
      printf("a line exceeds %ld characters, increase constant maxstring\n",
	     (long)maxstring);
      halt();
    }
    aline->letters[aline->length - 1] = getc(thefile->f);
    if (aline->letters[aline->length - 1] == '\n')
      aline->letters[aline->length - 1] = ' ';
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
}


Static Void untail(astring)
stringDelila *astring;
{
  if (astring->length > 1) {
    while (astring->letters[astring->length - 1] == ' ' && astring->length > 1)
      astring->length--;
  }
}


Static Void writeline(thefile, aline)
_TEXT *thefile;
stringDelila *aline;
{
  long count = 1;

  while (count <= aline->length) {
    putc(aline->letters[count-1], thefile->f);
    count++;
  }
  putc('\n', thefile->f);
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
  printf(" flag78 %4.2f\n", version);
  if (*fin.name != '\0') {
    if (fin.f != NULL)
      fin.f = freopen(fin.name, "r", fin.f);
    else
      fin.f = fopen(fin.name, "r");
  } else
    rewind(fin.f);
  if (fin.f == NULL)
    _EscIO2(FileNotFound, fin.name);
  RESETBUF(fin.f, Char);
  if (*fout.name != '\0') {
    if (fout.f != NULL)
      fout.f = freopen(fout.name, "w", fout.f);
    else
      fout.f = fopen(fout.name, "w");
  } else {
    if (fout.f != NULL)
      rewind(fout.f);
    else
      fout.f = tmpfile();
  }
  if (fout.f == NULL)
    _EscIO2(FileNotFound, fout.name);
  SETUPBUF(fout.f, Char);
  fprintf(fout.f, " flag78 %4.2f", version);
  fprintf(fout.f, "  lines that exceed %ld characters\n\n", (long)maxline);

  checkin(&fin);

  readline(&fin, &buffer);
  writeline(&fout, &buffer);
  /*

*/

  if (*fin.name != '\0') {
    if (fin.f != NULL)
      fin.f = freopen(fin.name, "r", fin.f);
    else
      fin.f = fopen(fin.name, "r");
  } else
    rewind(fin.f);
  if (fin.f == NULL)
    _EscIO2(FileNotFound, fin.name);
  RESETBUF(fin.f, Char);
  countlines = 0;
  errors = 0;

  while (!BUFEOF(fin.f)) {
    readline(&fin, &buffer);
    countlines++;
    untail(&buffer);
    if (buffer.length > maxline) {
      errors++;
      fprintf(fout.f, "\nline number %ld\n", countlines);
      writeline(&fout, &buffer);
      fprintf(fout.f, "%*c^\n", (int)(maxline - 1), ' ');
    }
  }

  printf(" %ld lines are over %ld characters long\n", errors, (long)maxline);
  if (errors > 0)
    printf("\n this file may be difficult to transport\n\n");
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
