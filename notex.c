/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "notex.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.32


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *fout;
  long blanks, lines;
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

  if (LINK->blanks > 0)
    putc(' ', LINK->fout->f);
  putc(c, LINK->fout->f);
  LINK->blanks = 0;
}


/*
*/



/*































*/



Static Void themain(fin, fout_)
_TEXT *fin, *fout_;
{
  struct LOC_themain V;
  Char ch;
  boolean done;

  V.fout = fout_;
  V.blanks = 0;
  V.lines = 0;
  while (!BUFEOF(fin->f)) {
    while (!P_eoln(fin->f)) {
      ch = getc(fin->f);


      if (ch == '\n')
	ch = ' ';
      if (ch == '}' || ch == '$' || ch == '{') {
	out(' ', &V);
	continue;
      }


      if ((ch == ')' || ch == '(' || ch == ',' || ch == '.') && V.blanks > 0)
	continue;


      if (ch == '\\') {
	out(' ', &V);
	ch = getc(fin->f);
	if (ch == '\n')
	  ch = ' ';
	while (!P_eoln(fin->f) && (ch == 'Z' || ch == 'Y' || ch == 'X' ||
				   ch == 'W' || ch == 'V' || ch == 'U' ||
				   ch == 'T' || ch == 'S' || ch == 'R' ||
				   ch == 'Q' || ch == 'P' || ch == 'O' ||
				   ch == 'N' || ch == 'M' || ch == 'L' ||
				   ch == 'K' || ch == 'J' || ch == 'I' ||
				   ch == 'H' || ch == 'G' || ch == 'F' ||
				   ch == 'E' || ch == 'D' || ch == 'C' ||
				   ch == 'B' || ch == 'A' || ch == 'z' ||
				   ch == 'y' || ch == 'x' || ch == 'w' ||
				   ch == 'v' || ch == 'u' || ch == 't' ||
				   ch == 's' || ch == 'r' || ch == 'q' ||
				   ch == 'p' || ch == 'o' || ch == 'n' ||
				   ch == 'm' || ch == 'l' || ch == 'k' ||
				   ch == 'j' || ch == 'i' || ch == 'h' ||
				   ch == 'g' || ch == 'f' || ch == 'e' ||
				   ch == 'd' || ch == 'c' || ch == 'b' ||
				   ch == 'a')) {
/* p2c: notex.p, line 139: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 143 [251] */
	  ch = getc(fin->f);
	  if (ch == '\n')
	    ch = ' ';
	}
	if (ch != '[' && ch != '{')
	  continue;
	do {
	  ch = getc(fin->f);
	  if (ch == '\n')
	    ch = ' ';
	  done = (ch == '}') | BUFEOF(fin->f);
	  if (!done) {
	    if (P_eoln(fin->f)) {
	      fscanf(fin->f, "%*[^\n]");
	      getc(fin->f);
	    }
	  }
	} while (!done);
	continue;
      }


      if (ch == '%') {
	while (!P_eoln(fin->f)) {
	  ch = getc(fin->f);
	  if (ch == '\n')
	    ch = ' ';
	}
      }



      else if (ch == ' ' ||
	       ch == '9' || ch == '8' ||
	       ch == '7' || ch == '6' || ch == '5' || ch == '4' ||
	       ch == '3' || ch == '2' ||
	       ch == '1' ||
	       ch == '0' || ch == 'Z' ||
	       ch == 'Y' ||
	       ch == 'X' || ch == 'W' || ch == 'V' || ch == 'U' ||
	       ch == 'T' ||
	       ch == 'S' || ch == 'R' || ch == 'Q' ||
	       ch == 'P' || ch == 'O' ||
	       ch == 'N' ||
	       ch == 'M' ||
	       ch == 'L' || ch == 'K' ||
	       ch == 'J' || ch == 'I' || ch == 'H' || ch == 'G' ||
	       ch == 'F' ||
	       ch == 'E' || ch == 'D' ||
	       ch == 'C' || ch == 'B' ||
	       ch == 'A' || ch == 'z' ||
	       ch == 'y' ||
	       ch == 'x' ||
	       ch == 'w' || ch == 'v' || ch == 'u' ||
	       ch == 't' || ch == 's' || ch == 'r' ||
	       ch == 'q' || ch == 'p' || ch == 'o' ||
	       ch == 'n' || ch == 'm' || ch == 'l' || ch == 'k' ||
	       ch == 'j' ||
	       ch == 'i' ||
	       ch == 'h' ||
	       ch == 'g' || ch == 'f' ||
	       ch == 'e' ||
	       ch == 'd' || ch == 'c' ||
	       ch == 'b' ||
	       ch == 'a')
/* p2c: notex.p, line 139: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 210 [251] */
	out(ch, &V);
      else
	out(' ', &V);
    }
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
    out('\\', &V);
  }
}


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
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
