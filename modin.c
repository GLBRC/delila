/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "modin.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.45
/*



*/



/*































*/

/*
*/


/*






























*/




#define fillermax       20


#define maxstring       150



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



/*


*/
typedef Char filler[fillermax];


Static _TEXT fin, inst;
Static boolean got;
Static stringDelila buff, tok;


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



Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}


#define fsversion       2.01


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

#undef fsversion






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



Static Void gettoken(afile, buffer, token, gotten)
_TEXT *afile;
stringDelila *buffer, *token;
boolean *gotten;
{
  long first, last;
  Char what, cha;
  long int_;
  double rea;
  long index;

  if (buffer->length == 0) {
    *gotten = false;
    readstring(afile, buffer);
    return;
  }
  figurestring(buffer, &first, &last, &what, &cha, &int_, &rea);

  if (what == ' ') {
    *gotten = false;
    return;
  }
  clearstring(token);
  for (index = first; index <= last; index++)
    token->letters[index - first] = buffer->letters[index-1];
  token->length = last - first + 1;
  token->current = 1;

  *gotten = true;
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

  if (buffer->length == 0) {
    *gotten = false;
    readstring(afile, buffer);
    return;
  }
  figurestring(buffer, &first, &last, &what, &cha, int_, &rea);

  if (what != 'i') {
    /*

*/
    *gotten = false;


    /*
*/

  } else
    *gotten = true;
}


#define maxcompare      150



Static boolean isstring(s, a)
stringDelila *s;
Char *a;
{
  boolean Result = true;

  Char compare[maxcompare];
  long counter = 1;


  memcpy(compare, a, maxcompare);
  while (s->letters[counter-1] != ' ' || compare[counter-1] != ' ') {
    if (s->letters[counter-1] != compare[counter-1])
      Result = false;
    counter++;
  }
  return Result;
}

#undef maxcompare



Static Void setprocess(infile, buffer, outfile, from, upto, setdone)
_TEXT *infile;
stringDelila *buffer;
_TEXT *outfile;
long *from, *upto;
boolean *setdone;
{
  /*
*/
  /*


*/

  /*
*/
  long counter, int_;
  stringDelila organismstructure, token;
  boolean gotten;



  if (nostring(buffer)) {
    *setdone = false;

    return;
  }
  gettoken(infile, buffer, &token, &gotten);

  if (!isstring(&token, "set                 ")) {
    *setdone = false;

    return;
  }
  for (counter = 1; counter <= 3; counter++) {
    if (nostring(buffer)) {
      *setdone = false;

    } else {
      gettoken(infile, buffer, &token, &gotten);

      switch (counter) {

      case 1:
	fillstring(&organismstructure, "organism            ");
	break;

      case 2:
	fillstring(&organismstructure, " chromosome         ");
	break;

      case 3:
	if (token.letters[0] == 'p')
	  fillstring(&organismstructure, " piece              ");
	else if (token.letters[0] == 'g')
	  fillstring(&organismstructure, " gene               ");
	else if (token.letters[0] == 't')
	  fillstring(&organismstructure, " transcript         ");
	else
	  halt();
	gettoken(infile, buffer, &token, &gotten);
	break;
      }

      writestring(outfile, &organismstructure);
      putc(' ', outfile->f);
      writestring(outfile, &token);
      putc(';', outfile->f);
    }
  }



  fprintf(outfile->f, "\n\n");

  if (nostring(buffer)) {
    *setdone = false;

    return;
  }
  getinteger(infile, buffer, &int_, &gotten);

  if (!gotten) {
    *setdone = false;

    return;
  }
  *from = int_;

  if (nostring(buffer)) {
    *setdone = false;




    return;
  }
  getinteger(infile, buffer, &int_, &gotten);
  if (!gotten)
    *setdone = false;
  else {
    *upto = int_;


  }
}



Static Void modparprocess(infile, buffer, from, upto, outfile, modpardone)
_TEXT *infile;
stringDelila *buffer;
long from, upto;
_TEXT *outfile;
boolean *modpardone;
{
  /*
*/
  /*
*/


  /*
*/
  long position;
  stringDelila token;
  boolean minus = false;
  boolean gotint, gottok;

  /*
*/


  *modpardone = true;

  do {
    if (!nostring(buffer)) {
      getinteger(infile, buffer, &position, &gotint);
      if (!gotint) {
	buffer->current = 1;
	gettoken(infile, buffer, &token, &gottok);
	if (isstring(&token, "m                   "))
	  minus = true;
	else
	  *modpardone = false;
	if (nostring(buffer))
	  *modpardone = false;
	else
	  getinteger(infile, buffer, &position, &gotint);
      }

      if (gotint) {
	if (nostring(buffer))
	  *modpardone = false;
	else
	  gettoken(infile, buffer, &token, &gottok);
	fprintf(outfile->f, "(* begin module ");
	writestring(outfile, &token);
	fprintf(outfile->f, " *)\n");

	fprintf(outfile->f, "get from %ld ", position);

	if (from >= 0)
	  putc('+', outfile->f);
	fprintf(outfile->f, "%ld", from);
	fprintf(outfile->f, " to %ld ", position);

	if (upto >= 0)
	  putc('+', outfile->f);
	fprintf(outfile->f, "%ld", upto);
	fprintf(outfile->f, " direction ");

	if (minus)
	  fprintf(outfile->f, "-;\n");
	else
	  fprintf(outfile->f, "+;\n");



	fprintf(outfile->f, "(* end module ");
	writestring(outfile, &token);
	fprintf(outfile->f, " *)\n");

      }

      else
	*modpardone = false;
    } else
      *modpardone = false;
  } while (!nostring(buffer));
}



Static Void modinorder(infile, outfile)
_TEXT *infile, *outfile;
{
  /*
*/
  /*

*/
  stringDelila buffer, token;
  long linenumber = 0, from = 0, upto = 0;
  boolean gotten = false, setdone = true, modpardone = true;


  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else
    rewind(infile->f);
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);
  if (*outfile->name != '\0') {
    if (outfile->f != NULL)
      outfile->f = freopen(outfile->name, "w", outfile->f);
    else
      outfile->f = fopen(outfile->name, "w");
  } else {
    if (outfile->f != NULL)
      rewind(outfile->f);
    else
      outfile->f = tmpfile();
  }
  if (outfile->f == NULL)
    _EscIO2(FileNotFound, outfile->name);
  SETUPBUF(outfile->f, Char);





  if (BUFEOF(infile->f)) {
    printf(" input file is empty.\n");
    halt();
  }




  getstring(infile, &buffer, &gotten);
  linenumber++;
  gettoken(infile, &buffer, &token, &gotten);

  if (isstring(&token, "title               ")) {
    buffer.current = 1;
    writestring(outfile, &buffer);
    fprintf(outfile->f, ";\n");
  }

  else {
    printf(" title line needed in input file.\n");
    halt();
  }


  fprintf(outfile->f, "default out-of-range reduce-range;\n");

  getstring(infile, &buffer, &gotten);


  do {
    linenumber++;
    setprocess(infile, &buffer, outfile, &from, &upto, &setdone);
    if (setdone) {
      do {
	getstring(infile, &buffer, &gotten);
	linenumber++;
	modparprocess(infile, &buffer, from, upto, outfile, &modpardone);
	putc('\n', outfile->f);
	setdone = false;

      } while (modpardone);
    } else {
      printf("format error, line %ld of input file.\n", linenumber);
      if (*outfile->name != '\0') {
	if (outfile->f != NULL)
	  outfile->f = freopen(outfile->name, "w", outfile->f);
	else
	  outfile->f = fopen(outfile->name, "w");
      } else {
	if (outfile->f != NULL)
	  rewind(outfile->f);
	else
	  outfile->f = tmpfile();
      }
      if (outfile->f == NULL)
	_EscIO2(FileNotFound, outfile->name);
      SETUPBUF(outfile->f, Char);
      halt();
    }
  } while (!BUFEOF(infile->f));

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  inst.f = NULL;
  strcpy(inst.name, "inst");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  printf(" modin %4.2f\n", version);
  modinorder(&fin, &inst);
  printf(" instructions have been written.\n");
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (inst.f != NULL)
    fclose(inst.f);
  exit(EXIT_SUCCESS);
}



/* End. */
