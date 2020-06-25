/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "loocat.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*




*/


#define version         1.18
/*




*/



/*






























*/


#define namelength      200


typedef Char alpha[namelength];

typedef struct name {
  alpha letters;
  uchar length;
} name;

typedef struct item {
  Char nodeletter;
  name nam;
  long line;
} item;

typedef struct recordfile {
  FILE *f;
  FILEBUFNC(f,item);
  Char name[_FNSIZE];
} recordfile;


main(argc, argv)
int argc;
Char *argv[];
{
  recordfile cat;
  _TEXT list;
  item x;

  PASCAL_MAIN(argc, argv);
  list.f = NULL;
  strcpy(list.name, "list");
  cat.f = NULL;
  strcpy(cat.name, "cat");
  printf(" loocat %4.2f\n", version);

  if (*list.name != '\0')
    list.f = fopen(list.name, "w");
  else
    list.f = tmpfile();
  if (list.f == NULL)
    _EscIO2(FileNotFound, list.name);
  SETUPBUF(list.f, Char);
  fprintf(list.f, " loocat %4.2f look at a catalogue\n\n", version);
  if (*cat.name != '\0')
    cat.f = fopen(cat.name, "rb");
  else
    rewind(cat.f);
  if (cat.f == NULL)
    _EscIO2(FileNotFound, cat.name);
  RESETBUF(cat.f, item);

  if (!BUFEOF(cat.f)) {
    fprintf(list.f, " length  line id name%*c\n", (int)(namelength - 5), ' ');

    while (!BUFEOF(cat.f)) {
      fread(&x, sizeof(item), 1, cat.f);
      fprintf(list.f, " %5d", x.nam.length);
      fprintf(list.f, " %5ld", x.line);
      fprintf(list.f, " %c ", x.nodeletter);
      if (x.nam.length > namelength) {
	printf("warning: name length (%12d) is too big\n", x.nam.length);
/* p2c: loocat.p, line 104: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
	fprintf(list.f, " %.*s", namelength, x.nam.letters);
      } else
	fprintf(list.f, " %.*s", x.nam.length, x.nam.letters);
/* p2c: loocat.p, line 107: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
      putc('\n', list.f);
    }
  } else
    fprintf(list.f, " empty file\n");
  fclose(cat.f);
  fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
