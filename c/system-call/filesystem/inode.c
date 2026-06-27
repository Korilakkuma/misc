#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./inode pathname\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *pathname = argv[1];

  struct stat inode;

  if (stat(pathname, &inode) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  fputs("Print inode struct\n", stdout);

  putchar('\n');

  fprintf(stdout, "st_ino   = %lld\n", inode.st_ino);
  fprintf(stdout, "st_nlink = %d\n", inode.st_nlink);

  putchar('\n');

  fprintf(stdout, "st_mode = %d\n", inode.st_mode);

  putchar('\n');

  fprintf(stdout, "st_dev  = %d\n", inode.st_dev);
  fprintf(stdout, "st_rdev = %d\n", inode.st_rdev);

  putchar('\n');

  fprintf(stdout, "st_size    = %lld\n", inode.st_size);
  fprintf(stdout, "st_blocks  = %lld\n", inode.st_blocks);
  fprintf(stdout, "st_blksize = %d\n", inode.st_blksize);

  putchar('\n');

  fprintf(stdout, "st_uid = %d\n", inode.st_uid);
  fprintf(stdout, "st_gid = %d\n", inode.st_gid);

  putchar('\n');

  fprintf(stdout, "st_atimespec.tv_sec = %ld\n", inode.st_atimespec.tv_sec);
  fprintf(stdout, "st_ctimespec.tv_sec = %ld\n", inode.st_ctimespec.tv_sec);
  fprintf(stdout, "st_mtimespec.tv_sec = %ld\n", inode.st_mtimespec.tv_sec);

  return 0;
}
