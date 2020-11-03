/*
 * author: Thomas Yao
 */

#ifndef _FILE_STREAM_H_
#define _FILE_STREAM_H_

#include <stdio.h>
#include <wchar.h>

#include "../utils/char_list.h"
#include "../utils/int_list.h"

#define FILE_STREAM_BUFFER_SIZE 1024

struct _descriptor
{
  long long position;
  FILE* stream;
};

struct _file_stream
{
  FILE* stream;
  long long buffer_start;
  int buffer_length;
  int buffer_position;
  long long length;
  struct _char_list* chars;
  int buffer[1024];
  struct _descriptor* file;
  char* stream_path;
};

struct _file_stream*
fs_initial(char* _file_name, char* _open_type);

struct _file_stream*
fs_initial_mem(char* string_value, char* open_type);

struct _file_stream*
fs_initial_mem2();

struct _file_stream*
fs_initial_clone(struct _file_stream* s);

int
fs_close(struct _file_stream* _fs);
int
fs_seek(struct _file_stream* _fs, long long _offset);
long long
fs_tell(struct _file_stream* _fs);

/* read methods */
int
fs_read_byte(struct _file_stream* s);
int
fs_read_vint(struct _file_stream* s);
int
fs_read_int(struct _file_stream* _fs);
long long
fs_read_vlong(struct _file_stream* s);
long long
fs_read_long(struct _file_stream* _fs);
void
fs_read_chars(struct _file_stream* s, char buffer[16], int start, int length);
char*
fs_read_string(struct _file_stream* _fs);
void
fs_read_bytes(struct _file_stream* s, int b[], int offset, int len);

/* write methods */
void
fs_write_byte(struct _file_stream* s, char i);
void
fs_write_vint(struct _file_stream*s, int i);
void
fs_write_int(struct _file_stream* _fs, int _data);
void
fs_write_vlong(struct _file_stream* s, long long i);
void
fs_write_long(struct _file_stream* _fs, long long _data);
void
fs_write_chars(struct _file_stream* s, char _data[], int start, int length);
void
fs_write_string(struct _file_stream* _fs, char* _data);
void
fs_write_to(struct _file_stream* s, struct _file_stream* out);
void
fs_write_string_sl(struct _file_stream* _fs, char _data[], int _start,
    int _length);

void
fs_write_wchar(struct _file_stream* _fs, wchar_t _data);
void
fs_write_wstring(struct _file_stream* _fs, wchar_t* _data);
void
fs_write_wstring_sl(struct _file_stream* _fs, wchar_t _data[], int _start,
    int _length);

#endif

