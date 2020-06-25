/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "decat.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/




#define version         1.15


/*



*/



/*



































*/


Static _TEXT decatp, f0, f1, f2, f3, f4, f5, f6, f7, f8, f9;


Static Void themain(afile)
_TEXT *afile;
{
  Char c;
  long count = 0;
  long size;
  long total = 0;
  boolean overflow = false;
  long which = 0;

  printf("decat %4.2f\n", version);

  if (*decatp.name != '\0') {
    if (decatp.f != NULL)
      decatp.f = freopen(decatp.name, "r", decatp.f);
    else
      decatp.f = fopen(decatp.name, "r");
  } else
    rewind(decatp.f);
  if (decatp.f == NULL)
    _EscIO2(FileNotFound, decatp.name);
  RESETBUF(decatp.f, Char);
  fscanf(decatp.f, "%ld%*[^\n]", &size);
  getc(decatp.f);


  /*


*/

  /*



*/
  if (!BUFEOF(afile->f)) {
    if (*f0.name != '\0') {
      if (f0.f != NULL)
	f0.f = freopen(f0.name, "w", f0.f);
      else
	f0.f = fopen(f0.name, "w");
    } else {
      if (f0.f != NULL)
	rewind(f0.f);
      else
	f0.f = tmpfile();
    }
    if (f0.f == NULL)
      _EscIO2(FileNotFound, f0.name);
    SETUPBUF(f0.f, Char);
  }

  while (!BUFEOF(afile->f)) {
    while (!P_eoln(afile->f)) {
      c = getc(afile->f);
      if (c == '\n')
	c = ' ';
      switch (which) {

      case 0:
	putc(c, f0.f);
	break;

      case 1:
	putc(c, f1.f);
	break;

      case 2:
	putc(c, f2.f);
	break;

      case 3:
	putc(c, f3.f);
	break;

      case 4:
	putc(c, f4.f);
	break;

      case 5:
	putc(c, f5.f);
	break;

      case 6:
	putc(c, f6.f);
	break;

      case 7:
	putc(c, f7.f);
	break;

      case 8:
	putc(c, f8.f);
	break;

      case 9:
	putc(c, f9.f);
	break;
      }

      count++;
    }
    fscanf(afile->f, "%*[^\n]");

    getc(afile->f);
    switch (which) {

    case 0:
      putc('\n', f0.f);
      break;

    case 1:
      putc('\n', f1.f);
      break;

    case 2:
      putc('\n', f2.f);
      break;

    case 3:
      putc('\n', f3.f);
      break;

    case 4:
      putc('\n', f4.f);
      break;

    case 5:
      putc('\n', f5.f);
      break;

    case 6:
      putc('\n', f6.f);
      break;

    case 7:
      putc('\n', f7.f);
      break;

    case 8:
      putc('\n', f8.f);
      break;

    case 9:
      putc('\n', f9.f);
      break;
    }

    count++;

    if (count < size)
      continue;
    if (!overflow)
      printf(" file %ld: %ld bytes\n", which, count);

    total += count;
    count = 0;

    if (which >= 9) {
      if (!overflow) {
	overflow = true;
	printf("WARNING: last file will be too big\n");
      }
      continue;
    }
    which++;
    /*












*/

    /*











*/



    if (which == 0) {
      if (*f0.name != '\0') {
	if (f0.f != NULL)
	  f0.f = freopen(f0.name, "w", f0.f);
	else
	  f0.f = fopen(f0.name, "w");
      } else {
	if (f0.f != NULL)
	  rewind(f0.f);
	else
	  f0.f = tmpfile();
      }
      if (f0.f == NULL)
	_EscIO2(FileNotFound, f0.name);
      SETUPBUF(f0.f, Char);
    }
    if (which == 1) {
      if (*f1.name != '\0') {
	if (f1.f != NULL)
	  f1.f = freopen(f1.name, "w", f1.f);
	else
	  f1.f = fopen(f1.name, "w");
      } else {
	if (f1.f != NULL)
	  rewind(f1.f);
	else
	  f1.f = tmpfile();
      }
      if (f1.f == NULL)
	_EscIO2(FileNotFound, f1.name);
      SETUPBUF(f1.f, Char);
    }
    if (which == 2) {
      if (*f2.name != '\0') {
	if (f2.f != NULL)
	  f2.f = freopen(f2.name, "w", f2.f);
	else
	  f2.f = fopen(f2.name, "w");
      } else {
	if (f2.f != NULL)
	  rewind(f2.f);
	else
	  f2.f = tmpfile();
      }
      if (f2.f == NULL)
	_EscIO2(FileNotFound, f2.name);
      SETUPBUF(f2.f, Char);
    }
    if (which == 3) {
      if (*f3.name != '\0') {
	if (f3.f != NULL)
	  f3.f = freopen(f3.name, "w", f3.f);
	else
	  f3.f = fopen(f3.name, "w");
      } else {
	if (f3.f != NULL)
	  rewind(f3.f);
	else
	  f3.f = tmpfile();
      }
      if (f3.f == NULL)
	_EscIO2(FileNotFound, f3.name);
      SETUPBUF(f3.f, Char);
    }
    if (which == 4) {
      if (*f4.name != '\0') {
	if (f4.f != NULL)
	  f4.f = freopen(f4.name, "w", f4.f);
	else
	  f4.f = fopen(f4.name, "w");
      } else {
	if (f4.f != NULL)
	  rewind(f4.f);
	else
	  f4.f = tmpfile();
      }
      if (f4.f == NULL)
	_EscIO2(FileNotFound, f4.name);
      SETUPBUF(f4.f, Char);
    }
    if (which == 5) {
      if (*f5.name != '\0') {
	if (f5.f != NULL)
	  f5.f = freopen(f5.name, "w", f5.f);
	else
	  f5.f = fopen(f5.name, "w");
      } else {
	if (f5.f != NULL)
	  rewind(f5.f);
	else
	  f5.f = tmpfile();
      }
      if (f5.f == NULL)
	_EscIO2(FileNotFound, f5.name);
      SETUPBUF(f5.f, Char);
    }
    if (which == 6) {
      if (*f6.name != '\0') {
	if (f6.f != NULL)
	  f6.f = freopen(f6.name, "w", f6.f);
	else
	  f6.f = fopen(f6.name, "w");
      } else {
	if (f6.f != NULL)
	  rewind(f6.f);
	else
	  f6.f = tmpfile();
      }
      if (f6.f == NULL)
	_EscIO2(FileNotFound, f6.name);
      SETUPBUF(f6.f, Char);
    }
    if (which == 7) {
      if (*f7.name != '\0') {
	if (f7.f != NULL)
	  f7.f = freopen(f7.name, "w", f7.f);
	else
	  f7.f = fopen(f7.name, "w");
      } else {
	if (f7.f != NULL)
	  rewind(f7.f);
	else
	  f7.f = tmpfile();
      }
      if (f7.f == NULL)
	_EscIO2(FileNotFound, f7.name);
      SETUPBUF(f7.f, Char);
    }
    if (which == 8) {
      if (*f8.name != '\0') {
	if (f8.f != NULL)
	  f8.f = freopen(f8.name, "w", f8.f);
	else
	  f8.f = fopen(f8.name, "w");
      } else {
	if (f8.f != NULL)
	  rewind(f8.f);
	else
	  f8.f = tmpfile();
      }
      if (f8.f == NULL)
	_EscIO2(FileNotFound, f8.name);
      SETUPBUF(f8.f, Char);
    }
    if (which != 9)
      continue;

    if (*f9.name != '\0') {
      if (f9.f != NULL)
	f9.f = freopen(f9.name, "w", f9.f);
      else
	f9.f = fopen(f9.name, "w");
    } else {
      if (f9.f != NULL)
	rewind(f9.f);
      else
	f9.f = tmpfile();
    }
    if (f9.f == NULL)
      _EscIO2(FileNotFound, f9.name);
    SETUPBUF(f9.f, Char);
  }

  if (count > 0) {
    printf(" file %ld: %ld bytes\n", which, count);
    total += count;
  }

  printf("total bytes split out: %ld\n", total);
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  f9.f = NULL;
  strcpy(f9.name, "f9");
  f8.f = NULL;
  strcpy(f8.name, "f8");
  f7.f = NULL;
  strcpy(f7.name, "f7");
  f6.f = NULL;
  strcpy(f6.name, "f6");
  f5.f = NULL;
  strcpy(f5.name, "f5");
  f4.f = NULL;
  strcpy(f4.name, "f4");
  f3.f = NULL;
  strcpy(f3.name, "f3");
  f2.f = NULL;
  strcpy(f2.name, "f2");
  f1.f = NULL;
  strcpy(f1.name, "f1");
  f0.f = NULL;
  strcpy(f0.name, "f0");
  decatp.f = NULL;
  strcpy(decatp.name, "decatp");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  themain(&TEMP);
  if (decatp.f != NULL)
    fclose(decatp.f);
  if (f0.f != NULL)
    fclose(f0.f);
  if (f1.f != NULL)
    fclose(f1.f);
  if (f2.f != NULL)
    fclose(f2.f);
  if (f3.f != NULL)
    fclose(f3.f);
  if (f4.f != NULL)
    fclose(f4.f);
  if (f5.f != NULL)
    fclose(f5.f);
  if (f6.f != NULL)
    fclose(f6.f);
  if (f7.f != NULL)
    fclose(f7.f);
  if (f8.f != NULL)
    fclose(f8.f);
  if (f9.f != NULL)
    fclose(f9.f);
  exit(EXIT_SUCCESS);
}



/* End. */
