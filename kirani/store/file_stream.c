/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_stream.h"

#include "../utils/utils.h"

struct _file_stream*
fs_initial(char* _file_name, char* _open_type)
{
  struct _file_stream* fs = (struct _file_stream*) malloc(
      sizeof(struct _file_stream));
  if (fs == NULL)
    err(1, "fs is null");
  fs->stream = fopen(_file_name, _open_type);
  return fs;
}

struct _file_stream*
fs_initial_mem(char* string_value, char* open_type)
{
  struct _file_stream* fs = (struct _file_stream*) malloc(
      sizeof(struct _file_stream));
  if (fs == NULL)
    err(1, "fs is null");

  fs->stream = fmemopen(string_value, strlen(string_value), open_type);
  return fs;
}

struct _file_stream*
fs_initial_mem2()
{
  struct _file_stream* fs = (struct _file_stream*) malloc(
      sizeof(struct _file_stream));
  char* bp;
  size_t size;
  fs->stream = open_memstream(&bp, &size);
  fs->stream_path = "mem";
  return fs;
}

struct _file_stream*
fs_initial_clone(struct _file_stream* s)
{
  struct _file_stream* clone = (struct _file_stream*) malloc(
      sizeof(struct _file_stream));

  int b;
  char* bp;
  size_t size;
  clone->stream = open_memstream(&bp, &size);
  while ((b = fgetc(s->stream)) != EOF)
    fputc(b, clone->stream);
  fseek(s->stream, 0, SEEK_SET);
  fseek(clone->stream, 0, SEEK_SET);
  //fflush(clone->stream);  //world a error
  clone->stream_path = strdup(s->stream_path);
  return clone;
}

int
fs_close(struct _file_stream* _fs)
{
  return fclose(_fs->stream);
}

int
fs_seek(struct _file_stream* _fs, long long _offset)
{
  return fseek(_fs->stream, _offset, SEEK_SET);
}

long long
fs_tell(struct _file_stream* _fs)
{
  return ftell(_fs->stream);
}

void
read_internal(struct _file_stream* s, int b[100], int offset, int len)
{
  long position = fs_tell(s);
  if (position != s->file->position)
    {
      fseek(s->file->stream, position, SEEK_SET);
      s->file->position = position;
    }
  int total = 0;
  do
    {
      int i = 0;//fs_read_bytes(file, b, offset + total, len - total);
      if (i == -1)
        err(1, "read past EOF");
      s->file->position += i;
      total += i;
    }
  while (total < len);
}

void
seek_internal(struct _file_stream* s, long position)
{
}

/* read methods */
int
fs_read_byte(struct _file_stream* s)
{
  return fgetc(s->stream);
}

int
fs_read_vint(struct _file_stream* s)
{
  int b = fgetc(s->stream);
  int i = b & 0x7F;
  int shift;
  for (shift = 7; (b & 0x80) != 0; shift += 7)
    {
      b = fgetc(s->stream);
      i |= (b & 0x7F) << shift;
    }
  return i;
}

int
fs_read_int(struct _file_stream* s)
{
  return ((fgetc(s->stream) & 0xFF) << 24) | ((fgetc(s->stream) & 0xFF) << 16)
      | ((fgetc(s->stream) & 0xFF) << 8) | (fgetc(s->stream) & 0xFF);
}

long long
fs_read_vlong(struct _file_stream* s)
{
  int b = fgetc(s->stream);
  long long i = b & 0x7F;
  int shift;
  for (shift = 7; (b & 0x80) != 0; shift += 7)
    {
      b = fgetc(s->stream);
      i |= (b & 0x7FL) << shift;
    }
  return i;
}

long long
fs_read_long(struct _file_stream* s)
{
  long long i = (((long long) fs_read_int(s)) << 32) | (fs_read_int(s)
      & 0xFFFFFFFFL);
  return i;
}

void
fs_read_chars(struct _file_stream* s, char buffer[16], int start, int length)
{
  int end = start + length;
  int i;
  int b;
  for (i = start; i < end; i++)
    {
      b = fgetc(s->stream);
      if ((b & 0x80) == 0)
        buffer[i] = (char) (b & 0x7F);
      else if ((b & 0xE0) != 0xE0)
        {
          buffer[i] = (char) (((b & 0x1F) << 6) | (fgetc(s->stream) & 0x3F));
        }
      else
        buffer[i] = (char) (((b & 0x0F) << 12) | ((fgetc(s->stream) & 0x3F)
            << 6) | (fgetc(s->stream) & 0x3F));
    }
  buffer[i++] = '\000';
}

char*
fs_read_string(struct _file_stream* s)
{
  int length = fs_read_vint(s);
  static char chars[100];
  fs_read_chars(s, chars, 0, length);
  return strdup(chars);
}

void
fs_read_bytes(struct _file_stream* s, int b[], int offset, int len)
{
  if (len < FILE_STREAM_BUFFER_SIZE)
    {
      int i;
      for (i = 0; i < len; i++) // read byte-by-byte
        b[i + offset] = (int) fs_read_byte(s);
    }
  else
    { // read all-at-once
      long start = fs_tell(s);
      seek_internal(s, start);
      read_internal(s, b, offset, len);

      s->buffer_start = start + len; // adjust stream variables
      s->buffer_position = 0;
      s->buffer_length = 0; // trigger refill() on read
    }
}

/* write methods */

void
fs_write_byte(struct _file_stream* s, char i)
{
  fputc(i, s->stream);
}

void
fs_write_vint(struct _file_stream*s, int i)
{
  while ((i & ~0x7F) != 0)
    {
      fputc((int) ((i & 0x7f) | 0x80), s->stream);
      i >>= 7;
    }
  fputc((int) i, s->stream);
}

void
fs_write_int(struct _file_stream* s, int i)
{
  fputc(i >> 24, s->stream);
  fputc(i >> 16, s->stream);
  fputc(i >> 8, s->stream);
  fputc(i, s->stream);
}

void
fs_write_vlong(struct _file_stream* s, long long i)
{
  while ((i & ~0x7F) != 0)
    {
      fputc((int) ((i & 0x7f) | 0x80), s->stream);
      i >>= 7;
    }
  fputc((int) i, s->stream);
}

void
fs_write_long(struct _file_stream* s, long long i)
{
  fs_write_int(s, (int) (i >> 32));
  fs_write_int(s, (int) i);
}

void
fs_write_chars(struct _file_stream* s, char _data[], int start, int length)
{
  int end = start + length;
  int i;
  int code;
  for (i = start; i < end; i++)
    {
      code = (int) _data[i];
      if (code >= 0x01 && code <= 0x7F)
        fputc((int) code, s->stream);
      else if (((code >= 0x80) && (code <= 0x7FF)) || code == 0)
        {
          fputc((int) (0xC0 | (code >> 6)), s->stream);
          fputc((int) (0x80 | (code & 0x3F)), s->stream);
        }
      else
        {
          fputc((int) (0xE0 | (code >> 12)), s->stream);
          fputc((int) (0x80 | ((code >> 6) & 0x3F)), s->stream);
          fputc((int) (0x80 | (code & 0x3F)), s->stream);
        }
    }
}

void
fs_write_string(struct _file_stream* s, char* _data)
{
  int length = strlen(_data);
  fs_write_vint(s, length);
  fs_write_chars(s, _data, 0, length);
}

/** Copy the current contents of this buffer to the named output. */
void
fs_write_to(struct _file_stream* s, struct _file_stream* out)
{
  //flush();
  long long end = 1024; //file.length;
  long long pos = 0;
  int buffer = 0;
  while (pos < end)
    {
      int length = FILE_STREAM_BUFFER_SIZE;
      long long next_pos = pos + length;
      if (next_pos > end)
        { // at the last buffer
          length = (int) (end - pos);
        }
      //fs_write_bytes(((int[]) file.buffers.elementAt(buffer++), length);
      pos = next_pos;
    }
}

void
fs_write_string_sl(struct _file_stream* _fs, char _data[], int _start,
    int _length)
{
  int i;
  for (i = _start; i < _length; i++)
    {
      fputc(_data[i], _fs->stream);
    }
}

// wide character methods
void
fs_write_wchar(struct _file_stream* _fs, wchar_t _data)
{
  fputwc(_data, _fs->stream);
}

void
fs_write_wstring(struct _file_stream* _fs, wchar_t* _data)
{
  int i;
  int j = wcslen(_data);
  for (i = 0; i < j; i++)
    {
      fputwc(_data[i], _fs->stream);
    }
}

void
fs_write_wstring_sl(struct _file_stream* _fs, wchar_t _data[], int _start,
    int _length)
{
  int i;
  for (i = _start; i < _length; i++)
    {
      fputwc(_data[i], _fs->stream);
    }
}

