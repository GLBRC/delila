/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "diffint.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/


#define version         1.07


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

  PASCAL_MAIN(argc, argv);
  while (!P_eof(stdin)) {
    if (P_peek(stdin) == '*') {
      while (!P_eoln(stdin)) {
	putchar(P_peek(stdin));
	getc(stdin);
      }
      haveprevious = false;
      scanf("%*[^\n]");
      getchar();
      putchar('\n');
      continue;
    }

    scanf("%ld%*[^\n]", &current);
    getchar();
    if (haveprevious)
      printf("%10ld\n", current - previous);
    haveprevious = true;
    previous = current;
  }
  exit(EXIT_SUCCESS);
}













/* End. */
