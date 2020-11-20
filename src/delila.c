/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "delila.p" */
/* delila: the librarian for sequence manipulation
              by Thomas Schneider
                Gary Stormo
                Paul Morrissett
                useful suggestions by Jeff haemer
    
             module libraries needed: delman, delmods.
    
      Thomas D. Schneider, Ph.D.
      toms@alum.mit.edu
      https://alum.mit.edu/www/toms  */

/* version = 5.04; (* of delila.p 2017 Aug 09
2017 Aug 09: 5.04; clean up marksdelila output
2017 Jul 14: 5.03; namelength/linelength set to 200/210 (see catal)
2017 Jul 11: 5.02; namelength/linelength set to 200 to allow even longer names
2017 Jul 10: 5.01; namelength set to 200 to allow even longer names
2016 Jan 25: 5.00; debug: make parsedwordindex: integer; local.
2015 Aug 25: 4.99; backup
2015 Aug 25: 4.98; only warn about overlapping changes - error(222)
2015 Jul 31: 4.97; handle tabs as spaces
2015 Mar 23: 4.96; clean up
2015 Mar 23: 4.95; names from name command are missing with 'with'.
                   irlongname shows that longname was there.
                   pk^.key.hea.fulnam is the variable,
                   before writing it out, fill it with global longname.
2014 Feb 13: 4.94; when reversenames if no name -> duplicate
2013 Feb 13: 4.93; set default reversenames REVERSENAMES
2012 Feb 16: 4.92; Don't halt with error(222) - check for overlaps
                   ONLY if mutations are inside the piece
2010 Feb 16: 4.91; showallerrors for error 211
2010 Feb 16: 4.90; allow mutations of form A123C (capitals allowed)
2007 Dec 06: 4.89; always reduce coordinates if the piece is circular
2005 Sep  6: 4.88; demote error 220 to a warning: deleting a whole
                   sequence merely means not to put it in the book!
                   Implement by not writing to the book ...
2005 Jan 11: 4.87; cleanup
2005 Jan 11: 4.86; long names cause infnite loop
                   in irlongname this was missing:
                   longname^.next := nil;
2004 Jan 21: 4.85; finish up marks.
2004 Jan 21: 4.84; cleanup
2004 Jan 21: 4.83; gpctime.p upgrade.  compiles but gives segmentation fault!
                   needed to set pk^.dna := nil; libpie^.dna := nil;
2004 Jan 21: 4.82; marks. arrow -> marks.arrow
2003 Aug 18: 4.81; halt message includes the name 'delila'
2003 Apr  7: 4.80; bug fix - request for change before start of piece now ok.
2002 Apr 17: 4.79; tab and other odd ASCII characters disallowed: error 33.
2001 Oct  5: 4.78; mutations off piece are now warnings, not errors (215, 216)
2001 Mar 28: 4.77; pass 2 errors written independently of pass 1
2001 Mar 28: 4.75; bug: two titles bombs!
2001 Mar 16: 4.74; clean up
2001 Mar 16: 4.73; gene functions; specpiece must use libpie not pk!
2001 Mar 16: 4.72; upgrade bug documentation (gene no longer functions)
2000 Nov 15: 4.70; catch unclosed comments
2000 Oct 26: 4.69; set maximum book size
2000 Oct 18: 4.67; stmts w/o with still trigger marksdelila stepping
2000 Oct 18: 4.66; upgrade to gpc
2000 Oct 17: 4.63; 'direction + with a4021131c[cr]; caused parse bomb.
2000 Oct 17: 4.62; blank after 'direction +' caused parse bomb.
2000 Aug 17: 4.60; fixed withused bug; marksdelila only written in pass 2
2000 July 12: 4.59; withused control: marksdelila not touched unless needed.
2000 June 21: 4.58; upgrade the See Also section.
2000 May 26: 4.55; allow inserts to be complemented. zzz111
2000 May 25: 4.54; Fix insertion bug in complements (proc. changesequence)!
2000 May 25: 4.53; Remove duplication of error reports to output.
2000 Mar 29: 4.52; Report error numbers to output, saves checking listing.
2000 January 3: 4.50; {} comments for the html links
2000 January 3: 4.49; html link for delila instructions
1999 August 17: 4.48; changes not noted
1999 July 17: 4.47: Add message You_need_a_marks.arrow_definition to marksdelila
                    file so user will see this if they forget.
1999 July 13: 4.46: putbase, getbase etc now allow any length in each
                    dnasegment - much more robust.
1999 July 9: 4.20: bug fix in getdnasegment dnamax could not be small
                   (b and bDNAptr need to be stepped if they exceed dnamax)
1999 May 27: 4.07: user does not propagate coordinates
1999 May 24: 4.03: mutations cannot overlap - makes all marks work!
                   not implemented - need other changes first
1999 May 23: 4.00: a26g works as t26c on the complement
1999 May 14: 3.76: <INSERTION> and <DELETION> always are in increasing
                   order, following the new definition in Libdef.
1999 May 12: 3.66: new comment type, {}
1999 May 11: 3.56: ability of insertion/deletions to wrap around
1999 May 10: 3.50: upgraded error reporting for incorrect change commands.
1999 May 6: 3.21:  added errors 215, 216; moved mutation routines
                   below geteoinst so error output is cleaner.
1999 May 5: 3.17:  fix delila position misreading
1999 April 27: 3.10:  fix memory leak in circledna/invert
1999 April 26: 3.06:  fix bug in: get from 6 to 1 with i4,3ggttgg;
1999 April 15: 3.03:  fix bug in: get from 1 to 6 with i3,4ggttgg;
1999 April 14: 3.00: allow insert off ends of piece (fix bug)
1999 April 14: 2.99: fixed equalname: need to set i initially
1999 April 13: 2.98: fixed memory leak
1999 March 21: 2.90: marks are sorted so most displays will work.
1999 March 21: 2.85: Synonymes: default = set
1999 March 20: 2.84: Delila can now create mutation marks for lister!
1999 March 19: 2.70: Delila can now create mutations!
1999 March 17: 2.61: For completeness, Delila can now extract a single base
     from the complementary strand, as in "get from 1 to 1 direction -;"
1999 March 14: 2.44: dopiece functions entirely on internal variables!
1999 March 13: 2.40: conversion to standard delmod book reading routines.
1999 Mar 9: 2.34 The catal file gives a line number that each object starts
     on.  The bookreading routine getto used to get to the line after the
     start of an object, and the routines like brdna, brpiece, getocp all
     took this into account.  To make it more clear, getto in delmod and here
     now gets to the start of the object.  The routines that then read the
     object may readln past the start if they want.
1999 Mar 9: 2.31 Convert to standard book reading routings:  Procedure
     dopiece needs to read in the dna, make mutations, clip out the relevant
     part and then spit it out to the book.  It needs to keep track of
     library lines if the catal is to be of any use.  But the standard book
     reading routine brpiece does not track the lines read.  Therefore it was
     necessary to alter delmod.p so that the standard book reading routines
     keep track of the line number.  This was done.
1999 Mar 6: Delila absored the mutation mechanism from dbmutate.  Not
            functional yet because it needs to read pieces in standard way.
1998 June 24: default coordinate 0 was being set in pass 1 but not
     reset to normal at the start of pass 2.
1998 January 26: namelength set to 100 to allow long names
1998 January 4: dnamax set to 10 million for faster grabs
1997 January 10: For convenience, the default is: only pieces are numbered.
1996 September 2: Two reductions off end of piece is now flagged.
1996 August 12: introduction of 'same'.
1995 December 7: objects can now be named in the long name
1995 Nov 13: default coordinate zero now allows default coordinate (number)
last changes: 1989 November 14
origin: 1980 or so *)
(* end module version *)

(* begin module describe.delila *)
(*
name
   delila: the librarian for sequence manipulation

synopsis
   delila(inst: in, book: out, listing: out,
          marksdelila: out,
          lib1: in, cat1: in,
          lib2: in, cat2: in,
          lib3: in, cat3: in,
          output: out, debug: out)

files
   inst: instructions written in the language delila that tell the
      program delila what sequences to pull out of the library.

   book: the set of sequences pulled out of the library.

   listing: the instructions are listed along with errors found or
      actions taken.

   marksdelila: Colored marks for the lister program that indicate
      the locations of base changes, insertions and deletions.

   lib1: the first library from which to obtain sequences
   cat1: the first catalogue, corresponding to lib1
   lib2: the second library
   cat2: the second catalogue, corresponding to lib2
   lib3: the third library
   cat3: the third catalogue, corresponding to lib3

   debug: traces through the actions taken, for debugging delila
      (only produced if variable debugging is true.)

   output: messages to the user
description

   Delila is a data base manager for nucleic acid sequences.  It takes a set
   of instructions, written in the language delila (DEoxyribonucleic acid
   LIbrary LAnguage) and a large set of sequences called a library.  The
   output is a listing of the actions taken (or errors) corresponding to the
   instructions, and a "book" containing the sequences desired.

examples
   see the documentation

documentation
   libdef (defines delila), delman.intro, delman.use, delman.construction
   philgen.ps

see also

   {Definition of the database system:}
   libdef

   {Introduction to Delila Instructions:}
   https://alum.mit.edu/www/toms/delilainstructions.html

   {Related programs:}
   alist.p, catal.p, loocat.p, dbbk.p, lister.p

   dbmutate.p {is deprecated: use the mutation method, 'with'}

   {(} http://www.m-w.com/cgi-bin/dictionary?deprecated {)}

author
   Thomas D. Schneider, Gary D. Stormo and Paul Morrisett
   useful suggestions by Jeff Haemer

bugs

   There used to be many known bugs in delila, related to extracting linear
   fragments of circular sequences.  Delila was rebuilt in the spring of 1999
   and is much more robust now.

   The following features are not available in this program:  recognition
   classes and enzymes, markers, automatic printing to the book of structures
   that intersect a piece, get all (for org, chr, rec and enz), get every and
   if.  The gene mechanism was revived on 2001 Mar 16, eventually it may be
   used to implement features.

   Delila programs use a packed array of bases.  This means that a smart
   Pascal compiler can store DNA sequences in two bits per base.  When delila
   was originally designed, I thought that everybody would complete their
   sequences and therefore there would never be unknown bases in a database.
   Silly me.  GenBank has plenty of such cases.  The dbbk program avoids this
   problem by converting such bases to 'a'.  Fortunately these can now be
   displayed with lister.  Someday Delila may be upgraded to handle this
   case, but it might be at the cost of reducing the maximum sequence that
   can be handled.



files used:
      inst = instructions
      book = the book that is printed
      listing = instruction listing
      lib1,lib2,...numlibfil = the files of the library
      cat1,cat2,...numcatfil = the files of the catalogue
      debug = listing for debugging the code
      output = for fatal error messages to the terminal

procedure name conventions:
      cr catalogue read
      ir instruction read
      lr library read
      bw book write

 flow of information in the librarian
   [file name]  (procedure)

 [library]      [catalogue]  [instructions]
      :              :           :
      v              v           v
      :              :           :
      :          (catalogue      :
      :          procedures)     :
      :              :           :
      :....... ......:           :
             : :                 :
            (lr"s)           (ir"s)
              :                  :
              v                  v
              :                  :
              :  ................:
              :  :
              :  :
            (delila)
              :  :
              v  v
              :  :
       .......:  :................
       :                         :
       :                       (ir"s)
     (bw"s)                (writeerror"s)
       :                   (writevalue"s)
       :                         :
       v                         v
       :                         :
    [book]                    [listing]

      further documentation for this program is in:
      'organism and recognition class library definition:
       a dna sequence data base' 1980 june 9

      note.. the following features are not yet available in this program:
      recognition class and enzymes
      markers
      automatic printing to the book of structures that intersect a piece
      get all (for org, chr, rec and enz)
      get every
      if

      lll = places that must be changed when one changes the number of
         library files: numlibfil
      ccc = places that must be changed when one changes the number of
         catalogue files: numcatfil

TO COMPILE:

gcc  delila.c -o delila  -I/home/mplace/bin/p2c/src -L /home/mplace/bin/p2c/src -lm -lp2c

TO RUN:

delila -b l1 -i NC_007490.2_delila_instructions.inst -l outlisting.txt
*/

#include <getopt.h>  /* getopt API */ 
#include <stdio.h>   /* printf */
#include <stdlib.h> 
#include </home/mplace/bin/p2c/src/p2c.h>

#define version         5.04
#define numlibfil       3      /* number of library files lll */
#define numcatfil       3      /* number of catalogue files ccc */
#define namelength      200    /* maximum key name length */
#define linelength      200    /* maximum line readable in library */
#define dnamax          1024   /* maximum bases of dna in a part of a dna string */
#define dnalinemax      60     /* bases of dna per line written in the book */
#define sitemax         20     /* bases of a site in a part of a site string */
#define maxstep         10     /* the maximum number of steps of the traversal chart */
#define instwidth       6      /* width of left edge numbers on the listing */
#define marksdeliladebug  false  /* debug lines for marksdelila */
#define decbase         2      /* number of decimal places for bases in marksdelila */
#define widbase         6      /* width of places for bases in marksdelila */
#define decbits         2      /* number of decimal places for bits in marksdelila */
#define widbits         6      /* width of places for bits in marksdelila */
#define maxbook         LONG_MAX /* maximum book size */
#define datetimearraylength  19  /* length of dataarray for dates,
 It is just long enough to include the 4 digit year - solving the
 year 2000 problem:
 1980/06/09 18:49:11
 123456789 123456789 
          1         2  */
#define changesetmax    20       /* maximum number of changes allowed */
#define insertmax       1000     /* maximum insertion length allowed (bp) */

/* types *****************************************************************/
/* the nodes in the library tree */
typedef enum {
  libnode, orgnode, chrnode, marnode, mardnanode, tranode, gennode, pienode,
  piednanode, recnode, enznode, sitenode
} node;

typedef Char step[maxstep];

/* begin module book.type ****************)
types needed for book manipulations  */
typedef long chset[5];
typedef Char alpha[namelength];

/* name is a left justified string with blanks following the characters */
typedef struct name {
  alpha letters;
  uchar length;    /* zero means an unspecified structure */
} name;

/* a line of characters */
typedef struct line {
  Char letters[linelength];
  uchar length;
  struct line *next;
} line;

typedef enum {
  plus, minus, dircomplement, dirhomologous
} direction;

typedef enum {
  linear, circular
} configuration;

typedef enum {
  on, off
} state;

/* header of key */
typedef struct header {
  name keynam;    /* key name of structure */
  line *fulnam;   /* full name of structure */
  line *note;     /* note key  */
} header;

/* define the four nucleotide bases */
typedef enum {
  a, c, g, t
} base;

/* sequence types */
typedef short dnarange;

/* p2c: delila.p, line 403:
 * Note: Field width for seq assumes enum base has 4 elements [105] */
typedef uchar seq[(dnamax + 3) / 4];

typedef struct dnastring {
  seq part;
  dnarange length;
  struct dnastring *next;
} dnastring;

/* organism key */
typedef struct orgkey {
  header hea;
  line *mapunit;  /* genetic map units */
} orgkey;

/* chromosome key */
typedef struct chrkey {
  header hea;
  double mapbeg; /* number of genetic map beginning*/
  double mapend; /* number of genetic map ending */ 
} chrkey;

/* piece key */
typedef struct piekey {
  header hea;
  double mapbeg;        /* genetic map beginning */
  configuration coocon; /* configruation (circular/linear) */
  direction coodir;     /* direction (+/-) relative to genetic map */
  long coobeg;          /* beginning nucleotide */
  long cooend;          /* ending nucleotide */
  configuration piecon; /* configruation (circular/linear) */
  direction piedir;     /* direction (+/-) relative to coordinates */
  long piebeg;          /* beginning nucleotide *  */  
  long pieend;          /* ending nucleotide */
} piekey;


typedef struct piece {
  piekey key;
  dnastring *dna;
} piece;

typedef struct reference {
  name pienam;          /* name of piece referred to */
  double mapbeg;        /* genetic map beginning */
  direction refdir;     /* direction relative to coordinates */
  long refbeg;          /* beginning nucleotide */ 
  long refend;          /* ending nucleotide */
} reference;

/* gene key */
typedef struct genkey {
  header hea;
  reference ref;
} genkey;

/* transcript key */
typedef struct trakey {
  header hea;
  reference ref;
} trakey;

/* marker key */
typedef struct markey {
  header hea;
  reference ref;
  state sta;
  line *phenotype;
  struct marker *next;
} markey;

typedef struct marker {
  markey key;
  dnastring *dna;
} marker;

/* types defined in library definition that are not in the standard
   book reading routines */
/* dna recognition class key */
typedef struct reckey {
  header hea;
} reckey;

/* enzyme key */
typedef struct enzkey {
  header hea;
} enzkey;

typedef enum {
  sa, sc, sg, st, pu, py, sn, modification, cleaveage, unknown, alternative
} sitebase;

/* a single recognition site */
typedef struct sitestring {
/* p2c: delila.p, line 494:
 * Note: Field width for part assumes enum sitebase has 11 elements [105] */
  uchar part[(sitemax + 1) / 2];
  char length;
  struct sitestring *next;   /* pointer to another recognition site */
} sitestring;

//
typedef struct enzyme {
  enzkey key;
  sitestring *sites;
} enzyme;

/* to print the key or not to print the key */
typedef struct defaultkey {
  state note; /* note */
  state mar;  /* marker */
  state gen;  /* gene */
  state tra;  /* transcript */
} defaultkey;

/* to act on the site or not to act on the site */
typedef struct defaultsite {
  state expand;  /* expand sites */
  state modify;  /* modify sites */
  state cleave;  /* cleave sites */
} defaultsite;

/* how to act when a request is out of range: */
typedef enum {
  rreduce, rcontinue, rhalt
} rangeaction;

typedef enum {
  orgnum, chrnum, marnum, tranum, gennum, pienum, recnum, enznum
} numberedstructure;

typedef struct defaultnumber {
  state sta;     /* whether or not to number in the book */
  state str[8];  /* what can be numbered */
  long item;     /* the next item"s number */
} defaultnumber;

/* coordinate type: */
typedef enum {
  coornormal, coorzero
} coordinatetype;

typedef struct default_ {
  defaultkey key;
  defaultsite sit;
  rangeaction defout;   /* odd name to get around sun4 bug... */
  defaultnumber num;
  coordinatetype coo;
  state doubling;       /* whether to make two pieces when mutating */
  double arrowlength;
  state reversenames;   /* reverse key and long name in book */
} default_;

/* catalogue types */
typedef struct item {
  Char letter;    /* type of structure */ 
  name nam;       /* the structure"s key name */
  long line_;     /* location of the structure in the library */
} item;

typedef struct catfile {
  FILE *f;
  FILEBUFNC(f,item);
  Char name[_FNSIZE];
} catfile;

typedef Char datetimearray[datetimearraylength];

typedef struct changedata {
  Char changetype, baseold, basenew;
  long basecoo1, basecoo2, internal1, internal2;
  boolean insertasdeletion;

  long inserts;
  Char insert[insertmax];

  boolean insertcomplement;
} changedata;

typedef struct changeset {
  changedata data[changesetmax];
  long number;
} changeset;

Static _TEXT lib1, lib2, lib3;
Static catfile cat1, cat2, cat3;
Static _TEXT inst, listing, book, marksdelila, debug;
Static long libline[numlibfil];  /* location in the libfiles */
Static long firstlibrary;        /* the first library used.  this is the one from 
                                 which the parent date of the book originates in bwbookheader */
Static boolean versioninbook;    /* this variable allows delila to write its version into the
                                  book after the first organism name */
Static datetimearray datetime;
Static char catnumber;
Static long catline[numcatfil];
Static long currento, currentc, currentr, currentl;
Static boolean itemfound;
Static default_ def;
Static numberedstructure indnum;
Static step traversalchart[12][12];
Static step illegaltraversal;
Static node pastlibrary, pastbook, pastcheck;
Static Char nodechar[12];
Static Char nodeletter;
Static boolean debugging;
Static boolean withused;
Static long booksize;
Static line *freeline;
Static dnastring *freedna;
Static boolean readnumber;
Static long number;
Static boolean numbered, skipunnum;
Static marker *freemarker;
Static sitestring *freesite;
Static Char ch, firstch, blank;
Static boolean skipping;
Static piece *libpie;
Static item libpieit;
Static long libpiefi;
Static long zerobase, zeroshift, zeroBS;
Static line *longname;
Static boolean longnameexists;
Static changeset mutations;
Static long mutposition1, mutposition2;
Static Char mutnotchar, mutischar;
Static changedata mutcd1, mutcd2;
Static name lastpiecename;
Static double insertupperbits, insertlowerbits, deleteupperbits,
	      deletelowerbits, changeupperbits, changelowerbits;

Static Void crash()
{
/* Crash the program by trying to open a nonexistant file.  This allows
tracing by the dbx program.  To use:  insert call into the halt program
or whereever a traceable stop is desired */
  _TEXT bogus;

  bogus.f = NULL;
  *bogus.name = '\0';
  printf(" program crash.\n");
  if (*bogus.name != '\0')
    bogus.f = fopen(bogus.name, "r");
  else
    rewind(bogus.f);
  if (bogus.f == NULL)
    _EscIO2(FileNotFound, bogus.name);
  RESETBUF(bogus.f, Char);
  fclose(bogus.f);
}

Static jmp_buf _JL1;

Static Void halt()
{
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}

Static Char decapitalize(c_)
Char c_;
{
  long n = c_;

  if (n >= 'A' && n <= 'Z')
    c_ = _tolower(n);
  else
    c_ = (Char)n;
  return c_;
}

Static Void getlineDelila(l)
line **l;
{
  if (freeline != NULL) {
    *l = freeline;
    freeline = freeline->next;
  } else
    *l = (line *)Malloc(sizeof(line));
  (*l)->length = 0;
  (*l)->next = NULL;
}


Static Void getdna(l)
dnastring **l;
{
  if (freedna != NULL) {
    *l = freedna;
    freedna = freedna->next;
  } else
    *l = (dnastring *)Malloc(sizeof(dnastring));
  (*l)->length = 0;
  (*l)->next = NULL;
}

/* clear procedures should be called each time the records are no longer needed
   failure to do this may result in a stack overflow. */
Static Void clearline(l)
line **l;
{
  line *lptr;

  if (*l == NULL)
    return;
  lptr = *l;
  *l = (*l)->next;
  lptr->next = freeline;
  freeline = lptr;
}

/* write a line to a file, with carriage return if
carriagereturn is true. */
Static Void writeline(afile, l, carriagereturn)
_TEXT *afile;
line *l;
boolean carriagereturn;
{
  long index, FORLIM;

  FORLIM = l->length;
  for (index = 0; index < FORLIM; index++)
    putc(l->letters[index], afile->f);
  if (carriagereturn)
    putc('\n', afile->f);
}

/* show the freedna list */
Static Void showfreedna()
{
  long counter = 0;
  dnastring *l;

  l = freedna;
  while (l != NULL) {
    counter++;
    printf("%ld", counter);
    printf(", length = %d\n", l->length);
    l = l->next;
  }
}

Static Void cleardna(l)
dnastring **l;
{
  dnastring *lptr;

  if (*l == NULL)
    return;
  lptr = *l;
  *l = (*l)->next;
  lptr->next = freedna;
  freedna = lptr;
}

/* clear the header h (remove lines to free storage) */
Static Void clearheader(h)
header *h;
{
  clearline(&h->fulnam);
  while (h->note != NULL)
    clearline(&h->note);
}

/* clear the dna of the piece */
Static Void clearpiece(p)
piece **p;
{
  while ((*p)->dna != NULL)
    cleardna(&(*p)->dna);
  clearheader(&(*p)->key.hea);
}

/* convert a character into a base */
Static base chartobase(ch)
Char ch;
{
  base Result;

  switch (ch) {
    case 'a':
      Result = a;
      break;
    case 'c':
      Result = c;
      break;
    case 'g':
      Result = g;
      break;
    case 't':
      Result = t;
      break;
  }
  return Result;
}

/* convert a base into a character */
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

/* take the complement of ba */
Static base complement(ba)
base ba;
{
  base Result;

  switch (ba) {
    case a:
      Result = t;
      break;
    case c:
      Result = g;
      break;
    case g:
      Result = c;
      break;
    case t:
      Result = a;
      break;
  }
  return Result;
}


Static Char chomplement(b)
Char b;
{
  return (basetochar(complement(chartobase(b))));
}

Static long pietoint(p, pie)
long p;
piece *pie;
{
  /* p is a coordinate on the piece.
   we want to transform p into a number
   from 1 to n: an internal coordinate system for
   easy manipulation of piece coordinates */
  long i;
  piekey *WITH;

  WITH = &pie->key;
  switch (WITH->piedir) {

  case plus:
    if (p >= WITH->piebeg)
      i = p - WITH->piebeg + 1;
    else
      i = p - WITH->coobeg + WITH->cooend - WITH->piebeg + 2;
    break;

  case minus:
    if (p <= WITH->piebeg)
      i = WITH->piebeg - p + 1;
    else
      i = WITH->cooend - p + WITH->piebeg - WITH->coobeg + 2;
    break;
  }
  return i;
}

Static long inttopie(i, pie)
long i;
piece *pie;
{
  /** i is in the range 1 to some maximum.  it is an internal coordinate
   system for the program.  we want to do a
   coordinate transformation to obtain
   a value in the range of the piece called pie:
   i=1 corresponds to piebeg and
   i=its maximum corresponds to pieend **/
  long p;
  piekey *WITH;

  WITH = &pie->key;
  switch (WITH->piedir) {

  case plus:
    p = WITH->piebeg + i - 1;
    if (p > WITH->cooend) {
      if (WITH->coocon == circular)
	p += WITH->coobeg - WITH->cooend - 1;
    }
    break;

  case minus:
    p = WITH->piebeg - i + 1;
    if (p < WITH->coobeg) {
      if (WITH->coocon == circular)
	p += WITH->cooend - WITH->coobeg + 1;
    }
    break;
  }
  return p;
}

/* return the length of the dna in pie */
Static long piecelength(pie)
piece *pie;
{
  return (pietoint(pie->key.pieend, pie));
}

Static Char getto(thefile, theline, ch)
_TEXT *thefile;
long *theline;
long *ch;
{
/*search the file for a character in the first line which is a member of the set ch.
Note:  on 1999 March 10 the definition of this function was cleaned
up.  Instead of putting thefile on the line AFTER the charcter ch
has been found, it puts thefile ON the line.  Other routines like
brdna and brpiece have to move to the next line themselves.  This makes
getto give the OBJECT.*/
  Char achar = ' ';
  boolean done = false;

  while (!done) {
    if (BUFEOF(thefile->f)) {
      done = true;
      break;
    }
    achar = P_peek(thefile->f);
    if (P_inset(achar, ch)) {
      done = true;
      break;
    }
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    (*theline)++;
  }
  if (P_inset(achar, ch))
    return achar;
  else {
    return ' ';
    /**/
  }
}


Static Void skipstar(thefile)
_TEXT *thefile;
{
  if (BUFEOF(thefile->f)) {
    printf(" procedure skipstar: end of book found\n");
    halt();
    return;
  }
  if (P_peek(thefile->f) != '*') {
    printf(" procedure skipstar: bad book\n");
    printf(" \"*\" expected as first character on the line, but \"%c\" was found\n",
	   P_peek(thefile->f));
    halt();
  }
  getc(thefile->f);
  if (P_peek(thefile->f) != ' ') {
    printf(" procedure skipstar: bad book\n");
    printf(" \"* \" expected on a line but \"*%c\" was found\n",
	   P_peek(thefile->f));
    halt();
  }
  getc(thefile->f);
}

/* read a name from the file */
Static Void brreanum(thefile, theline, reanum)
_TEXT *thefile;
long *theline;
double *reanum;
{
  skipstar(thefile);
  fscanf(thefile->f, "%lg%*[^\n]", reanum);
  getc(thefile->f);
  (*theline)++;
}

Static Void brnumber(thefile, theline, num)
_TEXT *thefile;
long *theline, *num;
{
  skipstar(thefile);
  fscanf(thefile->f, "%ld%*[^\n]", num);
  getc(thefile->f);
  (*theline)++;
}

Static Void brname(thefile, theline, nam)
_TEXT *thefile;
long *theline;
name *nam;
{
  long i;
  Char c_;

  skipstar(thefile);
  nam->length = 0;
  do {
    nam->length++;
    c_ = getc(thefile->f);
    if (c_ == '\n')
      c_ = ' ';
    nam->letters[nam->length - 1] = c_;
  } while (!(P_eoln(thefile->f) || nam->length >= namelength ||
	     nam->letters[nam->length - 1] == ' '));
  if (nam->letters[nam->length - 1] == ' ')
    nam->length--;
  if (nam->length < namelength) {
    for (i = nam->length; i < namelength; i++)
      nam->letters[i] = ' ';
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

Static Void brline(thefile, theline, l)
_TEXT *thefile;
long *theline;
line **l;
{
  long i = 0;
  Char acharacter;

  skipstar(thefile);
  while (!P_eoln(thefile->f)) {
    i++;
    acharacter = getc(thefile->f);
    if (acharacter == '\n')
      acharacter = ' ';
    (*l)->letters[i-1] = acharacter;
  }
  (*l)->length = i;
  (*l)->next = NULL;
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

Static Void brdirect(thefile, theline, direct)
_TEXT *thefile;
long *theline;
direction *direct;
{
  Char ch;

  skipstar(thefile);
  fscanf(thefile->f, "%c%*[^\n]", &ch);
  getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  (*theline)++;
  if (ch == '+')
    *direct = plus;
  else
    *direct = minus;
}

/* read a configuration */
Static Void brconfig(thefile, theline, config)
_TEXT *thefile;
long *theline;
configuration *config;
{
  Char ch;

  skipstar(thefile);
  fscanf(thefile->f, "%c%*[^\n]", &ch);
  getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  (*theline)++;
  if (ch == 'l')
    *config = linear;
  else
    *config = circular;
}

Static Void brnotenumber(thefile, theline, note)
_TEXT *thefile;
long *theline;
line **note;
{
  /** book note reading to obtain the number of the object.
the procedure returns the value of the number as a global.
(this is not such a good practice, but we are stuck with it for now.) *)
begin (* brnotenumber **/
  *note = NULL;
  numbered = false;
  number = 0;
  /**/

  if (P_peek(thefile->f) != 'n')
    return;
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
  if (P_peek(thefile->f) != 'n') {
    skipstar(thefile);
    if (!P_eoln(thefile->f)) {
      if (P_peek(thefile->f) == '#') {
	numbered = true;
	getc(thefile->f);
	fscanf(thefile->f, "%ld", &number);
      }
    }
    do {
      fscanf(thefile->f, "%*[^\n]");
      getc(thefile->f);
      (*theline)++;
    } while (P_peek(thefile->f) != 'n');
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    (*theline)++;
    return;
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}



Static Void brnote(thefile, theline, note)
_TEXT *thefile;
long *theline;
line **note;
{
  line *newnote, *previousnote;

  *note = NULL;
  if (P_peek(thefile->f) != 'n')
    return;
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
  if (P_peek(thefile->f) != 'n') {
    getlineDelila(note);
    newnote = *note;
    while (P_peek(thefile->f) != 'n') {
      brline(thefile, theline, &newnote);
      previousnote = newnote;

      getlineDelila(&newnote->next);
      newnote = newnote->next;
    }

    clearline(&newnote);
    previousnote->next = NULL;
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    (*theline)++;
    return;
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

Static Void brheader(thefile, theline, hea)
_TEXT *thefile;
long *theline;
header *hea;
{
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;

  brname(thefile, theline, &hea->keynam);

  getlineDelila(&hea->fulnam);
  brline(thefile, theline, &hea->fulnam);

  if (readnumber)
    brnotenumber(thefile, theline, &hea->note);
  else
    brnote(thefile, theline, &hea->note);
}

Static Void copyheader(fromhea, tohea)
header fromhea, *tohea;
{
  /**/
  memcpy(tohea->keynam.letters, fromhea.keynam.letters, sizeof(alpha));
  tohea->keynam.length = fromhea.keynam.length;
  tohea->note = fromhea.note;
  tohea->fulnam = fromhea.fulnam;
}

Static Void brpiekey(thefile, theline, pie)
_TEXT *thefile;
long *theline;
piekey *pie;
{
  brheader(thefile, theline, &pie->hea);
  brreanum(thefile, theline, &pie->mapbeg);
  brconfig(thefile, theline, &pie->coocon);
  brdirect(thefile, theline, &pie->coodir);
  brnumber(thefile, theline, &pie->coobeg);
  brnumber(thefile, theline, &pie->cooend);
  brconfig(thefile, theline, &pie->piecon);
  brdirect(thefile, theline, &pie->piedir);
  brnumber(thefile, theline, &pie->piebeg);
  brnumber(thefile, theline, &pie->pieend);
}

/* read in dna from thefile, track the line *)
(* note: if the dna were circularized, by linking the last dnastring
to the first, then the cleardna routine could not clear properly,
and would loop forever... there is no reason to do that, since a simple
mod function will allow one to access the circle. */
Static Void brdna(thefile, theline, dna)
_TEXT *thefile;
long *theline;
dnastring **dna;
{
  Char ch;
  dnastring *workdna;
  long SET[5];
  long TEMP;

  getdna(dna);
  workdna = *dna;
  ch = getto(thefile, theline, P_addset(P_expset(SET, 0L), 'd'));
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
  ch = getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  while (ch == '*') {
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
    do {
      ch = getc(thefile->f);
      if (ch == '\n')
	ch = ' ';
      if (ch == 't' || ch == 'g' || ch == 'c' || ch == 'a') {
	if (workdna->length == dnamax) {
	  getdna(&workdna->next);
	  workdna = workdna->next;
	}
	workdna->length++;
	TEMP = workdna->length - 1;
	P_clrbits_B(workdna->part, TEMP, 1, 3);
	P_putbits_UB(workdna->part, TEMP, (int)chartobase(ch), 1, 3);
      }
    } while (!P_eoln(thefile->f));
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    (*theline)++;
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

/* read in a piece, change theline to reflect the lines traversed */
Static Void brpiece(thefile, theline, pie)
_TEXT *thefile;
long *theline;
piece **pie;
{
/**/
  brpiekey(thefile, theline, &(*pie)->key);
  if (numbered || !skipunnum)
    brdna(thefile, theline, &(*pie)->dna);
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  (*theline)++;
}

/* check that the book is ok to read, and
set up the global variables for br routines */
Static Void brinit(book, theline)
_TEXT *book;
long *theline;
{
  if (*book->name != '\0') {
    if (book->f != NULL)
      book->f = freopen(book->name, "r", book->f);
    else
      book->f = fopen(book->name, "r");
  } else
    rewind(book->f);
  if (book->f == NULL)
    _EscIO2(FileNotFound, book->name);
  RESETBUF(book->f, Char);
  if (!BUFEOF(book->f)) {
    if (P_peek(book->f) != '*') {
      if (P_peek(book->f) != 'h')
	printf(" this is not the first line of a book:\n");
      else
	printf(" bad book:\n");
      putchar(' ');

      while (!(P_eoln(book->f) | BUFEOF(book->f))) {
	putchar(P_peek(book->f));
	getc(book->f);
      }
      putchar('\n');
      halt();
    }
  }  else {
    printf(" book is empty\n");
    halt();
  }
  freeline = NULL;
  freedna = NULL;
  readnumber = true;
  number = 0;
  numbered = false;
  skipunnum = false;
  *theline = 1;
}

/* move to and read in the next piece in the book */
Static Void getpiece(thefile, theline, pie)
_TEXT *thefile;
long *theline;
piece **pie;
{
  Char ch;
  long SET[5];
  ch = getto(thefile, theline, P_addset(P_expset(SET, 0L), 'p'));
  if (ch != ' ') {
    brpiece(thefile, theline, pie);
  } else
    clearpiece(pie);
}

Static Void brorgkey(thefile, theline, org)
_TEXT *thefile;
long *theline;
orgkey *org;
{
  brheader(thefile, theline, &org->hea);
  getlineDelila(&org->mapunit);
  brline(thefile, theline, &org->mapunit);
}

Static Void brchrkey(thefile, theline, chr)
_TEXT *thefile;
long *theline;
chrkey *chr;
{
  brheader(thefile, theline, &chr->hea);
  brreanum(thefile, theline, &chr->mapbeg);
  brreanum(thefile, theline, &chr->mapend);
}

/* start a line of output to the book */
Static Void bwstartline(book)
_TEXT *book;
{
  fprintf(book->f, "* ");
}

/* write a line to the book */
Static Void bwline(book, l)
_TEXT *book;
line *l;
{
  long i, FORLIM;

  if (l == NULL)
    return;
  bwstartline(book);
  if (l->length != 0) {
    FORLIM = l->length;
    for (i = 0; i < FORLIM; i++)
      putc(l->letters[i], book->f);
  }
  putc('\n', book->f);
}

/* write a set of lines to the book */
Static Void bwtext(book, lines)
_TEXT *book;
line *lines;
{
  line *l = lines;

  while (l != NULL) {
    bwline(book, l);
    l = l->next;
  }
}
/* writes the notes pointed to by 'note' to 'book' */
Static Void bwnote(book, note)
_TEXT *book;
line *note;
{
  if (note == NULL)
    return;
  fprintf(book->f, "note\n");
  bwtext(book, note);
  fprintf(book->f, "note\n");
}
/* write a real number to the book */
Static Void bwnumber(book, num)
_TEXT *book;
long num;
{
  bwstartline(book);
  fprintf(book->f, "%ld\n", num);
}

/* write a real number to the book */
Static Void bwreanum(book, reanum)
_TEXT *book;
double reanum;
{
  bwstartline(book);
  fprintf(book->f, "%1.2f\n", reanum);
}

/* write a state to the book */
Static Void bwstate(book, sta)
_TEXT *book;
state sta;
{
  bwstartline(book);
  switch (sta) {

  case on:
    fprintf(book->f, "on\n");
    break;

  case off:
    fprintf(book->f, "off\n");
    break;
  }
}
/* write a name to the book */
Static Void bwname(book, nam)
_TEXT *book;
name nam;
{
  long i;

  bwstartline(book);
  for (i = 0; i < nam.length; i++)
    putc(nam.letters[i], book->f);
  putc('\n', book->f);
}
/* write a direction to the book */
Static Void bwdirect(book, direct)
_TEXT *book;
direction direct;
{
  bwstartline(book);
  switch (direct) {
    case plus:
      fprintf(book->f, "+\n");
      break;
    case minus:
      fprintf(book->f, "-\n");
      break;
  }
}
/* write a configuration to the book */
Static Void bwconfig(book, config)
_TEXT *book;
configuration config;
{
  bwstartline(book);
  switch (config) {
    case linear:
      fprintf(book->f, "linear\n");
      break;
    case circular:
      fprintf(book->f, "circular\n");
      break;
  }
}

Static Void bwheader(book, hea)
_TEXT *book;
header hea;
{
  if (def.reversenames == off) {
    bwname(book, hea.keynam);
    bwline(book, hea.fulnam);
  }  else {
    if (hea.fulnam->length > 0) {
      bwline(book, hea.fulnam);
      bwname(book, hea.keynam);
    } else {
      bwname(book, hea.keynam);
      bwname(book, hea.keynam);
    }
  }
  bwnote(book, hea.note);
}

Static Void bworgkey(book, org)
_TEXT *book;
orgkey org;
{
  bwheader(book, org.hea);
  bwline(book, org.mapunit);
}

Static Void bwchrkey(book, chr)
_TEXT *book;
chrkey chr;
{
  bwheader(book, chr.hea);
  bwreanum(book, chr.mapbeg);
  bwreanum(book, chr.mapend);
}

Static Void bworg(thefile, org, chropen, orgopen)
_TEXT *thefile;
orgkey org;
boolean *chropen, *orgopen;
{
  if (*chropen) {
    fprintf(thefile->f, "chromosome\n");
    *chropen = false;
  }
  if (*orgopen)
    fprintf(thefile->f, "organism\n");
  fprintf(thefile->f, "organism\n");
  bworgkey(&book, org);
  *orgopen = true;
}

Static Void bwchr(thefile, chr, chropen)
_TEXT *thefile;
chrkey chr;
boolean *chropen;
{
  if (*chropen)
    fprintf(thefile->f, "chromosome\n");
  fprintf(thefile->f, "chromosome\n");
  bwchrkey(&book, chr);
  *chropen = true;
}

Static Void bwdna(thefile, d)
_TEXT *thefile;
dnastring *d;
{
  long i, l;
  boolean newline = true;
  long FORLIM;

  fprintf(thefile->f, "dna\n");
  while (d != NULL) {
    FORLIM = d->length;
    for (i = 1; i <= FORLIM; i++) {
      if (newline) {
	bwstartline(thefile);
	l = 0;
	newline = false;
      }
      fputc(basetochar((base)P_getbits_UB(d->part, i - 1, 1, 3)), thefile->f);
      l++;
      if (l % 60 == 0 || i == d->length && d->next == NULL) {
	putc('\n', thefile->f);
	newline = true;
      }
    }
    d = d->next;
  }
  if (!newline)
    putc('\n', thefile->f);
  fprintf(thefile->f, "dna\n");
}

Static Void bwpie(thefile, pie)
_TEXT *thefile;
piece *pie;
{
  fprintf(thefile->f, "piece\n");
  bwheader(thefile, pie->key.hea);

  bwstartline(thefile);
  fprintf(thefile->f, "%1.2f\n", pie->key.mapbeg);

  bwstartline(thefile);
  if (pie->key.coocon == circular)
    fprintf(thefile->f, "circular\n");
  else
    fprintf(thefile->f, "linear\n");

  bwstartline(thefile);
  if (pie->key.coodir == plus)
    fprintf(thefile->f, "+\n");
  else
    fprintf(thefile->f, "-\n");

  bwstartline(thefile);
  fprintf(thefile->f, "%ld\n", pie->key.coobeg);

  bwstartline(thefile);
  fprintf(thefile->f, "%ld\n", pie->key.cooend);

  bwstartline(thefile);
  if (pie->key.piecon == circular)
    fprintf(thefile->f, "circular\n");
  else
    fprintf(thefile->f, "linear\n");

  bwstartline(thefile);
  if (pie->key.piedir == plus)
    fprintf(thefile->f, "+\n");
  else
    fprintf(thefile->f, "-\n");

  bwstartline(thefile);
  fprintf(thefile->f, "%ld\n", pie->key.piebeg);

  bwstartline(thefile);
  fprintf(thefile->f, "%ld\n", pie->key.pieend);
  bwdna(thefile, pie->dna);
  fprintf(thefile->f, "piece\n");
}

Static Void bwref(book, ref)
_TEXT *book;
reference ref;
{
  bwname(book, ref.pienam);
  bwreanum(book, ref.mapbeg);
  bwdirect(book, ref.refdir);
  bwnumber(book, ref.refbeg);
  bwnumber(book, ref.refend);
}

Static Void bwgen(thefile, gene)
_TEXT *thefile;
genkey gene;
{
  fprintf(thefile->f, "gene\n");
  bwheader(thefile, gene.hea);
  bwref(thefile, gene.ref);
  fprintf(thefile->f, "gene\n");
}

Static Void bwtra(thefile, trans)
_TEXT *thefile;
trakey trans;
{
  fprintf(thefile->f, "transcript\n");
  bwheader(thefile, trans.hea);
  bwref(thefile, trans.ref);
  fprintf(thefile->f, "transcript\n");
}

Static Void bwmar(thefile, mark)
_TEXT *thefile;
marker mark;
{
  long i, FORLIM;

  fprintf(thefile->f, "marker\n");
  bwheader(thefile, mark.key.hea);
  bwref(thefile, mark.key.ref);
  bwstate(thefile, mark.key.sta);
  bwstartline(thefile);
  FORLIM = mark.key.phenotype->length;
  for (i = 0; i < FORLIM; i++)
    putc(mark.key.phenotype->letters[i], thefile->f);
  putc('\n', thefile->f);
  bwdna(thefile, mark.dna);
  fprintf(thefile->f, "marker\n");
}

/* no wrap version of pietoint *)
 p is a coordinate on the piece.
   we want to transform p into a number
   from 1 to n: an internal coordinate system for
   easy manipulation of piece coordinates.  If the coordinate
   is off the end, bring it to just before the end. */

Static long nwpietoint(p, pie)
long p;
piece *pie;
{
  long i;
  piekey *WITH;

  WITH = &pie->key;
  switch (WITH->piedir) {

  case plus:
    i = p - WITH->piebeg + 1;
    break;

  case minus:
    i = WITH->piebeg - p + 1;
    break;
  }
  if (i > piecelength(pie))
    i = piecelength(pie) + 1;
  if (i < 0)
    i = 0;
  /**/
  return i;
}

Static Void tvrslibrary(future)
node future;
{
  step thisstep;

  memcpy(thisstep, traversalchart[(long)pastlibrary]
	 [(long)future], sizeof(step));
  if (!strncmp(thisstep, illegaltraversal, sizeof(step))) {
    printf(" program error:  illegal library traversal %12d to %12d\n",
	   (int)pastlibrary, (int)future);
    halt();
  }
  pastlibrary = future;
  nodeletter = nodechar[(long)future];
}

Static Void grabitem(cat, it)
catfile *cat;
item *it;
{
  *it = GETFBUF(cat->f, item);
  if (!BUFEOF(cat->f))
    GET(cat->f, item);
}

Static Void crcatheader(cat, catnum, alp)
catfile *cat;
long catnum;
Char *alp;
{
  item it;
  long i;

  if (*cat->name != '\0') {
    if (cat->f != NULL)
      cat->f = freopen(cat->name, "rb", cat->f);
    else
      cat->f = fopen(cat->name, "rb");
  } else
    rewind(cat->f);
  if (cat->f == NULL)
    _EscIO2(FileNotFound, cat->name);
  RESETBUF(cat->f, item);
  if (BUFEOF(cat->f))
    return;

  grabitem(cat, &it);
  for (i = 0; i < datetimearraylength; i++)
    alp[i] = it.nam.letters[i];
  catline[catnum-1] = 2;
}

/* this procedure finds the next item in the files,
   returns that item and the new current line and
   makes itemfound = true.  if itemfound is false after calling
   this routine it means that the last item was the end of last file.
      by gary stormo aug 28,1979
      modified by tom schneider 79 sep 7 */
Static Void nextitem(newitem)
item *newitem;
{
  itemfound = false;
  switch (catnumber) {
    case 1:
      if (!BUFEOF(cat1.f)) {
        grabitem(&cat1, newitem);
        itemfound = true;
      }
      break;
    case 2:
      if (!BUFEOF(cat2.f)) {
        grabitem(&cat2, newitem);
        itemfound = true;
      }
      break;
    case 3:
      if (!BUFEOF(cat3.f)) {
        grabitem(&cat3, newitem);
        itemfound = true;
      }
      break;
    }
    if (itemfound) {
      catline[catnumber-1]++;
      return;
    }
  switch (catnumber) {
    case 1:
      catnumber = 2;
      if (*cat2.name != '\0') {
        if (cat2.f != NULL)
    cat2.f = freopen(cat2.name, "rb", cat2.f);
        else
    cat2.f = fopen(cat2.name, "rb");
      } else
        rewind(cat2.f);
      if (cat2.f == NULL)
        _EscIO2(FileNotFound, cat2.name);
      RESETBUF(cat2.f, item);
      if (!BUFEOF(cat2.f)) {
        grabitem(&cat2, newitem);
        grabitem(&cat2, newitem);
        itemfound = true;
      }
      break;
    case 2:
      catnumber = 3;
      if (*cat3.name != '\0') {
        if (cat3.f != NULL)
    cat3.f = freopen(cat3.name, "rb", cat3.f);
        else
    cat3.f = fopen(cat3.name, "rb");
      } else
        rewind(cat3.f);
      if (cat3.f == NULL)
        _EscIO2(FileNotFound, cat3.name);
      RESETBUF(cat3.f, item);
      if (!BUFEOF(cat3.f)) {
        grabitem(&cat3, newitem);
        grabitem(&cat3, newitem);
        itemfound = true;
      }
      break;
    case 3:
      /* blank case */
      break;
  }
  catline[catnumber-1] = 2;
}

/* Local variables for finditem: */
struct LOC_finditem {
  node newnode;
  name n;
  item *newitem;
  long *fi, numsearched;
} ;

/* Local variables for search: */
struct LOC_search {
  struct LOC_finditem *LINK;
  catfile *cat;
} ;

/**/
Local Void readcat(LINK)
struct LOC_search *LINK;
{
  grabitem(LINK->cat, LINK->LINK->newitem);
  catline[catnumber-1]++;
  /**/
}

Local Void find(LINK)
struct LOC_search *LINK;
{
  readcat(LINK);
  if (LINK->LINK->newitem->letter != nodeletter)
    return;
  if (strncmp(LINK->LINK->newitem->nam.letters, LINK->LINK->n.letters,
	      sizeof(alpha)))
    return;
  if (debugging)
    fprintf(debug.f, "found %c %.*s\n",
	    LINK->LINK->newitem->letter, namelength,
	    LINK->LINK->newitem->nam.letters);
  itemfound = true;

  *LINK->LINK->fi = catnumber;

  switch (nodeletter) {
    case 'o':
      currento = catline[catnumber-1];
      break;
    case 'c':
      currentc = catline[catnumber-1];
      break;
    case 'r':
      currentr = catline[catnumber-1];
      break;
    case 't':
    case 'm':
    case 'g':
    case 'p':
    case 'e':
      currentl = catline[catnumber-1];
      break;
  }
}

Local Void searchbetween(start, stop, LINK)
long start, stop;
struct LOC_search *LINK;
{
  if (*LINK->cat->name != '\0') {
    if (LINK->cat->f != NULL)
      LINK->cat->f = freopen(LINK->cat->name, "rb", LINK->cat->f);
    else
      LINK->cat->f = fopen(LINK->cat->name, "rb");
  } else
    rewind(LINK->cat->f);
  if (LINK->cat->f == NULL)
    _EscIO2(FileNotFound, LINK->cat->name);
  RESETBUF(LINK->cat->f, item);
  catline[catnumber-1] = 1;
  readcat(LINK);

  while (catline[catnumber-1] < start)
    readcat(LINK);

  while (!itemfound && catline[catnumber-1] < stop)
    find(LINK);
}

/**/
Local Void search(cat_, LINK)
catfile *cat_;
struct LOC_finditem *LINK;
{
  struct LOC_search V;
  boolean stuck = false;
  V.LINK = LINK;
  V.cat = cat_;
  /**/
  if (debugging)
    fprintf(debug.f, "search cat %d\n", catnumber);
  itemfound = false;
  tvrslibrary(LINK->newnode);
  while (((!itemfound) & (!BUFEOF(V.cat->f))) && !stuck) {
    switch (nodeletter) {
      case 'o':
      case 'r':
        find(&V);
        break;
      case 'c':
      case 'e':
        find(&V);
        if (LINK->newitem->letter == 'r' || LINK->newitem->letter == 'o')
    stuck = true;
        break;
      case 't':
      case 'm':
      case 'g':
      case 'p':
        find(&V);
        if (LINK->newitem->letter == 'c' || LINK->newitem->letter == 'r' ||
      LINK->newitem->letter == 'o')
    stuck = true;
        break;
    }
  }

  if (itemfound)
    return;
  switch (nodeletter) {
    case 'c':
      searchbetween(currento, currentc, &V);
      break;
    case 'e':
      searchbetween(currentr, currentl, &V);
      break;
    case 't':
    case 'm':
    case 'g':
    case 'p':
      searchbetween(currentc, currentl, &V);
      break;
    case 'o':
    case 'r':
      while (LINK->numsearched <= numcatfil && !itemfound) {
        LINK->numsearched++;
        catnumber = catnumber % numcatfil + 1;
        catline[catnumber-1] = 1;

        switch (catnumber) {
          case 1:
            if (*cat1.name != '\0') {
              if (cat1.f != NULL)
                cat1.f = freopen(cat1.name, "rb", cat1.f);
              else
                cat1.f = fopen(cat1.name, "rb");
            } else
              rewind(cat1.f);
            if (cat1.f == NULL)
              _EscIO2(FileNotFound, cat1.name);
            RESETBUF(cat1.f, item);
            search(&cat1, LINK);
            break;
    case 2:
          if (*cat2.name != '\0') {
            if (cat2.f != NULL)
              cat2.f = freopen(cat2.name, "rb", cat2.f);
            else
              cat2.f = fopen(cat2.name, "rb");
          } else
            rewind(cat2.f);
          if (cat2.f == NULL)
            _EscIO2(FileNotFound, cat2.name);
          RESETBUF(cat2.f, item);
          search(&cat2, LINK);
	break;
      case 3:
        if (*cat3.name != '\0') {
          if (cat3.f != NULL)
            cat3.f = freopen(cat3.name, "rb", cat3.f);
          else
            cat3.f = fopen(cat3.name, "rb");
        } else
          rewind(cat3.f);
        if (cat3.f == NULL)
          _EscIO2(FileNotFound, cat3.name);
        RESETBUF(cat3.f, item);
        search(&cat3, LINK);
	break;
      }
    }
    break;
  }
}

/* procedure to search the catalogue for the requested item,
   returns the file number (fi) and line number of the request in the library.
   by gary stormo,  aug 28, 1979
   modified by tom schneider 79 sep 5 
  this version assumes that no organism or recognition has been
   split in two between two files */
Static Void finditem(newnode_, n_, newitem_, fi_)
node newnode_;
name n_;
item *newitem_;
long *fi_;
{
  /**/
  
  struct LOC_finditem V;

  V.newnode = newnode_;
  V.n = n_;
  V.newitem = newitem_;
  V.fi = fi_;
  V.numsearched = 1;
  switch (catnumber) {
    case 1:
      search(&cat1, &V);
      break;
    case 2:
      search(&cat2, &V);
      break;
    case 3:
      search(&cat3, &V);
      break;
  }
}

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
  
  if (adatetime[4] == '/' && adatetime[7] == '/' && adatetime[13] == ':' &&
      adatetime[16] == ':')
    return;
  printf("readdatetime: bad date time read:\n");
  for (index = 0; index < datetimearraylength; index++)
    putchar(adatetime[index]);
  putchar('\n');
  halt();
}

Static Void writedatetime(thefile, adatetime)
_TEXT *thefile;
Char *adatetime;
{
  long index;

  for (index = 0; index < datetimearraylength; index++)
    putc(adatetime[index], thefile->f);
}

Static Void addtoseed(seed, power, c_)
double *seed, *power;
Char c_;
{
  long n;

  *power /= 10;
  /**/
  n = c_ - '0';
  if ((unsigned long)n > 9) {
    printf("timeseed: error in datetime\n");
    printf("it contains \"%c\" which is not a number.\n", c_);
    printf("The getdatetime routine must be fixed.\n");
    halt();
  }
  *seed += *power * n;
}


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

Static Void timeseed(seed)
double *seed;
{
  datetimearray adatetime;
  getdatetime(adatetime);
  makeseed(adatetime, seed);
}

Static Void limitdate(a_, b, c_, d, limitdatetime_)
Char a_, b, c_, d;
Char *limitdatetime_;
{
  datetimearray limitdatetime, adatetime;
  double Dday, now;
  memcpy(limitdatetime, limitdatetime_, sizeof(datetimearray));
  getdatetime(adatetime);
  orderseedDelila(adatetime, &now);

  if (limitdatetime[0] != ' ' || limitdatetime[1] != ' ' ||
      limitdatetime[2] != ' ' || limitdatetime[3] != ' ')
    halt();

  limitdatetime[0] = a_;
  limitdatetime[1] = b;
  limitdatetime[2] = c_;
  limitdatetime[3] = d;

  orderseedDelila(limitdatetime, &Dday);

  if (now <= Dday)
    return;
  printf("This program expired on %.*s\n", datetimearraylength, limitdatetime);
  printf("See: https://alum.mit.edu/www/toms/walker/contacts.html\n");
  halt();
}

Static Void clearname(n)
name n;
{
  long i;

  n.length = 0;
  for (i = 0; i < namelength; i++)
    n.letters[i] = ' ';
}


Static Void writename(f, n)
_TEXT *f;
name n;
{
  long i;

  for (i = 0; i < n.length; i++)
    putc(n.letters[i], f->f);
}


Static Void copyname(a_, b)
name a_, *b;
{
  long i;

  for (i = 0; i < a_.length; i++)
    b->letters[i] = a_.letters[i];
  b->length = a_.length;
}


Static boolean equalname(a_, b)
name a_, b;
{
  long i = 1;
  boolean same = true;

  if (b.length != a_.length)
    return false;
  while (same && i <= a_.length) {
    same = (b.letters[i-1] == a_.letters[i-1]);
    i++;
/* p2c: delila.p: Note: Eliminated unused assignment statement [338] */
  }
  return same;
}

Static Void emptydna(l)
dnastring **l;
{
  while (*l != NULL)
    cleardna(l);
}

Static Void emptyline(l)
line **l;
{
  while (*l != NULL)
    clearline(l);
}

Static Void copyline(fromline, toline)
line *fromline, **toline;
{
  line *f, *t_;

  emptyline(toline);
  if (fromline == NULL)
    return;
  getlineDelila(toline);
  f = fromline;
  t_ = *toline;
  while (f != NULL) {
    memcpy(t_->letters, f->letters, (long)linelength);
    t_->length = f->length;
    f = f->next;
    if (f != NULL) {
      getlineDelila(&t_->next);
      t_ = t_->next;
    } else
      t_->next = NULL;
  }
}

Static Void copydna(fromdna, todna)
dnastring **fromdna, **todna;
{
  dnastring *aDNAptr, *memdna;

  while (*todna != NULL)
    cleardna(todna);
  aDNAptr = *fromdna;
  if (aDNAptr == NULL)
    return;
  getdna(&memdna);
  *todna = memdna;
  while (aDNAptr != NULL) {
    memcpy(memdna->part, aDNAptr->part, sizeof(seq));
    memdna->length = aDNAptr->length;
    aDNAptr = aDNAptr->next;
    if (aDNAptr != NULL) {
      getdna(&memdna->next);
      memdna = memdna->next;
    } else
      memdna->next = NULL;
  }
}

Static Void copypiece(Apiece, Bpiece)
piece *Apiece, **Bpiece;
{
  (*Bpiece)->key.hea.keynam = Apiece->key.hea.keynam;
  copyline(Apiece->key.hea.fulnam, &(*Bpiece)->key.hea.fulnam);
  copyline(Apiece->key.hea.note, &(*Bpiece)->key.hea.note);
  (*Bpiece)->key.mapbeg = Apiece->key.mapbeg;
  (*Bpiece)->key.coocon = Apiece->key.coocon;
  (*Bpiece)->key.coodir = Apiece->key.coodir;
  (*Bpiece)->key.coobeg = Apiece->key.coobeg;
  (*Bpiece)->key.cooend = Apiece->key.cooend;
  (*Bpiece)->key.piecon = Apiece->key.piecon;
  (*Bpiece)->key.piedir = Apiece->key.piedir;
  (*Bpiece)->key.piebeg = Apiece->key.piebeg;
  (*Bpiece)->key.pieend = Apiece->key.pieend;
  copydna(&Apiece->dna, &(*Bpiece)->dna);
}

Static boolean between(a_, b, c_)
long a_, b, c_;
{
  return (a_ <= b && b <= c_ || c_ <= b && b <= a_);
}

Static boolean within(pie, p)
piece *pie;
long p;
{
  boolean Result;
  piekey *WITH;

  WITH = &pie->key;
  switch (WITH->coocon) {
      case linear:
        Result = between(WITH->piebeg, p, WITH->pieend);
        break;
      case circular:
        switch (WITH->piecon) {
        case linear:
          switch (WITH->piedir) {
          case plus:
      if (WITH->pieend >= WITH->piebeg)
        Result = between(WITH->piebeg, p, WITH->pieend);
      else
        Result = between(WITH->piebeg, p, WITH->cooend) |
          between(WITH->coobeg, p, WITH->pieend);
      break;
          case minus:
      if (WITH->pieend <= WITH->piebeg)
        Result = between(WITH->piebeg, p, WITH->pieend);
      else
        Result = between(WITH->piebeg, p, WITH->coobeg) |
          between(WITH->cooend, p, WITH->pieend);
      break;
          }
          break;
        case circular:
          Result = between(WITH->coobeg, p, WITH->cooend);
          break;
        }
        break;
  }
  return Result;
}

Static boolean withininternal(pie, p)
piece *pie;
long p;
{
  return ((p >= 1) & (p <= piecelength(pie)));
}

Static Void showdnasegment(f, d, spot)
_TEXT *f;
dnastring *d;
long spot;
{
  long j, FORLIM;

  FORLIM = d->length;
  for (j = 1; j <= FORLIM; j++) {
    if (j == spot)
      putc('(', f->f);
    if (((1L << P_getbits_UB(d->part, j - 1, 1, 3)) & ((1L << ((long)a)) |
	   (1L << ((long)c)) | (1L << ((long)g)) | (1L << ((long)t)))) != 0)
      fputc(basetochar((base)P_getbits_UB(d->part, j - 1, 1, 3)), f->f);
    else
      fprintf(f->f, "%d", (int)((base)P_getbits_UB(d->part, j - 1, 1, 3)));
    if (j == spot)
      putc(')', f->f);
  }
  putc(' ', f->f);
}

Static Void showsegments(f, d)
_TEXT *f;
dnastring *d;
{
  dnastring *i = d;
  long n = 0;

  while (i != NULL) {
    showdnasegment(f, i, (long)i->length);
    n++;
    fprintf(f->f, " segment %ld\n", n);
    i = i->next;
  }
  putc('\n', f->f);
}

Static base getbase(position, pie)
long position;
piece *pie;
{
  dnastring *workdna;
  long p, spot, thelength;

  thelength = piecelength(pie);
  while (position < 1)
    position += thelength;
  while (position > thelength)
    position -= thelength;

  workdna = pie->dna;
  p = workdna->length;
  while (position > p) {
    workdna = workdna->next;
    if (workdna == NULL) {
      printf("error in function getbase!\n");
      halt();
    }
    p += workdna->length;
  }
   if (true) {
    spot = workdna->length - p + position;
     if (spot <= 0) {
      printf("error in getbase, spot (= %ld) must be positive\n", spot);
      halt();
    }
    if (spot > workdna->length) {
      printf("error in getbase, spot (=%ld) must be less than length (=%d)\n",
	     spot, workdna->length);
      halt();
    }

    return ((base)P_getbits_UB(workdna->part, spot - 1, 1, 3));
  }
  printf("error in getbase: request off end of piece\n");
  halt();
}


Static Void reduceposition(pie, p)
piece *pie;
long *p;
{
  long size;
  piekey *WITH;

  WITH = &pie->key;
  if (WITH->piecon == circular) {
    size = WITH->cooend - WITH->coobeg + 1;
    while (*p < WITH->coobeg)
      *p += size;
    while (*p > WITH->cooend)
      *p -= size;
    return;
  }

  switch (WITH->piedir) {
  case plus:
      if (*p < WITH->piebeg)
        *p = WITH->piebeg;
      else
        *p = WITH->pieend;
      break;
    case minus:
      if (*p < WITH->piebeg)
        *p = WITH->pieend;
      else
        *p = WITH->piebeg;
      break;
  }
}

Static Void getmarker(l)
marker **l;
{
  if (freemarker != NULL) {
    *l = freemarker;
    freemarker = freemarker->key.next;
  } else
    *l = (marker *)Malloc(sizeof(marker));
  (*l)->key.next = NULL;
}

Static Void clearmarker(l)
marker **l;
{
  marker *lptr;

  if (*l == NULL)
    return;
  lptr = *l;
  *l = (*l)->key.next;
  lptr->key.next = freemarker;
  freemarker = lptr;
}

Static Void getsite(l)
sitestring **l;
{
  if (freesite != NULL) {
    *l = freesite;
    freesite = freesite->next;
  } else
    *l = (sitestring *)Malloc(sizeof(sitestring));
  (*l)->length = 0;
  (*l)->next = NULL;
}

Static Void clearsite(l)
sitestring **l;
{
  sitestring *lptr;

  if (*l == NULL)
    return;
  lptr = *l;
  *l = (*l)->next;
  lptr->next = freesite;
  freesite = lptr;
}

Static boolean copylibname(tofile, lib, libnumber)
_TEXT *tofile, *lib;
long libnumber;
{
  Char ch;

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
  if (!BUFEOF(lib->f)) {
    fprintf(tofile->f, "%ld ", libnumber);
    while (!P_eoln(lib->f)) {
      ch = getc(lib->f);
      if (ch == '\n')
	ch = ' ';
      putc(ch, tofile->f);
    }
    fscanf(lib->f, "%*[^\n]");
    getc(lib->f);
    putc('\n', tofile->f);
    libline[libnumber-1] = 2;
    return true;
  } else
    return false;
}

Static Void checklib(thefile, chexpected, fi, li)
_TEXT *thefile;
Char chexpected;
long fi, li;
{
  firstch = P_peek(thefile->f);
  if (chexpected == firstch)
    return;

  printf(" delila: library does not match the catalogue\n");
  printf(" library %ld line %ld character expected: %c character found: %c\n",
	 fi, li, chexpected, firstch);
  printf(" (the order of the libraries and catalogues is probably wrong,\n");
  printf("  or the file is not a library)\n");
  halt();
}

Static Void libskipstar(thefile)
_TEXT *thefile;
{
  Char c_;

  if (skipping) {
    firstch = getc(thefile->f);
    if (firstch == '\n')
      firstch = ' ';
    if (firstch != '*') {
      printf(" delila: the file lib%d is bad.\n", catnumber);
      printf(" either line %ld is missing an asterisk (*) on the attribute\n",
	     libline[catnumber-1]);
      printf(" or the file is not a delila data base.\n");
      printf("The library line is:\n");
      putchar(firstch);
      while (!P_eoln(thefile->f)) {
	c_ = getc(thefile->f);
	if (c_ == '\n')
	  c_ = ' ';
	putchar(c_);
      }
      putchar('\n');
      halt();
    }
  } else
    skipping = true;
  blank = getc(thefile->f);
  if (blank == '\n')
    blank = ' ';
}

Static Void lrreanum(thefile, reanum)
_TEXT *thefile;
double *reanum;
{
  libskipstar(thefile);
  fscanf(thefile->f, "%lg%*[^\n]", reanum);
  getc(thefile->f);
}

Static Void lrnumber(thefile, num)
_TEXT *thefile;
long *num;
{
  libskipstar(thefile);
  fscanf(thefile->f, "%ld%*[^\n]", num);
  getc(thefile->f);
}

typedef Char ualpha[namelength];

Static Void lrname(thefile, nam)
_TEXT *thefile;
name *nam;
{
  long i;
  ualpha unamlets;

  libskipstar(thefile);
  nam->length = 0;
  while (!P_eoln(thefile->f) && nam->length < namelength) {
    nam->length++;
    unamlets[nam->length - 1] = getc(thefile->f);
    if (unamlets[nam->length - 1] == '\n')
      unamlets[nam->length - 1] = ' ';
  }
  memcpy(nam->letters, unamlets, sizeof(alpha));
  if (nam->length < namelength) {
    for (i = nam->length; i < namelength; i++)
      nam->letters[i] = ' ';
  }
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
}

Static Void lrline(thefile, l)
_TEXT *thefile;
line **l;
{
  long i = 0;
  long j;
  Char acharacter;
  long FORLIM;

  libskipstar(thefile);
  while (!P_eoln(thefile->f)) {
    i++;
    acharacter = getc(thefile->f);
    if (acharacter == '\n')
      acharacter = ' ';
    (*l)->letters[i-1] = acharacter;
  }
  if (i < (*l)->length) {
    FORLIM = (*l)->length;
    for (j = i; j < FORLIM; j++)
      (*l)->letters[j] = ' ';
  }
  (*l)->length = i;
  (*l)->next = NULL;
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
}

Static Void lrdirect(thefile, direct)
_TEXT *thefile;
direction *direct;
{
  libskipstar(thefile);
  fscanf(thefile->f, "%c%*[^\n]", &ch);
  getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == '+')
    *direct = plus;
  else
    *direct = minus;
}

Static Void lrconfig(thefile, config)
_TEXT *thefile;
configuration *config;
{
  libskipstar(thefile);
  fscanf(thefile->f, "%c%*[^\n]", &ch);
  getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == 'l')
    *config = linear;
  else
    *config = circular;
}

Static Void lrstate(thefile, sta)
_TEXT *thefile;
state *sta;
{
  libskipstar(thefile);
  ch = getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  if (ch != 'o') {
    printf("delila: in procedure lrstate: state does not start with o\n");
    halt();
  }
  fscanf(thefile->f, "%c%*[^\n]", &ch);
  getc(thefile->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == 'n')
    *sta = on;
  else
    *sta = off;
}

Static Void lrlibheader(lib, libnum, alp)
_TEXT *lib;
long libnum;
Char *alp;
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
  if (BUFEOF(lib->f))
    return;
  libskipstar(lib);
  readdatetime(lib, alp);
  fscanf(lib->f, "%*[^\n]");
  getc(lib->f);
  libline[libnum-1] = 2;
}

/* Local variables for lrheader: */
struct LOC_lrheader {
  _TEXT *thefile;
  long lines;
} ;

Local Void lrnote(note, LINK)
line **note;
struct LOC_lrheader *LINK;
{
  line *newnote, *previousnote;

  firstch = getc(LINK->thefile->f);
  if (firstch == '\n')
    firstch = ' ';
  if (firstch != 'n') {
    skipping = false;
    return;
  }
  fscanf(LINK->thefile->f, "%*[^\n]");
  getc(LINK->thefile->f);
  LINK->lines++;
  firstch = getc(LINK->thefile->f);
  if (firstch == '\n')
    firstch = ' ';
  if (firstch != 'n') {
    getlineDelila(note);
    newnote = *note;
    while (firstch != 'n') {
      skipping = false;
      lrline(LINK->thefile, &newnote);
      LINK->lines++;
      firstch = getc(LINK->thefile->f);
      if (firstch == '\n')
	firstch = ' ';
      previousnote = newnote;

      getlineDelila(&newnote->next);
      newnote = newnote->next;
    }

    clearline(&newnote);
    previousnote->next = NULL;
    fscanf(LINK->thefile->f, "%*[^\n]");
    getc(LINK->thefile->f);
    LINK->lines++;
    return;
  }
  fscanf(LINK->thefile->f, "%*[^\n]");
  getc(LINK->thefile->f);
  LINK->lines++;
}


Static Void lrheader(thefile_, fi, hea)
_TEXT *thefile_;
long fi;
header *hea;
{
  struct LOC_lrheader V;
  V.thefile = thefile_;
  clearheader(hea);
  lrname(V.thefile, &hea->keynam);
  getlineDelila(&hea->fulnam);
  lrline(V.thefile, &hea->fulnam);
  V.lines = 2;
  lrnote(&hea->note, &V);
  libline[fi-1] += V.lines;
}


Static Void lrreference(thefile, fi, ref)
_TEXT *thefile;
long fi;
reference *ref;
{
  lrname(thefile, &ref->pienam);
  lrreanum(thefile, &ref->mapbeg);
  lrdirect(thefile, &ref->refdir);
  lrnumber(thefile, &ref->refbeg);
  lrnumber(thefile, &ref->refend);
  libline[fi-1] += 5;
}


Static Void libgetto(thefile, fi, li)
_TEXT *thefile;
long fi, li;
{
  long currentline;
  datetimearray dummyname;
  currentline = libline[fi-1];

  if (currentline > li) {
    lrlibheader(thefile, fi, dummyname);
    currentline = libline[fi-1];
  }

  while (currentline < li) {
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    currentline++;
  }

  libline[fi-1] = currentline;
}

Static Void marksautomate(markspots)
_TEXT *markspots;
{
  fprintf(markspots->f,
	  "* marksdelila: define markings for the lister program\n");
  fprintf(markspots->f, "* written by Delila version %4.2f\n", version);
  fprintf(markspots->f,
	  "* The standard marks.arrow must be used prior to this file.\n\n");
  fprintf(markspots->f, "u\n");
  fprintf(markspots->f, "%% This message will appear if you forgot your\n");
  fprintf(markspots->f, "%% marks.arrow definition:\n");
  fprintf(markspots->f, "You_need_a_marks.arrow_definition\n\n");
  fprintf(markspots->f, "/setmarkspotarrow{\n");
  fprintf(markspots->f, "/bodycolor {black} def\n");
  fprintf(markspots->f, "/strokecolor {black} def\n");
  fprintf(markspots->f, "/BodyThick     0.30 fs def\n");
  fprintf(markspots->f, "/HeadWidth     0.90 fs def\n");
  fprintf(markspots->f, "/HeadLength    1.50 fs def\n");
  fprintf(markspots->f, "} def\n");
  fprintf(markspots->f, "setmarkspotarrow\n\n");
  fprintf(markspots->f,
	  "/change   {%% tailx taily headx heady shift change\n");
  fprintf(markspots->f, "%% the head of an arrow\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "setmarkspotarrow\n");
  fprintf(markspots->f, "fixedarrow\n");
  fprintf(markspots->f, "} def\n\n");
  fprintf(markspots->f,
	  "/changeworra{%% tailx taily headx heady shift changeworra\n");
  fprintf(markspots->f,
	  "%% the tail of an arrow is a 'worra' (spelling backards)\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "setmarkspotarrow\n");
  fprintf(markspots->f, "fixedworra\n");
  fprintf(markspots->f, "} def\n\n");
  fprintf(markspots->f,
	  "/insertion{%% tailx taily headx heady shift insertion\n");
  fprintf(markspots->f, "%% an insertion is a green rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "/bodycolor {lightgreen} def\n");
  fprintf(markspots->f, "boundrectangle\n");
  fprintf(markspots->f, "} def\n\n");
  fprintf(markspots->f,
	  "/deletion {%% tailx taily headx heady shift deletion\n");
  fprintf(markspots->f, "%% a deletion is a red rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "/bodycolor {lightred} def\n");
  fprintf(markspots->f, "boundrectangle\n");
  fprintf(markspots->f, "} def\n\n");

  fprintf(markspots->f,
	  "/fullinsertion{%% tailx taily headx heady shift insertion\n");
  fprintf(markspots->f, "%% an insertion is a green rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "{lightgreen} {black} fullbox\n");
  fprintf(markspots->f, "} def\n\n");

  fprintf(markspots->f,
	  "/leftinsertion{%% tailx taily headx heady shift insertion\n");
  fprintf(markspots->f, "%% an insertion is a green rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "{lightgreen} {black} leftbox\n");
  fprintf(markspots->f, "} def\n\n");

  fprintf(markspots->f,
	  "/midinsertion{%% tailx taily headx heady shift insertion\n");
  fprintf(markspots->f, "%% an insertion is a green rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "{lightgreen} {black} midbox\n");
  fprintf(markspots->f, "} def\n\n");

  fprintf(markspots->f,
	  "/rightinsertion{%% tailx taily headx heady shift insertion\n");
  fprintf(markspots->f, "%% an insertion is a green rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "{lightgreen} {black} rightbox\n");
  fprintf(markspots->f, "} def\n\n");

  fprintf(markspots->f,
	  "/fulldeletion {%% tailx taily headx heady shift deletion\n");
  fprintf(markspots->f, "%% a deletion is a red rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "{lightred} {black} fullbox\n");
  fprintf(markspots->f, "} def\n\n");

  fprintf(markspots->f,
	  "/leftdeletion {%% tailx taily headx heady shift deletion\n");
  fprintf(markspots->f, "%% a deletion is a red rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "{lightred} {black} leftbox\n");
  fprintf(markspots->f, "} def\n\n");

  fprintf(markspots->f,
	  "/middeletion {%% tailx taily headx heady shift deletion\n");
  fprintf(markspots->f, "%% a deletion is a red rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "{lightred} {black} midbox\n");
  fprintf(markspots->f, "} def\n\n");

  fprintf(markspots->f,
	  "/rightdeletion {%% tailx taily headx heady shift deletion\n");
  fprintf(markspots->f, "%% a deletion is a red rectangle\n");
  fprintf(markspots->f, "pop\n");
  fprintf(markspots->f, "{lightred} {black} rightbox\n");
  fprintf(markspots->f, "} def\n\n");

  fprintf(markspots->f, "!\n\n");
}

/* Local variables for initlibrarian: */
struct LOC_initlibrarian {
  long libcount;
} ;

Local Void checklibcat(lib, libnum, cat, catnum, LINK)
_TEXT *lib;
long libnum;
catfile *cat;
long catnum;
struct LOC_initlibrarian *LINK;
{
  datetimearray libheader, catheader;

  if (!BUFEOF(lib->f)) {
    if (firstlibrary == 0)
      firstlibrary = libnum;
    catnumber = catnum;
    lrlibheader(lib, libnum, libheader);
    crcatheader(cat, catnum, catheader);

    if (BUFEOF(cat->f)) {
      printf(" delila: missing catalogue %ld although library %ld is not empty\n",
	     catnum, libnum);
      halt();
    }

    if (strncmp(libheader, catheader, sizeof(datetimearray))) {
      printf(" delila: library %ld and catalogue %ld dates are not identical\n",
	     libnum, catnum);
      printf(" %.*s <> %.*s\n",
	     datetimearraylength, libheader, datetimearraylength, catheader);
      halt();
    }

    LINK->libcount++;
    return;
  }
  if (!BUFEOF(cat->f)) {
    printf(" delila: library %ld is empty, yet catalogue %ld is not empty\n",
	   libnum, catnum);
    halt();
  }
}


Static Void initlibrarian()
{
  struct LOC_initlibrarian V;
  long i;
  node nodefrom, nodeto;

  printf("delila %4.2f\n", version);
  printf("sequence segment size (dnamax) is %ld\n", (long)dnamax);

  getdatetime(datetime);

  for (i = 0; i < numlibfil; i++)
    libline[i] = 1;

  if (*lib1.name != '\0') {
    if (lib1.f != NULL)
      lib1.f = freopen(lib1.name, "r", lib1.f);
    else
      lib1.f = fopen(lib1.name, "r");
  } else
    rewind(lib1.f);
  if (lib1.f == NULL)
    _EscIO2(FileNotFound, lib1.name);
  RESETBUF(lib1.f, Char);
  if (*lib2.name != '\0') {
    if (lib2.f != NULL)
      lib2.f = freopen(lib2.name, "r", lib2.f);
    else
      lib2.f = fopen(lib2.name, "r");
  } else
    rewind(lib2.f);
  if (lib2.f == NULL)
    _EscIO2(FileNotFound, lib2.name);
  RESETBUF(lib2.f, Char);
  if (*lib3.name != '\0') {
    if (lib3.f != NULL)
      lib3.f = freopen(lib3.name, "r", lib3.f);
    else
      lib3.f = fopen(lib3.name, "r");
  } else
    rewind(lib3.f);
  if (lib3.f == NULL)
    _EscIO2(FileNotFound, lib3.name);
  RESETBUF(lib3.f, Char);

  for (i = 0; i < numcatfil; i++)
    catline[i] = 1;
  catnumber = 1;

  if (*cat1.name != '\0') {
    if (cat1.f != NULL)
      cat1.f = freopen(cat1.name, "rb", cat1.f);
    else
      cat1.f = fopen(cat1.name, "rb");
  } else
    rewind(cat1.f);
  if (cat1.f == NULL)
    _EscIO2(FileNotFound, cat1.name);
  RESETBUF(cat1.f, item);
  if (*cat2.name != '\0') {
    if (cat2.f != NULL)
      cat2.f = freopen(cat2.name, "rb", cat2.f);
    else
      cat2.f = fopen(cat2.name, "rb");
  } else
    rewind(cat2.f);
  if (cat2.f == NULL)
    _EscIO2(FileNotFound, cat2.name);
  RESETBUF(cat2.f, item);
  if (*cat3.name != '\0') {
    if (cat3.f != NULL)
      cat3.f = freopen(cat3.name, "rb", cat3.f);
    else
      cat3.f = fopen(cat3.name, "rb");
  } else
    rewind(cat3.f);
  if (cat3.f == NULL)
    _EscIO2(FileNotFound, cat3.name);
  RESETBUF(cat3.f, item);

  if (*inst.name != '\0') {
    if (inst.f != NULL)
      inst.f = freopen(inst.name, "r", inst.f);
    else
      inst.f = fopen(inst.name, "r");
  } else
    rewind(inst.f);
  if (inst.f == NULL)
    _EscIO2(FileNotFound, inst.name);
  RESETBUF(inst.f, Char);
  if (*listing.name != '\0') {
    if (listing.f != NULL)
      listing.f = freopen(listing.name, "w", listing.f);
    else
      listing.f = fopen(listing.name, "w");
  } else {
    if (listing.f != NULL)
      rewind(listing.f);
    else
      listing.f = tmpfile();
  }
  if (listing.f == NULL)
    _EscIO2(FileNotFound, listing.name);
  SETUPBUF(listing.f, Char);
  if (*book.name != '\0') {
    if (book.f != NULL)
      book.f = freopen(book.name, "w", book.f);
    else
      book.f = fopen(book.name, "w");
  } else {
    if (book.f != NULL)
      rewind(book.f);
    else
      book.f = tmpfile();
  }
  if (book.f == NULL)
    _EscIO2(FileNotFound, book.name);
  SETUPBUF(book.f, Char);
  if (debugging) {
    if (*debug.name != '\0') {
      if (debug.f != NULL)
	debug.f = freopen(debug.name, "w", debug.f);
      else
	debug.f = fopen(debug.name, "w");
    } else {
      if (debug.f != NULL)
	rewind(debug.f);
      else
	debug.f = tmpfile();
    }
    if (debug.f == NULL)
      _EscIO2(FileNotFound, debug.name);
    SETUPBUF(debug.f, Char);
  }

  skipping = true;

  if (BUFEOF(inst.f)) {
    printf(" delila: no instructions\n");
    halt();
  }

  V.libcount = 0;
  firstlibrary = 0;
  checklibcat(&lib1, 1L, &cat1, 1L, &V);
  checklibcat(&lib2, 2L, &cat2, 2L, &V);
  checklibcat(&lib3, 3L, &cat3, 3L, &V);
  if (V.libcount == 0) {
    printf("delila: no libraries\n");
    halt();
  }
  printf("%ld librar", V.libcount);
  if (V.libcount == 1)
    printf("y\n");
  else
    printf("ies\n");

  freeline = NULL;
  freemarker = NULL;
  freedna = NULL;
  freesite = NULL;
  libpie = (piece *)Malloc(sizeof(piece));
  libpie->key.hea.fulnam = NULL;
  libpie->key.hea.note = NULL;
  libpie->dna = NULL;
  getdna(&libpie->dna);

  memcpy(illegaltraversal, "illegal   ", sizeof(step));

  for (nodefrom = libnode;
       (long)nodefrom <= (long)sitenode;
       nodefrom = (node)((long)nodefrom + 1)) {
    for (nodeto = libnode;
	 (long)nodeto <= (long)sitenode;
	 nodeto = (node)((long)nodeto + 1))
      memcpy(traversalchart[(long)nodefrom]
	     [(long)nodeto], illegaltraversal, sizeof(step));
  }


  memcpy(traversalchart[(long)libnode]
	 [(long)orgnode], "o.        ", sizeof(step));
  memcpy(traversalchart[(long)libnode]
	 [(long)recnode], "r.        ", sizeof(step));

  memcpy(traversalchart[(long)orgnode]
	 [(long)libnode], "o.        ", sizeof(step));
  memcpy(traversalchart[(long)orgnode]
	 [(long)orgnode], "oo.       ", sizeof(step));
  memcpy(traversalchart[(long)orgnode]
	 [(long)chrnode], "c.        ", sizeof(step));
  memcpy(traversalchart[(long)orgnode]
	 [(long)recnode], "or.       ", sizeof(step));

  memcpy(traversalchart[(long)chrnode]
	 [(long)libnode], "co.       ", sizeof(step));
  memcpy(traversalchart[(long)chrnode]
	 [(long)orgnode], "coo.      ", sizeof(step));
  memcpy(traversalchart[(long)chrnode]
	 [(long)chrnode], "cc.       ", sizeof(step));
  memcpy(traversalchart[(long)chrnode]
	 [(long)marnode], "m.        ", sizeof(step));
  memcpy(traversalchart[(long)chrnode]
	 [(long)tranode], "t.        ", sizeof(step));
  memcpy(traversalchart[(long)chrnode]
	 [(long)gennode], "g.        ", sizeof(step));
  memcpy(traversalchart[(long)chrnode]
	 [(long)pienode], "p.        ", sizeof(step));
  memcpy(traversalchart[(long)chrnode]
	 [(long)recnode], "cor.      ", sizeof(step));

  memcpy(traversalchart[(long)marnode]
	 [(long)mardnanode], "d.        ", sizeof(step));

  memcpy(traversalchart[(long)mardnanode]
	 [(long)libnode], "dmco.     ", sizeof(step));
  memcpy(traversalchart[(long)mardnanode]
	 [(long)orgnode], "dmcoo.    ", sizeof(step));
  memcpy(traversalchart[(long)mardnanode]
	 [(long)chrnode], "dmcc.     ", sizeof(step));
  memcpy(traversalchart[(long)mardnanode]
	 [(long)marnode], "dmm.      ", sizeof(step));
  memcpy(traversalchart[(long)mardnanode]
	 [(long)tranode], "dmt.      ", sizeof(step));
  memcpy(traversalchart[(long)mardnanode]
	 [(long)gennode], "dmg.      ", sizeof(step));
  memcpy(traversalchart[(long)mardnanode]
	 [(long)pienode], "dmp.      ", sizeof(step));
  memcpy(traversalchart[(long)mardnanode]
	 [(long)recnode], "dmcor.    ", sizeof(step));

  memcpy(traversalchart[(long)tranode]
	 [(long)libnode], "tco.      ", sizeof(step));
  memcpy(traversalchart[(long)tranode]
	 [(long)orgnode], "tcoo.     ", sizeof(step));
  memcpy(traversalchart[(long)tranode]
	 [(long)chrnode], "tcc.      ", sizeof(step));
  memcpy(traversalchart[(long)tranode]
	 [(long)marnode], "tm.       ", sizeof(step));
  memcpy(traversalchart[(long)tranode]
	 [(long)tranode], "tt.       ", sizeof(step));
  memcpy(traversalchart[(long)tranode]
	 [(long)gennode], "tg.       ", sizeof(step));
  memcpy(traversalchart[(long)tranode]
	 [(long)pienode], "tp.       ", sizeof(step));
  memcpy(traversalchart[(long)tranode]
	 [(long)recnode], "tcor.     ", sizeof(step));

  memcpy(traversalchart[(long)gennode]
	 [(long)libnode], "gco.      ", sizeof(step));
  memcpy(traversalchart[(long)gennode]
	 [(long)orgnode], "gcoo.     ", sizeof(step));
  memcpy(traversalchart[(long)gennode]
	 [(long)chrnode], "gcc.      ", sizeof(step));
  memcpy(traversalchart[(long)gennode]
	 [(long)marnode], "gm.       ", sizeof(step));
  memcpy(traversalchart[(long)gennode]
	 [(long)tranode], "gt.       ", sizeof(step));
  memcpy(traversalchart[(long)gennode]
	 [(long)gennode], "gg.       ", sizeof(step));
  memcpy(traversalchart[(long)gennode]
	 [(long)pienode], "gp.       ", sizeof(step));
  memcpy(traversalchart[(long)gennode]
	 [(long)recnode], "gcor.     ", sizeof(step));

  memcpy(traversalchart[(long)pienode]
	 [(long)piednanode], "d.        ", sizeof(step));

  memcpy(traversalchart[(long)piednanode]
	 [(long)libnode], "dpco.     ", sizeof(step));
  memcpy(traversalchart[(long)piednanode]
	 [(long)orgnode], "dpcoo.    ", sizeof(step));
  memcpy(traversalchart[(long)piednanode]
	 [(long)chrnode], "dpcc.     ", sizeof(step));
  memcpy(traversalchart[(long)piednanode]
	 [(long)marnode], "dpm.      ", sizeof(step));
  memcpy(traversalchart[(long)piednanode]
	 [(long)tranode], "dpt.      ", sizeof(step));
  memcpy(traversalchart[(long)piednanode]
	 [(long)gennode], "dpg.      ", sizeof(step));
  memcpy(traversalchart[(long)piednanode]
	 [(long)pienode], "dpp.      ", sizeof(step));
  memcpy(traversalchart[(long)piednanode]
	 [(long)recnode], "dpcor.    ", sizeof(step));

  memcpy(traversalchart[(long)recnode]
	 [(long)orgnode], "r.        ", sizeof(step));
  memcpy(traversalchart[(long)recnode]
	 [(long)orgnode], "ro.       ", sizeof(step));
  memcpy(traversalchart[(long)recnode]
	 [(long)recnode], "rr.       ", sizeof(step));
  memcpy(traversalchart[(long)recnode]
	 [(long)enznode], "e.        ", sizeof(step));

  memcpy(traversalchart[(long)enznode]
	 [(long)sitenode], "s.        ", sizeof(step));

  memcpy(traversalchart[(long)sitenode]
	 [(long)libnode], "ser.      ", sizeof(step));
  memcpy(traversalchart[(long)sitenode]
	 [(long)orgnode], "sero.     ", sizeof(step));
  memcpy(traversalchart[(long)sitenode]
	 [(long)recnode], "serr.     ", sizeof(step));
  memcpy(traversalchart[(long)sitenode]
	 [(long)enznode], "see.      ", sizeof(step));
  memcpy(traversalchart[(long)sitenode]
	 [(long)sitenode], "ss.       ", sizeof(step));

  pastlibrary = libnode;
  pastbook = libnode;
  pastcheck = libnode;
  nodechar[(long)libnode] = 'l';
  nodechar[(long)orgnode] = 'o';
  nodechar[(long)chrnode] = 'c';
  nodechar[(long)marnode] = 'm';
  nodechar[(long)mardnanode] = 'd';
  nodechar[(long)tranode] = 't';
  nodechar[(long)gennode] = 'g';
  nodechar[(long)pienode] = 'p';
  nodechar[(long)piednanode] = 'd';
  nodechar[(long)recnode] = 'r';
  nodechar[(long)enznode] = 'e';
  nodechar[(long)sitenode] = 's';
  nodeletter = nodechar[(long)libnode];
  zerobase = 0;
  zeroshift = 0;
  clearname(lastpiecename);
  withused = false;
  booksize = 0;
}

/* Local variables for lrorgkey: */
struct LOC_lrorgkey {
  orgkey *org;
  item it;
  long fi;
} ;

Local Void rorgkey(thefile, LINK)
_TEXT *thefile;
struct LOC_lrorgkey *LINK;
{
  orgkey *WITH;

  WITH = LINK->org;
  libgetto(thefile, LINK->fi, LINK->it.line_);
  checklib(thefile, nodeletter, LINK->fi, LINK->it.line_);
  brorgkey(thefile, &libline[LINK->fi-1], LINK->org);
}

Static Void lrorgkey(nam, org_)
name nam;
orgkey *org_;
{
  struct LOC_lrorgkey V;

  V.org = org_;
  clearname(lastpiecename);
  finditem(orgnode, nam, &V.it, &V.fi);
  if (!itemfound)
    return;
  switch (V.fi) {
    case 1:
      rorgkey(&lib1, &V);
      break;
    case 2:
      rorgkey(&lib2, &V);
      break;
    case 3:
      rorgkey(&lib3, &V);
      break;
  }
}

/* Local variables for lrchrkey: */
struct LOC_lrchrkey {
  chrkey *chr;
  item it;
  long fi;
} ;

Local Void rchrkey(thefile, LINK)
_TEXT *thefile;
struct LOC_lrchrkey *LINK;
{
  chrkey *WITH;
  WITH = LINK->chr;
  libgetto(thefile, LINK->fi, LINK->it.line_);
  checklib(thefile, nodeletter, LINK->fi, LINK->it.line_);
  brchrkey(thefile, &libline[LINK->fi-1], LINK->chr);
}

Static Void lrchrkey(nam, chr_)
name nam;
chrkey *chr_;
{
  struct LOC_lrchrkey V;
  V.chr = chr_;
  clearname(lastpiecename);
  finditem(chrnode, nam, &V.it, &V.fi);
  if (!itemfound)
    return;
  switch (V.fi) {
    case 1:
      rchrkey(&lib1, &V);
      break;
    case 2:
      rchrkey(&lib2, &V);
      break;
    case 3:
      rchrkey(&lib3, &V);
      break;
  }
}

/* Local variables for lrtrakey: */
struct LOC_lrtrakey {
  trakey *tra;
  item it;
  long fi;
} ;

Local Void rtrakey(thefile, LINK)
_TEXT *thefile;
struct LOC_lrtrakey *LINK;
{
  trakey *WITH;
  WITH = LINK->tra;
  libgetto(thefile, LINK->fi, LINK->it.line_);
  checklib(thefile, nodeletter, LINK->fi, LINK->it.line_);
  lrheader(thefile, LINK->fi, &WITH->hea);
  lrreference(thefile, LINK->fi, &WITH->ref);
}

Static Void lrtrakey(nam, tra_)
name nam;
trakey *tra_;
{
  struct LOC_lrtrakey V;
  V.tra = tra_;
  finditem(tranode, nam, &V.it, &V.fi);

  if (!itemfound)
    return;
  switch (V.fi) {
    case 1:
      rtrakey(&lib1, &V);
      break;
    case 2:
      rtrakey(&lib2, &V);
      break;
    case 3:
      rtrakey(&lib3, &V);
      break;
  }
}

/* Local variables for lrgenkey: */
struct LOC_lrgenkey {
  genkey *gen;
  item it;
  long filenumber;
} ;

Local Void rgenkey(thefile, LINK)
_TEXT *thefile;
struct LOC_lrgenkey *LINK;
{
  genkey *WITH;
  WITH = LINK->gen;
  libgetto(thefile, LINK->filenumber, LINK->it.line_);
  checklib(thefile, nodeletter, LINK->filenumber, LINK->it.line_);
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  libline[LINK->filenumber-1]++;
  lrheader(thefile, LINK->filenumber, &WITH->hea);
  lrreference(thefile, LINK->filenumber, &WITH->ref);
}


Static Void lrgenkey(nam, gen_)
name nam;
genkey *gen_;
{
  struct LOC_lrgenkey V;
  V.gen = gen_;
  finditem(gennode, nam, &V.it, &V.filenumber);

  if (!itemfound)
    return;
  switch (V.filenumber) {
    case 1:
      rgenkey(&lib1, &V);
      break;
    case 2:
      rgenkey(&lib2, &V);
      break;
    case 3:
      rgenkey(&lib3, &V);
      break;
  }
}

/* Local variables for lrpiece: */
struct LOC_lrpiece {
  name nam;
  piece **libpie;
  item it;
  long filenumber;
} ;

Local Void rpiece(thefile, LINK)
_TEXT *thefile;
struct LOC_lrpiece *LINK;
{
  _TEXT TEMP;
  libgetto(thefile, LINK->filenumber, LINK->it.line_);
  tvrslibrary(piednanode);
  printf("Request for: ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writename(&TEMP, LINK->nam);
  if (equalname(LINK->nam, lastpiecename) && lastpiecename.length > 0) {
    printf(" - same name as last time, keeping the same piece\n");
    return;
  }
  printf(" - reading in the piece\n");
  brpiece(thefile, &libline[LINK->filenumber-1], LINK->libpie);
  copyname((*LINK->libpie)->key.hea.keynam, &lastpiecename);
}


Static Void lrpiece(nam_, libpie_)
name nam_;
piece **libpie_;
{
  struct LOC_lrpiece V;
  V.nam = nam_;
  V.libpie = libpie_;
  finditem(pienode, V.nam, &V.it, &V.filenumber);

  if (!itemfound)
    return;
  switch (V.filenumber) {
    case 1:
      rpiece(&lib1, &V);
      break;
    case 2:
      rpiece(&lib2, &V);
      break;
    case 3:
      rpiece(&lib3, &V);
      break;
  }
}

/* procedure to print in the book the appropriate tree
   traversal characters: traverse to the new (future) node */
Static Void tvrsbook(future)
node future;
{
  step thisstep;
  char steps = 1;

  memcpy(thisstep, traversalchart[(long)pastbook][(long)future], sizeof(step));
  if (!strncmp(thisstep, illegaltraversal, sizeof(step))) {
    printf(" delila: program error:  illegal book traversal %12d to %12d\n",
	   (int)pastbook, (int)future);
    halt();
  }
  while (thisstep[steps-1] != '.') {
    switch (thisstep[steps-1]) {
      case 'o':
        fprintf(book.f, "organism");
        if (!versioninbook) {
    fprintf(book.f, " - book produced by delila %4.2f", version);
    versioninbook = true;
        }
        putc('\n', book.f);
        break;
      case 'c':
        fprintf(book.f, "chromosome\n");
        break;
      case 'm':
        fprintf(book.f, "marker\n");
        break;
      case 'd':
        fprintf(book.f, "dna\n");
        break;
      case 't':
        fprintf(book.f, "transcript\n");
        break;
      case 'g':
        fprintf(book.f, "gene\n");
        break;
      case 'p':
        fprintf(book.f, "piece\n");
        break;
      case 'r':
        fprintf(book.f, "recognition-class\n");
        break;
      case 'e':
        fprintf(book.f, "enzyme\n");
        break;
      case 's':
        fprintf(book.f, "site\n");
        break;
    }
    steps++;
  }
  pastbook = future;
}


Static Void bwbookheader(title)
line **title;
{
  datetimearray libheader;
  long i, FORLIM;
  bwstartline(&book);
  writedatetime(&book, datetime);
  fprintf(book.f, ", ");
  switch (firstlibrary) {
    case 1:
      lrlibheader(&lib1, 1L, libheader);
      break;
    case 2:
      lrlibheader(&lib2, 2L, libheader);
      break;
    case 3:
      lrlibheader(&lib3, 3L, libheader);
      break;
  }
  writedatetime(&book, libheader);

  if (*title != NULL) {
    if ((*title)->length != 0) {
      fprintf(book.f, ", ");
      FORLIM = (*title)->length;
      for (i = 0; i < FORLIM; i++)
	putc((*title)->letters[i], book.f);
    }
    clearline(title);
  }

  putc('\n', book.f);
}

Static Void wchangedata(f, c_)
_TEXT *f;
changedata c_;
{
  long i;

  switch (c_.changetype) {
  case 'c':
    fprintf(f->f, "%c%ld%c", c_.baseold, c_.basecoo1, c_.basenew);
    break;
  case 'i':
    fprintf(f->f, "i%ld,%ld", c_.basecoo1, c_.basecoo2);
    if (c_.inserts > 0) {
      for (i = 0; i < c_.inserts; i++) {
	putc(c_.insert[i], f->f);
	    }
    }
    break;
  case 'd':
    fprintf(f->f, "d%ld,%ld", c_.basecoo1, c_.basecoo2);
    break;
  }
}

Static Void writechangeset(f, changes)
_TEXT *f;
changeset changes;
{
  long n;

  for (n = 1; n <= changes.number; n++) {
    if (n > 1)
      putc('.', f->f);
    wchangedata(f, changes.data[n-1]);
  }
}

Static Void checkchangeset(f, changes)
_TEXT *f;
changeset changes;
{
  long i, n, FORLIM;
  changedata *WITH;
  long FORLIM1;

  FORLIM = changes.number;
  for (n = 1; n <= FORLIM; n++) {
    if (n > 1)
      putc('.', f->f);
    WITH = &changes.data[n-1];
    switch (WITH->changetype) {
      case 'c':
        fprintf(f->f, "%c%ld%c", WITH->baseold, WITH->internal1, WITH->basenew);
        break;
      case 'i':
        fprintf(f->f, "i%ld,%ld", WITH->internal1, WITH->internal2);
        if (WITH->inserts > 0) {
	FORLIM1 = WITH->inserts;
	for (i = 0; i < FORLIM1; i++) {
	  putc(WITH->insert[i], f->f);
	}
      }
      break;
    case 'd':
      fprintf(f->f, "d%ld,%ld", WITH->internal1, WITH->internal2);
      break;
    }
  }
}

Static Void describechangeset(f, changes)
_TEXT *f;
changeset changes;
{
  long i, n, FORLIM;
  changedata *WITH;
  long FORLIM1;

  if (changes.number == 0) {
    fprintf(f->f, "no changes");
    return;
  }
  FORLIM = changes.number;
  for (n = 1; n <= FORLIM; n++) {
    if (n > 1)
      fprintf(f->f, ", ");
    WITH = &changes.data[n-1];
    switch (WITH->changetype) {
    case 'c':
      fprintf(f->f, "at %ld %c->%c",
	      WITH->basecoo1, WITH->baseold, WITH->basenew);
      break;
    case 'i':
      fprintf(f->f, "insert ");
      if (WITH->inserts > 0) {
	FORLIM1 = WITH->inserts;
	for (i = 0; i < FORLIM1; i++)
	  putc(WITH->insert[i], f->f);
      } else
	fprintf(f->f, "NOTHING");
      fprintf(f->f, " between %ld and %ld", WITH->basecoo1, WITH->basecoo2);
      break;
    case 'd':
      fprintf(f->f, "delete %ld to %ld", WITH->basecoo1, WITH->basecoo2);
      break;
    }
  }
}

/*this routine is to be used to check that the instructions are
   in proper order:
   'if tvrschecks(this-try) then continue-analysis-of-instructions;'

   in three cases we must advance (force) the node since no reads are
   done at this time. (library reads do a force for the other traversal
   routines) */
Static boolean tvrschecks(futurecheck)
node futurecheck;
{
  boolean Result;
  Result = (strncmp(traversalchart[(long)pastcheck][(long)futurecheck],
		    illegaltraversal, sizeof(step)) != 0);
  if (futurecheck == marnode) {
    pastcheck = mardnanode;
    return Result;
  }
  if (futurecheck == pienode) {
    pastcheck = piednanode;
    return Result;
  }
  if (futurecheck == enznode)
    pastcheck = sitenode;
  else
    pastcheck = futurecheck;
  return Result;
}


#define pagelength      57
#define widinst         namelength
#define minword         2
#define max1errors      33
#define max2errors      223
#define maxerrors       10
#define maxpositions    15
#define maxnumbers      15
#define numberlength    2

typedef enum {
  alldelila, arrdelila, begdelila, chrdelila, cledelila, comdelila, condelila,
  coodelila, cutdelila, defdelila, dirdelila, doudelila, elsdelila, enddelila,
  enzdelila, evedelila, expdelila, frodelila, gendelila, getdelila, haldelila,
  homdelila, ifdelila, keydelila, mardelila, moddelila, namdelila, notdelila,
  numdelila, offdelila, ondelila, orgdelila, outdelila, piedelila, recdelila,
  reddelila, revdelila, samdelila, setdelila, sitdelila, sizdelila, thedelila,
  titdelila, todelila, tradelila, witdelila, nordelila, zerdelila, eodelila
} delilaword;
/* p2c: delila.p, line 3957:
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 4918 [251] */

typedef Char instword[widinst];

typedef struct elnrecord {
  long pos[maxerrors];
  long err[maxerrors];
} elnrecord;

typedef struct plnrecord {
  long pos[maxpositions];
  long psn[maxpositions];
  double rea[maxpositions];
} plnrecord;

typedef struct nlnrecord {
  long pos[maxnumbers];
  long num[maxnumbers];
} nlnrecord;


typedef long position;
#define shiftdown       1
#define showcase        false
#define ln10            2.30259
#define epsilon         0.00001
#define debug_          false
#define precision       1e-7

/* Local variables for librarian: 
* by paul morrissett
   readinstruction modified, and supporting routines written
   by tom schnieder *)
(* there are two passes made through the delila instructions.

      pass 1 reads the code using ir routines (which all rely on
procedure ichread to read the instructions).
nothing happens when delila gets to a 'peak' in the code (after
passing all the if statements), since this represents a syntactically
correct instruction.  (in other words, the fact that one got to a
certain spot in the code means that so far the instruction is correct.)
ichread is responsible for writing the lines into the listing during this
pass.  instructions are written as they are scanned.  all writes are
done inside the read procedures.  syntax errors are pointed to.
the title is read, if it is in the instructions.

      pass 2 runs only if pass 1 had no errors.
the instructions are reread, and variables are collected.
calls to the library read, and the book write routines are then made.
numerical values and numbers of items are pointed to.

      note on irs and the parser:
these routines assume that the previous actions left the inst
file in good order.  this means that they have read past all they
needed, and they have read their parse stop symbol.  they need not
have done a findword however, so this is often required as the
first action of an ir.  (extra findword calls will not hurt.)
*/
struct LOC_librarian {
  char pass;
  long pageline, pagenumber;
  boolean pass1errors, pass2errors, warnings;
  boolean error1list[max1errors + 1];
  boolean error2list[max2errors - 200];
  long numoferrors;
  elnrecord errorline;
  boolean showallerrors;
  instword wordlist[49];
  long numbers[3];
  Char chr;
  instword parsedword;
  long parsedlength, parsedlocation;
  delilaword word;
  long inumber;
  double rnumber;
  name keyname;
  delilaword save;
  line *usernotes;
  line *title;
  boolean titleexists;
  long instructioncount, linecount, lineposition;
  boolean correct;
  boolean eoinst;
  long parentheses;
  boolean comment;
  long commentline;
  boolean quote, linebreak;
  boolean significant;
  Char numberword[numberlength];
  double okspec, ckspec, mkspec, tkspec, gkspec, pkspec, rkspec, ekspec;
  orgkey ok;
  chrkey ck;
  marker *mkoff, *mkon;
  trakey tk;
  genkey gk;
  piece *pk;
  reckey rk;
  enzyme *ek;

  long fromposition, toposition;
  long cutposition;
  direction dirwanted;
  long numofpositions;
  plnrecord positionline;
  long numofnumbers, numofchanges;
  nlnrecord numberline;
  long structure[(long)eodelila / 32 + 2];
  long previousfromposition;
  boolean sameusageisvalid;
  boolean reduced;
  long indentlisting;
  direction coordinateside;
  long getcount;
} ;

Local Void startheader(hea, LINK)
header *hea;
struct LOC_librarian *LINK;
{
  hea->fulnam = NULL;
  hea->note = NULL;
}

Local Void initializedelila(LINK)
struct LOC_librarian *LINK;
{
  long i;

  versioninbook = false;
  LINK->pagenumber = 0;

  memcpy(LINK->wordlist[(long)alldelila],
    "all                                                                                                                                                                                                     ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)arrdelila],
    "arrowlength                                                                                                                                                                                             ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)begdelila],
    "beginning                                                                                                                                                                                               ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)chrdelila],
    "chromosome                                                                                                                                                                                              ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)cledelila],
    "cleave                                                                                                                                                                                                  ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)comdelila],
    "complement                                                                                                                                                                                              ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)condelila],
    "continue                                                                                                                                                                                                ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)coodelila],
    "coordinate                                                                                                                                                                                              ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)cutdelila],
    "cut                                                                                                                                                                                                     ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)defdelila],
    "default                                                                                                                                                                                                 ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)dirdelila],
    "direction                                                                                                                                                                                               ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)doudelila],
    "doubling                                                                                                                                                                                                ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)elsdelila],
    "else                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)enddelila],
    "ending                                                                                                                                                                                                  ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)enzdelila],
    "enzyme                                                                                                                                                                                                  ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)evedelila],
    "every                                                                                                                                                                                                   ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)expdelila],
    "expand                                                                                                                                                                                                  ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)frodelila],
    "from                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)gendelila],
    "gene                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)getdelila],
    "get                                                                                                                                                                                                     ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)haldelila],
    "halt                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)homdelila],
    "homologous                                                                                                                                                                                              ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)ifdelila],
    "if                                                                                                                                                                                                      ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)keydelila],
    "key                                                                                                                                                                                                     ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)mardelila],
    "marker                                                                                                                                                                                                  ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)moddelila],
    "modify                                                                                                                                                                                                  ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)namdelila],
    "name                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)notdelila],
    "note                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)numdelila],
    "numbering                                                                                                                                                                                               ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)offdelila],
    "off                                                                                                                                                                                                     ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)ondelila],
    "on                                                                                                                                                                                                      ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)orgdelila],
    "organism                                                                                                                                                                                                ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)outdelila],
    "out-of-range                                                                                                                                                                                            ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)piedelila],
    "piece                                                                                                                                                                                                   ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)reddelila],
    "reduce-range                                                                                                                                                                                            ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)revdelila],
    "reversenames                                                                                                                                                                                            ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)samdelila],
    "same                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)setdelila],
    "set                                                                                                                                                                                                     ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)sitdelila],
    "site                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)sizdelila],
    "size                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)thedelila],
    "then                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)titdelila],
    "title                                                                                                                                                                                                   ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)todelila],
    "to                                                                                                                                                                                                      ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)tradelila],
    "transcript                                                                                                                                                                                              ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)witdelila],
    "with                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)zerdelila],
    "zero                                                                                                                                                                                                    ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)nordelila],
    "normal                                                                                                                                                                                                  ",
    sizeof(instword));
  memcpy(LINK->wordlist[(long)eodelila],
    "end-of-delila-words                                                                                                                                                                                     ",
    sizeof(instword));

  LINK->pass1errors = false;
  LINK->pass2errors = false;
  LINK->showallerrors = false;
  for (i = 0; i <= max1errors; i++)
    LINK->error1list[i] = false;
  for (i = 0; i <= max2errors - 201; i++)
    LINK->error2list[i] = false;

  LINK->warnings = false;
  LINK->title = NULL;
  LINK->titleexists = false;
  longname = NULL;
  LINK->usernotes = NULL;
  memcpy(LINK->numberword, "# ", (long)numberlength);
  P_addset(P_expset(LINK->numbers, 0L), '0');
  P_addset(LINK->numbers, '1');
  P_addset(LINK->numbers, '2');
  P_addset(LINK->numbers, '3');
  P_addset(LINK->numbers, '4');
  P_addset(LINK->numbers, '5');
  P_addset(LINK->numbers, '6');
  P_addset(LINK->numbers, '7');
  P_addset(LINK->numbers, '8');
  P_addset(LINK->numbers, '9');
  P_addset(P_expset(LINK->structure, 0L), (int)orgdelila);
  P_addset(LINK->structure, (int)chrdelila);
  P_addset(LINK->structure, (int)mardelila);
  P_addset(LINK->structure, (int)tradelila);
  P_addset(LINK->structure, (int)gendelila);
  P_addset(LINK->structure, (int)piedelila);
  getmarker(&LINK->mkoff);
  LINK->mkon = NULL;
  LINK->pk = (piece *)Malloc(sizeof(piece));
  LINK->ek = (enzyme *)Malloc(sizeof(enzyme));
  startheader(&LINK->ok.hea, LINK);
  LINK->ok.mapunit = NULL;
  startheader(&LINK->ck.hea, LINK);
  startheader(&LINK->mkoff->key.hea, LINK);
  LINK->mkoff->key.phenotype = NULL;
  LINK->mkoff->key.next = NULL;
  startheader(&LINK->tk.hea, LINK);
  startheader(&LINK->gk.hea, LINK);
  startheader(&LINK->pk->key.hea, LINK);
  startheader(&LINK->rk.hea, LINK);
  startheader(&LINK->ek->key.hea, LINK);
  LINK->okspec = 0.5;
  LINK->ckspec = 0.5;
  LINK->mkspec = 0.5;
  LINK->tkspec = 0.5;
  LINK->gkspec = 0.5;
  LINK->pkspec = 0.5;
  LINK->rkspec = 0.5;
  LINK->ekspec = 0.5;
  LINK->indentlisting = instwidth * 2 + 4;
  LINK->coordinateside = plus;
  LINK->getcount = 0;
}

Local Void pageheader(LINK)
struct LOC_librarian *LINK;
{
  LINK->pageline = 1;
  LINK->pagenumber++;
  fprintf(listing.f, "   ");
  writedatetime(&listing, datetime);
  fprintf(listing.f, "     delila %4.2f     pass %d           page %ld\n\n",
	  version, LINK->pass, LINK->pagenumber);
  LINK->pageline += 2;
}

Local Void startlistingline(LINK)
struct LOC_librarian *LINK;
{
  if (!BUFEOF(inst.f)) {
    LINK->linecount++;
    fprintf(listing.f, " %*ld %*ld  ",
	    instwidth, LINK->linecount, instwidth, LINK->instructioncount);
  }
}

Local Void initreadinst(LINK)
struct LOC_librarian *LINK;
{
  LINK->eoinst = false;
  LINK->parentheses = 0;
  LINK->comment = false;
  LINK->quote = false;
  LINK->chr = ' ';
  LINK->instructioncount = 1;
  LINK->linecount = 0;
  LINK->lineposition = 0;
  LINK->numoferrors = 0;
  LINK->numofpositions = 0;
  LINK->numofnumbers = 0;
  LINK->numofchanges = 0;
  def.coo = coornormal;
  pageheader(LINK);
  fprintf(listing.f,
	  "    Parent               Grand Parent         Library\n");
  fprintf(listing.f, "#   Library Date/Time:  ");
  fprintf(listing.f, " Library Date/Time:");
  fprintf(listing.f, "   Title:\n");
  LINK->pageline += 2;

  if (copylibname(&listing, &lib1, 1L))
    LINK->pageline++;
  if (copylibname(&listing, &lib2, 2L))
    LINK->pageline++;
  if (copylibname(&listing, &lib3, 3L))
    LINK->pageline++;
  fprintf(listing.f, "\n   inst statement\n");
  fprintf(listing.f, "   line number   \n");
  LINK->pageline += 3;
  startlistingline(LINK);
  longnameexists = false;
  clearname(lastpiecename);
  mutations.number = 0;
  LINK->pk->dna = NULL;
  libpie->dna = NULL;
  clearpiece(&LINK->pk);
  clearpiece(&libpie);

  def.key.note = on;
  def.key.mar = on;
  def.key.gen = on;
  def.key.tra = on;
  def.sit.expand = on;
  def.sit.modify = off;
  def.sit.cleave = off;
  def.defout = rhalt;
  def.num.sta = on;

  for (indnum = orgnum;
       (long)indnum <= (long)enznum;
       indnum = (numberedstructure)((long)indnum + 1))
    def.num.str[(long)indnum] = off;

  def.num.str[(long)pienum] = on;
  def.num.item = 0;
  def.doubling = off;
  def.arrowlength = 1.5;
  def.reversenames = off;
  insertlowerbits = -1.3;
  insertupperbits = -0.1;
  deletelowerbits = -1.3;
  deleteupperbits = -0.1;
  changelowerbits = 0.3;
  changeupperbits = def.arrowlength + changelowerbits;
}

Local boolean awarning(e, LINK)
long e;
struct LOC_librarian *LINK;
{
  return (e == 206 || e == 208 || e == 209 || e == 210 || e == 212 ||
	  e == 213 || e == 215 || e == 216 || e == 219 || e == 220 ||
	  e == 221 || e == 222 || e == 3);
}

Local Void writepasserrors(listing, pass, LINK)
_TEXT *listing;
long pass;
struct LOC_librarian *LINK;
{
  long e, i;
  boolean warn1;
  name *WITH;
  long FORLIM1;

  if (pass == 1 && !LINK->pass1errors)
    fprintf(listing->f, "No syntax errors found.\n");
  if (pass == 2 && !LINK->pass2errors)
    fprintf(listing->f, "No extraction errors found.\n");

  warn1 = (pass == 2 && LINK->error1list[3]);
  if (pass == 1 || warn1) {
    if (LINK->pass1errors || warn1) {
      fprintf(listing->f, "Key to error and warning numbers:\n");
      for (e = 0; e <= max1errors; e++) {
	if (LINK->error1list[e]) {
	  if (awarning(e, LINK))
	    fprintf(listing->f, "WARNING!  ");
	  fprintf(listing->f, "%4ld: ", e);
	  switch (e) {
	  case 0:
	    fprintf(listing->f,
		    "I do not know how to do this instruction yet, sorry");
	    break;
	  case 1:
	    fprintf(listing->f,
		    "Instruction longer than expected (missing semicolon)");
	    break;
	  case 2:
	    fprintf(listing->f, "I can not recognize this word");
	    break;
	  case 3:
	    fprintf(listing->f, "Warning: this title was ignored");
	    break;
	  case 4:
	    fprintf(listing->f, "I can not recognize this integer");
	    break;
	  case 6:
	    fprintf(listing->f,
	      "You are missing a specification (illegal tree traversal)");
	    break;
	  case 7:
	    fprintf(listing->f, "This word is not allowed here");
	    break;
	  case 8:
	    fprintf(listing->f, "A cut is only allowed with a piece");
	    break;
	  case 9:
	    fprintf(listing->f, "This word is too long for me (>%3ld chars)",
		    (long)widinst);
	    break;
	  case 10:
	    fprintf(listing->f,
		    "Unclosed comment found at end of instructions.\n");
	    fprintf(listing->f, "%6cThe comment started on line %ld.",
		    ' ', LINK->commentline);
	    break;
	  case 11:
	    fprintf(listing->f, "Unclosed ( or ) in this instruction");
	    break;
	  case 12:
	    fprintf(listing->f, "Unclosed quote");
	    break;
	  case 13:
	    fprintf(listing->f, "No closing ;");
	    break;
	  case 14:
	    fprintf(listing->f, "This key name is too long (>%3ld chars)",
		    (long)namelength);
	    break;
	  case 15:
	    fprintf(listing->f, "The statement ended before I expected it to");
	    break;
	  case 16:
	    fprintf(listing->f, "Quote expected");
	    break;
	  case 17:
	    fprintf(listing->f, "A title is required");
	    break;
	  case 18:
	    fprintf(listing->f,
	      "\"from same\" is not allowed; use it only as \"to same\"");
	    break;
	  case 19:
	    fprintf(listing->f,
		    "mutation: Change must be identified by one of: acgtdi\n");
	    fprintf(listing->f,
	      "%6c          Instead, an illegal change character was found",
	      ' ');
	    break;
	  case 20:
	    fprintf(listing->f, "Old base must be: a, c, g, t");
	    break;
	  case 21:
	    fprintf(listing->f, "New base must be: a, c, g, t");
	    break;
	  case 22:
	    fprintf(listing->f,
		    "Insertion bases must be one of a, c, g, or t.");
	    break;
	  case 23:
	    fprintf(listing->f,
	      "A comma (,) is expected between coordinates for deletion.");
	    break;
	  case 24:
	    fprintf(listing->f,
	      "No more than %ld insertion bases allowed, increase constant insertmax",
	      (long)insertmax);
	    break;
	  case 25:
	    fprintf(listing->f,
	      "A comma (,) is expected between coordinates for insertion.");
	    break;
	  case 26:
	    fprintf(listing->f,
		    "A number was expected but no digits were found.");
	    break;
	  case 27:
	    fprintf(listing->f,
	      "First coordinate number must not be larger than second:\n");
	    fprintf(listing->f,
		    "      reverse their order.  See libdef for the reason.");
	    break;
	  case 28:
	    fprintf(listing->f,
	      "Too many changes requested, increase constant changesetmax.");
	    break;
	  case 29:
	    fprintf(listing->f, "Extra dots not allowed.");
	    break;
	  case 30:
	    fprintf(listing->f, "Unidentified change command.");
	    break;
	  case 31:
	    fprintf(listing->f,
		    "Numbers cannot have more than 1 sign (+ or -).");
	    break;
	  case 32:
	    fprintf(listing->f,
	      "Insertion complement symbol must be before insertion sequence.");
	    break;
	  case 33:
	    fprintf(listing->f,
	      "Non-printable ASCII character found.  Eg: tabs not allowed.");
	    break;
	  }
	  putc('\n', listing->f);
	}
      }
    }
  }

  if (pass == 2) {
    if (LINK->pass2errors || LINK->warnings) {
      fprintf(listing->f, "Key to error or warning numbers:\n");
      for (e = 201; e <= max2errors; e++) {
	if (LINK->error2list[e-201]) {
	  fprintf(listing->f, "%4ld: ", e);
	  if (awarning(e, LINK))
	    fprintf(listing->f, "WARNING!  ");
	  switch (e) {
	  case 201:
	    fprintf(listing->f, "I can not find this item in the library");
	    break;
	  case 202:
	    fprintf(listing->f, "This item was not previously specified");
	    break;
	  case 203:
	    fprintf(listing->f, "Out of range and default range = halt");
	    break;
	  case 204:
	    fprintf(listing->f,
		    "Positions not consistent with requested direction");
	    break;
	  case 205:
	    fprintf(listing->f,
		    "This thing was not on the previously specified piece");
	    break;
	  case 206:
	    fprintf(listing->f, "We do not know this limit");
	    break;
	  case 207:
	    fprintf(listing->f,
		    "For cutting, the piece must be circular. it is linear");
	    break;
	  case 208:
	    fprintf(listing->f, "Out of range and default range = reduce");
	    break;
	  case 209:
	    fprintf(listing->f, "Out of range and default range = continue");
	    break;
	  case 210:
	    fprintf(listing->f,
		    "Piece had two end reductions: length will be 1 base!");
	    printf("Piece ");
	    WITH = &LINK->pk->key.hea.keynam;
	    FORLIM1 = WITH->length;
	    for (i = 0; i < FORLIM1; i++)
	      putchar(WITH->letters[i]);
	    printf(" had two end reductions: length will be 1 base!\n");
	    break;
	  case 211:
	    fprintf(listing->f,
		    "The base you want to mutate is not what you said it is");
	    break;
	  case 212:
	    fprintf(listing->f,
		    "mutation: inserted sequence alters coordinate system");
	    break;
	  case 213:
	    fprintf(listing->f,
		    "mutation: deleted sequence alters coordinate system");
	    break;
	  case 214:
	    fprintf(listing->f,
		    "mutation: the initial and final bases are the same,\n");
	    fprintf(listing->f, "      so you did not request any change!");
	    break;
	  case 215:
	    fprintf(listing->f,
	      "mutation: Requested coordinate off piece in 5' direction");
	    break;
	  case 216:
	    fprintf(listing->f,
	      "mutation: requested coordinate off piece in 3' direction");
	    break;
	  case 218:
	    fprintf(listing->f,
		    "mutation: The requested coordinates cannot be equal.");
	    break;
	  case 219:
	    fprintf(listing->f, "There will be no change to the sequence!");
	    break;
	  case 220:
	    fprintf(listing->f,
		    "The entire sequence was deleted!  NO PIECE WAS OUTPUT!");
	    break;
	  case 221:
	    fprintf(listing->f,
		    "Deletion outside of sequence will have no effect!");
	    break;
	  case 222:
	    fprintf(listing->f, "WARNING!  Overlapping changes!");
	    break;
	  case 223:
	    fprintf(listing->f, "Booksize would exceed %ld bases.", maxbook);
	    break;
	  }
	  putc('\n', listing->f);
	}
      }
    }
  }
  putc('\n', listing->f);
}

Local Void copyfile(fromfile, tofile)
_TEXT *fromfile, *tofile;
{
  Char ch;

  while (!BUFEOF(fromfile->f)) {
    while (!P_eoln(fromfile->f)) {
      ch = getc(fromfile->f);
      if (ch == '\n')
	ch = ' ';
      putc(ch, tofile->f);
    }
    fscanf(fromfile->f, "%*[^\n]");
    getc(fromfile->f);
    putc('\n', tofile->f);
  }
}

Local Void bookhalt(LINK)
struct LOC_librarian *LINK;
{
  _TEXT bookcopy;
  bookcopy.f = NULL;
  *bookcopy.name = '\0';
  if (LINK->pass == 2) {
    if (*book.name != '\0') {
      if (book.f != NULL)
	book.f = freopen(book.name, "r", book.f);
      else
	book.f = fopen(book.name, "r");
    } else
      rewind(book.f);
    if (book.f == NULL)
      _EscIO2(FileNotFound, book.name);
    RESETBUF(book.f, Char);
    if (*bookcopy.name != '\0') {
      if (bookcopy.f != NULL)
	bookcopy.f = freopen(bookcopy.name, "w", bookcopy.f);
      else
	bookcopy.f = fopen(bookcopy.name, "w");
    } else {
      if (bookcopy.f != NULL)
	rewind(bookcopy.f);
      else
	bookcopy.f = tmpfile();
    }
    if (bookcopy.f == NULL)
      _EscIO2(FileNotFound, bookcopy.name);
    SETUPBUF(bookcopy.f, Char);
    copyfile(&book, &bookcopy);
  }
  if (*book.name != '\0') {
    if (book.f != NULL)
      book.f = freopen(book.name, "w", book.f);
    else
      book.f = fopen(book.name, "w");
  } else {
    if (book.f != NULL)
      rewind(book.f);
    else
      book.f = tmpfile();
  }
  if (book.f == NULL)
    _EscIO2(FileNotFound, book.name);
  SETUPBUF(book.f, Char);
  fprintf(book.f, "halt: error in Delila pass %d\n", LINK->pass);
  if (LINK->pass == 2) {
    if (*bookcopy.name != '\0') {
      if (bookcopy.f != NULL)
	bookcopy.f = freopen(bookcopy.name, "r", bookcopy.f);
      else
	bookcopy.f = fopen(bookcopy.name, "r");
    } else
      rewind(bookcopy.f);
    if (bookcopy.f == NULL)
      _EscIO2(FileNotFound, bookcopy.name);
    RESETBUF(bookcopy.f, Char);
    copyfile(&bookcopy, &book);
  }
  if (bookcopy.f != NULL)
    fclose(bookcopy.f);
}

Local Void iwritenumber(c_, n, LINK)
Char c_;
long n;
struct LOC_librarian *LINK;
{
  long spots, absn;
  long power = 10;

  if (n >= 0) {
    spots = 2;
    absn = n;
  } else {
    spots = 3;
    absn = -n;
  }

  while (absn >= power) {
    power *= 10;
    spots++;
  }
  fprintf(listing.f, "%c%*ld", c_, (int)(spots - 1), n);
  LINK->lineposition += spots;
}

Local Void rwritenumber(c_, n, LINK)
Char c_;
double n;
struct LOC_librarian *LINK;
{
  long spots;
  long power = 10;
  double absn;

  if (n >= 0) {
    spots = 2;
    absn = n;
  } else {
    spots = 3;
    absn = -n;
  }

  while (absn >= power) {
    power *= 10;
    spots++;
  }
  spots += 2;
  fprintf(listing.f, "%c%*.2f", c_, (int)(spots - 1), n);
  LINK->lineposition += spots;
}

Local Void error(err, LINK)
long err;
struct LOC_librarian *LINK;
{
  if (awarning(err, LINK))
    printf("WARNING %ld\n", err);
  else
    printf("ERROR %ld\n", err);

  LINK->correct = false;
  if (awarning(err, LINK)) {
    LINK->warnings = true;
    LINK->correct = true;
  }
  LINK->numoferrors++;
  if (LINK->numoferrors <= maxerrors) {
    LINK->errorline.pos[LINK->numoferrors-1] = LINK->lineposition;
    LINK->errorline.err[LINK->numoferrors-1] = err;
    if (LINK->pass == 1) {
      LINK->pass1errors = (LINK->pass1errors || !LINK->correct);
      LINK->error1list[err] = true;
    } else {
      LINK->pass2errors = (LINK->pass2errors || !LINK->correct);
      LINK->error2list[err-201] = true;
    }
  }
  if (debugging)
    fprintf(debug.f, "error(%4ld)\n", err);
}

Local Void plural(thefile, number, blank, LINK)
_TEXT *thefile;
long number;
Char blank;
struct LOC_librarian *LINK;
{
  if (number != 1)
    putc('s', thefile->f);
  else {
    putc(blank, thefile->f);
 }
}

Local Void writeerrors(LINK)
struct LOC_librarian *LINK;
{
  long another, e;
  long errornumber = 1, errorsonline = 0, warningsonline = 0;
  long FORLIM;

  if (LINK->numoferrors == 0)
    return;

  FORLIM = LINK->numoferrors;
  for (e = 0; e < FORLIM; e++) {
    if (!awarning(LINK->errorline.err[e], LINK))
      errorsonline++;
    else
      warningsonline++;
  }
 
  if (errorsonline > 0) {
    fprintf(listing.f, " ---error");
    plural(&listing, errorsonline, '-', LINK);
    fprintf(listing.f, "------");
  } else {
    fprintf(listing.f, " ---warning");
    plural(&listing, warningsonline, '-', LINK);
    fprintf(listing.f, "----");
  }

  LINK->lineposition = 1;
  while (LINK->numoferrors > 0 && errornumber <= maxerrors) {
    if (LINK->lineposition < LINK->errorline.pos[errornumber-1]) {
      LINK->lineposition++;
      putc('-', listing.f);
      continue;
    }
    iwritenumber('^', LINK->errorline.err[errornumber-1], LINK);
    if (LINK->errorline.err[errornumber-1] == 19 ||
	(LINK->errorline.err[errornumber-1] >= 21 &&
	 LINK->errorline.err[errornumber-1] <= 23) ||
	LINK->errorline.err[errornumber-1] == 25 ||
	LINK->errorline.err[errornumber-1] == 30 ||
	LINK->errorline.err[errornumber-1] == 222 ||
	LINK->errorline.err[errornumber-1] == 218 ||
	LINK->errorline.err[errornumber-1] == 217 ||
	LINK->errorline.err[errornumber-1] == 216 ||
	LINK->errorline.err[errornumber-1] == 215 ||
	LINK->errorline.err[errornumber-1] == 211)
      another = LINK->errorline.err[errornumber-1];

    errornumber++;
    LINK->numoferrors--;
  }

  if (LINK->numoferrors > 0)
    fprintf(listing.f, " and other errors");
  LINK->numoferrors = 0;
  putc('\n', listing.f);

  LINK->pageline++;

  if (another == 19) {
    fprintf(listing.f, " Change must be identified by one of: acgtdi\n");
    fprintf(listing.f,
	    " Instead, the illegal change character \"%c\" was found\n",
	    mutischar);
    LINK->pageline++;
  }

  if (another == 21) {
    fprintf(listing.f, " \n");
    fprintf(listing.f, " Change bases must be one of a, c, g, or t.\n");
    fprintf(listing.f, " Instead, \"%c\" was found\n", mutischar);
    LINK->pageline++;
  }

  if (another == 22) {
    fprintf(listing.f, " Insertion bases must be one of a, c, g, or t.\n");
    fprintf(listing.f, " Instead, \"%c\" was found\n", mutischar);
    LINK->pageline++;
  }

  if (another == 25) {
    fprintf(listing.f, " A comma (,) must separate insertion parts.\n");
    fprintf(listing.f, " Instead, \"%c\" was found\n", mutischar);
    LINK->pageline++;
  }

  if (another == 23) {
    fprintf(listing.f, " A comma (,) must separate deletion parts.\n");
    fprintf(listing.f, " Instead, \"%c\" was found\n", mutischar);
    LINK->pageline++;
  }
  if (another == 30) {
    fprintf(listing.f, " Unidentified change command:");
    fprintf(listing.f, " \"%c\"\n", mutischar);
    LINK->pageline++;
  }

  if (another == 211) {
    fprintf(listing.f,
      "On the positively oriented strand, the old base at %ld is NOT %c!  It is %c.\n",
      mutposition1, mutnotchar, mutischar);
    LINK->pageline++;
  }
  if (another == 215) {
    fprintf(listing.f,
      "The requested coordinate at %ld is off the piece in the 5' direction\n",
      mutposition1);
    LINK->pageline++;
  }
  if (another == 216) {
    fprintf(listing.f,
      "The requested coordinate at %ld is off the piece in the 3' direction\n",
      mutposition1);
    LINK->pageline++;
  }
  if (another == 217) {
    fprintf(listing.f,
	    "The requested coordinates %ld and %ld are out of order.\n",
	    mutposition1, mutposition2);
    LINK->pageline++;
  }
  if (another == 218) {
    fprintf(listing.f,
	    "The requested coordinates \"%ld,%ld\" cannot be equal.\n",
	    mutposition1, mutposition2);
    LINK->pageline++;
  }

  if (another != 222)
    return;

  fprintf(listing.f, "Overlapping changes: ");
  wchangedata(&listing, mutcd1);
  fprintf(listing.f, " overlaps ");
  wchangedata(&listing, mutcd2);
  putc('\n', listing.f);

  LINK->pageline++;

}

Local Void ivaluenumber(p, LINK)
long p;
struct LOC_librarian *LINK;
{
   LINK->numofnumbers++;
  if (LINK->numofnumbers <= maxnumbers) {
    LINK->numberline.pos[LINK->numofnumbers-1] = LINK->lineposition - 1;
    LINK->numberline.num[LINK->numofnumbers-1] = p;
  }
  if (debugging)
    fprintf(debug.f, "ivaluenumber%5ld\n", p);
}

Local Void writenumbers(LINK)
struct LOC_librarian *LINK;
{
  long i;
  long number = 1;
  long FORLIM;

  if (LINK->numofnumbers == 0)
    return;
  FORLIM = LINK->indentlisting;
  for (i = 1; i <= FORLIM; i++)
    putc(' ', listing.f);
  LINK->lineposition = 0;
  while (LINK->numofnumbers > 0 && number <= maxnumbers) {
    if (LINK->lineposition >= LINK->numberline.pos[number-1]) {
      iwritenumber('#', LINK->numberline.num[number-1], LINK);
      number++;
      LINK->numofnumbers--;
    } else {
      LINK->lineposition++;
      putc(' ', listing.f);
    }
  }
  if (LINK->numofnumbers > 0)
    fprintf(listing.f, "and others (sorry)");
  LINK->numofnumbers = 0;
  putc('\n', listing.f);
  LINK->pageline++;
}

Local Void ivalueposition(p, LINK)
long p;
struct LOC_librarian *LINK;
{
  LINK->numofpositions++;
  if (LINK->numofpositions <= maxpositions) {
    LINK->positionline.pos[LINK->numofpositions-1] = LINK->lineposition - 1;
    LINK->positionline.psn[LINK->numofpositions-1] = p;
    LINK->positionline.rea[LINK->numofpositions-1] = p;
  }
  if (debugging)
    fprintf(debug.f, "ivalueposition%5ld\n", p);
}

Local Void rvalueposition(r, LINK)
double r;
struct LOC_librarian *LINK;
{
  LINK->numofpositions++;
  if (LINK->numofpositions > maxpositions)
    return;
  LINK->positionline.pos[LINK->numofpositions-1] = LINK->lineposition - 1;

  LINK->positionline.psn[LINK->numofpositions-1] = (long)floor(r + 0.5) + 10;
  LINK->positionline.rea[LINK->numofpositions-1] = r;
}

Local Void writepositions(LINK)
struct LOC_librarian *LINK;
{
  long i;
  long number = 1;
  long FORLIM;

  if (LINK->numofpositions == 0)
    return;
  FORLIM = LINK->indentlisting;
  for (i = 1; i <= FORLIM; i++)
    putc(' ', listing.f);
  LINK->lineposition = 1;
  while (LINK->numofpositions > 0 && number <= maxpositions) {
    if (LINK->lineposition < LINK->positionline.pos[number-1]) {
      LINK->lineposition++;
      putc(' ', listing.f);
      continue;
    }
    if (LINK->positionline.psn[number-1] !=
	(long)floor(LINK->positionline.rea[number-1] + 0.5))
      rwritenumber('^', LINK->positionline.rea[number-1], LINK);
    else
      iwritenumber('^', LINK->positionline.psn[number-1], LINK);
    number++;
    LINK->numofpositions--;
  }
  if (LINK->numofpositions > 0)
    fprintf(listing.f, "and others (sorry)");
  LINK->numofpositions = 0;
  putc('\n', listing.f);
  LINK->pageline++;
}

Local Void writechanges(LINK)
struct LOC_librarian *LINK;
{
  long i;

  if (LINK->numofchanges > 0) {
    fprintf(listing.f, "Mutation");
    if (mutations.number > 1)
      fprintf(listing.f, "s:");
    else
      fprintf(listing.f, ": ");

    for (i = 1; i <= instwidth * 2 - 6; i++)
      putc(' ', listing.f);
    describechangeset(&listing, mutations);
    putc('\n', listing.f);
  }
  LINK->numofchanges = 0;
}

Local Void writelineinformation(LINK)
struct LOC_librarian *LINK;
{
  writenumbers(LINK);
  writepositions(LINK);
  writechanges(LINK);
}

/* Local variables for readinstruction: */
struct LOC_readinstruction {
  struct LOC_librarian *LINK;
} ;

Local node noder(word, LINK)
delilaword *word;
struct LOC_readinstruction *LINK;
{
  node Result;

  switch (*word) {
  case orgdelila:
    Result = orgnode;
    break;
  case chrdelila:
    Result = chrnode;
    break;
  case mardelila:
    Result = marnode;
    break;
  case tradelila:
    Result = tranode;
    break;
  case gendelila:
    Result = gennode;
    break;
  case piedelila:
    Result = pienode;
    break;
  case recdelila:
    Result = recnode;
    break;
  case enzdelila:
    Result = enznode;
    break;
  }
  return Result;
}

Local Void ichread(LINK)
struct LOC_readinstruction *LINK;
{
  LINK->LINK->linebreak = false;

  if (!BUFEOF(inst.f)) {
    if (P_eoln(inst.f)) {
      do {
	if (debugging)
	  fprintf(listing.f, "ichread: LINE BREAK\n");
	LINK->LINK->linebreak = true;
	fscanf(inst.f, "%*[^\n]");
	getc(inst.f);
	putc('\n', listing.f);
	LINK->LINK->pageline++;
	if (LINK->LINK->pass == 2)
	  writelineinformation(LINK->LINK);
	writeerrors(LINK->LINK);
	if ((LINK->LINK->pageline >= pagelength) & (!BUFEOF(inst.f))) {
	  fprintf(listing.f, "\f");
	  pageheader(LINK->LINK);
	}
	if (!BUFEOF(inst.f))
	  startlistingline(LINK->LINK);
	LINK->LINK->lineposition = 0;
      } while (!(BUFEOF(inst.f) | (!P_eoln(inst.f))));
    }
  }
  if (BUFEOF(inst.f)) {
    if (LINK->LINK->quote)
      error(12L, LINK->LINK);
    if (!LINK->LINK->eoinst)
      error(13L, LINK->LINK);
    LINK->LINK->eoinst = true;
    if (LINK->LINK->pass == 2)
      writelineinformation(LINK->LINK);
    writeerrors(LINK->LINK);
  } else {
    LINK->LINK->chr = getc(inst.f);
    if (LINK->LINK->chr == '\n')
      LINK->LINK->chr = ' ';
    if (LINK->LINK->chr == 9)
      LINK->LINK->chr = ' ';
    LINK->LINK->lineposition++;

    putc(LINK->LINK->chr, listing.f);
  }

  if (LINK->LINK->chr < 32 || LINK->LINK->chr > 126) {
    printf("Bad character found in inst file: ord(chr)=%12d\n",
	   LINK->LINK->chr);
    error(33L, LINK->LINK);
  }
}

/* Local variables for findword: */
struct LOC_findword {
  struct LOC_readinstruction *LINK;
} ;

/* Local variables for findnonblank: */
struct LOC_findnonblank {
  struct LOC_findword *LINK;
} ;

Local Void skipblanks(LINK)
struct LOC_findnonblank *LINK;
{
  while ((LINK->LINK->LINK->LINK->chr == ' ') & (!BUFEOF(inst.f)))
    ichread(LINK->LINK->LINK);

}

Local Void findnonblank(LINK)
struct LOC_findword *LINK;
{
  struct LOC_findnonblank V;

  V.LINK = LINK;
  skipblanks(&V);
  if (BUFEOF(inst.f))
    return;
  if (LINK->LINK->LINK->chr == '(') {
    LINK->LINK->LINK->parentheses++;
    ichread(LINK->LINK);
    if (LINK->LINK->LINK->chr != '*')
      return;
    LINK->LINK->LINK->commentline = LINK->LINK->LINK->linecount;
    LINK->LINK->LINK->comment = true;
    while ((LINK->LINK->LINK->chr != ')') & (!BUFEOF(inst.f))) {
      while ((LINK->LINK->LINK->chr != '*') & (!BUFEOF(inst.f)))
	ichread(LINK->LINK);
      if (BUFEOF(inst.f))
	error(10L, LINK->LINK->LINK);
      else
	ichread(LINK->LINK);
    }
    LINK->LINK->LINK->comment = false;
    if (!BUFEOF(inst.f))
      ichread(LINK->LINK);
    LINK->LINK->LINK->parentheses--;
    return;
  }
  if (LINK->LINK->LINK->chr == '{') {
    LINK->LINK->LINK->commentline = LINK->LINK->LINK->linecount;
    while ((LINK->LINK->LINK->chr != '}') & (!BUFEOF(inst.f)))
      ichread(LINK->LINK);
    if (BUFEOF(inst.f))
      error(10L, LINK->LINK->LINK);
    else
      ichread(LINK->LINK);
    return;
  }
  if (LINK->LINK->LINK->chr == ')') {
    LINK->LINK->LINK->parentheses--;
    ichread(LINK->LINK);
    skipblanks(&V);
    return;
  }
  if (LINK->LINK->LINK->chr == ';') {
    LINK->LINK->LINK->eoinst = true;
    ichread(LINK->LINK);
  } else if (LINK->LINK->LINK->chr != ' ')
    LINK->LINK->LINK->significant = true;
}

Local Void findword(LINK)
struct LOC_readinstruction *LINK;
{
  struct LOC_findword V;

  V.LINK = LINK;
  LINK->LINK->significant = false;

  while ((!LINK->LINK->significant) & (!BUFEOF(inst.f)))
    findnonblank(&V);
}

Local Void showparsed(debug, parsedword, parsedlength, parsedlocation, LINK)
_TEXT *debug;
Char *parsedword;
long parsedlength, parsedlocation;
struct LOC_readinstruction *LINK;
{
  long index;

  fprintf(debug->f, "parsedword: \"");
  for (index = 0; index < parsedlength; index++)
    putc(parsedword[index], debug->f);
  fprintf(debug->f, "\" <- parsedlength = %ld\n", parsedlength);
  fprintf(debug->f, "             ");
  for (index = 1; index < parsedlocation; index++)
    putc(' ', debug->f);
  fprintf(debug->f, "^ parsedlocation = %ld\n", parsedlocation);
}

Local Void parse(anumber, LINK)
boolean anumber;
struct LOC_readinstruction *LINK;
{
  long parsedwordindex, j;
  boolean stopcharacter, stopcondition;

  for (parsedwordindex = 0; parsedwordindex < widinst; parsedwordindex++)
    LINK->LINK->parsedword[parsedwordindex] = ' ';
  LINK->LINK->parsedlength = 1;
  LINK->LINK->parsedword[LINK->LINK->parsedlength-1] = LINK->LINK->chr;

  if (!P_eoln(inst.f)) {
    do {
      LINK->LINK->correct = true;
      if (LINK->LINK->parsedlength >= widinst)
	error(9L, LINK->LINK);
      if (LINK->LINK->correct) {
	ichread(LINK);

	if (!BUFEOF(inst.f)) {
	  LINK->LINK->parsedlength++;
	  LINK->LINK->parsedword[LINK->LINK->parsedlength-1] = LINK->LINK->chr;
	}
      }

      stopcharacter = (LINK->LINK->chr == ';' || LINK->LINK->chr == ')' ||
		       LINK->LINK->chr == '(' || LINK->LINK->chr == ' ') |
		      (!P_inset(LINK->LINK->chr, LINK->LINK->numbers) &&
		       LINK->LINK->chr != '-' && LINK->LINK->chr != '+' &&
		       LINK->LINK->chr != '.' && anumber);
      stopcondition = ((LINK->LINK->linebreak | BUFEOF(inst.f)) ||
		       !LINK->LINK->correct);
    } while (!(stopcharacter || stopcondition));

    if (stopcharacter)
      LINK->LINK->parsedlength--;


    if (LINK->LINK->chr == ')' || LINK->LINK->chr == '(') {
      switch (LINK->LINK->chr) {

      case '(':
	LINK->LINK->parentheses++;
	break;

      case ')':
	LINK->LINK->parentheses--;
	break;
      }
      ichread(LINK);
    }
    if (LINK->LINK->parsedlength < widinst) {
      for (j = LINK->LINK->parsedlength; j < widinst; j++)
	LINK->LINK->parsedword[j] = ' ';
    }
    LINK->LINK->eoinst = (LINK->LINK->chr == ';') | BUFEOF(inst.f);
  }

  else {
    ichread(LINK);
 
  }
 
  if (debugging)
    showparsed(&debug, LINK->LINK->parsedword, LINK->LINK->parsedlength,
	       LINK->LINK->parsedlocation, LINK);

}

Local Void geteoinst(LINK)
struct LOC_readinstruction *LINK;
{

  boolean previouscorrect;

  if (debugging)
    fprintf(debug.f, "geteoinst\n");
  previouscorrect = LINK->LINK->correct;
  while ((!LINK->LINK->eoinst) & (!BUFEOF(inst.f))) {
    findword(LINK);
    if (!LINK->LINK->eoinst) {
      if (!BUFEOF(inst.f))
	parse(false, LINK);
    }
  }

  LINK->LINK->correct = (LINK->LINK->correct && previouscorrect);
}

Local Void irquote(aline, LINK)
line **aline;
struct LOC_readinstruction *LINK;
{
  line *workline, *nextline;
  Char aquote;

  LINK->LINK->correct = true;
  findword(LINK);
  if (!LINK->LINK->correct)
    return;
  workline = *aline;
  if (workline != NULL) {
    while (workline->next != NULL) {
      if (workline == workline->next) {
	printf("PROGRAM ERROR: An infinite end loop was found in irquote\n");
	workline->next = NULL;
      } else
	workline = workline->next;
    }
    getlineDelila(&nextline);
    workline->next = nextline;
    workline = nextline;
  } else {
    getlineDelila(&workline);
    *aline = workline;
  }
  aquote = LINK->LINK->chr;
  if (LINK->LINK->chr != '"' && LINK->LINK->chr != '\'') {
    error(16L, LINK->LINK);
    geteoinst(LINK);
    return;
  }
  LINK->LINK->quote = true;
  ichread(LINK);
  while (LINK->LINK->chr != aquote && LINK->LINK->correct) {
    if (workline->length == linelength || LINK->LINK->linebreak) {
      getlineDelila(&nextline);
      workline->next = nextline;
      workline = nextline;
    }
    workline->length++;
    workline->letters[workline->length - 1] = LINK->LINK->chr;
    ichread(LINK);
  }
  LINK->LINK->quote = false;
  if (LINK->LINK->correct)
    ichread(LINK);
}

Local Void irtitle(LINK)
struct LOC_readinstruction *LINK;
{
  line *extra;

  irquote(&LINK->LINK->title, LINK);
  if (!LINK->LINK->correct)
    return;

  extra = LINK->LINK->title->next;
  while (extra != NULL)
    clearline(&extra);
  LINK->LINK->title->next = NULL;
}

Local Void irlongname(LINK)
struct LOC_readinstruction *LINK;
{
  line *extra;
  _TEXT TEMP;

  clearline(&longname);
  irquote(&longname, LINK);
  if (LINK->LINK->correct)
    longnameexists = true;
  else
    longnameexists = false;

  extra = longname->next;
  if (extra == NULL)
    return;
  printf("WARNING: the extra letters of this long name are being ignored:\n");
  putchar('"');
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeline(&TEMP, longname, false);
  printf("\"\n");
  printf("The extra letters are: \n");
  putchar('"');
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeline(&TEMP, extra, false);
  printf("\"\n");
  emptyline(&extra);
  longname->next = NULL;
}

Local Void irword(LINK)
struct LOC_readinstruction *LINK;
{
  boolean matching;
  long i;

  findword(LINK);
  if (LINK->LINK->eoinst) {
    error(15L, LINK->LINK);
    return;
  }
  parse(false, LINK);
  if (LINK->LINK->correct) {
    matching = false;
    LINK->LINK->word = alldelila;


    while (!matching && (int)LINK->LINK->word < (int)eodelila) {
      matching = true;
      i = 0;
      while (LINK->LINK->parsedword[i] != ' ' && matching && i < widinst - 1) {
	i++;
	matching = (LINK->LINK->parsedword[i-1] ==
		    LINK->LINK->wordlist[(long)LINK->LINK->word][i-1]);
      }
      if (matching) {
	if (i <= minword) {
	  if (LINK->LINK->wordlist[(long)LINK->LINK->word][i] != ' ')
	    matching = false;
	}
      }
      if (!matching)
	LINK->LINK->word = (delilaword)((long)LINK->LINK->word + 1);
    }
    LINK->LINK->correct = matching;
  }
  if (LINK->LINK->correct)
    return;

  LINK->LINK->lineposition--;
  error(2L, LINK->LINK);
  LINK->LINK->lineposition++;
  geteoinst(LINK);
}

Local Void irkeyname(LINK)
struct LOC_readinstruction *LINK;
{
  long i;

  if (LINK->LINK->eoinst) {
    error(15L, LINK->LINK);
    return;
  }
  findword(LINK);
  parse(false, LINK);
  if (!LINK->LINK->correct)
    return;
  LINK->LINK->keyname.length = 0;

  for (i = 0; i < namelength; i++) {
    LINK->LINK->keyname.letters[i] = LINK->LINK->parsedword[i];
    if (LINK->LINK->parsedword[i] != ' ')
      LINK->LINK->keyname.length++;
  }
  if (LINK->LINK->keyname.length < widinst)
    LINK->LINK->correct = (LINK->LINK->parsedword[LINK->LINK->keyname.length] ==
			   ' ');
  else
    LINK->LINK->correct = true;
  if (!LINK->LINK->correct) {
    error(14L, LINK->LINK);
    geteoinst(LINK);
  }

}

/* Local variables for irnumber: */
struct LOC_irnumber {
  struct LOC_readinstruction *LINK;
  long i, start, increment;
} ;

Local Void digitize(LINK)
struct LOC_irnumber *LINK;
{
  switch (LINK->LINK->LINK->parsedword[LINK->i-1]) {
    case '0':
      LINK->increment = 0;
      break;
    case '1':
      LINK->increment = 1;
      break;
    case '2':
      LINK->increment = 2;
      break;
    case '3':
      LINK->increment = 3;
      break;
    case '4':
      LINK->increment = 4;
      break;
    case '5':
      LINK->increment = 5;
      break;
    case '6':
      LINK->increment = 6;
      break;
    case '7':
      LINK->increment = 7;
      break;
    case '8':
      LINK->increment = 8;
      break;
    case '9':
      LINK->increment = 9;
      break;
  }
}

Local Void signblank(LINK)
struct LOC_irnumber *LINK;
{
  if (LINK->LINK->LINK->parsedword[LINK->i] != ' ')
    return;
  findword(LINK->LINK);
  parse(true, LINK->LINK);
  LINK->start = 1;
}

Local Void irnumber(LINK)
struct LOC_readinstruction *LINK;
{
  struct LOC_irnumber V;
  long sign, stop, power;
  boolean areal = false;
  long FORLIM;

  V.LINK = LINK;
  LINK->LINK->inumber = -LONG_MAX;
  if (LINK->LINK->eoinst) {
    error(15L, LINK->LINK);
    return;
  }
  findword(LINK);
  parse(true, LINK);
  if (LINK->LINK->correct) {
    V.i = 1;
    if (LINK->LINK->parsedword[V.i-1] == '+') {
      sign = 1;
      V.start = 2;
      signblank(&V);
    } else if (LINK->LINK->parsedword[V.i-1] == '-') {
      sign = -1;
      V.start = 2;
      signblank(&V);
    } else {
      sign = 1;
      V.start = 1;
    }
    V.i = V.start;
    LINK->LINK->correct = true;
    while (V.i < widinst && LINK->LINK->parsedword[V.i-1] != ' ' &&
	   LINK->LINK->correct && !areal) {
      if (LINK->LINK->parsedword[V.i-1] == '.')
	areal = true;
      LINK->LINK->correct = (P_inset(LINK->LINK->parsedword[V.i-1],
				     LINK->LINK->numbers) || areal);
      if (!areal)
	V.i++;
    }

    if (LINK->LINK->correct) {
      stop = V.i - 1;
      power = 1;
      LINK->LINK->inumber = 0;
      FORLIM = V.start;
      for (V.i = stop; V.i >= FORLIM; V.i--) {
	digitize(&V);
	LINK->LINK->inumber += power * V.increment;
	power *= 10;
      }
      LINK->LINK->inumber *= sign;

      LINK->LINK->rnumber = LINK->LINK->inumber;
      if (areal) {
	V.start = stop + 2;
	power = 10;
	V.i = V.start;
	while (LINK->LINK->parsedword[V.i-1] != ' ') {
	  digitize(&V);

	  LINK->LINK->rnumber += (double)V.increment / power;
	  power *= 10;
	  V.i++;
	}
      }

      if (areal)
	rvalueposition(LINK->LINK->rnumber, LINK->LINK);
      else
	ivalueposition(LINK->LINK->inumber, LINK->LINK);
    }
  }
  if (!LINK->LINK->correct) {
    error(4L, LINK->LINK);
    geteoinst(LINK);
  }
}

Local Void mutationparseerror(e, LINK)
long e;
struct LOC_readinstruction *LINK;
{
  long shift;
  shift = LINK->LINK->parsedlength - LINK->LINK->parsedlocation + 1;
  LINK->LINK->lineposition -= shift;
  error(e, LINK->LINK);
  LINK->LINK->lineposition += shift;
}

Local Void grabcharacter(c_, done, LINK)
Char *c_;
boolean *done;
struct LOC_readinstruction *LINK;
{
  if (LINK->LINK->parsedlocation >= LINK->LINK->parsedlength) {
    *done = true;
    *c_ = ' ';
    return;
  }
  LINK->LINK->parsedlocation++;
  *c_ = LINK->LINK->parsedword[LINK->LINK->parsedlocation-1];
  *done = false;
}

/* Local variables for grabnumber: */
struct LOC_grabnumber {
  struct LOC_readinstruction *LINK;
  boolean *correct;
  long digits, signnumber;
} ;

Local Void signstuff(LINK)
struct LOC_grabnumber *LINK;
{
  if (LINK->digits > 0) {
    mutationparseerror(31L, LINK->LINK);
    printf("Numbers cannot have more than 1 sign (+ or -).\n");
    *LINK->correct = false;
  }
  LINK->signnumber++;
}

Local Void grabnumber(number, donereading, correct_, LINK)
long *number;
boolean *donereading, *correct_;
struct LOC_readinstruction *LINK;
{
  struct LOC_grabnumber V;
  long digit;
  boolean donenumber = false;
  long sign = 1;
  V.LINK = LINK;
  V.correct = correct_;

  if (!*V.correct)
    return;

  *number = 0;
  V.digits = 0;
  V.signnumber = 0;
  while (!donenumber) {
    grabcharacter(&mutischar, donereading, LINK);
    if (*donereading)
      donenumber = true;

    if (donenumber)
      break;
    if (P_inset(mutischar, LINK->LINK->numbers)) {
      digit = mutischar - '0';
      V.digits++;
      *number = *number * 10 + digit;

      continue;
    }
    if (mutischar == '-') {
      sign = -1;
      signstuff(&V);
      continue;
    }
    if (mutischar == '+') {
      sign = 1;
      signstuff(&V);
    } else {
      LINK->LINK->parsedlocation--;
      donenumber = true;
    }
  }
  if (V.digits == 0) {
    mutationparseerror(26L, LINK);
    *V.correct = false;
  }
  if (V.signnumber > 1) {
    mutationparseerror(31L, LINK);
    *V.correct = false;
  }

  if (sign < 0)
    *number = -*number;
}

Local Void readchanges(c_, done, LINK)
changeset *c_;
boolean *done;
struct LOC_readinstruction *LINK;
{
  changedata *WITH;
  WITH = &c_->data[c_->number - 1];
  WITH->changetype = 'c';
  grabcharacter(&WITH->baseold, done, LINK);
  WITH->baseold = decapitalize(WITH->baseold);

  if (WITH->baseold != 't' && WITH->baseold != 'g' && WITH->baseold != 'c' &&
      WITH->baseold != 'a') {
    printf("ERROR: old base usually should be a, c, g, t\n");
    mutationparseerror(20L, LINK);
  }
  grabnumber(&WITH->basecoo1, done, &LINK->LINK->correct, LINK);
 
  if (!LINK->LINK->correct)
    return;
  WITH->basecoo2 = WITH->basecoo1;
  grabcharacter(&WITH->basenew, done, LINK);
  WITH->basenew = decapitalize(WITH->basenew);

  if (WITH->basenew != 't' && WITH->basenew != 'g' && WITH->basenew != 'c' &&
      WITH->basenew != 'a') {
    printf("ERROR: new base should be a, c, g, or t\n");
    mutischar = WITH->basenew;
    mutationparseerror(21L, LINK);
  }
  WITH->inserts = 0;
}

Local Void checknumberorder(basecoo1, basecoo2, LINK)
double basecoo1, basecoo2;
struct LOC_readinstruction *LINK;
{
  if (basecoo1 > basecoo2) {
    error(27L, LINK->LINK);

    LINK->LINK->correct = false;
  }
}

Local Void readinsertion(c_, done, LINK)
changeset *c_;
boolean *done;
struct LOC_readinstruction *LINK;
{
  boolean doneinsert = false;
  changedata *WITH;
  WITH = &c_->data[c_->number - 1];

  grabcharacter(&WITH->changetype, done, LINK);
  if (*done)
    LINK->LINK->correct = false;
  if (!LINK->LINK->correct)
    return;
  grabnumber(&WITH->basecoo1, done, &LINK->LINK->correct, LINK);
  if (*done)
    LINK->LINK->correct = false;
  if (!LINK->LINK->correct)
    return;
  grabcharacter(&mutischar, done, LINK);
  if (*done)
    LINK->LINK->correct = false;
  if (mutischar != ',') {
    printf(" comma expected between coordinates for insertion\n");
    mutationparseerror(25L, LINK);
  }
  if (!LINK->LINK->correct)
    return;
  grabnumber(&WITH->basecoo2, done, &LINK->LINK->correct, LINK);
  if (LINK->LINK->correct)
    checknumberorder((double)WITH->basecoo1, (double)WITH->basecoo2, LINK);
  if (!LINK->LINK->correct)
    return;
  WITH->inserts = 0;
  WITH->insertcomplement = false;
  do {
    grabcharacter(&mutischar, done, LINK);
    if (!*done) {
        if (mutischar == '.')
	doneinsert = true;
      else if (mutischar == '~') {
	if (WITH->inserts != 0) {
	  doneinsert = true;
	  LINK->LINK->correct = false;
	  mutationparseerror(32L, LINK);

	} else
	  WITH->insertcomplement = true;
      } else if (mutischar == 't' || mutischar == 'g' || mutischar == 'c' ||
		 mutischar == 'a') {
	WITH->inserts++;
	WITH->insert[WITH->inserts - 1] = mutischar;
	if (WITH->inserts > insertmax) {
	  printf(
	    " no more than %ld insertion bases allowed, increase constant insertmax\n",
	    (long)insertmax);
	  mutationparseerror(24L, LINK);
	  doneinsert = true;
	  LINK->LINK->correct = false;
	}
      } else {
	doneinsert = true;
	LINK->LINK->correct = false;
	mutationparseerror(22L, LINK);

      }
      if (LINK->LINK->parsedlocation >= LINK->LINK->parsedlength)
	doneinsert = true;
    } else
      doneinsert = true;
  } while (!doneinsert);
}

Local Void readdeletion(c_, done, LINK)
changeset *c_;
boolean *done;
struct LOC_readinstruction *LINK;
{
  changedata *WITH;
  WITH = &c_->data[c_->number - 1];

  WITH->inserts = 0;
  grabcharacter(&WITH->changetype, done, LINK);
  if (!LINK->LINK->correct)
    return;
  grabnumber(&WITH->basecoo1, done, &LINK->LINK->correct, LINK);
  if (!LINK->LINK->correct)
    return;
  grabcharacter(&mutischar, done, LINK);
  if (!LINK->LINK->correct)
    return;
  if (mutischar != ',') {
    printf("comma expected between coordinates for deletion\n");
    mutationparseerror(23L, LINK);
    *done = true;
    LINK->LINK->correct = false;
  }
 
  if (LINK->LINK->correct)
    grabnumber(&WITH->basecoo2, done, &LINK->LINK->correct, LINK);
 
  if (LINK->LINK->correct)
    checknumberorder((double)WITH->basecoo1, (double)WITH->basecoo2, LINK);
}

Local Void readchangeset(c_, LINK)
changeset *c_;
struct LOC_readinstruction *LINK;
{
  boolean done = false, dotfound = false;
  Char lastcharacter;

  c_->number = 0;
  findword(LINK);
  parse(false, LINK);

  if (!LINK->LINK->correct)
    return;

  LINK->LINK->parsedlocation = 0;
  grabcharacter(&mutischar, &done, LINK);
  if (mutischar == '.') {
    printf("Extra dots not allowed\n");
    mutationparseerror(29L, LINK);
    LINK->LINK->correct = false;
  }


  LINK->LINK->parsedlocation = 0;

  while (!done) {
    if (!LINK->LINK->correct)
      continue;

    grabcharacter(&mutischar, &done, LINK);
    if (!done) {
      if (mutischar == '.') {
	if (dotfound) {
	  printf("Extra dots not allowed\n");
	  mutationparseerror(29L, LINK);
	  done = true;
	} else
	  dotfound = true;
      } else if (mutischar == 'i' || mutischar == 'd' || mutischar == 'T' ||
		 mutischar == 'G' || mutischar == 'C' || mutischar == 'A' ||
		 mutischar == 't' || mutischar == 'g' || mutischar == 'c' ||
		 mutischar == 'a') {
	c_->number++;
	if (c_->number > changesetmax) {
	  printf("Too many changes requested, increase constant changesetmax.");
	  mutationparseerror(28L, LINK);
	  LINK->LINK->correct = false;
	  done = true;
	}
	if (LINK->LINK->correct) {
	  dotfound = false;
	  LINK->LINK->parsedlocation--;
	
	  switch (mutischar) {
      case 'A':
      case 'C':
      case 'G':
      case 'T':
        readchanges(c_, &done, LINK);
        break;
      case 'a':
      case 'c':
      case 'g':
      case 't':
        readchanges(c_, &done, LINK);
        break;
      case 'd':
        readdeletion(c_, &done, LINK);
        break;
      case 'i':
        readinsertion(c_, &done, LINK);
        break;
	  }
	}
      } else {
	printf("Unidentified change command:");
	printf(" \"%c\"\n", mutischar);
	mutationparseerror(30L, LINK);
	done = true;
	LINK->LINK->correct = false;
      }
      lastcharacter = mutischar;
    }
    if (!LINK->LINK->correct)
      done = true;
  }
  if (lastcharacter == '.') {
    printf("Extra dots not allowed\n");
    mutationparseerror(29L, LINK);
  }
}

/* Local variables for sortchanges: */
struct LOC_sortchanges {
  struct LOC_readinstruction *LINK;
  changeset *sorted;
  Char phenotype;
} ;

/* Local variables for lessthan: */
struct LOC_lessthan {
  struct LOC_sortchanges *LINK;
} ;

Local long adjust(x, LINK)
long x;
struct LOC_lessthan *LINK;
{
  long xposition;
  changedata *WITH;

  xposition = LINK->LINK->sorted->data[x-1].internal1;
  if (LINK->LINK->sorted->data[x-1].changetype != 'i')
    return xposition;
 
  WITH = &LINK->LINK->sorted->data[x-1];
 
  if (WITH->internal1 + 1 <= WITH->internal2)
    xposition++;
  return xposition;
}

Local boolean lessthan(a_, b, LINK)
long a_, b;
struct LOC_sortchanges *LINK;
{
  struct LOC_lessthan V;
  boolean Result;
  long aposition, bposition;
  Char atype, btype;

  V.LINK = LINK;
  aposition = adjust(a_, &V);
  bposition = adjust(b, &V);

  if (aposition != bposition || a_ == b)
    return (aposition < bposition);
  atype = LINK->sorted->data[a_-1].changetype;
  btype = LINK->sorted->data[b-1].changetype;

  switch (LINK->phenotype) {
    case 'w':
      if (atype == 'i')
        Result = true;
      else
        Result = false;
      break;
    case 'm':
      if (atype == 'd')
        Result = true;
      else
        Result = false;
      break;
  }

   return Result;
}

Local Void swap_(a_, b, LINK)
long a_, b;
struct LOC_sortchanges *LINK;
{
  changedata hold;

  hold = LINK->sorted->data[a_-1];
  LINK->sorted->data[a_-1] = LINK->sorted->data[b-1];
  LINK->sorted->data[b-1] = hold;

}

Local Void quicksort(left, right, LINK)
position left, right;
struct LOC_sortchanges *LINK;
{
  position lower = left;
  position upper, center;

  center = (left + right) / 2;
  upper = right;
  do {
    while (lessthan(lower, center, LINK))
      lower++;
    while (lessthan(center, upper, LINK))
      upper--;
    if (lower <= upper) {
      if (lower == center)
	center = upper;
      else if (upper == center)
	center = lower;
      swap_(lower, upper, LINK);
      lower++;
      upper--;
    }

  } while (lower <= upper);
  if (left < upper)
    quicksort(left, upper, LINK);
  if (lower < right)
    quicksort(lower, right, LINK);
}

Local Void sortchanges(unsorted, sorted_, phenotype_, LINK)
changeset unsorted, *sorted_;
Char phenotype_;
struct LOC_readinstruction *LINK;
{
  struct LOC_sortchanges V;

  V.LINK = LINK;
  V.sorted = sorted_;
  V.phenotype = phenotype_;
  *V.sorted = unsorted;
  quicksort(1L, V.sorted->number, &V);
}

/* Local variables for propagateonechange: */
struct LOC_propagateonechange {
  struct LOC_readinstruction *LINK;
  changeset *changes;
  long m, location, shift;
} ;

Local Void loop(LINK)
struct LOC_propagateonechange *LINK;
{
  long n, FORLIM;
  changedata *WITH;

  FORLIM = LINK->changes->number;
  for (n = 0; n < FORLIM; n++) {
    if (n + 1 != LINK->m) {
      if (LINK->location <= LINK->changes->data[n].internal1 ||
	  (LINK->location <= LINK->changes->data[n].internal1 + 1 &&
	   LINK->changes->data[n].changetype == 'i')) {
	WITH = &LINK->changes->data[n];
	WITH->internal1 += LINK->shift;
	WITH->internal2 += LINK->shift;

      }
    }
  }

}

Local Void propagateonechange(changes_, m_, pie, LINK)
changeset *changes_;
long m_;
piece *pie;
struct LOC_readinstruction *LINK;
{
  struct LOC_propagateonechange V;
  long b1, b2;
  long deletes, pielength;
  changeset *WITH;
  changedata *WITH1;

  V.LINK = LINK;
  V.changes = changes_;
  V.m = m_;
  WITH = V.changes;
  V.location = WITH->data[V.m-1].internal1;
  pielength = piecelength(pie);
  WITH1 = &WITH->data[V.m-1];
  b1 = WITH1->internal1;
  b2 = WITH1->internal2;
  switch (WITH1->changetype) {
      case 'c':
        /* blank case */
        break;
      case 'i':
        V.location++;
        if (b1 > pielength)
          b1 = pielength;
        if (b2 > pielength)
          b2 = pielength + 1;
        if (b1 < 0)
          b1 = 0;
        if (b2 < 1)
          b2 = 1;
        deletes = b2 - b1 - 1;
        V.shift = WITH1->inserts - deletes;
        loop(&V);
        break;
      case 'd':
        if (b1 < 1)
          b1 = 1;
        if (b2 < 1)
          b2 = 1;
        if (b1 > pielength)
          b1 = pielength;
        if (b2 > pielength)
          b2 = pielength;
        V.shift = b2 - b1 + 1;
        V.shift = -V.shift;
        loop(&V);
        break;

  }
}

Local Void doubleYmark(markspots, internal1, internal2, pie, insertlowerbits,
		       insertupperbits, LINK)
_TEXT *markspots;
long internal1, internal2;
piece *pie;
double insertlowerbits, insertupperbits;
struct LOC_readinstruction *LINK;
{
  long b1, b2, shiftY;

  if (marksdeliladebug) {
    fprintf(markspots->f, "* doubleY: internal1 = %ld\n", internal1);
    fprintf(markspots->f, "* doubleY: internal2 = %ld\n", internal2);
  }
  if (internal2 - internal1 != 1) {
    printf("doubleY: program error: internal2-internal1 <> 1\n");
    crash();
  }

  if (withininternal(pie, internal2)) {
    b1 = internal2;
    b2 = internal2;
    shiftY = 0;
    if (marksdeliladebug)
      fprintf(markspots->f, "* doubleY: case 1\n");
  } else {
    if (withininternal(pie, internal1)) {
      b1 = internal1;
      b2 = b1;
      shiftY = 1;

      if (marksdeliladebug)
	fprintf(markspots->f, "* doubleY: case 2a\n");
    } else {
      b1 = internal2;
      reduceposition(pie, &b1);
      b2 = b1;
      shiftY = 0;
      if (marksdeliladebug)
	fprintf(markspots->f, "* doubleY: case 2b\n");
    }
  }

  fprintf(markspots->f, "U %*ld %*.*f %*ld %*.*f %ld doubleY\n",
	  widbase, inttopie(b1, pie), widbits, decbits, insertlowerbits,
	  widbase, inttopie(b2, pie), widbits, decbits, insertupperbits,
	  shiftY);
}

/* Local variables for createmark: */
struct LOC_createmark {
  struct LOC_readinstruction *LINK;
  _TEXT *markspots;
} ;
Local Void kind(c_, LINK)
Char c_;
struct LOC_createmark *LINK;
{
  switch (c_) {
      case 'd':
        fprintf(LINK->markspots->f, "deletion");
        break;
      case 'i':
        fprintf(LINK->markspots->f, "insertion");
        break;
      case 'f':
        fprintf(LINK->markspots->f, "full");
        break;
      case 'l':
        fprintf(LINK->markspots->f, "left");
        break;
      case 'm':
        fprintf(LINK->markspots->f, "mid");
        break;
      case 'r':
        fprintf(LINK->markspots->f, "right");
        break;
  }
}

Local Void createmark(markspots_, b1, b2, boxposition, changetype, pie,
		      insertupperbits, insertlowerbits, deleteupperbits,
		      deletelowerbits, LINK)
_TEXT *markspots_;
long b1, b2;
Char boxposition, changetype;
piece *pie;
double insertupperbits, insertlowerbits, deleteupperbits, deletelowerbits;
struct LOC_readinstruction *LINK;
{
  struct LOC_createmark V;
  double lowerbits, upperbits;
  long piecebase1, piecebase2;

  V.LINK = LINK;
  V.markspots = markspots_;
  if (!(withininternal(pie, b1) & withininternal(pie, b2)))
    return;
  switch (changetype) {
    case 'd':
      lowerbits = deletelowerbits;
      upperbits = deleteupperbits;
      break;
    case 'i':
      lowerbits = insertlowerbits;
      upperbits = insertupperbits;
      break;
  }
  piecebase1 = inttopie(b1, pie);
  piecebase2 = inttopie(b2, pie);
  fprintf(V.markspots->f, "U %*ld %*.*f %*ld %*.*f %*d",
	  widbase, piecebase1, widbits, decbits, lowerbits, widbase,
	  piecebase2, widbits, decbits, upperbits, widbits, 0);
  fprintf(V.markspots->f, " (");
  kind(boxposition, &V);
  kind(changetype, &V);
  fprintf(V.markspots->f, ") ");
  kind(boxposition, &V);
  kind(changetype, &V);
  putc('\n', V.markspots->f);
}

Local Void multimarks(markspots, internal1, internal2, changetype, pie,
		      insertupperbits, insertlowerbits, deleteupperbits,
		      deletelowerbits, LINK)
_TEXT *markspots;
long internal1, internal2;
Char changetype;
piece *pie;
double insertupperbits, insertlowerbits, deleteupperbits, deletelowerbits;
struct LOC_readinstruction *LINK;
{
  long partindex;

  if (marksdeliladebug) {
    fprintf(markspots->f, "* Multimarks, changetype = %c\n", changetype);
    fprintf(markspots->f, "* internal1 = %ld\n", internal1);
    fprintf(markspots->f, "* internal2 = %ld\n", internal2);
  }
  if (internal1 == internal2) {
    createmark(markspots, internal1, internal2, 'f', changetype, pie,
	       insertupperbits, insertlowerbits, deleteupperbits,
	       deletelowerbits, LINK);
    return;
  }
  createmark(markspots, internal1, internal1, 'l', changetype, pie,
	     insertupperbits, insertlowerbits, deleteupperbits,
	     deletelowerbits, LINK);
  partindex = internal1 + 1;
  while (partindex < internal2) {
    createmark(markspots, partindex, partindex, 'm', changetype, pie,
	       insertupperbits, insertlowerbits, deleteupperbits,
	       deletelowerbits, LINK);

    partindex++;
  }
  createmark(markspots, internal2, internal2, 'r', changetype, pie,
	     insertupperbits, insertlowerbits, deleteupperbits,
	     deletelowerbits, LINK);
}

Local Void setinternal(changes, pie, LINK)
changeset *changes;
piece *pie;
struct LOC_readinstruction *LINK;
{
  long hold, n, FORLIM;
  changedata *WITH;

  FORLIM = changes->number;
  for (n = 0; n < FORLIM; n++) {
    WITH = &changes->data[n];

    WITH->internal1 = nwpietoint(WITH->basecoo1, pie);
    WITH->internal2 = nwpietoint(WITH->basecoo2, pie);



    if (WITH->internal1 > WITH->internal2) {
      hold = WITH->internal2;
      WITH->internal2 = WITH->internal1;
      WITH->internal1 = hold;
     
    }

    if (WITH->changetype == 'i') {

      if (WITH->internal1 == 0 && WITH->internal2 == 0)
	WITH->internal1 = -1;
      if ((WITH->internal1 > piecelength(pie)) &
	  (WITH->internal2 > piecelength(pie))) {
	WITH->internal1--;
	WITH->internal2 = WITH->internal1 + 1;
      }
      
      if (labs(WITH->internal1 - WITH->internal2) == 1)
	WITH->insertasdeletion = false;
      else {
	WITH->insertasdeletion = true;

      }
    }

  }
}

Local Void skippiece(marksdelila, LINK)
_TEXT *marksdelila;
struct LOC_readinstruction *LINK;
{
  fprintf(marksdelila->f, "\npiece #%ld - skip ahead to next piece\n",
	  def.num.item);
}

Local Void writewildtypemarks(markspots, changes, insertupperbits,
  insertlowerbits, deleteupperbits, deletelowerbits, changeupperbits,
  changelowerbits, pie, thenumber, LINK)
_TEXT *markspots;
changeset changes;
double insertupperbits, insertlowerbits, deleteupperbits, deletelowerbits,
       changeupperbits, changelowerbits;
piece *pie;
long thenumber;
struct LOC_readinstruction *LINK;
{
  long chorient;
  long n;
  changeset sorted;
  long thelength;
  changeset unsorted;
  long FORLIM;
  changedata *WITH;

  fprintf(markspots->f, "\n* piece #%ld ", thenumber);
  writechangeset(markspots, changes);
  putc('\n', markspots->f);
  if (pie->key.piedir == pie->key.coodir)
    chorient = 1;
  else
    chorient = -1;
  
  thelength = piecelength(pie);
  unsorted = changes;
  FORLIM = unsorted.number;

  for (n = 0; n < FORLIM; n++) {
    WITH = &unsorted.data[n];

    switch (WITH->changetype) {
    case 'c':
      /* blank case */
      break;
    case 'i':
      /* blank case */
      break;
    case 'd':
      break;

    }
  }
  sortchanges(unsorted, &sorted, 'w', LINK);

  FORLIM = sorted.number;
  for (n = 0; n < FORLIM; n++) {
    WITH = &sorted.data[n];
    switch (WITH->changetype) {

    case 'c':
      fprintf(markspots->f,
	      "U %*ld %*.*f %*ld %*.*f %*.*f (%c->%c) changeworra \n",
	      widbits, inttopie(WITH->internal1, pie), widbits, decbits,
	      changeupperbits - shiftdown, widbits,
	      inttopie(WITH->internal2, pie), widbits, decbits,
	      changelowerbits - shiftdown, widbits, decbits, 0.0,
	      WITH->baseold, WITH->basenew);

      break;

    case 'i':
      if (marksdeliladebug) {
	fprintf(markspots->f, "* alive\n");
	fprintf(markspots->f, "* basecoo1 = %ld\n", WITH->basecoo1);
	fprintf(markspots->f, "* basecoo2 = %ld\n", WITH->basecoo2);
	fprintf(markspots->f, "* internal1 = %ld\n", WITH->internal1);
	fprintf(markspots->f, "* internal2 = %ld\n", WITH->internal2);
      }

    if (WITH->insertasdeletion)
	multimarks(markspots, WITH->internal1 + 1, WITH->internal2 - 1, 'd',
		   pie, insertupperbits, insertlowerbits, deleteupperbits,
		   deletelowerbits, LINK);
      else
	doubleYmark(markspots, WITH->internal1, WITH->internal2, pie,
		    insertlowerbits, insertupperbits, LINK);
      break;
     case 'd':
      multimarks(markspots, WITH->internal1, WITH->internal2, 'd', pie,
		 insertupperbits, insertlowerbits, deleteupperbits,
		 deletelowerbits, LINK);
      break;

    }
  }
}

#undef shiftdown

Local Void writemutantmarks(markspots, changes, insertupperbits,
  insertlowerbits, deleteupperbits, deletelowerbits, changeupperbits,
  changelowerbits, pie, thenumber, LINK)
_TEXT *markspots;
changeset changes;
double insertupperbits, insertlowerbits, deleteupperbits, deletelowerbits,
       changeupperbits, changelowerbits;
piece *pie;
long thenumber;
struct LOC_readinstruction *LINK;
{
  long displacement;
  long chorient;
  long n;
  changeset sorted;
  long thelength;
  changeset unsorted;
  long FORLIM;
  changedata *WITH;

  fprintf(markspots->f, "\n* piece #%ld ", thenumber);
 
  writechangeset(markspots, changes);
  putc('\n', markspots->f);
  if (pie->key.piedir == pie->key.coodir)
    chorient = 1;
  else
    chorient = -1;
  displacement = chorient;
  thelength = piecelength(pie);
  unsorted = changes;
  FORLIM = unsorted.number;

  for (n = 1; n <= FORLIM; n++)
    propagateonechange(&unsorted, n, pie, LINK);
  FORLIM = unsorted.number;

  for (n = 0; n < FORLIM; n++) {
    WITH = &unsorted.data[n];

    switch (WITH->changetype) {
    case 'c':
      /* blank case */
      break;
    case 'i':
      if (WITH->inserts == 0) {
	WITH->internal2 = WITH->internal1 + 1;
      } else {
	WITH->internal1++;
	WITH->internal2 = WITH->internal1 + WITH->inserts - 1;
      }
      break;
     
    case 'd':
      break;
    }
  }
  sortchanges(unsorted, &sorted, 'm', LINK);

  FORLIM = sorted.number;

  for (n = 0; n < FORLIM; n++) {
    WITH = &sorted.data[n];
    switch (WITH->changetype) {
    case 'c':
      fprintf(markspots->f,
	      "U %*ld %*.*f %*ld %*.*f %*.*f (%c->%c) change \n",
	      widbits, inttopie(WITH->internal1, pie), widbits, decbits,
	      changeupperbits, widbits, inttopie(WITH->internal2, pie),
	      widbits, decbits, changelowerbits, widbits, decbits, 0.0,
	      WITH->baseold, WITH->basenew);
      break;
    case 'i':
      if (WITH->internal1 == 0) {
	WITH->internal1++;
	WITH->internal2++;
      }
      if (WITH->internal1 > thelength) {

	WITH->internal1--;
	WITH->internal2--;
      }

      if (marksdeliladebug) {
	fprintf(markspots->f, "* NOWH internal1 = %ld\n", WITH->internal1);
	fprintf(markspots->f, "* NOWH internal2 = %ld\n", WITH->internal2);
      }
      if (WITH->inserts == 0)
	doubleYmark(markspots, WITH->internal1, WITH->internal2, pie,
		    insertlowerbits, insertupperbits, LINK);
      else
	multimarks(markspots, WITH->internal1, WITH->internal2, 'i', pie,
		   insertupperbits, insertlowerbits, deleteupperbits,
		   deletelowerbits, LINK);

      if (marksdeliladebug) {
	fprintf(markspots->f, "* VOILA internal1 = %ld\n", WITH->internal1);
	fprintf(markspots->f, "* VOILA internal2 = %ld\n", WITH->internal2);
      }
      break;
    case 'd':
      WITH->internal1--;

      WITH->internal2 = WITH->internal1 + 1;
      if (marksdeliladebug) {
	fprintf(markspots->f, "* writeMUTANTmarks DELETION internal1 = %ld\n",
		WITH->internal1);
	fprintf(markspots->f, "* writeMUTANTmarks DELETION internal2 = %ld\n",
		WITH->internal2);
      }
      doubleYmark(markspots, WITH->internal1, WITH->internal2, pie,
		  insertlowerbits, insertupperbits, LINK);
      break;

    }
  }
}

Local Void dnaconcat(Adna, Bdna, Cdna, LINK)
dnastring *Adna, *Bdna, **Cdna;
struct LOC_readinstruction *LINK;
{
  dnastring *aDNAptr, *bDNAptr, *cpart;
  long pb = 1;
  long pc, TEMP;
  emptydna(Cdna);
  getdna(Cdna);
  cpart = *Cdna;
  aDNAptr = Adna;

  while (aDNAptr != NULL) {
    memcpy(cpart->part, aDNAptr->part, sizeof(seq));
    cpart->length = aDNAptr->length;
    aDNAptr = aDNAptr->next;
    if (aDNAptr != NULL) {
      getdna(&cpart->next);
      cpart = cpart->next;
    } else
      cpart->next = NULL;
  }

  pc = cpart->length;
  bDNAptr = Bdna;
  do {
    if (cpart->length == dnamax) {
      getdna(&cpart->next);
      cpart = cpart->next;
      cpart->next = NULL;
      cpart->length = 1;
    } else {
      cpart->length++;
    }
    TEMP = cpart->length - 1;
    P_clrbits_B(cpart->part, TEMP, 1, 3);
    P_putbits_UB(cpart->part, TEMP,
		 (int)((base)P_getbits_UB(bDNAptr->part, pb - 1, 1, 3)), 1,
		 3);
    pb++;

    if (pb > bDNAptr->length) {
      bDNAptr = bDNAptr->next;
      pb = 1;
    }
  } while (bDNAptr != NULL);
}

Local Void dnacomplement(Adna, Bdna, LINK)
dnastring *Adna, **Bdna;
struct LOC_readinstruction *LINK;
{
  dnastring *a_, *bpart;
  long p;
  boolean done = false;
  long FORLIM, TEMP;
  emptydna(Bdna);
  getdna(Bdna);
  bpart = *Bdna;
  a_ = Adna;
  while (!done) {
    bpart->length = a_->length;
    FORLIM = a_->length;
    for (p = 1; p <= FORLIM; p++) {
      TEMP = a_->length - p;
      P_clrbits_B(bpart->part, TEMP, 1, 3);
      P_putbits_UB(bpart->part, TEMP,
		   (int)complement((base)P_getbits_UB(a_->part, p - 1, 1, 3)),
		   1, 3);
    }
    if (a_->next == NULL) {
      done = true;

      break;
    }
    bpart = (dnastring *)Malloc(sizeof(dnastring));
    bpart->next = *Bdna;
    *Bdna = bpart;
    a_ = a_->next;
  }
  
}

Local Void getdnasegment(big, little, s, e, LINK)
dnastring *big, **little;
long s, e;
struct LOC_readinstruction *LINK;
{
  long b;
  dnastring *bDNAptr;
  long l = 0;
  dnastring *ldna;
  long p;

  if (s > e) {
    printf("getdnasegment: s < e is required\n");
    halt();
  }

  bDNAptr = big;
  p = bDNAptr->length;
  if (p < s - 1) {
    while (p < s - 1) {
      if (bDNAptr->next == NULL) {
	printf("getdnasegment: request off end of piece\n");
	halt();
      }
      bDNAptr = bDNAptr->next;
      p += bDNAptr->length;
    }
    b = bDNAptr->length - p + s - 1;

  } else
    b = s - 1;

  emptydna(little);
  getdna(little);
  ldna = *little;
  for (p = s; p <= e; p++) {
    l++;
    if (l > dnamax) {
     
      ldna->length = dnamax;
      getdna(&ldna->next);
      ldna = ldna->next;
      l = 1;
  
    }
    b++;

    if (b > bDNAptr->length) {

      if (bDNAptr->next == NULL) {
	printf("getdnasegment: request e (%ld) is beyond piece end\n", e);
	crash();
	halt();
      }
      b = 1;
      bDNAptr = bDNAptr->next;
    }

    P_clrbits_B(ldna->part, l - 1, 1, 3);
    P_putbits_UB(ldna->part, l - 1,
		 (int)((base)P_getbits_UB(bDNAptr->part, b - 1, 1, 3)), 1, 3);
  }
  ldna->length = l;

}

/* Local variables for circledna: */
struct LOC_circledna {
  struct LOC_readinstruction *LINK;
} ;

Local Void invert(d, LINK)
dnastring **d;
struct LOC_circledna *LINK;
{
  dnastring *i;
  i = *d;
  *d = NULL;
  dnacomplement(i, d, LINK->LINK);

  cleardna(&i);
}

Local Void circledna(big, little, s, e, inverting, LINK)
piece *big, **little;
long s, e;
boolean inverting;
struct LOC_readinstruction *LINK;
{
  struct LOC_circledna V;
  dnastring *first;
  long n;
  dnastring *second;
  V.LINK = LINK;
  n = piecelength(big);

  if (s < e) {
    if (inverting) {
      if (showcase)
	printf("    1<-s e<-n     inverting\n");
      first = NULL;
      second = NULL;
      getdnasegment(big->dna, &first, 1L, s, LINK);
      invert(&first, &V);
      getdnasegment(big->dna, &second, e, n, LINK);
      invert(&second, &V);
      dnaconcat(first, second, &(*little)->dna, LINK);
      cleardna(&first);
      cleardna(&second);
      return;
    }
    if (showcase)
      printf("1  s->e  n not inverting\n");
    getdnasegment(big->dna, &(*little)->dna, s, e, LINK);
    return;
  }
  if (s > e) {
    if (inverting) {
      if (showcase)
	printf("    1  e<-s  n     inverting\n");
      getdnasegment(big->dna, &(*little)->dna, e, s, LINK);
      invert(&(*little)->dna, &V);
      return;
    }
    if (showcase)
      printf("1->e  s->n not inverting\n");
    first = NULL;
    second = NULL;
    getdnasegment(big->dna, &first, s, n, LINK);
    getdnasegment(big->dna, &second, 1L, e, LINK);
    dnaconcat(first, second, &(*little)->dna, LINK);
    cleardna(&first);
    cleardna(&second);
    return;
  }
  getdnasegment(big->dna, &(*little)->dna, s, s, LINK);
  if (inverting) {
    invert(&(*little)->dna, &V);
    
  }
}

#undef showcase

Local Void compress(pie, LINK)
piece **pie;
struct LOC_readinstruction *LINK;
{
  long p, pielength;
  long previous = 0;
  dnastring *workdna;
  pielength = piecelength(*pie);

  workdna = (*pie)->dna;
  p = workdna->length;
  while (pielength > p && workdna->next != NULL) {
 
    previous += workdna->length;
    workdna = workdna->next;
    p += workdna->length;
  }
  
  workdna->length = pielength - previous;
  if (workdna->next != NULL)
    emptydna(&workdna->next);
  
}


Local Void fixpiececoordinate(pie, excess, coordinateside, LINK)
piece **pie;
long excess;
direction coordinateside;
struct LOC_readinstruction *LINK;
{
  piekey *WITH;
  WITH = &(*pie)->key;
  switch (coordinateside) {

  case minus:
    switch (WITH->piedir) {
      case minus:
        WITH->pieend -= excess;
        WITH->coobeg -= excess;
        break;
      case plus:
        WITH->piebeg -= excess;
        WITH->coobeg -= excess;
        break;
      }
      break;
  case plus:
    switch (WITH->piedir) {
    case minus:
      WITH->piebeg += excess;
      WITH->cooend += excess;
      break;
    case plus:
      WITH->pieend += excess;
      WITH->cooend += excess;
      break;
    }
    break;
  }
}

Local Void putbase(b, position_, pie, coordinateside, LINK)
base b;
long position_;
piece **pie;
direction coordinateside;
struct LOC_readinstruction *LINK;
{
  long excess;
  dnastring *workdna;
  long p, pielength, TEMP;
  pielength = piecelength(*pie);
  
  if (position_ < 1) {
    printf(
      "putbase: can not put bases before the start of the piece (position < 1)\n");
    printf("Program error!  Please report it to toms@alum.mit.edu.\n");
    halt();
  }
  workdna = (*pie)->dna;
  if (position_ > pielength) {
    excess = position_ - pielength;
    fixpiececoordinate(pie, excess, coordinateside, LINK);
    p = workdna->length;
    while (workdna->next != NULL) {
      workdna = workdna->next;
      p += workdna->length;
    }

    if (workdna->length + excess <= dnamax) {
      workdna->length += excess;
      TEMP = workdna->length - 1;
      P_clrbits_B(workdna->part, TEMP, 1, 3);
      P_putbits_UB(workdna->part, TEMP, (int)b, 1, 3);
      return;
    }

    p += dnamax - workdna->length;
    workdna->length = dnamax;

    while (p < position_) {
      p += dnamax;
      getdna(&workdna->next);
      workdna = workdna->next;
      workdna->length = dnamax;

    }
    workdna->length = dnamax - p + position_;
    TEMP = workdna->length - 1;
    P_clrbits_B(workdna->part, TEMP, 1, 3);
    P_putbits_UB(workdna->part, TEMP, (int)b, 1, 3);
    return;
  }

  p = workdna->length;
  while (position_ > p) {

    if (workdna->next != NULL)
      workdna = workdna->next;
    p += workdna->length;

  }
  TEMP = workdna->length - p + position_ - 1;
  P_clrbits_B(workdna->part, TEMP, 1, 3);
  P_putbits_UB(workdna->part, TEMP, (int)b, 1, 3);
}


Local Char getbasech(position_, pie, LINK)
long position_;
piece *pie;
struct LOC_readinstruction *LINK;
{
  return (basetochar(getbase(position_, pie)));
}

Local Void putbasech(c_, position_, pie, coordinateside, LINK)
Char c_;
long position_;
piece *pie;
direction coordinateside;
struct LOC_readinstruction *LINK;
{
  putbase(chartobase(c_), position_, &pie, coordinateside, LINK);
}

/* Local variables for numberdigit: */
struct LOC_numberdigit {
  struct LOC_readinstruction *LINK;
  long number, place, myabsolute;
  Char acharacter;
} ;

Local Void digit(LINK)
struct LOC_numberdigit *LINK;
{
  long tenplace, z, d;
  tenplace = LINK->place * 10;
  z = LINK->myabsolute - LINK->myabsolute / tenplace * tenplace;
  if (LINK->place == 1)
    d = z;
  else
    d = z / LINK->place;
  switch (d) {
    case 0:
      LINK->acharacter = '0';
      break;
    case 1:
      LINK->acharacter = '1';
      break;
    case 2:
      LINK->acharacter = '2';
      break;
    case 3:
      LINK->acharacter = '3';
      break;
    case 4:
      LINK->acharacter = '4';
      break;
    case 5:
      LINK->acharacter = '5';
      break;
    case 6:
      LINK->acharacter = '6';
      break;
    case 7:
      LINK->acharacter = '7';
      break;
    case 8:
      LINK->acharacter = '8';
      break;
    case 9:
      LINK->acharacter = '9';
      break;
  }
}

Local Void sign(LINK)
struct LOC_numberdigit *LINK;
{
  if (LINK->number < 0)
    LINK->acharacter = '-';
  else
    LINK->acharacter = '+';
}

Local Char numberdigit(number_, logplace, LINK)
long number_, logplace;
struct LOC_readinstruction *LINK;
{
  struct LOC_numberdigit V;
  long count;
  V.LINK = LINK;
  V.number = number_;
  V.place = 1;
  for (count = 1; count <= logplace; count++)
    V.place *= 10;

  if (V.number == 0) {
    if (V.place == 1)
      V.acharacter = '0';
    else
      V.acharacter = ' ';
    return V.acharacter;
  }
  V.myabsolute = labs(V.number);
  if (V.myabsolute < V.place / 10) {
    V.acharacter = ' ';
    return V.acharacter;
  }
  if (V.myabsolute >= V.place)
    digit(&V);
  else
    sign(&V);
  return V.acharacter;
}

Local long numbersize(n, LINK)
long n;
struct LOC_readinstruction *LINK;
{
  long size;

  if (n == 0)
    return 1;
  else {
    size = (long)(log((double)labs(n)) / ln10 + epsilon) + 1;

    if (n < 0)
      size++;
    return size;
  }
}

#undef ln10
#undef epsilon

/* Local variables for namechangeset: */
struct LOC_namechangeset {
  struct LOC_readinstruction *LINK;
  piece **pie;
} ;

Local Void putin(c_, LINK)
Char c_;
struct LOC_namechangeset *LINK;
{
  name *WITH;
  WITH = &(*LINK->pie)->key.hea.keynam;

  if (WITH->length < namelength) {
    WITH->length++;
    WITH->letters[WITH->length - 1] = c_;
  }
}

Local Void putnumber(r, LINK)
double r;
struct LOC_namechangeset *LINK;
{
  Char c_;
  long n, i;

  i = (long)floor(r + 0.5);
  for (n = numbersize(i, LINK->LINK) - 1; n >= 0; n--) {
    c_ = numberdigit(i, n, LINK->LINK);

    if (c_ != '+')
      putin(c_, LINK);

  }
}


Local Void namechangeset(pie_, changes, LINK)
piece **pie_;
changeset changes;
struct LOC_readinstruction *LINK;
{
  struct LOC_namechangeset V;
  long i, n, FORLIM;
  changedata *WITH;
  long FORLIM1;

  V.LINK = LINK;
  V.pie = pie_;
  FORLIM = changes.number;
  for (n = 0; n < FORLIM; n++) {
    putin('.', &V);
    WITH = &changes.data[n];
    switch (WITH->changetype) {

    case 'c':
      putin(WITH->baseold, &V);
      putnumber((double)WITH->basecoo1, &V);
      putin(WITH->basenew, &V);
      break;

    case 'i':
      putin('i', &V);
      putnumber((double)WITH->basecoo1, &V);
      putin(',', &V);
      putnumber((double)WITH->basecoo2, &V);

      if (WITH->insertcomplement)
	putin('~', &V);
      FORLIM1 = WITH->inserts;
      for (i = 0; i < FORLIM1; i++)
	putin(WITH->insert[i], &V);
      break;

    case 'd':
      putin('d', &V);
      putnumber((double)WITH->basecoo1, &V);
      putin(',', &V);
      putnumber((double)WITH->basecoo2, &V);
      break;
    }
  }
}

Local Void putinline(pie, c_, LINK)
piece **pie;
Char c_;
struct LOC_readinstruction *LINK;
{
  line *WITH;

  WITH = (*pie)->key.hea.fulnam;
  if (WITH->length < linelength) {
    WITH->length++;
    WITH->letters[WITH->length - 1] = c_;
  }
}

Local Void putnumline(pie, r, LINK)
piece **pie;
double r;
struct LOC_readinstruction *LINK;
{
  Char c_;
  long n, i;
  i = (long)floor(r + 0.5);

  for (n = numbersize(i, LINK) - 1; n >= 0; n--) {
    c_ = numberdigit(i, n, LINK);

    if (c_ != '+')
      putinline(pie, c_, LINK);
  }
}

Local Void linechangeset(pie, changes, LINK)
piece **pie;
changeset changes;
struct LOC_readinstruction *LINK;
{
  long i, n, FORLIM;
  changedata *WITH;
  long FORLIM1;

  FORLIM = changes.number;
  for (n = 1; n <= FORLIM; n++) {
    if (n > 1) {
      putinline(pie, ',', LINK);
      putinline(pie, ' ', LINK);
    }
    WITH = &changes.data[n-1];
    switch (WITH->changetype) {
    case 'c':
      putinline(pie, 'a', LINK);
      putinline(pie, 't', LINK);
      putinline(pie, ' ', LINK);
      putnumline(pie, (double)WITH->basecoo1, LINK);
      putinline(pie, ' ', LINK);
      putinline(pie, WITH->baseold, LINK);
      putinline(pie, '-', LINK);
      putinline(pie, '>', LINK);
      putinline(pie, WITH->basenew, LINK);
      break;
    case 'i':
      putinline(pie, 'i', LINK);
      putinline(pie, 'n', LINK);
      putinline(pie, 's', LINK);
      putinline(pie, 'e', LINK);
      putinline(pie, 'r', LINK);
      putinline(pie, 't', LINK);
      putinline(pie, ' ', LINK);
      if (WITH->insertcomplement)
	putinline(pie, '~', LINK);
      if (WITH->inserts > 0) {
	FORLIM1 = WITH->inserts;
	for (i = 0; i < FORLIM1; i++)
	  putinline(pie, WITH->insert[i], LINK);
      } else {
	putinline(pie, 'N', LINK);
	putinline(pie, 'O', LINK);
	putinline(pie, 'T', LINK);
	putinline(pie, 'H', LINK);
	putinline(pie, 'I', LINK);
	putinline(pie, 'N', LINK);
	putinline(pie, 'G', LINK);
      }
      putinline(pie, ' ', LINK);
      putinline(pie, 'b', LINK);
      putinline(pie, 'e', LINK);
      putinline(pie, 't', LINK);
      putinline(pie, 'w', LINK);
      putinline(pie, 'e', LINK);
      putinline(pie, 'e', LINK);
      putinline(pie, 'n', LINK);
      putinline(pie, ' ', LINK);
      putnumline(pie, (double)WITH->basecoo1, LINK);
      putinline(pie, ' ', LINK);
      putinline(pie, 'a', LINK);
      putinline(pie, 'n', LINK);
      putinline(pie, 'd', LINK);
      putinline(pie, ' ', LINK);
      putnumline(pie, (double)WITH->basecoo2, LINK);
      break;

    case 'd':
      putinline(pie, 'd', LINK);
      putinline(pie, 'e', LINK);
      putinline(pie, 'l', LINK);
      putinline(pie, 'e', LINK);
      putinline(pie, 't', LINK);
      putinline(pie, 'e', LINK);
      putinline(pie, ' ', LINK);
      putnumline(pie, (double)WITH->basecoo1, LINK);
      putinline(pie, ' ', LINK);
      putinline(pie, 't', LINK);
      putinline(pie, 'o', LINK);
      putinline(pie, ' ', LINK);
      putnumline(pie, (double)WITH->basecoo2, LINK);
      break;
    }
  }
}

Local boolean realbetween(a_, b, c_, LINK)
double a_, b, c_;
struct LOC_readinstruction *LINK;
{
  return (a_ <= b && b <= c_ || c_ <= b && b <= a_);
}

/* Local variables for checkoneoverlap: */
struct LOC_checkoneoverlap {
  struct LOC_readinstruction *LINK;
  changeset c_;
} ;

Local Void setrange(n, first, last, LINK)
long n;
double *first, *last;
struct LOC_checkoneoverlap *LINK;
{
  changedata *WITH;
  WITH = &LINK->c_.data[n-1];
  *first = WITH->internal1;
  *last = WITH->internal2;

  if (WITH->changetype == 'i') {
    if (*first + 1 == *last) {
      *first += 0.5;
      *last -= 0.5;
    } else {
      (*first)++;
      (*last)--;
    }
  }
  if (debug_) {
    printf("change #%ld first = %1.1f", n, *first);
    printf(" last  = %1.1f\n", *last);
  }
}

Local Void checkoneoverlap(c__, x, LINK)
changeset c__;
long x;
struct LOC_readinstruction *LINK;
{
  struct LOC_checkoneoverlap V;
  long y;
  double x1, x2, y1, y2;
  _TEXT TEMP;

  V.LINK = LINK;
  V.c_ = c__;
  if (debug_) {
    printf("=========== checkoverlap: =========");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writechangeset(&TEMP, V.c_);
    printf("\ncheckoverlap: x = %ld\n", x);
  }
  setrange(x, &x1, &x2, &V);
  for (y = 1; y < x; y++) {
    if (debug_)
      printf("checkoverlap:       y=%ld\n", y);
    setrange(y, &y1, &y2, &V);
    if (debug_)
      printf("% .1E % .1E % .1E % .1E % .1E % .1E\n", x1, y1, x2, x1, y2, x2);
    if ((realbetween(x1, y1, x2, LINK) | realbetween(x1, y2, x2, LINK)) &&
	y1 != 0 && y2 != 0) {

      mutcd1 = V.c_.data[x-1];
      mutcd2 = V.c_.data[y-1];
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writechangeset(&TEMP, V.c_);
      putchar(' ');
      TEMP.f = stdout;
      *TEMP.name = '\0';
      wchangedata(&TEMP, V.c_.data[x-1]);
      printf(" overlaps ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      wchangedata(&TEMP, V.c_.data[y-1]);
      putchar('\n');
      error(222L, LINK->LINK);
    }

  }
}

#undef debug_

Local Void changesequence(changes, thesequence, coordinateside, deleted, LINK)
changeset changes;
piece **thesequence;
direction coordinateside;
boolean *deleted;
struct LOC_readinstruction *LINK;
{
  long b1, b2;
  long deletes, i, n, shift, pielength, FORLIM;
  changedata *WITH;
  _TEXT TEMP;
  long FORLIM1;
  namechangeset(thesequence, changes, LINK);
  pielength = piecelength(*thesequence);
  *deleted = false;
  FORLIM = changes.number;

  for (n = 1; n <= FORLIM; n++) {
    WITH = &changes.data[n-1];

    checkoneoverlap(changes, n, LINK);
    b1 = WITH->internal1;
    b2 = WITH->internal2;

    switch (WITH->changetype) {
    case 'c':
      if (WITH->baseold == WITH->basenew) {
	printf("You wrote ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writechangeset(&TEMP, changes);
	printf(" \n");
	printf("but the initial and final bases are the same,\n");
	printf("so you did not request any change!\n");
	error(214L, LINK->LINK);
	geteoinst(LINK);
      } else {
	if ((*thesequence)->key.coodir != (*thesequence)->key.piedir) {
	  WITH->baseold = basetochar(complement(chartobase(WITH->baseold)));
	  WITH->basenew = basetochar(complement(chartobase(WITH->basenew)));
	  
	}
	if (b1 < 1) {
	  mutposition1 = WITH->basecoo1;
	  printf(
	    "A requested mutation coordinate is off the piece in the 5' direction at %ld\n",
	    mutposition1);
	  error(215L, LINK->LINK);
	  geteoinst(LINK);
	} else if (b1 > pielength) {
	  mutposition1 = WITH->basecoo1;
	  printf(
	    "A requested mutation coordinate is off the piece in the 3' direction at %ld\n",
	    mutposition1);
	  error(216L, LINK->LINK);
	  geteoinst(LINK);
	} else if (WITH->baseold != getbasech(b1, *thesequence, LINK)) {

	  mutposition1 = WITH->basecoo1;
	  mutnotchar = WITH->baseold;
	  mutischar = getbasech(b1, *thesequence, LINK);
	  if ((*thesequence)->key.coodir != (*thesequence)->key.piedir) {
	   
	    WITH->baseold = chomplement(WITH->baseold);
	    WITH->basenew = chomplement(WITH->basenew);
	    mutischar = chomplement(mutischar);
	    mutnotchar = chomplement(mutnotchar);
	  
	  }
	  printf(
	    "On the positively oriented strand, the old base at %ld is NOT %c!  It is %c.\n",
	    WITH->basecoo1, WITH->baseold, mutischar);

	  error(211L, LINK->LINK);
	  LINK->LINK->showallerrors = true;
	  geteoinst(LINK);
	}
      }
      break;

    case 'i':
      /* blank case */
      break;

    case 'd':
      /* blank case */
      break;
    }
  }
  FORLIM = changes.number;
 
  for (n = 1; n <= FORLIM; n++) {
    WITH = &changes.data[n-1];
    b1 = WITH->internal1;
    b2 = WITH->internal2;

    switch (WITH->changetype) {
    case 'c':
      if (b1 < 1 || b2 > pielength)
      error(219L, LINK->LINK);
          else
      putbasech(WITH->basenew, b1, *thesequence, coordinateside, LINK);
          break;
    case 'i':
      if (b1 < 0 && b2 > pielength && WITH->inserts == 0) {
	error(220L, LINK->LINK);

	geteoinst(LINK);
	*deleted = true;
      } else {
	if (b1 > pielength)
	  b1 = pielength;
	if (b2 > pielength)
	  b2 = pielength + 1;
	if (b1 < 0)
	  b1 = 0;
	if (b2 < 1)
	  b2 = 1;

	if (b1 == b2) {
	  printf(
	    " The first base, %ld, must not equal  the second base, %ld for insertion\n",
	    WITH->basecoo1, WITH->basecoo2);

	  mutposition1 = WITH->basecoo1;
	  mutposition2 = WITH->basecoo2;
	  error(218L, LINK->LINK);
	  geteoinst(LINK);
	}

	deletes = b2 - b1 - 1;
	shift = WITH->inserts - deletes;
	pielength += shift;

	if (shift > 0) {

	  switch ((*thesequence)->key.piedir) {
	  case plus:
	    if (b2 - shift < 1)
	      b2 += shift;
	    break;
	  case minus:
	    if (b2 - shift < 1)
	      b2 += shift;
	    break;
	  }

	  for (i = pielength; i >= b2; i--)
	    putbasech(getbasech(i - shift, *thesequence, LINK), i,
		      *thesequence, coordinateside, LINK);

	}	else if (shift < 0) {
	 
	  for (i = b2 + shift; i <= pielength; i++) {
	    putbasech(getbasech(i - shift, *thesequence, LINK), i,
		      *thesequence, coordinateside, LINK);

	  }

	  fixpiececoordinate(thesequence, shift, coordinateside, LINK);
	  if (shift < 0)
	    compress(thesequence, LINK);
	} else {
	  if (WITH->inserts == 0)
	    error(219L, LINK->LINK);

	}

	if (((*thesequence)->key.coodir == (*thesequence)->key.piedir) !=
	    WITH->insertcomplement) {
	  FORLIM1 = WITH->inserts;
	  for (i = 1; i <= FORLIM1; i++)
	    putbasech(WITH->insert[i-1], b1 + i, *thesequence, coordinateside,
		      LINK);

	}

	else {
	  FORLIM1 = WITH->inserts;
	  for (i = 1; i <= FORLIM1; i++) {
	    putbasech(chomplement(WITH->insert[i-1]),
		      b1 + WITH->inserts - i + 1, *thesequence,
		      coordinateside, LINK);

	  }

	}
      }
      break;
    case 'd':
      if (b1 <= 1 && b2 >= pielength) {
      	error(220L, LINK->LINK);
	      geteoinst(LINK);
	      *deleted = true;
      } else if (b1 > pielength && b2 > pielength || b1 < 1 && b2 < 1) {
          	error(221L, LINK->LINK);
      }

      else {
	if (b1 < 1)
	  b1 = 1;
	if (b2 < 1)
	  b2 = 1;
	if (b1 > pielength)
	  b1 = pielength;
	if (b2 > pielength)
	  b2 = pielength;

	shift = b2 - b1 + 1;

	pielength -= shift;
	for (i = b1; i <= pielength; i++)
	  putbasech(getbasech(i + shift, *thesequence, LINK), i, *thesequence,
		    coordinateside, LINK);

	fixpiececoordinate(thesequence, -shift, coordinateside, LINK);

	if (shift > 0)
	  compress(thesequence, LINK);
      }
      break;
    }
    propagateonechange(&changes, n, *thesequence, LINK);

  }
}

/* Local variables for extractpiece: */
struct LOC_extractpiece {
  struct LOC_readinstruction *LINK;
  piece *libpie, **pie;
  long cutposition;
  boolean reversing, acrossboundary;
  long startread, stopread;
} ;

Local Void setvariables(LINK)
struct LOC_extractpiece *LINK;
{
  LINK->reversing = ((*LINK->pie)->key.piedir != LINK->libpie->key.piedir);


  if ((*LINK->pie)->key.coocon == circular &&
      (*LINK->pie)->key.piecon == linear)
    LINK->acrossboundary = within(*LINK->pie, LINK->libpie->key.piebeg) &
			   within(*LINK->pie, LINK->libpie->key.pieend);
  else
    LINK->acrossboundary = false;
 
  if (LINK->acrossboundary) {
    if (LINK->reversing) {
      if (LINK->libpie->key.piebeg == (*LINK->pie)->key.pieend &&
	  LINK->libpie->key.pieend == (*LINK->pie)->key.piebeg)
	LINK->acrossboundary = false;
    } else {
      if (LINK->libpie->key.piebeg == (*LINK->pie)->key.piebeg &&
	  LINK->libpie->key.pieend == (*LINK->pie)->key.pieend)
	LINK->acrossboundary = false;
    }
  }


  getdna(&(*LINK->pie)->dna);
 
}

Local Void case1(LINK)
struct LOC_extractpiece *LINK;
{
  if (debugging)
    fprintf(debug.f, "case1\n");
 
  LINK->startread = pietoint((*LINK->pie)->key.piebeg, LINK->libpie);
  LINK->stopread = pietoint((*LINK->pie)->key.pieend, LINK->libpie);
  circledna(LINK->libpie, LINK->pie, LINK->startread, LINK->stopread,
	    LINK->reversing, LINK->LINK);
}

Local Void case2(LINK)
struct LOC_extractpiece *LINK;
{
  if (debugging)
    fprintf(debug.f, "case2\n");
 
  if (LINK->reversing) {
    LINK->startread = pietoint(LINK->cutposition, LINK->libpie);
    if (LINK->startread < piecelength(LINK->libpie))
      LINK->stopread = LINK->startread + 1;
    else
      LINK->stopread = 1;
    circledna(LINK->libpie, LINK->pie, LINK->startread, LINK->stopread,
	      LINK->reversing, LINK->LINK);
    return;
  }
  LINK->startread = pietoint(LINK->cutposition, LINK->libpie);
  if (LINK->startread > 1)
    LINK->stopread = LINK->startread - 1;
  else
    LINK->stopread = piecelength(LINK->libpie);
  circledna(LINK->libpie, LINK->pie, LINK->startread, LINK->stopread,
	    LINK->reversing, LINK->LINK);
}

Local Void case3(LINK)
struct LOC_extractpiece *LINK;
{
  if (debugging)
    fprintf(debug.f, "case3\n");
  LINK->startread = pietoint((*LINK->pie)->key.piebeg, LINK->libpie);
  LINK->stopread = pietoint((*LINK->pie)->key.pieend, LINK->libpie);
  circledna(LINK->libpie, LINK->pie, LINK->startread, LINK->stopread,
	    LINK->reversing, LINK->LINK);
}

Local Void extractpiece(libpie_, pie_, cutposition_, LINK)
piece *libpie_, **pie_;
long cutposition_;
struct LOC_readinstruction *LINK;
{
  struct LOC_extractpiece V;
  V.LINK = LINK;
  V.libpie = libpie_;
  V.pie = pie_;
  V.cutposition = cutposition_;
  setvariables(&V);

  if (V.acrossboundary) {
    case3(&V);
    return;
  }
  if ((*V.pie)->key.piecon == linear)
    case1(&V);
  else
    case2(&V);
}

Local Void gozero(Apiece, LINK)
piece **Apiece;
struct LOC_readinstruction *LINK;
{
  if (def.coo != coorzero)
    return;
  (*Apiece)->key.coocon = linear;
  (*Apiece)->key.coodir = plus;
  switch ((*Apiece)->key.piedir) {
    case plus:
      zeroBS = zerobase - zeroshift;
      (*Apiece)->key.coobeg = (*Apiece)->key.piebeg - zeroBS;
      (*Apiece)->key.cooend = (*Apiece)->key.pieend - zeroBS;
      (*Apiece)->key.piecon = linear;
      (*Apiece)->key.piedir = plus;
      (*Apiece)->key.piebeg -= zeroBS;
      (*Apiece)->key.pieend -= zeroBS;
      break;
    case minus:
      zeroBS = zerobase + zeroshift;
      (*Apiece)->key.coobeg = zeroBS - (*Apiece)->key.piebeg;
      (*Apiece)->key.cooend = zeroBS - (*Apiece)->key.pieend;
      (*Apiece)->key.piecon = linear;
      (*Apiece)->key.piedir = plus;
      (*Apiece)->key.piebeg = zeroBS - (*Apiece)->key.piebeg;
      (*Apiece)->key.pieend = zeroBS - (*Apiece)->key.pieend;
      break;
  }
}

Local Void addnumber(hea, n, LINK)
header *hea;
long n;
struct LOC_readinstruction *LINK;
{
  line *numbernote;
  long signspace;
  long numdigits, digit, index;
  Char charnum;
  line *noteindex;
  getlineDelila(&numbernote);

  for (index = 0; index < numberlength; index++)
    numbernote->letters[index] = LINK->LINK->numberword[index];
  if (n < 0) {
    numbernote->letters[numberlength] = '-';
    signspace = 1;
    n = -n;
  } else
    signspace = 0;
  if (n == 0) {
    numdigits = 1;
 
  } else
    numdigits = (long)(log((double)n) / log(10.0) + 1 + precision);

  numbernote->length = numberlength + signspace + numdigits;
  for (index = numbernote->length - 1; index >= numberlength + signspace; index--) {
    digit = n % 10;
    n /= 10;
    switch (digit) {
        case 0:
          charnum = '0';
          break;
        case 1:
          charnum = '1';
          break;
        case 2:
          charnum = '2';
          break;
        case 3:
          charnum = '3';
          break;
        case 4:
          charnum = '4';
          break;
        case 5:
          charnum = '5';
          break;
        case 6:
          charnum = '6';
          break;
        case 7:
          charnum = '7';
          break;
        case 8:
          charnum = '8';
          break;
        case 9:
          charnum = '9';
          break;
    }
    numbernote->letters[index] = charnum;
  }

  numbernote->next = hea->note;
  hea->note = numbernote;
  if (LINK->LINK->usernotes == NULL)
    return;
  if (hea->note != NULL) {
    noteindex = hea->note;

    while (noteindex->next != NULL)
      noteindex = noteindex->next;

    noteindex->next = LINK->LINK->usernotes;
  }  else
    hea->note = LINK->LINK->usernotes;
  LINK->LINK->usernotes = NULL;
}

#undef precision

Local Void spec(hea, numberable, object, LINK)
header *hea;
state numberable;
double *object;
struct LOC_readinstruction *LINK;
{
  if (def.key.note == off) {
    while (hea->note != NULL)
      clearline(&hea->note);
  }

  if (numberable == on) {
    def.num.item++;
    *object = def.num.item;
    ivaluenumber((long)(*object), LINK->LINK);
  } else
    *object = -0.5;

  if (def.num.sta == on && numberable == on)
    addnumber(hea, def.num.item, LINK);
}

Local Void specified(object, LINK)
double object;
struct LOC_readinstruction *LINK;
{
  LINK->LINK->correct = (object != 0.5);
}

Local boolean numbered_(object, LINK)
double object;
struct LOC_readinstruction *LINK;
{
  return (fabs(object) != 0.5);
}

Local Void specpiece(ref, LINK)
reference ref;
struct LOC_readinstruction *LINK;
{
  specified(LINK->LINK->pkspec, LINK);
  if (!LINK->LINK->correct ||
      strncmp(ref.pienam.letters, LINK->LINK->pk->key.hea.keynam.letters,
	      sizeof(alpha)))
    lrpiece(ref.pienam, &libpie);

  LINK->LINK->pkspec = -0.5;
}

Local Void unspec(object, LINK)
double *object;
struct LOC_readinstruction *LINK;
{
  *object = 0.5;
}

Local Void unspecbelowck(LINK)
struct LOC_readinstruction *LINK;
{
  unspec(&LINK->LINK->mkspec, LINK);
  unspec(&LINK->LINK->tkspec, LINK);
  unspec(&LINK->LINK->gkspec, LINK);
  unspec(&LINK->LINK->pkspec, LINK);
}

Local Void checksize(pk, LINK)
piece *pk;
struct LOC_readinstruction *LINK;
{
  booksize += piecelength(pk);
  if (booksize <= maxbook)
    return;
  booksize = 0;
  LINK->LINK->getcount = 0;
  error(223L, LINK->LINK);
  geteoinst(LINK);
}

Local long dirvalue(d)
direction d;
{
  long Result;

  switch (d) {
      case plus:
        Result = 1;
        break;
      case minus:
        Result = -1;
        break;
      case dirhomologous:
        Result = 0;
        break;
      case dircomplement:
        Result = 0;
        break;
  }
  return Result;
}

Local long sign_(thetimes)
long thetimes;
{
  if (thetimes >= 0)
    return 1;
  else
    return -1;
}

Local boolean BooleanXOR(a_, b)
boolean a_, b;
{
  return (a_ && !b || b && !a_);
}

Local Void dopiece(LINK)
struct LOC_readinstruction *LINK;
{
  
  long vdirwanted, vdirimplied, libstart;
  boolean deleted;
  piekey *WITH;

  WITH = &LINK->LINK->pk->key;
  
  if (((1L << ((long)LINK->LINK->dirwanted)) &
       ((1L << ((long)plus)) | (1L << ((long)minus)))) != 0)
    vdirwanted = dirvalue(LINK->LINK->dirwanted);
  else if (LINK->LINK->dirwanted == dirhomologous)
    vdirwanted = dirvalue(libpie->key.piedir);
  else
    vdirwanted = -dirvalue(libpie->key.piedir);

  if (LINK->LINK->toposition == LINK->LINK->fromposition)
    vdirimplied = vdirwanted;
  else {
    switch (WITH->coocon) {
    case linear:
      vdirimplied = sign_(LINK->LINK->toposition - LINK->LINK->fromposition);
      break;
    case circular:
      switch (libpie->key.piecon) {
      case circular:
	vdirimplied = vdirwanted;
	break;
      case linear:
	if (within(libpie, WITH->coobeg) & within(libpie, WITH->cooend)) {
	  switch (libpie->key.piedir) {
	  case plus:
	    libstart = libpie->key.cooend;
	    break;
	  case minus:
	    libstart = libpie->key.coobeg;
	    break;
	  }
	
	  if (BooleanXOR(between(libpie->key.piebeg, LINK->LINK->toposition,
				 libstart),
			 between(libpie->key.piebeg, LINK->LINK->fromposition,
				 libstart))) {
	    vdirimplied = -sign_(LINK->LINK->toposition - LINK->LINK->fromposition);

	  }

	  else
	    vdirimplied = sign_(LINK->LINK->toposition - LINK->LINK->fromposition);
	}

	else
	  vdirimplied = sign_(LINK->LINK->toposition - LINK->LINK->fromposition);
	break;
      }
      break;
    }

    switch (LINK->LINK->dirwanted) {
    case plus:
    case minus:
      if (vdirwanted != vdirimplied)
	        error(204L, LINK->LINK);
      break;
    case dirhomologous:
      vdirwanted = vdirimplied;
      break;

    case dircomplement:
      if (libpie->key.piecon == circular)
	vdirwanted = -vdirimplied;
      else
	error(204L, LINK->LINK);
      break;
    }
  }

  if (LINK->LINK->correct) {
    WITH->piebeg = LINK->LINK->fromposition;
    WITH->pieend = LINK->LINK->toposition;
    switch (vdirwanted) {
    case 1:
      WITH->piedir = plus;
      break;
    case -1:
      WITH->piedir = minus;
      break;
    }
    if (debugging) {
      fprintf(debug.f, "piebeg:%5ld pieend:%5ld cut:%5ld\n",
	      WITH->piebeg, WITH->pieend, LINK->LINK->cutposition);
      fprintf(debug.f, "ord(piecon)=%12d\n", (int)WITH->piecon);
      fprintf(debug.f, "ord(piedir)=%12d\n", (int)WITH->piedir);
    }

    copyline(libpie->key.hea.fulnam, &LINK->LINK->pk->key.hea.fulnam);
    extractpiece(libpie, &LINK->LINK->pk, LINK->LINK->cutposition, LINK);
    gozero(&LINK->LINK->pk, LINK);
    deleted = false;
    if (mutations.number > 0) {
      setinternal(&mutations, LINK->LINK->pk, LINK);
      if (def.doubling == on) {
      	if (longnameexists) {
            copyline(longname, &LINK->LINK->pk->key.hea.fulnam);
	}

	checksize(LINK->LINK->pk, LINK);
	bwpie(&book, LINK->LINK->pk);
	LINK->LINK->getcount++;

	spec(&LINK->LINK->pk->key.hea, def.num.str[(long)pienum],
	     &LINK->LINK->pkspec, LINK);
      }
      if (def.doubling == on) {
	writewildtypemarks(&marksdelila, mutations, insertupperbits,
			   insertlowerbits, deleteupperbits, deletelowerbits,
			   changeupperbits, changelowerbits, LINK->LINK->pk,
			   def.num.item, LINK);
	skippiece(&marksdelila, LINK);
      }

      changesequence(mutations, &LINK->LINK->pk, LINK->LINK->coordinateside,
		     &deleted, LINK);
       if (!deleted)
	writemutantmarks(&marksdelila, mutations, insertupperbits,
			 insertlowerbits, deleteupperbits, deletelowerbits,
			 changeupperbits, changelowerbits, LINK->LINK->pk,
			 def.num.item, LINK);

      clearline(&LINK->LINK->pk->key.hea.fulnam);
      getlineDelila(&LINK->LINK->pk->key.hea.fulnam);
      linechangeset(&LINK->LINK->pk, mutations, LINK);
    }

    if (!deleted) {
         if (longnameexists) {
	copyline(longname, &LINK->LINK->pk->key.hea.fulnam);
	longnameexists = false;
      }
      if (withused)
	skippiece(&marksdelila, LINK);
      checksize(LINK->LINK->pk, LINK);
      bwpie(&book, LINK->LINK->pk);
      LINK->LINK->getcount++;
    }
    else {
      if (withused)
	skippiece(&marksdelila, LINK);
    }
  }

  if (debugging)
    fprintf(debug.f, "dopiece-out\n");
}

Local Void irdirection(LINK)
struct LOC_readinstruction *LINK;
{
  if (LINK->LINK->eoinst) {
    error(15L, LINK->LINK);
    return;
  }
  findword(LINK);
  if (!LINK->LINK->correct)
    return;
  if (LINK->LINK->chr == '-' || LINK->LINK->chr == '+') {
     parse(false, LINK);
     if (!LINK->LINK->correct)
      return;
    if (LINK->LINK->parsedword[1] != ' ') {
      error(2L, LINK->LINK);
      return;
    }
    switch (LINK->LINK->parsedword[0]) {
    case '+':
      LINK->LINK->dirwanted = plus;
      break;
    case '-':
      LINK->LINK->dirwanted = minus;
      break;
    case ' ':
      printf("irdirection: Delila parse fault\n");
      printf(" chr: \"%c\"\n", LINK->LINK->chr);
      printf(" parsedword[1]: \"%c\"\n", LINK->LINK->parsedword[0]);
      printf(" parsedword[2]: \"%c\"\n", LINK->LINK->parsedword[1]);
      halt();
      break;
    }
    return;
  }

  irword(LINK);
  if (!LINK->LINK->correct)
    return;
  if ((unsigned long)LINK->LINK->word < 32 &&
      ((1L << ((long)LINK->LINK->word)) &
       ((1L << ((long)comdelila)) | (1L << ((long)homdelila)))) != 0) {
    switch (LINK->LINK->word) {

    case comdelila:
      LINK->LINK->dirwanted = dircomplement;
      break;

    case homdelila:
      LINK->LINK->dirwanted = dirhomologous;
      break;
    }
    return;
  }
  if (LINK->LINK->word != (int)piedelila &&
      LINK->LINK->word != (int)gendelila &&
      LINK->LINK->word != (int)tradelila &&
      LINK->LINK->word != (int)mardelila) {
    error(7L, LINK->LINK);
    geteoinst(LINK);
    return;
  }
  if (LINK->LINK->pass != 2)
    return;
  switch (LINK->LINK->word) {
    case mardelila:
      specified(LINK->LINK->mkspec, LINK);
      if (LINK->LINK->correct)
        LINK->LINK->dirwanted = LINK->LINK->mkoff->key.ref.refdir;
      else
        error(202L, LINK->LINK);
      break;
    case tradelila:
      specified(LINK->LINK->tkspec, LINK);
      if (LINK->LINK->correct)
        LINK->LINK->dirwanted = LINK->LINK->tk.ref.refdir;
      break;
    case gendelila:
      specified(LINK->LINK->gkspec, LINK);
      if (LINK->LINK->correct)
        LINK->LINK->dirwanted = LINK->LINK->gk.ref.refdir;
      break;
    case piedelila:
      specified(LINK->LINK->pkspec, LINK);
      if (LINK->LINK->correct)
        LINK->LINK->dirwanted = LINK->LINK->pk->key.piedir;
      break;
  }
}

Local Void irwith(LINK)
struct LOC_readinstruction *LINK;
{
  if (!withused) {
    if (*marksdelila.name != '\0') {
      if (marksdelila.f != NULL)
	marksdelila.f = freopen(marksdelila.name, "w", marksdelila.f);
      else
	marksdelila.f = fopen(marksdelila.name, "w");
    } else {
      if (marksdelila.f != NULL)
	rewind(marksdelila.f);
      else
	marksdelila.f = tmpfile();
    }
    if (marksdelila.f == NULL)
      _EscIO2(FileNotFound, marksdelila.name);
    SETUPBUF(marksdelila.f, Char);
    marksautomate(&marksdelila);
    withused = true;
  }

  readchangeset(&mutations, LINK);
  LINK->LINK->numofchanges = mutations.number;
}

Local Void irdirwit(LINK)
struct LOC_readinstruction *LINK;
{
  if (LINK->LINK->word != (int)witdelila && LINK->LINK->word != (int)dirdelila)
    return;
  if (LINK->LINK->word == witdelila) {
    irwith(LINK);
    return;
  }
  if (LINK->LINK->word != dirdelila) {
    error(7L, LINK->LINK);
    return;
  }
  irdirection(LINK);
  if (!LINK->LINK->correct)
    return;
  if (!LINK->LINK->eoinst)
    findword(LINK);
  if (LINK->LINK->eoinst)
    return;
  if (LINK->LINK->word != witdelila)
    irword(LINK);
  if (!LINK->LINK->correct)
    return;
  if (LINK->LINK->word == witdelila)
    irwith(LINK);
  else
    error(7L, LINK->LINK);
}

Local Void outofrange(pie, p, LINK)
piece *pie;
long *p;
struct LOC_readinstruction *LINK;
{
  piekey *WITH;
  WITH = &pie->key;
  switch (def.defout) {
  case rreduce:
    error(208L, LINK->LINK);
    reduceposition(pie, p);
    ivalueposition(*p, LINK->LINK);

    if (*p == LINK->LINK->previousfromposition) {
      if (LINK->LINK->reduced) {
	error(210L, LINK->LINK);
      } else
	LINK->LINK->reduced = true;
    }

    LINK->LINK->correct = true;
    break;
  case rcontinue:
    error(209L, LINK->LINK);
    LINK->LINK->correct = false;
    geteoinst(LINK);
    break;
  case rhalt:
    error(203L, LINK->LINK);
    LINK->LINK->correct = false;
    geteoinst(LINK);
    break;
  }
}

Local Void known(theposition, LINK)
long *theposition;
struct LOC_readinstruction *LINK;
{
  if (!between(libpie->key.coobeg, *theposition, libpie->key.cooend)) {
    error(206L, LINK->LINK);
    outofrange(libpie, theposition, LINK);
  }
}

Local Void okposition(p, LINK)
long *p;
struct LOC_readinstruction *LINK;
{
  if (within(libpie, *p))
    return;
  if (libpie->key.piecon == circular)
    reduceposition(libpie, p);

  else
    outofrange(libpie, p, LINK);
}

/* Local variables for posit: */
struct LOC_posit {
  struct LOC_readinstruction *LINK;
  long *theposition;
} ;

Local Void relative(LINK)
struct LOC_posit *LINK;
{
  findword(LINK->LINK);
  if (!(LINK->LINK->LINK->correct &&
	(LINK->LINK->LINK->chr == '-' || LINK->LINK->LINK->chr == '+')))
    return;
  zerobase = *LINK->theposition;

  irnumber(LINK->LINK);
  *LINK->theposition += LINK->LINK->LINK->inumber;
  if (LINK->LINK->LINK->correct)
    ivalueposition(*LINK->theposition, LINK->LINK->LINK);
}

Local Void limitref(ref, LINK)
reference ref;
struct LOC_posit *LINK;
{
  if (LINK->LINK->LINK->pass == 2) {
    if (strncmp(ref.pienam.letters, libpie->key.hea.keynam.letters,
		sizeof(alpha)))
      error(205L, LINK->LINK->LINK);
  }
  if (!LINK->LINK->LINK->correct)
    return;
  irword(LINK->LINK);
  if (!LINK->LINK->LINK->correct)
    return;
  if ((unsigned long)LINK->LINK->LINK->word >= 32 ||
      ((1L << ((long)LINK->LINK->LINK->word)) &
       ((1L << ((long)begdelila)) | (1L << ((long)enddelila)))) == 0) {
    error(7L, LINK->LINK->LINK);
    return;
  }
  if (LINK->LINK->LINK->pass != 2)
    return;
  switch (LINK->LINK->LINK->word) {
      case begdelila:
        *LINK->theposition = ref.refbeg;
        break;
      case enddelila:
        *LINK->theposition = ref.refend;
        break;
  }
  known(LINK->theposition, LINK->LINK);
}

Local Void limitmkoff(mkoff, LINK)
marker *mkoff;
struct LOC_posit *LINK;
{
  if (numbered_(LINK->LINK->LINK->mkspec, LINK->LINK))
    ivaluenumber((long)LINK->LINK->LINK->mkspec, LINK->LINK->LINK);
  limitref(mkoff->key.ref, LINK);
}

Local Void limittk(tk, LINK)
trakey tk;
struct LOC_posit *LINK;
{
  if (numbered_(LINK->LINK->LINK->tkspec, LINK->LINK))
    ivaluenumber((long)LINK->LINK->LINK->tkspec, LINK->LINK->LINK);
  limitref(tk.ref, LINK);
}

Local Void limitgk(gk, LINK)
genkey gk;
struct LOC_posit *LINK;
{
  if (numbered_(LINK->LINK->LINK->gkspec, LINK->LINK))
    ivaluenumber((long)LINK->LINK->LINK->gkspec, LINK->LINK->LINK);
  limitref(gk.ref, LINK);
}

Local Void limitpk(pk, LINK)
piece *pk;
struct LOC_posit *LINK;
{
  piekey *WITH;
  ivaluenumber((long)LINK->LINK->LINK->pkspec, LINK->LINK->LINK);
  irword(LINK->LINK);
  if (!LINK->LINK->LINK->correct)
    return;
  if ((unsigned long)LINK->LINK->LINK->word >= 32 ||
      ((1L << ((long)LINK->LINK->LINK->word)) &
       ((1L << ((long)begdelila)) | (1L << ((long)enddelila)))) == 0) {
    error(7L, LINK->LINK->LINK);
    return;
  }
  if (LINK->LINK->LINK->pass != 2)
    return;
  WITH = &pk->key;
  switch (LINK->LINK->LINK->word) {
        case begdelila:
          *LINK->theposition = WITH->piebeg;
          break;
        case enddelila:
          *LINK->theposition = WITH->pieend;
          break;
  }
}

Local Void limitco(pk, LINK)
piece *pk;
struct LOC_posit *LINK;
{
  piekey *WITH;

  ivaluenumber((long)LINK->LINK->LINK->pkspec, LINK->LINK->LINK);
  irword(LINK->LINK);
  if (!LINK->LINK->LINK->correct)
    return;
  if ((unsigned long)LINK->LINK->LINK->word >= 32 ||
      ((1L << ((long)LINK->LINK->LINK->word)) &
       ((1L << ((long)begdelila)) | (1L << ((long)enddelila)))) == 0) {
    error(7L, LINK->LINK->LINK);
    return;
  }
  if (LINK->LINK->LINK->pass != 2)
    return;
  WITH = &pk->key;
  switch (LINK->LINK->LINK->word) {
        case begdelila:
          *LINK->theposition = WITH->coobeg;
          break;
        case enddelila:
          *LINK->theposition = WITH->cooend;
          break;
  }
}

Local Void posit(theposition_, LINK)
long *theposition_;
struct LOC_readinstruction *LINK;
{
  struct LOC_posit V;
  V.LINK = LINK;
  V.theposition = theposition_;
  findword(LINK);
  if (P_inset(LINK->LINK->chr, LINK->LINK->numbers) ||
      LINK->LINK->chr == '-' || LINK->LINK->chr == '+') {
    irnumber(LINK);
    if (LINK->LINK->correct) {
      *V.theposition = LINK->LINK->inumber;
      LINK->LINK->previousfromposition = *V.theposition;
      relative(&V);
    }
  }

  else if (LINK->LINK->chr == 's') {
    irword(LINK);
    if (LINK->LINK->correct) {
      if (LINK->LINK->word == samdelila) {
	if (LINK->LINK->sameusageisvalid) {
	  *V.theposition = LINK->LINK->previousfromposition;
	  relative(&V);
	} else {
	  error(18L, LINK->LINK);
	  geteoinst(LINK);
	}
      }
    }
  }


  else {
    if (LINK->LINK->parentheses == 1) {
      irword(LINK);

     if (LINK->LINK->correct) {
	ivalueposition(*V.theposition, LINK->LINK);
	relative(&V);
      }
    } else {
      irword(LINK);
      if (LINK->LINK->correct) {
	if (LINK->LINK->word == (int)coodelila ||
	    LINK->LINK->word == (int)piedelila ||
	    LINK->LINK->word == (int)gendelila ||
	    LINK->LINK->word == (int)tradelila ||
	    LINK->LINK->word == (int)mardelila) {
	  switch (LINK->LINK->word) {

	  case mardelila:
	    if (LINK->LINK->pass == 2)
	      specified(LINK->LINK->mkspec, LINK);
	    else
	      LINK->LINK->correct = true;
	    if (LINK->LINK->correct)
	      limitmkoff(LINK->LINK->mkoff, &V);
	    else
	      error(202L, LINK->LINK);
	    break;
	  case tradelila:
	    if (LINK->LINK->pass == 2)
	      specified(LINK->LINK->tkspec, LINK);
	    else
	      LINK->LINK->correct = true;
	    if (LINK->LINK->correct)
	      limittk(LINK->LINK->tk, &V);
	    else
	      error(202L, LINK->LINK);
	    break;
	  case gendelila:
	    if (LINK->LINK->pass == 2)
	      specified(LINK->LINK->gkspec, LINK);
	    else
	      LINK->LINK->correct = true;
	    if (LINK->LINK->correct)
	      limitgk(LINK->LINK->gk, &V);
	    else
	      error(202L, LINK->LINK);
	    break;
	  case piedelila:
	    if (LINK->LINK->pass == 2)
	      specified(LINK->LINK->pkspec, LINK);
	    else
	      LINK->LINK->correct = true;
	    if (LINK->LINK->correct)
	      limitpk(LINK->LINK->pk, &V);
	    else
	      error(202L, LINK->LINK);
	    break;
	  case coodelila:
	    if (LINK->LINK->pass == 2)
	      specified(LINK->LINK->pkspec, LINK);
	    else
	      LINK->LINK->correct = true;
	    if (LINK->LINK->correct)
	      limitco(LINK->LINK->pk, &V);
	    else
	      error(202L, LINK->LINK);
	    break;
	  }
	} else
	  error(7L, LINK->LINK);
      }
      if (LINK->LINK->correct) {
	ivalueposition(*V.theposition, LINK->LINK);
	relative(&V);
      }
    }
  }
  if (LINK->LINK->pass == 2 && LINK->LINK->correct)
    okposition(V.theposition, LINK);

}

Local Void posits(LINK)
struct LOC_readinstruction *LINK;
{
  if (debugging)
    fprintf(debug.f, "positions-in\n");
  LINK->LINK->previousfromposition = 0;
  LINK->LINK->sameusageisvalid = false;
  posit(&LINK->LINK->fromposition, LINK);
  if (LINK->LINK->correct) {
    irword(LINK);
    LINK->LINK->sameusageisvalid = true;
    if (LINK->LINK->correct) {
      if (LINK->LINK->word == todelila)
	posit(&LINK->LINK->toposition, LINK);
      else
	error(7L, LINK->LINK);
    }
  }
  if (debugging)
    fprintf(debug.f, "positions-out%5ld%5ld\n",
	    LINK->LINK->fromposition, LINK->LINK->toposition);
}

Local Void absdirection(wanted, reference_, LINK)
direction *wanted, *reference_;
struct LOC_readinstruction *LINK;
{
  switch (*wanted) {
  case plus:
    /* blank case */
    break;
  case minus:
    /* blank case */
    break;
  case dircomplement:
    switch (*reference_) {
    case plus:
      *wanted = minus;
      break;
    case minus:
      *wanted = plus;
      break;
    }
    break;
  case dirhomologous:
    *wanted = *reference_;
    break;
  }
}

Local Void allref(ref, LINK)
reference ref;
struct LOC_readinstruction *LINK;
{
  absdirection(&LINK->LINK->dirwanted, &ref.refdir, LINK);

  if (LINK->LINK->dirwanted == ref.refdir) {
    LINK->LINK->fromposition = ref.refbeg;
    LINK->LINK->toposition = ref.refend;
  } else {
    LINK->LINK->fromposition = ref.refend;
    LINK->LINK->toposition = ref.refbeg;
  }

  ivalueposition(LINK->LINK->fromposition, LINK->LINK);
  known(&LINK->LINK->fromposition, LINK);
  if (!LINK->LINK->correct)
    return;
  ivalueposition(LINK->LINK->toposition, LINK->LINK);
  known(&LINK->LINK->toposition, LINK);
  if (LINK->LINK->correct) {
    LINK->LINK->pk->key.piecon = linear;
    dopiece(LINK);
  }

}

Local Void ircondition(LINK)
struct LOC_readinstruction *LINK;
{
  
}

Local Void numberstructure(LINK)
struct LOC_readinstruction *LINK;
{
   numberedstructure indnum;

  if (P_inset(LINK->LINK->word, LINK->LINK->structure)) {
    switch (LINK->LINK->word) {
          case orgdelila:
            def.num.str[(long)orgnum] = on;
            break;
          case chrdelila:
            def.num.str[(long)chrnum] = on;
            break;
          case mardelila:
            def.num.str[(long)marnum] = on;
            break;
          case tradelila:
            def.num.str[(long)tranum] = on;
            break;
          case gendelila:
            def.num.str[(long)gennum] = on;
            break;
          case piedelila:
            def.num.str[(long)pienum] = on;
            break;
          case recdelila:
            def.num.str[(long)recnum] = on;
            break;
          case enzdelila:
            def.num.str[(long)enznum] = on;
            break;
    }
    return;
  }
  if (LINK->LINK->word == alldelila) {
    for (indnum = orgnum;
	 (long)indnum <= (long)enznum;
	 indnum = (numberedstructure)((long)indnum + 1))
      def.num.str[(long)indnum] = on;
  } else {
    error(7L, LINK->LINK);
    geteoinst(LINK);
  }
}


Local Void readinstruction(LINK)
struct LOC_librarian *LINK;
{
  struct LOC_readinstruction V;
  numberedstructure indnum;
  piekey *WITH;
  V.LINK = LINK;
  LINK->eoinst = true;
  LINK->reduced = false;

  mutations.number = 0;
  findword(&V);
  if (!BUFEOF(inst.f)) {
    LINK->eoinst = false;
    irword(&V);
    if (LINK->correct) {
      if (LINK->word == titdelila) {
	if (LINK->instructioncount == 1) {
	  irtitle(&V);
	  LINK->titleexists = true;
	  if (LINK->pass == 2)
	    clearline(&LINK->title);
	} else {
	  if (LINK->pass == 1)
	    error(3L, LINK);
	  geteoinst(&V);
	}
      } else if (P_inset(LINK->word, LINK->structure)) {
	if (tvrschecks(noder(&LINK->word, &V))) {
	  irkeyname(&V);
	  if (LINK->correct) {
	    if (LINK->word == (int)enzdelila ||
		LINK->word == (int)recdelila || LINK->word == (int)mardelila)
	      error(0L, LINK);
	    if (LINK->pass == 2) {
	      switch (LINK->word) {

	      case orgdelila:
		lrorgkey(LINK->keyname, &LINK->ok);
		if (itemfound) {
		  spec(&LINK->ok.hea, def.num.str[(long)orgnum],
		       &LINK->okspec, &V);
		  tvrsbook(orgnode);
		  bworgkey(&book, LINK->ok);

		  unspec(&LINK->ckspec, &V);
		  unspecbelowck(&V);
		} else
		  error(201L, LINK);
		break;

	      case chrdelila:
		lrchrkey(LINK->keyname, &LINK->ck);
		if (itemfound) {
		  spec(&LINK->ck.hea, def.num.str[(long)chrnum],
		       &LINK->ckspec, &V);
		  tvrsbook(chrnode);
		  bwchrkey(&book, LINK->ck);
		  unspecbelowck(&V);
		} else
		  error(201L, LINK);
		break;

	      
	      case gendelila:
		lrgenkey(LINK->keyname, &LINK->gk);
		if (itemfound) {
		  spec(&LINK->gk.hea, def.num.str[(long)gennum],
		       &LINK->gkspec, &V);
		  if (def.key.gen == on)
		    bwgen(&book, LINK->gk);
		  specpiece(LINK->gk.ref, &V);
		} else
		  error(201L, LINK);
		break;

	      case piedelila:
		lrpiece(LINK->keyname, &libpie);
		if (itemfound)
		  LINK->pkspec = -0.5;
		else {
		  /*
*/
		  error(201L, LINK);
		}
		break;
		
	      }
	    }
	  }

	} else {
	  error(6L, LINK);
	  geteoinst(&V);
	}
      }

      else if (LINK->word == getdelila) {

	if (LINK->pass == 2) {
	  specified(LINK->pkspec, &V);
	  if (LINK->correct) {
	  	    copyline(libpie->key.hea.note, &LINK->pk->key.hea.note);
	    spec(&LINK->pk->key.hea, def.num.str[(long)pienum], &LINK->pkspec,
		 &V);

      clearpiece(&LINK->pk);
      copyname(libpie->key.hea.keynam, &LINK->pk->key.hea.keynam);
	    addnumber(&LINK->pk->key.hea, def.num.item, &V);
	    LINK->pk->key.mapbeg = libpie->key.mapbeg;
	    LINK->pk->key.coocon = libpie->key.coocon;
	    LINK->pk->key.coodir = libpie->key.coodir;
	    LINK->pk->key.coobeg = libpie->key.coobeg;
	    LINK->pk->key.cooend = libpie->key.cooend;
	    LINK->pk->key.piecon = libpie->key.piecon;
	    LINK->pk->key.piedir = libpie->key.piedir;
	    LINK->pk->key.piebeg = libpie->key.piebeg;
	    LINK->pk->key.pieend = libpie->key.pieend;

	  } else {
	    error(202L, LINK);
	    geteoinst(&V);
	  }
	}
	if (LINK->correct)
	  irword(&V);
	if (LINK->correct) {
	  LINK->dirwanted = dirhomologous;
	  LINK->cutposition = LINK->pk->key.coobeg;
	  while (LINK->mkon != NULL)
	    clearmarker(&LINK->mkon);

	  if (LINK->word == frodelila) {
	    posits(&V);
	    if (LINK->correct) {
	      if (!LINK->eoinst) {
		irword(&V);
		if (LINK->correct)
		  irdirwit(&V);
	      }
	      if (LINK->correct && LINK->pass == 2) {
		LINK->pk->key.piecon = linear;

		dopiece(&V);
		geteoinst(&V);
	      }
	    } else
	      geteoinst(&V);
	  } else if (LINK->word == alldelila) {
	    irword(&V);
	    if (LINK->correct) {
	      if (P_inset(LINK->word, LINK->structure)) {
		LINK->save = LINK->word;
		if (!LINK->eoinst)
		  findword(&V);
		if (!LINK->eoinst) {
		  irword(&V);
		  if (LINK->correct) {
		    if (LINK->word == (int)witdelila ||
			LINK->word == (int)dirdelila ||
			LINK->word == (int)cutdelila) {
		      if (LINK->word == cutdelila) {
			if (LINK->save == piedelila) {
			  if (LINK->pass == 2) {
			    if (libpie->key.piecon != circular)
			      error(207L, LINK);
			  }
			  if (LINK->correct) {
			    posit(&LINK->cutposition, &V);
			    if (LINK->correct && !LINK->eoinst) {
			      irword(&V);
			      if (LINK->correct)
				irdirwit(&V);
			    }
			  }
			} else {
			  error(8L, LINK);
			  geteoinst(&V);
			}
		      } else
			irdirwit(&V);
		    } else
		      error(7L, LINK);
		  }
		}

		if (LINK->correct) {
		  if (LINK->save == (int)enzdelila ||
		      LINK->save == (int)recdelila ||
		      LINK->save == (int)mardelila ||
		      LINK->save == (int)chrdelila ||
		      LINK->save == (int)orgdelila)
		    error(0L, LINK);
		}
		if (LINK->correct && LINK->pass == 2) {
		  switch (LINK->save) {

		  case orgdelila:
		    /* blank case */
		    break;
		  case chrdelila:
		    /* blank case */
		    break;
		  case mardelila:
		    specified(LINK->mkspec, &V);
		    if (LINK->correct)
		      allref(LINK->mkoff->key.ref, &V);
		    else
		      error(202L, LINK);
		    break;
		  case tradelila:
		    specified(LINK->tkspec, &V);
		    if (LINK->correct)
		      allref(LINK->tk.ref, &V);
		    else
		      error(202L, LINK);
		    break;
		  case gendelila:
		    specified(LINK->gkspec, &V);
		    if (LINK->correct)
		      allref(LINK->gk.ref, &V);
		    else
		      error(202L, LINK);
		    break;
		  case piedelila:
		    WITH = &LINK->pk->key;
		    absdirection(&LINK->dirwanted, &libpie->key.piedir, &V);
		    if (WITH->piecon == linear) {
		      if (LINK->dirwanted == libpie->key.piedir) {
			LINK->fromposition = libpie->key.piebeg;
			LINK->toposition = libpie->key.pieend;
		      } else {
			LINK->fromposition = libpie->key.pieend;
			LINK->toposition = libpie->key.piebeg;
		      }
		    } else {
		      LINK->fromposition = LINK->cutposition;


		      if (LINK->dirwanted != libpie->key.piedir) {
			if (LINK->cutposition == WITH->cooend)
			  LINK->toposition = WITH->coobeg;
			else
			  LINK->toposition = LINK->cutposition + 1;
		      } else if (LINK->cutposition == WITH->coobeg)
			LINK->toposition = WITH->cooend;
		      else
			LINK->toposition = LINK->cutposition - 1;
		    }
		    ivalueposition(LINK->fromposition, LINK);
		    ivalueposition(LINK->toposition, LINK);


		    dopiece(&V);
		    geteoinst(&V);

		    break;

		  case recdelila:
		    /* blank case */
		    break;

		  case enzdelila:
		    /* blank case */
		    break;
		  }
		}
	      }
	    }
	  } else if (LINK->word == evedelila) {
	    irword(&V);
	    if (LINK->correct) {
	      if (P_inset(LINK->word, LINK->structure)) {

		error(0L, LINK);
	      } else
		error(7L, LINK);
	    }
	  } else
	    error(7L, LINK);
	}

      } else if (LINK->word == ifdelila) {

	ircondition(&V);
	if (LINK->correct) {
	  irword(&V);
	  if (LINK->correct) {
	    if (LINK->word == thedelila)
	      error(0L, LINK);

	    else
	      error(7L, LINK);
	  }
	}
      } else if (LINK->word == defdelila || LINK->word == setdelila) {
	irword(&V);
	if (LINK->correct) {
	  if (LINK->word == keydelila) {
	    irword(&V);
	    if (LINK->correct) {
	      if (LINK->word == (int)tradelila ||
		  LINK->word == (int)gendelila ||
		  LINK->word == (int)mardelila ||
		  LINK->word == (int)notdelila) {
		LINK->save = LINK->word;
		irword(&V);
		if (LINK->correct) {
		  if ((unsigned long)LINK->word < 32 &&
		      ((1L << ((long)LINK->word)) & ((1L << ((long)ondelila)) |
			 (1L << ((long)offdelila)))) != 0) {
		    if (LINK->pass == 2) {
		      switch (LINK->save) {

		      case notdelila:
			if (LINK->word == ondelila)
			  def.key.note = on;
			else
			  def.key.note = off;
			break;

		      case mardelila:
			if (LINK->word == ondelila)
			  def.key.mar = on;
			else
			  def.key.mar = off;
			break;

		      case gendelila:
			if (LINK->word == ondelila)
			  def.key.gen = on;
			else
			  def.key.gen = off;
			break;

		      case tradelila:
			if (LINK->word == ondelila)
			  def.key.tra = on;
			else
			  def.key.tra = off;
			break;
		      }
		    }
		  } else
		    error(7L, LINK);
		}
	      } else
		error(7L, LINK);
	    }
	  } else if (LINK->word == sitdelila) {

	    irword(&V);
	    if (LINK->correct) {
	      if ((unsigned long)LINK->word < 32 &&
		  ((1L << ((long)LINK->word)) & ((1L << ((long)expdelila)) |
		     (1L << ((long)moddelila)) | (1L << ((long)cledelila)))) != 0) {
		LINK->save = LINK->word;
		irword(&V);
		if (LINK->correct) {
		  if ((unsigned long)LINK->word < 32 &&
		      ((1L << ((long)LINK->word)) & ((1L << ((long)ondelila)) |
			 (1L << ((long)offdelila)))) != 0) {
		    if (LINK->eoinst) {
		      if (LINK->pass == 2) {
			switch (LINK->save) {
          case expdelila:
            if (LINK->word == ondelila)
              def.sit.expand = on;
            else
              def.sit.expand = off;
            break;
          case moddelila:
            if (LINK->word == ondelila)
              def.sit.modify = on;
            else
              def.sit.modify = off;
            break;
          case cledelila:
            if (LINK->word == ondelila)
              def.sit.cleave = on;
            else
              def.sit.cleave = off;
            break;
			}
		      }
		    }
		  } else
		    error(7L, LINK);
		}
	      } else
		error(7L, LINK);
	    }
	  }

	  else if (LINK->word == outdelila) {
	    irword(&V);
	    if (LINK->correct) {
	      if (LINK->word == (int)haldelila ||
		  LINK->word == (int)condelila ||
		  LINK->word == (int)reddelila) {
		if (LINK->pass == 2) {
		  switch (LINK->word) {
            case reddelila:
              def.defout = rreduce;
              break;
            case condelila:
              def.defout = rcontinue;
              break;
            case haldelila:
              def.defout = rhalt;
              break;
		  }
		}
	      } else
		error(7L, LINK);
	    }
	  }


	  else if (LINK->word == coodelila) {

	    findword(&V);
	    if (P_inset(LINK->chr, LINK->numbers) || LINK->chr == '-' ||
		LINK->chr == '+') {
	      irnumber(&V);
	      if (LINK->correct) {
		def.coo = coorzero;
		zeroshift = LINK->inumber;
	      }

	    }	    else {
	      irword(&V);
	      if (LINK->correct) {
		if (LINK->word == (int)zerdelila ||
		    LINK->word == (int)nordelila) {
		   if (LINK->pass == 2) {
		    switch (LINK->word) {

		    case nordelila:
		      def.coo = coornormal;
		      break;
		    case zerdelila:
		      def.coo = coorzero;
		      break;
		    }
		  }

		}
	      }

	    }
	  }	  else if (LINK->word == doudelila) {

	    irword(&V);
	    if (LINK->correct) {
	      if ((unsigned long)LINK->word < 32 &&
		  ((1L << ((long)LINK->word)) &
		   ((1L << ((long)ondelila)) | (1L << ((long)offdelila)))) != 0) {
		if (LINK->word == ondelila)
		  def.doubling = on;
		else
		  def.doubling = off;
	      } else
		error(7L, LINK);
	    }
	  }	  else if (LINK->word == revdelila) {

	    irword(&V);
	    if (LINK->correct) {
	      if ((unsigned long)LINK->word < 32 &&
		  ((1L << ((long)LINK->word)) &
		   ((1L << ((long)ondelila)) | (1L << ((long)offdelila)))) != 0) {
		if (LINK->word == ondelila)
		  def.reversenames = on;
		else
		  def.reversenames = off;
	      } else
		error(7L, LINK);
	    }
	  }	  else if (LINK->word == arrdelila) {

	    findword(&V);
	    if (P_inset(LINK->chr, LINK->numbers) || LINK->chr == '-' ||
		LINK->chr == '+') {
	      irnumber(&V);
	      if (LINK->correct) {

		if (LINK->rnumber < 0.1)
		  LINK->rnumber = 0.1;
		def.arrowlength = LINK->rnumber;
		changeupperbits = def.arrowlength + changelowerbits;
	      }
	    }
	  }	  else if (LINK->word == numdelila) {
	    findword(&V);
	    if (!LINK->eoinst) {
	      if (P_inset(LINK->chr, LINK->numbers) || LINK->chr == '-' ||
		  LINK->chr == '+') {
		irnumber(&V);
		if (LINK->correct) {
		  if (LINK->pass == 2)
		    def.num.item = LINK->inumber - 1;
		}
	      } else {
		irword(&V);
		if (LINK->correct) {
		  if ((unsigned long)LINK->word < 32 &&
		      ((1L << ((long)LINK->word)) & ((1L << ((long)ondelila)) |
			 (1L << ((long)offdelila)))) != 0) {
		    switch (LINK->word) {
		    case ondelila:
		      def.num.sta = on;
		      break;
		    case offdelila:
		      def.num.sta = off;
		      break;
		    }
		  } else {
		    for (indnum = orgnum;
			 (long)indnum <= (long)enznum;
			 indnum = (numberedstructure)((long)indnum + 1))
		      def.num.str[(long)indnum] = off;
		    numberstructure(&V);
		    while (!LINK->eoinst) {
		      findword(&V);
		      if (LINK->eoinst)
			break;
		      irword(&V);
		      if (LINK->correct) {
			numberstructure(&V);


		      }
		    }
		  }
		}
	      }
	    } else
	      error(15L, LINK);
	  } else
	    error(7L, LINK);
	}
      }

      else if (LINK->word == notdelila) {
	irquote(&LINK->usernotes, &V);
	if (LINK->pass == 1) {
	  while (LINK->usernotes != NULL)
	    clearline(&LINK->usernotes);
	}
      }

      else if (LINK->word == namdelila) {

	irlongname(&V);
	if (LINK->pass == 1)
	  emptyline(&longname);
      }

      else
	error(7L, LINK);
    } else
      geteoinst(&V);
  }
  LINK->instructioncount++;
  if (!LINK->eoinst) {
    findword(&V);
    if (!LINK->eoinst) {
      error(1L, LINK);
      geteoinst(&V);
    }
  }
  if (LINK->parentheses != 0) {
    error(11L, LINK);
    LINK->parentheses = 0;
  }
  if (LINK->chr == ';')
    ichread(&V);

}

Static Void librarian()
{
  struct LOC_librarian V;
  _TEXT TEMP;
  initializedelila(&V);
  V.pass = 1;
  printf("Pass 1\n");
  initreadinst(&V);

  while (!BUFEOF(inst.f))
    readinstruction(&V);
  if (!V.titleexists)
    error(17L, &V);

  if (!tvrschecks(libnode)) {
    error(6L, &V);
    writeerrors(&V);
  }
  putc('\n', listing.f);
  writepasserrors(&listing, (long)V.pass, &V);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writepasserrors(&TEMP, (long)V.pass, &V);

  if (!V.pass1errors) {
    printf("Pass 2\n");

    V.pass = 2;
    if (*inst.name != '\0') {
      if (inst.f != NULL)
	inst.f = freopen(inst.name, "r", inst.f);
      else
	inst.f = fopen(inst.name, "r");
    } else
      rewind(inst.f);
    if (inst.f == NULL)
      _EscIO2(FileNotFound, inst.name);
    RESETBUF(inst.f, Char);

    bwbookheader(&V.title);
    fprintf(listing.f, "\f");
    initreadinst(&V);
    
    while (!BUFEOF(inst.f) && (!V.pass2errors || V.showallerrors))
      readinstruction(&V);
    putc('\n', listing.f);
    writelineinformation(&V);
    if (V.pass2errors) {
      writeerrors(&V);
      bookhalt(&V);
    } else {
      tvrslibrary(libnode);
      tvrsbook(libnode);
    }
    putc('\n', listing.f);
    writepasserrors(&listing, (long)V.pass, &V);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writepasserrors(&TEMP, (long)V.pass, &V);
  } else
    bookhalt(&V);

  if (V.pass2errors || V.pass1errors)
    printf("Error(s) in pass %d: see the listing file for details\n", V.pass);
  if (V.warnings)
    printf("There are warnings: see the listing\n");

  printf("%ld piece", V.getcount);
  if (V.getcount != 1)
    putchar('s');
  printf(" extracted\n");

  printf("%ld base", booksize);
  if (booksize != 1)
    putchar('s');
  printf(" extracted\n");
}

#undef pagelength
#undef widinst
#undef minword
#undef max1errors
#undef max2errors
#undef maxerrors
#undef maxpositions
#undef maxnumbers
#undef numberlength

/* Print help for user */
void usage() {
  printf("\n");
  printf(" Delila: the librarian for sequence manipulation.\n");
  printf("\n  delila -b myBook.txt -i instructions.txt -l outlisting.txt\n\n");
  printf("  -b book output name: the set of sequences pulled out of the library.\n");
  printf("  -i Instruction file: instructions written in the language delila that tell the\n");
  printf("     program delila what sequences to pull out of the library.\n");
  printf("  -l listing output file name : the instructions are listed along with errors found or\n");
  printf("     actions taken \n");
  printf("\n");
  printf(" It is assumed the following files exist in the current directory.\n\n");
  printf("  lib1: the first library from which to obtain sequences\n");
  printf("  cat1: the first catalogue, corresponding to lib1\n");
  printf("  lib2: the second library\n");
  printf("  cat2: the second catalogue, corresponding to lib2\n");
  printf("  lib3: the third library\n");
  printf("  cat3: the third catalogue, corresponding to lib3\n\n");
  printf("  version %4.2f\n", version);
  exit(EXIT_SUCCESS);
}

int main(int argc, Char **argv) 
{
  extern char *optarg;
	extern int optind;
	int c, err = 0; 
  /* flags marking arguments passed */
  int bflag=0;       /* book output file name  */
	int iflag=0;       /* instruction file flag */
  int lflag=0;       /* listing output file name */
	char *bookName = "book.txt";
  char *instructions = "instructions.txt";
  char *list = "listing.txt";

/* Process command line arguments  */
while ((c = getopt(argc, argv, "b:i:l:")) != -1)
		switch (c) {
		case 'b':
      bflag = 1;
			bookName = optarg;
			break;
		case 'i':
      iflag = 1;
			instructions = optarg;
			break;
		case 'l':
      lflag = 1;
			list = optarg;
			break;
		case '?':
			err = 1;
			break;
		}

  /* Is the book file name present */  
	if (bflag == 0) {	/* -b bookname was mandatory */ 
		fprintf(stderr, "%s: missing -b bookname\n", argv[0]);
		usage();
		exit(1);
	} 

  /* Instruction file ? */
  if (iflag == 0) { /* -i was mandatory */        
		fprintf(stderr, "%s: missing -i instruction file\n", argv[0]);
		usage();
		exit(1);
  } 

  /* listing file name  */  
  if (lflag == 0) { 
    fprintf(stderr, "%s: missing -l listing file name \n", argv[0]);
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
  debug.f = NULL;
  strcpy(debug.name, "debug");
  marksdelila.f = NULL;
  strcpy(marksdelila.name, "marksdelila");
  book.f = NULL;
  strcpy(book.name, bookName);
  listing.f = NULL;
  strcpy(listing.name, list);
  inst.f = NULL;
  strcpy(inst.name, instructions  );
  cat3.f = NULL;
  strcpy(cat3.name, "cat3");
  cat2.f = NULL;
  strcpy(cat2.name, "cat2");
  cat1.f = NULL;
  strcpy(cat1.name, "cat1");
  lib3.f = NULL;
  strcpy(lib3.name, "lib3");
  lib2.f = NULL;
  strcpy(lib2.name, "lib2");
  lib1.f = NULL;
  strcpy(lib1.name, "lib1");
  debugging = false;
  initlibrarian();
  librarian();
_L1:
  if (lib1.f != NULL)
    fclose(lib1.f);
  if (lib2.f != NULL)
    fclose(lib2.f);
  if (lib3.f != NULL)
    fclose(lib3.f);
  if (cat1.f != NULL)
    fclose(cat1.f);
  if (cat2.f != NULL)
    fclose(cat2.f);
  if (cat3.f != NULL)
    fclose(cat3.f);
  if (inst.f != NULL)
    fclose(inst.f);
  if (listing.f != NULL)
    fclose(listing.f);
  if (book.f != NULL)
    fclose(book.f);
  if (marksdelila.f != NULL)
    fclose(marksdelila.f);
  if (debug.f != NULL)
    fclose(debug.f);
  exit(EXIT_SUCCESS);

  return 0;
}



/* End. */
