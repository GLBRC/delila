/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "indana.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         5.32
/*






*/



/*








































*/



#define histmax         60000L

#define graphmax        100
#define percentmax      50


Static _TEXT ind, ana, subind, indanap;


Static long window, pbefore, pwindow, pafter, number, sequences, linears,
	    skip;


Static long hist[histmax + 1];
Static long similarity;
Static boolean warning;
/*
*/

Static long highest;


Static double mean, sd;
/*
*/

Static long sublower, subhigher;
/*
*/
Static struct {
  FILE *f;
  FILEBUFNC(f,long);
  Char name[_FNSIZE];
} sublist;
/*
*/
Static boolean breaklines;


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



Static long copylines(fin, fout, n)
_TEXT *fin, *fout;
long n;
{
  /*
*/
  long index = 0;

  while (!BUFEOF(fin->f) && index < n) {
    copyaline(fin, fout);
    index++;
  }

  return index;
}


/* Local variables for numberdigit: */
struct LOC_numberdigit {
  long number, place, absolute;
  Char acharacter;
} ;

Local Void digit(LINK)
struct LOC_numberdigit *LINK;
{
  long tenplace, z, d;

  tenplace = LINK->place * 10;
  z = LINK->absolute - LINK->absolute / tenplace * tenplace;
  if (LINK->place == 1)
    d = z;
  else
    d = z / LINK->place;
  switch (d) {

  case 0:
    LINK->acharacter = '0';
    break;

  case 1:
    LINK->acharacter = '1';
    break;

  case 2:
    LINK->acharacter = '2';
    break;

  case 3:
    LINK->acharacter = '3';
    break;

  case 4:
    LINK->acharacter = '4';
    break;

  case 5:
    LINK->acharacter = '5';
    break;

  case 6:
    LINK->acharacter = '6';
    break;

  case 7:
    LINK->acharacter = '7';
    break;

  case 8:
    LINK->acharacter = '8';
    break;

  case 9:
    LINK->acharacter = '9';
    break;
  }
}

Local Void sign(LINK)
struct LOC_numberdigit *LINK;
{
  if (LINK->number < 0)
    LINK->acharacter = '-';
  else
    LINK->acharacter = '+';
}








Static Char numberdigit(number_, logplace)
long number_, logplace;
{
  /*



*/
  struct LOC_numberdigit V;
  long count;

  V.number = number_;
  V.place = 1;
  for (count = 1; count <= logplace; count++)
    V.place *= 10;

  if (V.number == 0) {
    if (V.place == 1)
      V.acharacter = '0';
    else
      V.acharacter = ' ';
    return V.acharacter;
  }
  V.absolute = labs(V.number);
  if (V.absolute < V.place / 10) {
    V.acharacter = ' ';
    return V.acharacter;
  }
  if (V.absolute >= V.place)
    digit(&V);
  else
    sign(&V);
  return V.acharacter;
}


#define ln10            2.30259
#define epsilon         0.00001



Static long numbersize(n)
long n;
{
  if (n == 0)
    return 1;
  else {
    return ((long)(log((double)labs(n)) / ln10 + epsilon) + 2);
    /*


*/

  }
}

#undef ln10
#undef epsilon



Static Void numberbar(afile, spaces, firstnumber, lastnumber, linesused)
_TEXT *afile;
long spaces, firstnumber, lastnumber, *linesused;
{
  /*
*/
  long logplace, spacecount, number;

  if (labs(firstnumber) > labs(lastnumber))
    *linesused = numbersize(firstnumber);
  else
    *linesused = numbersize(lastnumber);

  for (logplace = *linesused - 1; logplace >= 0; logplace--) {
    for (spacecount = 1; spacecount <= spaces; spacecount++)
      putc(' ', afile->f);
    for (number = firstnumber; number <= lastnumber; number++)
      fputc(numberdigit(number, logplace), afile->f);
    putc('\n', afile->f);
  }
}


#define headerlength    11
/*
*/
#define constart        3




Static Void rhindex(ind, print, window, pbefore, pwindow, pafter, number,
		    sequences, linears, skip)
_TEXT *ind, *print;
long *window, *pbefore, *pwindow, *pafter, *number, *sequences, *linears,
     *skip;
{
  /*
*/
  /*
*/
  Char c;
  long i;

  if (*ind->name != '\0') {
    if (ind->f != NULL)
      ind->f = freopen(ind->name, "r", ind->f);
    else
      ind->f = fopen(ind->name, "r");
  } else
    rewind(ind->f);
  if (ind->f == NULL)
    _EscIO2(FileNotFound, ind->name);
  RESETBUF(ind->f, Char);
  if (BUFEOF(ind->f)) {
    printf(" index file is empty\n");
    halt();
  }
  for (i = 1; i <= headerlength; i++) {
    if (!P_eoln(ind->f))
      putc(P_peek(ind->f), print->f);
    while (!P_eoln(ind->f)) {
      getc(ind->f);
      putc(P_peek(ind->f), print->f);
    }
    fscanf(ind->f, "%*[^\n]");
    getc(ind->f);
    putc('\n', print->f);
  }


  if (*ind->name != '\0') {
    if (ind->f != NULL)
      ind->f = freopen(ind->name, "r", ind->f);
    else
      ind->f = fopen(ind->name, "r");
  } else
    rewind(ind->f);
  if (ind->f == NULL)
    _EscIO2(FileNotFound, ind->name);
  RESETBUF(ind->f, Char);
  for (i = 1; i <= constart; i++) {
    fscanf(ind->f, "%*[^\n]");
    getc(ind->f);
  }
  fscanf(ind->f, "%c%ld%*[^\n]", &c, window);
  getc(ind->f);
  if (c == '\n')
    c = ' ';
  fscanf(ind->f, "%c%ld%*[^\n]", &c, pbefore);
  getc(ind->f);
  if (c == '\n')
    c = ' ';
  fscanf(ind->f, "%c%ld%*[^\n]", &c, pwindow);
  getc(ind->f);
  if (c == '\n')
    c = ' ';
  fscanf(ind->f, "%c%ld%*[^\n]", &c, pafter);
  getc(ind->f);
  if (c == '\n')
    c = ' ';
  fscanf(ind->f, "%c%ld%*[^\n]", &c, number);
  getc(ind->f);
  if (c == '\n')
    c = ' ';
  fscanf(ind->f, "%c%ld%*[^\n]", &c, sequences);
  getc(ind->f);
  if (c == '\n')
    c = ' ';
  fscanf(ind->f, "%c%ld%*[^\n]", &c, linears);
  getc(ind->f);
  if (c == '\n')
    c = ' ';
  fscanf(ind->f, "%*[^\n]");


  getc(ind->f);
  if (!BUFEOF(ind->f)) {
    getc(ind->f);
    getc(ind->f);
    if (!BUFEOF(ind->f)) {
      if (P_peek(ind->f) == 't') {
	printf(" indana can not be used on a teaching index.\n");
	halt();
      }
    }
  }


  fscanf(ind->f, "%*[^\n]");


  getc(ind->f);
  *skip = *pbefore + *pwindow + *pafter + 4;
}

#undef headerlength
#undef constart


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


Static Void readsim(ind, similarity)
_TEXT *ind;
long *similarity;
{
  /*

*/
  /*
*/
  skipblanks(ind);
  skipnonblanks(ind);
  skipblanks(ind);
  skipnonblanks(ind);
  skipblanks(ind);
  skipnonblanks(ind);
  skipblanks(ind);
  skipnonblanks(ind);
  skipblanks(ind);
  skipnonblanks(ind);
  skipblanks(ind);
  skipnonblanks(ind);
  fscanf(ind->f, "%ld%*[^\n]", similarity);
  getc(ind->f);
}


Local Void nosubindex()
{
  subhigher = -1;
  sublower = window + 1;
  breaklines = true;
  fprintf(ana.f, " no subindex made.\n");
}


Static Void initialize()
{
  long clearmax, similarity;

  printf(" indana %4.2f\n", version);
  if (*ana.name != '\0') {
    if (ana.f != NULL)
      ana.f = freopen(ana.name, "w", ana.f);
    else
      ana.f = fopen(ana.name, "w");
  } else {
    if (ana.f != NULL)
      rewind(ana.f);
    else
      ana.f = tmpfile();
  }
  if (ana.f == NULL)
    _EscIO2(FileNotFound, ana.name);
  SETUPBUF(ana.f, Char);

  fprintf(ana.f, " indana: similarity analysis %4.2f using:\n", version);

  rhindex(&ind, &ana, &window, &pbefore, &pwindow, &pafter, &number,
	  &sequences, &linears, &skip);

  if (window > histmax)
    clearmax = histmax;
  else
    clearmax = window;


  for (similarity = 0; similarity <= clearmax; similarity++)
    hist[similarity] = 0;


  if (*indanap.name != '\0') {
    if (indanap.f != NULL)
      indanap.f = freopen(indanap.name, "r", indanap.f);
    else
      indanap.f = fopen(indanap.name, "r");
  } else
    rewind(indanap.f);
  if (indanap.f == NULL)
    _EscIO2(FileNotFound, indanap.name);
  RESETBUF(indanap.f, Char);
  if (BUFEOF(indanap.f))
    nosubindex();
  else {
    fscanf(indanap.f, "%ld%*[^\n]", &sublower);
    getc(indanap.f);
    fscanf(indanap.f, "%ld%*[^\n]", &subhigher);
    getc(indanap.f);
    if ((unsigned long)sublower <= subhigher) {
      fprintf(ana.f, " subindex made from similarities in the range ");
      fprintf(ana.f, "[%ld to %ld]\n", sublower, subhigher);
    } else
      nosubindex();
    if (BUFEOF(indanap.f))
      breaklines = true;
    else if (P_peek(indanap.f) == 'b')
      breaklines = true;
    else
      breaklines = false;
  }
  if (*sublist.name != '\0') {
    if (sublist.f != NULL)
      sublist.f = freopen(sublist.name, "wb", sublist.f);
    else
      sublist.f = fopen(sublist.name, "wb");
  } else {
    if (sublist.f != NULL)
      rewind(sublist.f);
    else
      sublist.f = tmpfile();
  }
  if (sublist.f == NULL)
    _EscIO2(FileNotFound, sublist.name);
  SETUPBUF(sublist.f, long);
  if (*subind.name != '\0') {
    if (subind.f != NULL)
      subind.f = freopen(subind.name, "w", subind.f);
    else
      subind.f = fopen(subind.name, "w");
  } else {
    if (subind.f != NULL)
      rewind(subind.f);
    else
      subind.f = tmpfile();
  }
  if (subind.f == NULL)
    _EscIO2(FileNotFound, subind.name);
  SETUPBUF(subind.f, Char);
}


Static Void makehist()
{
  long lines;


  double sumx = 0.0, sumxsqr = 0.0;
  long FORLIM;

  warning = false;


  FORLIM = number;

  for (lines = 1; lines <= FORLIM; lines++) {
    readsim(&ind, &similarity);

    if (sublower <= similarity && similarity <= subhigher)
      fwrite(&lines, sizeof(long), 1, sublist.f);

    if (similarity <= histmax)
      hist[similarity]++;
    else {
      warning = true;
      hist[histmax]++;
    }


    sumx += similarity;
    sumxsqr += similarity * similarity;
  }



  mean = sumx / number;
  sd = sqrt(sumxsqr / number - mean * mean);
}


Static Void printhist()
{
  long similarity, FORLIM;

  fprintf(ana.f, "\n mean similarity: %4.2f\n", mean);
  fprintf(ana.f, " standard deviation of similarity: %4.2f\n\n", sd);
  fprintf(ana.f, " similarity number frequency\n");

  if (warning)
    highest = histmax;
  else
    highest = window;
  while (hist[highest] == 0 && highest != 0)
    highest--;

  FORLIM = highest;
  for (similarity = 0; similarity <= FORLIM; similarity++)
    fprintf(ana.f, " %6ld %8ld %10.2f\n",
	    similarity, hist[similarity], (double)hist[similarity] / number);
}


Static Void printgraph()
{
  long hifreq = 0;
  long s, f;
  long percent[histmax + 1];
  long p, FORLIM, FORLIM1;


  if (graphmax < highest)
    highest = graphmax;


  FORLIM = highest;
  for (s = 0; s <= FORLIM; s++) {
    p = (long)floor(hist[s] * 100.0 / number + 0.5);
    if (p > hifreq)
      hifreq = p;
    percent[s] = p;
  }
  if (percentmax < hifreq)
    hifreq = percentmax;


  fprintf(ana.f, "\f");
  fprintf(ana.f, " frequency distribution of similarities\n\n");
  fprintf(ana.f, " frequency\n");


  for (f = hifreq; f >= 1; f--) {
    fprintf(ana.f, " %4.2f i", f / 100.0);
    FORLIM1 = highest;
    for (s = 0; s <= FORLIM1; s++) {
      if (percent[s] >= f)
	putc('*', ana.f);
      else
	putc(' ', ana.f);
    }
    putc('\n', ana.f);
  }


  fprintf(ana.f, " %4.2f +", 0.0);
  FORLIM = highest;
  for (s = 0; s <= FORLIM; s++)
    putc('-', ana.f);
  putc('\n', ana.f);


  numberbar(&ana, 7L, 0L, highest, &s);
  fprintf(ana.f, "%*csimilarity\n", (int)(highest + 10), ' ');

  if (!warning)
    return;
  printf("one or more similarity was greater than %ld\n", histmax);
  fprintf(ana.f,
    " one or more similarity was greater than %ld and is recorded at %ld.\n",
    histmax, histmax);
  fprintf(ana.f, " the statistical calculations are on the original data.\n");
}


/* Local variables for printsubindex: */
struct LOC_printsubindex {
  long line;
} ;

Local Void copyline(LINK)
struct LOC_printsubindex *LINK;
{
  do {
    putc(P_peek(ind.f), subind.f);
    getc(ind.f);
  } while (!P_eoln(ind.f));
  fscanf(ind.f, "%*[^\n]");
  getc(ind.f);
  LINK->line++;
}


Static Void printsubindex()
{
  /*
*/
  struct LOC_printsubindex V;

  if (subhigher < 0)
    return;
  if (*sublist.name != '\0') {
    if (sublist.f != NULL)
      sublist.f = freopen(sublist.name, "rb", sublist.f);
    else
      sublist.f = fopen(sublist.name, "rb");
  } else
    rewind(sublist.f);
  if (sublist.f == NULL)
    _EscIO2(FileNotFound, sublist.name);
  RESETBUF(sublist.f, long);
  fprintf(subind.f,
    "* subindex of indana %4.2f (similarity range = [%ld to %ld]) from ",
    version, sublower, subhigher);


  if (*ind.name != '\0') {
    if (ind.f != NULL)
      ind.f = freopen(ind.name, "r", ind.f);
    else
      ind.f = fopen(ind.name, "r");
  } else
    rewind(ind.f);
  if (ind.f == NULL)
    _EscIO2(FileNotFound, ind.name);
  RESETBUF(ind.f, Char);
  while (P_peek(ind.f) == '*') {
    if (!P_eoln(ind.f))
      putc(P_peek(ind.f), subind.f);
    while (!P_eoln(ind.f)) {
      getc(ind.f);
      putc(P_peek(ind.f), subind.f);
    }
    fscanf(ind.f, "%*[^\n]");
    getc(ind.f);
    putc('\n', subind.f);
  }

  V.line = 1;
  while (!BUFEOF(sublist.f)) {
    while (V.line < GETFBUF(sublist.f, long) - 1) {
      fscanf(ind.f, "%*[^\n]");
      getc(ind.f);
      V.line++;
    }


    if (V.line == GETFBUF(sublist.f, long) - 1) {
      if (breaklines)
	putc('\n', subind.f);
      copyline(&V);
      putc('\n', subind.f);
    }


    copyline(&V);
    fprintf(subind.f, " *\n");

    GET(sublist.f, long);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  sublist.f = NULL;
  *sublist.name = '\0';
  indanap.f = NULL;
  strcpy(indanap.name, "indanap");
  subind.f = NULL;
  strcpy(subind.name, "subind");
  ana.f = NULL;
  strcpy(ana.name, "ana");
  ind.f = NULL;
  strcpy(ind.name, "ind");
  initialize();
  makehist();
  printhist();
  printgraph();
  printsubindex();
_L1:
  if (ind.f != NULL)
    fclose(ind.f);
  if (ana.f != NULL)
    fclose(ana.f);
  if (subind.f != NULL)
    fclose(subind.f);
  if (indanap.f != NULL)
    fclose(indanap.f);
  if (sublist.f != NULL)
    fclose(sublist.f);
  exit(EXIT_SUCCESS);
}



/* End. */
