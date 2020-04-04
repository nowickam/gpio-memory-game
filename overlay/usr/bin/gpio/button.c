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
    struct gpiod_line *line;
    struct gpiod_line_event event;
    unsigned int event_size;
    unsigned int line_nums[] = {12, 13, 14, 15, 16, 17, 18, 19};
    unsigned int line_len = 8;

    struct timespec ts = {5, 0};
    struct timespec prev = {0, 0};

    long ns_diff;
    int s_diff;
    int bnc_count = 0;

    int *user = (int *)calloc(difficulty, sizeof(int));
    if (user == NULL)
    {
        perror("Failed to allocate memory!");
        return NULL;
    }

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

    i = 0;

    while (i < difficulty)
    {
        ret = gpiod_line_request_bulk_falling_edge_events(&lines, "button_click");
        if (ret < 0)
        {
            perror("Request falling edge event failed!\n");
            gpiod_line_release_bulk(&lines);
            gpiod_chip_close(chip);
            free(user);
            free(led_seq);
            return NULL;
        }
        ret = gpiod_line_event_wait_bulk(&lines, &ts, &event_lines);
        if (ret < 0)
        {
            perror("Wait failed!");
            gpiod_line_release_bulk(&lines);
            gpiod_chip_close(chip);
            free(user);
            free(led_seq);
            return NULL;
        }
        else if (ret == 0)
        {
            printf("TIME IS UP!");
            gpiod_line_release_bulk(&lines);
            gpiod_chip_close(chip);
            free(user);
            free(led_seq);
            return false;
        }

        line = gpiod_line_bulk_get_line(&event_lines, 0);
        if (ret < 0)
        {
            perror("Reading event number failed!");
            gpiod_line_release_bulk(&lines);
            gpiod_chip_close(chip);
            free(user);
            free(led_seq);
            return NULL;
        }

        ret = gpiod_line_event_read(line, &event);
        if (ret < 0)
        {
            perror("Read last event notification failed\n");
            gpiod_line_release_bulk(&lines);
            gpiod_chip_close(chip);
            free(user);
            free(led_seq);
            return NULL;
        }

        ret = gpiod_line_event_get_fd(line);
        if (ret < 0)
        {
            perror("Read last event notification failed\n");
            gpiod_line_release_bulk(&lines);
            gpiod_chip_close(chip);
            free(user);
            free(led_seq);
            return NULL;
        }

        // bouncing
        s_diff=event.ts.tv_sec - prev.tv_sec;
        printf("s: %d\n",s_diff);
        if (event.ts.tv_sec - prev.tv_sec <= 1 && i != 0)
        {

            if (event.ts.tv_nsec - prev.tv_nsec < 3e+8)
            {
                bnc_count++;
            }
            else
                bnc_count = 0;
        }
        else
            bnc_count = 0;

        if (bnc_count < 1)
        {
            ret -= 3;
            user[i] = ret;
        }
        prev.tv_sec = event.ts.tv_sec;
        prev.tv_nsec = event.ts.tv_nsec;

        if (user[i] != led_seq[i] && bnc_count < 1)
        {
            gpiod_line_release_bulk(&lines);
            gpiod_chip_close(chip);
            free(user);
            free(led_seq);
            return false;
        }

        if (bnc_count < 1)
            i++;
        gpiod_line_release_bulk(&lines);
    }

    gpiod_line_release_bulk(&lines);
    gpiod_chip_close(chip);
    free(user);
    free(led_seq);
    return true;
}