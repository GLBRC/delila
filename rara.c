/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rara.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*
*/



/*








































*/



#define maxdata         10000

#define infofield       12
#define infodecim       6

#define nfield          6



typedef struct datatype {
  double sortdata, x, y;
  long xrank, yrank;
} datatype;


typedef short position;



typedef datatype maptype[maxdata];


Static _TEXT data, xyin;
Static maptype map;


Static jmp_buf _JL1;


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



Static boolean lessthan(a, b)
position a, b;
{
  return (map[a-1].sortdata < map[b-1].sortdata);
}


Static Void swap_(a, b)
position a, b;
{
  datatype hold;

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





Static Void themain(data, xyin)
_TEXT *data, *xyin;
{
  position number = 0;
  position p;

  printf("rara %4.2f\n", version);
  if (*xyin->name != '\0') {
    if (xyin->f != NULL)
      xyin->f = freopen(xyin->name, "w", xyin->f);
    else
      xyin->f = fopen(xyin->name, "w");
  } else {
    if (xyin->f != NULL)
      rewind(xyin->f);
    else
      xyin->f = tmpfile();
  }
  if (xyin->f == NULL)
    _EscIO2(FileNotFound, xyin->name);
  SETUPBUF(xyin->f, Char);
  fprintf(xyin->f, "* rara %4.2f\n", version);

  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "r", data->f);
    else
      data->f = fopen(data->name, "r");
  } else
    rewind(data->f);
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  RESETBUF(data->f, Char);
  while (!BUFEOF(data->f)) {
    if (P_peek(data->f) == '*')
      copyaline(data, xyin);
    else {
      number++;
      fscanf(data->f, "%lg%lg%*[^\n]", &map[number-1].x, &map[number-1].y);
      getc(data->f);
    }
  }


  for (p = 0; p <= number - 1; p++)
    map[p].sortdata = map[p].y;

  quicksort(1, number);

  for (p = 1; p <= number; p++) {
    map[p-1].xrank = 0;
    map[p-1].yrank = p;
  }


  for (p = 0; p <= number - 1; p++)
    map[p].sortdata = map[p].x;

  quicksort(1, number);

  for (p = 1; p <= number; p++)
    map[p-1].xrank = p;

  for (p = 0; p <= number - 1; p++) {
    fprintf(xyin->f, " %*.*f", infofield, infodecim, map[p].x);
    fprintf(xyin->f, " %*.*f", infofield, infodecim, map[p].y);
    fprintf(xyin->f, " %*ld", nfield, map[p].xrank);
    fprintf(xyin->f, " %*ld\n", nfield, map[p].yrank);
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &xyin);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
