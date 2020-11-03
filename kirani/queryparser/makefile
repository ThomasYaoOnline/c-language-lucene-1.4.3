# author: Thomas Yao

qry : qry query_parser.o lexyy.o
	cc -o $@ $^

query_parser.o : query_parser.c
	cc -c $<

lexyy.o : lexyy.c
	cc -c $<

query_parser.c : query_parser.y
	bison -oquery_parser.c -d query_parser.y

lexyy.c : query_parser.l
	flex -olexyy.c query_parser.l

.PHONY: clean
clean:
	rm qry query_parser.o query_parser.c query_parser.h lexyy.o lexyy.c
