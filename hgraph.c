/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "hgraph.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"



#define version         1.08


/*


*/



/*


























































*/


Static long d, divisions;
Static double p, h, ln2, total;


Static double plop(p)
double p;
{
  return (-(p * log(p) / ln2));
}


main(argc, argv)
int argc;
Char *argv[];
{
  long FORLIM;

  PASCAL_MAIN(argc, argv);
  printf("* hgraph %4.2f\n", version);
  ln2 = log(2.0);

  printf("* number of divisions: \n");
  scanf("%ld%*[^\n]", &divisions);
  getchar();
  printf("* %ld\n", divisions);
  if (divisions < 2)
    divisions = 2;

  FORLIM = divisions;
  for (d = 0; d <= FORLIM; d++) {
    if (d != 0 && d != divisions) {
      p = (double)d / divisions;
      h = plop(p) + plop(1 - p);
    } else {
      if (d == 0)
	p = 0.0;
      else
	p = 1.0;
      h = 0.0;
    }

    printf("%10.8f %10.8f\n", p, h);
  }
  exit(EXIT_SUCCESS);
}



/* End. */
