/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "shrink.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
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


Static _TEXT shrinkp;


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



Static Void themain(fin, shrinkp, fout)
_TEXT *fin, *shrinkp, *fout;
{
  Char c;
  double factor;
  boolean gotit = false;
  trigger translate;

  fprintf(fout->f, "%% shrink %4.2f\n", version);
  if (*shrinkp->name != '\0') {
    if (shrinkp->f != NULL)
      shrinkp->f = freopen(shrinkp->name, "r", shrinkp->f);
    else
      shrinkp->f = fopen(shrinkp->name, "r");
  } else
    rewind(shrinkp->f);
  if (shrinkp->f == NULL)
    _EscIO2(FileNotFound, shrinkp->name);
  RESETBUF(shrinkp->f, Char);
  fscanf(shrinkp->f, "%lg%*[^\n]", &factor);
  getc(shrinkp->f);



  filltrigger(&translate, "translate           ");
  resettrigger(&translate);



  while (!BUFEOF(fin->f) && !gotit) {
    while (!P_eoln(fin->f)) {
      c = getc(fin->f);
      if (c == '\n')
	c = ' ';
      putc(c, fout->f);
      testfortrigger(c, &translate);
      if (translate.found)
	gotit = true;
    }
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
    putc('\n', fout->f);
  }

  if (gotit)
    printf("%10.4f %10.4f scale\n", factor, factor);


  while (!BUFEOF(fin->f))
    copyaline(fin, fout);

}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  shrinkp.f = NULL;
  strcpy(shrinkp.name, "shrinkp");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &shrinkp, &TEMP1);
_L1:
  if (shrinkp.f != NULL)
    fclose(shrinkp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
