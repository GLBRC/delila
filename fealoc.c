/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "fealoc.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.13
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




typedef struct featabstructure {
  stringDelila accession, name;
  long first, last;
  Char orientation;
  struct featabstructure *next;
} featabstructure;


Static _TEXT featab, data, fealocp, fealocreport;


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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}


#define tabcharacter    9



Static Void onetoken(afile, buffer, gotten)
_TEXT *afile;
stringDelila *buffer;
boolean *gotten;
{
  /*

*/
  long index = 0;
  boolean done = false;

  skipblanks(afile);
  emptystring(buffer);
  if (BUFEOF(afile->f)) {
    *gotten = false;
    return;
  }
  while (!P_eoln(afile->f) && index < maxstring && !done) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
    if (buffer->letters[index-1] == ' ' ||
	buffer->letters[index-1] == tabcharacter) {
      done = true;
      index--;
    }
  }
  buffer->length = index;
  buffer->current = 1;
  *gotten = true;
}

#undef tabcharacter



Static Void writefeatab(afile, f)
_TEXT *afile;
featabstructure *f;
{
  fprintf(afile->f, "accession: ");
  writestring(afile, &f->accession);
  fprintf(afile->f, "\nname: ");
  writestring(afile, &f->name);
  fprintf(afile->f, "\nfirst: %ld\n", f->first);
  fprintf(afile->f, "last: %ld\n", f->last);
  fprintf(afile->f, "orientation: %c\n", f->orientation);
}


#define debugging       false



Static Void showfeatab(afile, f)
_TEXT *afile;
featabstructure *f;
{
  fprintf(afile->f, " of");
  if (debugging)
    fprintf(afile->f, "accession: ");
  else
    putc(' ', afile->f);
  writestring(afile, &f->accession);
  if (debugging)
    fprintf(afile->f, " name: ");
  else
    putc(' ', afile->f);
  writestring(afile, &f->name);
  fprintf(afile->f, " =");
  if (debugging)
    fprintf(afile->f, "first: %ld", f->first);
  fprintf(afile->f, " %ld", f->first);
  if (debugging)
    fprintf(afile->f, "last: %ld", f->last);
  fprintf(afile->f, " %ld", f->last);
  if (debugging)
    fprintf(afile->f, "orientation: ");
  else
    putc(' ', afile->f);
  putc(f->orientation, afile->f);
  /*

*/
}

#undef debugging


#define debugging       false


/* Local variables for readfeatab: */
struct LOC_readfeatab {
  _TEXT *featab;
  stringDelila accession, name;
  long first, last;
  Char orientation;
  boolean gotten;
} ;

Local Void justread(LINK)
struct LOC_readfeatab *LINK;
{
  _TEXT TEMP;

  onetoken(LINK->featab, &LINK->accession, &LINK->gotten);
  if (!LINK->gotten) {
    printf("readfeatab: failed to get buffer for accession\n");
    halt();
  }
  onetoken(LINK->featab, &LINK->name, &LINK->gotten);
  if (!LINK->gotten) {
    printf("readfeatab: failed to get buffer for name\n");
    halt();
  }
  fscanf(LINK->featab->f, "%ld", &LINK->first);
  fscanf(LINK->featab->f, "%ld", &LINK->last);
  skipblanks(LINK->featab);
  LINK->orientation = getc(LINK->featab->f);
  if (LINK->orientation == '\n')
    LINK->orientation = ' ';
  fscanf(LINK->featab->f, "%*[^\n]");
  getc(LINK->featab->f);
  printf("accession: ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &LINK->accession);
  printf("\nname: ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &LINK->name);
  printf("\nfirst: %ld\n", LINK->first);
  printf("last: %ld\n", LINK->last);
  printf("orientation: %c\n", LINK->orientation);
}



Static Void readfeatab(featab_, ftsp)
_TEXT *featab_;
featabstructure **ftsp;
{
  /*

*/
  struct LOC_readfeatab V;
  long skip;
  long n = 0;
  featabstructure *f;
  long hold;
  _TEXT TEMP;

  V.featab = featab_;
  printf("readfeatab: reading featab file\n");

  if (*V.featab->name != '\0') {
    if (V.featab->f != NULL)
      V.featab->f = freopen(V.featab->name, "r", V.featab->f);
    else
      V.featab->f = fopen(V.featab->name, "r");
  } else
    rewind(V.featab->f);
  if (V.featab->f == NULL)
    _EscIO2(FileNotFound, V.featab->name);
  RESETBUF(V.featab->f, Char);

  for (skip = 1; skip <= 2; skip++) {
    if (BUFEOF(V.featab->f)) {
      printf("featab is empty\n");
      halt();
    }
    fscanf(V.featab->f, "%*[^\n]");
    getc(V.featab->f);
  }

  /*

*/

  *ftsp = (featabstructure *)Malloc(sizeof(featabstructure));
  f = *ftsp;

  while (!BUFEOF(V.featab->f)) {
    n++;
    onetoken(V.featab, &f->accession, &V.gotten);
    if (!V.gotten) {
      printf("readfeatab: failed to get buffer for accession\n");
      halt();
    }

    onetoken(V.featab, &f->name, &V.gotten);
    if (!V.gotten) {
      printf("readfeatab: failed to get buffer for name\n");
      halt();
    }

    fscanf(V.featab->f, "%ld", &f->first);
    fscanf(V.featab->f, "%ld", &f->last);
    skipblanks(V.featab);
    f->orientation = getc(V.featab->f);

    if (f->orientation == '\n')
      f->orientation = ' ';
    fscanf(V.featab->f, "%*[^\n]");
    getc(V.featab->f);
    if (debugging) {
      putchar('\n');
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writefeatab(&TEMP, f);
    }

    if (f->first > f->last) {
      /*


*/
      hold = f->first;
      f->first = f->last;
      f->last = hold;
      /*




*/
    }

    if (!BUFEOF(V.featab->f)) {
      f->next = (featabstructure *)Malloc(sizeof(featabstructure));
      f = f->next;
    } else
      f->next = NULL;

    if (debugging) {
      if (n >= 3)
	halt();
    }
  }
  printf("%ld features read from featab\n", n);

}

#undef debugging


Static boolean between(a, b, c)
long a, b, c;
{
  return (a <= b && b <= c || c <= b && b <= a);
}


Static boolean orderedbetween(a, b, c)
long a, b, c;
{
  return (a <= b && b <= c);
  /*


*/
}


/* Local variables for themain: */
struct LOC_themain {
  featabstructure *afeature;

  /*






*/
  long piecenumber, piecelength;
  stringDelila piecename;
  long piececoo;
  /*

*/
  long orientation;
  double ri;
} ;

Local Void writedata(LINK)
struct LOC_themain *LINK;
{
  _TEXT TEMP;

  printf("1 piecenumber: %ld\n", LINK->piecenumber);
  printf("2 piecelength: %ld\n", LINK->piecelength);
  printf("3 piecename: ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &LINK->piecename);
  printf("\n4 piececoo: %ld\n", LINK->piececoo);
  printf("5 orientation: %ld\n", LINK->orientation);
  printf("6 ri: %4.2f\n", LINK->ri);
}

Local Void showdata(afile, LINK)
_TEXT *afile;
struct LOC_themain *LINK;
{
  fprintf(afile->f, "%8ld", LINK->piececoo);
  putc(' ', afile->f);
  if (LINK->orientation == 1)
    putc('+', afile->f);
  else
    putc('-', afile->f);
  fprintf(afile->f, " %4.2f bits", LINK->ri);
  fprintf(afile->f, " |");
}

Local Void showinside(afile, feature, LINK)
_TEXT *afile;
featabstructure *feature;
struct LOC_themain *LINK;
{
  showdata(afile, LINK);
  fprintf(afile->f, " INSIDE  -      ");
  showfeatab(afile, LINK->afeature);
  putc('\n', afile->f);
}

Local Void showoutside(afile, feature, beforeafter, LINK)
_TEXT *afile;
featabstructure *feature;
Char beforeafter;
struct LOC_themain *LINK;
{
  showdata(afile, LINK);
  fprintf(afile->f, " OUTSIDE ");
  if (feature->orientation == beforeafter)
    fprintf(afile->f, "CONTROL");
  else
    fprintf(afile->f, "-      ");
  showfeatab(afile, feature);
  putc('\n', afile->f);
}



Static Void themain(featab, data, fealocp, fealocreport)
_TEXT *featab, *data, *fealocp, *fealocreport;
{
  struct LOC_themain V;
  double parameterversion;
  featabstructure *features, *previousfeature;
  /*

*/
  boolean gotten, done;

  double ricutoff;
  long line = 0, sites = 0;
  long maxsites;
  Char b = '\b';
  _TEXT TEMP;

  printf("fealoc %4.2f\n", version);
  if (*fealocp->name != '\0') {
    if (fealocp->f != NULL)
      fealocp->f = freopen(fealocp->name, "r", fealocp->f);
    else
      fealocp->f = fopen(fealocp->name, "r");
  } else
    rewind(fealocp->f);
  if (fealocp->f == NULL)
    _EscIO2(FileNotFound, fealocp->name);
  RESETBUF(fealocp->f, Char);
  fscanf(fealocp->f, "%lg%*[^\n]", &parameterversion);
  getc(fealocp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }
  fscanf(fealocp->f, "%lg%*[^\n]", &ricutoff);
  getc(fealocp->f);
  printf("Ri cutoff: %4.2f bits\n", ricutoff);
  fscanf(fealocp->f, "%ld%*[^\n]", &maxsites);
  getc(fealocp->f);


  readfeatab(featab, &features);
  printf("\nThe first feature read is:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writefeatab(&TEMP, features);
  putchar('\n');


  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "r", data->f);
    else
      data->f = fopen(data->name, "r");
  } else
    rewind(data->f);
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  RESETBUF(data->f, Char);
  if (*fealocreport->name != '\0') {
    if (fealocreport->f != NULL)
      fealocreport->f = freopen(fealocreport->name, "w", fealocreport->f);
    else
      fealocreport->f = fopen(fealocreport->name, "w");
  } else {
    if (fealocreport->f != NULL)
      rewind(fealocreport->f);
    else
      fealocreport->f = tmpfile();
  }
  if (fealocreport->f == NULL)
    _EscIO2(FileNotFound, fealocreport->name);
  SETUPBUF(fealocreport->f, Char);


  fprintf(fealocreport->f,
    "*coo orient bits bits | in/out control? of Accession gene = first last orient\n");

  printf("data file line:\n");
  while (!BUFEOF(data->f)) {
    line++;
    if (P_peek(data->f) == '*') {
      fscanf(data->f, "%*[^\n]");
      getc(data->f);
      continue;
    }
    printf("%ld%c%c%c%c%c%c%c%c%c", line, b, b, b, b, b, b, b, b, b);
    /*
*/
    fscanf(data->f, "%ld", &V.piecenumber);
    fscanf(data->f, "%ld", &V.piecelength);
    onetoken(data, &V.piecename, &gotten);
    if (!gotten) {
      printf("unable to get a piece name\n");
      halt();
    }
    fscanf(data->f, "%ld", &V.piececoo);
    skipblanks(data);
    fscanf(data->f, "%ld", &V.orientation);
    fscanf(data->f, "%lg", &V.ri);
    fscanf(data->f, "%*[^\n]");

    getc(data->f);
    if (V.ri >= ricutoff) {
      sites++;
      /*

*/
      previousfeature = features;
      V.afeature = features;
      done = false;
      while (!done) {
	if (orderedbetween(V.afeature->first, V.piececoo, V.afeature->last)) {
	  showinside(fealocreport, V.afeature, &V);

	  if (V.afeature->first == 0) {
	    printf("Something is terribly wrong if a coordinate is zero!\n");
	    printf("These come from join() not properly handled in dbinst!\n");
	    halt();
	  }
	}
	if (V.piececoo < V.afeature->first &&
	    V.piececoo >= previousfeature->last) {
	  showoutside(fealocreport, previousfeature, '-', &V);
	  showoutside(fealocreport, V.afeature, '+', &V);
	}
	previousfeature = V.afeature;
	V.afeature = V.afeature->next;
	if (V.afeature == NULL)
	  done = true;
      }
    }

    if (maxsites >= 0) {
      if (sites >= maxsites) {
	printf("sites = %ld >= maxsites = %ld\n", sites, maxsites);
	halt();
      }
    }

  }
  fprintf(fealocreport->f, "* sites: %ld \n", sites);
  printf("sites: %ld \n", sites);
  printf("fealoc is done\n");

  /*


*/


}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fealocreport.f = NULL;
  strcpy(fealocreport.name, "fealocreport");
  fealocp.f = NULL;
  strcpy(fealocp.name, "fealocp");
  data.f = NULL;
  strcpy(data.name, "data");
  featab.f = NULL;
  strcpy(featab.name, "featab");
  themain(&featab, &data, &fealocp, &fealocreport);
_L1:
  if (featab.f != NULL)
    fclose(featab.f);
  if (data.f != NULL)
    fclose(data.f);
  if (fealocp.f != NULL)
    fclose(fealocp.f);
  if (fealocreport.f != NULL)
    fclose(fealocreport.f);
  exit(EXIT_SUCCESS);
}



/* End. */
