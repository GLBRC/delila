/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "htmlink.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.62
/*
























*/



/*


































































































































































*/



#define maxline         100
#define fontsize        2



#define maxstring       2000




#define fillermax       50





typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
  struct stringDelila *previous, *next;
} stringDelila;




typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;



/*


*/
typedef Char filler[fillermax];



Static _TEXT htmlinkp, list;


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
  /*



*/
  emptystring(ribbon);
  ribbon->previous = NULL;
  ribbon->next = NULL;
}


Static Void disposestring(ribbon)
stringDelila **ribbon;
{
  stringDelila *t;

  t = *ribbon;

  while (t->next != NULL)
    t = t->next;

  while (t->previous != NULL) {
    t = t->previous;
    Free(t->next);
  }
  Free(t);
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




Static Void fillstring(s, a)
stringDelila *s;
Char *a;
{
  /*
*/


  /*


*/
  long length = fillermax;
  long index;

  clearstring(s);
  while (length > 1 && a[length-1] == ' ')
    length--;
  if (length == 1 && a[length-1] == ' ') {
    printf("fillstring: the string is empty\n");
    halt();
  }

  for (index = 0; index < length; index++)
    s->letters[index] = a[index];
  s->length = length;
  s->current = 1;
}



Static Void filltrigger(t, a)
trigger *t;
Char *a;
{
  fillstring(&t->seek, a);
}


/*




*/

Static Void resettrigger(t)
trigger *t;
{
  t->state = 0;
  t->skip = false;
  t->found = false;
}


Static Void testfortrigger(ch, t)
Char ch;
trigger *t;
{
  /*









*/
  t->state++;
  /*



*/
  if (t->seek.letters[t->state - 1] == ch) {
    t->skip = false;
    if (t->state == t->seek.length)
      t->found = true;
    else
      t->found = false;
    return;
  }
  /*
*/
  if (t->seek.letters[0] == ch) {
    t->state = 1;
    t->skip = false;
    t->found = false;
    return;
  }
  t->state = 0;
  t->skip = true;
  t->found = false;
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




Static Void writealine(fout, aline, start, stop, escape)
_TEXT *fout;
stringDelila aline;
long start, stop;
Char escape;
{
  /*
*/
  Char c = ' ';
  long i;
  Char p;

  for (i = start - 1; i < stop; i++) {
    p = c;
    c = aline.letters[i];
    if (p == escape)
      putc(c, fout->f);
    else if (c != escape && p != escape) {
      putc(c, fout->f);
      /*


*/
    }
  }
}


Static Void copyindentation(aline)
stringDelila aline;
{
  long i = 1;

  while (aline.letters[i-1] == ' ' || aline.letters[i-1] == '\t') {
    putchar(aline.letters[i-1]);
    i++;
  }
}


Static Void copystring(a, b)
stringDelila a, *b;
{
  /*
*/
  long l;

  b->length = a.length;
  for (l = 0; l < a.length; l++)
    b->letters[l] = a.letters[l];
  b->current = a.current;
  b->next = a.next;
}


#define escape          '\\'



Static Void themain(fin, htmlinkp, list)
_TEXT *fin, *htmlinkp, *list;
{
  stringDelila ARCHIVE, MANUAL, aline;
  long beforedot;
  Char c;
  boolean comment = false;
  boolean insidecomment;
  /*
*/
  Char cprevious;
  boolean done;
  long l, fontsize_;
  boolean gotten;
  long follow, i;
  boolean image;
  boolean linkprocess = false;
  long seealsocount = 0;
  /*
*/
  boolean slashdone;
  long slash;
  boolean potentiate = false;
  /*



*/
  boolean pureurl;

  trigger seealso, author, http, https, ftp, gif, jpg;

  trigger width;
  stringDelila imagecontrol;
  boolean suppress = false;

  long numbers[3];
  _TEXT TEMP;




  filltrigger(&seealso, "see also                                          ");
  filltrigger(&author, "author                                            ");
  filltrigger(&http, "http://                                           ");
  filltrigger(&https, "https://                                          ");
  filltrigger(&ftp, "ftp://                                            ");
  filltrigger(&gif, ".gif                                              ");
  filltrigger(&jpg, ".jpg                                              ");
  filltrigger(&width, "width                                             ");

  if (*htmlinkp->name != '\0') {
    if (htmlinkp->f != NULL)
      htmlinkp->f = freopen(htmlinkp->name, "r", htmlinkp->f);
    else
      htmlinkp->f = fopen(htmlinkp->name, "r");
  } else
    rewind(htmlinkp->f);
  if (htmlinkp->f == NULL)
    _EscIO2(FileNotFound, htmlinkp->name);
  RESETBUF(htmlinkp->f, Char);
  if (BUFEOF(htmlinkp->f)) {
    printf("htmlinkp is empty\n");
    halt();
  }

  getstring(htmlinkp, &ARCHIVE, &gotten);
  if (!gotten) {
    printf("Could not get ARCHIVE pointer\n");
    halt();
  }

  P_addset(P_expset(numbers, 0L), '0');
  P_addset(numbers, '1');
  P_addset(numbers, '2');
  P_addset(numbers, '3');
  P_addset(numbers, '4');
  P_addset(numbers, '5');
  P_addset(numbers, '6');
  P_addset(numbers, '7');
  P_addset(numbers, '8');
  P_addset(numbers, '9');
  if (P_inset(P_peek(htmlinkp->f), numbers)) {
    printf("BAD PARAMETERS:  ");
    printf("htmlinkp expects 3 parameters now; see the manual\n");
    halt();
  }

  getstring(htmlinkp, &MANUAL, &gotten);
  if (!gotten) {
    printf("Could not get MANUAL pointer in htmlinkp\n");
    halt();
  }

  if (BUFEOF(htmlinkp->f)) {
    printf("missing fontsize parameter in htmlinkp\n");
    halt();
  }

  fscanf(htmlinkp->f, "%ld%*[^\n]", &fontsize_);
  getc(htmlinkp->f);

  if (*list->name != '\0') {
    if (list->f != NULL)
      list->f = freopen(list->name, "w", list->f);
    else
      list->f = fopen(list->name, "w");
  } else {
    if (list->f != NULL)
      rewind(list->f);
    else
      list->f = tmpfile();
  }
  if (list->f == NULL)
    _EscIO2(FileNotFound, list->name);
  SETUPBUF(list->f, Char);
  printf("<!-- htmlink %4.2f -->\n", version);
  printf("<font size = %ld>\n", fontsize_);
  printf("<strong>\n");
  printf("<PRE>\n");

  clearstring(&aline);
  clearstring(&imagecontrol);
  while (!BUFEOF(fin->f)) {
    getstring(fin, &aline, &gotten);
    if (!gotten) {
      printf("htmlink: program error?\n");
      halt();
    }

    resettrigger(&seealso);
    resettrigger(&author);
    for (l = 1; l <= aline.length; l++) {
      c = aline.letters[l-1];

      testfortrigger(c, &seealso);
      if (l == 8) {
	if (seealso.found) {
	  seealsocount++;
	  if (seealsocount == 1)
	    potentiate = true;
	}
      }

      testfortrigger(c, &author);
      if (l == 6) {
	if (author.found) {
	  linkprocess = false;
	  potentiate = false;
	}
      }
    }

    if (linkprocess) {
      follow = 1;

      pureurl = false;
      c = ' ';
      suppress = false;
      while (follow <= aline.length) {
	/*

*/
	cprevious = c;
	c = aline.letters[follow-1];
	insidecomment = (comment == true);
	if (c == '{')
	  comment = true;
	if (cprevious != escape && c == '}')
	  comment = false;
	if (c == ' ')
	  putchar(' ');
	if (c == ' ')
	  putc(' ', list->f);
	if (insidecomment && cprevious == escape) {
	  /*

*/
	  if (c == '{')
	    putchar('{');
	  if (c == '{')
	    putc('{', list->f);
	  if (c == '}')
	    putchar('}');
	  if (c == '}')
	    putc('}', list->f);
	}

	if (c != ' ' && c != ',' && c != '{' && c != '}') {
	  resettrigger(&http);
	  resettrigger(&https);
	  resettrigger(&ftp);
	  resettrigger(&gif);
	  resettrigger(&jpg);
	  resettrigger(&width);
	  image = false;
	  l = follow;
	  /*

*/
	  done = false;
	  while (!done) {
	    testfortrigger(aline.letters[l-1], &http);
	    if (http.found)
	      pureurl = true;

	    testfortrigger(aline.letters[l-1], &https);
	    if (https.found)
	      pureurl = true;

	    testfortrigger(aline.letters[l-1], &ftp);
	    if (ftp.found)
	      pureurl = true;

	    testfortrigger(aline.letters[l-1], &gif);
	    if (gif.found)
	      image = true;

	    testfortrigger(aline.letters[l-1], &jpg);
	    if (jpg.found)
	      image = true;

	    testfortrigger(aline.letters[l-1], &width);
	    if (width.found) {
	      copystring(aline, &imagecontrol);
	      suppress = true;
	    }

	    /*

*/

	    if (l == aline.length) {
	      done = true;
	      break;
	    }
	    l++;
	    if (aline.letters[l-1] == ' ' || aline.letters[l-1] == ',' ||
		aline.letters[l-1] == '}' || aline.letters[l-1] == '{') {
	      done = true;
	      l--;
	    }
	  }
	  if (!suppress) {
	    if (comment) {
	      TEMP.f = stdout;
	      *TEMP.name = '\0';
	      writealine(&TEMP, aline, follow, l, escape);
	      writealine(list, aline, follow, l, escape);
	    } else {
	      if (aline.letters[l-2] == '.' && aline.letters[l-1] == 'p') {
		printf("<A HREF =\"");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writestring(&TEMP, &MANUAL);
		for (i = follow - 1; i <= l - 2; i++)
		  putchar(aline.letters[i]);
		printf("html\">");
		for (i = follow - 1; i < l; i++)
		  putchar(aline.letters[i]);
		printf("</A>");
		for (i = follow - 1; i < l; i++)
		  putc(aline.letters[i], list->f);
	      }

	      else {
		if (image) {
		  printf("<img\n");

		  if (imagecontrol.length > 0) {
		    TEMP.f = stdout;
		    *TEMP.name = '\0';
		    writestring(&TEMP, &imagecontrol);
		    clearstring(&imagecontrol);
		    putchar('\n');
		    suppress = false;
		  }
		  copyindentation(aline);
		  printf("src=\"");
		} else
		  printf("<a href=\"");

		if (!pureurl) {
		  TEMP.f = stdout;
		  *TEMP.name = '\0';
		  writestring(&TEMP, &ARCHIVE);
		}
		for (i = follow - 1; i < l; i++)
		  putchar(aline.letters[i]);
		putchar('"');
		if (image) {
		  putchar('\n');
		  copyindentation(aline);
		  printf("alt=\"");

		  slash = l;
		  beforedot = l;
		  slashdone = false;
		  while (!slashdone) {
		    if (aline.letters[slash-1] == '/')
		      slashdone = true;
		    else
		      slash--;

		    if (aline.letters[slash-1] == '.') {
		      if (beforedot == l)
			beforedot = slash - 1;
		    }
		    if (slash == follow) {
		      slashdone = true;

		      slash--;
		    }
		  }
		  printf("image for ");
		  for (i = slash; i < beforedot; i++)
		    putchar(aline.letters[i]);
		  putchar('"');
		}
		putchar('>');

		if (!image) {
		  for (i = follow - 1; i < l; i++)
		    putchar(aline.letters[i]);
		  printf("</A>");
		}


		for (i = follow - 1; i < l; i++) {
		  putc(aline.letters[i], list->f);

		}

	      }
	    }
	  }


	  follow = l;
	}

	if (c == ',')
	  putchar(',');
	follow++;

      }


      putchar('\n');
      putc('\n', list->f);
    }


    else {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      writestring(&TEMP, &aline);
      putchar('\n');
    }


    if (potentiate)
      linkprocess = true;

  }



  printf("</PRE>\n");
  printf("</strong>\n");
  printf("{<a href = \"htmlink.html\">created by htmlink %4.2f</a>}\n",
	 version);
}

#undef escape


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  list.f = NULL;
  strcpy(list.name, "list");
  htmlinkp.f = NULL;
  strcpy(htmlinkp.name, "htmlinkp");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  themain(&TEMP, &htmlinkp, &list);
_L1:
  if (htmlinkp.f != NULL)
    fclose(htmlinkp.f);
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
