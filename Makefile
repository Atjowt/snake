all:
	$(MAKE) -C snake-server
	$(MAKE) -C snake-client

clean:
	$(MAKE) -C snake-server clean
	$(MAKE) -C snake-client clean

.PHONY: all clean
