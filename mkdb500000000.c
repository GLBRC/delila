/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "mkdb500000000.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.19
/*





*/
#define updateversion   1.09



/*
















































































































*/



#define maxnamelength   100

#define maxsequencelength  500000000L

#define debugging       false


Static _TEXT sequ, mkdbp, entries;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Char capitalize(c)
Char c;
{
  long n = c;

  if (n >= 'a' && n <= 'z')
    c = _toupper(n);

  return c;
}



Static Char decapitalize(c)
Char c;
{
  long n = c;

  if (n >= 'A' && n <= 'Z')
    c = _tolower(n);
  else
    c = (Char)n;

  return c;
}


typedef struct object {
  Char id[maxnamelength];
  long length;
} object;

typedef struct sequence {
  Char id[maxsequencelength];
  long length;
} sequence;


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *sequ, *entries;
  Char c;
  boolean die;
  long entrylength, exoncutoff;
  object name;
  long na, nc, ng, nt, nn;
  object organism;
  long position;
  sequence seq;
  long start, stop;
  boolean waslower;
} ;

Local Void writename(afile, name, LINK)
_TEXT *afile;
object name;
struct LOC_themain *LINK;
{
  long n;

  for (n = 0; n < name.length; n++)
    putc(name.id[n], afile->f);
}

Local Void readname(afile, name, LINK)
_TEXT *afile;
object *name;
struct LOC_themain *LINK;
{
  _TEXT TEMP;

  if ((P_peek(afile->f) == '>') | (P_peek(afile->f) == '*')) {
    getc(afile->f);
    while ((P_peek(afile->f) == ' ') & (!P_eoln(afile->f)))
      getc(afile->f);

    if (P_eoln(afile->f)) {
      printf("missing name\n");
      halt();
    }

    name->length = 0;
    while (!P_eoln(afile->f)) {
      name->length++;
      if (name->length > maxnamelength) {
	printf("name too long:\n");
	putchar('"');
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writename(&TEMP, *name, LINK);
	printf("\"\n");
	halt();
      }
      name->id[name->length - 1] = getc(afile->f);
      if (name->id[name->length - 1] == '\n')
	name->id[name->length - 1] = ' ';
    }
    fscanf(afile->f, "%*[^\n]");

    getc(afile->f);
    printf("READING: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writename(&TEMP, *name, LINK);
    putchar('\n');
    return;
  }
  printf("Assigned name: ");
  name->id[0] = 'n';
  name->id[1] = 'a';
  name->id[2] = 'm';
  name->id[3] = 'e';
  name->id[4] = 's';
  name->id[5] = 't';
  name->id[6] = 'e';
  name->length = 7;
  putchar('"');
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writename(&TEMP, *name, LINK);
  printf("\"\n");
  /*



*/
}

Local Void indent(afile, amount, LINK)
_TEXT *afile;
long amount;
struct LOC_themain *LINK;
{
  long n;

  for (n = 1; n <= amount; n++)
    putc(' ', afile->f);
}

Local Void featurecoordinate(afile, start, stop, entrylength, LINK)
_TEXT *afile;
long start, stop, entrylength;
struct LOC_themain *LINK;
{
  /*
*/
  if (start < 1)
    putc('<', afile->f);
  fprintf(afile->f, "%ld", start);
  fprintf(afile->f, "..");
  if (stop > entrylength)
    putc('>', afile->f);
  fprintf(afile->f, "%ld", stop);
}

Local Void countem(b, LINK)
Char b;
struct LOC_themain *LINK;
{
  switch (decapitalize(b)) {

  case 'a':
    LINK->na++;
    break;

  case 'c':
    LINK->nc++;
    break;

  case 'g':
    LINK->ng++;
    break;

  case 't':
    LINK->nt++;
    break;

  case 'n':
    LINK->nn++;
    break;
  }
}

Local Void skipcomments(LINK)
struct LOC_themain *LINK;
{
  if (BUFEOF(LINK->sequ->f))
    return;
  if ((P_peek(LINK->sequ->f) == '>') | (P_peek(LINK->sequ->f) == '*')) {
    fscanf(LINK->sequ->f, "%*[^\n]");
    getc(LINK->sequ->f);
  }
}

Local Void chooseandwrite(LINK)
struct LOC_themain *LINK;
{
  if (LINK->stop - LINK->start + 1 > LINK->exoncutoff) {
    indent(LINK->entries, 5L, LINK);
    fprintf(LINK->entries->f, "exon   ");
    featurecoordinate(LINK->entries, LINK->start, LINK->stop,
		      LINK->entrylength, LINK);
    putc('\n', LINK->entries->f);
    return;
  }
  indent(LINK->entries, 5L, LINK);
  fprintf(LINK->entries->f, "primer ");
  featurecoordinate(LINK->entries, LINK->start, LINK->stop, LINK->entrylength,
		    LINK);
  putc('\n', LINK->entries->f);
}

Local Void processsequence(LINK)
struct LOC_themain *LINK;
{
  Char TEMP;
  long FORLIM;


  LINK->position = 0;
  LINK->die = false;
  LINK->c = ' ';
  LINK->na = 0;
  LINK->nc = 0;
  LINK->ng = 0;
  LINK->nt = 0;
  LINK->nn = 0;
  /*

*/
  while (!BUFEOF(LINK->sequ->f) && LINK->c != '.') {
    if (P_eoln(LINK->sequ->f)) {
      fscanf(LINK->sequ->f, "%*[^\n]");

      getc(LINK->sequ->f);
      if (!BUFEOF(LINK->sequ->f)) {
	if ((P_peek(LINK->sequ->f) == '*') | (P_peek(LINK->sequ->f) == '>'))
	  LINK->c = '.';
      }
      if (debugging)
	putchar('\n');
      /*

*/
      continue;
    }
    LINK->c = getc(LINK->sequ->f);
    if (LINK->c == '\n')
      LINK->c = ' ';
    if (debugging)
      putchar(LINK->c);
    TEMP = decapitalize(LINK->c);
    if (TEMP == 'n' || TEMP == 't' || TEMP == 'g' || TEMP == 'c' ||
	TEMP == 'a') {
      LINK->position++;
      LINK->seq.id[LINK->position-1] = LINK->c;
      countem(LINK->c, LINK);
      continue;
    }
    if (LINK->c == '.') {
      fscanf(LINK->sequ->f, "%*[^\n]");
      getc(LINK->sequ->f);
    } else if (LINK->c != ' ' && LINK->c != '9' && LINK->c != '8' &&
	       LINK->c != '7' && LINK->c != '6' && LINK->c != '5' &&
	       LINK->c != '4' && LINK->c != '3' && LINK->c != '2' &&
	       LINK->c != '1' && LINK->c != '0') {
      printf("unidentified character: %c\n", LINK->c);
      LINK->die = true;
    }
  }
  LINK->entrylength = LINK->position;
  if (LINK->die)
    halt();


  fprintf(LINK->entries->f, "\nLOCUS ");
  writename(LINK->entries, LINK->name, LINK);
  fprintf(LINK->entries->f, " %ld bp  DNA", LINK->entrylength);
  fprintf(LINK->entries->f, " * mkdb %4.2f\n", version);
  fprintf(LINK->entries->f, "DEFINITION  ");
  writename(LINK->entries, LINK->organism, LINK);
  fprintf(LINK->entries->f, "\nACCESSION   ");
  writename(LINK->entries, LINK->name, LINK);
  fprintf(LINK->entries->f, "\nSOURCE   ");
  writename(LINK->entries, LINK->organism, LINK);
  fprintf(LINK->entries->f, "\n  ORGANISM   ");
  writename(LINK->entries, LINK->organism, LINK);
  fprintf(LINK->entries->f, "\nFEATURES\n");

  LINK->waslower = true;
  LINK->start = 1;
  LINK->stop = 1;
  FORLIM = LINK->entrylength;
  for (LINK->position = 1; LINK->position <= FORLIM; LINK->position++) {
    LINK->c = LINK->seq.id[LINK->position-1];
    if (LINK->c == 'n' || LINK->c == 't' || LINK->c == 'g' ||
	LINK->c == 'c' || LINK->c == 'a') {
      if (LINK->position != 1 && !LINK->waslower) {
	LINK->stop = LINK->position - 1;
	chooseandwrite(LINK);
      }
      LINK->waslower = true;
    } else if (LINK->c == 'N' || LINK->c == 'T' || LINK->c == 'G' ||
	       LINK->c == 'C' || LINK->c == 'A') {
      if (LINK->waslower)
	LINK->start = LINK->position;
      LINK->waslower = false;
    } else {
      printf("program error:\n");
      printf("illegal character \"%c\" found in sequence array\n", LINK->c);
      halt();
    }
  }

  if (!LINK->waslower) {
    /*
*/
    LINK->stop = LINK->entrylength;
    chooseandwrite(LINK);
  }

  fprintf(LINK->entries->f, "BASE COUNT %7ld a %7ld c %7ld g %7ld t %7ld n\n",
	  LINK->na, LINK->nc, LINK->ng, LINK->nt, LINK->nn);
  if (LINK->na + LINK->nc + LINK->ng + LINK->nt + LINK->nn != LINK->entrylength) {
    printf("program error: BASE COUNT <> sum of bases\n");
    halt();
  }
  fprintf(LINK->entries->f, "ORIGIN");


  FORLIM = LINK->entrylength;
  for (LINK->position = 1; LINK->position <= FORLIM; LINK->position++) {
    if (LINK->position % 10 == 1)
      putc(' ', LINK->entries->f);
    if ((LINK->position - 1) % 60 == 0)
      fprintf(LINK->entries->f, "\n%10ld ", LINK->position);
/* p2c: mkdb500000000.p, line 459:
 * Note: Using % for possibly-negative arguments [317] */
    fputc(decapitalize(LINK->seq.id[LINK->position-1]), LINK->entries->f);
  }

  fprintf(LINK->entries->f, "\n//\n");
}



Static Void themain(sequ_, mkdbp, entries_)
_TEXT *sequ_, *mkdbp, *entries_;
{
  struct LOC_themain V;
  double parameterversion;

  V.sequ = sequ_;
  V.entries = entries_;
  printf("mkdb %4.2f\n", version);
  if (*mkdbp->name != '\0') {
    if (mkdbp->f != NULL)
      mkdbp->f = freopen(mkdbp->name, "r", mkdbp->f);
    else
      mkdbp->f = fopen(mkdbp->name, "r");
  } else
    rewind(mkdbp->f);
  if (mkdbp->f == NULL)
    _EscIO2(FileNotFound, mkdbp->name);
  RESETBUF(mkdbp->f, Char);
  fscanf(mkdbp->f, "%lg%*[^\n]", &parameterversion);
  getc(mkdbp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }


  fscanf(mkdbp->f, "%ld%*[^\n]", &V.exoncutoff);
  getc(mkdbp->f);

  if (*V.entries->name != '\0') {
    if (V.entries->f != NULL)
      V.entries->f = freopen(V.entries->name, "w", V.entries->f);
    else
      V.entries->f = fopen(V.entries->name, "w");
  } else {
    if (V.entries->f != NULL)
      rewind(V.entries->f);
    else
      V.entries->f = tmpfile();
  }
  if (V.entries->f == NULL)
    _EscIO2(FileNotFound, V.entries->name);
  SETUPBUF(V.entries->f, Char);
  if (*V.sequ->name != '\0') {
    if (V.sequ->f != NULL)
      V.sequ->f = freopen(V.sequ->name, "r", V.sequ->f);
    else
      V.sequ->f = fopen(V.sequ->name, "r");
  } else
    rewind(V.sequ->f);
  if (V.sequ->f == NULL)
    _EscIO2(FileNotFound, V.sequ->name);
  RESETBUF(V.sequ->f, Char);
  while (!BUFEOF(V.sequ->f)) {
    /*


*/
    readname(V.sequ, &V.name, &V);

    if ((P_peek(V.sequ->f) == '>') | (P_peek(V.sequ->f) == '*'))
      readname(V.sequ, &V.organism, &V);
    else
      V.organism = V.name;

    processsequence(&V);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  entries.f = NULL;
  strcpy(entries.name, "entries");
  mkdbp.f = NULL;
  strcpy(mkdbp.name, "mkdbp");
  sequ.f = NULL;
  strcpy(sequ.name, "sequ");
  themain(&sequ, &mkdbp, &entries);
_L1:
  if (sequ.f != NULL)
    fclose(sequ.f);
  if (mkdbp.f != NULL)
    fclose(mkdbp.f);
  if (entries.f != NULL)
    fclose(entries.f);
  exit(EXIT_SUCCESS);
}



/* End. */
