/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "mstrip.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"



#define version         1.01
/*
*/



/*





















*/


#define controlm        '\015'


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  while (!P_eof(stdin)) {
    while (!P_eoln(stdin)) {
      if (P_peek(stdin) != controlm)
	putchar(P_peek(stdin));
      getc(stdin);
    }
    scanf("%*[^\n]");
    getchar();
    putchar('\n');
  }
  exit(EXIT_SUCCESS);
}



/* End. */
