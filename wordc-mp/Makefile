# sources
SRCS = wordc-mp.c wordc.c 
OBJS = $(SRCS:.c=.o)
TARG = wordc-mp

compile: $(TARG)
debug: $(TARG)
	gcc -g -c $(SRCS)
	gdb $(TARG)
run: $(TARG)
	./$(TARG)
clean:
	rm -f *.o *~ $(TARG)


$(TARG): $(OBJS)
	gcc -g $(OBJS) -o $(TARG)
$(OBJS):$(SRCS)
	gcc -c $(SRCS)
AN:
	diff countfile countfile.an
PB:
	diff countfile countfile.bp
HA:
	diff countfile countfile.ha
