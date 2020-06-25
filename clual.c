/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "clual.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.09
/*



*/
#define updateversion   1.00



/*























































































*/



#define maxstring       2000



#define fillermax       50




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;



/*


*/
typedef Char filler[fillermax];


Static _TEXT clustalout, clualp, protseq;


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


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
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



Static Void copystring(a, b)
stringDelila a, *b;
{
  long l;

  b->length = a.length;
  for (l = 0; l < a.length; l++)
    b->letters[l] = a.letters[l];
}



Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) == ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) != ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}


typedef struct protein {
  stringDelila name, *data, *lastdata;
  struct protein *next;
} protein;




Static Void themain(clustalout, clualp, protseq)
_TEXT *clustalout, *clualp, *protseq;
{
  stringDelila aline, clustalid, *d;
  boolean gotten;
  boolean newsegment = false;
  double parameterversion;
  protein *p = NULL, *proteins = NULL;
  /*
*/
  long protcount = 0, segment = 0;
  stringDelila thedata, thename;
  Char verbose;
  _TEXT TEMP;
  protein *WITH;

  printf("clual %4.2f\n", version);
  if (*clualp->name != '\0') {
    if (clualp->f != NULL)
      clualp->f = freopen(clualp->name, "r", clualp->f);
    else
      clualp->f = fopen(clualp->name, "r");
  } else
    rewind(clualp->f);
  if (clualp->f == NULL)
    _EscIO2(FileNotFound, clualp->name);
  RESETBUF(clualp->f, Char);
  fscanf(clualp->f, "%lg%*[^\n]", &parameterversion);
  getc(clualp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*protseq->name != '\0') {
    if (protseq->f != NULL)
      protseq->f = freopen(protseq->name, "w", protseq->f);
    else
      protseq->f = fopen(protseq->name, "w");
  } else {
    if (protseq->f != NULL)
      rewind(protseq->f);
    else
      protseq->f = tmpfile();
  }
  if (protseq->f == NULL)
    _EscIO2(FileNotFound, protseq->name);
  SETUPBUF(protseq->f, Char);
  fprintf(protseq->f, "> clual %4.2f\n", version);
  if (*clustalout->name != '\0') {
    if (clustalout->f != NULL)
      clustalout->f = freopen(clustalout->name, "r", clustalout->f);
    else
      clustalout->f = fopen(clustalout->name, "r");
  } else
    rewind(clustalout->f);
  if (clustalout->f == NULL)
    _EscIO2(FileNotFound, clustalout->name);
  RESETBUF(clustalout->f, Char);


  getstring(clualp, &clustalid, &gotten);
  if (!gotten) {
    printf("The second line of clualp must match the first line\n");
    printf("of the clustalout file.\n");
    printf("This is used to check that the clustalout file is correct.\n");
    halt();
  }

  fscanf(clualp->f, "%c%*[^\n]", &verbose);
  getc(clualp->f);

  if (verbose == '\n')
    verbose = ' ';
  getstring(clustalout, &aline, &gotten);
  if (!gotten) {
    printf("clustalout is empty?\n");
    halt();
  }

  if (!equalstring(aline, clustalid)) {
    printf("clustalout is not a correct file\n");
    printf("The first line MUST be:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &clustalid);
    printf("\nbut this was found instead:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &aline);
    putchar('\n');
    halt();
  }


  while (!BUFEOF(clustalout->f)) {
    if (!P_eoln(clustalout->f)) {
      if (P_peek(clustalout->f) == ' ') {
	while (!P_eoln(clustalout->f))
	  getc(clustalout->f);
      }
    }


    if (P_eoln(clustalout->f)) {
      fscanf(clustalout->f, "%*[^\n]");
      getc(clustalout->f);
      newsegment = true;
      p = NULL;
      continue;
    }

    if (newsegment) {
      segment++;
      newsegment = false;
      if (verbose == 'v')
	printf("segment %ld\n", segment);
      if (p != NULL) {
	printf("segment %ld is too short!\n", segment);
	halt();
      }
    }

    if (proteins == NULL) {
      proteins = (protein *)Malloc(sizeof(protein));
      clearstring(&proteins->name);
      proteins->data = (stringDelila *)Malloc(sizeof(stringDelila));
      proteins->lastdata = NULL;
      proteins->next = NULL;
      p = proteins;
      if (segment == 1)
	protcount = 1;
    } else {
      if (p == NULL)
	p = proteins;

      else {
	if (p->next == NULL) {
	  p->next = (protein *)Malloc(sizeof(protein));
	  WITH = p->next;
	  clearstring(&WITH->name);
	  WITH->data = (stringDelila *)Malloc(sizeof(stringDelila));
	  WITH->lastdata = NULL;
	  WITH->next = NULL;
	}

	p = p->next;
	if (segment == 1)
	  protcount++;
      }
    }


    getstring(clustalout, &aline, &gotten);
    if (!gotten) {
      printf("clual: could not read line correctly?\n");
      printf("in segment %ld the line is:\n", segment);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &aline);
      putchar('\n');
      halt();
    }

    clearstring(&thename);
    token(&aline, &thename, &gotten);
    if (!gotten) {
      printf("clual: could not read name correctly?\n");

      printf("in segment %ld\n", segment);
      printf("the line read is:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &aline);
      printf("\nthe PREVIOUS name read is:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &thename);
      printf("\nthe PREVIOUS data read is:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &thedata);
      putchar('\n');

      halt();
    }

    token(&aline, &thedata, &gotten);
    if (!gotten) {
      printf("clual: could not read data correctly?\n");

      printf("in segment %ld\n", segment);
      printf("the line read is:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &aline);
      printf("\nthe PREVIOUS name read is:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &thename);
      printf("\nthe PREVIOUS data read is:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &thedata);
      putchar('\n');

      halt();
    }

    if (p->name.length == 0) {
      copystring(thename, &p->name);
      p->data = (stringDelila *)Malloc(sizeof(stringDelila));
      clearstring(p->data);
      p->lastdata = p->data;
      copystring(thedata, p->data);
      p->lastdata->next = NULL;
      continue;
    }

    if (!equalstring(p->name, thename)) {
      printf("name \"");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &p->name);
      printf("\"\n");
      printf("DOES NOT MATCH PREVIOUS NAME\n");
      printf("name \"");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &thename);
      printf("\"\n");
      halt();
    }

    p->lastdata->next = (stringDelila *)Malloc(sizeof(stringDelila));
    p->lastdata = p->lastdata->next;
    clearstring(p->lastdata);
    copystring(thedata, p->lastdata);
    p->lastdata->next = NULL;

  }



  if (verbose != 'v') {
    printf("%ld segment", segment);
    if (segment > 1)
      putchar('s');
    printf("\n%ld protein", protcount);

    if (protcount > 1)
      putchar('s');
    putchar('\n');
  }


  p = proteins;
  while (p != NULL) {
    if (verbose == 'v') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &p->name);
      putchar('\n');
    }
    fprintf(protseq->f, "> ");
    writestring(protseq, &p->name);
    putc('\n', protseq->f);
    d = p->data;
    while (d != NULL) {
      writestring(protseq, d);
      putc('\n', protseq->f);

      if (d->next == NULL) {
	if (d != p->lastdata) {
	  printf("ERROR: lastdata is not end of list\n");
	  halt();
	}
      }
      d = d->next;
    }
    p = p->next;
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  protseq.f = NULL;
  strcpy(protseq.name, "protseq");
  clualp.f = NULL;
  strcpy(clualp.name, "clualp");
  clustalout.f = NULL;
  strcpy(clustalout.name, "clustalout");
  themain(&clustalout, &clualp, &protseq);
_L1:
  if (clustalout.f != NULL)
    fclose(clustalout.f);
  if (clualp.f != NULL)
    fclose(clualp.f);
  if (protseq.f != NULL)
    fclose(protseq.f);
  exit(EXIT_SUCCESS);
}

/*








*/



/* End. */
