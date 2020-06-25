/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "gifnormalize.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.07
/*




*/
#define updateversion   1.00



/*





























































































































*/


Static _TEXT gifnormalizep;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define maxwidth        1000
#define maxheight       1000

#define zerovalue       '\0'



/*

*/
Static Void themain(gifnormalizep)
_TEXT *gifnormalizep;
{
  long c, endwidth, endheight, h, height;
  Char m1, m2;
  long maximumcolorcomponentvalue;
  double parameterversion;
  Char store[maxwidth][maxheight][3];
  long width, w, startwidth, startheight;

  /*

*/
  if (*gifnormalizep->name != '\0') {
    if (gifnormalizep->f != NULL)
      gifnormalizep->f = freopen(gifnormalizep->name, "r", gifnormalizep->f);
    else
      gifnormalizep->f = fopen(gifnormalizep->name, "r");
  } else
    rewind(gifnormalizep->f);
  if (gifnormalizep->f == NULL)
    _EscIO2(FileNotFound, gifnormalizep->name);
  RESETBUF(gifnormalizep->f, Char);
  fscanf(gifnormalizep->f, "%lg%*[^\n]", &parameterversion);
  getc(gifnormalizep->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(gifnormalizep->f, "%ld%ld%*[^\n]", &startwidth, &startheight);
  getc(gifnormalizep->f);
  fscanf(gifnormalizep->f, "%ld%ld%*[^\n]", &endwidth, &endheight);
  getc(gifnormalizep->f);
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
  gifnormalizep.f = NULL;
  strcpy(gifnormalizep.name, "gifnormalizep");
  themain(&gifnormalizep);
_L1:
  if (gifnormalizep.f != NULL)
    fclose(gifnormalizep.f);
  exit(EXIT_SUCCESS);
}
















/* End. */
