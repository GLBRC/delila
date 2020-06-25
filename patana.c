/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "patana.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         2.19
/*
*/



/*














































*/



#define maxmatrix       120


typedef enum {
  a, c, g, t
} base;

typedef long matrix[4][maxmatrix];


Static _TEXT anal, pattern;
Static matrix wmatrix;
Static long i, width, beginning, sumsum, maxsum, minsum;
Static double varsum, avesum;
Static long funcmin, nfuncmax;


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


Static jmp_buf _JL1;



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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


Static Void positsum(i)
long i;
{
  /*

*/
  base ba;
  double vari = 0.0, ave = 0.0;
  long sum = 0;

  for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1)) {
    sum += wmatrix[(long)ba][i-1];
    vari += wmatrix[(long)ba][i-1] * wmatrix[(long)ba][i-1];
  }
  ave = sum / 4.0;
  vari = vari / 4 - ave * ave;
  sumsum += sum;
  varsum += vari;
  avesum += ave;
  fprintf(anal.f, "%8ld%8.2f%10.2f", sum, ave, vari);
}


Static Void positmax(i)
long i;
{
  long max;

  max = wmatrix[(long)a][i-1];
  if (max < wmatrix[(long)c][i-1])
    max = wmatrix[(long)c][i-1];
  if (max < wmatrix[(long)g][i-1])
    max = wmatrix[(long)g][i-1];
  if (max < wmatrix[(long)t][i-1])
    max = wmatrix[(long)t][i-1];
  maxsum += max;
  fprintf(anal.f, "%6ld", max);
}


Static Void positmin(i)
long i;
{
  long min;

  min = wmatrix[(long)a][i-1];
  if (min > wmatrix[(long)c][i-1])
    min = wmatrix[(long)c][i-1];
  if (min > wmatrix[(long)g][i-1])
    min = wmatrix[(long)g][i-1];
  if (min > wmatrix[(long)t][i-1])
    min = wmatrix[(long)t][i-1];
  minsum += min;
  fprintf(anal.f, "%6ld\n", min);
}


Static Void header()
{
  fprintf(anal.f, " patana version %4.2f; analysis of pattern matrix of \n",
	  version);
  fscanf(pattern.f, "%*[^\n]");
  getc(pattern.f);
  copyaline(&pattern, &anal);
  putc('\n', anal.f);
}


Static Void analyze()
{
  /*
*/
  base ba;
  long i, FORLIM;

  fprintf(anal.f, " minimum value for    functional sequences: ");
  fprintf(anal.f, "%4ld\n", funcmin);
  fprintf(anal.f, " maximum value for nonfunctional sequences: ");
  fprintf(anal.f, "%4ld\n\n", nfuncmax);
  fprintf(anal.f, " range: %ld to %ld  width: %ld\n\n",
	  beginning, beginning + width - 1, width);


  fprintf(anal.f, "  pos. %5c%5c%5c%5c", 'a', 'c', 'g', 't');
  fprintf(anal.f, "%10s%8s%10s%6s%6s\n", "sum", "ave", "var", "max", "min");

  fprintf(anal.f, "%6c", ' ');
  for (i = 1; i <= 23; i++)
    putc('-', anal.f);
  putc('\n', anal.f);

  FORLIM = width;
  for (i = 1; i <= FORLIM; i++) {
    fprintf(anal.f, " %4ld :", beginning + i - 1);
    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1))
      fprintf(anal.f, " %4ld", wmatrix[(long)ba][i-1]);
    fprintf(anal.f, " :");

    positsum(i);
    positmax(i);
    positmin(i);
  }

  fprintf(anal.f, "%6c", ' ');
  for (i = 1; i <= 23; i++)
    putc('-', anal.f);
  fprintf(anal.f, "    ----  ------    ------  ----  ----\n");
  for (i = 1; i <= 29; i++)
    putc(' ', anal.f);
  fprintf(anal.f, "%8ld%8.2f%10.2f%6ld%6ld\n",
	  sumsum, avesum, varsum, maxsum, minsum);
}


Static Void initialize()
{
  printf("patana %4.2f\n", version);
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
  if (BUFEOF(pattern.f)) {
    printf("no pattern file\n");
    halt();
  }
  if (*anal.name != '\0') {
    if (anal.f != NULL)
      anal.f = freopen(anal.name, "w", anal.f);
    else
      anal.f = fopen(anal.name, "w");
  } else {
    if (anal.f != NULL)
      rewind(anal.f);
    else
      anal.f = tmpfile();
  }
  if (anal.f == NULL)
    _EscIO2(FileNotFound, anal.name);
  SETUPBUF(anal.f, Char);
  maxsum = 0;
  minsum = 0;
  avesum = 0.0;
  sumsum = 0;
  varsum = 0.0;
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  pattern.f = NULL;
  strcpy(pattern.name, "pattern");
  anal.f = NULL;
  strcpy(anal.name, "anal");
  initialize();
  header();
  readmatrix(&pattern, wmatrix, &beginning, &width);
  readthresholds(&pattern, &funcmin, &nfuncmax);
  analyze();
_L1:
  if (anal.f != NULL)
    fclose(anal.f);
  if (pattern.f != NULL)
    fclose(pattern.f);
  exit(EXIT_SUCCESS);
}



/* End. */
