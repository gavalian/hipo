all: 
	@test -d lib || mkdir -p lib
	@cd lz4 ; make lz4
	@cp lz4/lib/liblz4.a lib/.
	@cd hipo4; make

clean:
	@cd Lz4; make clean
	@cd hipo4; make clean
	@echo "Cleaning lib directory"
	@rm -rf lib/*
