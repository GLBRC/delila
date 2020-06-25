/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dashseq.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00
/*

*/
#define updateversion   1.00



/*






























































*/



#define maxstring       2000




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;


Static _TEXT dashedseq, dashseqp, sequ;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}






Static Void emptystring(ribbon)
stringDelila *ribbon;
{
  /*

*/
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}


Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  emptystring(ribbon);
  ribbon->next = NULL;
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*



*/
  printf("remove initializestring routine!\n");
  printf("replace it with clearstring routine!\n");
  halt();
  clearstring(ribbon);
  ribbon->next = NULL;
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




Static Void themain(dashedseq, dashseqp, sequ)
_TEXT *dashedseq, *dashseqp, *sequ;
{
  stringDelila canonical, buffer;
  long i;
  boolean gotten;
  long FORLIM;

  printf("dashseq %4.2f\n", version);
  /*






*/

  if (*dashedseq->name != '\0') {
    if (dashedseq->f != NULL)
      dashedseq->f = freopen(dashedseq->name, "r", dashedseq->f);
    else
      dashedseq->f = fopen(dashedseq->name, "r");
  } else
    rewind(dashedseq->f);
  if (dashedseq->f == NULL)
    _EscIO2(FileNotFound, dashedseq->name);
  RESETBUF(dashedseq->f, Char);
  getstring(dashedseq, &canonical, &gotten);
  if (!gotten) {
    printf("canonical sequence not found\n");
    halt();
  }

  if (*sequ->name != '\0') {
    if (sequ->f != NULL)
      sequ->f = freopen(sequ->name, "w", sequ->f);
    else
      sequ->f = fopen(sequ->name, "w");
  } else {
    if (sequ->f != NULL)
      rewind(sequ->f);
    else
      sequ->f = tmpfile();
  }
  if (sequ->f == NULL)
    _EscIO2(FileNotFound, sequ->name);
  SETUPBUF(sequ->f, Char);
  writestring(sequ, &canonical);
  fprintf(sequ->f, ".\n");
  while (!BUFEOF(dashedseq->f)) {
    getstring(dashedseq, &buffer, &gotten);
    FORLIM = buffer.length;
    for (i = 0; i < FORLIM; i++) {
      if (buffer.letters[i] == '-')
	buffer.letters[i] = canonical.letters[i];
    }

    if (gotten) {
      writestring(sequ, &buffer);
      fprintf(sequ->f, ".\n");
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
  sequ.f = NULL;
  strcpy(sequ.name, "sequ");
  dashseqp.f = NULL;
  strcpy(dashseqp.name, "dashseqp");
  dashedseq.f = NULL;
  strcpy(dashedseq.name, "dashedseq");
  themain(&dashedseq, &dashseqp, &sequ);
_L1:
  if (dashedseq.f != NULL)
    fclose(dashedseq.f);
  if (dashseqp.f != NULL)
    fclose(dashseqp.f);
  if (sequ.f != NULL)
    fclose(sequ.f);
  exit(EXIT_SUCCESS);
}



/* End. */
