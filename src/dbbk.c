/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dbbk.p" */

#include <stdio.h> /* printf */
#include <getopt.h>  /* getopt API */ 
#include <stdlib.h> 
#include </home/mplace/bin/p2c/src/p2c.h>

#define version         3.50
#define datetimearraylength  19
#define idlength        100
#define namelength      idlength
#define lclength        3
#define lcloc           "LOC"
#define lcid            "ID "
#define lc3spc          "   "
#define lcos            "OS "
#define lcdef           "  O"
#define lcbas           "BAS"
#define lcsq            "SQ "
#define lcori           "ORI"
#define lcsit           "SIT"
#define lcterm          "// "
#define lcdat           "DAT"
#define genuslimit      1

typedef Char idutype[idlength];
typedef Char idptype[idlength];
typedef idptype alpha;
typedef Char lcutype[lclength];
typedef Char lcptype[lclength];
typedef enum {
  embl, genb, none
} libsused;

typedef Char datetimearray[datetimearraylength];
Static FILE *db, *l1, *changes;
Static boolean notwarned;
Static jmp_buf _JL1;

Static Void halt()
{
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Static Void decapitilize(c)
Char *c;
{
  long n;

  n = *c;

  if (n >= 'A' && n <= 'Z')
    *c = _tolower(n);
}


Static Void writeidptype(fout, writeasterisk, carriagereturn, thename)
FILE **fout;
boolean writeasterisk, carriagereturn;
Char *thename;
{
  long index = 1;

  if (writeasterisk)
    fprintf(*fout, "* ");
  do {
    if (thename[index-1] != ' ')
      putc(thename[index-1], *fout);
    index++;
  } while (index != idlength && thename[index-1] != ' ');
  if (carriagereturn)
    putc('\n', *fout);
}


Static Void finishchanges(changes, inchanges, changestart, entryname,
			  basenumber)
FILE **changes;
boolean *inchanges;
long *changestart;
Char *entryname;
long basenumber;
{
  *inchanges = false;
  fprintf(*changes, "define \"unknown:%ld-%ld\" \"?\" \"[]\" \"[]\" 0 %ld\n",
	  *changestart, basenumber, basenumber - *changestart - 1);
  fprintf(*changes, "@ ");
  writeidptype(changes, false, false, entryname);
  fprintf(*changes, " %ld.0 +1 \"unknown:%ld-%ld\" \"\"\n",
	  *changestart, *changestart, basenumber);
}


Static Void copydna(fin, fout, changes, basenumber, entryname, inchanges,
		    changestart)
FILE **fin, **fout, **changes;
long *basenumber;
Char *entryname;
boolean *inchanges;
long *changestart;
{
  Char c;

  while (!P_eoln(*fin)) {
    c = P_peek(*fin);
    decapitilize(&c);

    if (c >= 'a' && c <= 'z' || c == ' ') {
      if (c >= 'a' && c <= 'z')
	(*basenumber)++;

      if (c == 'u')
	c = 't';

      if (c != ' ' && c != 't' && c != 'g' && c != 'c' && c != 'a') {
	if (notwarned) {
	  notwarned = false;
/* p2c: dbbk.p, line 129: Note: REWRITE does not specify a name [181] */
	  if (*changes != NULL)
	    rewind(*changes);
	  else
	    *changes = tmpfile();
	  if (*changes == NULL)
	    _EscIO(FileNotFound);
	  fprintf(*changes, "* dbbk %4.2f\n", version);
	}

	if (!*inchanges) {
	  *inchanges = true;
	  *changestart = *basenumber;
	}

	c = 'a';
      }

      else {
	if (c != ' ' && *inchanges)
	  finishchanges(changes, inchanges, changestart, entryname,
			*basenumber);
      }

      putc(c, *fout);
/* p2c: dbbk.p, line 147: Note:
 * File parameter fout can't access buffers (try StructFiles = 1) [318] */
    }
/* p2c: dbbk.p, line 150: Note:
 * File parameter fin can't access buffers (try StructFiles = 1) [318] */
    getc(*fin);
  }
  fscanf(*fin, "%*[^\n]");
  getc(*fin);
  putc('\n', *fout);
}


Static Void readlcu(lib, liblcu, liblcp)
FILE **lib;
Char *liblcu;
Char *liblcp;
{
  long index;

  if (P_eof(*lib)) {
    printf("in readlcu: hit end of file; last line type was linetype: %.*s\n",
	   lclength, liblcp);
    halt();
  }


  for (index = 0; index < lclength; index++) {
    if (P_eoln(*lib))
      liblcu[index] = ' ';
    else {
      liblcu[index] = getc(*lib);
      if (liblcu[index] == '\n')
	liblcu[index] = ' ';
    }
  }
  memcpy(liblcp, liblcu, sizeof(lcptype));
}


Static boolean lcequal(lcp1, lcp2)
Char *lcp1, *lcp2;
{
/* p2c: dbbk.p: Note: Eliminated unused assignment statement [338] */
  if (!strncmp(lcp1, lcp2, sizeof(lcptype)))
    return true;
  return false;
}


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


Static Void idclear(ida)
Char *ida;
{
  long i;

  for (i = 0; i < idlength; i++)
    ida[i] = ' ';
}


Static Void idcopy(ida, idb)
Char *ida, *idb;
{
  long i;

  for (i = 0; i < idlength; i++)
    idb[i] = ida[i];
}


Static Void getid(fin, finidp)
FILE **fin;
Char *finidp;
{
  long index = 0;
  idutype finidu;

  while (P_peek(*fin) == ' ')
    getc(*fin);
/* p2c: dbbk.p, line 218: Note:
 * File parameter fin can't access buffers (try StructFiles = 1) [318] */

  while (P_peek(*fin) != ' ' && index < idlength) {
    index++;
    if (P_eoln(*fin))
      finidu[index-1] = ' ';
    else {
      finidu[index-1] = getc(*fin);
      if (finidu[index-1] == '\n')
	finidu[index-1] = ' ';
    }
  }


  while (index < idlength) {
    index++;
    finidu[index-1] = ' ';
  }
  memcpy(finidp, finidu, sizeof(idptype));
}


Static Void getspecies(fin, finidp)
FILE **fin;
Char *finidp;
{
  long index = 0;
  idutype finidu;

  while (P_peek(*fin) == ' ')
    getc(*fin);
/* p2c: dbbk.p, line 245: Note:
 * File parameter fin can't access buffers (try StructFiles = 1) [318] */



  while (((P_peek(*fin) != ' ') & (P_peek(*fin) != '_')) && index < idlength) {
    index++;
    if (P_eoln(*fin))
      finidu[index-1] = ' ';
    else {
      finidu[index-1] = getc(*fin);
      if (finidu[index-1] == '\n')
	finidu[index-1] = ' ';
    }
    if (finidu[index-1] == '_')
      finidu[index-1] = ' ';

  }


  while ((P_peek(*fin) == ' ') | (P_peek(*fin) == '_'))
    getc(*fin);
/* p2c: dbbk.p, line 262: Note:
 * File parameter fin can't access buffers (try StructFiles = 1) [318] */


  if (index > genuslimit)
    index = genuslimit;

  index++;


  finidu[index-1] = '.';

  while (!P_eoln(*fin)) {
    index++;
    if (P_eoln(*fin)) {
      finidu[index-1] = ' ';
      continue;
    }
    finidu[index-1] = getc(*fin);
    if (finidu[index-1] == '\n')
      finidu[index-1] = ' ';
    if (finidu[index-1] == ' ')
      finidu[index-1] = '-';
  }

  while (index < idlength) {
    index++;
    finidu[index-1] = ' ';
  }

  memcpy(finidp, finidu, sizeof(idptype));
}


/* Local variables for pluckdigit: */
struct LOC_pluckdigit {
  long place, myabsolute;
  Char acharacter;
} ;

Local Void digit(LINK)
struct LOC_pluckdigit *LINK;
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


Local Char pluckdigit(number, logplace)
long number, logplace;
{
  struct LOC_pluckdigit V;
  long count;

  V.place = 1;
  for (count = 1; count <= logplace; count++)
    V.place *= 10;
  if (number == 0) {
    V.acharacter = '0';
    return V.acharacter;
  }
  V.myabsolute = number;
  if (V.myabsolute >= V.place)
    digit(&V);
  else
    V.acharacter = '0';
  return V.acharacter;
}


Static Void getdatetime(adatetime)
Char *adatetime;
{
/* p2c: dbbk.p, line 295: Warning: Expected '..', found a semicolon [227] */
  long t;
  _PROCEDURE TEMP;


  GetTimeStamp(t);
/* p2c: dbbk.p, line 346:
 * Warning: Symbol 'GETTIMESTAMP' is not defined [221] */

/* p2c: dbbk.p, line 348: Warning: Argument of WITH is not a RECORD [264] */

  if (!TimeValid) {
    printf("getdatetime: invalid time!\n");
    halt();
    return;
  }
/* p2c: dbbk.p, line 349:
 * Warning: Symbol 'TIMEVALID' is not defined [221] */
  memcpy(adatetime, "year/mm/dd hh:mm:ss", sizeof(datetimearray));
  adatetime[0] = pluckdigit(year, 3L);
/* p2c: dbbk.p, line 351: Warning: Symbol 'YEAR' is not defined [221] */
  adatetime[1] = pluckdigit(year, 2L);
/* p2c: dbbk.p, line 352: Warning: Symbol 'YEAR' is not defined [221] */
  adatetime[2] = pluckdigit(year, 1L);
/* p2c: dbbk.p, line 353: Warning: Symbol 'YEAR' is not defined [221] */
  adatetime[3] = pluckdigit(year, 0L);
/* p2c: dbbk.p, line 354: Warning: Symbol 'YEAR' is not defined [221] */
  adatetime[5] = pluckdigit(month, 1L);
/* p2c: dbbk.p, line 355: Warning: Symbol 'MONTH' is not defined [221] */
  adatetime[6] = pluckdigit(month, 0L);
/* p2c: dbbk.p, line 356: Warning: Symbol 'MONTH' is not defined [221] */
  adatetime[8] = pluckdigit(day, 1L);
/* p2c: dbbk.p, line 357: Warning: Symbol 'DAY' is not defined [221] */
  adatetime[9] = pluckdigit(day, 0L);
/* p2c: dbbk.p, line 358: Warning: Symbol 'DAY' is not defined [221] */
  adatetime[11] = pluckdigit(hour, 1L);
/* p2c: dbbk.p, line 359: Warning: Symbol 'HOUR' is not defined [221] */
  adatetime[12] = pluckdigit(hour, 0L);
/* p2c: dbbk.p, line 360: Warning: Symbol 'HOUR' is not defined [221] */
  adatetime[14] = pluckdigit(minute, 1L);
/* p2c: dbbk.p, line 361: Warning: Symbol 'MINUTE' is not defined [221] */
  adatetime[15] = pluckdigit(minute, 0L);
/* p2c: dbbk.p, line 362: Warning: Symbol 'MINUTE' is not defined [221] */
  adatetime[17] = pluckdigit(second, 1L);
/* p2c: dbbk.p, line 363: Warning: Symbol 'SECOND' is not defined [221] */
  adatetime[18] = pluckdigit(second, 0L);
/* p2c: dbbk.p, line 364: Warning: Symbol 'SECOND' is not defined [221] */
}



Static Void writedatetime(thefile, adatetime)
FILE **thefile;
Char *adatetime;
{
  long index;

  for (index = 0; index < datetimearraylength; index++)
    putc(adatetime[index], *thefile);
}



Static Void readdatetime(thefile, adatetime)
FILE **thefile;
Char *adatetime;
{
  long index;
  Char udatetime[datetimearraylength];

  for (index = 0; index < datetimearraylength; index++) {
    udatetime[index] = getc(*thefile);
    if (udatetime[index] == '\n')
      udatetime[index] = ' ';
  }
  memcpy(adatetime, udatetime, sizeof(datetimearray));
  if (adatetime[2] == '/' && adatetime[11] == ':')
    printf(" old datetime (only 2 year digits) read: %.*s\n",
	   datetimearraylength, adatetime);
/* p2c: dbbk.p, line 396: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
}


#define tab             9


Static boolean isblank(c)
Char c;
{
  return (c == ' ' || c == tab);
}

#undef tab


Static Void skipblanks(thefile)
FILE **thefile;
{
  while (isblank(P_peek(*thefile)) & (!P_eoln(*thefile)))
    getc(*thefile);
/* p2c: dbbk.p, line 411: Note: File
 * parameter thefile can't access buffers (try StructFiles = 1) [318] */
}


Static Void skipnonblanks(thefile)
FILE **thefile;
{
  while ((!isblank(P_peek(*thefile))) & (!P_eoln(*thefile)))
    getc(*thefile);
/* p2c: dbbk.p, line 417: Note: File
 * parameter thefile can't access buffers (try StructFiles = 1) [318] */
}


Static Void skipcolumn(thefile)
FILE **thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}


Static Void note(fout, piecenum)
FILE **fout;
long piecenum;
{
  fprintf(*fout, "note\n");
  fprintf(*fout, "* #%ld\n", piecenum);
  fprintf(*fout, "note\n");
}


Static Void dna(fin, fout, libtitle, entryname)
FILE **fin, **fout;
libsused libtitle;
Char *entryname;
{
  long basenumber = 0;
  long dumpint;
  lcutype finlcu;
  lcptype finlcp;
  boolean inchanges = false;
  long changestart = -LONG_MAX;

  fprintf(*fout, "dna\n");
  switch (libtitle) {

  case none:
    printf(" PROCEDURE DNA HAS BIZARRE VALUE OF\n");
    printf(" LIBTITLE = NONE\n");
    halt();
    break;

  case embl:
    do {
      readlcu(fin, finlcu, finlcp);
      if (lcequal(finlcp, lc3spc)) {
	fprintf(*fout, "* ");
	copydna(fin, fout, &changes, &basenumber, entryname, &inchanges,
		&changestart);
      }

      else {
	fscanf(*fin, "%*[^\n]");
	getc(*fin);
      }
    } while (!lcequal(finlcp, lcterm));
    break;

  case genb:
    readlcu(fin, finlcu, finlcp);
    while ((!lcequal(finlcp, lcsit)) & (!lcequal(finlcp, lcterm))) {
      fscanf(*fin, "%ld", &dumpint);
      fprintf(*fout, "* ");
      copydna(fin, fout, &changes, &basenumber, entryname, &inchanges,
	      &changestart);
      readlcu(fin, finlcu, finlcp);
    }
    break;
  }

  if (inchanges)
    finishchanges(&changes, &inchanges, &changestart, entryname, basenumber);
  fprintf(*fout, "dna\n");
}


Static Void piece(fin, fout, piecenum, libtitle, bpint, entryname, topology)
FILE **fin, **fout;
long piecenum;
libsused libtitle;
long bpint;
Char *entryname;
Char topology;
{
  long index;

  fprintf(*fout, "piece\n");
  writeidptype(fout, true, true, entryname);
  fprintf(*fout, "* \n");
  note(fout, piecenum);
  fprintf(*fout, "* 1\n");

  for (index = 1; index <= 2; index++) {
    if (topology == 'l')
      fprintf(*fout, "* linear\n");
    else
      fprintf(*fout, "* circular\n");
    fprintf(*fout, "* +\n");
    fprintf(*fout, "* 1\n");
    fprintf(*fout, "* %ld\n", bpint);
  }
  dna(fin, fout, libtitle, entryname);
  fprintf(*fout, "piece\n");
}


Static Void chromosome(fin, fout, piecenum, libtitle, bpint, entryname,
		       orgname, topology)
FILE **fin, **fout;
long piecenum;
libsused libtitle;
long bpint;
Char *entryname, *orgname;
Char topology;
{
  fprintf(*fout, "chromosome\n");
  writeidptype(fout, true, true, orgname);
  fprintf(*fout, "* \n");
  fprintf(*fout, "* 1\n");
  fprintf(*fout, "* %ld\n", bpint);
  piece(fin, fout, piecenum, libtitle, bpint, entryname, topology);
}


Static Void organism(fin, fout, piecenum, libtitle, bpint, entryname,
		     firstorganism, orgname, oldorgname, topology)
FILE **fin, **fout;
long piecenum;
libsused libtitle;
long bpint;
Char *entryname;
boolean *firstorganism;
Char *orgname, *oldorgname;
Char topology;
{
  FILE *TEMP;

  if (idequal(orgname, oldorgname))
    piece(fin, fout, piecenum, libtitle, bpint, entryname, topology);
  else {
    if (*firstorganism)
      *firstorganism = false;
    else {
      fprintf(*fout, "chromosome\n");
      fprintf(*fout, "organism\n");
    }

    printf("organism ");
    TEMP = stdout;
/* p2c: dbbk.p, line 569:
 * Note: Taking address of stdout; consider setting VarFiles = 0 [144] */
    writeidptype(&TEMP, true, true, orgname);

    fprintf(*fout, "organism\n");

    writeidptype(fout, true, true, orgname);
    fprintf(*fout, "* \n");

    fprintf(*fout, "* bases\n");
    chromosome(fin, fout, piecenum, libtitle, bpint, entryname, orgname,
	       topology);

    idcopy(orgname, oldorgname);
  }
  TEMP = stdout;

/* p2c: dbbk.p, line 583:
 * Note: Taking address of stdout; consider setting VarFiles = 0 [144] */
  writeidptype(&TEMP, false, false, entryname);
  putchar('\n');
}


Static Void getentry(fin, fout, piecenum, firstorganism, oldorgname)
FILE **fin, **fout;
long piecenum;
boolean *firstorganism;
Char *oldorgname;
{
  boolean done = false;
  idptype dumpword;
  lcutype finlcu;
  lcptype finlcp;
  libsused libtitle;
  long bpint;
  idptype entryname, locusname, orgname;

  Char topology;
  FILE *TEMP;

  do {
    readlcu(fin, finlcu, finlcp);
    if (lcequal(finlcp, lcid)) {
      libtitle = embl;
      getid(fin, entryname);

      do {
	readlcu(fin, finlcu, finlcp);
	if (lcequal(finlcp, lcos)) {
	  getid(fin, orgname);
	  done = true;
	}

	fscanf(*fin, "%*[^\n]");
	getc(*fin);
      } while (!done);
      done = false;
      do {
	readlcu(fin, finlcu, finlcp);
	if (lcequal(finlcp, lcsq) | P_eof(*fin)) {
	  getid(fin, dumpword);
	  fscanf(*fin, "%ld%*[^\n]", &bpint);
	  getc(*fin);

	  done = true;
	} else {
	  fscanf(*fin, "%*[^\n]");
	  getc(*fin);
	}
      } while (!done);
      topology = 'l';
      organism(fin, fout, piecenum, libtitle, bpint, entryname, firstorganism,
	       orgname, oldorgname, topology);
    } else if (lcequal(finlcp, lcloc)) {
      libtitle = genb;

/* p2c: dbbk.p, line 646: Note:
 * File parameter fin can't access buffers (try StructFiles = 1) [318] */
      getc(*fin);
/* p2c: dbbk.p, line 646: Note:
 * File parameter fin can't access buffers (try StructFiles = 1) [318] */
      getc(*fin);

      getid(fin, locusname);
      fscanf(*fin, "%ld", &bpint);
      skipcolumn(fin);
      skipcolumn(fin);
      skipblanks(fin);
      if (P_eoln(*fin)) {
	printf("Cannot get topology for ");
	TEMP = stdout;
/* p2c: dbbk.p, line 655:
 * Note: Taking address of stdout; consider setting VarFiles = 0 [144] */
	writeidptype(&TEMP, false, true, locusname);
	halt();
      }
      if ((P_peek(*fin) == 'l') | (P_peek(*fin) == 'c'))
	topology = P_peek(*fin);
      else {
	printf("Topology for ");
	TEMP = stdout;
/* p2c: dbbk.p, line 662:
 * Note: Taking address of stdout; consider setting VarFiles = 0 [144] */
	writeidptype(&TEMP, false, false, locusname);
	printf(" is not l(inear) or c(ircular)\n");
	while (!P_eoln(*fin)) {
	  putchar(P_peek(*fin));
/* p2c: dbbk.p, line 666: Note:
 * File parameter fin can't access buffers (try StructFiles = 1) [318] */
	  getc(*fin);
	}
	printf("\nASSUMING LINEAR\n");

	topology = 'l';
      }
      fscanf(*fin, "%*[^\n]");


      getc(*fin);
      while (P_peek(*fin) != 'V') {
	fscanf(*fin, "%*[^\n]");
	getc(*fin);
	if (!P_eof(*fin)) {

	  continue;
	}

	rewind(*fin);
	while (P_peek(*fin) != 'A') {
	  fscanf(*fin, "%*[^\n]");
	  getc(*fin);
	  if (P_eof(*fin)) {
	    printf("could not find VERSION or ACCESSSION\n");
	    halt();
	  }
	}
      }

      while (P_peek(*fin) != ' ')
	getc(*fin);
/* p2c: dbbk.p, line 692: Note:
 * File parameter fin can't access buffers (try StructFiles = 1) [318] */
      while (P_peek(*fin) == ' ')
	getc(*fin);
/* p2c: dbbk.p, line 693: Note:
 * File parameter fin can't access buffers (try StructFiles = 1) [318] */
      getid(fin, entryname);

      do {
	readlcu(fin, finlcu, finlcp);

	if (lcequal(finlcp, lcbas)) {
	  done = true;
	  printf("WARNING: in entry ");
	  TEMP = stdout;
/* p2c: dbbk.p, line 704:
 * Note: Taking address of stdout; consider setting VarFiles = 0 [144] */
	  writeidptype(&TEMP, false, false, entryname);
	  printf("there was no ORGANISM.  Using:");
	  TEMP = stdout;
/* p2c: dbbk.p, line 706:
 * Note: Taking address of stdout; consider setting VarFiles = 0 [144] */
	  writeidptype(&TEMP, false, false, entryname);
	  memcpy(orgname, entryname, sizeof(idptype));
	}

	else {
	  if (lcequal(finlcp, lcdef) | P_eof(*fin)) {
	    getid(fin, dumpword);
	    getspecies(fin, orgname);
	    done = true;
	  }
	  fscanf(*fin, "%*[^\n]");
	  getc(*fin);
	}
      } while (!done);

      do {
	readlcu(fin, finlcu, finlcp);
	fscanf(*fin, "%*[^\n]");
	getc(*fin);
      } while (!lcequal(finlcp, lcori));
      organism(fin, fout, piecenum, libtitle, bpint, entryname, firstorganism,
	       orgname, oldorgname, topology);

    } else {
      libtitle = none;

      fscanf(*fin, "%*[^\n]");
      getc(*fin);
    }
  } while (!((libtitle != none) | P_eof(*fin)));
}


Static Void datebook(fin, fout)
FILE **fin, **fout;
{
  long index;
  Char dumpchar;
  boolean findated = false;
  datetimearray oldate, adatetime;
  lcutype finlcu;
  lcptype finlcp;

  readlcu(fin, finlcu, finlcp);
  if (lcequal(finlcp, lcdat)) {
    findated = true;
    dumpchar = getc(*fin);
    if (dumpchar == '\n')
      dumpchar = ' ';
    do {
      getc(*fin);
    } while (P_peek(*fin) == ' ');
/* p2c: dbbk.p, line 760: Note:
 * File parameter fin can't access buffers (try StructFiles = 1) [318] */
    readdatetime(fin, oldate);
    fscanf(*fin, "%*[^\n]");
    getc(*fin);
  } else
    rewind(*fin);
  getdatetime(adatetime);
  fprintf(*fout, "* ");
  if (findated) {
    writedatetime(fout, adatetime);
    fprintf(*fout, ", ");
    writedatetime(fout, oldate);
    fprintf(*fout, ", ");
    return;
  }
  for (index = 1; index <= 2; index++) {
    writedatetime(fout, adatetime);
    fprintf(*fout, ", ");
  }
}


Static Void order(fin, fout)
FILE **fin, **fout;
{
  boolean firstorganism = true;
  idptype oldorgname;
  long piecenum = 0;

  rewind(*fin);
/* p2c: dbbk.p, line 791: Note: REWRITE does not specify a name [181] */
  if (*fout != NULL)
    rewind(*fout);
  else
    *fout = tmpfile();
  if (*fout == NULL)
    _EscIO(FileNotFound);
  if (P_eof(*fin)) {
    printf(" INPUT FILE FIN IS EMPTY\n");
    halt();
  }
  idclear(oldorgname);
  notwarned = true;


  datebook(fin, fout);
  fprintf(*fout, "dbbk %4.2f\n", version);

  do {
    piecenum++;
    getentry(fin, fout, piecenum, &firstorganism, oldorgname);
  } while (!P_eof(*fin));

  fprintf(*fout, "chromosome\n");
  fprintf(*fout, "organism\n");

  if (!notwarned) {
    printf("WARNING: some sequences have been altered,");
    printf("see the changes file.\n");
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  changes = NULL;
  l1 = NULL;
  db = NULL;
  printf(" dbbk %4.2f\n", version);
  order(&db, &l1);
_L1:
  if (db != NULL)
    fclose(db);
  if (l1 != NULL)
    fclose(l1);
  if (changes != NULL)
    fclose(changes);
  exit(EXIT_SUCCESS);

  return 0;
}



/* End. */
