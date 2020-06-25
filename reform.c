/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "reform.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/



#define version         1.21
/*
*/



/*









































*/



#define maxseq          1000
/*


*/


#define maxstring       150



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;


Static _TEXT fin, fout;
Static Char operation;
Static long linelength;


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


Static Void getcontrols(operation, linelength)
Char *operation;
long *linelength;
{
  stringDelila buffer;
  boolean gotten;
  _TEXT TEMP;

  clearstring(&buffer);

  do {
    if (nostring(&buffer))
      printf("reformat, invert, complement (r/i/c)\n");
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getchar_(&TEMP, &buffer, operation, &gotten);
    if (gotten) {
      if (*operation != 'c' && *operation != 'i' && *operation != 'r') {
	gotten = false;
	clearstring(&buffer);
	printf("type one of these letters: r i c\n");
      }
    }
  } while (!gotten);

  do {
    if (nostring(&buffer))
      printf("number of bases per line:\n");
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getinteger(&TEMP, &buffer, linelength, &gotten);

    if (gotten) {
      if (*linelength < 1) {
	printf("length must be positive\n");
	gotten = false;
      }
    }
  } while (!gotten);
}


typedef enum {
  a, c, g, t
} base;


typedef struct seq {
/* p2c: reform.p, line 585:
 * Note: Field width for bases assumes enum base has 4 elements [105] */
  uchar bases[(maxseq + 3) / 4];
  long place;
  struct seq *next;
} seq;


/* Local variables for dooperation: */
struct LOC_dooperation {
  _TEXT *fin, *fout;
  long linelength, count;
} ;


Local base chartobase(ch, LINK)
Char ch;
struct LOC_dooperation *LINK;
{
  base Result;

  switch (ch) {

  case 'a':
    Result = a;
    break;

  case 'c':
    Result = c;
    break;

  case 'g':
    Result = g;
    break;

  case 't':
    Result = t;
    break;
  }
  return Result;
}

Local Char basetochar(ba, LINK)
base ba;
struct LOC_dooperation *LINK;
{
  Char Result;

  switch (ba) {

  case a:
    Result = 'a';
    break;

  case c:
    Result = 'c';
    break;

  case g:
    Result = 'g';
    break;

  case t:
    Result = 't';
    break;
  }
  return Result;
}

Local base complement(ba, LINK)
base ba;
struct LOC_dooperation *LINK;
{
  base Result;

  switch (ba) {

  case a:
    Result = t;
    break;

  case c:
    Result = g;
    break;

  case g:
    Result = c;
    break;

  case t:
    Result = a;
    break;
  }
  return Result;
}


Local Void findnonblank(afile, ch, LINK)
_TEXT *afile;
Char *ch;
struct LOC_dooperation *LINK;
{
  *ch = ' ';
  while (!BUFEOF(afile->f) && *ch == ' ') {
    *ch = getc(afile->f);
    if (*ch == '\n')
      *ch = ' ';
    if (P_eoln(afile->f)) {
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
    }
  }
}


Local boolean getbase(b, LINK)
base *b;
struct LOC_dooperation *LINK;
{
  /*
*/
  Char ch;

  findnonblank(LINK->fin, &ch, LINK);


  if (ch != ' ') {
    if (ch == 'u')
      ch = 't';
    if (ch != 't' && ch != 'g' && ch != 'c' && ch != 'a') {
      printf("the character \"%c\" is not allowed in a raw sequence file...\n",
	     ch);
      halt();
    }
    *b = chartobase(ch, LINK);
    return true;
  } else
    return false;
}

Local Void putbase(b, LINK)
base b;
struct LOC_dooperation *LINK;
{
  fputc(basetochar(b, LINK), LINK->fout->f);
  LINK->count++;
  if (LINK->count % LINK->linelength == 0)
    putc('\n', LINK->fout->f);
}

Local Void rawreformat(LINK)
struct LOC_dooperation *LINK;
{
  base b;

  while (!BUFEOF(LINK->fin->f)) {
    if (getbase(&b, LINK))
      putbase(b, LINK);
  }
}

Local Void rawinvert(LINK)
struct LOC_dooperation *LINK;
{
  base b;

  while (!BUFEOF(LINK->fin->f)) {
    if (getbase(&b, LINK))
      putbase(complement(b, LINK), LINK);
  }
}

Local Void rawcomplement(LINK)
struct LOC_dooperation *LINK;
{
  seq *root, *newroot;
  /*
*/
  base b;
  long index;
  seq *WITH;
  long TEMP;

  root = (seq *)Malloc(sizeof(seq));

  root->next = NULL;
  root->place = maxseq;
  /*
*/
  while (!BUFEOF(LINK->fin->f)) {
    if (!getbase(&b, LINK))
      continue;
    b = complement(b, LINK);


    if (root->place == 0) {
      root->place = 1;


      newroot = (seq *)Malloc(sizeof(seq));
      newroot->next = root;
      newroot->place = maxseq;
      root = newroot;
    }


    TEMP = root->place - 1;
    P_clrbits_B(root->bases, TEMP, 1, 3);
    P_putbits_UB(root->bases, TEMP, (int)b, 1, 3);
    root->place--;
  }



  root->place++;
  /*

*/

  while (root != NULL) {
    WITH = root;
    for (index = WITH->place - 1; index < maxseq; index++)
      putbase((base)P_getbits_UB(WITH->bases, index, 1, 3), LINK);


    root = root->next;
  }
}


Static Void dooperation(fin_, fout_, operation, linelength_)
_TEXT *fin_, *fout_;
Char operation;
long linelength_;
{
  /*



*/
  struct LOC_dooperation V;

  V.fin = fin_;
  V.fout = fout_;
  V.linelength = linelength_;
  V.count = 0;

  switch (operation) {

  case 'r':
    rawreformat(&V);
    break;

  case 'i':
    rawinvert(&V);
    break;

  case 'c':
    rawcomplement(&V);
    break;
  }


  if (V.count % V.linelength != 0)
    putc('\n', V.fout->f);

  printf(" %ld bases ", V.count);
  switch (operation) {

  case 'r':
    printf("reformatted\n");
    break;

  case 'i':
    printf("inverted\n");
    break;

  case 'c':
    printf("complemented\n");
    break;
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fout.f = NULL;
  strcpy(fout.name, "fout");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  printf("reform %4.2f\n", version);
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

  getcontrols(&operation, &linelength);
  dooperation(&fin, &fout, operation, linelength);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
