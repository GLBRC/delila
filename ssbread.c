/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "ssbread.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.27
/*
*/



/*



































*/



#define maxseq          1000
#define maxname         30
#define maxfillname     10
#define quicksequence   25


typedef struct block {
  Char data[1500];
  long length;
} block;


typedef Char fillname[maxfillname];

typedef struct name {
  Char letters[maxname];
  long length;
} name;

typedef struct sequence {
  Char bases[maxseq];
  long length, lanenumber;
  name plasmidname, primername, date;
  long start, stop;
  boolean editdeleted;
  /*
*/
  struct sequence *next;
} sequence;

typedef struct locations {
  long spot;
  struct locations *next;
} locations;


Static _TEXT ssb, report;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Static Void readsegment(f, fromplace, toplace, b, l)
_TEXT *f;
long fromplace, toplace;
block *b;
long *l;
{
  /*


*/
  long j;

  if (*l > fromplace) {
    if (*f->name != '\0') {
      if (f->f != NULL)
	f->f = freopen(f->name, "r", f->f);
      else
	f->f = fopen(f->name, "r");
    } else
      rewind(f->f);
    if (f->f == NULL)
      _EscIO2(FileNotFound, f->name);
    RESETBUF(f->f, Char);
    *l = 1;
  }
  while (*l < fromplace) {
    getc(f->f);
    (*l)++;
  }
  for (j = fromplace; j <= toplace; j++) {
    b->data[j - fromplace] = getc(f->f);
    if (b->data[j - fromplace] == '\n')
      b->data[j - fromplace] = ' ';
  }
  b->length = toplace - fromplace + 1;
  b->data[b->length] = '\0';
  *l += b->length;
}


Static Void uptonull(f, b)
_TEXT *f;
block b;
{
  long j = 1;

  while (b.data[j-1] != 0 && j <= b.length) {
    putc(b.data[j-1], f->f);
    j++;
  }
}


Static Void skipnulluptonull(f, b)
_TEXT *f;
block b;
{
  long j = 1;
  boolean nonulls = true;

  while (b.data[j-1] == 0 && j <= b.length) {
    if (nonulls) {
      fprintf(f->f, "(NULLs found)");
      nonulls = false;
    }
    j++;
  }
  while (b.data[j-1] != 0 && j <= b.length) {
    putc(b.data[j-1], f->f);
    j++;
  }
}


Static Void grabandshow(f, t, fromplace, toplace, b, l)
_TEXT *f, *t;
long fromplace, toplace;
block *b;
long *l;
{
  /*

*/
  readsegment(f, fromplace, toplace, b, l);
  fprintf(t->f, "from %5ld", fromplace);
  fprintf(t->f, " to %5ld | ", toplace);
  putc('"', t->f);

  skipnulluptonull(t, *b);
  fprintf(t->f, "\"\n");
}




Static Void bar(f, n, c)
_TEXT *f;
long n;
Char c;
{
  long i;

  for (i = 1; i <= n; i++)
    putc(c, f->f);
}



Static Void clearname(n, f)
name *n;
Char *f;
{
  long i;

  for (i = 0; i < maxfillname; i++)
    n->letters[i] = f[i];
  n->length = maxfillname;
}



Static Void copyname(n1, n2)
name n1, *n2;
{
  long i;

  n2->length = n1.length;
  for (i = 0; i < n1.length; i++)
    n2->letters[i] = n1.letters[i];
}



Static Void clearseq(seq)
sequence *seq;
{
  seq->length = 0;
  seq->lanenumber = 0;
  clearname(&seq->plasmidname, "plasmid?  ");
  clearname(&seq->primername, "primer?   ");
  clearname(&seq->date, "date?     ");
  seq->start = 0;
  seq->stop = 0;
  seq->editdeleted = false;
  seq->next = NULL;
}




Static Void readname(f, n, location)
_TEXT *f;
name *n;
long *location;
{
  Char c;

  n->length = 0;

  /*

*/
  while (P_peek(f->f) == '/' || P_peek(f->f) == ' ') {
    getc(f->f);
    (*location)++;
  }
  /*





*/
  do {
    c = getc(f->f);
    if (c == '\n')
      c = ' ';
    (*location)++;
    if (c != '\0' && c != ' ' && c != '/') {
      n->length++;
      n->letters[n->length - 1] = c;
    }
  } while (!((c == '\0' || c == ' ' || c == '/') | P_eoln(f->f)));
}



Static Void writename(f, n)
_TEXT *f;
name n;
{
  long i;

  for (i = 0; i < n.length; i++)
    putc(n.letters[i], f->f);
}


#define lanebot         131

#define namebot         163

#define lanelength      586


/* Local variables for readssblane: */
struct LOC_readssblane {
  _TEXT *ssb;
  long lanenumber, *location;
} ;

Local long spot(a, LINK)
long a;
struct LOC_readssblane *LINK;
{
  return (a + lanelength * (LINK->lanenumber - 1));
}

Local Void tolocation(a, LINK)
long a;
struct LOC_readssblane *LINK;
{
  if (*LINK->location > a) {
    if (*LINK->ssb->name != '\0') {
      if (LINK->ssb->f != NULL)
	LINK->ssb->f = freopen(LINK->ssb->name, "r", LINK->ssb->f);
      else
	LINK->ssb->f = fopen(LINK->ssb->name, "r");
    } else
      rewind(LINK->ssb->f);
    if (LINK->ssb->f == NULL)
      _EscIO2(FileNotFound, LINK->ssb->name);
    RESETBUF(LINK->ssb->f, Char);
    *LINK->location = 1;
  }
  while (*LINK->location < a) {
    getc(LINK->ssb->f);
    (*LINK->location)++;
  }

}



Static Void readssblane(ssb_, lanenumber_, plasmidname, primername, location_)
_TEXT *ssb_;
long lanenumber_;
name *plasmidname, *primername;
long *location_;
{
  /*




*/
  struct LOC_readssblane V;
  long numbers[3];
  Char number1, number2;

  V.ssb = ssb_;
  V.lanenumber = lanenumber_;
  V.location = location_;
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
  tolocation(spot((long)lanebot, &V), &V);
  while (!P_inset(P_peek(V.ssb->f), numbers)) {
    getc(V.ssb->f);
    (*V.location)++;
  }
  number1 = getc(V.ssb->f);
  number2 = getc(V.ssb->f);
  if (number1 == '\n')
    number1 = ' ';
  if (number2 == '\n')
    number2 = ' ';
  *V.location += 2;
  if (!P_inset(number2, numbers)) {
    printf("readssblane: bad number: \"%c%c\"\n", number1, number2);
    printf("(ords: %d & %12d)\n", number1, number2);
    printf("for lane number %ld\n", V.lanenumber);
    halt();
  }

  if ((number1 - '0') * 10 + number2 - '0' != V.lanenumber) {
    printf("readssblane: trying to read lane number %ld but found %c%c\n",
	   V.lanenumber, number1, number2);
    halt();
  }

  tolocation(spot((long)namebot, &V), &V);

  if (P_peek(V.ssb->f) == 0) {
    plasmidname->length = 0;
    primername->length = 0;
  } else {
    readname(V.ssb, plasmidname, V.location);
    readname(V.ssb, primername, V.location);
  }
}

#undef lanebot
#undef namebot
#undef lanelength



Static Void readssb(ssb, now, data)
_TEXT *ssb;
name now;
sequence **data;
{
  sequence *d;
  long location = 1;
  long lane;

  clearseq(*data);
  d = *data;
  copyname(now, &d->date);
  if (*ssb->name != '\0') {
    if (ssb->f != NULL)
      ssb->f = freopen(ssb->name, "r", ssb->f);
    else
      ssb->f = fopen(ssb->name, "r");
  } else
    rewind(ssb->f);
  if (ssb->f == NULL)
    _EscIO2(FileNotFound, ssb->name);
  RESETBUF(ssb->f, Char);
  for (lane = 1; lane <= 24; lane++) {
    if (lane > 1) {
      d->next = (sequence *)Malloc(sizeof(sequence));
      d = d->next;
      clearseq(d);
      copyname(now, &d->date);
    }
    readssblane(ssb, lane, &d->plasmidname, &d->primername, &location);
    d->lanenumber = lane;
  }
}



Static Void reportlanes(results, data, expectsequence)
_TEXT *results;
sequence *data;
boolean expectsequence;
{
  /*
*/
  sequence *d;
  long l;
  sequence *WITH;

  putc('\n', results->f);
  bar(results, 80L, '*');
  fprintf(results->f, "\n*** Sample Sheet *");
  bar(results, 62L, '*');
  fprintf(results->f, "\n\n");
  d = data;
  while (d != NULL) {
    WITH = d;
    fprintf(results->f, "lane number: %2ld", WITH->lanenumber);
    if (WITH->plasmidname.length > 0) {
      fprintf(results->f, "  plasmid: ");
      writename(results, WITH->plasmidname);
      fprintf(results->f, "  primer: ");
      writename(results, WITH->primername);
    }

    if (expectsequence) {
      if (WITH->length > 0) {
	if (WITH->plasmidname.length <= 0)
	  fprintf(results->f, "  THAT'S ODD!  SEQUENCE WITHOUT A NAME:");
	putc(' ', results->f);
	l = 1;
	while (l <= WITH->length && l <= quicksequence) {
	  putc(WITH->bases[l-1], results->f);
	  l++;
	}
	if (l < WITH->length)
	  fprintf(results->f, "...");

      } else
	fprintf(results->f, " NO SEQUENCE DATA FOUND! ******");
    }

    putc('\n', results->f);
    d = d->next;
  }
  putc('\n', results->f);
}


#define samplebot       131
#define sampletop       162
#define namebot         163
#define nametop         418
#define samplelength    586


/* Local variables for themain: */
struct LOC_themain {

  long samplenumber;
} ;

Local long spot_(a, LINK)
long a;
struct LOC_themain *LINK;
{
  return (a + samplelength * (LINK->samplenumber - 1));
}




Static Void themain(ssb, report)
_TEXT *ssb, *report;
{
  struct LOC_themain V;
  block b;
  long location = 1;

  sequence *data;
  name now;
  _TEXT TEMP;

  if (*ssb->name != '\0') {
    if (ssb->f != NULL)
      ssb->f = freopen(ssb->name, "r", ssb->f);
    else
      ssb->f = fopen(ssb->name, "r");
  } else
    rewind(ssb->f);
  if (ssb->f == NULL)
    _EscIO2(FileNotFound, ssb->name);
  RESETBUF(ssb->f, Char);
  if (*report->name != '\0') {
    if (report->f != NULL)
      report->f = freopen(report->name, "w", report->f);
    else
      report->f = fopen(report->name, "w");
  } else {
    if (report->f != NULL)
      rewind(report->f);
    else
      report->f = tmpfile();
  }
  if (report->f == NULL)
    _EscIO2(FileNotFound, report->name);
  SETUPBUF(report->f, Char);
  fprintf(report->f, "ssbread %4.2f\n", version);
  printf("ssbread %4.2f\n", version);
  grabandshow(ssb, report, 3L, 64L, &b, &location);
  putc('\n', report->f);

  for (V.samplenumber = 1; V.samplenumber <= 24; V.samplenumber++) {
    grabandshow(ssb, report, spot_((long)samplebot, &V),
		spot_((long)sampletop, &V), &b, &location);
    grabandshow(ssb, report, spot_((long)namebot, &V),
		spot_((long)nametop, &V), &b, &location);
  }

  putc('\n', report->f);


  data = (sequence *)Malloc(sizeof(sequence));
  readssb(ssb, now, &data);
  reportlanes(report, data, false);

  printf("Name: \"");
  readsegment(ssb, 3L, 64L, &b, &location);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  skipnulluptonull(&TEMP, b);
  printf("\"\n");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  reportlanes(&TEMP, data, false);

  /*





*/

  /*





*/

  /*






*/
}

#undef samplebot
#undef sampletop
#undef namebot
#undef nametop
#undef samplelength


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  report.f = NULL;
  strcpy(report.name, "report");
  ssb.f = NULL;
  strcpy(ssb.name, "ssb");
  themain(&ssb, &report);
_L1:
  if (ssb.f != NULL)
    fclose(ssb.f);
  if (report.f != NULL)
    fclose(report.f);
  exit(EXIT_SUCCESS);
}



/* End. */
