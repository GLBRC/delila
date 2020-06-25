/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "lidel.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.30
/*
*/



/*














































































*/



#define maxstring       150



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;


Static _TEXT fin, lidelp, inst;


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



Static boolean equalstring(a, b)
stringDelila a, b;
{
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
  long l;

  b->length = a.length;
  for (l = 0; l < a.length; l++)
    b->letters[l] = a.letters[l];
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



Static Void ncopyaline(fin, fout)
_TEXT *fin, *fout;
{
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
}



Static Void sign(fout, int_)
_TEXT *fout;
long int_;
{
  /*
*/
  if (int_ < 0)
    fprintf(fout->f, " %ld", int_);
  else
    fprintf(fout->f, " +%ld", int_);
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *inst;
  stringDelila b, bold;
  long fromrange, torange;
} ;

Local Void dopiecename(fin, LINK)
_TEXT *fin;
struct LOC_themain *LINK;
{
  grabtoken(fin, &LINK->b);
  if (equalstring(LINK->b, LINK->bold))
    return;
  fprintf(LINK->inst->f, "\npiece ");
  writestring(LINK->inst, &LINK->b);
  fprintf(LINK->inst->f, ";\n");
  copystring(LINK->b, &LINK->bold);
}

Local Void makeinst(inst, l, fromfirst, LINK)
_TEXT *inst;
long l;
boolean fromfirst;
struct LOC_themain *LINK;
{
  /*
*/
  fprintf(inst->f, "get from %ld", l);
  if (fromfirst)
    sign(inst, LINK->fromrange);
  else
    sign(inst, LINK->torange);
  fprintf(inst->f, " to %ld", l);
  if (fromfirst)
    sign(inst, LINK->torange);
  else
    sign(inst, LINK->fromrange);
  if (fromfirst)
    fprintf(inst->f, " direction +");
  else
    fprintf(inst->f, " direction -");
  fprintf(inst->f, ";\n");
}



Static Void themain(fin, lidelp, inst_)
_TEXT *fin, *lidelp, *inst_;
{
  struct LOC_themain V;
  long datacol, duplex, index, l, piecol;
  Char side, symmetry;

  V.inst = inst_;
  printf("lidel %4.2f\n", version);

  if (*lidelp->name != '\0') {
    if (lidelp->f != NULL)
      lidelp->f = freopen(lidelp->name, "r", lidelp->f);
    else
      lidelp->f = fopen(lidelp->name, "r");
  } else
    rewind(lidelp->f);
  if (lidelp->f == NULL)
    _EscIO2(FileNotFound, lidelp->name);
  RESETBUF(lidelp->f, Char);
  fscanf(lidelp->f, "%ld%ld%*[^\n]", &V.fromrange, &V.torange);
  getc(lidelp->f);
  fscanf(lidelp->f, "%ld%*[^\n]", &piecol);
  getc(lidelp->f);
  fscanf(lidelp->f, "%ld%*[^\n]", &datacol);
  getc(lidelp->f);
  if (piecol == datacol) {
    printf("piece name column can not equal coordinate column\n");
    halt();
  }
  if (piecol < 1 || datacol < 1) {
    printf("columns must be postive integers\n");
    halt();
  }

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
  fprintf(V.inst->f, "title \"");
  ncopyaline(lidelp, V.inst);
  fprintf(V.inst->f, "\";\n");

  fprintf(V.inst->f, "(* lidel %4.2f *)\n", version);

  fprintf(V.inst->f, "organism ");
  ncopyaline(lidelp, V.inst);
  fprintf(V.inst->f, ";\n");

  fprintf(V.inst->f, "chromosome ");
  ncopyaline(lidelp, V.inst);
  fprintf(V.inst->f, ";\n");

  fscanf(lidelp->f, "%c%c%*[^\n]", &symmetry, &side);
  getc(lidelp->f);
  if (symmetry == '\n')
    symmetry = ' ';
  if (side == '\n')
    side = ' ';
  if (symmetry != 'o' && symmetry != 'e') {
    printf("symmetry must be e (even) or o (odd)\n");
    halt();
  }
  if (symmetry == 'e') {
    if (side != 'r' && side != 'l') {
      printf("side of fin coordinates must be l (left) or r (right)\n");
      halt();
    }
  }

  fscanf(lidelp->f, "%ld%*[^\n]", &duplex);
  getc(lidelp->f);
  if (duplex < 1 || duplex > 2) {
    printf("You can have only 1 or 2 instructions per integer\n");
    halt();
  }

  fprintf(V.inst->f, "default numbering piece;\n");
  fprintf(V.inst->f, "default numbering 1;\n");
  fprintf(V.inst->f, "default out-of-range reduce-range;\n");


  clearstring(&V.b);
  clearstring(&V.bold);
  while (!BUFEOF(fin->f)) {
    if (P_peek(fin->f) == '*') {
      fprintf(V.inst->f, "(* ");
      ncopyaline(fin, V.inst);
      fprintf(V.inst->f, " *)\n");
      continue;
    }
    if (piecol < datacol) {
      for (index = 1; index < piecol; index++)
	skipcolumn(fin);
      dopiecename(fin, &V);
      for (index = piecol + 1; index < datacol; index++)
	skipcolumn(fin);
      fscanf(fin->f, "%ld%*[^\n]", &l);
      getc(fin->f);
    } else {
      for (index = 1; index < datacol; index++)
	skipcolumn(fin);
      fscanf(fin->f, "%ld", &l);
      for (index = datacol + 1; index < piecol; index++)
	skipcolumn(fin);
      dopiecename(fin, &V);
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
    }

    if (duplex == 2)
      putc('\n', V.inst->f);

    switch (symmetry) {

    case 'e':
      switch (side) {

      case 'l':
	makeinst(V.inst, l + 1, true, &V);
	break;

      case 'r':
	makeinst(V.inst, l - 1, true, &V);
	break;
      }
      break;

    case 'o':
      makeinst(V.inst, l, true, &V);
      break;
    }

    if (duplex == 2)
      makeinst(V.inst, l, false, &V);

  }
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
  lidelp.f = NULL;
  strcpy(lidelp.name, "lidelp");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &lidelp, &inst);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (lidelp.f != NULL)
    fclose(lidelp.f);
  if (inst.f != NULL)
    fclose(inst.f);
  exit(EXIT_SUCCESS);
}



/* End. */
