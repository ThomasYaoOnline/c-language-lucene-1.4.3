/*
 * author: Thomas Yao
 */

#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include <wchar.h>
#include <dirent.h>

#include "file_stream.h"
#include "lock.h"

#include "../utils/utils.h"
#include "../utils/char_list.h"

struct _char_directorys_node
{
  struct _char_directorys_node* next;
  struct _char_directorys_node* prev;
  char* name;
  struct _directory* dir;
  int index;
};

struct _char_directorys
{
  struct _char_directorys_node* curr;
  int size;
};

struct _char_directorys*
char_directorys_initial();

void
char_directorys_add(struct _char_directorys* _s, char* _name,
    struct _directory* _dir);

struct _directory*
char_directorys_get(struct _char_directorys* _s, char* _dir);

struct _directory*
char_directorys_get_index(struct _char_directorys* s, int _index);

void
char_directorys_destory(struct _char_directorys* _s);

struct _directory
{
  char* index_dir_name;
  struct _char_list* file_names;
  struct _char_directorys* dirs;
  int ref_count;
  char* dir_name;
  char* lock_dir_name;
};

struct _directory*
directory_initial(char* _files_dir_name, char* _index_dir_name, int _create);

struct _file_stream*
directory_open_file(struct _directory* _dir, char* _file_name, char* _suffix,
    char* _open_type);

struct _directory*
directory_get_directory(struct _directory* _dir, char* _path, int _create);

void
directory_pop_files(struct _directory* _dir);

struct _lock*
directory_make_lock(struct _directory* _dir, char* name);

void
directory_close(struct _directory* _dir);

#endif

