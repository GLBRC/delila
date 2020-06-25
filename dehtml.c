/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dehtml.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"



#define version         1.00


/*
*/



/*
























*/


#define debug           false


/*




*/

main(argc, argv)
int argc;
Char *argv[];
{
  Char c = ' ';
  Char p;
  long state = 0;

  PASCAL_MAIN(argc, argv);
  while (!P_eof(stdin)) {
    while (!P_eoln(stdin)) {
      c = getchar();

      if (c == '\n')
	c = ' ';
      switch (state) {

      case 0:
	if (c == '<')
	  state = 1;
	break;

      case 1:
	if (c == '>')
	  state = 2;
	break;

      case 2:
	if (c == '<')
	  state = 1;
	else
	  state = 0;
	break;

      }

      if (state == 0)
	putchar(c);
      if (debug) {
	if (state == 1)
	  putchar('1');
	if (state == 2)
	  putchar('2');
      }

    }
    scanf("%*[^\n]");
    getchar();
    putchar('\n');
  }
  exit(EXIT_SUCCESS);
}



/* End. */
