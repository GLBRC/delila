/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "yes.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/

/*

*/


#define version         1.02


/*


*/



/*





































*/



Static Void themain(fout)
_TEXT *fout;
{
  while (true)
    fprintf(fout->f, "y\n");
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  themain(&TEMP);
  exit(EXIT_SUCCESS);
}



/* End. */
