/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "nomultiblank.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/

/*

*/


#define version         1.00
/*
*/
#define updateversion   1.00


#define debugging       false



/*































*/


/*














*/


Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  Char c;
  long b, blanks;
  boolean blankline;
  long countblanklines = 0;

  while (!BUFEOF(fin->f)) {
    blanks = 0;
    blankline = true;
    while (!P_eoln(fin->f)) {
      c = getc(fin->f);
      if (c == '\n')
	c = ' ';
      if (c == ' ' && blankline)
	blanks++;
      else {
	blankline = false;
	countblanklines = 0;
	for (b = 1; b <= blanks; b++)
	  putc(' ', fout->f);
	blanks = 0;
      }
      if (!blankline)
	putc(c, fout->f);
    }
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
    countblanklines++;
    if (debugging)
      fprintf(fout->f, "[%ld]", countblanklines);
    if (countblanklines <= 2)
      putc('\n', fout->f);
  }

}

#undef debugging


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