/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "piechart.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.07
/*








*/
#define updateversion   1.03



/*







































































*/


Static _TEXT pienumbers, piechartp, pieps;


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



Static Void copyalinenocr(fin, fout)
_TEXT *fin, *fout;
{
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
}


Static long filelength(title)
_TEXT *title;
{
  long n = 0;

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
  while (!P_eoln(title->f)) {
    getc(title->f);
    n++;
  }
  return n;
}


Static Void themain(piechartp, pienumbers, pieps)
_TEXT *piechartp, *pienumbers, *pieps;
{
  double parameterversion;

  double diametercm, angle;
  long direction, fontsize;
  _TEXT title;

  double total = 0.0;
  /*
*/
  double anumber, anumbermax = -LONG_MAX;
  long sections = 0, section = 0, wid = 10, dec = 8;

  long diameterpts, radiuspts;
  long edgepts = 3;
  long squareedge;

  Char colortype;
  double redhue, greensaturation, bluebrightness;


  double anglecurrent, anglenext;

  title.f = NULL;
  *title.name = '\0';
  printf("piechart %4.2f\n", version);
  if (*piechartp->name != '\0') {
    if (piechartp->f != NULL)
      piechartp->f = freopen(piechartp->name, "r", piechartp->f);
    else
      piechartp->f = fopen(piechartp->name, "r");
  } else
    rewind(piechartp->f);
  if (piechartp->f == NULL)
    _EscIO2(FileNotFound, piechartp->name);
  RESETBUF(piechartp->f, Char);
  fscanf(piechartp->f, "%lg%*[^\n]", &parameterversion);
  getc(piechartp->f);
  if ((long)floor(100 * parameterversion + 0.5) <
      (long)floor(100 * updateversion + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }
  fscanf(piechartp->f, "%lg%*[^\n]", &diametercm);
  getc(piechartp->f);
  fscanf(piechartp->f, "%lg%*[^\n]", &angle);
  getc(piechartp->f);
  fscanf(piechartp->f, "%ld%*[^\n]", &direction);
  getc(piechartp->f);
  if (*title.name != '\0') {
    if (title.f != NULL)
      title.f = freopen(title.name, "w", title.f);
    else
      title.f = fopen(title.name, "w");
  } else {
    if (title.f != NULL)
      rewind(title.f);
    else
      title.f = tmpfile();
  }
  if (title.f == NULL)
    _EscIO2(FileNotFound, title.name);
  SETUPBUF(title.f, Char);
  copyaline(piechartp, &title);
  fscanf(piechartp->f, "%ld%*[^\n]", &fontsize);
  getc(piechartp->f);


  if (*pienumbers->name != '\0') {
    if (pienumbers->f != NULL)
      pienumbers->f = freopen(pienumbers->name, "r", pienumbers->f);
    else
      pienumbers->f = fopen(pienumbers->name, "r");
  } else
    rewind(pienumbers->f);
  if (pienumbers->f == NULL)
    _EscIO2(FileNotFound, pienumbers->name);
  RESETBUF(pienumbers->f, Char);
  while (!BUFEOF(pienumbers->f)) {
    fscanf(pienumbers->f, "%lg%*[^\n]", &anumber);
    getc(pienumbers->f);
    total += anumber;
    sections++;
    if (anumbermax < anumber)
      anumbermax = anumber;
  }


  diameterpts = (long)floor(diametercm / 2.54 * 72 + 0.5);
  radiuspts = (long)floor(diameterpts / 2.0 + 0.5);
  squareedge = diameterpts + edgepts * 2;

  if (*pieps->name != '\0') {
    if (pieps->f != NULL)
      pieps->f = freopen(pieps->name, "w", pieps->f);
    else
      pieps->f = fopen(pieps->name, "w");
  } else {
    if (pieps->f != NULL)
      rewind(pieps->f);
    else
      pieps->f = tmpfile();
  }
  if (pieps->f == NULL)
    _EscIO2(FileNotFound, pieps->name);
  SETUPBUF(pieps->f, Char);
  fprintf(pieps->f, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(pieps->f, "%%%%Title: piechart %4.2f\n", version);
  fprintf(pieps->f,
    "%%%%Creator: Tom Schneider, toms@alum.mit.edu https://alum.mit.edu/www/toms\n");
  fprintf(pieps->f, "%%%%BoundingBox:    0 0 %ld %ld\n",
	  squareedge, squareedge);
  fprintf(pieps->f, "%%%%Pages: 1\n");
  fprintf(pieps->f, "%%%%DocumentFonts:\n");
  fprintf(pieps->f, "%%%%EndComments\n");
  fprintf(pieps->f,
	  "/cmfactor 72 2.54 div def %% defines points -> cm conversion\n");
  fprintf(pieps->f, "/cm {cmfactor mul} bind def %% defines centimeters\n");
  fprintf(pieps->f, "<< /PageSize [%ld %ld] >> setpagedevice\n",
	  squareedge, squareedge);

  fprintf(pieps->f, "%%\n");
  fprintf(pieps->f, "%% Parameters:\n");
  fprintf(pieps->f, "%% %*.*f diameter (cm)\n",
	  (int)wid, (int)dec, diametercm);
  fprintf(pieps->f, "%% %*.*f start angle, degrees\n",
	  (int)wid, (int)dec, angle);
  fprintf(pieps->f, "%% %ld direction\n", direction);
  fprintf(pieps->f, "%% \n");
  fprintf(pieps->f, "%% %ld sections\n", sections);
  fprintf(pieps->f, "%% %*.*f total\n\n", (int)wid, (int)dec, total);

  fprintf(pieps->f, "%ld dup translate\n", radiuspts + edgepts);

  /*

*/


  /*


*/

  fprintf(pieps->f, "/startarc {newpath 0 0 moveto 0 0 %ld} def\n", radiuspts);
  fprintf(pieps->f, "/positiveendarc {arc  closepath fill} def\n");
  fprintf(pieps->f, "/negativeendarc {arcn closepath fill} def\n");

  /*

*/


  anglecurrent = angle;
  if (*pienumbers->name != '\0') {
    if (pienumbers->f != NULL)
      pienumbers->f = freopen(pienumbers->name, "r", pienumbers->f);
    else
      pienumbers->f = fopen(pienumbers->name, "r");
  } else
    rewind(pienumbers->f);
  if (pienumbers->f == NULL)
    _EscIO2(FileNotFound, pienumbers->name);
  RESETBUF(pienumbers->f, Char);
  while (!BUFEOF(pienumbers->f)) {
    fscanf(pienumbers->f, "%lg", &anumber);
    skipblanks(pienumbers);
    colortype = getc(pienumbers->f);
    if (colortype == '\n')
      colortype = ' ';
    if (colortype == 'H') {
      /*





*/
      redhue = anumber / anumbermax * 0.84 + 0.16;
      /*



*/
      greensaturation = 1.0;
      bluebrightness = 1.0;
    } else
      fscanf(pienumbers->f, "%lg%lg%lg", &redhue, &greensaturation,
	     &bluebrightness);
    fscanf(pienumbers->f, "%*[^\n]");

    getc(pienumbers->f);
    fprintf(pieps->f, "%% %*.*f a number\n", (int)wid, (int)dec, anumber);
    fprintf(pieps->f, "%% %*.*f the number as a fraction\n",
	    (int)wid, (int)dec, anumber / total);
    fprintf(pieps->f, "%% %*c %% color type\n", (int)wid, colortype);
    fprintf(pieps->f, " %*.*f %% redhue\n", (int)wid, (int)dec, redhue);
    fprintf(pieps->f, " %*.*f %% greensaturation\n",
	    (int)wid, (int)dec, greensaturation);
    fprintf(pieps->f, " %*.*f %% bluebrightness\n",
	    (int)wid, (int)dec, bluebrightness);

    if (colortype == 'h' || colortype == 'H')
      fprintf(pieps->f, "sethsbcolor \n");
    else
      fprintf(pieps->f, "setrgbcolor \n");

    if (direction > 0) {
      anglenext = anglecurrent + 360 * anumber / total;
      if (anglenext < 360)
	anglenext -= 360;
      fprintf(pieps->f, "startarc\n");
      fprintf(pieps->f, "%*.*f\n", (int)wid, (int)dec, anglecurrent);
      fprintf(pieps->f, "%*.*f\n", (int)wid, (int)dec, anglenext);
      fprintf(pieps->f, "positiveendarc\n");
    } else {
      anglenext = anglecurrent - 360 * anumber / total;
      if (anglenext < 0)
	anglenext += 360;
      fprintf(pieps->f, "startarc\n");
      fprintf(pieps->f, "%*.*f\n", (int)wid, (int)dec, anglecurrent);
      fprintf(pieps->f, "%*.*f\n", (int)wid, (int)dec, anglenext);
      fprintf(pieps->f, "negativeendarc\n");
    }
    anglecurrent = anglenext;
    section++;

  }


  fprintf(pieps->f, "\n0 0 moveto\n");

  /*




*/


  fprintf(pieps->f, "%*.*f\n",
	  (int)wid, (int)dec, -3.0 / 5 * fontsize * filelength(&title) / 2);
  fprintf(pieps->f, "%*.*f\n", (int)wid, (int)dec, fontsize / -4.0);
  fprintf(pieps->f, " moveto\n");
  fprintf(pieps->f, "/Courier-Bold findfont\n");
  fprintf(pieps->f, "%ld scalefont\n", fontsize);
  fprintf(pieps->f, "setfont\n");
  fprintf(pieps->f, "0 setgray");
  putc('(', pieps->f);
  if (*title.name != '\0') {
    if (title.f != NULL)
      title.f = freopen(title.name, "r", title.f);
    else
      title.f = fopen(title.name, "r");
  } else
    rewind(title.f);
  if (title.f == NULL)
    _EscIO2(FileNotFound, title.name);
  RESETBUF(title.f, Char);
  copyalinenocr(&title, pieps);
  fprintf(pieps->f, ") show\n");

  if (title.f != NULL)
    fclose(title.f);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  pieps.f = NULL;
  strcpy(pieps.name, "pieps");
  piechartp.f = NULL;
  strcpy(piechartp.name, "piechartp");
  pienumbers.f = NULL;
  strcpy(pienumbers.name, "pienumbers");
  themain(&piechartp, &pienumbers, &pieps);
_L1:
  if (pienumbers.f != NULL)
    fclose(pienumbers.f);
  if (piechartp.f != NULL)
    fclose(piechartp.f);
  if (pieps.f != NULL)
    fclose(pieps.f);
  exit(EXIT_SUCCESS);
}



/* End. */
