/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dangles.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"



#define version         1.07
/*

*/



/*



















*/


#define bppt            10.6
#define pi              3.14159265354


Static double angle;
Static boolean first;
Static long position;
Static double turns, x, y;


Static Void polrec(r, theta, x, y)
double r, theta, *x, *y;
{
  /*
*/
  *x = r * cos(theta);
  *y = r * sin(theta);
}



Static double degtorad(angle)
double angle;
{
  return (angle / 360 * 2 * pi);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  first = true;
  while (!P_eof(stdin)) {
    if (P_peek(stdin) == '*') {
      while (!P_eoln(stdin)) {
	putchar(P_peek(stdin));
	getc(stdin);
      }
    } else {
      if (first) {
	printf("* columns:\n");
	printf("* 1. position\n");
	printf("* 2. turns\n");
	printf("* 3. angle\n");
	printf("* 4. x\n");
	printf("* 5. y\n");
	first = false;
      }
      scanf("%ld", &position);

      turns = position / bppt;
      angle = 360 * (turns - (long)turns);
      polrec(1.0, degtorad(angle), &x, &y);

      printf("%6ld %11.1f %11.1f %11.8f %11.8f", position, turns, angle, x, y);
    }
    scanf("%*[^\n]");
    getchar();
    putchar('\n');
  }
  exit(EXIT_SUCCESS);
}



/* End. */
