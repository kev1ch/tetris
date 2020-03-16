#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef int INTEGER;

#define LOOP_COUNT 10
#define FIELD_HEIGHT 20
#define FIELD_WIDTH 20
#define SHAPE_ELEMENT_COUNT 4
#define IR_BUFFER_SIZE 128

COORD shape[SHAPE_ELEMENT_COUNT];

char field[FIELD_WIDTH][FIELD_HEIGHT];

void GoToXY(int column, int line);
void drawBorders();
void newShape();
void squareShape();
void lineShape();
void sLShape();
void sRShape();
void lLShape();
void lRShape();
int moveShapeDown();
void moveShape(int dir);
void rotateShape();
void drawField();
void addShapeToField();
void readUserInput();
void init();

int main() {

    init();

    while (1) {
        drawField();
        readUserInput();
        if (!moveShapeDown()) {
            addShapeToField();
            newShape();
        } // end if
        Sleep(500);
    } // end while (main game loop)

    GoToXY(0, 30);

	return 0;
} // end main

void GoToXY(int column, int line) {
    // Create a COORD structure and fill in its members.
    // This specifies the new position of the cursor that we will set.
    COORD coord;
    coord.X = column;
    coord.Y = line;

    // Obtain a handle to the console screen buffer.
    // (You're just using the standard console, so you can use STD_OUTPUT_HANDLE
    // in conjunction with the GetStdHandle() to retrieve the handle.)
    // Note that because it is a standard handle, we don't need to close it.
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(hConsole, coord)) {
        // Uh-oh! The function call failed, so you need to handle the error.
        // You can call GetLastError() to get a more specific error code.
        // ...
    } // end if
} // end GoToXY

void drawBorders() {
    for (int i = 0; i < FIELD_HEIGHT + 1; i++) {
        GoToXY(0, i);
        printf("#");
        GoToXY(FIELD_WIDTH + 1, i);
        printf("#");
    } // end for
    GoToXY(1, FIELD_HEIGHT);
    for (int i = 0; i < FIELD_WIDTH + 1; i++) {
        printf("#");
    } // end for
} // end drawField

void newShape() {
    int rand_num = rand() % 6;
    switch (rand_num) {
        case 0:
            squareShape();
            break;
        case 1:
            lineShape();
            break;
        case 2:
            sLShape();
            break;
        case 3:
            sRShape();
            break;
        case 4:
            lLShape();
            break;
        case 5:
            lRShape();
            break;
    } // end switch
} // end newShape

void squareShape() {
    shape[0].X = 0;
    shape[0].Y = 0;
    shape[1].X = 1;
    shape[1].Y = 0;
    shape[2].X = 0;
    shape[2].Y = 1;
    shape[3].X = 1;
    shape[3].Y = 1;
} // end squareShape()

void lineShape() {
    shape[0].X = 0;
    shape[0].Y = 0;
    shape[1].X = 1;
    shape[1].Y = 0;
    shape[2].X = 2;
    shape[2].Y = 0;
    shape[3].X = 3;
    shape[3].Y = 0;
} // end lineShape()

void lLShape() {
    shape[0].X = 0;
    shape[0].Y = 0;
    shape[1].X = 0;
    shape[1].Y = 1;
    shape[2].X = 1;
    shape[2].Y = 1;
    shape[3].X = 2;
    shape[3].Y = 1;
} // end lLShape

void lRShape() {
    shape[0].X = 0;
    shape[0].Y = 1;
    shape[1].X = 1;
    shape[1].Y = 1;
    shape[2].X = 2;
    shape[2].Y = 1;
    shape[3].X = 2;
    shape[3].Y = 0;
} // end lRShape

void sLShape() {
    shape[0].X = 0;
    shape[0].Y = 1;
    shape[1].X = 1;
    shape[1].Y = 1;
    shape[2].X = 1;
    shape[2].Y = 0;
    shape[3].X = 2;
    shape[3].Y = 0;
} // end sLShape

void sRShape() {
    shape[0].X = 0;
    shape[0].Y = 0;
    shape[1].X = 1;
    shape[1].Y = 0;
    shape[2].X = 1;
    shape[2].Y = 1;
    shape[3].X = 2;
    shape[3].Y = 1;
} // end sRShape

void drawField() {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            GoToXY(j + 1, i);
            if (field[j][i]) {
                printf("&");
            } else {
                printf(" ");
            } // end if
        } // end for
    } // end for
    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        int x = shape[i].X + 1;
        int y = shape[i].Y;
        GoToXY(x, y);
        printf("+");
    } // end for
} // end drawField

int moveShapeDown() {
    COORD temp_shape[SHAPE_ELEMENT_COUNT];
    int result = 1;
    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        temp_shape[i].X = shape[i].X;
        temp_shape[i].Y = shape[i].Y + 1;
        if (temp_shape[i].Y >= FIELD_HEIGHT || field[temp_shape[i].X][temp_shape[i].Y]) {
            result = 0;
            break;
        } // end if
    } // end for
    if (result) {
        for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
            shape[i].X = temp_shape[i].X;
            shape[i].Y = temp_shape[i].Y;
            GoToXY(0, 25);
            printf("(%d, %d)", shape[i].X, shape[i].Y);
        } // end for
    } // end if
    return result;
} // end moveShape

void addShapeToField() {
    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        int x = shape[i].X;
        int y = shape[i].Y;
        field[x][y] = 1;
    } // end for
} // end addShapeToField

void init() {

    time_t t;
    srand((unsigned)time(&t));

    drawBorders();
    newShape();

    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD fdw_mode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    SetConsoleMode(hstdin, fdw_mode);

} // end init

void readUserInput() {

    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD ir_in_buf[IR_BUFFER_SIZE];
    int cnumread;
    int event_num;
    GetNumberOfConsoleInputEvents(hstdin, &event_num);
    if (event_num) {
        ReadConsoleInput(hstdin, ir_in_buf, IR_BUFFER_SIZE, &cnumread);
        GoToXY(0, 26);
        printf("%d", cnumread);
        for (int i = 0; i < cnumread; i++) {
            INPUT_RECORD event = ir_in_buf[i];
            if (event.EventType == KEY_EVENT) {
                KEY_EVENT_RECORD key_event = event.Event.KeyEvent;
                if (!key_event.bKeyDown) {
                    continue;
                } // end if
                WORD virtual_key_code = key_event.wVirtualKeyCode;
                GoToXY(0, 27);
                if (virtual_key_code == VK_LEFT) {
                    moveShape(-1);
                    printf("left");
                } else if (virtual_key_code == VK_RIGHT) {
                    moveShape(1);
                    printf("right");
                } else if (virtual_key_code == VK_SPACE) {
                    printf("spacebar");
                    rotateShape();
                } // end if
            } // end if
        } // end for
    } // end if

} // end readUserInput

void moveShape(int dir) {

    COORD temp_shape[SHAPE_ELEMENT_COUNT];

    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        temp_shape[i].X = shape[i].X + dir;
        temp_shape[i].Y = shape[i].Y;
        if (temp_shape[i].X < 0 || temp_shape[i].X >= FIELD_WIDTH) {
            goto out_label;
        } // end if
    } // end for

    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        shape[i].X = temp_shape[i].X;
        shape[i].Y = temp_shape[i].Y;
    } // end for
    
out_label:
    return;

} // end moveShape

void rotateShape() {
    
    COORD temp_shape[SHAPE_ELEMENT_COUNT];

    int min_x = FIELD_WIDTH + 1, max_x = -1, min_y = FIELD_HEIGHT + 1, max_y = -1;

    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        min_x = min_x > shape[i].X ? shape[i].X : min_x;
        min_y = min_y > shape[i].Y ? shape[i].Y : min_y;
        max_x = max_x < shape[i].X ? shape[i].X : max_x;
        max_y = max_y < shape[i].Y ? shape[i].Y : max_y;
    } // end for

    int centre_x = (min_x + max_x) / 2;
    int centre_y = (min_y + max_y) / 2;

    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        int delta_x = shape[i].X - centre_x;
        int delta_y = shape[i].Y - centre_y;
        temp_shape[i].X = centre_x - delta_y;
        temp_shape[i].Y = centre_y + delta_x;
        if (field[temp_shape[i].X][temp_shape[i].Y] || temp_shape[i].X < 0 || temp_shape[i].X >= FIELD_WIDTH ||
            temp_shape[i].Y < 0 || temp_shape[i].Y > FIELD_HEIGHT) {
            goto out_label;
        } // end if
    } // end for

    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        shape[i].X = temp_shape[i].X;
        shape[i].Y = temp_shape[i].Y;
    } // end for

out_label:
    return;

} // end rotateShape