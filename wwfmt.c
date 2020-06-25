/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "wwfmt.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/


#define version         1.01
/*
*/



/*














































*/


#define maxline         65


main(argc, argv)
int argc;
Char *argv[];
{
  Char c;
  long n = 0;
  boolean waitforspace;
  boolean paragraph = false;

  PASCAL_MAIN(argc, argv);
  while (!P_eof(stdin)) {
    /*

*/
    if (P_eoln(stdin)) {
      scanf("%*[^\n]");
      getchar();
      if (P_eof(stdin))
	break;
      if (!P_eoln(stdin)) {
	if (n > 0) {
	  putchar(' ');
	  n++;
	  paragraph = false;
	  /*

*/
	}
	continue;
      }
      putchar('\n');
      if (!paragraph)
	putchar('\n');
      paragraph = true;
      n = 0;
      continue;
    }
    waitforspace = false;
    while (!P_eoln(stdin)) {
      if (n > maxline)
	waitforspace = true;
      c = getchar();

      if (c == '\n')
	c = ' ';
      if (waitforspace && c == ' ') {
	printf(" \n");
	n = 0;
	/*

*/
	waitforspace = false;
      } else {
	putchar(c);
	n++;

      }
    }
  }

  if (n != 0)
    putchar('\n');

  exit(EXIT_SUCCESS);
}



/* End. */
