/** 
 * When passed two integer values, draws a spiral from the first to the
 * second number. Passed numbers must be positive, the second number must
 * be larger that the first and the absolute difference between the numbers
 * should result in an integer if placed in this formula:
 *      (2x - 1)^2 = number_difference
 * 
 * example - 1 25 as parameters
 *  21      22      23      24      25
 *  20      7       8       9       10
 *  19      6       1       2       11
 *  18      5       4       3       12
 *  17      16      15      14      13
 *
 *  This program uses the patterns which appear in the resulting spirals
 *  to produce them instead of looping thorough each value in order.
 *
 *  Calculation order
 *  1. the corners marked "C" C = (2x - 1)^2
 *  2. the values immediately to the right of C marked "D" D = (C + 1)
 *  3. the values to the left of C marked "B"
 *  4. the right tringle marked "E" E = (D + n) where n = height diff of D and R
 *  5. the left triangle marked "A" A = (B - n) --------       //       --------
 *  6. the bottom triangle marked "Y" Y = E + n
 *  7. the position directly under the center marked "X" (X = C + 3)
 *  
 *  B     B      B      B      C
 *  B     B      B      C      D
 *  A     B      C      D      E
 *  A     A      X      E      E
 *  A     Y      Y      Y      E
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
    if (argc < 3) {
        printf("You must pass two arguments to this program: "
                "the start and end numbers of the spiral.\n");
        exit(1);
    }

    unsigned start = atoi(argv[1]);
    unsigned end = atoi(argv[2]);

    if (end < start) {
        printf("End number mustn't be smaller than the start "
                "number!\n");
        exit(1);
    }

    unsigned number_count = end - start + 1;

    // start == end is an edge case where only one number needs to be printed
    if (number_count == 1) {
        printf("%d\n", start);
        exit(0);
    }

    // validate whether the passed numbers can make a square spiral
    {
        double square_root = sqrt(number_count);

        // root must be an integer: we cant have 3.14 numbers
        if (square_root != floor(square_root)) {
            exit(3);
        }

        // root must be uneven (2x - 1 always produces uneven numbers)
        if ((int)square_root % 2 <= 0) {
            exit(4);
        }
    }

    // if the spiral doesn't start at 1, we'll add an offset to each field
    unsigned offset = start - 1;

    // this is the col and row count
    unsigned size = sqrt(number_count);

    int matrix[size][size];
    int mid = floor((double) size / 2);

    for (int corner_col = size - 1; corner_col >= 0; corner_col--) {
        // calculate each corner via (2x -1)^2
        unsigned corner_value = pow(2 * (corner_col - mid + 1) - 1, 2) + offset; 

        // the corner row is the inverse of its column
        int corner_row = size - corner_col - 1;

        // corners => C
        matrix[corner_row][corner_col] = corner_value;

        // first value to the right of corners => D
        if (corner_col >= mid)  {
            if (corner_col < size - 1) {
                matrix[corner_row][corner_col + 1] = corner_value + 1;
            }
        }

        // to the left of corners => B
        for (int j = corner_col - 1; j > corner_row - 1; j--) {
            matrix[corner_row][j] = matrix[corner_row][j + 1] - 1;
        }

        if (corner_row > 1) {
            // values to the right of corners => E
            for (int j = corner_row; j < size - corner_row + 2; j++) {
                int col = corner_col + 2;

                matrix[j][col] = matrix[j - 1][col] + 1;
            }

            // lower left triangle => A
            {
                int line_start = corner_row - 1;
                int line_end = size - corner_row + 1;
                int col = corner_row - 2;

                for (int row = line_start; row <= line_end; row++) {
                    matrix[row][col] = matrix[row - 1][col] - 1;
                }
            }

            // bottom triangle => Y
            if (corner_row > mid) {
                int row = corner_row + 1;

                for (int col = corner_col; col < size - corner_col; col++) {
                    matrix[row][col] = matrix[row][col - 1] - 1;
                }
            }
        }
    }

    int input = 500;
    int result = 0;
    {
        int temp = sqrt(input) / 2;
        result = temp > 12 ? 12 : temp;
    }
    
    // the value right below 1 => X
    // the left/right items of this point are only available after the 
    // second iteration of the loop. having all of them set in the same place
    // would mean having another nested loop. no es bueno, this is good enough
    matrix[mid + 1][mid] = start + 3;

    // print the matrix
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d\t", matrix[i][j]);
         }

        printf("\n");
    }
}
