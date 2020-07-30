/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dalvec.p" */
/* 
dalvec: converts Rseq rsdata file to symvec format
   by Thomas Schneider
module libraries required: delman, prgmods

name
   dalvec: converts Rseq rsdata file to symvec format

synopsis
   dalvec(rsdata: in, dalvecp: in, symvec: out, output: out)

files
   rsdata: data file from rseq program

   dalvecp: parameters to control dalvec
      If empty, then the normal sequence logo will be produced.
      If the first character of the first line is a 'c', then a chi-logo
      is produced.  The height of this logo is the information.  The
      heights of the individual letters are, however, not the frequencies,
      but rather their partial chi-square values.  The expected value
      is 1/4 of the number of characters.  This is compared to the observed
      value by:
        partial chi-square =(observed - expected)^2/expected
      These partial values are normalized and placed in symvec in place of
      the relative frequencies.  Thus the significance of each letter is
      used.  When the observed is less than expected, the reported value
      is made negative.  Makelogo prints these characters upside down.
   symvec: reformating of the rsdata file for input to the makelogo program.
      A series of header lines begining with asterisk ("*") are produced.
      The next line contains one integer which is the number of symbols
      in the vector (4 for DNA or RNA, 20 for proteins).
      After this, the format of the file is a series of entries.  Each entry
      has two parts.  The first part is on one line and contains
         position total information
         position: the position number
         total:  the sum of the values in the vector
         information: the information content of the vector.
      The remaining parameters on the line are from the rsdata file:
         rs: sum of rsl
         varhnb: variance of rsl
         sumvar: sum of varhnb
         ehnb: 2-e(n)

      The second part consists of a list of a series of symbol lines.  The
      number of these matches the numer of symbols (4 in the case of DNA),
      representing the the numbers of bases or amino acids at the position in
      an aligned set of sequences.  Each line begins with the character of the
      symbol, followed by the number of that symbols.

   output: messages to the user

description
   Convert the rsdata file from rseq into a format that the makelogo program
   can use.  The format is a 'symbol vector'.

   ChiLogos: If you leave the parameter file empty, then the standard sequence
   logo will be created.  However, if the first letter of the file is a 'c',
   then a new kind of logo will emerge from makelogo: the chi-logo.  The height
   is as it was before.  The height of the individual letters is different,
   instead of being proportional to the frequency of the letter, it is
   proportional to the significance of the letter, based on the chi-square
   test.  That is, the expected number of letters is the number of letters at
   that position, n(l) divided by 4 (for simplicity!).  The observed number
   comes from the rsdata file.  The partial-chi square is
   (observed-expected)^2/expected.  Note that the sum of the partials is the
   normal chi-square.  So bases that contribute strongly get big.  Also, bases
   that are under represented are printed UPSIDE DOWN, so you can (usually)
   tell you have a chilogo at a glance.  The chilogo allows one to see the
   importance of the infrequent letters.  The technical mechanism for making a
   letter upside down is to have its number negative in the symvec file.

examples

see also
   rseq.p, makelogo.p

author
   Thomas D. Schneider

bugs

   The program originally only created a vector that contained the characters
   of the alphabet, so the output was called an 'alvec'.  To reflect the use of
   symbols, the name of the output file was changed to symvec, but I like
   'dalvec', and 'dsymvec' is so awkward that I decided to keep the name
   dalvec.

To Compile:

gcc dalvec.c -o dalvec -I/home/mplace/bin/p2c/src -L /home/mplace/bin/p2c/src -lm -lp2c

To Run:

dalvec -r rsdata -p dalvecp 

*/
#include <getopt.h>  /* getopt API */ 
#include <stdio.h>   /* printf */
#include <stdlib.h> 
#include </home/mplace/bin/p2c/src/p2c.h>

#define version         2.20
#define infofield       8
#define infodecim       5
#define nfield          4
#define mapmax          26
#define pwid            8
#define pdec            5
#define pnum            100000L
#define cxmove          30
#define maxstring       150
#define fillermax       21

typedef struct rstype {
  long rstart, rstop, l, nal, ncl, ngl, ntl;
  double rsl, rs, varhnb, sumvar;
  long nl;
  double ehnb;
  Char flag;
} rstype;

typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;

typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;

typedef Char filler[fillermax];
Static _TEXT rsdata, dalvecp, symvec;
Static jmp_buf _JL1;

Static Void halt()
{
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

Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}

Static Void resettrigger(t)
trigger *t;
{
  t->state = 0;
  t->skip = false;
  t->found = false;
}

Static Void testfortrigger(ch, t)
Char ch;
trigger *t;
{
  t->state++;

  if (t->seek.letters[t->state - 1] == ch) {
    t->skip = false;
    if (t->state == t->seek.length)
      t->found = true;
    else
      t->found = false;
    return;
  }
  t->state = 0;
  t->skip = true;
  t->found = false;
}

Static Void fillstring(s, a)
stringDelila *s;
Char *a;
{
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

Static Void filltrigger(t, a)
trigger *t;
Char *a;
{
  fillstring(&t->seek, a);
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

Static Void header(infile, outfile)
_TEXT *infile, *outfile;
{
  long index;

  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else
    rewind(infile->f);
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);
  if (*outfile->name != '\0') {
    if (outfile->f != NULL)
      outfile->f = freopen(outfile->name, "w", outfile->f);
    else
      outfile->f = fopen(outfile->name, "w");
  } else {
    if (outfile->f != NULL)
      rewind(outfile->f);
    else
      outfile->f = tmpfile();
  }
  if (outfile->f == NULL)
    _EscIO2(FileNotFound, outfile->name);
  SETUPBUF(outfile->f, Char);

  fprintf(outfile->f, "* dalvec %4.2f\n", version);


  for (index = 1; index <= 3; index++)
    copyaline(infile, outfile);

  fprintf(outfile->f, "*\n");
  fprintf(outfile->f,
	  "* position, number of sequences, information Rs, variance of Rs\n");
}

Static Void readrsrange(rsdata, r)
_TEXT *rsdata;
rstype *r;
{
  long index;
  Char skip;

  for (index = 1; index <= 11; index++) {
    fscanf(rsdata->f, "%*[^\n]");
    getc(rsdata->f);
  }
  fscanf(rsdata->f, "%c%ld%ld%*[^\n]", &skip, &r->rstart, &r->rstop);
  getc(rsdata->f);

  if (skip == '\n')
    skip = ' ';
}

Static Void getrsbegin(infile)
_TEXT *infile;
{
  Char ch;
  trigger begindata;
  filltrigger(&begindata, "l    a    c    g    t");
  resettrigger(&begindata);

  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else
    rewind(infile->f);
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);
  while (!begindata.found) {
    if (P_eoln(infile->f)) {
      fscanf(infile->f, "%*[^\n]");
      getc(infile->f);
    }
    if (BUFEOF(infile->f)) {
      printf("beginning of data not found\n");
      halt();
    }
    ch = getc(infile->f);
    if (ch == '\n')
      ch = ' ';
    testfortrigger(ch, &begindata);
  }
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
}

Static Void readrsdata(rsdata, rdata)
_TEXT *rsdata;
rstype *rdata;
{
  fscanf(rsdata->f, "%ld%ld%ld%ld%ld%lg%lg%lg%lg%ld%lg", &rdata->l,
	 &rdata->nal, &rdata->ncl, &rdata->ngl, &rdata->ntl, &rdata->rsl,
	 &rdata->rs, &rdata->varhnb, &rdata->sumvar, &rdata->nl,
	 &rdata->ehnb);

  while (P_peek(rsdata->f) == ' ')
    getc(rsdata->f);
  fscanf(rsdata->f, "%c%*[^\n]", &rdata->flag);
  getc(rsdata->f);

  if (rdata->flag == '\n')
    rdata->flag = ' ';
}

Static double sign(a)
double a;
{
  if (a >= 0)
    return 1.0;
  else
    return -1.0;
}

#define bignumber       pnum

Static Void themain(rsdata, dalvecp, symvec)
_TEXT *rsdata, *dalvecp, *symvec;
{
  boolean chilogo = false;
  double chia, chic, chig, chit, chitotal, nl4;
  rstype rdata;
  Char parameter;
  long position, FORLIM;
  double TEMP;

  printf("dalvec %4.2f\n", version);
  if (*rsdata->name != '\0') {
    if (rsdata->f != NULL)
      rsdata->f = freopen(rsdata->name, "r", rsdata->f);
    else
      rsdata->f = fopen(rsdata->name, "r");
  } else
    rewind(rsdata->f);
  if (rsdata->f == NULL)
    _EscIO2(FileNotFound, rsdata->name);
  RESETBUF(rsdata->f, Char);
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

  if (*dalvecp->name != '\0') {
    if (dalvecp->f != NULL)
      dalvecp->f = freopen(dalvecp->name, "r", dalvecp->f);
    else
      dalvecp->f = fopen(dalvecp->name, "r");
  } else
    rewind(dalvecp->f);
  if (dalvecp->f == NULL)
    _EscIO2(FileNotFound, dalvecp->name);
  RESETBUF(dalvecp->f, Char);
  if (!BUFEOF(dalvecp->f)) {
    if (!P_eoln(dalvecp->f)) {
      fscanf(dalvecp->f, "%c%*[^\n]", &parameter);
      getc(dalvecp->f);
      if (parameter == '\n')
	parameter = ' ';
    }
    if (parameter == 'c')
      chilogo = true;
  }

  if (chilogo)
    printf("Chilogo will be produced\n");
  else
    printf("Normal sequence logo will be produced\n");

  if (BUFEOF(rsdata->f)) {
    printf("empty rsdata file\n");
    halt();
    return;
  }
  header(rsdata, symvec);
  fprintf(symvec->f, "4 number of symbols in DNA or RNA\n");

  if (*rsdata->name != '\0') {
    if (rsdata->f != NULL)
      rsdata->f = freopen(rsdata->name, "r", rsdata->f);
    else
      rsdata->f = fopen(rsdata->name, "r");
  } else
    rewind(rsdata->f);
  if (rsdata->f == NULL)
    _EscIO2(FileNotFound, rsdata->name);
  RESETBUF(rsdata->f, Char);
  readrsrange(rsdata, &rdata);
  getrsbegin(rsdata);
  getrsbegin(rsdata);
  FORLIM = rdata.rstop;

  for (position = rdata.rstart; position <= FORLIM; position++) {
    if (P_peek(rsdata->f) != '*') {
      readrsdata(rsdata, &rdata);

      if (chilogo) {
	nl4 = rdata.nl / 4.0;
	TEMP = rdata.nal - nl4;
	chia = TEMP * TEMP;
	TEMP = rdata.ncl - nl4;
	chic = TEMP * TEMP;
	TEMP = rdata.ngl - nl4;
	chig = TEMP * TEMP;
	TEMP = rdata.ntl - nl4;
	chit = TEMP * TEMP;
	chitotal = chia + chic + chig + chit;
	if (chitotal > 0) {
	  rdata.nal = (long)floor(
		bignumber * (chia / chitotal) * sign(rdata.nal - nl4) + 0.5);
	  rdata.ncl = (long)floor(
		bignumber * (chic / chitotal) * sign(rdata.ncl - nl4) + 0.5);
	  rdata.ngl = (long)floor(
		bignumber * (chig / chitotal) * sign(rdata.ngl - nl4) + 0.5);
	  rdata.ntl = (long)floor(
		bignumber * (chit / chitotal) * sign(rdata.ntl - nl4) + 0.5);
	  rdata.nl = bignumber;
	} else {
	  rdata.nal = 1;
	  rdata.ncl = 1;
	  rdata.ngl = 1;
	  rdata.ntl = 1;
	  rdata.nl = 4;
	}
      }

      fprintf(symvec->f, "%*ld %*ld %*.*f % .*E\n",
	      nfield, rdata.l, infofield, rdata.nl, infofield, infodecim,
	      rdata.rsl, P_max((int)(infofield + 3) - 7, 1), rdata.varhnb);
      fprintf(symvec->f, "a %*ld\n", nfield, rdata.nal);
      fprintf(symvec->f, "c %*ld\n", nfield, rdata.ncl);
      fprintf(symvec->f, "g %*ld\n", nfield, rdata.ngl);
      fprintf(symvec->f, "t %*ld\n", nfield, rdata.ntl);
    }
  }
}
#undef bignumber

/* Print help for user */
void usage() {
  printf("\n");
  printf(" dalvec: converts Rseq rsdata file to symvec format\n");
  printf("\n  dalvec -r rsdata -p dalvecp\n\n");
  printf(" parameters: \n");
  printf("   -r data file from rseq program \n");
  printf("   -p parameters to control dalvec, If empty, then the normal sequence\n");
  printf("      logo will be produced.\n\n");
  printf(" Outputs:\n");
  printf("   symvec: reformating of the rsdata file for input to the makelogo program \n");
  printf("   output: messages to user\n");
  printf("\n");
  printf("  version %4.2f\n", version);
  exit(EXIT_SUCCESS);
}

int main(int argc, Char **argv)
{
  long FORLIM;
  extern char *optarg;
	extern int optind;
	int c, err = 0; 
  /* flags marking arguments passed */
  int rflag=0;       /* rsdata file name  */
  int pflag=0;       /* dalvecp file */
	char *rsdataFile  = "rsdata.txt";
  char *dalvecpFile = "dalvecp.txt";

/* Process command line arguments  */
while ((c = getopt(argc, argv, "r:p:")) != -1)
		switch (c) {
		case 'r':
      rflag = 1;
			rsdataFile = optarg;
      break;
		case 'p':
      pflag = 1;
      dalvecpFile  = optarg;
      break;
    case '?':
			err = 1;
			break;
		}

  /* Is the rsdata file name present */  
	if (rflag == 0) {	/* -r rsdata file is mandatory */ 
		fprintf(stderr, "%s: missing -r rsdata file \n", argv[0]);
		usage();
		exit(1);
	} 

  /* parameters file  */  
  if (pflag == 0) { 
    fprintf(stderr, "%s: missing -p dalvecp file \n", argv[0]);
		usage();
		exit(1);
    }  

  if (err) {
		usage();
		exit(1);
	}

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  symvec.f = NULL;
  strcpy(symvec.name, "symvec");
  dalvecp.f = NULL;
  strcpy(dalvecp.name, dalvecpFile);
  rsdata.f = NULL;
  strcpy(rsdata.name, rsdataFile);
  themain(&rsdata, &dalvecp, &symvec);
_L1:
  if (rsdata.f != NULL)
    fclose(rsdata.f);
  if (dalvecp.f != NULL)
    fclose(dalvecp.f);
  if (symvec.f != NULL)
    fclose(symvec.f);
  exit(EXIT_SUCCESS);
  return 0;
}/* End. */