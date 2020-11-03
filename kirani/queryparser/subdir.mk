# author: Thomas Yao

objs +=\
	$(debugpath)query_parser.o\
	$(debugpath)query_parser_lex.o
	
rmobjs +=\
    queryparser/query_parser.c\
    queryparser/query_parser.h\
    queryparser/query_parser_lex.c

$(debugpath)query_parser.o: queryparser/query_parser.c 
	cc -c -o $@ $<
queryparser/query_parser.c: queryparser/query_parser.yy
	bison -p qp -d -o $@ $<


$(debugpath)query_parser_lex.o: queryparser/query_parser_lex.c
	cc -c -o$@ $<
queryparser/query_parser_lex.c: queryparser/query_parser.ll
	flex -Pqp -o$@ $<


