/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "unverb.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.19
/*
*/



/*
























*/



#define maxstring       150



#define fillermax       20




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






Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  Char ch;
  boolean copying = true;
  trigger bv, ev;


  filltrigger(&bv, "\\begin{verbatim}    ");
  filltrigger(&ev, "\\end{verbatim}      ");
  resettrigger(&bv);
  while (!BUFEOF(fin->f)) {
    while (!P_eoln(fin->f)) {
      ch = getc(fin->f);
      if (ch == '\n')
	ch = ' ';
      testfortrigger(ch, &bv);

      if (copying) {
	putc(ch, fout->f);
	if (bv.found)
	  copying = false;
	resettrigger(&ev);
      } else {
	testfortrigger(ch, &ev);
	if (ev.found)
	  copying = true;
      }
    }
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
    if (copying)
      putc('\n', fout->f);
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
