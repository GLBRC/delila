/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "codecomments.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/


#define version         2.10
/*






*/



/*










































































































*/



#define debugging       false


main(argc, argv)
int argc;
Char *argv[];
{
  long numchar = 0, numcomchar = 0, numcom = 0, numline = 0;
  boolean comment = false;

  Char first, second;
  double percent;

  PASCAL_MAIN(argc, argv);
  printf(" codecomments %4.2f: comment density of a pascal program\n",
	 version);
  printf(" Only comments of the form (* ... *) are recorded, \n");
  printf(" including comments inside quote marks...\n\n");




  if (!P_eof(stdin)) {
    second = getchar();
    if (second == '\n')
      second = ' ';
    numchar++;
  }

  while (!P_eof(stdin)) {
    if (P_eoln(stdin)) {
      scanf("%*[^\n]");
      getchar();
      numline++;
      second = ' ';
      if (debugging)
	printf("eoln %3ld\n", numline);
      continue;
    }
    first = second;
    second = getchar();
    if (second == '\n')
      second = ' ';
    numchar++;
    if (debugging)
      printf("%c%3ld", second, numchar);

    switch (comment) {

    case false:
      if (first == '(') {
	if (second == '*') {
	  comment = true;
	  numcom++;
	  numcomchar += 2;
	}
      }
      break;

    case true:
      numcomchar++;
      if (first == '*') {
	if (second == ')')
	  comment = false;
      }
      break;
    }
    if (debugging) {
      if (comment)
	printf("c\n");
      else
	putchar('\n');
    }
  }


  printf(" number of characters: %ld\n", numchar);
  printf(" number of lines: %ld\n", numline);

  if (numchar == 0)
    exit(EXIT_SUCCESS);
  printf(" number of comment characters: %ld\n", numcomchar);
  printf(" number of comments: %ld\n", numcom);
  percent = 100 * ((double)numcomchar / numchar);
  printf(" percent comments by characters: %3.1f\n", percent);
  if (percent < 30)
    printf(" The program needs more comments!\n");
  else
    putchar('\n');
  exit(EXIT_SUCCESS);
}



/* End. */
