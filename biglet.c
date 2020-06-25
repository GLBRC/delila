/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "biglet.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/


#define version         1.69
/*




*/



/*














































































































*/





#define letmaxhi        40
#define letmaxwi        40
#define pagemaxhi       2000
#define pagemaxwi       2000





typedef struct fontrec {
  Char letter;
  uchar image[letmaxhi][(letmaxwi + 7) / 8];
  /*
*/
  struct fontrec *next;
} fontrec;

typedef fontrec *loadtype[pagemaxwi];

/*
*/





Static _TEXT fin, font, bigletp, fout;


Static jmp_buf _JL1;





Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}





Static Void fontload(font, height, width, first)
_TEXT *font;
long *height, *width;
fontrec **first;
{
  long counthi, countwi;
  Char letterhold;
  /*
*/
  fontrec *p;
  long FORLIM, FORLIM1;

  if (*font->name != '\0') {
    if (font->f != NULL)
      font->f = freopen(font->name, "r", font->f);
    else
      font->f = fopen(font->name, "r");
  } else
    rewind(font->f);
  if (font->f == NULL)
    _EscIO2(FileNotFound, font->name);
  RESETBUF(font->f, Char);
  if (BUFEOF(font->f)) {
    printf(" font file is empty\n");
    halt();
  }

  fscanf(font->f, "%ld%ld%*[^\n]", height, width);
  getc(font->f);
  if (*height > letmaxhi) {
    printf(" font images are too tall for biglet use. you\n");
    printf(" should change biglet source code constant letmaxhi.\n");
    halt();
  }
  if (*width > letmaxwi) {
    printf(" font images are too wide for biglet use. you\n");
    printf(" should change biglet source code constant letmaxwi.\n");
    halt();
  }
  *first = (fontrec *)Malloc(sizeof(fontrec));
  p = *first;
  while (!BUFEOF(font->f)) {
    fscanf(font->f, "%c%*[^\n]", &p->letter);
    getc(font->f);
    if (p->letter == '\n')
      p->letter = ' ';
    FORLIM = *height;
    for (counthi = 0; counthi < FORLIM; counthi++) {
      FORLIM1 = *width;
      for (countwi = 0; countwi < FORLIM1; countwi++) {
	if (P_eoln(font->f)) {
	  printf(" either font of \"%c\" or previous\n", p->letter);
	  printf(" font has a line that is too short, is missing\n");
	  printf(" a line, or is missing a matching letter above\n");
	  printf(" the letter image. it is also possible that\n");
	  printf(" height and width parameters at the top of the\n");
	  printf(" font file are improperly set.\n");
	  halt();
	}
	letterhold = getc(font->f);
	if (letterhold == '\n')
	  letterhold = ' ';
	if (letterhold == ' ') {
	  printf(" font character image of \"%c\"\n", p->letter);
	  printf(" contains a space which will cause\n");
	  printf(" transportation errors.\n");
	  halt();
	}
	P_clrbits_B(p->image[counthi], countwi, 0, 3);
	P_putbits_UB(p->image[counthi], countwi, letterhold == p->letter, 0,
		     3);
      }
      /*
*/
      fscanf(font->f, "%*[^\n]");
      getc(font->f);
    }
    if (BUFEOF(font->f))
      p->next = NULL;
    else {
      p->next = (fontrec *)Malloc(sizeof(fontrec));
      p = p->next;
    }
  }
}





Static Void double_(first)
fontrec *first;
{
  fontrec *pt = first;
  /*
*/
  fontrec *pt2 = first;
  /*
*/
  boolean double_;
  long index = 1;
  long index2;

  /*
*/

  while (pt->next != NULL || pt2->next != NULL) {
    index2 = 1;
    pt = first;
    do {
      /*
*/
      /*

*/
      while (index2 <= index) {
	index2++;
	pt = pt->next;
      }
      double_ = (pt->letter == pt2->letter);
      if (double_) {
	printf(" font number %ld(%c)\n", index, pt2->letter);
	printf(" has a duplicate at font number %ld\n", index2);
	halt();
      }
      index2++;
      if (pt->next != NULL)
	pt = pt->next;
    } while (pt->next != NULL);
    pt2 = pt2->next;
    index++;
  }

}





Static Void paraload(bigletp, parahi, parawi, linechar, lctest)
_TEXT *bigletp;
long *parahi, *parawi;
Char *linechar;
boolean *lctest;
{
  /*
*/
  /*
*/
  /*
*/
  /*

*/
  Char quotehold;
  boolean done = false;

  *lctest = false;
  if (BUFEOF(bigletp->f))
    return;
  /*
*/
  fscanf(bigletp->f, "%ld%ld", parahi, parawi);
  if (P_eoln(bigletp->f)) {
    fscanf(bigletp->f, "%*[^\n]");
    getc(bigletp->f);
    return;
  }
  do {
    getc(bigletp->f);
    if (P_eoln(bigletp->f))
      done = true;
    else {
      if (P_peek(bigletp->f) == '"' || P_peek(bigletp->f) == '\'') {
	quotehold = getc(bigletp->f);
	if (P_eoln(bigletp->f)) {
	  printf(" bigletp optional letter missing\n");
	  halt();
	}
	*linechar = getc(bigletp->f);
	*lctest = true;
	/*
*/
	done = true;
	if (P_peek(bigletp->f) != quotehold) {
	  printf(" bigletp letter %c poorly quoted\n", *linechar);
	  halt();
	}
      } else if (P_peek(bigletp->f) != ' ') {
	printf(" quote marks required around bigletp\n");
	printf(" optional character\n");
	halt();
      }
    }
  } while (!done);
  fscanf(bigletp->f, "%*[^\n]");
  getc(bigletp->f);
}





Static Void textcheck(bigletp, fin, height, width)
_TEXT *bigletp, *fin;
long height, width;
{
  /*
*/
  long countchar;
  Char charhold;
  long parahi = 1, parawi = 1, totalhi = 0;

  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  RESETBUF(fin->f, Char);
  if (BUFEOF(fin->f)) {
    printf(" no fin, no fout\n");
    halt();
  }
  if (*bigletp->name != '\0') {
    if (bigletp->f != NULL)
      bigletp->f = freopen(bigletp->name, "r", bigletp->f);
    else
      bigletp->f = fopen(bigletp->name, "r");
  } else
    rewind(bigletp->f);
  if (bigletp->f == NULL)
    _EscIO2(FileNotFound, bigletp->name);
  RESETBUF(bigletp->f, Char);
  do {
    if (!BUFEOF(bigletp->f)) {
      fscanf(bigletp->f, "%ld%ld%*[^\n]", &parahi, &parawi);
      getc(bigletp->f);
    }
    if (parahi <= 0 || parawi <= 0) {
      printf(" expansion parameters of 0 or less\n");
      printf(" are not permitted\n");
      halt();
    }
    countchar = 0;
    do {
      countchar++;
      charhold = getc(fin->f);
      if (charhold == '\n')
	charhold = ' ';
    } while (!P_eoln(fin->f));
    fscanf(fin->f, "%*[^\n]");

    getc(fin->f);
    totalhi += height * parahi;
    if (totalhi > pagemaxhi) {
      printf(" text is too tall for page. you may want to\n");
      printf(" alter constant pagemaxhi(%ld lines)\n", (long)pagemaxhi);
      printf(" in biglet source code.\n");
      halt();
    }
    if (parawi * countchar * width > pagemaxwi) {
      printf(" text is too wide for page. you may want to\n");
      printf(" alter constant pagemaxwi(%ld", (long)pagemaxwi);
      printf(" characters)\n");
      printf(" in biglet source code.\n");
      halt();
    }
  } while (!BUFEOF(fin->f));
}





Static Void charptr(charhold, first, pt)
Char charhold;
fontrec *first, **pt;
{
  /*
*/
  /*
*/
  *pt = first;
  while ((*pt)->next != NULL && (*pt)->letter != charhold)
    *pt = (*pt)->next;

  if ((*pt)->letter != charhold) {
    printf(" font not found for character '%c'\n", charhold);
    halt();
  }
}





Static Void lineload(fin, textcount, loadl, first)
_TEXT *fin;
long *textcount;
fontrec **loadl;
fontrec *first;
{
  /*
*/
  /*
*/
  /*
*/
  Char lethold;

  /*
*/

  *textcount = 0;
  do {
    (*textcount)++;
    lethold = getc(fin->f);
    if (lethold == '\n')
      lethold = ' ';
    charptr(lethold, first, &loadl[*textcount - 1]);

  } while (!P_eoln(fin->f));
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
}





Static Void makebig(fout, textcount, height, width, parahi, parawi, linechar,
		    lctest, loadl, first)
_TEXT *fout;
long textcount, height, width, parahi, parawi;
Char linechar;
boolean lctest;
fontrec **loadl;
fontrec *first;
{
  /*
*/
  /*
*/
  /*
*/
  /*
*/
  long countph, countpw, countext, counthi, countwi;
  Char optionlet;
  /*
*/
  Char ch;
  fontrec *pt;

  for (counthi = 0; counthi < height; counthi++) {
    /*
*/
    for (countph = 1; countph <= parahi; countph++) {
      for (countext = 0; countext < textcount; countext++) {
	pt = loadl[countext];
	if (lctest == false)
	  optionlet = pt->letter;
	else
	  optionlet = linechar;

	for (countwi = 0; countwi < width; countwi++) {
	  /*

*/
	  switch (P_getbits_UB(pt->image[counthi], countwi, 0, 3)) {

	  case true:
	    ch = optionlet;
	    break;

	  case false:
	    ch = ' ';
	    break;
	  }
	  for (countpw = 1; countpw <= parawi; countpw++)
	    putc(ch, fout->f);
	}
      }
      putc('\n', fout->f);
    }
  }
}





Static Void bletorder(fin, font, bigletp, fout)
_TEXT *fin, *font, *bigletp, *fout;
{
  /*
*/
  long height, width;
  long parahi = 1, parawi = 1;
  long textcount;
  loadtype loadl;
  Char linechar;
  boolean lctest;
  fontrec *first;

  fontload(font, &height, &width, &first);
  double_(first);
  textcheck(bigletp, fin, height, width);
  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);
  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  RESETBUF(fin->f, Char);
  if (*bigletp->name != '\0') {
    if (bigletp->f != NULL)
      bigletp->f = freopen(bigletp->name, "r", bigletp->f);
    else
      bigletp->f = fopen(bigletp->name, "r");
  } else
    rewind(bigletp->f);
  if (bigletp->f == NULL)
    _EscIO2(FileNotFound, bigletp->name);
  RESETBUF(bigletp->f, Char);
  do {
    paraload(bigletp, &parahi, &parawi, &linechar, &lctest);
    lineload(fin, &textcount, loadl, first);
    makebig(fout, textcount, height, width, parahi, parawi, linechar, lctest,
	    loadl, first);
  } while (!BUFEOF(fin->f));
}




main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fout.f = NULL;
  strcpy(fout.name, "fout");
  bigletp.f = NULL;
  strcpy(bigletp.name, "bigletp");
  font.f = NULL;
  strcpy(font.name, "font");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  printf(" biglet %4.2f\n", version);
  bletorder(&fin, &font, &bigletp, &fout);
  printf(" text has been enlarged\n");
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (font.f != NULL)
    fclose(font.f);
  if (bigletp.f != NULL)
    fclose(bigletp.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
