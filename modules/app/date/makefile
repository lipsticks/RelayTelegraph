MODULES:=../..

date.exe: date.cpp lib.date.o
	@g++ -o $@ $^

lib.date.o: $(MODULES)/lib/date/date.cpp
	@g++ -c -o $@ $^

run: date.exe
	@./date.exe

clean:
	@rm -f date.exe
	@rm -f *.o
