# author: Thomas Yao
objs += \
  $(debugpath)documents_writer.o \
  $(debugpath)field_info.o \
  $(debugpath)fields_reader.o \
  $(debugpath)fields_writer.o \
  $(debugpath)index_reader.o \
  $(debugpath)index_writer.o \
  $(debugpath)norm.o \
  $(debugpath)segment_info.o \
  $(debugpath)segment_merge_info.o \
  $(debugpath)segment_merger.o \
  $(debugpath)term_docs.o \
  $(debugpath)term_enum.o \
  $(debugpath)term_info.o \
  $(debugpath)term_infos_reader.o \
  $(debugpath)term_infos_writer.o \
  $(debugpath)term_posting.o \
  $(debugpath)term_vector.o \
  $(debugpath)term_vectors_reader.o \
  $(debugpath)term_vectors_writer.o \
  $(debugpath)tv_field.o \
  $(debugpath)tv_term.o

$(debugpath)documents_writer.o: index/documents_writer.c
	cc -c -o $@ $<
$(debugpath)field_info.o: index/field_info.c
	cc -c -o $@ $<
$(debugpath)fields_reader.o: index/fields_reader.c
	cc -c -o $@ $<
$(debugpath)fields_writer.o: index/fields_writer.c
	cc -c -o $@ $<
$(debugpath)index_reader.o: index/index_reader.c
	cc -c -o $@ $<
$(debugpath)index_writer.o: index/index_writer.c
	cc -c -o $@ $<
$(debugpath)norm.o: index/norm.c
	cc -c -o $@ $<
$(debugpath)segment_info.o: index/segment_info.c
	cc -c -o $@ $<
$(debugpath)segment_merge_info.o: index/segment_merge_info.c
	cc -c -o $@ $<
$(debugpath)segment_merger.o: index/segment_merger.c
	cc -c -o $@ $<
$(debugpath)term_docs.o: index/term_docs.c
	cc -c -o $@ $<
$(debugpath)term_enum.o: index/term_enum.c
	cc -c -o $@ $<
$(debugpath)term_info.o: index/term_info.c
	cc -c -o $@ $<
$(debugpath)term_infos_reader.o: index/term_infos_reader.c
	cc -c -o $@ $<
$(debugpath)term_infos_writer.o: index/term_infos_writer.c
	cc -c -o $@ $<
$(debugpath)term_posting.o: index/term_posting.c
	cc -c -o $@ $<
$(debugpath)term_vector.o: index/term_vector.c
	cc -c -o $@ $<
$(debugpath)term_vectors_reader.o: index/term_vectors_reader.c
	cc -c -o $@ $<
$(debugpath)term_vectors_writer.o: index/term_vectors_writer.c
	cc -c -o $@ $<
$(debugpath)tv_field.o: index/tv_field.c
	cc -c -o $@ $<
$(debugpath)tv_term.o: index/tv_term.c
	cc -c -o $@ $<


