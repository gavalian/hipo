all: 
	@cd Lz4 ; make lz4
	@cp Lz4/lib/liblz4.a lib/.
	@cd Hipo4; make
#	@cd Banks; make
#	@cd Utils; make

clean:
	@cd Lz4; make clean
	@cd Hipo4; make clean
#	@cd Banks; make clean
#	@cd Utils; make clean
	@echo "Cleaning lib directory"
	@rm -rf lib/*
