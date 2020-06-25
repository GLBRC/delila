/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "todobfcite.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.06
/*







*/
#define updateversion   1.00



/*





















































*/



#define maxstring       2000



#define fillermax       50




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;



/*


*/
typedef Char filler[fillermax];



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;


Static _TEXT todobfcitep, todobfcitelist;


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
  /*



*/
  emptystring(ribbon);
  ribbon->previous = NULL;
  ribbon->next = NULL;
}


Static Void disposestring(ribbon)
stringDelila **ribbon;
{
  stringDelila *t;

  t = *ribbon;

  while (t->next != NULL)
    t = t->next;

  while (t->previous != NULL) {
    t = t->previous;
    Free(t->next);
  }
  Free(t);
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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}



Static Void fillstring(s, a)
stringDelila *s;
Char *a;
{
  /*
*/


  /*


*/
  long length = fillermax;
  long index;

  clearstring(s);
  while (length > 1 && a[length-1] == ' ')
    length--;
  if (length == 1 && a[length-1] == ' ') {
    printf("fillstring: the string is empty\n");
    halt();
  }

  for (index = 0; index < length; index++)
    s->letters[index] = a[index];
  s->length = length;
  s->current = 1;
}



Static Void filltrigger(t, a)
trigger *t;
Char *a;
{
  fillstring(&t->seek, a);
}



/*




*/

Static Void resettrigger(t)
trigger *t;
{
  t->state = 0;
  t->skip = false;
  t->found = false;
}


Static Void testfortrigger(ch, t)
Char ch;
trigger *t;
{
  /*









*/
  t->state++;
  /*



*/
  if (t->seek.letters[t->state - 1] == ch) {
    t->skip = false;
    if (t->state == t->seek.length)
      t->found = true;
    else
      t->found = false;
    return;
  }
  /*
*/
  if (t->seek.letters[0] == ch) {
    t->state = 1;
    t->skip = false;
    t->found = false;
    return;
  }
  t->state = 0;
  t->skip = true;
  t->found = false;
}


#define tab             9






Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == tab);
}

#undef tab


Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while (isblankDelila(P_peek(thefile->f)) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((!isblankDelila(P_peek(thefile->f))) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}






Static Void themain(input, todobfcitep, todobfcitelist)
_TEXT *input, *todobfcitep, *todobfcitelist;
{
  /*

*/
  Char c;
  trigger todobf, cite;
  boolean intodobf = false, incite = false;
  long ptodobf = 0, pcite = 0;
  boolean doverbose = true, justbroke = false;

  /*







*/

/* p2c: todobfcite.p: Note: Eliminated unused assignment statement [338] */




  filltrigger(&todobf, "\\todobf                                           ");
  filltrigger(&cite, "\\cite                                             ");
  resettrigger(&todobf);
  resettrigger(&cite);
  if (*todobfcitelist->name != '\0') {
    if (todobfcitelist->f != NULL)
      todobfcitelist->f = freopen(todobfcitelist->name, "w", todobfcitelist->f);
    else
      todobfcitelist->f = fopen(todobfcitelist->name, "w");
  } else {
    if (todobfcitelist->f != NULL) {

      rewind(todobfcitelist->f);
    } else
      todobfcitelist->f = tmpfile();
  }
  if (todobfcitelist->f == NULL)
    _EscIO2(FileNotFound, todobfcitelist->name);
  SETUPBUF(todobfcitelist->f, Char);
  while (!BUFEOF(input->f)) {
    if (P_eoln(input->f)) {
      fscanf(input->f, "%*[^\n]");
      getc(input->f);
      putchar('\n');
      continue;
    }
    c = getc(input->f);
    if (c == '\n')
      c = ' ';
    putchar(c);

    testfortrigger(c, &todobf);
    if (todobf.found) {
      intodobf = true;
      ptodobf = 0;
    } else {
      if (intodobf && c == '{')
	ptodobf++;
    }
    if (intodobf && c == '}')
      ptodobf--;
    if (!todobf.found && intodobf && ptodobf == 0) {
      intodobf = false;
      if (doverbose)
	printf("[*** END OF todobf ***]");
    }


    if (intodobf) {
      testfortrigger(c, &cite);
      if (cite.found) {
	incite = true;
	pcite = 0;
      } else {
	if (incite && c == '{')
	  pcite++;
      }

      if (incite && c == '}') {
	pcite--;
	/*

*/
      }


      if (!cite.found && incite && pcite == 0) {
	incite = false;
	putc('\n', todobfcitelist->f);
	justbroke = true;
	if (doverbose)
	  printf("[*** END OF cite ***]");
      }

    }

    if (!(intodobf && incite && pcite == 1 && c != '{'))
      continue;

    if (c != ',' && c != ' ') {
      putc(c, todobfcitelist->f);
      justbroke = false;
    }


    /*


*/

    if (c == ',' || c == ' ') {
      if (!justbroke)
	putc('\n', todobfcitelist->f);
      justbroke = true;
    }

  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  todobfcitelist.f = NULL;
  strcpy(todobfcitelist.name, "todobfcitelist");
  todobfcitep.f = NULL;
  strcpy(todobfcitep.name, "todobfcitep");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  themain(&TEMP, &todobfcitep, &todobfcitelist);
_L1:
  if (todobfcitep.f != NULL)
    fclose(todobfcitep.f);
  if (todobfcitelist.f != NULL)
    fclose(todobfcitelist.f);
  exit(EXIT_SUCCESS);
}



/* End. */
