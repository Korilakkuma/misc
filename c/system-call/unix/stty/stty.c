#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <term.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("stty ttyname\n", stderr);
  }

  struct termios term;

  int fd;

  char *ttyname = argv[1];

  if ((fd = open(ttyname, O_RDONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  if (tcgetattr(fd, &term) == -1) {
    perror("tcgetattr");
    exit(EXIT_FAILURE);
  }

  speed_t speed = cfgetospeed(&term);

  fputs("the baud rate is ", stdout);

  switch (speed) {
   case B300: {
     fprintf(stdout, "300\n", stdout);
     break;
   }

   case B600: {
     fprintf(stdout, "600\n", stdout);
     break;
   }

   case B1200: {
     fprintf(stdout, "1200\n", stdout);
     break;
   }

   case B1800: {
     fprintf(stdout, "1800\n", stdout);
     break;
   }

   case B2400: {
     fprintf(stdout, "2400\n", stdout);
     break;
   }

   case B4800: {
     fprintf(stdout, "4800\n", stdout);
     break;

   }

   case B9600: {
     fprintf(stdout, "9600\n", stdout);
     break;
   }

   default: {
     fprintf(stdout, "Fast\n", stdout);
     break;
   }
  }

  fprintf(stdout, "The erase character is ascii %d, Ctrl-%c\n", term.c_cc[VERASE], (term.c_cc[VERASE] - 1 + 'A'));
  fprintf(stdout, "The line kill character is ascii %d, Ctrl-%c\n", term.c_cc[VKILL], (term.c_cc[VKILL] - 1 + 'A'));

  fputs("lflag: ", stdout);

  if (term.c_lflag & ICANON) {
    fputs("icanon ", stdout);
  } else {
    fputs("-icanon ", stdout);
  }

  if (term.c_lflag & ISIG) {
    fputs("isig ", stdout);
  } else {
    fputs("-isig ", stdout);
  }

  if (term.c_lflag & IEXTEN) {
    fputs("iexten ", stdout);
  } else {
    fputs("-iexten ", stdout);
  }

  if (term.c_lflag & ECHO) {
    fputs("echo ", stdout);
  } else {
    fputs("-echo ", stdout);
  }

  if (term.c_lflag & ECHOE) {
    fputs("echoe ", stdout);
  } else {
    fputs("-echoe ", stdout);
  }

  if (term.c_lflag & ECHOK) {
    fputs("echok ", stdout);
  } else {
    fputs("-echok ", stdout);
  }

  if (term.c_lflag & ECHOKE) {
    fputs("echoke ", stdout);
  } else {
    fputs("-echoke ", stdout);
  }

  if (term.c_lflag & ECHONL) {
    fputs("echonl ", stdout);
  } else {
    fputs("-echonl ", stdout);
  }

  if (term.c_lflag & ECHOCTL) {
    fputs("echoctl ", stdout);
  } else {
    fputs("-echoctl ", stdout);
  }

  if (term.c_lflag & ECHOPRT) {
    fputs("echoprt ", stdout);
  } else {
    fputs("-echoprt ", stdout);
  }

  if (term.c_lflag & ALTWERASE) {
    fputs("altwerase ", stdout);
  } else {
    fputs("-altwerase ", stdout);
  }

  if (term.c_lflag & NOFLSH) {
    fputs("noflsh ", stdout);
  } else {
    fputs("-noflsh ", stdout);
  }

  if (term.c_lflag & TOSTOP) {
    fputs("tostop ", stdout);
  } else {
    fputs("-tostop ", stdout);
  }

  if (term.c_lflag & FLUSHO) {
    fputs("flusho ", stdout);
  } else {
    fputs("-flusho ", stdout);
  }

  if (term.c_lflag & PENDIN) {
    fputs("pendin ", stdout);
  } else {
    fputs("-pendin ", stdout);
  }

  if (term.c_lflag & NOKERNINFO) {
    fputs("nokerninfo ", stdout);
  } else {
    fputs("-nokerninfo ", stdout);
  }

  if (term.c_lflag & EXTPROC) {
    fputs("extproc ", stdout);
  } else {
    fputs("-extproc ", stdout);
  }

  fputc('\n', stdout);
  fputs("iflag: ", stdout);

  if (term.c_iflag & ISTRIP) {
    fputs("istrip ", stdout);
  } else {
    fputs("-istrip ", stdout);
  }

  if (term.c_iflag & ICRNL) {
    fputs("icrnl ", stdout);
  } else {
    fputs("-icrnl ", stdout);
  }

  if (term.c_iflag & IXON) {
    fputs("ixon ", stdout);
  } else {
    fputs("-ixon ", stdout);
  }

  if (term.c_iflag & IXANY) {
    fputs("ixany ", stdout);
  } else {
    fputs("-ixany ", stdout);
  }

  if (term.c_iflag & IMAXBEL) {
    fputs("imaxbel ", stdout);
  } else {
    fputs("-imaxbel ", stdout);
  }

  if (term.c_iflag & IUTF8) {
    fputs("iutf8 ", stdout);
  } else {
    fputs("-iutf8 ", stdout);
  }

  if (term.c_iflag & IGNBRK) {
    fputs("ignbrk ", stdout);
  } else {
    fputs("-ignbrk ", stdout);
  }

  if (term.c_iflag & BRKINT) {
    fputs("brkint ", stdout);
  } else {
    fputs("-brkint ", stdout);
  }

  if (term.c_iflag & INPCK) {
    fputs("inpck ", stdout);
  } else {
    fputs("-inpck ", stdout);
  }

  if (term.c_iflag & IGNPAR) {
    fputs("ignpar ", stdout);
  } else {
    fputs("-ignpar ", stdout);
  }

  if (term.c_iflag & PARMRK) {
    fputs("parmrk ", stdout);
  } else {
    fputs("-parmrk ", stdout);
  }

  fputc('\n', stdout);
  fputs("oflag: ", stdout);

  if (term.c_oflag & OPOST) {
    fputs("opost ", stdout);
  } else {
    fputs("-opost ", stdout);
  }

  if (term.c_oflag & ONLCR) {
    fputs("onlcr ", stdout);
  } else {
    fputs("-onlcr ", stdout);
  }

  if (term.c_oflag & OXTABS) {
    fputs("oxtabs ", stdout);
  } else {
    fputs("-oxtabs ", stdout);
  }

  if (term.c_oflag & ONOCR) {
    fputs("onocr ", stdout);
  } else {
    fputs("-onocr ", stdout);
  }

  if (term.c_oflag & ONLRET) {
    fputs("onlret ", stdout);
  } else {
    fputs("-onlret ", stdout);
  }

  fputc('\n', stdout);
  fputs("cflag: ", stdout);

  if (term.c_cflag & CREAD) {
    fputs("cread ", stdout);
  } else {
    fputs("-cread ", stdout);
  }

  if (term.c_cflag & CS8) {
    fputs("cs8 ", stdout);
  } else {
    fputs("-cs8 ", stdout);
  }

  if (term.c_cflag & PARENB) {
    fputs("parenb ", stdout);
  } else {
    fputs("-parenb ", stdout);
  }

  if (term.c_cflag & PARODD) {
    fputs("parodd ", stdout);
  } else {
    fputs("-parodd ", stdout);
  }

  if (term.c_cflag & HUPCL) {
    fputs("hupcl ", stdout);
  } else {
    fputs("-hupcl ", stdout);
  }

  if (term.c_cflag & CLOCAL) {
    fputs("clocal ", stdout);
  } else {
    fputs("-clocal ", stdout);
  }

  if (term.c_cflag & CSTOPB) {
    fputs("cstopb ", stdout);
  } else {
    fputs("-cstopb ", stdout);
  }

  if (term.c_cflag & CRTSCTS) {
    fputs("crtscts ", stdout);
  } else {
    fputs("-crtscts ", stdout);
  }

  fputc('\n', stdout);
  fputs("cchars: ", stdout);

  fprintf(stdout, "discard = ^%c; ", (term.c_cc[VDISCARD] - 1 + 'A'));
  fprintf(stdout, "dsusp = ^%c; ",   (term.c_cc[VDSUSP]   - 1 + 'A'));
  fprintf(stdout, "eof = ^%c; ",     (term.c_cc[VEOF]     - 1 + 'A'));
  fprintf(stdout, "eol = ^%c; ",     (term.c_cc[VEOL]     - 1 + 'A'));
  fprintf(stdout, "eol2 = ^%c; ",    (term.c_cc[VEOL2]    - 1 + 'A'));
  fprintf(stdout, "erase = ^%c; ",   (term.c_cc[VERASE]   - 1 + 'A'));
  fprintf(stdout, "intr = ^%c; ",    (term.c_cc[VINTR]    - 1 + 'A'));
  fprintf(stdout, "kill = ^%c; ",    (term.c_cc[VKILL]    - 1 + 'A'));
  fprintf(stdout, "lnext = ^%c; ",   (term.c_cc[VLNEXT]   - 1 + 'A'));
  fprintf(stdout, "min = ^%c; ",     (term.c_cc[VMIN]     - 1 + 'A'));
  fprintf(stdout, "quit = ^%c; ",    (term.c_cc[VQUIT]    - 1 + 'A'));
  fprintf(stdout, "reprint = ^%c; ", (term.c_cc[VREPRINT] - 1 + 'A'));
  fprintf(stdout, "start = ^%c; ",   (term.c_cc[VSTART]   - 1 + 'A'));
  fprintf(stdout, "status = ^%c; ",  (term.c_cc[VSTART]   - 1 + 'A'));
  fprintf(stdout, "stop = ^%c; ",    (term.c_cc[VSTOP]    - 1 + 'A'));
  fprintf(stdout, "susp = ^%c; ",    (term.c_cc[VSUSP]    - 1 + 'A'));
  fprintf(stdout, "time = ^%c; ",    (term.c_cc[VTIME]    - 1 + 'A'));
  fprintf(stdout, "werase = ^%c; ",  (term.c_cc[VWERASE]  - 1 + 'A'));

  fputc('\n', stdout);

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
