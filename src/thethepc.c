/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "thethepc.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         2.01
/*


*/



/*







































*/



#define onlycharacters  true
/*
*/
#define caseindependent  true


#define maxstring       1024



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



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



Static Void decapitilize(c)
Char *c;
{
  long n;

  n = *c;

  if (n >= 'A' && n <= 'Z')
    *c = _tolower(n);
}



Static Void decapstring(t)
stringDelila *t;
{
  long i, FORLIM;

  FORLIM = t->length;
  for (i = 0; i < FORLIM; i++)
    decapitilize(&t->letters[i]);
}



Static boolean alphabetic(c)
Char *c;
{
  long n;

  n = *c;

  if (n >= 'A' && n <= 'Z' || n >= 'a' && n <= 'z')
    return true;
  else
    return false;
}



Static Void whatisthere(t, charsok)
stringDelila t;
boolean *charsok;
{
  /*
*/
  long i, FORLIM;

  *charsok = true;
  FORLIM = t.length;

  /*
*/
  for (i = 0; i < FORLIM; i++)
    *charsok &= alphabetic(&t.letters[i]);
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *infile, *outfile;
  boolean gotten;
  long no, lasttokenline;

  /*
*/
  boolean bonefirst;
  stringDelila bufferone, buffertwo;

  /*
*/
  boolean tonefirst;
  stringDelila tokenone, tokentwo;

  boolean charsok;
} ;

/*


*/

Local Void comparetokens(buffer, LINK)
stringDelila *buffer;
struct LOC_themain *LINK;
{
  getstring(LINK->infile, buffer, &LINK->gotten);

  if (!LINK->gotten)
    return;
  buffer->current = 1;

  while (buffer->current <= buffer->length && LINK->gotten == true) {
    if (LINK->tonefirst) {
      gettoken(LINK->infile, buffer, &LINK->tokenone, &LINK->gotten);
      if (caseindependent)
	decapstring(&LINK->tokenone);
      if (onlycharacters)
	whatisthere(LINK->tokenone, &LINK->charsok);
    } else {
      gettoken(LINK->infile, buffer, &LINK->tokentwo, &LINK->gotten);
      if (caseindependent)
	decapstring(&LINK->tokentwo);
      if (onlycharacters)
	whatisthere(LINK->tokentwo, &LINK->charsok);
    }

    if (!LINK->gotten)
      break;
    /*









*/

    if (equalstring(LINK->tokenone, LINK->tokentwo) && LINK->charsok) {
      fprintf(LINK->outfile->f, "------------\n");
      fprintf(LINK->outfile->f, "duplicated token: \"");
      writestring(LINK->outfile, &LINK->tokenone);
      fprintf(LINK->outfile->f, "\"\n");

      if (LINK->no != LINK->lasttokenline) {
	fprintf(LINK->outfile->f, "Found across lines %ld-%ld:\n",
		LINK->lasttokenline, LINK->no);
	if (LINK->bonefirst)
	  writestring(LINK->outfile, &LINK->buffertwo);
	else
	  writestring(LINK->outfile, &LINK->bufferone);
	putc('\n', LINK->outfile->f);
	if (LINK->bonefirst)
	  writestring(LINK->outfile, &LINK->bufferone);
	else
	  writestring(LINK->outfile, &LINK->buffertwo);
	putc('\n', LINK->outfile->f);
	if (LINK->lasttokenline < LINK->no - 1)
	  fprintf(LINK->outfile->f, "(only the last two lines are shown)\n");
      } else {
	fprintf(LINK->outfile->f, "Found in line %ld:\n", LINK->no);
	writestring(LINK->outfile, buffer);
	putc('\n', LINK->outfile->f);
      }
    }

    LINK->tonefirst = !LINK->tonefirst;
    LINK->lasttokenline = LINK->no;
  }
}



Static Void themain(infile_, outfile_)
_TEXT *infile_, *outfile_;
{
  struct LOC_themain V;

  V.infile = infile_;
  V.outfile = outfile_;
  fprintf(V.outfile->f, "thethepc %4.2f\n", version);
  V.bonefirst = true;
  V.tonefirst = true;
  V.no = 0;
  V.lasttokenline = V.no;
  V.charsok = true;

  while (!BUFEOF(V.infile->f)) {
    V.no++;

    if (V.bonefirst)
      comparetokens(&V.bufferone, &V);
    else
      comparetokens(&V.buffertwo, &V);

    V.bonefirst = !V.bonefirst;
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
