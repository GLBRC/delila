/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "alword.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*




*/



#define version         2.07
/*

*/



/*






















































*/



#define maxpos          1000



typedef struct sequence {
  Char letters[maxpos];
  long length;
} sequence;


Static _TEXT words, alwordp, symvec;


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



Static Char capitalize(c)
Char c;
{
  long n = c;

  if (n >= 'a' && n <= 'z')
    c = _toupper(n);

  return c;
}


Static Void die()
{
  if (*symvec.name != '\0') {
    if (symvec.f != NULL)
      symvec.f = freopen(symvec.name, "w", symvec.f);
    else
      symvec.f = fopen(symvec.name, "w");
  } else {
    if (symvec.f != NULL)
      rewind(symvec.f);
    else
      symvec.f = tmpfile();
  }
  if (symvec.f == NULL)
    _EscIO2(FileNotFound, symvec.name);
  SETUPBUF(symvec.f, Char);
  halt();
}


Static Void readsequence(f, oneperline, s)
_TEXT *f;
boolean oneperline;
sequence *s;
{
  /*


*/
  Char c;
  boolean done = false;
  long i = 0;


  /*

*/
  skipblanks(f);
  while (!done) {
    c = getc(f->f);

    if (c == '\n')
      c = ' ';
    /*

*/

    i++;
    if (i > maxpos) {
      printf("A sequence is longer than constant maxpos (%ld).\n",
	     (long)maxpos);
      die();
    } else
      s->letters[i-1] = c;

    if (P_eoln(f->f)) {
      done = true;
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
    }
    /*





*/
  }


  if (i == 0) {
    printf("zero length sequence found!\n");
    die();
  }

  else
    s->length = i;
}



Static Void themain(words, alwordp, symvec)
_TEXT *words, *alwordp, *symvec;
{
  Char alignment;
  double avarhnb;
  long b;
  Char c;
  double e;
  boolean endalignment;
  double f;
  long i, j;
  long imax = 0;
  double ln2 = log(2.0);
  double hmax;
  long maxword;
  long n = 0;
  long ntrue;
  long nbi[27][maxpos];
  /*
*/
  boolean oneperline;
  double r;
  long s = 26;
  boolean skipped;
  /*
*/
  sequence seq;

  printf("alword %4.2f\n", version);
  if (*words->name != '\0') {
    if (words->f != NULL)
      words->f = freopen(words->name, "r", words->f);
    else
      words->f = fopen(words->name, "r");
  } else
    rewind(words->f);
  if (words->f == NULL)
    _EscIO2(FileNotFound, words->name);
  RESETBUF(words->f, Char);
  if (*symvec->name != '\0') {
    if (symvec->f != NULL)
      symvec->f = freopen(symvec->name, "w", symvec->f);
    else
      symvec->f = fopen(symvec->name, "w");
  } else {
    if (symvec->f != NULL)
      rewind(symvec->f);
    else
      symvec->f = tmpfile();
  }
  if (symvec->f == NULL)
    _EscIO2(FileNotFound, symvec->name);
  SETUPBUF(symvec->f, Char);
  fprintf(symvec->f, "* alword %4.2f\n", version);


  if (*alwordp->name != '\0') {
    if (alwordp->f != NULL)
      alwordp->f = freopen(alwordp->name, "r", alwordp->f);
    else
      alwordp->f = fopen(alwordp->name, "r");
  } else
    rewind(alwordp->f);
  if (alwordp->f == NULL)
    _EscIO2(FileNotFound, alwordp->name);
  RESETBUF(alwordp->f, Char);
  if (BUFEOF(alwordp->f)) {
    alignment = ' ';
    oneperline = true;
  } else {
    fscanf(alwordp->f, "%c%*[^\n]", &alignment);
    getc(alwordp->f);

    if (alignment == '\n')
      alignment = ' ';
    if (!BUFEOF(alwordp->f)) {
      fscanf(alwordp->f, "%ld%*[^\n]", &maxword);
      getc(alwordp->f);
    } else {
      maxword = 20;
      printf("maximum word size: %ld\n", maxword);
    }
  }
  endalignment = (alignment == 'e');
  if (!BUFEOF(alwordp->f)) {
    if (P_peek(alwordp->f) == 'a')
      oneperline = false;
    else
      oneperline = true;
    fscanf(alwordp->f, "%*[^\n]");
    getc(alwordp->f);
  }


  for (i = 0; i < maxpos; i++) {
    for (b = 0; b <= 26; b++)
      nbi[b][i] = 0;
  }

  while (!BUFEOF(words->f)) {
    while (P_peek(words->f) == '*') {
      fprintf(symvec->f, "*\n");
      copyaline(words, symvec);
    }
    n++;
    readsequence(words, oneperline, &seq);
    /*

*/

    skipped = false;
    for (i = 0; i < seq.length; i++) {
      c = capitalize(seq.letters[i]);

      if (c < 'A' || c > 'Z')
	b = 0;
      else
	b = c - 'A' + 1;
      /*

*/

      if (endalignment) {
	if (seq.length > maxword) {
	  if (!skipped) {
	    printf("skipping ");
	    for (j = 0; j < seq.length; j++)
	      putchar(seq.letters[j]);
	    putchar('\n');
	    skipped = true;
	  }
	} else {
	  j = maxword - seq.length + i + 1;
	  if (j > 0)
	    nbi[b][j-1]++;
	}
      } else
	nbi[b][i]++;
    }
    /*

*/
    if (seq.length > imax)
      imax = seq.length;
  }

  /*
*/
  if (endalignment)
    imax = maxword;

  hmax = log((double)s);

  fprintf(symvec->f, "%ld number of symbols\n", s);


  for (i = 1; i <= imax; i++) {
    ntrue = 0;
    for (b = 1; b <= 26; b++)
      ntrue += nbi[b][i-1];
    /*





*/



    if (ntrue <= 0) {
      printf(" WARNING: something is really STRANGE!");
      printf("  Position %ld has %ld sequences!\n", i, ntrue);
      r = 0.0;
    } else {
      r = hmax;
      for (b = 1; b <= 26; b++) {
	f = (double)nbi[b][i-1] / ntrue;
	if (f > 0)
	  r += f * log(f);
      }
      r /= ln2;


      e = (s - 1) / (2 * ln2 * ntrue);
      r -= e;
      avarhnb = e * e;
      if (r < 0.0)
	r = 0.0;
    }


    fprintf(symvec->f,
      "%5ld %5ld %8.5f %8.5f (position, samples, information, variance)\n",
      i, ntrue, r, avarhnb);
    for (b = 1; b <= 26; b++)
      fprintf(symvec->f, "%c %4ld\n", (Char)(b - 1 + 'A'), nbi[b][i-1]);
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  symvec.f = NULL;
  strcpy(symvec.name, "symvec");
  alwordp.f = NULL;
  strcpy(alwordp.name, "alwordp");
  words.f = NULL;
  strcpy(words.name, "words");
  themain(&words, &alwordp, &symvec);
_L1:
  if (words.f != NULL)
    fclose(words.f);
  if (alwordp.f != NULL)
    fclose(alwordp.f);
  if (symvec.f != NULL)
    fclose(symvec.f);
  exit(EXIT_SUCCESS);
}



/* End. */
