/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "unnumber.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*


*/
#define updateversion   1.00



/*










































*/



#define maxstring       20000




typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *next;
} stringDelila;


Static _TEXT infile, unnumberp, outfile;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}





Static Void emptystring(ribbon)
stringDelila *ribbon;
{
  /*

*/
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}


Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  emptystring(ribbon);
  ribbon->next = NULL;
}


Static Void initializestring(ribbon)
stringDelila *ribbon;
{
  /*



*/
  printf("remove initializestring routine!\n");
  printf("replace it with clearstring routine!\n");
  halt();
  clearstring(ribbon);
  ribbon->next = NULL;
}



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}



Static Void getstring(afile, buffer, gotten)
_TEXT *afile;
stringDelila *buffer;
boolean *gotten;
{
  /*
*/
  long index = 0;

  clearstring(buffer);
  if (BUFEOF(afile->f)) {
    *gotten = false;
    return;
  }
  while (!P_eoln(afile->f) && index < maxstring) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
  }
  if (!P_eoln(afile->f)) {
    printf(" getstring: a line exceeds maximum string size (%ld)\n",
	   (long)maxstring);
    halt();
  }
  buffer->length = index;
  buffer->current = 1;
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  *gotten = true;
}



Static Void makenumber(name, number, found)
stringDelila name;
long *number;
boolean *found;
{
  /*
*/
  long l;

  *found = false;
  *number = 0;
  for (l = 0; l < name.length; l++) {
    if (name.letters[l] == '9' || name.letters[l] == '8' ||
	name.letters[l] == '7' || name.letters[l] == '6' ||
	name.letters[l] == '5' || name.letters[l] == '4' ||
	name.letters[l] == '3' || name.letters[l] == '2' ||
	name.letters[l] == '1' || name.letters[l] == '0') {
      *found = true;
      *number *= 10;
      switch (name.letters[l]) {

      case '0':
	/* blank case */
	break;

      case '1':
	(*number)++;
	break;

      case '2':
	*number += 2;
	break;

      case '3':
	*number += 3;
	break;

      case '4':
	*number += 4;
	break;

      case '5':
	*number += 5;
	break;

      case '6':
	*number += 6;
	break;

      case '7':
	*number += 7;
	break;

      case '8':
	*number += 8;
	break;

      case '9':
	*number += 9;
	break;
      }
    }
  }
}



Static boolean isonlynumber(l)
stringDelila l;
{
  boolean alldigits;
  long p;

  if (l.length > 0) {
    alldigits = true;
    p = 1;
    while (p <= l.length && alldigits) {
      if (l.letters[p-1] != '9' && l.letters[p-1] != '8' &&
	  l.letters[p-1] != '7' && l.letters[p-1] != '6' &&
	  l.letters[p-1] != '5' && l.letters[p-1] != '4' &&
	  l.letters[p-1] != '3' && l.letters[p-1] != '2' &&
	  l.letters[p-1] != '1' && l.letters[p-1] != '0')
	alldigits = false;
      p++;
    }
    return alldigits;
  } else
    return false;
}



Static Void themain(infile, unnumberp, outfile)
_TEXT *infile, *unnumberp, *outfile;
{
  double parameterversion;
  stringDelila aline;
  long linenumber = 10;
  /*
*/
  long increment = 5;
  boolean gotten = true;
  boolean numberfound;
  long readnumber;
  _TEXT TEMP;

  printf("unnumber %4.2f\n", version);
  if (*unnumberp->name != '\0') {
    if (unnumberp->f != NULL)
      unnumberp->f = freopen(unnumberp->name, "r", unnumberp->f);
    else
      unnumberp->f = fopen(unnumberp->name, "r");
  } else
    rewind(unnumberp->f);
  if (unnumberp->f == NULL)
    _EscIO2(FileNotFound, unnumberp->name);
  RESETBUF(unnumberp->f, Char);
  fscanf(unnumberp->f, "%lg%*[^\n]", &parameterversion);
  getc(unnumberp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else
    rewind(infile->f);
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);
  if (*outfile->name != '\0') {
    if (outfile->f != NULL)
      outfile->f = freopen(outfile->name, "w", outfile->f);
    else
      outfile->f = fopen(outfile->name, "w");
  } else {
    if (outfile->f != NULL)
      rewind(outfile->f);
    else
      outfile->f = tmpfile();
  }
  if (outfile->f == NULL)
    _EscIO2(FileNotFound, outfile->name);
  SETUPBUF(outfile->f, Char);
  clearstring(&aline);

  while (gotten) {
    getstring(infile, &aline, &gotten);
    if (!gotten)
      break;

    /*
*/

    if (!isonlynumber(aline)) {

      writestring(outfile, &aline);
      putc('\n', outfile->f);
      continue;
    }

    makenumber(aline, &readnumber, &numberfound);

    if (!numberfound) {
      printf("program error - not numberfound\n");
      halt();
      continue;
    }
    if (readnumber == linenumber) {
      printf("line number: ");
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &aline);
      putchar('\n');
      linenumber += increment;
    }
    /*
*/
    else {
      writestring(outfile, &aline);
      putc('\n', outfile->f);

    }
  }

  printf("unnumber is done\n");

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  outfile.f = NULL;
  strcpy(outfile.name, "outfile");
  unnumberp.f = NULL;
  strcpy(unnumberp.name, "unnumberp");
  infile.f = NULL;
  strcpy(infile.name, "infile");
  themain(&infile, &unnumberp, &outfile);
_L1:
  if (infile.f != NULL)
    fclose(infile.f);
  if (unnumberp.f != NULL)
    fclose(unnumberp.f);
  if (outfile.f != NULL)
    fclose(outfile.f);
  exit(EXIT_SUCCESS);
}



/* End. */
