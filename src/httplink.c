/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "httplink.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.05
/*
*/
#define updateversion   1.00



/*


































*/



#define maxstring       1000



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


/*


*/


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



Static Void themain()
{
  stringDelila aline;
  Char c;
  boolean debugging = true;
  boolean done, gotten;
  trigger http;
  long iw, ir, j, start, stop;
  _TEXT TEMP;

  /*

*/
  printf("<!-- httplink %4.2f -->\n", version);
  /*






*/



  filltrigger(&http, "http://                                           ");

  debugging = false;

  while (!P_eof(stdin)) {
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getstring(&TEMP, &aline, &gotten);
    resettrigger(&http);
    if (!gotten)
      continue;
    ir = 1;
    iw = 1;
    while (ir <= aline.length) {
      c = aline.letters[ir-1];
      testfortrigger(c, &http);
      if (!http.found) {
	/*

*/
	ir++;
	continue;
      }
      start = ir - http.seek.length + 1;
      if (debugging) {
	printf("!\n");
	printf("aline.length = %ld\n", aline.length);
	printf("http.seek.length = %ld\n", http.seek.length);
	printf("start = %ld\n", start);
      }

      done = false;
      while (!done) {
	if (aline.letters[ir-1] == ' ' || aline.letters[ir-1] == ',' ||
	    aline.letters[ir-1] == ')' || aline.letters[ir-1] == ']' ||
	    aline.letters[ir-1] == ':') {
	  done = true;
	  stop = ir - 1;
	  continue;
	}
	if (ir == aline.length) {
	  stop = ir;
	  ir++;
	  done = true;
	} else
	  ir++;
      }
      if (debugging) {
	printf("stop = %ld\n", stop);
	printf("captured: \"");
	for (j = start - 1; j < stop; j++)
	  putchar(aline.letters[j]);
	printf("\"\n");
      }

      /*

*/
      while (iw < start) {
	putchar(aline.letters[iw-1]);
	iw++;
      }
      /*

*/


      printf("<a href = \"");
      for (j = start - 1; j < stop; j++)
	putchar(aline.letters[j]);
      printf("\">");
      for (j = start - 1; j < stop; j++)
	putchar(aline.letters[j]);
      printf("</a>");
      iw = stop + 1;
    }

    while (iw <= aline.length) {
      putchar(aline.letters[iw-1]);
      iw++;
    }
    putchar('\n');
  }

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
