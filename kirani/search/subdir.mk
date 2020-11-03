# aughor: Thomas Yao
objs += \
  $(debugpath)clause.o \
  $(debugpath)explanation.o \
  $(debugpath)filter.o \
  $(debugpath)hit_queue.o \
  $(debugpath)hits.o \
  $(debugpath)index_searcher.o \
  $(debugpath)query.o \
  $(debugpath)scorer.o \
  $(debugpath)similarity.o \
  $(debugpath)sort_comparator_source.o \
  $(debugpath)sort_field.o \
  $(debugpath)sort.o \
  $(debugpath)top_docs.o

$(debugpath)clause.o: search/clause.c
	cc -c -o $@ $<
$(debugpath)explanation.o: search/explanation.c
	cc -c -o $@ $<
$(debugpath)filter.o: search/filter.c
	cc -c -o $@ $<
$(debugpath)hit_queue.o: search/hit_queue.c
	cc -c -o $@ $<
$(debugpath)hits.o: search/hits.c
	cc -c -o $@ $<
$(debugpath)index_searcher.o: search/index_searcher.c
	cc -c -o $@ $<
$(debugpath)query.o: search/query.c
	cc -c -o $@ $<
$(debugpath)scorer.o: search/scorer.c
	cc -c -o $@ $<
$(debugpath)similarity.o: search/similarity.c
	cc -c -o $@ $<
$(debugpath)sort_comparator_source.o: search/sort_comparator_source.c
	cc -c -o $@ $<
$(debugpath)sort_field.o: search/sort_field.c
	cc -c -o $@ $<
$(debugpath)sort.o: search/sort.c
	cc -c -o $@ $<
$(debugpath)top_docs.o: search/top_docs.c
	cc -c -o $@ $<


