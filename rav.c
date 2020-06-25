/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rav.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.22
/*

*/
#define updateversion   1.20



/*

















































































































































































*/



#define maxwin          2000
#define infowid         10
#define infodec         5



typedef struct infopoint {

  long position, samples;
  double information, variance;
} infopoint;

typedef struct infoarray {

  long symbols;

  infopoint data[maxwin];

  long minwindowsize, maxwindowsize, stepwindowsize, windowsize;
  double halfwindowsize;

  long current, last;

  double Rtotal, Vtotal;


  double colorX, colorY, colorWidth, colorHeight, colorThick;
  long colorTransform;
  double colorAtransform, colorBtransform;
} infoarray;


Static _TEXT symvec, ravp, xyin, marks, colorkey;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define magic           0.85



Static Void docolortransform(colorTransform, colorAtransform, colorBtransform,
			     hue)
long colorTransform;
double colorAtransform, colorBtransform, *hue;
{
  /*














*/
  switch (colorTransform) {

  case 0:
    /* blank case */
    break;

  case 1:
    *hue = colorAtransform * *hue + colorBtransform;
    break;

  case 2:
    if (*hue < colorAtransform / 2)
      *hue = 0.3;
    else if (*hue > colorBtransform / 2)
      *hue = 0.3;
    else
      *hue = 0.0;
    break;

  case 3:
    *hue = 0.84 * *hue + 0.16;
    break;

  case 4:
    *hue = magic - magic * *hue;
    break;
  }
}

#undef magic


#define ticjump         0.1
/*
*/
#define ticlength       0.2



Static Void makecolorkey(psout, colorX, colorY, colorWidth, colorHeight,
			 colorThick, colorTransform, colorAtransform,
			 colorBtransform)
_TEXT *psout;
double colorX, colorY, colorWidth, colorHeight, colorThick;
long colorTransform;
double colorAtransform, colorBtransform;
{
  /*


*/
  double cmtopoints = 72 / 2.54;
  long hueinteger;
  double hue, y;

  if (*psout->name != '\0') {
    if (psout->f != NULL)
      psout->f = freopen(psout->name, "w", psout->f);
    else
      psout->f = fopen(psout->name, "w");
  } else {
    if (psout->f != NULL)
      rewind(psout->f);
    else
      psout->f = tmpfile();
  }
  if (psout->f == NULL)
    _EscIO2(FileNotFound, psout->name);
  SETUPBUF(psout->f, Char);
  fprintf(psout->f, "%%! colorkey\n");


  fprintf(psout->f, "/Times-Roman findfont 10 scalefont setfont\n");
  fprintf(psout->f, "/c {1 1 sethsbcolor} def\n");
  fprintf(psout->f, "%% define the colors\n");
  fprintf(psout->f, "gsave\n");
  fprintf(psout->f, "%8.5f setlinewidth\n", colorThick * cmtopoints);
  for (hueinteger = 0; hueinteger <= 100; hueinteger++) {
    hue = hueinteger / 100.0;
    y = (colorHeight * hue + colorY) * cmtopoints;
    docolortransform(colorTransform, colorAtransform, colorBtransform, &hue);
    fprintf(psout->f, "%8.5f c", hue);
    fprintf(psout->f, " %8.5f %8.5f moveto %8.5f 0 rlineto stroke\n",
	    colorX * cmtopoints, y, colorWidth * cmtopoints);
    if (hueinteger % 5 == 0) {
      fprintf(psout->f, "gsave\n");
      fprintf(psout->f, "1 setlinewidth\n");
      fprintf(psout->f, "1 0 0 sethsbcolor\n");
      fprintf(psout->f, " %8.5f %8.5f moveto\n",
	      (colorX + colorWidth + ticjump) * cmtopoints, y);
      fprintf(psout->f, "currentpoint translate 0 0 moveto\n");
      fprintf(psout->f, "%8.5f 0 rlineto currentpoint stroke\n",
	      ticlength * cmtopoints);
      fprintf(psout->f, "translate 0 0 moveto\n");
      if (hueinteger % 50 == 0) {
	fprintf(psout->f, "(%2ld\n", (long)floor(hueinteger / 50.0 + 0.5));
	if (hueinteger == 100)
	  fprintf(psout->f, " bits\n");
	fprintf(psout->f, ") show\n");
      }
      fprintf(psout->f, "grestore\n");
      fprintf(psout->f, "%8.5f setlinewidth\n", colorThick * cmtopoints);
    }
  }
  fprintf(psout->f, "grestore\n");
}

#undef ticjump
#undef ticlength



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



Static Void setheaders(symvec, xyin, marks, store)
_TEXT *symvec, *xyin, *marks;
infoarray *store;
{
  /*
*/
  if (*symvec->name != '\0') {
    if (symvec->f != NULL)
      symvec->f = freopen(symvec->name, "r", symvec->f);
    else
      symvec->f = fopen(symvec->name, "r");
  } else
    rewind(symvec->f);
  if (symvec->f == NULL)
    _EscIO2(FileNotFound, symvec->name);
  RESETBUF(symvec->f, Char);
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
  fprintf(xyin->f, "* rav %4.2f\n", version);
  while (P_peek(symvec->f) == '*')
    copyaline(symvec, xyin);

  if (*symvec->name != '\0') {
    if (symvec->f != NULL)
      symvec->f = freopen(symvec->name, "r", symvec->f);
    else
      symvec->f = fopen(symvec->name, "r");
  } else
    rewind(symvec->f);
  if (symvec->f == NULL)
    _EscIO2(FileNotFound, symvec->name);
  RESETBUF(symvec->f, Char);
  if (*marks->name != '\0') {
    if (marks->f != NULL)
      marks->f = freopen(marks->name, "w", marks->f);
    else
      marks->f = fopen(marks->name, "w");
  } else {
    if (marks->f != NULL)
      rewind(marks->f);
    else
      marks->f = tmpfile();
  }
  if (marks->f == NULL)
    _EscIO2(FileNotFound, marks->name);
  SETUPBUF(marks->f, Char);
  fprintf(marks->f, "* rav %4.2f\n", version);
  while (P_peek(symvec->f) == '*')
    copyaline(symvec, marks);

  fscanf(symvec->f, "%ld%*[^\n]", &store->symbols);
  getc(symvec->f);
  fprintf(xyin->f, "* number of symbols = %ld\n", store->symbols);
  fprintf(marks->f, "* number of symbols = %ld\n", store->symbols);

  fprintf(xyin->f, "* minimum window size = %ld\n", store->minwindowsize);
  fprintf(xyin->f, "* maximum window size = %ld\n", store->maxwindowsize);

  fprintf(marks->f, "* minimum window size = %ld\n", store->minwindowsize);
  fprintf(marks->f, "* maximum window size = %ld\n", store->maxwindowsize);

  fprintf(xyin->f, "*\n");
  fprintf(xyin->f, "* data columns:\n");
  fprintf(xyin->f, "* 1: position of window center\n");
  fprintf(xyin->f, "* 2: window size\n");
  fprintf(xyin->f, "* 3: average of information in window (bits)\n");
  fprintf(xyin->f, "* 4: standard deviation of information (bits)\n");
  fprintf(xyin->f, "* 5: color hue (0 to 1)\n");
  fprintf(xyin->f, "* 6: color saturation (0 to 1)\n");
  fprintf(xyin->f, "* 7: color brightness (0 to 1)\n");
  fprintf(xyin->f, "* 8: xsize\n");
  fprintf(xyin->f, "* 9: ysize\n");
}


Local Void die()
{
  printf("unexpected end of symvec file\n");
  halt();
}



Static Void readoneposition(symvec, i, symbols)
_TEXT *symvec;
infopoint *i;
long symbols;
{
  /*
*/
  long s;

  if (BUFEOF(symvec->f))
    die();
  fscanf(symvec->f, "%ld%ld%lg%lg%*[^\n]", &i->position, &i->samples,
	 &i->information, &i->variance);
  getc(symvec->f);

  for (s = 1; s <= symbols; s++) {
    if (BUFEOF(symvec->f))
      die();
    fscanf(symvec->f, "%*[^\n]");
    getc(symvec->f);
  }
}



Static Void reportwindow(xyin, marks, store)
_TEXT *xyin, *marks;
infoarray *store;
{
  double hue, Rav, Vav, stdev, windowcenter;
  double saturation = 1.0, brightness = 1.0, xsize = 1.0;
  double ysize;

  ysize = store->stepwindowsize;

  windowcenter = store->data[store->current - 1].position - store->halfwindowsize;
  windowcenter += 0.5;
  /*



*/

  Rav = store->Rtotal / store->windowsize;
  Vav = store->Vtotal / store->windowsize;

  /*
*/
  if (store->Vtotal <= 0.0)
    stdev = 0.0;
  else
    stdev = sqrt(Vav);

  fprintf(xyin->f, "%*.*f", infowid, infodec, windowcenter);
  fprintf(xyin->f, "%*ld", infowid, store->windowsize);
  fprintf(xyin->f, " %*.*f", infowid, infodec, Rav);
  fprintf(xyin->f, " %*.*f", infowid, infodec, stdev);
  hue = Rav / 2.0;
  docolortransform(store->colorTransform, store->colorAtransform,
		   store->colorBtransform, &hue);
  fprintf(xyin->f, " %*.*f", infowid, infodec, hue);
  fprintf(xyin->f, " %*.*f", infowid, infodec, saturation);
  fprintf(xyin->f, " %*.*f", infowid, infodec, brightness);
  fprintf(xyin->f, " %*.*f", infowid, infodec, xsize);
  fprintf(xyin->f, " %*.*f\n", infowid, infodec, ysize);



  fprintf(marks->f, "bs  %*.*f %*.*f %*.*f %*.*f\n",
	  infowid, infodec, windowcenter + 0.5, infowid, infodec, Rav - stdev,
	  infowid, infodec, windowcenter - 0.5, infowid, infodec,
	  Rav + stdev);

}



Static Void initialread(symvec, store)
_TEXT *symvec;
infoarray *store;
{
  long w, FORLIM;

  if (*symvec->name != '\0') {
    if (symvec->f != NULL)
      symvec->f = freopen(symvec->name, "r", symvec->f);
    else
      symvec->f = fopen(symvec->name, "r");
  } else
    rewind(symvec->f);
  if (symvec->f == NULL)
    _EscIO2(FileNotFound, symvec->name);
  RESETBUF(symvec->f, Char);
  while (P_peek(symvec->f) == '*') {
    fscanf(symvec->f, "%*[^\n]");
    getc(symvec->f);
  }
  fscanf(symvec->f, "%*[^\n]");
  getc(symvec->f);
  store->halfwindowsize = store->windowsize / 2.0;
  store->current = 0;
  store->Rtotal = 0.0;
  store->Vtotal = 0.0;
  FORLIM = store->windowsize;
  for (w = 0; w < FORLIM; w++) {
    readoneposition(symvec, &store->data[w], store->symbols);
    store->current++;
    store->Rtotal += store->data[store->current - 1].information;
    store->Vtotal += store->data[store->current - 1].variance;
  }
  store->last = 1;
}



Static Void doaposition(symvec, xyin, marks, store)
_TEXT *symvec, *xyin, *marks;
infoarray *store;
{
  store->current++;
  if (store->current > store->windowsize)
    store->current = 1;

  store->Rtotal -= store->data[store->current - 1].information;
  store->Vtotal -= store->data[store->current - 1].variance;

  readoneposition(symvec, &store->data[store->current - 1], store->symbols);

  store->Rtotal += store->data[store->current - 1].information;
  store->Vtotal += store->data[store->current - 1].variance;

  reportwindow(xyin, marks, store);

}



Static Void readwindowsize(f, windowsize)
_TEXT *f;
long *windowsize;
{
  fscanf(f->f, "%ld%*[^\n]", windowsize);
  getc(f->f);
  if (*windowsize > maxwin) {
    printf("windowsize requested was %ld; ", *windowsize);
    printf("windowsize must not exceed %ld\n", (long)maxwin);
    halt();
  }
  if (*windowsize >= 1)
    return;
  printf("windowsize requested was %ld; ", *windowsize);
  printf("windowsize must be positive\n");
  halt();
}



Static Void themain(symvec, ravp, xyin, marks, colorkey)
_TEXT *symvec, *ravp, *xyin, *marks, *colorkey;
{
  double parameterversion;
  infoarray store;
  long w;

  printf("rav %4.2f\n", version);
  if (*ravp->name != '\0') {
    if (ravp->f != NULL)
      ravp->f = freopen(ravp->name, "r", ravp->f);
    else
      ravp->f = fopen(ravp->name, "r");
  } else
    rewind(ravp->f);
  if (ravp->f == NULL)
    _EscIO2(FileNotFound, ravp->name);
  RESETBUF(ravp->f, Char);
  fscanf(ravp->f, "%lg%*[^\n]", &parameterversion);
  getc(ravp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }


  readwindowsize(ravp, &store.minwindowsize);
  readwindowsize(ravp, &store.maxwindowsize);
  fscanf(ravp->f, "%ld%*[^\n]", &store.stepwindowsize);
  getc(ravp->f);
  fscanf(ravp->f, "%lg%lg%*[^\n]", &store.colorX, &store.colorY);
  getc(ravp->f);
  fscanf(ravp->f, "%lg%lg%*[^\n]", &store.colorHeight, &store.colorWidth);
  getc(ravp->f);
  fscanf(ravp->f, "%lg%*[^\n]", &store.colorThick);
  getc(ravp->f);
  fscanf(ravp->f, "%ld%*[^\n]", &store.colorTransform);
  getc(ravp->f);
  fscanf(ravp->f, "%lg%*[^\n]", &store.colorAtransform);
  getc(ravp->f);
  fscanf(ravp->f, "%lg%*[^\n]", &store.colorBtransform);
  getc(ravp->f);

  makecolorkey(colorkey, store.colorX, store.colorY, store.colorHeight,
	       store.colorWidth, store.colorThick, store.colorTransform,
	       store.colorAtransform, store.colorBtransform);
  setheaders(symvec, xyin, marks, &store);

  w = store.minwindowsize;
  while (w <= store.maxwindowsize) {
    printf("working on window size %ld\n", w);
    store.windowsize = w;
    initialread(symvec, &store);
    reportwindow(xyin, marks, &store);

    while (!BUFEOF(symvec->f))
      doaposition(symvec, xyin, marks, &store);
    w += store.stepwindowsize;
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  colorkey.f = NULL;
  strcpy(colorkey.name, "colorkey");
  marks.f = NULL;
  strcpy(marks.name, "marks");
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  ravp.f = NULL;
  strcpy(ravp.name, "ravp");
  symvec.f = NULL;
  strcpy(symvec.name, "symvec");
  themain(&symvec, &ravp, &xyin, &marks, &colorkey);
_L1:
  if (symvec.f != NULL)
    fclose(symvec.f);
  if (ravp.f != NULL)
    fclose(ravp.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  if (marks.f != NULL)
    fclose(marks.f);
  if (colorkey.f != NULL)
    fclose(colorkey.f);
  exit(EXIT_SUCCESS);
}



/* End. */
