# sources
SRCS = main.c iterator.c file.c
OBJS = $(SRCS:.c=.o)
TARG = wordc

compile: $(TARG)
debug: $(TARG)
	gdb $(TARG)
run: $(TARG)
	./$(TARG)
clean:
	rm -f *.o *~ $(TARG)


$(TARG): $(OBJS)
	gcc -g $(OBJS) -o $(TARG)
$(OBJS):$(SRCS)
	gcc -c -Wall $(SRCS)
AN:
	diff countfile countfile.an
PB:
	diff countfile countfile.bp
HA:
	diff countfile countfile.ha
