/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "shortline.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/


#define version         1.00
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

  PASCAL_MAIN(argc, argv);
  while (!P_eof(stdin)) {
    n = 0;
    while (!P_eoln(stdin)) {
      c = getchar();
      if (c == '\n')
	c = ' ';
      if (n > maxline) {
	putchar('\n');
	n = 0;
      } else {
	putchar(c);
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
