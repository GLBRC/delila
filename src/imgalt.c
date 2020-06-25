/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "imgalt.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         2.40
/*





















































































































































*/
#define updateversion   1.70



/*








































































































































































































































*/


#define pagewidth       60


#define maxstring       2000



#define fillermax       50




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


Static _TEXT orihtml, imgaltp, imagereport, imagenames, altstrings, althtml,
	     stop, stophold;


/*
*/



Static Void copyfileversion(fin, fout)
_TEXT *fin, *fout;
{
  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  RESETBUF(fin->f, Char);
  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);
  fprintf(fout->f, "* imgalt %4.2f\n", version);
  while (!BUFEOF(fin->f)) {
    while (!P_eoln(fin->f)) {
      putc(P_peek(fin->f), fout->f);
      getc(fin->f);
    }
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
    putc('\n', fout->f);
  }
  fprintf(fout->f, "program halt.\n");
}


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  /*



*/
  _TEXT TEMP;

  copyfileversion(&stophold, &stop);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  copyfileversion(&stophold, &TEMP);
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


#define tab             9






Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == tab);
}

#undef tab


Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while (isblankDelila(P_peek(thefile->f)) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((!isblankDelila(P_peek(thefile->f))) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
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



Static Void onetoken(afile, buffer, gotten)
_TEXT *afile;
stringDelila *buffer;
boolean *gotten;
{
  /*

*/
  long index = 0;
  boolean done = false;

  skipblanks(afile);
  emptystring(buffer);
  if (BUFEOF(afile->f)) {
    *gotten = false;
    return;
  }
  while (!P_eoln(afile->f) && index < maxstring && !done) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
    if (buffer->letters[index-1] == ' ') {
      done = true;
      index--;
    }
  }
  buffer->length = index;
  buffer->current = 1;
  *gotten = true;
}


#define tabcharacter    9



Static Void readquotestring(afile, s)
_TEXT *afile;
stringDelila *s;
{
  /*
*/
  Char c;
  long i, FORLIM;

  emptystring(s);
  skipblanks(afile);
  c = getc(afile->f);
  if (c == '\n')
    c = ' ';
  if (c != '"') {
    printf("quote string expected but \" missing\n");
    printf("character found instead:\"%c\"\n", c);
    halt();
  }
  if (P_eoln(afile->f)) {
    printf("missing end of quote string \" string\n");
    halt();
  }
  do {
    c = getc(afile->f);
    if (c == '\n')
      c = ' ';
    if (c != '"') {
      s->length++;
      s->letters[s->length - 1] = c;
    }
  } while (!(((c == '"') | P_eoln(afile->f)) || s->length == maxstring));
  if (s->length == maxstring) {
    printf("A quote string exceeded %ld characters.\n", (long)maxstring);
    printf("Use a shorter string or increase constant maxstring.\n");
    halt();
  }
  if (c != '"') {
    printf("The second \" mark of a string is missing.\n");
    halt();
  }

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++) {
    if (s->letters[i] == ' ')
      s->letters[i] = (Char)tabcharacter;
  }
}

#undef tabcharacter


#define tabcharacter    9



Static Void detabstring(s)
stringDelila *s;
{
  /*

*/
  Char c;
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++) {
    c = s->letters[i];
    /*


*/
    if (c == tabcharacter)
      c = ' ';
    s->letters[i] = c;
  }
}

#undef tabcharacter



Static Void writequotestring(afile, s)
_TEXT *afile;
stringDelila s;
{
  /*


*/
  detabstring(&s);
  putc('"', afile->f);
  writestring(afile, &s);
  putc('"', afile->f);
}



Static Char decapitalize(c)
Char c;
{
  long n = c;

  if (n >= 'A' && n <= 'Z')
    c = _tolower(n);
  else
    c = (Char)n;
  return c;
}



Static Void decapstring(t)
stringDelila *t;
{
  long i, FORLIM;

  FORLIM = t->length;
  for (i = 0; i < FORLIM; i++)
    t->letters[i] = decapitalize(t->letters[i]);
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



Static boolean equalstring(a, b)
stringDelila a, b;
{
  /*





*/
  long index;
  boolean equal;

  if (a.length == b.length) {
    index = 1;
    do {
      equal = (a.letters[index-1] == b.letters[index-1]);
      index++;
    } while (equal && index <= a.length);
    return equal;
  } else
    return false;
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






Static Void writewrapquotestring(f, indentation, startindent, linewidth,
				 thestring)
_TEXT *f;
stringDelila indentation;
long startindent, linewidth;
stringDelila thestring;
{
  /*














*/
  long column;
  long i = 0;
  long break_;

  column = indentation.length + startindent;
  putc('"', f->f);
  break_ = linewidth - column + 1;
  while (i < thestring.length) {
    i++;


    if (break_ < thestring.length) {
      while (thestring.letters[break_-1] != ' ' && break_ > i)
	break_--;
    }
    if (i != break_ || thestring.letters[i-1] != ' ') {
      putc(thestring.letters[i-1], f->f);
      continue;
    }
    putc('\n', f->f);
    writestring(f, &indentation);

    break_ = i + linewidth;
    /*
*/


    if (thestring.letters[i-1] != ' ')
      putc(thestring.letters[i-1], f->f);
  }

  putc('"', f->f);
}


#define tabcharacter    9

#define debug           false


#define debug_          false


#define debug__         false


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *imgaltp, *imagereport, *imagenames, *althtml;

  stringDelila altstring, srcstring, *auto_;
  /*

*/
  stringDelila *autoindex, *autofile;

  stringDelila *dupindex;
  long b;
  stringDelila buffer;
  long countpairs, removed;
  boolean done, foundfile;
  stringDelila filename;
  boolean gotten;
  long gotstp, linewidth;
  stringDelila indentation;

  stringDelila temporarytag;
  /*
*/
  long imagesALT, imagesFixedFromTemporaryTag;
  /*
*/
  long imagesFixedFromFile;



  long NAYaltNAYsup;

  long NAYaltYEAsup;

  long YEAaltNAYsup;

  long YEAaltYEAsup, imagesTOTAL;

  long linenumberO, linenumberOurl, linenumberOalt, linenumberA,
       linenumberAurl, linenumberAalt, lind, showedbuffer;
  boolean showconversion;
  trigger stptrigger;

  boolean foundalt, foundsrc, foundsrceq, foundalteq;

  boolean inimage;
  /*
*/
  boolean inquote;
  /*
*/
  boolean replacedstring, incomment;

  _TEXT hold;
} ;

Local Void showbuffer(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  if (LINK->showedbuffer >= LINK->linenumberO)
    return;
  fprintf(f->f, "%*ld ", (int)LINK->lind, LINK->linenumberO);
  writestring(f, &LINK->buffer);
  putc('\n', f->f);
  LINK->showedbuffer = LINK->linenumberO;
}

Local Void uparrow(f, column, LINK)
_TEXT *f;
long column;
struct LOC_themain *LINK;
{
  /*
*/
  fprintf(f->f, "%*c^ %ld ", (int)(column + LINK->lind), ' ', column);
}

Local Void makefilename(srcstring, filename, LINK)
stringDelila *srcstring, *filename;
struct LOC_themain *LINK;
{
  /*
*/
  long srcstringindex = 1;

  clearstring(filename);
  while (srcstringindex <= srcstring->length) {
    if (srcstring->letters[srcstringindex-1] == '/')
      filename->length = 0;
    else {
      filename->length++;
      filename->letters[filename->length - 1] = srcstring->letters[srcstringindex-1];
    }
    srcstringindex++;
  }

  /*


*/
}

Local boolean checkforfile(auto_, filename, LINK)
stringDelila *auto_, filename;
struct LOC_themain *LINK;
{
  /*

*/
  boolean Result;
  boolean foundfile = false;
  _TEXT TEMP;

  if (debug_)
    printf("checkforfile =================== BEGIN\n");
  LINK->autoindex = auto_;
  LINK->done = false;
  while (!LINK->done) {
    if (LINK->autoindex == NULL) {
      LINK->done = true;
      break;
    }
    if (debug_) {
      printf("CHECKING: ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, LINK->autoindex);
      printf(" against filename ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writequotestring(&TEMP, filename);
    }
    if (equalstring(filename, *LINK->autoindex)) {
      if (debug_)
	printf(" - FOUND IT!\n");
      foundfile = true;
      LINK->done = true;
      LINK->autoindex->current++;
      if (debug_) {
	printf(" with alt string:");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, *LINK->autoindex->next);
	printf(" <<<<<<<<<<<<<<<<\n");
      }
      continue;
    }

    if (debug_)
      printf(" - NOT FOUND\n");
    LINK->autoindex = LINK->autoindex->next;
    if (LINK->autoindex == NULL) {
      fprintf(stophold.f, "checkforfile: link error 2\n");
      halt();
      continue;
    }
    if (debug_) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, LINK->autoindex);
    }
    if (debug_)
      putchar('\n');
    LINK->autoindex = LINK->autoindex->next;
  }
  Result = foundfile;
  if (debug_)
    printf("checkforfile =================== END\n");
  return Result;
}

#undef debug_

Local Void writealtstrings(altstrings, filename, qstring, LINK)
_TEXT *altstrings;
stringDelila filename, qstring;
struct LOC_themain *LINK;
{
  writestring(altstrings, &filename);
  putc(' ', altstrings->f);
  writequotestring(altstrings, qstring);
  putc('\n', altstrings->f);
}

Local Void showauto(f, auto_, LINK)
_TEXT *f;
stringDelila **auto_;
struct LOC_themain *LINK;
{
  stringDelila *a;
  long count = 0;

  a = *auto_;
/* p2c: imgalt.p: Note: Eliminated unused assignment statement [338] */
  while (a != NULL) {
    count++;
    writestring(f, a);
    if (a->length == 0) {
      fprintf(stophold.f, "ERROR: ZERO LENGTH FILE NAME!\n");
      fprintf(stophold.f, "Are you missing quotes around your src string?\n");
/* p2c: imgalt.p: Note: Eliminated unused assignment statement [338] */
      a = a->next;
      if (a != NULL) {
	fprintf(stophold.f, "The next string (#%ld) in the auto list is:\n",
		count);

	writequotestring(&stophold, *a);
	putc('\n', stophold.f);
      } else
	fprintf(stophold.f, "no string name\n");
      halt();
    }

    putc(' ', f->f);
    a = a->next;
    if (a == NULL) {
      fprintf(stophold.f, "showauto: strings not in pairs, program error\n");
      halt();
    }
    writequotestring(f, *a);
    putc('\n', f->f);
    a = a->next;
  }

}

Local Void addaltstrings(auto_, filename, qstring, LINK)
stringDelila **auto_, filename, qstring;
struct LOC_themain *LINK;
{
  stringDelila *current, *last;
  boolean done = false, duplicate = false;
  _TEXT TEMP;

  if (debug__) {
    printf("\naddaltstrings BEGIN part <<<<<<<<<<<<<<<<<<<< 0000000000\n");
    printf("filename: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, filename);
    printf("\n qstring: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, qstring);
    putchar('\n');
  }

  if (*auto_ == NULL) {
    if (debug__)
      printf("addaltstrings first part <<<<<<<<<<<<<<<<<<<< 1111111111\n");
    *auto_ = (stringDelila *)Malloc(sizeof(stringDelila));
    clearstring(*auto_);
    copystring(filename, *auto_);
    (*auto_)->next = (stringDelila *)Malloc(sizeof(stringDelila));
    current = (*auto_)->next;
    copystring(qstring, current);
    current->previous = *auto_;
    return;
  }
  if (debug__)
    printf("addaltstrings       part <<<<<<<<<<<<<<<<<<<< 2222222222\n");

  current = *auto_;
  last = current;
  while (current != NULL && !done) {
    if (equalstring(*current, filename)) {
      done = true;
      duplicate = true;
      printf("DUPLICATE: ");

      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &filename);
      putchar('\n');
    }
    last = current;
    current = current->next;
  }
  if (duplicate)
    return;
  if (debug__)
    printf("NOT DUPLICATE\n");

  if (last->next != NULL) {
    fprintf(stophold.f, "program error in addaltstrings\n");
    halt();
  }

  last->next = (stringDelila *)Malloc(sizeof(stringDelila));
  current = last->next;
  emptystring(current);
  copystring(filename, current);
  current->current = 1;
  current->previous = last;

  current->next = (stringDelila *)Malloc(sizeof(stringDelila));
  current = current->next;
  emptystring(current);
  copystring(qstring, current);
  current->next = NULL;
  current->previous = last;

}

#undef debug__

Local Void showlist(LINK)
struct LOC_themain *LINK;
{
  stringDelila *a, indentation;
  long spaces;
  _TEXT TEMP;

  clearstring(&indentation);
  a = LINK->auto_;
  LINK->done = false;
  while (!LINK->done) {
    if (a == NULL) {
      LINK->done = true;
      break;
    }
    printf("LIST: \"");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, a);
    printf("\" %2ld", a->length);

    spaces = pagewidth - a->length;
    if (spaces < 1)
      spaces = 1;
    if (a->previous == NULL)
      printf("%*c ------ previous is nil", (int)spaces, ' ');
    putchar('\n');
    a = a->next;
  }
}

Local Void replacealtstring(LINK)
struct LOC_themain *LINK;
{
  /*



*/
  if (LINK->foundfile) {
    copystring(*LINK->autoindex->next, &LINK->altstring);
    LINK->imagesFixedFromFile++;
  } else {
    if (!LINK->foundalt) {
      /*

*/
      copystring(LINK->temporarytag, &LINK->altstring);
      LINK->imagesFixedFromTemporaryTag++;
    }
    addaltstrings(&LINK->auto_, LINK->filename, LINK->altstring, LINK);
  }
  LINK->replacedstring = true;
}

Local Void readfilealtpairs(LINK)
struct LOC_themain *LINK;
{
  long aii;
  _TEXT TEMP;
  stringDelila *WITH;
  long FORLIM;

  LINK->countpairs = 0;
  LINK->removed = 0;
  LINK->auto_ = (stringDelila *)Malloc(sizeof(stringDelila));
  emptystring(LINK->auto_);
  printf("Reading file/alt pairs from imgaltp:\n");
  LINK->auto_->previous = NULL;
  LINK->auto_->next = NULL;
  LINK->autoindex = LINK->auto_;
  while (!BUFEOF(LINK->imgaltp->f)) {
    if (P_eoln(LINK->imgaltp->f)) {
      fscanf(LINK->imgaltp->f, "%*[^\n]");
      getc(LINK->imgaltp->f);
      if (!BUFEOF(LINK->imgaltp->f))
	continue;

      /*



*/
      if (LINK->autoindex->length != 0) {
	printf("autoindex should be empty but is not: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, *LINK->autoindex);
	putchar('\n');
	halt();
      }
      if (LINK->autoindex->previous != NULL)
	LINK->autoindex->previous->next = NULL;
      /*
*/
      Free(LINK->autoindex);
      continue;
    }
    onetoken(LINK->imgaltp, LINK->autoindex, &LINK->gotten);
    if (!LINK->gotten) {
      fprintf(stophold.f, "could not get file name from parameters\n");
      halt();
      continue;
    }
    if (LINK->showconversion) {
      printf("   ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, LINK->autoindex);
      printf(" -> ");
    }
    LINK->autoindex->next = (stringDelila *)Malloc(sizeof(stringDelila));
    emptystring(LINK->autoindex->next);
    LINK->autoindex->next->previous = LINK->autoindex;
    LINK->autoindex->next->next = NULL;
    LINK->autoindex->current = 0;
    LINK->autofile = LINK->autoindex;
    LINK->autoindex = LINK->autoindex->next;
    readquotestring(LINK->imgaltp, LINK->autoindex);
    fscanf(LINK->imgaltp->f, "%*[^\n]");
    getc(LINK->imgaltp->f);
    /*
*/
    WITH = LINK->autoindex;
    FORLIM = WITH->length;
    for (aii = 0; aii < FORLIM; aii++) {
      if (WITH->letters[aii] == (Char)tabcharacter)
	WITH->letters[aii] = ' ';
    }
    if (LINK->showconversion) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writequotestring(&TEMP, *LINK->autoindex);
      putchar('\n');
    }
    LINK->countpairs++;
    if (LINK->autoindex->previous != LINK->auto_) {
      LINK->dupindex = LINK->auto_;
      LINK->done = false;
      while (!LINK->done) {
	if (equalstring(*LINK->dupindex, *LINK->autofile)) {
	  printf("%*c => ", (int)LINK->autofile->length, ' ');
	  printf("DUPLICATE string in imgaltp - REMOVED\n");
	  LINK->autoindex = LINK->autoindex->previous->previous;
	  Free(LINK->autoindex->next->next);
	  Free(LINK->autoindex->next);
	  LINK->autoindex->next = NULL;
	  LINK->done = true;
	  LINK->removed++;
	  continue;
	}
	if (LINK->dupindex->next == NULL) {
	  fprintf(stophold.f, "broken list\n");
	  halt();
	}
	LINK->dupindex = LINK->dupindex->next->next;
	if (LINK->dupindex == LINK->autofile)
	  LINK->done = true;
      }
    }

    else {

      if (debug)
	printf("skip duplicate check since it is first\n");
    }
    if (BUFEOF(LINK->imgaltp->f))
      break;
    LINK->autoindex->next = (stringDelila *)Malloc(sizeof(stringDelila));
    emptystring(LINK->autoindex->next);
    LINK->autoindex->next->previous = LINK->autoindex;
    LINK->autoindex->next->next = NULL;
    LINK->autoindex = LINK->autoindex->next;
    LINK->autoindex->current = 0;
  }
}

Local Void showparsedata(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  fprintf(f->f, "%4ld ", LINK->linenumberO);
  if (LINK->incomment)
    putc('C', f->f);
  else
    putc('-', f->f);
  if (LINK->inquote)
    putc('Q', f->f);
  else
    putc('-', f->f);
  if (LINK->inimage)
    putc('I', f->f);
  else
    putc('-', f->f);
  putc(' ', f->f);
  if (LINK->foundsrc)
    putc('s', f->f);
  else
    putc('-', f->f);
  if (LINK->foundsrceq)
    putc('=', f->f);
  else
    putc('-', f->f);
  if (LINK->foundalt)
    putc('a', f->f);
  else
    putc('-', f->f);
  if (LINK->foundalteq)
    putc('=', f->f);
  else
    putc('-', f->f);
}

Local Void processimage(LINK)
struct LOC_themain *LINK;
{
  /*

*/
  long blanks = 0;

  /*





*/


  makefilename(&LINK->srcstring, &LINK->filename, LINK);

  LINK->foundfile = checkforfile(LINK->auto_, LINK->filename, LINK);

  replacealtstring(LINK);

  LINK->gotstp = LINK->b;
  LINK->imagesTOTAL++;



  clearstring(&LINK->indentation);
  while (blanks <= LINK->buffer.length &&
	 (LINK->buffer.letters[blanks] == ' ' ||
	  LINK->buffer.letters[blanks] == (Char)tabcharacter)) {
    LINK->indentation.length++;
    LINK->indentation.letters[LINK->indentation.length - 1] =
      LINK->buffer.letters[blanks];
    blanks++;
  }


  if (LINK->foundalt) {
    LINK->imagesALT++;
    if (LINK->foundfile)
      LINK->YEAaltYEAsup++;
    else
      LINK->YEAaltNAYsup++;
  } else {
    if (LINK->foundfile)
      LINK->NAYaltYEAsup++;
    else
      LINK->NAYaltNAYsup++;
    showbuffer(&LINK->hold, LINK);
    uparrow(&LINK->hold, LINK->gotstp, LINK);
    fprintf(LINK->hold.f, "stp\n");
    fprintf(LINK->hold.f, " NO ");
    writestring(&LINK->hold, &LINK->temporarytag);
    fprintf(LINK->hold.f, " FOR THIS IMAGE\n");

    if (*LINK->hold.name != '\0') {
      if (LINK->hold.f != NULL)
	LINK->hold.f = freopen(LINK->hold.name, "r", LINK->hold.f);
      else
	LINK->hold.f = fopen(LINK->hold.name, "r");
    } else
      rewind(LINK->hold.f);
    if (LINK->hold.f == NULL)
      _EscIO2(FileNotFound, LINK->hold.name);
    RESETBUF(LINK->hold.f, Char);
    while (!BUFEOF(LINK->hold.f))
      copyaline(&LINK->hold, LINK->imagereport);
  }
  if (*LINK->hold.name != '\0') {
    if (LINK->hold.f != NULL)
      LINK->hold.f = freopen(LINK->hold.name, "w", LINK->hold.f);
    else
      LINK->hold.f = fopen(LINK->hold.name, "w");
  } else {
    if (LINK->hold.f != NULL)
      rewind(LINK->hold.f);
    else
      LINK->hold.f = tmpfile();
  }
  if (LINK->hold.f == NULL)
    _EscIO2(FileNotFound, LINK->hold.name);
  SETUPBUF(LINK->hold.f, Char);
  LINK->inimage = false;

  LINK->foundsrc = false;
  LINK->foundsrceq = false;
  LINK->foundalteq = false;


  putc('\n', LINK->althtml->f);
  writestring(LINK->althtml, &LINK->indentation);
  fprintf(LINK->althtml->f, "src=");
  writequotestring(LINK->althtml, LINK->srcstring);
  putc('\n', LINK->althtml->f);




  writestring(LINK->althtml, &LINK->indentation);
  fprintf(LINK->althtml->f, "alt=");

  /*





*/

  writewrapquotestring(LINK->althtml, LINK->indentation, 4L, LINK->linewidth,
		       LINK->altstring);
  /*


*/


  writestring(LINK->althtml, &LINK->stptrigger.seek);

  /*





*/

  /*

*/
  /*






*/



  if (!LINK->foundalt) {
    LINK->linenumberA++;
    LINK->linenumberAalt = LINK->linenumberA;
    if (LINK->foundfile)
      fprintf(LINK->imagenames->f, "+++ ");
    else
      fprintf(LINK->imagenames->f, "--- ");
  } else {
    fprintf(LINK->imagenames->f, "alt ");

  }

  writestring(LINK->imagenames, &LINK->srcstring);
  fprintf(LINK->imagenames->f, " %ld %ld %ld %ld\n",
	  LINK->linenumberOurl, LINK->linenumberOalt, LINK->linenumberAurl,
	  LINK->linenumberAalt);


}

Local Void bufferout(f, buffero, LINK)
_TEXT *f;
stringDelila *buffero;
struct LOC_themain *LINK;
{
  boolean done = false;

  while (!done) {
    if (buffero->length <= 0) {
      done = true;
      break;
    }
    if (buffero->letters[buffero->length - 1] == ' ')
      buffero->length--;
    else
      done = true;
  }
  writestring(f, buffero);
}



Static Void themain(orihtml, imgaltp_, imagereport_, imagenames_, althtml_,
		    altstrings, stop)
_TEXT *orihtml, *imgaltp_, *imagereport_, *imagenames_, *althtml_,
      *altstrings, *stop;
{
  struct LOC_themain V;
  stringDelila buffero;
  Char c, d;
  long gotimg, gotalt, imagesSUM;
  double parameterversion;

  trigger imgtrigger, alttrigger, quotrigger, srctrigger, cbgtrigger,
	  cedtrigger;
  boolean fusedlines = false;
  /*
*/

  Char lastbuffero = '_';
  /*
*/
  boolean skipquote = false, skipalt = false, skipsrc = false,
	  processed = false;
  stringDelila currentquote;
  boolean notdoublespace, showparse;
  boolean parsefail = false;
  _TEXT TEMP;

  V.imgaltp = imgaltp_;
  V.imagereport = imagereport_;
  V.imagenames = imagenames_;
  V.althtml = althtml_;
  V.hold.f = NULL;
  *V.hold.name = '\0';
  printf("imgalt %4.2f\n", version);
  if (*stophold.name != '\0') {
    if (stophold.f != NULL)
      stophold.f = freopen(stophold.name, "w", stophold.f);
    else
      stophold.f = fopen(stophold.name, "w");
  } else {
    if (stophold.f != NULL)
      rewind(stophold.f);
    else
      stophold.f = tmpfile();
  }
  if (stophold.f == NULL)
    _EscIO2(FileNotFound, stophold.name);
  SETUPBUF(stophold.f, Char);

  if (*V.imgaltp->name != '\0') {
    if (V.imgaltp->f != NULL)
      V.imgaltp->f = freopen(V.imgaltp->name, "r", V.imgaltp->f);
    else
      V.imgaltp->f = fopen(V.imgaltp->name, "r");
  } else
    rewind(V.imgaltp->f);
  if (V.imgaltp->f == NULL)
    _EscIO2(FileNotFound, V.imgaltp->name);
  RESETBUF(V.imgaltp->f, Char);
  fscanf(V.imgaltp->f, "%lg%*[^\n]", &parameterversion);
  getc(V.imgaltp->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    fprintf(stophold.f, "You have an old parameter file!\n");
    halt();
  }
  if (!BUFEOF(V.imgaltp->f)) {
    V.showconversion = (P_peek(V.imgaltp->f) == 'c');
    getc(V.imgaltp->f);
    if (!P_eoln(V.imgaltp->f))
      showparse = (P_peek(V.imgaltp->f) == 'p');
    else
      showparse = false;
    fscanf(V.imgaltp->f, "%*[^\n]");
    getc(V.imgaltp->f);
  } else {
    fprintf(stophold.f, "imgaltp is missing line 2\n");
    halt();
  }

  if (!BUFEOF(V.imgaltp->f)) {
    getstring(V.imgaltp, &V.temporarytag, &V.gotten);
    printf("temporary tag: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &V.temporarytag);
    putchar('\n');
  } else {
    fprintf(stophold.f, "imgaltp missing is line 3\n");
    halt();
  }


  readfilealtpairs(&V);

  if (V.countpairs == 0) {
    /*
*/
    V.auto_ = NULL;
  }



  printf("counted pairs in imgaltp: %ld\n", V.countpairs);
  printf("removed pairs in imgaltp: %ld\n", V.removed);

  if (debug)
    showlist(&V);



  filltrigger(&imgtrigger,
	      "<img                                              ");
  filltrigger(&alttrigger,
	      "alt                                               ");
  filltrigger(&srctrigger,
	      "src                                               ");
  filltrigger(&V.stptrigger,
	      ">                                                 ");
  filltrigger(&quotrigger,
	      "\"                                                 ");
  filltrigger(&cbgtrigger,
	      "<!--                                              ");
  /*

*/

  filltrigger(&cedtrigger,
	      "->                                                ");
  resettrigger(&imgtrigger);
  resettrigger(&alttrigger);
  resettrigger(&srctrigger);
  resettrigger(&V.stptrigger);
  resettrigger(&quotrigger);
  resettrigger(&cbgtrigger);
  resettrigger(&cedtrigger);

  clearstring(&currentquote);
  clearstring(&V.altstring);
  clearstring(&V.srcstring);

  V.linenumberO = 0;
  V.linenumberOurl = 0;
  V.linenumberOalt = 0;
  V.linenumberA = 0;
  V.linenumberAurl = 0;
  V.linenumberAalt = 0;
  V.linewidth = pagewidth;
  V.lind = 3;
  V.showedbuffer = 0;
  V.foundalt = false;
  V.replacedstring = false;
  V.filename.current = 0;

  V.inimage = false;
  V.inquote = false;
  V.incomment = false;


  V.imagesALT = 0;
  V.imagesFixedFromTemporaryTag = 0;
  V.imagesFixedFromFile = 0;
  V.imagesTOTAL = 0;
  V.NAYaltNAYsup = 0;
  V.NAYaltYEAsup = 0;
  V.YEAaltNAYsup = 0;
  V.YEAaltYEAsup = 0;

  if (*orihtml->name != '\0') {
    if (orihtml->f != NULL)
      orihtml->f = freopen(orihtml->name, "r", orihtml->f);
    else
      orihtml->f = fopen(orihtml->name, "r");
  } else
    rewind(orihtml->f);
  if (orihtml->f == NULL)
    _EscIO2(FileNotFound, orihtml->name);
  RESETBUF(orihtml->f, Char);
  if (*V.althtml->name != '\0') {
    if (V.althtml->f != NULL)
      V.althtml->f = freopen(V.althtml->name, "w", V.althtml->f);
    else
      V.althtml->f = fopen(V.althtml->name, "w");
  } else {
    if (V.althtml->f != NULL)
      rewind(V.althtml->f);
    else
      V.althtml->f = tmpfile();
  }
  if (V.althtml->f == NULL)
    _EscIO2(FileNotFound, V.althtml->name);
  SETUPBUF(V.althtml->f, Char);
  if (*V.imagereport->name != '\0') {
    if (V.imagereport->f != NULL)
      V.imagereport->f = freopen(V.imagereport->name, "w", V.imagereport->f);
    else
      V.imagereport->f = fopen(V.imagereport->name, "w");
  } else {
    if (V.imagereport->f != NULL)
      rewind(V.imagereport->f);
    else
      V.imagereport->f = tmpfile();
  }
  if (V.imagereport->f == NULL)
    _EscIO2(FileNotFound, V.imagereport->name);
  SETUPBUF(V.imagereport->f, Char);
  if (*V.imagenames->name != '\0') {
    if (V.imagenames->f != NULL)
      V.imagenames->f = freopen(V.imagenames->name, "w", V.imagenames->f);
    else
      V.imagenames->f = fopen(V.imagenames->name, "w");
  } else {
    if (V.imagenames->f != NULL)
      rewind(V.imagenames->f);
    else
      V.imagenames->f = tmpfile();
  }
  if (V.imagenames->f == NULL)
    _EscIO2(FileNotFound, V.imagenames->name);
  SETUPBUF(V.imagenames->f, Char);
  fprintf(V.imagenames->f, "* imgalt %4.2f\n", version);
  fprintf(V.imagenames->f,
    "* 1: alt/---; 2: url; 3: orihtml url; 3: orihtml alt; 4: althtml url; 5: althtml alt\n");
  if (*V.hold.name != '\0') {
    if (V.hold.f != NULL)
      V.hold.f = freopen(V.hold.name, "w", V.hold.f);
    else
      V.hold.f = fopen(V.hold.name, "w");
  } else {
    if (V.hold.f != NULL)
      rewind(V.hold.f);
    else
      V.hold.f = tmpfile();
  }
  if (V.hold.f == NULL)
    _EscIO2(FileNotFound, V.hold.name);
  SETUPBUF(V.hold.f, Char);

  if (showparse)
    printf("PARSE of the orihtml:\n\n");

  /*
*/
  while (!BUFEOF(orihtml->f)) {
    V.linenumberO++;
    V.linenumberA++;
    resettrigger(&imgtrigger);
    resettrigger(&alttrigger);
    resettrigger(&V.stptrigger);
    if (P_eoln(orihtml->f)) {
      fscanf(orihtml->f, "%*[^\n]");
      getc(orihtml->f);
      putc('\n', V.althtml->f);
      if (showparse) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showparsedata(&TEMP, &V);
	putchar('\n');
      }
      continue;
    }
    clearstring(&buffero);
    getstring(orihtml, &V.buffer, &V.gotten);
    if (!V.gotten) {
      fprintf(stophold.f, "buffer string not gotten\n");
      halt();
      continue;
    }
    if (showparse) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      showparsedata(&TEMP, &V);
      putchar(' ');
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &V.buffer);
      putchar('\n');
    }


    V.b = 1;
    while (V.b <= V.buffer.length) {
      c = V.buffer.letters[V.b-1];

      d = decapitalize(c);

      /*

*/

      if (!skipquote && !skipalt && !skipsrc) {
	if (V.inimage) {
	  /*
*/
	  if (lastbuffero != ' ' && V.b == 1) {
	    buffero.length++;
	    buffero.letters[buffero.length - 1] = ' ';
	  }
	}
	buffero.length++;
	buffero.letters[buffero.length - 1] = c;

	/*
*/
	if (V.inimage) {
	  if (c == ' ') {
	    if (buffero.length > 1) {
	      if (buffero.letters[buffero.length - 2] == ' ')
		buffero.length--;
	    }
	  }
	}



      }



      testfortrigger(d, &cbgtrigger);
      /*








*/
      if (cbgtrigger.found)
	V.incomment = true;
      testfortrigger(d, &cedtrigger);
      if (cedtrigger.found) {
	/*

*/
	if (V.b >= 3) {
	  if (V.buffer.letters[V.b-3] == '-') {
	    /*





*/
	    V.incomment = false;
	  }

	}
      }


      if (!V.inimage && !V.incomment) {
	testfortrigger(d, &imgtrigger);
	if (imgtrigger.found) {
	  gotimg = V.b;
	  V.inimage = true;
	  V.foundsrc = false;
	  V.foundsrceq = false;
	  V.foundalt = false;
	  processed = false;
	  V.foundalteq = false;
	  resettrigger(&quotrigger);
	  V.inquote = false;
	  skipquote = false;
	  /*

*/
	  clearstring(&currentquote);
	  clearstring(&V.altstring);
	  clearstring(&V.srcstring);
	  V.replacedstring = false;
	  fprintf(V.hold.f, "===========\n");
	  showbuffer(&V.hold, &V);
	  uparrow(&V.hold, gotimg, &V);
	  fprintf(V.hold.f, "img\n");
	  V.linenumberOalt = 0;
	  V.filename.length = 0;
	  fusedlines = false;
	  lastbuffero = '_';

	}

      }

      if (V.inimage && !V.incomment) {
	if (V.inquote && c == '"') {
	  if (skipalt)
	    skipalt = false;
	  if (skipsrc)
	    skipsrc = false;
	}



	if (c == '"')
	  V.inquote = !V.inquote;
	showbuffer(&V.hold, &V);

	if (V.inquote) {
	  if (c != '"') {
	    if (V.b == 1) {
	      /*
*/
	      currentquote.length++;
	      currentquote.letters[currentquote.length - 1] = ' ';
	    }


	    if (currentquote.length > 0) {
	      if (c != ' ' ||
		  currentquote.letters[currentquote.length - 1] != ' ')
		notdoublespace = true;
	      else
		notdoublespace = false;
	    } else
	      notdoublespace = true;

	    /*


*/
	    if (currentquote.length > 3 && !notdoublespace) {
	      if (currentquote.letters[currentquote.length - 2] == '.')
		notdoublespace = true;
	    }

	    if (notdoublespace) {
	      currentquote.length++;
	      currentquote.letters[currentquote.length - 1] = c;
	    }
	  }


	}

	else {
	  skipquote = false;

	  testfortrigger(d, &alttrigger);
	  if (alttrigger.found) {
	    gotalt = V.b;
	    showbuffer(&V.hold, &V);
	    uparrow(&V.hold, gotalt, &V);
	    fprintf(V.hold.f, "alt\n");
	    fprintf(V.hold.f, "alt\n");
	    V.linenumberOalt = V.linenumberO;
	    V.linenumberAalt = V.linenumberA;
	    V.foundalt = true;
	    skipalt = true;
	    if (!skipsrc)
	      buffero.length -= alttrigger.seek.length;

	    if (buffero.length > 0) {
	      if (buffero.letters[buffero.length - 1] == ' ')
		buffero.length--;
	    }
	  }
	  if (V.foundalt) {
	    if (c == '=')
	      V.foundalteq = true;
	  }

	  testfortrigger(d, &srctrigger);
	  if (srctrigger.found) {
	    showbuffer(&V.hold, &V);
	    uparrow(&V.hold, V.b, &V);
	    fprintf(V.hold.f, "src\n");
	    V.linenumberOurl = V.linenumberO;
	    V.linenumberAurl = V.linenumberA;
	    V.foundsrc = true;
	    skipsrc = true;
	    if (!skipalt)
	      buffero.length -= srctrigger.seek.length;

	    if (buffero.length > 0) {
	      if (buffero.letters[buffero.length - 1] == ' ')
		buffero.length--;
	    }
	  }
	  if (V.foundsrc) {
	    if (c == '=')
	      V.foundsrceq = true;
	  }

	  /*
*/
	  if (currentquote.length > 0) {
	    if (V.foundsrceq && V.srcstring.length == 0) {
	      copystring(currentquote, &V.srcstring);

	      if (showparse) {
		printf("srcstring ");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writequotestring(&TEMP, V.srcstring);
		putchar('\n');
	      }
	    }
	    if (V.foundalteq && V.altstring.length == 0) {
	      copystring(currentquote, &V.altstring);
	      if (showparse) {
		printf("altstring ");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writequotestring(&TEMP, V.altstring);
		putchar('\n');
	      }
	    }

	    clearstring(&currentquote);
	  }

	  testfortrigger(d, &V.stptrigger);
	  if (V.stptrigger.found) {
	    buffero.length -= V.stptrigger.seek.length;

	    bufferout(V.althtml, &buffero, &V);

	    if (buffero.length > 0)
	      lastbuffero = buffero.letters[buffero.length - 1];
	    else
	      lastbuffero = '_';
	    clearstring(&buffero);



	    if (V.srcstring.length == 0) {
	      fprintf(stophold.f, "ERROR: ZERO LENGTH FILE NAME!\n");
	      fprintf(stophold.f,
		      "Are you missing quotes around your src string?\n");
	      fprintf(stophold.f, "At line %ld of orihtml.\n", V.linenumberO);

	      halt();
	    }
	    /*
*/
	    processimage(&V);

	    processed = true;
	    V.foundsrc = false;
	    V.foundsrceq = false;
	    V.foundalt = false;
	    V.foundalteq = false;
	    skipalt = false;
	    skipsrc = false;
	  }


	  else
	    processed = false;
	}
      }



      V.b++;
    }

    bufferout(V.althtml, &buffero, &V);
    if (!skipalt && !skipsrc && !V.inimage)
      putc('\n', V.althtml->f);

    /*






*/

  }

  printf("--- Done reading orihtml ---\n");

  if (showparse)
    putchar('\n');
  printf("%2ld orihtml images had alt tags\n", V.imagesALT);
  printf("%2ld orihtml images fixed\n", V.imagesFixedFromFile);
  printf("%2ld orihtml images fixed using temporary tag\n",
	 V.imagesFixedFromTemporaryTag);


  printf("-----------------------------\n");
  printf("%2ld no alt tag, alternative not supplied: fill in with temporary tag\n",
	 V.NAYaltNAYsup);
  printf("%2ld no alt tag, alternative     supplied: use tag supplied in imgaltp\n",
	 V.NAYaltYEAsup);
  printf("%2ld    alt tag, alternative not supplied: leave alone\n",
	 V.YEAaltNAYsup);
  printf("%2ld    alt tag, alternative     supplied: use tag supplied in imgaltp\n",
	 V.YEAaltYEAsup);
  imagesSUM = V.NAYaltNAYsup + V.NAYaltYEAsup + V.YEAaltNAYsup + V.YEAaltYEAsup;
  printf("-----------------------------\n");
  printf("%2ld images total\n", imagesSUM);
  if (imagesSUM != V.imagesTOTAL)
    printf("COUNT ERROR: total images counted directly: %ld\n", V.imagesTOTAL);
  printf("The temporary tag is \"");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &V.temporarytag);
  printf("\".\n");

  printf("--- number of alt tags inserted for each file name: ----\n");
  if (V.countpairs > 0) {
    V.autoindex = V.auto_;
    while (V.autoindex != NULL) {
      if (V.autoindex->current > 0) {
	printf("* %ld ", V.autoindex->current);
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, V.autoindex);
	if (V.autoindex->current == 0)
	  printf(" -------------- in imgaltp but not used in orihtml");
	putchar('\n');
      }
      V.autoindex = V.autoindex->next;
      if (V.autoindex != NULL)
	V.autoindex = V.autoindex->next;
      else {
	fprintf(stophold.f, "themain: link error 3\n");
	halt();
      }
    }
  } else {
    printf("NONE\n");


  }
  if (V.inimage) {
    fprintf(stophold.f, "Still inside image at end of file\n");
    parsefail = true;
  }
  if (V.inquote) {
    fprintf(stophold.f, "Still inside quote at end of file\n");
    parsefail = true;
  }
  if (V.incomment) {
    fprintf(stophold.f, "Still inside comment at end of file\n");
    parsefail = true;
  }
  if (parsefail) {
    fprintf(stophold.f, " - PARSING FAILED - ERROR IN HTML\n");
    halt();
  }



  if (*altstrings->name != '\0') {
    if (altstrings->f != NULL)
      altstrings->f = freopen(altstrings->name, "w", altstrings->f);
    else
      altstrings->f = fopen(altstrings->name, "w");
  } else {
    if (altstrings->f != NULL)
      rewind(altstrings->f);
    else
      altstrings->f = tmpfile();
  }
  if (altstrings->f == NULL)
    _EscIO2(FileNotFound, altstrings->name);
  SETUPBUF(altstrings->f, Char);
  showauto(altstrings, &V.auto_, &V);

  if (V.hold.f != NULL)
    fclose(V.hold.f);
}

#undef tabcharacter
#undef debug


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  stophold.f = NULL;
  *stophold.name = '\0';
  stop.f = NULL;
  strcpy(stop.name, "stop");
  althtml.f = NULL;
  strcpy(althtml.name, "althtml");
  altstrings.f = NULL;
  strcpy(altstrings.name, "altstrings");
  imagenames.f = NULL;
  strcpy(imagenames.name, "imagenames");
  imagereport.f = NULL;
  strcpy(imagereport.name, "imagereport");
  imgaltp.f = NULL;
  strcpy(imgaltp.name, "imgaltp");
  orihtml.f = NULL;
  strcpy(orihtml.name, "orihtml");
  themain(&orihtml, &imgaltp, &imagereport, &imagenames, &althtml,
	  &altstrings, &stop);
_L1:
  if (orihtml.f != NULL)
    fclose(orihtml.f);
  if (imgaltp.f != NULL)
    fclose(imgaltp.f);
  if (imagereport.f != NULL)
    fclose(imagereport.f);
  if (imagenames.f != NULL)
    fclose(imagenames.f);
  if (altstrings.f != NULL)
    fclose(altstrings.f);
  if (althtml.f != NULL)
    fclose(althtml.f);
  if (stop.f != NULL)
    fclose(stop.f);
  if (stophold.f != NULL)
    fclose(stophold.f);
  exit(EXIT_SUCCESS);
}



/* End. */
