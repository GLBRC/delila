/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "waveplot.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.09
/*










*/
#define updateversion   1.00



/*





































































*/



#define wid             9
#define dec             5


Static _TEXT searchfeatures, waveplotp, xyin;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}






Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == '\t');
}


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



Static Void skipquote(thefile)
_TEXT *thefile;
{
  /*
*/
  Char c;
  boolean done = false;

  skipblanks(thefile);
  c = P_peek(thefile->f);

  if (c != '"' && c != '\'') {
    /*


*/
    skipnonblanks(thefile);


    return;
  }

  getc(thefile->f);
  while (!done) {
    if (P_eoln(thefile->f)) {
      printf("(eoln)");
      done = true;
    } else {
      if (P_peek(thefile->f) == c) {
	done = true;

	getc(thefile->f);
      } else {
	getc(thefile->f);

      }
    }
  }

}



Static Void backspace(f, n)
_TEXT *f;
long n;
{
  long i;

  for (i = 1; i <= n; i++)
    putchar('\b');
}



Static double realmodulo(position, wavelength)
double position, wavelength;
{
  /*
*/
  double p = position;

  while (p > wavelength)
    p -= wavelength;

  return p;
}



Static Void themain(searchfeatures, waveplotp, xyin)
_TEXT *searchfeatures, *waveplotp, *xyin;
{
  double parameterversion;

  double position, orientation, firstbase = LONG_MAX;
  double reducedposition;
  /*
*/
  double normalizedposition;
  /*
*/


  long zerobase;
  double wavelength;

  long wavevertical, waveheight;

  double ri, riconsensus, hue, saturation, brightness;
  _TEXT TEMP;

  printf("waveplot %4.2f\n", version);
  if (*waveplotp->name != '\0') {
    if (waveplotp->f != NULL)
      waveplotp->f = freopen(waveplotp->name, "r", waveplotp->f);
    else
      waveplotp->f = fopen(waveplotp->name, "r");
  } else
    rewind(waveplotp->f);
  if (waveplotp->f == NULL)
    _EscIO2(FileNotFound, waveplotp->name);
  RESETBUF(waveplotp->f, Char);
  fscanf(waveplotp->f, "%lg%*[^\n]", &parameterversion);
  getc(waveplotp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }
  fscanf(waveplotp->f, "%ld%*[^\n]", &zerobase);
  getc(waveplotp->f);
  fscanf(waveplotp->f, "%lg%*[^\n]", &wavelength);
  getc(waveplotp->f);
  if (!BUFEOF(waveplotp->f)) {
    fscanf(waveplotp->f, "%lg%*[^\n]", &riconsensus);
    getc(waveplotp->f);
  } else
    riconsensus = 20.0;
  printf("riconsensus : %*.*f\n", wid, dec, riconsensus);
  fscanf(waveplotp->f, "%ld%*[^\n]", &wavevertical);
  getc(waveplotp->f);
  fscanf(waveplotp->f, "%ld%*[^\n]", &waveheight);
  getc(waveplotp->f);

  if (*searchfeatures->name != '\0') {
    if (searchfeatures->f != NULL)
      searchfeatures->f = freopen(searchfeatures->name, "r", searchfeatures->f);
    else
      searchfeatures->f = fopen(searchfeatures->name, "r");
  } else
    rewind(searchfeatures->f);
  if (searchfeatures->f == NULL)
    _EscIO2(FileNotFound, searchfeatures->name);
  RESETBUF(searchfeatures->f, Char);
  if (*xyin->name != '\0') {
    if (xyin->f != NULL)
      xyin->f = freopen(xyin->name, "w", xyin->f);
    else
      xyin->f = fopen(xyin->name, "w");
  } else {
    if (xyin->f != NULL)
      rewind(xyin->f);
    else
      xyin->f = tmpfile();
  }
  if (xyin->f == NULL)
    _EscIO2(FileNotFound, xyin->name);
  SETUPBUF(xyin->f, Char);

  printf("coordinate: ");

  /*

*/

  while (!BUFEOF(searchfeatures->f)) {
    if (P_eoln(searchfeatures->f)) {
      fscanf(searchfeatures->f, "%*[^\n]");
      getc(searchfeatures->f);
      continue;
    }
    if (P_peek(searchfeatures->f) != '@') {
      fscanf(searchfeatures->f, "%*[^\n]");
      getc(searchfeatures->f);
      continue;
    }

    skipcolumn(searchfeatures);

    skipcolumn(searchfeatures);

    fscanf(searchfeatures->f, "%lg", &position);

    fscanf(searchfeatures->f, "%lg", &orientation);

    skipquote(searchfeatures);


    skipquote(searchfeatures);


    if (P_peek(searchfeatures->f) == '0' ||
	P_peek(searchfeatures->f) == '9' ||
	P_peek(searchfeatures->f) == '8' ||
	P_peek(searchfeatures->f) == '7' ||
	P_peek(searchfeatures->f) == '6' ||
	P_peek(searchfeatures->f) == '5' ||
	P_peek(searchfeatures->f) == '4' ||
	P_peek(searchfeatures->f) == '3' ||
	P_peek(searchfeatures->f) == '2' || P_peek(searchfeatures->f) == '1')
      fscanf(searchfeatures->f, "%lg", &ri);
    else
      skipcolumn(searchfeatures);
    fscanf(searchfeatures->f, "%*[^\n]");



    getc(searchfeatures->f);
    if (firstbase == LONG_MAX)
      firstbase = position;


    reducedposition = realmodulo(position, wavelength);
    normalizedposition = reducedposition / wavelength;
    hue = 1.0;
    if (riconsensus <= 0)
      saturation = 1.0;
    else
      saturation = ri / riconsensus;
    brightness = 1.0;

    fprintf(xyin->f, "%*ld %*ld %*.*f %*.*f %*.*f %*.*f %*.*f %*.*f\n",
	    wid, (long)floor(position + 0.5), wid,
	    (long)floor(position - firstbase + 0.5), wid, dec,
	    reducedposition, wid, dec, normalizedposition, wid, dec, ri, wid,
	    dec, hue, wid, dec, saturation, wid, dec, brightness);
    printf("%*ld", wid, (long)floor(position + 0.5));
    TEMP.f = stdout;
    *TEMP.name = '\0';
    backspace(&TEMP, (long)wid);
    /*

*/
  }
  putchar('\n');

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
  waveplotp.f = NULL;
  strcpy(waveplotp.name, "waveplotp");
  searchfeatures.f = NULL;
  strcpy(searchfeatures.name, "searchfeatures");
  themain(&searchfeatures, &waveplotp, &xyin);
_L1:
  if (searchfeatures.f != NULL)
    fclose(searchfeatures.f);
  if (waveplotp.f != NULL)
    fclose(waveplotp.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}

/*














*/



/* End. */
