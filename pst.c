/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "pst.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.02
/*


*/
#define updateversion   1.00



/*












































































*/


#define separator       '\t'


Static _TEXT afile, bfile;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(afile, bfile)
_TEXT *afile, *bfile;
{
  boolean done = false;
  Char c;

  /*

*/
  /*







*/

  if (*afile->name != '\0') {
    if (afile->f != NULL)
      afile->f = freopen(afile->name, "r", afile->f);
    else
      afile->f = fopen(afile->name, "r");
  } else
    rewind(afile->f);
  if (afile->f == NULL)
    _EscIO2(FileNotFound, afile->name);
  RESETBUF(afile->f, Char);
  if (*bfile->name != '\0') {
    if (bfile->f != NULL)
      bfile->f = freopen(bfile->name, "r", bfile->f);
    else
      bfile->f = fopen(bfile->name, "r");
  } else
    rewind(bfile->f);
  if (bfile->f == NULL)
    _EscIO2(FileNotFound, bfile->name);
  RESETBUF(bfile->f, Char);

  while (!done) {
    if (BUFEOF(afile->f))
      done = true;
    if (BUFEOF(bfile->f))
      done = true;
    if (done)
      break;


    while (!P_eoln(afile->f)) {
      c = getc(afile->f);
      if (c == '\n')
	c = ' ';
      putchar(c);
    }
    fscanf(afile->f, "%*[^\n]");

    getc(afile->f);
    putchar(separator);


    while (!P_eoln(bfile->f)) {
      c = getc(bfile->f);
      if (c == '\n')
	c = ' ';
      putchar(c);
    }
    fscanf(bfile->f, "%*[^\n]");

    getc(bfile->f);
    putchar('\n');

  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  bfile.f = NULL;
  strcpy(bfile.name, "bfile");
  afile.f = NULL;
  strcpy(afile.name, "afile");
  themain(&afile, &bfile);
_L1:
  if (afile.f != NULL)
    fclose(afile.f);
  if (bfile.f != NULL)
    fclose(bfile.f);
  exit(EXIT_SUCCESS);
}












/* End. */
