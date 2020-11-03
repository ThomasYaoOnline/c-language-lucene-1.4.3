# author: Thomas Yao

objs += \
  $(debugpath)directory.o \
  $(debugpath)file_stream.o \
  $(debugpath)lock.o

$(debugpath)directory.o: store/directory.c
	cc -c -o $@ $<
$(debugpath)file_stream.o: store/file_stream.c
	cc -c -o $@ $<
$(debugpath)lock.o: store/lock.c
	cc -c -o $@ $<
