/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "exon.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         2.38
/*





































*/
#define updateversion   1.26



/*

















































































































































































































*/


/*
























































*/



#define exonmax         30000



#define maxstring       2000




#define fillermax       20





typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;




/*


*/
typedef Char filler[fillermax];



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;




typedef short position;


Static _TEXT exonp, db, lengths, dinst, ainst, einst, exonfeatures;


Static long columnposition;


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


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
  clearstring(ribbon);
  ribbon->next = NULL;
}




Static boolean equalstring(a, b)
stringDelila a, b;
{
  /*





*/
  long index;
  boolean equal;

  if (a.length == b.length) {
    index = 1;
    do {
      equal = (a.letters[index-1] == b.letters[index-1]);
      index++;
    } while (equal && index <= a.length);
    return equal;
  } else
    return false;
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
  if (t->seek.letters[t->state - 1] == ch) {
    t->skip = false;
    if (t->state == t->seek.length)
      t->found = true;
    else
      t->found = false;
    return;
  }
  /*
*/
  if (t->seek.letters[0] == ch) {
    t->state = 1;
    t->skip = false;
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



Static long copylines(fin, fout, n)
_TEXT *fin, *fout;
long n;
{
  /*
*/
  long index = 0;

  while (!BUFEOF(fin->f) && index < n) {
    copyaline(fin, fout);
    index++;
  }

  return index;
}



Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) == ' ') & (!P_eoln(thefile->f))) {
    columnposition++;
    getc(thefile->f);
  }

}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) != ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


/*





*/



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



Static Void grabquote(thefile, thestring)
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
    if (c == '"')
      done = true;
    else {
      thestring->length++;
      thestring->letters[thestring->length - 1] = c;
    }
  }
}


#define colwid          7


typedef struct exonrecord {

  long thestart, thestop, FEA;
  boolean complement, getthestart, getthestop, printasexon;
  /*
*/
  boolean genenamefound;
  stringDelila genenamestring;
  boolean genenumberfound;
  /*
*/
  stringDelila genenumberstring, notestring;
  long notenumber, featurenumber;
  /*

*/
} exonrecord;

/*

*/

typedef struct exonset {
  exonrecord a[exonmax];
} exonset;



/* Local variables for themain: */
struct LOC_themain {
  _TEXT *exonp, *db, *lengths, *dinst, *ainst, *einst, *exonfeatures;
  stringDelila accname;
  /*
*/
  boolean allowalternative, atcomplement;
  Char c, cprevious;
  /*
*/
  trigger FEA;
  long FEAcount;
  trigger closeparen, complement;
  boolean completeunit, debug, getexons, getintrons;
  long dotcount;
  /*


*/
  exonset *fealist;
  boolean capturegenename;
  /*
*/

  long feastored, featarget, featurecount;
  boolean firstnumber;
  trigger genename, genenumber;
  _TEXT holdfile;
  boolean infeatures, good;
  /*
*/
  long hold;
  trigger join;
  boolean killentry, killputative, killnotexperimental, killgeneprediction,
	  killunpublished, killpseudo;
  long length, linenumber, locuscount;
  boolean noends;
  long numbers[3];
  trigger organism;
  double parameterversion;

  boolean printon;
  long relorient, start, stop, theAfromrange, theAtorange;
  /*
*/
  long theDfromrange, theDtorange;
  Char thefeature;
  long totalfeastored, totalentrycount, unitcount;
  boolean unitend;
  stringDelila unknownstring;
  boolean usenotes, verbose;
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
    /*



*/

    while (!P_eoln(db->f)) {
      if (P_peek(db->f) != ' ')
	CPUTFBUF(f->f, P_peek(db->f));
      else
	CPUTFBUF(f->f, '-');
      CPUT(f->f);
      getc(db->f);
    }
  }
  if (!LINK->organism.found) {
    printf("ORGANISM line missing in db\n");
    halt();
  }
}

Local Void startinst(f, db, sitetype, LINK)
_TEXT *f, *db;
Char sitetype;
struct LOC_themain *LINK;
{
  /*


*/
  if (*f->name != '\0') {
    if (f->f != NULL)
      f->f = freopen(f->name, "w", f->f);
    else
      f->f = fopen(f->name, "w");
  } else {
    if (f->f != NULL)
      rewind(f->f);
    else
      f->f = tmpfile();
  }
  if (f->f == NULL)
    _EscIO2(FileNotFound, f->name);
  SETUPBUF(f->f, Char);
  fprintf(f->f, "title \"");
  if (sitetype == 'a')
    fprintf(f->f, "acceptor");
  if (sitetype == 'd')
    fprintf(f->f, "donor");
  if (sitetype == 'e')
    fprintf(f->f, "exon");
  fprintf(f->f, "\";\n");

  fprintf(f->f, "(* exon %4.2f *)\n", version);

  fprintf(f->f, "default out-of-range reduce-range;\n");
  fprintf(f->f, "default numbering piece;\n");

  fprintf(f->f, "organism ");
  grabspecies(db, f, LINK);
  fprintf(f->f, ";\n");
  fprintf(f->f, "chromosome ");
  grabspecies(db, f, LINK);
  fprintf(f->f, ";\n\n");
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
  columnposition = 0;
}

Local Void nextcharacter(f, cprevious, c, LINK)
_TEXT *f;
Char *cprevious, *c;
struct LOC_themain *LINK;
{
  /*
*/
  *cprevious = *c;
  *c = getc(f->f);
  if (*c == '\n')
    *c = ' ';
  columnposition++;
  if (!LINK->verbose)
    return;
  /*





*/
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
  if (!P_inset(first, LINK->numbers)) {
    printf("readaninteger: bad number!\n");
    printf("first is: %c\n", first);
    halt();
  }
  while (P_inset(P_peek(f->f), LINK->numbers)) {
    nextcharacter(f, &LINK->cprevious, &LINK->c, LINK);
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

Local Void thename(f, e, LINK)
_TEXT *f;
long e;
struct LOC_themain *LINK;
{
  /*









*/
  long start, stop;

  /*

*/
  /*



*/
  if (LINK->fealist->a[e-1].printasexon) {
    start = LINK->fealist->a[e-1].thestart;
    stop = LINK->fealist->a[e-1].thestop;
  } else {
    start = LINK->fealist->a[e-1].thestop;
    stop = LINK->fealist->a[e-1].thestart;
  }
  writestring(f, &LINK->accname);

  if (LINK->fealist->a[e-1].genenamefound) {
    putc('.', f->f);
    writestring(f, &LINK->fealist->a[e-1].genenamestring);
  }

  if (LINK->fealist->a[e-1].genenumberfound) {
    fprintf(f->f, ".#");
    writestring(f, &LINK->fealist->a[e-1].genenumberstring);
  }

  if (!LINK->fealist->a[e-1].genenamefound &&
      !LINK->fealist->a[e-1].genenumberfound) {
    /*

*/
    if (LINK->thefeature == 'r')
      fprintf(f->f, ".mRNA:");
    else
      fprintf(f->f, ".CDS:");
    fprintf(f->f, "%ld-%ld", start, stop);
  }

  if (equalstring(LINK->fealist->a[e-1].genenamestring, LINK->unknownstring))
    fprintf(f->f, ".%ld-%ld", start, stop);


  if (!LINK->usenotes)
    return;

  if (LINK->fealist->a[e-1].notenumber > 0) {
    putc('.', f->f);
    writestring(f, &LINK->fealist->a[e-1].notestring);
    /*





*/

  }
}

Local Void writeDelilainstructions(inst, location, thefromrange, thetorange,
				   e, LINK)
_TEXT *inst;
long location, thefromrange, thetorange, e;
struct LOC_themain *LINK;
{
  fprintf(inst->f, "piece ");
  writestring(inst, &LINK->accname);
  putc(';', inst->f);

  fprintf(inst->f, " name \"");
  thename(inst, e, LINK);
  fprintf(inst->f, "\";");

  fprintf(inst->f, " get from %ld", location);
  putc(' ', inst->f);
  sign(inst, thefromrange * LINK->relorient, LINK);

  if (-LINK->relorient > 0)
    fprintf(inst->f, "%ld", labs(thefromrange));
  else
    fprintf(inst->f, "%ld", labs(thefromrange));

  fprintf(inst->f, " to same ");

  sign(inst, thetorange * LINK->relorient, LINK);

  if (LINK->relorient > 0)
    fprintf(inst->f, "%ld", labs(thetorange));
  else
    fprintf(inst->f, "%ld", labs(thetorange));

  fprintf(inst->f, " direction ");
  sign(inst, LINK->relorient, LINK);
  fprintf(inst->f, ";\n");
}




Local boolean lessthan(a, b, LINK)
position a, b;
struct LOC_themain *LINK;
{
  /*










*/
  return (LINK->fealist->a[a-1].thestart < LINK->fealist->a[b-1].thestart);
}

Local Void swap_(a, b, LINK)
position a, b;
struct LOC_themain *LINK;
{
  exonrecord hold;

  hold = LINK->fealist->a[a-1];
  LINK->fealist->a[a-1] = LINK->fealist->a[b-1];
  LINK->fealist->a[b-1] = hold;

}



Local Void quicksort(left, right, LINK)
position left, right;
struct LOC_themain *LINK;
{
  /*












*/
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



Local Void sortexons(LINK)
struct LOC_themain *LINK;
{
  quicksort(1, (int)LINK->feastored, LINK);
}



Local Void printexons(LINK)
struct LOC_themain *LINK;
{
  long currentexons;
  boolean duplicate;
  long current, previous, e;
  long firstexon = 1;
  long lastexon;
  exonrecord *WITH;

  if (LINK->debug) {
    printf("**** in printexons *********\n");
    printf("****************************");
    printf("---> %4ld %4ld %4ld %s\n",
	   LINK->feastored, LINK->start, LINK->stop,
	   LINK->killentry ? " TRUE" : "FALSE");
  }

  lastexon = LINK->feastored;

  currentexons = lastexon - firstexon + 1;
  if (currentexons > 0 && !LINK->killentry) {
    sortexons(LINK);
    LINK->totalfeastored += currentexons;
    LINK->totalentrycount++;
    for (e = firstexon; e <= lastexon; e++) {
      if (LINK->debug)
	printf("PRINTEXONS e = %ld==========\n", e);
      LINK->start = LINK->fealist->a[e-1].thestart;
      LINK->stop = LINK->fealist->a[e-1].thestop;

      if (LINK->fealist->a[e-1].printasexon) {
	if (LINK->fealist->a[e-1].complement)
	  LINK->length = LINK->start - LINK->stop + 1;
	else
	  LINK->length = LINK->stop - LINK->start + 1;

	if (LINK->length <= 0) {
	  fprintf(LINK->lengths->f, " ERROR: length < 0 for this entry:\n");
	  printf("       \007ERROR: see lengths file\n");
	}


	fprintf(LINK->lengths->f, " %*ld %*ld %*ld",
		colwid, LINK->length, colwid, LINK->start, colwid,
		LINK->stop);
	if (LINK->fealist->a[e-1].complement)
	  fprintf(LINK->lengths->f, " %*c", colwid, '-');
	else
	  fprintf(LINK->lengths->f, " %*c", colwid, '+');
	fprintf(LINK->lengths->f, " %*ld %*ld %*ld",
		colwid, e, colwid, LINK->fealist->a[e-1].FEA, colwid,
		LINK->locuscount);

	putc(' ', LINK->lengths->f);
	writestring(LINK->lengths, &LINK->accname);

	putc('\n', LINK->lengths->f);
      }

      if (LINK->fealist->a[e-1].complement)
	LINK->relorient = -1;
      else
	LINK->relorient = 1;


      /*

*/
      WITH = &LINK->fealist->a[e-1];

      if (WITH->getthestart) {
	if (e > firstexon) {
	  previous = LINK->fealist->a[e-2].thestart;
	  if (LINK->fealist->a[e-2].printasexon) {
	    if (LINK->fealist->a[e-1].complement)
	      previous--;
	    else
	      previous++;
	  }

	  if (LINK->fealist->a[e-1].printasexon)
	    current = WITH->thestart - LINK->relorient;
	  else
	    current = WITH->thestart;

	  duplicate = (current == previous &&
		       LINK->fealist->a[e-2].getthestart);
	} else
	  duplicate = false;

	if (!duplicate) {
	  if (LINK->fealist->a[e-1].printasexon)
	    writeDelilainstructions(LINK->ainst,
	      WITH->thestart - LINK->relorient, LINK->theAfromrange,
	      LINK->theAtorange, e, LINK);
	  else
	    writeDelilainstructions(LINK->ainst, WITH->thestart,
				    LINK->theAfromrange, LINK->theAtorange, e,
				    LINK);
	}

      }


      else {
	if (LINK->fealist->a[e-1].printasexon)
	  printf("%ld exon start was dropped from ainst: end of CDS or sequence\n",
		 WITH->thestart);
	else
	  printf("%ld intron stop was dropped from ainst: end of sequence\n",
		 WITH->thestart);
      }

      if (WITH->getthestop) {
	if (e > firstexon) {
	  previous = LINK->fealist->a[e-2].thestop;
	  if (LINK->fealist->a[e-2].printasexon) {
	    if (LINK->fealist->a[e-1].complement)
	      previous--;
	    else
	      previous++;
	  }

	  if (LINK->fealist->a[e-1].printasexon)
	    current = WITH->thestop - LINK->relorient;
	  else
	    current = WITH->thestop;

	  duplicate = (current == previous &&
		       LINK->fealist->a[e-2].getthestop);
	} else
	  duplicate = false;

	if (!duplicate) {
	  if (LINK->fealist->a[e-1].printasexon)
	    writeDelilainstructions(LINK->dinst,
	      WITH->thestop + LINK->relorient, LINK->theDfromrange,
	      LINK->theDtorange, e, LINK);
	  else
	    writeDelilainstructions(LINK->dinst, WITH->thestop,
				    LINK->theDfromrange, LINK->theDtorange, e,
				    LINK);
	}
      }

      else {
	if (LINK->fealist->a[e-1].printasexon)
	  printf("%ld exon stop was dropped from dinst: end of CDS or sequence\n",
		 WITH->thestop);
	else {
	  printf("%ld intron start was dropped from dinst: end of sequence\n",
		 WITH->thestop);
	  /*






*/
	}
      }
      if (LINK->fealist->a[e-1].printasexon) {
	WITH = &LINK->fealist->a[e-1];

	/*

*/
	fprintf(LINK->einst->f, "piece ");
	writestring(LINK->einst, &LINK->accname);
	fprintf(LINK->einst->f, "; ");
	if (LINK->relorient == 1) {
	  fprintf(LINK->einst->f, "name \"");
	  thename(LINK->einst, e, LINK);
	  fprintf(LINK->einst->f, "\"; ");
	  fprintf(LINK->einst->f, "get from ");
	  fprintf(LINK->einst->f, "%ld", WITH->thestart);
	  putc(' ', LINK->einst->f);
	  sign(LINK->einst, LINK->theAfromrange, LINK);
	  fprintf(LINK->einst->f, "%ld", labs(LINK->theAfromrange));
	  fprintf(LINK->einst->f, " to ");
	  fprintf(LINK->einst->f, "%ld", WITH->thestop);
	  putc(' ', LINK->einst->f);
	  sign(LINK->einst, LINK->theDtorange, LINK);
	  fprintf(LINK->einst->f, "%ld", labs(LINK->theDtorange));
	  fprintf(LINK->einst->f, " direction +;\n");
	} else {
	  fprintf(LINK->einst->f, "name \"");
	  thename(LINK->einst, e, LINK);
	  fprintf(LINK->einst->f, "\"; ");
	  fprintf(LINK->einst->f, "get from ");
	  fprintf(LINK->einst->f, "%ld", WITH->thestop);
	  putc(' ', LINK->einst->f);
	  sign(LINK->einst, -LINK->theDtorange, LINK);
	  fprintf(LINK->einst->f, "%ld", labs(LINK->theDtorange));
	  fprintf(LINK->einst->f, " to ");
	  fprintf(LINK->einst->f, "%ld", WITH->thestart);
	  putc(' ', LINK->einst->f);
	  sign(LINK->einst, -LINK->theAfromrange, LINK);
	  fprintf(LINK->einst->f, "%ld", labs(LINK->theAfromrange));
	  fprintf(LINK->einst->f, " direction +;\n");
	}
	fprintf(LINK->exonfeatures->f, "define \"");
	thename(LINK->exonfeatures, e, LINK);
	fprintf(LINK->exonfeatures->f, "\" \"-\" \"<]\" \"[>\" 0 %ld\n",
		LINK->length - 1);

	/*

*/
	fprintf(LINK->exonfeatures->f, "@ ");
	writestring(LINK->exonfeatures, &LINK->accname);
	fprintf(LINK->exonfeatures->f, " %ld %ld \"",
		LINK->start, LINK->relorient);
	thename(LINK->exonfeatures, e, LINK);
	fprintf(LINK->exonfeatures->f, "\" \"\"\n");
      }


    }
  }

  LINK->feastored = 0;
}

Local Void addfeature(featureisexon, getstart, getstop, LINK)
boolean featureisexon, getstart, getstop;
struct LOC_themain *LINK;
{
  /*





*/
  long e = 1;
  long previous;
  boolean good = true;

  if (LINK->debug)
    putchar('\n');
  if (LINK->debug)
    printf("addfeature\n");
  if (LINK->debug)
    printf("    good unit: %ld..%ld\n", LINK->start, LINK->stop);
  if (LINK->debug)
    printf("    feastored: %ld\n", LINK->feastored);

  if (LINK->verbose) {
    if (featureisexon)
      printf("  CDS/exon %ld-%ld", LINK->start, LINK->stop);
    else
      printf("  intron %ld-%ld", LINK->start, LINK->stop);
  }

  while (e <= LINK->feastored) {
    if (LINK->debug)
      printf("    exon %4ld %ld..%ld",
	     e, LINK->fealist->a[e-1].thestart,
	     LINK->fealist->a[e-1].thestop);
    /*



*/
    if ((LINK->allowalternative &&
	 LINK->fealist->a[e-1].thestart == LINK->start &&
	 LINK->fealist->a[e-1].thestop == LINK->stop) ||
	(!LINK->allowalternative &&
	 (LINK->fealist->a[e-1].thestart == LINK->start ||
	  LINK->fealist->a[e-1].thestop == LINK->stop))) {
      /*


*/
      if (LINK->verbose) {
	printf(" was dropped because it is ");
	if (LINK->fealist->a[e-1].thestart == LINK->start &&
	    LINK->fealist->a[e-1].thestop == LINK->stop)
	  printf("duplicate");
	else
	  printf("alternative");
      }

      /*





*/

      good = false;
      previous = e;
      e = LINK->feastored;
    }

    else if (LINK->fealist->a[e-1].thestart == LINK->start ||
	     LINK->fealist->a[e-1].thestop == LINK->stop) {
      if (LINK->verbose)
	printf(" alternative");
    }
    e++;
  }

  if (good) {
    LINK->feastored++;
    if (LINK->feastored > exonmax) {
      printf("\nexonmax = %ld exceeded\n", (long)exonmax);
      halt();
    }

    LINK->fealist->a[LINK->feastored-1].complement = LINK->atcomplement;
    LINK->fealist->a[LINK->feastored-1].FEA = LINK->FEAcount;
    LINK->fealist->a[LINK->feastored-1].printasexon = featureisexon;
    LINK->fealist->a[LINK->feastored-1].genenamefound = false;
    LINK->fealist->a[LINK->feastored-1].genenumberfound = false;
    LINK->fealist->a[LINK->feastored-1].notenumber = 0;
    clearstring(&LINK->fealist->a[LINK->feastored-1].notestring);
    LINK->fealist->a[LINK->feastored-1].featurenumber = LINK->featurecount;
    /*

*/

    if (featureisexon) {
      LINK->fealist->a[LINK->feastored-1].thestart = LINK->start;
      LINK->fealist->a[LINK->feastored-1].thestop = LINK->stop;
      LINK->fealist->a[LINK->feastored-1].getthestart = getstart;
      LINK->fealist->a[LINK->feastored-1].getthestop = getstop;
    } else {
      LINK->fealist->a[LINK->feastored-1].thestart = LINK->stop;
      LINK->fealist->a[LINK->feastored-1].thestop = LINK->start;
      LINK->fealist->a[LINK->feastored-1].getthestart = getstop;
      LINK->fealist->a[LINK->feastored-1].getthestop = getstart;
    }

    LINK->featarget = LINK->feastored;
  } else {
    LINK->featarget = previous;

  }
  /*
*/
  /*









*/



  if (LINK->verbose)
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
  if (LINK->verbose)
    putchar('\n');
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
      nextcharacter(LINK->db, &LINK->cprevious, &LINK->c, LINK);
    else {
      nextline(LINK);
      nextcharacter(LINK->db, &LINK->cprevious, &LINK->c, LINK);
      if (LINK->c == ',' || LINK->c == ')') {
	LINK->unitend = true;
	LINK->unitcount++;

      } else {
	if (P_inset(LINK->c, LINK->numbers)) {
	  if (LINK->firstnumber) {
	    readaninteger(LINK->db, LINK->c, &LINK->start, LINK);

	    if (LINK->start == LINK->stop + 1) {
	      TEMP.f = stdout;
	      *TEMP.name = '\0';
	      writestring(&TEMP, &LINK->accname);
	      printf(" zero length intron containing entry! DELETED\n");
	    }
	    /*
*/


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
  if (LINK->verbose)
    printf(")\n");

  resettrigger(&LINK->closeparen);
  LINK->atcomplement = false;
}

Local Void dofeature(featureisexon, LINK)
boolean featureisexon;
struct LOC_themain *LINK;
{
  /*







*/
  boolean done;
  boolean getstart = false, getstop = false;

  /*
*/

  LINK->start = 0;
  LINK->stop = 0;
  skipblanks(LINK->db);
  LINK->c = getc(LINK->db->f);

  if (LINK->c == '\n')
    LINK->c = ' ';
  LINK->atcomplement = false;
  if (LINK->c == 'c') {
    resettrigger(&LINK->complement);

    LINK->complement.state++;
    done = false;
    while (!done) {
      if (P_eoln(LINK->db->f))
	break;
      LINK->c = getc(LINK->db->f);

      if (LINK->c == '\n') {
/* p2c: exon.p: Note: Eliminated unused assignment statement [338] */
	LINK->c = ' ';
      }
      testfortrigger(LINK->c, &LINK->complement);
      if (!LINK->complement.found)
	continue;
      LINK->atcomplement = true;
      done = true;
      LINK->c = getc(LINK->db->f);
      if (LINK->c == '\n')
	LINK->c = ' ';
    }
  }



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
  /*



*/
  if (LINK->start == 0)
    getstart = false;
  if (LINK->stop == 0)
    getstart = false;

  if (LINK->atcomplement) {
    LINK->hold = LINK->start;
    LINK->start = LINK->stop;
    LINK->stop = LINK->hold;
  }



  if (getstart || getstop)
    addfeature(featureisexon, getstart, getstop, LINK);

}

Local Void readparameters(LINK)
struct LOC_themain *LINK;
{
  if (*LINK->exonp->name != '\0') {
    if (LINK->exonp->f != NULL)
      LINK->exonp->f = freopen(LINK->exonp->name, "r", LINK->exonp->f);
    else
      LINK->exonp->f = fopen(LINK->exonp->name, "r");
  } else
    rewind(LINK->exonp->f);
  if (LINK->exonp->f == NULL)
    _EscIO2(FileNotFound, LINK->exonp->name);
  RESETBUF(LINK->exonp->f, Char);


  if (P_peek(LINK->exonp->f) == '9' || P_peek(LINK->exonp->f) == '8' ||
      P_peek(LINK->exonp->f) == '7' || P_peek(LINK->exonp->f) == '6' ||
      P_peek(LINK->exonp->f) == '5' || P_peek(LINK->exonp->f) == '4' ||
      P_peek(LINK->exonp->f) == '3' || P_peek(LINK->exonp->f) == '2' ||
      P_peek(LINK->exonp->f) == '1' || P_peek(LINK->exonp->f) == '0') {
    fscanf(LINK->exonp->f, "%lg%*[^\n]", &LINK->parameterversion);
    getc(LINK->exonp->f);
  } else {
    printf("No version parameter, upgrading exonp.\n");

    if (*LINK->holdfile.name != '\0') {
      if (LINK->holdfile.f != NULL)
	LINK->holdfile.f = freopen(LINK->holdfile.name, "w", LINK->holdfile.f);
      else
	LINK->holdfile.f = fopen(LINK->holdfile.name, "w");
    } else {
      if (LINK->holdfile.f != NULL)
	rewind(LINK->holdfile.f);
      else
	LINK->holdfile.f = tmpfile();
    }
    if (LINK->holdfile.f == NULL)
      _EscIO2(FileNotFound, LINK->holdfile.name);
    SETUPBUF(LINK->holdfile.f, Char);
    fprintf(LINK->holdfile.f,
      "%4.2f      version of exon that this parameter file is designed for.\n",
      version);

    if (copylines(LINK->exonp, &LINK->holdfile, 8L) != 8) {
      printf("Cannot copy 8 lines of exonp:\n");
      printf("Build it yourself.\n");
      halt();
    }
    fprintf(LINK->holdfile.f, "CDNA      c/C: CDNA, m/R/r: mRNA\n");

    while (!BUFEOF(LINK->exonp->f))
      copyaline(LINK->exonp, &LINK->holdfile);

    if (*LINK->holdfile.name != '\0') {
      if (LINK->holdfile.f != NULL)
	LINK->holdfile.f = freopen(LINK->holdfile.name, "r", LINK->holdfile.f);
      else
	LINK->holdfile.f = fopen(LINK->holdfile.name, "r");
    } else
      rewind(LINK->holdfile.f);
    if (LINK->holdfile.f == NULL)
      _EscIO2(FileNotFound, LINK->holdfile.name);
    RESETBUF(LINK->holdfile.f, Char);
    if (*LINK->exonp->name != '\0') {
      if (LINK->exonp->f != NULL)
	LINK->exonp->f = freopen(LINK->exonp->name, "w", LINK->exonp->f);
      else
	LINK->exonp->f = fopen(LINK->exonp->name, "w");
    } else {
      if (LINK->exonp->f != NULL)
	rewind(LINK->exonp->f);
      else
	LINK->exonp->f = tmpfile();
    }
    if (LINK->exonp->f == NULL)
      _EscIO2(FileNotFound, LINK->exonp->name);
    SETUPBUF(LINK->exonp->f, Char);
    while (!BUFEOF(LINK->holdfile.f))
      copyaline(&LINK->holdfile, LINK->exonp);
    if (*LINK->exonp->name != '\0') {
      if (LINK->exonp->f != NULL)
	LINK->exonp->f = freopen(LINK->exonp->name, "r", LINK->exonp->f);
      else
	LINK->exonp->f = fopen(LINK->exonp->name, "r");
    } else
      rewind(LINK->exonp->f);
    if (LINK->exonp->f == NULL)
      _EscIO2(FileNotFound, LINK->exonp->name);
    RESETBUF(LINK->exonp->f, Char);
    fscanf(LINK->exonp->f, "%lg%*[^\n]", &LINK->parameterversion);
    getc(LINK->exonp->f);
  }
  fscanf(LINK->exonp->f, "%c%*[^\n]", &LINK->c);
  getc(LINK->exonp->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'n')
    LINK->noends = true;
  else
    LINK->noends = false;
  fscanf(LINK->exonp->f, "%c%*[^\n]", &LINK->c);
  getc(LINK->exonp->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'd') {
    LINK->debug = true;
    LINK->verbose = true;
  } else {
    if (LINK->c == 'v') {
      LINK->verbose = true;
      LINK->debug = false;
    } else {
      LINK->verbose = false;
      LINK->debug = false;
    }
  }

  fscanf(LINK->exonp->f, "%ld%ld%*[^\n]", &LINK->theDfromrange,
	 &LINK->theDtorange);
  getc(LINK->exonp->f);

  fscanf(LINK->exonp->f, "%ld%ld%*[^\n]", &LINK->theAfromrange,
	 &LINK->theAtorange);
  getc(LINK->exonp->f);

  LINK->c = getc(LINK->exonp->f);
  if (LINK->c == '\n')
    LINK->c = ' ';
  if (LINK->c == 'e')
    LINK->getexons = true;
  else
    LINK->getexons = false;
  LINK->c = getc(LINK->exonp->f);
  if (LINK->c == '\n')
    LINK->c = ' ';
  if (LINK->c == 'i')
    LINK->getintrons = true;
  else
    LINK->getintrons = false;
  fscanf(LINK->exonp->f, "%*[^\n]");

  getc(LINK->exonp->f);
  fscanf(LINK->exonp->f, "%c%*[^\n]", &LINK->c);
  getc(LINK->exonp->f);
  if (LINK->c == '\n')
    LINK->c = ' ';
  if (LINK->c == 'a')
    LINK->allowalternative = true;
  else
    LINK->allowalternative = false;
  LINK->c = getc(LINK->exonp->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'P')
    LINK->killputative = true;
  else
    LINK->killputative = false;
  LINK->c = getc(LINK->exonp->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'N')
    LINK->killnotexperimental = true;
  else
    LINK->killnotexperimental = false;
  LINK->c = getc(LINK->exonp->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'G')
    LINK->killgeneprediction = true;
  else
    LINK->killgeneprediction = false;
  LINK->c = getc(LINK->exonp->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'U')
    LINK->killunpublished = true;
  else
    LINK->killunpublished = false;
  LINK->c = getc(LINK->exonp->f);
  if (LINK->c == '\n') {

    LINK->c = ' ';
  }
  if (LINK->c == 'S')
    LINK->killpseudo = true;
  else
    LINK->killpseudo = false;
  fscanf(LINK->exonp->f, "%*[^\n]");


  getc(LINK->exonp->f);
  if ((!P_eoln(LINK->exonp->f)) & (!BUFEOF(LINK->exonp->f))) {
    LINK->c = getc(LINK->exonp->f);
    if (LINK->c == '\n')
      LINK->c = ' ';
    if (LINK->c == 'n')
      LINK->usenotes = true;
    else
      LINK->usenotes = false;
  } else
    LINK->usenotes = false;
  fscanf(LINK->exonp->f, "%*[^\n]");

  getc(LINK->exonp->f);
  fscanf(LINK->exonp->f, "%c%*[^\n]", &LINK->thefeature);
  getc(LINK->exonp->f);

  if (LINK->thefeature == '\n')
    LINK->thefeature = ' ';
  if (LINK->thefeature == 'R' || LINK->thefeature == 'r' ||
      LINK->thefeature == 'm')
    LINK->thefeature = 'r';


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

  fprintf(V.f->f, "* the DONOR    from range: %3ld\n", LINK->theDfromrange);
  fprintf(V.f->f, "* the DONOR      to range: %3ld\n", LINK->theDtorange);
  fprintf(V.f->f, "* the ACCEPTOR from range: %3ld\n", LINK->theAfromrange);
  fprintf(V.f->f, "* the ACCEPTOR   to range: %3ld\n", LINK->theAtorange);

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

  no(!LINK->usenotes, &V);
  fprintf(V.f->f, "/notes             included in names.\n");

  fprintf(V.f->f, "* %c                     feature: ", LINK->thefeature);
  if (LINK->thefeature == 'r')
    fprintf(V.f->f, "mRNA\n");
  else
    fprintf(V.f->f, "CDNA\n");

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
  if (killt)
    *killentry = true;
  if (LINK->verbose) {
    if (n.length > 0) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &n);
    } else
      printf("The next entry");
    printf(" will be");
    if (killt)
      printf(" KILLED");
    else
      printf(" KEPT");
    printf(".  It contains: \"");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &t->seek);
    printf("\".\n");
  }
  resettrigger(t);
}

Local Void doFEA(LINK)
struct LOC_themain *LINK;
{
  _TEXT TEMP;

  if (columnposition >= 6 && columnposition <= 20)
    testfortrigger(LINK->c, &LINK->FEA);

  if (LINK->FEA.found) {
    if (LINK->infeatures) {
      skipblanks(LINK->db);
      LINK->FEAcount++;
      if (LINK->debug)
	putchar('\n');
      if (LINK->debug)
	printf("CDS %ld ======================\n", LINK->FEAcount);

      while (!P_eoln(LINK->db->f)) {
	nextcharacter(LINK->db, &LINK->cprevious, &LINK->c, LINK);

	if (LINK->verbose)
	  putchar(LINK->c);
	testfortrigger(LINK->c, &LINK->complement);
	if (LINK->complement.found)
	  LINK->atcomplement = true;

	if (LINK->atcomplement) {
	  testfortrigger(LINK->c, &LINK->closeparen);
	  if (LINK->closeparen.found)
	    LINK->atcomplement = false;
	}

	testfortrigger(LINK->c, &LINK->join);
	if (LINK->join.found) {
	  dojoin(LINK);
	  continue;
	}
	if (!P_inset(LINK->c, LINK->numbers))
	  continue;
	skipblanks(LINK->db);
	readaninteger(LINK->db, LINK->c, &LINK->start, LINK);

	LINK->c = getc(LINK->db->f);
	if (LINK->c == '\n')
	  LINK->c = ' ';
	if (LINK->c != '.') {
	  printf("error in reading CDS or mRNA\"%c\" found instead of \".\"\n",
		 LINK->c);
	  printf("ord(c) is %12d\n", LINK->c);
	  halt();
	}

	LINK->c = getc(LINK->db->f);
	if (LINK->c == '\n')
	  LINK->c = ' ';
	if (LINK->c != '.') {
	  printf("error in reading CDS or mRNA\"%c\" found instead of \".\"\n",
		 LINK->c);
	  halt();
	}

	LINK->c = getc(LINK->db->f);
	if (LINK->c == '\n')
	  LINK->c = ' ';
	if (!P_inset(LINK->c, LINK->numbers)) {
	  if (LINK->c == '>') {
	    LINK->c = getc(LINK->db->f);
	    if (LINK->c == '\n')
	      LINK->c = ' ';
	    if (P_inset(LINK->c, LINK->numbers))
	      readaninteger(LINK->db, LINK->c, &LINK->stop, LINK);
	    else
	      LINK->stop = LONG_MAX;
	  } else {
	    printf("error in reading CDS or mRNA\"%c\" is not a number\n",
		   LINK->c);
	    halt();
	  }
	} else
	  readaninteger(LINK->db, LINK->c, &LINK->stop, LINK);
	/*


*/
	if (LINK->atcomplement) {
	  LINK->hold = LINK->start;
	  LINK->start = LINK->stop;
	  LINK->stop = LINK->hold;
	}

	addfeature(true, true, true, LINK);
      }

      resettrigger(&LINK->FEA);
    }

  }



  if (!(LINK->infeatures && LINK->getexons))
    return;
  testfortrigger(LINK->c, &LINK->genename);


  if (LINK->genename.found) {
    if (LINK->feastored > 0) {
      if (LINK->fealist->a[LINK->featarget-1].genenamefound != true &&
	  LINK->featurecount == LINK->fealist->a[LINK->featarget-1].featurenumber)
	LINK->capturegenename = true;

      else
	LINK->capturegenename = false;
    }

    else {
      LINK->capturegenename = true;
      LINK->featarget = 1;
      LINK->feastored = 1;
    }

    if (LINK->capturegenename) {
      if (LINK->thefeature == 'r')
	LINK->fealist->a[LINK->featarget-1].genenamefound = false;
      else {
	LINK->fealist->a[LINK->featarget-1].genenamefound = true;

	grabquote(LINK->db,
		  &LINK->fealist->a[LINK->featarget-1].genenamestring);
	if (LINK->verbose) {
	  printf("    gene: ");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writestring(&TEMP,
		      &LINK->fealist->a[LINK->featarget-1].genenamestring);
	  putchar('\n');
	}
      }
    }
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
    resettrigger(&LINK->genename);
  }
  /*








*/




  testfortrigger(LINK->c, &LINK->genenumber);
  if (!LINK->genenumber.found)
    return;

  if (LINK->fealist->a[LINK->featarget-1].genenumberfound != true &&
      LINK->featurecount == LINK->fealist->a[LINK->featarget-1].featurenumber) {
    /*


*/
    LINK->fealist->a[LINK->featarget-1].genenumberfound = true;

    grabtoken(LINK->db, &LINK->fealist->a[LINK->featarget-1].genenumberstring);
    if (LINK->verbose) {
      printf("    ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP,
		  &LINK->fealist->a[LINK->featarget-1].genenumberstring);
      putchar('\n');
    }
    /*

*/
  }
  resettrigger(&LINK->genenumber);
}



Static Void themain(exonp_, db_, lengths_, dinst_, ainst_, einst_,
		    exonfeatures_)
_TEXT *exonp_, *db_, *lengths_, *dinst_, *ainst_, *einst_, *exonfeatures_;
{
  struct LOC_themain V;
  trigger accession, comma;
  boolean done;
  trigger features, entryend, exon, intron, geneprediction, locus, putative,
	  notebegin, noteend;
  stringDelila noteholder;
  trigger notexperimental, origin, pseudo, unpublished;
  Char controlH = '\b';
  long controlindexH;
  long controlHwid = 6;
  _TEXT TEMP;
  stringDelila *WITH;

  V.exonp = exonp_;
  V.db = db_;
  V.lengths = lengths_;
  V.dinst = dinst_;
  V.ainst = ainst_;
  V.einst = einst_;
  V.exonfeatures = exonfeatures_;
  V.holdfile.f = NULL;
  *V.holdfile.name = '\0';
  printf("exon %4.2f\n", version);

  V.fealist = (exonset *)Malloc(sizeof(exonset));
  /*
*/

  readparameters(&V);

  if (*V.lengths->name != '\0') {
    if (V.lengths->f != NULL)
      V.lengths->f = freopen(V.lengths->name, "w", V.lengths->f);
    else
      V.lengths->f = fopen(V.lengths->name, "w");
  } else {
    if (V.lengths->f != NULL)
      rewind(V.lengths->f);
    else
      V.lengths->f = tmpfile();
  }
  if (V.lengths->f == NULL)
    _EscIO2(FileNotFound, V.lengths->name);
  SETUPBUF(V.lengths->f, Char);
  fprintf(V.lengths->f, "* exon %4.2f\n", version);
  writeparameters(V.lengths, &V);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeparameters(&TEMP, &V);


  if (V.debug)
    printf("DEBUGGING\n");



  filltrigger(&locus, "LOCUS               ");
  filltrigger(&V.organism, "ORGANISM            ");
  /*

*/
  filltrigger(&accession, "VERSION             ");
  filltrigger(&features, "FEATURES            ");

  if (V.thefeature == 'r')
    filltrigger(&V.FEA, "mRNA                ");
  else
    filltrigger(&V.FEA, "CDS                 ");

  filltrigger(&V.genename, "/gene=\"             ");
  filltrigger(&V.genenumber, "/number=            ");
  filltrigger(&exon, "exon                ");
  filltrigger(&intron, "intron              ");
  filltrigger(&V.join, "join(               ");
  filltrigger(&V.closeparen, ")                   ");
  filltrigger(&comma, ",                   ");
  filltrigger(&V.complement, "complement(         ");
  filltrigger(&putative, "putative            ");
  filltrigger(&notexperimental, "not_experimental    ");
  filltrigger(&geneprediction, "gene prediction     ");
  filltrigger(&unpublished, "Unpublished         ");
  filltrigger(&pseudo, "pseudo              ");
  filltrigger(&entryend, "//                  ");
  filltrigger(&notebegin, "/note=\"             ");
  filltrigger(&noteend, "\"                   ");
  filltrigger(&origin, "ORIGIN              ");

  fillstring(&V.unknownstring, "unknown             ");

  resettrigger(&V.FEA);
  resettrigger(&exon);
  resettrigger(&intron);
  resettrigger(&locus);
  resettrigger(&accession);
  resettrigger(&features);
  resettrigger(&V.genename);
  resettrigger(&V.genenumber);
  resettrigger(&V.join);
  resettrigger(&V.closeparen);
  resettrigger(&V.complement);
  resettrigger(&comma);
  resettrigger(&putative);
  resettrigger(&notexperimental);
  resettrigger(&geneprediction);
  resettrigger(&unpublished);
  resettrigger(&pseudo);
  resettrigger(&entryend);
  resettrigger(&notebegin);
  resettrigger(&noteend);
  resettrigger(&origin);

  V.locuscount = 0;
  V.FEAcount = 0;
  V.totalentrycount = 0;
  V.totalfeastored = 0;
  V.feastored = 0;
  V.infeatures = false;
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

  fprintf(V.lengths->f, "* columns\n");

  fprintf(V.lengths->f,
    "* length   start    stop     dir   exon#    FEA#  locus# accession\n");

  startinst(V.dinst, V.db, 'd', &V);
  startinst(V.ainst, V.db, 'a', &V);
  startinst(V.einst, V.db, 'e', &V);
  if (*V.exonfeatures->name != '\0') {
    if (V.exonfeatures->f != NULL)
      V.exonfeatures->f = freopen(V.exonfeatures->name, "w", V.exonfeatures->f);
    else
      V.exonfeatures->f = fopen(V.exonfeatures->name, "w");
  } else {
    if (V.exonfeatures->f != NULL)
      rewind(V.exonfeatures->f);
    else
      V.exonfeatures->f = tmpfile();
  }
  if (V.exonfeatures->f == NULL)
    _EscIO2(FileNotFound, V.exonfeatures->name);
  SETUPBUF(V.exonfeatures->f, Char);
  fprintf(V.exonfeatures->f, "* exon: exonfeatures %4.2f\n", version);

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


  if (!V.verbose)
    printf("the changing number is the number of features recorded\n");
  V.c = ' ';
  while (!BUFEOF(V.db->f)) {
    columnposition = 0;
    while (!P_eoln(V.db->f)) {
      if (!V.verbose) {
	if (V.featurecount > 0) {
	  printf("%*ld", (int)controlHwid, V.featurecount);
	  controlHwid = (long)(log((double)V.featurecount) / log(10.0) + 2);
	  for (controlindexH = 1; controlindexH <= controlHwid; controlindexH++)
	    putchar(controlH);
	}
      }
      V.cprevious = V.c;
      nextcharacter(V.db, &V.cprevious, &V.c, &V);


      if (V.infeatures && columnposition == 6 && V.c != ' ') {
	/*
*/
	V.featurecount++;
	resettrigger(&V.genename);
	resettrigger(&V.genenumber);
      }


      testfortrigger(V.c, &locus);
      if (locus.found) {
	V.killentry = false;
	V.locuscount++;
	V.FEAcount = 0;
	clearstring(&V.accname);
	V.infeatures = false;
	V.atcomplement = false;
	V.stop = -LONG_MAX;
	V.start = -LONG_MAX;
	V.featurecount = 0;
	if (V.debug)
	  putchar('\n');
	if (V.debug)
	  printf("LOCUS %ld ======================\n", V.locuscount);
      }


      testfortrigger(V.c, &notebegin);
      if (notebegin.found) {
	/*
*/
	if (V.debug)
	  printf("skipping note:%c", V.c);
	clearstring(&noteholder);

	if (V.feastored > 0) {
	  if (V.featurecount == V.fealist->a[V.feastored-1].featurenumber)
	    V.fealist->a[V.feastored-1].notenumber++;
	}

	while (!noteend.found) {
	  nextcharacter(V.db, &V.cprevious, &V.c, &V);

	  /*
*/

	  if (V.feastored > 0) {
	    if (V.fealist->a[V.feastored-1].notestring.length < maxstring &&
		V.c != '"' &&
		V.featurecount == V.fealist->a[V.feastored-1].featurenumber) {
	      WITH = &V.fealist->a[V.feastored-1].notestring;
	      WITH->length++;
	      WITH->letters[WITH->length - 1] = V.c;
	      /*


*/
	    } else {




	      if (noteholder.length < maxstring) {
		noteholder.length++;
		noteholder.letters[noteholder.length - 1] = V.c;
		/*




*/
	      }
	    }
	  }



	  if (V.debug)
	    putchar(V.c);
	  if (V.debug) {
	    if (P_eoln(V.db->f))
	      putchar('\n');
	  }
	  testfortrigger(V.c, &noteend);
	}
	if (V.debug)
	  putchar('\n');
	resettrigger(&notebegin);
	resettrigger(&noteend);
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

      testfortrigger(V.c, &features);
      if (features.found)
	V.infeatures = true;



      /*





*/
      doFEA(&V);

      if (V.infeatures) {
	if (V.getexons) {
	  testfortrigger(V.c, &exon);
	  if (exon.found) {
	    dofeature(true, &V);
	    resettrigger(&exon);
	  }
	}
      }

      if (V.infeatures) {
	if (V.getintrons) {
	  testfortrigger(V.c, &intron);
	  if (intron.found) {
	    dofeature(false, &V);
	    resettrigger(&intron);
	  }
	}
      }


      testfortrigger(V.c, &origin);

      if (!origin.found)
	continue;

      printf("Skipping sequence ...\n");
      fscanf(V.db->f, "%*[^\n]");
      getc(V.db->f);
      done = false;
      while (!done) {
	V.c = getc(V.db->f);
	if (V.c == '\n')
	  V.c = ' ';
	if (V.c != '/')
	  continue;

	V.c = getc(V.db->f);
	if (V.c == '\n')
	  V.c = ' ';
	if (V.c == '/')
	  done = true;
      }
      printf("... done skipping sequence\n");
    }

    nextline(&V);
    if (V.debug)
      putchar('\n');
  }

  printexons(&V);

  printf("%ld entry(s) have features\n", V.totalentrycount);
  printf("%ld features processed\n", V.totalfeastored);
  printf("%ld features\n", V.featurecount);
  fprintf(V.lengths->f, "* %ld entry(s) have features\n", V.totalentrycount);
  fprintf(V.lengths->f, "* %ld features processed\n", V.totalfeastored);
  fprintf(V.lengths->f, "* %ld features\n", V.featurecount);
  if (V.holdfile.f != NULL)
    fclose(V.holdfile.f);
}

#undef colwid


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  exonfeatures.f = NULL;
  strcpy(exonfeatures.name, "exonfeatures");
  einst.f = NULL;
  strcpy(einst.name, "einst");
  ainst.f = NULL;
  strcpy(ainst.name, "ainst");
  dinst.f = NULL;
  strcpy(dinst.name, "dinst");
  lengths.f = NULL;
  strcpy(lengths.name, "lengths");
  db.f = NULL;
  strcpy(db.name, "db");
  exonp.f = NULL;
  strcpy(exonp.name, "exonp");
  themain(&exonp, &db, &lengths, &dinst, &ainst, &einst, &exonfeatures);
_L1:
  if (exonp.f != NULL)
    fclose(exonp.f);
  if (db.f != NULL)
    fclose(db.f);
  if (lengths.f != NULL)
    fclose(lengths.f);
  if (dinst.f != NULL)
    fclose(dinst.f);
  if (ainst.f != NULL)
    fclose(ainst.f);
  if (einst.f != NULL)
    fclose(einst.f);
  if (exonfeatures.f != NULL)
    fclose(exonfeatures.f);
  exit(EXIT_SUCCESS);
}



/* End. */
