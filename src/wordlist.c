/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "wordlist.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/

/*

*/


#define version         1.16


/*




*/



/*




























*/




Static Void themain(afile)
_TEXT *afile;
{
  Char c;
  long count = 0;
  boolean inside = false;

  /*


*/


  while (!BUFEOF(afile->f)) {
    while (!P_eoln(afile->f)) {
      c = getc(afile->f);
      if (c == '\n')
	c = ' ';
      /*

*/
      if (c == '9' || c == '8' || c == '7' || c == '6' || c == '5' ||
	  c == '4' || c == '3' || c == '2' || c == '1' || c == '0' ||
	  c == 'Z' || c == 'Y' || c == 'X' || c == 'W' || c == 'V' ||
	  c == 'U' || c == 'T' || c == 'S' || c == 'R' || c == 'Q' ||
	  c == 'P' || c == 'O' || c == 'N' || c == 'M' || c == 'L' ||
	  c == 'K' || c == 'J' || c == 'I' || c == 'H' || c == 'G' ||
	  c == 'F' || c == 'E' || c == 'D' || c == 'C' || c == 'B' ||
	  c == 'A' || c == 'z' || c == 'y' || c == 'x' || c == 'w' ||
	  c == 'v' || c == 'u' || c == 't' || c == 's' || c == 'r' ||
	  c == 'q' || c == 'p' || c == 'o' || c == 'n' || c == 'm' ||
	  c == 'l' || c == 'k' || c == 'j' || c == 'i' || c == 'h' ||
	  c == 'g' || c == 'f' || c == 'e' || c == 'd' || c == 'c' ||
	  c == 'b' || c == 'a') {
/* p2c: wordlist.p, line 109: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 100 [251] */
	if (!inside)
	  count++;
	/*

*/
	inside = true;
      }


      else if (c == '\\')
	inside = true;
      else {
	if (inside == true)
	  putchar('\n');
	inside = false;
      }
      if (inside)
	putchar(c);
    }
    if (inside) {
      putchar('\n');
      inside = false;
    }
    fscanf(afile->f, "%*[^\n]");
    getc(afile->f);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdin;
  *TEMP.name = '\0';
  themain(&TEMP);
  exit(EXIT_SUCCESS);
}



/* End. */
