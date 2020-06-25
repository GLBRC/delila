/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "sortbibtex.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         2.29
/*







*/
#define updateversion   2.25



/*


































































*/



#define mapmax          200000L
/*
*/

#define linewidth       80


/*


*/

typedef long position;

/*
*/

/*

*/

typedef struct entryline {
  Char stringDelila[linewidth];
  long stringlength;
  struct entryline *next;
} entryline;

/*

*/

typedef struct entry_ {
  Char key[linewidth];
  long yearkey, yearentry;
  entryline *line;
  long number;
} entry_;


Static _TEXT fin, sortbibtexp, fout;


/*

*/
Static entry_ *map[mapmax];
Static Char sortcontrol;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Static Void writekey(f, e)
_TEXT *f;
entry_ *e;
{
  long k = 1;

  while (e->key[k-1] != ' ') {
    putc(e->key[k-1], f->f);
    k++;
  }
}


Static boolean lessthan(alow, blow)
position alow, blow;
{
  /*
*/
  boolean Result;
  Char a, b;
  boolean done;
  long k;
  Char sortit;

  if (sortcontrol == 'k')
    sortit = sortcontrol;
  else {
    if (map[alow-1]->yearentry == map[blow-1]->yearentry ||
	map[alow-1]->yearentry == 0 || map[blow-1]->yearentry == 0)
      sortit = 'k';
    else
      sortit = sortcontrol;

    if (map[alow-1]->yearentry == 0 && map[blow-1]->yearentry != 0)
      sortit = 'a';

    if (map[blow-1]->yearentry == 0 && map[alow-1]->yearentry != 0)
      sortit = 'b';
  }
  /*
*/


  /*


*/

  /*









*/

  switch (sortit) {

  case 'a':
    Result = true;
    break;

  case 'b':
    Result = false;
    break;

  case 'y':
    if (map[alow-1]->yearentry < map[blow-1]->yearentry)
      Result = true;
    else
      Result = false;
    break;

  case 'r':
    if (map[alow-1]->yearentry < map[blow-1]->yearentry)
      Result = false;
    else
      Result = true;
    break;

  case 'k':
    done = false;
    k = 0;
    /*




*/
    while (!done) {
      k++;
      a = map[alow-1]->key[k-1];
      b = map[blow-1]->key[k-1];

      /*

*/

      if (a == ' ' && b == ' ') {
	/*


*/
	done = true;
	Result = false;
      }

      if (done)
	break;
      if (a == ' ' || b == ' ') {
	done = true;
	if (a == ' ')
	  Result = true;
	else
	  Result = false;
	continue;
      }

      if (a < b) {
	/*




*/
	Result = true;
	done = true;
      }

      if (a > b) {
	/*




*/
	Result = false;
	done = true;
      }

    }
    break;
  }
  return Result;
}


Static Void swap_(a, b)
position a, b;
{
  entry_ *hold;

  hold = map[a-1];
  map[a-1] = map[b-1];
  map[b-1] = hold;
}



Static Void quicksort(left, right)
position left, right;
{
  /*












*/
  position lower = left;
  position upper, center;

  center = (left + right) / 2;
  upper = right;

  do {
    while (lessthan(lower, center))
      lower++;
    while (lessthan(center, upper))
      upper--;

    if (lower <= upper) {
      if (lower == center)
	center = upper;
      else if (upper == center)
	center = lower;
      swap_(lower, upper);
      lower++;
      upper--;
    }

  } while (lower <= upper);

  if (left < upper)
    quicksort(left, upper);
  if (lower < right)
    quicksort(lower, right);
}



Static Void readline(f, l, linenumber)
_TEXT *f;
entryline **l;
long linenumber;
{
  Char c;
  entryline *WITH;

  /*
*/
  *l = (entryline *)Malloc(sizeof(entryline));
  WITH = *l;
  WITH->stringlength = 0;
  while (!P_eoln(f->f) && WITH->stringlength < linewidth) {
    WITH->stringlength++;
    /*

*/
    c = getc(f->f);
    if (c == '\n')
      c = ' ';
    (*l)->stringDelila[WITH->stringlength - 1] = c;
    if ((WITH->stringlength == linewidth) & (!P_eoln(f->f)))
      printf("line %ld is longer than %ld characters.  Make it two lines.\n",
	     linenumber, (long)linewidth);
  }
  fscanf(f->f, "%*[^\n]");
  getc(f->f);
  WITH->next = NULL;
}


Static Void readentry(f, linenumber, e)
_TEXT *f;
long *linenumber;
entry_ **e;
{
  /*
*/
  Char c;
  entryline *l;
  long i;
  long numberlength = 0;
  long numbers[3];
  long p = 1;
  long pkey;
  entry_ *WITH;
  entryline *WITH1;
  _TEXT TEMP;

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
  *e = (entry_ *)Malloc(sizeof(entry_));
  WITH = *e;



  while ((!BUFEOF(f->f)) & (P_peek(f->f) != '@')) {
    fscanf(f->f, "%*[^\n]");
    getc(f->f);
    (*linenumber)++;
  }

  if (BUFEOF(f->f)) {
    Free(*e);
    *e = NULL;
    /*
*/
    return;
  }

  readline(f, &WITH->line, *linenumber);
  (*linenumber)++;


  while (WITH->line->stringDelila[p-1] != '{') {
    if (p == linewidth) {
      printf("line %ld is missing the \"{\"\n", *linenumber);
      halt();
    }
    p++;
  }
  p++;
  pkey = p;


  WITH->yearkey = 0;
  while (WITH->line->stringDelila[p-1] != ',') {
    if (p == linewidth) {
      printf("line %ld is missing the \",\"\n", *linenumber);
      halt();
    }
    c = WITH->line->stringDelila[p-1];
    WITH->key[p - pkey] = c;


    if (P_inset(c, numbers)) {
      WITH->yearkey = WITH->yearkey * 10 + c - '0';
      numberlength++;
    } else
      WITH->yearkey = 0;


    p++;
  }

  if (numberlength < 4)
    WITH->yearkey = 0;


  WITH->key[p - pkey] = ' ';

  /*



*/


  l = WITH->line;
  while ((!BUFEOF(f->f)) & (!P_eoln(f->f))) {
    (*linenumber)++;
    readline(f, &l->next, *linenumber);
    if (!BUFEOF(f->f))
      l = l->next;
  }


  l = WITH->line;
  WITH->yearentry = 0;

  while (l != NULL) {
    WITH1 = l;
    i = 1;

    while (i < WITH1->stringlength && WITH1->stringDelila[i-1] == ' ')
      i++;

    if (WITH1->stringDelila[i-1] == 'y') {
      if (WITH1->stringDelila[i] == 'e') {
	if (WITH1->stringDelila[i+1] == 'a') {
	  if (WITH1->stringDelila[i+2] == 'r') {
	    i += 4;
	    while (i < WITH1->stringlength && WITH1->stringDelila[i-1] == ' ')
	      i++;
	    if (WITH1->stringDelila[i-1] == '=') {
	      i++;
	      while (i < WITH1->stringlength &&
		     WITH1->stringDelila[i-1] == ' ')
		i++;
	      if (WITH1->stringDelila[i-1] == '"')
		i++;
	      numberlength = 0;
	      while (P_inset(WITH1->stringDelila[i-1], numbers) &&
		     numberlength <= 4) {
		c = WITH1->stringDelila[i-1];
		WITH->yearentry = WITH->yearentry * 10 + c - '0';
		numberlength++;
		i++;
	      }
	      if (numberlength != 4) {
		printf("bad year found in:\n");
		TEMP.f = stdout;
		*TEMP.name = '\0';
		writekey(&TEMP, *e);
		putchar('\n');
	      }
	    }
	  }

	}
      }
    }

    l = l->next;
  }
  /*

*/
  if (WITH->yearentry == WITH->yearkey || WITH->yearkey == 0)
    return;

  printf("year in entry not equal to year in key in:\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writekey(&TEMP, *e);
  putchar('\n');
}


Static Void writeentry(f, e)
_TEXT *f;
entry_ *e;
{
  entryline *l;
  long p;
  entryline *WITH;
  long FORLIM;

  l = e->line;
  while (l != NULL) {
    WITH = l;
    FORLIM = WITH->stringlength;
    for (p = 0; p < FORLIM; p++)
      putc(WITH->stringDelila[p], f->f);
    putc('\n', f->f);
    l = l->next;
  }
}


Static Void showentries(afile, entries)
_TEXT *afile;
long entries;
{
  position e;
  entry_ *WITH;

  for (e = 0; e <= entries - 1; e++) {
    WITH = map[e];
    if (WITH->yearentry != 0)
      fprintf(afile->f, "%4ld ", WITH->yearentry);
    else
      fprintf(afile->f, "%5c", ' ');
    writekey(afile, map[e]);
    putc('\n', afile->f);
  }
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *fout;
  long entries;
  position e;
} ;

/*
*/
Local Void giveentry(LINK)
struct LOC_themain *LINK;
{
  writeentry(LINK->fout, map[LINK->e-1]);
  if (LINK->e != LINK->entries)
    putc('\n', LINK->fout->f);
}



Static Void themain(fin, fout_)
_TEXT *fin, *fout_;
{
  struct LOC_themain V;
  long linenumber = 0;
  Char numbered;
  double parameterversion;
  Char removeduplicates;
  _TEXT TEMP;
  position FORLIM;

  /*







*/
  V.fout = fout_;
  printf("sortbibtex %4.2f\n", version);

  if (*sortbibtexp.name != '\0') {
    if (sortbibtexp.f != NULL)
      sortbibtexp.f = freopen(sortbibtexp.name, "r", sortbibtexp.f);
    else
      sortbibtexp.f = fopen(sortbibtexp.name, "r");
  } else
    rewind(sortbibtexp.f);
  if (sortbibtexp.f == NULL)
    _EscIO2(FileNotFound, sortbibtexp.name);
  RESETBUF(sortbibtexp.f, Char);
  fscanf(sortbibtexp.f, "%lg%*[^\n]", &parameterversion);
  getc(sortbibtexp.f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }
  fscanf(sortbibtexp.f, "%c%*[^\n]", &sortcontrol);
  getc(sortbibtexp.f);
  if (sortcontrol == '\n')
    sortcontrol = ' ';
  if (sortcontrol != 'r' && sortcontrol != 'y' && sortcontrol != 'k') {
    printf("sortcontrol must be one of k, y, r\n");
    halt();
  }
  fscanf(sortbibtexp.f, "%c%*[^\n]", &numbered);
  getc(sortbibtexp.f);
  if (numbered == '\n')
    numbered = ' ';
  fscanf(sortbibtexp.f, "%c%*[^\n]", &removeduplicates);
  getc(sortbibtexp.f);

  if (removeduplicates == '\n')
    removeduplicates = ' ';
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
  if (*V.fout->name != '\0') {
    if (V.fout->f != NULL)
      V.fout->f = freopen(V.fout->name, "w", V.fout->f);
    else
      V.fout->f = fopen(V.fout->name, "w");
  } else {
    if (V.fout->f != NULL)
      rewind(V.fout->f);
    else
      V.fout->f = tmpfile();
  }
  if (V.fout->f == NULL)
    _EscIO2(FileNotFound, V.fout->name);
  SETUPBUF(V.fout->f, Char);

  V.entries = 0;
  while (!BUFEOF(fin->f)) {
    V.entries++;
    readentry(fin, &linenumber, &map[V.entries-1]);
    if (map[V.entries-1] == NULL)
      V.entries--;
  }

  printf("%ld entries read\n", V.entries);

  quicksort(1L, V.entries);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  showentries(&TEMP, V.entries);

  /*









*/


  FORLIM = V.entries;
  for (V.e = 1; V.e <= FORLIM; V.e++) {
    if (numbered == 'n') {
      if (sortcontrol == 'r')
	fprintf(V.fout->f, "%% %ld\n", V.entries - V.e + 1);
      else
	fprintf(V.fout->f, "%% %ld\n", V.e);
    }

    if (V.e > 1) {
      if (removeduplicates == 'r') {
	if ((!lessthan(V.e, V.e - 1)) & (!lessthan(V.e - 1, V.e))) {
	  printf("duplicate entry: ");
	  TEMP.f = stdout;
	  *TEMP.name = '\0';
	  writekey(&TEMP, map[V.e-1]);
	  putchar('\n');
	} else
	  giveentry(&V);
      } else
	giveentry(&V);
    } else
      giveentry(&V);
  }

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
  sortbibtexp.f = NULL;
  strcpy(sortbibtexp.name, "sortbibtexp");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &fout);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (sortbibtexp.f != NULL)
    fclose(sortbibtexp.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
