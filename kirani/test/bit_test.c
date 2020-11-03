/*
 * author: Thomas Yao
 */

#include <stdio.h>

#include "../store/directory.h"
#include "../store/file_stream.h"

void
bit_test_main()
{
  int bits = 0000;
  bits |= 0x2;
  printf("%s\n", "HELLO");
  printf("%d\n", bits);
}

void
bit_test_main2()
{
  int i = 1;
  short int li = 1;
  printf("sizeof:int: %d\n", sizeof(i));
  printf("sizeof:int: %d\n", sizeof(li));
}

void
bit_test_main3()
{
  struct _directory* dir = directory_initial("/root/sdoc", "/root/index", 0);
  struct _file_stream* in = directory_open_file(dir, "segments", ".x", "r");
  char* name = fs_read_string(in);
  printf("name: %s\n", name);
}


void
bit_test_main4()
{
  struct _directory* dir = directory_initial("/root/sdoc", "/root/index", 0);
  struct _file_stream* in = directory_open_file(dir, "segments", ".x", "w");
  char* n = "hello";
  fs_write_string(in, n);
}

