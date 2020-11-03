/*
 * author: Thomas Yao
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "directory.h"

#include "../utils/char_list.h"
#include "../utils/utils.h"

struct _directory*
directory_initial(char* _files_dir_name, char* _index_dir_name, int _create)
{
  struct _directory* dir = (struct _directory*) malloc(
      sizeof(struct _directory));

  if (dir == NULL)
    err(1, "dir is null");

  dir->index_dir_name = _index_dir_name;
  dir->file_names = char_list_initial();

  if (_files_dir_name != NULL)
    char_list_add(dir->file_names, _files_dir_name);

  return dir;
}

char*
directory_get_lock_prefix(struct _directory* _dir)
{
  //char* dir_name = _dir->dir_name;
  //int digest;
  char* result = "lock_lazy";
  return result;
}

void
directory_create(struct _directory* _dir)
{
  DIR* dir = opendir(_dir->dir_name);
  struct dirent *ep;
  struct _char_list* files;

  if (dir != NULL)
    {
      if (mkdir(_dir->dir_name, 744) == -1)
        {
          err(1, "cann't create directory");
        }
    }

  /* clear old files */
  while ((ep = readdir(dir)) != NULL)
    {
      remove(ep->d_name);
      // unlink(ep->d_name);
    }
  (void) closedir(dir);

  remove(_dir->dir_name);
  //rmdir(_dir->dir_name);

  /* clear old locks */

  char* lock_prefix = directory_get_lock_prefix(_dir);

  DIR* lock_dir = opendir(_dir->lock_dir_name);
  while ((ep = readdir(lock_dir)) != NULL)
    {
      char_list_add(files, ep->d_name);
    }

  struct _char_list_node* curr_node = files->curr;

  int i;
  int j = files->size;
  for (i = 0; i < j; i++)
    {
      utils_str_start_with(curr_node->data, lock_prefix);
      continue;
      remove(_dir->lock_dir_name);
      curr_node = curr_node->prev;
    }
}

struct _file_stream*
directory_open_file(struct _directory* _dir, char* _file_name, char* _suffix,
    char* _open_type)
{
  char* file_path = utils_strcat4(_dir->index_dir_name, "/", _file_name,
      _suffix);

  struct _file_stream* fs = fs_initial(file_path, _open_type);
  fs->stream_path = strdup(file_path);
  free(file_path);
  return fs;
}

struct _directory*
directory_get_directory(struct _directory* _dir, char* _path, int _create)
{
  //DIR* dir = opendir(_path);
  struct _directory* dir;
  dir = directory_initial(NULL, _path, _create);
  /*
   dir = char_directorys_get(_dir->dirs, _path);

   if (dir == NULL)
   {
   //dir = directory_initial(_path, _create);
   char_directorys_add(_dir->dirs, _path, dir);
   }
   else if (_create)
   {
   create(_dir);
   }

   dir->ref_count++;
   */
  return dir;
}

void
directory_pop_files(struct _directory* _dir)
{
  DIR* dp;
  struct dirent* ep;

  struct stat* file_attributes = (struct stat*) malloc(sizeof(struct stat));

  char* file_name;
  char* file_name_p;
  struct _char_list* file_names_catch = char_list_initial();

  char_list_add(file_names_catch, char_list_pop(_dir->file_names));

  while (file_names_catch->size != 0)
    {
      file_name_p = char_list_pop(file_names_catch);
      dp = opendir(file_name_p);

      if (dp == NULL)
        {
          continue;
          //err(1, "couldn't open the directory");
        }

      while ((ep = readdir(dp)) != NULL)
        {
          if (0 == strcmp(ep->d_name, ".") || 0 == strcmp(ep->d_name, ".."))
            continue;
          file_name = utils_strcat3(file_name_p, "/", ep->d_name);
          (void) stat(file_name, file_attributes);

          if ((file_attributes->st_mode & S_IFMT) == S_IFLNK)
            continue;

          if ((file_attributes->st_mode & S_IFMT) == S_IFDIR)
            {
              char_list_add(file_names_catch, file_name);
            }
          else
            {
              char_list_add(_dir->file_names, file_name);
            }
        }
    }

  (void) closedir(dp);
  free(file_attributes);
  char_list_destory(file_names_catch);
}

struct _lock*
directory_make_lock(struct _directory* _dir, char* name)
{
  struct _lock* lk = lock_initial();
  return lk;
}

void
directory_close(struct _directory* _dir)
{
  return;
}

struct _char_directorys*
char_directorys_initial()
{
  struct _char_directorys* map = (struct _char_directorys*) malloc(
      sizeof(struct _char_directorys));
  if (map == NULL)
    err(1, "map is null");
  map->curr = NULL;
  map->size = 0;
  return map;
}

void
char_directorys_add(struct _char_directorys* _s, char* _name,
    struct _directory* _dir)
{
  struct _char_directorys_node* node = (struct _char_directorys_node*) malloc(
      sizeof(struct _char_directorys_node));

  if (node == NULL)
    err(1, "node is null");

  if (_s->size == 0)
    {
      _s->curr = node;
      node->next = node;
      node->prev = node;
    }

  struct _char_directorys_node* cn = _s->curr;
  struct _char_directorys_node* nn = _s->curr->next;

  cn->next = node;
  nn->prev = node;

  node->next = nn;
  node->prev = cn;

  node->name = _name;
  node->dir = _dir;

  _s->curr = node;
  _s->curr->index = _s->size++;
}

void
char_directorys_rm_curr(struct _char_directorys* s)
{
  if (s == NULL)
    err(1, "_s is null");
  struct _char_directorys_node* cn = s->curr;
  cn->prev->next = cn->next;
  cn->next->prev = cn->prev;

  s->curr = cn->prev;
  s->size--;
  free(cn);
}

struct _directory*
char_directorys_get(struct _char_directorys* _s, char* _dir_name)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      if (0 == strcmp(_s->curr->dir->dir_name, _dir_name))
        return _s->curr->dir;
      else
        _s->curr = _s->curr->next;
    }
  return NULL;
}


struct _directory*
char_directorys_get_index(struct _char_directorys* s, int _index)
{
  int i;
  for (i=0; i<s->size; i++)
    {
      if (s->curr->index == _index)
        return s->curr->dir;
      s->curr = s->curr->next;
    }
  return NULL;
}


void
char_directorys_destory(struct _char_directorys* _s)
{
  if (_s == NULL)
    err(1, "_s is null");
  while (_s->size != 0)
    {
      char_directorys_rm_curr(_s);
    }
  free(_s);
}

