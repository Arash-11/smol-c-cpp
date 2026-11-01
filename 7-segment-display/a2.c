#include <stdio.h>

#define MAX_DIGITS      10
#define DIGIT_NUM_ROWS  3                     // a digit is 3 rows high
#define DIGIT_NUM_COLS  (DIGIT_NUM_ROWS + 1)  // a digit is 3 columns wide + 1 col for gap
#define MAX_DIGIT_COLS  (MAX_DIGITS * DIGIT_NUM_COLS)

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
 * coordinates for each of the 7 segments in a 3x3 grid (index 0 = row, index 1 = col):
 *
 *   0 1 2
 * 0   _
 * 1 | _ |
 * 2 | _ |
 */
const int segment_grid[7][2] = {
    {0, 1}, // segment 0
    {1, 2}, // segment 1
    {2, 2}, // segment 2
    {2, 1}, // segment 3
    {2, 0}, // segment 4
    {1, 0}, // segment 5
    {1, 1}  // segment 6
};

char digits[DIGIT_NUM_ROWS][MAX_DIGIT_COLS];

void process_digit(int digit, int pos) {
    for (int i = 0; i < 7; i++) {
        if (segments[digit][i]) {
            int row = segment_grid[i][0];
            int col = segment_grid[i][1] + (pos * DIGIT_NUM_COLS);
            digits[row][col] = i % 3 == 0 ? '_' : '|';
        }
    }
}

void print_digits_array(void) {
    for (int row = 0; row < DIGIT_NUM_ROWS; row++) {
        for (int col = 0; col < MAX_DIGIT_COLS; col++) {
            printf("%c", digits[row][col]);
        }
        printf("\n");
    }
}

void clear_digits_array(void) {
    for (int row = 0; row < DIGIT_NUM_ROWS; row++) {
        for (int col = 0; col < MAX_DIGIT_COLS; col++) {
            digits[row][col] = ' ';
        }
    }
}

int main(void) {
    int len = 0, nums[MAX_DIGITS] = { 0 };

    clear_digits_array();

    printf("Enter a number: ");
    for (int c; (c = getchar()) != '\n' && c != EOF && len < MAX_DIGITS; len++) {
        int digit = c - '0';
        nums[len] = digit;
        process_digit(digit, len);
    }

    print_digits_array();
}
