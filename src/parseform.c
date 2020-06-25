/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "parseform.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.05
/*



*/
#define updateversion   1.00


Static jmp_buf _JL1;



/*




















































*/


/*



*/


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Char decapitalize(c)
Char c;
{
  long n = c;

  if (n >= 'A' && n <= 'Z')
    c = _tolower(n);
  else
    c = (Char)n;

  return c;
}


#define showoriginal    2
#define maxstring       200



Static Void themain()
{
  Char c;
  boolean inhex = false, inname = true;
  long hexnumber, hexcount, n, namelength;
  /*

*/
  Char typestring[maxstring];

  /*







*/

  if (showoriginal == 0) {
    while (!P_eof(stdin)) {
      c = P_peek(stdin);
      putc(c, stdout);
      c = getchar();
      if (c == '\n')
	c = ' ';
    }
    return;
  }

  if (showoriginal == 1) {
    while (!P_eof(stdin)) {
      if (P_eoln(stdin))
	printf("eoln!\n");
      c = P_peek(stdin);
      if (c == '&')
	putchar('\n');
      else if (c == '+')
	putchar(' ');
      else
	putc(c, stdout);
      c = getchar();
      if (c == '\n')
	c = ' ';
    }
    return;
  }

  if (showoriginal != 2)
    return;

  while (!P_eof(stdin)) {
    /*


*/
    c = P_peek(stdin);

    if (c == '%') {
      inhex = true;
      hexnumber = 0;
      hexcount = 0;
    } else if (inhex) {

      c = decapitalize(c);
      /*


*/
      if (c >= '0' && c <= '9') {
	hexnumber = hexnumber * 16 + c - '0';
	hexcount++;
	/*

*/
      } else if (c >= 'a' && c <= 'e') {
	hexnumber = hexnumber * 16 + c + 10 - 'a';
	/*


*/
	putchar((Char)hexnumber);
	inhex = false;
      } else {
	printf("Error reading hexidecimal number, character = %c\n", c);
	halt();
      }
    }

    else if (inname) {
      if (c == '=')
	inname = false;
      else {
	namelength++;
	if (namelength > maxstring)
	  inname = false;
	else
	  typestring[namelength-1] = c;
      }
    }

    else {

      if ((c == '&') | P_eoln(stdin)) {
	putchar(' ');
	for (n = 0; n < namelength; n++)
	  putchar(typestring[n]);
	putchar('\n');
	namelength = 0;
	inname = true;
      } else if (c == '+')
	putchar(' ');
      else
	putc(c, stdout);
    }

    if (hexcount > 1) {
      putchar((Char)hexnumber);
      /*

*/
      hexcount = 0;
      inhex = false;
    }

    c = getchar();
    if (c == '\n')
      c = ' ';
  }
}

#undef showoriginal
#undef maxstring


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  themain();
  /*
*/
_L1:
  exit(EXIT_SUCCESS);
}

/*




*/



/* End. */
