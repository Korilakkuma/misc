#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#define _XOPEN_SOURCE 600

int pty_master_open(void);
int pty_slave_open(int fd_master);
struct termios set_tty_raw(int fd);
pid_t pty_fork(int fd_master);
void loop(int rfd1, int wfd1, int rfd2, int wfd2, pid_t pid);

int main(int argc, char **argv) {
  int fd_master = pty_master_open();

  pid_t pid = pty_fork(fd_master);

  if (pid == 0) {
    execvp(argv[1], (argv + 1));
  } else {
    loop(STDIN_FILENO, fd_master, fd_master, STDOUT_FILENO, pid);
  }

  return 0;
}

int pty_master_open(void) {
  int fd_master = posix_openpt(O_RDWR);

  grantpt(fd_master);
  unlockpt(fd_master);

  return fd_master;
}

int pty_slave_open(int fd_master) {
  int fd_slave = open(ptsname(fd_master), O_RDWR);

  return fd_slave;
}

struct termios set_tty_raw(int fd) {
  struct termios term;
  struct termios new_term;

  tcgetattr(fd, &term);

  new_term = term;

  new_term.c_iflag &= ~(BRKINT | ICRNL | INLCR | IGNCR | INPCK | ISTRIP | IXON);
  new_term.c_oflag &= ~(OPOST);
  new_term.c_lflag &= ~(ECHO | ECHOE | ECHONL | ICANON | IEXTEN | ISIG);
  new_term.c_cflag &= ~(PARENB | CSIZE);
  new_term.c_cflag |= CS8;

  new_term.c_cc[VMIN]  = 1;
  new_term.c_cc[VTIME] = 0;

  tcsetattr(fd, TCSANOW, &new_term);

  return term;
}

pid_t pty_fork(int fd_master) {
  struct termios termios_slave = set_tty_raw(STDIN_FILENO);

  pid_t pid = fork();

  if (pid == 0) {
    setsid();

    int fd_slave = pty_slave_open(fd_master);

    ioctl(fd_slave, TIOCSCTTY, (char *)0);

    close(fd_master);

    tcsetattr(fd_slave, TCSANOW, &termios_slave);

    dup2(fd_slave, STDIN_FILENO);
    dup2(fd_slave, STDOUT_FILENO);
    dup2(fd_slave, STDERR_FILENO);

    if (fd_slave > STDERR_FILENO) {
      close(fd_slave);
    }

    return 0;
  }

  return pid;
}

void loop(int rfd1, int wfd1, int rfd2, int wfd2, pid_t pid) {
  char buf[1024];

  fd_set rfd_set;

  while (1) {
    FD_ZERO(&rfd_set);

    FD_SET(rfd1, &rfd_set);
    FD_SET(rfd2, &rfd_set);

    select((((rfd1 > rfd2) ? rfd1 : rfd2) + 1), &rfd_set, NULL, NULL, NULL);

    if (FD_ISSET(rfd1, &rfd_set)) {
      ssize_t nbytes = read(rfd1, buf, sizeof(buf));

      if (nbytes <= 0) {
        kill(pid, SIGHUP);
        exit(EXIT_FAILURE);
      }

      write(wfd1, buf, nbytes);
    }

    if (FD_ISSET(rfd2, &rfd_set)) {
      ssize_t nbytes = read(rfd2, buf, sizeof(buf));

      if (nbytes <= 0) {
        exit(EXIT_FAILURE);
      }

      write(wfd2, buf, nbytes);
    }
  }
}
