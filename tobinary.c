/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "tobinary.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.01
/*
*/
#define updateversion   1.00


Static jmp_buf _JL1;



/*








































































































































*/



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain()
{
  long count, n, number;
  boolean dodecimals;
  long s, sign;
  Char store[50];

  if (P_peek(stdin) == '0') {
    scanf("%*[^\n]");
    getchar();
    dodecimals = false;
  } else
    dodecimals = true;

  while (!P_eof(stdin)) {
    scanf("%ld%*[^\n]", &number);
    getchar();
    if (dodecimals)
      printf("%ld ", number);

    if (number == 0) {
      printf(" 0\n");
      continue;
    }
    if (number < 0)
      sign = -1;
    else
      sign = 1;

    n = labs(number);
    count = 1;
    while (n > 0) {
      count++;
      if ((n & 1) == 0)
	store[count-1] = '0';
      else
	store[count-1] = '1';
      /*


*/
      n /= 2;
    }

    if (sign < 0)
      putchar('-');
    for (s = count - 1; s >= 0; s--)
      putchar(store[s]);
    putchar('\n');
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  themain();
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
