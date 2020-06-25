/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "vernam.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00
/*

*/



/*



























*/




#define maxstring       150



#define fillermax       50



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
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
  t->state = 0;
  t->skip = true;
  t->found = false;
}


#define debugging       false



Static Void themain(source)
_TEXT *source;
{
  Char s;
  trigger t;
  double theversion;

  /*
*/



  filltrigger(&t, "version =                                         ");
  resettrigger(&t);

  do {
    testfortrigger(P_peek(source->f), &t);
    getc(source->f);
  } while (!(BUFEOF(source->f) || t.found));

  if (!t.found)
    return;

  fscanf(source->f, "%lg", &theversion);

  if (debugging)
    printf("%4.2f\n", theversion);

  while (P_peek(source->f) != 'o') {
    s = getc(source->f);
    if (s == '\n')
      s = ' ';
    if (debugging)
      printf("BUBBA 1 \"%c\"\n", s);
  }
  while (P_peek(source->f) != 'f') {
    s = getc(source->f);
    if (s == '\n')
      s = ' ';
    if (debugging)
      printf("BUBBA 2 \"%c\"\n", s);
  }
  s = getc(source->f);
  if (s == '\n')
    s = ' ';
  s = getc(source->f);
  if (s == '\n')
    s = ' ';
  if (debugging)
    printf("BUBBA 3 \"%c\"\n", s);
  while (P_peek(source->f) != ' ') {
    s = getc(source->f);
    if (s == '\n')
      s = ' ';
    if (debugging)
      printf("BUBBA 4 \"%c\"\n", s);
    putchar(s);
  }
  putchar('\n');
}

#undef debugging


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  TEMP.f = stdin;
  *TEMP.name = '\0';
  themain(&TEMP);
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
