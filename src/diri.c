/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "diri.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*










*/



#define version         1.10
/*










*/
#define updateversion   1.00



/*






















































*/


#define maxarray        1000



typedef struct box {
  double range;
  long occur;
  double probdist;
  boolean isitzero;
} box;


typedef box map[maxarray];



Static _TEXT ari, histog, dcout, bri;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define tab             9






Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == tab);
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



Static Void distribution(thefile, distsize, correction, store)
_TEXT *thefile;
long *distsize;
double *correction;
box *store;
{
  /*
*/
  long x;
  double normvalue;
  long sum = 0;
  double ln2 = log(2.0);
  Char c;
  long FORLIM;

  if (*thefile->name != '\0') {
    if (thefile->f != NULL)
      thefile->f = freopen(thefile->name, "r", thefile->f);
    else
      thefile->f = fopen(thefile->name, "r");
  } else
    rewind(thefile->f);
  if (thefile->f == NULL)
    _EscIO2(FileNotFound, thefile->name);
  RESETBUF(thefile->f, Char);
  c = getc(thefile->f);
  if (c == '\n')
    c = ' ';
  while (c == '*') {
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    c = getc(thefile->f);
    if (c == '\n')
      c = ' ';
  }

  *distsize = 0;
  while (!BUFEOF(thefile->f)) {
    (*distsize)++;

    fscanf(thefile->f, "%lg", &store[*distsize - 1].range);
    fscanf(thefile->f, "%ld%*[^\n]", &store[*distsize - 1].occur);
    getc(thefile->f);

    if ((long)floor(store[*distsize - 1].range + 0.5) !=
	store[*distsize - 1].range) {
      printf("distribution values must be increments of 1\n");
      halt();
    }
  }

  FORLIM = *distsize;
  for (x = 0; x < FORLIM; x++)
    sum += store[x].occur;

  FORLIM = *distsize;
  for (x = 0; x < FORLIM; x++) {
    store[x].isitzero = false;
    normvalue = (double)store[x].occur / sum;
    if (normvalue == 0)
      store[x].isitzero = true;
    else
      store[x].probdist = -1 * log(normvalue) / ln2;
  }


  *correction = (*distsize - 1) / (2 * log(2.0) * sum);
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



Static Void diloop(correction, distsize, store)
double correction;
long distsize;
box *store;
{
  Char c;
  double sum;
  long coorda, coordb;
  double ria, rib;
  long z, x, difference;
  boolean foundsite;

  if (*ari.name != '\0') {
    if (ari.f != NULL)
      ari.f = freopen(ari.name, "r", ari.f);
    else
      ari.f = fopen(ari.name, "r");
  } else
    rewind(ari.f);
  if (ari.f == NULL)
    _EscIO2(FileNotFound, ari.name);
  RESETBUF(ari.f, Char);
  if (*bri.name != '\0') {
    if (bri.f != NULL)
      bri.f = freopen(bri.name, "r", bri.f);
    else
      bri.f = fopen(bri.name, "r");
  } else
    rewind(bri.f);
  if (bri.f == NULL)
    _EscIO2(FileNotFound, bri.name);
  RESETBUF(bri.f, Char);
  if (*dcout.name != '\0') {
    if (dcout.f != NULL)
      dcout.f = freopen(dcout.name, "w", dcout.f);
    else
      dcout.f = fopen(dcout.name, "w");
  } else {
    if (dcout.f != NULL)
      rewind(dcout.f);
    else
      dcout.f = tmpfile();
  }
  if (dcout.f == NULL)
    _EscIO2(FileNotFound, dcout.name);
  SETUPBUF(dcout.f, Char);
  printf("* diri %4.2f\n", version);
  fprintf(dcout.f, "* diri %4.2f\n", version);
  fprintf(dcout.f,
    "*  loc. a    loc. b        dist.  Ri A        Ri B        log2(p.d.) tot info\n");
  fprintf(dcout.f, "*  Sample correction value = %1.6f\n", correction);
  while ((!BUFEOF(ari.f)) & (!BUFEOF(bri.f))) {
    c = getc(ari.f);
    if (c == '\n')
      c = ' ';
    c = getc(bri.f);
    if (c == '\n')
      c = ' ';
    if (c == '*') {
      putc('*', dcout.f);
      copyaline(&ari, &dcout);
      putc('*', dcout.f);
      copyaline(&bri, &dcout);
      continue;
    }
    /*


*/
    for (x = 1; x <= 4; x++)
      skipcolumn(&ari);
    for (x = 1; x <= 4; x++)
      skipcolumn(&bri);
    fscanf(ari.f, "%ld", &coorda);
    fscanf(bri.f, "%ld", &coordb);
    fscanf(ari.f, "%lg", &ria);
    fscanf(bri.f, "%lg", &rib);
    difference = coorda - coordb;
    foundsite = false;
    for (z = 0; z < distsize; z++) {
      if (labs(difference) == fabs(store[z].range) && store[z].occur > 0) {
	sum = ria + rib - store[z].probdist + correction;
	foundsite = true;

	fprintf(dcout.f, "%10ld", coorda);
	fprintf(dcout.f, "%10ld", coordb);
	fprintf(dcout.f, "%10ld", -labs(difference));
	fprintf(dcout.f, "%12.6f", ria);
	fprintf(dcout.f, "%12.6f", rib);
	fprintf(dcout.f, "%12.6f", store[z].probdist);
	fprintf(dcout.f, "%12.6f\n", sum);

      }
    }
    if (foundsite == false) {
      fprintf(dcout.f, "* not site");
      fprintf(dcout.f, "%10ld", coorda);
      fprintf(dcout.f, "%10ld\n", coordb);
    }
    fscanf(ari.f, "%*[^\n]");
    getc(ari.f);
    fscanf(bri.f, "%*[^\n]");
    getc(bri.f);
  }
  /*

*/
}



Static Void themain()
{
  long distsize;
  double correction;
  map store;
  _TEXT TEMP;

  printf("diri %4.2f\n", version);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  PatentMessage(&TEMP);

  distribution(&histog, &distsize, &correction, store);
  printf("%2ld\n", store[19].occur);
  printf("here\n");
  diloop(correction, distsize, store);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  bri.f = NULL;
  strcpy(bri.name, "bri");
  dcout.f = NULL;
  strcpy(dcout.name, "dcout");
  histog.f = NULL;
  strcpy(histog.name, "histog");
  ari.f = NULL;
  strcpy(ari.name, "ari");
  themain();
  printf("done\n");
_L1:
  if (ari.f != NULL)
    fclose(ari.f);
  if (histog.f != NULL)
    fclose(histog.f);
  if (dcout.f != NULL)
    fclose(dcout.f);
  if (bri.f != NULL)
    fclose(bri.f);
  exit(EXIT_SUCCESS);
}



/* End. */
