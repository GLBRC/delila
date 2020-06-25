/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "doudiff.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/


#define version         1.00


main(argc, argv)
int argc;
Char *argv[];
{
  /*

*/



  /*






























*/


  long current, previous;
  boolean haveprevious = false;
  long PREprevious;
  boolean havePREprevious = false;

  PASCAL_MAIN(argc, argv);
  while (!P_eof(stdin)) {
    if (P_peek(stdin) == '*') {
      while (!P_eoln(stdin)) {
	putchar(P_peek(stdin));
	getc(stdin);
      }
      haveprevious = false;
      havePREprevious = false;
      scanf("%*[^\n]");
      getchar();
      putchar('\n');
      continue;
    }

    scanf("%ld%*[^\n]", &current);
    getchar();
    /*

*/
    if (havePREprevious)
      printf("%ld %ld\n", current - previous, current - PREprevious);


    if (haveprevious) {
      havePREprevious = true;
      PREprevious = previous;
    }
    haveprevious = true;
    previous = current;
  }
  exit(EXIT_SUCCESS);
}













/* End. */
