/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "pizza.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.30
/*


*/



/*























































































*/



#define tinside         200

#define shrink          0.9
/*
*/
#define toparea         0.8


typedef struct tin {

  long height, width;
  Char surface[tinside + 1][tinside + 1];
} tin;


Static _TEXT pizzap, epizza;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define pow14           16384

#define pow15           32768L
#define pow22           4194304L
#define pow23           8388608L



Static Void randomDelila(seed)
double *seed;
{
  /*



*/

  /*











*/
  long iseed, i, nrep;

  iseed = (long)floor(*seed * pow23 + 0.5);
  if (iseed < 1 || iseed >= pow23)
    iseed = 1;
  nrep = (iseed & 7) + 4;
  for (i = 1; i <= nrep; i++) {
    if ((iseed & 1) == ((iseed & (pow15 - 1)) >= pow14))
      iseed /= 2;
    else
      iseed = iseed / 2 + pow22;
  }

  *seed = (double)iseed / pow23;
}

#undef pow14
#undef pow15
#undef pow22
#undef pow23



Static Void wash(plate, height, width)
tin *plate;
long height, width;
{
  long column, line, FORLIM, FORLIM1;

  plate->height = height;
  plate->width = width;
  FORLIM = plate->height;
  for (line = 1; line <= FORLIM; line++) {
    FORLIM1 = plate->width;
    for (column = 1; column <= FORLIM1; column++)
      plate->surface[line][column] = ' ';
  }
}



Static boolean ellipse(x, y, A, B, r)
double x, y, A, B, r;
{
  double TEMP, TEMP1;

  TEMP = (x - A / 2) / (A / 2);
  TEMP1 = (y - B / 2) / (B / 2);
  return (TEMP * TEMP + TEMP1 * TEMP1 <= r);
}



Static Void adddough(plate)
tin *plate;
{
  long column, line, FORLIM, FORLIM1;

  FORLIM = plate->height;
  for (line = 1; line <= FORLIM; line++) {
    FORLIM1 = plate->width;
    for (column = 1; column <= FORLIM1; column++) {
      if (ellipse((double)line, (double)column, (double)plate->height,
		  (double)plate->width, shrink))
	plate->surface[line][column] = '.';
    }
  }
}



Static Void addsauceandcheese(plate, fraction, seed)
tin *plate;
double fraction, *seed;
{
  /*
*/
  long column, line, FORLIM, FORLIM1;

  FORLIM = plate->height;
  for (line = 1; line <= FORLIM; line++) {
    FORLIM1 = plate->width;
    for (column = 1; column <= FORLIM1; column++) {
      if (ellipse((double)line, (double)column, (double)plate->height,
		  (double)plate->width, shrink * toparea)) {
	randomDelila(seed);
	if (*seed <= fraction)
	  plate->surface[line][column] = '~';
	randomDelila(seed);
	if (*seed <= 1 - fraction)
	  plate->surface[line][column] = '=';
      }
    }
  }
}


#define kinds           15



Static Void addtoppings(plate, toppings, seed)
tin *plate;
long toppings;
double *seed;
{
  /*
*/
  long column, line;
  long toss = 0;
  long t;

  while (toss < toppings) {
    randomDelila(seed);
    line = (long)(*seed * plate->height);
    randomDelila(seed);
    column = (long)(*seed * plate->width);

    if (!ellipse((double)line, (double)column, (double)plate->height,
		 (double)plate->width, shrink * toparea))
      continue;

    randomDelila(seed);

    t = (long)floor(kinds * *seed + 0.5);
    switch (t) {

    case 0:
      plate->surface[line][column] = '*';
      break;

    case 1:
      plate->surface[line][column] = 'O';
      break;

    case 2:
      plate->surface[line][column] = '&';
      break;

    case 3:
      plate->surface[line][column] = '[';
      break;

    case 4:
      plate->surface[line][column] = ']';
      break;

    case 5:
      plate->surface[line][column] = '=';
      plate->surface[line][column+1] = ')';
      break;

    case 6:
      plate->surface[line][column] = '(';
      plate->surface[line][column+1] = '=';
      break;

    case 7:
      plate->surface[line][column] = '/';
      plate->surface[line][column+1] = '\\';
      break;

    case 8:
      plate->surface[line][column] = '$';
      break;

    case 9:
      plate->surface[line][column] = '{';
      break;

    case 10:
      plate->surface[line][column] = '}';
      break;

    case 11:
      plate->surface[line][column] = 'R';
      break;

    case 12:
      plate->surface[line][column] = '@';
      break;

    case 13:
      plate->surface[line][column] = '#';
      break;

    case 14:
      plate->surface[line][column] = 'H';
      break;

    case 15:
      plate->surface[line][column] = '!';
      break;
    }
    toss++;
  }
}

#undef kinds



Static Void key(customer)
_TEXT *customer;
{
  fprintf(customer->f, "\nkey to this electronic pizza:\n");
  fprintf(customer->f, " . crust\n");
  fprintf(customer->f, " ~ sauce\n");
  fprintf(customer->f, " = cheese\n");
  fprintf(customer->f, " * meatball\n");
  fprintf(customer->f, " O olive\n");
  fprintf(customer->f,
	  " & anchovy (lop off the top part and you have a little fish)\n");
  fprintf(customer->f, " ] [ green peppers\n");
  fprintf(customer->f, " } { red peppers\n");
  fprintf(customer->f, " =) (= mushrooms\n");
  fprintf(customer->f, " /\\ pineapple\n");
  fprintf(customer->f, " $ coin (heads or tails, 8 bits = 1 byte)\n");
  fprintf(customer->f,
	  " R red herring (the head is on top, tail on bottom)\n");
  fprintf(customer->f, " @ pepperoni\n");
  /*


*/
  fprintf(customer->f, " # spinach\n");
  fprintf(customer->f, " H ham (aka pork)\n");
  fprintf(customer->f, " ! little dried hot pepper seeds\n\n");
  /*
*/

  fprintf(customer->f, "Your chef, Pizza version %4.2f", version);
  fprintf(customer->f, ", says:  \"Have a Byte!\"\n\n");
}


#define pi              3.14159



Static Void toangle(x, y, angle)
double x, y, *angle;
{
  double d = 360 / (2 * pi);

  if (y == 0.0) {
    if (x >= 0.0)
      *angle = 0.0;
    else if (x < 0.0)
      *angle = 180.0;
    return;
  }
  if (x == 0.0) {
    if (y >= 0.0)
      *angle = 90.0;
    else if (y < 0.0)
      *angle = 270.0;
    return;
  }


  if (x > 0.0 && y > 0.0) {
    *angle = atan(y / x) * d;

    return;
  }
  if (x < 0.0 && y > 0.0) {
    *angle = 180 - atan(y / -x) * d;
    return;
  }
  if (x < 0.0 && y < 0.0)
    *angle = 180 + atan(y / x) * d;
  else if (x > 0.0 && y < 0.0)
    *angle = 270 + atan(x / -y) * d;
}

#undef pi



Static Void slice(plate, lowangle, highangle)
tin *plate;
double lowangle, highangle;
{
  /*
*/
  double angle;
  long column;
  double h2;
  long line;
  double w2, x, y;
  long FORLIM, FORLIM1;

  h2 = plate->height / 2.0;
  w2 = plate->width / 2.0;
  FORLIM = plate->height;
  for (line = 1; line <= FORLIM; line++) {
    y = plate->height - line + 1 - h2;
    FORLIM1 = plate->width;
    for (column = 1; column <= FORLIM1; column++) {
      x = column - w2;
      toangle(x, y, &angle);

      if (lowangle > angle || angle > highangle)
	plate->surface[line][column] = ' ';
    }
  }
}



Static Void serve(customer, plate)
_TEXT *customer;
tin plate;
{
  long column;
  boolean foundstart = false, foundstop = false;
  long line, maximumprinting;
  /*
*/
  long startline = 0;
  long stopline;

  while (!foundstart && startline < plate.height) {
    startline++;
    column = 0;
    while (column < plate.width && !foundstart) {
      column++;
      if (plate.surface[startline][column] != ' ')
	foundstart = true;
    }
  }


  stopline = plate.height;
  while (!foundstop && stopline > 1) {
    stopline--;
    column = 0;
    while (column < plate.width && !foundstop) {
      column++;
      if (plate.surface[stopline][column] != ' ')
	foundstop = true;
    }
  }


  for (line = startline; line <= stopline; line++) {
    maximumprinting = plate.width;
    do {
      maximumprinting--;
    } while (plate.surface[line][maximumprinting] == ' ' &&
	     maximumprinting != 1);

    for (column = 1; column <= maximumprinting; column++)
      putc(plate.surface[line][column], customer->f);
    putc('\n', customer->f);
  }

}



Static Void themain(pizzap, epizza)
_TEXT *pizzap, *epizza;
{
  double highangle, lowangle;
  tin plate;
  long height;
  double seed, sauce;
  long toppings, width;

  printf("pizza %4.2f\n", version);

  if (*pizzap->name != '\0') {
    if (pizzap->f != NULL)
      pizzap->f = freopen(pizzap->name, "r", pizzap->f);
    else
      pizzap->f = fopen(pizzap->name, "r");
  } else
    rewind(pizzap->f);
  if (pizzap->f == NULL)
    _EscIO2(FileNotFound, pizzap->name);
  RESETBUF(pizzap->f, Char);
  fscanf(pizzap->f, "%ld%*[^\n]", &height);
  getc(pizzap->f);
  fscanf(pizzap->f, "%ld%*[^\n]", &width);
  getc(pizzap->f);
  fscanf(pizzap->f, "%lg%*[^\n]", &sauce);
  getc(pizzap->f);
  fscanf(pizzap->f, "%ld%*[^\n]", &toppings);
  getc(pizzap->f);
  fscanf(pizzap->f, "%lg%*[^\n]", &seed);
  getc(pizzap->f);
  if (height > tinside || width > tinside) {
    printf("requested pizza too big\n");
    halt();
  }
  fscanf(pizzap->f, "%lg%*[^\n]", &lowangle);
  getc(pizzap->f);
  fscanf(pizzap->f, "%lg%*[^\n]", &highangle);
  getc(pizzap->f);

  if (*epizza->name != '\0') {
    if (epizza->f != NULL)
      epizza->f = freopen(epizza->name, "w", epizza->f);
    else
      epizza->f = fopen(epizza->name, "w");
  } else {
    if (epizza->f != NULL)
      rewind(epizza->f);
    else
      epizza->f = tmpfile();
  }
  if (epizza->f == NULL)
    _EscIO2(FileNotFound, epizza->name);
  SETUPBUF(epizza->f, Char);

  wash(&plate, height, width);
  adddough(&plate);
  addsauceandcheese(&plate, seed, &sauce);
  addtoppings(&plate, toppings, &seed);
  slice(&plate, lowangle, highangle);
  serve(epizza, plate);
  key(epizza);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  epizza.f = NULL;
  strcpy(epizza.name, "epizza");
  pizzap.f = NULL;
  strcpy(pizzap.name, "pizzap");
  themain(&pizzap, &epizza);
_L1:
  if (pizzap.f != NULL)
    fclose(pizzap.f);
  if (epizza.f != NULL)
    fclose(epizza.f);
  exit(EXIT_SUCCESS);
}



/* End. */
