/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "malopt.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.09
/*
*/



/*































































*/



#define maxclasses      100
/*
*/
#define maxsequences    300
#define nwid            4
#define wid             10
#define dec             5


Static _TEXT optalign, maloptp, xyin;


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


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *xyin;
  long brightness;
  double colorbound, distance;
  double H[maxclasses + 1];
  long saturation;

  long xclass;
  double xrectangle;
  /*
*/
  long yclass;
  double yrectangle;
} ;

Local double hue(color, LINK)
double color;
struct LOC_themain *LINK;
{
  /*
*/
  return (color / LINK->colorbound);
}

Local Void columndefinition(LINK)
struct LOC_themain *LINK;
{
  fprintf(LINK->xyin->f, "*\n");
  fprintf(LINK->xyin->f, "* data columns:\n");
  fprintf(LINK->xyin->f, "*  1: x class\n");
  fprintf(LINK->xyin->f, "*  2: y class\n");
  fprintf(LINK->xyin->f, "*  3: x H, uncertainty in bits\n");
  fprintf(LINK->xyin->f, "*  4: y H, uncertainty in bits\n");
  fprintf(LINK->xyin->f, "*  5: x rectangle width\n");
  fprintf(LINK->xyin->f, "*  6: y rectangle height\n");
  fprintf(LINK->xyin->f, "*  7: distance from x class to y class\n");
  fprintf(LINK->xyin->f, "*  8: distance converted to color\n");
  fprintf(LINK->xyin->f, "*  9: hue for color\n");
  fprintf(LINK->xyin->f, "* 10: saturation for color\n");
  fprintf(LINK->xyin->f, "* 11: brightness for color\n");
  fprintf(LINK->xyin->f, "*\n");
}

Local Void generateoutput(LINK)
struct LOC_themain *LINK;
{
  if (LINK->distance > LINK->colorbound)
    LINK->brightness = 0;
  else
    LINK->brightness = 1;
  fprintf(LINK->xyin->f, " %*ld", nwid, LINK->xclass);
  fprintf(LINK->xyin->f, " %*ld", nwid, LINK->yclass);
  fprintf(LINK->xyin->f, " %*.*f", wid, dec, LINK->H[LINK->xclass]);
  fprintf(LINK->xyin->f, " %*.*f", wid, dec, LINK->H[LINK->yclass]);
  fprintf(LINK->xyin->f, " %*.*f", nwid, dec, LINK->xrectangle);
  fprintf(LINK->xyin->f, " %*.*f", nwid, dec, LINK->yrectangle);
  fprintf(LINK->xyin->f, " %*.*f", wid, dec, LINK->distance);
  fprintf(LINK->xyin->f, " %*.*f", wid, dec, hue(LINK->distance, LINK));
  fprintf(LINK->xyin->f, " %*ld", nwid, LINK->saturation);
  fprintf(LINK->xyin->f, " %*ld\n", nwid, LINK->brightness);
}



Static Void themain(optalign, maloptp, xyin_)
_TEXT *optalign, *maloptp, *xyin_;
{
  struct LOC_themain V;
  long avalue, class_;
  long classes = 0;
  double d;
  long doclasses, s, sequences;
  double sumsq;
  long theclasses[maxclasses][maxsequences];
  /*
*/
  long zerovalue, FORLIM1;

  V.xyin = xyin_;
  printf("malopt %4.2f\n", version);
  if (*optalign->name != '\0') {
    if (optalign->f != NULL)
      optalign->f = freopen(optalign->name, "r", optalign->f);
    else
      optalign->f = fopen(optalign->name, "r");
  } else
    rewind(optalign->f);
  if (optalign->f == NULL)
    _EscIO2(FileNotFound, optalign->name);
  RESETBUF(optalign->f, Char);


  if (*maloptp->name != '\0') {
    if (maloptp->f != NULL)
      maloptp->f = freopen(maloptp->name, "r", maloptp->f);
    else
      maloptp->f = fopen(maloptp->name, "r");
  } else
    rewind(maloptp->f);
  if (maloptp->f == NULL)
    _EscIO2(FileNotFound, maloptp->name);
  RESETBUF(maloptp->f, Char);
  fscanf(maloptp->f, "%ld%*[^\n]", &doclasses);
  getc(maloptp->f);
  fscanf(maloptp->f, "%lg%*[^\n]", &V.colorbound);
  getc(maloptp->f);
  fscanf(maloptp->f, "%lg%lg%*[^\n]", &V.xrectangle, &V.yrectangle);
  getc(maloptp->f);


  while (P_peek(optalign->f) == '*') {
    fscanf(optalign->f, "%*[^\n]");
    getc(optalign->f);
  }
  fscanf(optalign->f, "%*[^\n]");
  getc(optalign->f);
  fscanf(optalign->f, "%*[^\n]");
  getc(optalign->f);
  fscanf(optalign->f, "%*[^\n]");
  getc(optalign->f);
  while (!BUFEOF(optalign->f)) {
    if (P_eoln(optalign->f))
      classes++;
    fscanf(optalign->f, "%*[^\n]");
    getc(optalign->f);
  }



  if (*V.xyin->name != '\0') {
    if (V.xyin->f != NULL)
      V.xyin->f = freopen(V.xyin->name, "w", V.xyin->f);
    else
      V.xyin->f = fopen(V.xyin->name, "w");
  } else {
    if (V.xyin->f != NULL)
      rewind(V.xyin->f);
    else
      V.xyin->f = tmpfile();
  }
  if (V.xyin->f == NULL)
    _EscIO2(FileNotFound, V.xyin->name);
  SETUPBUF(V.xyin->f, Char);
  fprintf(V.xyin->f, "* malopt %4.2f\n", version);
  if (*optalign->name != '\0') {
    if (optalign->f != NULL)
      optalign->f = freopen(optalign->name, "r", optalign->f);
    else
      optalign->f = fopen(optalign->name, "r");
  } else
    rewind(optalign->f);
  if (optalign->f == NULL)
    _EscIO2(FileNotFound, optalign->name);
  RESETBUF(optalign->f, Char);
  while (P_peek(optalign->f) == '*')
    copyaline(optalign, V.xyin);
  fscanf(optalign->f, "%*[^\n]");
  getc(optalign->f);
  fscanf(optalign->f, "%ld%*[^\n]", &sequences);
  getc(optalign->f);
  fscanf(optalign->f, "%*[^\n]");

  getc(optalign->f);
  fprintf(V.xyin->f, "* %ld classes found\n", classes);
  printf("%ld classes found\n", classes);
  fprintf(V.xyin->f, "* %ld sequences aligned\n", sequences);
  printf("%ld sequences aligned\n", sequences);

  fprintf(V.xyin->f, "*\n");
  fprintf(V.xyin->f, "* maloptp:\n");
  fprintf(V.xyin->f,
	  "* %10ld1: doclasses: integer number of classes to process\n",
	  doclasses);
  fprintf(V.xyin->f, "* %*.*f2: colorbound: maximum color distance\n",
	  wid, dec, V.colorbound);
  columndefinition(&V);

  if (doclasses > classes) {
    printf("You asked to process %ld but there are only %ld classes\n",
	   doclasses, classes);
    printf("%ld will be processed\n", classes);
    doclasses = classes;
  }

  if (doclasses > maxclasses) {
    printf("You asked to process %ld but I can only handle %ld classes\n",
	   doclasses, (long)maxclasses);
    printf("%ld will be processed\n", (long)maxclasses);
    doclasses = maxclasses;
  }


  V.saturation = 1;
  V.brightness = 1;
  for (V.yclass = 0; V.yclass < doclasses; V.yclass++) {
    V.xclass = 0;
    V.distance = V.colorbound * ((double)V.yclass / doclasses);

    V.H[V.xclass] = 0.0;
    V.H[V.yclass] = V.yclass;
    generateoutput(&V);
  }


  for (V.xclass = 1; V.xclass <= doclasses; V.xclass++) {
    fscanf(optalign->f, "%*[^\n]");
    getc(optalign->f);
    fscanf(optalign->f, "%ld", &class_);

    while (P_peek(optalign->f) != '=')
      getc(optalign->f);

    getc(optalign->f);
    fscanf(optalign->f, "%lg%*[^\n]", &V.H[V.xclass]);
    getc(optalign->f);
    if (class_ != V.xclass) {
      printf("class misalignment\n");
      halt();
    }

    fscanf(optalign->f, "%ld", &zerovalue);
    theclasses[V.xclass-1][0] = 0;
    for (s = 1; s < sequences; s++) {
      fscanf(optalign->f, "%ld", &avalue);
      theclasses[V.xclass-1][s] = avalue - zerovalue;
    }
    fscanf(optalign->f, "%*[^\n]");
    getc(optalign->f);
  }
  printf("class vectors loaded\n");

  for (V.xclass = 1; V.xclass <= doclasses; V.xclass++) {
    FORLIM1 = V.xclass;
    for (V.yclass = 1; V.yclass <= FORLIM1; V.yclass++) {
      sumsq = 0.0;
      for (s = 0; s < sequences; s++) {
	d = theclasses[V.xclass-1][s] - theclasses[V.yclass-1][s];
	sumsq += d * d;
      }
      V.distance = sqrt(sumsq);


      if (V.distance > V.colorbound)
	V.brightness = 0;
      else
	V.brightness = 1;

      generateoutput(&V);
    }

  }
  printf("done\n");

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  maloptp.f = NULL;
  strcpy(maloptp.name, "maloptp");
  optalign.f = NULL;
  strcpy(optalign.name, "optalign");
  themain(&optalign, &maloptp, &xyin);
_L1:
  if (optalign.f != NULL)
    fclose(optalign.f);
  if (maloptp.f != NULL)
    fclose(maloptp.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
