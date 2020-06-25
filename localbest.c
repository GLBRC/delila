/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "localbest.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*









*/



#define version         1.22
/*














*/
#define updateversion   1.01
#define versionupperbound  9.00



/*




























































*/





#define dnamax          3000
#define namelength      100
#define linelength      80




#define minribl         (-2000)
#define maxribl         2000
#define defnegativeinfinity  (-1000)
/*
*/



#define maxstring       2000




#define defbasesperline  30
#define defaastate      1
#define defframeallowed  7
#define defcodelength   3
#define defseqlines     1

#define defpageaction   'c'

#define defpagelength   55

#define defhopi         "HOPI"
#define defnkfc         "NKFC"
#define defndaf         "NDAF"
#define defsof          "SOF"

#define prime           '\''

#define shiftright      3
/*
*/
#define tabcharacter    9
#define maxdatacontrol  4
#define maxsof          3

/*
*/


#define defaultllx      10.08
#define defaultlly      8.91
#define defaulturx      588.06
#define defaultury      779.85

/*





*/


#define pwid            8
#define pdec            5
#define pdecolor        4
/*
*/

#define defcharacterratio  0.6
/*


*/

#define tolerateratio   10000
/*

*/




#define infofield       10
#define infodecim       6

#define nfield          8


#define KeepDuplicates  false


/*
*/





typedef long chset[5];




typedef Char alpha[namelength];

/*
*/

typedef struct name {
  alpha letters;
  char length;
} name;

typedef struct line {
  Char letters[linelength];
  char length;
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

typedef struct header {
  name keynam;
  line *fulnam, *note;
} header;



typedef enum {
  a, c, g, t
} base;



typedef short dnarange;

/* p2c: localbest.p, line 230:
 * Note: Field width for seq assumes enum base has 4 elements [105] */
typedef uchar seq[(dnamax + 3) / 4];

typedef struct dnastring {
  seq part;
  dnarange length;
  struct dnastring *next;
} dnastring;


typedef struct orgkey {
  header hea;
  line *mapunit;
} orgkey;

typedef struct chrkey {
  header hea;
  double mapbeg, mapend;
} chrkey;

typedef struct piekey {
  header hea;
  double mapbeg;
  configuration coocon;
  direction coodir;
  long coobeg, cooend;
  configuration piecon;
  direction piedir;
  long piebeg, pieend;
} piekey;


typedef struct piece {
  piekey key;
  dnastring *dna;
} piece;

typedef struct reference {
  name pienam;
  double mapbeg;
  direction refdir;
  long refbeg, refend;
} reference;

typedef struct genkey {
  header hea;
  reference ref;
} genkey;

typedef struct trakey {
  header hea;
  reference ref;
} trakey;

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




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;



typedef struct waveparam {

  Char extreme;
  double wavelocation, wavebit, waveamplitude, wavelength;
  /*




*/
  double dashon;
  /*
*/
  double dashoff, dashoffset, thickness;
  struct waveparam *next;
} waveparam;



/*


*/
typedef double rblarray[(long)t - (long)a + 1][maxribl - minribl + 1];

typedef struct ribltype {
  stringDelila riblname, *riblheader;
  rblarray data;
  long numbers[(long)t - (long)a + 1][maxribl - minribl + 1];
  long frombase, tobase;
  double mean, stdev, consensus, anticonsensus, averageRi;
  long n;
  Char symmetry;
  waveparam *waves;
  double cmperbase, cmperbit, Ribound, Zbound, Pbound;
} ribltype;

/*
*/





typedef struct petaltype {

  long el;
  Char c_;

  double eh, es, eb;

  double fh, fs, fb;

  double spare;
} petaltype;

typedef struct definetype {
  /*

*/
  stringDelila nametag, background, negparts, posparts;
  double locations[maxstring];
  /*
*/


  long marks;
  double min, max;
  long number;

  ribltype *matrix;
  /*



*/
  double RiboundFeature, ZboundFeature, PboundFeature;
  petaltype petal;
  boolean isgap;
  /*
*/
  boolean istotal;
  /*
*/
  boolean ismulti;
  /*
*/

  struct definetype *next;
} definetype;


typedef struct featurecarrier {
  /*
*/
  struct featuretype *afeature;
  struct featurecarrier *next;
} featurecarrier;

typedef struct featuretype {
  /*






*/
  stringDelila id;
  double coordinate;
  long orientation;
  stringDelila nametag, othertag;

  definetype *definition;


  /*
*/
  double Ri, Z, probability;

  /*

*/
  Char Tparam;
  /*
*/
  double Aparam, Bparam, Cparam, Dparam;
  /*

















*/


  double evencoordinate;
  /*

*/


  boolean unsatisfied;
  /*
*/
  double fromrange, torange;
  long number, desiredline;
  /*

*/
  /*







*/
  featurecarrier *others, *mothers;

  struct featuretype *next;
} featuretype;


Static _TEXT features, localbestp, bestfeatures;
/*
*/
Static _TEXT lowerfeatures;


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


#define tab             9






Static boolean isblankDelila(c_)
Char c_;
{
  return (c_ == ' ' || c_ == tab);
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





Static Void emptystring(ribbon)
stringDelila *ribbon;
{
  /*

*/
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}


Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  emptystring(ribbon);
  ribbon->next = NULL;
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*



*/
  printf("remove initializestring routine!\n");
  printf("replace it with clearstring routine!\n");
  halt();
  clearstring(ribbon);
  ribbon->next = NULL;
}


#define tabcharacter_   9



Static Void detabstring(s)
stringDelila *s;
{
  /*

*/
  Char c_;
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++) {
    c_ = s->letters[i];
    /*


*/
    if (c_ == tabcharacter_)
      c_ = ' ';
    s->letters[i] = c_;
  }
}

#undef tabcharacter_



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}



Static boolean equalstring(a_, b)
stringDelila a_, b;
{
  /*





*/
  long index;
  boolean equal;

  if (a_.length == b.length) {
    index = 1;
    do {
      equal = (a_.letters[index-1] == b.letters[index-1]);
      index++;
    } while (equal && index <= a_.length);
    return equal;
  } else
    return false;
}



Static Void grabtoken(thefile, thestring)
_TEXT *thefile;
stringDelila *thestring;
{
  Char c_;
  boolean done = false;

  skipblanks(thefile);
  thestring->length = 0;
  while (!done) {
    if (P_eoln(thefile->f)) {
      done = true;
      break;
    }
    c_ = getc(thefile->f);
    if (c_ == '\n')
      c_ = ' ';
    if (c_ == ' ')
      done = true;
    else {
      thestring->length++;
      thestring->letters[thestring->length - 1] = c_;
    }
  }
}



Static Void readstringline(afile, buffer)
_TEXT *afile;
stringDelila *buffer;
{
  long index = 0;

  clearstring(buffer);
  while (!P_eoln(afile->f) && index < maxstring) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
  }
  if (!P_eoln(afile->f)) {
    printf("readstringline: a line exceeds maximum string size (%ld)\n",
	   (long)maxstring);
    halt();
  }
  buffer->length = index;
  buffer->current = 1;
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
}



Static Void readheader(afile, ch, theheader)
_TEXT *afile;
Char ch;
stringDelila **theheader;
{
  /*
*/
  stringDelila *h;

  if (BUFEOF(afile->f)) {
    clearstring(*theheader);
    return;
  }
  if (P_peek(afile->f) != ch) {
    *theheader = NULL;
    return;
  }
  *theheader = (stringDelila *)Malloc(sizeof(stringDelila));
  h = *theheader;
  while (P_peek(afile->f) == '*') {
    readstringline(afile, h);
    if (!BUFEOF(afile->f)) {
      if (P_peek(afile->f) == '*') {
	h->next = (stringDelila *)Malloc(sizeof(stringDelila));
	h = h->next;
      } else
	h->next = NULL;
    }
  }
}


#define tabcharacter_   9



Static Void readquotestring(afile, s)
_TEXT *afile;
stringDelila *s;
{
  /*
*/
  Char c_;
  long i, FORLIM;

  clearstring(s);
  skipblanks(afile);
  c_ = getc(afile->f);
  if (c_ == '\n')
    c_ = ' ';
  if (c_ != '"') {
    printf("quote string expected but \" missing\n");
    printf("character found instead:\"%c\"\n", c_);
    halt();
  }
  if (P_eoln(afile->f)) {
    printf("missing end of quote string \" string\n");
    halt();
  }
  do {
    c_ = getc(afile->f);
    if (c_ == '\n')
      c_ = ' ';
    if (c_ != '"') {
      s->length++;
      s->letters[s->length - 1] = c_;
    }
  } while (!(((c_ == '"') | P_eoln(afile->f)) || s->length == maxstring));
  if (s->length == maxstring) {
    printf("A quote string exceeded %ld characters.\n", (long)maxstring);
    printf("Use a shorter string or increase constant maxstring.\n");
    halt();
  }
  if (c_ != '"') {
    printf("The second \" mark of a string is missing.\n");
    halt();
  }

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++) {
    if (s->letters[i] == ' ')
      s->letters[i] = (Char)tabcharacter_;
  }
}

#undef tabcharacter_



Static Void writequotestring(afile, s)
_TEXT *afile;
stringDelila s;
{
  /*


*/
  detabstring(&s);
  putc('"', afile->f);
  writestring(afile, &s);
  putc('"', afile->f);
}



Static Void readname(afile, id)
_TEXT *afile;
name *id;
{
  /*

*/
  Char c_;
  long i;

  skipblanks(afile);
  id->length = 0;
  do {
    id->length++;
    c_ = getc(afile->f);
    if (c_ == '\n')
      c_ = ' ';
    id->letters[id->length - 1] = c_;
  } while (!(P_eoln(afile->f) || id->length >= namelength || c_ == ' '));
  if (id->letters[id->length - 1] == ' ')
    id->length--;
  if (id->length < namelength) {
    for (i = id->length; i < namelength; i++)
      id->letters[i] = ' ';
  }
}



Static Void copystring(a_, b)
stringDelila a_, *b;
{
  /*
*/
  long l;

  b->length = a_.length;
  for (l = 0; l < a_.length; l++)
    b->letters[l] = a_.letters[l];
  b->current = a_.current;
  b->next = a_.next;
}



Static Void writeNOTABstring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  /*
*/
  stringDelila notabs;

  copystring(*s, &notabs);
  detabstring(&notabs);
  writestring(tofile, &notabs);
}



Static Void writename(afile, s)
_TEXT *afile;
name s;
{
  long i;

  for (i = 0; i < s.length; i++)
    putc(s.letters[i], afile->f);
}



Static Void writeafeature(afile, f)
_TEXT *afile;
featuretype *f;
{
  /*



*/
  featurecarrier *o;

  /*
*/
  if (f == NULL) {
    printf("writeafeature: nil feature pointer!\n");
    halt();
  }
  fprintf(afile->f, "@ ");
  writeNOTABstring(afile, &f->id);
  fprintf(afile->f, " %1.1f", f->coordinate);
  putc(' ', afile->f);
  if (f->orientation > 0)
    fprintf(afile->f, "+%ld", f->orientation);
  else
    fprintf(afile->f, "%ld", f->orientation);
  putc(' ', afile->f);
  writequotestring(afile, f->nametag);
  putc(' ', afile->f);
  writequotestring(afile, f->othertag);

  if (f->definition->matrix != NULL) {
    fprintf(afile->f, " %*.*f", infofield, infodecim, f->Ri);
    fprintf(afile->f, " %*.*f", infofield, infodecim, f->Z);
    fprintf(afile->f, " %*.*f", infofield, infodecim, f->probability);
  } else {
    fprintf(afile->f, " %c", f->Tparam);
    fprintf(afile->f, " %*.*f", infofield, infodecim, f->Aparam);
    fprintf(afile->f, " %*.*f", infofield, infodecim, f->Bparam);
    fprintf(afile->f, " %*.*f", infofield, infodecim, f->Cparam);
    fprintf(afile->f, " %*.*f", infofield, infodecim, f->Dparam);
  }
  if (f->others == NULL)
    return;

  if (f->others->afeature != NULL)
    fprintf(afile->f, " -> ");
  o = f->others;
  while (o != NULL) {
    if (o != f->others)
      fprintf(afile->f, ", ");
    if (o->afeature != NULL) {
      putc('[', afile->f);
      writeNOTABstring(afile, &o->afeature->nametag);
      putc(']', afile->f);
    }
    o = o->next;
  }
}




Static Void writeadefinition(afile, d)
_TEXT *afile;
definetype *d;
{
  long i, FORLIM;

  fprintf(afile->f, "define");
  putc(' ', afile->f);
  writequotestring(afile, d->nametag);
  putc(' ', afile->f);
  writequotestring(afile, d->background);
  putc(' ', afile->f);
  writequotestring(afile, d->negparts);
  putc(' ', afile->f);
  writequotestring(afile, d->posparts);
  FORLIM = d->marks;
  for (i = 0; i < FORLIM; i++)
    fprintf(afile->f, " %1.1f", d->locations[i]);
  putc('\n', afile->f);
  /*





*/
}


#define pwid_           8
#define pdec_           5



Static Void showadefinition(afile, d)
_TEXT *afile;
definetype *d;
{
  long i, FORLIM;
  petaltype *WITH1;

  fprintf(afile->f, "DEFINITION: \n");
  fprintf(afile->f, "nametag: ");
  writequotestring(afile, d->nametag);
  fprintf(afile->f, "\nbackground: ");

  writequotestring(afile, d->background);
  fprintf(afile->f, "\nnegative parts: ");

  writequotestring(afile, d->negparts);
  fprintf(afile->f, "\npositive parts: ");

  writequotestring(afile, d->posparts);
  fprintf(afile->f, "\nlocations: ");

  FORLIM = d->marks;
  for (i = 0; i < FORLIM; i++)
    fprintf(afile->f, " %1.1f", d->locations[i]);
  fprintf(afile->f, "\nmarks: %ld\n", d->marks);

  fprintf(afile->f, "min: %5.1f\n", d->min);
  fprintf(afile->f, "max: %5.1f\n", d->max);

  fprintf(afile->f, "petal: ");
  WITH1 = &d->petal;
  fprintf(afile->f, "%ld", WITH1->el);
  fprintf(afile->f, " %c", WITH1->c_);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->eh);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->es);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->eb);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->fh);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->fs);
  fprintf(afile->f, " %*.*f", pwid_, pdec_, WITH1->fb);
  fprintf(afile->f, " %*.*f\n", pwid_, pdec_, WITH1->spare);

  /*






*/


}

#undef pwid_
#undef pdec_



Static Void showafeature(afile, f)
_TEXT *afile;
featuretype *f;
{
  featurecarrier *o;

  fprintf(afile->f, "FEATURE: \n");
  fprintf(afile->f, "id: ");
  writequotestring(afile, f->id);
  fprintf(afile->f, "\ncoordinate: %1.1f\n", f->coordinate);


  fprintf(afile->f, "orientation: ");
  if (f->orientation > 0)
    fprintf(afile->f, "+%ld\n", f->orientation);
  else
    fprintf(afile->f, "%ld\n", f->orientation);

  fprintf(afile->f, "nametag: ");
  writequotestring(afile, f->nametag);
  fprintf(afile->f, "\nothertag: ");

  writequotestring(afile, f->othertag);
  fprintf(afile->f, "\nunsatisfied: ");

  if (f->unsatisfied)
    fprintf(afile->f, "true\n");
  else
    fprintf(afile->f, "false\n");

  fprintf(afile->f, "fromrange: %5.1f\n", f->fromrange);
  fprintf(afile->f, "  torange: %5.1f\n", f->torange);
  fprintf(afile->f, "desiredline: %5ld\n", f->desiredline);

  if (f->others != NULL) {
    fprintf(afile->f, "This feature points at these OTHERS:\n");
    fprintf(afile->f, "feature -> ");
    o = f->others;
    while (o != NULL) {
      if (o != f->others)
	fprintf(afile->f, ", ");
      if (o->afeature != NULL) {
	putc('[', afile->f);
	writeNOTABstring(afile, &o->afeature->nametag);
	putc(']', afile->f);
      }
      o = o->next;
    }
    putc('\n', afile->f);
    o = f->others;
    while (o != NULL) {
      if (o->afeature != NULL) {
	fprintf(afile->f, "= ");
	writeafeature(afile, o->afeature);
	putc('\n', afile->f);
      }
      o = o->next;
    }
  } else
    fprintf(afile->f, "This feature does not point to any OTHERS.\n");

  if (f->mothers == NULL) {
    fprintf(afile->f, "This feature has no MOTHERS.\n");

    return;
  }
  fprintf(afile->f,
	  "The MOTHERS of this feature (that point to this feature) are:\n");
  o = f->mothers;
  while (o != NULL) {
    if (o->afeature != NULL) {
      fprintf(afile->f, "feature <- \"");
      writeNOTABstring(afile, &o->afeature->nametag);
      fprintf(afile->f, "\"\n");
    }
    o = o->next;
  }
  o = f->mothers;
  while (o != NULL) {
    if (o->afeature != NULL) {
      fprintf(afile->f, "= ");
      writeafeature(afile, o->afeature);
      putc('\n', afile->f);
    }
    o = o->next;
  }
}



Static Void showfeatures(afile, featurelist)
_TEXT *afile;
featuretype *featurelist;
{
  featuretype *f = featurelist;

  while (f != NULL) {
/* p2c: localbest.p: Note: Eliminated unused assignment statement [338] */
    putc('\n', afile->f);
    showafeature(afile, f);
    f = f->next;
  }
}



Static Void locatefeature(aname, coo, featurelist, thefeature)
stringDelila aname;
double coo;
featuretype *featurelist, **thefeature;
{
  /*

*/
  boolean done = false;

  *thefeature = featurelist;
  do {
    if (*thefeature == NULL)
      done = true;
    else {
      /*





*/
      if (equalstring((*thefeature)->nametag, aname)) {
	if ((*thefeature)->coordinate == coo)
	  done = true;
	else {
	  *thefeature = (*thefeature)->next;
	  /*












*/
	}
      } else {
	*thefeature = (*thefeature)->next;

      }
    }
  } while (!done);
}


#define tab             9

#define debugging       false


Local Void IsGapOrIsTotal(d)
definetype **d;
{
  /*
*/
  stringDelila totstring, gapstring;
  long deflocation, gaplocation, totlocation;
  boolean done = false;
  _TEXT TEMP;

  if (debugging) {
    printf("IsGapOrIsTotal test of this definition:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeadefinition(&TEMP, *d);
  }

  clearstring(&totstring);

  totstring.letters[0] = (Char)tab;
  totstring.letters[1] = 't';
  totstring.letters[2] = 'o';
  totstring.letters[3] = 't';
  totstring.letters[4] = 'a';
  totstring.letters[5] = 'l';
  totstring.length = 6;
  clearstring(&gapstring);

  /*

*/


  gapstring.letters[0] = (Char)tab;
  gapstring.letters[1] = 'G';
  gapstring.letters[2] = 'a';
  gapstring.letters[3] = 'p';
  gapstring.length = 4;
  (*d)->isgap = true;
  deflocation = (*d)->nametag.length;
  gaplocation = gapstring.length;
  while (!done) {
    /*




*/
    if ((*d)->nametag.letters[deflocation-1] !=
	gapstring.letters[gaplocation-1]) {
      (*d)->isgap = false;
      done = true;
    }
    deflocation--;
    gaplocation--;
    /*


*/
    if (deflocation < 1)
      done = true;
    if (gaplocation < 1)
      done = true;
  }

  /*




















*/


  (*d)->istotal = true;
  done = false;

  deflocation = (*d)->nametag.length;
  while (!done) {
    /*

*/
    if ((*d)->nametag.letters[deflocation-1] == (Char)tab)
      done = true;
    deflocation--;
    if (deflocation < 1) {
      done = true;
      (*d)->istotal = false;
    }
  }
  /*

*/
  if ((*d)->istotal == true) {
    /*

*/
    totlocation = totstring.length;
    done = false;
    /*


*/
    while (!done) {
      /*




*/
      if ((*d)->nametag.letters[deflocation-1] !=
	  totstring.letters[totlocation-1]) {
	(*d)->istotal = false;
	done = true;
      }
      deflocation--;
      totlocation--;
      /*


*/
      if (deflocation < 1)
	done = true;
      if (totlocation < 1)
	done = true;
    }
  }


  if ((*d)->isgap || (*d)->istotal)
    (*d)->ismulti = true;
  else
    (*d)->ismulti = false;

  if (debugging) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    /*

*/
    showadefinition(&TEMP, *d);
    /*




*/
    printf("IsGapOrIsTotal: end &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
  }


}

#undef tab
#undef debugging




Static Void readadefinition(afile, d)
_TEXT *afile;
definetype **d;
{
  long i;
  double location;
  definetype *WITH;
  _TEXT TEMP;
  long FORLIM;



  skipnonblanks(afile);
  skipblanks(afile);
  WITH = *d;
  /*



*/
  readquotestring(afile, &WITH->nametag);


  readquotestring(afile, &WITH->background);
  if (WITH->background.length < 1 || WITH->background.length > 2) {
    printf("Background string must contain only 1 or 2 characters.\n");
    printf("Instead, it contained: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->background);
    putchar('\n');
    halt();
  }

  readquotestring(afile, &WITH->negparts);
  readquotestring(afile, &WITH->posparts);
  if (WITH->negparts.length != WITH->posparts.length) {
    printf("While reading definition of ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->nametag);
    printf("\nthe negative part ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->negparts);
    printf(" and\n");
    printf("the positive part ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->posparts);
    printf(" are not the same length.\n");
    printf("(%ld<>%ld)\n", WITH->negparts.length, WITH->posparts.length);
    halt();
  }

  WITH->marks = WITH->negparts.length;

  FORLIM = WITH->marks;
  for (i = 0; i < FORLIM; i++)
    fscanf(afile->f, "%lg", &WITH->locations[i]);


  WITH->matrix = NULL;
  WITH->RiboundFeature = -LONG_MAX;
  WITH->ZboundFeature = LONG_MAX;
  WITH->PboundFeature = 0.0;


  WITH->ismulti = false;

  WITH->next = NULL;




  WITH->min = LONG_MAX;
  WITH->max = -LONG_MAX;
  FORLIM = WITH->marks;
  for (i = 0; i < FORLIM; i++) {
    location = WITH->locations[i];
    if (location < WITH->min)
      WITH->min = location;
    if (location > WITH->max)
      WITH->max = location;
  }
  if (WITH->min == LONG_MAX) {
    printf("readadefinition: program error at min\n");
    halt();
  }
  if (WITH->max == -LONG_MAX) {
    printf("readadefinition: program error at max\n");
    halt();
  }



  IsGapOrIsTotal(d);

}



Static Void locatedefinition(aname, definitionlist, thedefinition)
stringDelila aname;
definetype *definitionlist, **thedefinition;
{
  /*
*/
  boolean done = false;

  *thedefinition = definitionlist;
  do {
    if (*thedefinition == NULL)
      done = true;
    else {
      /*



*/
      if (equalstring((*thedefinition)->nametag, aname))
	done = true;
      else {
	*thedefinition = (*thedefinition)->next;

      }
    }
  } while (!done);
}



Static Void readawaveparameter(afile, wp)
_TEXT *afile;
waveparam *wp;
{
  /*

*/

  /*










*/
  fscanf(afile->f, "%c%*[^\n]", &wp->extreme);
  getc(afile->f);
  if (wp->extreme == '\n')
    wp->extreme = ' ';
  fscanf(afile->f, "%lg%*[^\n]", &wp->wavelocation);
  getc(afile->f);
  fscanf(afile->f, "%lg%*[^\n]", &wp->wavebit);
  getc(afile->f);
  fscanf(afile->f, "%lg%*[^\n]", &wp->waveamplitude);
  getc(afile->f);
  fscanf(afile->f, "%lg%*[^\n]", &wp->wavelength);
  getc(afile->f);
  if (wp->wavelength <= 0.0) {
    printf("wave parameters: wavelength must be positive\n");
    halt();
  }
  /*


*/
  if (BUFEOF(afile->f)) {
    wp->dashon = 0.0;
    wp->dashoff = 0.0;
    wp->dashoffset = 0.0;
    wp->thickness = 0.0;
    return;
  }
  /*
*/
  if (P_peek(afile->f) != 'd') {
    fscanf(afile->f, "%lg%*[^\n]", &wp->dashon);
    getc(afile->f);
    wp->dashoff = wp->dashon;
    wp->dashoffset = 0.0;
  } else {
    getc(afile->f);
    /*




*/
    fscanf(afile->f, "%lg%lg%lg%*[^\n]", &wp->dashon, &wp->dashoff,
	   &wp->dashoffset);
    getc(afile->f);

    /*






























*/

  }
  if (BUFEOF(afile->f))
    wp->thickness = 0.0;
  else {
    fscanf(afile->f, "%lg%*[^\n]", &wp->thickness);
    getc(afile->f);
  }
}


/* Local variables for readwaveparameters: */
struct LOC_readwaveparameters {
  _TEXT *afile;
  boolean done;
} ;

Local Void waystoend(LINK)
struct LOC_readwaveparameters *LINK;
{
  boolean clear = false;

  if (BUFEOF(LINK->afile->f))
    LINK->done = true;
  if (!LINK->done) {
    while (!clear && !LINK->done) {
      if (BUFEOF(LINK->afile->f)) {
	LINK->done = true;
	clear = true;
      } else if (P_peek(LINK->afile->f) == '*') {
	fscanf(LINK->afile->f, "%*[^\n]");
	getc(LINK->afile->f);
      } else
	clear = true;
    }
  }
  if (!LINK->done) {
    if (P_eoln(LINK->afile->f))
      LINK->done = true;
  }
  if (LINK->done)
    return;
  if (P_peek(LINK->afile->f) != '.')
    return;
  fscanf(LINK->afile->f, "%*[^\n]");
  getc(LINK->afile->f);
  LINK->done = true;
}


Static Void readwaveparameters(afile_, w)
_TEXT *afile_;
waveparam **w;
{
  /*

*/
  /*

*/
  struct LOC_readwaveparameters V;
  boolean morethanone = false;
  waveparam *p;

  V.afile = afile_;
  V.done = false;
  *w = NULL;
  waystoend(&V);
  if (V.done) {
    *w = NULL;
    return;
  }
  *w = (waveparam *)Malloc(sizeof(waveparam));
  p = *w;
  while (!V.done) {
    waystoend(&V);
    if (V.done) {
      p->next = NULL;
      continue;
    }
    if (P_peek(V.afile->f) == '*') {
      fscanf(V.afile->f, "%*[^\n]");
      getc(V.afile->f);
      continue;
    }
    if (morethanone) {
      p->next = (waveparam *)Malloc(sizeof(waveparam));
      p = p->next;
    }
    /*

*/
    readawaveparameter(V.afile, p);

    morethanone = true;
  }
}


#define clearvalue      5.0


/* Local variables for getriblmatrix: */
struct LOC_getriblmatrix {
  _TEXT *afile;
} ;

Local Void testforend(LINK)
struct LOC_getriblmatrix *LINK;
{
  if (BUFEOF(LINK->afile->f)) {
    printf("part or all of the ribl matrix is missing\n");
    halt();
  }
}

Local Void skip(LINK)
struct LOC_getriblmatrix *LINK;
{
  testforend(LINK);
  while (P_peek(LINK->afile->f) == '*') {
    fscanf(LINK->afile->f, "%*[^\n]");
    getc(LINK->afile->f);
    testforend(LINK);
  }
}



Static Void getriblmatrix(afile_, matrix)
_TEXT *afile_;
ribltype **matrix;
{
  /*

*/
  struct LOC_getriblmatrix V;
  /*
*/
  base b;
  long l, thel;

  stringDelila teststring, riblstart;
  ribltype *WITH;
  _TEXT TEMP;
  long FORLIM;

  V.afile = afile_;
  *matrix = (ribltype *)Malloc(sizeof(ribltype));
  WITH = *matrix;


  grabtoken(V.afile, &riblstart);
  teststring.letters[0] = 'r';
  teststring.letters[1] = 'i';
  teststring.letters[2] = 'b';
  teststring.letters[3] = 'l';
  teststring.length = 4;

  if (!equalstring(riblstart, teststring)) {
    printf("The ribl matrix must begin with 'ribl \"name\"'\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, riblstart);
    printf(" seen instead\n");
    halt();
  }

  skipblanks(V.afile);
  readquotestring(V.afile, &(*matrix)->riblname);
  fscanf(V.afile->f, "%*[^\n]");

  getc(V.afile->f);
  readheader(V.afile, '*', &WITH->riblheader);

  fscanf(V.afile->f, "%ld%ld%*[^\n]", &WITH->frombase, &WITH->tobase);
  getc(V.afile->f);

  if (WITH->frombase < minribl) {
    printf("Matrix is too large for program.\n");
    printf("frombase (=%ld)\n", WITH->frombase);
    printf("minribl (=%ld)\n", (long)minribl);
    printf("Decrease minribl or increase frombase\n");
    halt();
  }

  if (WITH->tobase > maxribl) {
    printf("Matrix is too large for program.\n");
    printf("tobase (=%ld)\n", WITH->tobase);
    printf("maxribl (=%ld)\n", (long)maxribl);
    printf("Increase maxribl or decrease tobase\n");
    halt();
  }


  FORLIM = WITH->frombase;
  for (l = minribl; l < FORLIM; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      (*matrix)->data[(long)b - (long)a][l - minribl] = clearvalue;
  }
  for (l = WITH->tobase + 1; l <= maxribl; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      (*matrix)->data[(long)b - (long)a][l - minribl] = clearvalue;
  }

  FORLIM = WITH->tobase;
  for (l = WITH->frombase; l <= FORLIM; l++) {
    testforend(&V);
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fscanf(V.afile->f, "%lg", &(*matrix)->data[(long)b - (long)a]
	     [l - minribl]);
    fscanf(V.afile->f, "%ld", &thel);
    if (thel != l) {
      printf("positions in ribl do not match from-to range\n");
      halt();
    }
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fscanf(V.afile->f, "%ld", &(*matrix)->numbers[(long)b - (long)a]
	     [l - minribl]);
    fscanf(V.afile->f, "%*[^\n]");
    getc(V.afile->f);
  }

  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->mean);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->stdev);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->consensus);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->anticonsensus);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->averageRi);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%ld%*[^\n]", &WITH->n);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%c%*[^\n]", &WITH->symmetry);
  getc(V.afile->f);
  if (WITH->symmetry == '\n')
    WITH->symmetry = ' ';
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->Ribound);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->Zbound);
  getc(V.afile->f);
  skip(&V);
  fscanf(V.afile->f, "%lg%*[^\n]", &WITH->Pbound);
  getc(V.afile->f);
  /*



*/
  readwaveparameters(V.afile, &WITH->waves);
}

#undef clearvalue


/* Local variables for featureinlist: */
struct LOC_featureinlist {
  featuretype *newfeature, *f;
  boolean found;
} ;

Local Void checkorpos(LINK)
struct LOC_featureinlist *LINK;
{
  /*

*/
  LINK->found = (LINK->newfeature->orientation == LINK->f->orientation &&
		 LINK->newfeature->coordinate == LINK->f->coordinate);
}




Static boolean featureinlist(newfeature_, featurelist)
featuretype *newfeature_, *featurelist;
{
  struct LOC_featureinlist V;
  /*


*/
  boolean done = false;

  V.newfeature = newfeature_;
  /*







*/

  V.found = false;
  V.f = featurelist;
  /*



*/
  while (!done) {
    /*



*/

    if (equalstring(V.newfeature->id, V.f->id) &
	equalstring(V.newfeature->nametag, V.f->nametag)) {
      if (V.f->definition == NULL)
	checkorpos(&V);

      else if (V.f->definition->matrix == NULL)
	checkorpos(&V);
      else {
	V.found = (V.newfeature->Ri == V.f->Ri &&
		   ((V.f->definition->matrix->symmetry == 'o' &&
		     V.newfeature->coordinate == V.f->coordinate) ||
		    (V.f->definition->matrix->symmetry == 'a' &&
		     V.newfeature->orientation == V.f->orientation &&
		     V.newfeature->coordinate == V.f->coordinate) ||
		    (V.f->definition->matrix->symmetry == 'e' &&
		     1 == fabs(V.newfeature->coordinate - V.f->coordinate) &&
		     V.newfeature->orientation != V.f->orientation) ||
		    (V.f->definition->matrix->symmetry == 'e' &&
		     V.newfeature->coordinate == V.f->coordinate &&
		     V.newfeature->orientation == V.f->orientation)));

	/*
*/
	/*




*/









	/*


*/







      }
    }

    if (V.found) {
      done = true;
      /*









*/
      break;
    }
    if (V.f->next == NULL)
      done = true;
    else
      V.f = V.f->next;
  }

  /*



*/
  return V.found;
}



Static Void plural(thefile, number)
_TEXT *thefile;
long number;
{
  if (number != 1)
    putc('s', thefile->f);
}



Static Void cleardefinition(d)
definetype **d;
{
  long l;
  definetype *WITH;

  WITH = *d;
  clearstring(&WITH->nametag);
  clearstring(&WITH->background);
  clearstring(&WITH->negparts);
  clearstring(&WITH->posparts);
  for (l = 0; l < maxstring; l++)
    WITH->locations[l] = 0.0;
  WITH->marks = 0;
  WITH->min = 0.0;
  WITH->max = 0.0;
  WITH->number = 0;

  WITH->matrix = NULL;
  WITH->RiboundFeature = 0.0;
  WITH->ZboundFeature = 0.0;
  WITH->PboundFeature = 0.0;
  WITH->petal.el = 0;
  WITH->isgap = false;
  WITH->istotal = false;
  WITH->ismulti = false;
  WITH->next = NULL;

}



Static Void clearfeature(f)
featuretype **f;
{
  featuretype *WITH;

  WITH = *f;
  clearstring(&WITH->id);
  WITH->coordinate = 0.0;
  WITH->orientation = 1;
  clearstring(&WITH->nametag);
  clearstring(&WITH->othertag);
  WITH->definition = NULL;

  WITH->Ri = 0.0;
  WITH->Z = 0.0;
  WITH->probability = 0.0;

  WITH->Tparam = ' ';
  WITH->Aparam = 0.0;
  WITH->Bparam = 0.0;
  WITH->Cparam = 0.0;
  WITH->Dparam = 0.0;
  WITH->evencoordinate = 0.0;
  WITH->unsatisfied = true;
  WITH->fromrange = 0.0;
  WITH->torange = 0.0;
  WITH->number = 0;
  WITH->desiredline = 0;
  WITH->others = NULL;
  WITH->mothers = NULL;
  WITH->next = NULL;
}


/* Local variables for readafeature: */
struct LOC_readafeature {
  featuretype **f;
} ;

Local Void reportandhalt(LINK)
struct LOC_readafeature *LINK;
{
  _TEXT TEMP;

  printf("Feature details:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeafeature(&TEMP, *LINK->f);
  putchar('\n');
  halt();
}



Static Void readafeature(afile, definitionlist, f_)
_TEXT *afile;
definetype *definitionlist;
featuretype **f_;
{
  /*

*/
  struct LOC_readafeature V;
  definetype *d;
  boolean done = false;
  featuretype *WITH;
  _TEXT TEMP;

  V.f = f_;
  WITH = *V.f;
  skipnonblanks(afile);
  skipblanks(afile);
  grabtoken(afile, &WITH->id);
  fscanf(afile->f, "%lg", &WITH->coordinate);
  fscanf(afile->f, "%ld", &WITH->orientation);
  readquotestring(afile, &WITH->nametag);
  readquotestring(afile, &(*V.f)->othertag);


  if (WITH->orientation != -1 && WITH->orientation != 1) {
    printf("while reading feature id ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->id);
    printf("\nfeature orientation must be -1 or +1\n");
    printf("It is %ld\n", WITH->orientation);
    reportandhalt(&V);
  }


  d = definitionlist;
  while (!done) {
    if (d == NULL) {
      printf("No definition was found for feature ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writequotestring(&TEMP, WITH->nametag);
      putchar('\n');
      reportandhalt(&V);
    }
    if (equalstring(d->nametag, (*V.f)->nametag)) {
      WITH->definition = d;
      done = true;
    } else
      d = d->next;
  }



  if (d->matrix != NULL)
    fscanf(afile->f, "%lg%lg%lg", &WITH->Ri, &WITH->Z, &WITH->probability);
  else {
    WITH->Ri = -LONG_MAX;
    WITH->Z = LONG_MAX;
    WITH->probability = 1.0;


    /*

*/

    skipblanks(afile);
    if (P_eoln(afile->f)) {
      WITH->Tparam = ' ';
      WITH->Aparam = 0.0;
      WITH->Bparam = 0.0;
      WITH->Cparam = 0.0;
      WITH->Dparam = 0.0;
    } else {
      if (P_peek(afile->f) == '9' || P_peek(afile->f) == '8' ||
	  P_peek(afile->f) == '7' || P_peek(afile->f) == '6' ||
	  P_peek(afile->f) == '5' || P_peek(afile->f) == '4' ||
	  P_peek(afile->f) == '3' || P_peek(afile->f) == '2' ||
	  P_peek(afile->f) == '1' || P_peek(afile->f) == '0' ||
	  P_peek(afile->f) == '+' || P_peek(afile->f) == '-') {
	fscanf(afile->f, "%lg", &WITH->Aparam);
	fscanf(afile->f, "%lg", &WITH->Bparam);
	fscanf(afile->f, "%lg", &WITH->Cparam);
	WITH->Dparam = 0.0;
      }

      else {
	WITH->Tparam = getc(afile->f);
	if (WITH->Tparam == '\n')
	  WITH->Tparam = ' ';
	fscanf(afile->f, "%lg", &WITH->Aparam);
	fscanf(afile->f, "%lg", &WITH->Bparam);
	fscanf(afile->f, "%lg", &WITH->Cparam);
	fscanf(afile->f, "%lg", &WITH->Dparam);

      }
    }
  }


  /*



*/


  WITH->unsatisfied = true;
  WITH->fromrange = WITH->definition->min;
  WITH->torange = WITH->definition->max;

  WITH->number = 0;
  WITH->desiredline = 0;
  WITH->next = NULL;

}


#define tab             9

#define debugging       false


Local Void yell()
{
  printf("ERROR in reading definitions and features\n");
}

/* Local variables for markismulti: */
struct LOC_markismulti {
  featuretype **featurelist;
} ;

Local Void crosstickle(f, q, LINK)
featuretype **f, **q;
struct LOC_markismulti *LINK;
{
  /*






*/
  boolean duplicateMother, duplicateOther;
  featurecarrier *otherSpot;
  /*
*/
  featurecarrier *motherSpot, *previousSpot;
  _TEXT TEMP;

  printf("CROSSTICKLE BEGIN BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n");
  if ((*f)->definition->istotal) {
    printf("WORKING WITH A TOTAL FOR f:");

  }
  if ((*f)->definition->isgap)
    printf("WORKING WITH A GAP FOR f:");
  if ((*f)->definition->ismulti) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showafeature(&TEMP, *f);
    printf("-----\n");
    printf("WITH THIS q:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showafeature(&TEMP, *q);
    printf("-----\n");
  }

  /*
*/
  if ((*q)->mothers == NULL) {
    (*q)->mothers = (featurecarrier *)Malloc(sizeof(featurecarrier));
    (*q)->mothers->afeature = *f;
    (*q)->mothers->next = NULL;
  } else {
    motherSpot = (*q)->mothers;
    previousSpot = motherSpot;
    duplicateMother = false;
    while (motherSpot != NULL) {
      if (motherSpot->afeature == *f)
	duplicateMother = true;
      previousSpot = motherSpot;
      motherSpot = motherSpot->next;
    }
    if (!duplicateMother) {
      previousSpot->next = (featurecarrier *)Malloc(sizeof(featurecarrier));
      motherSpot = previousSpot->next;
      motherSpot->afeature = *f;
      motherSpot->next = NULL;
    }

  }




  if ((*f)->others == NULL) {
    (*f)->others = (featurecarrier *)Malloc(sizeof(featurecarrier));
    (*f)->others->afeature = *q;
    (*f)->others->next = NULL;
  } else {
    otherSpot = (*f)->others;
    previousSpot = otherSpot;
    duplicateOther = false;
    while (otherSpot != NULL) {
      if (otherSpot->afeature == *q)
	duplicateOther = true;
      previousSpot = otherSpot;
      otherSpot = otherSpot->next;
    }
    if (!duplicateOther) {
      previousSpot->next = (featurecarrier *)Malloc(sizeof(featurecarrier));
      otherSpot = previousSpot->next;
      otherSpot->afeature = *q;
      otherSpot->next = NULL;
    }

  }


  otherSpot = (*f)->mothers;
  while (otherSpot != NULL) {
    motherSpot = otherSpot->next;
    while (motherSpot != NULL) {
      if (motherSpot->afeature == otherSpot->afeature) {
	printf("DUPLICATE MOTHER FOUND MMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showafeature(&TEMP, *f);
	printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
	halt();
      }
      motherSpot = motherSpot->next;
    }
    otherSpot = otherSpot->next;
  }


  otherSpot = (*f)->others;
  while (otherSpot != NULL) {
    motherSpot = otherSpot->next;
    while (motherSpot != NULL) {
      if (motherSpot->afeature == otherSpot->afeature) {
	printf("DUPLICATE OTHER FOUND OOOOOOOOOOOOOOOOOOOOOOOOOOO\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showafeature(&TEMP, *f);
	printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
	halt();
      }
      motherSpot = motherSpot->next;
    }
    otherSpot = otherSpot->next;
  }

  printf("CROSSTICKLE RESULTS: EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");

  if ((*f)->definition->istotal)
    printf("WORKING WITH A TOTAL FOR f:");
  if ((*f)->definition->isgap)
    printf("WORKING WITH A GAP FOR f:");
  if ((*f)->definition->ismulti) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showafeature(&TEMP, *f);
    printf("-----\n");
    printf("WITH THIS q:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showafeature(&TEMP, *q);
    printf("-----\n");
  }

  printf("CROSSTICKLE END EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
  if ((*f)->coordinate == 3407873L)
    printf("CROSSTICKLE f^.coordinate = 3407873\n");

}

Local Void changelist(featurelist, aname, coo, f, q, LINK)
featuretype *featurelist;
stringDelila aname;
long coo;
featuretype **f, **q;
struct LOC_markismulti *LINK;
{
  /*




*/
  _TEXT TEMP;

  locatefeature(aname, (double)coo, featurelist, q);

  if (*q != NULL) {
    crosstickle(f, q, LINK);

    return;
  }
  printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
  printf("ERROR in markismulti:");
  printf(" AT COORDINATE: %ld\n", coo);
  printf(" cannot locate a feature for the name \"");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  /*



*/
  writestring(&TEMP, &aname);
  printf("\"\n");
  printf("from this multi-part definition:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  /*

*/
  writeadefinition(&TEMP, (*f)->definition);
  printf("for this feature:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeafeature(&TEMP, *f);
  putchar('\n');

  /*

*/

  /*






*/
  /*

*/
}

/* Local variables for locatesubfeatures: */
struct LOC_locatesubfeatures {
  struct LOC_markismulti *LINK;
  long m;
} ;

/*
*/
Local Void failLocations(LINK)
struct LOC_locatesubfeatures *LINK;
{
  printf("locatesubfeatures: the location %ld was used before\n", LINK->m);
  halt();
}

Local Void failOrientation(LINK)
struct LOC_locatesubfeatures *LINK;
{
  printf("locatesubfeatures: the feature orientation was not consistent\n");
  /*

*/
}

Local Void locatesubfeatures(featurelist, namelist, f, LINK)
featuretype *featurelist;
stringDelila *namelist;
featuretype **f;
struct LOC_markismulti *LINK;
{
  /*








*/
  struct LOC_locatesubfeatures V;
  double coo;
  boolean found;
  boolean foundPos = false, foundNeg = false;
  featuretype *q;
  stringDelila *n;
  boolean usedlocations[maxstring];
  _TEXT TEMP;
  definetype *WITH;
  long FORLIM;

  V.LINK = LINK;
  printf("locatesubfeatures llllllllllllllllllllllllllllllllll\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeafeature(&TEMP, *f);
  putchar('\n');
  WITH = (*f)->definition;
  printf("| f^.definition.marks =  %ld\n", WITH->marks);
  printf("| f^.definition.locations[1] =  %1.1f\n", WITH->locations[0]);
  printf("| f^.definition.locations[2] =  %1.1f\n", WITH->locations[1]);
  if (namelist->next != NULL) {
    printf("| namelist^.next^      =\"");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, namelist->next);
    printf("\"\n");
  }

  printf("| round(f^.coordinate) =  %ld\n",
	 (long)floor((*f)->coordinate + 0.5));

  n = namelist;
  FORLIM = (*f)->definition->marks;
  for (V.m = 1; V.m <= FORLIM; V.m++)
    usedlocations[V.m-1] = false;
  while (n != NULL) {
    WITH = (*f)->definition;
    printf("locatesubfeatures, looking at name: \"");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, n);
    printf("\"\n");


    FORLIM = WITH->marks;
    for (V.m = 1; V.m <= FORLIM; V.m++) {
      printf("   locations[%ld] = %2.1f\n", V.m, WITH->locations[V.m-1]);

      found = false;
      coo = (*f)->coordinate - WITH->locations[V.m-1];
      locatefeature(*n, coo, featurelist, &q);
      if (q == NULL) {
	/*
*/
	coo = (*f)->coordinate + WITH->locations[V.m-1];
	locatefeature(*n, coo, featurelist, &q);
	if (q != NULL) {
	  found = true;
	  if (foundNeg)
	    failOrientation(&V);
	  else
	    foundPos = true;
	  printf("   FOUND +\n");
	  if (usedlocations[V.m-1])
	    failLocations(&V);
	  else
	    usedlocations[V.m-1] = true;
	  crosstickle(f, &q, LINK);
	}
      } else {
	found = true;
	if (foundPos)
	  failOrientation(&V);
	else
	  foundNeg = true;
	printf("   FOUND -\n");
	if (usedlocations[V.m-1])
	  failLocations(&V);
	else
	  usedlocations[V.m-1] = true;
	crosstickle(f, &q, LINK);
	if ((*f)->coordinate == 3407873L) {
	  printf("locatesubfeatures f^.coordinate = 3407873\n");
	  printf("hopefully the first time\n");
	}
      }
    }


    /*


*/
    /*









*/
    /*

*/

    n = n->next;
  }




  /*











*/

}

Local Void checkcount(f, finalcount, LINK)
featuretype *f;
long finalcount;
struct LOC_markismulti *LINK;
{
  /*
*/
  long thecount = 0;
  featurecarrier *trace;
  stringDelila *thisname;
  _TEXT TEMP;

  trace = f->others;
  while (trace != NULL) {
    thecount++;
    trace = trace->next;
  }
  if (thecount >= finalcount)
    return;

  printf("ERROR in markismulti: checkcount failed for this feature:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeafeature(&TEMP, f);
  putchar('\n');
  TEMP.f = stdout;
  *TEMP.name = '\0';
  showafeature(&TEMP, f);
  printf("Expected minimum of others is %ld\n", finalcount);
  printf("    Found number of others is %ld\n", thecount);

  /*



















*/

  /*

*/

  thisname = (stringDelila *)Malloc(sizeof(stringDelila));
  clearstring(thisname);
  thisname->length = 3;
  thisname->letters[0] = 'p';
  thisname->letters[1] = '1';
  thisname->letters[2] = '0';
  thisname->next = NULL;
  printf("thisname is \"");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, thisname);
  printf("\"\n");
  locatesubfeatures(*LINK->featurelist, thisname, &f, LINK);
  printf("END checkcount\n");

}


Local Void markismulti(featurelist_, f)
featuretype **featurelist_, **f;
{
  /*


















*/
  struct LOC_markismulti V;
  Char aletter;
  boolean atGap = false;
  long gaptotal = 0;
  /*




*/
  long gap = 0;
  definetype *d;
  boolean done = false;
  long i = 1;
  stringDelila *n;
  /*
*/
  stringDelila *namelist;
  /*




*/
  stringDelila *previousname;
  /*
*/
  featuretype *q, *qMayBeGap;
  /*
*/
  long parcoo = 0;
  _TEXT TEMP;



  V.featurelist = featurelist_;
  /*






*/

  if (false) {
    printf("BEGIN markismulti: \n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeadefinition(&TEMP, (*f)->definition);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeafeature(&TEMP, *f);
    printf("\n..... markismulti: \n");
    if ((*f)->definition->istotal)
      printf("markismulti at a total\n");
  }

  if (!(*f)->definition->ismulti)
    return;
  /*

*/


  /*

*/
  printf("about to parse ooo\n");
  /*

*/
  if ((*f)->others != NULL) {
    printf("markismulti: the feature\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeafeature(&TEMP, *f);
    printf("\nalready points to other features!\n");
    halt();
  }
  printf("PAST parse if ooo\n");
  namelist = (stringDelila *)Malloc(sizeof(stringDelila));
  clearstring(namelist);
  previousname = namelist;
  d = (*f)->definition;
  while (!done) {
    aletter = d->nametag.letters[i-1];

    /*


*/
    if (aletter == (Char)tab)
      done = true;
    /*

*/

    if ((aletter == '-' || aletter == (Char)tab) && namelist->length > 0) {
      atGap = false;
      /*




*/

      /*


*/



      if (aletter != (Char)tab) {
	n = (stringDelila *)Malloc(sizeof(stringDelila));
	previousname = namelist;
	n->next = namelist;
	namelist = n;
      }

    }

    else if (aletter == '(') {

      atGap = true;
      gap = 0;
      /*

*/

    } else if (atGap) {

      if (aletter == ')') {
	atGap = false;
	gaptotal += gap;
	/*

*/
	previousname->current = gap;
      } else {
	if (aletter != '9' && aletter != '8' && aletter != '7' &&
	    aletter != '6' && aletter != '5' && aletter != '4' &&
	    aletter != '3' && aletter != '2' && aletter != '1' &&
	    aletter != '0') {
	  printf("markismulti: gap number is bad\n");
	  printf("non-number found: aletter = %c\n", aletter);
	  halt();
	}
	gap = gap * 10 + aletter - '0';
      }
    } else if (aletter != '-' && aletter != (Char)tab) {
      /*

*/
      /*

*/
      namelist->length++;
      namelist->letters[namelist->length - 1] = aletter;
    }

    /*

*/
    i++;
    if (i > d->nametag.length)
      done = true;
  }

  /*
*/
  printf("===================== writeafeature:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeafeature(&TEMP, *f);
  printf("\n===================== showafeature:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  showafeature(&TEMP, *f);
  printf("markismulti: ONE Done OK? OK!!\n");

  /*


*/
  done = false;
  while (!done) {
    aletter = d->nametag.letters[i-1];
    /*


*/
    if (aletter != '9' && aletter != '8' && aletter != '7' &&
	aletter != '6' && aletter != '5' && aletter != '4' &&
	aletter != '3' && aletter != '2' && aletter != '1' && aletter != '0')
      done = true;
    else
      parcoo = parcoo * 10 + aletter - '0';
    i++;
  }
  /*

*/


  if (namelist == NULL) {
    printf("ERROR: markismulti: could not find any names!\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeafeature(&TEMP, *f);
    putchar('\n');
    halt();
  }
  if (namelist->next == NULL) {
    printf("ERROR: markismulti: could not find two names!\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeafeature(&TEMP, *f);
    putchar('\n');
    halt();
  }




  printf("1 NOT BLOCKED CODE!\n");
  locatesubfeatures(*V.featurelist, namelist, f, &V);
  printf("0 NOT BLOCKED CODE!\n");


  if (false) {
    printf("1 BLOCKED CODE!\n");
    halt();
    /*

*/
    printf("markismulti: ANALYSIS OF TOTAL ---------- <<<<<<<<<<<<<<<\n");
    printf("IT MISSED THE TOTAL!\n");
    halt();

    if (parcoo != (long)floor((*f)->coordinate + 0.5)) {
      printf("ERROR in markismulti: Feature:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeafeature(&TEMP, *f);
      printf("\nhas coordinate %1.1f\n", (*f)->coordinate);
      printf("but its id says it is at %ld\n", parcoo);
      halt();
    }




    n = namelist;
    while (n != NULL) {
      printf("LOCATE Gap part: ");
      putchar('"');
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, n);
      printf("\" with gap = %ld\n", n->current);
      /*


*/
      /*

*/
      qMayBeGap = *V.featurelist;
      while (qMayBeGap != NULL && qMayBeGap != *f) {
	/*














*/
	if (qMayBeGap->definition->isgap) {
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  /*

*/
	  writeafeature(&TEMP, qMayBeGap);
	  printf(" A GAP!\n");
	  /*






*/
	  if (qMayBeGap->id.current == (*f)->coordinate) {
	    printf(" ACCEPTED\n");
	    /*

*/

	    /*





*/
	    printf("IS THIS THE SECOND CROSSTICKLE?\n");
	    crosstickle(f, &qMayBeGap, &V);
	    crosstickle(f, &qMayBeGap->others->afeature, &V);
	    crosstickle(f, &qMayBeGap->others->next->afeature, &V);


	    /*



*/
	    printf("================== showafeature istotal - analysis f:\n");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    showafeature(&TEMP, *f);
	    printf("================== showafeature istotal - analysis qMayBeGap:\n");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    showafeature(&TEMP, qMayBeGap);
	    /*



*/
	    /*

*/
	  }
	  /*

*/
	  else
	    printf(" REJECTED\n");
	}
	qMayBeGap = qMayBeGap->next;
      }
      if (qMayBeGap == NULL) {
	printf("ERROR in markismulti: total feature\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeafeature(&TEMP, *f);
	printf("\nhas no gaps\n");
	halt();
      }
      n = n->next;
    }
  }
  /*


*/

  /*




*/
  if (false) {
    printf("3 BLOCKED CODE!\n");
    halt();

    printf("markismulti: ANALYSIS OF GAP ---------- <<<<<<<<<<<<<<<\n");
    if (!(*f)->definition->isgap) {
      printf("ERROR: markismulti: ismulti but not isgap!\n");
      halt();
    }
    /*




*/

    if ((*f)->definition->marks != 2) {
      printf("ERROR in markismulti:");
      printf(" A Gap feature must have exactly 2 marks!\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeafeature(&TEMP, *f);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writeadefinition(&TEMP, (*f)->definition);
      halt();
    }
    /*





*/
    /*


*/


    /*
*/
    /*






*/

    printf("4 BLOCKED CODE!\n");
    halt();
    locatesubfeatures(*V.featurelist, namelist, f, &V);


    printf("FIRST change list 111111111111111111111111111111111\n");
    printf("namelist^=\"");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, namelist);
    printf("\"\n");
    printf("parcoo =  %ld\n", parcoo);

    changelist(*V.featurelist, *namelist, parcoo, f, &q, &V);
    printf("SECOND change list 22222222222222222222222222222222\n");
    printf("namelist^.next^=\"");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, namelist->next);
    printf("\"\n");
    printf(
      "round(f^.coordinate+f^.orientation*f^.definition^.locations[2]) =  %ld\n",
      (long)floor((*f)->coordinate + (*f)->orientation * (*f)->definition->
							 locations[1] + 0.5));

    changelist(*V.featurelist, *namelist->next,
      (long)floor((*f)->coordinate + (*f)->orientation * (*f)->definition->
							 locations[1] + 0.5),
      f, &q, &V);
    if (q == NULL) {
      printf(
	"TRY THE OTHER END (ORIENTION) round(f^.coordinate-f^.orientation*f^.definition^.locations[2])= %ld\n",
	(long)floor((*f)->coordinate - (*f)->orientation * (*f)->definition->
					 locations[1] + 0.5));
      /*



*/
      /*

*/
      changelist(*V.featurelist, *namelist->next,
	(long)floor((*f)->coordinate -
		    (*f)->orientation * (*f)->definition->locations[1] + 0.5),
	f, &q, &V);
      if (q == NULL) {
	printf("COULD NOT LOCATE AT OTHER END - showfeatures\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	showfeatures(&TEMP, *V.featurelist);
	printf("COULD NOT LOCATE AT OTHER END - halt\n");
	halt();
      }
      TEMP.f = stdout;
      *TEMP.name = '\0';
      showafeature(&TEMP, *f);
      printf("q STOP\n");
      /*

*/
    }

    /*


















*/


    /*

*/
    /*




*/
    /*










*/
    (*f)->id.current = parcoo;


  }


  if (namelist != NULL) {
    n = namelist->next;
    while (namelist != NULL) {
      Free(namelist);
      namelist = n;
      if (n != NULL)
	n = n->next;
    }
  }

  /*

*/
  /*


*/
  if ((*f)->definition->istotal)
    checkcount(*f, 2L, &V);
  if ((*f)->definition->isgap)
    checkcount(*f, 2L, &V);

  if (!debugging)
    return;

  if ((*f)->definition->isgap) {
    printf("===================== showafeature isgap:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showafeature(&TEMP, *f);
  }
  if (!(*f)->definition->istotal)
    return;
  printf("===================== showafeature istotal:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  showafeature(&TEMP, *f);
  printf("markismulti: DONE FOR NOW <<<<<<<<<<<<<<< showfeatures BEGIN\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  showfeatures(&TEMP, *V.featurelist);
  printf("markismulti: DONE FOR NOW <<<<<<<<<<<<<<< showfeatures END\n");
}

#undef tab
#undef debugging




Static Void readdefinitionsandfeatures(afile, KeepDuplicates_, definitionlist,
				       featurelist)
_TEXT *afile;
boolean *KeepDuplicates_;
definetype **definitionlist;
featuretype **featurelist;
{
  /*















*/
  stringDelila aname;

  long countdefinitions = 0, countfeatures = 0;

  long countribls = 0, countboundaries = 0, countpetals = 0;

  definetype *d;
  boolean done;
  definetype *dprevious, *dtest;
  featuretype *f;
  boolean firstfeature = true;
  Char identifier;

  ribltype *matrix;
  boolean missingmatrix;

  featuretype *newfeature;
  /*
*/

  definetype *thedefinition;
  /*
*/

  long uniquedefinitions = 0, uniquefeatures = 0;
  _TEXT TEMP;
  petaltype *WITH;


  if (*afile->name != '\0') {
    if (afile->f != NULL)
      afile->f = freopen(afile->name, "r", afile->f);
    else
      afile->f = fopen(afile->name, "r");
  } else
    rewind(afile->f);
  if (afile->f == NULL)
    _EscIO2(FileNotFound, afile->name);
  RESETBUF(afile->f, Char);
  *definitionlist = NULL;
  *featurelist = (featuretype *)Malloc(sizeof(featuretype));
  clearfeature(featurelist);


  d = *definitionlist;
  f = *featurelist;
  while (!BUFEOF(afile->f)) {
    if ((P_peek(afile->f) == '*') | P_eoln(afile->f)) {
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      continue;
    }
    skipblanks(afile);


    identifier = P_peek(afile->f);

    if (identifier == 'd') {
      if (*definitionlist == NULL) {
	*definitionlist = (definetype *)Malloc(sizeof(definetype));
	cleardefinition(definitionlist);
	d = *definitionlist;
	dprevious = d;
      } else {
	dprevious = d;
	d->next = (definetype *)Malloc(sizeof(definetype));
	d = d->next;
	cleardefinition(&d);
      }
      readadefinition(afile, &d);
      /*

*/
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      countdefinitions++;
      uniquedefinitions++;
      d->number = uniquedefinitions;


      dtest = *definitionlist;
      done = false;
      while (!done) {
	if (dtest == NULL) {
	  done = true;
	  break;
	}
	if (equalstring(dtest->nametag, d->nametag) &&
	    dtest->number != d->number) {
	  done = true;
	  /*
*/
	  printf("WARNING: A duplicate definition was found for definition %ld ",
		 countdefinitions);
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writequotestring(&TEMP, d->nametag);
	  printf(".\n");
	  printf("         The first definition (number %ld) will be used.\n",
		 dtest->number);

	  /*













*/



	  dprevious->next = NULL;
	  Free(d);
	  done = true;
	  /*
*/

	  d = dprevious;
	  /*



*/

	  /*



*/

	  /*


*/

	  uniquedefinitions--;
	}
	if (!done)
	  dtest = dtest->next;
      }
      continue;
    }


    if (identifier == 'r') {
      getriblmatrix(afile, &matrix);

      locatedefinition(matrix->riblname, *definitionlist, &thedefinition);
      if (thedefinition == NULL) {
	printf("A definition for Ribl matrix ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, matrix->riblname);
	printf(" was not found in the features.\n");
	halt();
      }

      /*
*/
      if (thedefinition->min < matrix->frombase ||
	  thedefinition->max > matrix->tobase) {
	printf("Definition:\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writeadefinition(&TEMP, thedefinition);
	printf("\nViolates The From-To Bounds of its Matrix:\n");
	printf("matrix frombase: %ld\n", matrix->frombase);
	printf("matrix   tobase: %ld\n", matrix->tobase);
	halt();
      }


      thedefinition->matrix = matrix;
      countribls++;
      continue;
    }


    if (identifier == 'b') {
      skipnonblanks(afile);
      skipblanks(afile);
      readquotestring(afile, &aname);
      locatedefinition(aname, *definitionlist, &thedefinition);
      if (thedefinition == NULL) {
	printf("WARNING: A definition for the boundaries: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf("          was not found in the features.\n");
      } else {
	fscanf(afile->f, "%lg%lg%lg%*[^\n]", &thedefinition->RiboundFeature,
	       &thedefinition->ZboundFeature, &thedefinition->PboundFeature);
	getc(afile->f);
	countboundaries++;
      }
      continue;
    }


    if (identifier == 'p') {
      skipnonblanks(afile);
      skipblanks(afile);
      readquotestring(afile, &aname);
      locatedefinition(aname, *definitionlist, &thedefinition);
      if (thedefinition == NULL) {
	printf("WARNING: A definition for the color rectangle: ");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf("\n         was not found in the features.\n");
	fscanf(afile->f, "%*[^\n]");
	getc(afile->f);
	continue;
      }
      WITH = &thedefinition->petal;
      fscanf(afile->f, "%ld", &WITH->el);
      /*

*/
      skipblanks(afile);
      WITH->c_ = getc(afile->f);
      if (WITH->c_ == '\n')
	WITH->c_ = ' ';
      if (WITH->c_ != 'r' && WITH->c_ != 'h') {
	yell();
	printf(" feature\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf(": color type must be r or h, not \"%c\"\n", WITH->c_);
	halt();
      }
      fscanf(afile->f, "%lg%lg%lg%lg%lg%lg%lg%*[^\n]", &WITH->eh, &WITH->es,
	     &WITH->eb, &WITH->fh, &WITH->fs, &WITH->fb, &WITH->spare);
      getc(afile->f);

      /*















*/

      if (WITH->eh > 1 || WITH->es > 1 || WITH->eb > 1 || WITH->fh > 1 ||
	  WITH->fs > 1 || WITH->fb > 1) {
	yell();
	printf(" feature\n");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writequotestring(&TEMP, aname);
	printf(": eh, es, eb, fh, fs, and fb \n");
	printf("must have values less than 1\n");
	halt();
      }

      /*











*/

      countpetals++;
      continue;
    }





    if (identifier != '@') {

      printf(
	"features file lines must be blank or begin with one of \"*d@rbp\", not \"%c\".\n",
	identifier);
      halt();
      continue;
    }

    countfeatures++;
    if (firstfeature) {
      firstfeature = false;
      readafeature(afile, *definitionlist, &f);
      markismulti(featurelist, &f);
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      /*
*/
      f->number = countfeatures;
      uniquefeatures++;
      continue;
    }

    newfeature = (featuretype *)Malloc(sizeof(featuretype));
    clearfeature(&newfeature);
    readafeature(afile, *definitionlist, &newfeature);
    fscanf(afile->f, "%*[^\n]");
    getc(afile->f);
    /*
*/
    if (!featureinlist(newfeature, *featurelist)) {
      f->next = newfeature;
      f = f->next;
      f->number = countfeatures;
      uniquefeatures++;
      markismulti(featurelist, &f);
      continue;
    }

    printf("This feature was found more than once:\n");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writeafeature(&TEMP, newfeature);
    putchar('\n');
    if (*KeepDuplicates_) {
      f->next = newfeature;
      f = f->next;
      f->number = countfeatures;
      printf("It is being kept.\n");
    } else {
      printf("It will be ignored.\n");
      Free(newfeature);
    }
  }

  /*

*/

  /*

*/


  /*
*/
  if (*definitionlist != NULL) {
    d = *definitionlist;
    missingmatrix = false;
    while (d != NULL) {
      /*



*/
      if (d->background.length == 2) {
	if (d->background.letters[1] == 'I' ||
	    d->background.letters[1] == 'i' ||
	    d->background.letters[1] == 'L' ||
	    d->background.letters[1] == 'l' ||
	    d->background.letters[1] == 'W' ||
	    d->background.letters[1] == 'w') {
	  if (d->matrix == NULL) {
	    missingmatrix = true;
	    printf("Definition:\n");
	    TEMP.f = stdout;
	    *TEMP.name = '\0';
	    writeadefinition(&TEMP, d);
	    printf("\nis missing a Ribl weight matrix\n");
	  }
	}
      }
      d = d->next;
    }
    if (missingmatrix) {
      printf("readdefinitionsandfeatures: missingmatrix is true\n");
      halt();
    }
  }


  printf("%ld definition", countdefinitions);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  plural(&TEMP, countdefinitions);
  printf(" read in\n");

  printf("%ld unique definition", uniquedefinitions);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  plural(&TEMP, uniquedefinitions);
  printf(" found\n");


  printf("%ld definition", countribls);
  if (countribls == 1)
    printf(" has an Ribl matrix\n");
  else
    printf("s have Ribl matricies\n");

  printf("%ld definition", countboundaries);
  if (countboundaries == 1)
    printf(" boundry defined\n");
  else
    printf(" boundries defined\n");

  printf("%ld", countpetals);
  if (countpetals == 1)
    printf(" petal (color rectangle behind a walker) defined\n");
  else
    printf(" petals (color rectangles behind walkers) defined\n");



  printf("%ld feature", countfeatures);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  plural(&TEMP, countfeatures);
  printf(" read in\n");

  printf("%ld unique feature", uniquefeatures);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  plural(&TEMP, uniquefeatures);
  printf(" found\n");

  /*






*/

}



Static Void readparameters(localbestp, window, asterisk)
_TEXT *localbestp;
long *window;
boolean *asterisk;
{
  /*
*/
  double parameterversion;
  Char ch;

  if (*localbestp->name != '\0') {
    if (localbestp->f != NULL)
      localbestp->f = freopen(localbestp->name, "r", localbestp->f);
    else
      localbestp->f = fopen(localbestp->name, "r");
  } else
    rewind(localbestp->f);
  if (localbestp->f == NULL)
    _EscIO2(FileNotFound, localbestp->name);
  RESETBUF(localbestp->f, Char);

  fscanf(localbestp->f, "%lg%*[^\n]", &parameterversion);
  getc(localbestp->f);
  if (parameterversion < updateversion || parameterversion > versionupperbound) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(localbestp->f, "%ld%*[^\n]", window);
  getc(localbestp->f);
  fscanf(localbestp->f, "%c%*[^\n]", &ch);
  getc(localbestp->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == 'A')
    *asterisk = true;
  else
    *asterisk = false;

  printf("\nAny features which are within ");
  printf("%ld", *window);
  printf(" bp of a feature\n");
  printf("with a higher Ri will be ");

  if (*asterisk)
    printf("written to the lowerfeatures file\n");
  else
    printf("deleted\n");
}


/* Local variables for filterfeatures: */
struct LOC_filterfeatures {
  featuretype **discardfeature;


  long c_;
  featuretype *t_;
} ;


Local Void testit(LINK)
struct LOC_filterfeatures *LINK;
{
  LINK->c_ = 0;
  LINK->t_ = *LINK->discardfeature;
  while (LINK->t_ != NULL) {
    LINK->c_++;
    printf("%ld\n", LINK->c_);
    LINK->t_ = LINK->t_->next;
  }
}



Static Void filterfeatures(f, filteredfeature, discardfeature_, window)
featuretype *f, **filteredfeature, **discardfeature_;
long window;
{
  /*



*/
  struct LOC_filterfeatures V;
  featuretype *filteredfeature2;
  boolean first = false;
  featuretype *firstf, *firstfi;
  boolean found;


  V.discardfeature = discardfeature_;
  firstf = f;

  /*
*/
  Malloc(sizeof(featuretype));
/* p2c: localbest.p: Note: Eliminated unused assignment statement [338] */
  filteredfeature2 = (*filteredfeature)->next;

  *V.discardfeature = NULL;

  /*

*/
  while (!first) {
    first = true;
    f = firstf;
    found = false;

    /*

*/

    while (f != NULL && !found) {
      /*
*/
      if ((fabs((*filteredfeature)->coordinate - f->coordinate) <= window &&
	   (*filteredfeature)->Ri < f->Ri) & equalstring(
	    (*filteredfeature)->nametag,
	    f->nametag) & equalstring(f->id, (*filteredfeature)->id)) {
	/*
*/

	(*filteredfeature)->next = *V.discardfeature;
	*V.discardfeature = *filteredfeature;
	*filteredfeature = filteredfeature2;
	filteredfeature2 = (*filteredfeature)->next;

	found = true;
	first = false;
      }

      f = f->next;
    }
  }

  firstfi = *filteredfeature;

  while (filteredfeature2 != NULL) {
    f = firstf;
    found = false;

    /*

*/
    while (f != NULL && !found) {
      if ((fabs(filteredfeature2->coordinate - f->coordinate) <= window &&
	   filteredfeature2->Ri < f->Ri) & equalstring(filteredfeature2->nametag,
	    f->nametag) & equalstring(f->id, filteredfeature2->id)) {
	/*
*/

	found = true;

	(*filteredfeature)->next = filteredfeature2->next;
	filteredfeature2->next = *V.discardfeature;
	*V.discardfeature = filteredfeature2;
	filteredfeature2 = (*filteredfeature)->next;

      }

      f = f->next;
    }

    /*
*/
    if (!found) {
      *filteredfeature = (*filteredfeature)->next;
      filteredfeature2 = filteredfeature2->next;
    }
  }


  *filteredfeature = firstfi;
  /*
*/
}



Static Void writefilteredfeatures(filteredfeature, features, bestfeatures,
				  filterednumber)
featuretype *filteredfeature;
_TEXT *features, *bestfeatures;
long *filterednumber;
{
  Char ch;

  if (*bestfeatures->name != '\0') {
    if (bestfeatures->f != NULL)
      bestfeatures->f = freopen(bestfeatures->name, "w", bestfeatures->f);
    else
      bestfeatures->f = fopen(bestfeatures->name, "w");
  } else {
    if (bestfeatures->f != NULL)
      rewind(bestfeatures->f);
    else
      bestfeatures->f = tmpfile();
  }
  if (bestfeatures->f == NULL)
    _EscIO2(FileNotFound, bestfeatures->name);
  SETUPBUF(bestfeatures->f, Char);
  if (*features->name != '\0') {
    if (features->f != NULL)
      features->f = freopen(features->name, "r", features->f);
    else
      features->f = fopen(features->name, "r");
  } else
    rewind(features->f);
  if (features->f == NULL)
    _EscIO2(FileNotFound, features->name);
  RESETBUF(features->f, Char);
  *filterednumber = 0;

  /*
*/
  while (!BUFEOF(features->f)) {
    if (P_peek(features->f) != '@') {
      while (!P_eoln(features->f)) {
	ch = getc(features->f);
	if (ch == '\n')
	  ch = ' ';
	putc(ch, bestfeatures->f);
      }
      putc('\n', bestfeatures->f);
    }
    fscanf(features->f, "%*[^\n]");
    getc(features->f);
  }


  while (filteredfeature != NULL) {
    writeafeature(bestfeatures, filteredfeature);
    putc('\n', bestfeatures->f);
    filteredfeature = filteredfeature->next;
    (*filterednumber)++;
  }

  printf("number filtered:  %ld\n", *filterednumber);

}



Static Void writediscardedfeatures(discardfeature, definitionlist, features,
				   lowerfeatures, discardednumber)
featuretype **discardfeature;
definetype *definitionlist;
_TEXT *features, *lowerfeatures;
long *discardednumber;
{
  Char ch;
  long x;
  definetype *firstd;
  stringDelila *WITH;
  long FORLIM;

  if (*lowerfeatures->name != '\0') {
    if (lowerfeatures->f != NULL)
      lowerfeatures->f = freopen(lowerfeatures->name, "w", lowerfeatures->f);
    else
      lowerfeatures->f = fopen(lowerfeatures->name, "w");
  } else {
    if (lowerfeatures->f != NULL)
      rewind(lowerfeatures->f);
    else
      lowerfeatures->f = tmpfile();
  }
  if (lowerfeatures->f == NULL)
    _EscIO2(FileNotFound, lowerfeatures->name);
  SETUPBUF(lowerfeatures->f, Char);
  if (*features->name != '\0') {
    if (features->f != NULL)
      features->f = freopen(features->name, "r", features->f);
    else
      features->f = fopen(features->name, "r");
  } else
    rewind(features->f);
  if (features->f == NULL)
    _EscIO2(FileNotFound, features->name);
  RESETBUF(features->f, Char);
  firstd = definitionlist;
  *discardednumber = 0;

  while (definitionlist != NULL) {
    /*

*/
    WITH = &definitionlist->nametag;
    WITH->letters[WITH->length] = '.';
    WITH->letters[WITH->length + 1] = 'f';
    WITH->letters[WITH->length + 2] = 'i';
    WITH->letters[WITH->length + 3] = 'l';
    WITH->letters[WITH->length + 4] = 't';
    WITH->letters[WITH->length + 5] = 'e';
    WITH->letters[WITH->length + 6] = 'r';
    WITH->letters[WITH->length + 7] = 'e';
    WITH->letters[WITH->length + 8] = 'd';
    WITH->length += 9;
    WITH = &definitionlist->background;
    FORLIM = WITH->length;
    for (x = 0; x < FORLIM; x++) {
      if (WITH->letters[x] == 'w')
	WITH->letters[x] = ' ';
    }
    WITH = &definitionlist->negparts;
    WITH->length = 1;
    WITH->letters[WITH->length - 1] = '*';
    WITH = &definitionlist->posparts;
    WITH->length = 1;
    WITH->letters[WITH->length - 1] = '*';
    definitionlist->locations[0] = 0.0;
    definitionlist->marks = 1;
    definitionlist->min = 0.0;
    definitionlist->max = 0.0;

    definitionlist = definitionlist->next;
  }

  definitionlist = firstd;

  /*
*/
  while (!BUFEOF(features->f)) {
    if ((P_peek(features->f) != '@') & (P_peek(features->f) != 'd')) {
      while (!P_eoln(features->f)) {
	ch = getc(features->f);
	if (ch == '\n')
	  ch = ' ';
	putc(ch, lowerfeatures->f);
      }
      putc('\n', lowerfeatures->f);
    }
    if (P_peek(features->f) == 'd') {
      while (definitionlist != NULL) {
	writeadefinition(lowerfeatures, definitionlist);
	putc('\n', lowerfeatures->f);
	definitionlist = definitionlist->next;
      }
    }
    fscanf(features->f, "%*[^\n]");
    getc(features->f);
  }


  while (*discardfeature != NULL) {
    WITH = &(*discardfeature)->nametag;


    WITH->letters[WITH->length] = '.';
    WITH->letters[WITH->length + 1] = 'f';
    WITH->letters[WITH->length + 2] = 'i';
    WITH->letters[WITH->length + 3] = 'l';
    WITH->letters[WITH->length + 4] = 't';
    WITH->letters[WITH->length + 5] = 'e';
    WITH->letters[WITH->length + 6] = 'r';
    WITH->letters[WITH->length + 7] = 'e';
    WITH->letters[WITH->length + 8] = 'd';
    WITH->length += 9;
    /*



*/

    writeafeature(lowerfeatures, *discardfeature);
    putc('\n', lowerfeatures->f);
    *discardfeature = (*discardfeature)->next;
    (*discardednumber)++;
  }



  printf("number discarded: %ld\n", *discardednumber);

}


/* Local variables for PatentMessage: */
struct LOC_PatentMessage {
  _TEXT *f;
} ;

Local Void makebar(LINK)
struct LOC_PatentMessage *LINK;
{
  fprintf(LINK->f->f,
    "**********************************************************************\n");
}



Static Void PatentMessage(f_)
_TEXT *f_;
{
  /*

*/
  struct LOC_PatentMessage V;

  V.f = f_;
  makebar(&V);
  fprintf(V.f->f,
    "* By downloading this code you agree to the Source Code Use License: *\n");
  fprintf(V.f->f,
    "* https://alum.mit.edu/www/toms/Source_Code_Use_License.txt           *\n");
  fprintf(V.f->f,
    "* Contact: https://alum.mit.edu/www/toms/contacts.html                *\n");
  makebar(&V);
}



Static Void themain(features, localbestp, bestfeatures, lowerfeatures)
_TEXT *features, *localbestp, *bestfeatures, *lowerfeatures;
{
  boolean asterisk;
  /*
*/
  double parameterversion;
  definetype *definitionlist;
  featuretype *discardfeature;
  long discardednumber;
  featuretype *f, *filteredfeature;
  /*
*/
  long filterednumber, window;
  /*
*/
  boolean KeepDuplicates_ = true;
  _TEXT TEMP;

  printf("localbest %4.2f\n", version);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  PatentMessage(&TEMP);

  if (*localbestp->name != '\0') {
    if (localbestp->f != NULL)
      localbestp->f = freopen(localbestp->name, "r", localbestp->f);
    else
      localbestp->f = fopen(localbestp->name, "r");
  } else
    rewind(localbestp->f);
  if (localbestp->f == NULL)
    _EscIO2(FileNotFound, localbestp->name);
  RESETBUF(localbestp->f, Char);
  fscanf(localbestp->f, "%lg%*[^\n]", &parameterversion);
  getc(localbestp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  readparameters(localbestp, &window, &asterisk);


  /*

*/
  printf("\nfirst feature read\n");
  readdefinitionsandfeatures(features, &KeepDuplicates_, &definitionlist, &f);

  printf("\nsecond feature read\n");
  readdefinitionsandfeatures(features, &KeepDuplicates_, &definitionlist,
			     &filteredfeature);


  printf("\nfiltering\n");
  filterfeatures(f, &filteredfeature, &discardfeature, window);

  printf("\nwrite out filtered features\n");
  writefilteredfeatures(filteredfeature, features, bestfeatures,
			&filterednumber);

  if (asterisk) {
    printf("\nwrite out discarded features\n");
    writediscardedfeatures(&discardfeature, definitionlist, features,
			   lowerfeatures, &discardednumber);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  lowerfeatures.f = NULL;
  strcpy(lowerfeatures.name, "lowerfeatures");
  bestfeatures.f = NULL;
  strcpy(bestfeatures.name, "bestfeatures");
  localbestp.f = NULL;
  strcpy(localbestp.name, "localbestp");
  features.f = NULL;
  strcpy(features.name, "features");
  themain(&features, &localbestp, &bestfeatures, &lowerfeatures);
_L1:
  if (features.f != NULL)
    fclose(features.f);
  if (localbestp.f != NULL)
    fclose(localbestp.f);
  if (bestfeatures.f != NULL)
    fclose(bestfeatures.f);
  if (lowerfeatures.f != NULL)
    fclose(lowerfeatures.f);
  exit(EXIT_SUCCESS);
}



/* End. */
