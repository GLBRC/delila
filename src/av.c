/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "av.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/


#define version         1.03


/*
*/



/*




























*/



Static Void themain()
{
  long a, b, c;

  printf("av: average with rounding %4.2f\n", version);
  do {
    printf("Enter pairs of integers, \"0 0\" to quit\n");
    scanf("%ld%ld%*[^\n]", &a, &b);
    getchar();
    if (a != 0 || b != 0) {
      c = (long)floor((a + b) / 2.0 + 0.5);
      printf("(%ld + %ld)/2 = %ld\n", a, b, c);
    }
  } while (a != 0 || b != 0);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  themain();
  exit(EXIT_SUCCESS);
}



/* End. */
