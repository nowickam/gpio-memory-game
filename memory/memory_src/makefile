CC=$(CROSS_COMPILE)gcc
OBJS := led.o memory.o button.o
memory: $(OBJS)
	$(CC) -o memory $(CFLAGS) $(LDFLAGS) $(OBJS) -lgpiod
$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) -lgpiod $< -o $@
