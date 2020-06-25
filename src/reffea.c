/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "reffea.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*
*/



/*





























































*/



#define maxstring       150



#define fillermax       20




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



/*


*/
typedef Char filler[fillermax];



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;


Static _TEXT reffeap, db, reffeatures;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
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



Static Void fillstring(s, a)
stringDelila *s;
Char *a;
{
  /*
*/


  /*


*/
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



/*




*/

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
  /*




*/
  t->state++;

  /*





*/

  /*



*/

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



Static Void copyline(fin, fout)
_TEXT *fin, *fout;
{
  /*

*/
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
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



Static Void grabtoken(thefile, thestring)
_TEXT *thefile;
stringDelila *thestring;
{
  Char c;
  boolean done = false;

  skipblanks(thefile);
  thestring->length = 0;
  while (!done) {
    if (P_eoln(thefile->f)) {
      done = true;
      break;
    }
    c = getc(thefile->f);
    if (c == '\n')
      c = ' ';
    if (c == ' ')
      done = true;
    else {
      thestring->length++;
      thestring->letters[thestring->length - 1] = c;
    }
  }
}


#define exonmax         10000


typedef struct exonrecord {

  long thestart, thestop, cds;
  boolean complement, getthestart, getthestop, printasexon;
  /*
*/
} exonrecord;


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *reffeap, *db, *reffeatures;
  stringDelila accname;
  boolean allowalternative, atcomplement;
  Char c;
  long cdscount;
  trigger closeparen;
  boolean completeunit, debug, getexons, getintrons;
  long dotcount, e;
  exonrecord fealist[exonmax];
  long feacount;
  boolean firstnumber, good;
  /*
*/
  long hold;
  trigger join;
  boolean killentry, killputative, killnotexperimental, killgeneprediction,
	  killunpublished, killpseudo;
  long length, linenumber;
  boolean noends;
  long numbers[3];
  trigger organism;
  boolean printon;
  long relorient, start, stop, theAfromrange, theAtorange, theDfromrange,
       theDtorange, totalfeacount, totalentrycount, unitcount;
  boolean unitend;
} ;


Local Void grabspecies(db, f, LINK)
_TEXT *db, *f;
struct LOC_themain *LINK;
{
  Char c;

  /*

*/
  resettrigger(&LINK->organism);
  if (*db->name != '\0') {
    if (db->f != NULL)
      db->f = freopen(db->name, "r", db->f);
    else
      db->f = fopen(db->name, "r");
  } else
    rewind(db->f);
  if (db->f == NULL)
    _EscIO2(FileNotFound, db->name);
  RESETBUF(db->f, Char);
  while (!BUFEOF(db->f) && !LINK->organism.found) {
    if (P_eoln(db->f)) {
      fscanf(db->f, "%*[^\n]");
      getc(db->f);
      continue;
    }
    c = getc(db->f);
    if (c == '\n')
      c = ' ';
    testfortrigger(c, &LINK->organism);
    if (!LINK->organism.found)
      continue;
    skipblanks(db);
    c = getc(db->f);
    if (c == '\n')
      c = ' ';
    fprintf(f->f, "%c.", c);
    skipnonblanks(db);
    skipblanks(db);
    copyline(db, f);
  }
  if (!LINK->organism.found) {
    printf("ORGANISM line missing in db\n");
    halt();
  }
}

Local Void nextline(LINK)
struct LOC_themain *LINK;
{
  if (!P_eoln(LINK->db->f))
    return;
  fscanf(LINK->db->f, "%*[^\n]");

  getc(LINK->db->f);
  LINK->linenumber++;
  if (LINK->debug) {
    if (LINK->printon)
      putchar('\n');
  }
}

Local Void nextcharacter(f, c, LINK)
_TEXT *f;
Char *c;
struct LOC_themain *LINK;
{
  *c = getc(f->f);
  if (*c == '\n')
    *c = ' ';
  if (LINK->debug) {
    if (LINK->printon)
      putchar(*c);
  }
}

Local Void readaninteger(f, first, n, LINK)
_TEXT *f;
Char first;
long *n;
struct LOC_themain *LINK;
{
  /*
*/
  *n = first - '0';
  while (P_inset(P_peek(f->f), LINK->numbers)) {
    nextcharacter(f, &LINK->c, LINK);
    *n = *n * 10 + LINK->c - '0';
  }

}

Local Void sign(f, x, LINK)
_TEXT *f;
long x;
struct LOC_themain *LINK;
{
  if (x < 0)
    putc('-', f->f);
  else
    putc('+', f->f);
}

/* Local variables for printexons: */
struct LOC_printexons {
  struct LOC_themain *LINK;
} ;

Local Void thename(f, LINK)
_TEXT *f;
struct LOC_printexons *LINK;
{
  writestring(f, &LINK->LINK->accname);
  fprintf(f->f, ".CDS.");
  fprintf(f->f, "%ld-%ld", LINK->LINK->start, LINK->LINK->stop);
}

Local Void printexons(LINK)
struct LOC_themain *LINK;
{
  struct LOC_printexons V;
  long currentexons, e;
  long firstexon = 1;
  long lastexon;

  V.LINK = LINK;
  if (LINK->debug) {
    if (P_eoln(LINK->db->f))
      putchar('\n');
  }
  if (LINK->debug)
    putchar('\n');
  if (LINK->debug)
    printf("****************************");
  if (LINK->debug)
    printf("---> %4ld %4ld %4ld %s\n",
	   LINK->feacount, LINK->start, LINK->stop,
	   LINK->killentry ? " TRUE" : "FALSE");


  lastexon = LINK->feacount;

  currentexons = lastexon - firstexon + 1;
  if (currentexons > 0 && !LINK->killentry) {
    LINK->totalfeacount += currentexons;
    LINK->totalentrycount++;
    for (e = firstexon - 1; e < lastexon; e++) {
      LINK->start = LINK->fealist[e].thestart;
      LINK->stop = LINK->fealist[e].thestop;

      if (LINK->fealist[e].printasexon) {
	if (LINK->fealist[e].complement)
	  LINK->length = LINK->start - LINK->stop + 1;
	else
	  LINK->length = LINK->stop - LINK->start + 1;
      }

      if (LINK->fealist[e].complement)
	LINK->relorient = -1;
      else
	LINK->relorient = 1;

      if (LINK->fealist[e].printasexon) {
	fprintf(LINK->reffeatures->f, "define \"");
	thename(LINK->reffeatures, &V);
	fprintf(LINK->reffeatures->f, "\" \"-\" \"<]\" \"[>\" 0 %ld\n",
		LINK->length - 1);

	/*

*/
	fprintf(LINK->reffeatures->f, "@ ");
	writestring(LINK->reffeatures, &LINK->accname);
	fprintf(LINK->reffeatures->f, " %ld %ld \"",
		LINK->start, LINK->relorient);
	thename(LINK->reffeatures, &V);
	fprintf(LINK->reffeatures->f, "\" \"\"\n");
      }

      /*

*/

    }
  }
  LINK->feacount = 0;
}

Local Void addfeature(featureisexon, getstart, getstop, LINK)
boolean featureisexon, getstart, getstop;
struct LOC_themain *LINK;
{
  /*





*/
  boolean good = true;

  if (LINK->debug)
    putchar('\n');
  if (LINK->debug)
    printf("addfeature\n");
  if (LINK->debug)
    printf("    good unit: %ld..%ld\n", LINK->start, LINK->stop);
  if (LINK->debug)
    printf("    feacount: %ld\n", LINK->feacount);

  LINK->e = 1;
  if (featureisexon)
    printf("exon %ld-%ld", LINK->start, LINK->stop);
  else
    printf("intron %ld-%ld", LINK->start, LINK->stop);
  while (LINK->e <= LINK->feacount) {
    if (LINK->debug)
      printf("    exon %4ld %ld..%ld",
	     LINK->e, LINK->fealist[LINK->e-1].thestart,
	     LINK->fealist[LINK->e-1].thestop);
    /*



*/
    if ((LINK->allowalternative &&
	 LINK->fealist[LINK->e-1].thestart == LINK->start &&
	 LINK->fealist[LINK->e-1].thestop == LINK->stop) ||
	(!LINK->allowalternative &&
	 (LINK->fealist[LINK->e-1].thestart == LINK->start ||
	  LINK->fealist[LINK->e-1].thestop == LINK->stop))) {
      printf(" was dropped because it is ");
      if (LINK->fealist[LINK->e-1].thestart == LINK->start &&
	  LINK->fealist[LINK->e-1].thestop == LINK->stop)
	printf("duplicate");
      else
	printf("alternative");

      /*





*/

      good = false;
      LINK->e = LINK->feacount;
    }

    /*


*/
    else if (LINK->fealist[LINK->e-1].thestart == LINK->start ||
	     LINK->fealist[LINK->e-1].thestop == LINK->stop)
      printf(" alternative");
    LINK->e++;
  }

  if (good) {
    LINK->feacount++;
    if (LINK->feacount > exonmax) {
      printf("exonmax = %ld exceeded\n", (long)exonmax);
      halt();
    }

    LINK->fealist[LINK->feacount-1].complement = LINK->atcomplement;
    LINK->fealist[LINK->feacount-1].cds = LINK->cdscount;
    LINK->fealist[LINK->feacount-1].printasexon = featureisexon;

    if (featureisexon) {
      LINK->fealist[LINK->feacount-1].thestart = LINK->start;
      LINK->fealist[LINK->feacount-1].thestop = LINK->stop;
      LINK->fealist[LINK->feacount-1].getthestart = getstart;
      LINK->fealist[LINK->feacount-1].getthestop = getstop;
    } else {
      LINK->fealist[LINK->feacount-1].thestart = LINK->stop;
      LINK->fealist[LINK->feacount-1].thestop = LINK->start;
      LINK->fealist[LINK->feacount-1].getthestart = getstop;
      LINK->fealist[LINK->feacount-1].getthestop = getstart;
    }
  }
  putchar('\n');
}

Local Void dojoin(LINK)
struct LOC_themain *LINK;
{
  boolean getstart, getstop;
  _TEXT TEMP;

  /*
*/
  if (LINK->debug)
    printf("[JOIN FOUND]\n");
  LINK->printon = true;
  resettrigger(&LINK->join);



  resettrigger(&LINK->closeparen);
  LINK->unitcount = 0;
  LINK->firstnumber = true;
  LINK->good = true;
  LINK->dotcount = 0;
  LINK->unitend = false;
  LINK->completeunit = false;
  do {
    nextline(LINK);
    if (P_peek(LINK->db->f) == ' ')
      nextcharacter(LINK->db, &LINK->c, LINK);
    else {
      nextline(LINK);
      nextcharacter(LINK->db, &LINK->c, LINK);
      if (LINK->c == ',' || LINK->c == ')') {
	LINK->unitend = true;
	LINK->unitcount++;

      } else {
	if (P_inset(LINK->c, LINK->numbers)) {
	  if (LINK->firstnumber) {
	    readaninteger(LINK->db, LINK->c, &LINK->start, LINK);

	    if (LINK->start == LINK->stop + 1) {
	      LINK->killentry = true;
	      TEMP.f = stdout;
	      *TEMP.name = '\0';
	      writestring(&TEMP, &LINK->accname);
	      printf(" zero length intron containing entry! DELETED\n");
	    }

	    LINK->completeunit = false;
	    LINK->firstnumber = false;
	  } else {
	    readaninteger(LINK->db, LINK->c, &LINK->stop, LINK);
	    LINK->completeunit = true;
	  }
	} else if (LINK->c == '.')
	  LINK->dotcount++;
	else {
	  LINK->good = false;

	}
      }



      if (LINK->unitend && LINK->completeunit && LINK->good) {
	if (LINK->dotcount != 2)
	  LINK->good = false;
      }

      if (LINK->noends && LINK->unitend) {
	if (LINK->unitcount == 1)
	  LINK->good = false;
	if (LINK->c == ')')
	  LINK->good = false;
      }


      /*



*/

      if (LINK->unitend && LINK->completeunit && LINK->good) {
	if (LINK->atcomplement) {
	  LINK->hold = LINK->start;
	  LINK->start = LINK->stop;
	  LINK->stop = LINK->hold;
	}


	/*








*/

	getstart = true;
	getstop = true;

	if (LINK->unitcount == 1)
	  getstart = false;


	if (LINK->c == ')')
	  getstop = false;

	addfeature(true, getstart, getstop, LINK);
      }

      if (LINK->unitend) {
	LINK->unitend = false;
	LINK->good = true;
	LINK->dotcount = 0;
	LINK->firstnumber = true;
	LINK->completeunit = false;
      }

    }

    testfortrigger(LINK->c, &LINK->closeparen);
  } while (!LINK->closeparen.found);
  resettrigger(&LINK->closeparen);
  LINK->atcomplement = false;
}

Local Void dofeature(featureisexon, LINK)
boolean featureisexon;
struct LOC_themain *LINK;
{
  /*







*/
  boolean getstart = false, getstop = false;

  /*
*/

  LINK->start = 0;
  LINK->stop = 0;
  skipblanks(LINK->db);
  LINK->c = getc(LINK->db->f);
  if (LINK->c == '\n')
    LINK->c = ' ';
  if (P_inset(LINK->c, LINK->numbers)) {
    readaninteger(LINK->db, LINK->c, &LINK->start, LINK);
    getstart = true;

  } else if (LINK->c == '<') {

    LINK->c = getc(LINK->db->f);
    if (LINK->c == '\n')
      LINK->c = ' ';
    if (P_inset(LINK->c, LINK->numbers))
      readaninteger(LINK->db, LINK->c, &LINK->start, LINK);
    else
      LINK->start = 0;
  }
  LINK->c = getc(LINK->db->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == '.') {
    LINK->c = getc(LINK->db->f);

    if (LINK->c == '\n')
      LINK->c = ' ';
    if (LINK->c == '.') {
      LINK->c = getc(LINK->db->f);

      if (LINK->c == '\n')
	LINK->c = ' ';
      if (LINK->c == '>') {
	LINK->c = getc(LINK->db->f);
	if (LINK->c == '\n')
	  LINK->c = ' ';
	if (P_inset(LINK->c, LINK->numbers))
	  readaninteger(LINK->db, LINK->c, &LINK->stop, LINK);
	else
	  LINK->stop = LONG_MAX;
      } else if (P_inset(LINK->c, LINK->numbers)) {
	readaninteger(LINK->db, LINK->c, &LINK->stop, LINK);
	getstop = true;

      }
    }

  }
  fscanf(LINK->db->f, "%*[^\n]");


  getc(LINK->db->f);
  if (getstart || getstop)
    addfeature(featureisexon, getstart, getstop, LINK);

}

Local Void readparameters(LINK)
struct LOC_themain *LINK;
{
  if (*LINK->reffeap->name != '\0') {
    if (LINK->reffeap->f != NULL)
      LINK->reffeap->f = freopen(LINK->reffeap->name, "r", LINK->reffeap->f);
    else
      LINK->reffeap->f = fopen(LINK->reffeap->name, "r");
  } else
    rewind(LINK->reffeap->f);
  if (LINK->reffeap->f == NULL)
    _EscIO2(FileNotFound, LINK->reffeap->name);
  RESETBUF(LINK->reffeap->f, Char);

  fscanf(LINK->reffeap->f, "%c%*[^\n]", &LINK->c);
  getc(LINK->reffeap->f);
  if (LINK->c == '\n')
    LINK->c = ' ';
  if (LINK->c == 'n')
    LINK->noends = true;
  else
    LINK->noends = false;
  fscanf(LINK->reffeap->f, "%c%*[^\n]", &LINK->c);
  getc(LINK->reffeap->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'd')
    LINK->debug = true;
  else
    LINK->debug = false;

  fscanf(LINK->reffeap->f, "%ld%ld%*[^\n]", &LINK->theAfromrange,
	 &LINK->theAtorange);
  getc(LINK->reffeap->f);

  fscanf(LINK->reffeap->f, "%ld%ld%*[^\n]", &LINK->theDfromrange,
	 &LINK->theDtorange);
  getc(LINK->reffeap->f);

  LINK->c = getc(LINK->reffeap->f);
  if (LINK->c == '\n')
    LINK->c = ' ';
  if (LINK->c == 'e')
    LINK->getexons = true;
  else
    LINK->getexons = false;
  LINK->c = getc(LINK->reffeap->f);
  if (LINK->c == '\n')
    LINK->c = ' ';
  if (LINK->c == 'i')
    LINK->getintrons = true;
  else
    LINK->getintrons = false;
  fscanf(LINK->reffeap->f, "%*[^\n]");

  getc(LINK->reffeap->f);
  fscanf(LINK->reffeap->f, "%c%*[^\n]", &LINK->c);
  getc(LINK->reffeap->f);
  if (LINK->c == '\n')
    LINK->c = ' ';
  if (LINK->c == 'a')
    LINK->allowalternative = true;
  else
    LINK->allowalternative = false;
  LINK->c = getc(LINK->reffeap->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'P')
    LINK->killputative = true;
  else
    LINK->killputative = false;
  LINK->c = getc(LINK->reffeap->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'N')
    LINK->killnotexperimental = true;
  else
    LINK->killnotexperimental = false;
  LINK->c = getc(LINK->reffeap->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'G')
    LINK->killgeneprediction = true;
  else
    LINK->killgeneprediction = false;
  LINK->c = getc(LINK->reffeap->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'U')
    LINK->killunpublished = true;
  else
    LINK->killunpublished = false;
  LINK->c = getc(LINK->reffeap->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'S')
    LINK->killpseudo = true;
  else {
    LINK->killpseudo = false;

  }
}

/* Local variables for writeparameters: */
struct LOC_writeparameters {
  struct LOC_themain *LINK;
  _TEXT *f;
} ;

Local Void no(body, LINK)
boolean body;
struct LOC_writeparameters *LINK;
{
  fprintf(LINK->f->f, "* ");
  if (body)
    fprintf(LINK->f->f, "no ");
  else
    fprintf(LINK->f->f, "   ");
}

Local Void writeparameters(f_, LINK)
_TEXT *f_;
struct LOC_themain *LINK;
{
  struct LOC_writeparameters V;

  V.LINK = LINK;
  V.f = f_;
  fprintf(V.f->f, "***********************************************\n");
  fprintf(V.f->f, "**************** parameters *******************\n");
  no(LINK->noends, &V);
  fprintf(V.f->f, "exons on the ends of CDS included.\n");

  fprintf(V.f->f, "* the ACCEPTOR from range: %3ld\n", LINK->theAfromrange);
  fprintf(V.f->f, "* the ACCEPTOR   to range: %3ld\n", LINK->theAtorange);
  fprintf(V.f->f, "* the DONOR    from range: %3ld\n", LINK->theDfromrange);
  fprintf(V.f->f, "* the DONOR      to range: %3ld\n", LINK->theDtorange);

  no(!LINK->getexons, &V);
  fprintf(V.f->f, "exon features      included.\n");

  no(!LINK->getintrons, &V);
  fprintf(V.f->f, "intron features    included.\n");

  no(!LINK->allowalternative, &V);
  fprintf(V.f->f, "alternative exons  included.\n");

  no(!LINK->killputative, &V);
  fprintf(V.f->f, "\"putative\"         entries will be killed.\n");

  no(!LINK->killnotexperimental, &V);
  fprintf(V.f->f, "\"not_experimental\" entries will be killed.\n");

  no(!LINK->killgeneprediction, &V);
  fprintf(V.f->f, "\"gene prediction\"  entries will be killed.\n");

  no(!LINK->killunpublished, &V);
  fprintf(V.f->f, "\"Unpublished\"      entries will be killed.\n");

  no(!LINK->killpseudo, &V);
  fprintf(V.f->f, "\"pseudo\"           entries will be killed.\n");

  fprintf(V.f->f, "***********************************************\n");

}

Local Void killtest(c, t, n, killt, killentry, LINK)
Char c;
trigger *t;
stringDelila n;
boolean killt, *killentry;
struct LOC_themain *LINK;
{
  /*


*/
  _TEXT TEMP;

  testfortrigger(c, t);
  if (!t->found)
    return;
  if (n.length > 0) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &n);
  } else
    printf("The next entry");
  printf(" will be");
  if (killt) {
    printf(" KILLED");
    *killentry = true;
  } else
    printf(" KEPT");
  printf(".  It contains: \"");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &t->seek);
  printf("\".\n");
  resettrigger(t);
}



Static Void themain(reffeap_, db_, reffeatures_)
_TEXT *reffeap_, *db_, *reffeatures_;
{
  struct LOC_themain V;
  trigger accession, cds, complement, comma, features, entryend, exon, intron,
	  geneprediction, locus;
  long locuscount = 0;
  trigger putative, notexperimental;
  boolean infeatures = false;
  trigger unpublished, pseudo;

  trigger reference;
  long refnumber;
  _TEXT TEMP;

  V.reffeap = reffeap_;
  V.db = db_;
  V.reffeatures = reffeatures_;
  printf("ref %4.2f\n", version);

  readparameters(&V);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeparameters(&TEMP, &V);

  if (V.debug)
    printf("DEBUGGING\n");



  filltrigger(&locus, "LOCUS               ");
  filltrigger(&V.organism, "ORGANISM            ");
  filltrigger(&accession, "ACCESSION           ");
  filltrigger(&reference, "REFERENCE           ");

  filltrigger(&features, "FEATURES            ");
  filltrigger(&cds, "CDS                 ");
  filltrigger(&exon, "exon                ");
  filltrigger(&intron, "intron              ");
  filltrigger(&V.join, "join(               ");
  filltrigger(&V.closeparen, ")                   ");
  filltrigger(&comma, ",                   ");
  filltrigger(&complement, "complement(         ");
  filltrigger(&putative, "putative            ");
  filltrigger(&notexperimental, "not_experimental    ");
  filltrigger(&geneprediction, "gene prediction     ");
  filltrigger(&unpublished, "Unpublished         ");
  filltrigger(&pseudo, "pseudo              ");
  filltrigger(&entryend, "//                  ");

  resettrigger(&reference);


  resettrigger(&cds);
  resettrigger(&exon);
  resettrigger(&intron);
  resettrigger(&locus);
  resettrigger(&accession);
  resettrigger(&features);
  resettrigger(&V.join);
  resettrigger(&V.closeparen);
  resettrigger(&complement);
  resettrigger(&comma);
  resettrigger(&putative);
  resettrigger(&notexperimental);
  resettrigger(&geneprediction);
  resettrigger(&unpublished);
  resettrigger(&pseudo);
  resettrigger(&entryend);

  V.cdscount = 0;
  V.totalentrycount = 0;
  V.totalfeacount = 0;
  V.feacount = 0;
  V.linenumber = 1;
  P_addset(P_expset(V.numbers, 0L), '0');
  P_addset(V.numbers, '1');
  P_addset(V.numbers, '2');
  P_addset(V.numbers, '3');
  P_addset(V.numbers, '4');
  P_addset(V.numbers, '5');
  P_addset(V.numbers, '6');
  P_addset(V.numbers, '7');
  P_addset(V.numbers, '8');
  P_addset(V.numbers, '9');
  V.killentry = false;

  if (*V.reffeatures->name != '\0') {
    if (V.reffeatures->f != NULL)
      V.reffeatures->f = freopen(V.reffeatures->name, "w", V.reffeatures->f);
    else
      V.reffeatures->f = fopen(V.reffeatures->name, "w");
  } else {
    if (V.reffeatures->f != NULL)
      rewind(V.reffeatures->f);
    else
      V.reffeatures->f = tmpfile();
  }
  if (V.reffeatures->f == NULL)
    _EscIO2(FileNotFound, V.reffeatures->name);
  SETUPBUF(V.reffeatures->f, Char);
  fprintf(V.reffeatures->f, "* ref: reffeatures %4.2f\n", version);

  if (*V.db->name != '\0') {
    if (V.db->f != NULL)
      V.db->f = freopen(V.db->name, "r", V.db->f);
    else
      V.db->f = fopen(V.db->name, "r");
  } else
    rewind(V.db->f);
  if (V.db->f == NULL)
    _EscIO2(FileNotFound, V.db->name);
  RESETBUF(V.db->f, Char);

  while (!BUFEOF(V.db->f)) {
    while (!P_eoln(V.db->f)) {
      nextcharacter(V.db, &V.c, &V);

      testfortrigger(V.c, &locus);
      if (locus.found) {
	V.killentry = false;
	locuscount++;
	V.cdscount = 0;
	clearstring(&V.accname);
	infeatures = false;
	V.atcomplement = false;
	V.stop = -LONG_MAX;
	V.start = -LONG_MAX;
	if (V.debug)
	  putchar('\n');
	if (V.debug)
	  printf("LOCUS %ld ======================\n", locuscount);
      }

      testfortrigger(V.c, &entryend);
      if (entryend.found)
	printexons(&V);


      testfortrigger(V.c, &accession);
      if (accession.found) {
	grabtoken(V.db, &V.accname);
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &V.accname);
	putchar('\n');
	if (V.accname.length <= 0)
	  printf("MISSING ACCESSION STRING\n");
	resettrigger(&accession);
	if (V.debug)
	  printf("ACCESSION ");
	if (V.debug) {
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writestring(&TEMP, &V.accname);
	}
	if (V.debug)
	  printf(" ======================\n");
      }



      if (!V.killentry) {
	killtest(V.c, &putative, V.accname, V.killputative, &V.killentry, &V);
	killtest(V.c, &notexperimental, V.accname, V.killnotexperimental,
		 &V.killentry, &V);
	killtest(V.c, &geneprediction, V.accname, V.killgeneprediction,
		 &V.killentry, &V);
	killtest(V.c, &unpublished, V.accname, V.killunpublished,
		 &V.killentry, &V);
	killtest(V.c, &pseudo, V.accname, V.killpseudo, &V.killentry, &V);
      }

      testfortrigger(V.c, &reference);
      if (reference.found) {
	skipblanks(V.db);

	V.c = getc(V.db->f);
	if (V.c == '\n')
	  V.c = ' ';
	if (P_inset(V.c, V.numbers))
	  readaninteger(V.db, V.c, &refnumber, &V);
	else {
	  printf("a numeral does not follow the REFERENCE!\n");
	  halt();
	}

	skipblanks(V.db);

	V.c = getc(V.db->f);
	if (V.c == '\n')
	  V.c = ' ';
	if (V.c != '(') {
	  printf("REFERENCE number is not followed by a \"(\"\n");
	  halt();
	}

	V.c = getc(V.db->f);
	if (V.c == '\n')
	  V.c = ' ';
	if (V.c == 'b') {
	  skipnonblanks(V.db);

	  skipblanks(V.db);

	  V.c = getc(V.db->f);
	  if (V.c == '\n')
	    V.c = ' ';
	  if (P_inset(V.c, V.numbers))
	    readaninteger(V.db, V.c, &V.start, &V);
	  else {
	    printf("a numeral does not follow the REFERENCE!\n");
	    halt();
	  }

	  skipblanks(V.db);
	  V.c = getc(V.db->f);
	  if (V.c == '\n')
	    V.c = ' ';
	  if (V.c != 't') {
	    printf("first REFERENCE number is not followed by \"to\"\n");
	    halt();
	  }

	  skipnonblanks(V.db);

	  skipblanks(V.db);

	  V.c = getc(V.db->f);
	  if (V.c == '\n')
	    V.c = ' ';
	  if (P_inset(V.c, V.numbers))
	    readaninteger(V.db, V.c, &V.stop, &V);
	  else {
	    printf("a numeral does not follow the \"to\"!\n");
	    halt();
	  }

	  V.length = V.stop - V.start + 1;

	  printf("reference number %ld", refnumber);
	  printf(" start %ld", V.start);
	  printf(" stop %ld", V.stop);
	  printf(" length %ld\n", V.length);

	  /*


*/
	  fprintf(V.reffeatures->f, "define");


	  fprintf(V.reffeatures->f, " \"");
	  writestring(V.reffeatures, &V.accname);
	  fprintf(V.reffeatures->f, ".ref.%ld\"", refnumber);

	  fprintf(V.reffeatures->f, " \"-\" \"<]\" \"[>\" 0 %ld\n",
		  V.length - 1);



	  fprintf(V.reffeatures->f, "@ ");
	  writestring(V.reffeatures, &V.accname);
	  fprintf(V.reffeatures->f, " %ld", V.start);


	  if (V.stop > V.start)
	    fprintf(V.reffeatures->f, " 1");
	  else
	    fprintf(V.reffeatures->f, " -1");


	  fprintf(V.reffeatures->f, " \"");
	  writestring(V.reffeatures, &V.accname);
	  fprintf(V.reffeatures->f, ".ref.%ld\"", refnumber);
	  fprintf(V.reffeatures->f, " \"\"\n");

	  V.totalentrycount++;
	  V.totalfeacount++;
	}


      }
      /*

*/


      testfortrigger(V.c, &features);


      features.found = false;

      if (features.found)
	infeatures = true;


      testfortrigger(V.c, &cds);
      if (cds.found) {
	if (infeatures) {
	  skipblanks(V.db);
	  V.cdscount++;
	  if (V.debug)
	    putchar('\n');
	  if (V.debug)
	    printf("CDS %ld ======================\n", V.cdscount);

	  while (!P_eoln(V.db->f)) {
	    nextcharacter(V.db, &V.c, &V);

	    testfortrigger(V.c, &complement);
	    if (complement.found)
	      V.atcomplement = true;

	    if (V.atcomplement) {
	      testfortrigger(V.c, &V.closeparen);
	      if (V.closeparen.found)
		V.atcomplement = false;
	    }

	    testfortrigger(V.c, &V.join);
	    if (V.join.found) {
	      dojoin(&V);
	      continue;
	    }
	    if (!P_inset(V.c, V.numbers))
	      continue;

	    V.atcomplement = false;
	    skipblanks(V.db);
	    readaninteger(V.db, V.c, &V.start, &V);

	    V.c = getc(V.db->f);
	    if (V.c == '\n')
	      V.c = ' ';
	    if (V.c != '.') {
	      printf("error in reading CDS \"%c\" found instead of \".\"\n",
		     V.c);
	      halt();
	    }

	    V.c = getc(V.db->f);
	    if (V.c == '\n')
	      V.c = ' ';
	    if (V.c != '.') {
	      printf("error in reading CDS \"%c\" found instead of \".\"\n",
		     V.c);
	      halt();
	    }

	    V.c = getc(V.db->f);
	    if (V.c == '\n')
	      V.c = ' ';
	    if (!P_inset(V.c, V.numbers)) {
	      if (V.c == '>') {
		V.c = getc(V.db->f);
		if (V.c == '\n')
		  V.c = ' ';
		if (P_inset(V.c, V.numbers))
		  readaninteger(V.db, V.c, &V.stop, &V);
		else
		  V.stop = LONG_MAX;
	      } else {
		printf("error in reading CDS \"%c\" is not a number\n", V.c);

		halt();
	      }
	    } else
	      readaninteger(V.db, V.c, &V.stop, &V);
	    /*


*/
	    addfeature(true, true, true, &V);
	  }
	}
      }


      if (infeatures) {
	if (V.getexons) {
	  testfortrigger(V.c, &exon);
	  if (exon.found) {
	    dofeature(true, &V);
	    resettrigger(&exon);
	  }
	}
      }

      if (!infeatures)
	continue;

      if (V.getintrons) {
	testfortrigger(V.c, &intron);
	if (intron.found) {
	  dofeature(false, &V);
	  resettrigger(&intron);
	}
      }
    }
    nextline(&V);
  }

  printexons(&V);

  printf("%ld entrys have REFERENCES\n", V.totalentrycount);
  printf("%ld REFERENCES processed\n", V.totalfeacount);
}

#undef exonmax


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  reffeatures.f = NULL;
  strcpy(reffeatures.name, "reffeatures");
  db.f = NULL;
  strcpy(db.name, "db");
  reffeap.f = NULL;
  strcpy(reffeap.name, "reffeap");
  themain(&reffeap, &db, &reffeatures);
_L1:
  if (reffeap.f != NULL)
    fclose(reffeap.f);
  if (db.f != NULL)
    fclose(db.f);
  if (reffeatures.f != NULL)
    fclose(reffeatures.f);
  exit(EXIT_SUCCESS);
}



/* End. */
