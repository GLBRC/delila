/* mkdb: read sequence; make GenBank entry with features for capitalized regions

  Thomas D. Schneider, Ph.D.
  toms@alum.mit.edu
  toms@alum.mit.edu (permanent)
  https://alum.mit.edu/www/toms (permanent)
*/
/*
Description:
name
   mkdb: read sequence; make GenBank entry with features for capitalized regions

synopsis
   mkdb(sequ: in, mkdbp: in, entries: out, output: out)

files
   sequ:  raw DNA sequences in lower case except for objects
      of interest marked in upper case.  The program also accepts 'n'.
      Sequences are separated by periods.
      Each sequence may be preceeded by a name line and a species line.
      These lines can begin with '>' or '*', but this is not necessary.
      Spaces, blank lines and numbers are ignored.
      Other lines that begin with '>' or '*' are comments
      If there is no species name, the single name will be used
      for the species also.  This kludge allows the program to read
      the fasta format.  If there is no name at all (just sequence)
      then a name will be assigned: nameste.

      name: a string of characters to name the sequence.
      organism: the species this sequence represents

   entries:  GenBank entries for the sequences, with features for
      the capitalized regions marked as exons and features for the lower case
      regions (not including primers) as introns.

   mkdbp:  parameters to control the program.  The file must contain the
      following parameters, one per line:

      parameterversion: The version number of the program.  This allows the
         user to be warned if an old parameter file is used.

      exoncutoff (integer): Capitalized regions longer than this
         number of bases will be called exons, the others will be
         called primers.

      multipart (character): What to do if a name has spaces in it.
         'i' ignore the rest of the name
         'u' replace spaces with underscores

   output: messages to the user

description

   Sequences are often marked by people with capital letters to
   indicate interesting regions (exons, primers, mutations, etc).
   This program uses raw sequences to create simple flat-file GenBank
   style entries with features marked by capital letters.  Long
   features are called 'exons' while short ones are called 'primers'.
   The division between these two is given by the exoncutoff
   parameter.

Example

If the sequ contains:

* T7stuff
* Bacteriophage T7
aacataaaggacacaatgcaatgaacattaccgacatcatgaacgctatc
gacgcaatcaaagcactgccaatctgtgaacttgacaagcgtcaaggtat
gcttatcgacttactggtcgagatggtcaacagcgagacgtgtgatggcg
agctaacCGAACTAAATCAGGCACttgagcatcaagattggtggactacc
ttgaagtgtctcacggctgacgcagggttcaagATGCTCGGTAATGGTCA
CTTCTCGGCTGCTTATAGTCACCCGCTGCTACCTAACAGAGTGATTAAGG
TGGGCTTTAAGAAAGAGGATTCAGGCGCAGCCTATACCGCATTCTgccgc
atgtatcagggtcgtcctggtatccctaacgtctacgatgtacagcgcca
cgctggatgctatacggtggtacttgacgcacttaaggattgcgagcgtt
tcaacaatgatgccCATTATAAATACGCTGAgattgcaagcgacatcatt
gattgcaattcggatgagcatgatgagttaactggatgggatggtgagtt
tgttgaaacttgtaaactaatccgcaagttctttgagggcatcgcctcat
.

The entries file will contain:

LOCUS T7stuff 600 bp  DNA * mkdb 1.21
DEFINITION  Bacteriophage T7
ACCESSION   T7stuff
VERSION     T7stuff.1
SOURCE   Bacteriophage T7
  ORGANISM   Bacteriophage T7
FEATURES
     primer 158..174
     exon   234..345
     primer 465..481
BASE COUNT     166 a     133 c     151 g     150 t       0 n
ORIGIN
         1 aacataaagg acacaatgca atgaacatta ccgacatcat gaacgctatc gacgcaatca
        61 aagcactgcc aatctgtgaa cttgacaagc gtcaaggtat gcttatcgac ttactggtcg
       121 agatggtcaa cagcgagacg tgtgatggcg agctaaccga actaaatcag gcacttgagc
       181 atcaagattg gtggactacc ttgaagtgtc tcacggctga cgcagggttc aagatgctcg
       241 gtaatggtca cttctcggct gcttatagtc acccgctgct acctaacaga gtgattaagg
       301 tgggctttaa gaaagaggat tcaggcgcag cctataccgc attctgccgc atgtatcagg
       361 gtcgtcctgg tatccctaac gtctacgatg tacagcgcca cgctggatgc tatacggtgg
       421 tacttgacgc acttaaggat tgcgagcgtt tcaacaatga tgcccattat aaatacgctg
       481 agattgcaag cgacatcatt gattgcaatt cggatgagca tgatgagtta actggatggg
       541 atggtgagtt tgttgaaact tgtaaactaa tccgcaagtt ctttgagggc atcgcctcat
//

documentation

see also

   {example parameter file:} mkdbp
   {example sequ file:} mkdb.sequ {move to the name 'sequ' to use it}

   {Program for listing the sequences:} lister.p
   {Program for generating search for capitalized sequence:} capsmark.p

author
   Thomas Dana Schneider

bugs

technical notes

   Capitalization that abuts either end of the sequence will be
   indicated in the entry as beyond the end.  This way the ends of the
   sequence will not be marked as donors or acceptors.

   The maximum name and sequence lengths are constants maxobjectlength
   and maxsequencelength respectively.

gcc -Wall mkdb.c -o mkdb -I/home/mplace/bin/p2c/home -L /home/mplace/bin/p2c/home -lm -lp2c

*/

/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "mkdb.p" */

#include <stdio.h> /* printf */
#include <getopt.h>  /* getopt API */ 
#include <stdlib.h> 
#include </home/mplace/bin/p2c/src/p2c.h>

#define version         1.27
#define updateversion   1.09

#define maxnamelength   100
#define maxsequencelength  5000000 
#define debugging       false
int debug = 0;

/*
files used by this program:
sequ
mkdbp
entries
_TEXT is defined in p2c.h  it is a char literal
*/
Static _TEXT sequ;
Static _TEXT mkdbp;
Static _TEXT entries;
Static jmp_buf _JL1;

Static Void halt()
{
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}

/* Function to convert the character c to upper case */
Static Char capitalize(Char c)
{
  long n = c;    /* c is the n'th letter of the alphabet */

  if (n >= 'a' && n <= 'z')
    c = _toupper(n);

  return c;
}

Static Char decapitalize(Char c)
{
  long n = c;    /* c is the n'th letter of the alphabet */

  if (n >= 'A' && n <= 'Z')
    c = _tolower(n);
  else
    c = (Char)n;

  return c;
}

typedef struct object {
  Char id[maxnamelength];
  long length;
} object;

typedef struct sequence {
  Char id[maxsequencelength];
  long length;
} sequence;


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *sequ; 
  _TEXT *entries;
  Char c;                        /* a character in the sequence */
  boolean die;                   /* stop the program if bad sequence characters found */
  long entrylength;              /* length of the sequence */
  long exoncutoff;               /* exon cutoff size */
  Char multipart;                /* shortest feature to report as an exon */
  object name;                   /* what to do for multipart names */
  long na, nc, ng, nt, nn;       /* numbers of bases and unknown base */
  object organism;               /* organism for the sequence */
  float parameterversion;        /* parameter version number */
  sequence seq;                  /* the sequence */
  long start      ;              /* position in the sequence, start of feature */
  long stop;                     /* position in the sequence, end of feature */
  Char topology;                 /* topology of the sequence */
  boolean waslower;              /* true if previous character was lower case */
} ;

/* write the name to the file.
   Force any spaces to be underscores. */
Local Void writename(_TEXT *afile, object name, struct LOC_themain *LINK)
{
  long n;                /* index to the name */
  boolean skip = false;  /* skip the rest of the line */

  for (n = 0; n < name.length; n++) {
    if (!skip) {
      if (name.id[n] == ' ') {
	if (LINK->multipart == 'i')
	  skip = true;
	else
	  putc('_', afile->f);
      } else
	putc(name.id[n], afile->f);
    }
  }
}

/* write the name to the file */
Local Void readname(_TEXT *afile, object *name, struct LOC_themain *LINK)
{
  _TEXT TEMP;

  if ((P_peek(afile->f) == '>') | (P_peek(afile->f) == '*')) {
    getc(afile->f);                                           /* read the name in */
    while ((P_peek(afile->f) == ' ') & (!P_eoln(afile->f)))
      getc(afile->f);

    if (P_eoln(afile->f)) {
      printf("missing name\n");
      halt();
    }

    name->length = 0;
    while (!P_eoln(afile->f)) {
      name->length++;
      if (name->length > maxnamelength) {
	printf("name too long:\n");
	putchar('"');
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writename(&TEMP, *name, LINK);
	printf("\"\n");
	halt();
      }
      name->id[name->length - 1] = getc(afile->f);
      if (name->id[name->length - 1] == '\n')
	name->id[name->length - 1] = ' ';
    }
    fscanf(afile->f, "%*[^\n]");

    getc(afile->f);
    printf("READING: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writename(&TEMP, *name, LINK);
    putchar('\n');
    return;
  }
  printf("Assigned name: ");
  name->id[0] = 'n';
  name->id[1] = 'a';
  name->id[2] = 'm';
  name->id[3] = 'e';
  name->id[4] = 's';
  name->id[5] = 't';
  name->id[6] = 'e';
  name->length = 7;
  putchar('"');
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writename(&TEMP, *name, LINK);
  printf("\"\n");

}
/* put some indentation to the file */
Local Void indent(_TEXT *afile, long amount,  struct LOC_themain *LINK)
{
  long n;           /* index to the name */

  for (n = 1; n <= amount; n++)
    putc(' ', afile->f);
}

/* write the feature coordinates start and stop to a file.
If they are at or beyond the entry size, put < or > */
Local Void featurecoordinate(_TEXT *afile, long start, long stop, long entrylength, struct LOC_themain *LINK)
{
  /*
*/
  if (start < 1)
    putc('<', afile->f);
  fprintf(afile->f, "%ld", start);
  fprintf(afile->f, "..");
  if (stop > entrylength)
    putc('>', afile->f);
  fprintf(afile->f, "%ld", stop);
}

Local Void countem(Char b, struct LOC_themain *LINK)
{
  switch (decapitalize(b))
  {
      case 'a':
        LINK->na++;
        break;
      case 'c':
        LINK->nc++;
        break;
      case 'g':
        LINK->ng++;
        break;
      case 't':
        LINK->nt++;
        break;
      case 'n':
        LINK->nn++;
        break;
  }
}

/* choose what kind of feature to write out */
Local Void chooseandwrite(struct LOC_themain *LINK)
{
  if (LINK->stop - LINK->start + 1 > LINK->exoncutoff) {
    indent(LINK->entries, 5L, LINK);
    fprintf(LINK->entries->f, "exon   ");
    featurecoordinate(LINK->entries, LINK->start, LINK->stop,
		      LINK->entrylength, LINK);
    putc('\n', LINK->entries->f);
    return;
  }
  indent(LINK->entries, 5L, LINK);
  fprintf(LINK->entries->f, "primer ");
  featurecoordinate(LINK->entries, LINK->start, LINK->stop, LINK->entrylength,
		    LINK);
  putc('\n', LINK->entries->f);
}
/* process one sequence ending in a period or end of file */
Local Void processsequence(struct LOC_themain *LINK)
{
  long position = 0;           /* position in the sequence, starting with 1, loop variable - must be local */
  long linenumber = 0;         /* line number in file */
  Char TEMP;
  long FORLIM;

/*  read the next sequence */
  LINK->die = false;
  LINK->c = ' ';
  LINK->na = 0;
  LINK->nc = 0;
  LINK->ng = 0;
  LINK->nt = 0;
  LINK->nn = 0;
  /*

*/
  while (!BUFEOF(LINK->sequ->f) && LINK->c != '.') {
    if (P_eoln(LINK->sequ->f))     /* end of line */
    {
      fscanf(LINK->sequ->f, "%*[^\n]");
      getc(LINK->sequ->f);
      linenumber++;

      if (!BUFEOF(LINK->sequ->f)) {
	if ((P_peek(LINK->sequ->f) == '*') | (P_peek(LINK->sequ->f) == '>'))
	  LINK->c = '.';
      }
      if (debugging)
	putchar('\n');

      continue;
    }
    LINK->c = getc(LINK->sequ->f);
    if (LINK->c == '\n')
      LINK->c = ' ';
    if (debugging)
      putchar(LINK->c);
    TEMP = decapitalize(LINK->c);
    if (TEMP == 'n' || TEMP == 't' || TEMP == 'g' || TEMP == 'c' ||
	TEMP == 'a') {
      position++;
      if (position > maxsequencelength) {
	printf("Sequence exceeds the maximum sequence length: %d\n",
	       maxsequencelength);
	printf("Increase constant maxsequencelength\n");
	halt();
      }
      LINK->seq.id[position-1] = LINK->c;
      countem(LINK->c, LINK);
      continue;
    }
    if (LINK->c == '.') {
      fscanf(LINK->sequ->f, "%*[^\n]");
      getc(LINK->sequ->f);
      linenumber++;
    } else if (LINK->c != ' ' && LINK->c != '9' && LINK->c != '8' &&
	       LINK->c != '7' && LINK->c != '6' && LINK->c != '5' &&
	       LINK->c != '4' && LINK->c != '3' && LINK->c != '2' &&
	       LINK->c != '1' && LINK->c != '0') {
      printf("unidentified character: \"%c\"\n", LINK->c);
      printf("at line number: %ld\n", linenumber);
      LINK->die = true;
    }
  }
  LINK->entrylength = position;
  if (LINK->die)
    halt();

 /* start header of GenBank entry */
  fprintf(LINK->entries->f, "\nLOCUS ");
  writename(LINK->entries, LINK->name, LINK);
  fprintf(LINK->entries->f, " %ld bp  DNA", LINK->entrylength);


  if (LINK->topology == 'l')
    fprintf(LINK->entries->f, " linear");
  else
    fprintf(LINK->entries->f, " circular");

  fprintf(LINK->entries->f, " * mkdb %4.2f\n", version);
  fprintf(LINK->entries->f, "DEFINITION  ");
  writename(LINK->entries, LINK->organism, LINK);
  fprintf(LINK->entries->f, "\nACCESSION   ");
  writename(LINK->entries, LINK->name, LINK);
  fprintf(LINK->entries->f, "\nVERSION     ");
  writename(LINK->entries, LINK->name, LINK);
  fprintf(LINK->entries->f, ".1\n");
  fprintf(LINK->entries->f, "SOURCE   ");
  writename(LINK->entries, LINK->organism, LINK);
  fprintf(LINK->entries->f, "\n  ORGANISM   ");
  writename(LINK->entries, LINK->organism, LINK);
  fprintf(LINK->entries->f, "\nFEATURES\n");

  LINK->waslower = true;
  LINK->start = 1;
  LINK->stop = 1;
  FORLIM = LINK->entrylength;
  for (position = 1; position <= FORLIM; position++) {
    LINK->c = LINK->seq.id[position-1];
    if (LINK->c == 'n' || LINK->c == 't' || LINK->c == 'g' ||
	LINK->c == 'c' || LINK->c == 'a') {
      if (position != 1 && !LINK->waslower) {
	LINK->stop = position - 1;
	chooseandwrite(LINK);
      }
      LINK->waslower = true;
    } else if (LINK->c == 'N' || LINK->c == 'T' || LINK->c == 'G' ||
	       LINK->c == 'C' || LINK->c == 'A') {
      if (LINK->waslower)
	LINK->start = position;
      LINK->waslower = false;
    } else {
      printf("program error:\n");
      printf("illegal character \"%c\" found in sequence array\n", LINK->c);
      halt();
    }
  }

  if (!LINK->waslower) {    /* i.e. was upper */
    /*  After the end of sequence is found the
        sequence can end on capital letters, catch that case.  */
    LINK->stop = LINK->entrylength;
    chooseandwrite(LINK);
  }

  fprintf(LINK->entries->f, "BASE COUNT %7ld a %7ld c %7ld g %7ld t %7ld n\n",
	  LINK->na, LINK->nc, LINK->ng, LINK->nt, LINK->nn);
  if (LINK->na + LINK->nc + LINK->ng + LINK->nt + LINK->nn != LINK->entrylength) {
    printf("program error: BASE COUNT <> sum of bases\n");
    halt();
  }
  fprintf(LINK->entries->f, "ORIGIN");

  FORLIM = LINK->entrylength;
  /* write out sequence */
  for (position = 1; position <= FORLIM; position++) {
    if (position % 10 == 1)
      putc(' ', LINK->entries->f);
    if ((position - 1) % 60 == 0)
      fprintf(LINK->entries->f, "\n%10ld ", position);
/* p2c: mkdb.p, line 530:
 * Note: Using % for possibly-negative arguments [317] */
    fputc(decapitalize(LINK->seq.id[position-1]), LINK->entries->f);
  }

  fprintf(LINK->entries->f, "\n//\n");
}

/*
File input done here
*/
Static Void themain(_TEXT *sequ_, _TEXT *mkdbp, _TEXT *entries_)
{
  struct LOC_themain V;
  double parameterversion;

  V.sequ = sequ_;
  V.entries = entries_;
  printf("themain mkdb %4.2f\n", version);     /* print version to screen */
  /* HANDLE THE PARAMETER FILE */
  if (*mkdbp->name != '\0') {
    if (mkdbp->f != NULL)
      mkdbp->f = freopen(mkdbp->name, "r", mkdbp->f);
    else
      mkdbp->f = fopen(mkdbp->name, "r");
  } else
    rewind(mkdbp->f);
  if (mkdbp->f == NULL)
    _EscIO2(FileNotFound, mkdbp->name);
  RESETBUF(mkdbp->f, Char);
  fscanf(mkdbp->f, "%lg%*[^\n]", &parameterversion);
  getc(mkdbp->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(mkdbp->f, "%ld%*[^\n]", &V.exoncutoff);
  getc(mkdbp->f);
  if (!BUFEOF(mkdbp->f)) {
    fscanf(mkdbp->f, "%c%*[^\n]", &V.multipart);
    getc(mkdbp->f);
    if (V.multipart == '\n')
      V.multipart = ' ';
  } else
    V.multipart = 'i';

  V.topology = 'l';

  if (*V.entries->name != '\0') {
    if (V.entries->f != NULL)
      V.entries->f = freopen(V.entries->name, "w", V.entries->f);
    else
      V.entries->f = fopen(V.entries->name, "w");
  } else {
    if (V.entries->f != NULL)
      rewind(V.entries->f);
    else
      V.entries->f = tmpfile();
  }
  if (V.entries->f == NULL)
    _EscIO2(FileNotFound, V.entries->name);
  SETUPBUF(V.entries->f, Char);
  if (*V.sequ->name != '\0') {
    if (V.sequ->f != NULL)
      V.sequ->f = freopen(V.sequ->name, "r", V.sequ->f);
    else
      V.sequ->f = fopen(V.sequ->name, "r");
  } else
    rewind(V.sequ->f);
  if (V.sequ->f == NULL)
    _EscIO2(FileNotFound, V.sequ->name);
  RESETBUF(V.sequ->f, Char);
  while (!BUFEOF(V.sequ->f)) {
    /*


*/
    readname(V.sequ, &V.name, &V);

    if ((P_peek(V.sequ->f) == '>') | (P_peek(V.sequ->f) == '*'))
      readname(V.sequ, &V.organism, &V);
    else
      V.organism = V.name;

    processsequence(&V);
  }
}

int main(int argc, char **argv)
{
  extern char *optarg;
	extern int optind;
	int c, err = 0; 
	int sflag=0;
  int pflag=0;
  int oflag=0;
	char *sName = "sequence";
  char *params = "parameters";
  char *outName = "output.gnbk";
	static char usage[] = "usage: %s -s sequ -p mkdbp -o out\n";

while ((c = getopt(argc, argv, "s:p:o:")) != -1)
		switch (c) {
		case 'o':
      oflag = 1;
			outName = optarg;
			break;
		case 's':
      sflag = 1;
			sName = optarg;
			break;
		case 'p':
      pflag = 1;
			params = optarg;
			break;
		case '?':
			err = 1;
			break;
		}
	if (oflag == 0) {	/* -o was mandatory */ 
		fprintf(stderr, "%s: missing -o option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} 
    
    if (sflag == 0) { /* -s was mandatory */        
		fprintf(stderr, "%s: missing -s option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
    } 
    
    if (pflag == 0) { 
        fprintf(stderr, "%s: missing -p option really\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
    } 

    if (err) {
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}

  PASCAL_MAIN(argc, argv); 
  if (setjmp(_JL1))
    goto _L1;
  entries.f = NULL;
  strcpy(entries.name, outName);
  mkdbp.f = NULL;
  strcpy(mkdbp.name, params);
  sequ.f = NULL;
  strcpy(sequ.name, sName);
  themain(&sequ, &mkdbp, &entries);
_L1:
  if (sequ.f != NULL)
    fclose(sequ.f);
  if (mkdbp.f != NULL)
    fclose(mkdbp.f);
  if (entries.f != NULL)
    fclose(entries.f);
  exit(EXIT_SUCCESS);

  return 0;
}
/* End. */
