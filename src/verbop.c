/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "verbop.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         2.10
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



Static Void capture(field, rabbit, thisprogram)
_TEXT *field;
double *rabbit;
_TEXT *thisprogram;
{
  /*
*/
  while ((!P_eoln(field->f)) & (P_peek(field->f) == ' '))
    getc(field->f);


  if (isdigit(P_peek(field->f))) {
    fscanf(field->f, "%lg", rabbit);
    return;
  }
  printf(" version must be a real number\n");
  if (*thisprogram->name != '\0') {
    if (thisprogram->f != NULL)
      thisprogram->f = freopen(thisprogram->name, "w", thisprogram->f);
    else
      thisprogram->f = fopen(thisprogram->name, "w");
  } else {
    if (thisprogram->f != NULL)
      rewind(thisprogram->f);
    else
      thisprogram->f = tmpfile();
  }
  if (thisprogram->f == NULL)
    _EscIO2(FileNotFound, thisprogram->name);
  SETUPBUF(thisprogram->f, Char);
  halt();
}



Static Void themain(source)
_TEXT *source;
{
  _TEXT internal;
  trigger t;
  double theversion;

  internal.f = NULL;
  *internal.name = '\0';
  printf(" verbop %4.2f\n", version);


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
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  while (!BUFEOF(source->f))
    copyaline(source, &internal);

  /*
*/
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  if (*source->name != '\0') {
    if (source->f != NULL)
      source->f = freopen(source->name, "w", source->f);
    else
      source->f = fopen(source->name, "w");
  } else {
    if (source->f != NULL)
      rewind(source->f);
    else
      source->f = tmpfile();
  }
  if (source->f == NULL)
    _EscIO2(FileNotFound, source->name);
  SETUPBUF(source->f, Char);



  filltrigger(&t, "version =                                         ");

  resettrigger(&t);

  do {
    testfortrigger(P_peek(internal.f), &t);


    if (P_eoln(internal.f))
      putc('\n', source->f);
    else
      putc(P_peek(internal.f), source->f);

    getc(internal.f);
  } while (!(BUFEOF(internal.f) || t.found));

  if (t.found) {
    capture(&internal, &theversion, source);
    theversion += 0.01;
    fprintf(source->f, " %4.2f", theversion);
    printf(" the new version is %4.2f\n", theversion);
  } else
    printf("no \"version =\" string\n");


  while (!BUFEOF(internal.f))
    copyaline(&internal, source);
  if (internal.f != NULL)
    fclose(internal.f);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  source.f = NULL;
  strcpy(source.name, "source");
  themain(&source);
_L1:
  if (source.f != NULL)
    fclose(source.f);
  exit(EXIT_SUCCESS);
}



/* End. */
