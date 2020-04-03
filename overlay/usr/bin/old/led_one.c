#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

void led()
{
	char *chipname = "gpiochip1";
	unsigned int line_num = 24;	
	unsigned int val;
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int i, ret;

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		return;
	}

	for(i=line_num;i<line_num+8;i++){
		line = gpiod_chip_get_line(chip, i);
		if (!line) {
			perror("Get line failed\n");
			gpiod_chip_close(chip);
			return ;
		}

		ret = gpiod_line_request_output(line, "led_blink", 0);
		if (ret < 0) {
			perror("Request line as output failed\n");
			gpiod_line_release(line);
			return;
		}

		val=1;

		ret = gpiod_line_set_value(line, val);
		if (ret < 0) {
			perror("Set line output failed\n");
			gpiod_line_release(line);
			return;
		}
		printf("Output %u on line #%u\n", val, line_num);
		sleep(1);
		val = !val;
		ret = gpiod_line_set_value(line, val);
		if (ret < 0) {
			perror("Set line output failed\n");
			gpiod_line_release(line);
			return;
		}
		printf("Output %u on line #%u\n", val, line_num);
		val = !val;
		gpiod_line_release(line);
	}

	return;
}
