/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "denav.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00
/*

*/
#define updateversion   1.00



/*




















































*/


Static _TEXT data, denavp, bfile;


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


#define debugging       true



Static Void themain(data, denavp, bfile)
_TEXT *data, *denavp, *bfile;
{
  double parameterversion;
  long column, coordinatecolumn, ricolumn, coordinate;
  double ri;
  double risumneg = 0.0, risumzer = 0.0, risumpos = 0.0;
  long negatives = 0, zeros = 0, positives = 0;

  printf("denav %4.2f\n", version);
  if (*denavp->name != '\0') {
    if (denavp->f != NULL)
      denavp->f = freopen(denavp->name, "r", denavp->f);
    else
      denavp->f = fopen(denavp->name, "r");
  } else
    rewind(denavp->f);
  if (denavp->f == NULL)
    _EscIO2(FileNotFound, denavp->name);
  RESETBUF(denavp->f, Char);
  fscanf(denavp->f, "%lg%*[^\n]", &parameterversion);
  getc(denavp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(denavp->f, "%ld%*[^\n]", &coordinatecolumn);
  getc(denavp->f);
  fscanf(denavp->f, "%ld%*[^\n]", &ricolumn);
  getc(denavp->f);

  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "r", data->f);
    else
      data->f = fopen(data->name, "r");
  } else {

    rewind(data->f);
  }
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  RESETBUF(data->f, Char);
  while (!BUFEOF(data->f)) {
    if ((P_peek(data->f) == '*') | P_eoln(data->f)) {
      fscanf(data->f, "%*[^\n]");
      getc(data->f);
      continue;
    }
    column = 1;
    while (column < coordinatecolumn) {
      column++;
      skipcolumn(data);
    }

    fscanf(data->f, "%ld", &coordinate);
    column++;

    while (column < ricolumn) {
      column++;
      skipcolumn(data);
    }
    fscanf(data->f, "%lg", &ri);

    fscanf(data->f, "%*[^\n]");

    /*

*/



    getc(data->f);
    if (coordinate < 0) {
      risumneg = ri + risumneg;
      negatives++;
      /*

*/
      continue;
    }
    if (coordinate > 0) {
      risumpos = ri + risumpos;
      positives++;
      /*

*/
    } else {
      risumzer = ri + risumzer;
      zeros++;
    }
  }




  if (negatives > 0)
    printf("Average of %7ld negative coordinate Ri values: %10.6f bits per site\n",
	   negatives, risumneg / negatives);
  /*

*/
  else
    printf("                   No negative coordinates\n");

  if (zeros > 0)
    printf("Average of %7ld zero     coordinate Ri values: %10.6f bits per site\n",
	   zeros, risumzer / zeros);
  /*

*/
  else
    printf("                   No zero coordinates\n");

  if (positives > 0)
    printf("Average of %7ld positive coordinate Ri values: %10.6f bits per site\n",
	   positives, risumpos / positives);
  /*

*/
  else
    printf("                   No positive coordinates\n");

  if (negatives + zeros + positives > 0)
    printf(
      "Average of %7ld total    coordinate Ri values: %10.6f bits per site\n",
      positives + zeros + negatives,
      (risumneg + risumzer + risumpos) / (negatives + zeros + positives));
  else {
    printf("                   No coordinates\n");

  }
}

#undef debugging


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  bfile.f = NULL;
  strcpy(bfile.name, "bfile");
  denavp.f = NULL;
  strcpy(denavp.name, "denavp");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &denavp, &bfile);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (denavp.f != NULL)
    fclose(denavp.f);
  if (bfile.f != NULL)
    fclose(bfile.f);
  exit(EXIT_SUCCESS);
}



/* End. */
