#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 17
#define STRING_SIZE 16

int main(void){
  char buffer[BUFFER_SIZE];
  buffer[STRING_SIZE] = '\0';
  int file = open("./output.txt", O_CREAT | O_WRONLY);
  write(file, "String from file", STRING_SIZE);
  close(file);
  file = open("./output.txt", O_RDONLY);

  read(file, buffer, STRING_SIZE);
  close(file);

  printf("%s\n", buffer);

  return 0;
}