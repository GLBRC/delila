/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "counter.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00
/*

*/
#define updateversion   1.00



/*























































*/


Static _TEXT afile, counterp, bfile;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define ordControlH     8



Static Void digitcounter(afile, counter, previous)
_TEXT *afile;
long *counter, *previous;
{
  /*





*/
  /*

*/

  long digits, digitcount;

  if (*previous > 0) {
    /*

*/
    digits = (long)(log(*previous + 0.5) / log(10.0) + 1);
    /*


*/
    for (digitcount = 1; digitcount <= digits; digitcount++)
      putchar((Char)ordControlH);
    /*

*/
  }

  printf("%ld", *counter);
  *previous = *counter;
}

#undef ordControlH


#define ordControlH     8



Static Void themain(afile, counterp, bfile)
_TEXT *afile, *counterp, *bfile;
{
  /*

*/

  double parameterversion;
  long widthindex, number, width;

  /*

*/

  scanf("%ld%ld", &number, &width);
  printf("%*ld", (int)width, number);
  for (widthindex = 1; widthindex <= width; widthindex++)
    putchar((Char)ordControlH);

  /*


*/

}

#undef ordControlH


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  bfile.f = NULL;
  *bfile.name = '\0';
  counterp.f = NULL;
  *counterp.name = '\0';
  afile.f = NULL;
  *afile.name = '\0';
  themain(&afile, &counterp, &bfile);
_L1:
  if (afile.f != NULL)
    fclose(afile.f);
  if (counterp.f != NULL)
    fclose(counterp.f);
  if (bfile.f != NULL)
    fclose(bfile.f);
  exit(EXIT_SUCCESS);
}



/* End. */
