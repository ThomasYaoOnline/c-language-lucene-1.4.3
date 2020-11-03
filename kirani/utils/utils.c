/*
 * author: Thomas Yao
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

void
utils_tolower(char* _str)
{
  int j = strlen(_str) + 1;
  char str_array[j];
  int i;
  for (i = 0; i < j; i++, *_str++)
    str_array[i] = tolower(*_str);
  _str = str_array;
}

char*
utils_strcat(char* _str1, char* _str2)
{
  char* str3 = (char*) calloc((strlen(_str1) + strlen(_str2)), sizeof(char));
  str3 = strcat(str3, _str1);
  str3 = strcat(str3, _str2);
  return str3;
}

char*
utils_strcat3(char* _str1, char* _str2, char* _str3)
{
  char* str4 = (char*) calloc((strlen(_str1) + strlen(_str2) + strlen(_str3)),
      sizeof(char));
  str4 = strcat(str4, _str1);
  str4 = strcat(str4, _str2);
  str4 = strcat(str4, _str3);
  return str4;
}

char*
utils_strcat4(char* _str1, char* _str2, char* _str3, char* _str4)
{
  char* str5 = (char*) calloc((strlen(_str1) + strlen(_str2) + strlen(_str3)
      + strlen(_str4)), sizeof(char));
  str5 = strcat(str5, _str1);
  str5 = strcat(str5, _str2);
  str5 = strcat(str5, _str3);
  str5 = strcat(str5, _str4);
  return str5;
}

int
utils_string_difference(char* _str1, char* _str2)
{
  int len1 = strlen(_str1);
  int len2 = strlen(_str2);
  int len = len1 < len2 ? len1 : len2;
  int i;
  for (i = 0; i < len; i++)
    {
      if (_str1[i] != _str2[i])
        return i;
    }
  return len;
}

int
utils_wstring_difference(wchar_t* _str1, wchar_t* _str2)
{
  int len1 = wcslen(_str1);
  int len2 = wcslen(_str2);
  int len = len1 < len2 ? len1 : len2;
  int i;
  for (i = 0; i < len; i++)
    {
      if (_str1[i] != _str2[i])
        return i;
    }
  return len;
}

int
utils_str_start_with(char* _str1, char* _prefix)
{
  int i;
  int j = strlen(_prefix);
  for (i = 0; i < j; i++)
    {
      if (_str1[i] != _prefix[i])
        {
          return 0;
        }
    }
  return 1;
}

char*
utils_itoa(int value, int base)
{
  char* result = (char*) malloc(10);

  if (base < 2 || base > 36)
    {
      *result = '\0';
      return result;
    }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do
    {
      tmp_value = value;
      value /= base;
      *ptr++
          = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    }
  while (value);

  // Apply negative sign
  if (tmp_value < 0)
    *ptr++ = '-';
  *ptr-- = '\0';
  while (ptr1 < ptr)
    {
      tmp_char = *ptr;
      *ptr-- = *ptr1;
      *ptr1++ = tmp_char;
    }
  return result;
}

