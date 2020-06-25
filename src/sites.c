/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "sites.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         8.09
/*


*/



/*


























































































































































*/



#define maxstring       800
#define maxseq          100
#define maxentries      1000
#define primerlength    4
#define fillermax       21



#define negativeinfinity  (-1000000L)
/*
*/

#define scaleup         10000
/*

*/
#define posfield        4
#define infofield       8
#define infodecim       5
#define nfield          4



typedef struct rstype {
  long rstart, rstop, l, nal, ncl, ngl, ntl;
  double rsl, rs, varhnb, sumvar;
  long nl;
  double ehnb;
  Char flag;
} rstype;



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;



/*


*/
typedef Char filler[fillermax];



typedef enum {
  a, c, g, t, x
} base;

/*


*/

typedef struct entry_ {
  stringDelila seq;

  stringDelila name;
  long number;
  stringDelila primer;

  stringDelila ex, date;

  struct entry_ *next;
} entry_;

typedef entry_ *entries[maxentries];

typedef struct coordinates {
  /*


*/

  long fromrange, torange, lengthrange;

  long fromrandom, torandom, lengthrandom;

  double fracan;
  /*
*/
  double notfracan;
} coordinates;

typedef struct ilarray {
  long data[maxseq];
  coordinates coor;
} ilarray;

typedef struct iblarray {
  long data[(long)t - (long)a + 1][maxseq];
  coordinates coor;
} iblarray;

typedef struct rlarray {
  double data[maxseq];
  coordinates coor;
} rlarray;

typedef struct rblarray {
  double data[(long)t - (long)a + 1][maxseq];
  coordinates coor;
} rblarray;




Static _TEXT database, tables, caps, latex, list, sorted, standard, stats,
	     rsdata, sequ, makebkp;


Static jmp_buf _JL1;



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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



Static Char capitalize(c_)
Char c_;
{
  long n = c_;

  if (n >= 'a' && n <= 'z')
    c_ = _toupper(n);

  return c_;
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



/*




*/

Static Void resettrigger(t_)
trigger *t_;
{
  t_->state = 0;
  t_->skip = false;
  t_->found = false;
}


Static Void testfortrigger(ch, t_)
Char ch;
trigger *t_;
{
  /*




*/
  t_->state++;
  /*



*/
  if (t_->seek.letters[t_->state - 1] == ch) {
    t_->skip = false;
    if (t_->state == t_->seek.length)
      t_->found = true;
    else
      t_->found = false;
    return;
  }
  t_->state = 0;
  t_->skip = true;
  t_->found = false;
}



Static Void fillstring(s, a_)
stringDelila *s;
Char *a_;
{
  /*
*/


  /*


*/
  long length = fillermax;
  long index;

  clearstring(s);
  while (length > 1 && a_[length-1] == ' ')
    length--;
  if (length == 1 && a_[length-1] == ' ') {
    printf("fillstring: the string is empty\n");
    halt();
  }

  for (index = 0; index < length; index++)
    s->letters[index] = a_[index];
  s->length = length;
  s->current = 1;
}



Static Void filltrigger(t_, a_)
trigger *t_;
Char *a_;
{
  fillstring(&t_->seek, a_);
}






Static Void readrsrange(rsdata, r)
_TEXT *rsdata;
rstype *r;
{
  /*
*/
  long index;
  Char skip;

  for (index = 1; index <= 11; index++) {
    fscanf(rsdata->f, "%*[^\n]");
    getc(rsdata->f);
  }
  fscanf(rsdata->f, "%c%ld%ld%*[^\n]", &skip, &r->rstart, &r->rstop);
  getc(rsdata->f);

  if (skip == '\n')
    skip = ' ';
}



Static Void getrsbegin(infile)
_TEXT *infile;
{
  /*
*/
  Char ch;
  trigger begindata;


  filltrigger(&begindata, "l    a    c    g    t");
  resettrigger(&begindata);

  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else
    rewind(infile->f);
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);
  while (!begindata.found) {
    if (P_eoln(infile->f)) {
      fscanf(infile->f, "%*[^\n]");
      getc(infile->f);
    }
    if (BUFEOF(infile->f)) {
      printf("beginning of data not found\n");
      halt();
    }
    ch = getc(infile->f);
    if (ch == '\n')
      ch = ' ';
    testfortrigger(ch, &begindata);
  }
  fscanf(infile->f, "%*[^\n]");
  getc(infile->f);
}



Static Void readrsdata(rsdata, rdata)
_TEXT *rsdata;
rstype *rdata;
{
  fscanf(rsdata->f, "%ld%ld%ld%ld%ld%lg%lg%lg%lg%ld%lg", &rdata->l,
	 &rdata->nal, &rdata->ncl, &rdata->ngl, &rdata->ntl, &rdata->rsl,
	 &rdata->rs, &rdata->varhnb, &rdata->sumvar, &rdata->nl,
	 &rdata->ehnb);

  while (P_peek(rsdata->f) == ' ')
    getc(rsdata->f);
  fscanf(rsdata->f, "%c%*[^\n]", &rdata->flag);
  getc(rsdata->f);
  /*
*/
  if (rdata->flag == '\n')
    rdata->flag = ' ';
}


/* Local variables for numberdigit: */
struct LOC_numberdigit {
  long number, place, absolute;
  Char acharacter;
} ;

Local Void digit(LINK)
struct LOC_numberdigit *LINK;
{
  long tenplace, z, d;

  tenplace = LINK->place * 10;
  z = LINK->absolute - LINK->absolute / tenplace * tenplace;
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








Static Char numberdigit(number_, logplace)
long number_, logplace;
{
  /*



*/
  struct LOC_numberdigit V;
  long count;

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
  V.absolute = labs(V.number);
  if (V.absolute < V.place / 10) {
    V.acharacter = ' ';
    return V.acharacter;
  }
  if (V.absolute >= V.place)
    digit(&V);
  else
    sign(&V);
  return V.acharacter;
}


#define ln10            2.30259
#define epsilon         0.00001



Static long numbersize(n)
long n;
{
  if (n == 0)
    return 1;
  else {
    return ((long)(log((double)labs(n)) / ln10 + epsilon) + 2);
    /*


*/

  }
}

#undef ln10
#undef epsilon



Static Void numberbar(afile, spaces, firstnumber, lastnumber, linesused)
_TEXT *afile;
long spaces, firstnumber, lastnumber, *linesused;
{
  /*
*/
  long logplace, spacecount, number;

  if (labs(firstnumber) > labs(lastnumber))
    *linesused = numbersize(firstnumber);
  else
    *linesused = numbersize(lastnumber);

  for (logplace = *linesused - 1; logplace >= 0; logplace--) {
    for (spacecount = 1; spacecount <= spaces; spacecount++)
      putc(' ', afile->f);
    for (number = firstnumber; number <= lastnumber; number++)
      fputc(numberdigit(number, logplace), afile->f);
    putc('\n', afile->f);
  }
}





Static base chartorange(ch)
Char ch;
{
  base Result;

  if (ch != 'x' && ch != 't' && ch != 'g' && ch != 'c' && ch != 'a') {
    printf("chartorange: %c is not a base\n", ch);
    halt();
    return Result;
  }
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

  case 'x':
    Result = x;
    break;
  }
  return Result;
}



Static Char basetochar(b)
base b;
{
  Char Result;

  switch (b) {

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

  case x:
    Result = 'x';
    break;
  }
  return Result;
}



Static Void makenumber(name, number, found)
stringDelila name;
long *number;
boolean *found;
{
  /*
*/
  long l;

  *found = false;
  *number = 0;
  for (l = 0; l < name.length; l++) {
    if (name.letters[l] == '9' || name.letters[l] == '8' ||
	name.letters[l] == '7' || name.letters[l] == '6' ||
	name.letters[l] == '5' || name.letters[l] == '4' ||
	name.letters[l] == '3' || name.letters[l] == '2' ||
	name.letters[l] == '1' || name.letters[l] == '0') {
      *found = true;
      *number *= 10;
      switch (name.letters[l]) {

      case '0':
	/* blank case */
	break;

      case '1':
	(*number)++;
	break;

      case '2':
	*number += 2;
	break;

      case '3':
	*number += 3;
	break;

      case '4':
	*number += 4;
	break;

      case '5':
	*number += 5;
	break;

      case '6':
	*number += 6;
	break;

      case '7':
	*number += 7;
	break;

      case '8':
	*number += 8;
	break;

      case '9':
	*number += 9;
	break;
      }
    }
  }
}



Static Void getstring(f, s, spaces)
_TEXT *f;
stringDelila *s;
boolean spaces;
{
  /*


*/
  Char c_ = 'x';

  s->length = 0;
  while ((c_ != ' ' || spaces) & (!P_eoln(f->f))) {
    c_ = getc(f->f);

    if (c_ == '\n')
      c_ = ' ';
    if (!(c_ != ' ' || spaces))
      continue;
    s->length++;
    if (s->length > maxstring) {
      printf("a string being read is bigger than %ld\n", (long)maxstring);
      printf("increase constant maxstring\n");
      halt();
    }
    s->letters[s->length - 1] = c_;
  }
}



Static Void putstring(f, s)
_TEXT *f;
stringDelila *s;
{
  long l, FORLIM;

  FORLIM = s->length;
  for (l = 0; l < FORLIM; l++)
    putc(s->letters[l], f->f);
}



Static Void copystring(a_, b)
stringDelila a_, *b;
{
  long l;

  b->length = a_.length;
  for (l = 0; l < a_.length; l++)
    b->letters[l] = a_.letters[l];
}



Static boolean equalstring(a_, b)
stringDelila a_, b;
{
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



Static boolean lessstring(a_, b)
stringDelila a_, b;
{
  boolean Result;
  boolean done = false;
  long l = 0;


  do {
    l++;

    if (l > a_.length) {
      done = true;
      Result = false;
    } else if (b.letters[l-1] != a_.letters[l-1]) {
      done = true;
      Result = (b.letters[l-1] > a_.letters[l-1]);
    }
  } while (!done);
  return Result;
}



Static boolean equalseq(a_, b)
stringDelila a_, b;
{
  /*
*/
  long p;
  boolean failure;

  if (a_.length != b.length)
    return false;
  else {
    failure = false;
    p = 1;
    do {
      if (a_.letters[p-1] != 'x' && b.letters[p-1] != 'x') {
	if (a_.letters[p-1] != b.letters[p-1])
	  failure = true;
      }

      p++;
    } while (!(p > a_.length || failure));

    return (!failure);
  }
}



Static Void diffseq(f, a_, b)
_TEXT *f;
stringDelila a_, b;
{
  long p = 1;

  if (a_.length != b.length) {
    fprintf(f->f, "sequences are different in length\n");
    return;
  }
  for (p = 0; p < a_.length; p++) {
    if (a_.letters[p] != 'x' && b.letters[p] != 'x') {
      if (a_.letters[p] != b.letters[p])
	putc('^', f->f);
      else
	putc(' ', f->f);
    } else {
      putc(' ', f->f);
      /*



*/
    }
  }
  putc('\n', f->f);
}



Static Void identify(f, e)
_TEXT *f;
entry_ *e;
{
  fprintf(f->f, "seq: \"");
  putstring(f, &e->seq);
  fprintf(f->f, "\"\n");
  fprintf(f->f, "name: \"");
  putstring(f, &e->name);
  fprintf(f->f, "\"\n");
  fprintf(f->f, "primer: \"");
  putstring(f, &e->primer);
  fprintf(f->f, "\"\n");
  fprintf(f->f, "experiment: \"");
  putstring(f, &e->ex);
  fprintf(f->f, "\"\n");
  fprintf(f->f, "date: \"");
  putstring(f, &e->date);
  fprintf(f->f, "\"\n");
}



Static Void getentry(f, e, std, gotten)
_TEXT *f;
entry_ **e, *std;
boolean *gotten;
{
  /*



*/
  long extra;
  boolean foundnumber;
  long l = 1;
  entry_ *WITH;
  _TEXT TEMP;
  long FORLIM;

  WITH = *e;
  getstring(f, &WITH->seq, false);
  getstring(f, &WITH->name, false);
  makenumber(WITH->name, &WITH->number, &foundnumber);
  if (!foundnumber) {
    if (std == NULL)
      WITH->number = 0;
    else {
      printf("getentry: sequence:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      putstring(&TEMP, &WITH->seq);
      printf("\nhas a name:\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      putstring(&TEMP, &WITH->name);
      printf("\nwhich cannot be converted to a number.\n");
      printf("The sites program requires a plasmid number for\n");
      printf("every database entry except the canonical standard.\n");
      halt();
    }
  }


  getstring(f, &WITH->primer, false);

  if (WITH->primer.length < primerlength) {
    for (extra = WITH->primer.length; extra < primerlength; extra++)
      WITH->primer.letters[extra] = ' ';
    WITH->primer.length = primerlength;
  }

  getstring(f, &WITH->ex, false);
  getstring(f, &WITH->date, true);
  WITH->next = NULL;


  *gotten = true;
  FORLIM = WITH->seq.length;
  for (l = 0; l < FORLIM; l++) {
    if (WITH->seq.letters[l] != '.' && WITH->seq.letters[l] != 'd' &&
	WITH->seq.letters[l] != 'x' && WITH->seq.letters[l] != 't' &&
	WITH->seq.letters[l] != 'g' && WITH->seq.letters[l] != 'c' &&
	WITH->seq.letters[l] != 'a') {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      identify(&TEMP, *e);
      printf("non \"acgtxd.\" character found in sequence\n");
      halt();
    }

    if (WITH->seq.letters[l] == '.') {
      if (std == NULL) {
	printf("A period (.) cannot be used in a standard sequence.\n");
	halt();
      }

      WITH->seq.letters[l] = std->seq.letters[l];
    }

    if (*gotten) {
      if (WITH->seq.letters[l] == 'd') {
	/*

*/
	*gotten = false;
      }
    }
  }

  /*




*/
  fscanf(f->f, "%*[^\n]");
  getc(f->f);
}



Static Void putid(f, s)
_TEXT *f;
entry_ *s;
{
  putstring(f, &s->name);
  putc(' ', f->f);
  putstring(f, &s->primer);
  putc(' ', f->f);
  putstring(f, &s->ex);
  putc(' ', f->f);
  putstring(f, &s->date);
}



Static Void putentry(f, s)
_TEXT *f;
entry_ *s;
{
  putstring(f, &s->seq);
  putc(' ', f->f);
  putid(f, s);
  putc('\n', f->f);
}



Static Void clearcoor(c_)
coordinates *c_;
{
  c_->fromrange = 0;
  c_->torange = 0;
  c_->fromrandom = 0;
  c_->torandom = 0;
  c_->lengthrange = 0;
  c_->lengthrandom = 0;
  c_->fracan = 0.0;
  c_->notfracan = 0.0;
}



Static Void clearibl(fbl)
iblarray *fbl;
{
  base b;
  long l;
  coordinates *WITH;

  WITH = &fbl->coor;
  for (l = 0; l < maxseq; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fbl->data[(long)b - (long)a][l] = 0;
  }
  clearcoor(&fbl->coor);
}



Static Void clearrbl(fbl)
rblarray *fbl;
{
  base b;
  long l;
  coordinates *WITH;

  WITH = &fbl->coor;
  for (l = 0; l < maxseq; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fbl->data[(long)b - (long)a][l] = 0.0;
  }
  clearcoor(&fbl->coor);
}



Static Void copycoords(a_, b)
coordinates a_, *b;
{
  b->fromrange = a_.fromrange;
  b->torange = a_.torange;
  b->lengthrange = a_.lengthrange;
  b->fromrandom = a_.fromrandom;
  b->torandom = a_.torandom;
  b->lengthrandom = a_.lengthrandom;
  b->fracan = a_.fracan;
  b->notfracan = a_.notfracan;
}



Static Void copyrbl(Aarray, Barray)
rblarray Aarray, *Barray;
{
  base b;
  long l;

  copycoords(Aarray.coor, &Barray->coor);
  for (l = 0; l < Aarray.coor.lengthrange; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      Barray->data[(long)b - (long)a][l] = Aarray.data[(long)b - (long)a][l];
  }
}



Static Void makenotfracan(fracan, notfracan)
double fracan, *notfracan;
{
  /*

*/
  *notfracan = (1.0 - fracan) / 3.0;
}



Static Void setcoordinates(a_)
coordinates *a_;
{
  /*
*/
  a_->lengthrange = a_->torange - a_->fromrange + 1;
  a_->lengthrandom = a_->torandom - a_->fromrandom + 1;
  makenotfracan(a_->fracan, &a_->notfracan);
}



Static Void showiblarray(afile, ar)
_TEXT *afile;
iblarray ar;
{
  base b;
  long coordinate, l;

  for (coordinate = ar.coor.fromrange; coordinate <= ar.coor.torange; coordinate++) {
    l = coordinate - ar.coor.fromrange + 1;
    fprintf(afile->f, "%4ld ", coordinate);
    if (coordinate >= ar.coor.fromrandom && coordinate <= ar.coor.torandom)
      putc('R', afile->f);
    else
      putc(' ', afile->f);
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fprintf(afile->f, " %5ld", ar.data[(long)b - (long)a][l-1]);
    putc('\n', afile->f);
  }
}



Static Void showrblarray(afile, ar)
_TEXT *afile;
rblarray ar;
{
  base b;
  long coordinate, l;

  for (coordinate = ar.coor.fromrange; coordinate <= ar.coor.torange; coordinate++) {
    l = coordinate - ar.coor.fromrange + 1;
    fprintf(afile->f, "%4ld ", coordinate);
    if (coordinate >= ar.coor.fromrandom && coordinate <= ar.coor.torandom)
      putc('R', afile->f);
    else
      putc(' ', afile->f);
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fprintf(afile->f, " %5.2f", ar.data[(long)b - (long)a][l-1]);
    putc('\n', afile->f);
  }
}



Static Void getrsdata(rsdata, fbl)
_TEXT *rsdata;
iblarray *fbl;
{
  /*
*/
  long l;
  rstype rdata;
  long position;
  coordinates *WITH;
  long FORLIM;

  if (*rsdata->name != '\0') {
    if (rsdata->f != NULL)
      rsdata->f = freopen(rsdata->name, "r", rsdata->f);
    else
      rsdata->f = fopen(rsdata->name, "r");
  } else
    rewind(rsdata->f);
  if (rsdata->f == NULL)
    _EscIO2(FileNotFound, rsdata->name);
  RESETBUF(rsdata->f, Char);
  clearibl(fbl);

  if (BUFEOF(rsdata->f)) {
    printf("empty standard rsdata file\n");
    halt();
    return;
  }
  /*
*/
  readrsrange(rsdata, &rdata);
  getrsbegin(rsdata);


  WITH = &fbl->coor;
  WITH->fromrange = rdata.rstart;
  WITH->torange = rdata.rstop;
  WITH->fromrandom = WITH->fromrange;
  WITH->torandom = WITH->torange;
  WITH->fracan = 0.25;
  setcoordinates(&fbl->coor);


  FORLIM = rdata.rstop + 1;
  for (position = rdata.rstart + 1; position <= FORLIM; position++) {
    if (P_peek(rsdata->f) != '*') {
      readrsdata(rsdata, &rdata);
      l = position - rdata.rstart;


      fbl->data[0][l-1] = rdata.nal;
      fbl->data[(long)c - (long)a][l-1] = rdata.ncl;
      fbl->data[(long)g - (long)a][l-1] = rdata.ngl;
      fbl->data[(long)t - (long)a][l-1] = rdata.ntl;

      /*










*/
    }
  }

}



Static Void announcement(list, header)
_TEXT *list;
stringDelila header;
{
  fprintf(list->f, "sites %4.2f\n", version);
  fprintf(list->f, "the database is:\n");
  putstring(list, &header);
  putc('\n', list->f);
}



Static Void mkheader(list, header, experiment, coords, std)
_TEXT *list;
stringDelila header;
long experiment;
coordinates coords;
entry_ *std;
{
  long l;

  if (experiment == 1)
    announcement(list, header);

  fprintf(list->f, "\nExperiment %ld\n", experiment);
  fprintf(list->f, " range:  from %4ld to %4ld\n",
	  coords.fromrange, coords.torange);
  fprintf(list->f, " random: from %4ld to %4ld\n",
	  coords.fromrandom, coords.torandom);
  fprintf(list->f, " fraction canonical: %4.2f\n\n", coords.fracan);


  numberbar(list, 0L, coords.fromrange, coords.torange, &l);
  putentry(list, std);
}



Static Void dataheader(list, header, std)
_TEXT *list;
stringDelila header;
entry_ *std;
{
  fprintf(list->f, "* sites %4.2f\n", version);

  fprintf(list->f, "* the database is: ");
  putstring(list, &header);
  fprintf(list->f, "\n* ");

  putid(list, std);
  putc('\n', list->f);
}



Static Void crunch(data, numentries, standard, symbol)
entry_ **data;
long numentries;
entry_ *standard;
Char symbol;
{
  /*

*/
  long l, n, FORLIM1;

  for (n = 0; n < numentries; n++) {
    FORLIM1 = data[n]->seq.length;
    for (l = 0; l < FORLIM1; l++) {
      if (data[n]->seq.letters[l] == standard->seq.letters[l])
	data[n]->seq.letters[l] = symbol;
    }
  }
}



Static Void uncrunch(data, numentries, standard, symbol)
entry_ **data;
long numentries;
entry_ *standard;
Char symbol;
{
  long l, n, FORLIM1;

  for (n = 0; n < numentries; n++) {
    FORLIM1 = data[n]->seq.length;
    for (l = 0; l < FORLIM1; l++) {
      if (data[n]->seq.letters[l] == symbol)
	data[n]->seq.letters[l] = standard->seq.letters[l];
    }
  }
}


typedef long position;



/* Local variables for sortdata: */
struct LOC_sortdata {
  entry_ **data;
  boolean onname;
} ;

Local boolean lessthan(a_, b, LINK)
position a_, b;
struct LOC_sortdata *LINK;
{
  if (LINK->onname)
    return (LINK->data[a_-1]->number < LINK->data[b-1]->number);
  else
    return (lessstring(LINK->data[a_-1]->seq, LINK->data[b-1]->seq));
}

Local Void swap_(a_, b, LINK)
position a_, b;
struct LOC_sortdata *LINK;
{
  entry_ *hold;

  hold = LINK->data[a_-1];
  LINK->data[a_-1] = LINK->data[b-1];
  LINK->data[b-1] = hold;
}



Local Void quicksort(left, right, LINK)
position left, right;
struct LOC_sortdata *LINK;
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



Static Void sortdata(data_, numentries, onname_)
entry_ **data_;
long numentries;
boolean onname_;
{
  /*
*/
  struct LOC_sortdata V;




  V.data = data_;
  V.onname = onname_;
  quicksort(1L, numentries, &V);
}



Static Void readheader(database, header)
_TEXT *database;
stringDelila *header;
{
  if (*database->name != '\0') {
    if (database->f != NULL)
      database->f = freopen(database->name, "r", database->f);
    else
      database->f = fopen(database->name, "r");
  } else
    rewind(database->f);
  if (database->f == NULL)
    _EscIO2(FileNotFound, database->name);
  RESETBUF(database->f, Char);
  getstring(database, header, true);
  fscanf(database->f, "%*[^\n]");
  getc(database->f);
}



Static Void readexpt(database, coords, std)
_TEXT *database;
coordinates *coords;
entry_ **std;
{
  /*

*/
  boolean gotten;
  Char skip;

  if (P_peek(database->f) != '@') {
    printf("PROGRAM ERROR: @ missing\n");
    halt();
  }
  fscanf(database->f, "%c%ld%ld%ld%ld%lg%*[^\n]", &skip, &coords->fromrange,
	 &coords->torange, &coords->fromrandom, &coords->torandom,
	 &coords->fracan);
  getc(database->f);
  if (skip == '\n')
    skip = ' ';
  setcoordinates(coords);


  if (*std == NULL)
    *std = (entry_ *)Malloc(sizeof(entry_));
  getentry(database, std, NULL, &gotten);

  if (!gotten) {
    printf("The standard must not contain deletion bases (d).\n");
    halt();
  }

  if ((*std)->seq.length != coords->torange - coords->fromrange + 1) {
    printf(
      "the length of the standard is not consistent with the given coordinates\n");
    halt();
  }

  if (coords->fromrange > coords->torange) {
    printf("fromrange (%ld) is greater than torange (%ld)!\n",
	   coords->fromrange, coords->torange);
    halt();
  }

  if (coords->fromrandom < coords->fromrange) {
    printf("fromrandom (%ld) is less than fromrange (%ld)!\n",
	   coords->fromrandom, coords->fromrange);
    printf("Randomized positions must be within the region sequenced\n");
    halt();
  }

  if (coords->torandom > coords->torange) {
    printf("torandom (%ld) is greater than torange (%ld)!\n",
	   coords->torandom, coords->torange);
    printf("Randomized positions must be within the region sequenced\n");
    halt();
  }

  if (coords->fromrandom > coords->torandom) {
    printf("fromrandom  (%ld) is greater than torandom (%ld)!\n",
	   coords->fromrandom, coords->torandom);
    halt();
  }

  if ((unsigned)coords->fracan > 1.0) {
    printf("fraction canonical (%4.2f) must be between 0 and 1!\n",
	   coords->fracan);
    halt();
  }
}



Static Void findextent(database, global)
_TEXT *database;
rblarray *global;
{
  coordinates c_;
  long experiments = 0;
  entry_ *std = NULL;
  coordinates *WITH;

  clearrbl(global);
  WITH = &global->coor;


  WITH->fromrange = LONG_MAX;
  WITH->torange = -LONG_MAX;
  WITH->fromrandom = LONG_MAX;
  WITH->torandom = -LONG_MAX;
  printf("\nChecking Experiments\n");
  while (!BUFEOF(database->f)) {
    if (P_peek(database->f) != '@') {
      fscanf(database->f, "%*[^\n]");
      getc(database->f);
      continue;
    }
    experiments++;
    printf("Experiment %ld\n", experiments);

    readexpt(database, &c_, &std);


    if (c_.fromrange < WITH->fromrange)
      WITH->fromrange = c_.fromrange;
    if (c_.torange > WITH->torange)
      WITH->torange = c_.torange;
    if (c_.fromrandom < WITH->fromrandom)
      WITH->fromrandom = c_.fromrandom;
    if (c_.torandom > WITH->torandom)
      WITH->torandom = c_.torandom;
  }
  if (experiments == 0) {
    printf("No experiments found in database!\n");
    printf("(perhaps you forgot the @ and standard lines?)\n");
    halt();
  }
  if (WITH->fromrange == LONG_MAX || WITH->torange == -LONG_MAX) {
    printf("The experimental information is missing!\n");
    halt();
  }
  printf("      overall global range: %ld to %ld\n",
	 WITH->fromrange, WITH->torange);
  printf("overall randomized regions: %ld to %ld\n",
	 WITH->fromrandom, WITH->torandom);
  global->coor.fracan = c_.fracan;
  setcoordinates(&global->coor);

  Free(std);
}



Static Void readdata(database, data, std, numentries)
_TEXT *database;
entry_ **data;
entry_ *std;
long *numentries;
{
  /*
*/
  long d = 0;
  entry_ *datum;
  boolean done = false;
  boolean gotten;
  _TEXT TEMP;

  *numentries = 0;

  while (!done) {
    if (BUFEOF(database->f))
      done = true;
    if (!done) {
      if (P_peek(database->f) == '@')
	done = true;
    }
    if (done)
      break;

    (*numentries)++;
    d++;

    if (*numentries > maxentries) {
      printf("The number of entries exceeds %ld\n", (long)maxentries);
      printf("increase constant maxentries.\n");
      halt();
    }

    /*


*/
    if (data[*numentries - 1] == NULL)
      data[*numentries - 1] = (entry_ *)Malloc(sizeof(entry_));
    datum = data[*numentries - 1];

    getentry(database, &datum, std, &gotten);



    if (datum->seq.length != std->seq.length) {
      printf(
	"\nreaddata: length of sequence datum %ld does not equal the standard.\n",
	d);
      printf("standard: %5ld\n", std->seq.length);
      printf("   datum: %5ld\n", datum->seq.length);
      TEMP.f = stdout;
      *TEMP.name = '\0';
      identify(&TEMP, datum);
      printf("WARNING:  THIS SEQUENCE WILL BE SKIPPED\n\n");
      (*numentries)--;
    }


    /*






*/

    if (!gotten) {
      printf("WARNING: this entry was dropped because of deletions (\"d\"):\n");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      identify(&TEMP, data[*numentries - 1]);
      putchar('\n');

      (*numentries)--;
    }
  }
}



Static Void copyentry(a_, b)
entry_ *a_, **b;
{
  (*b)->seq.length = a_->seq.length;

  copystring(a_->name, &(*b)->name);
  (*b)->number = a_->number;
  copystring(a_->primer, &(*b)->primer);
  copystring(a_->ex, &(*b)->ex);
  copystring(a_->date, &(*b)->date);
}



Static Void capsequence(datum, naked, standard)
entry_ *datum, **naked, *standard;
{
  /*
*/
  long l, FORLIM;

  copyentry(datum, naked);

  FORLIM = datum->seq.length;
  for (l = 0; l < FORLIM; l++) {
    if (datum->seq.letters[l] != standard->seq.letters[l]) {
      switch (datum->seq.letters[l]) {

      case 'a':
	(*naked)->seq.letters[l] = 'A';
	break;

      case 'c':
	(*naked)->seq.letters[l] = 'C';
	break;

      case 'g':
	(*naked)->seq.letters[l] = 'G';
	break;

      case 't':
	(*naked)->seq.letters[l] = 'T';
	break;

      case 'x':
	(*naked)->seq.letters[l] = 'x';
	break;
      }
    } else
      (*naked)->seq.letters[l] = datum->seq.letters[l];
  }
}



Static Void strip(coords, datum, naked, standard, symbol, blank)
coordinates coords;
entry_ *datum, **naked, *standard;
Char symbol, blank;
{
  /*

*/
  long coordinate, l;

  copyentry(datum, naked);

  for (coordinate = coords.fromrange; coordinate <= coords.torange; coordinate++) {
    l = coordinate - coords.fromrange + 1;

    if (datum->seq.letters[l-1] == standard->seq.letters[l-1]) {
      if (coordinate >= coords.fromrandom && coordinate <= coords.torandom)
	(*naked)->seq.letters[l-1] = symbol;
      else
	(*naked)->seq.letters[l-1] = blank;
    } else
      (*naked)->seq.letters[l-1] = datum->seq.letters[l-1];
  }
}



Static Void checkdata(list, data, d, failures)
_TEXT *list;
entry_ **data;
long d, *failures;
{
  /*

*/
  /*



*/
  /*


*/
  /*










*/

  if (d <= 1)
    return;
  if (data[d-1]->number != data[d-2]->number)
    return;
  if (equalseq(data[d-1]->seq, data[d-2]->seq))
    return;
  diffseq(list, data[d-1]->seq, data[d-2]->seq);
  (*failures)++;

  fprintf(list->f, "DATA ERROR: sequence of \"");
  putid(list, data[d-2]);
  fprintf(list->f, "\"\n");

  fprintf(list->f, "         does not equal \"");
  putid(list, data[d-1]);
  fprintf(list->f, "\"\n");
}



Static Void figure(f, data, numentries, experiment, coords, std, kind, blank,
		   testforequal)
_TEXT *f;
entry_ **data;
long numentries, experiment;
coordinates coords;
entry_ *std;
Char kind, blank;
boolean testforequal;
{

  /*
*/
  /*






*/
  entry_ *datum;
  long duplicates = 0;
  long dummy;
  entry_ *naked;
  long number;

  fprintf(f->f, "Experiment %ld\n\n", experiment);
  numberbar(f, 0L, coords.fromrange, coords.torange, &dummy);
  putentry(f, std);
  naked = (entry_ *)Malloc(sizeof(entry_));
  for (number = 1; number <= numentries; number++) {
    datum = data[number-1];

    if (kind == 's') {
      if (testforequal) {
	if (number > 1) {
	  if (equalstring(datum->seq, data[number-2]->seq)) {
	    strip(coords, datum, &naked, std, '=', blank);
	    duplicates++;
	  } else
	    strip(coords, datum, &naked, std, '.', blank);
	} else
	  strip(coords, datum, &naked, std, '.', blank);
      } else
	strip(coords, datum, &naked, std, '.', blank);
    } else if (kind == 'c')
      capsequence(datum, &naked, std);


    putstring(f, &naked->seq);
    putc(' ', f->f);


    putstring(f, &naked->name);
    fprintf(f->f, " \n");
    /*




*/
  }

  Free(naked);
}


#define dashline        80


/* Local variables for display: */
struct LOC_display {
  _TEXT *f;
} ;

Local Void dashit(LINK)
struct LOC_display *LINK;
{
  long dashcount;

  for (dashcount = 1; dashcount <= dashline; dashcount++)
    putc('-', LINK->f->f);
  putc('\n', LINK->f->f);
}



Static Void display(f_, data, numentries, header, experiment, coords, std,
		    kind, blank, testforequal, duplicates)
_TEXT *f_;
entry_ **data;
long numentries;
stringDelila header;
long experiment;
coordinates coords;
entry_ *std;
Char kind, blank;
boolean testforequal;
long *duplicates;
{
  /*







*/
  struct LOC_display V;
  entry_ *datum;
  long failures = 0;
  entry_ *naked;
  long number;

  V.f = f_;
  mkheader(V.f, header, experiment, coords, std);
  *duplicates = 0;
  naked = (entry_ *)Malloc(sizeof(entry_));
  for (number = 1; number <= numentries; number++) {
    /*

*/
    datum = data[number-1];

    if (kind == 's') {
      if (testforequal) {
	if (number > 1) {
	  if (equalstring(datum->seq, data[number-2]->seq)) {
	    strip(coords, datum, &naked, std, '=', blank);
	    (*duplicates)++;
	  } else
	    strip(coords, datum, &naked, std, '.', blank);
	} else
	  strip(coords, datum, &naked, std, '.', blank);
      } else
	strip(coords, datum, &naked, std, '.', blank);
    } else if (kind == 'c') {
      capsequence(datum, &naked, std);

      if (number == 1)
	dashit(&V);
      else if (!equalstring(datum->name, data[number-2]->name))
	dashit(&V);
    }

    putentry(V.f, naked);

    checkdata(V.f, data, number, &failures);
  }

  if (failures > 0) {
    printf("\n%ld data errors:\n", failures);
    printf("inconsistant readings, no analysis will be done\n");
    halt();
  }

  Free(naked);
}

#undef dashline


/* Local variables for compress: */
struct LOC_compress {
  entry_ **data;
  entry_ *std;
  _TEXT *caps;
  Char f;
  long fillspot, l;
  Char r;
  long readspot;
} ;

Local Void death(LINK)
struct LOC_compress *LINK;
{
  printf("program error in compress:\n");
  printf("position in sequence:%2ld\n", LINK->l);
  printf("filling at %2ld, letter is: %c\n", LINK->fillspot, LINK->f);
  printf("reading at %2ld, letter is: %c\n", LINK->readspot, LINK->r);
  printf("this sequence difference was missed\n");
  halt();
}

Local Void warning(b, LINK)
Char b;
struct LOC_compress *LINK;
{
  if (b == LINK->std->seq.letters[LINK->l-1])
    return;
  printf("check sequence%12ld - see caps file\n",
	 LINK->data[LINK->fillspot-1]->number);
  fprintf(LINK->caps->f, "\nWARNING: sequence: ");
  putid(LINK->caps, LINK->data[LINK->fillspot-1]);
  fprintf(LINK->caps->f, "\nand sequence     : ");
  putid(LINK->caps, LINK->data[LINK->readspot-1]);
  fprintf(LINK->caps->f,
    "\ndiffer by an x, and the other base is mutant (%c).  Check that it IS mutant.\n",
    b);
}



Static Void compress(data_, numentries, std_, caps_)
entry_ **data_;
long *numentries;
entry_ *std_;
_TEXT *caps_;
{
  /*







*/
  struct LOC_compress V;
  long FORLIM;
  stringDelila *WITH;

  V.data = data_;
  V.std = std_;
  V.caps = caps_;
  V.fillspot = 1;
  V.readspot = 2;
  while (V.readspot <= *numentries) {
    if (V.data[V.fillspot-1]->number == V.data[V.readspot-1]->number) {
      FORLIM = V.data[V.fillspot-1]->seq.length;
      for (V.l = 1; V.l <= FORLIM; V.l++) {
	V.f = V.data[V.fillspot-1]->seq.letters[V.l-1];
	V.r = V.data[V.readspot-1]->seq.letters[V.l-1];

	if (V.f != V.r) {
	  if (V.f == 'x') {
	    V.data[V.fillspot-1]->seq.letters[V.l-1] = V.r;
	    warning(V.r, &V);
	  } else if (V.r != 'x')
	    death(&V);
	  else
	    warning(V.f, &V);
	}
      }


      if (!equalstring(V.data[V.fillspot-1]->primer,
		       V.data[V.readspot-1]->primer)) {
	WITH = &V.data[V.fillspot-1]->primer;
	WITH->letters[0] = 'b';
	WITH->letters[1] = 'o';
	WITH->letters[2] = 't';
	WITH->letters[3] = 'h';
	WITH->length = 4;
      }
    }

    else {
      V.fillspot++;
      V.data[V.fillspot-1] = V.data[V.readspot-1];
    }

    V.readspot++;
  }

  *numentries = V.fillspot;

  if (*numentries < maxentries)
    V.data[*numentries] = NULL;

  printf("compressed data set: %ld sequences\n", *numentries);
}



Static Void detfi(fo, std, fi)
rblarray fo;
entry_ *std;
rblarray *fi;
{
  /*
*/
  base b;
  long coordinate;
  boolean inrandom;
  long l;

  clearrbl(fi);
  copycoords(fo.coor, &fi->coor);
  for (coordinate = fo.coor.fromrandom;
       coordinate <= fo.coor.torandom;
       coordinate++) {
    l = coordinate - fo.coor.fromrange + 1;
    inrandom = (coordinate >= fo.coor.fromrandom &&
		coordinate <= fo.coor.torandom);
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      if (basetochar(b) == std->seq.letters[l-1]) {
	if (inrandom)
	  fi->data[(long)b - (long)a][l-1] = fo.coor.fracan;
	else
	  fi->data[(long)b - (long)a][l-1] = 1.0;
      } else if (inrandom)
	fi->data[(long)b - (long)a][l-1] = fo.coor.notfracan;
      else
	fi->data[(long)b - (long)a][l-1] = 0.0;
    }
  }
}



Static Void detrho(fi, fo, rho)
rblarray fi, fo, *rho;
{
  base b;
  long coordinate, l;

  clearrbl(rho);
  copycoords(fi.coor, &rho->coor);
  for (coordinate = fi.coor.fromrandom + 1;
       coordinate <= fi.coor.torandom + 1;
       coordinate++) {
    l = coordinate - fi.coor.fromrange;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      if (fi.data[(long)b - (long)a][l-1] == 0.0)
	printf("detrho: 7 zowie! PROGRAM ERROR!\n");
      if (fi.data[(long)b - (long)a][l-1] > 0.0)
	rho->data[(long)b - (long)a]
	  [l-1] = fo.data[(long)b - (long)a][l-1] / fi.data[(long)b - (long)a]
		  [l-1];
      else if (fo.data[(long)b - (long)a][l-1] == fi.data[(long)b - (long)a]
	       [l-1])
	rho->data[(long)b - (long)a][l-1] = 1.0;
      else
	rho->data[(long)b - (long)a][l-1] = 0.0;
    }
  }
}



Static Void detfpi(fi, fpi)
rblarray fi, *fpi;
{
  /*
*/
  base b;
  long coordinate, l;

  clearrbl(fpi);
  copycoords(fi.coor, &fpi->coor);
  for (coordinate = fi.coor.fromrandom + 1;
       coordinate <= fi.coor.torandom + 1;
       coordinate++) {
    l = coordinate - fi.coor.fromrange;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fpi->data[(long)b - (long)a][l-1] = 0.25;
  }
}



Static Void detfpo(fpi, rho, fpo)
rblarray fpi, rho, *fpo;
{
  base b;
  long coordinate, l;
  double sum;

  clearrbl(fpo);
  copycoords(fpi.coor, &fpo->coor);
  for (coordinate = fpi.coor.fromrandom + 1; coordinate <=
	 fpi.coor.torandom + 1; coordinate++) {
    l = coordinate - fpi.coor.fromrange;
    sum = 0.0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      sum += fpi.data[(long)b - (long)a][l-1] * rho.data[(long)b - (long)a]
	     [l-1];
    if (sum == 0.0)
      printf("detfpo: 6 zowie! PROGRAM ERROR!\n");

    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fpo->data[(long)b - (long)a]
	[l-1] = fpi.data[(long)b - (long)a][l-1] * rho.data[(long)b - (long)a]
		[l-1] / sum;
  }
}



Static Void detfpon(wildtype, fpi, fpon)
iblarray wildtype;
rblarray fpi, *fpon;
{
  /*

*/
  base b;
  long coordinate;
  boolean inrandom;
  long l;
  double sum;

  clearrbl(fpon);
  copycoords(fpi.coor, &fpon->coor);
  for (coordinate = fpi.coor.fromrange;
       coordinate <= fpi.coor.torange;
       coordinate++) {
    l = coordinate - fpi.coor.fromrange + 1;
    inrandom = (coordinate >= fpi.coor.fromrandom &&
		coordinate <= fpi.coor.torandom);
    if (inrandom) {
      sum = 0.0;
      for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
	sum += wildtype.data[(long)b - (long)a][l-1];
      if (sum > 0.0) {
	for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
	  fpon->data[(long)b - (long)a]
	    [l-1] = wildtype.data[(long)b - (long)a][l-1] / sum;
      } else {
	printf("\nWARNING: wild type frequency table is odd\n");
	printf("at coordinate %ld it does not have any data.\n\n", coordinate);
	fpon->data[(long)b - (long)a][l-1] = 1.0;
      }
    } else {
      /*

*/
      for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
	fpon->data[(long)b - (long)a][l-1] = fpi.data[(long)b - (long)a][l-1];
    }
  }
}



Static Void calcinfo(fi, fo, i)
rblarray fi, fo;
rlarray *i;
{
  /*
*/
  base b;
  long coordinate, l;
  double ln2 = log(2.0);

  for (coordinate = fi.coor.fromrandom + 1;
       coordinate <= fi.coor.torandom + 1;
       coordinate++) {
    l = coordinate - fi.coor.fromrange;
    i->data[l-1] = 0.0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      if (fo.data[(long)b - (long)a][l-1] > 0)
	i->data[l-1] += fo.data[(long)b - (long)a]
			[l-1] * log(fo.data[(long)b - (long)a]
				    [l-1] / fi.data[(long)b - (long)a][l-1]);
    }

    i->data[l-1] /= ln2;
    /*

*/
  }
}



Static Void detfproportion(nbl, std, fproportion)
iblarray nbl;
entry_ *std;
rblarray *fproportion;
{
  /*
*/
  base b;
  long coordinate;
  double frac[(long)t - (long)a + 1];
  boolean inrandom;
  long l;
  long prop[(long)t - (long)a + 1];
  long total = 0;

  clearrbl(fproportion);
  copycoords(nbl.coor, &fproportion->coor);
  for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
    prop[(long)b - (long)a] = 0;
  for (coordinate = nbl.coor.fromrandom + 1; coordinate <=
	 nbl.coor.torandom + 1; coordinate++) {
    l = coordinate - nbl.coor.fromrange;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      prop[(long)b - (long)a] += nbl.data[(long)b - (long)a][l-1];
  }
  for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
    total += prop[(long)b - (long)a];
  for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
    frac[(long)b - (long)a] = (double)prop[(long)b - (long)a] / total;
  if (total == 0.0)
    printf("9 zowie! PROGRAM ERROR!\n");

  fprintf(stats.f, " base proportions in random region: ");
  fprintf(stats.f, " a = %ld", prop[0]);
  fprintf(stats.f, " c = %ld", prop[(long)c - (long)a]);
  fprintf(stats.f, " g = %ld", prop[(long)g - (long)a]);
  fprintf(stats.f, " t = %ld\n", prop[(long)t - (long)a]);

  /*






*/

  for (coordinate = nbl.coor.fromrange;
       coordinate <= nbl.coor.torange;
       coordinate++) {
    l = coordinate - nbl.coor.fromrange + 1;
    inrandom = (coordinate >= nbl.coor.fromrandom &&
		coordinate <= nbl.coor.torandom);
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      if (basetochar(b) == std->seq.letters[l-1]) {
	if (inrandom)
	  fproportion->data[(long)b - (long)a][l-1] = frac[(long)b - (long)a];
	else
	  fproportion->data[(long)b - (long)a][l-1] = 1.0;
      } else if (inrandom)
	fproportion->data[(long)b - (long)a][l-1] = frac[(long)b - (long)a];
      else
	fproportion->data[(long)b - (long)a][l-1] = 0.0;
    }
  }
}



Static Void detfbottle(nbl, missing, std, fbotcon, fbottle)
iblarray nbl;
boolean *missing;
entry_ *std;
rblarray *fbotcon, *fbottle;
{
  /*







*/
  base b;
  boolean badbottle;
  /*
*/
  base bottle;
  long coordinate;
  double frac[(long)t - (long)a + 1];
  boolean inrandom;
  long l;
  long prop[(long)t - (long)a + 1];
  long total;

  clearrbl(fbotcon);
  copycoords(nbl.coor, &fbotcon->coor);
  clearrbl(fbottle);
  copycoords(nbl.coor, &fbottle->coor);
  *missing = false;
  fprintf(stats.f, " base proportions in random region\n");

  for (bottle = a; (long)bottle <= (long)t; bottle = (base)((long)bottle + 1)) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      prop[(long)b - (long)a] = 0;
    for (coordinate = nbl.coor.fromrandom + 1; coordinate <=
	   nbl.coor.torandom + 1; coordinate++) {
      l = coordinate - nbl.coor.fromrange;
      if (basetochar(bottle) == std->seq.letters[l-1]) {
	for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
	  prop[(long)b - (long)a] += nbl.data[(long)b - (long)a][l-1];
      }
    }
    total = 0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      total += prop[(long)b - (long)a];
    if (total == 0.0)
      printf("8 zowie! PROGRAM ERROR!\n");
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      frac[(long)b - (long)a] = (double)prop[(long)b - (long)a] / total;

    badbottle = false;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      badbottle = (badbottle || prop[(long)b - (long)a] == 0);
    *missing = (*missing || badbottle);

    fprintf(stats.f, " for bottle %c: ", basetochar(bottle));
    fprintf(stats.f, " a = %5ld", prop[0]);
    fprintf(stats.f, " c = %5ld", prop[(long)c - (long)a]);
    fprintf(stats.f, " g = %5ld", prop[(long)g - (long)a]);
    fprintf(stats.f, " t = %5ld", prop[(long)t - (long)a]);
    if (badbottle)
      fprintf(stats.f, " <= MISSING BASE(s)");
    fprintf(stats.f, "\n for bottle %c: ", basetochar(bottle));

    fprintf(stats.f, " a = %5.2f", frac[0]);
    fprintf(stats.f, " c = %5.2f", frac[(long)c - (long)a]);
    fprintf(stats.f, " g = %5.2f", frac[(long)g - (long)a]);
    fprintf(stats.f, " t = %5.2f\n", frac[(long)t - (long)a]);

    for (coordinate = nbl.coor.fromrange;
	 coordinate <= nbl.coor.torange;
	 coordinate++) {
      l = coordinate - nbl.coor.fromrange + 1;
      inrandom = (coordinate >= nbl.coor.fromrandom &&
		  coordinate <= nbl.coor.torandom);
      if (basetochar(bottle) == std->seq.letters[l-1]) {
	for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
	  if (inrandom) {
	    fbottle->data[(long)b - (long)a][l-1] = frac[(long)b - (long)a];
	    /*



*/
	    if (b == bottle)
	      fbotcon->data[(long)b - (long)a][l-1] = frac[(long)b - (long)a];
	    else
	      fbotcon->data[(long)b - (long)a]
		[l-1] = (1.0 - frac[(long)bottle - (long)a]) / 3.0;
	  }
	  /*
*/
	  else if (b == bottle) {
	    fbottle->data[(long)b - (long)a][l-1] = 1.0;
	    fbotcon->data[(long)b - (long)a][l-1] = 1.0;
	  } else {
	    fbottle->data[(long)b - (long)a][l-1] = 0.0;
	    fbotcon->data[(long)b - (long)a][l-1] = 0.0;
	  }
	}
      }
    }
  }
  if (!*missing)
    return;
  printf("***********************************************\n");
  printf("*   WARNING WARNING WARNING WARNING WARNING   *\n");
  printf("* One of the dirty bottles is missing a base! *\n");
  printf("* See the stats file for details.             *\n");
  printf("***********************************************\n");
}



Static Void signif(stats, nl, fi, fo, paramod)
_TEXT *stats;
ilarray nl;
rblarray fi, fo;
long paramod;
{
  /*


*/
  base b;
  long coordinate, degfree;
  double exp;
  double gsq = 0.0;
  boolean infinite = false;
  long l;
  double obs;
  double xsq = 0.0;
  double TEMP;

  for (coordinate = nl.coor.fromrandom;
       coordinate <= nl.coor.torandom;
       coordinate++) {
    l = coordinate - nl.coor.fromrange + 1;
    if (!infinite) {
      for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
	if (fi.data[(long)b - (long)a][l-1] == 0.0) {
	  fprintf(stats->f,
		  "fi(%c,%ld) = 0 so the significance is infinite\n",
		  basetochar(b), coordinate);
	  infinite = true;
	}
	if (!infinite) {
	  if (fo.data[(long)b - (long)a][l-1] > 0)
	    gsq += nl.data[l-1] * fo.data[(long)b - (long)a]
		   [l-1] * log(fo.data[(long)b - (long)a]
			       [l-1] / fi.data[(long)b - (long)a][l-1]);

	  obs = nl.data[l-1] * fo.data[(long)b - (long)a][l-1];
	  exp = nl.data[l-1] * fi.data[(long)b - (long)a][l-1];
	  if (exp > 0) {
	    TEMP = obs - exp;
	    xsq += TEMP * TEMP / exp;
	  }
	}
      }
    }

  }

  if (infinite)
    return;
  gsq = 2 * gsq;
  degfree = (nl.coor.torandom - nl.coor.fromrandom + 1) * 3 - paramod;
  fprintf(stats->f, "  %ld degrees of freedom\n", degfree);
  fprintf(stats->f, "  G-squared = %6.2f\n", gsq);
  fprintf(stats->f, "  X-squared = %6.2f\n\n", xsq);
}



Static Void mle(list, std, nbl)
_TEXT *list;
entry_ *std;
iblarray nbl;
{
  base b;
  long coordinate, l;
  long sum = 0, total = 0;

  for (coordinate = nbl.coor.fromrandom + 1; coordinate <=
	 nbl.coor.torandom + 1; coordinate++) {
    l = coordinate - nbl.coor.fromrange;

    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      if (basetochar(b) == std->seq.letters[l-1])
	sum += nbl.data[(long)b - (long)a][l-1];
      total += nbl.data[(long)b - (long)a][l-1];
    }
  }
  fprintf(list->f, "number of canonical bases: %5ld\n", sum);
  fprintf(list->f, "number of bases sequenced: %5ld\n", total);
  if (total == 0.0)
    printf("2 zowie! PROGRAM ERROR!\n");
  fprintf(list->f,
	  "maximum likelihood estimate of canonical: (%ld/%ld)=%5.3f\n",
	  sum, total, (double)sum / total);
}


/*
























*/


Static Void tabulate(data, numentries, co, nl, nbl, fbl)
entry_ **data;
long numentries;
coordinates co;
ilarray *nl;
iblarray *nbl;
rblarray *fbl;
{
  /*
*/
  base b;
  long l, d, FORLIM1;

  clearrbl(fbl);
  copycoords(co, &nl->coor);
  copycoords(co, &nbl->coor);
  copycoords(co, &fbl->coor);


  for (l = 0; l < maxseq; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      nbl->data[(long)b - (long)a][l] = 0;
    nl->data[l] = 0;
  }


  for (d = 0; d < numentries; d++) {
    FORLIM1 = data[d]->seq.length;
    for (l = 0; l < FORLIM1; l++) {
      b = chartorange(data[d]->seq.letters[l]);
      if (((1L << ((long)b)) & ((1L << ((long)a)) | (1L << ((long)c)) |
				(1L << ((long)g)) | (1L << ((long)t)))) != 0)
	nbl->data[(long)b - (long)a][l]++;
    }
  }


  FORLIM1 = data[0]->seq.length;
  for (l = 0; l < FORLIM1; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      nl->data[l] += nbl->data[(long)b - (long)a][l];
  }

  FORLIM1 = data[0]->seq.length;
  for (l = 0; l < FORLIM1; l++) {
    if (nl->data[l] == 0)
      printf("13 zowie! PROGRAM ERROR!\n");
  }
  if (data[0]->seq.length != nl->coor.lengthrange)
    printf("13.5 zowie!\n");


  FORLIM1 = data[0]->seq.length;
  for (l = 0; l < FORLIM1; l++) {
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fbl->data[(long)b - (long)a]
	[l] = (double)nbl->data[(long)b - (long)a][l] / nl->data[l];
  }
}



Static Void makefreq(rbl)
rblarray *rbl;
{
  base b;
  long l;
  double sum;
  long FORLIM;

  FORLIM = rbl->coor.lengthrange;
  for (l = 0; l < FORLIM; l++) {
    sum = 0.0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      sum += rbl->data[(long)b - (long)a][l];
    if (sum != 0.0) {
      for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
	rbl->data[(long)b - (long)a][l] /= sum;
    }
  }
}



Static Void normalize(std, fi, fo, rho, fpi, fpo)
entry_ *std;
rblarray *fi, fo, *rho, *fpi, *fpo;
{
  /*
*/
  /*




*/
  detfi(fo, std, fi);
  detrho(*fi, fo, rho);
  detfpi(*fi, fpi);
  detfpo(*fpi, *rho, fpo);
}



Static Void addrbl(ar, br)
rblarray ar, *br;
{
  /*






*/
  base b;
  long la, lb, la1toN, lb1toN;

  if (ar.coor.fromrandom < br->coor.fromrandom) {
    printf("addrbl: attempt to add array ar out of bounds of br\n");
    printf("ar.coor.fromrandom = %ld  br.coor.fromrandom = %ld\n",
	   ar.coor.fromrandom, br->coor.fromrandom);
    halt();
  }
  if (ar.coor.torandom > br->coor.torandom) {
    printf("addrbl: attempt to add array ar out of bounds of br\n");
    printf("ar.coor.torandom = %ld  br.coor.torandom = %ld\n",
	   ar.coor.torandom, br->coor.torandom);
    halt();
  }

  for (la = ar.coor.fromrandom; la <= ar.coor.torandom; la++) {
    la1toN = la - ar.coor.fromrange + 1;
    lb = la;
    lb1toN = lb - br->coor.fromrange + 1;


    if (lb < br->coor.fromrandom || lb > br->coor.torandom ||
	lb < br->coor.fromrange || lb > br->coor.torange) {
      printf("addrbl: program error in array b\n");
      halt();
    }

    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      br->data[(long)b - (long)a][lb1toN-1] += ar.data[(long)b - (long)a]
	[la1toN-1];
  }
}



Static Void showstatistics(stats, coords, experiment, header, std, wildtype,
			   nbl, duplicates)
_TEXT *stats;
coordinates coords;
long experiment;
stringDelila header;
entry_ *std;
iblarray wildtype, nbl;
long duplicates;
{
  base b;
  long coordinate, l;

  mkheader(stats, header, experiment, coords, std);
  fprintf(stats->f,
	  "\n     |   |    experimental     |       wildtype      |\n");
  /*


*/
  fprintf(stats->f,
	  "   l | b |    A    C    G    T |    A    C    G    T |\n");
  fprintf(stats->f,
	  "------------------------------------------------------\n");
  for (l = 0; l < coords.lengthrange; l++) {
    coordinate = l + coords.fromrange;

    fprintf(stats->f, "%4ld", coordinate);

    fprintf(stats->f, " |");

    if (coordinate < coords.fromrandom || coordinate > coords.torandom)
      fprintf(stats->f, " %c", capitalize(std->seq.letters[l]));
    else
      fprintf(stats->f, " %c", std->seq.letters[l]);
    fprintf(stats->f, " |");

    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fprintf(stats->f, " %4ld", nbl.data[(long)b - (long)a][l]);
    fprintf(stats->f, " |");

    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1))
      fprintf(stats->f, " %4ld", wildtype.data[(long)b - (long)a][l]);
    fprintf(stats->f, " |\n");
  }

  fprintf(stats->f, "\nnumber of duplicate sequences: %ld sequences\n\n",
	  duplicates);
}


#define nfactor         scaleup



Static Void makersdata(rsdata, database, standard, fpo, ipe, nlexpt)
_TEXT *rsdata, *database, *standard;
rblarray fpo;
rlarray ipe;
ilarray nlexpt;
{
  /*









*/
  /*
*/
  double avarhnb;
  base b;
  long coordinate;
  double e;
  long l;
  double ln2 = log(2.0);
  long nbl, nl;
  double rsl;
  double rs = 0.0;
  long s = 4;
  double sumavarhnb = 0.0;

  fprintf(rsdata->f, "* sites %4.2f\n", version);
  if (*database->name != '\0') {
    if (database->f != NULL)
      database->f = freopen(database->name, "r", database->f);
    else
      database->f = fopen(database->name, "r");
  } else
    rewind(database->f);
  if (database->f == NULL)
    _EscIO2(FileNotFound, database->name);
  RESETBUF(database->f, Char);
  fprintf(rsdata->f, "* ");
  copyaline(database, rsdata);
  if (*standard->name != '\0') {
    if (standard->f != NULL)
      standard->f = freopen(standard->name, "r", standard->f);
    else
      standard->f = fopen(standard->name, "r");
  } else
    rewind(standard->f);
  if (standard->f == NULL)
    _EscIO2(FileNotFound, standard->name);
  RESETBUF(standard->f, Char);
  fprintf(rsdata->f, "* ");
  copyaline(standard, rsdata);
  for (l = 1; l <= 8; l++)
    fprintf(rsdata->f, "*\n");
  fprintf(rsdata->f, "* %*ld %*ld\n",
	  nfield, fpo.coor.fromrandom, nfield, fpo.coor.torandom);
  for (l = 1; l <= 12; l++)
    fprintf(rsdata->f, "*\n");

  fprintf(rsdata->f,
    "*   l    a    c    g    t    rs(l)       rs var(hnb)  sum-var n(l)   e(hnb) f\n");

  for (coordinate = fpo.coor.fromrandom;
       coordinate <= fpo.coor.torandom;
       coordinate++) {
    l = coordinate - fpo.coor.fromrange + 1;


    fprintf(rsdata->f, " %*ld", nfield, coordinate);


    nl = 0;
    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      nbl = (long)floor(nfactor * fpo.data[(long)b - (long)a][l-1] + 0.5);
      nl += nbl;
      fprintf(rsdata->f, " %*ld", (int)(nfield + 1), nbl);
    }

    /*















*/

    e = (s - 1) / (2 * ln2 * nlexpt.data[l-1]);


    e = 0.0;


    rsl = ipe.data[l-1] - e;
    fprintf(rsdata->f, " %*.*f", infofield, infodecim, rsl);


    rs += rsl;
    fprintf(rsdata->f, " %*.*f", infofield, infodecim, rs);


    /*
*/
    avarhnb = e * e;
    avarhnb = -1.0;
    /*

*/
    sumavarhnb = avarhnb + sumavarhnb;
    fprintf(rsdata->f, " %*.*f %*.*f %*ld %*.*f a\n",
	    infofield, infodecim, avarhnb, infofield, infodecim, sumavarhnb,
	    nfield, nl, infofield, infodecim, 2.0 - e);

  }

  fprintf(rsdata->f, "* Rsequence = %*.*f for range from %ld to %ld\n",
	  infofield, infodecim, rs, fpo.coor.fromrandom, fpo.coor.torandom);

  printf("\nRsequence = %*.*f for range from %ld to %ld\n",
	 infofield, infodecim, rs, fpo.coor.fromrandom, fpo.coor.torandom);

}

#undef nfactor


/*
*/
Local Void bar(stats, experiment)
_TEXT *stats;
long experiment;
{
  fprintf(stats->f, "\nExperiment %ld", experiment);
  fprintf(stats->f, "---------------------------------\n");
}



Static Void dostats(stats, experiment, std, wildtype, nl, nbl, fi, fo, fpi)
_TEXT *stats;
long experiment;
entry_ *std;
iblarray wildtype;
ilarray nl;
iblarray nbl;
rblarray fi, fo, fpi;
{
  /*
*/
  rblarray fbottle;
  /*
*/
  rblarray fbotcon;
  /*

*/
  rblarray fproportion, fpon;
  /*



*/
  boolean missing;

  bar(stats, experiment);
  fprintf(stats->f, "significance for completely saturated model\n");
  signif(stats, nl, fi, fo, 0L);
  showrblarray(stats, fi);

  bar(stats, experiment);
  fprintf(stats->f, "significance for completely proportional model\n");
  detfproportion(nbl, std, &fproportion);
  signif(stats, nl, fproportion, fo, 3L);
  showrblarray(stats, fproportion);

  bar(stats, experiment);
  fprintf(stats->f,
	  "significance for completely proportional model, bottles\n");

  detfbottle(nbl, &missing, std, &fbotcon, &fbottle);

  if (missing)
    fprintf(stats->f,
      "Cannot calculate significance because of missing bases in dirty bottle\n");
  else
    signif(stats, nl, fbottle, fo, 12L);
  showrblarray(stats, fbottle);

  bar(stats, experiment);
  fprintf(stats->f,
	  "significance for completely proportional model, bottles\n");
  fprintf(stats->f, "canonical base, equally likely for other bases\n");
  if (missing)
    fprintf(stats->f,
      "Cannot calculate significance because of missing bases in dirty bottle\n");
  else
    signif(stats, nl, fbotcon, fo, 4L);
  showrblarray(stats, fbotcon);

  bar(stats, experiment);
  fprintf(stats->f,
	  "calculate the maximum-likelihood value for the canonical base\n");
  mle(stats, std, nbl);

  bar(stats, experiment);
  detfpon(wildtype, fpi, &fpon);
  fprintf(stats->f, "significance for wild-type sequences model\n");
  signif(stats, nl, fpon, fo, nl.coor.lengthrandom * 4);
  showrblarray(stats, fpon);

  fprintf(stats->f, "\n---------------------------------------------\n");

  /*

*/

}



Static Void fancytable(list, experiment, nl, nbl, std, LaTeX)
_TEXT *list;
long experiment;
ilarray nl;
iblarray nbl;
entry_ *std;
boolean LaTeX;
{
  /*

*/
  base b;
  long coordinate, l;

  if (LaTeX) {
    fprintf(list->f, "\n\\newpage\n");
    fprintf(list->f,
      "\\begin{table}[p] %% h here; t top; b bottom; p page of floats\n");
    fprintf(list->f, "\\begin{center}\n");
    /*

*/
    fprintf(list->f, "\\begin{tabular}{|rcrrrr|}\n");
    fprintf(list->f, "\\hline\n");
    fprintf(list->f,
      "Coor-       & Canonical & \\multicolumn{4}{c|}{Numbers of} \\\\\n");

    /*
*/
    fprintf(list->f, "dinate      & Base      & A & C & G & T \\\\\n");

    fprintf(list->f, "\\hline\n");
  } else {
    fprintf(list->f, "\n\f\n");
    fprintf(list->f, "   l  | b |    A |    C |    G |    T | number\n");
    fprintf(list->f, "------------------------------------------------\n");


  }

  for (coordinate = nbl.coor.fromrange;
       coordinate <= nbl.coor.torange;
       coordinate++) {
    l = coordinate - nbl.coor.fromrange + 1;

    fprintf(list->f, " %*ld", nfield, coordinate);

    if (LaTeX)
      fprintf(list->f, " &");
    else
      fprintf(list->f, " |");
    fprintf(list->f, " %c", std->seq.letters[l-1]);

    for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
      if (LaTeX)
	fprintf(list->f, " &");
      else
	fprintf(list->f, " |");
      fprintf(list->f, " %*ld", nfield, nbl.data[(long)b - (long)a][l-1]);
    }
    if (!LaTeX)
      fprintf(list->f, " |");


    if (!LaTeX)
      fprintf(list->f, " %*ld", nfield, nl.data[l-1]);

    if (LaTeX)
      fprintf(list->f, " \\\\");

    putc('\n', list->f);
  }
  if (LaTeX) {
    fprintf(list->f, "\\hline\n");
    fprintf(list->f, "\\end{tabular}\n");
    fprintf(list->f, "\\end{center}\n");
    fprintf(list->f, "\\caption{Number of bases observed at each position\n");
    fprintf(list->f, " in the randomized sequences of experiment %ld}\n",
	    experiment);
    fprintf(list->f, " \\label{table.nbl%ld}\n", experiment);
    fprintf(list->f, "\\end{table}\n\n");
  }

  putc('\n', list->f);
}



Static Void pageheader(list, header, experiment, std)
_TEXT *list;
stringDelila header;
long experiment;
entry_ *std;
{
  putc('\n', list->f);
  announcement(list, header);
  putentry(list, std);
  fprintf(list->f, "\nExperiment %ld\n", experiment);
}


/* Local variables for dotables: */
struct LOC_dotables {
  _TEXT *list;
  stringDelila header;
  long experiment;
  entry_ *std;
} ;

Local Void newpage(LINK)
struct LOC_dotables *LINK;
{
  fprintf(LINK->list->f, "\n\f");
  pageheader(LINK->list, LINK->header, LINK->experiment, LINK->std);
}



Static Void dotables(list_, header_, experiment_, std_, nbl, fi, fo, rho, fpi,
		     fpo)
_TEXT *list_;
stringDelila header_;
long experiment_;
entry_ *std_;
iblarray nbl;
rblarray fi, fo, rho, fpi, fpo;
{
  struct LOC_dotables V;

  V.list = list_;
  V.header = header_;
  V.experiment = experiment_;
  V.std = std_;
  if (V.experiment > 1)
    newpage(&V);
  else
    pageheader(V.list, V.header, V.experiment, V.std);
  fprintf(V.list->f, "n(b,l): input numbers\n");
  showiblarray(V.list, nbl);

  newpage(&V);
  fprintf(V.list->f,
    "fi(b,l), input frequency table (fractions canonical and not canonical):\n");
  showrblarray(V.list, fi);

  newpage(&V);
  fprintf(V.list->f, "fo(b,l), raw experimental frequencies:\n");
  showrblarray(V.list, fo);

  newpage(&V);
  fprintf(V.list->f, "rho(b,l), ratio of fo to fi tables:\n");
  showrblarray(V.list, rho);

  newpage(&V);
  fprintf(V.list->f, "f'i(b,l), normalized input frequencies (0.25)\n");
  showrblarray(V.list, fpi);

  newpage(&V);
  fprintf(V.list->f, "f'o(b,l), normalized output frequency table:\n");
  showrblarray(V.list, fpo);
}



Static Void putmaxstring(f, s, counter, maxcounter)
_TEXT *f;
stringDelila *s;
long *counter, maxcounter;
{
  /*

*/
  long l, FORLIM;

  FORLIM = s->length;
  for (l = 0; l < FORLIM; l++) {
    (*counter)++;
    if (*counter < maxcounter)
      putc(s->letters[l], f->f);
  }
}


#define namemax         20



Static Void makesequ(sequ, makebkp, coords, data, numentries, std)
_TEXT *sequ, *makebkp;
coordinates coords;
entry_ **data;
long numentries;
entry_ *std;
{
  /*
*/
  long counter = 0;
  long thenumber, l;
  boolean x_;
  entry_ *WITH;
  long FORLIM1;
  stringDelila *WITH1;

  if (*sequ->name != '\0') {
    if (sequ->f != NULL)
      sequ->f = freopen(sequ->name, "w", sequ->f);
    else
      sequ->f = fopen(sequ->name, "w");
  } else {
    if (sequ->f != NULL)
      rewind(sequ->f);
    else
      sequ->f = tmpfile();
  }
  if (sequ->f == NULL)
    _EscIO2(FileNotFound, sequ->name);
  SETUPBUF(sequ->f, Char);
  if (*makebkp->name != '\0') {
    if (makebkp->f != NULL)
      makebkp->f = freopen(makebkp->name, "w", makebkp->f);
    else
      makebkp->f = fopen(makebkp->name, "w");
  } else {
    if (makebkp->f != NULL)
      rewind(makebkp->f);
    else
      makebkp->f = tmpfile();
  }
  if (makebkp->f == NULL)
    _EscIO2(FileNotFound, makebkp->name);
  SETUPBUF(makebkp->f, Char);

  fprintf(makebkp->f, "sites %4.2f\n", version);

  fprintf(makebkp->f, "b\n");

  putstring(makebkp, &std->name);
  fprintf(makebkp->f, "\ny\n");

  putmaxstring(makebkp, &std->name, &counter, (long)namemax);
  fprintf(makebkp->f, " organism key name\n");
  putstring(makebkp, &std->name);
  fprintf(makebkp->f, " organism full name\n");
  fprintf(makebkp->f, "from sites %4.2f\n\n", version);
  fprintf(makebkp->f, "map units\n");

  counter = 0;
  putmaxstring(makebkp, &std->name, &counter, (long)namemax);
  fprintf(makebkp->f, " chromosome key name\n");
  putstring(makebkp, &std->name);
  fprintf(makebkp->f, " chromosome full name\n\n");
  fprintf(makebkp->f, "0\n");
  fprintf(makebkp->f, "100\n");

  for (thenumber = 1; thenumber <= numentries; thenumber++) {
    WITH = data[thenumber-1];
    /*
*/
    x_ = false;
    FORLIM1 = WITH->seq.length;
    for (l = 0; l < FORLIM1; l++) {
      WITH1 = &WITH->seq;
      if (WITH1->letters[l] == 'x') {
	putc(std->seq.letters[l], sequ->f);
	x_ = true;
	printf("\nNOTE: x bases replaced by standard in sequ.\n");
	printf("See notes in book created using makebk<makebkp.\n");
	printf("See piece number %ld\n", thenumber);
      } else
	putc(WITH->seq.letters[l], sequ->f);
    }
    fprintf(sequ->f, ".\n");


    counter = 0;
    putmaxstring(makebkp, &WITH->name, &counter, (long)namemax);
    /*





*/
    if (x_) {
      if (counter < namemax - 10)
	fprintf(makebkp->f, "_X_REMOVED");
      else if (counter < namemax - 3)
	fprintf(makebkp->f, "_X!");
    }
    putc('\n', makebkp->f);


    putstring(makebkp, &WITH->name);
    putc('_', makebkp->f);
    putstring(makebkp, &WITH->primer);
    putc('_', makebkp->f);
    putstring(makebkp, &WITH->ex);
    putc('_', makebkp->f);
    putstring(makebkp, &WITH->date);
    putc('\n', makebkp->f);
    if (x_) {
      fprintf(makebkp->f,
	      "The x bases were replaced by the standard sequence:\n");
      putstring(makebkp, &WITH->seq);
      fprintf(makebkp->f, "\nThe standard sequence is:\n");
      putstring(makebkp, &std->seq);
      putc('\n', makebkp->f);
    }
    fprintf(makebkp->f, "piece number %ld written by sites\n\n", thenumber);
    fprintf(makebkp->f, "0\n");
    fprintf(makebkp->f, "l\n");
    fprintf(makebkp->f, "+\n");
    fprintf(makebkp->f, "%ld\n", coords.fromrange);
    fprintf(makebkp->f, "%ld\n", coords.torange);
    fprintf(makebkp->f, "l\n");
    fprintf(makebkp->f, "+\n");
    fprintf(makebkp->f, "%ld\n", coords.fromrange);
    fprintf(makebkp->f, "%ld\n", coords.torange);
    fprintf(makebkp->f, "n\n");
    fprintf(makebkp->f, "0\n");
    fprintf(makebkp->f, "0\n");
    fprintf(makebkp->f, "0\n");
    fprintf(makebkp->f, "n\n");
    fprintf(makebkp->f, "n\n");
  }
  /*

*/


}

#undef namemax



Static Void themain(database, standard, caps, latex, list, sorted, stats,
		    tables, rsdata, sequ, makebkp)
_TEXT *database, *standard, *caps, *latex, *list, *sorted, *stats, *tables,
      *rsdata, *sequ, *makebkp;
{
  coordinates coords;
  entries data;
  long duplicates;
  long experiment = 0;

  rblarray fi, fo, rho, fpi, fpo;

  rblarray global;
  /*
*/
  stringDelila header;
  rlarray ipe;
  iblarray nbl;
  ilarray nl;
  long number, numentries;
  entry_ *std = NULL;
  iblarray wildtype;
  long totalentries = 0;
  _TEXT TEMP;

  /*
*/
  printf("sites %4.2f\n", version);

  if (*caps->name != '\0') {
    if (caps->f != NULL)
      caps->f = freopen(caps->name, "w", caps->f);
    else
      caps->f = fopen(caps->name, "w");
  } else {
    if (caps->f != NULL)
      rewind(caps->f);
    else
      caps->f = tmpfile();
  }
  if (caps->f == NULL)
    _EscIO2(FileNotFound, caps->name);
  SETUPBUF(caps->f, Char);
  if (*latex->name != '\0') {
    if (latex->f != NULL)
      latex->f = freopen(latex->name, "w", latex->f);
    else
      latex->f = fopen(latex->name, "w");
  } else {
    if (latex->f != NULL)
      rewind(latex->f);
    else
      latex->f = tmpfile();
  }
  if (latex->f == NULL)
    _EscIO2(FileNotFound, latex->name);
  SETUPBUF(latex->f, Char);
  if (*list->name != '\0') {
    if (list->f != NULL)
      list->f = freopen(list->name, "w", list->f);
    else
      list->f = fopen(list->name, "w");
  } else {
    if (list->f != NULL)
      rewind(list->f);
    else
      list->f = tmpfile();
  }
  if (list->f == NULL)
    _EscIO2(FileNotFound, list->name);
  SETUPBUF(list->f, Char);
  if (*sorted->name != '\0') {
    if (sorted->f != NULL)
      sorted->f = freopen(sorted->name, "w", sorted->f);
    else
      sorted->f = fopen(sorted->name, "w");
  } else {
    if (sorted->f != NULL)
      rewind(sorted->f);
    else
      sorted->f = tmpfile();
  }
  if (sorted->f == NULL)
    _EscIO2(FileNotFound, sorted->name);
  SETUPBUF(sorted->f, Char);
  if (*stats->name != '\0') {
    if (stats->f != NULL)
      stats->f = freopen(stats->name, "w", stats->f);
    else
      stats->f = fopen(stats->name, "w");
  } else {
    if (stats->f != NULL)
      rewind(stats->f);
    else
      stats->f = tmpfile();
  }
  if (stats->f == NULL)
    _EscIO2(FileNotFound, stats->name);
  SETUPBUF(stats->f, Char);
  if (*tables->name != '\0') {
    if (tables->f != NULL)
      tables->f = freopen(tables->name, "w", tables->f);
    else
      tables->f = fopen(tables->name, "w");
  } else {
    if (tables->f != NULL)
      rewind(tables->f);
    else
      tables->f = tmpfile();
  }
  if (tables->f == NULL)
    _EscIO2(FileNotFound, tables->name);
  SETUPBUF(tables->f, Char);
  if (*rsdata->name != '\0') {
    if (rsdata->f != NULL)
      rsdata->f = freopen(rsdata->name, "w", rsdata->f);
    else
      rsdata->f = fopen(rsdata->name, "w");
  } else {
    if (rsdata->f != NULL)
      rewind(rsdata->f);
    else
      rsdata->f = tmpfile();
  }
  if (rsdata->f == NULL)
    _EscIO2(FileNotFound, rsdata->name);
  SETUPBUF(rsdata->f, Char);


  fprintf(latex->f, "%% sites %4.2f\n", version);
  fprintf(latex->f, "%% LaTeX typesetting format: \n");
  fprintf(latex->f, "\\documentstyle[12pt]{article}\n");
  fprintf(latex->f, "\\textheight 9in %% this works\n");
  fprintf(latex->f,
	  "\\topmargin 0.0in %% -0.5 would shift the whole thing up\n");
  fprintf(latex->f, "\\headheight 0in\n");
  fprintf(latex->f, "\\headsep 0in\n");
  fprintf(latex->f, "\\textwidth 6in\n");
  fprintf(latex->f, "\\oddsidemargin 0.25in\n");
  fprintf(latex->f, "\\begin{document}\n");


  getrsdata(standard, &wildtype);


  readheader(database, &header);

  printf("Analysis of: \n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  putstring(&TEMP, &header);
  putchar('\n');

  /*
*/
  findextent(database, &global);


  for (number = 0; number < maxentries; number++)
    data[number] = NULL;
  data[0] = (entry_ *)Malloc(sizeof(entry_));


  readheader(database, &header);
  printf("\nPass through the experiments analyzing the data\n");

  while (!BUFEOF(database->f)) {
    if (P_peek(database->f) == '@') {
      experiment++;
      printf("Experiment %ld\n", experiment);
      /*

*/
      readexpt(database, &coords, &std);
      continue;
    }
    readdata(database, data, std, &numentries);

    sortdata(data, numentries, true);
    display(caps, data, numentries, header, experiment, coords, std, 'c', '-',
	    false, &duplicates);
    compress(data, &numentries, std, caps);

    totalentries += numentries;
    tabulate(data, numentries, coords, &nl, &nbl, &fo);
    normalize(std, &fi, fo, &rho, &fpi, &fpo);
    addrbl(fpo, &global);


    printf("   Number of entries in this experiment: %5ld\n", numentries);

    putc('\n', list->f);
    if (experiment > 1) {
      fprintf(list->f, "\f");
      fprintf(latex->f, "\\newpage\n");
    }
    display(list, data, numentries, header, experiment, coords, std, 's', ' ',
	    false, &duplicates);
    fancytable(list, experiment, nl, nbl, std, false);

    fprintf(latex->f, "\\twocolumn\n");
    fprintf(latex->f, "\\scriptsize\n");
    fprintf(latex->f, "\\begin{verbatim}\n");
    figure(latex, data, numentries, experiment, coords, std, 's', ' ', true);
    fprintf(latex->f, "\\end{verbatim}\n");
    fprintf(latex->f, "\\normalsize\n");
    fancytable(latex, experiment, nl, nbl, std, true);

    dotables(tables, header, experiment, std, nbl, fi, fo, rho, fpi, fpo);

    /*
*/
    crunch(data, numentries, std, 'z');

    sortdata(data, numentries, false);

    /*
*/
    uncrunch(data, numentries, std, 'z');

    display(sorted, data, numentries, header, experiment, coords, std, 's',
	    ' ', true, &duplicates);

    printf("number of duplicate sequences: %ld\n", duplicates);

    showstatistics(stats, coords, experiment, header, std, wildtype, nbl,
		   duplicates);
    dostats(stats, experiment, std, wildtype, nl, nbl, fi, fo, fpi);
  }

  printf("   Number of entries in ALL experiments: %5ld\n", totalentries);

  /*



*/
  makefreq(&global);
  fprintf(tables->f, "\n\f\n");
  pageheader(tables, header, experiment, std);
  fprintf(tables->f,
	  "Average of normalized frequency tables from all experiments:\n");
  showrblarray(tables, global);

  calcinfo(fpi, global, &ipe);
  /*
*/
  makersdata(rsdata, database, standard, global, ipe, nl);
  /*


*/


  fprintf(latex->f, "\\end{document}\n");


  makesequ(sequ, makebkp, coords, data, numentries, std);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  makebkp.f = NULL;
  strcpy(makebkp.name, "makebkp");
  sequ.f = NULL;
  strcpy(sequ.name, "sequ");
  rsdata.f = NULL;
  strcpy(rsdata.name, "rsdata");
  stats.f = NULL;
  strcpy(stats.name, "stats");
  standard.f = NULL;
  strcpy(standard.name, "standard");
  sorted.f = NULL;
  strcpy(sorted.name, "sorted");
  list.f = NULL;
  strcpy(list.name, "list");
  latex.f = NULL;
  strcpy(latex.name, "latex");
  caps.f = NULL;
  strcpy(caps.name, "caps");
  tables.f = NULL;
  strcpy(tables.name, "tables");
  database.f = NULL;
  strcpy(database.name, "database");
  themain(&database, &standard, &caps, &latex, &list, &sorted, &stats,
	  &tables, &rsdata, &sequ, &makebkp);
_L1:
  if (database.f != NULL)
    fclose(database.f);
  if (tables.f != NULL)
    fclose(tables.f);
  if (caps.f != NULL)
    fclose(caps.f);
  if (latex.f != NULL)
    fclose(latex.f);
  if (list.f != NULL)
    fclose(list.f);
  if (sorted.f != NULL)
    fclose(sorted.f);
  if (standard.f != NULL)
    fclose(standard.f);
  if (stats.f != NULL)
    fclose(stats.f);
  if (rsdata.f != NULL)
    fclose(rsdata.f);
  if (sequ.f != NULL)
    fclose(sequ.f);
  if (makebkp.f != NULL)
    fclose(makebkp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
