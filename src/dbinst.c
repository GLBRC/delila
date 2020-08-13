/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dbinst.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"




#define version         3.61
#define feaspot         6
#define startspot       22

#define comspot         31
#define offfromspot     13
#define offtospot       22
#define fromspot        19
#define tospot          28
#define genuslimit      1

#define maxstring       2000
#define fillermax       50

typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;

typedef struct parameters {

  stringDelila feature;
  long after, before;

  boolean dobin, doein, dooin, dosin;

  boolean dosln, dooln;

  boolean doremove;

  Char foundanyway;
  long alignmentshift;
 
  boolean generic;
} parameters;

typedef struct spot {

  long location;
  struct spot *next;
} spot;

typedef Char filler[fillermax];

typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;



Static _TEXT db, binst, einst, oinst, sinst, olength, slength, dbinstp,
	     locuslist, missing;
/*
*/
Static _TEXT featab;
Static spot *freespots;
Static jmp_buf _JL1;
Static Void halt()
{
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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

Static Void emptystring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}


Static Void clearstring(ribbon)
stringDelila *ribbon;
{
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
  printf("remove initializestring routine!\n");
  printf("replace it with clearstring routine!\n");
  halt();
  clearstring(ribbon);
  ribbon->next = NULL;
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





Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
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



Static Void makenumber(name, number, found)
stringDelila name;
long *number;
boolean *found;
{
  /*
*/
  long l;

  *found = false;
  *number = 0;
  for (l = 0; l < name.length; l++) {
    if (name.letters[l] == '9' || name.letters[l] == '8' ||
	name.letters[l] == '7' || name.letters[l] == '6' ||
	name.letters[l] == '5' || name.letters[l] == '4' ||
	name.letters[l] == '3' || name.letters[l] == '2' ||
	name.letters[l] == '1' || name.letters[l] == '0') {
      *found = true;
      *number *= 10;
      switch (name.letters[l]) {

      case '0':
	/* blank case */
	break;

      case '1':
	(*number)++;
	break;

      case '2':
	*number += 2;
	break;

      case '3':
	*number += 3;
	break;

      case '4':
	*number += 4;
	break;

      case '5':
	*number += 5;
	break;

      case '6':
	*number += 6;
	break;

      case '7':
	*number += 7;
	break;

      case '8':
	*number += 8;
	break;

      case '9':
	*number += 9;
	break;
      }
    }
  }
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






Static boolean nearstring(a, b, n)
stringDelila a, b;
long n;
{
  /*
*/
  long index;
  boolean equal;

  if (a.length > 0 && b.length > 0) {
    index = 1;
    do {
      equal = (a.letters[index-1] == b.letters[index-1]);
      index++;
    } while (equal && index <= a.length && index <= b.length && index <= n);
    return equal;
  }
  if (a.length == 0 && b.length == 0)
    equal = true;
  else
    equal = false;
  return equal;
}






Static Void writeparameters(tofile, par)
_TEXT *tofile;
parameters par;
{
  fprintf(tofile->f, "* searching for ");
  writestring(tofile, &par.feature);
  putc('\n', tofile->f);

  if (par.generic)
    fprintf(tofile->f, "* from before to after\n");
  else
    fprintf(tofile->f, "* from %ld to %ld\n", par.before, par.after);

  if (par.dobin)
    fprintf(tofile->f, "* begin instructions written\n");
  if (par.doein)
    fprintf(tofile->f, "* end instructions written\n");
  if (par.dooin)
    fprintf(tofile->f, "* object instructions written\n");
  if (par.dosin)
    fprintf(tofile->f, "* space instructions written\n");
  if (par.dooln)
    fprintf(tofile->f, "* object lengths written\n");
  if (par.dosln)
    fprintf(tofile->f, "* space lengths written\n");

  if (par.dobin || par.doein) {
    if (par.doremove) {
      fprintf(tofile->f,
	"* duplicate binst or einst instructions and objects (etc) removed\n");

    } else
      fprintf(tofile->f,
	"* DUPLICATE INSTRUCTIONS will NOT be REMOVED from binst or einst\n");
  }

  if (par.foundanyway == 'f')
    fprintf(tofile->f,
      "* found-anyway is on (f): coordinates off the piece will be written anyway\n");

  if (par.foundanyway == 's')
    fprintf(tofile->f,
      "* found-anyway is off (s): coordinates off the piece will NOT be written.\n");

  fprintf(tofile->f, "* alignment shift: %ld\n", par.alignmentshift);

}



Static Void readparameters(dbinstp, par)
_TEXT *dbinstp;
parameters *par;
{
  Char c;
  boolean gotten;
  stringDelila buffer;

  if (*dbinstp->name != '\0') {
    if (dbinstp->f != NULL)
      dbinstp->f = freopen(dbinstp->name, "r", dbinstp->f);
    else
      dbinstp->f = fopen(dbinstp->name, "r");
  } else {

    rewind(dbinstp->f);
  }
  if (dbinstp->f == NULL)
    _EscIO2(FileNotFound, dbinstp->name);
  RESETBUF(dbinstp->f, Char);
  getstring(dbinstp, &buffer, &gotten);
  if (!gotten) {
    printf("empty dbinstp\n");
    halt();
  }


  token(&buffer, &par->feature, &gotten);
  if (!gotten) {
    printf("missing first parameter\n");
    halt();
  }


  if (BUFEOF(dbinstp->f)) {
    printf("missing from-to parameters\n");
    halt();
  }
  fscanf(dbinstp->f, "%ld%ld%*[^\n]", &par->before, &par->after);
  getc(dbinstp->f);
  if (par->before > par->after)
    par->generic = true;
  else
    par->generic = false;


  if (BUFEOF(dbinstp->f)) {
    printf("missing beos instruction file parameters\n");
    halt();
  }
  c = getc(dbinstp->f);
  if (c == '\n')
    c = ' ';
  par->dobin = (c == 'b');
  c = getc(dbinstp->f);
  if (c == '\n')
    c = ' ';
  par->doein = (c == 'e');
  c = getc(dbinstp->f);
  if (c == '\n')
    c = ' ';
  par->dooin = (c == 'o');
  c = getc(dbinstp->f);
  if (c == '\n')
    c = ' ';
  par->dosin = (c == 's');
  fscanf(dbinstp->f, "%*[^\n]");


  getc(dbinstp->f);
  if (BUFEOF(dbinstp->f)) {
    printf("missing os instruction file parameters\n");
    halt();
  }
  c = getc(dbinstp->f);
  if (c == '\n')
    c = ' ';
  par->dooln = (c == 'o');
  c = getc(dbinstp->f);
  if (c == '\n')
    c = ' ';
  par->dosln = (c == 's');
  fscanf(dbinstp->f, "%*[^\n]");


  getc(dbinstp->f);
  if (BUFEOF(dbinstp->f)) {
    printf("missing remove instruction file parameter\n");
    halt();
  }
  if (P_peek(dbinstp->f) == 'r')
    par->doremove = true;
  else
    par->doremove = false;
  fscanf(dbinstp->f, "%*[^\n]");

  getc(dbinstp->f);
  fscanf(dbinstp->f, "%c%*[^\n]", &par->foundanyway);
  getc(dbinstp->f);

  if (par->foundanyway == '\n')
    par->foundanyway = ' ';
  fscanf(dbinstp->f, "%ld%*[^\n]", &par->alignmentshift);
  getc(dbinstp->f);
}



Static Char sign(l)
long l;
{
  /*
*/
  if (l >= 0)
    return '+';
  else {
    return '-';
    /*




*/
  }
}



Static Void writeinst(f, didpiece, locusname, featurename, first, last, par,
		      complement, number)
_TEXT *f;
boolean *didpiece;
stringDelila locusname, *featurename;
long first, last;
parameters par;
boolean complement;
long *number;
{
  /*




*/
  (*number)++;

  if (!*didpiece) {
    fprintf(f->f, "\npiece ");
    writestring(f, &locusname);
    fprintf(f->f, ";\n");
    *didpiece = true;
  }

  fprintf(f->f, "name \"");
  writestring(f, featurename);
  fprintf(f->f, "\"; get");

  if (complement) {
    if (par.generic)
      fprintf(f->f, " from %ld -after to %ld -before", first, last);
    else
      fprintf(f->f, " from %ld %c%ld to %ld %c%ld",
	      first, sign(-par.before), labs(par.before), last,
	      sign(-par.after), labs(par.after));
  } else if (par.generic)
    fprintf(f->f, " from %ld +before to %ld +after", first, last);
  else
    fprintf(f->f, " from %ld %c%ld to %ld %c%ld",
	    first, sign(par.before), labs(par.before), last, sign(par.after),
	    labs(par.after));
  fprintf(f->f, " direction ");
  if (complement)
    putc('-', f->f);
  else
    putc('+', f->f);

  fprintf(f->f, "; (* %ld", *number);
  if (complement)
    fprintf(f->f, " COMPLEMENT *)\n");
  else
    fprintf(f->f, " *)\n");
}



Static Void writetable(f, didpiece, locusname, featurename, first, last, par,
		       complement, number)
_TEXT *f;
boolean *didpiece;
stringDelila locusname, *featurename;
long first, last;
parameters par;
boolean complement;
long *number;
{
  /*




*/
  Char tab = '_';

  tab = '\t';
  (*number)++;
  writestring(f, &locusname);
  putc(tab, f->f);
  writestring(f, featurename);
  fprintf(f->f, "%c%ld", tab, first);
  fprintf(f->f, "%c%ld", tab, last);
  putc(tab, f->f);
  if (complement)
    putc('-', f->f);
  else
    putc('+', f->f);
  putc('\n', f->f);
}




Static Void writelength(afile, fromnumber, tonumber, locusname, complement,
			number)
_TEXT *afile;
long fromnumber, tonumber;
stringDelila locusname;
boolean complement;
long number;
{
  /*

*/
  long length;

  /*

*/
  /*





*/

  if (complement)
    length = fromnumber - tonumber + 1;
  else
    length = tonumber - fromnumber + 1;

  /*













*/

  fprintf(afile->f, "%8ld", length);
  putc(' ', afile->f);
  fprintf(afile->f, "%8ld", fromnumber);
  putc(' ', afile->f);
  fprintf(afile->f, "%8ld", tonumber);
  putc(' ', afile->f);
  writestring(afile, &locusname);
  fprintf(afile->f, " %5ld\n", number);
}



Static Void instheader(afile, inuse, kind, par)
_TEXT *afile;
boolean inuse;
Char kind;
parameters par;
{
  /*








*/
  /*

*/

  if (!inuse)
    return;
  if (*afile->name != '\0') {
    if (afile->f != NULL)
      afile->f = freopen(afile->name, "w", afile->f);
    else
      afile->f = fopen(afile->name, "w");
  } else {
    if (afile->f != NULL)
      rewind(afile->f);
    else
      afile->f = tmpfile();
  }
  if (afile->f == NULL)
    _EscIO2(FileNotFound, afile->name);
  SETUPBUF(afile->f, Char);

  fprintf(afile->f, "title \"");
  writestring(afile, &par.feature);

  switch (kind) {

  case 'b':
    fprintf(afile->f, " beginning");
    break;

  case 'e':
    fprintf(afile->f, " ending");
    break;

  case 'o':
    fprintf(afile->f, " object");
    break;

  case 's':
    fprintf(afile->f, " space");
    break;
  }

  if (par.generic)
    fprintf(afile->f, " from before to after");
  else
    fprintf(afile->f, " from %ld to %ld", par.before, par.after);
  fprintf(afile->f, "\";\n");

  fprintf(afile->f, "(* dbinst %4.2f *)\n", version);
  fprintf(afile->f, "default out-of-range reduce-range;\n");
  fprintf(afile->f, "default numbering piece;\n\n");

  fprintf(afile->f, "(* The parameters used were:\n");
  writeparameters(afile, par);
  fprintf(afile->f, "*)\n");
}



Static Void lengthheader(length, inuse, kind, par)
_TEXT *length;
boolean inuse;
Char kind;
parameters par;
{
  /*



*/
  /*

*/

  if (!inuse)
    return;
  if (*length->name != '\0') {
    if (length->f != NULL)
      length->f = freopen(length->name, "w", length->f);
    else
      length->f = fopen(length->name, "w");
  } else {
    if (length->f != NULL)
      rewind(length->f);
    else
      length->f = tmpfile();
  }
  if (length->f == NULL)
    _EscIO2(FileNotFound, length->name);
  SETUPBUF(length->f, Char);
  fprintf(length->f, "* dbinst %4.2f\n", version);

  switch (kind) {

  case 'o':
    fprintf(length->f, "* object");
    break;

  case 's':
    fprintf(length->f, "* space");
    break;
  }
  fprintf(length->f, " lengths\n");

  /*

*/


  fprintf(length->f, "*-\n");
  fprintf(length->f, "** MARKS BEGINNING OF DELILA INSTRUCTIONS\n");
  fprintf(length->f, "* title \"");
  writestring(length, &par.feature);

  switch (kind) {

  case 'b':
    fprintf(length->f, " beginning");
    break;

  case 'e':
    fprintf(length->f, " ending");
    break;

  case 'o':
    fprintf(length->f, " object");
    break;

  case 's':
    fprintf(length->f, " space");
    break;
  }

  if (par.generic)
    fprintf(length->f, " from before to after");
  else
    fprintf(length->f, " from %ld to %ld", par.before, par.after);
  fprintf(length->f, "\";\n");

  fprintf(length->f, "* (* dbinst %4.2f *)\n", version);
  fprintf(length->f, "* default out-of-range reduce-range;\n");
  fprintf(length->f, "* default numbering piece;\n");

  fprintf(length->f, "*-\n");
  fprintf(length->f, "* (* The parameters used were:\n");
  writeparameters(length, par);
  fprintf(length->f, "* *)\n");
  fprintf(length->f, "** MARKS ENDING OF DELILA INSTRUCTIONS\n");

  fprintf(length->f, "*-\n");
  fprintf(length->f, "* COLUMN identification:\n");
  fprintf(length->f,
	  "* length, first-position, last-position, piece-name, number\n");
}



Static Void die()
{
  printf("can't locate an expected part of the db\n");
  halt();
}



Static Void orgchr(inst, name, stars)
_TEXT *inst;
stringDelila name;
boolean stars;
{
  /*
*/
  if (stars)
    fprintf(inst->f, "**");
  putc('\n', inst->f);

  if (stars)
    fprintf(inst->f, "**");
  fprintf(inst->f, "organism ");
  writestring(inst, &name);
  fprintf(inst->f, ";\n");

  if (stars)
    fprintf(inst->f, "**");
  fprintf(inst->f, "chromosome ");
  writestring(inst, &name);
  fprintf(inst->f, ";\n");
}



Static Void recordspot(spot_, list, free)
long spot_;
spot **list, **free;
{
  spot *p;

  if (*free == NULL) {
    p = (spot *)Malloc(sizeof(spot));
    p->next = NULL;
  } else {
    p = *free;
    *free = (*free)->next;
  }


  p->location = spot_;


  if (*list == NULL) {
    *list = p;
    p->next = NULL;
  } else {
    p->next = *list;
    *list = p;
  }
}



Static Void dumpspots(list, free)
spot **list, **free;
{
  spot *p;

  while (*list != NULL) {
    p = *list;
    *list = (*list)->next;
    p->next = *free;
    *free = p;
  }
}



Static boolean onlist(list, number)
spot *list;
long number;
{
  spot *p = list;
  boolean found = false;

  while (p != NULL && !found) {
    if (p->location == number)
      found = true;
    p = p->next;
  }
  if (found)
    return true;
  else
    return false;
}



Static Void showspots(thefile, list)
_TEXT *thefile;
spot *list;
{
  spot *p = list;

  while (p != NULL) {
    fprintf(thefile->f, " %ld", p->location);
    p = p->next;
    if (p == list) {
      fprintf(thefile->f, "infinite loop!\n");
      halt();
    }
  }
  putc('\n', thefile->f);
}


/*















*/


Static Void skipstring(buffer, flag)
stringDelila *buffer;
Char flag;
{
  /*
*/
  /*

*/
  while (buffer->letters[buffer->current - 1] != flag &&
	 buffer->current <= buffer->length) {
    /*





*/
    buffer->current++;
  }
  /*





*/
}


/*
































*/


Static long inttochar(letter)
Char letter;
{
  long Result;

  switch (letter) {

  case '0':
    Result = 0;
    break;

  case '1':
    Result = 1;
    break;

  case '2':
    Result = 2;
    break;

  case '3':
    Result = 3;
    break;

  case '4':
    Result = 4;
    break;

  case '5':
    Result = 5;
    break;

  case '6':
    Result = 6;
    break;

  case '7':
    Result = 7;
    break;

  case '8':
    Result = 8;
    break;

  case '9':
    Result = 9;
    break;
  }
  return Result;
}



Static Void readonenumber(buffer, number, found)
stringDelila *buffer;
long *number;
boolean *found;
{
  /*



*/
  boolean done = false, greatless = false;

  *number = 0;
  *found = false;
  while (!done) {
    if (buffer->current > buffer->length)
      done = true;
    else {
      if (buffer->letters[buffer->current - 1] == '9' ||
	  buffer->letters[buffer->current - 1] == '8' ||
	  buffer->letters[buffer->current - 1] == '7' ||
	  buffer->letters[buffer->current - 1] == '6' ||
	  buffer->letters[buffer->current - 1] == '5' ||
	  buffer->letters[buffer->current - 1] == '4' ||
	  buffer->letters[buffer->current - 1] == '3' ||
	  buffer->letters[buffer->current - 1] == '2' ||
	  buffer->letters[buffer->current - 1] == '1' ||
	  buffer->letters[buffer->current - 1] == '0') {
	*found = true;
	*number = *number * 10 + inttochar(buffer->letters[buffer->current - 1]);
      } else {
	if (*found) {
	  /*
*/
	  done = true;
	}
      }
    }

    if (buffer->letters[buffer->current - 1] == '<' ||
	buffer->letters[buffer->current - 1] == '>')
      greatless = true;
    buffer->current++;
  }
  if (greatless)
    *found = false;
  /*

*/
}


#define debugging       false


Static boolean findstring(a, b)
trigger *a;
stringDelila b;
{
  boolean answer = false;
  long l;
  _TEXT TEMP;

  if (a->seek.length >= b.length)
    return answer;
  if (debugging) {
    printf("a = \"");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &a->seek);
    printf("\"\n");
    printf("b = \"");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &b);
    printf("\"\n");
    printf("a.seek.length = %ld < b.length = %ld\n", a->seek.length, b.length);
  }
  resettrigger(a);
  for (l = 0; l < b.length; l++) {
    testfortrigger(b.letters[l], a);
    if (a->found)
      answer = true;
  }
  return answer;
}

#undef debugging


#define debugging       false


/* Local variables for themain: */
struct LOC_themain {

  stringDelila buffer;

  long i;
} ;

Local Void grabfeaturename(featurename, LINK)
stringDelila *featurename;
struct LOC_themain *LINK;
{
  LINK->buffer.current = startspot;
  skipstring(&LINK->buffer, '"');
  LINK->i = 1;
  featurename->letters[LINK->i-1] =
    LINK->buffer.letters[LINK->buffer.current + LINK->i - 1];
  while (featurename->letters[LINK->i-1] != '"' &&
	 LINK->i < LINK->buffer.length) {
    LINK->i++;
    featurename->letters[LINK->i-1] =
      LINK->buffer.letters[LINK->buffer.current + LINK->i - 1];
  }
  featurename->length = LINK->i - 1;
  if (featurename->length != 0)
    return;
  printf("/gene or /locus_tag name not found!\n");
  printf("Using 'EMPTY' instead of /gene name\n");

  featurename->length = 5;
  featurename->letters[0] = 'E';
  featurename->letters[1] = 'M';
  featurename->letters[2] = 'P';
  featurename->letters[3] = 'T';
  featurename->letters[4] = 'Y';
}




Static Void themain(db, binst, einst, oinst, sinst, olength, slength, dbinstp,
		    missing, featab)
_TEXT *db, *binst, *einst, *oinst, *sinst, *olength, *slength, *dbinstp,
      *missing, *featab;
{
  /*


*/
  struct LOC_themain V;
  boolean featurewanted = false;
  /*
*/
  stringDelila atoken;
  long bnumber = 0;

  stringDelila comment;
  boolean complement;

  boolean didbpiece = false;
  /*
*/
  boolean didepiece = false;
  /*
*/
  boolean didopiece = false;
  /*
*/
  boolean didspiece = false;
  /*
*/
  boolean done;

  stringDelila endentry;
  long enumber = 0, tnumber = 0;

  stringDelila feature;
  boolean featurefound = false;
  /*
*/
  boolean fromfound = false;
  /*

*/
  long fromnumber = -1;

  spot *listfrom = NULL, *listto = NULL;

  long lownumber;

  stringDelila genus, genusspecies, strain, holdstring, dash;
  boolean gotten;

  stringDelila less, locus, locusname;

  stringDelila more;

  stringDelila note;

  stringDelila gene, loctag;

  stringDelila featurename;

  stringDelila oldgenusspecies;
  boolean oldtofound;
  long oldtonumber = 0;
  /*
*/
  long onumber = 0;
  stringDelila organism;

  parameters par;
  stringDelila period;

  boolean spacecomplement;
  stringDelila species;
  long snumber = 0;

  boolean tellaboutfeaturemissing = false;
  /*
*/
  boolean tofound = false;
  long tonumber = -1;

  boolean onemore = false;
  Char tab = '\t';

  trigger join, comp;
  long hold;
  _TEXT TEMP;


  printf("dbinst %4.2f\n", version);
  if (*missing->name != '\0') {
    if (missing->f != NULL)
      missing->f = freopen(missing->name, "w", missing->f);
    else
      missing->f = fopen(missing->name, "w");
  } else {
    if (missing->f != NULL)
      rewind(missing->f);
    else
      missing->f = tmpfile();
  }
  if (missing->f == NULL) {

    _EscIO2(FileNotFound, missing->name);
  }
  SETUPBUF(missing->f, Char);
  fprintf(missing->f, "dbinst %4.2f MISSING INSTRUCTIONS\n", version);
  fprintf(missing->f, "These loci contain features that are in the COMMENT\n");
  fprintf(missing->f,
	  "of an entry.  They are not listed in binst or einst.\n\n");
  if (*db->name != '\0') {
    if (db->f != NULL)
      db->f = freopen(db->name, "r", db->f);
    else
      db->f = fopen(db->name, "r");
  } else
    rewind(db->f);
  if (db->f == NULL)
    _EscIO2(FileNotFound, db->name);
  RESETBUF(db->f, Char);
  if (*locuslist.name != '\0') {
    if (locuslist.f != NULL)
      locuslist.f = freopen(locuslist.name, "w", locuslist.f);
    else
      locuslist.f = fopen(locuslist.name, "w");
  } else {
    if (locuslist.f != NULL)
      rewind(locuslist.f);
    else
      locuslist.f = tmpfile();
  }
  if (locuslist.f == NULL)
    _EscIO2(FileNotFound, locuslist.name);
  SETUPBUF(locuslist.f, Char);
  fprintf(locuslist.f, "dbinst %4.2f LOCUS LIST \n", version);
  fprintf(locuslist.f,
	  "These are the loci that contain features of interest\n\n");


  freespots = NULL;






  /*


*/
  fillstring(&locus, "VERSION                                           ");
  fillstring(&organism, "ORGANISM                                          ");
  fillstring(&comment, "COMMENT                                           ");
  fillstring(&feature, "FEATURES                                          ");
  fillstring(&note, "/note=\"                                           ");
  fillstring(&gene, "/gene=\"                                           ");
  fillstring(&loctag, "/locus_tag=\"                                      ");
  fillstring(&endentry, "//                                                ");
  fillstring(&period, ".                                                 ");
  fillstring(&less, "<                                                 ");
  fillstring(&more, ">                                                 ");
  fillstring(&dash, "-                                                 ");



  filltrigger(&join, "join(                                             ");
  filltrigger(&comp, "complement(                                       ");
  clearstring(&oldgenusspecies);

  readparameters(dbinstp, &par);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeparameters(&TEMP, par);


  instheader(binst, par.dobin, 'b', par);
  instheader(einst, par.doein, 'e', par);
  instheader(oinst, par.dooin, 'o', par);
  instheader(sinst, par.dosin, 's', par);
  lengthheader(olength, par.dooln, 'o', par);
  lengthheader(slength, par.dosln, 's', par);

  if (*featab->name != '\0') {
    if (featab->f != NULL)
      featab->f = freopen(featab->name, "w", featab->f);
    else
      featab->f = fopen(featab->name, "w");
  } else {
    if (featab->f != NULL)
      rewind(featab->f);
    else
      featab->f = tmpfile();
  }
  if (featab->f == NULL)
    _EscIO2(FileNotFound, featab->name);
  SETUPBUF(featab->f, Char);
  fprintf(featab->f, "* dbinst %4.2f featab feature table\n", version);
  fprintf(featab->f, "accession%cname%cbegin%cend%corientation\n",
	  tab, tab, tab, tab);



  clearstring(&V.buffer);
  clearstring(&featurename);

  while (!BUFEOF(db->f)) {
    /*

*/
    getstring(db, &V.buffer, &gotten);

    if (!gotten)
      continue;
    token(&V.buffer, &atoken, &gotten);
    if (!gotten)
      continue;



    /*

*/
    if (equalstring(atoken, locus)) {
      token(&V.buffer, &locusname, &gotten);
      if (!gotten)
	die();
      didbpiece = false;
      didepiece = false;
      didopiece = false;
      didspiece = false;
      featurewanted = false;

      /*

*/
      oldtofound = false;
      oldtonumber = 12345678L;


      dumpspots(&listfrom, &freespots);
      dumpspots(&listto, &freespots);

      /*





*/
    }


    /*

*/
    if (equalstring(atoken, organism)) {
      token(&V.buffer, &genus, &gotten);

      if (!gotten)
	die();
      token(&V.buffer, &species, &gotten);
      if (!gotten)
	die();


      if (genus.length > genuslimit)
	genus.length = genuslimit;
      concatstring(&genus, &period, &atoken);
      concatstring(&atoken, &species, &genusspecies);

      /*
*/
      gotten = true;
      while (gotten) {
	token(&V.buffer, &strain, &gotten);
	if (!gotten)
	  break;
	concatstring(&dash, &strain, &atoken);
	concatstring(&genusspecies, &atoken, &holdstring);
	copystring(holdstring, &genusspecies);
      }
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &genusspecies);
      printf(" = genus.species-strain\n");

      if (!equalstring(genusspecies, oldgenusspecies)) {
	if (par.dobin)
	  orgchr(binst, genusspecies, false);

	if (par.doein)
	  orgchr(einst, genusspecies, false);

	if (par.dooin)
	  orgchr(oinst, genusspecies, false);

	if (par.dosin)
	  orgchr(sinst, genusspecies, false);

	if (par.dosln)
	  orgchr(slength, genusspecies, true);
	if (par.dooln)
	  orgchr(olength, genusspecies, true);

	copystring(genusspecies, &oldgenusspecies);

	didbpiece = false;
	didepiece = false;
	didopiece = false;
	didspiece = false;
      }

    }

    /*





*/


    if (nearstring(atoken, note, 7L)) {
      /*
*/
      /*
*/
      V.buffer.current = 1;

      skipstring(&V.buffer, '"');

      V.buffer.current++;
      /*
*/
      do {
	skipstring(&V.buffer, '"');
	done = (V.buffer.letters[V.buffer.current - 1] == '"') | BUFEOF(db->f);
	if (!done)
	  getstring(db, &V.buffer, &gotten);
      } while (!done);
      clearstring(&V.buffer);
      clearstring(&atoken);
    }



    /*

*/
    if (equalstring(atoken, feature))
      featurefound = true;



    /*

*/
    if (equalstring(atoken, endentry))
      featurefound = false;
    /*

*/
    if (equalstring(atoken, comment)) {
      featurefound = false;
      tellaboutfeaturemissing = true;
    }

    /*
*/
    if (equalstring(atoken, feature) && !featurefound) {
      if (tellaboutfeaturemissing) {
	fprintf(missing->f, "entry ");
	writestring(missing, &locusname);
	putc('\n', missing->f);
	tellaboutfeaturemissing = false;
      }
      writestring(missing, &V.buffer);
      putc('\n', missing->f);
    }


    /*






*/



    /*
*/

    if ((nearstring(atoken, gene, 7L) | nearstring(atoken, loctag, 9L)) &&
	onemore) {
      grabfeaturename(&featurename, &V);
      printf("grabbing /gene ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &featurename);
      putchar('\n');
    }

    if (featurefound) {
      V.buffer.current = startspot;
      /*
*/

      if (equalstring(atoken, par.feature)) {
	featurewanted = true;
	onemore = true;
      } else if (onemore) {
	onemore = false;
	featurewanted = true;
      } else
	featurewanted = false;
    } else {

      featurewanted = false;
    }


    if (!featurewanted)
      continue;

    if (onemore == true) {
      fromfound = false;
      tofound = false;


      if (findstring(&comp, V.buffer))
	complement = true;
      else
	complement = false;


      /*

*/

      /*

















*/
      V.buffer.current = startspot;
      readonenumber(&V.buffer, &fromnumber, &fromfound);
      /*




*/
      /*
*/
      while (V.buffer.current < V.buffer.length)
	readonenumber(&V.buffer, &tonumber, &tofound);
      /*




*/

      if (complement) {
	hold = fromnumber;
	fromnumber = tonumber;
	tonumber = hold;
      }


      if (complement) {
	fromnumber -= par.alignmentshift;
	tonumber -= par.alignmentshift;
      } else {
	fromnumber += par.alignmentshift;
	tonumber += par.alignmentshift;
      }
      continue;
    }


    /*



*/

    /*





*/

    /*

*/

    if (par.foundanyway == 'f') {
      fromfound = true;
      tofound = true;
    } else if (par.foundanyway == 's') {

      if (!fromfound || !tofound) {
	fromfound = false;
	tofound = false;
      }
    }

    /*



*/

    if (fromfound && tofound) {
      if (par.dooin) {
	if (onlist(listto, tonumber) | onlist(listfrom, fromnumber)) {
	  fromfound = false;
	  tofound = false;
	}
      }
    }

    /*

*/
    if (fromfound && oldtofound) {
      if (par.dosin) {
	if (onlist(listto, tonumber) | onlist(listfrom, fromnumber)) {
	  fromfound = false;
	  tofound = false;
	}
      }
    }

    /*





*/




    /*




*/

    if (fromfound && tofound) {
      /*


*/
      if (par.dooin)
	writeinst(oinst, &didopiece, locusname, &featurename, fromnumber,
		  tonumber, par, complement, &onumber);

      if (par.dooln)
	writelength(olength, fromnumber, tonumber, locusname, complement,
		    onumber);
    }

    if (fromfound && oldtofound) {
      spacecomplement = (fromnumber < oldtonumber);


      if (fromnumber < tonumber)
	lownumber = fromnumber;
      else
	lownumber = tonumber;


      if (par.dosin) {
	/*

*/
	writeinst(sinst, &didspiece, locusname, &featurename, oldtonumber,
		  lownumber, par, spacecomplement, &snumber);
      }


      if (par.dosln) {
	/*

*/
	writelength(slength, oldtonumber, lownumber, locusname,
		    spacecomplement, onumber);
      }

    }
    /*


*/





    if (fromfound || tofound) {
      if (!(didbpiece || didepiece)) {
	writestring(&locuslist, &locusname);
	putc('\n', locuslist.f);
      }
    }

    if (par.dobin) {
      if (fromfound & (!onlist(listfrom, fromnumber))) {
	writeinst(binst, &didbpiece, locusname, &featurename, fromnumber,
		  fromnumber, par, complement, &bnumber);

	if (par.doremove)
	  recordspot(fromnumber, &listfrom, &freespots);
      }
    }

    if (par.doein) {
      if (tofound & (!onlist(listto, tonumber))) {
	writeinst(einst, &didepiece, locusname, &featurename, tonumber,
		  tonumber, par, complement, &enumber);

	if (par.doremove)
	  recordspot(tonumber, &listto, &freespots);
      }
    }


    writetable(featab, &didepiece, locusname, &featurename, fromnumber,
	       tonumber, par, complement, &tnumber);



    /*
*/
    if (V.buffer.letters[tospot-1] != ' ')
      oldtofound = tofound;
    oldtofound = tofound;
    /*

*/
    if (tofound) {
      /*



*/
      if (tonumber > fromnumber)
	oldtonumber = tonumber;
      else
	oldtonumber = fromnumber;
    }



    /*
*/
    /*



*/

  }
}

#undef debugging



main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  featab.f = NULL;
  strcpy(featab.name, "featab");
  missing.f = NULL;
  strcpy(missing.name, "missing");
  locuslist.f = NULL;
  strcpy(locuslist.name, "locuslist");
  dbinstp.f = NULL;
  strcpy(dbinstp.name, "dbinstp");
  slength.f = NULL;
  strcpy(slength.name, "slength");
  olength.f = NULL;
  strcpy(olength.name, "olength");
  sinst.f = NULL;
  strcpy(sinst.name, "sinst");
  oinst.f = NULL;
  strcpy(oinst.name, "oinst");
  einst.f = NULL;
  strcpy(einst.name, "einst");
  binst.f = NULL;
  strcpy(binst.name, "binst");
  db.f = NULL;
  strcpy(db.name, "db");
  themain(&db, &binst, &einst, &oinst, &sinst, &olength, &slength, &dbinstp,
	  &missing, &featab);
_L1:
  if (db.f != NULL)
    fclose(db.f);
  if (binst.f != NULL)
    fclose(binst.f);
  if (einst.f != NULL)
    fclose(einst.f);
  if (oinst.f != NULL)
    fclose(oinst.f);
  if (sinst.f != NULL)
    fclose(sinst.f);
  if (olength.f != NULL)
    fclose(olength.f);
  if (slength.f != NULL)
    fclose(slength.f);
  if (dbinstp.f != NULL)
    fclose(dbinstp.f);
  if (locuslist.f != NULL)
    fclose(locuslist.f);
  if (missing.f != NULL)
    fclose(missing.f);
  if (featab.f != NULL)
    fclose(featab.f);
  exit(EXIT_SUCCESS);
}



/* End. */
