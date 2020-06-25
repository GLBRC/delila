/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "ver.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         2.06
/*



*/



/*




























*/


#define debugging       false


#define maxstring       1000



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


Static trigger t;
Static stringDelila s;
Static long index_;
Static boolean gotten;


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


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  printf(" ver %4.2f\n", version);

  if (!P_eof(stdin)) {
    filltrigger(&t, "version =                                         ");

    resettrigger(&t);

    do {
      TEMP.f = stdin;
      *TEMP.name = '\0';
      getstring(&TEMP, &s, &gotten);
      if (gotten) {
	/*


*/
	index_ = 1;
	while (index_ < s.length && !t.found) {
	  testfortrigger(s.letters[index_-1], &t);
	  /*

*/
	  index_++;
	}
      }
    } while (gotten && !t.found);

    if (t.found) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &s);
      putchar('\n');
    } else
      printf("no \"version =\" string\n");

    /*
*/
    while (!P_eof(stdin)) {
      scanf("%*[^\n]");
      getchar();
    }
  }


  else
    printf(" input is empty\n");
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
