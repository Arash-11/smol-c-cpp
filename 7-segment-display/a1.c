#include <ctype.h>
#include <stdio.h>

#define MAX_DIGITS 10
#define SEGMENT_ROWS 3

/*
 * 7-segment representations for digits 0-9:
 *
 *     _0_
 *  5 |_6_| 1
 *  4 |_ _| 2
 *      3
 */
const int segments[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
};

/*
 * We'll use 3 rows for display. Segments in use for each row:
 * 1st =   0
 * 2nd = 5 6 1
 * 3rd = 4 3 2
*/
const int segment_rows[3][3] = {
    {0, 0, 0},
    {5, 6, 1},
    {4, 3, 2}
};

int main(void) {
    int c,
        len = 0,
        input = 0,
        nums[MAX_DIGITS] = { 0 },
        num_rows[MAX_DIGITS][3][3] = { 0 };

    printf("Enter a number: ");
    while ((c = getchar()) != '\n' && len < MAX_DIGITS) {
        if (isdigit(c))
            nums[len++] = c - '0';
    }

    // create segment_rows representation of nums
    for (int i = 0; i < len; i++) {
        int n = nums[i];
        num_rows[i][0][1] = segments[n][0];
        for (int c = 0; c < 3; c++)
            num_rows[i][1][c] = segments[n][segment_rows[1][c]];
        for (int c = 0; c < 3; c++)
            num_rows[i][2][c] = segments[n][segment_rows[2][c]];
    }

    // display nums
    for (int r = 0; r < 3; r++) {
        for (int i = 0; i < len; i++) {
            char c0 = num_rows[i][r][0] ? '|' : ' ';
            char c1 = num_rows[i][r][1] ? '_' : ' ';
            char c2 = num_rows[i][r][2] ? '|' : ' ';
            printf("%c%c%c ", c0, c1, c2);
        }
        printf("\n");
    }

    return 0;
}
