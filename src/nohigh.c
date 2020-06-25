/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "nohigh.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00
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


#define debugging       false



Static Void themain()
{
  Char c;
  long n = 0;

  /*

*/

  rewind(stdin);
/* p2c: nohigh.p, line 86: Note: REWRITE does not specify a name [181] */
/* p2c: nohigh.p, line 86:
 * Note: RESET/REWRITE ignored for file OUTPUT [319] */
  if (stdout == NULL)
    _EscIO(FileNotFound);

  while (!P_eof(stdin)) {
    if (debugging)
      printf("%ld\n", n);
    if (P_eoln(stdin)) {
      putchar('\n');
      scanf("%*[^\n]");
      getchar();
    } else {
      c = getchar();
      if (c == '\n')
	c = ' ';
      if (debugging)
	putchar(c);
      if ((c & 255) < 127)
	putchar(c);
    }
    if (debugging)
      n++;
  }
}

#undef debugging


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
