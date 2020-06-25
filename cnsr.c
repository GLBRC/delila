/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "cnsr.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*


*/



/*








































*/



#define maxstring       1500


#define fillermax       10


typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



/*


*/
typedef Char filler[fillermax];



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;



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



Static Char tocharacter(n)
long n;
{
  return ((Char)(n + '0'));
}



Static Void writeedit(tofile, state, s, e)
_TEXT *tofile;
long state;
stringDelila s, e;
{
  /*
*/
  /*

*/
  boolean doreturn;
  /*
*/
  long i;
  boolean printing;

/* p2c: cnsr.p: Note: Eliminated unused assignment statement [338] */
  if (s.length > 0) {
    for (i = 0; i < s.length; i++) {
      /*

*/
      printing = (e.letters[i] == '0');
      if (printing) {
	putc(s.letters[i], tofile->f);
/* p2c: cnsr.p: Note: Eliminated unused assignment statement [338] */
/* p2c: cnsr.p: Note: Eliminated unused assignment statement [338] */
      }
    }
  }
  /*

*/

/* p2c: cnsr.p: Note: Eliminated unused assignment statement [338] */
  doreturn = (state == 0);

  /*
*/
  if (doreturn)
    putc('\n', tofile->f);
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *outfile;
  boolean debugging;
  stringDelila idline;
  long state;
  /*




*/
  long readpoint;
} ;

Local Void fillback(back, LINK)
long back;
struct LOC_themain *LINK;
{
  long spot, FORLIM;

  if (LINK->debugging) {
    writestring(LINK->outfile, &LINK->idline);
    fprintf(LINK->outfile->f, "| fillback before\n");
  }
  FORLIM = LINK->readpoint;
  for (spot = LINK->readpoint - back - 1; spot < FORLIM; spot++)
    LINK->idline.letters[spot] = tocharacter(LINK->state);
  if (LINK->debugging) {
    writestring(LINK->outfile, &LINK->idline);
    fprintf(LINK->outfile->f, "| fillback after\n");
  }
}



Static Void themain(infile, outfile_)
_TEXT *infile, *outfile_;
{
  struct LOC_themain V;
  stringDelila buffer;
  Char c;
  boolean gotten;
  trigger t1b, t2b;

  V.outfile = outfile_;
  V.debugging = true;
  V.debugging = false;
  V.state = 0;


  filltrigger(&t1b, "[[        ");
  filltrigger(&t2b, "]]        ");

  while (!BUFEOF(infile->f)) {
    getstring(infile, &buffer, &gotten);
    if (!gotten)
      continue;
    if (V.debugging) {
      writestring(V.outfile, &buffer);
      putc('\n', V.outfile->f);
    }
    clearstring(&V.idline);

    resettrigger(&t1b);
    resettrigger(&t2b);

    V.idline.length = buffer.length;
    for (V.readpoint = 1; V.readpoint <= buffer.length; V.readpoint++) {
      V.idline.letters[V.readpoint-1] = tocharacter(V.state);
      c = buffer.letters[V.readpoint-1];

      testfortrigger(c, &t1b);
      testfortrigger(c, &t2b);

      switch (V.state) {

      case 0:
	if (t1b.found) {
	  V.state = 1;
	  fillback(1L, &V);
	} else if (t2b.found) {
	  V.state = 0;
	  fillback(0L, &V);
	}
	break;

      case 1:
	if (t1b.found) {
	  V.state = 1;
	  fillback(1L, &V);
	} else if (t2b.found)
	  V.state = 0;
	break;

      }
    }
    if (V.debugging) {
      writestring(V.outfile, &V.idline);
      putc('|', V.outfile->f);
      fprintf(V.outfile->f, " state = %ld\n", V.state);
    }
    writeedit(V.outfile, V.state, buffer, V.idline);
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
