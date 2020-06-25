/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "subfeatures.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*



*/
#define updateversion   1.00



/*
















































*/



#define maxstring       2000



#define infofield       10
#define infodecim       6

#define nfield          8



#define minribl         (-2000)
#define maxribl         2000
#define defnegativeinfinity  (-1000)


/*
*/




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;




typedef enum {
  a, c, g, t
} base;



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

  /*
*/
  struct featuretype *duplicateprevious;
  /*



*/


  struct featuretype *next;
} featuretype;


Static _TEXT exonfeatures, subfeaturesp, subexonfeatures;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static boolean between(a_, b, c_)
double a_, b, c_;
{
  return (a_ <= b && b <= c_);
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
  /*



*/
  emptystring(ribbon);
  ribbon->previous = NULL;
  ribbon->next = NULL;
}


Static Void disposestring(ribbon)
stringDelila **ribbon;
{
  stringDelila *t_;

  t_ = *ribbon;

  while (t_->next != NULL)
    t_ = t_->next;

  while (t_->previous != NULL) {
    t_ = t_->previous;
    Free(t_->next);
  }
  Free(t_);
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


#define tabcharacter    9



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
    if (c_ == tabcharacter)
      c_ = ' ';
    s->letters[i] = c_;
  }
}

#undef tabcharacter



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
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


#define tabcharacter    9



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
      s->letters[i] = (Char)tabcharacter;
  }
}

#undef tabcharacter



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


#define pwid            8
#define pdec            5



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
  fprintf(afile->f, " %*.*f", pwid, pdec, WITH1->eh);
  fprintf(afile->f, " %*.*f", pwid, pdec, WITH1->es);
  fprintf(afile->f, " %*.*f", pwid, pdec, WITH1->eb);
  fprintf(afile->f, " %*.*f", pwid, pdec, WITH1->fh);
  fprintf(afile->f, " %*.*f", pwid, pdec, WITH1->fs);
  fprintf(afile->f, " %*.*f", pwid, pdec, WITH1->fb);
  fprintf(afile->f, " %*.*f\n", pwid, pdec, WITH1->spare);

  /*






*/


}

#undef pwid
#undef pdec



Static boolean equaldefinition(a_, b)
definetype *a_, *b;
{
  boolean same = true;
  long i, FORLIM;

  if (!equalstring(a_->nametag, b->nametag))
    return false;
  if (!equalstring(a_->background, b->background))
    return false;
  if (!equalstring(a_->negparts, b->negparts))
    return false;

  if (a_->min != b->min)
    return false;
  if (a_->max != b->max)
    return false;
  /*

*/
  if (a_->marks != b->marks)
    return true;
  FORLIM = a_->marks;
  for (i = 0; i < FORLIM; i++) {
    if (a_->locations[i] != b->locations[i]) {
      same = false;
/* p2c: subfeatures.p: Note: Eliminated unused assignment statement [338] */
/* p2c: subfeatures.p: Note: Eliminated unused assignment statement [338] */
/* p2c: subfeatures.p: Note: Eliminated unused assignment statement [338] */
    }
  }
/* p2c: subfeatures.p: Note: Eliminated unused assignment statement [338] */
/* p2c: subfeatures.p: Note: Eliminated unused assignment statement [338] */
  return same;
  /*






*/
}



Static Void showdefinitions(afile, definitionlist)
_TEXT *afile;
definetype *definitionlist;
{
  definetype *f = definitionlist;

  while (f != NULL) {
/* p2c: subfeatures.p: Note: Eliminated unused assignment statement [338] */
    showadefinition(afile, f);
    putc('\n', afile->f);
    f = f->next;
  }
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

  if (f->definition == NULL) {
    fprintf(afile->f, " %*.*f", infofield, infodecim, 0.0);
    fprintf(afile->f, " %*.*f", infofield, infodecim, 0.0);
    fprintf(afile->f, " %*.*f", infofield, infodecim, 0.0);
  }
  /*









*/
  else {
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
/* p2c: subfeatures.p: Note: Eliminated unused assignment statement [338] */
    putc('\n', afile->f);
    showafeature(afile, f);
    f = f->next;
  }
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

  if (!debugging)
    return;


  TEMP.f = stdout;
  *TEMP.name = '\0';
  /*

*/
  showadefinition(&TEMP, *d);
  /*




*/
  if ((*d)->isgap)
    printf("Is a gap\n");
  else
    printf("Is NOT a gap\n");
  if ((*d)->istotal)
    printf("Is a total\n");
  else
    printf("Is NOT a total\n");
  printf("IsGapOrIsTotal: end &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
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



Static Void cleardefinition(d)
definetype **d;
{
  long l;
  definetype *WITH;
  petaltype *WITH1;

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

  WITH1 = &WITH->petal;

  WITH1->el = 0;
  WITH1->c_ = ' ';
  WITH1->eh = 0.0;
  WITH1->es = 0.0;
  WITH1->eb = 0.0;
  WITH1->fh = 0.0;
  WITH1->fs = 0.0;
  WITH1->fb = 0.0;
  WITH1->spare = 0.0;
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
  WITH->duplicateprevious = NULL;

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

  /*

















*/



  d = definitionlist;
  while (d != NULL) {
    if (equalstring(d->nametag, (*V.f)->nametag))
      WITH->definition = d;
    d = d->next;
  }
  if (WITH->definition == NULL) {
    printf("No definition was found for feature ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writequotestring(&TEMP, WITH->nametag);
    putchar('\n');
    reportandhalt(&V);
  }
  d = WITH->definition;




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



Static Void themain(exonfeatures, subfeaturesp, subexonfeatures)
_TEXT *exonfeatures, *subfeaturesp, *subexonfeatures;
{
  double parameterversion;
  definetype *d;
  long drcounter = 0;
  featuretype *f;
  long frcounter = 0, lcounter = 0;
  long mincoo, maxcoo;
  long wcounter = 0;
  _TEXT TEMP;

  printf("subfeatures %4.2f\n", version);
  if (*subfeaturesp->name != '\0') {
    if (subfeaturesp->f != NULL)
      subfeaturesp->f = freopen(subfeaturesp->name, "r", subfeaturesp->f);
    else
      subfeaturesp->f = fopen(subfeaturesp->name, "r");
  } else
    rewind(subfeaturesp->f);
  if (subfeaturesp->f == NULL)
    _EscIO2(FileNotFound, subfeaturesp->name);
  RESETBUF(subfeaturesp->f, Char);
  fscanf(subfeaturesp->f, "%lg%*[^\n]", &parameterversion);
  getc(subfeaturesp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }
  fscanf(subfeaturesp->f, "%ld%*[^\n]", &mincoo);
  getc(subfeaturesp->f);
  fscanf(subfeaturesp->f, "%ld%*[^\n]", &maxcoo);
  getc(subfeaturesp->f);
  printf("minimum coordinate: %ld\n", mincoo);
  printf("maximum coordinate: %ld\n", maxcoo);

  if (*subexonfeatures->name != '\0') {
    if (subexonfeatures->f != NULL)
      subexonfeatures->f = freopen(subexonfeatures->name, "w",
				   subexonfeatures->f);
    else
      subexonfeatures->f = fopen(subexonfeatures->name, "w");
  } else {
    if (subexonfeatures->f != NULL)
      rewind(subexonfeatures->f);
    else
      subexonfeatures->f = tmpfile();
  }
  if (subexonfeatures->f == NULL)
    _EscIO2(FileNotFound, subexonfeatures->name);
  SETUPBUF(subexonfeatures->f, Char);
  fprintf(subexonfeatures->f, "* subfeatures %4.2f\n", version);
  fprintf(subexonfeatures->f, "* minimum coordinate: %ld\n", mincoo);
  fprintf(subexonfeatures->f, "* maximum coordinate: %ld\n", maxcoo);

  f = (featuretype *)Malloc(sizeof(featuretype));
  d = (definetype *)Malloc(sizeof(definetype));
  clearfeature(&f);
  cleardefinition(&d);

  if (*exonfeatures->name != '\0') {
    if (exonfeatures->f != NULL)
      exonfeatures->f = freopen(exonfeatures->name, "r", exonfeatures->f);
    else
      exonfeatures->f = fopen(exonfeatures->name, "r");
  } else
    rewind(exonfeatures->f);
  if (exonfeatures->f == NULL)
    _EscIO2(FileNotFound, exonfeatures->name);
  RESETBUF(exonfeatures->f, Char);
  while (!BUFEOF(exonfeatures->f)) {
    if (P_peek(exonfeatures->f) == '*') {
      copyaline(exonfeatures, subexonfeatures);

    } else if (P_peek(exonfeatures->f) == '@') {
      readafeature(exonfeatures, d, &f);
      fscanf(exonfeatures->f, "%*[^\n]");

      getc(exonfeatures->f);
      if (between((double)mincoo,
		  f->definition->min * f->orientation + f->coordinate,
		  (double)maxcoo) | between((double)mincoo,
	    f->definition->max * f->orientation + f->coordinate,
	    (double)maxcoo)) {
	writeadefinition(subexonfeatures, d);
	writeafeature(subexonfeatures, f);
	putc('\n', subexonfeatures->f);
	wcounter++;
      }

      frcounter++;
    } else if (P_peek(exonfeatures->f) == 'd') {
      readadefinition(exonfeatures, &d);
      fscanf(exonfeatures->f, "%*[^\n]");
      getc(exonfeatures->f);
      drcounter++;
    } else {
      printf("WARNING: unidentified symbol at line %ld:\n", lcounter);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      copyaline(exonfeatures, &TEMP);
    }

    /*

*/
    lcounter++;
  }

  printf("%ld lines read\n", lcounter);
  printf("%ld features read\n", frcounter);
  printf("%ld definitions read\n", drcounter);
  printf("%ld definitions and features written\n", wcounter);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  subexonfeatures.f = NULL;
  strcpy(subexonfeatures.name, "subexonfeatures");
  subfeaturesp.f = NULL;
  strcpy(subfeaturesp.name, "subfeaturesp");
  exonfeatures.f = NULL;
  strcpy(exonfeatures.name, "exonfeatures");
  themain(&exonfeatures, &subfeaturesp, &subexonfeatures);
_L1:
  if (exonfeatures.f != NULL)
    fclose(exonfeatures.f);
  if (subfeaturesp.f != NULL)
    fclose(subfeaturesp.f);
  if (subexonfeatures.f != NULL)
    fclose(subexonfeatures.f);
  exit(EXIT_SUCCESS);
}



/* End. */
