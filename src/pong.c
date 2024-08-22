#include <stdio.h>
#define RECT_WIDTH 80
#define RECT_HEIGHT 25
#define BALL_SIZE 1
#define BALL_SIGN "o"
#define FIELD_SIGN " "
#define RACKET_SIGN "H"
#define RACKET_LEFT 2
#define RACKET_RIGHT 78

int racket_valid_positions(unsigned current_cell_x, unsigned current_cell_y, unsigned racket_position_x,
                           unsigned racket_position_y) {
    int status = 0;
    if ((current_cell_x == racket_position_x && current_cell_y == racket_position_y) ||
        (current_cell_y == racket_position_y + 1 && current_cell_x == racket_position_x) ||
        (current_cell_y == racket_position_y - 1 && current_cell_x == racket_position_x)) {
        status = 1;
    };
    return status;
}

void clear_buffer(void) { while (getchar() != '\n'); }

void clear_screen(void) { printf("\033[0d\033[2J"); }

void print_field(int boal_pos_x, int boal_pos_y, int left_racket_y, int right_racket_y) {
    for (int y_axis = 0; y_axis <= RECT_HEIGHT; y_axis++) {
        for (int x_axis = 0; x_axis <= RECT_WIDTH; x_axis++) {
            if (boal_pos_x == x_axis && boal_pos_y == y_axis) {
                printf(BALL_SIGN);
            } else if (racket_valid_positions(x_axis, y_axis, RACKET_LEFT, left_racket_y) ||
                       racket_valid_positions(x_axis, y_axis, RACKET_RIGHT, right_racket_y)) {
                printf(RACKET_SIGN);
            } else if (x_axis == RECT_WIDTH / 2) {
                printf("|");
            } else if (x_axis == 0 || x_axis == RECT_WIDTH) {
                printf("|");
            } else if (y_axis == 0 || y_axis == RECT_HEIGHT) {
                printf("—");
            } else {
                printf(FIELD_SIGN);
            };
        };
        printf("\n");
    };
}

int handle_ball_collision(int ballY, int vectorY) {
    return (ballY == 1 || ballY == RECT_HEIGHT - 1) ? -vectorY : vectorY;
}

int handle_x_vector(int ballX, int ballY, int left_racket_y, int right_racket_y, int vectorX) {
    int output_vector = vectorX;
    if (ballX == RACKET_LEFT + 1 &&
        (ballY == left_racket_y || ballY == left_racket_y - 1 || ballY == left_racket_y + 1)) {
        output_vector = -vectorX;
    }
    if (ballX == RACKET_RIGHT - 1 &&
        (ballY == right_racket_y || ballY == right_racket_y - 1 || ballY == right_racket_y + 1)) {
        output_vector = -vectorX;
    }
    return output_vector;
}

int check_user_points(int user, int user_id) {
    int win = 0;
    if (user == 21) {
        if (user_id == 0) {
            printf("\n\n\t\t*********************************\n");
            printf("\t\t*                               *\n");
            printf("\t\t*        LEFT PLAYER WINS!       *\n");
            printf("\t\t*                               *\n");
            printf("\t\t*********************************\n");
            win = 1;
        } else {
            printf("\n\n\t\t*********************************\n");
            printf("\t\t*                               *\n");
            printf("\t\t*       RIGHT PLAYER WINS!        *\n");
            printf("\t\t*                               *\n");
            printf("\t\t*********************************\n");
            win = 1;
        }
    }
    return win;
}

int move_racket(char command, int racket_axis, char racket_side) {
    if (racket_side == 'L') {  // Для левой ракетки
        if (command == 'a' && racket_axis > 2) return racket_axis - 1;
        if (command == 'z' && racket_axis < RECT_HEIGHT - 2) return racket_axis + 1;
    } else if (racket_side == 'R') {  // Для правой ракетки
        if (command == 'k' && racket_axis > 2) return racket_axis - 1;
        if (command == 'm' && racket_axis < RECT_HEIGHT - 2) return racket_axis + 1;
    }
    return racket_axis;
}

// JOPA - Junior Officer PROTECTION Association
void print_alert(void) { printf("Alert: JOPA*, trying again!\n"); }

int main(void) {
    int vectorX = 1, vectorY = 1, play = 1;
    int ballX = RECT_WIDTH / 2, ballY = RECT_HEIGHT / 2, score_left = 20, score_right = 20;
    int left_racket_y = 20, right_racket_y = 10;
    char command, buffer;
    do {
        clear_screen();
        print_field(ballX, ballY, left_racket_y, right_racket_y);
        ballX = ballX + vectorX;
        ballY = ballY + vectorY;
        printf("\t\t%d \t\t\t\t\t\t%d\n", score_left, score_right);
        scanf("%c%c", &command, &buffer);
        if (buffer == '\n') {
            left_racket_y = move_racket(command, left_racket_y, 'L');
            right_racket_y = move_racket(command, right_racket_y, 'R');
        } else {
            printf("Alert: JOPA*, trying again!\n");
            clear_buffer();
        }
        vectorY = handle_ball_collision(ballY, vectorY);
        vectorX = handle_x_vector(ballX, ballY, left_racket_y, right_racket_y, vectorX);
        if (ballX == 0 &&
            (ballY != left_racket_y || ballY != left_racket_y - 1 || ballY != left_racket_y + 1)) {
            score_right++;
            ballX = RECT_WIDTH / 2;
            ballY = RECT_HEIGHT / 2;
            vectorX = -vectorX;
            vectorY = -vectorY;
        }
        if (ballX == RECT_WIDTH &&
            (ballY != right_racket_y || ballY != right_racket_y - 1 || ballY != right_racket_y + 1)) {
            score_left++;
            ballX = RECT_WIDTH / 2;
            ballY = RECT_HEIGHT / 2;
            vectorX = -vectorX;
            vectorY = -vectorY;
        }
        if (check_user_points(score_left, 0) || check_user_points(score_right, 1)) {
            break;  // End game
        }
    } while (play);
    return 0;
}
