build:
	rm -f hollowing
	gcc -o hollowing main.c

example:
	rm -f example
	gcc -o example example.c -Os -lm	

listen-example:
	nc -lvnp 19999

shellcode:
	echo "shellcode is avaliable in https://github.com/Yyax13/shellcode in src/shellcodes/reverse.asm"

clean:
	rm -f hollowing
	rm -f example
