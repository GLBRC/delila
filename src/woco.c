/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "woco.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.09


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

  printf("woco %4.2f\n", version);


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
/* p2c: woco.p, line 91: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 87 [251] */
	if (!inside)
	  count++;
	/*

*/
	inside = true;
      }


      else if (c == '\\')
	inside = true;
      else
	inside = false;
    }
    fscanf(afile->f, "%*[^\n]");
    getc(afile->f);
    inside = false;
  }
  printf("\n%ld\n", count);
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
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */