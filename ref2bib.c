/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "ref2bib.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.46
/*
*/



/*


























*/



/*





































*/



#define maxstring       150



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



typedef struct list {
  stringDelila s;
  struct list *next;
} list;


Static _TEXT refs, bib;


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



Static Void partstring(tofile, s, start)
_TEXT *tofile;
stringDelila *s;
long start;
{
  /*
*/
  long i, FORLIM;

  FORLIM = s->length;
  for (i = start - 1; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}



Static Void make(l)
list **l;
{
  if (*l != NULL) {
    clearstring(&(*l)->s);
    return;
  }
  *l = (list *)Malloc(sizeof(list));
  clearstring(&(*l)->s);
  (*l)->next = NULL;
}



Static Void readentry(refs, l)
_TEXT *refs;
list **l;
{
  list *c;
  boolean done = false;
  boolean gotten;

  c = *l;
  while (!done) {
    getstring(refs, &c->s, &gotten);
    done = (c->s.length == 0 || !gotten);
    if (done)
      break;
    make(&c->next);
    c = c->next;
  }
}



Static Void show(afile, l, always)
_TEXT *afile;
list **l;
boolean always;
{
  /*

*/
  list *c;
  boolean missing = false;
  stringDelila *WITH;

  c = *l;
  while (c->s.length != 0) {
    WITH = &c->s;
    if (WITH->letters[WITH->length - 1] != '!')
      missing = true;
    c = c->next;
  }

  if (always || missing)
    fprintf(afile->f, "comment{\n");
  if (always || missing)
    fprintf(afile->f, "current list is: ******************\n");

  c = *l;
  while (c->s.length != 0) {
    WITH = &c->s;
    if (always || WITH->letters[WITH->length - 1] != '!')
      writestring(afile, &c->s);

    if (WITH->letters[WITH->length - 1] != '!')
      fprintf(afile->f, "<<<<<<<<<<<<<<<<<<");
    if (always || missing)
      putc('\n', afile->f);
    c = c->next;
  }
  if (always || missing)
    fprintf(afile->f, "********************************\n");
  if (always || missing)
    fprintf(afile->f, "}\n");
}



Static Void got(l)
list *l;
{
  stringDelila *WITH;

  WITH = &l->s;
  WITH->letters[WITH->length] = ' ';
  WITH->letters[WITH->length + 1] = 'G';
  WITH->letters[WITH->length + 2] = 'O';
  WITH->letters[WITH->length + 3] = 'T';
  WITH->letters[WITH->length + 4] = ' ';
  WITH->letters[WITH->length + 5] = 'I';
  WITH->letters[WITH->length + 6] = 'T';
  WITH->letters[WITH->length + 7] = '!';
  WITH->length += 8;
}



Static Void pluckauthor(l, bib, id, close)
list *l;
_TEXT *bib;
Char id;
boolean *close;
{
  list *c;
  boolean found = false, outside = true;
  stringDelila *WITH;

  if (*close)
    fprintf(bib->f, ",\n");
  if (id != 'E' && id != 'A') {
    printf("pluckauthor: unknown identifier: %c\n", id);
    halt();
  }
  c = l;
  while (c->s.length != 0) {
    WITH = &c->s;
    if (WITH->letters[0] == '%') {
      if (WITH->letters[1] == id) {
	if (outside) {
	  switch (WITH->letters[1]) {

	  case 'A':
	    fprintf(bib->f, "author = \"");
	    break;

	  case 'E':
	    fprintf(bib->f, "editor = \"");
	    break;
	  }
	  found = true;
	  outside = false;
	} else
	  fprintf(bib->f, "\n and ");
	partstring(bib, &c->s, 4L);
	got(c);
      } else if (!outside) {
	putc('"', bib->f);
	outside = true;
      }
    }
    c = c->next;
  }
  if (!outside)
    putc('"', bib->f);
  if (found)
    *close = true;
  else
    *close = false;
}



Static Void plucktitle(l, bib, id, what, close)
list *l;
_TEXT *bib;
Char id, what;
boolean *close;
{
  /*
*/
  list *c;
  boolean found = false, outside = true;
  stringDelila *WITH;

  if (*close)
    fprintf(bib->f, ",\n");
  if (id != 'B' && id != 'T') {
    printf("plucktitle: unknown identifier: %c\n", id);
    halt();
  }
  c = l;
  while (c->s.length != 0) {
    WITH = &c->s;
    if (WITH->letters[0] == '%') {
      if (WITH->letters[1] == id) {
	if (outside) {
	  switch (WITH->letters[1]) {

	  case 'T':
	    fprintf(bib->f, "title = \"");
	    break;

	  case 'B':
	    switch (what) {

	    case 'b':
	      fprintf(bib->f, "book = \"");
	      break;

	    case 'i':
	      fprintf(bib->f, "booktitle = \"");
	      break;

	    case 'a':
	      printf("in plucktitle\n");
	      halt();
	      break;
	    }
	    break;
	  }
	  found = true;
	  outside = false;
	}
	partstring(bib, &c->s, 4L);
	got(c);
      } else if (!outside) {
	putc('"', bib->f);
	outside = true;
      }
    } else if (!outside) {
      /*
*/
      putc('\n', bib->f);
      writestring(bib, &c->s);
      got(c);
    }
    c = c->next;
  }
  if (!outside)
    putc('"', bib->f);
  if (found)
    *close = true;
  else
    *close = false;
}



Static Void plucklines(l, bib, close)
list *l;
_TEXT *bib;
boolean *close;
{
  list *c;
  stringDelila *WITH;

  if (*close)
    fprintf(bib->f, ",\n");
  *close = false;
  c = l;
  while (c->s.length != 0) {
    WITH = &c->s;
    if (WITH->letters[0] == '%') {
      if (WITH->letters[1] == 'Z' || WITH->letters[1] == 'V' ||
	  WITH->letters[1] == 'P' || WITH->letters[1] == 'M' ||
	  WITH->letters[1] == 'J' || WITH->letters[1] == 'I' ||
	  WITH->letters[1] == 'D' || WITH->letters[1] == 'C') {
	if (*close)
	  fprintf(bib->f, ",\n");
	switch (WITH->letters[1]) {

	case 'C':
	  fprintf(bib->f, "address = \"");
	  break;

	case 'D':
	  fprintf(bib->f, "year = \"");
	  break;

	case 'I':
	  fprintf(bib->f, "institution = \"");
	  break;

	case 'J':
	  fprintf(bib->f, "journal = \"");
	  break;

	case 'M':
	  fprintf(bib->f, "month = \"");
	  break;

	case 'P':
	  fprintf(bib->f, "pages = \"");
	  break;

	case 'V':
	  fprintf(bib->f, "volume = \"");
	  break;

	case 'Z':
	  fprintf(bib->f, "comment = \"");
	  break;
	}
	partstring(bib, &c->s, 4L);
	got(c);
	putc('"', bib->f);
	*close = true;
      }
    }
    c = c->next;
  }
}



Static Void whatisit(l, bib, what)
list *l;
_TEXT *bib;
Char *what;
{
  /*
*/
  boolean article = false, book = false;
  list *c;
  stringDelila *WITH;
  _TEXT TEMP;

  putc('@', bib->f);

  c = l;
  while (c->s.length != 0) {
    WITH = &c->s;
    if (WITH->letters[0] == '%') {
      if (WITH->letters[1] == 'T' || WITH->letters[1] == 'B') {
	switch (WITH->letters[1]) {

	case 'B':
	  book = true;
	  break;

	case 'T':
	  article = true;
	  break;
	}
      }
    }
    c = c->next;
  }

  if (book && article) {
    fprintf(bib->f, "inbook");
    *what = 'i';
  } else if (book) {
    fprintf(bib->f, "book");
    *what = 'b';
  } else if (article) {
    fprintf(bib->f, "article");
    *what = 'a';
  } else {
    printf("whatisit: unidentified thing in: (assuming article)\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    show(&TEMP, &l, true);
    fprintf(bib->f, "article");
    *what = 'a';
  }
  putc('{', bib->f);
}



Static long crush(s)
stringDelila s;
{
  long i;

  i = s.length;
  while (i > 1 && s.letters[i-2] != ' ')
    i--;
  return i;
}



Static Void makekey(l, bib, close)
list *l;
_TEXT *bib;
boolean *close;
{
  list *a = NULL, *d = NULL, *c = l;
  stringDelila *WITH;
  _TEXT TEMP;

  while (c->s.length != 0) {
    WITH = &c->s;
    if (WITH->letters[0] == '%') {
      if (WITH->letters[1] == 'E' || WITH->letters[1] == 'A') {
	if (a == NULL)
	  a = c;
      }
      if (WITH->letters[1] == 'D') {
	if (d == NULL)
	  d = c;
      }
    }
    c = c->next;
  }

  if (a != NULL)
    partstring(bib, &a->s, crush(a->s));
  else {
    printf("missing AUTHOR or EDITOR in\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    show(&TEMP, &l, true);
    halt();
  }

  if (d != NULL)
    partstring(bib, &d->s, crush(d->s));
  else {
    printf("missing DATE in\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    show(&TEMP, &l, true);
    halt();
  }

  *close = true;
}



Static Void endentry(bib)
_TEXT *bib;
{
  fprintf(bib->f, "}\n\n");
}



Static Void themain(refs, bib)
_TEXT *refs, *bib;
{
  boolean close;
  long count = 0;
  list *l = NULL;
  Char what;

  printf("ref2bib %4.2f\n", version);
  if (*refs->name != '\0') {
    if (refs->f != NULL)
      refs->f = freopen(refs->name, "r", refs->f);
    else
      refs->f = fopen(refs->name, "r");
  } else
    rewind(refs->f);
  if (refs->f == NULL)
    _EscIO2(FileNotFound, refs->name);
  RESETBUF(refs->f, Char);
  if (*bib->name != '\0') {
    if (bib->f != NULL)
      bib->f = freopen(bib->name, "w", bib->f);
    else
      bib->f = fopen(bib->name, "w");
  } else {
    if (bib->f != NULL)
      rewind(bib->f);
    else
      bib->f = tmpfile();
  }
  if (bib->f == NULL)
    _EscIO2(FileNotFound, bib->name);
  SETUPBUF(bib->f, Char);
  make(&l);

  while (!BUFEOF(refs->f)) {
    readentry(refs, &l);
    count++;

    whatisit(l, bib, &what);
    makekey(l, bib, &close);

    pluckauthor(l, bib, 'A', &close);
    pluckauthor(l, bib, 'E', &close);
    plucktitle(l, bib, 'T', what, &close);
    plucktitle(l, bib, 'B', what, &close);
    plucklines(l, bib, &close);
    show(bib, &l, false);
    endentry(bib);
  }
  printf("%ld entries\n", count);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  bib.f = NULL;
  strcpy(bib.name, "bib");
  refs.f = NULL;
  strcpy(refs.name, "refs");
  themain(&refs, &bib);
_L1:
  if (refs.f != NULL)
    fclose(refs.f);
  if (bib.f != NULL)
    fclose(bib.f);
  exit(EXIT_SUCCESS);
}



/* End. */
