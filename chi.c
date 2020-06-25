/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "chi.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.04


/*
*/



/*





























*/



Static Void themain()
{
  long df;
  double TEMP;

  printf("chi %4.2f\n", version);
  printf("give degrees of freedom: \n");
  scanf("%ld%*[^\n]", &df);
  getchar();

  if (df <= 6)
    printf("WARNING: degrees too small\n");
  if (df > 100)
    printf("WARNING: degrees too big\n");
  printf("\nNOTE: result is around 1.3%% off\n\n");

  printf("for           chi squared is\n");
  printf("significance  \n");
  TEMP = sqrt((double)df) + 0.833;
  printf("0.1           %8.2f\n", TEMP * TEMP);
  TEMP = sqrt((double)df) + 1.115;
  printf("0.05          %8.2f\n", TEMP * TEMP);
  TEMP = sqrt((double)df) + 1.365;
  printf("0.025         %8.2f\n", TEMP * TEMP);
  TEMP = sqrt((double)df) + 1.662;
  printf("0.010         %8.2f\n", TEMP * TEMP);
  TEMP = sqrt((double)df) + 1.867;
  printf("0.005         %8.2f\n", TEMP * TEMP);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  themain();
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
