/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "asciicode.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01


/*
*/



/*
















































*/


Static _TEXT ascii, code;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void word(ascii, code, count)
_TEXT *ascii, *code;
long *count;
{
  long n;
  Char x, a, b, c;

  fscanf(ascii->f, "%c%ld%c%c%c%c", &x, &n, &x, &a, &b, &c);
  if (x == '\n')
    x = ' ';
  if (x == '\n')
    x = ' ';
  if (a == '\n')
    a = ' ';
  if (b == '\n')
    b = ' ';
  if (c == '\n')
    c = ' ';
  if (*count != n)
    printf("%12ld<>%12ld\n", *count, n);
  fprintf(code->f, "      %3ld: write(f,'%c", n, a);
  if (b == '\'')
    fprintf(code->f, "''");
  else
    putc(b, code->f);
  fprintf(code->f, "%c');\n", c);
  (*count)++;
}



Static Void line(ascii, code, count)
_TEXT *ascii, *code;
long *count;
{
  long x;

  for (x = 1; x <= 8; x++)
    word(ascii, code, count);
  fscanf(ascii->f, "%*[^\n]");
  getc(ascii->f);
}



Static Void themain(ascii, code)
_TEXT *ascii, *code;
{
  long count = 0;
  long y;

  printf("asciicode %4.2f\n", version);

  if (*ascii->name != '\0') {
    if (ascii->f != NULL)
      ascii->f = freopen(ascii->name, "r", ascii->f);
    else
      ascii->f = fopen(ascii->name, "r");
  } else
    rewind(ascii->f);
  if (ascii->f == NULL)
    _EscIO2(FileNotFound, ascii->name);
  RESETBUF(ascii->f, Char);
  if (*code->name != '\0') {
    if (code->f != NULL)
      code->f = freopen(code->name, "w", code->f);
    else
      code->f = fopen(code->name, "w");
  } else {
    if (code->f != NULL)
      rewind(code->f);
    else
      code->f = tmpfile();
  }
  if (code->f == NULL)
    _EscIO2(FileNotFound, code->name);
  SETUPBUF(code->f, Char);
  for (y = 1; y <= 16; y++)
    line(ascii, code, &count);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  code.f = NULL;
  strcpy(code.name, "code");
  ascii.f = NULL;
  strcpy(ascii.name, "ascii");
  themain(&ascii, &code);
_L1:
  if (ascii.f != NULL)
    fclose(ascii.f);
  if (code.f != NULL)
    fclose(code.f);
  exit(EXIT_SUCCESS);
}



/* End. */
