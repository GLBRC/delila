/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "helixside.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.00
/*

*/
#define updateversion   1.00



/*






































*/


Static _TEXT distances, helixsidep, data;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(distances, helixsidep, data)
_TEXT *distances, *helixsidep, *data;
{
  double parameterversion;
  long d;
  double angle, modulo;
  double twist = 10.6;

  printf("helixside %4.2f\n", version);
  if (*helixsidep->name != '\0') {
    if (helixsidep->f != NULL)
      helixsidep->f = freopen(helixsidep->name, "r", helixsidep->f);
    else
      helixsidep->f = fopen(helixsidep->name, "r");
  } else
    rewind(helixsidep->f);
  if (helixsidep->f == NULL)
    _EscIO2(FileNotFound, helixsidep->name);
  RESETBUF(helixsidep->f, Char);
  fscanf(helixsidep->f, "%lg%*[^\n]", &parameterversion);
  getc(helixsidep->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  if (*distances->name != '\0') {
    if (distances->f != NULL)
      distances->f = freopen(distances->name, "r", distances->f);
    else
      distances->f = fopen(distances->name, "r");
  } else {

    rewind(distances->f);
  }
  if (distances->f == NULL)
    _EscIO2(FileNotFound, distances->name);
  RESETBUF(distances->f, Char);
  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "w", data->f);
    else
      data->f = fopen(data->name, "w");
  } else {
    if (data->f != NULL)
      rewind(data->f);
    else
      data->f = tmpfile();
  }
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  SETUPBUF(data->f, Char);

  while (!BUFEOF(distances->f)) {
    fscanf(distances->f, "%ld%*[^\n]", &d);
    getc(distances->f);
    modulo = labs(d);
    while (modulo > twist)
      modulo -= twist;
    angle = 360 * modulo / twist;
    fprintf(data->f, "%10ld %10.1f\n", d, angle);
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  data.f = NULL;
  strcpy(data.name, "data");
  helixsidep.f = NULL;
  strcpy(helixsidep.name, "helixsidep");
  distances.f = NULL;
  strcpy(distances.name, "distances");
  themain(&distances, &helixsidep, &data);
_L1:
  if (distances.f != NULL)
    fclose(distances.f);
  if (helixsidep.f != NULL)
    fclose(helixsidep.f);
  if (data.f != NULL)
    fclose(data.f);
  exit(EXIT_SUCCESS);
}



/* End. */
