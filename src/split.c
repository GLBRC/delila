/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "split.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         3.54
/*




*/



/*
























































*/


/*

*/



#define defaultlinesperpage  54
#define defaultcolumnsperpage  80

#define defaultpageprompting  false

#define defaultheaderlines  0

#define pagecharacter   '\f'

#define quotemark       '"'


typedef struct pbr {
  _TEXT fi;
  struct pbr *next;
} pbr;


Static _TEXT fin, fout, splitp;

Static long linesperpage, columnsperpage;

Static long maximumcolumns, pagecolumn;

Static long currentline;

Static pbr *pages, *apage;

Static long pagex, pagey;

Static boolean pageprompting;
/*

*/
Static long chardup;
/*
*/

Static long headerlines;


Static jmp_buf _JL1;


/*
*/




Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



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



Static long copylines(fin, fout, n)
_TEXT *fin, *fout;
long n;
{
  /*
*/
  long index = 0;

  while (!BUFEOF(fin->f) && index < n) {
    copyaline(fin, fout);
    index++;
  }

  return index;
}





Static Void initialize()
{
  printf(" split %4.2f\n", version);
  if (*fout.name != '\0') {
    if (fout.f != NULL)
      fout.f = freopen(fout.name, "w", fout.f);
    else
      fout.f = fopen(fout.name, "w");
  } else {
    if (fout.f != NULL)
      rewind(fout.f);
    else
      fout.f = tmpfile();
  }
  if (fout.f == NULL)
    _EscIO2(FileNotFound, fout.name);
  SETUPBUF(fout.f, Char);
  fprintf(fout.f, " split %4.2f\n", version);
}


Static Void readpageparameters()
{
  if (*splitp.name != '\0') {
    if (splitp.f != NULL)
      splitp.f = freopen(splitp.name, "r", splitp.f);
    else
      splitp.f = fopen(splitp.name, "r");
  } else
    rewind(splitp.f);
  if (splitp.f == NULL)
    _EscIO2(FileNotFound, splitp.name);
  RESETBUF(splitp.f, Char);

  if (BUFEOF(splitp.f)) {
    pageprompting = defaultpageprompting;
    linesperpage = defaultlinesperpage;
    columnsperpage = defaultcolumnsperpage;
    headerlines = defaultheaderlines;
  } else {
    if (P_peek(splitp.f) == 'p')
      pageprompting = true;
    else
      pageprompting = false;
    fscanf(splitp.f, "%*[^\n]");

    getc(splitp.f);
    if (BUFEOF(splitp.f)) {
      printf(" missing second parameter.\n");
      halt();
    }
    fscanf(splitp.f, "%ld%*[^\n]", &linesperpage);
    getc(splitp.f);

    if (BUFEOF(splitp.f)) {
      printf(" missing third parameter.\n");
      halt();
    }
    fscanf(splitp.f, "%ld%*[^\n]", &columnsperpage);
    getc(splitp.f);

    if (!BUFEOF(splitp.f)) {
      fscanf(splitp.f, "%ld%*[^\n]", &headerlines);
      getc(splitp.f);
    } else
      headerlines = defaultheaderlines;
  }

  if (columnsperpage < 1) {
    printf(" columns per page must be >= 1.\n");
    halt();
  }


  putc('\n', fout.f);
  if (pageprompting) {
    chardup = linesperpage;
    if (chardup < 0)
      chardup = 0;
    fprintf(fout.f, " pages prompted from input.\n");
    fprintf(fout.f, " %ld character(s) duplicated on each line.\n", chardup);
  } else {
    chardup = 0;
    fprintf(fout.f, " not prompting pages from input.\n");
    fprintf(fout.f, " %ld lines per page\n", linesperpage);
  }
  fprintf(fout.f, " %ld columns per page\n", columnsperpage);
  fprintf(fout.f, " %ld fourth parameter (requested header lines)\n",
	  headerlines);
}


#define maxstring       50


typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;


/*




*/

Local Void resettrigger(t)
trigger *t;
{
  t->state = 0;
  t->skip = false;
  t->found = false;
}

Local Void testfortrigger(ch, t)
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
  t->state = 0;
  t->skip = true;
  t->found = false;
}


Static Void findwidthheader(afile, headerp, report, maximumcolumns,
			    headerlines)
_TEXT *afile, *headerp, *report;
long *maximumcolumns, *headerlines;
{
  /*

*/
  long currentcolumn;
  long linenumber = 1;
  trigger headertrigger;
  /*
*/
  long skip, FORLIM;


  if (*headerlines < 0) {
    fprintf(report->f, " trigger phrase: ");

    while (P_peek(headerp->f) != quotemark) {
      if (BUFEOF(headerp->f)) {
	printf(" trigger phrase not found in quotes (%c)\n", quotemark);
	halt();
      }
      if (P_eoln(headerp->f)) {
	printf(" trigger phrase missing\n");
	halt();
      }
      getc(headerp->f);
    }
    putc(P_peek(headerp->f), report->f);
    getc(headerp->f);

    resettrigger(&headertrigger);
    headertrigger.seek.length = 0;
    while (P_peek(headerp->f) != quotemark) {
      if (BUFEOF(headerp->f) | P_eoln(headerp->f)) {
	printf(" end of trigger not found\n");
	halt();
      }

      headertrigger.seek.length++;
      if (headertrigger.seek.length > maxstring) {
	printf(" trigger exceeds %ld characters\n", (long)maxstring);
	halt();
      }
      headertrigger.seek.letters[headertrigger.seek.length - 1] = P_peek(headerp->f);
      putc(P_peek(headerp->f), report->f);
      getc(headerp->f);
    }
    fprintf(report->f, "%c\n", P_peek(headerp->f));
  }


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
  *maximumcolumns = 0;
  while (!BUFEOF(afile->f)) {
    currentcolumn = 0;
    while (!P_eoln(afile->f)) {
      if (*headerlines < 0) {
	testfortrigger(P_peek(afile->f), &headertrigger);
	if (headertrigger.found) {
	  FORLIM = -*headerlines;
	  for (skip = 1; skip <= FORLIM; skip++) {
	    if (BUFEOF(afile->f)) {
	      printf(" the entire file was a header\n");
	      halt();
	    }
	    fscanf(afile->f, "%*[^\n]");
	    getc(afile->f);
	    linenumber++;
	  }
	  *maximumcolumns = 0;
	  currentcolumn = 0;


	  *headerlines = linenumber - 1;
	}

      }
      currentcolumn++;
      getc(afile->f);
    }
    fscanf(afile->f, "%*[^\n]");
    getc(afile->f);
    linenumber++;
    if (currentcolumn > *maximumcolumns)
      *maximumcolumns = currentcolumn;
  }

  if (*headerlines < 0) {
    printf(" trigger phrase not found in fin\n");
    halt();
  }

  fprintf(report->f, "\n actual number of header lines: %ld\n\n",
	  *headerlines);
  fprintf(report->f, " input file width: %ld characters\n", *maximumcolumns);

}

#undef maxstring


Static Void allocatepagebuffers()
{
  long currentcolumn = 0, totalpagesacross = 1;

  pagecolumn = 0;
  apage = (pbr *)Malloc(sizeof(pbr));
  apage->fi.f = NULL;
  *apage->fi.name = '\0';
  pages = apage;

  while (currentcolumn < maximumcolumns) {
    if (pagecolumn == columnsperpage) {
      apage->next = (pbr *)Malloc(sizeof(pbr));
      apage->next->fi.f = NULL;
      *apage->next->fi.name = '\0';
      totalpagesacross++;
      apage = apage->next;
      pagecolumn = 0;
    }
    currentcolumn++;
    pagecolumn++;
  }
  apage->next = NULL;

  fprintf(fout.f, "\n total pages across: %ld\n\n", totalpagesacross);
}


Static Void coordinate(afile)
_TEXT *afile;
{
  fprintf(afile->f, " (%3ld,%3ld)", pagex, pagey);
}


Static Void newpage()
{
  /*
*/
  putc(' ', fout.f);
  pagex++;
  pagey = -1;
  apage = pages;
  currentline = 0;

  while (apage != NULL) {
    pagey++;
    putc(pagecharacter, apage->fi.f);
    coordinate(&apage->fi);
    coordinate(&fout);
    putc('\n', apage->fi.f);
    apage = apage->next;
  }
  putc('\n', fout.f);
}


Static Void breakfinintobuffers()
{
  long skip, duplicated, FORLIM;

  /*
*/

  if (*fin.name != '\0') {
    if (fin.f != NULL)
      fin.f = freopen(fin.name, "r", fin.f);
    else
      fin.f = fopen(fin.name, "r");
  } else
    rewind(fin.f);
  if (fin.f == NULL)
    _EscIO2(FileNotFound, fin.name);
  RESETBUF(fin.f, Char);
  FORLIM = headerlines;
  for (skip = 1; skip <= FORLIM; skip++) {
    fscanf(fin.f, "%*[^\n]");
    getc(fin.f);
  }
  pagecolumn = 0;


  apage = pages;
  while (apage != NULL) {
    if (*apage->fi.name != '\0') {
      if (apage->fi.f != NULL)
	apage->fi.f = freopen(apage->fi.name, "w", apage->fi.f);
      else
	apage->fi.f = fopen(apage->fi.name, "w");
    } else {
      if (apage->fi.f != NULL)
	rewind(apage->fi.f);
      else
	apage->fi.f = tmpfile();
    }
    if (apage->fi.f == NULL)
      _EscIO2(FileNotFound, apage->fi.name);
    SETUPBUF(apage->fi.f, Char);
    apage = apage->next;
  }
  pagex = -1;


  if (pageprompting) {
    if (P_peek(fin.f) != pagecharacter)
      newpage();
  } else
    newpage();


  while (!BUFEOF(fin.f)) {
    if (pageprompting) {
      duplicated = 0;

      if (P_peek(fin.f) == pagecharacter) {
	newpage();
	getc(fin.f);
	apage = pages;
	while (apage != NULL) {
	  putc(' ', apage->fi.f);
	  apage = apage->next;
	}
	duplicated++;
      }


      while ((duplicated < chardup) & (!P_eoln(fin.f))) {
	apage = pages;
	while (apage != NULL) {
	  if (!P_eoln(fin.f))
	    putc(P_peek(fin.f), apage->fi.f);
	  apage = apage->next;
	}
	getc(fin.f);
	duplicated++;

      }
    } else {
      if (currentline >= linesperpage)
	newpage();



      apage = pages;
      while (apage != NULL) {
	putc(' ', apage->fi.f);
	apage = apage->next;
      }

    }



    apage = pages;
    while (!P_eoln(fin.f)) {
      if (pagecolumn == columnsperpage) {
	apage = apage->next;
	pagecolumn = 0;
      }
      pagecolumn++;
      putc(P_peek(fin.f), apage->fi.f);
      getc(fin.f);
    }


    apage = pages;
    while (apage != NULL) {
      putc('\n', apage->fi.f);
      apage = apage->next;
    }

    fscanf(fin.f, "%*[^\n]");
    getc(fin.f);
    pagecolumn = 0;
    currentline++;
  }
}


Static Void copybufferstofout()
{
  apage = pages;
  while (apage != NULL) {
    if (*apage->fi.name != '\0') {
      if (apage->fi.f != NULL)
	apage->fi.f = freopen(apage->fi.name, "r", apage->fi.f);
      else
	apage->fi.f = fopen(apage->fi.name, "r");
    } else
      rewind(apage->fi.f);
    if (apage->fi.f == NULL)
      _EscIO2(FileNotFound, apage->fi.name);
    RESETBUF(apage->fi.f, Char);
    while (!BUFEOF(apage->fi.f)) {
      if (!P_eoln(apage->fi.f))
	putc(P_peek(apage->fi.f), fout.f);
      while (!P_eoln(apage->fi.f)) {
	getc(apage->fi.f);
	putc(P_peek(apage->fi.f), fout.f);
      }
      fscanf(apage->fi.f, "%*[^\n]");
      getc(apage->fi.f);
      putc('\n', fout.f);
    }
    apage = apage->next;
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  splitp.f = NULL;
  strcpy(splitp.name, "splitp");
  fout.f = NULL;
  strcpy(fout.name, "fout");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  initialize();
  readpageparameters();
  findwidthheader(&fin, &splitp, &fout, &maximumcolumns, &headerlines);

  allocatepagebuffers();

  breakfinintobuffers();

  if (*fin.name != '\0') {
    if (fin.f != NULL)
      fin.f = freopen(fin.name, "r", fin.f);
    else
      fin.f = fopen(fin.name, "r");
  } else
    rewind(fin.f);
  if (fin.f == NULL)
    _EscIO2(FileNotFound, fin.name);
  RESETBUF(fin.f, Char);
  if (headerlines != 0)
    fprintf(fout.f, "\f");
  headerlines = copylines(&fin, &fout, headerlines);

  copybufferstofout();
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  if (splitp.f != NULL)
    fclose(splitp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
