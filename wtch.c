/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "wtch.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/


#define version         1.27
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


#define NULL_           0
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





  fprintf(fout->f, "** wtch %4.2f\n", version);
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
    if (P_eoln(fin->f)) {
      if (!BUFEOF(fin->f))
	count++;
      WITH = &chars[CR];

      if (WITH->nu > 0) {
	if (!BUFEOF(fin->f))
	  WITH->nu++;
      } else {
	WITH->nu = 1;
	WITH->ch = P_peek(fin->f);
      }
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      continue;
    }
    count++;
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


  fprintf(fout->f, "** %ld characters found\n", count);
  fprintf(fout->f, "**\n");
  if (count == 0) {
    fprintf(fout->f, "**\n");
    return;
  }
  fprintf(fout->f, "** char ord(char)  count percent\n");
  for (TEMP = 0; TEMP <= maxchars; TEMP++) {
    index = TEMP;
    if (chars[index].nu != 0) {
      if (index == NULL_ || index == CR || index == LF || index == TAB ||
	  index == SPACE || index > HIGH)
	fprintf(fout->f, "__-");
      else {
	fprintf(fout->f, "  ");

	putc(chars[index].ch, fout->f);
      }
      fprintf(fout->f, "     %5d", index);
      fprintf(fout->f, "   %8ld", chars[index].nu);
      fprintf(fout->f, " %7.2f", 100 * ((double)chars[index].nu / count));
      if (chars[index].ch != index) {
	if (index == CR)
	  fprintf(fout->f, " CR: Carriage Return");
	else {
	  fprintf(fout->f, " warning: program error,");
	  fprintf(fout->f, " actual character is different");
	}
      }
      if (index == SPACE)
	fprintf(fout->f, " Space");
      else if (index == TAB)
	fprintf(fout->f, " TAB");
      else if (index == LF)
	fprintf(fout->f, " LF ");
      else if (index == CR)
	fprintf(fout->f, " CR = control-M");
      else if (index == NULL_)
	fprintf(fout->f, " NULL");
      else if (index > HIGH) {
	fprintf(fout->f, " HIGH ORDER BIT IS ON");

	if (index < 32)
	  fprintf(fout->f, " char-32=\"%c\"", index + 32);
	if (index > 128)
	  fprintf(fout->f, " char-128=\"%c\"", index - 128);
      }
      putc('\n', fout->f);
    }

  }
}

#undef NULL_
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
