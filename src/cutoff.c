/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "cutoff.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.03
/*




*/
#define updateversion   1.00



/*






































*/



#define infofield       10
#define infodecim       6

#define nfield          8



#define maxstring       200





typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;



Static _TEXT data, cutoffp, datacut;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void copyline(fin, fout)
_TEXT *fin, *fout;
{
  /*

*/
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
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



Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*


*/
  clearstring(ribbon);
  ribbon->next = NULL;
}




Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}




Static Void grabtoken(thefile, thestring)
_TEXT *thefile;
stringDelila *thestring;
{
  Char c;
  boolean done = false;

  skipblanks(thefile);
  thestring->length = 0;
  while (!done) {
    if (P_eoln(thefile->f)) {
      done = true;
      break;
    }
    c = getc(thefile->f);
    if (c == '\n')
      c = ' ';
    if (c == ' ')
      done = true;
    else {
      thestring->length++;
      thestring->letters[thestring->length - 1] = c;
    }
  }
}



Static Void themain(data, cutoffp, datacut)
_TEXT *data, *cutoffp, *datacut;
{
  double bound, parameterversion;

  /*









*/


  long number;
  stringDelila name, sequence;
  long length, coordinate;
  double Rindividual, value, stdev;

  printf("cutoff %4.2f\n", version);
  if (*cutoffp->name != '\0') {
    if (cutoffp->f != NULL)
      cutoffp->f = freopen(cutoffp->name, "r", cutoffp->f);
    else
      cutoffp->f = fopen(cutoffp->name, "r");
  } else
    rewind(cutoffp->f);
  if (cutoffp->f == NULL)
    _EscIO2(FileNotFound, cutoffp->name);
  RESETBUF(cutoffp->f, Char);
  fscanf(cutoffp->f, "%lg%*[^\n]", &parameterversion);
  getc(cutoffp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(cutoffp->f, "%lg%*[^\n]", &bound);
  getc(cutoffp->f);
  printf("the lower bound is %4.2f\n", bound);
  if (*datacut->name != '\0') {
    if (datacut->f != NULL)
      datacut->f = freopen(datacut->name, "w", datacut->f);
    else
      datacut->f = fopen(datacut->name, "w");
  } else {
    if (datacut->f != NULL)
      rewind(datacut->f);
    else
      datacut->f = tmpfile();
  }
  if (datacut->f == NULL)
    _EscIO2(FileNotFound, datacut->name);
  SETUPBUF(datacut->f, Char);
  fprintf(datacut->f, "* bits\n");
  fprintf(datacut->f, "* cutoff %4.2f\n", version);
  fprintf(datacut->f, "* the lower bound is %4.2f\n", bound);

  initializestring(&name);
  initializestring(&sequence);

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
    if (P_peek(data->f) == '*') {
      copyline(data, datacut);
      putc('\n', datacut->f);
      continue;
    }
    if (P_eoln(data->f)) {
      fscanf(data->f, "%*[^\n]");
      getc(data->f);
      putc('\n', datacut->f);
      continue;
    }
    fscanf(data->f, "%ld", &number);
    grabtoken(data, &name);
    grabtoken(data, &sequence);
    fscanf(data->f, "%ld", &length);
    fscanf(data->f, "%ld", &coordinate);
    fscanf(data->f, "%lg", &Rindividual);
    fscanf(data->f, "%lg", &value);
    fscanf(data->f, "%lg", &stdev);
    fscanf(data->f, "%*[^\n]");

    getc(data->f);
    if (Rindividual < bound)
      continue;

    /*

*/

    fprintf(datacut->f, "%*ld", nfield, number);
    putc(' ', datacut->f);
    writestring(datacut, &name);
    putc(' ', datacut->f);
    writestring(datacut, &sequence);
    fprintf(datacut->f, "%*ld", nfield, length);
    fprintf(datacut->f, "%*ld", nfield, coordinate);
    fprintf(datacut->f, "%*.*f", infofield, infodecim, Rindividual);
    fprintf(datacut->f, "%*.*f", infofield, infodecim, value);
    fprintf(datacut->f, "%*.*f\n", infofield, infodecim, stdev);



  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  datacut.f = NULL;
  strcpy(datacut.name, "datacut");
  cutoffp.f = NULL;
  strcpy(cutoffp.name, "cutoffp");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &cutoffp, &datacut);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (cutoffp.f != NULL)
    fclose(cutoffp.f);
  if (datacut.f != NULL)
    fclose(datacut.f);
  exit(EXIT_SUCCESS);
}



/* End. */
