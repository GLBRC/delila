/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "chaos.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.05
/*
*/



/*

















































































*/



#define infofield       12
#define infodecim       6


Static _TEXT data, xyin;


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



Static Void themain(data, xyin)
_TEXT *data, *xyin;
{
  long piecenumber, piecelength;

  long piececoordinate, matrixorientation;
  double Ri, Z, probability;

  long oldpiecenumber = LONG_MAX, oldpiecelength = LONG_MAX;

  long oldpiececoordinate = LONG_MAX, oldmatrixorientation = LONG_MAX;
  double oldRi = LONG_MAX, oldZ = LONG_MAX, oldprobability = LONG_MAX;

  long oldoldpiecenumber = 0;

  double hue;
  double saturation = 1.00, brightness = 1.00;

  long deltapiececoordinate, olddeltapiececoordinate = LONG_MAX;

  printf("chaos %4.2f\n", version);
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
  fprintf(xyin->f, "* chaos %4.2f\n", version);

  fprintf(xyin->f, "*\n");
  fprintf(xyin->f, "* DEFINITION OF THE DATA COLUMNS:\n");
  fprintf(xyin->f, "* 1 delta old piece coordinate\n");
  fprintf(xyin->f, "* 2 delta piece coordinate\n");
  fprintf(xyin->f, "* 3 hue\n");
  fprintf(xyin->f, "* 4 saturation\n");
  fprintf(xyin->f, "* 5 brightness\n");
  fprintf(xyin->f, "*\n");

  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "r", data->f);
    else
      data->f = fopen(data->name, "r");
  } else
    rewind(data->f);
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  RESETBUF(data->f, Char);





  while (!BUFEOF(data->f)) {
    /*



*/

    while (P_peek(data->f) == '*')
      copyaline(data, xyin);
    if (BUFEOF(data->f))
      break;

    fscanf(data->f, "%ld", &piecenumber);
    fscanf(data->f, "%ld", &piecelength);
    skipcolumn(data);
    fscanf(data->f, "%ld", &piececoordinate);
    fscanf(data->f, "%ld", &matrixorientation);
    fscanf(data->f, "%lg", &Ri);
    fscanf(data->f, "%lg", &Z);
    fscanf(data->f, "%lg", &probability);
    fscanf(data->f, "%*[^\n]");

    getc(data->f);
    deltapiececoordinate = piececoordinate - oldpiececoordinate;


    if (oldoldpiecenumber == piecenumber) {
      hue = probability;
      fprintf(xyin->f, " %*.*f", infofield, infodecim, oldRi);
      fprintf(xyin->f, " %*.*f", infofield, infodecim, Ri);
      fprintf(xyin->f, " %*.*f", infofield, infodecim, hue);
      fprintf(xyin->f, " %*.*f", infofield, infodecim, saturation);
      fprintf(xyin->f, " %*.*f\n", infofield, infodecim, brightness);
    }



    oldoldpiecenumber = oldpiecenumber;

    oldpiecenumber = piecenumber;
    oldpiecelength = piecelength;
    oldpiececoordinate = piececoordinate;
    oldmatrixorientation = matrixorientation;
    oldRi = Ri;
    oldZ = Z;
    oldprobability = probability;

    olddeltapiececoordinate = deltapiececoordinate;

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
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &xyin);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}

/*





























*/



/* End. */
