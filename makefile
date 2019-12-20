TARGETS=${patsubst %.c,%,${wildcard ./exam.c}}

default:	${TARGETS}

%:	%.c
	${CC} -o $@ $< weighted_graph.c lib/Libfdr/jval.o lib/Libfdr/jrb.o lib/Libfdr/dllist.o -no-pie

clean:
	rm -f ${TARGETS}