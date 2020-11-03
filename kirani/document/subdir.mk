# author: Thomas Yao

objs += \
  $(debugpath)document.o \
  $(debugpath)field.o


$(debugpath)document.o: document/document.c
	cc -c -o $@ $<
$(debugpath)field.o: document/field.c
	cc -c -o $@ $<
