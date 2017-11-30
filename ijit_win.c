/**
 ijit:  interactive Just-In-Time compiler

 Win32 program designed to be a relatively reliable simple target 
 for buffer overflow attacks.

 Compile and run on TCP port 8888 with:
	cl ijit_win.c /o ijit.exe
	nc.exe -l -p 8888 | ./ijit.exe

 Use NetCat for Windows:
    https://joncraton.org/blog/46/netcat-for-windows/
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

struct ijit_code;

/** This function pointer runs the jit'd code */
typedef void (*ijit_interpreter)(struct ijit_code *code);

/** This struct stores a block of jit code */
struct ijit_code {
	char name[32]; /* run name */
	char jitted[1024]; /* machine code to run */
	ijit_interpreter interp; /* fallback interpreter function */
};

int main() {
	/* Allocate executable area to store JIT code: */
	
	struct ijit_code *code=(struct ijit_code *)VirtualAlloc(
		(void *)0x3badc000, /* pointer to base address of new jit_code structure */
		4096, /* bytes of code to run */
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE /* allow anything: RWX */
	);
	if (code==(struct ijit_code *)0) {
		printf("VirtualAlloc failed"); exit(1);
	}
	
	/* Set up structure */
	code->interp=0; 
	printf("Loading name into code struct at %p...\n",(void *)code);
	
	/* VULNERABILTY HERE: read name *without* checking name length. */
	if (1!=scanf("%s",code->name)) {
		printf("ERROR in scanf.  Exiting.\n"); exit(1);
	}
	printf("Loaded name %s (%zd bytes)\n", code->name, strlen(code->name));
	
	/* Run the code */
	if (code->interp) {
		printf("Seems to be an interpreter at %p: running it...\n",(void *)code->interp);
		code->interp(code);
		printf("Back from interpreter.\n");
	} else {
		printf("No interpreter found.  Exiting.\n");
	}
	return 0;
}







