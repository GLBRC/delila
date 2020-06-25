/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "epsclean.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         2.09
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


Static Void copydrop(trig, f, t)
trigger trig;
_TEXT *f, *t;
{
  /*

*/
  long count = 0;

  if (*f->name != '\0') {
    if (f->f != NULL)
      f->f = freopen(f->name, "r", f->f);
    else
      f->f = fopen(f->name, "r");
  } else
    rewind(f->f);
  if (f->f == NULL)
    _EscIO2(FileNotFound, f->name);
  RESETBUF(f->f, Char);
  resettrigger(&trig);

  while (count < 2) {
    testfortrigger(P_peek(f->f), &trig);
    if (trig.found) {
      count++;
      resettrigger(&trig);
    }
    if (P_eoln(f->f)) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
      putc('\n', t->f);
      continue;
    }
    /*


*/
    if (P_peek(f->f) == 13)
      CPUTFBUF(t->f, '\n');
    else
      CPUTFBUF(t->f, P_peek(f->f));
    CPUT(t->f);
    getc(f->f);
  }
  putc('\n', t->f);

}


Static Void skipcopy(f, t)
_TEXT *f, *t;
{
  /*

*/
  /*


*/
  /*


















*/

  while (!BUFEOF(f->f)) {
    if (P_eoln(f->f)) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
      putc('\n', t->f);
    } else {
      putc(P_peek(f->f), t->f);
      getc(f->f);
    }
  }

}


Static Void themain()
{
  trigger trig;
  _TEXT TEMP, TEMP1;

  /*

*/


  fillstring(&trig.seek, "dopic                                             ");

  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  copydrop(trig, &TEMP, &TEMP1);
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  skipcopy(&TEMP, &TEMP1);
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
