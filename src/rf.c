/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rf.p" */


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


  double gammaDelila, g, f;

  PASCAL_MAIN(argc, argv);
  printf("rf %4.2f\n", version);

  printf("number of potential binding sites, G: ");
  scanf("%lg%*[^\n]", &g);
  getchar();

  printf("number of sites in the genome, gamma: ");
  scanf("%lg%*[^\n]", &gammaDelila);
  getchar();

  f = gammaDelila / g;
  printf("f = % .3E\n", f);
  printf("Rfrequency = %5.2f bits\n", -(log(f) / log(2.0)));
  exit(EXIT_SUCCESS);
}



/* End. */
