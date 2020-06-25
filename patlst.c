/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "patlst.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/



#define version         1.09
/*
*/



/*
































*/


#define numfield        4
#define defpagewidth    30


#define maxmatrix       120


typedef enum {
  a, c, g, t
} base;

typedef long matrix[4][maxmatrix];


Static _TEXT pattern, patout, patlstp;
Static matrix wmatrix;
Static long pagewidth, funcmin, nfuncmax, beginning, width;


Static jmp_buf _JL1;




Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void unlimitln(afile)
_TEXT *afile;
{
  /*


*/
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






Static Char basetochar(ba)
base ba;
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


#define learnmax        16



Static Void findlearnend(pattern)
_TEXT *pattern;
{
  Char learnend[learnmax];
  long state = 1;

  if (*pattern->name != '\0') {
    if (pattern->f != NULL)
      pattern->f = freopen(pattern->name, "r", pattern->f);
    else
      pattern->f = fopen(pattern->name, "r");
  } else
    rewind(pattern->f);
  if (pattern->f == NULL)
    _EscIO2(FileNotFound, pattern->name);
  RESETBUF(pattern->f, Char);
  memcpy(learnend, "end of learning.", (long)learnmax);
  while (!BUFEOF(pattern->f) && state < learnmax) {
    if (learnend[state-1] == P_peek(pattern->f))
      state++;
    else
      state = 1;
    if (P_eoln(pattern->f)) {
      fscanf(pattern->f, "%*[^\n]");
      getc(pattern->f);
    } else
      getc(pattern->f);
  }
  if (!BUFEOF(pattern->f))
    return;
  printf("pattern matrix does not contain \"");
  printf("%.*s\" signal\n", learnmax, learnend);
  halt();
}

#undef learnmax


Static Void getcolon(f)
_TEXT *f;
{
  while (P_peek(f->f) != ':') {
    getc(f->f);
    if (BUFEOF(f->f)) {
      printf("pattern is missing colons\n");
      halt();
    }
  }
  getc(f->f);
}


Static Void readmatrix(thefile, wmatrix, beginning, width)
_TEXT *thefile;
long (*wmatrix)[maxmatrix];
long *beginning, *width;
{
  /*
*/
  long i;
  base ba;
  long FORLIM;

  findlearnend(thefile);


  getcolon(thefile);
  getcolon(thefile);

  getcolon(thefile);
  fscanf(thefile->f, "%ld", beginning);

  getcolon(thefile);
  fscanf(thefile->f, "%ld", width);

  if (*width > maxmatrix) {
    printf("input matrix too large\n");
    halt();
  }

  FORLIM = *width;
  for (i = 0; i < FORLIM; i++) {
    getcolon(thefile);
    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1))
      fscanf(thefile->f, "%ld", &wmatrix[(long)ba][i]);
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
  }

  for (i = *width; i < maxmatrix; i++) {
    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1))
      wmatrix[(long)ba][i] = 0;
  }
}


Static Void header()
{
  if (*pattern.name != '\0') {
    if (pattern.f != NULL)
      pattern.f = freopen(pattern.name, "r", pattern.f);
    else
      pattern.f = fopen(pattern.name, "r");
  } else
    rewind(pattern.f);
  if (pattern.f == NULL)
    _EscIO2(FileNotFound, pattern.name);
  RESETBUF(pattern.f, Char);
  if (*patout.name != '\0') {
    if (patout.f != NULL)
      patout.f = freopen(patout.name, "w", patout.f);
    else
      patout.f = fopen(patout.name, "w");
  } else {
    if (patout.f != NULL)
      rewind(patout.f);
    else
      patout.f = tmpfile();
  }
  if (patout.f == NULL)
    _EscIO2(FileNotFound, patout.name);
  SETUPBUF(patout.f, Char);
  fprintf(patout.f,
	  " patlst version %4.2f; listing of pattern matrix from:\n",
	  version);
  fscanf(pattern.f, "%*[^\n]");
  getc(pattern.f);
  copyaline(&pattern, &patout);
  putc('\n', patout.f);
}


Static Void setparam()
{
  if (*patlstp.name != '\0') {
    if (patlstp.f != NULL)
      patlstp.f = freopen(patlstp.name, "r", patlstp.f);
    else
      patlstp.f = fopen(patlstp.name, "r");
  } else
    rewind(patlstp.f);
  if (patlstp.f == NULL)
    _EscIO2(FileNotFound, patlstp.name);
  RESETBUF(patlstp.f, Char);
  if (BUFEOF(patlstp.f))
    pagewidth = defpagewidth;
  else {
    fscanf(patlstp.f, "%ld%*[^\n]", &pagewidth);
    getc(patlstp.f);
  }
}


Static long minimum(a_, b)
long a_, b;
{
  if (a_ < b)
    return a_;
  else
    return b;
}


Static Void readthresholds(pattern, funcmin, nfuncmax)
_TEXT *pattern;
long *funcmin, *nfuncmax;
{
  if (*pattern->name != '\0') {
    if (pattern->f != NULL)
      pattern->f = freopen(pattern->name, "r", pattern->f);
    else
      pattern->f = fopen(pattern->name, "r");
  } else
    rewind(pattern->f);
  if (pattern->f == NULL)
    _EscIO2(FileNotFound, pattern->name);
  RESETBUF(pattern->f, Char);
  findlearnend(pattern);

  getcolon(pattern);
  fscanf(pattern->f, "%ld%*[^\n]", funcmin);
  getc(pattern->f);
  getcolon(pattern);
  fscanf(pattern->f, "%ld%*[^\n]", nfuncmax);
  getc(pattern->f);
}


Static Void writepattern()
{
  long pageend;
  long i = 0;
  long j;
  base ba;
  long FORLIM;

  fprintf(patout.f, " minimum value for    functional sequences: ");
  fprintf(patout.f, "%4ld\n", funcmin);
  fprintf(patout.f, " maximum value for nonfunctional sequences: ");
  fprintf(patout.f, "%4ld\n\n", nfuncmax);
  fprintf(patout.f, " range: %ld to %ld  width: %ld\n\n",
	  beginning, beginning + width - 1, width);
  while (i < width) {
    pageend = minimum(i + pagewidth, width);

    fprintf(patout.f, " position :");
    for (j = i; j < pageend; j++)
      fprintf(patout.f, "%*ld", numfield, beginning + j);
    fprintf(patout.f, "\n%10c", ' ');

    FORLIM = (pageend - i) * numfield + 1;
    for (j = 1; j <= FORLIM; j++)
      putc('-', patout.f);
    if (pageend == width)
      fprintf(patout.f, "-\n");
    else
      putc('\n', patout.f);

    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1)) {
      fprintf(patout.f, "%8c  :", basetochar(ba));
      for (j = i; j < pageend; j++)
	fprintf(patout.f, "%*ld", numfield, wmatrix[(long)ba][j]);
      if (pageend == width)
	fprintf(patout.f, ":\n");
      else
	putc('\n', patout.f);
    }

    fprintf(patout.f, "%10c", ' ');
    FORLIM = (pageend - i) * numfield + 1;
    for (j = 1; j <= FORLIM; j++)
      putc('-', patout.f);
    if (pageend == width)
      fprintf(patout.f, "-\n");
    else
      putc('\n', patout.f);
    putc('\n', patout.f);
    i += pagewidth;
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  patlstp.f = NULL;
  strcpy(patlstp.name, "patlstp");
  patout.f = NULL;
  strcpy(patout.name, "patout");
  pattern.f = NULL;
  strcpy(pattern.name, "pattern");
  printf("patlst %4.2f\n", version);
  header();
  setparam();
  readmatrix(&pattern, wmatrix, &beginning, &width);
  readthresholds(&pattern, &funcmin, &nfuncmax);
  writepattern();
_L1:
  if (pattern.f != NULL)
    fclose(pattern.f);
  if (patout.f != NULL)
    fclose(patout.f);
  if (patlstp.f != NULL)
    fclose(patlstp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
