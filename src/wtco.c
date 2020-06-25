/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "wtco.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/


#define version         1.26
/*















*/



/*




























































































*/




#define maxchars        255


typedef struct charident {
  Char ch;
  long nu;
} charident;


Static Void copyaline(fin, fout)
_TEXT *fin, *fout;
{
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
  putc('\n', fout->f);
}


#define CR              13
#define LF              10
#define TAB             9
#define SPACE           32
#define HIGH            127


Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  charident chars[maxchars + 1];
  uchar index;
  long count = 0;
  short TEMP;
  charident *WITH;





  fprintf(fout->f, "** wtco %4.2f\n", version);
  /*




*/


  for (TEMP = 0; TEMP <= maxchars; TEMP++) {
    index = TEMP;
    WITH = &chars[index];
    WITH->ch = ' ';
    WITH->nu = 0;
  }
  chars[CR].ch = (Char)CR;


  while (!BUFEOF(fin->f)) {
    count++;


    /*


















*/

    if (P_peek(fin->f) > maxchars) {
      fprintf(fout->f,
	"** character with ord = %d this is greater than maxchars (%ld)\n",
	P_peek(fin->f), (long)maxchars);
      printf("** increase maxchars\n");
    } else if (P_peek(fin->f) < 0)
      fprintf(fout->f, "** character with negative ord: %d\n", P_peek(fin->f));
    else {
      WITH = &chars[P_peek(fin->f)];
      if (WITH->nu > 0)
	WITH->nu++;
      else {
	WITH->nu = 1;
	WITH->ch = P_peek(fin->f);
      }
    }
    getc(fin->f);
  }
  /*

*/


  fprintf(fout->f, "** %ld characters found\n", count);
  fprintf(fout->f, "**\n");
  if (count == 0)
    return;
  /*

*/
  /*

*/
  for (TEMP = 0; TEMP <= maxchars; TEMP++) {
    index = TEMP;
    if (chars[index].nu != 0) {
      fprintf(fout->f, "     %5d", index);
      fprintf(fout->f, "   %8ld\n", chars[index].nu);
      /*





























*/
      /*



*/
    }

    /*








*/

    /*

*/
  }
}

#undef CR
#undef LF
#undef TAB
#undef SPACE
#undef HIGH


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
  exit(EXIT_SUCCESS);
}



/* End. */
