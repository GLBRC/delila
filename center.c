/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "center.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.08
/*
*/



/*








































*/



#define maxstring       150


typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;


Static _TEXT centerp;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}




Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
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



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}


#define debug           false



Static Void themain(centerp)
_TEXT *centerp;
{
  long b;
  stringDelila buffer;
  boolean done;
  long first;
  boolean gotten;
  long last, length, thecenter;
  _TEXT TEMP;
  long FORLIM;

  if (debug)
    printf("center %4.2f\n", version);
  if (*centerp->name != '\0') {
    if (centerp->f != NULL)
      centerp->f = freopen(centerp->name, "r", centerp->f);
    else
      centerp->f = fopen(centerp->name, "r");
  } else
    rewind(centerp->f);
  if (centerp->f == NULL)
    _EscIO2(FileNotFound, centerp->name);
  RESETBUF(centerp->f, Char);
  fscanf(centerp->f, "%ld%*[^\n]", &thecenter);
  getc(centerp->f);

  if (debug)
    printf("centering at %ld\n", thecenter);

  while (!P_eof(stdin)) {
    TEMP.f = stdin;
    *TEMP.name = '\0';
    getstring(&TEMP, &buffer, &gotten);
    if (!gotten)
      continue;
    if (buffer.length == 0)
      putchar('\n');
    else {
      if (debug)
	putchar('"');
      if (debug) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &buffer);
      }
      if (debug)
	printf("\"\n");


      first = 1;
      done = false;
      while (!done) {
	if (buffer.letters[first-1] == ' ')
	  first++;
	else
	  done = true;
	if (first >= buffer.length)
	  done = true;
      }


      last = buffer.length;
      done = false;
      while (!done) {
	if (buffer.letters[last-1] == ' ')
	  last--;
	else
	  done = true;
	if (last <= 1)
	  done = true;
      }

      if (debug)
	printf("first: %ld\n", first);
      if (debug)
	printf("last: %ld\n", last);

      if (debug)
	putchar(' ');
      if (debug) {
	for (b = 1; b <= buffer.length; b++) {
	  if (b == first)
	    putchar('F');
	  else if (b == last)
	    putchar('L');
	  else
	    putchar(' ');
	}
      }
      if (debug)
	printf(" \n");

      if (debug)
	putchar('"');
      if (debug) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writestring(&TEMP, &buffer);
      }
      if (debug)
	printf("\"\n");

      length = last - first + 1;
      if (debug)
	printf("length = %ld\n", length);

      if (length > 0) {
	FORLIM = thecenter - (long)floor(length / 2.0 + 0.5);
	for (b = 1; b <= FORLIM; b++)
	  putchar(' ');


	buffer.length = last;
	if (debug)
	  putchar('"');
	for (b = first - 1; b < last; b++)
	  putchar(buffer.letters[b]);
	if (debug)
	  putchar('"');
      }

      putchar('\n');
    }
    if (debug)
      printf("--------------------------------------\n");
  }

}

#undef debug


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  centerp.f = NULL;
  strcpy(centerp.name, "centerp");
  themain(&centerp);
_L1:
  if (centerp.f != NULL)
    fclose(centerp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
