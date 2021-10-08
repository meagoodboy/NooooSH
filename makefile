noooosh: shell.c
	gcc -I. -o noooosh commands/*.c shell.c -fcommon
	chmod +x noooosh