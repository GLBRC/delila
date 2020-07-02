/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "catal.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/






#define version         9.64
/*


























*/



/*













































































*/


/*








































































































































*/


#define debugging       false
#define verbose         false
/*
*/

#define namelength      200
#define linelength      210

#define namespace       15
#define numlibfil       3
#define numcatfil       3
#define pagesize        60
#define listingwidth    60


#define specialchar     '*'
/*
*/

#define levelsize       1
/*
*/


#define datafield       110
/*
*/
#define bfield          10
#define cfield          9
#define dfield          5
#define nfield          8



#define firstnumber     '2'
/*

*/


#define datetimearraylength  19


/*





*/






typedef Char datetimearray[datetimearraylength];


typedef Char alpha[namelength];



/*
*/

typedef struct name {
  alpha letters;
  uchar length;
} name;



typedef enum {
  firstpage, chromosome, dna, enzyme, gene, library, marker, organism, piece,
  recognitionclass, transcript
} calltype;


typedef enum {
  cooconfigurationbad, coordirectionbad, coordbeginningbad, coordendingbad,
  pieconfigurationbad, piedirectionbad, piebeginningbad, pieendingbad,
  refdirectionbad, refbeginningbad, refendingbad, genebeginning, geneending,
  mapbeginning, maplocation, nolastpiece, noreference, wrongreference
} errtype;


typedef struct item {
  Char letter;
  name nam;
  long line;
} item;

typedef struct catfile {
  FILE *f;
  FILEBUFNC(f,item);
  Char name[_FNSIZE];
} catfile;



typedef Char buffer[linelength];



typedef struct namandlistptr {
  name nam;
  struct namandlistptr *nextonlist;
} namandlistptr;

typedef struct currvals {
  namandlistptr marker_, transcript_, gene_, piece_, chromosome_, enzyme_,
		organism_, recognition;
} currvals;


typedef enum {
  plus, minus
} direction;
typedef enum {
  linear, circular
} configuration;

typedef struct pieceinfo {
  configuration config;
  direction direct;
  long beginning, ending;
} pieceinfo;

typedef struct refnode {
  /*
*/
  enum {
    markref, transref, generef
  } nodetype;
  name nodename;
  direction nodedir;
  long nodebeg, nodeend;
  struct refnode *nodenext;
} refnode;



Static _TEXT catalp;
/*
*/
Static boolean keepdates;
/*



*/
Static boolean docomments, fromtoinst;


Static item catitem;
/*


*/
Static catfile cat1, cat2, cat3;

Static long catnumber;

Static _TEXT humcat, catin;
Static long humcatpage, humcatlines;

Static currvals current, first;
/*

*/

Static namandlistptr *freelistptr, *latest;

Static _TEXT l1, l2, l3;
/*


*/
Static _TEXT lib1, lib2, lib3;

Static boolean libdone;
/*
*/
Static long libline, libnumber;

Static long level;
/*
*/

Static boolean unique;
/*
*/

Static datetimearray daytime;
/*
*/
Static name none, aname;
/*
*/
Static boolean toolong;
/*
*/

Static buffer line;
Static long length_;

Static long uniquebeg;
/*
*/
Static long maxinteger;
/*
*/
/*

*/
Static long basecount;
/*
*/

Static name piecename;
/*
*/
Static boolean changed;
/*
*/
Static name newpiecename;
/*
*/

Static boolean piecefound;
/*
*/

Static long newnamelength;

Static pieceinfo coo, pie;

Static refnode pieceref;
/*
*/
Static refnode *refroot;

Static long fatal;
/*
*/

Static long pfield, todatafield;
/*
*/

Static double mapbeg, mapend;

Static boolean titlewritten;
/*

*/

Static long maxnamelength;


Static jmp_buf _JL1;







Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}






Static Void writename(f, n)
_TEXT *f;
name n;
{
  long i;

  for (i = 0; i < n.length; i++)
    putc(n.letters[i], f->f);
}





Static Void erroratline()
{
  /*

*/
  fprintf(humcat.f,
	  "\n error *********************************************\n");
  fprintf(humcat.f, " at line %ld in file %ld\n", libline - 1, libnumber);
  humcatlines += 3;
  printf(" *** error at line %ld in file %ld\n", libline - 1, libnumber);
  fprintf(catin.f, " *** error at line %ld in file %ld\n",
	  libline - 1, libnumber);
}


/* Local variables for error: */
struct LOC_error {
  errtype indicator;
} ;

/*
*/
Local Void message(thefile, filelines, LINK)
_TEXT *thefile;
long *filelines;
struct LOC_error *LINK;
{
  switch (LINK->indicator) {

  case mapbeginning:
    fprintf(thefile->f, " in map beginning\n");
    break;

  case maplocation:
    fprintf(thefile->f,
	    " genetic map location must be in the map [%5.2f, %5.2f]\n",
	    mapbeg, mapend);
    break;

  case cooconfigurationbad:
    fprintf(thefile->f,
	    " coordinate configuration must be  either linear or circular.\n");
    break;

  case coordirectionbad:
    fprintf(thefile->f, " coordinate direction must be a '+' or a '-'.\n");
    break;

  case coordbeginningbad:
    fprintf(thefile->f, " coordinate beginning must be an integer\n");
    break;

  case coordendingbad:
    fprintf(thefile->f, " coordinate ending must be an integer.\n");
    break;

  case pieconfigurationbad:
    fprintf(thefile->f,
	    " piece configuration must be either linear or circular.\n");
    break;

  case piedirectionbad:
    fprintf(thefile->f, " piece direction must be a '+' or a '-'.\n");
    break;

  case piebeginningbad:
    fprintf(thefile->f, " beginning piece coordinate must be an integer\n");
    break;

  case pieendingbad:
    fprintf(thefile->f, " ending piece coordinate must be an integer\n");
    break;

  case refdirectionbad:
    fprintf(thefile->f, " direction must be a '+' or a '-'.\n");
    break;

  case refbeginningbad:
    fprintf(thefile->f, " nucleotide beginning must be an integer.\n");
    break;

  case refendingbad:
    fprintf(thefile->f, " nucleotide ending must be an integer.\n");
    break;

  case genebeginning:
    fprintf(thefile->f, "in gene beginning \n");
    writename(thefile, current.gene_.nam);
    putc('\n', thefile->f);
    break;

  case geneending:
    fprintf(thefile->f, "in gene ending \n");
    writename(thefile, current.gene_.nam);
    putc('\n', thefile->f);
    break;

  case nolastpiece:
    fprintf(thefile->f, " error in family structure.\n");
    fprintf(thefile->f, " reference was made to a new piece: ");
    writename(thefile, aname);
    fprintf(thefile->f, "\n while a piece previously referenced: ");
    writename(thefile, piecename);
    fprintf(thefile->f, " was still unfound.\n");
    *filelines += 3;
    break;

  case noreference:
    fprintf(thefile->f,
	    " caution: no reference was previously made to this piece: ");
    writename(thefile, aname);
    putc('\n', thefile->f);
    printf(" (this is a warning: unreferenced piece)\n");
    (*filelines)++;
    break;

  case wrongreference:
    fprintf(thefile->f, " error in family structure.\n");
    fprintf(thefile->f, " piece ");
    writename(thefile, aname);
    fprintf(thefile->f, " was encountered while a reference to piece ");
    writename(thefile, piecename);
    fprintf(thefile->f, " was still active.\n");
    (*filelines)++;
    break;
  }

  if (((1L << ((long)LINK->indicator)) & ((1L << ((long)pieendingbad + 1)) -
	 (1L << ((long)cooconfigurationbad)))) != 0) {
    fprintf(thefile->f, " in piece ");
    writename(thefile, current.piece_.nam);
    putc('\n', thefile->f);
    (*filelines)++;
  }

  if (((1L << ((long)LINK->indicator)) &
       ((1L << ((long)refendingbad + 1)) - (1L << ((long)refdirectionbad)))) != 0) {
    fprintf(thefile->f, " error found in reference to piece ");
    writename(thefile, piecename);
    fprintf(thefile->f, "\n by ");
    switch (pieceref.nodetype) {

    case markref:
      fprintf(thefile->f, "marker");
      break;

    case generef:
      fprintf(thefile->f, "gene");
      break;

    case transref:
      fprintf(thefile->f, "transcript");
      break;
    }
    fprintf(thefile->f, " \n");
    writename(thefile, pieceref.nodename);
    putc('\n', thefile->f);

    *filelines += 2;
  }


  if (((1L << ((long)LINK->indicator)) & (1L << ((long)noreference))) == 0)
    fatal++;


  *filelines += 2;
  putc('\n', thefile->f);
}


Static Void error(indicator_)
errtype indicator_;
{
  /*










*/
  struct LOC_error V;
  long dummy;

  V.indicator = indicator_;
  erroratline();
  message(&humcat, &humcatlines, &V);
  message(&catin, &dummy, &V);
}


Static Void strange(callingproc)
calltype callingproc;
{
  printf(" '%c' found at line %ld in library no. %ld where",
	 line[0], libline - 1, libnumber);
  switch (callingproc) {

  case firstpage:
    printf(" (firstpage)");
    break;

  case marker:
    printf(" an m");
    break;

  case transcript:
    printf(" a t");
    break;

  case gene:
    printf(" a g");
    break;

  case piece:
    printf(" a p");
    break;

  case chromosome:
    printf(" a c, m, t, g, or p");
    break;

  case enzyme:
    printf(" an e or an s");
    break;

  case recognitionclass:
    printf(" an r or an e");
    break;

  case organism:
    printf(" an o or a c");
    break;

  case library:
    printf(" an o or an r");
    break;

  case dna:
    printf(" a d");
    break;
  }
  printf(" was expected.\n");
  halt();
}



Static Void docathelp(h)
_TEXT *h;
{
  fprintf(h->f, " All of the libraries were empty!\n");
  fprintf(h->f, " Usage of catal %4.2f:\n", version);
  fprintf(h->f,
    " catal(humcat,catalp,l1,c1,newl1,l2,c2,newl2,l3,c3,newc3,output)\n");
  fprintf(h->f,
	  " The catal program is used to check the format of one or more\n");
  fprintf(h->f, " libraries and to build catalogues corresponding to them.\n");
  fprintf(h->f, " It also produces a catalogue for humans, the humcat.\n");
  fprintf(h->f, " and Delila instructions in catin.\n");
  fprintf(h->f, " See delman for further help.\n");
}




Static Void traversealist(the)
namandlistptr **the;
{
  _TEXT TEMP;

  if (*the == NULL)
    return;
  printf("  ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writename(&TEMP, (*the)->nam);
  putchar('\n');

  traversealist(&(*the)->nextonlist);
}


Static Void dumplists()
{
  if (!verbose)
    return;

  printf(" lists at time of error:\n");

  /*
*/
  printf(" recognition-class names\n");
  traversealist(&first.recognition.nextonlist);


  printf(" enzyme names for the current recognition-class\n");
  traversealist(&first.enzyme_.nextonlist);


  printf(" organism names\n");
  traversealist(&first.organism_.nextonlist);


  printf(" chromosome names for the current organism\n");
  traversealist(&first.chromosome_.nextonlist);


  printf(" marker names for the current chromosome\n");
  traversealist(&first.marker_.nextonlist);


  printf(" transcript names for current chromosome\n");
  traversealist(&first.transcript_.nextonlist);


  printf(" gene names for the current chromosome\n");
  traversealist(&first.gene_.nextonlist);


  printf(" piece names for the current chromosome\n");
  traversealist(&first.piece_.nextonlist);
}







Static Void getdatetime(adatetime)
Char *adatetime;
{
  /*





*/
  Char adate[datetimearraylength], atime[datetimearraylength];
  /*


*/
  Char month[3];
  long index;

  /*




*/

  VAXdate(adate);
  VAXtime(atime);

  /*

*/


  for (index = 1; index <= 4; index++)
    adatetime[index-1] = adate[index+6];
  adatetime[4] = '/';
  for (index = 4; index <= 6; index++)
    month[index-4] = adate[index-1];
  if (!strncmp(month, "JAN", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '1';
  } else if (!strncmp(month, "FEB", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '2';
  } else if (!strncmp(month, "MAR", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '3';
  } else if (!strncmp(month, "APR", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '4';
  } else if (!strncmp(month, "MAY", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '5';
  } else if (!strncmp(month, "JUN", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '6';
  } else if (!strncmp(month, "JUL", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '7';
  } else if (!strncmp(month, "AUG", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '8';
  } else if (!strncmp(month, "SEP", 3)) {
    adatetime[5] = '0';
    adatetime[6] = '9';
  } else if (!strncmp(month, "OCT", 3)) {
    adatetime[5] = '1';
    adatetime[6] = '0';
  } else if (!strncmp(month, "NOV", 3)) {
    adatetime[5] = '1';
    adatetime[6] = '1';
  } else if (!strncmp(month, "DEC", 3)) {
    adatetime[5] = '1';
    adatetime[6] = '2';
  }
  adatetime[7] = '/';
  for (index = 7; index <= 8; index++)
    adatetime[index+1] = adate[index-7];


  if (adatetime[5] == ' ')
    adatetime[5] = '0';
  if (adatetime[8] == ' ')
    adatetime[8] = '0';

  adatetime[10] = ' ';
  for (index = 10; index <= 17; index++)
    adatetime[index+1] = atime[index-10];
}



Static Void readdatetime(thefile, adatetime)
_TEXT *thefile;
Char *adatetime;
{
  /*



*/
  /*



*/
  long index;
  /*
*/
  Char udatetime[datetimearraylength];

  for (index = 0; index < datetimearraylength; index++) {
    udatetime[index] = getc(thefile->f);
    if (udatetime[index] == '\n')
      udatetime[index] = ' ';
  }
  memcpy(adatetime, udatetime, sizeof(datetimearray));
  if (adatetime[2] == '/' && adatetime[11] == ':') {
    printf("You have an old datetime (only 2 year digits): \n");
    for (index = 0; index < datetimearraylength; index++)
      putchar(adatetime[index]);
    printf("\nConvert your database to 4 digit years.\n");
    halt();
  }
  /*

*/
  if (adatetime[4] == '/' && adatetime[7] == '/' && adatetime[13] == ':' &&
      adatetime[16] == ':')
    return;
  printf("readdatetime: bad date time read:\n");
  for (index = 0; index < datetimearraylength; index++)
    putchar(adatetime[index]);
  putchar('\n');
  halt();
}



Static Void writedatetime(thefile, adatetime)
_TEXT *thefile;
Char *adatetime;
{
  long index;

  for (index = 0; index < datetimearraylength; index++)
    putc(adatetime[index], thefile->f);
}



/*




*/
Static Void addtoseed(seed, power, c)
double *seed, *power;
Char c;
{
  long n;

  *power /= 10;
  /*


*/
  n = c - '0';
  if ((unsigned long)n > 9) {
    printf("timeseed: error in datetime\n");
    printf("it contains \"%c\" which is not a number.\n", c);
    printf("The getdatetime routine must be fixed.\n");
    halt();
  }
  *seed += *power * n;
}


Static Void makeseed(adatetime, seed)
Char *adatetime;
double *seed;
{
  /*




*/
  double power = 1.0;

  *seed = 0.0;
  addtoseed(seed, &power, adatetime[18]);
  addtoseed(seed, &power, adatetime[17]);

  addtoseed(seed, &power, adatetime[15]);
  addtoseed(seed, &power, adatetime[14]);

  addtoseed(seed, &power, adatetime[12]);
  addtoseed(seed, &power, adatetime[11]);

  addtoseed(seed, &power, adatetime[9]);
  addtoseed(seed, &power, adatetime[8]);

  addtoseed(seed, &power, adatetime[6]);
  addtoseed(seed, &power, adatetime[5]);

  addtoseed(seed, &power, adatetime[3]);
  addtoseed(seed, &power, adatetime[2]);
  addtoseed(seed, &power, adatetime[1]);
  addtoseed(seed, &power, adatetime[0]);
}


Static Void orderseedDelila(adatetime, seed)
Char *adatetime;
double *seed;
{
  double power = 1.0;

  *seed = 0.0;
  addtoseed(seed, &power, adatetime[2]);
  addtoseed(seed, &power, adatetime[3]);
  addtoseed(seed, &power, adatetime[5]);
  addtoseed(seed, &power, adatetime[6]);

  addtoseed(seed, &power, adatetime[8]);
  addtoseed(seed, &power, adatetime[9]);

  addtoseed(seed, &power, adatetime[11]);
  addtoseed(seed, &power, adatetime[12]);

  addtoseed(seed, &power, adatetime[14]);
  addtoseed(seed, &power, adatetime[15]);

  addtoseed(seed, &power, adatetime[17]);
  addtoseed(seed, &power, adatetime[18]);
}


Static Void timeseed(seed)
double *seed;
{
  /*



*/
  datetimearray adatetime;

  getdatetime(adatetime);
  /*

*/
  makeseed(adatetime, seed);
}




Static Void limitdate(a, b, c, d, limitdatetime_)
Char a, b, c, d;
Char *limitdatetime_;
{
  /*
*/
  datetimearray limitdatetime, adatetime;
  double Dday, now;

  memcpy(limitdatetime, limitdatetime_, sizeof(datetimearray));
  getdatetime(adatetime);
  /*






*/

  orderseedDelila(adatetime, &now);

  if (limitdatetime[0] != ' ' || limitdatetime[1] != ' ' ||
      limitdatetime[2] != ' ' || limitdatetime[3] != ' ')
    halt();

  limitdatetime[0] = a;
  limitdatetime[1] = b;
  limitdatetime[2] = c;
  limitdatetime[3] = d;

  orderseedDelila(limitdatetime, &Dday);

  /*


*/
  if (now <= Dday)
    return;
  printf("This program expired on %.*s\n", datetimearraylength, limitdatetime);
  printf("See: https://alum.mit.edu/www/toms/walker/contacts.html\n");
  halt();
}









Static Void maxnum(maxreal)
double *maxreal;
{
  /*








*/
  double small = 1.0;
  double lastsmall, prelastsmall, preprelastsmall;

  while (small != 0) {
    preprelastsmall = prelastsmall;
    prelastsmall = lastsmall;
    lastsmall = small;
    small /= 10;
    printf("maxnum: small = % .3E\n", small);
  }
  *maxreal = 1 / preprelastsmall;
  printf("maxnum: lastsmall = % .3E\n", lastsmall);
  printf("maxnum: prelastsmall = % .3E\n", prelastsmall);
  printf("maxnum: preprelastsmall = % .3E\n", preprelastsmall);
  printf("maxnum: maxreal = % .3E\n", *maxreal);
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


Static Void reportparent(l)
_TEXT *l;
{
  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "r", l->f);
    else
      l->f = fopen(l->name, "r");
  } else
    rewind(l->f);
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  RESETBUF(l->f, Char);
  if (BUFEOF(l->f))
    return;
  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "r", l->f);
    else
      l->f = fopen(l->name, "r");
  } else
    rewind(l->f);
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  RESETBUF(l->f, Char);
  copyaline(l, &humcat);
  humcatlines++;

  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "r", l->f);
    else
      l->f = fopen(l->name, "r");
  } else
    rewind(l->f);
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  RESETBUF(l->f, Char);
  copyaline(l, &catin);
  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "r", l->f);
    else
      l->f = fopen(l->name, "r");
  } else
    rewind(l->f);
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  RESETBUF(l->f, Char);
}


Static long countlibrary(l)
_TEXT *l;
{
  if (*l->name != '\0') {
    if (l->f != NULL)
      l->f = freopen(l->name, "r", l->f);
    else
      l->f = fopen(l->name, "r");
  } else
    rewind(l->f);
  if (l->f == NULL)
    _EscIO2(FileNotFound, l->name);
  RESETBUF(l->f, Char);
  if (BUFEOF(l->f))
    return 0;
  else
    return 1;
}


Static Void dopage()
{
  if (humcatpage != 0) {
    fprintf(humcat.f, "\f");
    humcatlines = 0;
  }
  humcatpage++;
  fprintf(humcat.f, "catal %4.2f", version);
  fprintf(humcat.f, " - Catalogue listing of ");
  writedatetime(&humcat, daytime);
  fprintf(humcat.f, " page %ld\n", humcatpage);
  if (titlewritten)
    return;
  fprintf(catin.f, "title \"Delila instructions for library ");
  writedatetime(&catin, daytime);
  fprintf(catin.f, "\";\n");
  fprintf(catin.f, "(* catal %4.2f *)\n", version);
  titlewritten = true;
}


Static Void initializecatalogs(catalp, c1, c2, c3)
_TEXT *catalp;
catfile *c1, *c2, *c3;
{
  long i;
  Char ch;




  if (*catalp->name != '\0') {
    if (catalp->f != NULL)
      catalp->f = freopen(catalp->name, "r", catalp->f);
    else
      catalp->f = fopen(catalp->name, "r");
  } else
    rewind(catalp->f);
  if (catalp->f == NULL)
    _EscIO2(FileNotFound, catalp->name);
  RESETBUF(catalp->f, Char);
  /*

*/
  docomments = true;
  fromtoinst = false;

  if (BUFEOF(catalp->f))
    keepdates = false;
  else {
    fscanf(catalp->f, "%c%*[^\n]", &ch);
    getc(catalp->f);
    if (ch == '\n')
      ch = ' ';
    if (ch == 'b' || ch == 'n')
      keepdates = true;
    else
      keepdates = false;

    if (!BUFEOF(catalp->f)) {
      if (P_peek(catalp->f) == 'n')
	docomments = false;
      fscanf(catalp->f, "%*[^\n]");
      getc(catalp->f);
    }

    if (!BUFEOF(catalp->f)) {
      if (P_peek(catalp->f) == 'f')
	fromtoinst = true;
      fscanf(catalp->f, "%*[^\n]");
      getc(catalp->f);
    }

  }
  printf(" library dates ");
  if (keepdates)
    printf("kept\n");
  else
    printf("advanced\n");
  if (*c1->name != '\0') {
    if (c1->f != NULL)
      c1->f = freopen(c1->name, "wb", c1->f);
    else
      c1->f = fopen(c1->name, "wb");
  } else {
    if (c1->f != NULL) {


      rewind(c1->f);
    } else
      c1->f = tmpfile();
  }
  if (c1->f == NULL)
    _EscIO2(FileNotFound, c1->name);
  SETUPBUF(c1->f, item);
  if (*c2->name != '\0') {
    if (c2->f != NULL)
      c2->f = freopen(c2->name, "wb", c2->f);
    else
      c2->f = fopen(c2->name, "wb");
  } else {
    if (c2->f != NULL)
      rewind(c2->f);
    else
      c2->f = tmpfile();
  }
  if (c2->f == NULL)
    _EscIO2(FileNotFound, c2->name);
  SETUPBUF(c2->f, item);
  if (*c3->name != '\0') {
    if (c3->f != NULL)
      c3->f = freopen(c3->name, "wb", c3->f);
    else
      c3->f = fopen(c3->name, "wb");
  } else {
    if (c3->f != NULL)
      rewind(c3->f);
    else
      c3->f = tmpfile();
  }
  if (c3->f == NULL)
    _EscIO2(FileNotFound, c3->name);
  SETUPBUF(c3->f, item);

  if (*humcat.name != '\0') {
    if (humcat.f != NULL)
      humcat.f = freopen(humcat.name, "w", humcat.f);
    else
      humcat.f = fopen(humcat.name, "w");
  } else {
    if (humcat.f != NULL)
      rewind(humcat.f);
    else
      humcat.f = tmpfile();
  }
  if (humcat.f == NULL)
    _EscIO2(FileNotFound, humcat.name);
  SETUPBUF(humcat.f, Char);
  if (*catin.name != '\0') {
    if (catin.f != NULL)
      catin.f = freopen(catin.name, "w", catin.f);
    else
      catin.f = fopen(catin.name, "w");
  } else {
    if (catin.f != NULL)
      rewind(catin.f);
    else
      catin.f = tmpfile();
  }
  if (catin.f == NULL)
    _EscIO2(FileNotFound, catin.name);
  SETUPBUF(catin.f, Char);
  humcatlines = 0;
  humcatpage = 0;

  titlewritten = false;
  dopage();

  putc('\n', humcat.f);
  humcatlines++;
  fprintf(humcat.f, "parent librar");
  if (countlibrary(&l1) + countlibrary(&l2) + countlibrary(&l3) > 1)
    fprintf(humcat.f, "ies");
  else
    putc('y', humcat.f);
  fprintf(humcat.f, ": \n");
  humcatlines++;
  if (docomments) {
    fprintf(catin.f, "(* parent library: \n");
    reportparent(&l1);
    reportparent(&l2);
    reportparent(&l3);
    fprintf(catin.f, " *)\n");
  }

  libnumber = 1;
  catnumber = 1;
  for (i = 0; i < namelength; i++)
    none.letters[i] = ' ';
  piecename = none;
  piecefound = false;
  refroot = NULL;


  current.marker_.nam = none;
  current.transcript_.nam = none;
  current.gene_.nam = none;
  current.piece_.nam = none;
  current.chromosome_.nam = none;
  current.enzyme_.nam = none;
  current.recognition.nam = none;
  current.organism_.nam = none;
  freelistptr = NULL;

  first.marker_.nextonlist = NULL;
  first.transcript_.nextonlist = NULL;
  first.gene_.nextonlist = NULL;
  first.piece_.nextonlist = NULL;
  first.chromosome_.nextonlist = NULL;
  first.enzyme_.nextonlist = NULL;
  first.recognition.nextonlist = NULL;
  first.organism_.nextonlist = NULL;
  /*
*/
  /*


*/
  todatafield = namespace;


  pfield = cfield + dfield + nfield * 2;


  fatal = 0;

}




Static Void dumpline(newlib, i)
_TEXT *newlib;
long i;
{
  /*



*/
  long j;

  if (libdone)
    return;

  for (j = 0; j < i; j++)
    putc(line[j], newlib->f);
  /*
*/
  if (i == 1)
    putc(' ', newlib->f);
  putc('\n', newlib->f);
}


Static Void dumpitem(cat, it)
catfile *cat;
item it;
{
  fwrite(&it, sizeof(item), 1, cat->f);
}


Static Void readline(lib, length)
_TEXT *lib;
long *length;
{
  long j;

  Char uline[linelength];

  /*
*/
  libline++;
  *length = 0;

  if (BUFEOF(lib->f)) {
    libdone = true;
    return;
  }
  while (!P_eoln(lib->f) && *length < linelength) {
    (*length)++;
    uline[*length - 1] = getc(lib->f);
    if (uline[*length - 1] == '\n')
      uline[*length - 1] = ' ';
  }

  memcpy(line, uline, sizeof(buffer));


  for (j = *length; j < linelength; j++)
    line[j] = ' ';

  if (!P_eoln(lib->f)) {
    erroratline();
    fprintf(humcat.f, " the line is longer than %ld characters\n",
	    (long)linelength);
    putc(' ', humcat.f);

    for (j = 0; j < linelength; j++)
      putc(line[j], humcat.f);
    putc('\n', humcat.f);
    humcatlines += 2;

    if (docomments) {
      fprintf(catin.f, " the line is longer than %ld characters\n",
	      (long)linelength);
      putc(' ', catin.f);
      for (j = 0; j < linelength; j++)
	putc(line[j], catin.f);
      putc('\n', catin.f);
    }

    fatal++;
  }

  if (*length == 0) {
    erroratline();
    fprintf(humcat.f, " blank line in library\n");
    fprintf(catin.f, " blank line in library\n");
    humcatlines++;
    fatal++;
  }
  fscanf(lib->f, "%*[^\n]");
  getc(lib->f);
}


/*


*/
Static Void needline(lib, length)
_TEXT *lib;
long *length;
{
  /*
*/
  readline(lib, length);
  if (!libdone)
    return;
  erroratline();
  printf("premature end of library file\n");
  halt();
}


Static Void needlibline(lib, newlib, length)
_TEXT *lib, *newlib;
long *length;
{
  /*
*/
  needline(lib, length);
  dumpline(newlib, *length);
}



Static Void checkstar(checkattribute)
boolean checkattribute;
{
  /*


*/
  if (length_ < 3) {
    if (!checkattribute)
      return;
    erroratline();
    fprintf(humcat.f, " attribute is missing\n");
    fprintf(catin.f, " attribute is missing\n");
    fatal++;
    humcatlines++;
    return;
  }
  if (line[0] != '*') {
    erroratline();
    fprintf(humcat.f, " asterisk (*) missing from start of line\n");
    fprintf(catin.f, " asterisk (*) missing from start of line\n");
    fatal++;
    humcatlines++;
  }

  if (line[1] == ' ')
    return;
  erroratline();
  fprintf(humcat.f, " blank missing from start of line\n");
  fprintf(catin.f, " blank missing from start of line\n");
  fatal++;
  humcatlines++;
}


Static Void skipblanks(linepos, bad)
long *linepos;
boolean *bad;
{
  /*








*/
  *linepos = 3;

  while (*linepos <= length_ && line[*linepos - 1] == ' ')
    (*linepos)++;

  *bad = (*linepos > length_);
}


Static Void getinteger(lib, newlib, intnumber, badtoken)
_TEXT *lib, *newlib;
long *intnumber;
boolean *badtoken;
{
  /*


*/
  long subtotal, linepos, thesign;

  needlibline(lib, newlib, &length_);
  checkstar(true);
  skipblanks(&linepos, badtoken);
  if (!*badtoken) {
    subtotal = 0;
    if (line[linepos-1] == '-' || line[linepos-1] == '+') {
      if (line[linepos-1] == '-')
	thesign = -1;
      else
	thesign = 1;
      linepos++;
    } else
      thesign = 1;
    while (linepos <= length_ && isdigit(line[linepos-1]) &&
	   subtotal < LONG_MAX / 10 - 10) {
      switch (line[linepos-1]) {

      case '0':
	subtotal *= 10;
	break;

      case '1':
	subtotal = subtotal * 10 + 1;
	break;

      case '2':
	subtotal = subtotal * 10 + 2;
	break;

      case '3':
	subtotal = subtotal * 10 + 3;
	break;

      case '4':
	subtotal = subtotal * 10 + 4;
	break;

      case '5':
	subtotal = subtotal * 10 + 5;
	break;

      case '6':
	subtotal = subtotal * 10 + 6;
	break;

      case '7':
	subtotal = subtotal * 10 + 7;
	break;

      case '8':
	subtotal = subtotal * 10 + 8;
	break;

      case '9':
	subtotal = subtotal * 10 + 9;
	break;
      }
      linepos++;
    }
    if (linepos <= length_ && line[linepos-1] != ' ')
      *badtoken = true;
    else
      *intnumber = subtotal;

    *intnumber *= thesign;
  }
  if (*badtoken)
    *intnumber = 0;
}


Static Void getreal(lib, newlib, realnumber, badtoken)
_TEXT *lib, *newlib;
double *realnumber;
boolean *badtoken;
{
  /*


*/
  double subtotal;
  long linepos;
  boolean pointfound;
  double multiplier;
  long thesign;

  needlibline(lib, newlib, &length_);
  checkstar(true);
  skipblanks(&linepos, badtoken);
  if (!*badtoken) {
    multiplier = 1.0;
    pointfound = false;
    subtotal = 0.0;
    if (line[linepos-1] == '-' || line[linepos-1] == '+') {
      if (line[linepos-1] == '-')
	thesign = -1;
      else
	thesign = 1;
      linepos++;
    } else
      thesign = 1;

    while (linepos <= length_ &&
	   (line[linepos-1] == '.' || isdigit(line[linepos-1]))) {
      /*

*/
      if (line[linepos-1] == '.') {
	if (pointfound)
	  *badtoken = true;
	else
	  pointfound = true;
      } else {
	if (pointfound)
	  multiplier /= 10;
	else
	  subtotal *= 10;

	switch (line[linepos-1]) {

	case '0':
	  /* blank case */
	  break;

	case '1':
	  subtotal += multiplier;
	  break;

	case '2':
	  subtotal += multiplier * 2;
	  break;

	case '3':
	  subtotal += multiplier * 3;
	  break;

	case '4':
	  subtotal += multiplier * 4;
	  break;

	case '5':
	  subtotal += multiplier * 5;
	  break;

	case '6':
	  subtotal += multiplier * 6;
	  break;

	case '7':
	  subtotal += multiplier * 7;
	  break;

	case '8':
	  subtotal += multiplier * 8;
	  break;

	case '9':
	  subtotal += multiplier * 9;
	  break;
	}
      }
      linepos++;
    }
    if (linepos <= length_ && line[linepos-1] != ' ' || *badtoken)
      *badtoken = true;
    else
      *realnumber = subtotal;

    *realnumber = thesign * *realnumber;
  }

  if (*badtoken)
    *realnumber = 0.0;
}


Static Void getconfig(lib, newlib, c, badtoken)
_TEXT *lib, *newlib;
configuration *c;
boolean *badtoken;
{
  needlibline(lib, newlib, &length_);
  checkstar(true);
  *badtoken = false;
  if (line[2] == 'c') {
    *c = circular;
    return;
  }
  if (line[2] == 'l')
    *c = linear;
  else
    *badtoken = true;
}


Static Void getdirect(lib, newlib, d, badtoken)
_TEXT *lib, *newlib;
direction *d;
boolean *badtoken;
{
  needlibline(lib, newlib, &length_);
  checkstar(true);
  *badtoken = false;
  if (line[2] == '-') {
    *d = minus;
    return;
  }
  if (line[2] == '+')
    *d = plus;
  else
    *badtoken = true;
}


Static Void getname()
{
  long i = 1;
  long j;

  /*

*/
  checkstar(true);
  while (i <= namelength && line[i+1] != ' ') {
    aname.letters[i-1] = line[i+1];
    i++;
  }

  aname.length = i - 1;
  if (maxnamelength < aname.length)
    maxnamelength = aname.length;
  j = i;
  while (j <= namelength) {
    aname.letters[j-1] = ' ';
    j++;
  }

  catitem.nam.length = i - 1;

}




Static Void readlibdate(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  Char ch;
  long i;
  datetimearray olddaytime;
  _TEXT TEMP;
  long FORLIM;

  fprintf(newlib->f, "* ");
  ch = getc(lib->f);
  ch = getc(lib->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == '\n')
    ch = ' ';
  if (keepdates) {
    readdatetime(lib, daytime);

    writedatetime(newlib, daytime);

  }

  else {
    writedatetime(newlib, daytime);


    fprintf(newlib->f, ", ");
    readdatetime(lib, olddaytime);
    writedatetime(newlib, olddaytime);


    ch = getc(lib->f);
    ch = getc(lib->f);
    if (ch == '\n')
      ch = ' ';
    if (ch == '\n')
      ch = ' ';
    readdatetime(lib, olddaytime);
  }



  while (!P_eoln(lib->f)) {
    ch = getc(lib->f);
    if (ch == '\n')
      ch = ' ';
    putc(ch, newlib->f);
  }
  putc('\n', newlib->f);

  printf("\n library %ld date will be: ", libnumber);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writedatetime(&TEMP, daytime);
  putchar('\n');




  catitem.letter = '*';
  catitem.nam.length = datetimearraylength;
  FORLIM = catitem.nam.length;
  /*


*/
  for (i = 0; i < FORLIM; i++)
    catitem.nam.letters[i] = daytime[i];


  for (i = catitem.nam.length; i < namelength; i++)
    catitem.nam.letters[i] = ' ';

  catitem.line = libline;
  switch (catnumber) {

  case 1:
    dumpitem(c1, catitem);
    break;

  case 2:
    dumpitem(c2, catitem);
    break;

  case 3:
    dumpitem(c3, catitem);
    break;
  }


  fscanf(lib->f, "%*[^\n]");
  getc(lib->f);
  libline++;
}




/*

*/
Static Void dumptocat(chr, c1, c2, c3)
Char chr;
catfile *c1, *c2, *c3;
{
  /*








*/

  memcpy(catitem.nam.letters, aname.letters, sizeof(alpha));

  catitem.letter = chr;
  catitem.line = libline - 2;
  switch (catnumber) {

  case 1:
    dumpitem(c1, catitem);
    break;

  case 2:
    dumpitem(c2, catitem);
    break;

  case 3:
    dumpitem(c3, catitem);
    break;
  }
}




Static Void shortname(afile, n)
_TEXT *afile;
name n;
{
  long i = 1;

  while (n.letters[i-1] != ' ') {
    putc(n.letters[i-1], afile->f);
    i++;
  }
}


Static Void writehumcat(callingproc)
calltype callingproc;
{
  long fieldsize = 11;
  long l, FORLIM;

  putc('\n', humcat.f);
  humcatlines++;
  /*


*/

  if (humcatlines >= pagesize)
    dopage();


  FORLIM = levelsize * level;
  for (l = 1; l <= FORLIM; l++)
    putc(' ', humcat.f);
  /*


*/
  /*

*/
  switch (callingproc) {

  case organism:
    fprintf(humcat.f, "organism ");
    writename(&humcat, current.organism_.nam);
    fprintf(humcat.f, "%*c", namespace - current.organism_.nam.length, ' ');
    break;

  case chromosome:
    fprintf(humcat.f, "chromosome ");
    writename(&humcat, current.chromosome_.nam);
    fprintf(humcat.f, "%*c", namespace - current.chromosome_.nam.length, ' ');
    break;

  case marker:
    fprintf(humcat.f, "marker ");
    writename(&humcat, current.marker_.nam);
    fprintf(humcat.f, "%*c", namespace - current.marker_.nam.length, ' ');
    break;

  case transcript:
    fprintf(humcat.f, "transcript ");
    writename(&humcat, current.transcript_.nam);
    fprintf(humcat.f, "%*c", namespace - current.transcript_.nam.length, ' ');
    break;

  case gene:
    fprintf(humcat.f, "gene ");
    writename(&humcat, current.gene_.nam);
    fprintf(humcat.f, "%*c", namespace - current.gene_.nam.length, ' ');
    break;

  case piece:
    fprintf(humcat.f, "piece ");
    writename(&humcat, current.piece_.nam);
    fprintf(humcat.f, "%*c", namespace - current.piece_.nam.length, ' ');
    break;

  case recognitionclass:
    fprintf(humcat.f, "recognition-class ");
    writename(&humcat, current.recognition.nam);
    fprintf(humcat.f, "%*c", namespace - current.recognition.nam.length, ' ');
    break;

  case enzyme:
    fprintf(humcat.f, "enzyme ");
    writename(&humcat, current.enzyme_.nam);
    fprintf(humcat.f, "%*c", namespace - current.enzyme_.nam.length, ' ');
    break;

    /*
















*/

  }



  if (docomments)
    putc('\n', catin.f);
  switch (callingproc) {

  case organism:
    fprintf(catin.f, "organism ");
    writename(&catin, current.organism_.nam);
    break;

  case chromosome:
    fprintf(catin.f, "chromosome ");
    writename(&catin, current.chromosome_.nam);
    break;

  case marker:
    fprintf(catin.f, "marker ");
    writename(&catin, current.marker_.nam);
    break;

  case transcript:
    fprintf(catin.f, "transcript ");
    writename(&catin, current.transcript_.nam);
    break;

  case gene:
    fprintf(catin.f, "gene ");
    writename(&catin, current.gene_.nam);
    break;

  case piece:
    fprintf(catin.f, "piece ");
    writename(&catin, current.piece_.nam);
    break;

  case recognitionclass:
    fprintf(catin.f, "recognition-class");
    writename(&catin, current.recognition.nam);
    break;

  case enzyme:
    fprintf(catin.f, "enzyme ");
    writename(&catin, current.enzyme_.nam);
    break;
  }
  fprintf(catin.f, ";\n");

}



Static Void allocate(last)
namandlistptr **last;
{
  if (freelistptr != NULL) {
    *last = freelistptr;
    freelistptr = freelistptr->nextonlist;
  } else
    *last = (namandlistptr *)Malloc(sizeof(namandlistptr));

  (*last)->nam = none;
  (*last)->nextonlist = NULL;
}


Static Void return_(node)
namandlistptr **node;
{
  namandlistptr *lptr;

  if (*node == NULL)
    return;
  lptr = *node;
  *node = (*node)->nextonlist;
  lptr->nextonlist = freelistptr;
  freelistptr = lptr;
}


Static boolean equalname(a, b)
name *a, *b;
{
  /*

*/
  boolean equal;
  long index;

  if (a->length == b->length) {
    index = 1;
    do {
      equal = (a->letters[index-1] == b->letters[index-1]);
      index++;
    } while (equal && index <= a->length);
    return equal;
  } else
    return false;
}


Static Void pushname(the, latest)
namandlistptr **the, **latest;
{
  if (*the == NULL) {
    allocate(the);
    (*the)->nam = aname;
    (*the)->nextonlist = NULL;
    *latest = *the;
    return;
  }
  if (equalname(&(*the)->nam, &aname))
    unique = false;
  else
    pushname(&(*the)->nextonlist, latest);
}


Static Void popnames(the)
namandlistptr **the;
{
  if (*the == NULL)
    return;
  while ((*the)->nextonlist != NULL)
    popnames(&(*the)->nextonlist);
  return_(the);
}


/* Local variables for up: */
struct LOC_up {
  long endofnum;
} ;

Local Void shift(endofnum)
long endofnum;
{
  /*

*/
  long position, FORLIM;

  FORLIM = uniquebeg;
  for (position = endofnum; position > FORLIM; position--)
    line[position] = line[position-1];
  line[uniquebeg] = '1';

  catitem.nam.length++;
  length_++;
}

/*

*/
Local Void addcarry(position, LINK)
long position;
struct LOC_up *LINK;
{
  /*




*/
  line[position-1] = '0';
  position--;
  if (line[position-1] == specialchar) {
    shift(LINK->endofnum);
    return;
  }
  switch (line[position-1]) {

  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
    line[position-1]++;
    break;

  case '9':
    addcarry(position, LINK);
    break;
  }
}



Static Void up(position)
long position;
{
  /*
*/
  struct LOC_up V;

  while (line[position-1] == ' ' && position > uniquebeg)
    position--;

  switch (line[position-1]) {

  case ' ':
    catitem.nam.length += 2;
    length_ += 2;
    line[position-1] = specialchar;
    position++;
    line[position-1] = firstnumber;
    break;

  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
    line[position-1]++;
    break;

  case '9':
    V.endofnum = position;
    addcarry(V.endofnum, &V);
    break;
  }
}


Static Void changename(the, latest)
namandlistptr **the, **latest;
{
  /*





*/
  toolong = false;
  if (line[namelength+1] == ' ' || isdigit(line[namelength+1]))
    up(namelength + 2L);
  else
    toolong = true;
  getname();
  if (catitem.nam.length > namelength)
    toolong = true;
  else
    newnamelength = catitem.nam.length;
  if (toolong) {
    printf("New generated name is too long.\n");
    printf("Change namelength constant.\n");
    printf("namelength = %ld\n", (long)namelength);
    printf("namelength = %ld\n", (long)namelength);
    halt();
  }

  unique = true;
  pushname(the, latest);
}


Static Void duplicate(callingproc, root)
calltype callingproc;
namandlistptr **root;
{
  /*

*/
  _TEXT TEMP;

  printf(" duplicate ");
  switch (callingproc) {

  case organism:
    printf("organism");
    break;

  case recognitionclass:
    printf("recognition-class");
    break;

  case enzyme:
    printf("enzyme");
    break;

  case chromosome:
    printf("chromosome");
    break;

  case marker:
    printf("marker");
    break;

  case transcript:
    printf("transcript");
    break;

  case gene:
    printf("gene");
    break;

  case piece:
    printf("piece");
    break;
  }
  printf(" name found.\n");

  dumplists();

  printf(" old name was ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writename(&TEMP, aname);
  putchar('\n');
  uniquebeg = catitem.nam.length + 3;

  do {
    changename(root, &latest);
  } while (!unique);
  printf(" new name is  ");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writename(&TEMP, latest->nam);
  putchar('\n');
}


Static Void changeto(newname)
name newname;
{
  long i, difference, FORLIM;

  /*
*/
  getname();
  difference = newnamelength - catitem.nam.length;
  length_ += difference;
  FORLIM = length_ - catitem.nam.length - 2;
  /*
*/
  for (i = 0; i <= FORLIM; i++)
    line[length_ - i - 1] = line[length_ - i - difference - 1];
  FORLIM = newnamelength;
  for (i = 1; i <= FORLIM; i++)
    line[i+1] = newname.letters[i-1];
  /*





*/
  getname();
}




Static Void writedirect(f, d)
_TEXT *f;
direction d;
{
  switch (d) {

  case minus:
    fprintf(f->f, "%*s", dfield, "- ");
    break;

  case plus:
    fprintf(f->f, "%*s", dfield, "+ ");
    break;
  }
}


Static Void writeconfig(f, c)
_TEXT *f;
configuration c;
{
  switch (c) {

  case linear:
    fprintf(f->f, "%*s", cfield, "   linear");
    break;

  case circular:
    fprintf(f->f, "%*s", cfield, " circular");
    break;
  }
}


Static Void writepieceinfo(f, p)
_TEXT *f;
pieceinfo p;
{
  writeconfig(f, p.config);
  writedirect(f, p.direct);
  fprintf(f->f, " %*ld", nfield, p.beginning);
  fprintf(f->f, " %*ld", nfield, p.ending);
}


Static Void checkmap(geneticlocation)
double geneticlocation;
{
  /*
*/
  if (geneticlocation < mapbeg || mapend < geneticlocation)
    error(maplocation);
}


Static Void skipheader(lib, newlib)
_TEXT *lib, *newlib;
{
  checkstar(true);
  needlibline(lib, newlib, &length_);
  checkstar(false);

  if (P_peek(lib->f) != 'n')
    return;
  needlibline(lib, newlib, &length_);
  do {
    needlibline(lib, newlib, &length_);
    if (line[0] != '*') {
      if (line[0] != 'n') {
	erroratline();
	fprintf(humcat.f, " missing asterisk (*) in note\n");
	fprintf(catin.f, " missing asterisk (*) in note\n");
	humcatlines++;
	fatal++;
      }
    } else if (length_ > 1) {
      if (line[1] != ' ') {
	erroratline();
	fprintf(humcat.f, " missing blank in note\n");
	fprintf(catin.f, " missing blank in note\n");
	humcatlines++;
	fatal++;
      }
    }
  } while (line[0] != 'n');
}


Static Void piecekey(lib, newlib, coo, pie)
_TEXT *lib, *newlib;
pieceinfo *coo, *pie;
{
  boolean badtoken;
  double geneticmap;

  getreal(lib, newlib, &geneticmap, &badtoken);
  if (badtoken)
    error(genebeginning);

  getconfig(lib, newlib, &coo->config, &badtoken);
  if (badtoken)
    error(cooconfigurationbad);

  getdirect(lib, newlib, &coo->direct, &badtoken);
  if (badtoken)
    error(coordirectionbad);

  getinteger(lib, newlib, &coo->beginning, &badtoken);
  if (badtoken)
    error(coordbeginningbad);

  getinteger(lib, newlib, &coo->ending, &badtoken);
  if (badtoken)
    error(coordendingbad);

  getconfig(lib, newlib, &pie->config, &badtoken);
  if (badtoken)
    error(pieconfigurationbad);

  getdirect(lib, newlib, &pie->direct, &badtoken);
  if (badtoken)
    error(piedirectionbad);

  getinteger(lib, newlib, &pie->beginning, &badtoken);
  if (badtoken)
    error(piebeginningbad);

  getinteger(lib, newlib, &pie->ending, &badtoken);
  if (badtoken)
    error(pieendingbad);


  fprintf(humcat.f, "%*.2f", nfield, geneticmap);
  fprintf(humcat.f, " |");
  writepieceinfo(&humcat, *coo);
  fprintf(humcat.f, " |");
  writepieceinfo(&humcat, *pie);
  checkmap(geneticmap);


  if (docomments) {
    fprintf(catin.f, "(* coordinates: ");
    fprintf(catin.f, "%*.1f", nfield, geneticmap);
    fprintf(catin.f, " genetic map beginning\n");
    writepieceinfo(&catin, *coo);
    fprintf(catin.f, ": coordinate config, dir, beg, end\n");
    writepieceinfo(&catin, *pie);
    fprintf(catin.f, ": piece config, dir, beg, end *)\n");
  }


  checkmap(geneticmap);
}


Static Void writehumpieceinfo()
{
  fprintf(humcat.f, "%*s", cfield, "|   config");
  fprintf(humcat.f, "%*s", dfield, "dir");
  fprintf(humcat.f, "%*s", nfield, "begin");
  fprintf(humcat.f, "%*s", nfield, "end");
}


Local Void alignit()
{
  fprintf(humcat.f, "       %*c", levelsize + namespace, ' ');
}


Static Void piecehumhead()
{
  alignit();
  /*

*/
  fprintf(humcat.f, "genetic  ");
  fprintf(humcat.f, "| coordinate%*c", (int)(pfield - 10), ' ');
  fprintf(humcat.f, "| piece\n");
  humcatlines++;

  alignit();
  fprintf(humcat.f, "map      ");
  writehumpieceinfo();
  putc(' ', humcat.f);
  writehumpieceinfo();
  fprintf(humcat.f, "%*s", bfield, "length");
  /*




*/
}


Static Void mapbegend(lib, newlib)
_TEXT *lib, *newlib;
{
  boolean badtoken;

  getreal(lib, newlib, &mapbeg, &badtoken);
  if (badtoken)
    error(genebeginning);

  getreal(lib, newlib, &mapend, &badtoken);
  if (badtoken)
    error(geneending);

  /*


*/
  fprintf(humcat.f, " %*.2f %*.2f (genetic map range)\n",
	  nfield, mapbeg, nfield, mapend);
  if (docomments)
    fprintf(catin.f, "(* genetic map range is: %*.2f to %*.2f *)\n",
	    nfield, mapbeg, nfield, mapend);

  piecehumhead();
}


Static Void piereference(lib, newlib)
_TEXT *lib, *newlib;
{
  double geneticmap;
  boolean badtoken;

  needline(lib, &length_);
  /*
*/
  getname();
  if (equalname(&piecename, &none)) {
    changed = false;
    piecename = aname;
    unique = true;
    pushname(&first.piece_.nextonlist, &latest);
    if (!unique) {
      duplicate(piece, &first.piece_.nextonlist);
      newpiecename = latest->nam;
      changed = true;
    }
  } else if (!equalname(&piecename, &aname))
    error(nolastpiece);
  else if (changed)
    changeto(newpiecename);
  dumpline(newlib, length_);

  getreal(lib, newlib, &geneticmap, &badtoken);
  if (badtoken)
    error(mapbeginning);
  else
    checkmap(geneticmap);

  getdirect(lib, newlib, &pieceref.nodedir, &badtoken);
  if (badtoken)
    error(refdirectionbad);

  getinteger(lib, newlib, &pieceref.nodebeg, &badtoken);
  if (badtoken)
    error(refbeginningbad);

  getinteger(lib, newlib, &pieceref.nodeend, &badtoken);
  if (badtoken)
    error(refendingbad);


  /*

*/


  putc(' ', humcat.f);
  fprintf(humcat.f, "%*.2f", nfield, geneticmap);

  fprintf(humcat.f, "%*c", (int)(cfield + pfield), ' ');
  writedirect(&humcat, pieceref.nodedir);
  fprintf(humcat.f, "%*ld", nfield, pieceref.nodebeg);
  fprintf(humcat.f, "%*ld", nfield, pieceref.nodeend);

  fprintf(catin.f, "(* ");
  fprintf(catin.f, "%*.2f", nfield, geneticmap);

  writedirect(&catin, pieceref.nodedir);
  fprintf(catin.f, "%*ld", nfield, pieceref.nodebeg);
  fprintf(catin.f, "%*ld", nfield, pieceref.nodeend);
  fprintf(catin.f, " *)\n");
}


Static Void dosite(lib, newlib)
_TEXT *lib, *newlib;
{
  do {
    needlibline(lib, newlib, &length_);
  } while (line[0] != 's');
  needlibline(lib, newlib, &length_);
}


Static Void dodna(lib, newlib)
_TEXT *lib, *newlib;
{
  /*
*/
  long i;
  _TEXT TEMP;

  needlibline(lib, newlib, &length_);
  basecount = 0;
  while (line[0] == '*') {
    checkstar(true);
    i = 3;
    while (i <= length_) {
      if (line[i-1] == 'g' || line[i-1] == 't' || line[i-1] == 'c' ||
	  line[i-1] == 'a')
	basecount++;
      else if (line[i-1] != ' ') {
	erroratline();
	fprintf(humcat.f,
	  " DNA sequence contains \"%c\" which is not allowed in a delila library.\n",
	  line[i-1]);

	fprintf(humcat.f, " in piece ");
	writename(&humcat, latest->nam);
	fprintf(humcat.f, "\n\n");

	fprintf(catin.f,
	  " DNA sequence contains \"%c\" which is not allowed in a delila library.\n",
	  line[i-1]);

	fprintf(catin.f, " in piece ");
	writename(&catin, latest->nam);
	fprintf(catin.f, "\n\n");

	humcatlines += 3;
	fatal++;
      }
      i++;
    }
    needlibline(lib, newlib, &length_);
  }
  if (line[0] != 'd')
    strange(dna);


  if (!equalname(&current.piece_.nam, &none)) {
    putchar(' ');
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writename(&TEMP, current.piece_.nam);
    printf(" %6ld bp\n", basecount);
  } else if (!equalname(&current.marker_.nam, &none)) {
    putchar(' ');
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writename(&TEMP, current.marker_.nam);
    printf(" %6ld bp (a marker)\n", basecount);
  } else {
    printf("program error in dodna\n");
    halt();
  }

  fprintf(humcat.f, " %6ld bp", basecount);
  if (docomments)
    fprintf(catin.f, "(* total length: %ld bp *)\n", basecount);
  if (fromtoinst)
    fprintf(catin.f, "get from %ld to same +%ld;\n",
	    pie.beginning, basecount - 1);
  else
    fprintf(catin.f, "get all piece;\n");


  /*
*/
  needlibline(lib, newlib, &length_);
}




Static Void domarker(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 2;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.marker_.nextonlist, &latest);
  if (!unique)
    duplicate(marker, &first.marker_.nextonlist);
  current.marker_.nam = latest->nam;
  dumptocat('m', c1, c2, c3);
  dumpline(newlib, length_);
  writehumcat(marker);
  skipheader(lib, newlib);
  pieceref.nodetype = markref;
  pieceref.nodename = current.marker_.nam;
  piereference(lib, newlib);
  needlibline(lib, newlib, &length_);
  checkstar(true);
  needlibline(lib, newlib, &length_);
  checkstar(true);
  needlibline(lib, newlib, &length_);
  if (line[0] == 'd')
    dodna(lib, newlib);
  else
    strange(dna);
  if (line[0] != 'm')
    strange(marker);
  needlibline(lib, newlib, &length_);
  current.marker_.nam = none;
}


Static Void dogene(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 2;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.gene_.nextonlist, &latest);
  if (!unique)
    duplicate(gene, &first.gene_.nextonlist);
  dumpline(newlib, length_);
  dumptocat('g', c1, c2, c3);
  current.gene_.nam = latest->nam;
  writehumcat(gene);

  skipheader(lib, newlib);
  pieceref.nodetype = generef;
  pieceref.nodename = current.gene_.nam;
  piereference(lib, newlib);
  needlibline(lib, newlib, &length_);
  if (line[0] != 'g')
    strange(gene);
  needlibline(lib, newlib, &length_);
  current.gene_.nam = none;
}


Static Void dotranscript(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 2;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.transcript_.nextonlist, &latest);
  if (!unique)
    duplicate(transcript, &first.transcript_.nextonlist);
  dumpline(newlib, length_);
  dumptocat('t', c1, c2, c3);
  current.transcript_.nam = latest->nam;
  writehumcat(transcript);

  skipheader(lib, newlib);
  pieceref.nodetype = transref;
  pieceref.nodename = current.transcript_.nam;
  piereference(lib, newlib);
  needlibline(lib, newlib, &length_);
  if (line[0] != 't')
    strange(transcript);
  needlibline(lib, newlib, &length_);
  /*
*/
  current.transcript_.nam = none;
}


Static long checkcoordinates(piedir, piebeg, pieend, coobeg, cooend)
direction piedir;
long piebeg, pieend, coobeg, cooend;
{
  /*

*/
  long length;

  switch (piedir) {

  case plus:
    if (pieend >= piebeg)
      length = pieend - piebeg + 1;
    else
      length = pieend - coobeg + cooend - piebeg + 2;
    break;

  case minus:
    if (pieend <= piebeg)
      length = piebeg - pieend + 1;
    else
      length = cooend - pieend + piebeg - coobeg + 2;
    break;
  }
  return length;
}


Static Void dopiece(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  long predictedlength;

  /*
*/
  level = 2;
  needline(lib, &length_);
  getname();
  if (equalname(&piecename, &none)) {
    /*
*/
    if (debugging)
      error(noreference);
    unique = true;
    pushname(&first.piece_.nextonlist, &latest);
    if (!unique) {
      duplicate(piece, &first.piece_.nextonlist);
      changed = true;
      newpiecename = latest->nam;
    }
  } else {
    if (!equalname(&piecename, &aname))
      error(wrongreference);
    else if (changed)
      changeto(newpiecename);
  }
  dumptocat('p', c1, c2, c3);
  if (!changed)
    current.piece_.nam = aname;
  else
    current.piece_.nam = newpiecename;
  piecefound = true;
  piecename = none;
  dumpline(newlib, length_);

  writehumcat(piece);
  skipheader(lib, newlib);
  piecekey(lib, newlib, &coo, &pie);
  needlibline(lib, newlib, &length_);
  if (line[0] == 'd')
    dodna(lib, newlib);
  else
    strange(dna);

  /*

*/
  predictedlength = checkcoordinates(pie.direct, pie.beginning, pie.ending,
				     coo.beginning, coo.ending);
  if (predictedlength != basecount) {
    erroratline();
    fprintf(humcat.f,
	    " length of piece predicted from piece coordinates was: %5ld\n",
	    predictedlength);
    fprintf(humcat.f, " the actual number of bases counted was:%20ld\n",
	    basecount);
    fprintf(humcat.f, " the difference is%42ld\n",
	    predictedlength - basecount);
    fprintf(humcat.f, " for piece ");
    writename(&humcat, current.piece_.nam);
    putc('\n', humcat.f);

    fprintf(catin.f,
	    " length of piece predicted from piece coordinates was: %5ld\n",
	    predictedlength);
    fprintf(catin.f, " the actual number of bases counted was:%20ld\n",
	    basecount);
    fprintf(catin.f, " the difference is%42ld\n", predictedlength - basecount);
    fprintf(catin.f, " for piece ");
    writename(&catin, current.piece_.nam);
    putc('\n', catin.f);


    /*







*/
    humcatlines += 4;
    fatal++;
  }

  /*
*/
  if (coo.beginning > coo.ending) {
    erroratline();
    fprintf(humcat.f,
	    " coordinate beginning must be less than or equal to ending\n");
    fprintf(catin.f,
	    " coordinate beginning must be less than or equal to ending\n");
    humcatlines++;
    fatal++;
  }

  /*
*/
  if (coo.config == linear) {
    if (pie.config != linear) {
      erroratline();
      fprintf(humcat.f, " linear coordinates imply linear pieces\n");
      fprintf(catin.f, " linear coordinates imply linear pieces\n");
      humcatlines++;
      fatal++;
    }
  }


  if (line[0] != 'p')
    strange(piece);
  needlibline(lib, newlib, &length_);
  current.piece_.nam = none;

  /*





*/

  /*
*/
  changed = false;
}


Static Void dochromosome(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 1;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.chromosome_.nextonlist, &latest);
  if (!unique)
    duplicate(chromosome, &first.chromosome_.nextonlist);
  dumptocat('c', c1, c2, c3);
  dumpline(newlib, length_);
  current.chromosome_.nam = latest->nam;
  writehumcat(chromosome);
  skipheader(lib, newlib);
  mapbegend(lib, newlib);
  needlibline(lib, newlib, &length_);
  while (line[0] == 'p' || line[0] == 'g' || line[0] == 't' || line[0] == 'm') {
    if (line[0] == 'm') {
      domarker(lib, newlib, c1, c2, c3);
      continue;
    }
    if (line[0] == 't')
      dotranscript(lib, newlib, c1, c2, c3);
    else {
      if (line[0] == 'g')
	dogene(lib, newlib, c1, c2, c3);
      else
	dopiece(lib, newlib, c1, c2, c3);
    }
  }
  if (line[0] != 'c') {
    /*
*/
    strange(chromosome);
  }

  /*
*/
  popnames(&first.marker_.nextonlist);
  popnames(&first.transcript_.nextonlist);
  popnames(&first.gene_.nextonlist);
  popnames(&first.piece_.nextonlist);
  needlibline(lib, newlib, &length_);
  current.chromosome_.nam = none;
}


Static Void doenzyme(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 1;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.enzyme_.nextonlist, &latest);
  if (!unique)
    duplicate(enzyme, &first.transcript_.nextonlist);
  dumptocat('e', c1, c2, c3);
  dumpline(newlib, length_);
  current.enzyme_.nam = latest->nam;
  writehumcat(enzyme);
  skipheader(lib, newlib);
  needlibline(lib, newlib, &length_);
  needlibline(lib, newlib, &length_);
  while (line[0] == 's')
    dosite(lib, newlib);
  if (line[0] != 'e')
    strange(enzyme);
  needlibline(lib, newlib, &length_);
  current.enzyme_.nam = none;
}


Static Void doorganism(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  long i, FORLIM;

  level = 0;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.organism_.nextonlist, &latest);
  if (!unique)
    duplicate(organism, &first.organism_.nextonlist);
  dumptocat('o', c1, c2, c3);
  dumpline(newlib, length_);
  current.organism_.nam = latest->nam;
  writehumcat(organism);
  skipheader(lib, newlib);
  needlibline(lib, newlib, &length_);
  checkstar(true);


  putc(' ', humcat.f);
  FORLIM = length_;
  for (i = 2; i < FORLIM; i++)
    putc(line[i], humcat.f);
  fprintf(humcat.f, " (genetic map units)");

  if (docomments) {
    fprintf(catin.f, "(* genetic map units are: ");
    FORLIM = length_;
    for (i = 2; i < FORLIM; i++)
      putc(line[i], catin.f);
    fprintf(catin.f, " *)\n");
  }

  needlibline(lib, newlib, &length_);
  while (line[0] == 'c')
    dochromosome(lib, newlib, c1, c2, c3);
  if (line[0] != 'o')
    strange(organism);
  popnames(&first.chromosome_.nextonlist);
  current.organism_.nam = none;
}


Static Void dorecognition(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  level = 0;
  needline(lib, &length_);
  getname();
  unique = true;
  pushname(&first.recognition.nextonlist, &latest);
  if (!unique)
    duplicate(recognitionclass, &first.recognition.nextonlist);
  dumptocat('r', c1, c2, c3);
  dumpline(newlib, length_);
  current.recognition.nam = latest->nam;
  writehumcat(recognitionclass);
  skipheader(lib, newlib);
  needlibline(lib, newlib, &length_);
  checkstar(true);
  needlibline(lib, newlib, &length_);
  while (line[0] == 'e')
    doenzyme(lib, newlib, c1, c2, c3);
  if (line[0] != 'r')
    strange(recognitionclass);
  popnames(&first.enzyme_.nextonlist);
  /*
*/
  current.recognition.nam = none;
  /*
*/
}


Static Void dolibrary(lib, newlib, c1, c2, c3)
_TEXT *lib, *newlib;
catfile *c1, *c2, *c3;
{
  if (*lib->name != '\0') {
    if (lib->f != NULL)
      lib->f = freopen(lib->name, "r", lib->f);
    else
      lib->f = fopen(lib->name, "r");
  } else
    rewind(lib->f);
  if (lib->f == NULL)
    _EscIO2(FileNotFound, lib->name);
  RESETBUF(lib->f, Char);
  if (*newlib->name != '\0') {
    if (newlib->f != NULL)
      newlib->f = freopen(newlib->name, "w", newlib->f);
    else
      newlib->f = fopen(newlib->name, "w");
  } else {
    if (newlib->f != NULL)
      rewind(newlib->f);
    else
      newlib->f = tmpfile();
  }
  if (newlib->f == NULL)
    _EscIO2(FileNotFound, newlib->name);
  SETUPBUF(newlib->f, Char);
  libdone = false;
  libline = 1;


  if (!BUFEOF(lib->f))
    readlibdate(lib, newlib, c1, c2, c3);

  while (!BUFEOF(lib->f)) {
    readline(lib, &length_);
    dumpline(newlib, length_);
    if (libdone)
      continue;
    if (line[0] == 'o')
      doorganism(lib, newlib, c1, c2, c3);
    else if (line[0] == 'r')
      dorecognition(lib, newlib, c1, c2, c3);
    else
      strange(library);
  }
  libnumber++;
  catnumber++;
}


Static Void themain(catalp, l1, c1, newl1, l2, c2, newl2, l3, c3, newl3,
		    humcat, catin, fout)
_TEXT *catalp, *l1;
catfile *c1;
_TEXT *newl1, *l2;
catfile *c2;
_TEXT *newl2, *l3;
catfile *c3;
_TEXT *newl3, *humcat, *catin, *fout;
{
  fprintf(fout->f, " catal %4.2f ", version);

  getdatetime(daytime);
  writedatetime(fout, daytime);
  putc('\n', fout->f);

  maxnamelength = 0;


  if (*l1->name != '\0') {
    if (l1->f != NULL)
      l1->f = freopen(l1->name, "r", l1->f);
    else
      l1->f = fopen(l1->name, "r");
  } else
    rewind(l1->f);
  if (l1->f == NULL)
    _EscIO2(FileNotFound, l1->name);
  RESETBUF(l1->f, Char);
  if (*l2->name != '\0') {
    if (l2->f != NULL)
      l2->f = freopen(l2->name, "r", l2->f);
    else
      l2->f = fopen(l2->name, "r");
  } else
    rewind(l2->f);
  if (l2->f == NULL)
    _EscIO2(FileNotFound, l2->name);
  RESETBUF(l2->f, Char);
  if (*l3->name != '\0') {
    if (l3->f != NULL)
      l3->f = freopen(l3->name, "r", l3->f);
    else
      l3->f = fopen(l3->name, "r");
  } else
    rewind(l3->f);
  if (l3->f == NULL)
    _EscIO2(FileNotFound, l3->name);
  RESETBUF(l3->f, Char);
  if (!((!BUFEOF(l1->f)) | (!BUFEOF(l2->f)) | (!BUFEOF(l3->f)))) {
    docathelp(fout);
    return;
  }
  initializecatalogs(catalp, c1, c2, c3);


  dolibrary(l1, newl1, c1, c2, c3);
  dolibrary(l2, newl2, c1, c2, c3);
  dolibrary(l3, newl3, c1, c2, c3);

  /*
*/
  if (fatal == 0) {
    putc('\n', humcat->f);
    return;
  }

  if (*newl1->name != '\0') {
    if (newl1->f != NULL)
      newl1->f = freopen(newl1->name, "w", newl1->f);
    else
      newl1->f = fopen(newl1->name, "w");
  } else {
    if (newl1->f != NULL)
      rewind(newl1->f);
    else
      newl1->f = tmpfile();
  }
  if (newl1->f == NULL)
    _EscIO2(FileNotFound, newl1->name);
  SETUPBUF(newl1->f, Char);
  if (*newl2->name != '\0') {
    if (newl2->f != NULL)
      newl2->f = freopen(newl2->name, "w", newl2->f);
    else
      newl2->f = fopen(newl2->name, "w");
  } else {
    if (newl2->f != NULL)
      rewind(newl2->f);
    else
      newl2->f = tmpfile();
  }
  if (newl2->f == NULL)
    _EscIO2(FileNotFound, newl2->name);
  SETUPBUF(newl2->f, Char);
  if (*newl3->name != '\0') {
    if (newl3->f != NULL)
      newl3->f = freopen(newl3->name, "w", newl3->f);
    else
      newl3->f = fopen(newl3->name, "w");
  } else {
    if (newl3->f != NULL)
      rewind(newl3->f);
    else
      newl3->f = tmpfile();
  }
  if (newl3->f == NULL)
    _EscIO2(FileNotFound, newl3->name);
  SETUPBUF(newl3->f, Char);
  if (*c1->name != '\0') {
    if (c1->f != NULL)
      c1->f = freopen(c1->name, "wb", c1->f);
    else
      c1->f = fopen(c1->name, "wb");
  } else {
    if (c1->f != NULL)
      rewind(c1->f);
    else
      c1->f = tmpfile();
  }
  if (c1->f == NULL)
    _EscIO2(FileNotFound, c1->name);
  SETUPBUF(c1->f, item);
  if (*c2->name != '\0') {
    if (c2->f != NULL)
      c2->f = freopen(c2->name, "wb", c2->f);
    else
      c2->f = fopen(c2->name, "wb");
  } else {
    if (c2->f != NULL)
      rewind(c2->f);
    else
      c2->f = tmpfile();
  }
  if (c2->f == NULL)
    _EscIO2(FileNotFound, c2->name);
  SETUPBUF(c2->f, item);
  if (*c3->name != '\0') {
    if (c3->f != NULL)
      c3->f = freopen(c3->name, "wb", c3->f);
    else
      c3->f = fopen(c3->name, "wb");
  } else {
    if (c3->f != NULL)
      rewind(c3->f);
    else
      c3->f = tmpfile();
  }
  if (c3->f == NULL)
    _EscIO2(FileNotFound, c3->name);
  SETUPBUF(c3->f, item);
  fprintf(fout->f,
	  " %ld fatal error(s): new libraries and catalogues destroyed\n",
	  fatal);
  fprintf(humcat->f,
	  " %ld fatal error(s): new libraries and catalogues destroyed\n",
	  fatal);
  fprintf(catin->f,
	  " %ld fatal error(s): new libraries and catalogues destroyed\n",
	  fatal);
  fprintf(fout->f, " see humcat.\n");
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  lib3.f = NULL;
  strcpy(lib3.name, "lib3");
  lib2.f = NULL;
  strcpy(lib2.name, "lib2");
  lib1.f = NULL;
  strcpy(lib1.name, "lib1");
  l3.f = NULL;
  strcpy(l3.name, "l3");
  l2.f = NULL;
  strcpy(l2.name, "l2");
  l1.f = NULL;
  strcpy(l1.name, "l1");
  catin.f = NULL;
  strcpy(catin.name, "catin");
  humcat.f = NULL;
  strcpy(humcat.name, "humcat");
  cat3.f = NULL;
  strcpy(cat3.name, "cat3");
  cat2.f = NULL;
  strcpy(cat2.name, "cat2");
  cat1.f = NULL;
  strcpy(cat1.name, "cat1");
  catalp.f = NULL;
  strcpy(catalp.name, "catalp");
  TEMP.f = stdout;
  *TEMP.name = '\0';
  themain(&catalp, &l1, &cat1, &lib1, &l2, &cat2, &lib2, &l3, &cat3, &lib3,
	  &humcat, &catin, &TEMP);
_L1:
  if (catalp.f != NULL)
    fclose(catalp.f);
  if (cat1.f != NULL)
    fclose(cat1.f);
  if (cat2.f != NULL)
    fclose(cat2.f);
  if (cat3.f != NULL)
    fclose(cat3.f);
  if (humcat.f != NULL)
    fclose(humcat.f);
  if (catin.f != NULL)
    fclose(catin.f);
  if (l1.f != NULL)
    fclose(l1.f);
  if (l2.f != NULL)
    fclose(l2.f);
  if (l3.f != NULL)
    fclose(l3.f);
  if (lib1.f != NULL)
    fclose(lib1.f);
  if (lib2.f != NULL)
    fclose(lib2.f);
  if (lib3.f != NULL)
    fclose(lib3.f);
  exit(EXIT_SUCCESS);
}



/* End. */
