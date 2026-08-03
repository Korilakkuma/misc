#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./nslookup domain\n", stderr);
    exit(EXIT_FAILURE);
  }

  struct hostent *hp;
  in_addr_t address;

  char *domain = argv[1];

  if ((hp = gethostbyname(domain)) == NULL) {
    herror("gethostbyname");
    exit(EXIT_FAILURE);
  }

  memmove(&address, hp->h_addr_list[0], sizeof(in_addr_t));

  fprintf(stdout, "h_name is '%s'\n", hp->h_name);
  fprintf(stdout, "h_aliases[0] is '%s'\n", hp->h_aliases[0]);
  fprintf(stdout, "h_addrtype is %d\n", hp->h_addrtype);
  fprintf(stdout, "h_length is %d\n", hp->h_length);
  fprintf(stdout, "h_addr_list[0] is '%x'\n", address);

  return 0;;
}
