/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rv.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.03
/*

*/
#define updateversion   1.00



/*

































*/



#define maxstring       1000



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;



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


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
  clearstring(ribbon);
  ribbon->next = NULL;
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



Static Void reversestring(instring, outstring)
stringDelila instring, *outstring;
{
  long i;

  initializestring(outstring);
  outstring->length = instring.length;
  for (i = 1; i <= instring.length; i++)
    outstring->letters[instring.length - i] = instring.letters[i-1];
}



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  stringDelila line, flippedline;
  boolean gotten;

  while (!BUFEOF(fin->f)) {
    getstring(fin, &line, &gotten);
    if (gotten) {
      reversestring(line, &flippedline);
      writestring(fout, &flippedline);
      putc('\n', fout->f);
    }
  }
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



















/* End. */
