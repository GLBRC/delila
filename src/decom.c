/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "decom.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/


#define version         1.07


/*



*/



/*




















































*/


#define debug           false

#define zap             '#'
#define zap2begin       zap
#define zap2end         zap
#define zap3begin       zap
#define zap3end         zap


/*





*/

main(argc, argv)
int argc;
Char *argv[];
{
  Char c = ' ';
  Char o, p;
  long state = 0;

  PASCAL_MAIN(argc, argv);
  while (!P_eof(stdin)) {
    while (!P_eoln(stdin)) {
      p = c;
      c = getchar();
      if (c == '\n')
	c = ' ';
      o = c;

      switch (state) {

      case 0:
	if (c == '\'')
	  state = 1;
	if (c == '{')
	  state = 2;
	if (p == '(' && c == '*')
	  state = 3;
	if (debug)
	  o = '0';
	break;

      case 1:
	if (c == '\'')
	  state = 0;
	if (debug)
	  o = '1';
	break;

      case 2:
	if (p == '(' && c == '*')
	  o = zap2begin;
	if (p == '*' && c == ')')
	  o = zap2end;
	if (c == '}')
	  state = 0;
	if (debug)
	  o = '2';
	break;

      case 3:
	if (c == '{')
	  o = zap3begin;
	if (c == '}')
	  o = zap3end;
	if (p == '*' && c == ')')
	  state = 0;
	if (debug)
	  o = '3';
	break;

      }
      putchar(o);

    }
    scanf("%*[^\n]");
    getchar();
    putchar('\n');
    p = ' ';
  }
  exit(EXIT_SUCCESS);
}



/* End. */
