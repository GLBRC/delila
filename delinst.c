/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "delinst.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.19
/*




















*/
#define updateversion   1.14
#define versionupperbound  20.00



/*
































































































































*/



#define maxstring       2000




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;


Static _TEXT title, organism, chromosome, piece, names, coordinates,
	     coordinates2, directions, delinstp, inst;


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



Static Void copyALLlines(fin, fout)
_TEXT *fin, *fout;
{
  while (!BUFEOF(fin->f))
    copyaline(fin, fout);
}



Static Void copyaword(fin, fout)
_TEXT *fin, *fout;
{
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
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
  stringDelila *t;

  t = *ribbon;

  while (t->next != NULL)
    t = t->next;

  while (t->previous != NULL) {
    t = t->previous;
    Free(t->next);
  }
  Free(t);
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



Static Void getstring(afile, buffer, gotten)
_TEXT *afile;
stringDelila *buffer;
boolean *gotten;
{
  /*
*/
  long index = 0;

  clearstring(buffer);
  if (BUFEOF(afile->f)) {
    *gotten = false;
    return;
  }
  while (!P_eoln(afile->f) && index < maxstring) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
  }
  if (!P_eoln(afile->f)) {
    printf(" getstring: a line exceeds maximum string size (%ld)\n",
	   (long)maxstring);
    halt();
  }
  buffer->length = index;
  buffer->current = 1;
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  *gotten = true;
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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
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



Static Void copystring(a, b)
stringDelila a, *b;
{
  /*
*/
  long l;

  b->length = a.length;
  for (l = 0; l < a.length; l++)
    b->letters[l] = a.letters[l];
  b->current = a.current;
  b->next = a.next;
}



Static Void copyfile(fin, fout)
_TEXT *fin, *fout;
{
  while (!BUFEOF(fin->f))
    copyaline(fin, fout);
}



Static Void fixchrname(chr)
stringDelila *chr;
{
  /*

*/
  if (chr->length >= 5)
    return;
  if (chr->letters[0] != 'c')
    return;
  /*




*/
  if (chr->letters[1] != 'h')
    return;
  if (chr->letters[2] != 'r')
    return;
  chr->letters[4] = chr->letters[3];
  chr->letters[3] = '0';
  chr->length = 5;
}


#define copylines       4





Static Void upgradeto114(delinstp)
_TEXT *delinstp;
{
  /*
*/
  _TEXT internal;
  long line;
  double parameterversion = 1.14;

  internal.f = NULL;
  *internal.name = '\0';
  printf("upgrading to version %4.2f ...\n", parameterversion);


  if (*delinstp->name != '\0') {
    if (delinstp->f != NULL)
      delinstp->f = freopen(delinstp->name, "r", delinstp->f);
    else
      delinstp->f = fopen(delinstp->name, "r");
  } else
    rewind(delinstp->f);
  if (delinstp->f == NULL)
    _EscIO2(FileNotFound, delinstp->name);
  RESETBUF(delinstp->f, Char);
  fscanf(delinstp->f, "%*[^\n]");
  getc(delinstp->f);
  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "w", internal.f);
    else
      internal.f = fopen(internal.name, "w");
  } else {
    if (internal.f != NULL)
      rewind(internal.f);
    else
      internal.f = tmpfile();
  }
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  SETUPBUF(internal.f, Char);
  for (line = 1; line <= copylines; line++)
    copyaline(delinstp, &internal);


  fprintf(internal.f,
	  "f       twocoordinates (char): If \"t\" use coordinates2\n");


  copyfile(delinstp, &internal);


  if (*internal.name != '\0') {
    if (internal.f != NULL)
      internal.f = freopen(internal.name, "r", internal.f);
    else
      internal.f = fopen(internal.name, "r");
  } else
    rewind(internal.f);
  if (internal.f == NULL)
    _EscIO2(FileNotFound, internal.name);
  RESETBUF(internal.f, Char);
  if (*delinstp->name != '\0') {
    if (delinstp->f != NULL)
      delinstp->f = freopen(delinstp->name, "w", delinstp->f);
    else
      delinstp->f = fopen(delinstp->name, "w");
  } else {
    if (delinstp->f != NULL)
      rewind(delinstp->f);
    else
      delinstp->f = tmpfile();
  }
  if (delinstp->f == NULL)
    _EscIO2(FileNotFound, delinstp->name);
  SETUPBUF(delinstp->f, Char);
  fprintf(delinstp->f,
    "%4.2f        version of delinstp that this parameter file is designed for.\n",
    parameterversion);
  copyfile(&internal, delinstp);




  if (*delinstp->name != '\0') {
    if (delinstp->f != NULL)
      delinstp->f = freopen(delinstp->name, "r", delinstp->f);
    else
      delinstp->f = fopen(delinstp->name, "r");
  } else
    rewind(delinstp->f);
  if (delinstp->f == NULL)
    _EscIO2(FileNotFound, delinstp->name);
  RESETBUF(delinstp->f, Char);
  fscanf(delinstp->f, "%*[^\n]");

  getc(delinstp->f);
  if (internal.f != NULL)
    fclose(internal.f);
}

#undef copylines


/*


*/

Local long r100(a)
double a;
{
  return ((long)floor(100 * a + 0.5));
}

Local boolean equalversion(a, b)
double a, b;
{
  if (r100(a) == r100(b))
    return true;
  else
    return false;
}

Local boolean lessthan(a, b)
double a, b;
{
  if (r100(a) < r100(b))
    return true;
  else
    return false;
}

Local boolean greaterthan(a, b)
double a, b;
{
  if (r100(a) > r100(b))
    return true;
  else
    return false;
}



Static Void upgradeparameters(delinstp)
_TEXT *delinstp;
{
  double parameterversion;


  if (*delinstp->name != '\0') {
    if (delinstp->f != NULL)
      delinstp->f = freopen(delinstp->name, "r", delinstp->f);
    else
      delinstp->f = fopen(delinstp->name, "r");
  } else
    rewind(delinstp->f);
  if (delinstp->f == NULL)
    _EscIO2(FileNotFound, delinstp->name);
  RESETBUF(delinstp->f, Char);
  fscanf(delinstp->f, "%lg%*[^\n]", &parameterversion);
  getc(delinstp->f);

  if (!(lessthan(parameterversion, updateversion) |
	greaterthan(parameterversion, versionupperbound)))
    return;

  printf("\007You have an old parameter file!, version %4.2f!\n",
	 parameterversion);
  printf("          version = %4.2f\n", version);
  printf("    updateversion = %4.2f\n", updateversion);
  printf(" parameterversion = %4.2f\n", parameterversion);
  printf("versionupperbound = %4.2f\n", versionupperbound);

  if (equalversion(parameterversion, 1.07))
    upgradeto114(delinstp);

}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *inst;
  Char multiline;
} ;

Local Void givecoo(afile, v, LINK)
_TEXT *afile;
long v;
struct LOC_themain *LINK;
{
  if (v < 0)
    putc('-', afile->f);
  else
    putc('+', afile->f);
}

Local Void endinst(quoteline, LINK)
Char quoteline;
struct LOC_themain *LINK;
{
  if (quoteline == '"')
    putc('"', LINK->inst->f);
  putc(';', LINK->inst->f);
  if (LINK->multiline == 'm')
    putc('\n', LINK->inst->f);
  else
    putc(' ', LINK->inst->f);
}



Static Void themain(title, organism, chromosome, piece, names, coordinates,
		    coordinates2, directions, delinstp, inst_)
_TEXT *title, *organism, *chromosome, *piece, *names, *coordinates,
      *coordinates2, *directions, *delinstp, *inst_;
{
  struct LOC_themain V;
  double parameterversion;
  long fromrange, torange;
  Char removeduplicates;
  boolean keepduplicates;
  Char twocoordinates;
  /*
*/
  boolean dotwocoordinates;

  long coo, coo2;
  stringDelila coostring;
  _TEXT coofile;
  long coocounter;
  boolean checkcoordinates = false;
  long coodigits;
  boolean skipinstruction;
  stringDelila skipstring;

  long coodir;
  Char dir, rid;
  long maxinstructions;
  long instructions = 0, skipped = 0, dataline = 0;
  stringDelila neworgname, newchrname, newpiename, oldorgname, oldchrname,
	       oldpiename;
  boolean gotten;
  _TEXT TEMP;

  V.inst = inst_;
  coofile.f = NULL;
  *coofile.name = '\0';
  printf("delinst %4.2f\n", version);
  if (*delinstp->name != '\0') {
    if (delinstp->f != NULL)
      delinstp->f = freopen(delinstp->name, "r", delinstp->f);
    else
      delinstp->f = fopen(delinstp->name, "r");
  } else
    rewind(delinstp->f);
  if (delinstp->f == NULL)
    _EscIO2(FileNotFound, delinstp->name);
  RESETBUF(delinstp->f, Char);
  fscanf(delinstp->f, "%lg%*[^\n]", &parameterversion);
  getc(delinstp->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    upgradeparameters(delinstp);
    /*


*/
  }


  fscanf(delinstp->f, "%ld%ld%*[^\n]", &fromrange, &torange);
  getc(delinstp->f);
  fscanf(delinstp->f, "%ld%*[^\n]", &maxinstructions);
  getc(delinstp->f);
  fscanf(delinstp->f, "%c%*[^\n]", &removeduplicates);
  getc(delinstp->f);
  if (removeduplicates == '\n')
    removeduplicates = ' ';
  fscanf(delinstp->f, "%c%*[^\n]", &V.multiline);
  getc(delinstp->f);
  if (V.multiline == '\n')
    V.multiline = ' ';
  fscanf(delinstp->f, "%c%*[^\n]", &twocoordinates);
  getc(delinstp->f);
  if (twocoordinates == '\n')
    twocoordinates = ' ';
  if (twocoordinates == 't')
    dotwocoordinates = true;
  else
    dotwocoordinates = false;

  keepduplicates = (removeduplicates != 'r');

  if (*title->name != '\0') {
    if (title->f != NULL)
      title->f = freopen(title->name, "r", title->f);
    else
      title->f = fopen(title->name, "r");
  } else
    rewind(title->f);
  if (title->f == NULL)
    _EscIO2(FileNotFound, title->name);
  RESETBUF(title->f, Char);
  if (*organism->name != '\0') {
    if (organism->f != NULL)
      organism->f = freopen(organism->name, "r", organism->f);
    else
      organism->f = fopen(organism->name, "r");
  } else
    rewind(organism->f);
  if (organism->f == NULL)
    _EscIO2(FileNotFound, organism->name);
  RESETBUF(organism->f, Char);
  if (*chromosome->name != '\0') {
    if (chromosome->f != NULL)
      chromosome->f = freopen(chromosome->name, "r", chromosome->f);
    else
      chromosome->f = fopen(chromosome->name, "r");
  } else
    rewind(chromosome->f);
  if (chromosome->f == NULL)
    _EscIO2(FileNotFound, chromosome->name);
  RESETBUF(chromosome->f, Char);
  if (*piece->name != '\0') {
    if (piece->f != NULL)
      piece->f = freopen(piece->name, "r", piece->f);
    else
      piece->f = fopen(piece->name, "r");
  } else
    rewind(piece->f);
  if (piece->f == NULL)
    _EscIO2(FileNotFound, piece->name);
  RESETBUF(piece->f, Char);
  if (*names->name != '\0') {
    if (names->f != NULL)
      names->f = freopen(names->name, "r", names->f);
    else
      names->f = fopen(names->name, "r");
  } else
    rewind(names->f);
  if (names->f == NULL)
    _EscIO2(FileNotFound, names->name);
  RESETBUF(names->f, Char);
  if (*coordinates->name != '\0') {
    if (coordinates->f != NULL)
      coordinates->f = freopen(coordinates->name, "r", coordinates->f);
    else
      coordinates->f = fopen(coordinates->name, "r");
  } else
    rewind(coordinates->f);
  if (coordinates->f == NULL)
    _EscIO2(FileNotFound, coordinates->name);
  RESETBUF(coordinates->f, Char);
  if (dotwocoordinates) {
    if (*coordinates2->name != '\0') {
      if (coordinates2->f != NULL)
	coordinates2->f = freopen(coordinates2->name, "r", coordinates2->f);
      else
	coordinates2->f = fopen(coordinates2->name, "r");
    } else
      rewind(coordinates2->f);
    if (coordinates2->f == NULL)
      _EscIO2(FileNotFound, coordinates2->name);
    RESETBUF(coordinates2->f, Char);
  }
  if (*directions->name != '\0') {
    if (directions->f != NULL)
      directions->f = freopen(directions->name, "r", directions->f);
    else
      directions->f = fopen(directions->name, "r");
  } else
    rewind(directions->f);
  if (directions->f == NULL)
    _EscIO2(FileNotFound, directions->name);
  RESETBUF(directions->f, Char);

  if (*V.inst->name != '\0') {
    if (V.inst->f != NULL)
      V.inst->f = freopen(V.inst->name, "w", V.inst->f);
    else
      V.inst->f = fopen(V.inst->name, "w");
  } else {
    if (V.inst->f != NULL)
      rewind(V.inst->f);
    else
      V.inst->f = tmpfile();
  }
  if (V.inst->f == NULL)
    _EscIO2(FileNotFound, V.inst->name);
  SETUPBUF(V.inst->f, Char);
  copyALLlines(title, V.inst);

  clearstring(&neworgname);
  clearstring(&newchrname);
  clearstring(&newpiename);

  clearstring(&oldorgname);
  clearstring(&oldchrname);
  clearstring(&oldpiename);

  oldorgname.letters[0] = '.';
  oldorgname.length = 1;
  clearstring(&oldchrname);
  clearstring(&oldpiename);


  while (!P_eoln(coordinates->f) &&
	 (instructions < maxinstructions || maxinstructions < 0)) {
    dataline++;

    skipinstruction = false;
    if (checkcoordinates | (P_peek(coordinates->f) != '9' &&
			    P_peek(coordinates->f) != '8' &&
			    P_peek(coordinates->f) != '7' &&
			    P_peek(coordinates->f) != '6' &&
			    P_peek(coordinates->f) != '5' &&
			    P_peek(coordinates->f) != '4' &&
			    P_peek(coordinates->f) != '3' &&
			    P_peek(coordinates->f) != '2' &&
			    P_peek(coordinates->f) != '1' &&
			    P_peek(coordinates->f) != '0')) {
      /*

*/
      getstring(coordinates, &coostring, &gotten);

      if (!gotten)
	halt();
      coodigits = 0;
      for (coocounter = 0; coocounter < coostring.length; coocounter++) {
	if (coostring.letters[coocounter] == '9' ||
	    coostring.letters[coocounter] == '8' ||
	    coostring.letters[coocounter] == '7' ||
	    coostring.letters[coocounter] == '6' ||
	    coostring.letters[coocounter] == '5' ||
	    coostring.letters[coocounter] == '4' ||
	    coostring.letters[coocounter] == '3' ||
	    coostring.letters[coocounter] == '2' ||
	    coostring.letters[coocounter] == '1' ||
	    coostring.letters[coocounter] == '0')
	  coodigits++;
      }
      if (coodigits == 0) {
	printf("\n** Bad coordinate at line %ld of the coordinates file **\n",
	       dataline);
	printf("The bad coordinate string is: \"");
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &coostring);
	printf("\"\n");
	printf("THE INSTRUCTION WILL NOT BE GENERATED\n");
	skipinstruction = true;
	skipped++;
      }
      if (!skipinstruction) {
	if (*coofile.name != '\0') {
	  if (coofile.f != NULL)
	    coofile.f = freopen(coofile.name, "w", coofile.f);
	  else
	    coofile.f = fopen(coofile.name, "w");
	} else {
	  if (coofile.f != NULL)
	    rewind(coofile.f);
	  else
	    coofile.f = tmpfile();
	}
	if (coofile.f == NULL)
	  _EscIO2(FileNotFound, coofile.name);
	SETUPBUF(coofile.f, Char);
	writestring(&coofile, &coostring);
	if (*coofile.name != '\0') {
	  if (coofile.f != NULL)
	    coofile.f = freopen(coofile.name, "r", coofile.f);
	  else
	    coofile.f = fopen(coofile.name, "r");
	} else
	  rewind(coofile.f);
	if (coofile.f == NULL)
	  _EscIO2(FileNotFound, coofile.name);
	RESETBUF(coofile.f, Char);
	/*
*/
	fscanf(coofile.f, "%ld", &coo);
      }
    } else {
      fscanf(coordinates->f, "%ld%*[^\n]", &coo);
      getc(coordinates->f);
      if (dotwocoordinates) {
	fscanf(coordinates2->f, "%ld%*[^\n]", &coo2);
	getc(coordinates2->f);
      }
    }

    if (!skipinstruction) {
      /*

*/
      if (!P_eoln(organism->f)) {
	getstring(organism, &neworgname, &gotten);
	if (!gotten)
	  halt();
	if (keepduplicates | (!equalstring(oldorgname, neworgname))) {
	  fprintf(V.inst->f, "org ");
	  writestring(V.inst, &neworgname);
	  endinst(' ', &V);
	}
	if (!equalstring(oldorgname, neworgname))
	  copystring(neworgname, &oldorgname);
      }

      if (!P_eoln(chromosome->f)) {
	getstring(chromosome, &newchrname, &gotten);
	/*

*/
	/*




*/

	if (!gotten)
	  halt();
	if (!equalstring(oldchrname, newchrname)) {
	  if (V.multiline == 'm')
	    putc('\n', V.inst->f);
	}
	if (keepduplicates | (!equalstring(oldchrname, newchrname))) {
	  fprintf(V.inst->f, "chr ");
	  writestring(V.inst, &newchrname);
	  endinst(' ', &V);
	}
	if (!equalstring(oldchrname, newchrname))
	  copystring(newchrname, &oldchrname);
      }

      if (!P_eoln(piece->f)) {
	getstring(piece, &newpiename, &gotten);
	if (!gotten)
	  halt();
	if (keepduplicates | (!equalstring(oldpiename, newpiename))) {
	  fprintf(V.inst->f, "pie ");
	  writestring(V.inst, &newpiename);
	  endinst(' ', &V);
	}
	if (!equalstring(oldpiename, newpiename)) {
	  copystring(newpiename, &oldpiename);

	  if (!keepduplicates) {
	    if (V.multiline == 'm')
	      putc('\n', V.inst->f);
	  }
	}
      }

      if (!P_eoln(names->f)) {
	fprintf(V.inst->f, "name \"");
	copyaword(names, V.inst);
	endinst('"', &V);
      }

      if (!P_eoln(directions->f)) {
	fscanf(directions->f, "%c%*[^\n]", &dir);
	getc(directions->f);
	if (dir == '\n')
	  dir = ' ';
	if (dir == 'F' || dir == 'f')
	  dir = '+';

	if (dir == '+')
	  rid = '-';
	else
	  rid = '+';

	if (dir == '+')
	  coodir = 1;
	else {
	  coodir = -1;

	}
      }

      fprintf(V.inst->f, "get from %ld ", coo);
      givecoo(V.inst, coodir * fromrange, &V);
      fprintf(V.inst->f, "%ld", labs(fromrange));
      fprintf(V.inst->f, " to ");

      if (dotwocoordinates)
	fprintf(V.inst->f, "%ld", coo2);
      else
	fprintf(V.inst->f, "same");

      putc(' ', V.inst->f);
      givecoo(V.inst, coodir * torange, &V);
      fprintf(V.inst->f, "%ld", labs(torange));
      fprintf(V.inst->f, " direction %c", dir);
      fprintf(V.inst->f, ";\n");

      instructions++;
      continue;
    }
    getstring(organism, &skipstring, &gotten);
    printf("  organism: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &skipstring);
    putchar('\n');

    getstring(chromosome, &skipstring, &gotten);
    printf("chromosome: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &skipstring);
    putchar('\n');

    getstring(piece, &skipstring, &gotten);
    printf("     piece: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &skipstring);
    putchar('\n');

    getstring(names, &skipstring, &gotten);
    printf("      name: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &skipstring);
    printf("\ncoordinate: ");

    /*


*/
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &coostring);
    printf(" (BAD)\n");

    getstring(directions, &skipstring, &gotten);
    printf(" direction: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &skipstring);
    putchar('\n');

  }



  printf("\n%ld instructions generated\n", instructions);
  printf("%ld instructions skipped\n", skipped);

  if (coofile.f != NULL)
    fclose(coofile.f);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  inst.f = NULL;
  strcpy(inst.name, "inst");
  delinstp.f = NULL;
  strcpy(delinstp.name, "delinstp");
  directions.f = NULL;
  strcpy(directions.name, "directions");
  coordinates2.f = NULL;
  strcpy(coordinates2.name, "coordinates2");
  coordinates.f = NULL;
  strcpy(coordinates.name, "coordinates");
  names.f = NULL;
  strcpy(names.name, "names");
  piece.f = NULL;
  strcpy(piece.name, "piece");
  chromosome.f = NULL;
  strcpy(chromosome.name, "chromosome");
  organism.f = NULL;
  strcpy(organism.name, "organism");
  title.f = NULL;
  strcpy(title.name, "title");
  themain(&title, &organism, &chromosome, &piece, &names, &coordinates,
	  &coordinates2, &directions, &delinstp, &inst);
_L1:
  if (title.f != NULL)
    fclose(title.f);
  if (organism.f != NULL)
    fclose(organism.f);
  if (chromosome.f != NULL)
    fclose(chromosome.f);
  if (piece.f != NULL)
    fclose(piece.f);
  if (names.f != NULL)
    fclose(names.f);
  if (coordinates.f != NULL)
    fclose(coordinates.f);
  if (coordinates2.f != NULL)
    fclose(coordinates2.f);
  if (directions.f != NULL)
    fclose(directions.f);
  if (delinstp.f != NULL)
    fclose(delinstp.f);
  if (inst.f != NULL)
    fclose(inst.f);
  exit(EXIT_SUCCESS);
}



/* End. */
