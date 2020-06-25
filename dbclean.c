/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dbclean.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.05
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



/*


*/
typedef Char filler[fillermax];



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;


Static _TEXT dbin, dbout;


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



Static Void copyline(fin, fout)
_TEXT *fin, *fout;
{
  /*

*/
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
}



Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) == ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) != ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}



Static Void grabtoken(thefile, thestring)
_TEXT *thefile;
stringDelila *thestring;
{
  Char c;
  boolean done = false;

  skipblanks(thefile);
  thestring->length = 0;
  while (!done) {
    if (P_eoln(thefile->f)) {
      done = true;
      break;
    }
    c = getc(thefile->f);
    if (c == '\n')
      c = ' ';
    if (c == ' ')
      done = true;
    else {
      thestring->length++;
      thestring->letters[thestring->length - 1] = c;
    }
  }
}



Static Void themain(dbin, dbout)
_TEXT *dbin, *dbout;
{
  Char c;
  boolean done;
  long linenumber = 1;
  trigger locus, locusend;
  long locuscount = 0;

  printf("dbclean %4.2f\n", version);
  if (*dbin->name != '\0') {
    if (dbin->f != NULL)
      dbin->f = freopen(dbin->name, "r", dbin->f);
    else
      dbin->f = fopen(dbin->name, "r");
  } else
    rewind(dbin->f);
  if (dbin->f == NULL)
    _EscIO2(FileNotFound, dbin->name);
  RESETBUF(dbin->f, Char);
  if (*dbout->name != '\0') {
    if (dbout->f != NULL)
      dbout->f = freopen(dbout->name, "w", dbout->f);
    else
      dbout->f = fopen(dbout->name, "w");
  } else {
    if (dbout->f != NULL)
      rewind(dbout->f);
    else
      dbout->f = tmpfile();
  }
  if (dbout->f == NULL)
    _EscIO2(FileNotFound, dbout->name);
  SETUPBUF(dbout->f, Char);



  filltrigger(&locus, "LOCUS               ");
  filltrigger(&locusend, "//                  ");

  resettrigger(&locus);
  resettrigger(&locusend);


  while (!BUFEOF(dbin->f)) {
    c = getc(dbin->f);

    if (c == '\n')
      c = ' ';
    testfortrigger(c, &locus);
    if (locus.found) {
      locuscount++;
      fprintf(dbout->f, "LOCUS");


      done = false;
      while (!done) {
	while (!P_eoln(dbin->f)) {
	  c = getc(dbin->f);
	  if (c == '\n')
	    c = ' ';
	  putc(c, dbout->f);

	  testfortrigger(c, &locusend);
	  if (locusend.found) {
	    putc('\n', dbout->f);
	    done = true;
	  }

	}
	resettrigger(&locus);
	resettrigger(&locusend);

	fscanf(dbin->f, "%*[^\n]");
	getc(dbin->f);
	putc('\n', dbout->f);

	if (BUFEOF(dbin->f)) {
	  if (done)
	    break;
	  printf("BAD DATABASE: end of dbin before end of an entry\n");
	  halt();
	}
      }
    }

    if (BUFEOF(dbin->f))
      break;
    if (P_eoln(dbin->f)) {
      fscanf(dbin->f, "%*[^\n]");
      getc(dbin->f);
    }
  }

  if (locuscount != 1)
    printf("%ld loci\n", locuscount);
  else
    printf("%ld locus\n", locuscount);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  dbout.f = NULL;
  strcpy(dbout.name, "dbout");
  dbin.f = NULL;
  strcpy(dbin.name, "dbin");
  themain(&dbin, &dbout);
_L1:
  if (dbin.f != NULL)
    fclose(dbin.f);
  if (dbout.f != NULL)
    fclose(dbout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
