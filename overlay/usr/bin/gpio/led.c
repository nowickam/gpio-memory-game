#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>


int* led(int difficulty)
{
	char *chipname = "gpiochip1";
	struct gpiod_chip *chip;
	int i, j, ret;

	struct gpiod_line_bulk lines;
	unsigned int line_nums[]={24,25,26,27,28,29,30,31};
	unsigned int line_len=8;
	int vals[]={0,0,0,0,0,0,0,0};
	int* led_seq=(int*)calloc(difficulty,sizeof(int));

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		return NULL;
	}
	
	ret=gpiod_chip_get_lines(chip,line_nums,line_len,&lines);
	if (ret < 0) {
		perror("Get lines failed\n");
		gpiod_chip_close(chip);
		return NULL;
	}


	ret=gpiod_line_request_bulk_output(&lines,"led_blink",vals);
	if (ret < 0) {
		perror("Request line as output failed\n");
		gpiod_line_release_bulk(&lines);
		return NULL;
	}

	printf("LEVEL %d\n",difficulty);

	for(i=0;i<difficulty;i++){
		j=rand()%line_len;
		vals[j]=1;
		led_seq[i]=j;

		ret = gpiod_line_set_value_bulk(&lines, vals);
		if (ret < 0) {
			perror("Set line output failed\n");
			gpiod_line_release_bulk(&lines);
			return NULL;
		}
		sleep(1);
		vals[j] = 0;
		ret = gpiod_line_set_value_bulk(&lines, vals);
		if (ret < 0) {
			perror("Set line output failed\n");
			gpiod_line_release_bulk(&lines);
			return NULL;
		}

		sleep(1);
	}


	gpiod_line_release_bulk(&lines);
	gpiod_chip_close(chip);
	free(led_seq);
	return led_seq;
}
