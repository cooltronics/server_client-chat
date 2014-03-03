sub-make:
	$(MAKE) -C src/ MAKEFLAGS=DEBUG
clean:
	cd src/ && make clean

