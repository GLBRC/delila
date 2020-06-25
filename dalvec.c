/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dalvec.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         2.20
/*
*/



/*


















































































*/



#define infofield       8
#define infodecim       5

#define nfield          4
#define mapmax          26

#define pwid            8
#define pdec            5

#define pnum            100000L

#define cxmove          30



#define maxstring       150



#define fillermax       21




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



Static _TEXT rsdata, dalvecp, symvec;


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
  t->state = 0;
  t->skip = true;
  t->found = false;
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





Static Void header(infile, outfile)
_TEXT *infile, *outfile;
{
  /*
*/
  long index;

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

  fprintf(outfile->f, "* dalvec %4.2f\n", version);


  for (index = 1; index <= 3; index++)
    copyaline(infile, outfile);

  fprintf(outfile->f, "*\n");
  fprintf(outfile->f,
	  "* position, number of sequences, information Rs, variance of Rs\n");
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






Static double sign(a)
double a;
{
  if (a >= 0)
    return 1.0;
  else
    return -1.0;
}


#define bignumber       pnum



Static Void themain(rsdata, dalvecp, symvec)
_TEXT *rsdata, *dalvecp, *symvec;
{
  boolean chilogo = false;
  double chia, chic, chig, chit, chitotal, nl4;
  rstype rdata;
  Char parameter;
  long position, FORLIM;
  double TEMP;

  printf("dalvec %4.2f\n", version);
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
  if (*symvec->name != '\0') {
    if (symvec->f != NULL)
      symvec->f = freopen(symvec->name, "w", symvec->f);
    else
      symvec->f = fopen(symvec->name, "w");
  } else {
    if (symvec->f != NULL)
      rewind(symvec->f);
    else
      symvec->f = tmpfile();
  }
  if (symvec->f == NULL)
    _EscIO2(FileNotFound, symvec->name);
  SETUPBUF(symvec->f, Char);

  if (*dalvecp->name != '\0') {
    if (dalvecp->f != NULL)
      dalvecp->f = freopen(dalvecp->name, "r", dalvecp->f);
    else
      dalvecp->f = fopen(dalvecp->name, "r");
  } else
    rewind(dalvecp->f);
  if (dalvecp->f == NULL)
    _EscIO2(FileNotFound, dalvecp->name);
  RESETBUF(dalvecp->f, Char);
  if (!BUFEOF(dalvecp->f)) {
    if (!P_eoln(dalvecp->f)) {
      fscanf(dalvecp->f, "%c%*[^\n]", &parameter);
      getc(dalvecp->f);
      if (parameter == '\n')
	parameter = ' ';
    }
    if (parameter == 'c')
      chilogo = true;
  }

  if (chilogo)
    printf("Chilogo will be produced\n");
  else
    printf("Normal sequence logo will be produced\n");

  if (BUFEOF(rsdata->f)) {
    printf("empty rsdata file\n");
    halt();
    return;
  }
  header(rsdata, symvec);
  fprintf(symvec->f, "4 number of symbols in DNA or RNA\n");


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
  readrsrange(rsdata, &rdata);
  getrsbegin(rsdata);


  getrsbegin(rsdata);


  FORLIM = rdata.rstop;
  for (position = rdata.rstart; position <= FORLIM; position++) {
    if (P_peek(rsdata->f) != '*') {
      readrsdata(rsdata, &rdata);


      if (chilogo) {
	/*

*/
	nl4 = rdata.nl / 4.0;
	TEMP = rdata.nal - nl4;
	chia = TEMP * TEMP;
	TEMP = rdata.ncl - nl4;
	chic = TEMP * TEMP;
	TEMP = rdata.ngl - nl4;
	chig = TEMP * TEMP;
	TEMP = rdata.ntl - nl4;
	chit = TEMP * TEMP;
	chitotal = chia + chic + chig + chit;
	if (chitotal > 0) {
	  rdata.nal = (long)floor(
		bignumber * (chia / chitotal) * sign(rdata.nal - nl4) + 0.5);
	  rdata.ncl = (long)floor(
		bignumber * (chic / chitotal) * sign(rdata.ncl - nl4) + 0.5);
	  rdata.ngl = (long)floor(
		bignumber * (chig / chitotal) * sign(rdata.ngl - nl4) + 0.5);
	  rdata.ntl = (long)floor(
		bignumber * (chit / chitotal) * sign(rdata.ntl - nl4) + 0.5);
	  rdata.nl = bignumber;
	} else {
	  rdata.nal = 1;
	  rdata.ncl = 1;
	  rdata.ngl = 1;
	  rdata.ntl = 1;
	  rdata.nl = 4;
	}
      }

      /*


*/

      fprintf(symvec->f, "%*ld %*ld %*.*f % .*E\n",
	      nfield, rdata.l, infofield, rdata.nl, infofield, infodecim,
	      rdata.rsl, P_max((int)(infofield + 3) - 7, 1), rdata.varhnb);

      fprintf(symvec->f, "a %*ld\n", nfield, rdata.nal);
      fprintf(symvec->f, "c %*ld\n", nfield, rdata.ncl);
      fprintf(symvec->f, "g %*ld\n", nfield, rdata.ngl);
      fprintf(symvec->f, "t %*ld\n", nfield, rdata.ntl);

    }
  }

}

#undef bignumber


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  symvec.f = NULL;
  strcpy(symvec.name, "symvec");
  dalvecp.f = NULL;
  strcpy(dalvecp.name, "dalvecp");
  rsdata.f = NULL;
  strcpy(rsdata.name, "rsdata");
  themain(&rsdata, &dalvecp, &symvec);
_L1:
  if (rsdata.f != NULL)
    fclose(rsdata.f);
  if (dalvecp.f != NULL)
    fclose(dalvecp.f);
  if (symvec.f != NULL)
    fclose(symvec.f);
  exit(EXIT_SUCCESS);
}



/* End. */
