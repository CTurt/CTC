.PHONY: clean

ctc: ctc.c main.c
	gcc $^ -o $@ -Wall -O2

clean:
	@rm -f ctc
	@rm -f ctc.exe