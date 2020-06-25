/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "pnmnormalize.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.09
/*





*/
#define updateversion   1.00



/*



















































































































































*/


Static _TEXT pnmnormalizep;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define maxwidth        1500
#define maxheight       1000

#define zerovalue       '\0'



/*

*/
Static Void themain(pnmnormalizep)
_TEXT *pnmnormalizep;
{
  long c, endwidth, endheight, h, height;
  Char m1, m2;
  long maximumcolorcomponentvalue;
  double parameterversion;
  Char store[maxwidth][maxheight][3];
  long width, w, startwidth, startheight;

  /*

*/
  if (*pnmnormalizep->name != '\0') {
    if (pnmnormalizep->f != NULL)
      pnmnormalizep->f = freopen(pnmnormalizep->name, "r", pnmnormalizep->f);
    else
      pnmnormalizep->f = fopen(pnmnormalizep->name, "r");
  } else
    rewind(pnmnormalizep->f);
  if (pnmnormalizep->f == NULL)
    _EscIO2(FileNotFound, pnmnormalizep->name);
  RESETBUF(pnmnormalizep->f, Char);
  fscanf(pnmnormalizep->f, "%lg%*[^\n]", &parameterversion);
  getc(pnmnormalizep->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(pnmnormalizep->f, "%ld%ld%*[^\n]", &startwidth, &startheight);
  getc(pnmnormalizep->f);
  fscanf(pnmnormalizep->f, "%ld%ld%*[^\n]", &endwidth, &endheight);
  getc(pnmnormalizep->f);
  if (endwidth > maxwidth) {
    printf("# endwidth exceeds maxwidth = %ld\n", (long)maxwidth);
    halt();
  }
  if (endheight > maxheight) {
    printf("# endheight exceeds maxheight = %ld\n", (long)maxheight);
    halt();
  }

  if (endwidth < startwidth) {
    printf("# endwidth must exceeds startwidth\n");
    halt();
  }

  if (startwidth < 1) {
    printf("# startwidth must be positive\n");
    halt();
  }

  if (endheight < startheight) {
    printf("# endheight must exceeds startheight\n");
    halt();
  }

  if (startheight < 1) {
    printf("# startheight must be positive\n");
    halt();
  }

  if (startwidth > maxwidth) {
    printf("# startwidth exceeds maxwidth = %ld\n", (long)maxwidth);
    halt();
  }
  if (startheight > maxheight) {
    printf("# startheight exceeds maxheight = %ld\n", (long)maxheight);
    halt();
  }

  m1 = getchar();
  m2 = getchar();

  if (m1 == '\n')
    m1 = ' ';
  if (m2 == '\n')
    m2 = ' ';
  if (m1 != 'P' || m2 != '6') {
    printf("# magic number is not P6\n");
    halt();
  }
  scanf("%ld%ld%ld%*[^\n]", &width, &height, &maximumcolorcomponentvalue);
  getchar();
  /*




*/

  if (width > maxwidth) {
    printf("width = %ld > %ld = maxwidth the maximum allowed width\n",
	   width, (long)maxwidth);
    halt();
  }

  if (height > maxheight) {
    printf("height = %ld > %ld = maxheight the maximum allowed height\n",
	   height, (long)maxheight);
    halt();
  }


  for (h = 0; h < maxheight; h++) {
    for (w = 0; w < maxwidth; w++) {
      for (c = 0; c <= 2; c++)
	store[w][h][c] = zerovalue;
    }
  }


  for (h = 0; h < height; h++) {
    for (w = 0; w < width; w++) {
      for (c = 0; c <= 2; c++) {
	store[w][h][c] = getchar();
	if (store[w][h][c] == '\n')
	  store[w][h][c] = ' ';
      }
    }
  }





  scanf("%*[^\n]");
  getchar();
  if (!P_eof(stdin))
    printf("# did not find end of file!\n");

  printf("%c%c\n", m1, m2);
  printf("%ld %ld\n", endwidth - startwidth + 1, endheight - startheight + 1);
  printf("%ld\n", maximumcolorcomponentvalue);

  for (h = startheight - 1; h < endheight; h++) {
    for (w = startwidth - 1; w < endwidth; w++) {
      for (c = 0; c <= 2; c++)
	putchar(store[w][h][c]);
    }
  }


}

#undef maxwidth
#undef maxheight
#undef zerovalue


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  pnmnormalizep.f = NULL;
  strcpy(pnmnormalizep.name, "pnmnormalizep");
  themain(&pnmnormalizep);
_L1:
  if (pnmnormalizep.f != NULL)
    fclose(pnmnormalizep.f);
  exit(EXIT_SUCCESS);
}



/* End. */
