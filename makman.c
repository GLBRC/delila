/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "makman.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.45
/*






*/



/*



























*/



#define maxstring       1000



#define fillermax       32



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
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


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
  clearstring(ribbon);
  ribbon->next = NULL;
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



Static Void copymanualpage(fin, fout)
_TEXT *fin, *fout;
{
  trigger debe, deen, vebe, veen, vt;
  stringDelila v;

  stringDelila aline, anamebegin;
  /*
*/
  boolean gotten;


  filltrigger(&vt, "version =                       ");
  filltrigger(&debe, "(@ begin module describe.       ");
  filltrigger(&deen, "(@ end module describe.         ");
  filltrigger(&vebe, "(@ begin module version *)      ");
  filltrigger(&veen, "(@ end module version           ");
  /*

*/
  debe.seek.letters[1] = '*';
  deen.seek.letters[1] = '*';
  vebe.seek.letters[1] = '*';
  veen.seek.letters[1] = '*';

  while (!BUFEOF(fin->f)) {
    getstring(fin, &aline, &gotten);



    resettrigger(&debe);
    resettrigger(&vebe);



    while (aline.current <= aline.length && !debe.found && !vebe.found) {
      testfortrigger(aline.letters[aline.current - 1], &debe);
      testfortrigger(aline.letters[aline.current - 1], &vebe);


      if (veen.found) {
	if (aline.current != veen.seek.length)
	  resettrigger(&veen);
      }
      if (debe.found) {
	if (aline.current != debe.seek.length)
	  resettrigger(&debe);
      }
      aline.current++;
    }
    if (vebe.found) {
      getstring(fin, &v, &gotten);
      /*


*/


      resettrigger(&vt);
      while (v.current <= v.length && !vt.found) {
	testfortrigger(v.letters[v.current - 1], &vt);
	v.current++;
      }


      /*

*/
      resettrigger(&veen);
      resettrigger(&debe);
      while (!BUFEOF(fin->f) && !veen.found && !debe.found) {
	getstring(fin, &aline, &gotten);
	while (aline.current <= aline.length && !veen.found && !debe.found) {
	  testfortrigger(aline.letters[aline.current - 1], &veen);
	  testfortrigger(aline.letters[aline.current - 1], &debe);


	  if (veen.found) {
	    if (aline.current != veen.seek.length)
	      resettrigger(&veen);
	  }
	  if (debe.found) {
	    if (aline.current != debe.seek.length)
	      resettrigger(&debe);
	  }

	  aline.current++;
	}
	/*


*/
      }

      if (!veen.found || debe.found) {
	fprintf(fout->f, "{The end of the version module was not found for\n");
	writestring(fout, &v);
	fprintf(fout->f, "}\n");
	halt();
      }
      continue;
    }

    if (!debe.found)
      continue;



    if (vt.found) {
      writestring(fout, &vebe.seek);
      fprintf(fout->f, "\n{");
      writestring(fout, &v);
      fprintf(fout->f, "}\n");
      writestring(fout, &veen.seek);
      fprintf(fout->f, " *)\n\n");
      resettrigger(&vt);
    } else
      fprintf(fout->f, "{no \"version =\" string found}\n");


    writestring(fout, &aline);
    putc('\n', fout->f);


    resettrigger(&deen);
    while (!BUFEOF(fin->f) && !deen.found) {
      getstring(fin, &aline, &gotten);
      resettrigger(&deen);



      while (aline.current < aline.length && !deen.skip && !deen.found) {
	testfortrigger(aline.letters[aline.current - 1], &deen);
	if (deen.found) {
	  if (aline.current != deen.seek.length)
	    resettrigger(&deen);
	}
	aline.current++;
      }

      writestring(fout, &aline);
      putc('\n', fout->f);
    }


    if (BUFEOF(fin->f) && !deen.found) {
      fprintf(fout->f, "{ module ");
      writestring(fout, &anamebegin);
      fprintf(fout->f, " has no end}\n");
      halt();
    }

  }


}



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  copymanualpage(fin, fout);
  fprintf(fout->f, "{This manual page was created by makman %4.2f}\n",
	  version);
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
