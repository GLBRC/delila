/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "difri.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.08
/*
*/



/*




































*/



#define defnegativeinfinity  (-1000)
/*
*/
#define maxribl         2000

#define infofield       12
#define infodecim       6

#define nfield          6



Static _TEXT data, difrip, xyin;


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



Static Void themain(data, difrip, xyin)
_TEXT *data, *difrip, *xyin;
{
  long distance;
  Char selector;
  /*
*/

  long coordinate, orientation;
  double p, Ri, Z;

  long oldcoordinate = 0, oldorientation = 0;
  double oldp = 0.0, oldRi = 0.0, oldZ = 0.0;

  printf("difri %4.2f\n", version);


  if (*difrip->name != '\0') {
    if (difrip->f != NULL)
      difrip->f = freopen(difrip->name, "r", difrip->f);
    else
      difrip->f = fopen(difrip->name, "r");
  } else
    rewind(difrip->f);
  if (difrip->f == NULL)
    _EscIO2(FileNotFound, difrip->name);
  RESETBUF(difrip->f, Char);
  fscanf(difrip->f, "%ld%*[^\n]", &distance);
  getc(difrip->f);
  if (distance < 1) {
    printf("distance between sites must be positive\n");
    halt();
  }
  fscanf(difrip->f, "%c%*[^\n]", &selector);
  getc(difrip->f);
  if (selector == '\n')
    selector = ' ';
  if (selector != 'z' && selector != 'p' && selector != 'r') {
    printf("selector must be one of: rpz\n");
    halt();
  }

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
  fprintf(xyin->f, "* difri %4.2f\n", version);

  fprintf(xyin->f, "************************************\n");
  fprintf(xyin->f, "* DEFINITION OF THE DATA COLUMNS:  *\n");

  fprintf(xyin->f, "* 1: ");
  switch (selector) {

  case 'r':
    fprintf(xyin->f, "Ri(n)         ");
    break;

  case 'p':
    fprintf(xyin->f, "probability(n)");
    break;

  case 'z':
    fprintf(xyin->f, "Z(n)          ");
    break;
  }
  fprintf(xyin->f, "                *\n");

  fprintf(xyin->f, "* 2: ");
  switch (selector) {

  case 'r':
    fprintf(xyin->f, "Ri(n+%2ld)         ", distance);
    break;

  case 'p':
    fprintf(xyin->f, "probability(n+%2ld)", distance);
    break;

  case 'z':
    fprintf(xyin->f, "Z(n+%ld)          ", distance);
    break;
  }
  fprintf(xyin->f, "             *\n");

  fprintf(xyin->f, "* 3: n                             *\n");
  fprintf(xyin->f, "* 4: n+%ld                          *\n", distance);
  fprintf(xyin->f, "************************************\n");

  while (!BUFEOF(data->f)) {
    if (P_peek(data->f) == '*') {
      copyaline(data, xyin);
      continue;
    }
    skipcolumn(data);
    skipcolumn(data);
    skipcolumn(data);
    fscanf(data->f, "%ld", &coordinate);
    fscanf(data->f, "%ld", &orientation);
    fscanf(data->f, "%lg", &Ri);
    fscanf(data->f, "%lg", &Z);
    fscanf(data->f, "%lg", &p);
    fscanf(data->f, "%*[^\n]");

    getc(data->f);
    if (coordinate - oldcoordinate == distance) {
      switch (selector) {

      case 'r':
	fprintf(xyin->f, "%*.*f", infofield, infodecim, oldRi);
	fprintf(xyin->f, " %*.*f", infofield, infodecim, Ri);
	break;

      case 'p':
	fprintf(xyin->f, "%*.*f", infofield, infodecim, oldp);
	fprintf(xyin->f, " %*.*f", infofield, infodecim, p);
	break;

      case 'z':
	fprintf(xyin->f, "%*.*f", infofield, infodecim, oldZ);
	fprintf(xyin->f, " %*.*f", infofield, infodecim, Z);
	break;
      }

      fprintf(xyin->f, " %*ld", nfield, oldcoordinate);
      fprintf(xyin->f, " %*ld\n", nfield, coordinate);
    }


    oldcoordinate = coordinate;
    oldorientation = orientation;
    oldp = p;
    oldRi = Ri;
    oldZ = Z;


  }
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
  difrip.f = NULL;
  strcpy(difrip.name, "difrip");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &difrip, &xyin);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (difrip.f != NULL)
    fclose(difrip.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}




































/* End. */
