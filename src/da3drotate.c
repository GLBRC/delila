/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "da3drotate.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.03
/*
*/



/*































*/



#define pi              3.14159265354

#define picfield        20
#define picwidth        5


Static _TEXT da3dp, da3drotatep;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


/* Local variables for themain: */
struct LOC_themain {


  Char dd;
  boolean dodiagonal;
  /*

*/
  double magnify, xmagnify, ymagnify, zmagnify, xlocation, ylocation,
	 zlocation;
  long datacolumn;
  double horizontal, vertical;
} ;

/*

*/


Local Void readda3d(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  fscanf(f->f, "%lg%*[^\n]", &LINK->horizontal);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->vertical);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->xlocation);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->ylocation);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->zlocation);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->magnify);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->xmagnify);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->ymagnify);
  getc(f->f);
  fscanf(f->f, "%lg%*[^\n]", &LINK->zmagnify);
  getc(f->f);
  fscanf(f->f, "%ld%*[^\n]", &LINK->datacolumn);
  getc(f->f);
  if (LINK->datacolumn <= 4 || LINK->datacolumn > 15) {
    printf("data column must be >4 and < 16\n");
    halt();
  }
  fscanf(f->f, "%c%*[^\n]", &LINK->dd);
  getc(f->f);
  if (LINK->dd == '\n')
    LINK->dd = ' ';
  LINK->dodiagonal = (LINK->dd == 't');
}

Local Void writeda3d(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  fprintf(f->f, "%*.*f horizontal adjust (cm)\n",
	  picfield, picwidth, LINK->horizontal);
  fprintf(f->f, "%*.*f vertical adjust (cm)\n",
	  picfield, picwidth, LINK->vertical);
  fprintf(f->f, "%*.*f xlocation (bases)\n",
	  picfield, picwidth, LINK->xlocation);
  fprintf(f->f, "%*.*f ylocation (bases)\n",
	  picfield, picwidth, LINK->ylocation);
  fprintf(f->f, "%*.*f zlocation (bases)\n",
	  picfield, picwidth, LINK->zlocation);
  fprintf(f->f, "%*.*f magnify\n", picfield, picwidth, LINK->magnify);
  fprintf(f->f, "%*.*f xmagnify\n", picfield, picwidth, LINK->xmagnify);
  fprintf(f->f, "%*.*f ymagnify\n", picfield, picwidth, LINK->ymagnify);
  fprintf(f->f, "%*.*f zmagnify\n", picfield, picwidth, LINK->zmagnify);
  fprintf(f->f, "%*ld data column\n", picfield, LINK->datacolumn);
  if (LINK->datacolumn <= 4 || LINK->datacolumn > 15) {
    printf("data column must be >4 and < 16\n");
    halt();
  }
  fprintf(f->f, "%c dodiagonal\n", LINK->dd);
}



Static Void themain(da3dp, da3drotatep)
_TEXT *da3dp, *da3drotatep;
{
  struct LOC_themain V;
  double currentangle, deltaangle, distance;


  printf("da3drotate %4.2f\n", version);

  if (*da3drotatep->name != '\0') {
    if (da3drotatep->f != NULL)
      da3drotatep->f = freopen(da3drotatep->name, "r", da3drotatep->f);
    else
      da3drotatep->f = fopen(da3drotatep->name, "r");
  } else
    rewind(da3drotatep->f);
  if (da3drotatep->f == NULL)
    _EscIO2(FileNotFound, da3drotatep->name);
  RESETBUF(da3drotatep->f, Char);
  fscanf(da3drotatep->f, "%lg%*[^\n]", &deltaangle);
  getc(da3drotatep->f);

  deltaangle *= pi / 180;


  if (*da3dp->name != '\0') {
    if (da3dp->f != NULL)
      da3dp->f = freopen(da3dp->name, "r", da3dp->f);
    else
      da3dp->f = fopen(da3dp->name, "r");
  } else
    rewind(da3dp->f);
  if (da3dp->f == NULL)
    _EscIO2(FileNotFound, da3dp->name);
  RESETBUF(da3dp->f, Char);
  readda3d(da3dp, &V);

  distance = sqrt(V.xlocation * V.xlocation + V.ylocation * V.ylocation);
  if (V.xlocation != 0)
    currentangle = atan(V.ylocation / V.xlocation);
  else
    currentangle = pi / 2.0;

  currentangle = deltaangle + currentangle;

  V.xlocation = distance * cos(currentangle);
  V.ylocation = distance * sin(currentangle);

  if (*da3dp->name != '\0') {
    if (da3dp->f != NULL)
      da3dp->f = freopen(da3dp->name, "w", da3dp->f);
    else
      da3dp->f = fopen(da3dp->name, "w");
  } else {
    if (da3dp->f != NULL)
      rewind(da3dp->f);
    else
      da3dp->f = tmpfile();
  }
  if (da3dp->f == NULL)
    _EscIO2(FileNotFound, da3dp->name);
  SETUPBUF(da3dp->f, Char);
  writeda3d(da3dp, &V);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  da3drotatep.f = NULL;
  strcpy(da3drotatep.name, "da3drotatep");
  da3dp.f = NULL;
  strcpy(da3dp.name, "da3dp");
  themain(&da3dp, &da3drotatep);
_L1:
  if (da3dp.f != NULL)
    fclose(da3dp.f);
  if (da3drotatep.f != NULL)
    fclose(da3drotatep.f);
  exit(EXIT_SUCCESS);
}



/* End. */
