/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dbfilter.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.08
/*
*/



/*















































*/






#define maxlines        20
/*
*/



#define maxstring       150



#define fillermax       50



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



/*


*/
typedef Char filler[fillermax];



typedef stringDelila linebuffer[maxlines];

/*
*/



Static _TEXT dbfilterp;


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
  long l;

  b->length = a.length;
  for (l = 0; l < a.length; l++)
    b->letters[l] = a.letters[l];
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








Static Void writeparameters(tofile, genus, species)
_TEXT *tofile;
stringDelila genus, species;
{
  fprintf(tofile->f, "* searching for organism ");
  writestring(tofile, &genus);
  putc(' ', tofile->f);
  writestring(tofile, &species);
  putc('\n', tofile->f);
}




Static Void readparameters(dbfilterp, genus, species)
_TEXT *dbfilterp;
stringDelila *genus, *species;
{
  boolean gotten;
  stringDelila buffer;

  if (*dbfilterp->name != '\0') {
    if (dbfilterp->f != NULL)
      dbfilterp->f = freopen(dbfilterp->name, "r", dbfilterp->f);
    else
      dbfilterp->f = fopen(dbfilterp->name, "r");
  } else
    rewind(dbfilterp->f);
  if (dbfilterp->f == NULL)
    _EscIO2(FileNotFound, dbfilterp->name);
  RESETBUF(dbfilterp->f, Char);
  getstring(dbfilterp, &buffer, &gotten);
  if (!gotten) {
    printf("empty dbfilterp\n");
    halt();
  }


  token(&buffer, genus, &gotten);
  if (!gotten) {
    printf("missing first parameter\n");
    halt();
  }


  token(&buffer, species, &gotten);
  if (!gotten) {
    printf("missing second parameter\n");
    halt();
  }

}








Static Void themain(fin, fout, dbfilterp)
_TEXT *fin, *fout, *dbfilterp;
{
  stringDelila atoken;

  stringDelila buffer;

  stringDelila endentry;

  stringDelila foundgenus, foundspecies;

  stringDelila genus;
  boolean gotten;

  long index;

  stringDelila locus;

  stringDelila newtoken;

  stringDelila organism;

  stringDelila species;
  linebuffer storage;

  boolean tokenfound;

  long x;

  printf("dbfilter %4.2f\n", version);
  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
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





  fillstring(&locus, "LOCUS                                             ");
  fillstring(&organism, "ORGANISM                                          ");
  fillstring(&endentry, "//                                                ");


  readparameters(dbfilterp, &genus, &species);
  writeparameters(fout, genus, species);

  while (!BUFEOF(fin->f)) {
    getstring(fin, &buffer, &gotten);
    if (!gotten)
      continue;
    token(&buffer, &atoken, &tokenfound);
    if (!equalstring(atoken, locus))
      continue;
    index = 1;
    copystring(buffer, &storage[index-1]);
    writestring(fout, &buffer);
    putc('\n', fout->f);
    while (!equalstring(atoken, organism)) {
      getstring(fin, &buffer, &gotten);
      index++;
      if (index > maxlines) {
	fprintf(fout->f,
		"buffer capacity exceeded increase constant maxlines\n");
	halt();
      }
      copystring(buffer, &storage[index-1]);

      token(&buffer, &atoken, &tokenfound);
      if (!(tokenfound & equalstring(atoken, organism)))
	continue;
      token(&buffer, &foundgenus, &tokenfound);
      if (!tokenfound)
	continue;
      token(&buffer, &foundspecies, &tokenfound);
      if (!tokenfound)
	continue;
      writestring(fout, &genus);
      putc(' ', fout->f);
      writestring(fout, &species);
      putc('\n', fout->f);
      if (!(equalstring(genus, foundgenus) & equalstring(species, foundspecies)))
	continue;
      for (x = 0; x < index; x++) {
	writestring(fout, &storage[x]);
	putc('\n', fout->f);
      }
      do {
	getstring(fin, &buffer, &gotten);
	if (!gotten) {
	  fprintf(fout->f, "incomplete entry\n");
	  halt();
	}
	writestring(fout, &buffer);
	putc('\n', fout->f);
	token(&buffer, &newtoken, &tokenfound);
	if (!tokenfound)
	  clearstring(&newtoken);
      } while (!equalstring(newtoken, endentry));
    }
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
  dbfilterp.f = NULL;
  strcpy(dbfilterp.name, "dbfilterp");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1, &dbfilterp);
_L1:
  if (dbfilterp.f != NULL)
    fclose(dbfilterp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
