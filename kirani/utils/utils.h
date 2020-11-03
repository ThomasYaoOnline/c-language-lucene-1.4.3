/*
 * author: Thomas Yao
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <wchar.h>

void
utils_tolower(char* _str);

char*
utils_strcat(char* _str1, char* _str2);

char*
utils_strcat3(char* _str1, char* _str2, char* _str3);

char*
utils_strcat4(char* _str1, char* _str2, char* _str3, char* str4);

int
utils_string_difference(char* _str1, char* _str2);

int
utils_wstring_difference(wchar_t* _str1, wchar_t* _str2);

int
utils_str_start_with(char* _str1, char* _prefix);

char*
utils_itoa(int value, int base);


#endif

