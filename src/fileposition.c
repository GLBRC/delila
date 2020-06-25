/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "fileposition.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.04
/*
*/



/*
































*/



#define maxstring       150


typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;





Static _TEXT fin;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}



Static Void flagstring(afile, buffer)
_TEXT *afile;
stringDelila *buffer;
{
  buffer->length = buffer->current;
  writestring(afile, buffer);
  fprintf(afile->f, "? ");
  clearstring(buffer);
}



Static Void themain(fin)
_TEXT *fin;
{
  stringDelila buffer;
  boolean done = false;
  boolean gotten;
  long i = 0;
  long j, n, prebuffer;
  _TEXT TEMP;
  long FORLIM;

  printf("fileposition %4.2f\n", version);

  printf("What file position do you want to see the line for? \n");
  scanf("%ld%*[^\n]", &n);
  getchar();
  printf("position: %ld\n", n);

  printf("How many characters do you want to see prior to the\n");
  printf("line at this position (\"prebuffer\")? \n");
  scanf("%ld%*[^\n]", &prebuffer);
  getchar();
  printf("prebuffer: %ld\n", prebuffer);
  printf("------------------\n");

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
  while (!done) {
    getstring(fin, &buffer, &gotten);
    if (!gotten) {
      done = true;
      break;
    }
    i += buffer.length + 1;

    /*


*/


    if (i >= n - prebuffer) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &buffer);
      putchar('\n');
    }


    if (i < n)
      continue;
    printf("------------------\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &buffer);
    putchar('\n');

    FORLIM = buffer.length - i + n;
    for (j = 1; j <= FORLIM; j++)
      putchar(' ');
    printf("^%ld\n", n);
    done = true;

  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
