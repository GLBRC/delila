/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dbbk.p" */







/* 
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


/*


*/



#define datetimearraylength  19
/*

*/





#define idlength        100
/*
*/
#define namelength      idlength
#define lclength        3
/*
*/

/*


*/

#define lcloc           "LOC"
/*
*/
#define lcid            "ID "
#define lc3spc          "   "
/*
*/
#define lcos            "OS "
/*
*/
/*
*/
#define lcdef           "  O"
/*
*/

#define lcbas           "BAS"
/*
*/
#define lcsq            "SQ "
/*
*/
#define lcori           "ORI"
/*
*/
#define lcsit           "SIT"
#define lcterm          "// "
#define lcdat           "DAT"
/*
*/

#define genuslimit      1


/*
*/
/*
*/





/*

*/

typedef Char idutype[idlength];


typedef Char idptype[idlength];


typedef idptype alpha;

typedef Char lcutype[lclength];


typedef Char lcptype[lclength];


typedef enum {
  embl, genb, none
} libsused;






typedef Char datetimearray[datetimearraylength];




Static _TEXT db, l1;
/*
*/
Static _TEXT changes;
Static boolean notwarned;


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
  /*
*/
  /*

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
boolean *inchanges;
long *changestart;
Char *entryname;
long basenumber;
{
  *inchanges = false;
  fprintf(changes->f,
	  "define \"unknown:%ld-%ld\" \"?\" \"[]\" \"[]\" 0 %ld\n",
	  *changestart, basenumber, basenumber - *changestart - 1);
  fprintf(changes->f, "@ ");
  writeidptype(changes, false, false, entryname);
  fprintf(changes->f, " %ld.0 +1 \"unknown:%ld-%ld\" \"\"\n",
	  *changestart, *changestart, basenumber);
  /*


*/
}



Static Void copydna(fin, fout, changes, basenumber, entryname, inchanges,
		    changestart)
_TEXT *fin, *fout, *changes;
long *basenumber;
Char *entryname;
boolean *inchanges;
long *changestart;
{
  /*
*/
  Char c;

  while (!P_eoln(fin->f)) {
    c = P_peek(fin->f);
    decapitilize(&c);

    /*
*/
    if (c >= 'a' && c <= 'z' || c == ' ') {
      if (c >= 'a' && c <= 'z')
	(*basenumber)++;


      if (c == 'u')
	c = 't';


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
	  /*

*/
	}

	if (!*inchanges) {
	  *inchanges = true;
	  *changestart = *basenumber;
	}

	/*




*/
	c = 'a';
      }



      else {
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





Static boolean lcequal(lcp1, lcp2)
Char *lcp1, *lcp2;
{
  /*

*/
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
_TEXT *fin;
Char *finidp;
{
  /*

*/
  long index = 0;
  idutype finidu;

  while (P_peek(fin->f) == ' ')
    getc(fin->f);

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


  while (index < idlength) {
    index++;
    finidu[index-1] = ' ';
  }
  memcpy(finidp, finidu, sizeof(idptype));
}



Static Void getspecies(fin, finidp)
_TEXT *fin;
Char *finidp;
{
  /*



*/
  long index = 0;
  idutype finidu;

  while (P_peek(fin->f) == ' ')
    getc(fin->f);



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


  while ((P_peek(fin->f) == ' ') | (P_peek(fin->f) == '_'))
    getc(fin->f);



  if (index > genuslimit)
    index = genuslimit;


  index++;


  finidu[index-1] = '.';


  /*

*/
  /*
*/
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



  while (index < idlength) {
    index++;
    finidu[index-1] = ' ';
  }

  memcpy(finidp, finidu, sizeof(idptype));
}



Static Void getdatetime(adatetime)
Char *adatetime;
{
  /*





*/
  Char adate[datetimearraylength], atime[datetimearraylength];
  /*


*/
  Char month[3];
  long index;

  /*




*/

  VAXdate(adate);
  VAXtime(atime);

  /*

*/


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



Static Void writedatetime(thefile, adatetime)
_TEXT *thefile;
Char *adatetime;
{
  long index;

  for (index = 0; index < datetimearraylength; index++)
    putc(adatetime[index], thefile->f);
}



Static Void readdatetime(thefile, adatetime)
_TEXT *thefile;
Char *adatetime;
{
  /*



*/
  /*



*/
  long index;
  /*
*/
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


#define tab             9






Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == tab);
}

#undef tab


Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while (isblankDelila(P_peek(thefile->f)) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((!isblankDelila(P_peek(thefile->f))) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}




Static Void note(fout, piecenum)
_TEXT *fout;
long piecenum;
{
  /*
*/
  fprintf(fout->f, "note\n");
  fprintf(fout->f, "* #%ld\n", piecenum);
  fprintf(fout->f, "note\n");
}





Static Void dna(fin, fout, libtitle, entryname)
_TEXT *fin, *fout;
libsused libtitle;
Char *entryname;
{
  /*
*/
  /*
*/
  long basenumber = 0;
  long dumpint;
  lcutype finlcu;
  lcptype finlcp;

  boolean inchanges = false;
  long changestart = -LONG_MAX;

  fprintf(fout->f, "dna\n");
  switch (libtitle) {

  case none:
    printf(" PROCEDURE DNA HAS BIZARRE VALUE OF\n");
    printf(" LIBTITLE = NONE\n");
    halt();
    break;

  /*
*/
  case embl:
    do {
      readlcu(fin, finlcu, finlcp);
      if (lcequal(finlcp, lc3spc)) {
	fprintf(fout->f, "* ");

	copydna(fin, fout, &changes, &basenumber, entryname, &inchanges,
		&changestart);
      }
      /*
*/
      else {
	fscanf(fin->f, "%*[^\n]");
	getc(fin->f);
      }
    } while (!lcequal(finlcp, lcterm));
    break;

  case genb:
    readlcu(fin, finlcu, finlcp);
    while ((!lcequal(finlcp, lcsit)) & (!lcequal(finlcp, lcterm))) {
      fscanf(fin->f, "%ld", &dumpint);
      /*

*/
      fprintf(fout->f, "* ");
      copydna(fin, fout, &changes, &basenumber, entryname, &inchanges,
	      &changestart);
      readlcu(fin, finlcu, finlcp);
    }
    /*
*/
    break;
  }

  if (inchanges)
    finishchanges(&changes, &inchanges, &changestart, entryname, basenumber);
  fprintf(fout->f, "dna\n");
}





Static Void piece(fin, fout, piecenum, libtitle, bpint, entryname, topology)
_TEXT *fin, *fout;
long piecenum;
libsused libtitle;
long bpint;
Char *entryname;
Char topology;
{
  /*
*/
  /*
*/
  /*
*/
  long index;

  fprintf(fout->f, "piece\n");
  writeidptype(fout, true, true, entryname);
  fprintf(fout->f, "* \n");
  note(fout, piecenum);
  fprintf(fout->f, "* 1\n");
  /*
*/
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
  /*

*/
  fprintf(fout->f, "piece\n");
}




Static Void chromosome(fin, fout, piecenum, libtitle, bpint, entryname,
		       orgname, topology)
_TEXT *fin, *fout;
long piecenum;
libsused libtitle;
long bpint;
Char *entryname, *orgname;
Char topology;
{
  /*
*/
  /*
*/
  /*
*/
  /*
*/
  fprintf(fout->f, "chromosome\n");

  /*


*/

  writeidptype(fout, true, true, orgname);
  fprintf(fout->f, "* \n");

  fprintf(fout->f, "* 1\n");
  fprintf(fout->f, "* %ld\n", bpint);
  piece(fin, fout, piecenum, libtitle, bpint, entryname, topology);
}





Static Void organism(fin, fout, piecenum, libtitle, bpint, entryname,
		     firstorganism, orgname, oldorgname, topology)
_TEXT *fin, *fout;
long piecenum;
libsused libtitle;
long bpint;
Char *entryname;
boolean *firstorganism;
Char *orgname, *oldorgname;
Char topology;
{
  /*
*/
  /*
*/
  /*
*/
  /*
*/
  /*
*/
  /*
*/
  /*

*/
  _TEXT TEMP;

  if (idequal(orgname, oldorgname))
    piece(fin, fout, piecenum, libtitle, bpint, entryname, topology);
  else {
    if (*firstorganism)
      *firstorganism = false;
    else {
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


  writeidptype(&TEMP, false, false, entryname);
  putchar('\n');
}





Static Void getentry(fin, fout, piecenum, firstorganism, oldorgname)
_TEXT *fin, *fout;
long piecenum;
boolean *firstorganism;
Char *oldorgname;
{
  /*
*/
  /*
*/
  /*

*/
  /*
*/
  boolean done = false;
  idptype dumpword;
  /*
*/
  lcutype finlcu;
  lcptype finlcp;
  libsused libtitle;
  /*
*/
  long bpint;
  /*
*/
  idptype entryname, locusname, orgname;
  /*
*/

  Char topology;
  _TEXT TEMP;

  do {
    readlcu(fin, finlcu, finlcp);
    if (lcequal(finlcp, lcid)) {
      libtitle = embl;
      getid(fin, entryname);

      do {
	readlcu(fin, finlcu, finlcp);
	if (lcequal(finlcp, lcos)) {
	  /*
*/
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
	  getid(fin, dumpword);
	  fscanf(fin->f, "%ld%*[^\n]", &bpint);
	  getc(fin->f);
	  /*
*/
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

      /*


*/

      getid(fin, locusname);
      fscanf(fin->f, "%ld", &bpint);
      skipcolumn(fin);
      skipcolumn(fin);
      skipblanks(fin);
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

      /*


*/

      /*


*/
      /*


*/
      /*











*/



      /*







*/
      getc(fin->f);
      /*



*/
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





Static Void datebook(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  long index;
  Char dumpchar;
  boolean findated = false;
  datetimearray oldate, adatetime;
  lcutype finlcu;
  lcptype finlcp;

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





Static Void order(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  boolean firstorganism = true;
  /*

*/
  idptype oldorgname;
  /*
*/
  long piecenum = 0;


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
	int fflag=0;
  int cflag=0;
  int oflag=0;
	char *fName = "filename.txt";
  char *change = "outputChanges.txt";
  char *outFile = "output.txt";
	static char usage[] = "usage: %s -f genome.gff -c changes.txt -o output.txt\n";

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
	if (oflag == 0) {	/* -o was mandatory */ 
		fprintf(stderr, "%s: missing -o option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} 
    
    if (fflag == 0) { /* -f was mandatory */        
		fprintf(stderr, "%s: missing -f option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
    } 
    
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
