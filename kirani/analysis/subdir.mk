# author: Thomas Yao

objs +=\
  $(debugpath)analyzer.o\
  $(debugpath)token_stream.o\
  $(debugpath)token.o\

rmobjs +=\
  analysis/token_stream.c

$(debugpath)analyzer.o: analysis/analyzer.c 
	cc -c -o $@ $<

$(debugpath)token_stream.o: analysis/token_stream.c 
	cc -c -o $@ $<

analysis/token_stream.c: analysis/token_stream.l
	flex -8 -Pts -o$@ $<

$(debugpath)token.o: analysis/token.c
	cc -c -o $@ $<



