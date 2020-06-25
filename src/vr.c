/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "vr.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.02
/*



*/



/*

















































*/





#define maxstring       2000




#define fillermax       50




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;




typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;



/*


*/
typedef Char filler[fillermax];







Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}






Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == '\t');
}


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




Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
  ribbon->next = NULL;
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
  clearstring(ribbon);
  ribbon->next = NULL;
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



Static Void capture(field, rabbit, thisprogram)
_TEXT *field;
double *rabbit;
_TEXT *thisprogram;
{
  /*
*/
  while ((!P_eoln(field->f)) & (P_peek(field->f) == ' '))
    getc(field->f);


  if (P_peek(field->f) == '-' || isdigit(P_peek(field->f)))
    fscanf(field->f, "%lg", rabbit);
  else {
    printf(" version must be a real number\n");
    halt();
  }
}



Static Void themain()
{
  trigger t, anof;
  double theversion;
  Char c;
  _TEXT TEMP, TEMP1;

  /*

*/



  filltrigger(&t, "version =                                         ");



  filltrigger(&anof, "of                                                ");

  resettrigger(&t);
  do {
    testfortrigger(P_peek(stdin), &t);
    c = getchar();
    if (c == '\n')
      c = ' ';
  } while (!(P_eof(stdin) || t.found));

  if (P_eof(stdin)) {
    printf("NO-VERSION-FOUND\n");
    halt();
  }

  if (t.found) {
    TEMP.f = stdin;
    *TEMP.name = '\0';
    TEMP1.f = stdin;
    *TEMP1.name = '\0';
    capture(&TEMP, &theversion, &TEMP1);
    printf("%4.2f\n", theversion);
  }

  resettrigger(&anof);
  do {
    testfortrigger(P_peek(stdin), &anof);
    c = getchar();
    if (c == '\n')
      c = ' ';
  } while (!(P_eof(stdin) || anof.found));

  if (P_eof(stdin)) {
    printf("NO-FILE-NAME-FOUND\n");
    halt();
  }

  TEMP.f = stdin;
  *TEMP.name = '\0';
  skipblanks(&TEMP);


  while ((P_peek(stdin) != ' ') & (!P_eoln(stdin))) {
    putchar(P_peek(stdin));
    c = getchar();
    if (c == '\n')
      c = ' ';
  }
  putchar('\n');

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  themain();
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
