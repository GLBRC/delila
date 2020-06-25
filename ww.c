/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "ww.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/


#define version         1.03
/*
*/



/*





























*/


#define maxline         70


main(argc, argv)
int argc;
Char *argv[];
{
  Char c;
  long n;
  boolean waitforspace;

  PASCAL_MAIN(argc, argv);
  while (!P_eof(stdin)) {
    n = 0;
    waitforspace = false;
    while (!P_eoln(stdin)) {
      if (n > maxline)
	waitforspace = true;

      if (waitforspace & (P_peek(stdin) == ' ')) {
	putchar('\n');
	getc(stdin);
	n = 0;
	waitforspace = false;
      } else {
	putchar(P_peek(stdin));
	getc(stdin);
	n++;
      }
    }

    scanf("%*[^\n]");
    getchar();
    putchar('\n');
  }
  exit(EXIT_SUCCESS);
}



/* End. */
