/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "versave.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.12
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



Static _TEXT source;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf("echo \"program halt.\"\n");
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
  t->state = 0;
  t->skip = true;
  t->found = false;
}



Static Void capture(field, rabbit)
_TEXT *field;
double *rabbit;
{
  /*
*/
  while ((!P_eoln(field->f)) &
	 ((P_peek(field->f) == ' ') | (P_peek(field->f) == '\'')))
    getc(field->f);


  if (P_peek(field->f) == '-' || isdigit(P_peek(field->f)))
    fscanf(field->f, "%lg", rabbit);
  else {
    printf("echo \"version must be a real number\"\n");
    halt();
  }
}



Static Void themain(source)
_TEXT *source;
{
  trigger t;
  stringDelila n;
  double theversion;
  _TEXT TEMP;

  /*



*/

  if (*source->name != '\0') {
    if (source->f != NULL)
      source->f = freopen(source->name, "r", source->f);
    else
      source->f = fopen(source->name, "r");
  } else
    rewind(source->f);
  if (source->f == NULL)
    _EscIO2(FileNotFound, source->name);
  RESETBUF(source->f, Char);



  filltrigger(&t, "version =                                         ");

  resettrigger(&t);

  do {
    testfortrigger(P_peek(source->f), &t);
    getc(source->f);
  } while (!(BUFEOF(source->f) || t.found));

  if (t.found) {
    capture(source, &theversion);
    /*

*/
  } else {
    printf("echo \"no 'version =' string\"\n");
    halt();
  }




  filltrigger(&t, "of                                                ");

  resettrigger(&t);

  do {
    testfortrigger(P_peek(source->f), &t);
    getc(source->f);
  } while (!(BUFEOF(source->f) || t.found));

  if (P_peek(source->f) == ' ')
    getc(source->f);
  else {
    printf("echo \" 'of' not followed by a space\"\n");
    halt();
  }

  if (t.found) {
    clearstring(&n);
    while ((!P_eoln(source->f)) & (P_peek(source->f) != ' ')) {
      n.length++;
      n.letters[n.length - 1] = getc(source->f);
    }
    /*



*/
  }

  else {
    printf("echo \"no 'of' string\"\n");
    halt();
  }

  printf("file\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &n);
  printf("\nversion\n");

  printf("%4.2f\n", theversion);

}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  source.f = NULL;
  *source.name = '\0';
  TEMP.f = stdin;
  *TEMP.name = '\0';
  themain(&TEMP);
_L1:
  if (source.f != NULL)
    fclose(source.f);
  exit(EXIT_SUCCESS);
}



/* End. */
