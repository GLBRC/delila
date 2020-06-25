/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "trex.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*
*/
#define updateversion   1.00



/*

















































*/



#define maxstring       150



typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



typedef Char idtype[2];


Static _TEXT fin, trexp, fout, frebp, fresep;


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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
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



Static Void copynoreturn(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  /*

*/

}


Static double plogp(f)
double f;
{
  if (f <= 0.0)
    return 0.0;
  else
    return (-(f * log(f) / log(2.0)));
}


Static Void copyto(fin, fout, idwanted)
_TEXT *fin, *fout;
Char *idwanted;
{
  stringDelila astring;
  idtype id;
  boolean gotten;

  memcpy(id, "  ", sizeof(idtype));
  while (strncmp(id, idwanted, sizeof(idtype))) {
    P_readpaoc(fin->f, id, (int)(sizeof(idtype)));
    getstring(fin, &astring, &gotten);
    if (!gotten) {
      printf("unexpected end of file\n");
      halt();
    }
    fprintf(fout->f, "%.2s", id);
    writestring(fout, &astring);
    putc('\n', fout->f);
  }
}



Static Void getidstring(fin, thestring)
_TEXT *fin;
stringDelila *thestring;
{
  boolean gotten;

  skipblanks(fin);
  getstring(fin, thestring, &gotten);
  if (!gotten) {
    printf("unexpected end of file\n");
    halt();
  }
}




Static Void testmatrix(nb, c, bad)
double nb;
Char c;
boolean *bad;
{
  /*
*/
  if (nb == (long)floor(nb + 0.5))
    return;
  *bad = true;
  printf("base %c has value %4.2f\n", c, nb);
  printf("It is from a consindex and the matrix is NO GOOD\n");
}


#define wid             7
#define dec             0

#define giveinfo        false



Static Void themain(fin, trexp, fout, fresep, frebp)
_TEXT *fin, *trexp, *fout, *fresep, *frebp;
{
  stringDelila acstring;
  boolean bad;
  stringDelila iddesired, idstring;
  long count = 0;
  boolean done = false;
  double H;
  idtype id;
  long i;
  double na, nc, ng, nt, n, parameterversion, R;
  _TEXT TEMP;
  long FORLIM;

  printf("trex %4.2f\n", version);
  if (*trexp->name != '\0') {
    if (trexp->f != NULL)
      trexp->f = freopen(trexp->name, "r", trexp->f);
    else
      trexp->f = fopen(trexp->name, "r");
  } else
    rewind(trexp->f);
  if (trexp->f == NULL)
    _EscIO2(FileNotFound, trexp->name);
  RESETBUF(trexp->f, Char);
  fscanf(trexp->f, "%lg%*[^\n]", &parameterversion);
  getc(trexp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  getidstring(trexp, &iddesired);
  printf("searching for ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writestring(&TEMP, &iddesired);
  putchar('\n');

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
  if (*fresep->name != '\0') {
    if (fresep->f != NULL)
      fresep->f = freopen(fresep->name, "w", fresep->f);
    else
      fresep->f = fopen(fresep->name, "w");
  } else {
    if (fresep->f != NULL)
      rewind(fresep->f);
    else
      fresep->f = tmpfile();
  }
  if (fresep->f == NULL)
    _EscIO2(FileNotFound, fresep->name);
  SETUPBUF(fresep->f, Char);
  if (*frebp->name != '\0') {
    if (frebp->f != NULL)
      frebp->f = freopen(frebp->name, "w", frebp->f);
    else
      frebp->f = fopen(frebp->name, "w");
  } else {
    if (frebp->f != NULL)
      rewind(frebp->f);
    else
      frebp->f = tmpfile();
  }
  if (frebp->f == NULL)
    _EscIO2(FileNotFound, frebp->name);
  SETUPBUF(frebp->f, Char);
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

  writestring(frebp, &iddesired);
  putc('\n', frebp->f);


  if (iddesired.letters[iddesired.length - 2] == '_' &&
      iddesired.letters[iddesired.length - 2] == 'C') {
    printf("This is a consindex matrix.\n");
    printf("It cannot be used.\n");
    halt();
  }
  while (!done) {
    if (BUFEOF(fin->f)) {
      done = true;
      break;
    }
    P_readpaoc(fin->f, id, (int)(sizeof(idtype)));
    /*

*/
    if (!strncmp(id, "AC", sizeof(idtype))) {
      getidstring(fin, &acstring);
      continue;
    }

    if (strncmp(id, "ID", sizeof(idtype))) {
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      continue;
    }
    getidstring(fin, &idstring);
    printf("idstring = ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writestring(&TEMP, &idstring);
    putchar('\n');

    if (!equalstring(iddesired, idstring))
      continue;
    printf("identified\n");

    done = true;

    fprintf(fout->f, "AC  ");
    writestring(fout, &acstring);
    fprintf(fout->f, "\nXX\n");

    fprintf(fout->f, "ID  ");
    writestring(fout, &idstring);
    putc('\n', fout->f);

    copyto(fin, fout, "P0");
    bad = false;
    while (strncmp(id, "XX", sizeof(idtype))) {
      P_readpaoc(fin->f, id, (int)(sizeof(idtype)));
      /*

*/
      if (strncmp(id, "XX", sizeof(idtype))) {
	fscanf(fin->f, "%lg%lg%lg%lg", &na, &nc, &ng, &nt);


	if (!bad) {
	  testmatrix(na, 'a', &bad);
	  testmatrix(nc, 'c', &bad);
	  testmatrix(ng, 'g', &bad);
	  testmatrix(nt, 't', &bad);
	  if (bad) {
	    printf("THE MATRIX IS BAD!\n");
	    printf("It is a consindex.\n");
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
	    if (*fresep->name != '\0') {
	      if (fresep->f != NULL)
		fresep->f = freopen(fresep->name, "w", fresep->f);
	      else
		fresep->f = fopen(fresep->name, "w");
	    } else {
	      if (fresep->f != NULL)
		rewind(fresep->f);
	      else
		fresep->f = tmpfile();
	    }
	    if (fresep->f == NULL)
	      _EscIO2(FileNotFound, fresep->name);
	    SETUPBUF(fresep->f, Char);
	    if (*frebp->name != '\0') {
	      if (frebp->f != NULL)
		frebp->f = freopen(frebp->name, "w", frebp->f);
	      else
		frebp->f = fopen(frebp->name, "w");
	    } else {
	      if (frebp->f != NULL)
		rewind(frebp->f);
	      else
		frebp->f = tmpfile();
	    }
	    if (frebp->f == NULL)
	      _EscIO2(FileNotFound, frebp->name);
	    SETUPBUF(frebp->f, Char);
	    halt();
	  }
	}

	fprintf(fout->f, "%.2s%*.*f%*.*f%*.*f%*.*f",
		id, wid, dec, na, wid, dec, nc, wid, dec, ng, wid, dec, nt);

	fprintf(fresep->f, "%.2s%*.*f%*.*f%*.*f%*.*f\n",
		id, wid, dec, na, wid, dec, nc, wid, dec, ng, wid, dec, nt);

	fprintf(frebp->f, "%.2s%*.*f%*.*f%*.*f%*.*f\n",
		id, wid, dec, na, wid, dec, nc, wid, dec, ng, wid, dec, nt);

	copynoreturn(fin, fout);

	if (giveinfo) {
	  fprintf(fout->f, " | ");

	  n = na + nc + ng + nt;
	  H = plogp(na / n) + plogp(nc / n) + plogp(ng / n) + plogp(nt / n);
	  R = 2 - H;

	  fprintf(fout->f, " R %5.2f", R);
	  fprintf(fout->f, " | ");
	  FORLIM = (long)floor(20 * R + 0.5);
	  for (i = 0; i <= FORLIM; i++)
	    putc('*', fout->f);
	  putc('|', fout->f);
	}

	putc('\n', fout->f);
      }
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
    }
    fprintf(fout->f, "XX\n");
    copyto(fin, fout, "//");
  }


  printf("DONE\n");

}

#undef wid
#undef dec
#undef giveinfo


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fresep.f = NULL;
  strcpy(fresep.name, "fresep");
  frebp.f = NULL;
  strcpy(frebp.name, "frebp");
  fout.f = NULL;
  strcpy(fout.name, "fout");
  trexp.f = NULL;
  strcpy(trexp.name, "trexp");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &trexp, &fout, &fresep, &frebp);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (trexp.f != NULL)
    fclose(trexp.f);
  if (fout.f != NULL)
    fclose(fout.f);
  if (frebp.f != NULL)
    fclose(frebp.f);
  if (fresep.f != NULL)
    fclose(fresep.f);
  exit(EXIT_SUCCESS);
}






























/* End. */
