/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "timep2c.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.33
/*



















*/



/*



















































*/

























#define datetimearraylength  19


/*





*/




typedef Char datetimearray[datetimearraylength];




Static datetimearray adatetime;


Static jmp_buf _JL1;



Static Void halt()
{
  /*



*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void testdatetimep2c()
{
  /*
*/
  Char adate[datetimearraylength], atime[datetimearraylength];

  printf("Note: the following routines are specific to the p2c compiler: \n");
  printf("the raw date and time are:\n");
  VAXdate(adate);
  VAXtime(atime);
  printf("adate[%.*s]\n", datetimearraylength, adate);
  printf("atime[%.*s] (note extra random characters)\n",
	 datetimearraylength, atime);
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




Static Void demotime(fout)
_TEXT *fout;
{
  datetimearray dateandtime;
  double seed;

  /*
*/
  getdatetime(dateandtime);
  fprintf(fout->f, "\nThe date and time is: \n");
  writedatetime(fout, dateandtime);
  fprintf(fout->f, " <- This should be the current time and date.\n");
  fprintf(fout->f, "1980/06/09 18:49:11");
  fprintf(fout->f, " <- Times and dates should look like this\n");
  fprintf(fout->f, "year mo da ho mi se <- with parts in these positions\n\n");

  timeseed(&seed);
  fprintf(fout->f, "A timeseed is %16.14f\n", seed);
  fprintf(fout->f,
	  "Timeseeds can be used to start random number generators.\n");
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  printf("timep2c %4.2f\n", version);


  testdatetimep2c();


  TEMP.f = stdout;
  *TEMP.name = '\0';
  demotime(&TEMP);
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
