#include "pipe_networking.h"

#define PUBLIC "fifo.pub" //visible to all
#define PRIVATE "fifo"    //visible only to client

/*=========================
  server_handshake
  args: int * to_client

  Perofrms the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client_pt) {
  
  mkfifo(PUBLIC, 0666);
  
  char input[BUFFER_SIZE];
  int from_client = open(PUBLIC, O_RDONLY);
  read(from_client, input, sizeof(input));
  
  remove(PUBLIC);

  char output[BUFFER_SIZE];
  sprintf(output, "server recieved: '%s'", input);
  printf("STEP ONE: %s\n", output);
  int to_client = open(input, O_WRONLY); //input should be $PRIVATE
  write(to_client, output, sizeof(output));
  printf("STEP TWO: server sent: '%s'\n", output);

  read(from_client, input, sizeof(input));
  printf("STEP THREE: server recieved: '%s'\n", input);

  *to_client_pt = to_client;
  
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Perofrms the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server_pt) {

  mkfifo(PRIVATE, 0666);
  
  int to_server = open(PUBLIC, O_WRONLY);
  write(to_server, PRIVATE, sizeof(PRIVATE));
  printf("STEP ONE: client sent: '%s'\n", PRIVATE);

  char input[BUFFER_SIZE];
  int from_server = open(PRIVATE, O_RDONLY);
  read(from_server, input, sizeof(input));

  remove(PRIVATE);

  char output[BUFFER_SIZE];
  sprintf(output, "client recieved: '%s'\n", input);
  printf("STEP TWO: %s\n", input);
  write(to_server, output, sizeof(output));
  printf("STEP THREE: client sent: '%s'\n", output);

  *to_server_pt = to_server;
  
  return from_server;
}
