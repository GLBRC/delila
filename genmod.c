/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "genmod.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.33
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



Static _TEXT entries;


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




Static Void objonline(object, line)
trigger *object;
stringDelila *line;
{
  /*

*/
  line->current = 0;
  resettrigger(object);
  while (line->current <= line->length && !object->found) {
    line->current++;
    testfortrigger(line->letters[line->current - 1], object);
  }
  /*


*/
}



Static Void find(thefile, object, line)
_TEXT *thefile;
trigger object;
stringDelila *line;
{
  /*


*/
  boolean gotten;

  resettrigger(&object);
  while ((!object.found) & (!BUFEOF(thefile->f))) {
    getstring(thefile, line, &gotten);
    if (gotten)
      objonline(&object, line);
  }
}



Static Void findat(thefile, object, spot, idline)
_TEXT *thefile;
trigger *object;
long spot;
stringDelila *idline;
{
  /*
*/
  boolean done = false;

  while ((!done) & (!BUFEOF(thefile->f))) {
    find(thefile, *object, idline);

    if (idline->current == object->seek.length + spot - 1)
      done = true;
  }
}



Static Void findentry(thefile, idline)
_TEXT *thefile;
stringDelila *idline;
{
  trigger entrystart;


  filltrigger(&entrystart, "LOCUS               ");
  findat(thefile, &entrystart, 1L, idline);
}



Static boolean endfeature(l)
stringDelila *l;
{
  /*
*/
  trigger sites, bc, ori;


  filltrigger(&sites, "SITES               ");
  filltrigger(&bc, "BASE COUNT          ");
  filltrigger(&ori, "ORIGIN              ");
  objonline(&sites, l);
  objonline(&bc, l);
  objonline(&ori, l);
  return (sites.found || bc.found || ori.found);
}



Static boolean findfeature(thefile, l)
_TEXT *thefile;
stringDelila *l;
{
  /*
*/
  boolean gotten;
  trigger feature;

  /*
*/


  filltrigger(&feature, "FEATURE             ");
  do {
    getstring(thefile, l, &gotten);

    if (gotten)
      objonline(&feature, l);
  } while (!((endfeature(l) || feature.found) | BUFEOF(thefile->f)));
  return (feature.found & (!BUFEOF(thefile->f)));
}



Static Void findsequence(thefile, idline)
_TEXT *thefile;
stringDelila *idline;
{
  /*
*/
  trigger seqstart;


  filltrigger(&seqstart, "ORIGIN              ");
  findat(thefile, &seqstart, 1L, idline);
}



Static long getint(l)
stringDelila *l;
{
  /*
*/
  long increment;
  /*
*/
  long sign;
  long sum = 0;

  while (l->current <= l->length && l->letters[l->current - 1] == ' ')
    l->current++;

  if (l->letters[l->current - 1] == '-') {
    sign = -1;
    l->current++;
  } else if (l->letters[l->current - 1] == '+') {
    sign = 1;
    l->current++;
  } else
    sign = 1;

  while (l->current <= l->length &&
	 (l->letters[l->current - 1] == '9' ||
	  l->letters[l->current - 1] == '8' ||
	  l->letters[l->current - 1] == '7' ||
	  l->letters[l->current - 1] == '6' ||
	  l->letters[l->current - 1] == '5' ||
	  l->letters[l->current - 1] == '4' ||
	  l->letters[l->current - 1] == '3' ||
	  l->letters[l->current - 1] == '2' ||
	  l->letters[l->current - 1] == '1' ||
	  l->letters[l->current - 1] == '0')) {
    switch (l->letters[l->current - 1]) {

    case '0':
      increment = 0;
      break;

    case '1':
      increment = 1;
      break;

    case '2':
      increment = 2;
      break;

    case '3':
      increment = 3;
      break;

    case '4':
      increment = 4;
      break;

    case '5':
      increment = 5;
      break;

    case '6':
      increment = 6;
      break;

    case '7':
      increment = 7;
      break;

    case '8':
      increment = 8;
      break;

    case '9':
      increment = 9;
      break;
    }
    sum = sum * 10 + increment;
    l->current++;
  }
  return (sign * sum);
}



Static Void getbase(thefile, base, gotten)
_TEXT *thefile;
Char *base;
boolean *gotten;
{
  /*








*/
  boolean done = false;

  /*
*/
  while (!done) {
    skipblanks(thefile);
    if (P_eoln(thefile->f)) {
      fscanf(thefile->f, "%*[^\n]");
      getc(thefile->f);
      continue;
    }
    *base = getc(thefile->f);

    if (*base == '\n')
      *base = ' ';
    if (*base != '/') {
      if (*base != '9' && *base != '8' && *base != '7' && *base != '6' &&
	  *base != '5' && *base != '4' && *base != '3' && *base != '2' &&
	  *base != '1' && *base != '0') {
/* p2c: genmod.p: Note: Eliminated unused assignment statement [338] */
	*gotten = true;
      }
      continue;
    }
    *base = getc(thefile->f);
    if (*base == '\n')
      *base = ' ';
    if (*base != '/') {
      printf("genbank entry ends with only one /\n");
      halt();
    }
    done = true;
    *gotten = false;
  }
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *entries;
  stringDelila line;
} ;

Local Void give(LINK)
struct LOC_themain *LINK;
{
  _TEXT TEMP;

  if (!BUFEOF(LINK->entries->f)) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &LINK->line);
    putchar('\n');
  }
}



Static Void themain(entries_)
_TEXT *entries_;
{
  struct LOC_themain V;
  Char base;
  long count;
  boolean gotten;
  long number = 0;

  V.entries = entries_;
  printf("genmod %4.2f\n", version);

  if (*V.entries->name != '\0') {
    if (V.entries->f != NULL)
      V.entries->f = freopen(V.entries->name, "r", V.entries->f);
    else
      V.entries->f = fopen(V.entries->name, "r");
  } else
    rewind(V.entries->f);
  if (V.entries->f == NULL)
    _EscIO2(FileNotFound, V.entries->name);
  RESETBUF(V.entries->f, Char);
  while (!BUFEOF(V.entries->f)) {
    findentry(V.entries, &V.line);
    give(&V);
    if (findfeature(V.entries, &V.line)) {
      printf("the features are: \n");
      do {
	getstring(V.entries, &V.line, &gotten);
	if (gotten) {
	  if (!endfeature(&V.line))
	    give(&V);
	}
      } while (!endfeature(&V.line) && gotten);
      printf("end of features\n");
    }
    findsequence(V.entries, &V.line);
    give(&V);

    if (BUFEOF(V.entries->f))
      break;
    number++;
    printf("entry number %ld\n", number);
    count = 0;
    do {
      getbase(V.entries, &base, &gotten);
      if (gotten) {
	putchar(base);
	count++;
	if (count >= 60) {
	  putchar('\n');
	  count = 0;
	}
      }
    } while (gotten);
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
  entries.f = NULL;
  strcpy(entries.name, "entries");
  themain(&entries);
_L1:
  if (entries.f != NULL)
    fclose(entries.f);
  exit(EXIT_SUCCESS);
}



/* End. */
