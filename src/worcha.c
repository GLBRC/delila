/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "worcha.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/* worcha: word changing program
      by patrick r. roche
      module libraries: delman, delmods */

/* end of worcha */

/* begin module version */

#define version         2.57
/* of worcha.p 2003 Nov 17
2003 Nov 17, 2.57: increase word length buffer size
2000 jul 25, 2.56: increase line length buffer size
1996 Jul 29, 2.55: previous version
previous version: 1989 April 5
origin before 1982 aug 13 */
/* end module version */

/* begin module describe.worcha */
/*
name
      worcha: word changing program

synopsis
      worcha(fin: in, fout: out, worchap: in, output: out)

files
      fin:     the file in which words need to be changed to other words.
      fout:    the file where the copy of fin with the words changed is written.
      worchap: the parameter file containing the words that need to
               be replaced and their replacements.  Worchap must be
               constructed as follows:

               a word that needs to be changed is on the first line, the
               following line contains the replacement word,
                  next line:  word to be replaced,
                  following line: replacement word,
               and so on....etc.

               so, the odd numbered lines, (1,3,5....), have the words
               from fin that will be replaced, and the even numbered
               lines, (2,4,6...), contain the replacement words.
      output:  where error messages will appear.

description
      This program was designed to go through a pascal program and
      locate and replace 'words', (pascal identifiers).

         Worcha will sort through a file and look for the words that need to be
      changed, ignoring comments and both single and double quotes.  Upon
      finding the old words, worcha will substitute the specified new words from
      worchap when copying the input file onto the specified output file.  As
      many words as necessary may be changed at one time.  Worcha produces a
      list of the changes within a comment at the end of the fout file.

documentation
      delman.assembly.worcha

author
      Patrick R. Roche

bugs
      The program will yell if word length is greater than or equal to
      wdlgthmax.

technical notes
      Worcha uses linked-lists for storing the words to be changed and their
      replacements.  Thus as many words as desired may be changed at one time.

*/
/* end module describe.worcha */

#define debug           false   /* debugging displays on or off */
/* 1995 dec 20.  The program bombed on a line with quotes in it:
      delman.intro.organization 'technical notes'
The trigger to bomb was a quote on the end of a line.  (A single space after it
would solve the problem.) The bug was corrected by making the routines
skip2quotes and skip1quotes be a little smarter.  They now stop putting dots
down when they exceed the end of the line and when they hit the end of the
buffer.  I am leaving the debug code in.  If it is stable it can be removed
later.  There is also debug code left by Pat.  */

#define listonoutput    false   /* listing of changes to output */
#define listonfout      false   /* listing of changes to fout */

#define lnlgthmax       2000   /* max length of a buffer line */
#define wdlgthmax       2000   /* max length of a word buffer */


typedef struct buffer {
  /* this will hold the line of fin */
  Char place[lnlgthmax];
  long length;
} buffer;

typedef struct buff2 {
  /* this will hold the old and new words */
  Char place[wdlgthmax];
  long length;
} buff2;

/* a pointer to a word-record */

typedef struct aword {
  /* these are what will make up the linked-list */
  buff2 oldword;   /* the old word */
  buff2 newword;   /* the new word */
  long numbchgs;   /* number of times old word ---> new word */
  struct aword *next;
} aword;


Static _TEXT fin, fout, worchap;

Static aword *first;   /* the 'root' word pointer */


Static jmp_buf _JL1;


/* begin module halt */
Static Void halt()
{
  /* stop the program.  the procedure performs a goto to the end of the
     program.  you must have a label:
        label 1;
     declared, and also the end of the program must have this label:
        1: end.
     examples are in the module libraries.
     this is the only goto in the delila system. */
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


/* end module halt version = 'delmod 6.51 85 apr 17 tds/gds' */

Static Void clearb(b)
buffer *b;
{
  /* clear the buffer by filling with blanks */
  long i;   /* counter and index */

  for (i = 0; i < lnlgthmax; i++)
    b->place[i] = ' ';
  b->length = 0;
}


Static Void clearb2(b)
buff2 *b;
{
  /* clear the buffer by filling with blanks */
  long i;   /* counter and index */

  for (i = 0; i < wdlgthmax; i++)
    b->place[i] = ' ';
  b->length = 0;
}


Static Void writebuffer(fout, b)
_TEXT *fout;
buffer b;
{
  /* writeln buffer b to fout */
  long i = 0;   /* index of b */
  long c;   /* counter */

  for (c = 1; c <= b.length; c++) {
    i++;
    putc(b.place[i-1], fout->f);
  }
  putc('\n', fout->f);
}


Static Void writetrigger(fout, b)
_TEXT *fout;
buffer b;
{
  /* writeln buffer b to fout within quotes */
  long i = 0;   /* index of b */
  long c;   /* counter */

  fprintf(fout->f, " '");
  for (c = 1; c <= b.length; c++) {
    i++;
    putc(b.place[i-1], fout->f);
  }
  fprintf(fout->f, "'\n");
}


Static Void write2trigger(fout, b)
_TEXT *fout;
buff2 b;
{
  /* writeln buff2 b to fout within quotes */
  long i = 0;   /* index of b */
  long c;   /* counter */

  fprintf(fout->f, " '");
  for (c = 1; c <= b.length; c++) {
    i++;
    putc(b.place[i-1], fout->f);
  }
  fprintf(fout->f, "'\n");
}


Static Void writeword(fout, b)
_TEXT *fout;
buff2 b;
{
  /* write buff2 b to fout */
  long i = 0;   /* index of b */
  long c;   /* counter */

  /*zzz*/
  /*writeln(output,'b.length = ',b.length:1);*/
  for (c = 1; c <= b.length; c++) {
    i++;
    /*write(output,'i = ',i:1,' "');*/
    putc(b.place[i-1], fout->f);
    /*;writeln(output,'"');*/
  }
}


Static Void removeblanks(b)
buff2 *b;
{
  /* remove blanks from the end of a buff2 */
  if (b->place[b->length - 1] == ' ')   /* if the last place is blank */
  {  /* remove the blank and check next place etc. */
    do {
      b->length--;
    } while (b->place[b->length - 1] == ' ');
  }
}


Static Void readintobuffer(fin, b)
_TEXT *fin;
buffer *b;
{
  /* read a line from fin into buffer b */
  long i = 0;   /* index for b.place[i] and counter */
  _TEXT TEMP;

  clearb(b);   /* clear the buffer */
  /* starting at begining of the line */

  while ((i < lnlgthmax) & (!(P_eoln(fin->f) | BUFEOF(fin->f)))) {
    /* keeping line length below max and going to end of line */
    i++;   /* next character */
    b->place[i-1] = getc(fin->f);   /* read in next character to buffer */
    if (b->place[i-1] == '\n')
      b->place[i-1] = ' ';
    b->length = i;   /* keeping track of length of the buffer */
  }
  if (i == lnlgthmax) {
    printf(" input to buffer is too big\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writebuffer(&TEMP, *b);
    halt();
  }
  /* if the input line is more
                than the buffer can handle */
  if (!BUFEOF(fin->f)) {   /* go to beginning of next line */
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
  }
}


Static Void readinword(fin, b)
_TEXT *fin;
buff2 *b;
{
  /* read a line from fin into wordbuffer b */
  long i = 1;   /* index for b.place[i] and counter */
  _TEXT TEMP;

  clearb2(b);   /* clear the buffer */
  /* necessary because of leading-blanks routine */

  /* routine to get rid of leading blanks */
  while ((P_peek(fin->f) == ' ') & (!P_eoln(fin->f)))
    getc(fin->f);
  if (P_eoln(fin->f)) {
    printf(" there are blank lines in worchap.\n");
	/* stop program and notify user */
    halt();
  }

  while ((i <= wdlgthmax) & (!(P_eoln(fin->f) | BUFEOF(fin->f)))) {
    b->place[i-1] = getc(fin->f);   /* read in next character to buffer */
    if (b->place[i-1] == '\n')
      b->place[i-1] = ' ';
    b->length = i;   /* keeping track of length of the buffer */
    i++;   /* next character */
  }
  /* keeping word length below wdlgthmax and going to end of line
   no words are allowed to be longer than wdlgthmax char */



  if (i == wdlgthmax + 1) {
    printf(" oldword is more than allowed characters\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    write2trigger(&TEMP, *b);
    halt();
  }
  /* if the word is longer then wdlgthmax,
               longer than any allowed word, then
               notify the user of the problem, write
               the word, and halt the program */
  if (!BUFEOF(fin->f)) {   /* go to beginning of next line */
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
  }

}


Static Void readintempword(temp, tempword, i, nomatch)
buffer temp;
buff2 *tempword;
long *i;
boolean *nomatch;
{
  /* this will read in a word from . to . in temp
     into tempword. if the word being read is longer than
     wdlgthmax, and therefore cannot match any word in the list,
     then nomatch will indicate this.(saves time) */
  long c = 1;   /* index for tempword */

  clearb2(tempword);   /* clear the buffer */
  *nomatch = false;   /* we do not know yet.. */
  /* start at the beginning of tempword */
  while (temp.place[*i - 1] != '.' && c <= wdlgthmax && *i <= temp.length) {
    tempword->place[c-1] = temp.place[*i - 1];
    tempword->length = c;
    (*i)++;
    c++;
  }
  /* read in the tempword till its end or the end of temp */

  if (c > wdlgthmax && temp.place[*i - 1] != '.') {
    /* if the tempword is longer than any word in the list */
    *nomatch = true;   /* then no match is possible */
  }
}


Static Void skipcomments(commentpasteol, i, b, finline)
boolean *commentpasteol;
long *i;
buffer *b, *finline;
{
  /* translating comments from fin to '.' in temp.  see changewords */
  if (finline->place[*i - 1] != '(' || finline->place[*i] != '*')
	/* move to end of comment assigning '.' to temp */
	  return;
  /* if this current place is the beginning of a comment */
  do {
    b->place[*i - 1] = '.';
    (*i)++;
  } while ((finline->place[*i - 1] != ')' || finline->place[*i - 2] != '*') &&
	   *i != finline->length);
  /* until you get to the end of the comment or
     the end of the line */

  if (finline->place[*i - 1] == ')' && finline->place[*i - 2] == '*')
  {   /* if end of comment */
    b->place[*i - 1] = '.';   /* change the ')' to '.' */
    b->length = *i;   /* keep accurate track of the length of b */
    (*i)++;
    /* leaves us on the next char of finline
                 or finline.length+1 if end of line */
    *commentpasteol = false;
    return;
  }
  if (*i != finline->length ||
      finline->place[*i - 1] == ')' && finline->place[*i - 2] == '*') {
    printf(" skipcomments is ending weird....\n");
    halt();
    return;
  }
  /* if end of line but
                                       not end of comment */
  *commentpasteol = true;
  b->place[*i - 1] = '.';   /* assign the last place */
  b->length = *i;   /* keep track of the length of b */
}


Static Void skip2quotes(quote2pasteol, i, b, finline)
boolean *quote2pasteol;
long *i;
buffer *b, *finline;
{
  /* translating quotes from fin to '.' in temp.  see changewords */
  _TEXT TEMP;

  /*zzzqqq*/
  /* clean halt; */
  if (debug)
    printf("BEGIN skip2quotes (cleanhalt)\n");
  if (finline->place[*i - 1] != '\'')
	/* move to end of quote assigning '.' to temp */
	  return;
  /* if current place is the beginning of
                                a quote */
  if (debug) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writebuffer(&TEMP, *finline);
  }
  if (debug)
    printf("finline.length:%ld\n", finline->length);
  if (debug)
    printf(" place %3ld is a single( was dbl) quote \n", *i);
  do {
    b->place[*i - 1] = '.';
    (*i)++;
    if (debug) {
      printf(" finline.place[%ld] is: \"%c\"\n", *i, finline->place[*i - 1]);
      /*
original:
                  until (finline.place[i]='''') or (i=finline.length);
      */
      /*THE FOLLOWING FIXED THE PROBLEM - it made the next halt CLEAN
also, it is always better to test for > than = for the length!! */
    }
  } while (finline->place[*i - 1] != '\'' && *i < finline->length &&
	   *i != lnlgthmax);
  /* until the end of the comment or the end of the line */
  /*halt; (* dirty *) CONVERTED TO CLEAN*/

  /* the problem is that the logic of the until is wrong.
since the first character is a single quote,
it never finds a second quote and i will never equal
finlinelength.  so it keeps running up memory until
it hits a segment limit. */

  /* this is a hack patch to make the thing work
     correctly if it overshot */
  if (*i > finline->length)
    *i = finline->length;

  if (finline->place[*i - 1] == '\'') {   /* if end of quote */
    b->place[*i - 1] = '.';   /* change the ' to . */
    b->length = *i;   /* keep track of the length of b */
    (*i)++;
    /* leaves us on the next char of finline
               or on finline.length+1 if end of line */
    *quote2pasteol = false;
    /*;writeln(output,'quote2pasteol is false');*/
    return;
  }
  if (finline->place[*i - 1] == '\'' || *i < finline->length) {
    printf(" skip2quotes is ending weird....\n");
    halt();

    return;
  }
  /* end of line but
                                          not end of quote */
  *quote2pasteol = true;
  /*writeln(output,'quote2pasteol is true');*/
  b->place[*i - 1] = '.';   /* assign the last place */
  b->length = *i;   /* keep track of the length of b */
}


Static Void skip1quotes(quote1pasteol, i, b, finline)
boolean *quote1pasteol;
long *i;
buffer *b, *finline;
{
  /* translating quotes from fin to '.' in temp.  see changewords */
  if (finline->place[*i - 1] != '"')
	/* move to end of quote assigning '.' to temp */
	  return;
  /* if current place is the
                                beginning of a quote */
  do {
    b->place[*i - 1] = '.';
    (*i)++;
    /* original:
                until (finline.place[i]='"') or (i=finline.length);
    */
    /*add the following protection, as in skip2quotes*/
  } while (finline->place[*i - 1] != '"' && *i < finline->length &&
	   *i != lnlgthmax);
  /* until end of quote or end of line */

  /* this is a hack patch to make the thing work
     correctly if it overshot */
  if (*i > finline->length)
    *i = finline->length;

  if (finline->place[*i - 1] == '"') {   /* if end of quote */
    b->place[*i - 1] = '.';   /* change the " to . */
    b->length = *i;   /* keep track of the length of b */
    (*i)++;
    /* leaves us on the next char of finline
              or on finline.length+1 if end of line */
    *quote1pasteol = false;
    return;
  }
  if (finline->place[*i - 1] == '"' || *i < finline->length) {
    printf(" skip1quotes is ending weird....\n");
    halt();
    return;
  }
  /* end of line but
                                         not end of quote */
  *quote1pasteol = true;
  b->place[*i - 1] = '.';   /* assign the last place */
  b->length = *i;   /* keep track of the length of b */
}


Static Void endcomment(commentpasteol, i, b, finline)
boolean *commentpasteol;
long *i;
buffer *b, *finline;
{
  /* if the last line had a comment that did not end on that line,
  search this line for the end to that comment.  see changewords and
  skipcomments */
  if (finline->length < 2) {  /* see last else in this proced. */
    *commentpasteol = true;
    /* if filnine is shorter than 2, it cannot
                                  contain the end-of-comment marks and
                                  therefor the comment must not end on
                                  this line */
    return;
  }
  *i = 1;
  b->place[0] = '.';
  /* these two steps are necessary because
                       of the way the loop is constructed below
                       if you do not start on position 2, then
                       the program will bomb because of the
                       testing of finline.place[i-1] which
                       would be out of the range if i=1. */
  do {   /* move to end of comment assigning '.' to temp */
    (*i)++;
    b->place[*i - 1] = '.';
  } while ((finline->place[*i - 1] != ')' || finline->place[*i - 2] != '*') &&
	   *i != finline->length);
  /* until you get to the end of the comment or
     the end of the line */

  /*if debug then writeln(output,' i= ',i);
if debug then writeln(output,' finline.place[i-1]= ',finline.place[i-1]);
if debug then writeln(output,' finline.place[i]= ',finline.place[i]);
if debug then writeln(output,' finline.length= ',finline.length); */

  if (finline->place[*i - 1] == ')' && finline->place[*i - 2] == '*')
  {   /* if end of comment */
    (*i)++;
    /* leaves us on the next char of finline
                 or on finline.length+1 if end of line */
    *commentpasteol = false;
    return;
  }
  if (*i == finline->length &&
      (finline->place[*i - 1] != ')' || finline->place[*i - 2] != '*')) {
    /* if end of line but
                                           not end of comment */
    *commentpasteol = true;
  } else {
    printf(" endcomment is ending weird....\n");
    halt();
  }
}


Static Void end2quote(quote2pasteol, i, b, finline)
boolean *quote2pasteol;
long *i;
buffer *b, *finline;
{
  /* if the last line had a quote that did not end on that line,
  search this line for the end to that quote.  see changewords and
  skip2quotes */
  if (finline->length < 1) {  /* see last else in this proced. */
    *quote2pasteol = true;
    /* if finline.length is less than
                                  1, it cannot contain the end-
                                  of-quote mark and therefor
                                  the quote will not end on this
                                  line */
    return;
  }
  *i = 0;
  do {   /* move to the end of the quote assigning '.' to temp */
    (*i)++;
    b->place[*i - 1] = '.';
  } while (finline->place[*i - 1] != '\'' && *i != finline->length);
  /* until the end of the quote or the end of the line */

  if (finline->place[*i - 1] == '\'') {   /* if end of quote */
    *quote2pasteol = false;
    (*i)++;
    /* leaves us on the next char
                or on finline.length+1 if end of line */
    return;
  }
  if (*i == finline->length && finline->place[*i - 1] != '\'') {
    /* if end of line, but
                                       not end of quote */
    *quote2pasteol = true;
  } else {
    printf(" end2quote is ending weird....\n");
    halt();
  }
}


Static Void end1quote(quote1pasteol, i, b, finline)
boolean *quote1pasteol;
long *i;
buffer *b, *finline;
{
  /* if the last line had a quote that did not end on that line,
  search this line for the end to that quote.  see changewords and
  skip1quotes */
  if (finline->length < 1) {  /* see last else in this proced. */
    *quote1pasteol = true;
    /* if finline.length is less than
                                  1, it cannot contain the end-
                                  of-quote mark and therefor
                                  the quote will not end on this
                                  line */
    return;
  }
  *i = 0;
  do {   /* move to the end of the quote assigning '.' to temp */
    (*i)++;
    b->place[*i - 1] = '.';
  } while (finline->place[*i - 1] != '"' && *i != finline->length);
  /* until the end of the quote or the end of the line */

  if (finline->place[*i - 1] == '"') {   /* if end of quote */
    *quote1pasteol = false;
    (*i)++;
    /* leaves us on the next char or on
                finline.length+1 if end of line */
    return;
  }
  if (*i == finline->length && finline->place[*i - 1] != '"') {
    /* if end of line, but
                                       not end of quote */
    *quote1pasteol = true;
  } else {
    printf(" end1quote is ending weird....\n");
    halt();
  }
}


Static Void assigntemp(finline, b, commentpasteol, quote2pasteol,
		       quote1pasteol)
buffer finline, *b;
boolean *commentpasteol, *quote2pasteol, *quote1pasteol;
{
  /* assign temp.place[i] values.  see changewords */
  long i = 1;   /* index of finline and temp */
  _TEXT TEMP;


  clearb(b);   /* clear the buffer */
  /* start at the beginning */
  if (*commentpasteol)
    endcomment(commentpasteol, &i, b, &finline);
  if (*quote2pasteol)
    end2quote(quote2pasteol, &i, b, &finline);
  if (*quote1pasteol)
    end1quote(quote1pasteol, &i, b, &finline);

  while (i <= finline.length) {
    if (i <= finline.length)
      skipcomments(commentpasteol, &i, b, &finline);
    /*clean halt;*/
    if (i <= finline.length)
      skip2quotes(quote2pasteol, &i, b, &finline);
    /*halt; NEUTRALIZE*/
    /* dirty halt;*/
    if (i <= finline.length)
      skip1quotes(quote1pasteol, &i, b, &finline);

    /* dirty halt;*/

    if (finline.place[i-1] == '\'' || finline.place[i-1] == '"')
      continue;
    /* if a quote ends right away
                                        do not want to do the subst.
                                        below b/c it will mess up
                                        our ability to detect the
                                        end of the quote by advancing
                                        finline.place to the next
                                        position before we check for
                                        the end of the quote. */
    if (i > finline.length)
      break;
    /* only want to continue
                                        if there is anything left */
    /* There was a bug here!  We now need both smalls AND caps! */
    if (isalnum(finline.place[i-1])) {
      /* only want letters and numbers in temp */
      b->place[i-1] = finline.place[i-1];
    } else
      b->place[i-1] = '.';
    b->length = i;   /* keeping track of the length of temp */
    i++;
  }
  /*clean halt;*/
  /* assign finline to temp buffer that will
     only contain the identifiers which are
     not in comments or quotes. everything
     else will be translated into '.' in temp */



  if (debug)
    printf("             |0        1         2         3");
  if (debug)
    printf("         4         5         6\n");
  if (debug)
    printf("             |123456789012345678901234567890");
  if (debug)
    printf("1234567890123456789012345678901234\n");
  if (debug)
    printf(" finline:    |");
  if (debug) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writebuffer(&TEMP, finline);
  }
  if (debug)
    printf(" temp:       |");
  if (debug) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writebuffer(&TEMP, *b);
  }

  if (debug)
    printf(" commentpasteol: %s\n", (*commentpasteol) ? " TRUE" : "FALSE");
  if (debug)
    printf(" quote2pasteol: %s\n", (*quote2pasteol) ? " TRUE" : "FALSE");
  if (debug)
    printf(" quote1pasteol: %s\n", (*quote1pasteol) ? " TRUE" : "FALSE");

}


Static Void assignwords(p)
aword **p;
{
  /* assign the values to the word-records */
  aword *WITH;

  if (BUFEOF(worchap.f)) {
    printf(" unexpected end of worchap encountered.\n");
    halt();
  }
  WITH = *p;
  readinword(&worchap, &WITH->oldword);
  removeblanks(&WITH->oldword);
  if (BUFEOF(worchap.f)) {
    printf(" odd number of words in worchap.\n");
    printf(" must have old-new pairs of words.\n");
    halt();
  }
  /* if no new word for a corresponding old word */
  readinword(&worchap, &WITH->newword);
  removeblanks(&WITH->newword);
  WITH->numbchgs = 0;
  WITH->next = NULL;   /* make sure that all ptr"s initialy are nil */
}


Static boolean afteryou(p)
aword *p;
{
  /* answers the question 'do i belong after you?' or more accurately,
     does p^.old belong after p^.next^.old in this list. */
  boolean Result;
  long n = 1;
  boolean done = false;
  _TEXT TEMP;

  /* start with first letters of words */
  /*
*/
  if (debug) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    write2trigger(&TEMP, p->oldword);
  }
  while (!done) {
    if (p->next == NULL) {   /* if we are at the end of the list */
      Result = false;
      done = true;
      break;
    }
    if (p->oldword.place[n-1] < p->next->oldword.place[n-1]) {
      /* we belong before you */
      Result = false;
      done = true;
      break;
      continue;
    }
    if (p->oldword.place[n-1] > p->next->oldword.place[n-1]) {
      /* we belong after you */
      Result = true;
      done = true;
      break;
      continue;
    }
    if (n == p->oldword.length && n < p->next->oldword.length) {
      /* we match to my length but you are longer
      therefore i belong before you */
      Result = false;
      done = true;
      break;
      continue;
    }
    if (n == p->next->oldword.length && n < p->oldword.length) {
      /* we match to your length but i am longer
      so i belong after you */
      Result = true;
      done = true;
      break;
      continue;
    }
    if (n == p->oldword.length && n == p->next->oldword.length) {
      printf(" word repeated in worchap.\n");
      halt();
    }
    /* we match so notify user and stop */
    else {   /* go to next letter of words */
      done = false;
      n++;
    }
  }
  return Result;
}


Static Void makelist(worchap, first)
_TEXT *worchap;
aword **first;
{
  /* makes an alphabetical linked list of records which contain
     old words and their corresponding newwords and the number
     of changes of the oldwords into their newwords */
  aword *newptr;   /* the ptr to the newly created word record */
  aword *lookptr;   /* a pointer used to look through the list*/
  boolean inlist;   /* wether or not the memb has been inserted */
  long listlength = 1;

  newptr = (aword *)Malloc(sizeof(aword));   /* make first member by hand */
  *first = newptr;
  assignwords(first);
  while (!BUFEOF(worchap->f)) {  /* make rest of list */
/* p2c: worcha.p: Note: Eliminated unused assignment statement [338] */
    newptr = (aword *)Malloc(sizeof(aword));
    assignwords(&newptr);
    listlength++;   /* keep track of list-length */
    lookptr = *first;   /* start at beginning of list */
    newptr->next = *first;
    if (!afteryou(newptr))   /* see 'function afteryou' */
    {  /* insert new member if it belongs as first memb */
      *first = newptr;
      continue;
    }
/* p2c: worcha.p: Note: Eliminated unused assignment statement [338] */
    newptr->next = (*first)->next;   /* move to next memb of list */
    while (afteryou(newptr)) {
      /* move thru list till find right spot for insertion */
      lookptr = lookptr->next;
      newptr->next = newptr->next->next;
    }
    lookptr->next = newptr;   /* insert new member */
/* p2c: worcha.p: Note: Eliminated unused assignment statement [338] */
  }
}


Static boolean wordmatch(aword_, bword)
buff2 aword_, bword;
{
  /* does aword match bword??? */
  boolean Result;
  long n = 1;   /* index of buff2"s */
  long level = 0;   /* matching level */
  boolean fail;   /* indicates mismatch,(speeds up funct.) */

  if (debug)
    printf("BEGIN wordmatch\n");
  if (aword_.length == bword.length) {   /* if not same length no match */
    if (aword_.place[0] == bword.place[0]) {
      /* quick check of the first letters of each, if they do not
         match, then the words do not match.  since the lists are
         in alphabetical order, this will aid in the speed of the
         search of the list */
      level++;   /* indicate match */
      fail = false;   /* has not failed yet ... */
      while (n < aword_.length && !fail) {
	/* check the rest of the word */
	n++;
	if (aword_.place[n-1] == bword.place[n-1])
	  level++;
	else
	  fail = true;
      }
      if (level == aword_.length)   /* if words match */
	Result = true;
      else
	Result = false;
    } else
      Result = false;
  } else
    Result = false;
  if (debug)
    printf("END wordmatch\n");
  return Result;
}


Static aword *matchptr(first, tempword)
aword *first;
buff2 tempword;
{
  /* take tempword (given from proced. changeword) and look
     through the linked list of oldwords looking for a match.
     if a match is found, matchptr will point to the matching
     oldword, and since the newword is on the same record,
     matchptr will also point to the newword. same for numbchgs.
     if no match is found, matchptr will be left pointing to nil */
  aword *Result, *p;   /* pointer to look thru list with */
  boolean done;   /* wether or not finished looking thru list */
  long n = 1;   /* number of memb of the list now looking at */
  _TEXT TEMP;

  if (debug)
    printf("BEGIN matchptr\n");
  p = first;
  if (debug) {
    if (p == NULL)
      printf("p=nil!\n");
    else
      printf("p<>nil\n");
  }
  do {   /* look thru the list */
    if (debug)
      printf("HERE 1 zzz\n");
    if (debug)
      printf("p^.old:\n");
    /* at this call, one never gets to writeword because the pointer p
is pointing out into far space.
This implies that first is bad - not set for example.  */
    if (debug) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeword(&TEMP, p->oldword);
    }
    if (debug)
      printf("HERE 1.5 zzz\n");
    /*halt; NEUTRALIZE*/
    if (!wordmatch(p->oldword, tempword)) {  /* go to next memb of list */
      if (debug)
	printf("HERE 2 zzz\n");
      p = p->next;
      n++;
      done = false;
    } else
      done = true;
    if (debug)
      printf("HERE 3 zzz\n");
    if (p == NULL)   /* if we get to the end of the list */
      done = true;
  } while (!done);
  Result = p;
  if (debug)
    printf("END matchptr\n");
  return Result;
}


#define tablewidth      80   /* maximum width of the table */
#define minwordsize     12   /* minimum size of the word field */


Static Void printlist(first, place)
aword *first;
_TEXT *place;
{
  /* this proced. will print out an alphabetical listing of
     the words, old and new, and the number of times that each
     oldword was replace by its newword, all in a table, with
     totals added at the bottom. */
  aword *p = first;   /* pointer to search through the list */
  long maxwordsize = (long)((tablewidth - 17.0) / 2);
      /* maximum size of the word field */
  long wordsize = 0;   /* size of the word field */
  long numwords = 0;   /* keeps track of the number of words for totals */
  long total = 0;   /* keeps the total number of changes */
  long i;   /* counter */
  boolean shortened = false;   /* true if a word was shortened in the table */

  /* get the wordsize from the list**********************************/
  /* start at the beginning of the list */
  do {
    if (p->oldword.length > wordsize)
      wordsize = p->oldword.length;
    if (p->newword.length > wordsize)
      wordsize = p->newword.length;
    p = p->next;
  } while (p != NULL);

  if (wordsize > maxwordsize)
    wordsize = maxwordsize;
  if (wordsize < minwordsize)
    wordsize = minwordsize;

  /* write the headings of the table ********************************/
  for (i = 1; i <= wordsize * 2 + 13; i++)
    putc(' ', place->f);
  fprintf(place->f, "number\n");
  for (i = 1; i <= wordsize * 2 + 15; i++)
    putc(' ', place->f);
  fprintf(place->f, "of\n");
  for (i = 1; i <= 8; i++)
    putc(' ', place->f);
  fprintf(place->f, "old word");
  for (i = 1; i <= wordsize - 6; i++)
    putc(' ', place->f);
  fprintf(place->f, "new word");
  for (i = 1; i <= wordsize - 5; i++)
    putc(' ', place->f);
  fprintf(place->f, "changes\n");

  /* underline headings of the table ********************************/
  for (i = 1; i <= 8; i++)
    putc(' ', place->f);
  for (i = 1; i <= 8; i++)
    putc('-', place->f);
  for (i = 1; i <= wordsize - 6; i++)
    putc(' ', place->f);
  for (i = 1; i <= 8; i++)
    putc('-', place->f);
  for (i = 1; i <= wordsize - 5; i++)
    putc(' ', place->f);
  for (i = 1; i <= 7; i++)
    putc('-', place->f);
  fprintf(place->f, "\n\n");

  /* write the contents of the table ********************************/
  p = first;   /* start at the beginning of the list again */
  while (p != NULL) {
    for (i = 1; i <= 8; i++)   /* initial spaces */
      putc(' ', place->f);
    if (p->oldword.length <= wordsize) {  /* word will fit so write word */
      writeword(place, p->oldword);
      for (i = p->oldword.length + 1; i <= wordsize + 1; i++)
	putc(' ', place->f);
    } else {
      /* word will not fit so shorten and write with a / at the end */
      for (i = 0; i < wordsize; i++)
	putc(p->oldword.place[i], place->f);
      putc('/', place->f);
      shortened = true;
    }
    putc(' ', place->f);   /* word separator blank */
    if (p->newword.length <= wordsize) {  /* word will fit so write word */
      writeword(place, p->newword);
      for (i = p->newword.length + 1; i <= wordsize + 1; i++)
	putc(' ', place->f);
    } else {
      /* word will not fit so shorten and write with a / at the end */
      for (i = 0; i < wordsize; i++)
	putc(p->newword.place[i], place->f);
      putc('/', place->f);
      shortened = true;
    }
    fprintf(place->f, "  ");   /* word-numbchgs separator blanks */
    fprintf(place->f, "%4ld\n", p->numbchgs);
    total += p->numbchgs;   /* keep track of total of numbchgs */
    numwords++;   /* keep track of total number of words */
    p = p->next;   /* point to next memb of list */
  }

  /* underline end of table for totals ******************************/
  for (i = 1; i <= 8; i++)   /* first spaces */
    putc(' ', place->f);
  for (i = 1; i <= wordsize * 2 + 9; i++)
    putc('-', place->f);
  fprintf(place->f, "\n totals: ");

  /* write the totals at the bottom of the table ********************/
  fprintf(place->f, "%5ld", numwords);
  for (i = 1; i <= wordsize - 3; i++)
    putc(' ', place->f);
  fprintf(place->f, "%5ld", numwords);
  for (i = 1; i <= wordsize - 3; i++)
    putc(' ', place->f);
  fprintf(place->f, "%4ld\n", total);
  if (shortened) {
    fprintf(place->f, "\n a \"/\" indicates the word was shortened for");
    fprintf(place->f, " printing in the table.\n");
  }
}  /* printlist */

#undef tablewidth
#undef minwordsize


Static Void changewords(fin, fout)
_TEXT *fin, *fout;
{
  /* this is what does all of the changing of words.  it uses all
  of the previous proced and funct. */
  aword *p;   /* to use the matchptr function value */
  aword *first;
  /* pointer to the first memb of a linked list
                       of the old and new words from worchap */
  buffer temp;
  /* the buffer made from finline that will hold only
                       pascal "words" */
  buffer finline;   /* a line from file fin */
  buff2 tempword;
  /* the current word we are looking at in
                       the temp buffer and the word we are using
                       to look for a match with the oldwords */
  long i = 1;   /* index of buffers */
  long c;   /* counter for for loop */
  long numline = 0;   /* number of the input line */
  long oldbeg = 0;
  /* the place of the beginning of the oldword
                       in finline */

  boolean commentpasteol = false;
  /* whether or not the comment runs past
                       the end of the line */
  boolean quote1pasteol = false;
  /* whether or not the quote runs past the
                       end of the line */
  boolean quote2pasteol = false;
  /* whether or not the quote runs past
                       the end of the line */
  boolean nomatch;   /* can there be a match?  see proced. readinwords */
  _TEXT TEMP1;


  /*zzz*/
  if (debug)
    printf("here is where the first is generated:\n");
  makelist(&worchap, &first);
  if (debug) {
    printf("past makelist\n");
    putchar('"');
    TEMP1.f = stdout;
    *TEMP1.name = '\0';
    writeword(&TEMP1, first->oldword);
    printf("\"\n");
    putchar('"');
    TEMP1.f = stdout;
    *TEMP1.name = '\0';
    writeword(&TEMP1, first->newword);
    printf("\"\n");
    printf("%ld\n", first->numbchgs);
  }
  /* if halt; here, clean ending => problem is below */

  while (!BUFEOF(fin->f)) {
    readintobuffer(fin, &finline);   /* pick up a line of fin */
    /* if halt; here, clean ending => problem is below */

    numline++;   /* keeping track of finline number */
    if (debug)
      putchar('\n');
    if (debug)
      putchar('\n');
    if (debug)
      printf(" numline is:%5ld\n", numline);

    /* if halt; here, clean ending => problem is below */
    assigntemp(finline, &temp, &commentpasteol, &quote2pasteol,
	       &quote1pasteol);
    /* if halt; is here it ends clean but gives bus error! */

    /* go through temp and fin writing to fout and
       replacing oldwords encountered with their
       newwords */

    i = 1;
    while (i <= finline.length) {  /* search line */
      /*
*/
      if (debug)
	printf(" searching line %5ld\n", numline);
      /* if halt; is here it ends clean but gives bus error! */

      if (debug)
	printf("finline.length=%ld\n", finline.length);
      if (temp.place[i-1] == '.') {  /* skip dots */
	if (debug)
	  printf("skipping dots\n");
	do {
	  if (debug)
	    printf("i=%ld\n", i);
	  if (debug)
	    printf("finline.place[i]=\"%c\"\n", finline.place[i-1]);
	  /*zzz*/
	  if (debug)
	    printf("HERE 4\n");
	  if (debug)
	    putchar(finline.place[i-1]);
	  /*halt; (* dirty *) NEUTRALIZE*/
	  putc(finline.place[i-1], fout->f);
	  if (debug)
	    printf("HERE 5\n");
	  i++;
	  if (debug)
	    printf("HERE 6\n");
	} while (temp.place[i-1] == '.' && i != finline.length + 1);
	/*halt; (* dirty *) NEUTRALIZE*/
	if (debug)
	  printf("HERE 7\n");
      }

      if (debug)
	printf("HERE 8\n");
      /*halt; (* dirty *) NEUTRALIZE*/

      if (i > temp.length)  /* found a word */
	continue;

      /*
*/
      if (debug)
	printf(" found a word at position %12ld\n", i);
      oldbeg = i;
      readintempword(temp, &tempword, &i, &nomatch);
      if (debug)
	printf("passed readintempword\n");

      if (!nomatch) {   /* a match is not impossible */
	if (debug)
	  printf("about to matchptr\n");
	p = matchptr(first, tempword);
	if (debug)
	  printf("passed matchptr\n");
	if (p != NULL) {  /* found a match */
	  /*
*/
	  if (debug)
	    printf(" found a match: ");
	  if (debug) {
	    TEMP1.f = stdout;
	    *TEMP1.name = '\0';
	    writeword(&TEMP1, p->oldword);
	  }
	  if (debug)
	    putchar('\n');
	  writeword(fout, p->newword);
	  p->numbchgs++;
	  continue;
	}  /* we are at the '.' after tempword in temp */


	/*
*/
	if (debug)
	  printf(" looked and no match found.\n");
	if (i <= temp.length) {
	  for (c = oldbeg - 1; c <= i - 2; c++) {
	    putc(finline.place[c], fout->f);
	    /* no substitution so write to fout up to after
	       end of tempword */
	  }
	} else {
	  for (c = oldbeg - 1; c < temp.length; c++) {
	    putc(finline.place[c], fout->f);
	    /* no substitution and i is temp.length+1 probably
	       from readintempword going to the end of temp */
	  }
	}
	continue;
      }
      /* did not find a match */
      /*
*/
      if (debug)
	printf(" no match is possible with this word.\n");
      i = oldbeg;
      while (temp.place[i-1] != '.' && i <= finline.length) {
	putc(finline.place[i-1], fout->f);
	i++;
      }

    }
    if (debug)
      printf(" line number: %12ld transfered to fout.\n", numline);
    putc('\n', fout->f);
  }

  if (listonfout) {  /* show user list in fout */
    for (i = 1; i <= 5; i++)   /* spacing before list */
      putc('\n', fout->f);
    fprintf(fout->f, "(*\n");   /* putting list in comment */
    printlist(first, fout);   /* write out list */
    fprintf(fout->f, "*)\n");   /* putting list in comment */
  }

  if (!listonoutput)  /* show user list on screen */
    return;
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  printlist(first, &TEMP1);
  printf("\n p.s.  fout also has this list at the end.\n");

  printf("       the list is within comments so the\n");
  printf("       program will compile.\n");
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  worchap.f = NULL;
  strcpy(worchap.name, "worchap");
  fout.f = NULL;
  strcpy(fout.name, "fout");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  printf(" worcha %4.2f\n", version);
  if (*fin.name != '\0') {
    if (fin.f != NULL)
      fin.f = freopen(fin.name, "r", fin.f);
    else
      fin.f = fopen(fin.name, "r");
  } else
    rewind(fin.f);
  if (fin.f == NULL)
    _EscIO2(FileNotFound, fin.name);
  RESETBUF(fin.f, Char);
  if (*worchap.name != '\0')
    worchap.f = fopen(worchap.name, "r");
  else
    rewind(worchap.f);
  if (worchap.f == NULL)
    _EscIO2(FileNotFound, worchap.name);
  RESETBUF(worchap.f, Char);
  if (*fout.name != '\0') {
    if (fout.f != NULL)
      fout.f = freopen(fout.name, "w", fout.f);
    else
      fout.f = fopen(fout.name, "w");
  } else {
    if (fout.f != NULL)
      rewind(fout.f);
    else
      fout.f = tmpfile();
  }
  if (fout.f == NULL)
    _EscIO2(FileNotFound, fout.name);
  SETUPBUF(fout.f, Char);

  changewords(&fin, &fout);

  /* makelist(worchap,first);  */
  /* printlist(first,output);   */

_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  if (worchap.f != NULL)
    fclose(worchap.f);
  exit(EXIT_SUCCESS);
}  /* worcha */



/* End. */
