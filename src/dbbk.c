/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dbbk.p" */

/* dbbk: database into delila book conversion program
        by Matthew Yarus
 modified by:
   Thomas D. Schneider, Ph.D.
   toms@alum.mit.edu
   https://alum.mit.edu/www/toms 
 
   module libraries required: delman, delmods

2018 Jan 06, 3.50: Use verson number when possible, otherwise give 1?
2017 Oct 02, 3.49: Allow ACCESSION line for old files
2017 Feb 09, 3.48: put VERSION number on the piece name
                   since that is now standard!!!
2011 Mar 17, 3.47: if can't find topology assume linear
2009 Apr 08, 3.46: correct test for VERSION
2008 Nov 03, 3.45: use VERSION instead of ACCESSION to get the version number
2007 Dec 06, 3.44: make the piece circular in the l1 if in the db
2007 Apr 03, 3.43: allow underscores in ORGANISM name
2006 Oct 25, 3.42: allow for strain name in organism:
  ORGANISM  Escherichia coli K12
  gives E.coli-K12
2004 Jul  8, 3.41: upgrade to gpctime
2003 Aug 19, 3.40: unknown sequences are now marked by 'unknown' and '?'.
2003 Aug 19, 3.39: cleanup
2003 Aug 19, 3.38: changes reported in blocks
2003 Aug 19, 3.36: improve changes so that they are in blocks
2000 Sep  6, 3.35: name lengths increased
1999 Jun 15: fixed bug in copydna that was counting spaces as bases,
             resulting in misplacement of changes features.
1999 Feb 11: make absolutely sure that there are no weird
             control characters copied to the DNA sequence output.
1997 May 4: unrecorded changes
1996 August 17: the changes file now uses the features format of the lister
   program so that the changes can be easily observed.
1995 December 8: program does not write long names anymore.
   This allows the new <NAME> feature of delila to give blanks
   and therefore the alist will not list accesion numbers for full names.
1994 June 10: program now uses ACCESSION numbers!
1992 sep 14: program now has input from db and output to l1 to fit catal.
origin before 1983 july 19 *)

name
      dbbk: database to delila book conversion program
synopsis
      dbbk(db: in, l1: out, changes: out, output: out)
files
      db: contains one or more complete entries from either the EMBL
         or GenBank genetic sequence data bases.  These entries may be
         obtained by using the original libraries or by using an entry
         extraction program.  Dbpull is the delila program for data base
         accessing; to get complete entries the instruction 'all' must
         have been used in the dbpull fin file.  (See delman.use.dbpull)

      l1: each db entry is represented in l1 by a delila style
         entry containing information extracted from the db entry.
         All of l1 has the biologically oriented structure of
         a standard delila book.  The first line of l1 is not part
         of an entry, but contains the computer system date and the
         title of the book.

      changes: Delila programs cannot handle sequences that have
         ambiguities because Delila was designed on the assumption
         that people would finish their sequences.  Unfortunately
         this is not true, and the databases contain bases other
         than acgt to indicate ambiguity.  These are converted to
         "a" and the cases are reported in this file as "unknown".
         NOTE:  "u" is converted to "t".

         The format is the one that the lister program uses as
         features.  In the lister map the unknown region is
         marked by a string of question marks: "???????????".

      output: messages to the user.
description

      This program converts GenBank and EMBL data base entries into a
      book of delila entries.  The organism name is fused together
      with a period and is used for both organsim and chromosome
      names.  Organism and chromosome only change if the name changes
      in db.

      The names of pieces were given by the ACCESSION number (1994
      June 10) but this does not track the versions.  So on 2008 Nov
      03 I switched it to VERSION which looks like: J04553.1.  This
      works with catal and delila.

examples

      The changes file looks like:

define "unknown:1220-4867" "?" "[]" "[]" 0 3646
@ AC012525 1220.0 +1 "unknown:1220-4867" ""

      Lister displays this as:

            *         *1210     *         *1220
 5' c g t g g a a c a a g g a a g a a t t a a a a a 3'
                                          [????????? ... unknown:1220-4867

[for brevity the middle part is skipped]

      *4850     *         *4860     *         *4870
 5' a a a a a a a a a a a a a a a a a a a a t a g a 3'
... ??????????????????????????????????] unknown:1220-4867

see also
      delila.p, dbpull.p,  catal.p, libdef, lister.p

author
      Matthew Yarus and Tom Schneider (modifications)

bugs
      Databases do not have enough data on genes within each piece to make
      a book with gene sections.

      The changes file is a design bug in Delila.

      Genus names are limited to genuslimit (a constant) to avoid
      names longer than the standard Delila limit.

      If a name is larger than idlength  the program simply stops
      reading the name and then dies when it reads the number of bases
      in the entry.  This is currently fixed by making the name 100
      characters but should be done better later.

technical notes

      dbbk is known to convert GenBank entries from July 1989.
      It may not work on later versions.

To COMPILE:
gcc  dbbk.c -o dbbk  -I/home/mplace/bin/p2c/src -L /home/mplace/bin/p2c/src -lm -lp2c

TO RUN:
./dbbk -f ../rhodo_genome.gbff -o dbbk_OUTTEST  -c changes_OUTTEST.txt
*/
#include <getopt.h>  /* getopt API */ 
#include <stdio.h> /* printf */
#include <stdlib.h> 
#include </home/mplace/bin/p2c/src/p2c.h>

#define version         3.50
#define datetimearraylength  19  /* length of dataarray for dates,
       It is just long enough to include the 4 digit year - solving the
       year 2000 problem: */

#define idlength        100 /* length of identification code at beginning of each
                        db library entry */
#define namelength      idlength   /* length of computer system date */
#define lclength        3   /* length of codes identifying each library line,
                            'lc' stands for 'line codes' */
#define lcloc           "LOC" /* short for locus, line code for genb entry
                           starting line. this line contains the entry id */
#define lcid            "ID "  /* marks starting line of embl entry, also has id */
#define lc3spc          "   "  /* three space line code always marks sequence
                               lines of embl type entry */
#define lcos            "OS " /* marks line of embl entry which contains a
                             name for the organism where the sequence is */
#define lcdef           "  O" /*  stands for 'ORGANISM', does for genb what
                              'os' does for embl */
#define lcbas           "BAS"  /* stands for 'BASE COUNT', the line just before
                               ORIGIN in Genbank (hopefully...) */
#define lcsq            "SQ " /* marks line just above embl sequence. this line
                             holds an origin and a base pair total */
#define lcori           "ORI" /* short for 'origin', code for line just above
                              genb sequences. */
#define lcsit           "SIT"  /* short for 'sites', line just below genb sequence */
#define lcterm          "// " /* this code terminates every entry */
#define lcdat           "DAT"  /* first 3 letters of 'date', the first word
                                of a db dateline */
#define genuslimit      1  /* maximum number of characters in the genus name
                            to generate */

 /* a 'u' after the 'lc' or 'id' in a type name indicates that the
    array is unpacked; a 'p' indicates that the array is packed;
    an 's' indicates that the array has more than one dimension */                           

typedef Char idutype[idlength];  /* holds id for reading-writing before packing */
typedef Char idptype[idlength];  /* holds id for string comparisons */
typedef idptype alpha;           /* holds computer system date */
typedef Char lcutype[lclength];  /* holds library line code for reading-writing before packing */
typedef Char lcptype[lclength]; /* holds line codes for string comparisons */

/* used to indicate whether an entry is of embl or genb type */
typedef enum {
  embl, genb, none
} libsused;

/* array for dates */
typedef Char datetimearray[datetimearraylength];

Static _TEXT db; /* contains complete embl or genbank data base entries */
Static _TEXT l1; /* contains one converted delila type entry for each entry in the db file */

Static _TEXT changes;  /*  changes made to the sequences  */
Static boolean notwarned;  /* True if the user has not yet been warned
                           about changes of bases */
Static jmp_buf _JL1;

Static Void halt()
{
  /* stop the program.  the procedure performs a goto to the end of the
   program.  you must have a label:
      label 1;
   declared, and also the end of the program must have this label:
      1: end.
   examples are in the module libraries.
   this is the only goto in the delila system.
*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}

/* convert the character c to lower case */
Static Void decapitilize(c)
Char *c;
{
  long n;
  n = *c;

  if (n >= 'A' && n <= 'Z')
    *c = _tolower(n);
}

Static Void writeidptype(fout, writeasterisk, carriagereturn, thename)
_TEXT *fout;
boolean writeasterisk, carriagereturn;
Char *thename;
{
/** holds the name of the organism in which the sequence occurs *)
(* Write the thename name to file fout.  If writeasterisk is true,
then write "*" at the start.  If carriagereturn is true, add a carriage
return at the end. *
*/
  long index = 1;

  if (writeasterisk)
    fprintf(fout->f, "* ");
  do {
    if (thename[index-1] != ' ')
      putc(thename[index-1], fout->f);
    index++;
  } while (index != idlength && thename[index-1] != ' ');
  if (carriagereturn)
    putc('\n', fout->f);
}



Static Void finishchanges(changes, inchanges, changestart, entryname,
			  basenumber)
_TEXT *changes;
boolean *inchanges;  /* inside a set of changes? */
long *changestart;   /* coordinate of start of changes */
Char *entryname;     /* holds the name of a fout entry */
long basenumber;     /* current coordinate */
{
  /* finish writing the change */
  *inchanges = false;
  fprintf(changes->f,
	  "define \"unknown:%ld-%ld\" \"?\" \"[]\" \"[]\" 0 %ld\n",
	  *changestart, basenumber, basenumber - *changestart - 1);
  fprintf(changes->f, "@ ");
  writeidptype(changes, false, false, entryname);
  fprintf(changes->f, " %ld.0 +1 \"unknown:%ld-%ld\" \"\"\n",
	  *changestart, *changestart, basenumber);
}



Static Void copydna(fin, fout, changes, basenumber, entryname, inchanges,
		    changestart)
_TEXT *fin, *fout, *changes;
long *basenumber;   /* count of bases written so far */
Char *entryname;    /* holds the name of a fout entry */
boolean *inchanges; /* inside a set of changes? */
long *changestart;  /* coordinate of start of changes */
{
/*copy a line from file fin to file fout, converting the letters
to lower case.  Report changes to the changes file */
  Char c;  /* the character being manipulated */

  while (!P_eoln(fin->f)) {
    c = P_peek(fin->f);
    decapitilize(&c);

/* 1999 Feb 11: make absolutely sure that there are no weird
   control characters copied to the DNA sequence output.  */
    if (c >= 'a' && c <= 'z' || c == ' ') {
      if (c >= 'a' && c <= 'z')
	(*basenumber)++;
/* convert 'u' to 't' */
  if (c == 'u')
    	c = 't';

/* see if it's ok sequence, otherwise, report to the boss */
  if (c != ' ' && c != 't' && c != 'g' && c != 'c' && c != 'a') {
    if (notwarned) {
      notwarned = false;
      if (*changes->name != '\0') {
        if (changes->f != NULL)
          changes->f = freopen(changes->name, "w", changes->f);
        else
          changes->f = fopen(changes->name, "w");
      } else {
        if (changes->f != NULL)
          rewind(changes->f);
        else
          changes->f = tmpfile();
      }
      if (changes->f == NULL)
        _EscIO2(FileNotFound, changes->name);
      SETUPBUF(changes->f, Char);
      fprintf(changes->f, "* dbbk %4.2f\n", version);
	}

	if (!*inchanges) {
	  *inchanges = true;
	  *changestart = *basenumber;
	}

	c = 'a';
 }  else {
	if (c != ' ' && *inchanges)
	  finishchanges(changes, inchanges, changestart, entryname,
			*basenumber);
      }

      putc(c, fout->f);
    }
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
  putc('\n', fout->f);
}

/* grabs a library line code and then packs it for string comparisons */
Static Void readlcu(lib, liblcu, liblcp)
_TEXT *lib;
Char *liblcu;
Char *liblcp;
{
  long index;

  if (BUFEOF(lib->f)) {
    printf("in readlcu: hit end of file; last line type was linetype: %.*s\n",
	   lclength, liblcp);
    halt();
  }

  for (index = 0; index < lclength; index++) {
    if (P_eoln(lib->f))
      liblcu[index] = ' ';
    else {
      liblcu[index] = getc(lib->f);
      if (liblcu[index] == '\n')
	liblcu[index] = ' ';
    }
  }
  memcpy(liblcp, liblcu, sizeof(lcptype));
}

/* tests two line codes for equality. if your computer system version of
   pascal does not do string comparisons simply change lcequal and
   the program will be fixed throughout */
Static boolean lcequal(lcp1, lcp2)
Char *lcp1, *lcp2;
{/* p2c: dbbk.p: Note: Eliminated unused assignment statement [338] */
  if (!strncmp(lcp1, lcp2, sizeof(lcptype)))
    return true;
  return false;
}
/* are id a and id b equal? */
Static boolean idequal(a, b)
Char *a, *b;
{
  boolean equal;
  long index = 1;

  do {
    equal = (a[index-1] == b[index-1]);
    index++;
  } while (equal && index <= idlength);
  return equal;
}

/* clear id a */
Static Void idclear(ida)
Char *ida;
{
  long i;

  for (i = 0; i < idlength; i++)
    ida[i] = ' ';
}

/* copy id a into id b */
Static Void idcopy(ida, idb)
Char *ida, *idb;
{
  long i;

  for (i = 0; i < idlength; i++)
    idb[i] = ida[i];
}



Static Void getid(fin, finidp)
_TEXT *fin;  /* see global */
Char *finidp;
{
/* holds requested library id finds the next string of non-space characters
   following the file cursor, adding spaces at the end if the string is too
   short, and then packs the string into the finidp array 
*/
  long index = 0;
  idutype finidu;  /* holds id for reading in before packing */ 

  while (P_peek(fin->f) == ' ')  /* advances to first id character */
    getc(fin->f);
/* the following loop grabs all id characters */
  while (P_peek(fin->f) != ' ' && index < idlength) {
    index++;
    if (P_eoln(fin->f))
      finidu[index-1] = ' ';
    else {
      finidu[index-1] = getc(fin->f);
      if (finidu[index-1] == '\n')
	finidu[index-1] = ' ';
    }
  }

/* the following loop fills out id if id is too short */
  while (index < idlength) {
    index++;
    finidu[index-1] = ' ';
  }
  memcpy(finidp, finidu, sizeof(idptype));
}


Static Void getspecies(fin, finidp)
_TEXT *fin;  /* see global */
Char *finidp;  /* holds requested library id */
{
/* skip blanks, pick up the next two names from fin, put a period between them
and return this as the species name.
Add spaces at the end if the string is too short, and pack the
string into the finidp array.  Note that the genus name is not longer
than genuslimit.  2007 Apr 3: interpret '_' as spaces in the name.
*/
  long index = 0;
  idutype finidu;  /* holds id for reading in before packing */

  while (P_peek(fin->f) == ' ')   /* advances to first id character */
    getc(fin->f);

/* grab the first part of the name */
  while (((P_peek(fin->f) != ' ') & (P_peek(fin->f) != '_')) &&
	 index < idlength) {
    index++;
    if (P_eoln(fin->f))
      finidu[index-1] = ' ';
    else {
      finidu[index-1] = getc(fin->f);
      if (finidu[index-1] == '\n')
	finidu[index-1] = ' ';
    }
    if (finidu[index-1] == '_')
      finidu[index-1] = ' ';
  }
/* now skip to the next part of the species name */
  while ((P_peek(fin->f) == ' ') | (P_peek(fin->f) == '_'))
    getc(fin->f);

/* force genus to be genuslimit characters long */
  if (index > genuslimit)
    index = genuslimit;

  index++;    
  finidu[index-1] = '.';   /* now put the period into the name */

/* grab the second part of the name */
  while (!P_eoln(fin->f)) {
    index++;
    if (P_eoln(fin->f)) {
      finidu[index-1] = ' ';
      continue;
    }
    finidu[index-1] = getc(fin->f);
    if (finidu[index-1] == '\n')
      finidu[index-1] = ' ';
    if (finidu[index-1] == ' ')
      finidu[index-1] = '-';
  }

/* the following loop fills out id if id is too short */
  while (index < idlength) {
    index++;
    finidu[index-1] = ' ';
  }
memcpy(finidp, finidu, sizeof(idptype));
}

Static Void getdatetime(adatetime)
Char *adatetime;
{
  /*get the date and time into a single array from the system clock.
     adatetime contains the date:
         1980/06/09 18:49:11
         ye mo da ho mi se
  (year, month, day, hour, minute, second).
  As of 2000 February 18, the Sun Pascal compiler requires a formatting
  statement.  This statement allows the date to be generated in this
  standard Delila format in a single call.  Information about the
  formatting statement is available on the manual page for date in Unix.
  If a computer does not have this method, see the 'oldgetdatetime' routine
  in delmod.p (https://alum.mit.edu/www/toms/delila/delmod.html)
  for some conversion code.
  
  GPC Functions:
  function  GetUnixTime (var MicroSecond : Integer) : UnixTimeType;
  
  http://agnes.dida.physik.uni-essen.de/~gnu-pascal/gpc_109.html#SEC109
  
  7.10.8 Date And Time Routines 
  
  procedure GetTimeStamp (var t : TimeStamp); 
  function Date (t : TimeStamp) : packed array [1 .. DateLength] of Char; 
  function Time (t : TimeStamp) : packed array [1 .. TimeLength] of Char; 
  
  DateLength and TimeLength are implementation dependent constants. 
  
  GetTimeStamp (t) fills the record `t' with values. If they are valid, the Boolean
  flags are set to True. 
  
  TimeStamp is a predefined type in the Extended Pascal standard. It may be
  extended in an implementation, and is indeed extended in GPC. For the full
  definition of `TimeStamp', see section 8.255 TimeStamp. 
*/
  Char adate[datetimearraylength], atime[datetimearraylength];
  /**/
  Char month[3];
  long index;

  /**/

  VAXdate(adate);
  VAXtime(atime);

  /**/

  for (index = 1; index <= 4; index++)
    adatetime[index-1] = adate[index+6];
  adatetime[4] = '/';
  for (index = 4; index <= 6; index++)
    month[index-4] = adate[index-1];
  if (!strncmp(month, "JAN", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '1';
  } else if (!strncmp(month, "FEB", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '2';
  } else if (!strncmp(month, "MAR", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '3';
  } else if (!strncmp(month, "APR", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '4';
  } else if (!strncmp(month, "MAY", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '5';
  } else if (!strncmp(month, "JUN", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '6';
  } else if (!strncmp(month, "JUL", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '7';
  } else if (!strncmp(month, "AUG", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '8';
  } else if (!strncmp(month, "SEP", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '9';
  } else if (!strncmp(month, "OCT", 3)) {
    adatetime[5] = '1';
    adatetime[6] = '0';
  } else if (!strncmp(month, "NOV", 3)) {
    adatetime[5] = '1';
    adatetime[6] = '1';
  } else if (!strncmp(month, "DEC", 3)) {
    adatetime[5] = '1';
    adatetime[6] = '2';
  }
  adatetime[7] = '/';
  for (index = 7; index <= 8; index++)
    adatetime[index+1] = adate[index-7];


  if (adatetime[5] == ' ')
    adatetime[5] = '0';
  if (adatetime[8] == ' ')
    adatetime[8] = '0';

  adatetime[10] = ' ';
  for (index = 10; index <= 17; index++)
    adatetime[index+1] = atime[index-10];
}


/* expand the date and time out and print in the file */
Static Void writedatetime(thefile, adatetime)
_TEXT *thefile;
Char *adatetime;
{
  long index;

  for (index = 0; index < datetimearraylength; index++)
    putc(adatetime[index], thefile->f);
}


/* read the date and time from the file */
Static Void readdatetime(thefile, adatetime)
_TEXT *thefile;
Char *adatetime;
{
  long index;
  Char udatetime[datetimearraylength];

  for (index = 0; index < datetimearraylength; index++) {
    udatetime[index] = getc(thefile->f);
    if (udatetime[index] == '\n')
      udatetime[index] = ' ';
  }
  memcpy(adatetime, udatetime, sizeof(datetimearray));
  if (adatetime[2] == '/' && adatetime[11] == ':') {
    printf("You have an old datetime (only 2 year digits): \n");
    for (index = 0; index < datetimearraylength; index++)
      putchar(adatetime[index]);
    printf("\nConvert your database to 4 digit years.\n");
    halt();
  }

  if (adatetime[4] == '/' && adatetime[7] == '/' && adatetime[13] == ':' &&
      adatetime[16] == ':')
    return;
  printf("readdatetime: bad date time read:\n");
  for (index = 0; index < datetimearraylength; index++)
    putchar(adatetime[index]);
  putchar('\n');
  halt();
}

/* is the character c blank or tab? */
#define tab             9
Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == tab);
}
#undef tab

/* skip over blanks until a non-blank, or end of line, is found */
Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while (isblankDelila(P_peek(thefile->f)) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}

/* skip over nonblanks until a blank, or end of line, is found */
Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((!isblankDelila(P_peek(thefile->f))) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}

/* skip over a data column */
Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}
/* contains delila type entries converted from the entries in the fin file */
Static Void note(fout, piecenum)
_TEXT *fout;   /* contains delila type entries converted from the entries in the fin file */
long piecenum; /* delila pieces are numbered */
{
/* simulates the note section of a delila entry */
  fprintf(fout->f, "note\n");
  fprintf(fout->f, "* #%ld\n", piecenum);
  fprintf(fout->f, "note\n");
}

Static Void dna(fin, fout, libtitle, entryname)
_TEXT *fin; /* holds embl and genb entries */
_TEXT *fout; /* contains delila type entries converted from the entries in the fin file */
libsused libtitle; /* indicates whether a fin entry is of the embl or genbank format */
Char *entryname;  /* holds the name of a fout entry */
{
/* simulates the dna section of a delila entry */
  long basenumber = 0; /* count of the bases written so far */
  long dumpint;    /* grabs an integer that is not used */ 
  lcutype finlcu; /* grabs fin line codes */
  lcptype finlcp; /* packs fin line codes for string comparisons */

  boolean inchanges = false; /* inside a set of changes? */
  long changestart = -LONG_MAX; /* coordinate of start of changes */

  fprintf(fout->f, "dna\n");
  switch (libtitle) {

  case none:
    printf(" PROCEDURE DNA HAS BIZARRE VALUE OF\n");
    printf(" LIBTITLE = NONE\n");
    halt();
    break;

  /* note: procedure getentry has placed file cursor at the
     beginning of sequence data before dna is called */
  case embl:
    do {
      readlcu(fin, finlcu, finlcp);
      /* lc3spc is used because embl sequence data lines have no line codes */
      if (lcequal(finlcp, lc3spc)) {
	        fprintf(fout->f, "* ");
          /* all data lines in delila entries start with '*'s */
	        copydna(fin, fout, &changes, &basenumber, entryname, &inchanges,&changestart);
      }  else {
	fscanf(fin->f, "%*[^\n]");
	getc(fin->f);
      }
    } while (!lcequal(finlcp, lcterm));
    break;

  case genb:
    readlcu(fin, finlcu, finlcp);
    /* sometimes no 'sites' section occurs after sequence, so we run into terminus code */
    while ((!lcequal(finlcp, lcsit)) & (!lcequal(finlcp, lcterm))) {
      fscanf(fin->f, "%ld", &dumpint);
      /** genb sequence data lines have no line code, but they
          do start with a cooridinate integer which is not needed here */
      fprintf(fout->f, "* ");
      copydna(fin, fout, &changes, &basenumber, entryname, &inchanges,
	      &changestart);
      readlcu(fin, finlcu, finlcp);
    }
    break;
  }

  if (inchanges)
    finishchanges(&changes, &inchanges, &changestart, entryname, basenumber);
  fprintf(fout->f, "dna\n");
}


Static Void piece(fin, fout, piecenum, libtitle, bpint, entryname, topology)
_TEXT *fin; /* holds embl and genbank entries */ 
_TEXT *fout; /* contains delila style entries converted from fin entries */
long piecenum; /* delila pieces are numbered */
libsused libtitle;  /* indicates whether entry is of embl or genbank type */
long bpint;  /* holds the number of base pairs found in the fin entry sequence data */
Char *entryname;  /* holds the name of a fout entry */
Char topology; /* linear or circular topology */
{
  long index;
  fprintf(fout->f, "piece\n");
  writeidptype(fout, true, true, entryname);
  fprintf(fout->f, "* \n");
    note(fout, piecenum);
  fprintf(fout->f, "* 1\n");
  /* this next loop is done twice because all sequence is assigned
     the value of 'linear' to avoid certain complications */
  for (index = 1; index <= 2; index++) {
    if (topology == 'l')
      fprintf(fout->f, "* linear\n");
    else
      fprintf(fout->f, "* circular\n");
    fprintf(fout->f, "* +\n");
    fprintf(fout->f, "* 1\n");
    fprintf(fout->f, "* %ld\n", bpint);
  }
  dna(fin, fout, libtitle, entryname);
  /* this procedure and each of the next three in turn calls the
     preceding procedure. this hierarchy is meant to simulate the
     hierarchies of biological classification */
  fprintf(fout->f, "piece\n");
}

Static Void chromosome(fin, fout, piecenum, libtitle, bpint, entryname,
		       orgname, topology)
_TEXT *fin;  /* holds embl and genb entries */
_TEXT *fout; /* contains delila style entries converted from fin entries */
long piecenum; /* delila pieces are numbered */
libsused libtitle; /* indicates whether fin entry is of an embl or genb format */
long bpint;  /* holds the number of base pairs found in the fin entry sequence data */
Char *entryname; /* holds name of a fout entry */
Char *orgname; /* holds the name of the organism in which the sequence occurs */
Char topology; /* linear or circular topology */
{
  fprintf(fout->f, "chromosome\n");

  /*formerly dbbk gave the entry name as the name, with the line
    writeln ( fout, '* ', entryname );
    writeln ( fout, '* ', entryname );
   suppress that and use the organism: */

  writeidptype(fout, true, true, orgname);
  fprintf(fout->f, "* \n");

  fprintf(fout->f, "* 1\n");
  fprintf(fout->f, "* %ld\n", bpint);
  piece(fin, fout, piecenum, libtitle, bpint, entryname, topology);
}
Static Void organism(fin, fout, piecenum, libtitle, bpint, entryname,
		     firstorganism, orgname, oldorgname, topology)
_TEXT *fin;   /* holds embl and genb entries */
_TEXT *fout;  /* contains delila style entries converted from fin entries */
long piecenum; /* delila pieces are numbered */
libsused libtitle;  /* indicates whether fin entry is of embl or genbank format */
long bpint;  /* holds number of base pairs found in the fin entry sequenc data */
Char *entryname;  /* holds the name of the fout delila style entry */
boolean *firstorganism;  /* true if this is the first organism to be written */
Char *orgname; /* holds the name of the organism in which the sequence occurs */
Char *oldorgname; /* old organism name,track of to avoid writing new organism structure when it is not needed. */
Char topology;  /* linear or circular topology */
{
  _TEXT TEMP;

  if (idequal(orgname, oldorgname))
    piece(fin, fout, piecenum, libtitle, bpint, entryname, topology);
  else {
    if (*firstorganism)
      *firstorganism = false;  /* avoid closing non-existant previous */
    else {
      /* close up previous organism */
      fprintf(fout->f, "chromosome\n");
      fprintf(fout->f, "organism\n");
    }

    printf("organism ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeidptype(&TEMP, true, true, orgname);

    fprintf(fout->f, "organism\n");

    writeidptype(fout, true, true, orgname);
    fprintf(fout->f, "* \n");

    fprintf(fout->f, "* bases\n");
    chromosome(fin, fout, piecenum, libtitle, bpint, entryname, orgname,
	       topology);

    idcopy(orgname, oldorgname);
  }
  TEMP.f = stdout;
  *TEMP.name = '\0';

  /* identify the entry under this organism */
  writeidptype(&TEMP, false, false, entryname);
  putchar('\n');
}

Static Void getentry(fin, fout, piecenum, firstorganism, oldorgname)
_TEXT *fin; /* holds embl and genb entries */
_TEXT *fout; /* contains delila style entries converted from fin entries */
long piecenum; /* delila pieces are numbered */
boolean *firstorganism; /* true if this is the first organism to be written */
Char *oldorgname; /* old organism name, track to avoid writing new organism structure when it is not needed. */
{
 /* goes to the beginning of a fin entry, and then runs through it
    grabbing values for dna, piece, etc. to use */
  boolean done = false; /* terminates loops when certain conditions are met */
  idptype dumpword; /* grabs words that come between the fin file cursor and data that it is trying to get to */
  lcutype finlcu;  /* grabs fin line codes */
  lcptype finlcp;  /* packs fin line codes for string comparisons */
  libsused libtitle; /* indicates whether a fin entry is an embl or genbank format */
  long bpint; /* holds the number of base pairs found in the fin entry sequence data */
  idptype entryname;  /* holds the name of a fout entry */
  idptype locusname;  /* dummy variable holds the name of a fout entry */
  idptype orgname; /* holds the name of the organism in which the genetic sequence occurs */
  Char topology; /* linear or circular topology for this piece */
  _TEXT TEMP;

  do {
    readlcu(fin, finlcu, finlcp);
    if (lcequal(finlcp, lcid)) {
      libtitle = embl;
      getid(fin, entryname); /* entryname is filled with the id code of the entry */

      do {
	readlcu(fin, finlcu, finlcp);
	if (lcequal(finlcp, lcos)) {
	  /** the first 'os ' coded line generally starts with the name of the entry organism */
	  getid(fin, orgname);
	  done = true;
	}
	fscanf(fin->f, "%*[^\n]");
	getc(fin->f);
      } while (!done);
      done = false;
      do {
	readlcu(fin, finlcu, finlcp);
	if (lcequal(finlcp, lcsq) | BUFEOF(fin->f)) {
	  getid(fin, dumpword);  /* dumps the word 'sequence' */
	  fscanf(fin->f, "%ld%*[^\n]", &bpint);  //* gets cursor to beginning of the genetic sequence data */
	  getc(fin->f);

	  done = true;
	} else {
	  fscanf(fin->f, "%*[^\n]");
	  getc(fin->f);
	}
      } while (!done);
      topology = 'l';
      organism(fin, fout, piecenum, libtitle, bpint, entryname, firstorganism,
	       orgname, oldorgname, topology);
    } else if (lcequal(finlcp, lcloc)) {
      libtitle = genb;

      getc(fin->f);
      getc(fin->f);
      getid(fin, locusname);  /* entryname assigned id code */
      fscanf(fin->f, "%ld", &bpint); /* get the length */
      skipcolumn(fin); /* skip 'bp' */
      skipcolumn(fin); /* skip 'DNA' */
      skipblanks(fin); /* skip blanks */
      if (P_eoln(fin->f)) {
	printf("Cannot get topology for ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeidptype(&TEMP, false, true, locusname);
	halt();
      }
      if ((P_peek(fin->f) == 'l') | (P_peek(fin->f) == 'c'))
	topology = P_peek(fin->f);
      else {
	printf("Topology for ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeidptype(&TEMP, false, false, locusname);
	printf(" is not l(inear) or c(ircular)\n");
	while (!P_eoln(fin->f)) {
	  putchar(P_peek(fin->f));
	  getc(fin->f);
	}
	printf("\nASSUMING LINEAR\n");

	topology = 'l';
      }
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
/* now instead we pick up the ACCSSION NUMBER: */
      while (P_peek(fin->f) != 'V') {
	fscanf(fin->f, "%*[^\n]");
	getc(fin->f);
	if (!BUFEOF(fin->f))
	  continue;

	if (*fin->name != '\0') {
	  if (fin->f != NULL)
	    fin->f = freopen(fin->name, "r", fin->f);
	  else
	    fin->f = fopen(fin->name, "r");
	} else
	  rewind(fin->f);
	if (fin->f == NULL) {

	  _EscIO2(FileNotFound, fin->name);
	}
	RESETBUF(fin->f, Char);
  /* now instead we pick up the VERSION NUMBER: */
	while (P_peek(fin->f) != 'A') {
	  fscanf(fin->f, "%*[^\n]");
	  getc(fin->f);
	  if (BUFEOF(fin->f)) {
	    printf("could not find VERSION or ACCESSSION\n");
	    halt();
	  }
	}
      }

      while (P_peek(fin->f) != ' ')
	getc(fin->f);
      while (P_peek(fin->f) == ' ')
	getc(fin->f);
      getid(fin, entryname);

      do {
	readlcu(fin, finlcu, finlcp);

	if (lcequal(finlcp, lcbas)) {
	  /*

*/
	  done = true;
	  printf("WARNING: in entry ");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writeidptype(&TEMP, false, false, entryname);
	  printf("there was no ORGANISM.  Using:");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writeidptype(&TEMP, false, false, entryname);
	  memcpy(orgname, entryname, sizeof(idptype));
	} else {
	  if (lcequal(finlcp, lcdef) | BUFEOF(fin->f)) {
	    getid(fin, dumpword);
	    getspecies(fin, orgname);
	    done = true;
	  }
	  fscanf(fin->f, "%*[^\n]");
	  getc(fin->f);
	}
      } while (!done);

      do {
	readlcu(fin, finlcu, finlcp);
	fscanf(fin->f, "%*[^\n]");
	getc(fin->f);
      } while (!lcequal(finlcp, lcori));
      organism(fin, fout, piecenum, libtitle, bpint, entryname, firstorganism,
	       orgname, oldorgname, topology);

    } else {
      libtitle = none;
      /*
*/
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
    }
  } while (!((libtitle != none) | BUFEOF(fin->f)));

}
/* adds computer system date and possibly fin date to first line of fout */
Static Void datebook(fin, fout)
_TEXT *fin, *fout;
{
  long index;
  Char dumpchar; /* holds an unwanted character */
  boolean findated = false; /* 'true' indicates that fin has a dateline */
  datetimearray oldate;     /* holds fin date */ 
  datetimearray adatetime;  /* holds computer system date */
  lcutype finlcu;           /* holds a fin line code */
  lcptype finlcp;           /* finlcu(unpacked), finlcp(packed) */

  readlcu(fin, finlcu, finlcp);
  if (lcequal(finlcp, lcdat)) {
    findated = true;
    dumpchar = getc(fin->f);
    if (dumpchar == '\n')
      dumpchar = ' ';
    do {
      getc(fin->f);
    } while (P_peek(fin->f) == ' ');
    readdatetime(fin, oldate);
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
  } else {
    if (*fin->name != '\0') {
      if (fin->f != NULL)
	fin->f = freopen(fin->name, "r", fin->f);
      else
	fin->f = fopen(fin->name, "r");
    } else
      rewind(fin->f);
    if (fin->f == NULL)
      _EscIO2(FileNotFound, fin->name);
    RESETBUF(fin->f, Char);
  }
  getdatetime(adatetime);
  fprintf(fout->f, "* ");
  if (findated) {
    writedatetime(fout, adatetime);
    fprintf(fout->f, ", ");
    writedatetime(fout, oldate);
    fprintf(fout->f, ", ");
    return;
  }
  for (index = 1; index <= 2; index++) {
    writedatetime(fout, adatetime);
    fprintf(fout->f, ", ");
  }
}
/* structures the whole program by calling other procedures */
Static Void order(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  boolean firstorganism = true;
  /*true only when the first organsism is
   about to be written.  used to avoid closing the (non-existant)
   previous organism */
  idptype oldorgname;
  /*old organism name.  kept track of to
    avoid writing new organism structure when it is not needed. */
  long piecenum = 0; /* each delila 'piece' posesses a unique number */

  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  RESETBUF(fin->f, Char);
  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);
  if (BUFEOF(fin->f)) {
    printf(" INPUT FILE FIN IS EMPTY\n");
    halt();
  }
  idclear(oldorgname);
  notwarned = true;

  datebook(fin, fout);
  fprintf(fout->f, "dbbk %4.2f\n", version);

  do {
    piecenum++;
    getentry(fin, fout, piecenum, &firstorganism, oldorgname);
  } while (!BUFEOF(fin->f));


  fprintf(fout->f, "chromosome\n");
  fprintf(fout->f, "organism\n");

  if (!notwarned) {
    printf("WARNING: some sequences have been altered,");
    printf("see the changes file.\n");
  }
}

int main(int argc, Char **argv)
{
  extern char *optarg;
	extern int optind;
	int c, err = 0; 
  /* flags marking arguments passed */
	int fflag=0;       /* file flag */
  int cflag=0;       /* Change output file name */
  int oflag=0;       /* Output file name  */
	char *fName = "filename.txt";
  char *change = "outputChanges.txt";
  char *outFile = "output.txt";
	static char usage[] = "usage: %s -f genome.gnbk -c changes.txt -o output.txt\n";

/* Process command line arguments  */
while ((c = getopt(argc, argv, "f:c:o:")) != -1)
		switch (c) {
		case 'o':
      oflag = 1;
			outFile = optarg;
			break;
		case 'f':
      fflag = 1;
			fName = optarg;
			break;
		case 'c':
      cflag = 1;
			change = optarg;
			break;
		case '?':
			err = 1;
			break;
		}
  /* Is the Output file name present */  
	if (oflag == 0) {	/* -o was mandatory */ 
		fprintf(stderr, "%s: missing -o option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} 

  /* Input file genbank file */
  if (fflag == 0) { /* -f was mandatory */        
		fprintf(stderr, "%s: missing -f option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
  } 

  /* Change file name  */  
  if (cflag == 0) { 
    fprintf(stderr, "%s: missing -c option really\n", argv[0]);
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
  changes.f = NULL;
  strcpy(changes.name, change);
  l1.f = NULL;
  strcpy(l1.name, outFile);
  db.f = NULL;
  strcpy(db.name, fName);
  printf(" dbbk %4.2f\n", version);
  order(&db, &l1);
_L1:
  if (db.f != NULL)
    fclose(db.f);
  if (l1.f != NULL)
    fclose(l1.f);
  if (changes.f != NULL)
    fclose(changes.f);
  exit(EXIT_SUCCESS);

  return 0;
}
/* End. */
