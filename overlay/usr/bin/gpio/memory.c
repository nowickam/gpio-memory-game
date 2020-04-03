#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "memory.h"

int main(int argc, char **argv)
{
	srand(time(NULL));
	printf("MEMORY GAME\n");
	int difficulty=3;
	bool correct=true;

	while(correct){
		int* led_seq;
		led_seq=led(difficulty);
		correct=button(led_seq);
		difficulty++;

		//TODO remove
		if(difficulty==5)correct=false;
	}

	printf("END OF GAME!\n");

	return 0;
}
