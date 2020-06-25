/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "show.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         3.10
/*



*/



/*





































































*/


/*







*/



#define period          '.'

#define defaultpagelength  48
#define defranbeg       1
#define defranend       10

#define lastcharacter   ' '


#define maxstring       2000


#define maxname         50

/*

*/
#define checkuptimes    2

#define debugging       true



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;




typedef struct treecat {
  stringDelila name;
  long linenumber;
  /*

*/
  struct treecat *down, *up, *next;
} treecat;

typedef struct name {
  Char letter[maxname];
  long length;
} name;

typedef struct trigger {
  name n;
  long state;
  boolean skip, found;
} trigger;

typedef struct modcatitem {
  name module;
  long line;
} modcatitem;

typedef struct modcatfile {
  FILE *f;
  FILEBUFNC(f,modcatitem);
  Char name[_FNSIZE];
} modcatfile;

typedef long answers[5];


Static _TEXT modlib, print;
Static boolean printingstarted;
/*


*/

Static modcatfile modcat;

Static treecat *root, *current, *last, *view;

Static stringDelila buffer, command, parent, lastparent;

Static long linenumber, shownext, printnext, pagelength;

Static Char begcom;

Static boolean showlinenumbers;
/*
*/
Static boolean gotten, haveperiod, error;


Static trigger begintrigger, endtrigger;


Static long rangebegin, rangeend;

/*




*/
Static boolean donthalt;
/*
*/
Static boolean haltcalled;


Static jmp_buf _JL1;








Static Void halt()
{
  if (donthalt)
    haltcalled = true;
  else {
    printf(" program halt.\n");
    longjmp(_JL1, 1);
  }
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



Static Void readstring(afile, line)
_TEXT *afile;
stringDelila *line;
{
  long index;
  Char cha;
  boolean done, acceptable;

  do {
    clearstring(line);
    prompt(afile);
    index = 0;
    while (!P_eoln(afile->f) && index < maxstring) {
      index++;
      readchar(afile, &cha);
      line->letters[index-1] = cha;
    }
    if (!P_eoln(afile->f)) {
      printf("type lines shorter than %ld characters.  please retype the line...\n",
	     maxstring + 1L);
      acceptable = false;
    } else
      acceptable = true;
  } while (!acceptable);

  line->length = index;
  if (line->length > 0) {
    done = false;
    do {
      /*

*/
      if (line->letters[line->length - 1] == ' ')
	line->length--;
      else
	done = true;

      if (line->length == 0)
	done = true;
    } while (!done);
  }

  if (line->length > 0)
    line->current = 1;
  else
    line->current = 0;
}


/* Local variables for figurestring: */
struct LOC_figurestring {
  stringDelila *line;
  long power;
} ;

Local long figureinteger(first, last, LINK)
long first, last;
struct LOC_figurestring *LINK;
{
  long i;
  long sum = 0;
  long increment;

  LINK->power = 1;
  for (i = last - 1; i >= first - 1; i--) {
    switch (LINK->line->letters[i]) {

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
    sum += LINK->power * increment;
    LINK->power *= 10;
  }
  return sum;
}



Static Void figurestring(line_, first, last, whzat, c, i, r)
stringDelila *line_;
long *first, *last;
Char *whzat, *c;
long *i;
double *r;
{
  /*













*/
  struct LOC_figurestring V;
  long numbers[3];
  long sign, numberstart;
  /*
*/
  long point = 0;

  long l;
  stringDelila *WITH;
  long FORLIM;


  V.line = line_;
  P_addset(P_expset(numbers, 0L), '0');
  P_addset(numbers, '1');
  P_addset(numbers, '2');
  P_addset(numbers, '3');
  P_addset(numbers, '4');
  P_addset(numbers, '5');
  P_addset(numbers, '6');
  P_addset(numbers, '7');
  P_addset(numbers, '8');
  P_addset(numbers, '9');
  *whzat = '.';


  WITH = V.line;

  if (WITH->length == 0 || WITH->current < 1 || WITH->current > WITH->length)
    *whzat = ' ';
  else {
    *first = V.line->current;
    while (V.line->letters[*first - 1] == ' ' && *first < V.line->length)
      (*first)++;
    if (*first == V.line->length && V.line->letters[*first - 1] == ' ')
      *whzat = ' ';
  }

  if (*whzat == ' ')
    return;
  *last = *first;
  while (V.line->letters[*last - 1] != ' ' && *last < V.line->length)
    (*last)++;
  if (V.line->letters[*last - 1] == ' ')
    (*last)--;



  *c = V.line->letters[*first - 1];
  if (P_inset(*c, numbers) || *c == '-' || *c == '+') {
    if (*c == '-' || *c == '+') {
      switch (*c) {

      case '+':
	sign = 1;
	break;

      case '-':
	sign = -1;
	break;
      }
      numberstart = *first + 1;
    } else {
      sign = 1;
      numberstart = *first;
    }

    *whzat = 'i';
    FORLIM = *last;
    for (l = numberstart; l <= FORLIM; l++) {
      if (!P_inset(V.line->letters[l-1], numbers)) {
	if (V.line->letters[l-1] == '.') {
	  if (*whzat == 'i') {
	    *whzat = 'r';
	    point = l;
	  } else
	    *whzat = 'g';
	} else
	  *whzat = 'g';
      }
    }




    if (*whzat == 'r' && point == *last)
      *whzat = 'i';

    if (*whzat == 'i') {
      if (point == *last)
	*i = sign * figureinteger(numberstart, *last - 1, &V);
      else
	*i = sign * figureinteger(numberstart, *last, &V);
      *r = *i;
    } else if (*whzat == 'r') {
      *i = figureinteger(numberstart, point - 1, &V);
      *r = sign * (*i + (double)figureinteger(point + 1, *last, &V) / V.power);
      *i *= sign;
    }
  } else
    *whzat = 'c';


  V.line->current = *last + 1;
}


/* Local variables for nostring: */
struct LOC_nostring {
  stringDelila *buffer;
  boolean answer;
} ;

Local Void kill(LINK)
struct LOC_nostring *LINK;
{
  LINK->answer = true;
  clearstring(LINK->buffer);
}









Static boolean nostring(buffer_)
stringDelila *buffer_;
{
  /*

*/
  struct LOC_nostring V;
  stringDelila *WITH;

  V.buffer = buffer_;
  WITH = V.buffer;
  if (WITH->length <= 0) {
    kill(&V);
    return V.answer;
  }
  if (WITH->length < maxstring) {
    while (WITH->letters[WITH->current - 1] == ' ' &&
	   WITH->current < WITH->length)
      WITH->current++;
  }

  if (WITH->current > maxstring) {
    kill(&V);
    return V.answer;
  }
  if (WITH->letters[WITH->current - 1] == ' ')
    kill(&V);
  else
    V.answer = false;
  return V.answer;
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



Static Void flagstring(afile, buffer)
_TEXT *afile;
stringDelila *buffer;
{
  buffer->length = buffer->current;
  writestring(afile, buffer);
  fprintf(afile->f, "? ");
  clearstring(buffer);
}



Static Void getchar_(afile, buffer, cha, gotten)
_TEXT *afile;
stringDelila *buffer;
Char *cha;
boolean *gotten;
{
  /*

*/
  long first, last;
  Char what;
  long int_;
  double rea;

  if (buffer->length == 0) {
    *gotten = false;
    readstring(afile, buffer);
  } else {
    figurestring(buffer, &first, &last, &what, cha, &int_, &rea);
    *gotten = (what != ' ');
  }
}



Static Void getinteger(afile, buffer, int_, gotten)
_TEXT *afile;
stringDelila *buffer;
long *int_;
boolean *gotten;
{
  long first, last;
  Char what, cha;
  double rea;
  _TEXT TEMP;

  if (buffer->length == 0) {
    *gotten = false;
    readstring(afile, buffer);
    return;
  }
  figurestring(buffer, &first, &last, &what, &cha, int_, &rea);

  if (what == 'i') {
    *gotten = true;
    return;
  }
  TEMP.f = stdout;
  *TEMP.name = '\0';
  flagstring(&TEMP, buffer);
  printf(" please type an integer\n");
  *gotten = false;
}



Static Void getreal(afile, buffer, rea, gotten)
_TEXT *afile;
stringDelila *buffer;
double *rea;
boolean *gotten;
{
  /*
*/
  long first, last;
  Char what, cha;
  long int_;
  _TEXT TEMP;

  if (buffer->length == 0) {
    *gotten = false;
    readstring(afile, buffer);
  } else {
    figurestring(buffer, &first, &last, &what, &cha, &int_, rea);

    if (what != 'i' && what != 'r') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      flagstring(&TEMP, buffer);
      printf(" please type a real number\n");
      *gotten = false;
    } else
      *gotten = true;
  }


  if (what == 'i')
    *rea = int_;
}



Static Void token(buffer, atoken, gotten)
stringDelila *buffer, *atoken;
boolean *gotten;
{
  long first, last;
  Char what, cha;
  long int_;
  double rea;
  long index;

  figurestring(buffer, &first, &last, &what, &cha, &int_, &rea);

  if (what == ' ') {
    *gotten = false;
    return;
  }
  clearstring(atoken);
  for (index = first; index <= last; index++)
    atoken->letters[index - first] = buffer->letters[index-1];
  atoken->length = last - first + 1;
  atoken->current = 1;

  *gotten = true;
}



Static Void gettoken(afile, buffer, atoken, gotten)
_TEXT *afile;
stringDelila *buffer, *atoken;
boolean *gotten;
{
  if (buffer->length == 0) {
    *gotten = false;
    readstring(afile, buffer);
  } else
    token(buffer, atoken, gotten);
}






Static boolean equalstring(a, b)
stringDelila a, b;
{
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
  long i;

  clearstring(b);
  for (i = 0; i < a.length; i++)
    b->letters[i] = a.letters[i];
  b->length = a.length;
  b->current = a.current;
}


Static Void removeperiods(s)
stringDelila *s;
{
  long index, FORLIM;

  FORLIM = s->length;
  for (index = 0; index < FORLIM; index++) {
    if (s->letters[index] == period)
      s->letters[index] = ' ';
  }
}


Static Void pushstring(newtoken, parent, lastparent)
stringDelila newtoken, *parent, *lastparent;
{
  /*
*/
  long index, final;

  final = parent->length + newtoken.length + 1;
  if (final <= 0)
    return;


  if (final > maxstring) {
    printf("in procedure pushstring: maxstring exceeded\n");
    halt();
  }
  copystring(*parent, lastparent);
  for (index = 0; index < newtoken.length; index++)
    parent->letters[index + parent->length] = newtoken.letters[index];
  parent->length = final;
  parent->letters[parent->length - 1] = period;
}


Static Void popstring(parent, lastparent)
stringDelila *parent, *lastparent;
{
  boolean done = false;
  _TEXT TEMP;

  copystring(*parent, lastparent);
  if (parent->length <= 0) {
    clearstring(parent);
    clearstring(lastparent);


    return;
  }
  if (parent->letters[parent->length - 1] != '.') {
    printf("error in popstring: parent does not end with \"%c\"\n", period);
    printf(" parent string was \"");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, parent);
    printf("\"\n");
    halt();
  }


  parent->length--;

  do {
    if (parent->length > 0) {
      if (parent->letters[parent->length - 1] == period)
	done = true;
      else
	parent->length--;
    } else
      done = true;
  } while (!done);
}





Static boolean needhelp(buffer)
stringDelila *buffer;
{
  Char c;
  boolean gotten, answer;
  _TEXT TEMP;

  printf("error: ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  flagstring(&TEMP, buffer);
  do {
    if (nostring(buffer))
      printf(" Do you need help(y/n)\n");
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getchar_(&TEMP, buffer, &c, &gotten);
  } while (!gotten);
  answer = (c == 'y');
  if (!answer)
    printf("help aborted\n");
  return answer;
}


Static boolean more(a, c)
long *a;
Char *c;
{
  Char l;
  _TEXT TEMP;

  do {
    if (nostring(&buffer)) {
      printf("MORE?\n");
      TEMP.f = stdin;
      *TEMP.name = '\0';
      getchar_(&TEMP, &buffer, c, &gotten);

      if (!nostring(&buffer)) {
	TEMP.f = stdin;
	*TEMP.name = '\0';
	getchar_(&TEMP, &buffer, c, &gotten);
      }
    } else {
      TEMP.f = stdin;
      *TEMP.name = '\0';
      getchar_(&TEMP, &buffer, c, &gotten);
    }


    if (!gotten)
      *c = 'y';

    if (!P_inset(*c, a)) {
      clearstring(&buffer);
      printf("'more?' means 'do you want more of this?'\n");
      printf(" type one of:");
      for (l = 'a'; l <= 'z'; l++) {
	if (P_inset(l, a))
	  printf(" '%c'", l);
      }
      printf("; an empty line is the same as 'yes'\n");
    }
  } while (!P_inset(*c, a));
  return (*c == 'y');
}


Static Void gohelp(t)
_TEXT *t;
{
  Char c;
  long SET[5];
  long SET1[5];
  long SET2[5];

  fprintf(t->f, "show commands are single letters without periods:\n");
  fprintf(t->f, "(you may type the whole command word if you want.)\n\n");
  fprintf(t->f, "f(ile), the next module is printed to the print file.\n");
  fprintf(t->f, "h(elp)  this list.\n");
  fprintf(t->f, "l(ast)  position is jumped to.\n");
  fprintf(t->f,
	  "n(ext)  module is shown.  Typing n lets you see the next page.\n");
  fprintf(t->f, "        You can also just type a carriage return\n");
  fprintf(t->f,
    "p(age)  size adjustment for showing modules.  (currently it is %ld).\n",
    pagelength);
  fprintf(t->f, "q(uit)  the program.\n");
  fprintf(t->f,
    "r(ange) of the name-list numbers displayed (currently it is %ld to %ld).\n",
    rangebegin, rangeend);
  fprintf(t->f,
	  "        if you see \"...\" it means there are modules above\n");
  fprintf(t->f,
	  "        or below those listed; reset the range to see them\n");
  fprintf(t->f, "s(how)  the next module instead of entering it.\n");
  fprintf(t->f, "        this is necessary for modules like \"*name.\". \n");
  fprintf(t->f, "        to print such a module type \"file show name.\"\n");
  fprintf(t->f, "t(op)   of the tree is jumped to.\n");
  fprintf(t->f, "u(p)    one name level.\n");
  fprintf(t->f, "w(here) are the modules?  a toggle switch to show\n");
  fprintf(t->f, "        the module line numbers.  currently: ");
  if (showlinenumbers)
    fprintf(t->f, "showing.\n");
  else
    fprintf(t->f, "not showing.\n");
  P_addset(P_expset(SET, 0L), 'y');
  P_addset(SET, 'n');

  if (!more(P_addset(SET, 'q'), &c))
    return;
  fprintf(t->f,
	  "The show program allows you to look at modules in a module\n");
  fprintf(t->f, "library.  Modules are named with several parts, as in\n");
  fprintf(t->f,
	  "\"describe.show\".  These are called name-parts.  One does\n");
  fprintf(t->f,
    "not type the full module name all at once (unless one wants to),\n");
  fprintf(t->f,
    "usually one types one name-part at a time.  Show lists all those\n");
  fprintf(t->f, "name-parts that you can choose from.  You must type\n");
  fprintf(t->f,
	  "a period in the name or at one end to indicate a name-part.\n");
  fprintf(t->f,
    "(Otherwise show will assume you mean one of the commands above.)\n\n");
  fprintf(t->f,
	  "If the name has no further name-parts then the module is shown.\n");
  fprintf(t->f, "The list includes numbers that you can type instead of the ");
  fprintf(t->f, "name-parts.\n\n");
  fprintf(t->f,
	  "Several commands can be typed on one line, separated by spaces.\n");
  P_addset(P_expset(SET1, 0L), 'y');
  P_addset(SET1, 'n');
  if (!more(P_addset(SET1, 'q'), &c))
    return;
  fprintf(t->f,
	  "The listing of module name-parts begins with the parent name.\n");
  fprintf(t->f,
	  "You would type the parent name to get to your current place\n");
  fprintf(t->f, "in the tree from the top of the tree (top means root,\n");
  fprintf(t->f,
	  "the tree is \"upside-down\", the leaves are in the earth...).\n");
  fprintf(t->f, "at the top of the tree, the parent name is blank.\n\n");
  fprintf(t->f, "On the listing of module name-parts are:\n");
  fprintf(t->f, "a number: this is a short hand way to state the name.\n");
  fprintf(t->f,
	  "[number]: the line number of the module.  See the w command.\n");
  fprintf(t->f, "*       : one can print these modules\n");
  fprintf(t->f, "name    : a part (or sometimes all) of the module name.\n");
  fprintf(t->f,
	  ".       : a period means that the module name has several parts\n");
  fprintf(t->f,
	  "          separated by periods.  It can be entered by typing\n");
  fprintf(t->f,
    "          \"name.\" or \".name\" (without quotes) or the number.\n\n");
  fprintf(t->f, "In the example below, typing \"1\" is the same as typing\n");
  fprintf(t->f, "\"son.\" and will display the module called parent.son\n");
  fprintf(t->f,
    "typing \"2\" or \"daughter.\" makes one go into the other module.\n");
  fprintf(t->f, "module parent.son begins at line 256.\n");
  fprintf(t->f, "   parent\n");
  fprintf(t->f, "    1 [ 256] * son\n");
  fprintf(t->f, "    2 [    ]   daughter.\n");
  P_addset(P_expset(SET2, 0L), 'y');
  P_addset(SET2, 'n');
  more(P_addset(SET2, 'q'), &c);
}


Static Void goprinthelp(t)
_TEXT *t;
{
  fprintf(t->f, "commands for printing\n");
  fprintf(t->f,
	  "h(elp)  this list (additional help is at show command level)\n");
  fprintf(t->f, "n(o)    more printing\n");
  fprintf(t->f,
    "p(age)  size adjustment for showing modules.  (currently it is %ld)\n",
    pagelength);
  fprintf(t->f, "q(uit)  printing\n");
  fprintf(t->f, "y(es)   continue printing the module\n");
  fprintf(t->f, "(blank) lines cause more printing\n");
}






Static Void gettoline(line, f, current)
long line;
_TEXT *f;
long *current;
{
  /*
*/
  if (*current > line) {
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
    *current = 1;
  }

  while ((*current < line) & (!BUFEOF(f->f))) {
    (*current)++;
    fscanf(f->f, "%*[^\n]");
    getc(f->f);
  }

  if (BUFEOF(f->f)) {
    printf(" modcat refers to a line (%ld) that is past the end of modlib.\n",
	   line);
    halt();
  }
}



Static Void clearname(n)
name *n;
{
  long l;

  for (l = 0; l < maxname; l++)
    n->letter[l] = ' ';
  n->length = 0;
}


Static Void printname(f, n)
_TEXT *f;
name n;
{
/* p2c: show.p, line 991: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  fprintf(f->f, "%.*s", (int)n.length, n.letter);
}


Static Void untrail(n)
name *n;
{
  /*
*/
  _TEXT TEMP;

  n->length = maxname;
  while (n->length > 0 && n->letter[n->length - 1] == ' ')
    n->length--;


  n->length++;

  if (n->length <= maxname) {
    n->letter[n->length - 1] = lastcharacter;
    return;
  }
  n->length = maxname;
  printf(" this name was found: \"");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  printname(&TEMP, *n);
  printf("\".\n");
  printf(" names must be one character shorter than %ld characters.\n",
	 (long)maxname);
  halt();
}


Static Void getname(source, n)
_TEXT *source;
name *n;
{
  /*
*/
  Char ch = '.';
  _TEXT TEMP;

  clearname(n);
  while (!P_eoln(source->f) && ch != lastcharacter && n->length <= maxname) {
    n->length++;
    ch = getc(source->f);
    if (ch == '\n')
      ch = ' ';
    n->letter[n->length - 1] = ch;
  }
  if (n->letter[n->length - 1] == lastcharacter)
    return;
  printf(" this module name: \n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  printname(&TEMP, *n);
  if (n->length == maxname) {
    printf(" is too long (>%ld characters)\n", maxname - 1L);

  } else
    printf(" did not end with a \"%c\".\n", lastcharacter);
  halt();
}


Static boolean equalname(a, b)
name a, b;
{
  if (a.length == b.length)
    return (strncmp(a.letter, b.letter, maxname) == 0);
  else
    return false;
}



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
  if (t->n.letter[t->state - 1] == ch) {
    t->skip = false;
    if (t->state == t->n.length)
      t->found = true;
    else
      t->found = false;
    return;
  }
  t->state = 0;
  t->skip = true;
  t->found = false;
}


Static Void findmoduleend(sin, module, sinline)
_TEXT *sin;
name module;
long *sinline;
{
  /*
*/
  boolean found = false;
  Char ch;
  name endname;
  _TEXT TEMP;

  while ((!found) & (!BUFEOF(sin->f))) {
    while ((!BUFEOF(modlib.f)) & (P_peek(modlib.f) != '(')) {
      fscanf(modlib.f, "%*[^\n]");
      getc(modlib.f);
      (*sinline)++;
    }
    if (BUFEOF(modlib.f))
      continue;
    resettrigger(&endtrigger);
    while (!(P_eoln(sin->f) || endtrigger.skip || endtrigger.found)) {
      ch = getc(sin->f);
      if (ch == '\n')
	ch = ' ';
      testfortrigger(ch, &endtrigger);
    }

    if (endtrigger.found) {
      getname(sin, &endname);
      if (equalname(endname, module))
	found = true;
    }


    fscanf(sin->f, "%*[^\n]");
    getc(sin->f);
    (*sinline)++;
  }


  if (!BUFEOF(sin->f) || found)
    return;
  printf(" no end to module ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  printname(&TEMP, module);
  printf(" whose contents were being skipped.\n");
  halt();
}







Static Void grab(f, item)
modcatfile *f;
modcatitem *item;
{
  fread(item, sizeof(modcatitem), 1, f->f);
}


Static Void drop(t, item)
modcatfile *t;
modcatitem *item;
{
  fwrite(item, sizeof(modcatitem), 1, t->f);
}


Static boolean inmodcat(modcat, module, line)
modcatfile *modcat;
name module;
long *line;
{
  /*


*/
  boolean Result;
  modcatitem n;
  boolean found = false;

  if (*modcat->name != '\0') {
    if (modcat->f != NULL)
      modcat->f = freopen(modcat->name, "rb", modcat->f);
    else
      modcat->f = fopen(modcat->name, "rb");
  } else
    rewind(modcat->f);
  if (modcat->f == NULL)
    _EscIO2(FileNotFound, modcat->name);
  RESETBUF(modcat->f, modcatitem);

  while (!(BUFEOF(modcat->f) || found)) {
    grab(modcat, &n);
    if (equalname(n.module, module))
      found = true;
  }

  if (found) {
    Result = true;
    *line = n.line;
  } else {
    Result = false;
    *line = -1;
  }
  return Result;
}



Static Void checkup(modlib, modcat)
_TEXT *modlib;
modcatfile *modcat;
{
  /*

*/
  long times = 0;
  boolean fail = false;
  modcatitem cat;
  Char ch;
  name libname;
  long modlibline = 1;
  long modcatline;
  _TEXT TEMP;

  printf(" check modlib-modcat correspondence:\n");
  if (*modlib->name != '\0') {
    if (modlib->f != NULL)
      modlib->f = freopen(modlib->name, "r", modlib->f);
    else
      modlib->f = fopen(modlib->name, "r");
  } else
    rewind(modlib->f);
  if (modlib->f == NULL)
    _EscIO2(FileNotFound, modlib->name);
  RESETBUF(modlib->f, Char);
  if (*modcat->name != '\0') {
    if (modcat->f != NULL)
      modcat->f = freopen(modcat->name, "rb", modcat->f);
    else
      modcat->f = fopen(modcat->name, "rb");
  } else
    rewind(modcat->f);
  if (modcat->f == NULL)
    _EscIO2(FileNotFound, modcat->name);
  RESETBUF(modcat->f, modcatitem);

  /*
*/
  donthalt = true;
  do {

    grab(modcat, &cat);


    gettoline(cat.line, modlib, &modlibline);

    if (haltcalled) {
      fail = true;
      haltcalled = false;
    }

    else {
      resettrigger(&begintrigger);

      while (!(P_eoln(modlib->f) || begintrigger.found || begintrigger.skip)) {
	ch = getc(modlib->f);
	if (ch == '\n')
	  ch = ' ';
	testfortrigger(ch, &begintrigger);
      }

      if (begintrigger.skip | P_eoln(modlib->f))
	fail = true;
      else {
	getname(modlib, &libname);
	if (!equalname(libname, cat.module))
	  fail = true;
      }

      times++;
    }
  } while (!(((times >= checkuptimes) | BUFEOF(modcat->f)) || fail));

  donthalt = false;

  /*
*/
  if (!fail) {
    if (*modlib->name != '\0') {
      if (modlib->f != NULL)
	modlib->f = freopen(modlib->name, "r", modlib->f);
      else
	modlib->f = fopen(modlib->name, "r");
    } else
      rewind(modlib->f);
    if (modlib->f == NULL)
      _EscIO2(FileNotFound, modlib->name);
    RESETBUF(modlib->f, Char);
    if (*modcat->name != '\0') {
      if (modcat->f != NULL)
	modcat->f = freopen(modcat->name, "rb", modcat->f);
      else
	modcat->f = fopen(modcat->name, "rb");
    } else
      rewind(modcat->f);
    if (modcat->f == NULL)
      _EscIO2(FileNotFound, modcat->name);
    RESETBUF(modcat->f, modcatitem);
    modlibline = 1;
    times = 0;

    do {
      resettrigger(&begintrigger);
      while (!(P_eoln(modlib->f) || begintrigger.skip || begintrigger.found)) {
	ch = getc(modlib->f);
	if (ch == '\n')
	  ch = ' ';
	testfortrigger(ch, &begintrigger);
      }

      if (begintrigger.found) {
	times++;
	getname(modlib, &libname);
	if (!inmodcat(modcat, libname, &modcatline)) {
	  fail = true;

	} else if (modcatline != modlibline)
	  fail = true;
	if (!fail)
	  findmoduleend(modlib, libname, &modlibline);
      } else {
	fscanf(modlib->f, "%*[^\n]");
	getc(modlib->f);
	modlibline++;
      }
    } while (!(((times > checkuptimes) | BUFEOF(modlib->f)) || fail));
  }


  if (fail) {
    printf(" failed:");
    do {
      if (nostring(&buffer))
	printf(" halt or rebuild (h/r)?\n");
      TEMP.f = stdin;
      *TEMP.name = '\0';
      gettoken(&TEMP, &buffer, &command, &gotten);
      begcom = command.letters[0];
      if (gotten) {
	if (begcom != 'r' && begcom != 'h')
	  gotten = false;
      }
    } while (!gotten);
    if (begcom == 'h') {
      halt();
      return;
    }
    if (*modcat->name != '\0') {
      if (modcat->f != NULL)
	modcat->f = freopen(modcat->name, "wb", modcat->f);
      else
	modcat->f = fopen(modcat->name, "wb");
    } else {
      if (modcat->f != NULL)
	rewind(modcat->f);
      else
	modcat->f = tmpfile();
    }
    if (modcat->f == NULL)
      _EscIO2(FileNotFound, modcat->name);
    SETUPBUF(modcat->f, modcatitem);
    if (*modlib->name != '\0') {
      if (modlib->f != NULL)
	modlib->f = freopen(modlib->name, "r", modlib->f);
      else
	modlib->f = fopen(modlib->name, "r");
    } else
      rewind(modlib->f);
    if (modlib->f == NULL)
      _EscIO2(FileNotFound, modlib->name);
    RESETBUF(modlib->f, Char);
    return;
  }
  printf(" passed.\n");
  if (*modlib->name != '\0') {
    if (modlib->f != NULL)
      modlib->f = freopen(modlib->name, "r", modlib->f);
    else
      modlib->f = fopen(modlib->name, "r");
  } else
    rewind(modlib->f);
  if (modlib->f == NULL)
    _EscIO2(FileNotFound, modlib->name);
  RESETBUF(modlib->f, Char);
  if (*modcat->name != '\0') {
    if (modcat->f != NULL)
      modcat->f = freopen(modcat->name, "rb", modcat->f);
    else
      modcat->f = fopen(modcat->name, "rb");
  } else
    rewind(modcat->f);
  if (modcat->f == NULL)
    _EscIO2(FileNotFound, modcat->name);
  RESETBUF(modcat->f, modcatitem);
}


Static Void namestring(s, n)
stringDelila *s;
name n;
{
  long index;

  clearstring(s);
  for (index = 0; index < n.length; index++)
    s->letters[index] = n.letter[index];
  s->length = n.length;
  s->current = 1;
}


Static Void getlibmodule(modlib, token, currentline, lineofmodule, found,
			 modcat)
_TEXT *modlib;
stringDelila *token;
long *currentline, *lineofmodule;
boolean *found;
modcatfile *modcat;
{
  /*


*/
  Char ch;
  name modnam;
  modcatitem item;
  _TEXT TEMP;

  *found = false;

  while (!BUFEOF(modlib->f) && !*found) {
    while ((!BUFEOF(modlib->f)) & (P_peek(modlib->f) != '(')) {
      fscanf(modlib->f, "%*[^\n]");
      getc(modlib->f);
      (*currentline)++;
    }
    if (BUFEOF(modlib->f))
      break;
    resettrigger(&begintrigger);
    resettrigger(&endtrigger);
    while (!(P_eoln(modlib->f) ||
	     ((begintrigger.skip || begintrigger.found) &&
	      (endtrigger.skip || endtrigger.found)))) {
      ch = getc(modlib->f);

      if (ch == '\n')
	ch = ' ';
      testfortrigger(ch, &begintrigger);
      testfortrigger(ch, &endtrigger);
    }

    if (begintrigger.found) {
      *lineofmodule = *currentline;
      *found = true;
      getname(modlib, &modnam);
      namestring(token, modnam);


      item.module = modnam;
      item.line = *lineofmodule;
      drop(modcat, &item);

      findmoduleend(modlib, modnam, currentline);
      continue;
    }
    if (!endtrigger.found) {
      fscanf(modlib->f, "%*[^\n]");
      getc(modlib->f);
      (*currentline)++;
      continue;
    }
    getname(modlib, &modnam);
    printf("no beginning to module \n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    printname(&TEMP, modnam);
    printf(" at line %ld\n", *currentline);
    halt();
  }


  /*




*/
}


Static Void getcatmodule(modcat, token, lineofmodule, found)
modcatfile *modcat;
stringDelila *token;
long *lineofmodule;
boolean *found;
{
  /*
*/
  if (BUFEOF(modcat->f)) {
    *found = false;
    return;
  }
  namestring(token, GETFBUF(modcat->f, modcatitem).module);
  *lineofmodule = GETFBUF(modcat->f, modcatitem).line;
  GET(modcat->f, modcatitem);
  *found = true;
}


Static Void filltree(place, above, ident)
treecat **place, **above;
stringDelila ident;
{
  /*
*/
  treecat *WITH;

  WITH = *place;
  WITH->name = ident;
  WITH->linenumber = 0;
  WITH->down = NULL;
  WITH->up = *above;
  WITH->next = NULL;
}


/* Local variables for growtree: */
struct LOC_growtree {
  treecat *current, *above, *endofname, *atreecat;
  boolean gotten;
  stringDelila namomod, token;
} ;

Local Void filllower(LINK)
struct LOC_growtree *LINK;
{
  /*
*/
  _TEXT TEMP;

  while (LINK->namomod.current <= LINK->namomod.length && LINK->gotten) {
    TEMP.f = stdin;
    *TEMP.name = '\0';
    gettoken(&TEMP, &LINK->namomod, &LINK->token, &LINK->gotten);
    if (!LINK->gotten)
      break;
    LINK->above = LINK->current;
    LINK->atreecat->down = (treecat *)Malloc(sizeof(treecat));
    LINK->atreecat = LINK->atreecat->down;
    filltree(&LINK->atreecat, &LINK->above, LINK->token);
    LINK->current = LINK->atreecat;
  }
  LINK->endofname = LINK->atreecat;
}


Static Void growtree(modlib, modcat, root)
_TEXT *modlib;
modcatfile *modcat;
treecat **root;
{
  struct LOC_growtree V;
  long currentline = 1;
  long lineofmodule;
  treecat *nowhere = NULL;
  boolean usemodlib;
  boolean foundmodule = true;
  /*
*/
  boolean newlevel;
  _TEXT TEMP;

  if (*modlib->name != '\0') {
    if (modlib->f != NULL)
      modlib->f = freopen(modlib->name, "r", modlib->f);
    else
      modlib->f = fopen(modlib->name, "r");
  } else
    rewind(modlib->f);
  if (modlib->f == NULL)
    _EscIO2(FileNotFound, modlib->name);
  RESETBUF(modlib->f, Char);
  if (BUFEOF(modlib->f)) {
    printf("... wow, the module library is empty\n");
    halt();
  }

  if (*modcat->name != '\0') {
    if (modcat->f != NULL)
      modcat->f = freopen(modcat->name, "rb", modcat->f);
    else
      modcat->f = fopen(modcat->name, "rb");
  } else
    rewind(modcat->f);
  if (modcat->f == NULL)
    _EscIO2(FileNotFound, modcat->name);
  RESETBUF(modcat->f, modcatitem);
  if (BUFEOF(modcat->f)) {
    if (*modcat->name != '\0') {
      if (modcat->f != NULL)
	modcat->f = freopen(modcat->name, "wb", modcat->f);
      else
	modcat->f = fopen(modcat->name, "wb");
    } else {
      if (modcat->f != NULL)
	rewind(modcat->f);
      else
	modcat->f = tmpfile();
    }
    if (modcat->f == NULL)
      _EscIO2(FileNotFound, modcat->name);
    SETUPBUF(modcat->f, modcatitem);
  } else
    checkup(modlib, modcat);
  usemodlib = BUFEOF(modcat->f);

  printf("building name tree from ");
  if (usemodlib)
    printf("modlib");
  else
    printf("modcat");
  printf(" ... ");

  *root = NULL;

  while (foundmodule) {
    if (usemodlib)
      getlibmodule(modlib, &V.namomod, &currentline, &lineofmodule,
		   &foundmodule, modcat);
    else
      getcatmodule(modcat, &V.namomod, &lineofmodule, &foundmodule);

    if (!foundmodule)
      break;
    V.current = *root;
    V.above = V.current;
    V.endofname = V.current;
    removeperiods(&V.namomod);
    V.gotten = true;
    newlevel = false;


    while (V.namomod.current <= V.namomod.length && V.gotten) {
      TEMP.f = stdin;
      *TEMP.name = '\0';
      gettoken(&TEMP, &V.namomod, &V.token, &V.gotten);
      if (!V.gotten)
	break;
      /*
*/
      V.endofname = V.current;

      if (newlevel) {
	V.atreecat->down = (treecat *)Malloc(sizeof(treecat));
	V.above = V.current;
	V.current = V.atreecat->down;
	V.atreecat = V.current;
	filltree(&V.current, &V.above, V.token);
	filllower(&V);
	newlevel = false;
	continue;
      }

      if (V.current == NULL) {
	*root = (treecat *)Malloc(sizeof(treecat));
	V.above = *root;
	filltree(root, &nowhere, V.token);
	V.atreecat = *root;
	V.current = *root;
	filllower(&V);
	continue;
      }
      V.atreecat = V.current;
      while ((V.atreecat->next != NULL) & (!equalstring(V.token,
							V.atreecat->name)))
	V.atreecat = V.atreecat->next;

      if (V.atreecat != V.current)
	V.endofname = V.atreecat;

      if (equalstring(V.token, V.atreecat->name)) {
	if (V.atreecat->down == NULL) {
	  /*
*/
	  newlevel = true;

	  V.endofname = V.atreecat;
	} else {
	  V.above = V.current;
	  V.current = V.atreecat->down;
	  /*

*/
	}
	continue;
      }

      V.atreecat->next = (treecat *)Malloc(sizeof(treecat));
      V.atreecat = V.atreecat->next;


      if (V.current == *root)
	filltree(&V.atreecat, &nowhere, V.token);
      else
	filltree(&V.atreecat, &V.above, V.token);


      filllower(&V);
    }



    if (V.endofname->linenumber != 0) {
      printf("duplicate module names at lines %ld and %ld\n",
	     V.endofname->linenumber, lineofmodule);
      halt();
    } else
      V.endofname->linenumber = lineofmodule;
  }
}





Static Void initialize()
{
  printf("show %4.2f\n", version);
  printingstarted = false;
  /*
*/
  pagelength = defaultpagelength;
  rangebegin = defranbeg;
  rangeend = defranend;
  clearstring(&buffer);
  clearstring(&parent);
  clearstring(&command);
  begcom = command.letters[0];
  shownext = 2;
  printnext = 2;



  memcpy(begintrigger.n.letter,
	 "(* begin module                                   ", (long)maxname);
  untrail(&begintrigger.n);



  memcpy(endtrigger.n.letter,
	 "(* end module                                     ", (long)maxname);
  untrail(&endtrigger.n);


  donthalt = false;
  haltcalled = false;


  growtree(&modlib, &modcat, &root);
  if (root == NULL) {
    printf("no modules in modlib file\n");
    halt();
  }
  current = root;
  view = root;
  last = current;
  if (*modlib.name != '\0') {
    if (modlib.f != NULL)
      modlib.f = freopen(modlib.name, "r", modlib.f);
    else
      modlib.f = fopen(modlib.name, "r");
  } else
    rewind(modlib.f);
  if (modlib.f == NULL)
    _EscIO2(FileNotFound, modlib.name);
  RESETBUF(modlib.f, Char);
  linenumber = 1;
  showlinenumbers = false;
  printf("done\n");
}


Static Void gonext(place)
treecat **place;
{
  /*
*/
  if (*place != NULL)
    *place = (*place)->next;
}


Static Void writetreecat(afile, place)
_TEXT *afile;
treecat *place;
{
  if (place->linenumber != 0)
    fprintf(afile->f, "* ");
  else
    fprintf(afile->f, "  ");
  writestring(afile, &place->name);
  if (place->down != NULL)
    putc('.', afile->f);
}


#define width           4


Static Void writelinenumber(afile, place)
_TEXT *afile;
treecat *place;
{
  putc('[', afile->f);
  if (place->linenumber == 0)
    fprintf(afile->f, "%*c", width, ' ');
  else
    fprintf(afile->f, "%*ld", width, place->linenumber);
  fprintf(afile->f, "] ");
}

#undef width


Static Void writelevel(afile, parent, place, showlinenumbers)
_TEXT *afile;
stringDelila parent;
treecat *place;
boolean showlinenumbers;
{
  /*

*/
  long counter = 0, lines = 1;
  /*
*/
  Char c;
  long SET[5];

  writestring(afile, &parent);
  putc('\n', afile->f);

  if (rangebegin > 1)
    fprintf(afile->f, "...\n");
  while (place != NULL && counter < rangeend) {
    if (lines >= pagelength) {
      P_addset(P_expset(SET, 0L), 'y');
      P_addset(SET, 'n');
      if (!more(P_addset(SET, 'q'), &c))
	counter = LONG_MAX;
      else
	lines = 0;
      continue;
    }
    counter++;
    if (counter >= rangebegin) {
      fprintf(afile->f, "%2ld ", counter);
      if (showlinenumbers)
	writelinenumber(afile, place);
      writetreecat(afile, place);
      putc('\n', afile->f);
      lines++;
    }
    gonext(&place);
  }
  if (place != NULL)
    fprintf(afile->f, "...\n");
}


Static Void findonlevel(place, theone, box)
treecat **place;
stringDelila theone;
treecat **box;
{
  boolean found = false;

  *box = *place;
  while (!found && *box != NULL) {
    if (equalstring((*box)->name, theone))
      found = true;
    else
      gonext(box);
  }
}






Static Void gopage(buffer, t)
stringDelila *buffer;
_TEXT *t;
{
  boolean gotten;
  _TEXT TEMP;

  do {
    if (nostring(buffer))
      fprintf(t->f, "page length was %ld type new:\n", pagelength);
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getinteger(&TEMP, buffer, &pagelength, &gotten);
    if (gotten) {
      if (pagelength <= 0) {
	fprintf(t->f, "type a positive integer\n");
	gotten = false;
      }
    }
  } while (!gotten);
}


Static Void gorange(buffer, t)
stringDelila *buffer;
_TEXT *t;
{
  boolean gotten;
  _TEXT TEMP;

  do {
    if (nostring(buffer))
      fprintf(t->f,
	"the visible range of the name-part list began at %ld, type new: \n",
	rangebegin);
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getinteger(&TEMP, buffer, &rangebegin, &gotten);
    if (gotten) {
      if (rangebegin < 1) {
	fprintf(t->f, "use a positive integer\n");
	gotten = false;
      }

      if (rangebegin >= LONG_MAX) {
	fprintf(t->f, "use a smaller value\n");
	gotten = false;
      }
    }
  } while (!gotten);

  do {
    if (nostring(buffer))
      fprintf(t->f,
	"the visible range of the name-part list ended at %ld, type new: \n",
	rangeend);
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getinteger(&TEMP, buffer, &rangeend, &gotten);
    if (gotten) {
      if (rangeend >= LONG_MAX) {
	fprintf(t->f, "use a smaller value\n");
	gotten = false;
      }

      if (rangebegin < 1) {
	fprintf(t->f, "use a positive integer\n");
	gotten = false;
      }

      if (rangebegin > rangeend) {
	fprintf(t->f, "use a value larger than or equal to the begin (%ld)\n",
		rangebegin);
	gotten = false;
      }
    }
  } while (!gotten);

  fprintf(t->f, "range %ld to %ld\n", rangebegin, rangeend);
}


Static Void goprintmodule(modlib, display, place, line)
_TEXT *modlib, *display;
treecat *place;
long *line;
{
  boolean done = false;
  boolean gotten;
  Char c = ' ';
  long count = 0;
  name thename, endname;
  boolean thenamefound = false;
  _TEXT TEMP;
  long SET[5];

  if (place->linenumber == 0) {
    printf("no text exists for ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &parent);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &place->name);
    putchar('\n');
    return;
  }

  gettoline(place->linenumber, modlib, line);

  resettrigger(&begintrigger);
  resettrigger(&endtrigger);

  while (!BUFEOF(modlib->f) && !done) {
    while (!P_eoln(modlib->f)) {
      c = getc(modlib->f);
      if (c == '\n')
	c = ' ';
      putc(c, display->f);

      if (thenamefound) {
	testfortrigger(c, &endtrigger);
	if (endtrigger.found) {
	  getname(modlib, &endname);
	  printname(display, endname);
	  if (equalname(endname, thename))
	    done = true;
	}
	continue;
      }
      testfortrigger(c, &begintrigger);
      if (begintrigger.found) {
	getname(modlib, &thename);
	printname(display, thename);
	thenamefound = true;
      }
    }
    fscanf(modlib->f, "%*[^\n]");
    getc(modlib->f);
    putc('\n', display->f);
    (*line)++;
    count++;


    if (done)
      break;
    if (count < pagelength)
      continue;
    count = 0;
    gotten = false;
    do {
      P_addset(P_expset(SET, 0L), 'y');
      P_addset(SET, 'n');
      P_addset(SET, 'q');
      P_addset(SET, 'h');
      if (more(P_addset(SET, 'p'), &c))
	gotten = true;
      else if (c == 'h') {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	goprinthelp(&TEMP);
      } else if (c == 'p') {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	gopage(&buffer, &TEMP);
      } else {
	gotten = true;
	done = true;
      }
    } while (!gotten);
  }


  if (pagelength == LONG_MAX) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &parent);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &place->name);
    printf(" printed\n");
  }

  /*
*/
  if (c != 'n' && c != 'q' && pagelength != LONG_MAX) {
    P_addset(P_expset(SET, 0L), 'y');
    P_addset(SET, 'n');
    if (more(P_addset(SET, 'q'), &c)) {
      printf("no more text\n");

    }
  }

}


Static Void down(place, modlib, display, parent, lastparent, line, printed)
treecat **place;
_TEXT *modlib, *display;
stringDelila *parent, *lastparent;
long *line;
boolean *printed;
{
  /*


*/
  long temppagelength;

  if (shownext == 1 || (*place)->down == NULL) {
    *printed = true;
    if (printnext != 1) {
      goprintmodule(modlib, display, *place, line);
      return;
    }
    temppagelength = pagelength;
    pagelength = LONG_MAX;

    if (!printingstarted) {
      if (*print.name != '\0') {
	if (print.f != NULL)
	  print.f = freopen(print.name, "w", print.f);
	else
	  print.f = fopen(print.name, "w");
      } else {
	if (print.f != NULL)
	  rewind(print.f);
	else
	  print.f = tmpfile();
      }
      if (print.f == NULL)
	_EscIO2(FileNotFound, print.name);
      SETUPBUF(print.f, Char);
      printingstarted = true;
    }
    goprintmodule(modlib, &print, *place, line);
    pagelength = temppagelength;
    return;
  }
  *printed = false;
  pushstring((*place)->name, parent, lastparent);
  *place = (*place)->down;
}


Static Void godown(dottednames, parent, lastparent, current, last, view,
		   modlib, display, linenumber, error)
stringDelila *dottednames, *parent, *lastparent;
treecat **current, **last, **view;
_TEXT *modlib, *display;
long *linenumber;
boolean *error;
{
  /*
*/
  boolean gotten = true, printed = false;
  long count = 0;
  stringDelila aname;
  treecat *hole, *originalcurrent;
  _TEXT TEMP;

  removeperiods(dottednames);
  originalcurrent = *current;

  while (dottednames->current <= dottednames->length && gotten && !printed) {
    TEMP.f = stdin;
    *TEMP.name = '\0';
    gettoken(&TEMP, dottednames, &aname, &gotten);
    if (!gotten)
      break;
    /*



*/

    findonlevel(current, aname, &hole);

    if (hole == NULL) {
      printf("can not find ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &aname);
      printf(" on this level\n");
      *error = true;
      continue;
    }
    down(&hole, modlib, display, parent, lastparent, linenumber, &printed);
    if (!printed) {
      count++;
      *current = hole;
    }
    *view = hole;
  }

  if (printed)
    return;
  if (count == 0) {
    printf("no movement\n");
    return;
  }
  *last = originalcurrent;
  printf("moved %ld name level", count);
  if (count != 1)
    putchar('s');
  printf(" down\n");
}


Static Void gonumber(nstring, parent, lastparent, current, last, view, modlib,
		     display, linenumber, error)
stringDelila *nstring, *parent, *lastparent;
treecat **current, **last, **view;
_TEXT *modlib, *display;
long *linenumber;
boolean *error;
{
  /*

*/
  long n;
  long count = 1;
  boolean gotten, printed;
  treecat *hole;
  _TEXT TEMP;

  TEMP.f = stdin;
  *TEMP.name = '\0';
  getinteger(&TEMP, nstring, &n, &gotten);
  if (!gotten)
    return;
  if (n < 1) {
    printf("only positive integers are permitted\n");
    *error = true;
    return;
  }
  hole = *current;
  gotten = false;
  do {
    if (n == count) {
      gotten = true;
      down(&hole, modlib, display, parent, lastparent, linenumber, &printed);

      if (!printed) {
	*last = *current;
	*current = hole;
      }
      *view = hole;
    }

    else {
      hole = hole->next;
      count++;
    }
  } while (!(gotten || hole == NULL));
  if (!gotten) {
    printf("type a number smaller than %ld\n", count);
    *error = true;
  }
}


/* Local variables for gotonext: */
struct LOC_gotonext {
  stringDelila *parent, *lastparent;
  treecat **current, **view;
} ;

Local Void shift(LINK)
struct LOC_gotonext *LINK;
{
  boolean upward = true;
  treecat *lastcurrent;

  if ((*LINK->view)->down != NULL) {
    pushstring((*LINK->view)->name, LINK->parent, LINK->lastparent);
    *LINK->view = (*LINK->view)->down;
    *LINK->current = *LINK->view;
    return;
  }
  if ((*LINK->view)->next != NULL) {
    *LINK->view = (*LINK->view)->next;
    return;
  }
  while (upward) {
    popstring(LINK->parent, LINK->lastparent);
    lastcurrent = *LINK->current;
    *LINK->view = (*LINK->view)->up;
    *LINK->current = *LINK->view;

    if (*LINK->view == NULL) {
      *LINK->view = root;
      *LINK->current = *LINK->view;
      upward = false;

      continue;
    }

    while ((*LINK->view)->down != lastcurrent && (*LINK->view)->next != NULL)
      *LINK->view = (*LINK->view)->next;


    if ((*LINK->view)->next != NULL) {
      *LINK->view = (*LINK->view)->next;
      upward = false;
    }

  }
}


Static Void gotonext(parent_, lastparent_, current_, last, view_, modlib,
		     display, linenumber)
stringDelila *parent_, *lastparent_;
treecat **current_, **last, **view_;
_TEXT *modlib, *display;
long *linenumber;
{
  struct LOC_gotonext V;

  V.parent = parent_;
  V.lastparent = lastparent_;
  V.current = current_;
  V.view = view_;
  *last = *V.current;


  if (*V.view == root)
    printf("at treetop\n");

  goprintmodule(modlib, display, *V.view, linenumber);


  do {
    shift(&V);
  } while ((*V.view)->linenumber == 0);
}


Static Void gofile()
{
  if (nostring(&buffer))
    printf("next name will be printed to file\n");
  printnext = 0;
  if (shownext == 1)
    shownext = 0;
}


Static Void gostar()
{
  /*
*/
  if (nostring(&buffer))
    printf("next name will be shown\n");
  shownext = 0;
  if (printnext == 1)
    printnext = 0;
}


Static Void golast(c, l, v, parent, lastparent)
treecat **c, **l, **v;
stringDelila *parent, *lastparent;
{
  /*
*/
  treecat *holdt;
  stringDelila holds;

  holdt = *c;
  *c = *l;
  *l = holdt;

  *v = *c;


  copystring(*parent, &holds);
  copystring(*lastparent, parent);
  copystring(holds, lastparent);

  printf("at last location\n");
}


Static Void gotop(current, top, last, view, parent, lastparent)
treecat **current, **top, **last, **view;
stringDelila *parent, *lastparent;
{
  *last = *current;
  *current = *top;
  *view = *current;
  copystring(*parent, lastparent);
  clearstring(parent);
}


Static Void goup(curlevel, last, view, parent, lastparent)
treecat **curlevel, **last, **view;
stringDelila *parent, *lastparent;
{
  if ((*curlevel)->up != NULL) {
    *last = *curlevel;
    popstring(parent, lastparent);
    *curlevel = (*curlevel)->up;
  } else
    printf("nothing is above this level\n");
  *view = current;
}


Static Void goquit()
{
  printf("quit\n");
}


Static Void gowhere(showlinenumbers)
boolean *showlinenumbers;
{
  *showlinenumbers = !*showlinenumbers;
  printf("now ");
  if (!*showlinenumbers)
    printf("not ");
  printf("showing line numbers.\n");
}






main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  modcat.f = NULL;
  strcpy(modcat.name, "modcat");
  print.f = NULL;
  strcpy(print.name, "print");
  modlib.f = NULL;
  strcpy(modlib.name, "modlib");
  initialize();
  while (begcom != 'q') {
    do {
      if (nostring(&buffer)) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writelevel(&TEMP, parent, current, showlinenumbers);
	if (nostring(&buffer))
	  printf("show\n");
      }
      TEMP.f = stdin;
      *TEMP.name = '\0';
      gettoken(&TEMP, &buffer, &command, &gotten);


      if ((!gotten) & nostring(&buffer)) {
	clearstring(&buffer);
	/*
*/

	TEMP.f = stdout;
	*TEMP.name = '\0';
	gotonext(&parent, &lastparent, &current, &last, &view, &modlib, &TEMP,
		 &linenumber);
      }
    } while (!gotten);
    begcom = command.letters[0];
    error = false;


    printnext++;
    shownext++;



    command.current = 1;
    haveperiod = false;
    while (command.current <= command.length && !haveperiod) {
      haveperiod = (command.letters[command.current - 1] == period);
      command.current++;
    }
    command.current = 1;
    if (haveperiod) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      godown(&command, &parent, &lastparent, &current, &last, &view, &modlib,
	     &TEMP, &linenumber, &error);
    } else if (begcom == 'f')
      gofile();
    else if (begcom == 'h') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      gohelp(&TEMP);
    } else if (begcom == 'l')
      golast(&current, &last, &view, &parent, &lastparent);
    else if (begcom == 'n') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      gotonext(&parent, &lastparent, &current, &last, &view, &modlib, &TEMP,
	       &linenumber);
    } else if (begcom == 'p') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      gopage(&buffer, &TEMP);
    } else if (begcom == 'q')
      goquit();
    else if (begcom == 'r') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      gorange(&buffer, &TEMP);
    } else if (begcom == 's')
      gostar();
    else if (begcom == 't')
      gotop(&current, &root, &last, &view, &parent, &lastparent);
    else if (begcom == 'u')
      goup(&current, &last, &view, &parent, &lastparent);
    else if (begcom == 'w')
      gowhere(&showlinenumbers);
    else if (begcom >= '1' && begcom <= '9' || begcom == '0') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      gonumber(&command, &parent, &lastparent, &current, &last, &view,
	       &modlib, &TEMP, &linenumber, &error);
    } else if (needhelp(&buffer)) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      gohelp(&TEMP);
    }

    if (error)
      clearstring(&buffer);
  }
_L1:
  if (modlib.f != NULL)
    fclose(modlib.f);
  if (print.f != NULL)
    fclose(print.f);
  if (modcat.f != NULL)
    fclose(modcat.f);
  exit(EXIT_SUCCESS);
}



/* End. */
