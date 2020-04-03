#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "memory.h"

int main(int argc, char **argv)
{
	srand(time(NULL));
	printf("MEMORY GAME\n");
	int difficulty=1;
	bool correct=true;
	int* led_seq;

	while(correct){
		printf("LEVEL %d \n",difficulty);
		led_seq=led(difficulty);

		for(int i=0;i<difficulty;i++){
			printf("%d ",led_seq[i]);
		}
		
		printf("YOUR TURN!\n");
		correct=button(led_seq,difficulty);
		if(!correct){
			printf("MISTAKE!\n");
			break;
		}
		printf("CORRECT!\n\n");
		difficulty++;
	}

	printf("END OF GAME!\n");

	return 0;
}
