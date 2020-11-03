/*
 * author: Thomas Yao
 */

#ifndef _FIELD_INFO_H_
#define _FIELD_INFO_H_

#include "../store/directory.h"
#include "../document/document.h"

struct _field_info
{
  char* name;
  int is_indexed;
  int number;
  int store_term_vector;
};

struct _field_info*
field_info_initial(char* _name, int _is_indexed, int _number, int _store_term_vector);

struct _field_infos_node
{
  struct _field_infos_node* next;
  struct _field_infos_node* prev;
  struct _field_info* data;
  int index;
};

struct _field_infos
{
  struct _field_infos_node* curr;
  int size;
};

struct _field_infos*
field_infos_initial();

struct _field_infos*
field_infos_initial_read(struct _directory* dir, char* segment_name);
void
field_infos_add_doc(struct _field_infos* _s, struct _document* _doc);

void
field_infos_add(struct _field_infos* s, struct _field_info* _data);

struct _field_info*
field_infos_pop(struct _field_infos* _s);

void
field_infos_rm_curr(struct _field_infos* s);

void
field_infos_destory(struct _field_infos* s);

void
field_infos_print(struct _field_infos* s);

struct _field_info*
field_infos_get_by_name(struct _field_infos* _fis, char* _field_name);

int
field_infos_get_number_by_name(struct _field_infos* s, char* _name);

char*
field_infos_get_name_by_number(struct _field_infos* _s, int _number);

struct _field_info*
field_infos_get_by_number(struct _field_infos* s, int _number);

struct _field_info*
field_infos_get_index(struct _field_infos* s, int _index);


void
field_infos_write(struct _field_infos* _s, struct _directory* _dir,
    char* _sagment, char* _suffix);

void
field_infos_read(struct _field_infos* fis, struct _file_stream* input);

void
add_internal(struct _field_infos* fis, char* name, int is_indexed,
    int store_term_vector);

void
field_infos_add_indexed(struct _field_infos* fis,
    struct _char_list* fieldnames, int _store_term_vectors);

void
field_infos_add_notyet(struct _field_infos* fis, char* _name, int _is_indexed,
    int _store_term_vector);

void
field_infos_add_not_storetermvectors(struct _field_infos* fis,
    struct _char_list* fieldnames, int _is_index);

#endif

