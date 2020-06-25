/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "tabify.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/


#define version         1.02
/*



*/
#define updateversion   1.00


#define tab             '\t'



/*






































*/


Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  long blankchars, tabchars;
  long blanklines = 0, totalblankchars = 0;
  long index;
  boolean wasblank, firstblank;

  /*

*/


  /*





*/

  while (!BUFEOF(fin->f)) {
    blankchars = 0;
    tabchars = 0;
    wasblank = true;
    firstblank = true;


    while (!P_eoln(fin->f)) {
      if (P_peek(fin->f) == ' ')
	blankchars++;
      else if (P_peek(fin->f) == tab)
	tabchars++;
      else {
	if (blanklines != 0) {
	  for (index = 1; index <= blanklines; index++)
	    putc('\n', fout->f);
	  blanklines = 0;
	}

	if (blankchars + tabchars != 0) {
	  /*

*/
	  /*




*/
	  if (!firstblank)
	    putc(tab, fout->f);
	  blankchars = 0;
	  tabchars = 0;
	}
	firstblank = false;

	/*




*/

	putc(P_peek(fin->f), fout->f);
	wasblank = false;
      }
      getc(fin->f);
    }


    totalblankchars += blankchars;

    if (wasblank)
      blanklines++;
    else
      putc('\n', fout->f);
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
  }

  /*







*/
}

#undef tab


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
  exit(EXIT_SUCCESS);
}



/* End. */
