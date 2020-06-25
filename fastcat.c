/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "fastcat.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.13
/*









*/
#define updateversion   1.00



/*


































































*/


/*
*/



#define dnamax          1024
#define namelength      200
#define linelength      210



#define datetimearraylength  19


/*
*/




typedef long chset[5];




typedef Char alpha[namelength];

/*
*/

typedef struct name {
  alpha letters;
  uchar length;
} name;

typedef struct line {
  Char letters[linelength];
  uchar length;
  struct line *next;
} line;

typedef enum {
  plus, minus, dircomplement, dirhomologous
} direction;
typedef enum {
  linear, circular
} configuration;
typedef enum {
  on, off
} state;

typedef struct header {
  name keynam;
  line *fulnam, *note;
} header;



typedef enum {
  a, c, g, t
} base;



typedef short dnarange;

/* p2c: fastcat.p, line 157:
 * Note: Field width for seq assumes enum base has 4 elements [105] */
typedef uchar seq[(dnamax + 3) / 4];

typedef struct dnastring {
  seq part;
  dnarange length;
  struct dnastring *next;
} dnastring;


typedef struct orgkey {
  header hea;
  line *mapunit;
} orgkey;

typedef struct chrkey {
  header hea;
  double mapbeg, mapend;
} chrkey;

typedef struct piekey {
  header hea;
  double mapbeg;
  configuration coocon;
  direction coodir;
  long coobeg, cooend;
  configuration piecon;
  direction piedir;
  long piebeg, pieend;
} piekey;


typedef struct piece {
  piekey key;
  dnastring *dna;
} piece;

typedef struct reference {
  name pienam;
  double mapbeg;
  direction refdir;
  long refbeg, refend;
} reference;

typedef struct genkey {
  header hea;
  reference ref;
} genkey;

typedef struct trakey {
  header hea;
  reference ref;
} trakey;

typedef struct markey {
  header hea;
  reference ref;
  state sta;
  line *phenotype;
  struct marker *next;
} markey;

typedef struct marker {
  markey key;
  dnastring *dna;
} marker;




typedef struct item {
  Char nodeletter;
  name nam;
  long line_;
} item;

typedef struct recordfile {
  FILE *f;
  FILEBUFNC(f,item);
  Char name[_FNSIZE];
} recordfile;


Static _TEXT book, fastcatp;

Static recordfile cat;


Static jmp_buf _JL1;




Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


/* Local variables for themain: */
struct LOC_themain {
  item catitem;
  long theline;
} ;

/* Local variables for grabname: */
struct LOC_grabname {
  struct LOC_themain *LINK;
  Char ch;
} ;

Local Void grabname(ch_, book, cat, objopen, LINK)
Char ch_;
_TEXT *book;
recordfile *cat;
boolean *objopen;
struct LOC_themain *LINK;
{
  /*
*/
  struct LOC_grabname V;
  long i;
  Char c_;
  long FORLIM;

  V.LINK = LINK;
  V.ch = ch_;
  *objopen = !*objopen;
  if (!*objopen)
    return;


  LINK->catitem.nodeletter = V.ch;
  LINK->catitem.line_ = LINK->theline - 1;



  V.ch = getc(book->f);
  V.ch = getc(book->f);
  if (V.ch == '\n')
    V.ch = ' ';
/* p2c: fastcat.p, line 298:
 * Note: Taking & of possibly promoted param ch [324] */
  if (V.ch == '\n')
    V.ch = ' ';
/* p2c: fastcat.p, line 298:
 * Note: Taking & of possibly promoted param ch [324] */

  LINK->catitem.nam.length = 0;
  do {
    LINK->catitem.nam.length++;
    c_ = getc(book->f);
    if (c_ == '\n')
      c_ = ' ';
    LINK->catitem.nam.letters[LINK->catitem.nam.length - 1] = c_;
  } while (!(P_eoln(book->f) || LINK->catitem.nam.length >= namelength ||
	     LINK->catitem.nam.letters[LINK->catitem.nam.length - 1] == ' '));
  if (LINK->catitem.nam.letters[LINK->catitem.nam.length - 1] == ' ')
    LINK->catitem.nam.length--;
  fscanf(book->f, "%*[^\n]");
  getc(book->f);
  LINK->theline++;
  fwrite(&LINK->catitem, sizeof(item), 1, cat->f);
  FORLIM = LINK->catitem.nam.length;


  for (i = 0; i < FORLIM; i++)
    LINK->catitem.nam.letters[i] = ' ';

}



Static Void themain(book, fastcatp, cat)
_TEXT *book, *fastcatp;
recordfile *cat;
{
  struct LOC_themain V;
  Char ch;
  long i;
  double parameterversion;

  boolean orgopen = false, chropen = false, pieopen = false, genopen = false,
	  maropen = false, traopen = false;
  long FORLIM;

  printf("fastcat %4.2f\n", version);
  if (*fastcatp->name != '\0') {
    if (fastcatp->f != NULL)
      fastcatp->f = freopen(fastcatp->name, "r", fastcatp->f);
    else
      fastcatp->f = fopen(fastcatp->name, "r");
  } else
    rewind(fastcatp->f);
  if (fastcatp->f == NULL)
    _EscIO2(FileNotFound, fastcatp->name);
  RESETBUF(fastcatp->f, Char);
  fscanf(fastcatp->f, "%lg%*[^\n]", &parameterversion);
  getc(fastcatp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*book->name != '\0') {
    if (book->f != NULL)
      book->f = freopen(book->name, "r", book->f);
    else
      book->f = fopen(book->name, "r");
  } else
    rewind(book->f);
  if (book->f == NULL)
    _EscIO2(FileNotFound, book->name);
  RESETBUF(book->f, Char);

  if (BUFEOF(book->f)) {
    printf("ERROR: the book is empty\n");
    halt();
  }

  if (P_peek(book->f) == 'h') {
    printf("ERROR: the book is bad\n");
    halt();
  }

  if (*cat->name != '\0') {
    if (cat->f != NULL)
      cat->f = freopen(cat->name, "wb", cat->f);
    else
      cat->f = fopen(cat->name, "wb");
  } else {
    if (cat->f != NULL)
      rewind(cat->f);
    else
      cat->f = tmpfile();
  }
  if (cat->f == NULL)
    _EscIO2(FileNotFound, cat->name);
  SETUPBUF(cat->f, item);


  V.theline = 1;


  for (i = 0; i < namelength; i++)
    V.catitem.nam.letters[i] = ' ';
  V.catitem.nodeletter = '*';
  V.catitem.nam.length = datetimearraylength;
  ch = getc(book->f);
  ch = getc(book->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == '\n')
    ch = ' ';
  i = 1;
  FORLIM = V.catitem.nam.length;
  for (i = 0; i < FORLIM; i++) {
    V.catitem.nam.letters[i] = getc(book->f);
    if (V.catitem.nam.letters[i] == '\n')
      V.catitem.nam.letters[i] = ' ';
  }
  V.catitem.line_ = V.theline;
  fwrite(&V.catitem, sizeof(item), 1, cat->f);
  FORLIM = V.catitem.nam.length;

  for (i = 0; i < FORLIM; i++)
    V.catitem.nam.letters[i] = ' ';
  fscanf(book->f, "%*[^\n]");
  getc(book->f);
  V.theline++;


  do {
    fscanf(book->f, "%c%*[^\n]", &ch);
    getc(book->f);
    if (ch == '\n')
      ch = ' ';
    V.theline++;
    if (ch == 'm' || ch == 't' || ch == 'g' || ch == 'p' || ch == 'c' ||
	ch == 'o') {
      switch (ch) {

      case 'o':
	grabname(ch, book, cat, &orgopen, &V);
	break;

      case 'c':
	grabname(ch, book, cat, &chropen, &V);
	break;

      case 'p':
	grabname(ch, book, cat, &pieopen, &V);
	break;

      case 'g':
	grabname(ch, book, cat, &genopen, &V);
	break;

      case 't':
	grabname(ch, book, cat, &traopen, &V);
	break;

      case 'm':
	grabname(ch, book, cat, &maropen, &V);
	break;
      }
    }
  } while (!BUFEOF(book->f));

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  cat.f = NULL;
  strcpy(cat.name, "cat");
  fastcatp.f = NULL;
  strcpy(fastcatp.name, "fastcatp");
  book.f = NULL;
  strcpy(book.name, "book");
  themain(&book, &fastcatp, &cat);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (fastcatp.f != NULL)
    fclose(fastcatp.f);
  if (cat.f != NULL)
    fclose(cat.f);
  exit(EXIT_SUCCESS);
}



/* End. */
