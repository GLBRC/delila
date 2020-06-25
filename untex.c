/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "untex.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.41


Static jmp_buf _JL1;


/*








*/



/*






























*/



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *fin, *fout;
  long blanks;
  Char ch;
  long lines;
} ;

Local Void out(c, LINK)
Char c;
struct LOC_themain *LINK;
{
  /*
*/
  if (c == ' ') {
    LINK->blanks++;
    return;
  }
  if (c == '\\') {
    LINK->lines++;
    return;
  }

  if (LINK->lines > 0) {
    if (LINK->lines > 1)
      putc('\n', LINK->fout->f);
    putc('\n', LINK->fout->f);
    LINK->lines = 0;
    LINK->blanks = 0;
  }

  if (LINK->blanks > 0) {
    if (LINK->blanks == 2)
      fprintf(LINK->fout->f, "  ");
    else
      putc(' ', LINK->fout->f);
  }
  putc(c, LINK->fout->f);
  LINK->blanks = 0;
}

Local Void removecommand(LINK)
struct LOC_themain *LINK;
{
  /*





*/
  LINK->ch = getc(LINK->fin->f);
  if (LINK->ch == '\n')
    LINK->ch = ' ';
  if (LINK->ch == '%' || LINK->ch == '&') {
    out(LINK->ch, LINK);
    return;
  }
  out(' ', LINK);
  while (!P_eoln(LINK->fin->f) && (LINK->ch == '/' || LINK->ch == 'Z' ||
	LINK->ch == 'Y' || LINK->ch == 'X' || LINK->ch == 'W' || LINK->ch ==
	  'V' || LINK->ch == 'U' || LINK->ch == 'T' || LINK->ch == 'S' ||
	LINK->ch == 'R' || LINK->ch == 'Q' || LINK->ch == 'P' || LINK->ch ==
	  'O' || LINK->ch == 'N' || LINK->ch == 'M' || LINK->ch == 'L' ||
	LINK->ch == 'K' || LINK->ch == 'J' || LINK->ch == 'I' || LINK->ch ==
	  'H' || LINK->ch == 'G' || LINK->ch == 'F' || LINK->ch == 'E' ||
	LINK->ch == 'D' || LINK->ch == 'C' || LINK->ch == 'B' || LINK->ch ==
	  'A' || LINK->ch == 'z' || LINK->ch == 'y' || LINK->ch == 'x' ||
	LINK->ch == 'w' || LINK->ch == 'v' || LINK->ch == 'u' ||
	LINK->ch == 't' || LINK->ch == 's' || LINK->ch == 'r' ||
	LINK->ch == 'q' || LINK->ch == 'p' || LINK->ch == 'o' ||
	LINK->ch == 'n' || LINK->ch == 'm' || LINK->ch == 'l' ||
	LINK->ch == 'k' || LINK->ch == 'j' || LINK->ch == 'i' ||
	LINK->ch == 'h' || LINK->ch == 'g' || LINK->ch == 'f' ||
	LINK->ch == 'e' || LINK->ch == 'd' || LINK->ch == 'c' ||
	LINK->ch == 'b' || LINK->ch == 'a')) {
/* p2c: untex.p, line 195: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 162 [251] */
    LINK->ch = getc(LINK->fin->f);
    if (LINK->ch == '\n')
      LINK->ch = ' ';
  }
  if (LINK->ch != '[' && LINK->ch != '{')
    return;


  putc(' ', LINK->fout->f);
  while (!P_eoln(LINK->fin->f) && LINK->ch != '}') {
    LINK->ch = getc(LINK->fin->f);
    if (LINK->ch == '\n')
      LINK->ch = ' ';
    if (LINK->ch == '\\')
      removecommand(LINK);
    else if (LINK->ch != ']' && LINK->ch != '}')
      putc(LINK->ch, LINK->fout->f);
  }
}



Static Void themain(fin_, fout_)
_TEXT *fin_, *fout_;
{
  struct LOC_themain V;

  V.fin = fin_;
  V.fout = fout_;
  V.blanks = 0;
  V.lines = 0;

  while (!BUFEOF(V.fin->f)) {
    while (!P_eoln(V.fin->f)) {
      V.ch = getc(V.fin->f);


      if (V.ch == '\n')
	V.ch = ' ';
      if (V.ch == '}' || V.ch == '_' || V.ch == '$' || V.ch == '{')
	continue;

      if (V.ch == '~') {
	/*


*/


	out(' ', &V);
	continue;
      }



      if (V.ch == '\\') {
	removecommand(&V);
	continue;
      }


      if (V.ch == '%') {
	while (!P_eoln(V.fin->f)) {
	  V.ch = getc(V.fin->f);
	  if (V.ch == '\n')
	    V.ch = ' ';
	}
      }



      else
	out(V.ch, &V);
    }
    fscanf(V.fin->f, "%*[^\n]");
    getc(V.fin->f);
    putc('\n', V.fout->f);
    /*


*/
  }
  putc('\n', V.fout->f);
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
