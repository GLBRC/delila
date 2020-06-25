/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "disum.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*
*/
#define updateversion   1.00



/*



































*/


typedef struct spot {

  long l;
  double ri;
  struct spot *next;
} spot;


Static _TEXT data, disump, xyin;


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



Static Void themain(data, disump, xyin)
_TEXT *data, *disump, *xyin;
{
  spot *allspots, *aspot, *bspot, *lastspot;
  double parameterversion;

  printf("disum %4.2f\n", version);
  if (*disump->name != '\0') {
    if (disump->f != NULL)
      disump->f = freopen(disump->name, "r", disump->f);
    else
      disump->f = fopen(disump->name, "r");
  } else
    rewind(disump->f);
  if (disump->f == NULL)
    _EscIO2(FileNotFound, disump->name);
  RESETBUF(disump->f, Char);
  fscanf(disump->f, "%lg%*[^\n]", &parameterversion);
  getc(disump->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
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

  allspots = (spot *)Malloc(sizeof(spot));
  aspot = allspots;
  while (!BUFEOF(data->f)) {
    if (P_peek(data->f) == '*') {
      fscanf(data->f, "%*[^\n]");
      getc(data->f);
      continue;
    }
    skipcolumn(data);
    skipcolumn(data);
    skipcolumn(data);
    fscanf(data->f, "%ld", &aspot->l);
    skipcolumn(data);
    fscanf(data->f, "%lg", &aspot->ri);
    fscanf(data->f, "%*[^\n]");
    getc(data->f);
    /*

*/
    lastspot = aspot;
    aspot->next = (spot *)Malloc(sizeof(spot));
    aspot = aspot->next;
  }

  Free(lastspot->next);
  lastspot->next = NULL;

  /*





*/

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
  fprintf(xyin->f, "* disum %4.2f\n", version);
  aspot = allspots;
  while (aspot != NULL) {
    printf("%ld %4.1f bits\n", aspot->l, aspot->ri);
    bspot = allspots;
    while (bspot != aspot) {
      fprintf(xyin->f, "%10ld %10.6f\n",
	      aspot->l - bspot->l, bspot->ri + aspot->ri);
      bspot = bspot->next;
    }

    aspot = aspot->next;
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
  disump.f = NULL;
  strcpy(disump.name, "disump");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &disump, &xyin);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (disump.f != NULL)
    fclose(disump.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}





























/* End. */
