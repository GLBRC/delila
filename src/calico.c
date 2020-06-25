/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "calico.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/


#define version         1.08


main(argc, argv)
int argc;
Char *argv[];
{
  /*
*/



  /*





























*/


  long tchars = 0, lines = 0;

  PASCAL_MAIN(argc, argv);
  printf(" calico %4.2f\n", version);
  while (!P_eof(stdin)) {
    while (!P_eoln(stdin)) {
      getc(stdin);
      tchars++;
    }

    scanf("%*[^\n]");
    getchar();
    lines++;
  }

  printf(" the file contains");
  printf(" %ld characters and %ld lines.\n", tchars, lines);
  exit(EXIT_SUCCESS);
}



/* End. */
