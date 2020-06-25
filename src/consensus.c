/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "consensus.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.17
/*












*/
#define updateversion   1.05



/*







































































































*/



#define maxstring       2000




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;


Static _TEXT symvec, consensusp, list, conmarks;


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


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
  clearstring(ribbon);
  ribbon->next = NULL;
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



Static Char capitalize(c)
Char c;
{
  long n = c;

  if (n >= 'a' && n <= 'z')
    c = _toupper(n);
  return c;
}



Static Char decapitalize(c)
Char c;
{
  long n = c;

  if (n >= 'A' && n <= 'Z')
    c = _tolower(n);
  else
    c = (Char)n;
  return c;
}



Static Char twocode(a, b)
Char a, b;
{
  /*


*/
  Char Result;

  a = decapitalize(a);
  b = decapitalize(b);
  switch (a) {

  case 'a':
    switch (b) {

    case 'a':
      Result = 'A';
      break;

    case 'c':
      Result = 'M';
      break;

    case 'g':
      Result = 'R';
      break;

    case 't':
      Result = 'W';
      break;
    }
    break;

  case 'c':
    switch (b) {

    case 'a':
      Result = 'M';
      break;

    case 'c':
      Result = 'C';
      break;

    case 'g':
      Result = 'S';
      break;

    case 't':
      Result = 'Y';
      break;
    }
    break;

  case 'g':
    switch (b) {

    case 'a':
      Result = 'R';
      break;

    case 'c':
      Result = 'S';
      break;

    case 'g':
      Result = 'G';
      break;

    case 't':
      Result = 'K';
      break;
    }
    break;

  case 't':
    switch (b) {

    case 'a':
      Result = 'W';
      break;

    case 'c':
      Result = 'Y';
      break;

    case 'g':
      Result = 'K';
      break;

    case 't':
      Result = 'T';
      break;
    }
    break;
  }
  return Result;
}



Static Char threecode(a, c, g)
Char a, c, g;
{
  /*



*/
  a = decapitalize(a);
  c = decapitalize(c);
  g = decapitalize(g);
  /*
*/
  if (a != 'a')
    return 'B';
  else {
    if (c != 'c')
      return 'D';
    else {
      if (g != 'g')
	return 'H';
      else
	return 'V';
    }
  }
}


#define wid             5
#define dec             2
#define maxs            4



Static Void themain(symvec, consensusp, list, conmarks)
_TEXT *symvec, *consensusp, *list, *conmarks;
{
  double bits = 0.0;
  Char c;
  long conposition;
  long concount = 0;
  /*


*/
  long conlevel[maxstring];
  /*
*/
  stringDelila constring[maxs];
  stringDelila compressed;
  double cutoff;
  boolean first = true;
  /*
*/
  long fromrange, n, ncl, l, d;
  double f;
  long letter;
  Char mfsymbol;
  double mffraction, parameterversion;
  double probability = 1.0;
  double Rsequence, Rvar;
  long torange, s, sindex;

  printf("consensus %4.2f\n", version);
  if (*consensusp->name != '\0') {
    if (consensusp->f != NULL)
      consensusp->f = freopen(consensusp->name, "r", consensusp->f);
    else
      consensusp->f = fopen(consensusp->name, "r");
  } else
    rewind(consensusp->f);
  if (consensusp->f == NULL)
    _EscIO2(FileNotFound, consensusp->name);
  RESETBUF(consensusp->f, Char);
  fscanf(consensusp->f, "%lg%*[^\n]", &parameterversion);
  getc(consensusp->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    if (parameterversion <= 1.05) {
      printf("You have an old parameter file!\n");
      printf("Upgrading ...\n");
      fscanf(consensusp->f, "%ld%ld%*[^\n]", &fromrange, &torange);
      getc(consensusp->f);
      if (*consensusp->name != '\0') {
	if (consensusp->f != NULL)
	  consensusp->f = freopen(consensusp->name, "w", consensusp->f);
	else
	  consensusp->f = fopen(consensusp->name, "w");
      } else {
	if (consensusp->f != NULL)
	  rewind(consensusp->f);
	else
	  consensusp->f = tmpfile();
      }
      if (consensusp->f == NULL)
	_EscIO2(FileNotFound, consensusp->name);
      SETUPBUF(consensusp->f, Char);
      fprintf(consensusp->f,
	"%4.2f    version of consensus that this parameter file is designed for.\n",
	version);
      fprintf(consensusp->f, "%3ld %3ld range to display\n",
	      fromrange, torange);
      cutoff = 0.5;
      fprintf(consensusp->f,
	"%4.2f    the lowest frequency for using a base in the consensus\n",
	cutoff);
      if (*consensusp->name != '\0') {
	if (consensusp->f != NULL)
	  consensusp->f = freopen(consensusp->name, "r", consensusp->f);
	else
	  consensusp->f = fopen(consensusp->name, "r");
      } else
	rewind(consensusp->f);
      if (consensusp->f == NULL)
	_EscIO2(FileNotFound, consensusp->name);
      RESETBUF(consensusp->f, Char);
      fscanf(consensusp->f, "%lg%*[^\n]", &parameterversion);
      getc(consensusp->f);
    }
  }
  fscanf(consensusp->f, "%ld%ld%*[^\n]", &fromrange, &torange);
  getc(consensusp->f);

  fscanf(consensusp->f, "%lg%*[^\n]", &cutoff);
  getc(consensusp->f);

  if (*symvec->name != '\0') {
    if (symvec->f != NULL)
      symvec->f = freopen(symvec->name, "r", symvec->f);
    else
      symvec->f = fopen(symvec->name, "r");
  } else
    rewind(symvec->f);
  if (symvec->f == NULL)
    _EscIO2(FileNotFound, symvec->name);
  RESETBUF(symvec->f, Char);
  if (*list->name != '\0') {
    if (list->f != NULL)
      list->f = freopen(list->name, "w", list->f);
    else
      list->f = fopen(list->name, "w");
  } else {
    if (list->f != NULL)
      rewind(list->f);
    else
      list->f = tmpfile();
  }
  if (list->f == NULL)
    _EscIO2(FileNotFound, list->name);
  SETUPBUF(list->f, Char);

  fprintf(list->f, "* consensus %4.2f\n", version);
  fprintf(list->f, "* range: %ld to %ld\n", fromrange, torange);
  fprintf(list->f, "* cutoff: %4.2f\n", cutoff);


  while (!BUFEOF(symvec->f)) {
    if (P_peek(symvec->f) == '*') {
      copyaline(symvec, list);
      continue;
    }
    if (first) {
      first = false;
      fscanf(symvec->f, "%ld%*[^\n]", &s);
      getc(symvec->f);
      fprintf(list->f, "%*ld number of symbols\n", wid, s);
      if (s > maxs) {
	printf(" number of symbols %ld exceeds the allowed maximum, maxs = %ld\n",
	       s, (long)maxs);
	halt();
      }
      for (sindex = 1; sindex <= s; sindex++)
	clearstring(&constring[s-1]);
      continue;
    }
    fscanf(symvec->f, "%ld%ld%lg%lg%*[^\n]", &l, &n, &Rsequence, &Rvar);
    getc(symvec->f);
    mfsymbol = ' ';
    mffraction = -1.0;

    if (l < fromrange || l > torange) {
      for (letter = 1; letter <= s; letter++) {
	fscanf(symvec->f, "%*[^\n]");
	getc(symvec->f);
      }
      continue;
    }



    conposition = l - fromrange + 1;
    for (sindex = 0; sindex < s; sindex++) {
      constring[sindex].letters[conposition-1] = 'N';
      constring[sindex].length++;
    }
    conlevel[conposition-1] = 0;

    for (letter = 1; letter <= s; letter++) {
      fscanf(symvec->f, "%c%ld%*[^\n]", &c, &ncl);
      getc(symvec->f);


      if (c == '\n')
	c = ' ';
      f = (double)ncl / n;

      if (f >= mffraction) {
	mffraction = f;
	mfsymbol = c;


	if (f >= cutoff) {
	  conlevel[conposition-1]++;
	  constring[conlevel[conposition-1] - 1].letters[conposition-1] =
	    capitalize(c);
	}
      }

      /*












*/

      fprintf(list->f, "%*ld %*ld %*ld %*.*f",
	      wid, l, wid, n, wid, ncl, wid, dec, f);


      fprintf(list->f, " |");
      for (d = 0; d <= 10; d++) {
	if (f < d / 10.0)
	  putc('-', list->f);
	else
	  putc(c, list->f);
      }
      fprintf(list->f, " |\n");


      if (letter == s) {
	fprintf(list->f, "* maximum fraction: %c at %*.*f\n",
		mfsymbol, wid, dec, mffraction);
	fprintf(list->f, "*\n");
	probability *= mffraction;
      }

    }

  }
  fprintf(list->f, "* probability of consensus: %*.*f\n",
	  wid, dec, probability);
  fprintf(list->f, "* Final consensus:\n");
  fprintf(list->f, "* \n");

  for (sindex = 1; sindex <= s; sindex++) {
    fprintf(list->f, "* ");
    for (l = fromrange + 1; l <= torange + 1; l++) {
      conposition = l - fromrange;
      if (constring[sindex-1].letters[conposition-1] != 'N' || sindex == 1)
	putc(constring[sindex-1].letters[conposition-1], list->f);
      else
	putc(' ', list->f);
    }
    putc('\n', list->f);
  }
  fprintf(list->f, "* \n");


  clearstring(&compressed);
  compressed.length = constring[0].length;
  for (l = fromrange + 1; l <= torange + 1; l++) {
    conposition = l - fromrange;

    /*


*/

    switch (conlevel[conposition-1]) {

    case 0:
      compressed.letters[conposition-1] = 'N';
      break;

    case 1:
      compressed.letters[conposition-1] = constring[0].letters[conposition-1];
      bits += 2.0;
      break;

    case 2:
      compressed.letters[conposition-1] = twocode(
	  constring[0].letters[conposition-1],
	  constring[1].letters[conposition-1]);
      bits += 1.0;
      break;

    case 3:
      compressed.letters[conposition-1] = threecode(
	  constring[0].letters[conposition-1],
	  constring[1].letters[conposition-1],
	  constring[2].letters[conposition-1]);
      bits += 2.0 - log(3.0) / log(2.0);
      break;

    case 4:
      compressed.letters[conposition-1] = 'n';
      break;
    }

  }
  fprintf(list->f, "* ");
  writestring(list, &compressed);
  fprintf(list->f, "\n* \n");

  fprintf(list->f, "* DO NOT PUBLISH THIS!!!\n");
  fprintf(list->f, "* Well, ok, you can if your purpose\n");
  fprintf(list->f, "* is to attack the concensus sequence concept.\n");
  fprintf(list->f, "* Otherwise, read this:\n");
  fprintf(list->f,
    "* https://alum.mit.edu/www/toms/glossary.html#consensus_sequence\n");

  if (*conmarks->name != '\0') {
    if (conmarks->f != NULL)
      conmarks->f = freopen(conmarks->name, "w", conmarks->f);
    else
      conmarks->f = fopen(conmarks->name, "w");
  } else {
    if (conmarks->f != NULL)
      rewind(conmarks->f);
    else
      conmarks->f = tmpfile();
  }
  if (conmarks->f == NULL)
    _EscIO2(FileNotFound, conmarks->name);
  SETUPBUF(conmarks->f, Char);

  for (l = fromrange; l <= torange; l++) {
    conposition = l - fromrange + 1;
    fprintf(list->f, "%5ld ", l);
    for (sindex = 0; sindex < s; sindex++) {
      if (true)
	putc(constring[sindex].letters[conposition-1], list->f);
    }
    if (constring[0].letters[conposition-1] != 'N')
      concount++;
    fprintf(list->f, " %c\n", compressed.letters[conposition-1]);

    /*


*/
    /*

*/
    fprintf(conmarks->f,
	    "U    5       0 %5ld -0.5    1.3 (%c) 0 0 0 ShiftLettering\n",
	    l, compressed.letters[conposition-1]);

  }

  fprintf(list->f, "%ld out of %ld non-N positions in the consensus\n",
	  concount, torange - fromrange + 1);

  fprintf(list->f, "%*.*f ", wid, dec, bits);
  writestring(list, &compressed);
  putc('\n', list->f);
}

#undef wid
#undef dec
#undef maxs


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  conmarks.f = NULL;
  strcpy(conmarks.name, "conmarks");
  list.f = NULL;
  strcpy(list.name, "list");
  consensusp.f = NULL;
  strcpy(consensusp.name, "consensusp");
  symvec.f = NULL;
  strcpy(symvec.name, "symvec");
  themain(&symvec, &consensusp, &list, &conmarks);
_L1:
  if (symvec.f != NULL)
    fclose(symvec.f);
  if (consensusp.f != NULL)
    fclose(consensusp.f);
  if (list.f != NULL)
    fclose(list.f);
  if (conmarks.f != NULL)
    fclose(conmarks.f);
  exit(EXIT_SUCCESS);
}



/* End. */
