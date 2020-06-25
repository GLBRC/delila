/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "numdate.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.05
/*






*/
#define updateversion   1.00


Static jmp_buf _JL1;



/*

















































*/


/*



*/


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void fail()
{
  printf("PARSE FAILED\n");
  halt();
}


#define debugging       false


typedef Char monthtype[3];


/*






*/


Static Void themain()
{
  /*


*/

  long year;
  Char c;
  monthtype monthstring;
  /*

*/
  long monthday = -1;
  long day, hour, minute, second;

  double days, hours, minutes, seconds;

  if (debugging)
    printf("numdate %4.2f\n", version);
  /*






*/

  if (P_eof(stdin))
    fail();

  /*

*/

  scanf("%ld", &year);
  if (debugging)
    printf("%ld\n", year);

  *monthstring = getchar();
  monthstring[1] = getchar();
  monthstring[2] = getchar();
  if (monthstring[0] == '\n')
    monthstring[0] = ' ';
  if (monthstring[1] == '\n')
    monthstring[1] = ' ';
  if (monthstring[2] == '\n')
    monthstring[2] = ' ';
  if (debugging)
    printf("%.3s\n", monthstring);


  if (!strncmp(monthstring, "Jan", sizeof(monthtype)))
    monthday = 0;
  if (!strncmp(monthstring, "Feb", sizeof(monthtype)))
    monthday = 31;
  if (!strncmp(monthstring, "Mar", sizeof(monthtype)))
    monthday = 59;
  if (!strncmp(monthstring, "Apr", sizeof(monthtype)))
    monthday = 90;
  if (!strncmp(monthstring, "May", sizeof(monthtype)))
    monthday = 120;
  if (!strncmp(monthstring, "Jun", sizeof(monthtype)))
    monthday = 151;
  if (!strncmp(monthstring, "Jul", sizeof(monthtype)))
    monthday = 181;
  if (!strncmp(monthstring, "Aug", sizeof(monthtype)))
    monthday = 212;
  if (!strncmp(monthstring, "Sep", sizeof(monthtype)))
    monthday = 243;
  if (!strncmp(monthstring, "Oct", sizeof(monthtype)))
    monthday = 273;
  if (!strncmp(monthstring, "Nov", sizeof(monthtype)))
    monthday = 304;
  if (!strncmp(monthstring, "Dec", sizeof(monthtype)))
    monthday = 334;
  if (monthday == -1)
    fail();

  scanf("%ld", &day);
  if (debugging)
    printf("%ld\n", day);

  c = getchar();
  if (c == '\n')
    c = ' ';
  if (c != '_' && c != '.')
    fail();

  scanf("%ld", &hour);
  if (debugging)
    printf("%ld\n", hour);

  c = getchar();
  if (c == '\n')
    c = ' ';
  if (c != ':')
    fail();

  scanf("%ld", &minute);
  if (debugging)
    printf("%ld\n", minute);

  c = getchar();
  if (c == '\n')
    c = ' ';
  if (c != ':')
    fail();

  scanf("%ld", &second);
  if (debugging)
    printf("%ld\n", second);

  days = year * 365.25 + monthday + day;
  hours = days * 24 + hour;
  minutes = hours * 60 + minute;
  seconds = minutes * 60 + second;

  printf("%ld\n", (long)floor(seconds + 0.5));

}

#undef debugging


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  themain();
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
