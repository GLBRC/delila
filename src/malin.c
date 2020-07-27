/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "malin.p" */
/* malin: make delila instructions from nth alignment of malign

  Dr. Thomas D. Schneider
  toms@alum.mit.edu
  permanent email: toms@alum.mit.edu (use only if first address fails)
  https://alum.mit.edu/www/toms/

name
   malin: make delila instructions from nth alignment of malign

synopsis
   malin(optinst: in, optalign: in, inst: in, malinp: in,
         cinst: out, distribution: out, output: out)

files
   optinst: output of malign program containing absolute alignments

   optalign: output of malign program containing relative alignments

   inst: Delila instructions
      Allowed forms:

       get from 5 -5 to 5 +5;
       get from 5 -5 to same +5;
       get from 5 -5 to piece end -5;

   malinp: parameters to control the program
      first line: The version number of the program.  This allows the user to
         be warned if an old parameter file is used.

      second line:  one integer that defines which alignment to use
         to create the cinst.

      third line:  one integer that defines how much to add to
         move the location of the zero base in the new instructions.

   cinst: Delila instructions of inst converted to the alignment
      of optinst chosen in malinp

   distribution: The distribution of the realignment.  Lines that begin with
      "*" are comments.  Otherwise, one integer per line, which is the
      separation in bases between the initial and final alignments.

   output: output program without private text

description
   This program allows one to select one of the alignments created by malign
   and to make the corresponding Delila instructions.  Because it copies the
   inst file it keeps the organism and chromosome information (along with all
   comments) so it is better than the "bestinst" file created by malign!

examples

documentation

see also

   {example parameter file:} malinp
   {related programs:} malign.p, malopt.p

author
   Thomas Dana Schneider

bugs
   WARNING:  This program does not use a book and so the coordinate
   shift of the third parameter will not work on coordinates that jump.

technical notes

   NOTE: THIS PROGRAM WILL NOT HANDLE COMMENTS WITHIN THE DELILA
   INSTRUCTION!  It must be of the form:
      get from 193 -20 to 193 +21;
   comments are allowed outside these statements.

To Compile:
  gcc malin.c -o malin -I/home/mplace/bin/p2c/home -L/home/mplace/bin/p2c/home/ -lm -lp2

To Run:


*/
#include <stdio.h>    /* printf */
#include <getopt.h>   /* getopt API */ 
#include <stdlib.h> 
#include </home/mplace/bin/p2c/src/p2c.h>

#define version         1.14
#define updateversion   1.07
#define maxstring       1500
#define fillermax       10

typedef struct stringDelila {
  Char letters[maxstring]; /* the letters in the string */
  long length;             /* the number of characters in the string */
  long current;            /* the letter we are working on */
} stringDelila;

/* the following is an array used to fill a string.
   it is convenient to have it much shorter than the maxstring, so that
   it is easy to fill the string using procedure fillstring.
   the user must declare the value of constant fillermax. */
typedef Char filler[fillermax];

typedef struct trigger {
  stringDelila seek;    /* the characters looked for */
  long state;           /* how close to triggering we are */
  boolean skip;         /* trigger not found- skip the line */ 
  boolean found;        /* the trigger was found */
} trigger;

Static _TEXT optinst, optalign, inst, malinp, cinst, distribution;
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

/* get a string from a file not using string calls.  this lets one
obtain lines from a file without interactive prompts */
Static Void getstring(afile, buffer, gotten)
_TEXT *afile;
stringDelila *buffer;
boolean *gotten;
{
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

/* write the string s to file tofile, no writeln */
Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;
  FORLIM = s->length;

  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}

/* this procedure makes it reasonably easy to fill the string s with
characters.  one calls the procedure as: *)
(*                           1         2         3         4         5 *)
(*                  12345678901234567890123456789012345678901234567890 *)
(*   fillstring(s, 'this-is-the-string                                ');
the two comments make it easy to line the characters up. also, for this
example, it was assumed that the length of filler as defined by the
constant fillermax was 50. */
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

/* this module allows one to scan a series of characters, as from
an array or a file, and to "trigger" or detect a simple string
in the series.  the advantage of the trigger is that several triggers
can "observe" a stream of characters at once, each looking for a
different thing.
some other modules required: interact.const, interact.type */
Static Void resettrigger(t)
trigger *t;
{
  t->state = 0;
  t->skip = false;
  t->found = false;
}

/* look at the character ch.
   if it is part of the trigger (at the current trigger state),
       then the trigger state goes higher.
   if it is not part of the trigger then the trigger state is reset,
      skip is true and one should skip onward to find the trigger.
   if the trigger is found, found is true. */
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

Local Char sign(i)
long i;
{
  if (i >= 0)
    return '+';
  else
    return '-';
}

Static Void themain(optinst, optalign, inst, malinp, cinst, distribution)
_TEXT *optinst, *optalign, *inst, *malinp, *cinst, *distribution;
{
  /*
*/
  long a = 1;               /* index for reading through alignments */
  long alignments;          /* number of alignments in optinst */
  long analignment;         /* an alignment to use from optinst */
  Char c;                   /* a character from the inst */
  long class_;              /* index to the classes of alignments */
  boolean debugging = false;  /* set to true if debugging */
  long fromvalue, fromrange; /* the from coordinate and the range from the from */
  double H;                  /* uncertainty, bits */
  long occurences;           /* number of times an alignment appeared */
  double parameterversion;   /* parameter version number */
  long s;                    /* index for reading through alignment sequence numbers */
  long sequences;            /* number of sequences */
  long shift;                /* how much a sequence is shifted */
  long state = 0;            //
  boolean shutup = false;    /* state of the program.
      state = 0; scan and copy, outside comments
                when     '(@' is found, move to state 1             t0a
                when    'get' is found, move to state 2             t0b
                when      '{' is found, move to state 5             t0c
                when      '"' is found, move to state 6             t0d
                when      "'" is found, move to state 7             t0e
      state = 1; scan and copy, copy program comments
                when     '@)' is found, move to state 0             t1a
      state = 2; scan and copy, find from
                when   'from' is found, move to state 3             t2a
      state = 3; read from value, relative from value, scan for 'to'
                when     'to' is found, move to state 4             t3a
      state = 4; read to values, relative to value
                move to state 0
      state = 5; scan and copy {} comment
                when '}' is found, move to state 0                  t5a
      state = 6; scan and copy " string
                when '"' is found, move to state 0
      state = 7; scan and copy ' string
                when "'" is found, move to state 0  */

  trigger t0a, t0b, t1a, t2a, t3a, t0c, t0d, t0e, t5a;  /* triggers for each state */
  long tovalue, torange;  /* the to coordinate and the range from the to */
  long theclass;          /* the current class according to optinst */
  long zerobase;          /* the new zero coordinate */  

  printf("malin%5.2f\n", version);

  filltrigger(&t0a, "(*        ");
  filltrigger(&t0b, "get       ");
  filltrigger(&t0c, "{         ");
  filltrigger(&t0d, "\"         ");
  filltrigger(&t0e, "'         ");
  filltrigger(&t1a, "*)        ");
  filltrigger(&t2a, "from      ");
  filltrigger(&t3a, "to        ");
  filltrigger(&t5a, "}         ");


  if (*malinp->name != '\0') {
    if (malinp->f != NULL)
      malinp->f = freopen(malinp->name, "r", malinp->f);
    else
      malinp->f = fopen(malinp->name, "r");
  } else
    rewind(malinp->f);
  if (malinp->f == NULL)
    _EscIO2(FileNotFound, malinp->name);
  RESETBUF(malinp->f, Char);
  fscanf(malinp->f, "%lg%*[^\n]", &parameterversion);
  getc(malinp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }
  fscanf(malinp->f, "%ld%*[^\n]", &analignment);
  getc(malinp->f);
  fscanf(malinp->f, "%ld%*[^\n]", &zerobase);
  getc(malinp->f);

  if (*optinst->name != '\0') {
    if (optinst->f != NULL)
      optinst->f = freopen(optinst->name, "r", optinst->f);
    else
      optinst->f = fopen(optinst->name, "r");
  } else
    rewind(optinst->f);
  if (optinst->f == NULL)
    _EscIO2(FileNotFound, optinst->name);
  RESETBUF(optinst->f, Char);
  fscanf(optinst->f, "%ld%ld%*[^\n]", &sequences, &alignments);
  getc(optinst->f);

  while (a < analignment) {
    if (BUFEOF(optinst->f)) {
      printf("alignment %ld does not exist\n", analignment);
      halt();
    }
    a++;
    fscanf(optinst->f, "%ld%lg%*[^\n]", &occurences, &H);
    getc(optinst->f);
    for (s = 1; s <= sequences; s++)
      fscanf(optinst->f, "%ld", &fromvalue);
    fscanf(optinst->f, "%*[^\n]");
    getc(optinst->f);
  }
  fscanf(optinst->f, "%ld%lg%*[^\n]", &occurences, &H);
  getc(optinst->f);

  if (*inst->name != '\0') {
    if (inst->f != NULL)
      inst->f = freopen(inst->name, "r", inst->f);
    else
      inst->f = fopen(inst->name, "r");
  } else
    rewind(inst->f);
  if (inst->f == NULL)
    _EscIO2(FileNotFound, inst->name);
  RESETBUF(inst->f, Char);
  if (*cinst->name != '\0') {
    if (cinst->f != NULL)
      cinst->f = freopen(cinst->name, "w", cinst->f);
    else
      cinst->f = fopen(cinst->name, "w");
  } else {
    if (cinst->f != NULL)
      rewind(cinst->f);
    else
      cinst->f = tmpfile();
  }
  if (cinst->f == NULL)
    _EscIO2(FileNotFound, cinst->name);
  SETUPBUF(cinst->f, Char);

  if (*distribution->name != '\0') {
    if (distribution->f != NULL)
      distribution->f = freopen(distribution->name, "w", distribution->f);
    else
      distribution->f = fopen(distribution->name, "w");
  } else {
    if (distribution->f != NULL)
      rewind(distribution->f);
    else
      distribution->f = tmpfile();
  }
  if (distribution->f == NULL)
    _EscIO2(FileNotFound, distribution->name);
  SETUPBUF(distribution->f, Char);
  fprintf(distribution->f, "* malin %5.2f alignment distribution\n", version);
  fprintf(distribution->f, "* alignment class: %ld\n", analignment);

  if (*optalign->name != '\0') {
    if (optalign->f != NULL)
      optalign->f = freopen(optalign->name, "r", optalign->f);
    else
      optalign->f = fopen(optalign->name, "r");
  } else
    rewind(optalign->f);
  if (optalign->f == NULL)
    _EscIO2(FileNotFound, optalign->name);
  RESETBUF(optalign->f, Char);

  while (P_peek(optalign->f) == '*') {
    fscanf(optalign->f, "%*[^\n]");
    getc(optalign->f);
  }
  fscanf(optalign->f, "%*[^\n]");
  getc(optalign->f);
  fscanf(optalign->f, "%*[^\n]");
  getc(optalign->f);
  fscanf(optalign->f, "%*[^\n]");

  getc(optalign->f);
  for (class_ = 1; class_ <= analignment; class_++) {
    if (BUFEOF(optalign->f)) {
      printf("premature end of optalign\n");
      halt();
    }
    fscanf(optalign->f, "%*[^\n]");
    getc(optalign->f);
    fscanf(optalign->f, "%ld%*[^\n]", &theclass);
    getc(optalign->f);
    if (theclass != class_) {
      printf("classes not being read correctly\n");
      halt();
    }
    for (s = 1; s <= sequences; s++) {
      fscanf(optalign->f, "%ld", &shift);
      if (class_ == analignment) {
	if (zerobase >= 0)
	  fprintf(distribution->f, "%ld\n", shift + zerobase);
	else {
	  fprintf(distribution->f, "%ld\n", -(shift + zerobase));

	}
      }
    }
  }

  while (!BUFEOF(inst->f)) {
    if (!P_eoln(inst->f)) {
      resettrigger(&t0a);
      resettrigger(&t0b);
      resettrigger(&t0c);
      resettrigger(&t0d);
      resettrigger(&t0e);
      resettrigger(&t1a);
      resettrigger(&t2a);
      resettrigger(&t3a);
      resettrigger(&t5a);

      while (!P_eoln(inst->f)) {
	c = getc(inst->f);
	if (c == '\n')
	  c = ' ';
	if (!shutup)
	  putc(c, cinst->f);

	if (debugging)
	  printf("%c[%ld]", c, state);

	testfortrigger(c, &t0a);
	testfortrigger(c, &t0b);
	testfortrigger(c, &t0c);
	testfortrigger(c, &t0d);
	testfortrigger(c, &t0e);
	testfortrigger(c, &t1a);
	testfortrigger(c, &t2a);
	testfortrigger(c, &t3a);
	testfortrigger(c, &t5a);

	switch (state) {

	case 0:
	  if (t0a.found)
	    state = 1;

	  else if (t0b.found)
	    state = 2;

	  else if (t0c.found)
	    state = 5;

	  else if (t0d.found)
	    state = 6;

	  else if (t0e.found) {

	    state = 7;
	  }
	  break;
	case 1:
	  if (t1a.found)
	    state = 0;
	  break;
	case 2:
	  if (t2a.found) {
	    fscanf(inst->f, "%ld%ld", &fromvalue, &fromrange);
	    fscanf(optinst->f, "%ld", &fromvalue);
	    fromvalue += zerobase;

	    fprintf(cinst->f, " %ld %c%ld",
		    fromvalue, sign(fromrange), labs(fromrange));

	    state = 3;
	  }
	  break;
	case 3:
	  if (t3a.found)
	    state = 4;
	  break;
	case 4:
	  skipblanks(inst);
	  if (P_peek(inst->f) == 's') {
	    tovalue = fromvalue;
	    skipnonblanks(inst);
	    fscanf(inst->f, "%ld", &torange);

	    fprintf(cinst->f, "same %c%ld", sign(torange), labs(torange));
	  }

	  else if (P_peek(inst->f) == 'p') {

	    tovalue = fromvalue;
	    skipnonblanks(inst);
	    torange = 100;
	    shutup = true;

	    fprintf(cinst->f, "same %c%ld", sign(torange), labs(torange));
	  }

	  else {
	    fscanf(inst->f, "%ld%ld", &tovalue, &torange);
	    fprintf(cinst->f, "%ld %c%ld",
		    fromvalue, sign(torange), labs(torange));
	  }
	  state = 0;
	  break;
	case 5:
	  if (t5a.found)
	    state = 0;
	  break;
	case 6:
	  if (t0d.found)
	    state = 0;
	  break;
	case 7:
	  if (t0e.found)
	    state = 0;
	  break;
	}
      }
      continue;
    }

    fscanf(inst->f, "%*[^\n]");
    getc(inst->f);
    if (debugging)
      putchar('\n');
    if (shutup) {
      fprintf(cinst->f, ";\n");
      shutup = false;
    } else
      putc('\n', cinst->f);
  }

  fprintf(cinst->f, "\n(* malin%5.2f *)\n", version);
  fprintf(cinst->f, "(* alignment: %ld, occurences: %ld, H: %10.5f bits *)\n",
	  analignment, occurences, H);
  printf("alignment: %ld, occurences: %ld, H: %10.5f bits\n",
	 analignment, occurences, H);
}


/* Print help for user */
void usage() {
  printf("\n");
  printf(" malin: make delila instructions from nth alignment of malign\n\n");
  printf(" Parameters:\n");
  printf("\n  malin -a optalign -i instructions -o optinst -p malinp \n");
  printf("  -a optalign file, output of malign program containing relative alignments \n");
  printf("  -i delila instruction file, same file used to run delila \n");
  
  printf("\tinst: Delila instructions\n\n");
  printf("\t\tAllowed forms:\n");
  printf("\t\tget from 5 -5 to 5 +5\n");
  printf("\t\tget from 5 -5 to same +5\n");
  printf("\t\tget from 5 -5 to piece end -5\n\n");

  printf("  -o optinst file, output of malign program containing absolute alignments \n");
  
  printf("  -p malinp parameter file to control the program\n");
  printf("     first line: The version number of the program.  This allows the\n");
  printf("                 the user to be warned if an old parameter file is used.\n");
  printf("     second line: one integer that defines which alignment to use\n");
  printf("                  to create the cinst(output). \n");
  printf("     third line:  one integer that defines how much to add to move the\n");
  printf("                  location of the zero base in the new instructions.\n");
    
  printf("  outputs:\n\n");
  printf("  cinst: Delila instructions converted to use the alignment of optinst\n");
  printf("         chosen in malinp \n\n");
  printf("  distribution: The distribution of the realignment.  Lines that begin with\n");
  printf("                \"*\" are comments.  Otherwise, one integer per line, which is the\n");
  printf("                separation in bases between the initial and final alignments.\n");
  printf(" \n");
  printf("  description: \n");
  printf("\tThis program allows one to select one of the alignments created by malign\n");
  printf("\tand to make the corresponding Delila instructions.  Because it copies the\n");
  printf("\tinst file it keeps the organism and chromosome information (along with all\n");
  printf("\tcomments) so it is better than the \"bestinst\" file created by malign!\n");
  printf(" \n");
  printf("  version %4.2f\n", version);
  exit(EXIT_SUCCESS);
}

int main(int argc, Char **argv)
{
 extern char *optarg;
	extern int optind;
	int c, err = 0; 
  /* flags marking arguments passed */
  int aflag=0;       /* optalign file flag */
	int iflag=0;       /* instruction file flag */
  int oflag=0;       /* optinst file */ 
  int pflag=0;       /* malinp file name */
	char *optalignFile  = "optalign";
  char *instructions  = "instructions.txt";
  char *malinparam    = "malinp.txt";
  char *optinstFile   = "optinst";

/* Process command line arguments  */
while ((c = getopt(argc, argv, "a:i:o:p:")) != -1)
		switch (c) {
		case 'a':
      aflag = 1;
			optalignFile = optarg;
			break;
		case 'i':
      iflag = 1;
			instructions = optarg;
			break;
    case 'o':
      oflag = 1;
      optinstFile = optarg;      
		case 'p':
      pflag = 1;
			malinparam = optarg;
			break;
		case '?':
			err = 1;
			break;
		}

  /* Is the optalign file name present */  
	if (aflag == 0) {	/* -a optalign file */ 
		fprintf(stderr, "%s: missing -a optalign file\n", argv[0]);
		usage();
		exit(1);
	} 

  /* Instruction file */
  if (iflag == 0) { /* -i was mandatory */        
		fprintf(stderr, "%s: missing -i instruction file\n", argv[0]);
		usage();
		exit(1);
  } 

  /*  */
  if (oflag == 0) { 
    fprintf(stderr, "%s: missing -o  optinst file\n", argv[0]);
		usage();
		exit(1);
    } 

  /* malinp file name  */  
  if (pflag == 0) { 
    fprintf(stderr, "%s: missing -p malinp file name\n", argv[0]);
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
  distribution.f = NULL;
  strcpy(distribution.name, "distribution");
  cinst.f = NULL;
  strcpy(cinst.name, "cinst");
  malinp.f = NULL;
  strcpy(malinp.name, malinparam);
  inst.f = NULL;
  strcpy(inst.name, instructions);
  optalign.f = NULL;
  strcpy(optalign.name, optalignFile);
  optinst.f = NULL;
  strcpy(optinst.name, optinstFile);
  themain(&optinst, &optalign, &inst, &malinp, &cinst, &distribution);
_L1:
  if (optinst.f != NULL)
    fclose(optinst.f);
  if (optalign.f != NULL)
    fclose(optalign.f);
  if (inst.f != NULL)
    fclose(inst.f);
  if (malinp.f != NULL)
    fclose(malinp.f);
  if (cinst.f != NULL)
    fclose(cinst.f);
  if (distribution.f != NULL)
    fclose(distribution.f);
  exit(EXIT_SUCCESS);

  return 0;
}
/* End. */