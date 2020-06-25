/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "scale.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.07
/*








*/
#define updateversion   1.00



/*
























































*/



#define maxstring       2000




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;


Static _TEXT fin, scalep, fout;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
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



Static Void grabtoken(thefile, thestring)
_TEXT *thefile;
stringDelila *thestring;
{
  Char c;
  boolean done = false;

  skipblanks(thefile);
  thestring->length = 0;
  while (!done) {
    if (P_eoln(thefile->f)) {
      done = true;
      break;
    }
    c = getc(thefile->f);
    if (c == '\n')
      c = ' ';
    if (c == ' ')
      done = true;
    else {
      thestring->length++;
      thestring->letters[thestring->length - 1] = c;
    }
  }
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



Static Void copyalineNOln(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
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




Static Void skipstringblanks(line)
stringDelila *line;
{
  while (line->letters[line->current - 1] == ' ' && line->current < maxstring)
    line->current++;
}


Static Void skipstringnonblanks(line)
stringDelila *line;
{
  while (line->letters[line->current - 1] != ' ' && line->current < maxstring)
    line->current++;
}


Static Void skipstringcolumn(line)
stringDelila *line;
{
  skipstringblanks(line);
  skipstringnonblanks(line);
}


/*

*/

#define debug           false


Static Void stringcolumn(line, column, datum)
stringDelila line;
long column;
stringDelila *datum;
{
  /*
*/
  long c;
  long columnnumber = 0;
  boolean done = false;
  long positionbeg, positionend;
  _TEXT TEMP;
  long FORLIM;

  if (debug) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &line);
    putchar('\n');
  }
  while (!done) {
    if (columnnumber == column || line.current >= line.length) {
      done = true;
      break;
    }
    skipstringblanks(&line);
    positionbeg = line.current;
    if (debug)
      printf("%*c %ld", (int)line.current, '|', line.current);
    if (debug)
      printf("\"%c\"", line.letters[positionbeg-1]);
    if (debug)
      putchar('\n');

    skipstringnonblanks(&line);
    positionend = line.current - 1;
    if (debug)
      printf("%*c %ld", (int)line.current, '|', line.current);
    if (debug)
      printf("\"%c\"", line.letters[positionend-1]);
    if (debug)
      putchar('\n');

    columnnumber++;
  }
  clearstring(datum);
  if (columnnumber == column) {
    datum->length = positionend - positionbeg + 1;
    FORLIM = datum->length;
    for (c = 1; c <= FORLIM; c++) {
      if (debug)
	putchar(line.letters[positionbeg + c - 2]);
      datum->letters[c-1] = line.letters[positionbeg + c - 2];
    }
  }
  if (debug)
    putchar('\n');
  /*

*/
}

#undef debug


#define verbose         false




Static Void themain(fin, scalep, fout)
_TEXT *fin, *scalep, *fout;
{
  double parameterversion;

  long inputcolumn;
  double lowerbound, upperbound, multiplier, adder;
  long colwid, coldec;
  Char newcolumn;

  double datum;
  /*
*/


  stringDelila line;
  long i;

  boolean gotten;
  stringDelila token;

  _TEXT internalfile;
  /*
*/

  long kept = 0, skipped = 0;
  _TEXT TEMP;

  internalfile.f = NULL;
  *internalfile.name = '\0';
  printf("scale %4.2f\n", version);
  if (*scalep->name != '\0') {
    if (scalep->f != NULL)
      scalep->f = freopen(scalep->name, "r", scalep->f);
    else
      scalep->f = fopen(scalep->name, "r");
  } else
    rewind(scalep->f);
  if (scalep->f == NULL)
    _EscIO2(FileNotFound, scalep->name);
  RESETBUF(scalep->f, Char);
  fscanf(scalep->f, "%lg%*[^\n]", &parameterversion);
  getc(scalep->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(scalep->f, "%ld%*[^\n]", &inputcolumn);
  getc(scalep->f);
  fscanf(scalep->f, "%lg%*[^\n]", &lowerbound);
  getc(scalep->f);
  fscanf(scalep->f, "%lg%*[^\n]", &upperbound);
  getc(scalep->f);
  fscanf(scalep->f, "%lg%*[^\n]", &multiplier);
  getc(scalep->f);
  fscanf(scalep->f, "%lg%*[^\n]", &adder);
  getc(scalep->f);
  fscanf(scalep->f, "%ld%*[^\n]", &colwid);
  getc(scalep->f);
  fscanf(scalep->f, "%ld%*[^\n]", &coldec);
  getc(scalep->f);
  fscanf(scalep->f, "%c%*[^\n]", &newcolumn);
  getc(scalep->f);

  if (newcolumn == '\n')
    newcolumn = ' ';
  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else {

    rewind(fin->f);
  }
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

  while (!BUFEOF(fin->f)) {
    if (P_peek(fin->f) == '*') {
      copyaline(fin, fout);
      continue;
    }
    if (*internalfile.name != '\0') {
      if (internalfile.f != NULL)
	internalfile.f = freopen(internalfile.name, "w", internalfile.f);
      else
	internalfile.f = fopen(internalfile.name, "w");
    } else {
      if (internalfile.f != NULL)
	rewind(internalfile.f);
      else
	internalfile.f = tmpfile();
    }
    if (internalfile.f == NULL)
      _EscIO2(FileNotFound, internalfile.name);
    SETUPBUF(internalfile.f, Char);
    for (i = 1; i < inputcolumn; i++) {
      skipblanks(fin);
      grabtoken(fin, &token);

      if (i > 1)
	putc(' ', internalfile.f);
      writestring(&internalfile, &token);
    }


    fscanf(fin->f, "%lg", &datum);

    if (lowerbound <= datum && datum <= upperbound) {
      kept++;
      if (verbose)
	printf("----- kept %ld\n", kept);


      /*

*/
      datum = multiplier * (datum + adder);

      fprintf(internalfile.f, " %*.*f", (int)colwid, (int)coldec, datum);

      copyalineNOln(fin, &internalfile);
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      putc('\n', internalfile.f);
      if (*internalfile.name != '\0') {
	if (internalfile.f != NULL)
	  internalfile.f = freopen(internalfile.name, "r", internalfile.f);
	else
	  internalfile.f = fopen(internalfile.name, "r");
      } else
	rewind(internalfile.f);
      if (internalfile.f == NULL)
	_EscIO2(FileNotFound, internalfile.name);
      RESETBUF(internalfile.f, Char);
      copyaline(&internalfile, fout);
      if (*internalfile.name != '\0') {
	if (internalfile.f != NULL)
	  internalfile.f = freopen(internalfile.name, "r", internalfile.f);
	else
	  internalfile.f = fopen(internalfile.name, "r");
      } else
	rewind(internalfile.f);
      if (internalfile.f == NULL)
	_EscIO2(FileNotFound, internalfile.name);
      RESETBUF(internalfile.f, Char);
      if (verbose) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	copyaline(&internalfile, &TEMP);
      }
      continue;
    }
    skipped++;
    if (!verbose) {
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      continue;
    }
    printf("----- skipped %ld\n", skipped);
    if (*internalfile.name != '\0') {
      if (internalfile.f != NULL)
	internalfile.f = freopen(internalfile.name, "r", internalfile.f);
      else
	internalfile.f = fopen(internalfile.name, "r");
    } else
      rewind(internalfile.f);
    if (internalfile.f == NULL)
      _EscIO2(FileNotFound, internalfile.name);
    RESETBUF(internalfile.f, Char);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    copyalineNOln(&internalfile, &TEMP);
    printf(" [skipping %*.*f]", (int)colwid, (int)coldec, datum);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    copyaline(fin, &TEMP);
  }




  printf("skipped: %5ld\n", skipped);
  printf("   kept: %5ld\n", kept);
  printf("  total: %5ld\n", skipped + kept);

  if (internalfile.f != NULL)
    fclose(internalfile.f);
}

#undef verbose


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fout.f = NULL;
  strcpy(fout.name, "fout");
  scalep.f = NULL;
  strcpy(scalep.name, "scalep");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &scalep, &fout);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (scalep.f != NULL)
    fclose(scalep.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
