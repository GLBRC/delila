/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "fpcfix.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.06
/*







*/



/*



































































*/


/*
*/

/*

*/

#define fillermax       20




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



Static Void copystring(a, b)
stringDelila a, *b;
{
  /*
*/
  long l;

  b->length = a.length;
  for (l = 0; l < a.length; l++)
    b->letters[l] = a.letters[l];
  b->current = a.current;
  b->next = a.next;
}



Static Void concatstring(a, b, c)
stringDelila *a, *b, *c;
{
  long abindex;
  long cindex = 0;
  long FORLIM;

  emptystring(c);
  c->length = a->length + b->length;
  if (c->length > maxstring) {
    printf("string too long in concatstring\n");
    halt();
  }

  FORLIM = a->length;
  for (abindex = 0; abindex < FORLIM; abindex++) {
    cindex++;
    c->letters[abindex] = a->letters[abindex];
  }

  FORLIM = a->length + b->length;
  for (abindex = a->length; abindex < FORLIM; abindex++) {
    cindex++;
    c->letters[abindex] = b->letters[abindex - a->length];
  }
}



Static Void attachstring(a, b)
stringDelila *a, b;
{
  stringDelila hold;

  copystring(*a, &hold);
  concatstring(&hold, &b, a);
}


Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  Char c = ' ';
  boolean inquote;
  boolean incomment1 = false, incomment2 = false;
  Char p;

  stringDelila aline, bline;
  long a, b;

  trigger resetFind, rewriteFind;
  long beginresetrewrite;

  stringDelila insert;
  /*
*/
  long acopy;
  boolean d = false;
  boolean docopy;
  long FORLIM;
  _TEXT TEMP;

  /*
*/


  /*




*/




  filltrigger(&resetFind, "reset(              ");
  filltrigger(&rewriteFind, "rewrite(            ");
  fillstring(&insert, "assign(             ");
  /*

*/


  resettrigger(&resetFind);
  resettrigger(&rewriteFind);



  while (!BUFEOF(fin->f)) {
    inquote = false;

    clearstring(&aline);
    clearstring(&bline);
    aline.length = 0;
    while (!P_eoln(fin->f)) {
      aline.length++;
      aline.letters[aline.length - 1] = getc(fin->f);
      if (aline.letters[aline.length - 1] == '\n')
	aline.letters[aline.length - 1] = ' ';
    }
    fscanf(fin->f, "%*[^\n]");
    /*

*/

    getc(fin->f);
    b = 0;
    docopy = true;
    FORLIM = aline.length;
    for (a = 1; a <= FORLIM; a++) {
      p = c;
      c = aline.letters[a-1];
      testfortrigger(c, &resetFind);
      testfortrigger(c, &rewriteFind);

      if (!(incomment1 || incomment2)) {
	if (c == '\'')
	  inquote = !inquote;
      }

      if (!inquote) {
	if (!incomment2) {
	  if (p == '(' && c == '*')
	    incomment1 = true;

	  if (p == '*' && c == ')' && incomment1)
	    incomment1 = false;
	}
	if (!incomment1) {
	  if (c == '{')
	    incomment2 = true;
	  if (c == '}' && incomment2)
	    incomment2 = false;
	}
      }

      if (!(incomment1 || incomment2 || inquote)) {
	if (resetFind.found || rewriteFind.found) {
	  if (resetFind.found)
	    beginresetrewrite = a - resetFind.seek.length + 1;
	  if (rewriteFind.found)
	    beginresetrewrite = a - rewriteFind.seek.length + 1;


	  if (d) {
	    printf("FOUND resetFind, beginresetrewrite = %ld\n",
		   beginresetrewrite);
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writestring(&TEMP, &aline);
	    printf("\n1.bline:\"");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writestring(&TEMP, &bline);
	    printf("\"\n");
	  }


	  /*

*/
	  b = beginresetrewrite;
	  bline.length = b - 1;

	  attachstring(&bline, insert);
	  /*


*/

	  if (d) {
	    printf("2.bline:\"");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writestring(&TEMP, &bline);
	    printf("\"\n");

	  }


	  acopy = a + 1;
	  b = bline.length;
	  while (aline.letters[acopy-1] != ')' && acopy < maxstring) {
	    b++;
	    bline.letters[b-1] = aline.letters[acopy-1];
	    acopy++;
	  }

	  bline.length = b + 1;
	  if (d) {
	    printf("3.bline:\"");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writestring(&TEMP, &bline);
	    printf("\"\n");
	  }
	  b = bline.length;
	  bline.letters[b-1] = ',';
	  b++;
	  bline.letters[b-1] = '\'';
	  bline.length = b;
	  if (d) {
	    printf("4.bline:\"");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writestring(&TEMP, &bline);
	    printf("\"\n");
	  }


	  acopy = a + 1;
	  while (aline.letters[acopy-1] != ')' && acopy < maxstring) {
	    b++;
	    bline.letters[b-1] = aline.letters[acopy-1];
	    acopy++;
	  }
	  bline.length = b;
	  if (d) {
	    printf("5.bline:\"");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writestring(&TEMP, &bline);
	    printf("\"\n");
	  }

	  b++;
	  bline.letters[b-1] = '\'';
	  bline.length = b;
	  b++;
	  bline.letters[b-1] = ')';
	  bline.length = b;
	  b++;
	  bline.letters[b-1] = ';';
	  bline.length = b;
	  d = true;
	  d = false;
	  if (d) {
	    printf("6.bline:\"");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writestring(&TEMP, &bline);
	    printf("\"\n");
	  }

	  /*
*/

	  acopy = beginresetrewrite;
	  while (aline.letters[acopy-1] != ';' && acopy < maxstring) {
	    b++;
	    bline.letters[b-1] = aline.letters[acopy-1];
	    acopy++;
	  }
	  bline.length = b;
	  b++;
	  bline.letters[b-1] = ';';
	  bline.length = b;

	  if (d) {
	    printf("7.bline:\"");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writestring(&TEMP, &bline);
	    printf("\"\n");
	  }


	  /*


*/
	  docopy = false;

	}

	else {
	  /*

*/
	  if (docopy) {
	    b++;
	    bline.letters[b-1] = c;
	    bline.length = b;
	  } else {
	    if (c == ';')
	      docopy = true;
	  }
	}
      } else {

	/*


*/
	b++;
	bline.letters[b-1] = c;
	bline.length = b;
      }
    }

    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &bline);
    putchar('\n');

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
