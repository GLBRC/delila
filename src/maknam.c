/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "maknam.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.07
/*
*/



/*






























*/



#define maxstring       150



#define fillermax       32



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



Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
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


Local Void nope()
{
  printf("MAKNAM: name line not found\n");
  halt();
}



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  stringDelila aline;
  trigger bmo;
  boolean gotten;
  long skip;



  filltrigger(&bmo, "(@ begin module describe.       ");
  /*
*/
  bmo.seek.letters[1] = '*';

  resettrigger(&bmo);

  while (!BUFEOF(fin->f)) {
    getstring(fin, &aline, &gotten);


    while (aline.current < aline.length && !bmo.skip && !bmo.found) {
      testfortrigger(aline.letters[aline.current - 1], &bmo);
      aline.current++;
    }
    if (bmo.found) {
      for (skip = 1; skip <= 3; skip++) {
	while (P_peek(fin->f) == ' ')
	  getc(fin->f);
	getstring(fin, &aline, &gotten);
	if (!gotten)
	  nope();
      }

      writestring(fout, &aline);
      putc('\n', fout->f);
    }
    resettrigger(&bmo);
  }
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