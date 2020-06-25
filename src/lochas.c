/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "lochas.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.05


/*
*/



/*













































*/


Static Void printchar(f, c)
_TEXT *f;
long c;
{
  /*

















*/
  switch (c) {

  case 0:
    fprintf(f->f, "NUL");
    break;

  case 1:
    fprintf(f->f, "SOH");
    break;

  case 2:
    fprintf(f->f, "STX");
    break;

  case 3:
    fprintf(f->f, "ETX");
    break;

  case 4:
    fprintf(f->f, "EOT");
    break;

  case 5:
    fprintf(f->f, "ENQ");
    break;

  case 6:
    fprintf(f->f, "ACK");
    break;

  case 7:
    fprintf(f->f, "BEL");
    break;

  case 8:
    fprintf(f->f, "BS ");
    break;

  case 9:
    fprintf(f->f, "HT ");
    break;

  case 10:
    fprintf(f->f, "NL ");
    break;

  case 11:
    fprintf(f->f, "VT ");
    break;

  case 12:
    fprintf(f->f, "NP ");
    break;

  case 13:
    fprintf(f->f, "CR ");
    break;

  case 14:
    fprintf(f->f, "SO ");
    break;

  case 15:
    fprintf(f->f, "SI ");
    break;

  case 16:
    fprintf(f->f, "DLE");
    break;

  case 17:
    fprintf(f->f, "DC1");
    break;

  case 18:
    fprintf(f->f, "DC2");
    break;

  case 19:
    fprintf(f->f, "DC3");
    break;

  case 20:
    fprintf(f->f, "DC4");
    break;

  case 21:
    fprintf(f->f, "NAK");
    break;

  case 22:
    fprintf(f->f, "SYN");
    break;

  case 23:
    fprintf(f->f, "ETB");
    break;

  case 24:
    fprintf(f->f, "CAN");
    break;

  case 25:
    fprintf(f->f, "EM ");
    break;

  case 26:
    fprintf(f->f, "SUB");
    break;

  case 27:
    fprintf(f->f, "ESC");
    break;

  case 28:
    fprintf(f->f, "FS ");
    break;

  case 29:
    fprintf(f->f, "GS ");
    break;

  case 30:
    fprintf(f->f, "RS ");
    break;

  case 31:
    fprintf(f->f, "US ");
    break;

  case 32:
    fprintf(f->f, "SP ");
    break;

  case 33:
    fprintf(f->f, " ! ");
    break;

  case 34:
    fprintf(f->f, " \" ");
    break;

  case 35:
    fprintf(f->f, " # ");
    break;

  case 36:
    fprintf(f->f, " $ ");
    break;

  case 37:
    fprintf(f->f, " %% ");
    break;

  case 38:
    fprintf(f->f, " & ");
    break;

  case 39:
    fprintf(f->f, " ' ");
    break;

  case 40:
    fprintf(f->f, " ( ");
    break;

  case 41:
    fprintf(f->f, " ) ");
    break;

  case 42:
    fprintf(f->f, " * ");
    break;

  case 43:
    fprintf(f->f, " + ");
    break;

  case 44:
    fprintf(f->f, " , ");
    break;

  case 45:
    fprintf(f->f, " - ");
    break;

  case 46:
    fprintf(f->f, " . ");
    break;

  case 47:
    fprintf(f->f, " / ");
    break;

  case 48:
    fprintf(f->f, " 0 ");
    break;

  case 49:
    fprintf(f->f, " 1 ");
    break;

  case 50:
    fprintf(f->f, " 2 ");
    break;

  case 51:
    fprintf(f->f, " 3 ");
    break;

  case 52:
    fprintf(f->f, " 4 ");
    break;

  case 53:
    fprintf(f->f, " 5 ");
    break;

  case 54:
    fprintf(f->f, " 6 ");
    break;

  case 55:
    fprintf(f->f, " 7 ");
    break;

  case 56:
    fprintf(f->f, " 8 ");
    break;

  case 57:
    fprintf(f->f, " 9 ");
    break;

  case 58:
    fprintf(f->f, " : ");
    break;

  case 59:
    fprintf(f->f, " ; ");
    break;

  case 60:
    fprintf(f->f, " < ");
    break;

  case 61:
    fprintf(f->f, " = ");
    break;

  case 62:
    fprintf(f->f, " > ");
    break;

  case 63:
    fprintf(f->f, " ? ");
    break;

  case 64:
    fprintf(f->f, " @ ");
    break;

  case 65:
    fprintf(f->f, " A ");
    break;

  case 66:
    fprintf(f->f, " B ");
    break;

  case 67:
    fprintf(f->f, " C ");
    break;

  case 68:
    fprintf(f->f, " D ");
    break;

  case 69:
    fprintf(f->f, " E ");
    break;

  case 70:
    fprintf(f->f, " F ");
    break;

  case 71:
    fprintf(f->f, " G ");
    break;

  case 72:
    fprintf(f->f, " H ");
    break;

  case 73:
    fprintf(f->f, " I ");
    break;

  case 74:
    fprintf(f->f, " J ");
    break;

  case 75:
    fprintf(f->f, " K ");
    break;

  case 76:
    fprintf(f->f, " L ");
    break;

  case 77:
    fprintf(f->f, " M ");
    break;

  case 78:
    fprintf(f->f, " N ");
    break;

  case 79:
    fprintf(f->f, " O ");
    break;

  case 80:
    fprintf(f->f, " P ");
    break;

  case 81:
    fprintf(f->f, " Q ");
    break;

  case 82:
    fprintf(f->f, " R ");
    break;

  case 83:
    fprintf(f->f, " S ");
    break;

  case 84:
    fprintf(f->f, " T ");
    break;

  case 85:
    fprintf(f->f, " U ");
    break;

  case 86:
    fprintf(f->f, " V ");
    break;

  case 87:
    fprintf(f->f, " W ");
    break;

  case 88:
    fprintf(f->f, " X ");
    break;

  case 89:
    fprintf(f->f, " Y ");
    break;

  case 90:
    fprintf(f->f, " Z ");
    break;

  case 91:
    fprintf(f->f, " [ ");
    break;

  case 92:
    fprintf(f->f, " \\ ");
    break;

  case 93:
    fprintf(f->f, " ] ");
    break;

  case 94:
    fprintf(f->f, " ^ ");
    break;

  case 95:
    fprintf(f->f, " _ ");
    break;

  case 96:
    fprintf(f->f, " ` ");
    break;

  case 97:
    fprintf(f->f, " a ");
    break;

  case 98:
    fprintf(f->f, " b ");
    break;

  case 99:
    fprintf(f->f, " c ");
    break;

  case 100:
    fprintf(f->f, " d ");
    break;

  case 101:
    fprintf(f->f, " e ");
    break;

  case 102:
    fprintf(f->f, " f ");
    break;

  case 103:
    fprintf(f->f, " g ");
    break;

  case 104:
    fprintf(f->f, " h ");
    break;

  case 105:
    fprintf(f->f, " i ");
    break;

  case 106:
    fprintf(f->f, " j ");
    break;

  case 107:
    fprintf(f->f, " k ");
    break;

  case 108:
    fprintf(f->f, " l ");
    break;

  case 109:
    fprintf(f->f, " m ");
    break;

  case 110:
    fprintf(f->f, " n ");
    break;

  case 111:
    fprintf(f->f, " o ");
    break;

  case 112:
    fprintf(f->f, " p ");
    break;

  case 113:
    fprintf(f->f, " q ");
    break;

  case 114:
    fprintf(f->f, " r ");
    break;

  case 115:
    fprintf(f->f, " s ");
    break;

  case 116:
    fprintf(f->f, " t ");
    break;

  case 117:
    fprintf(f->f, " u ");
    break;

  case 118:
    fprintf(f->f, " v ");
    break;

  case 119:
    fprintf(f->f, " w ");
    break;

  case 120:
    fprintf(f->f, " x ");
    break;

  case 121:
    fprintf(f->f, " y ");
    break;

  case 122:
    fprintf(f->f, " z ");
    break;

  case 123:
    fprintf(f->f, " { ");
    break;

  case 124:
    fprintf(f->f, " | ");
    break;

  case 125:
    fprintf(f->f, " } ");
    break;

  case 126:
    fprintf(f->f, " ~ ");
    break;

  case 127:
    fprintf(f->f, "DEL");
    break;

  }
}


Static Void themain(afile)
_TEXT *afile;
{
  long c;
  long count = 0;
  boolean highorderbit;
  _TEXT TEMP;

  /*
*/
  printf("* lochas %4.2f\n", version);
  if (*afile->name != '\0') {
    if (afile->f != NULL)
      afile->f = freopen(afile->name, "r", afile->f);
    else
      afile->f = fopen(afile->name, "r");
  } else
    rewind(afile->f);
  if (afile->f == NULL)
    _EscIO2(FileNotFound, afile->name);
  RESETBUF(afile->f, Char);
  while (!BUFEOF(afile->f)) {
    count++;
    if (P_eoln(afile->f)) {
      printf("END OF LINE\n");
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      continue;
    }
    c = P_peek(afile->f);
    if (c > 128) {
      c -= 128;
      highorderbit = true;
    } else
      highorderbit = false;

    printf("%6ld", count);

    putchar(' ');
    printf("%3ld", c);
    putchar(' ');

    putchar('|');
    TEMP.f = stdout;
    *TEMP.name = '\0';
    printchar(&TEMP, c);
    putchar('|');

    if (c == 32)
      printf(" space");
    if (c == 0)
      printf(" null");
    if (highorderbit)
      printf(" HIGH ORDER BIT REMOVED");

    putchar('\n');

    getc(afile->f);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdin;
  *TEMP.name = '\0';
  themain(&TEMP);
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
