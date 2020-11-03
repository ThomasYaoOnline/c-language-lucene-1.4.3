/*
* author: Thomas Yao
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
/*
void char_test_main() {
  char* str1 = "HELLOWORDL";
  char* str2 = (char*) malloc ( sizeof(str1) );

  str2 = str1;

  utils_to_lower_case(str2);

  printf("sss: %s\n", str2);
}
*/

void char_test_main()
{
  char* str1 = "hello";
  const char* str2 = "world";
  char* str3;
  str3 = (char*) calloc(strlen(str1) + strlen(str2), sizeof(char));
  strcat(str3, str1);
  strcat(str3,str2);

  printf("str3: %s\n", str3);
  free(str3);
}

int char_test_main2()
{
  char* str1 = "helloworld";
  int i;
  int j = strlen(str1);
  for (i=0; i<j; i++)
    {
      fputc(str1[i], stdout);
    }
  return 0;
}

