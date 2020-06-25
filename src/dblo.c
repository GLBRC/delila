/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dblo.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.11
/*

*/



/*




























*/



#define linewidth       10
/*
*/
#define libwidth        10
/*
*/





#define idlength        20
#define namelength      idlength
#define lclength        3
#define libtotal        8
#define lctotal         20
#define checknum        1

/*



*/

#define spraw           "RAW"
#define spall           "ALL"
#define lcloc           "LOC"
/*
*/
#define lcid            "ID "
#define lc3spc          "   "
/*
*/
#define lcxx            "XX "
/*
*/
#define lct             "  T"
/*
*/
#define lca             "  A"
/*
*/
#define lcj             "  J"
#define lcori           "ORI"
/*
*/
#define lcsit           "SIT"
#define lcterm          "// "
#define lcdat           "DAT"
#define idembl          "EMBL                "
/*
*/
#define idgenb          "GENB                "
/*
*/
#define idgenbank       "GENBANK             "
#define iddate          "DATE                "
/*
*/
#define idevery         "EVERY               "


/*
*/





/*

*/
typedef Char idutype[idlength];

typedef Char idptype[idlength];

typedef idptype alpha;
typedef char lnrange;


typedef struct catrec {
  idptype idp;
  lnrange libnum;
  /*
*/
  long linenumber;
  /*
*/
} catrec;

typedef struct dbcat {
  FILE *f;
  FILEBUFNC(f,catrec);
  Char name[_FNSIZE];
} dbcat;

typedef Char lcutype[lclength];

typedef Char lcptype[lclength];

typedef Char lcstype[lctotal][lclength];

typedef char countype;

typedef enum {
  embl, genb
} libsused;
/*
*/
typedef enum {
  notwc, xwc, wcx, xwcx
} wctype;

/*


*/




Static dbcat cat;
Static _TEXT list;


Static jmp_buf _JL1;



Static Void halt()
{
  /*





*/
  printf(" PROGRAM HALT.\n");
  longjmp(_JL1, 1);
}



Static Void themain(cat, list)
dbcat *cat;
_TEXT *list;
{
  catrec anentry;
  long entries = 0;
  long i;

  printf(" DBLO %4.2f\n", version);

  if (*cat->name != '\0') {
    if (cat->f != NULL)
      cat->f = freopen(cat->name, "rb", cat->f);
    else
      cat->f = fopen(cat->name, "rb");
  } else
    rewind(cat->f);
  if (cat->f == NULL)
    _EscIO2(FileNotFound, cat->name);
  RESETBUF(cat->f, catrec);
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

  fprintf(list->f, " DBLO %4.2f: LOOK AT A DATABASE CATALOGUE\n", version);
  fprintf(list->f, " ID%*c%*cLIBRARY%*cLINE\n",
	  (int)(idlength - 2), ' ', (int)(libwidth - 6), ' ',
	  (int)(linewidth - 3), ' ');

  while (!BUFEOF(cat->f)) {
    entries++;
    anentry = GETFBUF(cat->f, catrec);
    putc(' ', list->f);
    for (i = 0; i < idlength; i++)
      putc(anentry.idp[i], list->f);
    fprintf(list->f, " %*d", libwidth, anentry.libnum);
    fprintf(list->f, " %*ld\n", linewidth, anentry.linenumber);
    GET(cat->f, catrec);
  }

  printf(" %ld ENTRIES IN THE CATALOGUE\n", entries);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  list.f = NULL;
  strcpy(list.name, "list");
  cat.f = NULL;
  strcpy(cat.name, "cat");
  themain(&cat, &list);
_L1:
  if (cat.f != NULL)
    fclose(cat.f);
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
