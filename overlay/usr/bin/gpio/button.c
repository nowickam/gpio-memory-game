#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

bool button(int *led_seq, int difficulty)
{
    char *chipname = "gpiochip1";
    struct gpiod_chip *chip;
    int i, j, ret;

    struct gpiod_line_bulk lines;
    struct gpiod_line_bulk event_lines;
    struct gpiod_line_event event;
    struct gpiod_line *line;
    unsigned int event_size;
    unsigned int line_nums[] = {12, 13, 14, 15, 16, 17, 18, 19};
    unsigned int line_len = 8;

    struct timespec ts = {5, 0};

    chip = gpiod_chip_open_by_name(chipname);
    if (!chip)
    {
        perror("Open chip failed\n");
        return NULL;
    }

    ret = gpiod_chip_get_lines(chip, line_nums, line_len, &lines);
    if (ret < 0)
    {
        perror("Get lines failed\n");
        gpiod_chip_close(chip);
        return NULL;
    }

    ret = gpiod_line_request_bulk_falling_edge_events(&lines, "button_click");
    if (ret < 0)
    {
        perror("Request falling edge event failed!\n");
        gpiod_line_release_bulk(&lines);
        gpiod_chip_close(chip);
        return NULL;
    }

    i = 0;

    while (i < difficulty)
    {
        ret = gpiod_line_event_wait_bulk(&lines, &ts, &event_lines);
        if (ret < 0)
        {
            perror("Wait failed!");
            gpiod_line_release_bulk(&lines);
            gpiod_chip_close(chip);
            return NULL;
        }
        else if (ret == 0)
        {
            printf("TIME IS UP!");
            break;
        }

        event_size=gpiod_line_bulk_num_lines(&event_lines);

        for (j = 0; j < event_size; j++)
        {
            line = gpiod_line_bulk_get_line(&event_lines, j);
            if (ret < 0)
            {
                perror("Reading event number failed!");
                gpiod_line_release_bulk(&lines);
                gpiod_chip_close(chip);
                return NULL;
            }

            ret = gpiod_line_event_get_fd(line);
            if (ret < 0)
            {
                perror("Read last event notification failed\n");
                gpiod_line_release_bulk(&lines);
                gpiod_chip_close(chip);
                return NULL;
            }
            ret-=3;
            printf("%d\n",ret);

        }

        i += event_size;
    }

    gpiod_line_release_bulk(&lines);
    gpiod_chip_close(chip);
    return true;
}