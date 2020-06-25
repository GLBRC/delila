/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "alpro.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         2.09
/*




















































*/
#define updateversion   1.99



/*














































































































































































































































































































*/



#define infofield       8
#define infodecim       5
#define kickover        50
#define nfield          4
#define linelength      60

/*





























*/

#define maxpos          35000L




typedef struct sequence {
  Char letters[maxpos];
  long length;
} sequence;


Static _TEXT protseq, alprop, symvec, sequ;


Static double fprimeo[27][maxpos];
Static long nbi[27][maxpos];


Static jmp_buf _JL1;


/*
*/


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



Static boolean alphabetic(c)
Char c;
{
  return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}



Static Void readsequence(f, s, ok)
_TEXT *f;
sequence *s;
boolean *ok;
{
  /*


*/
  Char c;
  boolean done = false;
  long i = 0;

  *ok = true;

  while (!done && *ok) {
    while ((!BUFEOF(f->f)) & ((P_peek(f->f) == '*') | (P_peek(f->f) == '>'))) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
    }

    if (BUFEOF(f->f)) {
      printf("end of file without end of sequence\n");
      *ok = false;
    }


    while (!P_eoln(f->f) && !done) {
      c = getc(f->f);

      if (c == '\n')
	c = ' ';
      if (c == '.') {
	done = true;
	break;
      }
      if (c == ' ') {
	printf("no spaces allowed in the sequences\n");
	*ok = false;
	continue;
      }
      i++;
      if (i > maxpos) {
	printf("A sequence is longer than constant maxpos (%ld).\n", maxpos);
	*ok = false;
      } else
	s->letters[i-1] = c;
    }
    fscanf(f->f, "%*[^\n]");
    getc(f->f);
  }


  if (i == 0) {
    printf("zero length sequence found!\n");
    *ok = false;
  }

  else
    s->length = i;
}



Static Void readfasta(f, s, firstsequence, ok)
_TEXT *f;
sequence *s, *firstsequence;
boolean *ok;
{
  /*


*/
  Char c;
  boolean done = false;
  long i = 0;

  *ok = true;

  while (!done && *ok) {
    while ((!BUFEOF(f->f)) & (P_peek(f->f) == '>')) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
    }

    if (BUFEOF(f->f))
      done = true;
    else {
      while (!P_eoln(f->f)) {
	c = getc(f->f);

	if (c == '\n')
	  c = ' ';
	if (!(alphabetic(c) || c == '-' || c == '.')) {
	  if (c != ' ') {
	    printf(
	      "only alphabetic characters are allowed in the sequences, not \"%c\"\n",
	      c);
	    *ok = false;
	  }
	  continue;
	}
	i++;
	if (i > maxpos) {
	  if (*ok)
	    printf(
	      "A sequence is longer than constant maxpos (%ld). at position %ld\n",
	      maxpos, i);
	  *ok = false;
	} else {
	  if (c == '.') {
	    /*


*/
	    i--;
	  } else
	    s->letters[i-1] = c;
	}
      }

    }

    if (BUFEOF(f->f)) {
      done = true;
      break;
    }
    if (P_eoln(f->f)) {
      fscanf(f->f, "%*[^\n]");
      getc(f->f);
    }
    /*
*/
    if (!BUFEOF(f->f)) {
      if (P_peek(f->f) == '>')
	done = true;
    } else
      done = true;
  }


  if (i == 0) {
    printf("zero length sequence found!\n");
    *ok = false;
  }

  else
    s->length = i;
  if (*ok == false)
    printf("length seen: %ld\n", i);
}



Static Void writesequence(f, s, linewidth)
_TEXT *f;
sequence s;
long linewidth;
{
  long i;
  long line = 0;

  for (i = 0; i < s.length; i++) {
    if (line > linewidth) {
      putc('\n', f->f);
      line = 0;
    }
    putc(s.letters[i], f->f);
    line++;
  }
}



Static Char capitalize(c)
Char c;
{
  long n = c;

  if (n >= 'a' && n <= 'z')
    c = _toupper(n);

  return c;
}


#define maxsize         200
#define accuracy        10000




Static Void calehnb(n, gna, gnc, gng, gnt, hg, ehnb, varhnb)
long n, gna, gnc, gng, gnt;
double *hg, *ehnb, *varhnb;
{
  /*









*/
  /*
*/

  double log2Delila = log(2.0);
  double logn, nlog2;

  long gn;
  double logpa, logpc, logpg, logpt;

  /*
*/
  double logfact[maxsize + 1];

  /*
*/
  double mplog2p[maxsize + 1];

  long i;
  double logi;

  long na;
  long nc = 0, ng = 0, nt = 0;
  boolean done = false;

  double pnb;
  /*
*/
  double hnb, pnbhnb;
  double sshnb = 0.0;


  double total = 0.0;
  /*
*/
  long counter = 0;

  /*
*/


  if (n > maxsize) {
    printf(" procedure calehnb: n > maxsize (%ld>%ld)\n", n, (long)maxsize);
    halt();
  }

  logn = log((double)n);
  nlog2 = n * log2Delila;


  gn = gna + gnc + gng + gnt;
  logpa = log((double)gna / gn);
  logpc = log((double)gnc / gn);
  logpg = log((double)gng / gn);
  logpt = log((double)gnt / gn);


  *hg = -((gna * logpa + gnc * logpc + gng * logpg + gnt * logpt) /
	  (gn * log2Delila));

  *ehnb = 0.0;

  /*
*/
  logfact[0] = 0.0;
  mplog2p[0] = 0.0;
  for (i = 1; i <= n; i++) {
    logi = log((double)i);
    logfact[i] = logfact[i-1] + logi;
    mplog2p[i] = i * (logn - logi) / nlog2;
  }


  na = n;

  /*

























*/

  do {
    /*









*/

    pnb = exp(logfact[n] - logfact[na] - logfact[nc] - logfact[ng] -
	      logfact[nt] + na * logpa + nc * logpc + ng * logpg + nt * logpt);

    hnb = mplog2p[na] + mplog2p[nc] + mplog2p[ng] + mplog2p[nt];

    pnbhnb = pnb * hnb;

    *ehnb += pnbhnb;

    sshnb += pnbhnb * hnb;

    /*
*/
    counter++;
    /*







*/
    total += pnb;

    /*



*/
    if (nt > 0) {
      if (ng > 0) {
	ng--;
	nt++;
      } else if (nc > 0) {
	/*
*/
	nc--;
	ng = nt + 1;
	nt = 0;
      } else if (na > 0) {
	/*
*/
	na--;
	nc = nt + 1;
	nt = 0;
      } else
	done = true;
    } else {
      if (ng > 0) {
	ng--;
	nt++;
      } else if (nc > 0) {
	nc--;
	ng++;
      } else {
	na--;
	nc++;
      }
    }
  } while (!done);


  *varhnb = sshnb - *ehnb * *ehnb;

  /*
*/
  if (accuracy != (long)floor(accuracy * total + 0.5)) {
    printf(" procedure calehnb: the sum of probabilities is\n");
    printf(" not accurate to one part in %ld\n", (long)accuracy);
    printf(" the sum of the probabilities is %10.8f\n", total);
  }

  /*

*/
  if (counter == (long)floor((n + 1.0) * (n + 2) * (n + 3) / 6 + 0.5))
    return;
  /*


*/
  printf(" procedure calehnb: program error, the number of\n");
  printf(" calculations is in error\n");
  halt();
}

#undef maxsize
#undef accuracy




Static Void calaehnb(n, gna, gnc, gng, gnt, hg, aehnb, avarhnb)
long n, gna, gnc, gng, gnt;
double *hg, *aehnb, *avarhnb;
{
  /*











*/
  double log2Delila = log(2.0);

  long gn;
  double pa, pc, pg, pt, e;


  gn = gna + gnc + gng + gnt;
  pa = (double)gna / gn;
  pc = (double)gnc / gn;
  pg = (double)gng / gn;
  pt = (double)gnt / gn;

  *hg = -((pa * log(pa) + pc * log(pc) + pg * log(pg) + pt * log(pt)) / log2Delila);

  e = 3 / (2 * log2Delila * n);

  *aehnb = *hg - e;

  *avarhnb = e * e;
}


#define normalsum       100000L



Static Void themain(protseq, alprop, symvec, sequ)
_TEXT *protseq, *alprop, *symvec, *sequ;
{
  /*
*/
  long alignment = 0;
  long b;
  Char c;
  double e, f;
  boolean fasta;
  /*


*/
  long gna = 1, gnc = 1, gng = 1, gnt = 1;
  long i;
  long imax = 0;
  double ln2 = log(2.0);
  double hg, Hgenomic;
  long gaps = 0;
  double ehnb, Hmax;
  boolean lengthchange;
  long n = 0, na = 1, nc = 1, ng = 1, nt = 1;
  /*
*/
  boolean normalizing;
  double normsum;
  long ntotal, ntrue;
  /*



*/
  double parameterversion;
  long previouslength = -LONG_MAX;
  double rho[27];
  double r;
  boolean RNA = false;
  /*
*/
  long s = 4;
  double avarhnb;
  sequence seq, seq1;
  boolean sequdo;
  boolean varlogo = false, willdie = false;
  boolean ok;
  _TEXT TEMP;

  printf("alpro %4.2f\n", version);
  if (*protseq->name != '\0') {
    if (protseq->f != NULL)
      protseq->f = freopen(protseq->name, "r", protseq->f);
    else
      protseq->f = fopen(protseq->name, "r");
  } else
    rewind(protseq->f);
  if (protseq->f == NULL)
    _EscIO2(FileNotFound, protseq->name);
  RESETBUF(protseq->f, Char);
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


  if (*alprop->name != '\0') {
    if (alprop->f != NULL)
      alprop->f = freopen(alprop->name, "r", alprop->f);
    else
      alprop->f = fopen(alprop->name, "r");
  } else {


    rewind(alprop->f);
  }
  if (alprop->f == NULL)
    _EscIO2(FileNotFound, alprop->name);
  RESETBUF(alprop->f, Char);
  if (!BUFEOF(alprop->f)) {
    fscanf(alprop->f, "%lg%*[^\n]", &parameterversion);
    getc(alprop->f);
    /*



*/
    if ((long)floor(100 * parameterversion + 0.5) <
	(long)floor(100 * updateversion + 0.5)) {
      printf("You have an old parameter file!\n");

      if (parameterversion > 1.00)
	halt();
      else {
	printf("WARNING: automatically upgrading alprop\n");
	if (*alprop->name != '\0') {
	  if (alprop->f != NULL)
	    alprop->f = freopen(alprop->name, "w", alprop->f);
	  else
	    alprop->f = fopen(alprop->name, "w");
	} else {
	  if (alprop->f != NULL)
	    rewind(alprop->f);
	  else
	    alprop->f = tmpfile();
	}
	if (alprop->f == NULL)
	  _EscIO2(FileNotFound, alprop->name);
	SETUPBUF(alprop->f, Char);
	fprintf(alprop->f,
	  "%4.2f  version of alpro that this parameter file is designed for.\n",
	  version);
	fprintf(alprop->f, "1        alignment point\n");
	fprintf(alprop->f, "1 1 1 1  normalization bases\n");
	fprintf(alprop->f, "normal   a first letter 'v' will give varlogo\n");
	fprintf(alprop->f, "1 1 1 1  genomic composition\n");
	fprintf(alprop->f,
		"sequ     a first letter 's' will give the sequ file\n");
	if (*alprop->name != '\0') {
	  if (alprop->f != NULL)
	    alprop->f = freopen(alprop->name, "r", alprop->f);
	  else
	    alprop->f = fopen(alprop->name, "r");
	} else
	  rewind(alprop->f);
	if (alprop->f == NULL)
	  _EscIO2(FileNotFound, alprop->name);
	RESETBUF(alprop->f, Char);
	fscanf(alprop->f, "%*[^\n]");
	getc(alprop->f);
      }
    }

    fscanf(alprop->f, "%ld%*[^\n]", &alignment);
    getc(alprop->f);

    if (!BUFEOF(alprop->f)) {
      fscanf(alprop->f, "%ld%ld%ld%ld%*[^\n]", &na, &nc, &ng, &nt);
      getc(alprop->f);
    }

    if (!BUFEOF(alprop->f)) {
      if (P_peek(alprop->f) == 'v')
	varlogo = true;
      else
	varlogo = false;
    } else {
      printf("missing most of the alprop file\n");
      halt();
    }
    fscanf(alprop->f, "%*[^\n]");

    getc(alprop->f);
    if (!BUFEOF(alprop->f)) {
      fscanf(alprop->f, "%ld%ld%ld%ld%*[^\n]", &gna, &gnc, &gng, &gnt);
      getc(alprop->f);
    }

    if (!BUFEOF(alprop->f)) {
      if (P_peek(alprop->f) == 's')
	sequdo = true;
      else
	sequdo = false;
    } else
      sequdo = false;
    fscanf(alprop->f, "%*[^\n]");
    getc(alprop->f);
  }

  if (na != nc || nc != ng || ng != nt)
    normalizing = true;
  else
    normalizing = false;

  if (varlogo)
    printf("Creating varlogo form of data.\n");
  else
    printf("Creating logo form of data.\n");

  if (na < 1 || nc < 1 || ng < 1 || nt < 0) {
    printf("normalizing integers must be positive\n");
    halt();
  }

  ntotal = na + nc + ng + nt;

  if (ntotal < 1) {
    printf("alpro: sum normalizing integers must be positive\n");
    halt();
  }

  printf("alignment at %ld\n", alignment);
  printf("bases for normalization: %ld %ld %ld %ld\n", na, nc, ng, nt);
  printf("genomic composition: %ld %ld %ld %ld\n", gna, gnc, gng, gnt);

  if (BUFEOF(protseq->f)) {
    printf("empty of protseq file\n");
    halt();
  }
  if (P_peek(protseq->f) == '>')
    fasta = true;
  else if (P_peek(protseq->f) == '*')
    fasta = false;
  else {
    printf("protseq must be either fasta or original format\n");
    halt();
  }

  if (fasta)
    printf("Fasta format\n");
  else
    printf("protseq format\n");


  fprintf(symvec->f, "* alpro %4.2f\n", version);
  if (fasta) {
    while ((!BUFEOF(protseq->f)) & (P_peek(protseq->f) == '>')) {
      getc(protseq->f);
      putc('*', symvec->f);
      copyaline(protseq, symvec);
    }
  } else {
    while ((!BUFEOF(protseq->f)) & (P_peek(protseq->f) == '*'))
      copyaline(protseq, symvec);
  }


  for (b = 0; b <= 26; b++) {
    for (i = 0; i < maxpos; i++) {
      nbi[b][i] = 0;
      fprimeo[b][i] = 0.0;
    }
    rho[b] = 0.0;
  }


  if (sequdo) {
    printf("Writing sequ file\n");
    if (*sequ->name != '\0') {
      if (sequ->f != NULL)
	sequ->f = freopen(sequ->name, "w", sequ->f);
      else
	sequ->f = fopen(sequ->name, "w");
    } else {
      if (sequ->f != NULL)
	rewind(sequ->f);
      else
	sequ->f = tmpfile();
    }
    if (sequ->f == NULL)
      _EscIO2(FileNotFound, sequ->name);
    SETUPBUF(sequ->f, Char);
  }

  while (!BUFEOF(protseq->f)) {
    n++;
    if (fasta)
      readfasta(protseq, &seq, &seq1, &ok);
    else
      readsequence(protseq, &seq, &ok);

    if (sequdo) {
      if (n == 1) {
	seq1 = seq;
	printf("copied sequence:\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writesequence(&TEMP, seq1, (long)linelength);
      }
      if (n == 2) {
	printf("sequence %ld:\n", n);
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writesequence(&TEMP, seq, (long)linelength);
      }
    }



    if (sequdo) {
      writesequence(sequ, seq, (long)linelength);

      fprintf(sequ->f, ".\n");
    }

    if (!ok)
      willdie = true;
    /*


*/

    if (seq.length != previouslength) {
      printf("\nsequence %ld is %3ld letters long\n", n, seq.length);
      previouslength = seq.length;
      lengthchange = true;
    } else {
      if (lengthchange) {
	printf("\n... (only sequences that differ in length will be noted)\n");
	printf("The range is: from %*ld to %*ld\n",
	       nfield, 1 - alignment, nfield, seq.length - alignment);
	lengthchange = false;
      }
    }


    for (i = 0; i < seq.length; i++) {
      c = capitalize(seq.letters[i]);
      if (c == 'U')
	RNA = true;

      if (c < 'A' || c > 'Z')
	b = 0;
      else
	b = c - 'A' + 1;


      if (s != 26) {
	if (c != '-' && c != 'U' && c != 'T' && c != 'G' && c != 'C' &&
	    c != 'A') {
	  if (c == '-' || c == 'Z' || c == 'X' || c == 'U' || c == 'O' ||
	      c == 'J' || c == 'B') {
	    printf("The character \"%c\" is not valid for DNA, RNA or proteins\n",
		   seq.letters[i]);
	    s = 26;
	    printf("Number of symbols has been changed to %ld\n", s);
	  }
	  /*

*/

	  else if (s == 4) {
	    printf("The character \"%c\" is not valid for DNA or RNA\n",
		   seq.letters[i]);
	    /*

*/
	    s = 20;
	    printf("Number of symbols has been changed to %ld\n", s);
	  }
	}
      }

      nbi[b][i]++;
    }
    if (seq.length > imax)
      imax = seq.length;
  }
  if (willdie) {
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
    halt();
  }

  Hmax = log((double)s) / ln2;

  if (s == 4) {
    ntrue = gna + gnc + gng + gnt;
    Hgenomic = -(((double)gna / ntrue * log((double)gna / ntrue) +
		  (double)gnc / ntrue * log((double)gnc / ntrue) +
		  (double)gng / ntrue * log((double)gng / ntrue) +
		  (double)gnt / ntrue * log((double)gnt / ntrue)) / ln2);
    printf("Hgenomic = %*.*f bits\n", infofield, infodecim, Hgenomic);
  }

  else
    Hgenomic = Hmax;
  printf("Number of symbols, s = %ld\n", s);
  printf("Hmax = %5.2f bits\n", Hmax);

  if (s != 4)
    RNA = false;
  if (s == 26)
    fprintf(symvec->f, "* ALPHABETIC ALIGNMENT\n");
  if (s == 20)
    fprintf(symvec->f, "* PROTEIN ALIGNMENT\n");
  if (s == 4 && !RNA)
    fprintf(symvec->f, "* DNA ALIGNMENT\n");
  if (s == 4 && RNA)
    fprintf(symvec->f, "* RNA ALIGNMENT\n");

  if (s != 4 && normalizing) {
    printf("normalization is only possible for DNA/RNA\n");
    halt();
  }

  fprintf(symvec->f, "* position, samples, information, variance\n");
  fprintf(symvec->f, "%ld number of symbols\n", s);


  for (i = 0; i < imax; i++) {
    ntrue = 0;
    for (b = 1; b <= 26; b++)
      ntrue += nbi[b][i];

    if (normalizing) {
      normsum = 0.0;
      for (b = 1; b <= 26; b++) {
	c = (Char)(b + 'A' - 1);
	if (c == 'T' || c == 'G' || c == 'C' || c == 'A') {
	  switch (c) {

	  case 'A':
	    rho[b] = 1 / (s * ((double)na / ntotal));
	    break;

	  case 'C':
	    rho[b] = 1 / (s * ((double)nc / ntotal));
	    break;

	  case 'G':
	    rho[b] = 1 / (s * ((double)ng / ntotal));
	    break;

	  case 'T':
	    rho[b] = 1 / (s * ((double)nt / ntotal));
	    break;
	  }

	  normsum = (double)nbi[b][i] / ntrue * rho[b] + normsum;
	} else
	  rho[b] = 0.0;
      }
    }

    if (ntrue <= 0) {
      gaps++;
      if (gaps == 1) {
	printf(" WARNING: something is really STRANGE!");
	printf("  The following positions have %ld sequences!\n", ntrue);
	printf(" %ld", i - alignment + 1);
      } else
	printf(" %ld", i - alignment + 1);
      if (gaps % 10 == 0)
	putchar('\n');
      r = 0.0;
      avarhnb = 0.0;
    } else {
      r = 0.0;

      for (b = 1; b <= 26; b++) {
	if (normalizing) {
	  fprimeo[b][i] = (double)nbi[b][i] / ntrue * rho[b] / normsum;
	  f = fprimeo[b][i];
	}

	else
	  f = (double)nbi[b][i] / ntrue;

	if (f > 0)
	  r += f * log(f);
      }

      r = Hmax + r / ln2;

      if (ntrue <= kickover && s == 4) {
	/*
*/
	calehnb(ntrue, gna, gnc, gng, gnt, &hg, &ehnb, &avarhnb);


	r += ehnb - hg - Hmax + Hgenomic;

      } else {
	e = (s - 1) / (2 * ln2 * ntrue);

	r += Hgenomic - Hmax - e;

	/*
*/
	avarhnb = e * e;


	/*

*/
	/*

*/


      }
    }

    if (normalizing) {
      ntrue = 0;
      for (b = 1; b <= 26; b++)
	ntrue += (long)floor(fprimeo[b][i] * normalsum + 0.5);
    }


    if (varlogo)
      r = Hmax - r;



    fprintf(symvec->f, "%*ld %*ld %*.*f % .*E\n",
	    nfield, i - alignment + 1, infofield, ntrue, infofield, infodecim,
	    r, P_max((int)(infofield + 3) - 7, 1), avarhnb);


    if (s == 20) {
      for (b = 1; b <= 26; b++) {
	c = (Char)(b + 'A' - 1);
	if (c != 'Z' && c != 'X' && c != 'U' && c != 'O' && c != 'J' &&
	    c != 'B')
	  fprintf(symvec->f, "%c %4ld\n", c, nbi[b][i]);
      }
    }

    else if (s == 4) {

      for (b = 1; b <= 26; b++) {
	c = (Char)(b + 'A' - 1);

	/*










*/

	if (c == 'G' || c == 'C' || c == 'A' || c == 'T' && !RNA ||
	    c == 'U' && RNA) {
	  if (normalizing)
	    fprintf(symvec->f, "%c %4ld\n",
		    c, (long)floor(fprimeo[b][i] * normalsum + 0.5));
	  else
	    fprintf(symvec->f, "%c %4ld\n", c, nbi[b][i]);
	}

      }
    } else {
      for (b = 1; b <= 26; b++) {
	c = (Char)(b + 'A' - 1);
	fprintf(symvec->f, "%c %4ld\n", c, nbi[b][i]);

      }
    }
  }


  /*

*/



  if (gaps > 0)
    printf("\nThere are %ld extra columns of gaps in your data.\n", gaps);
}

#undef normalsum


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  sequ.f = NULL;
  strcpy(sequ.name, "sequ");
  symvec.f = NULL;
  strcpy(symvec.name, "symvec");
  alprop.f = NULL;
  strcpy(alprop.name, "alprop");
  protseq.f = NULL;
  strcpy(protseq.name, "protseq");
  themain(&protseq, &alprop, &symvec, &sequ);
_L1:
  if (protseq.f != NULL)
    fclose(protseq.f);
  if (alprop.f != NULL)
    fclose(alprop.f);
  if (symvec.f != NULL)
    fclose(symvec.f);
  if (sequ.f != NULL)
    fclose(sequ.f);
  exit(EXIT_SUCCESS);
}



/* End. */
