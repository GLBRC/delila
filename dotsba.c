/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dotsba.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.07
/*
*/



/*






































*/



#define maxstring       150




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;


Static _TEXT dots, database;


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



Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}



Static Void getstring(afile, buffer, gotten)
_TEXT *afile;
stringDelila *buffer;
boolean *gotten;
{
  /*
*/
  long index = 0;

  clearstring(buffer);
  if (BUFEOF(afile->f)) {
    *gotten = false;
    return;
  }
  while (!P_eoln(afile->f) && index < maxstring) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
  }

  if (!P_eoln(afile->f)) {
    printf(" getstring: a line exceeds maximum string size (%ld)\n",
	   (long)maxstring);
    halt();
  }

  buffer->length = index;
  buffer->current = 1;
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  *gotten = true;
}


#define mark_           '|'



Static Void themain(dots, database)
_TEXT *dots, *database;
{
  boolean gotten;
  stringDelila wildtype;

  printf("dotsba %4.2f\n", version);


  if (*dots->name != '\0') {
    if (dots->f != NULL)
      dots->f = freopen(dots->name, "r", dots->f);
    else
      dots->f = fopen(dots->name, "r");
  } else
    rewind(dots->f);
  if (dots->f == NULL)
    _EscIO2(FileNotFound, dots->name);
  RESETBUF(dots->f, Char);
  if (*database->name != '\0') {
    if (database->f != NULL)
      database->f = freopen(database->name, "w", database->f);
    else
      database->f = fopen(database->name, "w");
  } else {
    if (database->f != NULL)
      rewind(database->f);
    else
      database->f = tmpfile();
  }
  if (database->f == NULL)
    _EscIO2(FileNotFound, database->name);
  SETUPBUF(database->f, Char);
  copyaline(dots, database);

  if (*dots->name != '\0') {
    if (dots->f != NULL)
      dots->f = freopen(dots->name, "r", dots->f);
    else
      dots->f = fopen(dots->name, "r");
  } else
    rewind(dots->f);
  if (dots->f == NULL)
    _EscIO2(FileNotFound, dots->name);
  RESETBUF(dots->f, Char);
  fscanf(dots->f, "%*[^\n]");
  getc(dots->f);
  getstring(dots, &wildtype, &gotten);

  if (!gotten) {
    printf("could not find the sequence standard!\n");
    halt();
  }

  while (!BUFEOF(dots->f)) {
    wildtype.current = 1;
    while ((!P_eoln(dots->f)) & (P_peek(dots->f) != mark_)) {
      if (P_peek(dots->f) == '.')
	CPUTFBUF(database->f, wildtype.letters[wildtype.current - 1]);
      else
	CPUTFBUF(database->f, P_peek(dots->f));
      if (P_peek(dots->f) != ' ')
	CPUT(database->f);
      getc(dots->f);
      wildtype.current++;
    }
    if (P_peek(dots->f) != mark_) {
      printf("missing mark (%c): it was\"%c\"\n", mark_, P_peek(dots->f));
      halt();
    }
    getc(dots->f);
    copyaline(dots, database);
  }
}

#undef mark_


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  database.f = NULL;
  strcpy(database.name, "database");
  dots.f = NULL;
  strcpy(dots.name, "dots");
  themain(&dots, &database);
_L1:
  if (dots.f != NULL)
    fclose(dots.f);
  if (database.f != NULL)
    fclose(database.f);
  exit(EXIT_SUCCESS);
}



/* End. */
