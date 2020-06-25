/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "between.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/


#define version         1.00
/*

*/
#define updateversion   1.00



/*































*/


Static boolean dobetween(a, b, c)
double a, b, c;
{
  return (a <= b && b <= c || c <= b && b <= a);
}


Static Void themain(input, bfile)
_TEXT *input, *bfile;
{
  double a, b, c;

  if (*input->name != '\0') {
    if (input->f != NULL)
      input->f = freopen(input->name, "r", input->f);
    else
      input->f = fopen(input->name, "r");
  } else
    rewind(input->f);
  if (input->f == NULL)
    _EscIO2(FileNotFound, input->name);
  RESETBUF(input->f, Char);
  while (!BUFEOF(input->f)) {
    fscanf(input->f, "%lg%lg%lg%*[^\n]", &a, &b, &c);
    getc(input->f);
    if (dobetween(a, b, c))
      printf("t\n");
    else
      printf("f\n");
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
  exit(EXIT_SUCCESS);
}



/* End. */
