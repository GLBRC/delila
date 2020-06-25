/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "fl.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/



#define version         1.14
/*

*/



/*









































*/



#define maxline         80
#define maxstring       3000


typedef struct stringDelila {
  Char letters[maxstring];
  long length;
} stringDelila;


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
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  printf(" flag %4.2f\n", version);
  printf("  lines that exceed %ld characters\n\n", (long)maxline);
  TEMP.f = stdin;
  *TEMP.name = '\0';

  checkin(&TEMP);

  TEMP.f = stdin;
  *TEMP.name = '\0';
  readline(&TEMP, &buffer);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeline(&TEMP, &buffer);
  /*

*/

  rewind(stdin);
  countlines = 0;
  errors = 0;

  while (!P_eof(stdin)) {
    TEMP.f = stdin;
    *TEMP.name = '\0';
    readline(&TEMP, &buffer);
    countlines++;
    untail(&buffer);
    if (buffer.length > maxline) {
      errors++;
      printf("\nline number %ld\n", countlines);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeline(&TEMP, &buffer);
      printf("%*c^\n", (int)(maxline - 1), ' ');
    }
  }

  printf(" %ld lines are over %ld characters long\n", errors, (long)maxline);
  if (errors > 0)
    printf("\n this file may be difficult to transport\n\n");
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
