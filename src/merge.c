/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "merge.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*




*/



#define version         9.53



/*
























































*/


#define blank           ' '


#define maxstring       150



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;


typedef struct line {
  stringDelila str;
  long number;
} line;


typedef enum {
  ofa, ofb
} filecorrection;
typedef enum {
  mismatch, deletion, insertion, unidentified
} typecorrection;


Static _TEXT afile, bfile, apfile, bpfile;

Static line aline, bline;
Static boolean done;

Static boolean scanning;
/*
*/


Static long characters, mism, deli, unid;


Static long correction[(long)ofb - (long)ofa + 1]
	    [(long)unidentified - (long)mismatch + 1];


Static filecorrection offile;
Static typecorrection oftype;
Static long total, subtotal;
Static Char ofletter;


Static stringDelila buffer;
Static boolean gotten;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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



Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}



Static Void readstring(afile, line_)
_TEXT *afile;
stringDelila *line_;
{
  long index;
  Char cha;
  boolean done, acceptable;

  do {
    clearstring(line_);
    prompt(afile);
    index = 0;
    while (!P_eoln(afile->f) && index < maxstring) {
      index++;
      readchar(afile, &cha);
      line_->letters[index-1] = cha;
    }
    if (!P_eoln(afile->f)) {
      printf("type lines shorter than %ld characters.  please retype the line...\n",
	     maxstring + 1L);
      acceptable = false;
    } else
      acceptable = true;
  } while (!acceptable);

  line_->length = index;
  if (line_->length > 0) {
    done = false;
    do {
      /*

*/
      if (line_->letters[line_->length - 1] == ' ')
	line_->length--;
      else
	done = true;

      if (line_->length == 0)
	done = true;
    } while (!done);
  }

  if (line_->length > 0)
    line_->current = 1;
  else
    line_->current = 0;
}


/* Local variables for figurestring: */
struct LOC_figurestring {
  stringDelila *line_;
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
    switch (LINK->line_->letters[i]) {

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



Static Void figurestring(line__, first, last, whzat, c, i, r)
stringDelila *line__;
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


  V.line_ = line__;
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


  WITH = V.line_;

  if (WITH->length == 0 || WITH->current < 1 || WITH->current > WITH->length)
    *whzat = ' ';
  else {
    *first = V.line_->current;
    while (V.line_->letters[*first - 1] == ' ' && *first < V.line_->length)
      (*first)++;
    if (*first == V.line_->length && V.line_->letters[*first - 1] == ' ')
      *whzat = ' ';
  }

  if (*whzat == ' ')
    return;
  *last = *first;
  while (V.line_->letters[*last - 1] != ' ' && *last < V.line_->length)
    (*last)++;
  if (V.line_->letters[*last - 1] == ' ')
    (*last)--;



  *c = V.line_->letters[*first - 1];
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
      if (!P_inset(V.line_->letters[l-1], numbers)) {
	if (V.line_->letters[l-1] == '.') {
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


  V.line_->current = *last + 1;
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









Static Void increment(i)
long *i;
{
  (*i)++;
}


Static Void clearline(aline)
line *aline;
{
  clearstring(&aline->str);
  aline->number = 0;
}


Static Void writeline(afile, aline)
_TEXT *afile;
line aline;
{
  if (aline.str.length > 0) {
    writestring(afile, &aline.str);
    putc('\n', afile->f);
  }
}


Static Void flagline(afile, aline)
_TEXT *afile;
line aline;
{
  long p;

  fprintf(afile->f, "line %ld\n", aline.number);
  writeline(afile, aline);
  for (p = 1; p < aline.str.current; p++)
    putc(blank, afile->f);
  fprintf(afile->f, "^ at %ld\n", aline.str.current);
}


Static Void show(a, b)
line a, b;
{
  _TEXT TEMP;

  printf("--\n");
  printf("file a: ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  flagline(&TEMP, a);

  printf("file b: ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  flagline(&TEMP, b);
}


/* Local variables for fancyshow: */
struct LOC_fancyshow {
  Char thetype;
  long downstream;
} ;

Local Void pre(afile, id, aline, del, LINK)
_TEXT *afile;
Char id;
line aline;
long del;
struct LOC_fancyshow *LINK;
{
  /*

*/
  long p, FORLIM;

  fprintf(afile->f, "file %c: line %ld\n", id, aline.number);
  writeline(afile, aline);
  FORLIM = aline.str.current - del;
  for (p = 1; p < FORLIM; p++)
    putc(blank, afile->f);
}

Local Void post(afile, aline, bline, adel, bdel, LINK)
_TEXT *afile;
line aline, bline;
long adel, bdel;
struct LOC_fancyshow *LINK;
{
  /*
*/
  long pa, pb;
  Char a, b;
  boolean done;

  LINK->downstream = 0;
  pa = aline.str.current + adel;
  pb = bline.str.current + bdel;

  do {
    done = (pa > aline.str.length || pb > bline.str.length);

    if (!done) {
      a = aline.str.letters[pa-1];
      b = bline.str.letters[pb-1];
      done = (a == ' ' || b == ' ');

      if (!done) {
	if (a != b) {
	  putc('x', afile->f);
	  increment(&LINK->downstream);
	} else
	  putc(' ', afile->f);

	increment(&pa);
	increment(&pb);
      }
    }
  } while (!done);

  fprintf(afile->f, " at %ld", aline.str.current);
}

Local Void showdown(afile, LINK)
_TEXT *afile;
struct LOC_fancyshow *LINK;
{
  /*


*/
  if (LINK->downstream <= 0)
    return;
  fprintf(afile->f, ", %ld mismatch", LINK->downstream);
  if (LINK->downstream != 1)
    fprintf(afile->f, "es");
  fprintf(afile->f, " downstream");
}

Local Void showmis(afile, id, aline, bline, LINK)
_TEXT *afile;
Char id;
line aline, bline;
struct LOC_fancyshow *LINK;
{
  pre(afile, id, aline, 0L, LINK);
  putc('^', afile->f);
  post(afile, aline, bline, 1L, 1L, LINK);
  if (LINK->thetype == 'm')
    fprintf(afile->f, " mismatch");
  showdown(afile, LINK);
  putc('\n', afile->f);
}

Local Void showdel(afile, id, aline, bline, adel, bdel, LINK)
_TEXT *afile;
Char id;
line aline, bline;
long adel, bdel;
struct LOC_fancyshow *LINK;
{
  long p;

  if (aline.str.current > 1) {
    pre(afile, id, aline, 1L, LINK);
    putc('>', afile->f);
  } else
    pre(afile, id, aline, 0L, LINK);

  for (p = 1; p <= adel; p++)
    putc('i', afile->f);
  putc('<', afile->f);
  post(afile, aline, bline, adel + 1, bdel + 1, LINK);

  if (adel == 0)
    fprintf(afile->f, " deletion");
  else
    fprintf(afile->f, " insertion");
  showdown(afile, LINK);
  putc('\n', afile->f);
}


Static Void fancyshow(aline, bline, adel, bdel, thetype_)
line aline, bline;
long adel, bdel;
Char thetype_;
{
  struct LOC_fancyshow V;
  _TEXT TEMP;

  V.thetype = thetype_;
  switch (V.thetype) {

  case 'm':
  case 'u':
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showmis(&TEMP, 'a', aline, bline, &V);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showmis(&TEMP, 'b', bline, aline, &V);
    break;

  case 'd':
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showdel(&TEMP, 'a', aline, bline, adel, bdel, &V);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showdel(&TEMP, 'b', bline, aline, bdel, adel, &V);
    break;
  }
}


Static Void readline(afile, aline)
_TEXT *afile;
line *aline;
{
  /*

*/
  boolean skipping = true;
  stringDelila *WITH1;
  _TEXT TEMP;

  /*

*/
  /*


*/
  while (skipping) {
    if (BUFEOF(afile->f)) {
      skipping = false;
      break;
    }
    if (!P_eoln(afile->f)) {
      skipping = false;
      break;
    }
    fscanf(afile->f, "%*[^\n]");
    getc(afile->f);
    increment(&aline->number);
  }


  WITH1 = &aline->str;
  WITH1->length = 0;
  if (!BUFEOF(afile->f)) {
    while (!P_eoln(afile->f) && WITH1->length < maxstring) {
      increment(&WITH1->length);
      WITH1->letters[WITH1->length - 1] = getc(afile->f);
      if (WITH1->letters[WITH1->length - 1] == '\n')
	WITH1->letters[WITH1->length - 1] = ' ';
    }


    increment(&aline->number);
    if (!P_eoln(afile->f)) {
      printf("this line exceeds my buffer:\n");
      WITH1->current = WITH1->length;
      TEMP.f = stdout;
      *TEMP.name = '\0';
      flagline(&TEMP, *aline);
      printf("the excess will be lost.  continuing...\n");
    }

    fscanf(afile->f, "%*[^\n]");
    getc(afile->f);
  }
  WITH1->current = 0;
}


Static Void nextnonblank(afile, apfile, aline)
_TEXT *afile, *apfile;
line *aline;
{
  /*
*/
  boolean done = false;
  stringDelila *WITH1;

  WITH1 = &aline->str;
  do {

    increment(&WITH1->current);
    if (WITH1->current <= WITH1->length)
      done = (WITH1->letters[WITH1->current - 1] != blank);
    else {
      if (WITH1->length > 0)
	writeline(apfile, *aline);


      readline(afile, aline);
    }

    if (WITH1->length == 0)
      done = true;
    /*



*/
  } while (!done);
}




Static Void squack()
{
  printf("that insert would make the line longer than %ld, my buffer length\n",
	 (long)maxstring);
  printf("sorry, i cannot do it.\n");
  printf("your last typed line is lost.\n");
  printf("maybe you could use the break command.\n");
}


Static boolean makewayline(amount, l)
long amount;
line *l;
{
  /*
*/
  boolean Result;
  long p;
  stringDelila *WITH;
  long FORLIM;

  WITH = &l->str;
  if (WITH->length + amount > maxstring) {
    Result = false;
    squack();
    return Result;
  }
  Result = true;

  FORLIM = WITH->current - 1;
  for (p = WITH->length - 1; p >= FORLIM; p--)
    WITH->letters[p + amount] = WITH->letters[p];
  WITH->length += amount;
  return Result;
}


Static Void replaceline(i, l)
line *i, *l;
{
  long p, FORLIM;

  FORLIM = l->str.current + i->str.length;
  for (p = l->str.current; p < FORLIM; p++)
    l->str.letters[p-1] = i->str.letters[p - l->str.current];
}


Static Void copypartline(amount, fromline, toline)
long amount;
line *fromline, *toline;
{
  long p, FORLIM;

  FORLIM = fromline->str.current + amount;
  for (p = fromline->str.current - 1; p < FORLIM; p++)
    toline->str.letters[p - fromline->str.current + toline->str.current] =
      fromline->str.letters[p];
}


Static Void insertinline(i, l)
line *i, *l;
{
  if (makewayline(i->str.length, l))
    replaceline(i, l);
}


Static Void deleteinline(d, l)
long d;
line *l;
{
  long p;
  stringDelila *WITH;
  long FORLIM;

  WITH = &l->str;

  if (d < 0) {
    if (d + WITH->current < 1) {
      d = WITH->current - 1;
      WITH->current = 1;
    } else {
      WITH->current += d;
      d = -d;
    }
  }



  if (d <= 0)
    return;
  if (d + WITH->current > WITH->length) {
    WITH->length = WITH->current - 1;
    return;
  }
  FORLIM = WITH->length - d;
  for (p = WITH->current - 1; p < FORLIM; p++)
    WITH->letters[p] = WITH->letters[p + d];
  WITH->length -= d;
}


#define foresight       10
#define minsight        5

#define goodquality     0.60


/* Local variables for guess: */
struct LOC_guess {
  line aline, bline;
} ;

Local double frame(adeletion, bdeletion, LINK)
long adeletion, bdeletion;
struct LOC_guess *LINK;
{
  /*

*/
  long as, bs;
  long sight = 0, matches = 0;

  as = LINK->aline.str.current + adeletion - 1;
  bs = LINK->bline.str.current + bdeletion - 1;


  if (adeletion == 0 && bdeletion == 0) {
    increment(&as);
    increment(&bs);
  }


  while (sight <= foresight && as <= LINK->aline.str.length &&
	 bs <= LINK->bline.str.length) {
    do {
      increment(&as);
    } while (LINK->aline.str.letters[as-1] == blank &&
	     as <= LINK->aline.str.length);

    do {
      increment(&bs);
    } while (LINK->bline.str.letters[bs-1] == blank &&
	     bs <= LINK->bline.str.length);

    if (as <= LINK->aline.str.length && bs <= LINK->bline.str.length) {
      increment(&sight);
      if (LINK->aline.str.letters[as-1] == LINK->bline.str.letters[bs-1])
	increment(&matches);
    }
  }



  if (sight < minsight)
    return 0.0;
  else {
    return ((double)matches / sight);
    /*
*/
    /*



*/
  }
}





Static Void guess(aline_, bline_, adel, bdel, thetype)
line aline_, bline_;
long *adel, *bdel;
Char *thetype;
{
  /*




















*/
  struct LOC_guess V;

  long besta = 0, bestb = 0;
  /*
*/
  double bestq;

  long a;
  long b = 0;
  double q;


  V.aline = aline_;
  V.bline = bline_;
  bestq = frame(0, 0, &V);


  for (a = 1; a <= foresight; a++) {
    q = frame(a, b, &V);
    if (q > bestq) {
      besta = a;
      bestb = b;
      bestq = q;
    }
  }


  a = 0;
  for (b = 1; b <= foresight; b++) {
    q = frame(a, b, &V);
    if (q > bestq) {
      besta = a;
      bestb = b;
      bestq = q;
    }
  }

  *adel = besta;
  *bdel = bestb;


  if (bestq < goodquality) {
    *thetype = 'u';
    increment(&unid);
    return;
  }
  if (bestq > 0.99)
    bestq = 0.99;

  printf("i am %ld%% sure that this has a ", (long)floor(100 * bestq + 0.5));
  if (besta == 0 && bestb == 0) {
    *thetype = 'm';
    printf("single mismatch:\n");
    increment(&mism);
    return;
  }
  *thetype = 'd';
  printf("deletion in ");
  if (besta == 0)
    printf("a (insertion in b) of %ld", bestb);
  else
    printf("b (insertion in a) of %ld", besta);
  printf(" character");
  if (besta + bestb > 1)
    printf("s:\n");
  else
    printf(":\n");

  increment(&deli);
}

#undef foresight
#undef minsight
#undef goodquality



Static Void insert_(l)
line *l;
{
  line i;
  _TEXT TEMP;

  do {
    if (nostring(&buffer))
      printf("characters to insert: \n");
    TEMP.f = stdin;
    *TEMP.name = '\0';
    gettoken(&TEMP, &buffer, &i.str, &gotten);
  } while (!gotten);

  if (i.str.length != 0)
    insertinline(&i, l);
  else
    printf("no insert\n");
}


Static Void delete__(l)
line *l;
{
  long d;
  _TEXT TEMP;

  do {
    if (nostring(&buffer))
      printf("number of characters to delete\n");
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getinteger(&TEMP, &buffer, &d, &gotten);
  } while (!gotten);

  if (d != 0)
    deleteinline(d, l);
  else
    printf("no deletion\n");
}


Static Void jump(l)
line *l;
{
  long distance;
  _TEXT TEMP;
  stringDelila *WITH;

  do {
    if (nostring(&buffer))
      printf("distance to jump\n");
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getinteger(&TEMP, &buffer, &distance, &gotten);
  } while (!gotten);

  WITH = &l->str;
  WITH->current += distance;
  if (WITH->current < 1)
    WITH->current = 1;
  if (WITH->current > WITH->length + 1)
    WITH->current = WITH->length + 1;
}


Static Void replace(l)
line *l;
{
  line r;
  _TEXT TEMP;

  do {
    if (nostring(&buffer))
      printf("new replacing characters:\n");
    TEMP.f = stdin;
    *TEMP.name = '\0';
    gettoken(&TEMP, &buffer, &r.str, &gotten);
  } while (!gotten);

  if (l->str.current + r.str.length > maxstring)
    squack();
  else
    replaceline(&r, l);
}


Static Void break_(lpfile, l)
_TEXT *lpfile;
line *l;
{
  long p, tc, tl;
  stringDelila *WITH;
  long FORLIM;

  WITH = &l->str;
  if (WITH->current <= 1 || WITH->current > WITH->length)
    return;

  tc = WITH->current;
  tl = WITH->length;


  WITH->current--;
  WITH->length = WITH->current;
  writeline(lpfile, *l);


  WITH->current = tc;
  WITH->length = tl;


  FORLIM = WITH->length - WITH->current + 1;
  for (p = 1; p <= FORLIM; p++)
    WITH->letters[p-1] = WITH->letters[WITH->current + p - 2];
  WITH->length += 1 - WITH->current;
  WITH->current = 1;
}


Static Void modifyhelp()
{
  _TEXT TEMP;

  TEMP.f = stdout;
  *TEMP.name = '\0';
  flagstring(&TEMP, &buffer);
  printf("modification commands are:\n");
  printf("i(nsert)  characters before current spot\n");
  printf("d(elete)  characters starting from just before current spot\n");
  printf("j(ump)    around on the line\n");
  printf("r(eplace) several characters\n");
  printf("b(reak)   line into two lines prior to current spot\n");
  printf("          editing continues for second line only.\n");
  printf("v(erify)  display both lines\n");
  printf("q(uit)    to merge level\n\n");
}


Static Void modifyline(lpfile, l, c)
_TEXT *lpfile;
line *l;
Char c;
{
  Char command = blank;
  _TEXT TEMP;
  stringDelila *WITH;

  while (command != 'q') {
    do {
      if (nostring(&buffer))
	printf("modify\n");
      TEMP.f = stdin;
      *TEMP.name = '\0';
      getchar_(&TEMP, &buffer, &command, &gotten);
    } while (!gotten);

    if (command != 'q' && command != 'v' && command != 'b' &&
	command != 'j' && command != 'r' && command != 'd' && command != 'i')
      modifyhelp();
    else {
      switch (command) {

      case 'i':
	insert_(l);
	break;

      case 'd':
	delete__(l);
	break;

      case 'r':
	replace(l);
	break;

      case 'j':
	jump(l);
	break;

      case 'b':
	break_(lpfile, l);
	break;

      case 'v':
	fancyshow(aline, bline, 0L, 0L, 'u');
	printf("modifying %c\n", c);
	break;

      case 'q':
	/* blank case */
	break;
      }
    }

    if ((command != 'v' && command != 'q') & nostring(&buffer)) {
      printf("--\n");
      printf("file %c: ", c);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      flagline(&TEMP, *l);
    }
  }


  WITH = &l->str;
  if (WITH->current > WITH->length)
    WITH->current = WITH->length + 1;

  if (WITH->length > 0)
    return;

  WITH->length = 0;
  WITH->current = WITH->length + 5;
  switch (c) {

  case 'a':
    nextnonblank(&afile, &apfile, l);
    break;

  case 'b':
    nextnonblank(&bfile, &bpfile, l);
    break;
  }
}


Static Void modify(apfile, bpfile, a, b)
_TEXT *apfile, *bpfile;
line *a, *b;
{
  Char choice;
  _TEXT TEMP;

  do {
    if (nostring(&buffer))
      printf("chose a or b to modify\n");
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getchar_(&TEMP, &buffer, &choice, &gotten);
    if (gotten) {
      if (choice != 'b' && choice != 'a') {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	flagstring(&TEMP, &buffer);
	gotten = false;
      }
    }
  } while (!gotten);

  switch (choice) {

  case 'a':
    increment(&correction[0][(long)unidentified - (long)mismatch]);
    modifyline(apfile, a, choice);
    break;

  case 'b':
    increment(&correction[(long)ofb - (long)ofa]
	      [(long)unidentified - (long)mismatch]);
    modifyline(bpfile, b, choice);
    break;
  }

  fancyshow(*a, *b, 0L, 0L, 'u');
}


/* Local variables for autofix: */
struct LOC_autofix {
  Char *thetype;
} ;

Local Void fix(xdel, ydel, xline, yline, ofy, LINK)
long xdel, ydel;
line *xline, *yline;
filecorrection ofy;
struct LOC_autofix *LINK;
{
  switch (*LINK->thetype) {

  case 'm':
    increment(correction[(long)ofy - (long)ofa]);
    yline->str.letters[yline->str.current - 1] =
      xline->str.letters[xline->str.current - 1];
    break;

  case 'd':
    if (xdel == 0) {
      increment(&correction[(long)ofy - (long)ofa]
		[(long)deletion - (long)mismatch]);
      deleteinline(ydel, yline);
    } else {
      increment(&correction[(long)ofy - (long)ofa]
		[(long)insertion - (long)mismatch]);
      if (makewayline(xdel, yline))
	copypartline(xdel, xline, yline);
    }
    break;
  }
}


Static Void autofix(aline, bline, adel, bdel, thetype_)
line *aline, *bline;
long *adel, *bdel;
Char *thetype_;
{
  /*



*/
  struct LOC_autofix V;
  Char choice;
  _TEXT TEMP;


  V.thetype = thetype_;
  if (*V.thetype == 'u') {
    printf("i do not know how to fix this.\n");
    buffer.length = 0;
  } else if (*V.thetype == 'd' || *V.thetype == 'm') {
    do {
      if (nostring(&buffer))
	printf("which file is correct? (a, b, n(either))\n");
      TEMP.f = stdin;
      *TEMP.name = '\0';
      getchar_(&TEMP, &buffer, &choice, &gotten);
      if (gotten) {
	if (choice != 'n' && choice != 'b' && choice != 'a') {
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  flagstring(&TEMP, &buffer);
	  gotten = false;
	}
      }
    } while (!gotten);

    if (choice == 'b' || choice == 'a') {
      switch (choice) {

      case 'a':
	fix(*adel, *bdel, aline, bline, ofb, &V);
	break;

      case 'b':
	fix(*bdel, *adel, bline, aline, ofa, &V);
	break;
      }
      *V.thetype = 'u';
    } else
      printf("automatic correction aborted.\n");
  } else
    halt();

  fancyshow(*aline, *bline, *adel, *bdel, *V.thetype);
}


Static Void move_(afile, apfile, aline)
_TEXT *afile, *apfile;
line *aline;
{
  writeline(apfile, *aline);


  readline(afile, aline);


  nextnonblank(afile, apfile, aline);
}


Static Void linejump(afile, bfile, apfile, bpfile, a, b)
_TEXT *afile, *bfile, *apfile, *bpfile;
line *a, *b;
{
  Char choice;
  _TEXT TEMP;

  do {
    if (nostring(&buffer))
      printf("line to jump (a, b, 0 (none), or 2 (both))\n");
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getchar_(&TEMP, &buffer, &choice, &gotten);
    if (gotten) {
      if (choice != '2' && choice != '0' && choice != 'b' && choice != 'a') {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	flagstring(&TEMP, &buffer);
	gotten = false;
      }
    }
  } while (!gotten);

  switch (choice) {

  case 'a':
    move_(afile, apfile, a);
    break;

  case 'b':
    move_(bfile, bpfile, b);
    break;

  case '0':
    /* blank case */
    break;

  case '2':
    move_(afile, apfile, a);
    move_(bfile, bpfile, b);
    break;
  }
}


Static Void skipout(thefile, thepfile, theline)
_TEXT *thefile, *thepfile;
line *theline;
{
  while (theline->str.length != 0)
    nextnonblank(thefile, thepfile, theline);
}


Static Void differhelp()
{
  _TEXT TEMP;

  TEMP.f = stdout;
  *TEMP.name = '\0';
  flagstring(&TEMP, &buffer);
  printf("merge commands are:\n");
  printf("c(ont)   continue looking for differences\n");
  printf("f(ix)    automatic correction using the correct line\n");
  printf("j(ump)   over one or both lines (this will ignore any differences)\n");
  printf("m(odify) one of the lines by hand\n");
  printf("n(umber) of characters identical so far is shown.  blanks\n");
  printf("         are ignored.  this helps locate a difference.\n");
  printf("q(uit)   like scan, but do not show anything\n");
  printf("s(can)   ignore all further differences (but show them)\n");
  printf("v(erify) redisplay of both lines\n");
  printf("you can type on one line seperated by spaces:\n");
  printf("m a j 3 r a q\n\n");
}


Static Void different(afile, bfile, apfile, bpfile, aline, bline, done)
_TEXT *afile, *bfile, *apfile, *bpfile;
line *aline, *bline;
boolean *done;
{
  /*


*/
  Char command = blank;
  long adel = 0, bdel = 0;
  Char thetype = ' ';
  boolean finished;
  _TEXT TEMP;
  stringDelila *WITH;

  printf("\n---");

  if (scanning)
    printf(" %ld characters were identical before this difference:\n",
	   characters);
  else {
    printf(" difference:\n");

  }

  guess(*aline, *bline, &adel, &bdel, &thetype);
  fancyshow(*aline, *bline, adel, bdel, thetype);


  clearstring(&buffer);

  while (command != 's' && command != 'q' && command != 'c' && !scanning) {
    do {
      if (nostring(&buffer))
	printf("merge\n");
      TEMP.f = stdin;
      *TEMP.name = '\0';
      getchar_(&TEMP, &buffer, &command, &gotten);
    } while (!gotten);

    if (command != 'j' && command != 'v' && command != 'f' &&
	command != 'n' && command != 'm' && command != 's' &&
	command != 'q' && command != 'c') {
      differhelp();
      continue;
    }
    switch (command) {

    case 'c':
      /* blank case */
      break;

    case 'q':
      clearstring(&buffer);
      do {
	if (nostring(&buffer))
	  printf("do you really want to quit? (y/n)\n");
	TEMP.f = stdin;
	*TEMP.name = '\0';
	getchar_(&TEMP, &buffer, &command, &gotten);
      } while (!gotten);
      if (command == 'y') {
	skipout(afile, apfile, aline);
	skipout(bfile, bpfile, bline);
	printf(
	  "the rest of the input files was copied to their respective output files.\n");
	*done = true;
	command = 'q';
      } else
	command = blank;
      break;

    case 's':
      clearstring(&buffer);
      do {
	if (nostring(&buffer))
	  printf("do you really want to scan? (y/n)\n");
	TEMP.f = stdin;
	*TEMP.name = '\0';
	getchar_(&TEMP, &buffer, &command, &gotten);
      } while (!gotten);
      if (command == 'y')
	scanning = true;
      break;

    case 'm':
      thetype = 'u';
      modify(apfile, bpfile, aline, bline);
      break;

    case 'n':
      printf(" number of characters found identical so far: %ld\n",
	     characters);
      break;

    case 'f':
      autofix(aline, bline, &adel, &bdel, &thetype);
      break;

    case 'v':
      fancyshow(*aline, *bline, adel, bdel, thetype);
      break;

    case 'j':
      linejump(afile, bfile, apfile, bpfile, aline, bline);
      break;
    }
  }

  /*








*/

  if (command == 's' || command == 'c') {
    WITH = &aline->str;
    do {
      WITH->current--;
      if (WITH->current == 0)
	finished = true;
      else
	finished = (WITH->letters[WITH->current - 1] != blank);
    } while (!finished);
    WITH = &bline->str;
    do {
      WITH->current--;
      if (WITH->current == 0)
	finished = true;
      else
	finished = (WITH->letters[WITH->current - 1] != blank);
    } while (!finished);
    return;
  }
  if (!scanning)
    return;

  if (thetype != 'd')
    return;

  WITH = &aline->str;
  WITH->current += adel;
  WITH = &bline->str;
  WITH->current += bdel;
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  bpfile.f = NULL;
  strcpy(bpfile.name, "bpfile");
  apfile.f = NULL;
  strcpy(apfile.name, "apfile");
  bfile.f = NULL;
  strcpy(bfile.name, "bfile");
  afile.f = NULL;
  strcpy(afile.name, "afile");
  printf("merge %4.2f\n", version);

  if (*afile.name != '\0') {
    if (afile.f != NULL)
      afile.f = freopen(afile.name, "r", afile.f);
    else
      afile.f = fopen(afile.name, "r");
  } else
    rewind(afile.f);
  if (afile.f == NULL)
    _EscIO2(FileNotFound, afile.name);
  RESETBUF(afile.f, Char);
  if (*bfile.name != '\0') {
    if (bfile.f != NULL)
      bfile.f = freopen(bfile.name, "r", bfile.f);
    else
      bfile.f = fopen(bfile.name, "r");
  } else
    rewind(bfile.f);
  if (bfile.f == NULL)
    _EscIO2(FileNotFound, bfile.name);
  RESETBUF(bfile.f, Char);
  if (*apfile.name != '\0') {
    if (apfile.f != NULL)
      apfile.f = freopen(apfile.name, "w", apfile.f);
    else
      apfile.f = fopen(apfile.name, "w");
  } else {
    if (apfile.f != NULL)
      rewind(apfile.f);
    else
      apfile.f = tmpfile();
  }
  if (apfile.f == NULL)
    _EscIO2(FileNotFound, apfile.name);
  SETUPBUF(apfile.f, Char);
  if (*bpfile.name != '\0') {
    if (bpfile.f != NULL)
      bpfile.f = freopen(bpfile.name, "w", bpfile.f);
    else
      bpfile.f = fopen(bpfile.name, "w");
  } else {
    if (bpfile.f != NULL)
      rewind(bpfile.f);
    else
      bpfile.f = tmpfile();
  }
  if (bpfile.f == NULL)
    _EscIO2(FileNotFound, bpfile.name);
  SETUPBUF(bpfile.f, Char);
  scanning = false;
  characters = 0;
  mism = 0;
  deli = 0;
  unid = 0;
  for (offile = ofa;
       (long)offile <= (long)ofb;
       offile = (filecorrection)((long)offile + 1)) {
    for (oftype = mismatch;
	 (long)oftype <= (long)unidentified;
	 oftype = (typecorrection)((long)oftype + 1))
      correction[(long)offile - (long)ofa][(long)oftype - (long)mismatch] = 0;
  }

  clearline(&aline);
  clearline(&bline);

  readline(&afile, &aline);
  readline(&bfile, &bline);

  done = false;
  while (!done) {
    nextnonblank(&afile, &apfile, &aline);
    nextnonblank(&bfile, &bpfile, &bline);


    if (aline.str.length == 0)
      done = true;
    if (bline.str.length == 0)
      done = true;

    if (done) {
      if ((aline.str.length != 0 || bline.str.length == 0) &&
	  (aline.str.length == 0 || bline.str.length != 0))
	continue;

      printf("--\n");


      if (aline.str.length == 0)
	printf("first");
      else
	printf("second");
      printf(" file ended early.  ");


      printf("the other file is at ");
      if (aline.str.length == 0) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	flagline(&TEMP, bline);
      } else {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	flagline(&TEMP, aline);
      }
      continue;
    }


    if (aline.str.letters[aline.str.current - 1] !=
	bline.str.letters[bline.str.current - 1]) {
      different(&afile, &bfile, &apfile, &bpfile, &aline, &bline, &done);
      if (scanning)
	increment(&characters);
    } else
      increment(&characters);
  }


  printf("\n%ld non-blank characters were identical.\n\n", characters);

  if (mism + deli + unid == 0)
    printf("no differences\n");
  else {
    printf("%5ld mismatches\n", mism);
    printf("%5ld deletion/insertions\n", deli);
    printf("%5ld unidentified changes\n", unid);
    printf("=====\n");
    printf("%5ld detected differences\n\n", mism + deli + unid);


    total = 0;
    for (offile = ofa;
	 (long)offile <= (long)ofb;
	 offile = (filecorrection)((long)offile + 1)) {
      for (oftype = mismatch;
	   (long)oftype <= (long)unidentified;
	   oftype = (typecorrection)((long)oftype + 1))
	total += correction[(long)offile - (long)ofa]
	  [(long)oftype - (long)mismatch];
    }

    if (total > 0) {
      printf("corrections made:\n");
      printf("to file   mismatch   deletion insertion unidentified total\n");
      ofletter = 'a';
      for (offile = ofa;
	   (long)offile <= (long)ofb;
	   offile = (filecorrection)((long)offile + 1)) {
	printf("    %c", ofletter);
	total = 0;
	for (oftype = mismatch;
	     (long)oftype <= (long)unidentified;
	     oftype = (typecorrection)((long)oftype + 1)) {
	  printf(" %9ld", correction[(long)offile - (long)ofa]
			   [(long)oftype - (long)mismatch]);
	  total += correction[(long)offile - (long)ofa]
	    [(long)oftype - (long)mismatch];
	}
	printf(" %9ld\n", total);
	ofletter++;
      }
      total = 0;
      printf("total");
      for (oftype = mismatch;
	   (long)oftype <= (long)unidentified;
	   oftype = (typecorrection)((long)oftype + 1)) {
	subtotal = correction[0]
	    [(long)oftype - (long)mismatch] + correction[(long)ofb - (long)ofa]
	    [(long)oftype - (long)mismatch];
	printf(" %9ld", subtotal);
	total += subtotal;
      }
      printf(" %9ld\n", total);
    } else
      printf("no corrections made\n");

    printf("\ninput files were not modified.\n");
    printf("default output is in apfile and bpfile.\n");
  }
_L1:
  if (afile.f != NULL)
    fclose(afile.f);
  if (bfile.f != NULL)
    fclose(bfile.f);
  if (apfile.f != NULL)
    fclose(apfile.f);
  if (bpfile.f != NULL)
    fclose(bpfile.f);
  exit(EXIT_SUCCESS);
}



/* End. */
