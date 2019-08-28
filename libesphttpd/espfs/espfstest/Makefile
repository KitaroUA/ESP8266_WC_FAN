CFLAGS=-I../../lib/heatshrink -I.. -std=gnu99 -DESPFS_HEATSHRINK

espfstest: main.o espfs.o heatshrink_decoder.o
	$(CC) -o $@ $^

espfs.o: ../espfs.c
#	$(CC) $(CFLAGS) -c $^ -o $@
	$(CC) $(INCDIR) $(MODULE_INCDIR) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CFLAGS) -c $^ -o $@

heatshrink_decoder.o: ../heatshrink_decoder.c
#	$(CC) $(CFLAGS) -c $^ -o $@
	$(CC) $(INCDIR) $(MODULE_INCDIR) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o espfstest
