/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rembla.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/


#define version         2.10


/*



*/



/*






























*/


Static _TEXT fin, fout;

Static long blankchars, blanklines, totalblankchars, index_;

Static boolean wasblank;


Static Void plural(thefile, number)
_TEXT *thefile;
long number;
{
  if (number != 1)
    putc('s', thefile->f);
}


main(argc, argv)
int argc;
Char *argv[];
{
  long FORLIM;
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  fout.f = NULL;
  strcpy(fout.name, "fout");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  printf(" rembla %4.2f\n", version);
  if (*fin.name != '\0') {
    if (fin.f != NULL)
      fin.f = freopen(fin.name, "r", fin.f);
    else
      fin.f = fopen(fin.name, "r");
  } else
    rewind(fin.f);
  if (fin.f == NULL)
    _EscIO2(FileNotFound, fin.name);
  RESETBUF(fin.f, Char);
  if (*fout.name != '\0') {
    if (fout.f != NULL)
      fout.f = freopen(fout.name, "w", fout.f);
    else
      fout.f = fopen(fout.name, "w");
  } else {
    if (fout.f != NULL)
      rewind(fout.f);
    else
      fout.f = tmpfile();
  }
  if (fout.f == NULL)
    _EscIO2(FileNotFound, fout.name);
  SETUPBUF(fout.f, Char);

  blanklines = 0;
  totalblankchars = 0;

  /*





*/

  while (!BUFEOF(fin.f)) {
    blankchars = 0;
    wasblank = true;


    while (!P_eoln(fin.f)) {
      if (P_peek(fin.f) == ' ')
	blankchars++;
      else {
	if (blanklines != 0) {
	  FORLIM = blanklines;
	  for (index_ = 1; index_ <= FORLIM; index_++)
	    putc('\n', fout.f);
	  blanklines = 0;
	}

	if (blankchars != 0) {
	  FORLIM = blankchars;
	  for (index_ = 1; index_ <= FORLIM; index_++)
	    putc(' ', fout.f);
	  blankchars = 0;
	}

	putc(P_peek(fin.f), fout.f);
	wasblank = false;
      }
      getc(fin.f);
    }


    totalblankchars += blankchars;

    if (wasblank)
      blanklines++;
    else
      putc('\n', fout.f);
    fscanf(fin.f, "%*[^\n]");
    getc(fin.f);
  }

  printf(" %ld blank", totalblankchars);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  plural(&TEMP, totalblankchars);
  printf(" removed\n");

  printf(" %ld blank line", blanklines);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  plural(&TEMP, blanklines);
  printf(" at the end removed\n");
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
