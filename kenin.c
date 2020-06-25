/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "kenin.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.24
/*
*/



/*







































*/


#define maxstring       500


typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;


Static _TEXT allgen, keninp, inst;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *inst;
  stringDelila atoken;
  long count;
  stringDelila buffer;
  boolean gotten;
  long nocount;
  Char orfs;
  stringDelila piecename;
  /*
*/


  long first, last;
  Char what, cha;
  long int_;
  double rea;
} ;

Local Void testforinteger(LINK)
struct LOC_themain *LINK;
{
  token(&LINK->buffer, &LINK->atoken, &LINK->gotten);
  if (!LINK->gotten)
    return;
  figurestring(&LINK->atoken, &LINK->first, &LINK->last, &LINK->what,
	       &LINK->cha, &LINK->int_, &LINK->rea);
  if (LINK->what == 'i')
    return;
  printf("For piece \"");
  writestring(LINK->inst, &LINK->piecename);
  printf("\"\n");
  printf("figurestring found the token \"");
  writestring(LINK->inst, &LINK->atoken);
  printf("\" of type %c", LINK->what);
  printf(".  An integer was expected.\n");
  halt();
}

Local Char sign(n, LINK)
long n;
struct LOC_themain *LINK;
{
  if (n >= 0)
    return '+';
  else
    return ' ';
}

Local Void conclusions(thefile, LINK)
_TEXT *thefile;
struct LOC_themain *LINK;
{
  fprintf(thefile->f, "\n(* \n");
  fprintf(thefile->f, "%ld get instructions written\n", LINK->count);
  fprintf(thefile->f, "%ld get instructions commented out\n", LINK->nocount);
  if (LINK->orfs == 'n' || LINK->orfs == 'o') {
    fprintf(thefile->f, "deleted instructions were");
    if (LINK->orfs == 'o')
      fprintf(thefile->f, " not orfs");
    else
      fprintf(thefile->f, " orfs");
  }
  fprintf(thefile->f, " *)\n");
}



Static Void themain(allgen, keninp, inst_)
_TEXT *allgen, *keninp, *inst_;
{
  struct LOC_themain V;
  long l1, l2;
  boolean orffound;
  long thefrom, theto;
  boolean writeinst;
  _TEXT TEMP;

  V.inst = inst_;
  printf("kenin %4.2f\n", version);
  V.count = 0;
  V.nocount = 0;

  if (*keninp->name != '\0') {
    if (keninp->f != NULL)
      keninp->f = freopen(keninp->name, "r", keninp->f);
    else
      keninp->f = fopen(keninp->name, "r");
  } else
    rewind(keninp->f);
  if (keninp->f == NULL)
    _EscIO2(FileNotFound, keninp->name);
  RESETBUF(keninp->f, Char);
  fscanf(keninp->f, "%ld%ld%*[^\n]", &thefrom, &theto);
  getc(keninp->f);
  if (BUFEOF(keninp->f))
    V.orfs = 'b';
  else {
    fscanf(keninp->f, "%c%*[^\n]", &V.orfs);
    getc(keninp->f);
    if (V.orfs == '\n')
      V.orfs = ' ';
  }
  if (V.orfs != 'o' && V.orfs != 'n' && V.orfs != 'b') {
    printf("second parameter must be one of: bno\n");
    halt();
  }

  if (*V.inst->name != '\0') {
    if (V.inst->f != NULL)
      V.inst->f = freopen(V.inst->name, "w", V.inst->f);
    else
      V.inst->f = fopen(V.inst->name, "w");
  } else {
    if (V.inst->f != NULL)
      rewind(V.inst->f);
    else
      V.inst->f = tmpfile();
  }
  if (V.inst->f == NULL)
    _EscIO2(FileNotFound, V.inst->name);
  SETUPBUF(V.inst->f, Char);
  fprintf(V.inst->f, "title \"from kenin %4.2f\";\n", version);

  if (*allgen->name != '\0') {
    if (allgen->f != NULL)
      allgen->f = freopen(allgen->name, "r", allgen->f);
    else
      allgen->f = fopen(allgen->name, "r");
  } else
    rewind(allgen->f);
  if (allgen->f == NULL)
    _EscIO2(FileNotFound, allgen->name);
  RESETBUF(allgen->f, Char);
  fprintf(V.inst->f, "\n(*\n");
  while (P_peek(allgen->f) != ' ')
    copyaline(allgen, V.inst);
  fprintf(V.inst->f, "*)\n\n");

  fprintf(V.inst->f, "organism ecoli; chromosome ecoli;\n\n");
  fprintf(V.inst->f, "(* number only the pieces, starting at 1 *)\n");
  fprintf(V.inst->f, "      default numbering piece;\n");
  fprintf(V.inst->f, "      default numbering 1;\n");
  fprintf(V.inst->f, "      default out-of-range reduce-range;\n");

  while (!BUFEOF(allgen->f)) {
    getstring(allgen, &V.buffer, &V.gotten);
    if (!V.gotten)
      continue;
    token(&V.buffer, &V.piecename, &V.gotten);
    if (!V.gotten)
      continue;
    fprintf(V.inst->f, "\npiece ");
    writestring(V.inst, &V.piecename);
    fprintf(V.inst->f, ";\n");

    while (V.gotten) {
      testforinteger(&V);
      l1 = V.int_;

      testforinteger(&V);
      l2 = V.int_;

      token(&V.buffer, &V.atoken, &V.gotten);

      if (!V.gotten)
	break;

      orffound = (V.atoken.letters[0] == 'o' && V.atoken.letters[1] == 'r' &&
		  V.atoken.letters[2] == 'f');
      writeinst = (orffound && V.orfs == 'o' || !orffound && V.orfs == 'n' ||
		   V.orfs == 'b');


      if (!writeinst)
	fprintf(V.inst->f, "(* ");


      if (l1 < l2) {
	fprintf(V.inst->f, "get from %ld %c%ld",
		l1, sign(thefrom, &V), thefrom);
	fprintf(V.inst->f, " to %ld %c%ld", l1, sign(theto, &V), theto);
      } else {
	fprintf(V.inst->f, "get from %ld %c%ld",
		l1, sign(-thefrom, &V), -thefrom);
	fprintf(V.inst->f, " to %ld %c%ld", l1, sign(-theto, &V), -theto);
	fprintf(V.inst->f, " direction -");
      }
      fprintf(V.inst->f, "; ");

      if (writeinst)
	fprintf(V.inst->f, "(* ");
      writestring(V.inst, &V.atoken);

      if (writeinst)
	V.count++;
      else
	V.nocount++;

      if (writeinst)
	fprintf(V.inst->f, " #%ld", V.count);

      fprintf(V.inst->f, " *)\n");
    }
  }

  TEMP.f = stdout;
  *TEMP.name = '\0';
  conclusions(&TEMP, &V);
  conclusions(V.inst, &V);

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
  keninp.f = NULL;
  strcpy(keninp.name, "keninp");
  allgen.f = NULL;
  strcpy(allgen.name, "allgen");
  themain(&allgen, &keninp, &inst);
_L1:
  if (allgen.f != NULL)
    fclose(allgen.f);
  if (keninp.f != NULL)
    fclose(keninp.f);
  if (inst.f != NULL)
    fclose(inst.f);
  exit(EXIT_SUCCESS);
}

















/* End. */
