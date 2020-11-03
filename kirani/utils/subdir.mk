# author: Thomas Yao

objs += \
  $(debugpath)char_list.o \
  $(debugpath)float_list.o \
  $(debugpath)int_list.o \
  $(debugpath)long_long_list.o \
  $(debugpath)utils.o \
  $(debugpath)wchar_list.o

$(debugpath)char_list.o: utils/char_list.c 
	cc -c -o $@ $<
$(debugpath)float_list.o: utils/float_list.c
	cc -c -o $@ $<
$(debugpath)int_list.o: utils/int_list.c
	cc -c -o $@ $<
$(debugpath)long_long_list.o: utils/long_long_list.c
	cc -c -o $@ $<
$(debugpath)utils.o: utils/utils.c
	cc -c -o $@ $<
$(debugpath)wchar_list.o: utils/wchar_list.c
	cc -c -o $@ $<

