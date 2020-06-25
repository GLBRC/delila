/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "uncsv.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*


*/
#define updateversion   1.00



/*





















































*/


Static _TEXT csv, uncsvp, spacetable;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(csv, uncsvp, spacetable)
_TEXT *csv, *uncsvp, *spacetable;
{
  boolean inquote = false;
  Char c;
  Char p = ' ';

  printf("uncsv %4.2f\n", version);
  /*






*/

  if (*csv->name != '\0') {
    if (csv->f != NULL)
      csv->f = freopen(csv->name, "r", csv->f);
    else
      csv->f = fopen(csv->name, "r");
  } else
    rewind(csv->f);
  if (csv->f == NULL)
    _EscIO2(FileNotFound, csv->name);
  RESETBUF(csv->f, Char);
  if (*spacetable->name != '\0') {
    if (spacetable->f != NULL)
      spacetable->f = freopen(spacetable->name, "w", spacetable->f);
    else
      spacetable->f = fopen(spacetable->name, "w");
  } else {
    if (spacetable->f != NULL)
      rewind(spacetable->f);
    else
      spacetable->f = tmpfile();
  }
  if (spacetable->f == NULL)
    _EscIO2(FileNotFound, spacetable->name);
  SETUPBUF(spacetable->f, Char);

  while (!BUFEOF(csv->f)) {
    if (P_eoln(csv->f)) {
      fscanf(csv->f, "%*[^\n]");
      getc(csv->f);
      putc('\n', spacetable->f);
      p = ' ';
      continue;
    }
    c = getc(csv->f);
    if (c == '\n')
      c = ' ';
    if (inquote) {
      if (c == '"') {
	inquote = !inquote;
	putc(c, spacetable->f);
      } else {
	if (c == ' ')
	  putc('_', spacetable->f);
	else
	  putc(c, spacetable->f);
      }
    } else {
      if (c == ',') {
	/*
*/
	if (p == ',')
	  fprintf(spacetable->f, "- ");
	else
	  putc(' ', spacetable->f);
      } else
	putc(c, spacetable->f);
      if (c == '"')
	inquote = !inquote;
    }
    p = c;
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  spacetable.f = NULL;
  strcpy(spacetable.name, "spacetable");
  uncsvp.f = NULL;
  strcpy(uncsvp.name, "uncsvp");
  csv.f = NULL;
  strcpy(csv.name, "csv");
  themain(&csv, &uncsvp, &spacetable);
_L1:
  if (csv.f != NULL)
    fclose(csv.f);
  if (uncsvp.f != NULL)
    fclose(uncsvp.f);
  if (spacetable.f != NULL)
    fclose(spacetable.f);
  exit(EXIT_SUCCESS);
}



/* End. */
