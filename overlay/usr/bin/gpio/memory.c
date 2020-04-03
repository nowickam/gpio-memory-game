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
	int* led_seq;

	while(correct){
		printf("WATCH %d SIGNALS \n",difficulty);
		led_seq=led(difficulty);
		printf("YOUR TURN!\n");
		correct=button(led_seq,difficulty);
		difficulty++;

		//TODO remove
		if(difficulty==5)correct=false;
	}

	printf("END OF GAME!\n");

	return 0;
}
