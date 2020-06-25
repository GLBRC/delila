/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dnag.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.74
/*


*/



/*
















































*/



#define iascale         0.20
#define fudgex          (-0.02)
#define fudgey          (-0.04)



#define maxstring       150



#define pi              3.14159265354

#define picfield        12
#define picwidth        9

#define charwidth       0.05
/*
*/

#define defscale        81


/*

*/




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



typedef struct atom {
  Char group;
  Char id[2];
  /*

*/
  double radius, angle, z;
} atom;



typedef struct groupatom {
  atom element;
  struct groupatom *next;
} groupatom;




typedef struct nastructure {
  groupatom *p, *d, *a, *c, *g, *t;
} nastructure;



Static double toradians;

Static _TEXT bdna;

Static _TEXT dooin;



Static boolean inpicture;
/*
*/
Static double picxglobal, picyglobal, pictolerance;
/*
*/
Static double scale;


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





Static Void await()
{

  /*
*/
  /*


*/

  /*

*/
}



Static Void startpic(afile, setscale, x, y)
_TEXT *afile;
double setscale, x, y;
{
  /*
*/
  fprintf(afile->f, ".PS %*.*f %*.*f %*.*f\n",
	  picfield, picwidth, setscale, picfield, picwidth, x, picfield,
	  picwidth, y);
  scale = setscale;

  inpicture = true;
  picxglobal = 0.0;
  picyglobal = 0.0;
  pictolerance = (long)(exp(picwidth * log(10.0)) + 0.5);

}



Static Void stoppic(afile)
_TEXT *afile;
{
  fprintf(afile->f, ".PE\n");
  inpicture = false;
}



Static Void drawr(afile, dx, dy, visibility, spacing)
_TEXT *afile;
double dx, dy;
Char visibility;
double spacing;
{
  /*





*/
  fprintf(afile->f, "drawr %*.*f %*.*f %c %*.*f\n",
	  picfield, picwidth, dx, picfield, picwidth, dy, visibility,
	  picfield, picwidth, spacing);
  picxglobal += dx;
  picyglobal += dy;
}



Static Void mover(afile, dx, dy)
_TEXT *afile;
double dx, dy;
{
  fprintf(afile->f, "mover %*.*f %*.*f\n",
	  picfield, picwidth, dx, picfield, picwidth, dy);
  picxglobal += dx;
  picyglobal += dy;
}



Static Void liner(afile, dx, dy)
_TEXT *afile;
double dx, dy;
{
  fprintf(afile->f, "liner %*.*f %*.*f\n",
	  picfield, picwidth, dx, picfield, picwidth, dy);
  picxglobal += dx;
  picyglobal += dy;
}




Static Void drawa(afile, x, y, visibility, spacing)
_TEXT *afile;
double x, y;
Char visibility;
double spacing;
{
  /*





*/
  fprintf(afile->f, "drawa %*.*f %*.*f %c %*.*f\n",
	  picfield, picwidth, x, picfield, picwidth, y, visibility, picfield,
	  picwidth, spacing);
  picxglobal = x;
  picyglobal = y;
}



Static Void movea(afile, x, y)
_TEXT *afile;
double x, y;
{
  fprintf(afile->f, "movea %*.*f %*.*f\n",
	  picfield, picwidth, x, picfield, picwidth, y);
  picxglobal = x;
  picyglobal = y;
}



Static Void linea(afile, x, y)
_TEXT *afile;
double x, y;
{
  fprintf(afile->f, "linea %*.*f %*.*f\n",
	  picfield, picwidth, x, picfield, picwidth, y);
  picxglobal = x;
  picyglobal = y;
}



Static Void graphstring(tofile, s, centered)
_TEXT *tofile;
stringDelila *s;
boolean centered;
{
  /*


*/
  long i;
  boolean quoted;
  stringDelila sq;
  long FORLIM;

  if (s->length > 2) {
    if (s->letters[0] == '"' && s->letters[s->length - 1] == '"')
      quoted = true;
    else
      quoted = false;
  } else
    quoted = false;

  if (!quoted && centered) {
    clearstring(&sq);
    sq.length = s->length + 2;
    sq.letters[0] = '"';
    FORLIM = s->length;
    for (i = 1; i <= FORLIM; i++)
      sq.letters[i] = s->letters[i-1];
    sq.letters[sq.length - 1] = '"';
    writestring(tofile, &sq);
  } else
    writestring(tofile, s);
  putc('\n', tofile->f);
}



Static Void stringinteger(number, name, width, leadingzeros)
long number;
stringDelila *name;
long width;
boolean leadingzeros;
{
  /*

*/
  long bigdigit, dig, place, sign;

  if (number < 0) {
    sign = -1;
    name->length++;
    number = -number;
    if (leadingzeros)
      printf(
	"WARNING: stringinteger: the sign of a negative number with leading zeros is lost\n");
  } else
    sign = 1;

  /*


*/
  if (number > 9)
    dig = (long)(log(number + 0.1) / log(10.0)) + 1;
  else
    dig = 1;

  if (dig > width) {
    printf("stringinteger: number width too small\n");
    printf("%ld digit number (%ld)\n", dig, number);
    printf("does not fit in %ld characters\n", width);
    halt();
  }
  if (leadingzeros)
    bigdigit = name->length + 1;
  else {
    bigdigit = name->length + width - dig + 1;

    if (bigdigit <= name->length && sign < 0) {
      printf("stringinteger: no room for sign\n");
      halt();
    }
  }
  if (sign < 0)
    name->letters[bigdigit-2] = '-';

  for (place = name->length + width - 1; place >= bigdigit - 1; place--) {
    switch (number % 10) {

    case 0:
      name->letters[place] = '0';
      break;

    case 1:
      name->letters[place] = '1';
      break;

    case 2:
      name->letters[place] = '2';
      break;

    case 3:
      name->letters[place] = '3';
      break;

    case 4:
      name->letters[place] = '4';
      break;

    case 5:
      name->letters[place] = '5';
      break;

    case 6:
      name->letters[place] = '6';
      break;

    case 7:
      name->letters[place] = '7';
      break;

    case 8:
      name->letters[place] = '8';
      break;

    case 9:
      name->letters[place] = '9';
      break;
    }
    number /= 10;
  }
  name->length += width;
}



Static Void stringreal(number, name, width, decimal)
double number;
stringDelila *name;
long width, decimal;
{
  /*

*/
  /*



*/
  long abovezero;
  /*
*/
  long shift;
  /*
*/
  long sign, thedecimal, theupper;

  if (number < 0)
    sign = -1;
  else
    sign = 1;

  number = fabs(number);


  shift = (long)floor(exp(decimal * log(10.0)) + 0.5);
  abovezero = (long)floor(number * shift + 0.5);
  theupper = (long)((double)abovezero / shift);
  thedecimal = abovezero - shift * theupper;



  stringinteger(sign * theupper, name, width - decimal - 1, false);
  name->length++;
  name->letters[name->length - 1] = '.';
  stringinteger(thedecimal, name, decimal, true);
}



Static Void picnumber(afile, dx, dy, number, width, decimal, centered)
_TEXT *afile;
double dx, dy, number;
long width, decimal;
boolean centered;
{
  /*





*/
  fprintf(afile->f, "picnumber %*.*f %*.*f %*.*f %2ld %2ld",
	  picfield, picwidth, dx, picfield, picwidth, dy, picfield, picwidth,
	  number, width, decimal);
  if (centered)
    fprintf(afile->f, " true");
  else
    fprintf(afile->f, " false");
  putc('\n', afile->f);
}



Static Void xtic(afile, length, dx, dy, number, width, decimal)
_TEXT *afile;
double length, dx, dy, number;
long width, decimal;
{
  /*




*/
  fprintf(afile->f, "xtic %*.*f %*.*f %*.*f %*.*f %*ld %*ld\n",
	  picfield, picwidth, length, picfield, picwidth, dx, picfield,
	  picwidth, dy, picfield, picwidth, number, picfield, width, picfield,
	  decimal);
}



Static Void ytic(afile, length, dx, dy, number, width, decimal)
_TEXT *afile;
double length, dx, dy, number;
long width, decimal;
{
  /*




*/
  fprintf(afile->f, "ytic %*.*f %*.*f %*.*f %*.*f %*ld %*ld\n",
	  picfield, picwidth, length, picfield, picwidth, dx, picfield,
	  picwidth, dy, picfield, picwidth, number, picfield, width, picfield,
	  decimal);
}



Static Void xaxis(afile, axlength, fromtic, interval, totic, length, dx, dy,
		  width, decimal)
_TEXT *afile;
double axlength, fromtic, interval, totic, length, dx, dy;
long width, decimal;
{
  /*





*/
  fprintf(afile->f,
	  "xaxis %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*ld %*ld\n",
	  picfield, picwidth, axlength, picfield, picwidth, fromtic, picfield,
	  picwidth, interval, picfield, picwidth, totic, picfield, picwidth,
	  length, picfield, picwidth, dx, picfield, picwidth, dy, picfield,
	  width, picfield, decimal);
}



Static Void yaxis(afile, aylength, fromtic, interval, totic, length, dx, dy,
		  width, decimal)
_TEXT *afile;
double aylength, fromtic, interval, totic, length, dx, dy;
long width, decimal;
{
  /*





*/
  fprintf(afile->f,
	  "yaxis %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f %*ld %*ld\n",
	  picfield, picwidth, aylength, picfield, picwidth, fromtic, picfield,
	  picwidth, interval, picfield, picwidth, totic, picfield, picwidth,
	  length, picfield, picwidth, dx, picfield, picwidth, dy, picfield,
	  width, picfield, decimal);
}





Static Void circler(afile, radius)
_TEXT *afile;
double radius;
{
  fprintf(afile->f, "circler %*.*f\n", picfield, picwidth, radius);
}



Static Void polrec(r, theta, x, y)
double r, theta, *x, *y;
{
  /*
*/
  *x = r * cos(theta);
  *y = r * sin(theta);
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






/*













*/



Static Void header(outfile)
_TEXT *outfile;
{
  if (*outfile->name != '\0') {
    if (outfile->f != NULL)
      outfile->f = freopen(outfile->name, "w", outfile->f);
    else
      outfile->f = fopen(outfile->name, "w");
  } else {
    if (outfile->f != NULL)
      rewind(outfile->f);
    else
      outfile->f = tmpfile();
  }
  if (outfile->f == NULL)
    _EscIO2(FileNotFound, outfile->name);
  SETUPBUF(outfile->f, Char);

  fprintf(outfile->f, ".nf\n");
  fprintf(outfile->f, "dnag %4.2f\n", version);
}



Static Void readatom(afile, a, found)
_TEXT *afile;
atom *a;
boolean *found;
{
  /*







*/
  boolean done = false;

  /*




*/
  do {
    if (BUFEOF(afile->f)) {
      *found = false;
      done = true;
    } else if (P_peek(afile->f) == '*') {
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
    } else {
      *found = true;
      done = true;
    }
  } while (!done);

  if (!*found)
    return;
  a->group = getc(afile->f);
  if (a->group == '\n')
    a->group = ' ';
  getc(afile->f);
  *a->id = getc(afile->f);
  a->id[1] = getc(afile->f);
  if (a->id[0] == '\n')
    a->id[0] = ' ';
  if (a->id[1] == '\n')
    a->id[1] = ' ';
  fscanf(afile->f, "%lg%lg%lg%*[^\n]", &a->radius, &a->angle, &a->z);
  getc(afile->f);
}



Static Void readgroup(infile, c, g)
_TEXT *infile;
Char c;
groupatom **g;
{
  groupatom *current;
  boolean found;
  groupatom *previous;

  /*
*/
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
  *g = (groupatom *)Malloc(sizeof(groupatom));
  current = *g;
  previous = current;

  do {
    readatom(infile, &current->element, &found);
    if (found) {
      if (current->element.group == c) {
	current->next = (groupatom *)Malloc(sizeof(groupatom));
	previous = current;
	current = current->next;
      }
    } else {
      previous->next = NULL;
      Free(current);
    }
  } while (found);
}



Static Void readnastructure(infile, b)
_TEXT *infile;
nastructure *b;
{
  readgroup(infile, 'P', &b->p);
  readgroup(infile, 'D', &b->d);
  readgroup(infile, 'A', &b->a);
  readgroup(infile, 'C', &b->c);
  readgroup(infile, 'G', &b->g);
  readgroup(infile, 'T', &b->t);
}


/* Local variables for atomicradius: */
struct LOC_atomicradius {
  Char e1, e2;
} ;

Local Void unknown(LINK)
struct LOC_atomicradius *LINK;
{
  printf("radius of element %c%c not known\n", LINK->e1, LINK->e2);
  halt();
}



Static double atomicradius(e1_, e2_)
Char e1_, e2_;
{
  /*






*/
  struct LOC_atomicradius V;

  V.e1 = e1_;
  V.e2 = e2_;
  if (V.e2 == ' ') {
    if (V.e1 == 'H')
      return 0.25;
    if (V.e1 == 'C')
      return 0.70;
    if (V.e1 == 'N')
      return 0.65;
    if (V.e1 == 'O')
      return 0.50;
    if (V.e1 == 'P') {
      return 1.00;


    }
    if (V.e1 == 'M')
      return (atomicradius('C', ' '));
    unknown(&V);
  } else
    unknown(&V);
}



Static Void drawatom(outfile, a, zerox, zeroy, theta, scale, dyadic)
_TEXT *outfile;
atom a;
double zerox, zeroy, theta, scale;
boolean dyadic;
{
  /*

*/
  double x, y;

  if (dyadic)
    polrec(a.radius * scale, (theta - a.angle) * toradians, &x, &y);
  else
    polrec(a.radius * scale, (theta + a.angle) * toradians, &x, &y);

  movea(outfile, x + zerox, y + zeroy);
  circler(outfile, scale * atomicradius(a.id[0], ' '));

  movea(outfile, x + zerox + fudgex, y + zeroy + fudgey);
  fprintf(outfile->f, "\"%c%c\"\n", a.id[0], a.id[1]);
}



Static Void drawgroup(outfile, g, zerox, zeroy, theta, scale, dyadic)
_TEXT *outfile;
groupatom *g;
double zerox, zeroy, theta, scale;
boolean dyadic;
{
  /*





*/
  groupatom *current = g;
  boolean labeling;
  double x2, y2, x5, y5;
  atom *WITH;

  labeling = (g->element.group == 'T' || g->element.group == 'G' ||
	      g->element.group == 'C' || g->element.group == 'A');

  while (current != NULL) {
    drawatom(outfile, current->element, zerox, zeroy, theta, scale, dyadic);
    if (labeling) {
      WITH = &current->element;
      if (WITH->id[0] == 'C') {
	if (WITH->id[1] == '2') {
	  if (dyadic)
	    polrec(scale * WITH->radius, (theta - WITH->angle) * toradians,
		   &x2, &y2);
	  else
	    polrec(scale * WITH->radius, (theta + WITH->angle) * toradians,
		   &x2, &y2);
	}
	if (WITH->id[1] == '5') {
	  if (dyadic)
	    polrec(scale * WITH->radius, (theta - WITH->angle) * toradians,
		   &x5, &y5);
	  else
	    polrec(scale * WITH->radius, (theta + WITH->angle) * toradians,
		   &x5, &y5);
	}
      }
    }
    current = current->next;
  }

  if (labeling) {
    movea(outfile, (x2 + x5) / 2 + zerox + fudgex,
	  (y2 + y5) / 2 + zeroy + fudgey);

    fprintf(outfile->f, "\"%c\"\n", g->element.group);
  }

}



Static Void drawbase(outfile, b, dna, zerox, zeroy, angle, scale, dyadic)
_TEXT *outfile;
Char b;
nastructure dna;
double zerox, zeroy, angle, scale;
boolean dyadic;
{
  /*


*/
  drawgroup(outfile, dna.p, zerox, zeroy, angle, scale, dyadic);
  drawgroup(outfile, dna.d, zerox, zeroy, angle, scale, dyadic);
  switch (b) {

  case 'a':
    drawgroup(outfile, dna.a, zerox, zeroy, angle, scale, dyadic);
    break;

  case 'c':
    drawgroup(outfile, dna.c, zerox, zeroy, angle, scale, dyadic);
    break;

  case 'g':
    drawgroup(outfile, dna.g, zerox, zeroy, angle, scale, dyadic);
    break;

  case 't':
    drawgroup(outfile, dna.t, zerox, zeroy, angle, scale, dyadic);
    break;
  }
}



Static Void drawbp(outfile, b, dna, zerox, zeroy, angle, scale)
_TEXT *outfile;
Char b;
nastructure dna;
double zerox, zeroy, angle, scale;
{
  /*





*/
  drawbase(outfile, b, dna, zerox, zeroy, angle, scale, false);
  switch (b) {

  case 'a':
    drawbase(outfile, 't', dna, zerox, zeroy, angle, scale, true);
    break;

  case 'c':
    drawbase(outfile, 'g', dna, zerox, zeroy, angle, scale, true);
    break;

  case 'g':
    drawbase(outfile, 'c', dna, zerox, zeroy, angle, scale, true);
    break;

  case 't':
    drawbase(outfile, 'a', dna, zerox, zeroy, angle, scale, true);
    break;
  }
}



Static Void dnacircle(outfile, dnaradius, zerox, zeroy, angle, scale)
_TEXT *outfile;
double dnaradius, zerox, zeroy, angle, scale;
{
  /*





*/
  double x, y;

  movea(outfile, zerox, zeroy);
  circler(outfile, scale / 2);
  circler(outfile, dnaradius * scale);
  polrec(dnaradius * scale, angle * toradians, &x, &y);
  linea(outfile, x + zerox, y + zeroy);
}



Static Void themain(bdna, outfile)
_TEXT *bdna, *outfile;
{
  nastructure dna;

  printf("dnag %4.2f\n", version);



  header(outfile);
  readnastructure(bdna, &dna);
  toradians = 2 * pi / 360;

  startpic(outfile, 81.0, 4.0, 7.0);
  dnacircle(outfile, 10.0, 0.0, 0.0, -90.0, iascale);
  drawbp(outfile, 'a', dna, 0.0, 0.0, -90.0, iascale);
  /*





*/
  dnacircle(outfile, 10.0, 0.0, -4.3, -90.0, iascale);
  drawbp(outfile, 'g', dna, 0.0, -4.3, -90.0, iascale);
  stoppic(outfile);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  dooin.f = NULL;
  strcpy(dooin.name, "dooin");
  bdna.f = NULL;
  strcpy(bdna.name, "bdna");
  themain(&bdna, &dooin);
_L1:
  if (bdna.f != NULL)
    fclose(bdna.f);
  if (dooin.f != NULL)
    fclose(dooin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
