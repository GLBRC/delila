/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "catal.p" */

/* 
catal: the catalog program

    by Michael Aden and Thomas Schneider
    module libraries needed: delman, delmods.

Thomas D. Schneider, Ph.D.
https://alum.mit.edu/www/toms 
 
 version = 9.64; (* of catal.p 2020 Feb 20
2020 Feb 20, 9.64: add dbbk to list of See Also
2018 Sep 17, 9.63: switch to timegpc.p modules
2017 Oct 02, 9.62: ecoli fails: 'catin: Pascal system error 0'
                   This was files not permitted to be writable
2017 Jul 14, 9.61: duplicate piece name problem
                   -
                   In version 9.41 of catal originally linelength was
                   80 characters and namelength was 50 characters. 
                   Procedure changename assumes that linelength is at
                   least 2 characters longer than namelength, probably
                   because in Delila books the lines begin with '* '. 
                   So linelength should be at least 2 characters
                   longer than namelength.  I'll use 210 for safety.
                   -
2017 Jul 11, 9.60: namelength/linelength set to 300 to allow even longer names
2017 Jul 10, 9.59: namelength set to 200 to allow even longer names
2016 Jan 25, 9.58: backup
2011 Nov 03, 9.56: new parameters: no comments and get from to format
2011 Jan 19, 9.55: writepieceinfo not squish coordinates together
2004 Jul  8, 9.54: tidy up
2004 Jul  8, 9.53: fix invalid operands to binary, introduce equalname
2004 Jul  8, 9.52: attack gpc compiler errors, upgrademodule catal.p gpctime.p
2001 Mar 16, 9.51: length of date in cat file corrected
2000 Sep 25, 9.50: duplicate titles removed
1999 Mar 31, 9.49: previous change not noted
1999 Mar 18, 9.46: names only written to their length
prior version: 1998 January 27

name
      catal: cataloguer of delila libraries, the catalogue program

synopsis
      catal(catalp: in,
            l1: in, cat1: out, lib1: out,
            l2: in, cat2: out, lib2: out,
            l3: in, cat3: out, lib3: out,
            humcat: out, catin: out,
            output: out)

files
      catalp: parameters to control the program.

         first line:  the library dates are not changed if the first
         character is 'n' (no date modification) or 'b' (book source
         of library, dates are not to be changed).  otherwise the
         dates are advanced.

         second line:  docomments: If there is a second line, then if
         it begins with 'n' comments are suppressed.

         third line:  fromtoinst: If there is a second line, then the third line must exist
         If it begins with 'f' then the Delila instructions in catin will be in from-to form:

            get from 50 -0 to same +[piecelength-1];

      l1: the first input file of the library
      cat1: the first catalogue
      lib1: the first output library

      l2: the second input file of the library
      cat2: the second catalogue
      lib2: the second output library

      l3: the third input file of the library
      cat3: the third catalogue
      lib3: the third output library

      output: progress report and error messages

      humcat: the catalogue generated for humans.  it includes the names
         of things in the libraries and their coordinates.  humcat is quite
         wide so you will need a line-printer to print it.  alternatively
         you can use the split program.
      catin:  Catalog listing as delila instructions.  This is a set of Delila
         instructions for grabbing each of the pieces in the library.  These
         make it easy to start writing instructions.

description
      The catalogue program checks all the input libraries for correct
      structure.  Duplicated names are removed and a new set of library
      files is created, along with their catalogues for delila.  A catalogue
      is also generated for people to use.  Each new library is associated with
      one catalogue.  Under most circumstances this pair can be given to
      delila along with pairs created at different times.

documentation
      libdef (defines catal), delman.use.coordinates, delman.construction

see also
      dbbk.p loocat.p, delila.p, split.p

author
      Michael Aden and Thomas Schneider

bugs
      Not all checks on the library structure are made.  Some checks from
      libdef are now outdated or not done: p. 3.1 2 d, e, f, g and l.

technical notes
      The circumstances when a library-catalogue pair must not be used with
      another pair:  it is not possible for delila to check for two
      organisms with the same name that exist in different libraries.  In
      this case, run the two libraries through catal together to eliminate
      the ambiguity.  If this is not done, the results will be anomalous.

history of changes to catal:
      from original concepts by thomas schneider 1979 october 28
      written 1980 june 10 by michael aden
      modified 1980 june 14 - thomas schneider
      modified 1980 june 22 - michael aden
         insertion of code to handle duplicate names.
      modified 1980 july 5 - michael aden
         correction in listing indentation
      modified 1980 december 23 - thomas schneider: rearrange file names
      modified 1981 march 23 - michael aden: insertion of standard halt
         as opposed to non-standard halt.
      modified 1982 july 12 - michael aden: change to take care of forward
         reference duplicate names. more error checking is now done.
      modified 1982 july 14 - thomas schneider: standard program format and
         documentation.  modules inserted.  name changes.
      modified 1982 july 17 - thomas schneider: unlimitln added,
         readline logic altered.
      modified 1982 aug 1 - 'problems encountered', errors to humcat, checkstar
      modified 1983 dec 15 - duplicate names start with *2 using global
         constant firstnumber.
      modified 1984 jan 26 - procedure dumpline cannot produce lines
         shorter than 2 characters.  this assures that delila is happy
         when reading the library, since delila assumes that there is
         at least one space following each '*'.
      modified 1992 sep 14 - output names are the same as delila uses.
         This is by far the most common use of the program - why fight it?
         old name     new name
         --------     --------
         newl1        lib1
         newl2        lib2
         newl3        lib3
         c1           cat1
         c2           cat2
         c3           cat3
      modified 1994 March 4 - The humin file is introduced.
      modified 1994 April 7 - The humin file is renamed catin.

      1995 Nov 21:  Catal objected to a 1 base long piece, "coordinate
      beginning must be less than ending" This should be "less than or equal
      to".  "as required by libdef catalogue definition p. 3.1 2h" Libdef was
      also modified.

      1995 Dec 8:  Routine checkstar is modified so that it allows the
      fullname to be empty.  This allows the new <NAME> feature of Delila
      (see libdef) to give a blank when no name is assigned.

      1999 March 18: made names have type name, like in delila (!)
         tightened humcat output.

technical notes:
      l1, l2, ... numlibfil = the files of the old library
      c1, c2, ... numcatfil = the files of the new catalog for the librarian
      lib1, lib2, ... numcatfil = the files of the new library
      humcat = the catalog for humans
      catalp = a file used to set catalog parameters.
         the library dates are not modified if the first character
         is 'n' (no date modification) or
            'b' (book source of library, dates not to be changed)
      output = progress report and error messages


   the catalog program checks to see that the library
  is in proper format and checks to see that the names in each
  classification are unique .
     the catalog also generates:
(1) new library files
(2) a catalog for the librarian
(3) a human-readable catalog.

      lll = places that must be changed when one changes the number of
         library files: numlibfil
      ccc = places that must be changed when one changes the number of
         catalog files: numcatfil

      further documentation for this program is in:
      'organism and recognition class library definition:
       a dna sequence data base' 1980 june 9


problems encountered and resolved during delila catalog implementation

problem 1.
      duplicate entry names for any two of a given type of structure
      (e.g., two transcripts with the same name) may not necessarily
      be fatal (as in a library of transcripts, which may have dup-
      licate names after being pulled out).

resolution 1.
      duplicate names are changed and a warning is issued.

problem 2.
      if a piece name duplicates a previous piece name, it is not
      sufficient to change only the second piece name, since that
      piece may have had prior references made to it.

resolution 2.
      a list of pieces is started using the name of each piece
      reference encountered in transcripts, genes, etc.
      it was decided to conform to a strict familial structure
      which requires that only one piece reference may be active
      at any given time; (i.e., before going on to a new piece
      reference, a piece for the existing reference must first be
      encountered).


consider the following structure:

transcript
* transcript1
* ...
* piece1


transcript
transcript
* transcript2
* ...
* piece1


piece
* piece1

piece
piece
* piece1


      note that if strict family ordering was not observed, it would
      be impossible to tell which piece each of the transcripts made
      a reference to.

TO COMPILE:

gcc catal.c -o catal -I/home/mplace/bin/p2c/src -L /home/mplace/bin/p2c/src -lm -lp2c

TO RUN:
 catal -f sample.csv

 sample text file looks like:

l1=l1
l2=l2
l3=l3
cat1=cat1
cat2=cat2
cat3=cat3
humcat=humcat
catin=catin
lib1=lib1
lib2=lib2
lib3=lib3
catalp=catalp


*/

#include <getopt.h>  /* getopt API */ 
#include <stdio.h> /* printf */
#include <stdlib.h> 
#include </home/mplace/bin/p2c/src/p2c.h>

#define version         9.64      
#define debugging       false  /* flag for selective writes  */
#define verbose         false  /* control for all those long lists produced by the dumplists procedure */
#define namelength      200    /* maximum key name length */   
#define linelength      210    /* maximum line readable in the library */
#define namespace       15     /* maximum length for printing name on humcat */
#define numlibfil       3      /* number of library files lll */
#define numcatfil       3      /* number of catalog files ccc */
#define pagesize        60     /* page size for human catalog */
#define listingwidth    60     /* humcat listing width */
#define specialchar     '*'    /* separator between original part of a name and part added for uniqueness */
#define levelsize       1      /* the number of spaces to indent for a level of the library */
#define datafield       110    /* the last character before fields of data are printed */
#define bfield          10     /* for basepairs */
#define cfield          9      /* for coordinates */
#define dfield          5      /* for directions */
#define nfield          8      /* for numbers */
#define firstnumber     '2'    /* when a duplicate name is found the first time,
         this number is tacked on to the end.  the number '2' is recommended
         because it is the second name found. */
#define datetimearraylength  19 /* length of dataarray for dates,
It is just long enough to include the 4 digit year - solving the  year 2000 problem:
1980/06/09 18:49:11
123456789 123456789 
         1         2
*/
typedef Char datetimearray[datetimearraylength];
typedef Char alpha[namelength];

/* name is a left justified string with blanks following the characters */
typedef struct name {
  alpha letters;
  uchar length;
} name;

/* catalog types */
typedef enum {
  firstpage, chromosome, dna, enzyme, gene, library, marker, organism, piece,
  recognitionclass, transcript
} calltype;

/* routines which may invoke a non-fatal error */
typedef enum {
  cooconfigurationbad, coordirectionbad, coordbeginningbad, coordendingbad,
  pieconfigurationbad, piedirectionbad, piebeginningbad, pieendingbad,
  refdirectionbad, refbeginningbad, refendingbad, genebeginning, geneending,
  mapbeginning, maplocation, nolastpiece, noreference, wrongreference
} errtype;

/* an item in the catalog */
typedef struct item {
  Char letter; /* type of structure */
  name nam;    /* the structure"s key name */
  long line;   /* location of the structure in the library */
} item;

typedef struct catfile {
  FILE *f;
  FILEBUFNC(f,item);
  Char name[_FNSIZE];
} catfile;

/* types defined for the catalog program */
typedef Char buffer[linelength];

//
typedef struct namandlistptr {
  name nam;
  struct namandlistptr *nextonlist;
} namandlistptr;

/* used for maintaining a list of names */
typedef struct currvals {
  namandlistptr marker_, transcript_, gene_, piece_, chromosome_, enzyme_,
		organism_, recognition;
} currvals;

typedef enum {
  plus, minus
} direction;

typedef enum {
  linear, circular
} configuration;

/* info about numbering of a piece */
typedef struct pieceinfo {
  configuration config;
  direction direct;
  long beginning, ending;
} pieceinfo;

typedef struct refnode {
  /* record containing information for a piece reference */
  enum {
    markref, transref, generef
  } nodetype;
  name nodename;
  direction nodedir;
  long nodebeg, nodeend;
  struct refnode *nodenext;
} refnode;

Static _TEXT catalp; /* catalog parameters.  used to decide how to handle dates see var keepdates */
Static boolean keepdates; /* false: advance dates in library
                             true:  do not advance dates since date of
                             creation is date of book creation by the
                             librarian. keepdates is set in initialize-
                             catalogs from catalp. */
/* write comments to the inst file */
Static boolean docomments, fromtoinst;
Static item catitem; 
/* catalog files ccc */
Static catfile cat1, cat2, cat3;
/* catalog number */
Static long catnumber;

Static _TEXT humcat; /* human readable catalog listing */ 
Static _TEXT catin;  /* catalog listing as delila instructions */
Static long humcatpage;  /* current page of listing */
Static long humcatlines; /* current line on that page */

/* values for names so far..first has pointers to lists of names for each classification. */
Static currvals current, first; 

/* a list of free links */
Static namandlistptr *freelistptr, *latest;

/* old library files lll */
Static _TEXT l1, l2, l3;

/* new library files lll */
Static _TEXT lib1, lib2, lib3;

/* set if old library file has hit eof */
Static boolean libdone;

/* current line in old library */
Static long libline;

/* number of current library */
Static long libnumber;

/* levels deep in our structure used for indentation purposes */
Static long level;

/* set to false if duplicate name in any one family is encountered.  */
Static boolean unique;

/*holds date and time in one variable. */
Static datetimearray daytime;

/* for clearing out names  */
Static name none;

/* a name which is pulled out of buffer */
Static name aname;

/* set if the name is too long to change */
Static boolean toolong;

/* buffer holding the current line  */
Static buffer line;

/* current length of the line */
Static long length_;

/* point where we start to change a name. */
Static long uniquebeg;

/* largest integer available on this machine*/
Static long maxinteger;

/* number of bases for the current piece */
Static long basecount;

/* name of currently referenced piece */
Static name piecename;

/*  set if the current piece name is now different */
Static boolean changed;

/*  new name of currently referenced piece */
Static name newpiecename;

/* set if the piece for the current reference has been found yet*/
Static boolean piecefound;

/*newname length for a changing name  */
Static long newnamelength;

/* info about the numbering system */
Static pieceinfo coo, pie;

/* reference information for a transcript, gene, or piece */
Static refnode pieceref;

/* root of piece reference list */
Static refnode *refroot;

/* a count of errors found that prevent making
   a new library, but not expected to snow ball */
Static long fatal;

/*fields involved in piece info.  see constants */
Static long pfield;

/* the number of blanks to get to the beginning of the data fields in humcat */
Static long todatafield;

/* the genetic map range, read by mapbegend */
Static double mapbeg, mapend;

/* false until we have written the title.
   This prevents multiple titles from being written to catin in
   procedure writehumcat.  */
Static boolean titlewritten;

/* maximum name length found */
Static long maxnamelength;

Static jmp_buf _JL1;

/* stop the program.  the procedure performs a goto to the end of the
   program.  you must have a label:
      label 1;
   declared, and also the end of the program must have this label:
      1: end.
   examples are in the module libraries.
   this is the only goto in the delila system. */
Static Void halt()
{
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}

/* write the name n to file f */
Static Void writename(f, n)
_TEXT *f;
name n;
{
  long i;

  for (i = 0; i < n.length; i++)
    putc(n.letters[i], f->f);
}

/* indicate to both humcat, catin and output that there was an error
in the current line of the library.  note that the actual libarary
line number is pred(line).  (crazy, i know.) */
Static Void erroratline()
{
  fprintf(humcat.f,
	  "\n error *********************************************\n");
  fprintf(humcat.f, " at line %ld in file %ld\n", libline - 1, libnumber);
  humcatlines += 3;
  printf(" *** error at line %ld in file %ld\n", libline - 1, libnumber);
  fprintf(catin.f, " *** error at line %ld in file %ld\n",
	  libline - 1, libnumber);
}


/* Local variables for error: */
struct LOC_error {
  errtype indicator;
} ;

/* put the message to the file, increment filelines */
Local Void message(thefile, filelines, LINK)
_TEXT *thefile;
long *filelines;
struct LOC_error *LINK;
{
  switch (LINK->indicator) {

  case mapbeginning:
    fprintf(thefile->f, " in map beginning\n");
    break;

  case maplocation:
    fprintf(thefile->f,
	    " genetic map location must be in the map [%5.2f, %5.2f]\n",
	    mapbeg, mapend);
    break;

  case cooconfigurationbad:
    fprintf(thefile->f,
	    " coordinate configuration must be  either linear or circular.\n");
    break;

  case coordirectionbad:
    fprintf(thefile->f, " coordinate direction must be a '+' or a '-'.\n");
    break;

  case coordbeginningbad:
    fprintf(thefile->f, " coordinate beginning must be an integer\n");
    break;

  case coordendingbad:
    fprintf(thefile->f, " coordinate ending must be an integer.\n");
    break;

  case pieconfigurationbad:
    fprintf(thefile->f,
	    " piece configuration must be either linear or circular.\n");
    break;

  case piedirectionbad:
    fprintf(thefile->f, " piece direction must be a '+' or a '-'.\n");
    break;

  case piebeginningbad:
    fprintf(thefile->f, " beginning piece coordinate must be an integer\n");
    break;

  case pieendingbad:
    fprintf(thefile->f, " ending piece coordinate must be an integer\n");
    break;

  case refdirectionbad:
    fprintf(thefile->f, " direction must be a '+' or a '-'.\n");
    break;

  case refbeginningbad:
    fprintf(thefile->f, " nucleotide beginning must be an integer.\n");
    break;

  case refendingbad:
    fprintf(thefile->f, " nucleotide ending must be an integer.\n");
    break;

  case genebeginning:
    fprintf(thefile->f, "in gene beginning \n");
    writename(thefile, current.gene_.nam);
    putc('\n', thefile->f);
    break;

  case geneending:
    fprintf(thefile->f, "in gene ending \n");
    writename(thefile, current.gene_.nam);
    putc('\n', thefile->f);
    break;

  case nolastpiece:
    fprintf(thefile->f, " error in family structure.\n");
    fprintf(thefile->f, " reference was made to a new piece: ");
    writename(thefile, aname);
    fprintf(thefile->f, "\n while a piece previously referenced: ");
    writename(thefile, piecename);
    fprintf(thefile->f, " was still unfound.\n");
    *filelines += 3;
    break;

  case noreference:
    fprintf(thefile->f,
	    " caution: no reference was previously made to this piece: ");
    writename(thefile, aname);
    putc('\n', thefile->f);
    printf(" (this is a warning: unreferenced piece)\n");
    (*filelines)++;
    break;

  case wrongreference:
    fprintf(thefile->f, " error in family structure.\n");
    fprintf(thefile->f, " piece ");
    writename(thefile, aname);
    fprintf(thefile->f, " was encountered while a reference to piece ");
    writename(thefile, piecename);
    fprintf(thefile->f, " was still active.\n");
    (*filelines)++;
    break;
  }

  if (((1L << ((long)LINK->indicator)) & ((1L << ((long)pieendingbad + 1)) -
	 (1L << ((long)cooconfigurationbad)))) != 0) {
    fprintf(thefile->f, " in piece ");
    writename(thefile, current.piece_.nam);
    putc('\n', thefile->f);
    (*filelines)++;
  }

  if (((1L << ((long)LINK->indicator)) &
       ((1L << ((long)refendingbad + 1)) - (1L << ((long)refdirectionbad)))) != 0) {
    fprintf(thefile->f, " error found in reference to piece ");
    writename(thefile, piecename);
    fprintf(thefile->f, "\n by ");
    switch (pieceref.nodetype) {

    case markref:
      fprintf(thefile->f, "marker");
      break;

    case generef:
      fprintf(thefile->f, "gene");
      break;

    case transref:
      fprintf(thefile->f, "transcript");
      break;
    }
    fprintf(thefile->f, " \n");
    writename(thefile, pieceref.nodename);
    putc('\n', thefile->f);

    *filelines += 2;
  }


  if (((1L << ((long)LINK->indicator)) & (1L << ((long)noreference))) == 0)
    fatal++;


  *filelines += 2;
  putc('\n', thefile->f);
}


/* this procedure flags errors in piece referencing by
   markers, transcripts, and genes. it also as a result
   checks the ordering of families, with the stipulation
   that all references to a piece must be made before the
   piece is found, with only one active forward reference
   at any given time. if no references are made to a piece
   by the time that piece is found, then a warning will be
   issued. all of these warnings are non-fatal, with
   the checking continuing after the flagged error.
      note: one line of error text is counted at the end of the procedure;
   additional lines are counted separately.
*/
Static Void error(indicator_)
errtype indicator_;
{
  struct LOC_error V;
  long dummy;

  V.indicator = indicator_;
  erroratline();
  message(&humcat, &humcatlines, &V);
  message(&catin, &dummy, &V);
}

/* give message on strange structure and abort */
Static Void strange(callingproc)
calltype callingproc;
{
  printf(" '%c' found at line %ld in library no. %ld where",
	 line[0], libline - 1, libnumber);
  switch (callingproc) {

  case firstpage:
    printf(" (firstpage)");
    break;

  case marker:
    printf(" an m");
    break;

  case transcript:
    printf(" a t");
    break;

  case gene:
    printf(" a g");
    break;

  case piece:
    printf(" a p");
    break;

  case chromosome:
    printf(" a c, m, t, g, or p");
    break;

  case enzyme:
    printf(" an e or an s");
    break;

  case recognitionclass:
    printf(" an r or an e");
    break;

  case organism:
    printf(" an o or a c");
    break;

  case library:
    printf(" an o or an r");
    break;

  case dna:
    printf(" a d");
    break;
  }
  printf(" was expected.\n");
  halt();
}

/* help the user */
Static Void docathelp(h)
_TEXT *h;
{
  fprintf(h->f, " All of the libraries were empty!\n");
  fprintf(h->f, " Usage of catal %4.2f:\n", version);
  fprintf(h->f,
    " catal(humcat,catalp,l1,c1,newl1,l2,c2,newl2,l3,c3,newc3,output)\n");
  fprintf(h->f,
	  " The catal program is used to check the format of one or more\n");
  fprintf(h->f, " libraries and to build catalogues corresponding to them.\n");
  fprintf(h->f, " It also produces a catalogue for humans, the humcat.\n");
  fprintf(h->f, " and Delila instructions in catin.\n");
  fprintf(h->f, " See delman for further help.\n");
}

/* write out each name in a list */
Static Void traversealist(the)
namandlistptr **the;
{
  _TEXT TEMP;

  if (*the == NULL)
    return;
  printf("  ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writename(&TEMP, (*the)->nam);
  putchar('\n');

  traversealist(&(*the)->nextonlist);  /* do the rest of the list */
}

/* dump all of the names on each list up to now for this family.*/
Static Void dumplists()
{
  if (!verbose)
    return;

  printf(" lists at time of error:\n");

  /* print out the list of recognition -classes, starting at the
      root of the list. this is done by the traversealist procedure.*/
  printf(" recognition-class names\n");
  traversealist(&first.recognition.nextonlist);

/* list of enzyme names for the current recognition-class */
  printf(" enzyme names for the current recognition-class\n");
  traversealist(&first.enzyme_.nextonlist);

/* list of organism names */
  printf(" organism names\n");
  traversealist(&first.organism_.nextonlist);

/* list of chromosome names for the current organism */
  printf(" chromosome names for the current organism\n");
  traversealist(&first.chromosome_.nextonlist);

/* list of markers for the current chromosome */
  printf(" marker names for the current chromosome\n");
  traversealist(&first.marker_.nextonlist);

/* lists of transcript names for current chromosome */
  printf(" transcript names for current chromosome\n");
  traversealist(&first.transcript_.nextonlist);

/* lists of gene names for current chromosome */
  printf(" gene names for the current chromosome\n");
  traversealist(&first.gene_.nextonlist);

/* lists of piece names for the current chromosome */
  printf(" piece names for the current chromosome\n");
  traversealist(&first.piece_.nextonlist);
}

/* get the date and time into a single array from the system clock.
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
definition of `TimeStamp', see section 8.255 TimeStamp. */
Static Void getdatetime(adatetime)
Char *adatetime;
{
  
  Char adate[datetimearraylength], atime[datetimearraylength];
  Char month[3];
  long index;

  VAXdate(adate);
  VAXtime(atime);

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
  /*

*/
  if (adatetime[4] == '/' && adatetime[7] == '/' && adatetime[13] == ':' &&
      adatetime[16] == ':')
    return;
  printf("readdatetime: bad date time read:\n");
  for (index = 0; index < datetimearraylength; index++)
    putchar(adatetime[index]);
  putchar('\n');
  halt();
}

/* Read the computer date and time.  Reverse the order of the digits and put
a decimal point in front.  This gives a fraction between zero and one that
varies quite quickly, and is always unique (if the computer has sufficient
accuracy).  It is to be used as a seed to a random number generator.  This
has the nice property that the seed changes every second and does not repeat
for thousands of years!  */
Static Void writedatetime(thefile, adatetime)
_TEXT *thefile;
Char *adatetime;
{
  long index;

  for (index = 0; index < datetimearraylength; index++)
    putc(adatetime[index], thefile->f);
}

/* add the digit represented by c to the seed at the power position */
Static Void addtoseed(seed, power, c)
double *seed, *power;
Char c;
{
  long n;
  *power /= 10;
  
  n = c - '0';
  if ((unsigned long)n > 9) {
    printf("timeseed: error in datetime\n");
    printf("it contains \"%c\" which is not a number.\n", c);
    printf("The getdatetime routine must be fixed.\n");
    halt();
  }
  *seed += *power * n;
}

/* convert adatetime to a real number in seed, reversed order
Here is the standard adatetime format:
123456789 123456789
         1         
1980/06/09 18:49:11 */
Static Void makeseed(adatetime, seed)
Char *adatetime;
double *seed;
{
  double power = 1.0;

  *seed = 0.0;
  addtoseed(seed, &power, adatetime[18]);
  addtoseed(seed, &power, adatetime[17]);

  addtoseed(seed, &power, adatetime[15]);
  addtoseed(seed, &power, adatetime[14]);

  addtoseed(seed, &power, adatetime[12]);
  addtoseed(seed, &power, adatetime[11]);

  addtoseed(seed, &power, adatetime[9]);
  addtoseed(seed, &power, adatetime[8]);

  addtoseed(seed, &power, adatetime[6]);
  addtoseed(seed, &power, adatetime[5]);

  addtoseed(seed, &power, adatetime[3]);
  addtoseed(seed, &power, adatetime[2]);
  addtoseed(seed, &power, adatetime[1]);
  addtoseed(seed, &power, adatetime[0]);
}

/* convert adatetime to a real number in seed, normal order */
Static Void orderseedDelila(adatetime, seed)
Char *adatetime;
double *seed;
{
  double power = 1.0;

  *seed = 0.0;
  addtoseed(seed, &power, adatetime[2]);
  addtoseed(seed, &power, adatetime[3]);
  addtoseed(seed, &power, adatetime[5]);
  addtoseed(seed, &power, adatetime[6]);

  addtoseed(seed, &power, adatetime[8]);
  addtoseed(seed, &power, adatetime[9]);

  addtoseed(seed, &power, adatetime[11]);
  addtoseed(seed, &power, adatetime[12]);

  addtoseed(seed, &power, adatetime[14]);
  addtoseed(seed, &power, adatetime[15]);

  addtoseed(seed, &power, adatetime[17]);
  addtoseed(seed, &power, adatetime[18]);
}

/* read the computer date and time.  reverse the order of the digits
and put a decimal point in front.  this gives a fraction between
zero and one that varies quite quickly, and is always unique (if the
computer has sufficient accuracy).  it is to be used as a seed to
a random number generator. */
Static Void timeseed(seed)
double *seed;
{
  datetimearray adatetime;
  getdatetime(adatetime);
  makeseed(adatetime, seed);
}


/* test whether the current time is before the limit.
If it is later, halt the program */
Static Void limitdate(a, b, c, d, limitdatetime_)
Char a, b, c, d;
Char *limitdatetime_;
{
  /*
*/
  datetimearray limitdatetime, adatetime;
  double Dday; /* the critical day */
  double now;  /* this very moment */

  memcpy(limitdatetime, limitdatetime_, sizeof(datetimearray));
  getdatetime(adatetime);

  orderseedDelila(adatetime, &now);

  if (limitdatetime[0] != ' ' || limitdatetime[1] != ' ' ||
      limitdatetime[2] != ' ' || limitdatetime[3] != ' ')
    halt();

  limitdatetime[0] = a;
  limitdatetime[1] = b;
  limitdatetime[2] = c;
  limitdatetime[3] = d;

  orderseedDelila(limitdatetime, &Dday);

  if (now <= Dday)
    return;
  printf("This program expired on %.*s\n", datetimearraylength, limitdatetime);
  printf("See: https://alum.mit.edu/www/toms/walker/contacts.html\n");
  halt();
}

/* this procedure calculates the largest real number
  possible on this machine.
   this procedure assumes the range of numbers is somewhat
  balanced to either side of 1 (i.e., 2^-n< 1 < 2^n , approximately )
  The routine doesn't work too well with using 1/lastsmall because
  dividing by such a small number produces infinity on a Sun workstation.
  Using the small number before that one (prelastsmall) avoids the
  difficulty.  HA!  NO IT DOESN'T!!!  Even preprelastsmall still
  causes overflow.  Drop it!  Who cares? */
Static Void maxnum(maxreal)
double *maxreal;
{
  double small = 1.0;      /* a small real number */
  double lastsmall;        /* the small real number before small */
  double prelastsmall;     /* the small real number before lastsmall */
  double preprelastsmall;  /* the small real number before prelastsmall */

  while (small != 0) {
    preprelastsmall = prelastsmall;
    prelastsmall = lastsmall;
    lastsmall = small;
    small /= 10;
    printf("maxnum: small = % .3E\n", small);
  }
  *maxreal = 1 / preprelastsmall;
  printf("maxnum: lastsmall = % .3E\n", lastsmall);
  printf("maxnum: prelastsmall = % .3E\n", prelastsmall);
  printf("maxnum: preprelastsmall = % .3E\n", preprelastsmall);
  printf("maxnum: maxreal = % .3E\n", *maxreal);
}

/* copy a line from file fin to file fout */
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

/* report the parantage of the library (if it exists) */
Static Void reportparent(l)
_TEXT *l;
{
  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "r", l->f);
    else
      l->f = fopen(l->name, "r");
  } else
    rewind(l->f);
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  RESETBUF(l->f, Char);
  if (BUFEOF(l->f))
    return;
  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "r", l->f);
    else
      l->f = fopen(l->name, "r");
  } else
    rewind(l->f);
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  RESETBUF(l->f, Char);
  copyaline(l, &humcat);
  humcatlines++;

  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "r", l->f);
    else
      l->f = fopen(l->name, "r");
  } else
    rewind(l->f);
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  RESETBUF(l->f, Char);
  copyaline(l, &catin);
  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "r", l->f);
    else
      l->f = fopen(l->name, "r");
  } else
    rewind(l->f);
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  RESETBUF(l->f, Char);
}

/* provide 0 if the library is empty and 1 if it is not */
Static long countlibrary(l)
_TEXT *l;
{
  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "r", l->f);
    else
      l->f = fopen(l->name, "r");
  } else
    rewind(l->f);
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  RESETBUF(l->f, Char);
  if (BUFEOF(l->f))
    return 0;
  else
    return 1;
}


Static Void dopage()
{
  if (humcatpage != 0) {
    fprintf(humcat.f, "\f");
    humcatlines = 0;
  }
  humcatpage++;
  fprintf(humcat.f, "catal %4.2f", version);
  fprintf(humcat.f, " - Catalogue listing of ");
  writedatetime(&humcat, daytime);
  fprintf(humcat.f, " page %ld\n", humcatpage);
  if (titlewritten)
    return;
  fprintf(catin.f, "title \"Delila instructions for library ");
  writedatetime(&catin, daytime);
  fprintf(catin.f, "\";\n");
  fprintf(catin.f, "(* catal %4.2f *)\n", version);
  titlewritten = true;
}


Static Void initializecatalogs(catalp, c1, c2, c3)
_TEXT *catalp;
catfile *c1, *c2, *c3;
{
  long i;
  Char ch;  /* reading character */

  if (*catalp->name != '\0') {
    if (catalp->f != NULL)
      catalp->f = freopen(catalp->name, "r", catalp->f);
    else
      catalp->f = fopen(catalp->name, "r");
  } else
    rewind(catalp->f);
  if (catalp->f == NULL)
    _EscIO2(FileNotFound, catalp->name);
  RESETBUF(catalp->f, Char);
  /*

*/
  docomments = true;
  fromtoinst = false;

  if (BUFEOF(catalp->f))
    keepdates = false;
  else {
    fscanf(catalp->f, "%c%*[^\n]", &ch);
    getc(catalp->f);
    if (ch == '\n')
      ch = ' ';
    if (ch == 'b' || ch == 'n')
      keepdates = true;
    else
      keepdates = false;

    if (!BUFEOF(catalp->f)) {
      if (P_peek(catalp->f) == 'n')
	docomments = false;
      fscanf(catalp->f, "%*[^\n]");
      getc(catalp->f);
    }

    if (!BUFEOF(catalp->f)) {
      if (P_peek(catalp->f) == 'f')
	fromtoinst = true;
      fscanf(catalp->f, "%*[^\n]");
      getc(catalp->f);
    }

  }
  printf(" library dates ");
  if (keepdates)
    printf("kept\n");
  else
    printf("advanced\n");
  if (*c1->name != '\0') {
    if (c1->f != NULL)
      c1->f = freopen(c1->name, "wb", c1->f);
    else
      c1->f = fopen(c1->name, "wb");
  } else {
    if (c1->f != NULL) {


      rewind(c1->f);
    } else
      c1->f = tmpfile();
  }
  if (c1->f == NULL)
    _EscIO2(FileNotFound, c1->name);
  SETUPBUF(c1->f, item);
  if (*c2->name != '\0') {
    if (c2->f != NULL)
      c2->f = freopen(c2->name, "wb", c2->f);
    else
      c2->f = fopen(c2->name, "wb");
  } else {
    if (c2->f != NULL)
      rewind(c2->f);
    else
      c2->f = tmpfile();
  }
  if (c2->f == NULL)
    _EscIO2(FileNotFound, c2->name);
  SETUPBUF(c2->f, item);
  if (*c3->name != '\0') {
    if (c3->f != NULL)
      c3->f = freopen(c3->name, "wb", c3->f);
    else
      c3->f = fopen(c3->name, "wb");
  } else {
    if (c3->f != NULL)
      rewind(c3->f);
    else
      c3->f = tmpfile();
  }
  if (c3->f == NULL)
    _EscIO2(FileNotFound, c3->name);
  SETUPBUF(c3->f, item);

  if (*humcat.name != '\0') {
    if (humcat.f != NULL)
      humcat.f = freopen(humcat.name, "w", humcat.f);
    else
      humcat.f = fopen(humcat.name, "w");
  } else {
    if (humcat.f != NULL)
      rewind(humcat.f);
    else
      humcat.f = tmpfile();
  }
  if (humcat.f == NULL)
    _EscIO2(FileNotFound, humcat.name);
  SETUPBUF(humcat.f, Char);
  if (*catin.name != '\0') {
    if (catin.f != NULL)
      catin.f = freopen(catin.name, "w", catin.f);
    else
      catin.f = fopen(catin.name, "w");
  } else {
    if (catin.f != NULL)
      rewind(catin.f);
    else
      catin.f = tmpfile();
  }
  if (catin.f == NULL)
    _EscIO2(FileNotFound, catin.name);
  SETUPBUF(catin.f, Char);
  humcatlines = 0;
  humcatpage = 0;

  titlewritten = false;
  dopage();

  putc('\n', humcat.f);
  humcatlines++;
  fprintf(humcat.f, "parent librar");
  if (countlibrary(&l1) + countlibrary(&l2) + countlibrary(&l3) > 1)
    fprintf(humcat.f, "ies");
  else
    putc('y', humcat.f);
  fprintf(humcat.f, ": \n");
  humcatlines++;
  if (docomments) {
    fprintf(catin.f, "(* parent library: \n");
    reportparent(&l1);
    reportparent(&l2);
    reportparent(&l3);
    fprintf(catin.f, " *)\n");
  }

  libnumber = 1;
  catnumber = 1;
  for (i = 0; i < namelength; i++)
    none.letters[i] = ' ';
  piecename = none;
  piecefound = false;
  refroot = NULL;

  current.marker_.nam = none;
  current.transcript_.nam = none;
  current.gene_.nam = none;
  current.piece_.nam = none;
  current.chromosome_.nam = none;
  current.enzyme_.nam = none;
  current.recognition.nam = none;
  current.organism_.nam = none;
  freelistptr = NULL;
/* reset the pointers to the lists of names to nil */
  first.marker_.nextonlist = NULL;
  first.transcript_.nextonlist = NULL;
  first.gene_.nextonlist = NULL;
  first.piece_.nextonlist = NULL;
  first.chromosome_.nextonlist = NULL;
  first.enzyme_.nextonlist = NULL;
  first.recognition.nextonlist = NULL;
  first.organism_.nextonlist = NULL;

  todatafield = namespace;

/* these are the fields of a coordinate system half */
  pfield = cfield + dfield + nfield * 2;

/* we will start this enterprise with an open mind: */
  fatal = 0;

}

/* dump a line of length i to the new library.  if the line would be only
 one character long (just an '*'), then delila will have problems
 because it assumes that there is a space following each '*'.
 rather than slow down delila, we check for this case and add
 a space if there is none. */
Static Void dumpline(newlib, i)
_TEXT *newlib;
long i;
{
  long j;

  if (libdone)
    return;

  for (j = 0; j < i; j++)
    putc(line[j], newlib->f);
  /*
*/
  if (i == 1)
    putc(' ', newlib->f);
  putc('\n', newlib->f);
}

/* dump an item to a cat */
Static Void dumpitem(cat, it)
catfile *cat;
item it;
{
  fwrite(&it, sizeof(item), 1, cat->f);
}

/* read a line from the library into the global 'line' */
Static Void readline(lib, length)
_TEXT *lib;
long *length;
{
  long j;
  Char uline[linelength];
  /* update line number at this point so that if there is an error,
     the correct line number will be given... */
  libline++;
  *length = 0;

  if (BUFEOF(lib->f)) {
    libdone = true;
    return;
  }
  while (!P_eoln(lib->f) && *length < linelength) {
    (*length)++;
    uline[*length - 1] = getc(lib->f);
    if (uline[*length - 1] == '\n')
      uline[*length - 1] = ' ';
  }

  memcpy(line, uline, sizeof(buffer));

/* fill the rest of the line buffer with blanks */
  for (j = *length; j < linelength; j++)
    line[j] = ' ';

  if (!P_eoln(lib->f)) {
    erroratline();
    fprintf(humcat.f, " the line is longer than %ld characters\n",
	    (long)linelength);
    putc(' ', humcat.f);

    for (j = 0; j < linelength; j++)
      putc(line[j], humcat.f);
    putc('\n', humcat.f);
    humcatlines += 2;

    if (docomments) {
      fprintf(catin.f, " the line is longer than %ld characters\n",
	      (long)linelength);
      putc(' ', catin.f);
      for (j = 0; j < linelength; j++)
	putc(line[j], catin.f);
      putc('\n', catin.f);
    }

    fatal++;
  }

  if (*length == 0) {
    erroratline();
    fprintf(humcat.f, " blank line in library\n");
    fprintf(catin.f, " blank line in library\n");
    humcatlines++;
    fatal++;
  }
  fscanf(lib->f, "%*[^\n]");
  getc(lib->f);
}


/*procedure needline is called when it is imperative that we get a new
      line, but do not want to dump it to the new library yet. */
Static Void needline(lib, length)
_TEXT *lib;
long *length;
{
  readline(lib, length);
  if (!libdone)
    return;
  erroratline();
  printf("premature end of library file\n");
  halt();
}

/* needlibline is called when it is imperative that we get a line.
   it is then dumped to the catalog.  */
Static Void needlibline(lib, newlib, length)
_TEXT *lib, *newlib;
long *length;
{
  needline(lib, length);
  dumpline(newlib, *length);
}

/* check the star at the start of each line using the global 'line'.
If checkattribute is true, then not check for the existance of the attribute,
otherwise let it slide.  That allows one to use the Delila <NAME> function so
that unnamed objects get a blank name. */
Static Void checkstar(checkattribute)
boolean checkattribute;
{
  if (length_ < 3) {
    if (!checkattribute)
      return;
    erroratline();
    fprintf(humcat.f, " attribute is missing\n");
    fprintf(catin.f, " attribute is missing\n");
    fatal++;
    humcatlines++;
    return;
  }
  if (line[0] != '*') {
    erroratline();
    fprintf(humcat.f, " asterisk (*) missing from start of line\n");
    fprintf(catin.f, " asterisk (*) missing from start of line\n");
    fatal++;
    humcatlines++;
  }

  if (line[1] == ' ')
    return;
  erroratline();
  fprintf(humcat.f, " blank missing from start of line\n");
  fprintf(catin.f, " blank missing from start of line\n");
  fatal++;
  humcatlines++;
}

 /** skip blanks on the global line, after the '* '.  this procedure is
absolutely required because some compilers (eg:
      digital vax/vms 3.0 pascal
but not
      dec cyber pascal
or    ibm personal computer pascal )
will put an extra blank in front of real numbers (but not integers...).
this happens even when one writes with a field size 1 (as re:1:2).
programs that write books (eg. makebk) therefore make bad books if catal
does not accept the blanks... subtle and silly.  */
Static Void skipblanks(linepos, bad)
long *linepos;
boolean *bad;
{
  *linepos = 3;

  while (*linepos <= length_ && line[*linepos - 1] == ' ')
    (*linepos)++;

  *bad = (*linepos > length_);
}

/* this procedure pulls an integer off the current input line
  and returns its value if valid. if a valid integer does not exist,
  badtoken returns a value true. otherwise, badtoken is false.
*/
Static Void getinteger(lib, newlib, intnumber, badtoken)
_TEXT *lib, *newlib;
long *intnumber;
boolean *badtoken;
{

  long subtotal, linepos, thesign;

  needlibline(lib, newlib, &length_);
  checkstar(true);
  skipblanks(&linepos, badtoken);
  if (!*badtoken) {
    subtotal = 0;
    if (line[linepos-1] == '-' || line[linepos-1] == '+') {
      if (line[linepos-1] == '-')
	thesign = -1;
      else
	thesign = 1;
      linepos++;
    } else
      thesign = 1;
    while (linepos <= length_ && isdigit(line[linepos-1]) &&
	   subtotal < LONG_MAX / 10 - 10) {
      switch (line[linepos-1]) {

      case '0':
	subtotal *= 10;
	break;

      case '1':
	subtotal = subtotal * 10 + 1;
	break;

      case '2':
	subtotal = subtotal * 10 + 2;
	break;

      case '3':
	subtotal = subtotal * 10 + 3;
	break;

      case '4':
	subtotal = subtotal * 10 + 4;
	break;

      case '5':
	subtotal = subtotal * 10 + 5;
	break;

      case '6':
	subtotal = subtotal * 10 + 6;
	break;

      case '7':
	subtotal = subtotal * 10 + 7;
	break;

      case '8':
	subtotal = subtotal * 10 + 8;
	break;

      case '9':
	subtotal = subtotal * 10 + 9;
	break;
      }
      linepos++;
    }
    if (linepos <= length_ && line[linepos-1] != ' ')
      *badtoken = true;
    else
      *intnumber = subtotal;

    *intnumber *= thesign;
  }
  if (*badtoken)
    *intnumber = 0;
}

/* this procedure extracts a real number from the current input
  line, if one exists.if so, on exit, realnumber is the value
  of that real number;if not, badtoken is set to true.
*/
Static Void getreal(lib, newlib, realnumber, badtoken)
_TEXT *lib, *newlib;
double *realnumber; 
boolean *badtoken;
{
  double subtotal;    /* total for the token so far */
  long linepos;       /* position on the input line */
  boolean pointfound; /* set if a decimal point has been found yet */
  double multiplier;  /* multiplier for this point in token */
  long thesign;       /* multiplier for sign */

  needlibline(lib, newlib, &length_);
  checkstar(true);
  skipblanks(&linepos, badtoken);
  if (!*badtoken) {
    multiplier = 1.0;
    pointfound = false;
    subtotal = 0.0;
    if (line[linepos-1] == '-' || line[linepos-1] == '+') {
      if (line[linepos-1] == '-')
	thesign = -1;
      else
	thesign = 1;
      linepos++;
    } else
      thesign = 1;

    while (linepos <= length_ &&
	   (line[linepos-1] == '.' || isdigit(line[linepos-1]))) {
      /*

*/
      if (line[linepos-1] == '.') {
	if (pointfound)
	  *badtoken = true;
	else
	  pointfound = true;
      } else {
	if (pointfound)
	  multiplier /= 10;
	else
	  subtotal *= 10;

	switch (line[linepos-1]) {

	case '0':
	  /* blank case */
	  break;

	case '1':
	  subtotal += multiplier;
	  break;

	case '2':
	  subtotal += multiplier * 2;
	  break;

	case '3':
	  subtotal += multiplier * 3;
	  break;

	case '4':
	  subtotal += multiplier * 4;
	  break;

	case '5':
	  subtotal += multiplier * 5;
	  break;

	case '6':
	  subtotal += multiplier * 6;
	  break;

	case '7':
	  subtotal += multiplier * 7;
	  break;

	case '8':
	  subtotal += multiplier * 8;
	  break;

	case '9':
	  subtotal += multiplier * 9;
	  break;
	}
      }
      linepos++;
    }
    if (linepos <= length_ && line[linepos-1] != ' ' || *badtoken)
      *badtoken = true;
    else
      *realnumber = subtotal;

    *realnumber = thesign * *realnumber;
  }

  if (*badtoken)
    *realnumber = 0.0;
}

/* obtain a configuration and the line buffer */
Static Void getconfig(lib, newlib, c, badtoken)
_TEXT *lib, *newlib;
configuration *c;
boolean *badtoken;
{
  needlibline(lib, newlib, &length_);
  checkstar(true);
  *badtoken = false;
  if (line[2] == 'c') {
    *c = circular;
    return;
  }
  if (line[2] == 'l')
    *c = linear;
  else
    *badtoken = true;
}

/* obtain a configuration from the line buffer */
Static Void getdirect(lib, newlib, d, badtoken)
_TEXT *lib, *newlib;
direction *d;
boolean *badtoken;
{
  needlibline(lib, newlib, &length_);
  checkstar(true);
  *badtoken = false;
  if (line[2] == '-') {
    *d = minus;
    return;
  }
  if (line[2] == '+')
    *d = plus;
  else
    *badtoken = true;
}

/* pull a name out of the line buffer,  note that no needlibline is done, because of the way getname
      is used in changename.  changename is used in a loop in
      procedure duplicate... */
Static Void getname()
{
  long i = 1;
  long j;

  checkstar(true);
  /* skip star and space in first two positions */
  while (i <= namelength && line[i+1] != ' ') {
    aname.letters[i-1] = line[i+1];
    i++;
  }

  aname.length = i - 1;
  if (maxnamelength < aname.length)
    maxnamelength = aname.length;
  j = i;
  while (j <= namelength) {
    aname.letters[j-1] = ' '; /* fill character must be space to avoid trouble with delila */
    j++;
  }

  catitem.nam.length = i - 1;
}

/* read a date into the first catalog and update the library date */
Static Void readlibdate(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  Char ch; /* a reading character */
  long i;  /* index for transfering the date to the name */
  datetimearray olddaytime;
  _TEXT TEMP;
  long FORLIM;
/* readlibdate */
/* insert date in the new library */
  fprintf(newlib->f, "* ");
  ch = getc(lib->f);
  ch = getc(lib->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == '\n')
    ch = ' ';

  /* pick up daytime from library itself 
    readdatetime(lib, daytime);
    put daytime right back ... 
    writedatetime(newlib, daytime);
    and copy rest of line (below) */
  if (keepdates) {
    readdatetime(lib, daytime);

    writedatetime(newlib, daytime);

  }
  else {
    writedatetime(newlib, daytime); /* new date */
    fprintf(newlib->f, ", "); /* copy old date into second position in newlib */
    readdatetime(lib, olddaytime);
    writedatetime(newlib, olddaytime);

  /* skip second date of oldlib */
    ch = getc(lib->f);
    ch = getc(lib->f);
    if (ch == '\n')
      ch = ' ';
    if (ch == '\n')
      ch = ' ';
    readdatetime(lib, olddaytime);
  }

/* copy name of library (or rest of header if keepdates = true) */
  while (!P_eoln(lib->f)) {
    ch = getc(lib->f);
    if (ch == '\n')
      ch = ' ';
    putc(ch, newlib->f);
  }
  putc('\n', newlib->f);

  printf("\n library %ld date will be: ", libnumber);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writedatetime(&TEMP, daytime);
  putchar('\n');

/* set up the first item of the catalog */
  catitem.letter = '*';
  catitem.nam.length = datetimearraylength;
  FORLIM = catitem.nam.length;

  for (i = 0; i < FORLIM; i++)
    catitem.nam.letters[i] = daytime[i];

/* clear the rest of the array */
  for (i = catitem.nam.length; i < namelength; i++)
    catitem.nam.letters[i] = ' ';

  catitem.line = libline;

  /* insert date in the catalog */
  switch (catnumber) {
  case 1:
    dumpitem(c1, catitem);
    break;
  case 2:
    dumpitem(c2, catitem);
    break;
  case 3:
    dumpitem(c3, catitem);
    break;
  }
/* complete skip of library header line */
  fscanf(lib->f, "%*[^\n]");
  getc(lib->f);
  libline++;
}

/* catalog writing procedures *****************************************
 dump an item to the cat
*/

Static Void dumptocat(chr, c1, c2, c3)
Char chr;
catfile *c1, *c2, *c3;
{
/* reason for use of libline-2:
      we are already 2 lines ahead of the first line of the item.
      example:
      ...
      organism             libline-2
      * name               libline-1
      * ...                libline
      ^ is the next character to be read, but the organism
      starts at libline-2. */
  memcpy(catitem.nam.letters, aname.letters, sizeof(alpha));  /* the nam.length was set in procedure getname */

  catitem.letter = chr;
  catitem.line = libline - 2;
  switch (catnumber) {
    case 1:
      dumpitem(c1, catitem);
      break;
    case 2:
      dumpitem(c2, catitem);
      break;
    case 3:
      dumpitem(c3, catitem);
      break;
  }
}

/* write the name n without blanks */
Static Void shortname(afile, n)
_TEXT *afile;
name n;
{
  long i = 1;

  while (n.letters[i-1] != ' ') {
    putc(n.letters[i-1], afile->f);
    i++;
  }
}

/* write out a nice human listing */
Static Void writehumcat(callingproc)
calltype callingproc;
{
  long fieldsize = 11;   /* size to put a name or spaces in */
  long l;                /* index to levelsize */ 
  long FORLIM;

  putc('\n', humcat.f);
  humcatlines++;
  /*  doing this as a carriage return of the previous line allows
      many things to be put on each line of cat after the call to
      humcat.  note that the last carriage return is done at the end
      of the program. */
  if (humcatlines >= pagesize) /* paging control */
    dopage();


  FORLIM = levelsize * level;
  for (l = 1; l <= FORLIM; l++)
    putc(' ', humcat.f);
  
/* note: each part in quotes is 17 characters */
  switch (callingproc) {
    case organism:
      fprintf(humcat.f, "organism ");
      writename(&humcat, current.organism_.nam);
      fprintf(humcat.f, "%*c", namespace - current.organism_.nam.length, ' ');
      break;
    case chromosome:
      fprintf(humcat.f, "chromosome ");
      writename(&humcat, current.chromosome_.nam);
      fprintf(humcat.f, "%*c", namespace - current.chromosome_.nam.length, ' ');
      break;
    case marker:
      fprintf(humcat.f, "marker ");
      writename(&humcat, current.marker_.nam);
      fprintf(humcat.f, "%*c", namespace - current.marker_.nam.length, ' ');
      break;
    case transcript:
      fprintf(humcat.f, "transcript ");
      writename(&humcat, current.transcript_.nam);
      fprintf(humcat.f, "%*c", namespace - current.transcript_.nam.length, ' ');
      break;
    case gene:
      fprintf(humcat.f, "gene ");
      writename(&humcat, current.gene_.nam);
      fprintf(humcat.f, "%*c", namespace - current.gene_.nam.length, ' ');
      break;
    case piece:
      fprintf(humcat.f, "piece ");
      writename(&humcat, current.piece_.nam);
      fprintf(humcat.f, "%*c", namespace - current.piece_.nam.length, ' ');
      break;
    case recognitionclass:
      fprintf(humcat.f, "recognition-class ");
      writename(&humcat, current.recognition.nam);
      fprintf(humcat.f, "%*c", namespace - current.recognition.nam.length, ' ');
      break;
    case enzyme:
      fprintf(humcat.f, "enzyme ");
      writename(&humcat, current.enzyme_.nam);
      fprintf(humcat.f, "%*c", namespace - current.enzyme_.nam.length, ' ');
      break;

  }
/* old: we are now 28 + namelength characters over... 
   do the instructions */
  if (docomments)
    putc('\n', catin.f);
  switch (callingproc) {
    case organism:
      fprintf(catin.f, "organism ");
      writename(&catin, current.organism_.nam);
      break;
    case chromosome:
      fprintf(catin.f, "chromosome ");
      writename(&catin, current.chromosome_.nam);
      break;
    case marker:
      fprintf(catin.f, "marker ");
      writename(&catin, current.marker_.nam);
      break;
    case transcript:
      fprintf(catin.f, "transcript ");
      writename(&catin, current.transcript_.nam);
      break;
    case gene:
      fprintf(catin.f, "gene ");
      writename(&catin, current.gene_.nam);
      break;
    case piece:
      fprintf(catin.f, "piece ");
      writename(&catin, current.piece_.nam);
      break;
    case recognitionclass:
      fprintf(catin.f, "recognition-class");
      writename(&catin, current.recognition.nam);
      break;
    case enzyme:
      fprintf(catin.f, "enzyme ");
      writename(&catin, current.enzyme_.nam);
      break;
    }
    fprintf(catin.f, ";\n");
}

/* obtain a listptr */
Static Void allocate(last)
namandlistptr **last;
{
  if (freelistptr != NULL) {
    *last = freelistptr;
    freelistptr = freelistptr->nextonlist;
  } else
    *last = (namandlistptr *)Malloc(sizeof(namandlistptr));

  (*last)->nam = none;
  (*last)->nextonlist = NULL;
}

/* return a listptr to the free pool */
Static Void return_(node)
namandlistptr **node;
{
  namandlistptr *lptr; /* temporary ptr for holding the lists */

  if (*node == NULL)
    return;
  lptr = *node;
  *node = (*node)->nextonlist;
  lptr->nextonlist = freelistptr;
  freelistptr = lptr;
}

/* compare the two names.
2004 July 8: Code stolen from module equalstring in prgmod.p */
Static boolean equalname(a, b)
name *a, *b;
{
  boolean equal; /* true if the two are equal */
  long index;    /* counter for the names */

  if (a->length == b->length) {
    index = 1;
    do {
      equal = (a->letters[index-1] == b->letters[index-1]);
      index++;
    } while (equal && index <= a->length);
    return equal;
  } else
    return false;
}

/* put a name on a list */
Static Void pushname(the, latest)
namandlistptr **the, **latest;
{
  if (*the == NULL) {   /* no names on here */
    allocate(the);
    (*the)->nam = aname;
    (*the)->nextonlist = NULL;
    *latest = *the;
    return;
  }
  /* check for duplicate and go deeper */
  if (equalname(&(*the)->nam, &aname))
    unique = false;
  else
    pushname(&(*the)->nextonlist, latest);
}

/* clear a list completely */
Static Void popnames(the)
namandlistptr **the;
{
  if (*the == NULL)
    return;
  while ((*the)->nextonlist != NULL)
    popnames(&(*the)->nextonlist);
  return_(the);
}

/* duplicate name-handling procedures*********************************/
/* Local variables for up: */
struct LOC_up {
  long endofnum;
} ;

/* procedure carry shifts a number right starting with the special
   character and going to the end of the number. it puts a '1'
   in the new location and updates the name and line lengths */
Local Void shift(endofnum)
long endofnum;  /*the right end of a generated number in the
    line buffer, in case shifting is needed to
    fit the number */
{
 
  long position, FORLIM;

  FORLIM = uniquebeg;
  for (position = endofnum; position > FORLIM; position--)
    line[position] = line[position-1];
  line[uniquebeg] = '1';

  catitem.nam.length++;
  length_++;
}

/* procedure addcarry sets a nine to a zero and propagates a carry
   if we get back to the top of the number and still need a carry
   addcarry will call a subroutine, shift, to shift the number
   in the line buffer. to propagate carries through the number,
   addcarry calls itself with successively lower numbers as
   subscripts for the line buffer. */
Local Void addcarry(position, LINK)
long position;
struct LOC_up *LINK;
{
  line[position-1] = '0';
  position--;
  if (line[position-1] == specialchar) {
    shift(LINK->endofnum);
    return;
  }
  switch (line[position-1]) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
      line[position-1]++;
      break;

    case '9':
      addcarry(position, LINK);
      break;
  }
}

/* procedure up increments a 'number' in character format and calls
      a carry routine if needed. */
Static Void up(position)
long position;
{
  struct LOC_up V;

  while (line[position-1] == ' ' && position > uniquebeg)
    position--;

  switch (line[position-1]) {
    case ' ':
      catitem.nam.length += 2;
      length_ += 2;
      line[position-1] = specialchar;
      position++;
      line[position-1] = firstnumber;
      break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
      line[position-1]++;
      break;

    case '9':
      V.endofnum = position;
      addcarry(V.endofnum, &V);
      break;
  }
}

/* procedure changename calls one subroutine, up, if a name
   can be changed. if it cannot, a message is printed out and
   the program is aborted. if it can, it is changed and procedure
   pushname is called to insert the new name on the list. if
   it is still non-unique, the main program will call changename
   until it aborts or puts the name on the list.
   this is required by libdef catalogue definition p. 3.1 2b */
Static Void changename(the, latest)
namandlistptr **the, **latest;
{
  toolong = false;
  if (line[namelength+1] == ' ' || isdigit(line[namelength+1]))
    up(namelength + 2L);
  else
    toolong = true;
  getname();
  if (catitem.nam.length > namelength)
    toolong = true;
  else
    newnamelength = catitem.nam.length;
  if (toolong) {
    printf("New generated name is too long.\n");
    printf("Change namelength constant.\n");
    printf("namelength = %ld\n", (long)namelength);
    printf("namelength = %ld\n", (long)namelength);
    halt();
  }

  unique = true;
  pushname(the, latest);
}

/*duplicate writes out a message saying a duplicate name was found then
  dumps the lists of names. it then makes the name unique and writes
  a message giving the old and new names.*/
Static Void duplicate(callingproc, root)
calltype callingproc;
namandlistptr **root;
{
   _TEXT TEMP;

  printf(" duplicate ");
  switch (callingproc) {
    case organism:
      printf("organism");
      break;
    case recognitionclass:
      printf("recognition-class");
      break;
    case enzyme:
      printf("enzyme");
      break;
    case chromosome:
      printf("chromosome");
      break;
    case marker:
      printf("marker");
      break;
    case transcript:
      printf("transcript");
      break;
    case gene:
      printf("gene");
      break;
    case piece:
      printf("piece");
      break;
  }
  printf(" name found.\n");
  dumplists();

  printf(" old name was ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writename(&TEMP, aname);
  putchar('\n');
  uniquebeg = catitem.nam.length + 3;
/* star, space, then to the other side of the name */
  do {
    changename(root, &latest);
  } while (!unique);
  printf(" new name is  ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writename(&TEMP, latest->nam);
  putchar('\n');
}

/* changes a name in the current line to an already known new name. */
Static Void changeto(newname)
name newname;
{
  long i;
  long difference; /* difference in length between old new names */
  long FORLIM;
  getname();
  difference = newnamelength - catitem.nam.length;
  length_ += difference; /* note: we know at this time that the new name
                            is at least as long as the old name */
  FORLIM = length_ - catitem.nam.length - 2;

  for (i = 0; i <= FORLIM; i++)
    line[length_ - i - 1] = line[length_ - i - difference - 1];
  FORLIM = newnamelength;
  for (i = 1; i <= FORLIM; i++)
    line[i+1] = newname.letters[i-1];
 
  getname();
}

/* write the direction d to the file f */
Static Void writedirect(f, d)
_TEXT *f;
direction d;
{
  switch (d) {
    case minus:
      fprintf(f->f, "%*s", dfield, "- ");
      break;
    case plus:
      fprintf(f->f, "%*s", dfield, "+ ");
      break;
  }
}

/* write the configuration c to the file f */
Static Void writeconfig(f, c)
_TEXT *f;
configuration c;
{
  switch (c) {
    case linear:
      fprintf(f->f, "%*s", cfield, "   linear");
      break;
    case circular:
      fprintf(f->f, "%*s", cfield, " circular");
      break;
  }
}

/* write the piece information to f */
Static Void writepieceinfo(f, p)
_TEXT *f;
pieceinfo p;
{
  writeconfig(f, p.config);
  writedirect(f, p.direct);
  fprintf(f->f, " %*ld", nfield, p.beginning);
  fprintf(f->f, " %*ld", nfield, p.ending);
}

/* check that the location is in the map.  this is required by libdef
catalogue definition p. 3.1, 2c */
Static Void checkmap(geneticlocation)
double geneticlocation;
{
  if (geneticlocation < mapbeg || mapend < geneticlocation)
    error(maplocation);
}

/* skip the rest of the header-nothing useful here */
Static Void skipheader(lib, newlib)
_TEXT *lib, *newlib;
{
  checkstar(true); /* check short name */
  needlibline(lib, newlib, &length_);  /* skip full name */
  checkstar(false); /* check full name */
/* skip the note, if one exists */
  if (P_peek(lib->f) != 'n')
    return;
  needlibline(lib, newlib, &length_);
  do {
    needlibline(lib, newlib, &length_);
    if (line[0] != '*') {
      if (line[0] != 'n') {
        erroratline();
        fprintf(humcat.f, " missing asterisk (*) in note\n");
        fprintf(catin.f, " missing asterisk (*) in note\n");
        humcatlines++;
        fatal++;
      }
    } else if (length_ > 1) {
      if (line[1] != ' ') {
        erroratline();
        fprintf(humcat.f, " missing blank in note\n");
        fprintf(catin.f, " missing blank in note\n");
        humcatlines++;
        fatal++;
      }
    }
  } while (line[0] != 'n');
}

/* parses a piece key information */
Static Void piecekey(lib, newlib, coo, pie)
_TEXT *lib, *newlib;
pieceinfo *coo, *pie;
{
  boolean badtoken;
  double geneticmap;

  getreal(lib, newlib, &geneticmap, &badtoken);
  if (badtoken)
    error(genebeginning);

  getconfig(lib, newlib, &coo->config, &badtoken);
  if (badtoken)
    error(cooconfigurationbad);

  getdirect(lib, newlib, &coo->direct, &badtoken);
  if (badtoken)
    error(coordirectionbad);

  getinteger(lib, newlib, &coo->beginning, &badtoken);
  if (badtoken)
    error(coordbeginningbad);

  getinteger(lib, newlib, &coo->ending, &badtoken);
  if (badtoken)
    error(coordendingbad);

  getconfig(lib, newlib, &pie->config, &badtoken);
  if (badtoken)
    error(pieconfigurationbad);

  getdirect(lib, newlib, &pie->direct, &badtoken);
  if (badtoken)
    error(piedirectionbad);

  getinteger(lib, newlib, &pie->beginning, &badtoken);
  if (badtoken)
    error(piebeginningbad);

  getinteger(lib, newlib, &pie->ending, &badtoken);
  if (badtoken)
    error(pieendingbad);

/* write the piece info to humcat */
  fprintf(humcat.f, "%*.2f", nfield, geneticmap);
  fprintf(humcat.f, " |");
  writepieceinfo(&humcat, *coo);
  fprintf(humcat.f, " |");
  writepieceinfo(&humcat, *pie);
  checkmap(geneticmap);

/* write the piece info to catin */
  if (docomments) {
    fprintf(catin.f, "(* coordinates: ");
    fprintf(catin.f, "%*.1f", nfield, geneticmap);
    fprintf(catin.f, " genetic map beginning\n");
    writepieceinfo(&catin, *coo);
    fprintf(catin.f, ": coordinate config, dir, beg, end\n");
    writepieceinfo(&catin, *pie);
    fprintf(catin.f, ": piece config, dir, beg, end *)\n");
  }

  checkmap(geneticmap);
}

/* write the header info for pieces */
Static Void writehumpieceinfo()
{
  fprintf(humcat.f, "%*s", cfield, "|   config");
  fprintf(humcat.f, "%*s", dfield, "dir");
  fprintf(humcat.f, "%*s", nfield, "begin");
  fprintf(humcat.f, "%*s", nfield, "end");
}


Local Void alignit()
{
  fprintf(humcat.f, "       %*c", levelsize + namespace, ' ');
}

/* write the names of the information about each piece */
Static Void piecehumhead()
{
  alignit();
  fprintf(humcat.f, "genetic  ");
  fprintf(humcat.f, "| coordinate%*c", (int)(pfield - 10), ' ');
  fprintf(humcat.f, "| piece\n");
  humcatlines++;

  alignit();
  fprintf(humcat.f, "map      ");
  writehumpieceinfo();
  putc(' ', humcat.f);
  writehumpieceinfo();
  fprintf(humcat.f, "%*s", bfield, "length");
  
}

/* check the genetic map beginning and ending coordinates */
Static Void mapbegend(lib, newlib)
_TEXT *lib, *newlib;
{
  boolean badtoken;

  getreal(lib, newlib, &mapbeg, &badtoken);
  if (badtoken)
    error(genebeginning);

  getreal(lib, newlib, &mapend, &badtoken);
  if (badtoken)
    error(geneending);

  fprintf(humcat.f, " %*.2f %*.2f (genetic map range)\n",
	  nfield, mapbeg, nfield, mapend);
  if (docomments)
    fprintf(catin.f, "(* genetic map range is: %*.2f to %*.2f *)\n",
	    nfield, mapbeg, nfield, mapend);

  piecehumhead();
}


Static Void piereference(lib, newlib)
_TEXT *lib, *newlib;
{
  double geneticmap;  /* the genetic map location */
  boolean badtoken;   /* true iff next token is not proper */

  needline(lib, &length_);  /* note: must not write out yet: the name could change */

  getname();
  if (equalname(&piecename, &none)) {   /* no other references made yet */
    changed = false;
    piecename = aname;
    unique = true;
    pushname(&first.piece_.nextonlist, &latest);
    if (!unique) {
      duplicate(piece, &first.piece_.nextonlist);
      newpiecename = latest->nam;
      changed = true;
    }
  } else if (!equalname(&piecename, &aname))
    error(nolastpiece);
  else if (changed)
    changeto(newpiecename);
  dumpline(newlib, length_);

  getreal(lib, newlib, &geneticmap, &badtoken);
  if (badtoken)
    error(mapbeginning);
  else
    checkmap(geneticmap);

  getdirect(lib, newlib, &pieceref.nodedir, &badtoken);
  if (badtoken)
    error(refdirectionbad);

  getinteger(lib, newlib, &pieceref.nodebeg, &badtoken);
  if (badtoken)
    error(refbeginningbad);

  getinteger(lib, newlib, &pieceref.nodeend, &badtoken);
  if (badtoken)
    error(refendingbad);

/* write reference info to humcat */
  putc(' ', humcat.f);
  fprintf(humcat.f, "%*.2f", nfield, geneticmap);

  fprintf(humcat.f, "%*c", (int)(cfield + pfield), ' ');
  writedirect(&humcat, pieceref.nodedir);
  fprintf(humcat.f, "%*ld", nfield, pieceref.nodebeg);
  fprintf(humcat.f, "%*ld", nfield, pieceref.nodeend);

  fprintf(catin.f, "(* ");
  fprintf(catin.f, "%*.2f", nfield, geneticmap);

  writedirect(&catin, pieceref.nodedir);
  fprintf(catin.f, "%*ld", nfield, pieceref.nodebeg);
  fprintf(catin.f, "%*ld", nfield, pieceref.nodeend);
  fprintf(catin.f, " *)\n");
}

/* skip sites for now */
Static Void dosite(lib, newlib)
_TEXT *lib, *newlib;
{
  do {
    needlibline(lib, newlib, &length_);
  } while (line[0] != 's');
  needlibline(lib, newlib, &length_);
}

/* check the DNA of a piece, as required by
   libdef catalogue definition p. 3.1 2k */
Static Void dodna(lib, newlib)
_TEXT *lib, *newlib;
{
  long i;   /* the current character on a line */
  _TEXT TEMP;

  needlibline(lib, newlib, &length_);   /* move past the "DNA" */
  basecount = 0;
  while (line[0] == '*') {    /* for each line of DNA */
    checkstar(true);
    i = 3;   /* the first base is the third character on the line */
    while (i <= length_) {          /* count bases on the line */
      if (line[i-1] == 'g' || line[i-1] == 't' || line[i-1] == 'c' ||
	  line[i-1] == 'a')
	basecount++;
      else if (line[i-1] != ' ') {
	erroratline();
	fprintf(humcat.f,
	  " DNA sequence contains \"%c\" which is not allowed in a delila library.\n",
	  line[i-1]);

	fprintf(humcat.f, " in piece ");
	writename(&humcat, latest->nam);
	fprintf(humcat.f, "\n\n");

	fprintf(catin.f,
	  " DNA sequence contains \"%c\" which is not allowed in a delila library.\n",
	  line[i-1]);

	fprintf(catin.f, " in piece ");
	writename(&catin, latest->nam);
	fprintf(catin.f, "\n\n");

	humcatlines += 3;
	fatal++;
      }
      i++;
    }
    needlibline(lib, newlib, &length_);
  }
  if (line[0] != 'd')
    strange(dna);

/* print the name of the object containing the dna */
  if (!equalname(&current.piece_.nam, &none)) {
    putchar(' ');
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writename(&TEMP, current.piece_.nam);
    printf(" %6ld bp\n", basecount);
  } else if (!equalname(&current.marker_.nam, &none)) {
    putchar(' ');
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writename(&TEMP, current.marker_.nam);
    printf(" %6ld bp (a marker)\n", basecount);
  } else {
    printf("program error in dodna\n");
    halt();
  }

  fprintf(humcat.f, " %6ld bp", basecount);
  if (docomments)
    fprintf(catin.f, "(* total length: %ld bp *)\n", basecount);
  if (fromtoinst)
    fprintf(catin.f, "get from %ld to same +%ld;\n",
	    pie.beginning, basecount - 1);
  else
    fprintf(catin.f, "get all piece;\n");

  needlibline(lib, newlib, &length_);
}


/* high-level procedures**********************************************/

Static Void domarker(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 2;
  needline(lib, &length_);   /* get to line with marker name */
  getname();
  unique = true;
  pushname(&first.marker_.nextonlist, &latest);
  if (!unique)         /* duplicate names */
    duplicate(marker, &first.marker_.nextonlist);
  current.marker_.nam = latest->nam;
  dumptocat('m', c1, c2, c3);     /* dump a line to the cat */
  dumpline(newlib, length_);
  writehumcat(marker);
  skipheader(lib, newlib);
  pieceref.nodetype = markref;
  pieceref.nodename = current.marker_.nam;
  piereference(lib, newlib);
  needlibline(lib, newlib, &length_);
  checkstar(true);
  needlibline(lib, newlib, &length_);
  checkstar(true);
  needlibline(lib, newlib, &length_);
  if (line[0] == 'd')
    dodna(lib, newlib);
  else          /* must be dna here */
    strange(dna);
  if (line[0] != 'm')    /* after which we must be done */
    strange(marker);
  needlibline(lib, newlib, &length_);
  current.marker_.nam = none;
}


Static Void dogene(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 2;        /* level is set for the catalog listing */
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.gene_.nextonlist, &latest);
  if (!unique)
    duplicate(gene, &first.gene_.nextonlist);
  dumpline(newlib, length_);
  dumptocat('g', c1, c2, c3);
  current.gene_.nam = latest->nam;
  writehumcat(gene);

  skipheader(lib, newlib);
  pieceref.nodetype = generef;
  pieceref.nodename = current.gene_.nam;
  piereference(lib, newlib);
  needlibline(lib, newlib, &length_);
  if (line[0] != 'g')
    strange(gene);
  needlibline(lib, newlib, &length_);
  current.gene_.nam = none;    /* clear current gene name */
}


Static Void dotranscript(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 2;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.transcript_.nextonlist, &latest);
  if (!unique)
    duplicate(transcript, &first.transcript_.nextonlist);
  dumpline(newlib, length_);
  dumptocat('t', c1, c2, c3);
  current.transcript_.nam = latest->nam;
  writehumcat(transcript);

  skipheader(lib, newlib);
  pieceref.nodetype = transref;
  pieceref.nodename = current.transcript_.nam;
  piereference(lib, newlib);
  needlibline(lib, newlib, &length_);
  if (line[0] != 't')
    strange(transcript);
  needlibline(lib, newlib, &length_);

  current.transcript_.nam = none;
}

/* calculate the length of a piece with the input piecekey values.
the function was derived from the standard piecelength and
pietoint functions in delmods. */
Static long checkcoordinates(piedir, piebeg, pieend, coobeg, cooend)
direction piedir;
long piebeg, pieend, coobeg, cooend;
{
  long length;   /* temporary answer */

  switch (piedir) {
    case plus:
      if (pieend >= piebeg)
        length = pieend - piebeg + 1;
      else
        length = pieend - coobeg + cooend - piebeg + 2;
      break;
    case minus:
      if (pieend <= piebeg)
        length = piebeg - pieend + 1;
      else
        length = cooend - pieend + piebeg - coobeg + 2;
      break;
  }
  return length;
}


Static Void dopiece(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  long predictedlength;

  /*
*/
  level = 2;
  needline(lib, &length_);
  getname();
  if (equalname(&piecename, &none)) {
  /* cautions for pieces without a family - show only
     when debugging because they are a pain most of the time. */
    if (debugging)
      error(noreference);
    unique = true;
    pushname(&first.piece_.nextonlist, &latest);
    if (!unique) {
      duplicate(piece, &first.piece_.nextonlist);
      changed = true;
      newpiecename = latest->nam;
    }
  } else {
    if (!equalname(&piecename, &aname))
      error(wrongreference);
    else if (changed)
      changeto(newpiecename);
  }
  dumptocat('p', c1, c2, c3);
  if (!changed)
    current.piece_.nam = aname;
  else
    current.piece_.nam = newpiecename;
  piecefound = true;
  piecename = none;
  dumpline(newlib, length_);

  writehumcat(piece);
  skipheader(lib, newlib);
  piecekey(lib, newlib, &coo, &pie);
  needlibline(lib, newlib, &length_);
  if (line[0] == 'd')
    dodna(lib, newlib);
  else
    strange(dna);

/* use pie and coo to check the expected length of the piece
      compared to basecount as required by
      libdef catalogue definition p. 3.1 2i  */
  predictedlength = checkcoordinates(pie.direct, pie.beginning, pie.ending,
				     coo.beginning, coo.ending);
  if (predictedlength != basecount) {
    erroratline();
    fprintf(humcat.f,
	    " length of piece predicted from piece coordinates was: %5ld\n",
	    predictedlength);
    fprintf(humcat.f, " the actual number of bases counted was:%20ld\n",
	    basecount);
    fprintf(humcat.f, " the difference is%42ld\n",
	    predictedlength - basecount);
    fprintf(humcat.f, " for piece ");
    writename(&humcat, current.piece_.nam);
    putc('\n', humcat.f);

    fprintf(catin.f,
	    " length of piece predicted from piece coordinates was: %5ld\n",
	    predictedlength);
    fprintf(catin.f, " the actual number of bases counted was:%20ld\n",
	    basecount);
    fprintf(catin.f, " the difference is%42ld\n", predictedlength - basecount);
    fprintf(catin.f, " for piece ");
    writename(&catin, current.piece_.nam);
    putc('\n', catin.f);
    humcatlines += 4;
    fatal++;
  }

  /* check coordinate order
     as required by libdef catalogue definition p. 3.1 2h */
  if (coo.beginning > coo.ending) {
    erroratline();
    fprintf(humcat.f,
	    " coordinate beginning must be less than or equal to ending\n");
    fprintf(catin.f,
	    " coordinate beginning must be less than or equal to ending\n");
    humcatlines++;
    fatal++;
  }

  /* check configuration
         as required by libdef catalogue definition p. 3.1 2j */
  if (coo.config == linear) {
    if (pie.config != linear) {
      erroratline();
      fprintf(humcat.f, " linear coordinates imply linear pieces\n");
      fprintf(catin.f, " linear coordinates imply linear pieces\n");
      humcatlines++;
      fatal++;
    }
  }

  if (line[0] != 'p')
    strange(piece);
  needlibline(lib, newlib, &length_);
  current.piece_.nam = none;

  /* although the name of the piece of this family may have been
   changed, we do not want to change the names of later pieces: */
  changed = false;
}


Static Void dochromosome(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 1;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.chromosome_.nextonlist, &latest);
  if (!unique)
    duplicate(chromosome, &first.chromosome_.nextonlist);
  dumptocat('c', c1, c2, c3);
  dumpline(newlib, length_);
  current.chromosome_.nam = latest->nam;
  writehumcat(chromosome);
  skipheader(lib, newlib);
  mapbegend(lib, newlib);    /* check map beginning and ending */
  needlibline(lib, newlib, &length_);
  while (line[0] == 'p' || line[0] == 'g' || line[0] == 't' || line[0] == 'm') {
    if (line[0] == 'm') {           /* we have a marker next */
      domarker(lib, newlib, c1, c2, c3);
      continue;
    }
    if (line[0] == 't')   /* we have a transcript */
      dotranscript(lib, newlib, c1, c2, c3);
    else {
      if (line[0] == 'g')   /* we have a gene */ 
	dogene(lib, newlib, c1, c2, c3);
      else
	dopiece(lib, newlib, c1, c2, c3);
    }
  }
  
  /* no more of those, so our chromosome must be done */
  if (line[0] != 'c') {
    strange(chromosome);
  }

 /* clear the lists of names of markers, transcripts,
    genes, and pieces in this chromosome */
  popnames(&first.marker_.nextonlist);
  popnames(&first.transcript_.nextonlist);
  popnames(&first.gene_.nextonlist);
  popnames(&first.piece_.nextonlist);
  needlibline(lib, newlib, &length_);
  current.chromosome_.nam = none;
}


Static Void doenzyme(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 1;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.enzyme_.nextonlist, &latest);
  if (!unique)
    duplicate(enzyme, &first.transcript_.nextonlist);
  dumptocat('e', c1, c2, c3);
  dumpline(newlib, length_);
  current.enzyme_.nam = latest->nam;
  writehumcat(enzyme);
  skipheader(lib, newlib);
  needlibline(lib, newlib, &length_);
  needlibline(lib, newlib, &length_);
  while (line[0] == 's')
    dosite(lib, newlib);
  if (line[0] != 'e')
    strange(enzyme);
  needlibline(lib, newlib, &length_);
  current.enzyme_.nam = none;
}


Static Void doorganism(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  long i, FORLIM;

  level = 0;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.organism_.nextonlist, &latest);
  if (!unique)
    duplicate(organism, &first.organism_.nextonlist);
  dumptocat('o', c1, c2, c3);
  dumpline(newlib, length_);
  current.organism_.nam = latest->nam;
  writehumcat(organism);
  skipheader(lib, newlib);
  needlibline(lib, newlib, &length_);
  checkstar(true);


  putc(' ', humcat.f);
  FORLIM = length_;
  for (i = 2; i < FORLIM; i++)
    putc(line[i], humcat.f);
  fprintf(humcat.f, " (genetic map units)");

  if (docomments) {
    fprintf(catin.f, "(* genetic map units are: ");
    FORLIM = length_;
    for (i = 2; i < FORLIM; i++)
      putc(line[i], catin.f);
    fprintf(catin.f, " *)\n");
  }

  needlibline(lib, newlib, &length_);
  while (line[0] == 'c')
    dochromosome(lib, newlib, c1, c2, c3);
  if (line[0] != 'o')
    strange(organism);
  popnames(&first.chromosome_.nextonlist);
  current.organism_.nam = none;
}


Static Void dorecognition(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 0;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.recognition.nextonlist, &latest);
  if (!unique)
    duplicate(recognitionclass, &first.recognition.nextonlist);
  dumptocat('r', c1, c2, c3);
  dumpline(newlib, length_);
  current.recognition.nam = latest->nam;
  writehumcat(recognitionclass);
  skipheader(lib, newlib);
  needlibline(lib, newlib, &length_);
  checkstar(true);
  needlibline(lib, newlib, &length_);
  while (line[0] == 'e')
    doenzyme(lib, newlib, c1, c2, c3);
  if (line[0] != 'r')
    strange(recognitionclass);
  popnames(&first.enzyme_.nextonlist);
  /*
*/
  current.recognition.nam = none;
  /*
*/
}


Static Void dolibrary(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  if (*lib->name != '\0') {
    if (lib->f != NULL)
      lib->f = freopen(lib->name, "r", lib->f);
    else
      lib->f = fopen(lib->name, "r");
  } else
    rewind(lib->f);
  if (lib->f == NULL)
    _EscIO2(FileNotFound, lib->name);
  RESETBUF(lib->f, Char);
  if (*newlib->name != '\0') {
    if (newlib->f != NULL)
      newlib->f = freopen(newlib->name, "w", newlib->f);
    else
      newlib->f = fopen(newlib->name, "w");
  } else {
    if (newlib->f != NULL)
      rewind(newlib->f);
    else
      newlib->f = tmpfile();
  }
  if (newlib->f == NULL)
    _EscIO2(FileNotFound, newlib->name);
  SETUPBUF(newlib->f, Char);
  libdone = false;
  libline = 1;


  if (!BUFEOF(lib->f))
    readlibdate(lib, newlib, c1, c2, c3);

  while (!BUFEOF(lib->f)) {
    readline(lib, &length_);
    dumpline(newlib, length_);
    if (libdone)
      continue;
    if (line[0] == 'o')
      doorganism(lib, newlib, c1, c2, c3);
    else if (line[0] == 'r')
      dorecognition(lib, newlib, c1, c2, c3);
    else
      strange(library);
  }
  libnumber++;
  catnumber++;
}


Static Void themain(catalp, l1, c1, newl1, l2, c2, newl2, l3, c3, newl3,
		    humcat, catin, fout)
_TEXT *catalp, *l1;
catfile *c1;
_TEXT *newl1, *l2;
catfile *c2;
_TEXT *newl2, *l3;
catfile *c3;
_TEXT *newl3, *humcat, *catin, *fout;
{ 
  /* the main procedure of the program */
  fprintf(fout->f, " catal %4.2f ", version);

  getdatetime(daytime);
  writedatetime(fout, daytime);
  putc('\n', fout->f);

  maxnamelength = 0;

/* Check that there is at least one library. */
  if (*l1->name != '\0') {
    if (l1->f != NULL)
      l1->f = freopen(l1->name, "r", l1->f);
    else
      l1->f = fopen(l1->name, "r");
  } else
    rewind(l1->f);
  if (l1->f == NULL)
    _EscIO2(FileNotFound, l1->name);
  RESETBUF(l1->f, Char);
  if (*l2->name != '\0') {
    if (l2->f != NULL)
      l2->f = freopen(l2->name, "r", l2->f);
    else
      l2->f = fopen(l2->name, "r");
  } else
    rewind(l2->f);
  if (l2->f == NULL)
    _EscIO2(FileNotFound, l2->name);
  RESETBUF(l2->f, Char);
  if (*l3->name != '\0') {
    if (l3->f != NULL)
      l3->f = freopen(l3->name, "r", l3->f);
    else
      l3->f = fopen(l3->name, "r");
  } else
    rewind(l3->f);
  if (l3->f == NULL)
    _EscIO2(FileNotFound, l3->name);
  RESETBUF(l3->f, Char);
  if (!((!BUFEOF(l1->f)) | (!BUFEOF(l2->f)) | (!BUFEOF(l3->f)))) {
    docathelp(fout);
    return;
  }
  initializecatalogs(catalp, c1, c2, c3);


  dolibrary(l1, newl1, c1, c2, c3);
  dolibrary(l2, newl2, c1, c2, c3);
  dolibrary(l3, newl3, c1, c2, c3);

/* destroy new libraries and catalogues if there was any problem */
  if (fatal == 0) {
    putc('\n', humcat->f);
    return;
  }

  if (*newl1->name != '\0') {
    if (newl1->f != NULL)
      newl1->f = freopen(newl1->name, "w", newl1->f);
    else
      newl1->f = fopen(newl1->name, "w");
  } else {
    if (newl1->f != NULL)
      rewind(newl1->f);
    else
      newl1->f = tmpfile();
  }
  if (newl1->f == NULL)
    _EscIO2(FileNotFound, newl1->name);
  SETUPBUF(newl1->f, Char);
  if (*newl2->name != '\0') {
    if (newl2->f != NULL)
      newl2->f = freopen(newl2->name, "w", newl2->f);
    else
      newl2->f = fopen(newl2->name, "w");
  } else {
    if (newl2->f != NULL)
      rewind(newl2->f);
    else
      newl2->f = tmpfile();
  }
  if (newl2->f == NULL)
    _EscIO2(FileNotFound, newl2->name);
  SETUPBUF(newl2->f, Char);
  if (*newl3->name != '\0') {
    if (newl3->f != NULL)
      newl3->f = freopen(newl3->name, "w", newl3->f);
    else
      newl3->f = fopen(newl3->name, "w");
  } else {
    if (newl3->f != NULL)
      rewind(newl3->f);
    else
      newl3->f = tmpfile();
  }
  if (newl3->f == NULL)
    _EscIO2(FileNotFound, newl3->name);
  SETUPBUF(newl3->f, Char);
  if (*c1->name != '\0') {
    if (c1->f != NULL)
      c1->f = freopen(c1->name, "wb", c1->f);
    else
      c1->f = fopen(c1->name, "wb");
  } else {
    if (c1->f != NULL)
      rewind(c1->f);
    else
      c1->f = tmpfile();
  }
  if (c1->f == NULL)
    _EscIO2(FileNotFound, c1->name);
  SETUPBUF(c1->f, item);
  if (*c2->name != '\0') {
    if (c2->f != NULL)
      c2->f = freopen(c2->name, "wb", c2->f);
    else
      c2->f = fopen(c2->name, "wb");
  } else {
    if (c2->f != NULL)
      rewind(c2->f);
    else
      c2->f = tmpfile();
  }
  if (c2->f == NULL)
    _EscIO2(FileNotFound, c2->name);
  SETUPBUF(c2->f, item);
  if (*c3->name != '\0') {
    if (c3->f != NULL)
      c3->f = freopen(c3->name, "wb", c3->f);
    else
      c3->f = fopen(c3->name, "wb");
  } else {
    if (c3->f != NULL)
      rewind(c3->f);
    else
      c3->f = tmpfile();
  }
  if (c3->f == NULL)
    _EscIO2(FileNotFound, c3->name);
  SETUPBUF(c3->f, item);
  fprintf(fout->f,
	  " %ld fatal error(s): new libraries and catalogues destroyed\n",
	  fatal);
  fprintf(humcat->f,
	  " %ld fatal error(s): new libraries and catalogues destroyed\n",
	  fatal);
  fprintf(catin->f,
	  " %ld fatal error(s): new libraries and catalogues destroyed\n",
	  fatal);
  fprintf(fout->f, " see humcat.\n");
}

void usage() {
  printf("\n");
  printf(" catal: program checks all the input libraries for correct structure\n");
  printf("\n");
  printf( "parameters: \n");
  printf("\n  catal -f parameter file\n\n");
  printf("  -f parameter file\n");
  printf("   Text file, formatted as follows:\n");
  printf("\tl1=l1:\n");
  printf("\tl2=l2\n");
  printf("\tl3=l3\n");
  printf("\tcat1=cat1\n");
  printf("\tcat2=cat2\n");
  printf("\tcat3=cat3\n");
  printf("\thumcat=humcat\n");
  printf("\tcatin=catin\n");
  printf("\tlib1=lib1\n");
  printf("\tlib2=lib2\n");
  printf("\tlib3=lib3\n");
  printf("\tcatalp=catalp\n");  
  printf("\n");
  printf("  version %4.2f\n", version);
  exit(EXIT_SUCCESS);
}

int main(int argc, Char **argv)
{
  _TEXT TEMP;
	extern char *optarg;
	extern int optind;
	int c, err = 0; 
  /* flags marking arguments passed */
	int fflag=0;     // file flag 
  char *param;    // 
	char *fName;

  /* Process command line arguments  */
  while ((c = getopt(argc, argv, "f:")) != -1)
		switch (c) {
		case 'f':
      		fflag = 1;
			fName = optarg;
			break;
		case '?':
			err = 1;
			break;
		}
  
  /* Is the input parameter file name present */  
	if (fflag == 0) {	/* -f was mandatory */ 
		fprintf(stderr, "%s: missing -f parameter file\n", argv[0]);
		usage();
		exit(1);
	} 
  /* If no command line arguments, print help */ 
  if (err) {
		usage();
		exit(1);
	}
	
	// Open input file
	FILE *fp = fopen(fName, "r");
	char buf[1024];
	
	// Check if input file exists
	if (!fp){
		printf("Can't open file\n");
		return 1;
	}
	
	// Declaration of delimiter 
	const char d[4] = "=";
	char *rest = NULL;
  TEMP.f = stdout;
  *TEMP.name = '\0';
  
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
	  // Loop through file
  	while (fgets(buf, 1024, fp)){
      // split line and get values
      param = strtok_r(buf, d, &rest);
      fName = strtok_r(NULL, d, &rest);
      // Define input file names and parse file input
      // match values to variable
      if (strcmp(param, "l1") == 0){
        l1.f = NULL;
        strcpy(l1.name, fName);
        l1.name[strlen(l1.name) -1] = '\0';
      } 
      else if (strcmp(param, "l2") == 0){
        l2.f = NULL;        
        strcpy(l2.name, fName);
        l2.name[strlen(l2.name) -1] = '\0';
      } 
      else if (strcmp(param, "l3") == 0){
        l3.f = NULL;
        strcpy(l3.name, fName);
        l3.name[strlen(l3.name) -1] = '\0';
      }else if (strcmp(param, "lib1") == 0){
        lib1.f = NULL;
        strcpy(lib1.name, fName);
        lib1.name[strlen(lib1.name) -1] = '\0';
      }
      else if (strcmp(param, "lib2") == 0){
        lib2.f = NULL;
        strcpy(lib2.name, fName);
        lib2.name[strlen(lib2.name) -1] = '\0';
      }
      else if (strcmp(param, "lib3") == 0){
        lib3.f = NULL;
        strcpy(lib3.name, fName);
        lib3.name[strlen(lib3.name) -1] = '\0';
      }
      else if (strcmp(param, "cat1") == 0){
        cat1.f = NULL;
        strcpy(cat1.name, fName);
        cat1.name[strlen(cat1.name) -1] = '\0';
      }
      else if (strcmp(param, "cat2") == 0){
        cat2.f = NULL;
        strcpy(cat2.name, fName);
        cat2.name[strlen(cat2.name) -1] = '\0';
      }
      else if (strcmp(param, "cat3") == 0){
        cat3.f = NULL;
        strcpy(cat3.name, fName);
        cat3.name[strlen(cat3.name) -1] = '\0';
      }
      else if (strcmp(param, "catin") == 0){
        catin.f = NULL;
        strcpy(catin.name, fName);
        catin.name[strlen(catin.name) -1] = '\0';
      }
      else if (strcmp(param, "humcat") == 0){
        humcat.f = NULL;
        strcpy(humcat.name, fName);
        humcat.name[strlen(humcat.name) -1] = '\0';
      }
      else if (strcmp(param, "catalp") == 0){
        catalp.f = NULL;
        strcpy(catalp.name, fName);
        catalp.name[strlen(catalp.name) -1] = '\0';
      }
    }

    // Close parameter file
    fclose(fp); 
    themain(&catalp, &l1, &cat1, &lib1, &l2, &cat2, &lib2, &l3, &cat3, &lib3, 
    &humcat, &catin, &TEMP);
_L1:
  if (catalp.f != NULL)
    fclose(catalp.f);
  if (cat1.f != NULL)
    fclose(cat1.f);
  if (cat2.f != NULL)
    fclose(cat2.f);
  if (cat3.f != NULL)
    fclose(cat3.f);
  if (humcat.f != NULL)
    fclose(humcat.f);
  if (catin.f != NULL)
    fclose(catin.f);
  if (l1.f != NULL)
    fclose(l1.f);
  if (l2.f != NULL)
    fclose(l2.f);
  if (l3.f != NULL)
    fclose(l3.f);
  if (lib1.f != NULL)
    fclose(lib1.f);
  if (lib2.f != NULL)
    fclose(lib2.f);
  if (lib3.f != NULL)
    fclose(lib3.f);
  exit(EXIT_SUCCESS);

  return 0;
}
/* End. */
