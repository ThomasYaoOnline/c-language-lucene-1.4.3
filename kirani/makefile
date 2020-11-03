# author: Thomas Yao

debugpath := debug/

objs := $(debugpath)kirani.o
rmobjs := kirani

include utils/subdir.mk
include store/subdir.mk
include document/subdir.mk
include analysis/subdir.mk
include index/subdir.mk
include search/subdir.mk
include queryparser/subdir.mk

kirani: $(objs)
	cc -o $@ $^ -lm
	
$(debugpath)kirani.o: kirani.c
	cc -c -o $@ $<
	
.PHONY: clean
clean:
	rm $(objs) $(rmobjs) 

