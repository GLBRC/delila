/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "nocomequal.p" */


/*

*/


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.24
/*





















*/



/*
































































*/


/*
*/

#define fillermax       50




#define maxstring       2000






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




Static Void prompt(afile)
_TEXT *afile;
{
  /*



*/
  if (BUFEOF(afile->f)) {
    if (*afile->name != '\0') {
      if (afile->f != NULL)
	afile->f = freopen(afile->name, "r", afile->f);
      else
	afile->f = fopen(afile->name, "r");
    } else
      rewind(afile->f);
    if (afile->f == NULL)
      _EscIO2(FileNotFound, afile->name);
    RESETBUF(afile->f, Char);
  }
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
}



Static Void readchar(afile, ch)
_TEXT *afile;
Char *ch;
{
  if (BUFEOF(afile->f))
    prompt(afile);
  *ch = getc(afile->f);

  if (*ch == '\n')
    *ch = ' ';
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


Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  Char c = ' ';
  boolean inquote;
  boolean incomment1 = false, incomment2 = false;
  Char p;
  _TEXT internal;
  trigger comment1, comment2;
  stringDelila inline_;
  boolean gotten;
  stringDelila outline;
  boolean done;

  boolean equallines = true;

  /*

*/


  internal.f = NULL;
  *internal.name = '\0';
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
  while (!BUFEOF(fin->f)) {
    inquote = false;
    while (!P_eoln(fin->f)) {
      p = c;
      c = getc(fin->f);

      if (c == '\n')
	c = ' ';
      if (!(incomment1 || incomment2)) {
	if (c == '\'')
	  inquote = !inquote;
      }

      if (!(incomment1 || incomment2))
	putc(c, internal.f);

      if (inquote)
	continue;
      if (!incomment2) {
	if (p == '(' && c == '*')
	  incomment1 = true;

	if (p == '*' && c == ')' && incomment1) {
	  incomment1 = false;
	  fprintf(internal.f, "*)");
	}
      }
      if (incomment1)
	continue;
      if (c == '{')
	incomment2 = true;
      if (c == '}' && incomment2) {
	incomment2 = false;
	putc('}', internal.f);
      }
    }

    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
    if (!(incomment1 || incomment2))
      putc('\n', internal.f);
    else {
      if (equallines)
	putc('\n', internal.f);
    }
  }


  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else {

    rewind(internal.f);
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
/* p2c: nocomequal.p: Note: Eliminated unused assignment statement [338] */
/* p2c: nocomequal.p: Note: Eliminated unused assignment statement [338] */
  filltrigger(&comment1, "(**)                                              ");
  filltrigger(&comment2, "{}                                                ");
  while (!BUFEOF(internal.f)) {
    if (P_eoln(internal.f)) {
      fscanf(internal.f, "%*[^\n]");

      getc(internal.f);
      if (!BUFEOF(internal.f))
	putc('\n', fout->f);
      continue;
    }
    inquote = false;

    resettrigger(&comment1);
    resettrigger(&comment2);
    clearstring(&inline_);
    clearstring(&outline);
    getstring(&internal, &inline_, &gotten);


    while (inline_.current <= inline_.length) {
      c = inline_.letters[inline_.current - 1];
      outline.length++;
      outline.letters[outline.length - 1] = c;

      if (c == '\'')
	inquote = !inquote;

      if (!inquote) {
	testfortrigger(c, &comment1);
	testfortrigger(c, &comment2);

	if (comment1.found)
	  outline.length -= comment1.seek.length;


	if (comment2.found)
	  outline.length -= comment2.seek.length;

      }
      inline_.current++;
    }



    done = false;
    while (!done) {
      if (outline.length == 0) {
	done = true;
	break;
      }
      if (outline.letters[outline.length - 1] == ' ')
	outline.length--;
      else
	done = true;
    }

    if (outline.length > 0) {
      writestring(fout, &outline);
      putc('\n', fout->f);
    } else {
      if (equallines)
	putc('\n', fout->f);
    }
  }
  if (internal.f != NULL)
    fclose(internal.f);
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
