/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "datarange.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.09
/*












*/
#define updateversion   1.08



/*























































*/



#define maxstring       2000




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;


Static _TEXT datain, datarangep, dataout;


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
  emptystring(ribbon);
  ribbon->next = NULL;
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




Static Void stringtoreal(datum, rea, gotten)
stringDelila datum;
double *rea;
boolean *gotten;
{
  /*
*/
  long first, last;
  Char what, cha;
  long int_;

  if (datum.length == 0) {
    *gotten = false;
    *rea = LONG_MAX;
    return;
  }
  datum.current = 1;
  figurestring(&datum, &first, &last, &what, &cha, &int_, rea);
  if (what != 'i' && what != 'r') {
    *gotten = false;
    return;
  }
  *gotten = true;
  if (what == 'i')
    *rea = int_;
}


#define wid             10
#define dec             5



Static Void themain(datain, datarangep, dataout)
_TEXT *datain, *datarangep, *dataout;
{
  long column;
  stringDelila datum;
  boolean delete_;
  Char deleteinclude;
  boolean gotten, include;
  double mincut, maxcut;
  stringDelila line;
  long linenumber;
  double parameterversion, thedatum;
  _TEXT TEMP;

  printf("datarange %4.2f\n", version);
  if (*datarangep->name != '\0') {
    if (datarangep->f != NULL)
      datarangep->f = freopen(datarangep->name, "r", datarangep->f);
    else
      datarangep->f = fopen(datarangep->name, "r");
  } else
    rewind(datarangep->f);
  if (datarangep->f == NULL)
    _EscIO2(FileNotFound, datarangep->name);
  RESETBUF(datarangep->f, Char);
  fscanf(datarangep->f, "%lg%*[^\n]", &parameterversion);
  getc(datarangep->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(datarangep->f, "%ld%*[^\n]", &column);
  getc(datarangep->f);
  fscanf(datarangep->f, "%lg%*[^\n]", &mincut);
  getc(datarangep->f);
  fscanf(datarangep->f, "%lg%*[^\n]", &maxcut);
  getc(datarangep->f);
  fscanf(datarangep->f, "%c%*[^\n]", &deleteinclude);
  getc(datarangep->f);

  if (deleteinclude == '\n')
    deleteinclude = ' ';
  printf("%*ld column\n", wid, column);
  printf("%*.*f mincut\n", wid, dec, mincut);
  printf("%*.*f maxcut\n", wid, dec, maxcut);
  printf("%*c deleteinclude\n", wid, deleteinclude);

  if (deleteinclude == 'd') {
    delete_ = true;
    include = false;
  } else {
    if (deleteinclude == 'i') {
      delete_ = false;
      include = true;
    } else {
      printf("deleteinclude must be one of 'di'\n");
      halt();
    }
  }
  if (*datain->name != '\0') {
    if (datain->f != NULL)
      datain->f = freopen(datain->name, "r", datain->f);
    else
      datain->f = fopen(datain->name, "r");
  } else {

    rewind(datain->f);
  }
  if (datain->f == NULL)
    _EscIO2(FileNotFound, datain->name);
  RESETBUF(datain->f, Char);
  if (*dataout->name != '\0') {
    if (dataout->f != NULL)
      dataout->f = freopen(dataout->name, "w", dataout->f);
    else
      dataout->f = fopen(dataout->name, "w");
  } else {
    if (dataout->f != NULL)
      rewind(dataout->f);
    else
      dataout->f = tmpfile();
  }
  if (dataout->f == NULL)
    _EscIO2(FileNotFound, dataout->name);
  SETUPBUF(dataout->f, Char);

  while (!BUFEOF(datain->f)) {
    if (P_eoln(datain->f)) {
      fscanf(datain->f, "%*[^\n]");
      getc(datain->f);
      putc('\n', dataout->f);
      linenumber++;
      continue;
    }
    getstring(datain, &line, &gotten);
    linenumber++;
    if (!gotten)
      continue;

    if (line.length <= 0 || line.letters[0] == '*') {
      writestring(dataout, &line);
      putc('\n', dataout->f);
      continue;
    }
    stringcolumn(line, column, &datum);
    if (datum.length <= 0)
      continue;

    thedatum = 5.0;
    stringtoreal(datum, &thedatum, &gotten);
    if (!gotten) {
      printf("ERROR: data in column %ld line %ld is not an integer or real number:\n",
	     column, linenumber);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &datum);
      putchar('\n');
      halt();
    }

    /*

*/
    if ((thedatum < mincut || thedatum > maxcut) && delete_ ||
	thedatum >= mincut && thedatum <= maxcut && include) {
      writestring(dataout, &line);
      putc('\n', dataout->f);
    }
  }

}

#undef wid
#undef dec


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  dataout.f = NULL;
  strcpy(dataout.name, "dataout");
  datarangep.f = NULL;
  strcpy(datarangep.name, "datarangep");
  datain.f = NULL;
  strcpy(datain.name, "datain");
  themain(&datain, &datarangep, &dataout);
_L1:
  if (datain.f != NULL)
    fclose(datain.f);
  if (datarangep.f != NULL)
    fclose(datarangep.f);
  if (dataout.f != NULL)
    fclose(dataout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
