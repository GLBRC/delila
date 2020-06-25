/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "denri.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.12
/*









*/

#define updateversion   1.00



/*

























































*/



#define rangefrom       (-500)
#define rangeto         500
#define minRi           (-70)
#define maxRi           20


Static _TEXT data, denrip, den, xyplop;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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


#define wid             7
#define dec             5


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *data, *den;
  long bins[rangeto - rangefrom + 1][maxRi - minRi + 1];

  /*

*/
  double deltaRi;
  long numbertoprocess;
  double parameterversion;
  long position, wantfrom, wantto, wantminRi, wantmaxRi;
} ;

Local Void readparameters(denrip, LINK)
_TEXT *denrip;
struct LOC_themain *LINK;
{
  if (*denrip->name != '\0') {
    if (denrip->f != NULL)
      denrip->f = freopen(denrip->name, "r", denrip->f);
    else
      denrip->f = fopen(denrip->name, "r");
  } else
    rewind(denrip->f);
  if (denrip->f == NULL)
    _EscIO2(FileNotFound, denrip->name);
  RESETBUF(denrip->f, Char);
  fscanf(denrip->f, "%lg%*[^\n]", &LINK->parameterversion);
  getc(denrip->f);
  if (LINK->parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(denrip->f, "%ld%*[^\n]", &LINK->numbertoprocess);
  getc(denrip->f);
  fscanf(denrip->f, "%ld%ld%*[^\n]", &LINK->wantfrom, &LINK->wantto);
  getc(denrip->f);
  fscanf(denrip->f, "%ld%ld%*[^\n]", &LINK->wantminRi, &LINK->wantmaxRi);
  getc(denrip->f);

  if (LINK->wantfrom < rangefrom) {
    printf("wantfrom cannot be less than %ld\n", (long)rangefrom);
    halt();
  }

  if (LINK->wantto > rangeto) {
    printf("wantto cannot be more than %ld\n", (long)rangeto);
    halt();
  }

  if (LINK->wantfrom > LINK->wantto) {
    printf("wantfrom cannot be more than wantto\n");
    halt();
  }

  if (LINK->wantminRi < minRi) {
    printf("wantminRi cannot be less than %ld\n", (long)minRi);
    halt();
  }

  if (LINK->wantmaxRi > maxRi) {
    printf("maxRi cannot be more than %ld\n", (long)maxRi);
    halt();
  }

  if (LINK->wantminRi > LINK->wantmaxRi) {
    printf("wantminRi cannot be more than wantmaxRi\n");
    halt();
  }

}

Local Void writeparameters(out, LINK)
_TEXT *out;
struct LOC_themain *LINK;
{
  fprintf(out->f, "* denri %4.2f\n", version);
  fprintf(out->f, "* %9.2f parameterversion for denri\n",
	  LINK->parameterversion);
  fprintf(out->f, "* %9ld numbertoprocess\n", LINK->numbertoprocess);
  fprintf(out->f, "* %4ld %4ld wantfrom, wantto\n",
	  LINK->wantfrom, LINK->wantto);
  fprintf(out->f, "* %4ld %4ld wantminRi, wantmaxRi\n",
	  LINK->wantminRi, LINK->wantmaxRi);
}

Local Void clearbins(LINK)
struct LOC_themain *LINK;
{
  long position, d, FORLIM, FORLIM1;

  printf("clearing bins ...\n");
  FORLIM = LINK->wantto;
  for (position = LINK->wantfrom; position <= FORLIM; position++) {
    FORLIM1 = LINK->wantmaxRi;
    for (d = LINK->wantminRi; d <= FORLIM1; d++)
      LINK->bins[position - rangefrom][d - minRi] = 0;
  }
  printf("cleared\n");
}

Local Void fillonedatum(LINK)
struct LOC_themain *LINK;
{
  long d;

  if (P_peek(LINK->data->f) == '*') {
    fscanf(LINK->data->f, "%*[^\n]");
    getc(LINK->data->f);
    return;
  }
  skipcolumn(LINK->data);
  skipcolumn(LINK->data);
  skipcolumn(LINK->data);
  fscanf(LINK->data->f, "%ld", &LINK->position);
  skipcolumn(LINK->data);
  skipcolumn(LINK->data);
  skipcolumn(LINK->data);
  skipcolumn(LINK->data);
  fscanf(LINK->data->f, "%lg", &LINK->deltaRi);
  fscanf(LINK->data->f, "%*[^\n]");
  getc(LINK->data->f);
  d = (long)floor(LINK->deltaRi + 0.5);

  /*














*/

  if (d >= LINK->wantminRi && d <= LINK->wantmaxRi &&
      LINK->position >= LINK->wantfrom && LINK->position <= LINK->wantto)
    LINK->bins[LINK->position - rangefrom][d - minRi]++;
}

Local Void fillbins(LINK)
struct LOC_themain *LINK;
{
  long d, FORLIM;

  printf("filling bins ...\n");
  if (*LINK->data->name != '\0') {
    if (LINK->data->f != NULL)
      LINK->data->f = freopen(LINK->data->name, "r", LINK->data->f);
    else
      LINK->data->f = fopen(LINK->data->name, "r");
  } else
    rewind(LINK->data->f);
  if (LINK->data->f == NULL)
    _EscIO2(FileNotFound, LINK->data->name);
  RESETBUF(LINK->data->f, Char);
  if (LINK->numbertoprocess < 0) {
    while (!BUFEOF(LINK->data->f))
      fillonedatum(LINK);
  } else {
    FORLIM = LINK->numbertoprocess;
    for (d = 1; d <= FORLIM; d++)
      fillonedatum(LINK);
  }
  printf("filled\n");
}

Local Void process(LINK)
struct LOC_themain *LINK;
{
  long b, d, sum;
  double r, hue, saturation, brightness;
  long position, FORLIM, FORLIM1;

  printf("processing...\n");
  fprintf(LINK->den->f, "* columns:\n");
  fprintf(LINK->den->f, "* 1: position\n");
  fprintf(LINK->den->f, "* 2: bits\n");
  fprintf(LINK->den->f, "* 3: total counts at this position\n");
  fprintf(LINK->den->f,
    "* 4: (counts at this bit-position) / (total counts at this position)\n");
  fprintf(LINK->den->f, "* 5: 1\n");
  fprintf(LINK->den->f, "* 6: 1\n");
  fprintf(LINK->den->f, "* 7: hue\n");
  fprintf(LINK->den->f, "* 8: saturation\n");
  fprintf(LINK->den->f, "* 9: brightness\n");
  FORLIM = LINK->wantto;
  for (position = LINK->wantfrom; position <= FORLIM; position++) {
    sum = 0;
    FORLIM1 = LINK->wantmaxRi;
    for (d = LINK->wantminRi; d <= FORLIM1; d++)
      sum += LINK->bins[position - rangefrom][d - minRi];
    if (sum > 0) {
      FORLIM1 = LINK->wantmaxRi;
      for (d = LINK->wantminRi; d <= FORLIM1; d++) {
	b = LINK->bins[position - rangefrom][d - minRi];

	if (b == 0) {
	  brightness = 0.9;
	  saturation = 0.0;
	} else {
	  r = (double)b / sum;
	  hue = 0.84 * r + 0.16;
	  saturation = 1.0;
	  brightness = 1.0;
	}
	fprintf(LINK->den->f,
		"%4ld %3ld %*ld %*ld %*d %*d %*.*f %*.*f %*.*f\n",
		position, d, wid, sum, wid, b, wid, 1, wid, 1, wid, dec, hue,
		wid, dec, saturation, wid, dec, brightness);
      }
    }
  }
  printf("processed\n");
}

#undef wid
#undef dec

Local Void mkxyplop(xyplop, LINK)
_TEXT *xyplop;
struct LOC_themain *LINK;
{
  long ix, iy;

  ix = LINK->wantto - LINK->wantfrom;
  while (ix > 20)
    ix = (long)(ix / 10.0);
  iy = (long)((LINK->wantmaxRi - LINK->wantminRi) / 10.0);
  if (*xyplop->name != '\0') {
    if (xyplop->f != NULL)
      xyplop->f = freopen(xyplop->name, "w", xyplop->f);
    else
      xyplop->f = fopen(xyplop->name, "w");
  } else {
    if (xyplop->f != NULL)
      rewind(xyplop->f);
    else
      xyplop->f = tmpfile();
  }
  if (xyplop->f == NULL)
    _EscIO2(FileNotFound, xyplop->name);
  SETUPBUF(xyplop->f, Char);
  fprintf(xyplop->f, "3 6       zerox zeroy         \n");
  fprintf(xyplop->f, "x %ld %ld zx min max          \n",
	  LINK->wantfrom, LINK->wantto);
  fprintf(xyplop->f, "y %ld %ld  zy min max          \n",
	  LINK->wantminRi, LINK->wantmaxRi);
  fprintf(xyplop->f, "%ld %ld 2 2 xinterval yinterval \n", ix, iy);
  fprintf(xyplop->f, "4 4       xwidth    ywidth    \n");
  fprintf(xyplop->f, "0 0       xdecimal  ydecimal  \n");
  fprintf(xyplop->f, "15 15     xsize     ysize     \n");

  fprintf(xyplop->f, "position, bases\n");
  fprintf(xyplop->f, "deltaRi, bits.");
  fprintf(xyplop->f, "  number of sequences at position 0: %ld\n",
	  LINK->bins[-rangefrom][-minRi]);

  fprintf(xyplop->f, "a         zc                  \n");
  fprintf(xyplop->f, "n 2       zxl base            \n");
  fprintf(xyplop->f, "n 2       zyl base            \n");
  fprintf(xyplop->f, "          ********************\n");
  fprintf(xyplop->f, "1 2       xcolumn   ycolumn   \n");
  fprintf(xyplop->f, "0         symbol column       \n");
  fprintf(xyplop->f, "5  6      xscolumn  yscolumn  \n");
  fprintf(xyplop->f, "7 8 9     hue saturation brightnes\n");
  fprintf(xyplop->f, "          ********************\n");
  fprintf(xyplop->f, "R         symbol-to-plot      \n");
  fprintf(xyplop->f, "i         symbol-flag         \n");
  fprintf(xyplop->f, "-1.0      symbol sizex        \n");
  fprintf(xyplop->f, "-1.0      symbol sizey        \n");
  fprintf(xyplop->f, "n         connection size     \n");
  fprintf(xyplop->f, "n  0.125  linetype  size      \n");
  fprintf(xyplop->f, "          ********************\n");
  fprintf(xyplop->f, ".\n");
  fprintf(xyplop->f, "          **** version 8.50 of\n");
  fprintf(xyplop->f, "l 0 0 0.125  User defined line\n");
  /*

*/
}



Static Void themain(data_, denrip, den_, xyplop)
_TEXT *data_, *denrip, *den_, *xyplop;
{
  struct LOC_themain V;
  _TEXT TEMP;

  V.data = data_;
  V.den = den_;
  printf("denri %4.2f\n", version);
  readparameters(denrip, &V);
  if (*V.den->name != '\0') {
    if (V.den->f != NULL)
      V.den->f = freopen(V.den->name, "w", V.den->f);
    else
      V.den->f = fopen(V.den->name, "w");
  } else {
    if (V.den->f != NULL)
      rewind(V.den->f);
    else
      V.den->f = tmpfile();
  }
  if (V.den->f == NULL)
    _EscIO2(FileNotFound, V.den->name);
  SETUPBUF(V.den->f, Char);
  writeparameters(V.den, &V);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writeparameters(&TEMP, &V);
  clearbins(&V);
  fillbins(&V);
  process(&V);
  mkxyplop(xyplop, &V);
  printf("done\n");
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyplop.f = NULL;
  strcpy(xyplop.name, "xyplop");
  den.f = NULL;
  strcpy(den.name, "den");
  denrip.f = NULL;
  strcpy(denrip.name, "denrip");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &denrip, &den, &xyplop);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (denrip.f != NULL)
    fclose(denrip.f);
  if (den.f != NULL)
    fclose(den.f);
  if (xyplop.f != NULL)
    fclose(xyplop.f);
  exit(EXIT_SUCCESS);
}





















/* End. */
