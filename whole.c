/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "whole.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*
*/
#define updateversion   1.00


#define linemax         300



/*




































*/



Static Void themain()
{
  long i;
  Char l[linemax];
  long length = 0;

  while (!P_eof(stdin)) {
    if (!P_eoln(stdin)) {
      length++;
      l[length-1] = getchar();
      if (l[length-1] == '\n')
	l[length-1] = ' ';
      continue;
    }
    scanf("%*[^\n]");
    getchar();
    /*

*/
    if (!P_eof(stdin)) {
      for (i = 0; i < length; i++)
	putchar(l[i]);
      putchar('\n');
    }
    length = 0;
  }
}

#undef linemax


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
