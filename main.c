#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef int INTEGER;

#define LOOP_COUNT 10
#define FIELD_HEIGHT 20
#define FIELD_WIDTH 20
#define FIELD_SIZE (FIELD_WIDTH * 2 * FIELD_HEIGHT)
#define SHAPE_ELEMENT_COUNT 4
#define IR_BUFFER_SIZE 128

COORD shape[SHAPE_ELEMENT_COUNT];

char field[FIELD_WIDTH][FIELD_HEIGHT];

int score;

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
void dropShape();
int isCollision(int x, int y);
void drawField();
void addShapeToField();
int readUserInput();
void clearLine();
void collapseLine(int line);
int isFull(int line);
void init();

int main() {

    init();

    while (1) {
        drawField();
        for (int i = 0; i < 10; i++) {
            if (readUserInput()) {
                drawField();
            } // end if
            Sleep(50);
        } // end while
        if (!moveShapeDown()) {
            addShapeToField();
            clearLine();
            newShape();
        } // end if
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
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x11);
    for (int i = 0; i < FIELD_HEIGHT + 1; i++) {
        GoToXY(0, i);
        printf("%c", 186);
        GoToXY(FIELD_WIDTH*2 + 1, i);
        printf("%c", 186);
    } // end for
    GoToXY(1, FIELD_HEIGHT);
    for (int i = 0; i < FIELD_WIDTH*2 + 1; i++) {
        printf("%c", 205);
    } // end for
    GoToXY(0, FIELD_HEIGHT);
    printf("%c", 200);
    GoToXY(FIELD_WIDTH*2 + 1, FIELD_HEIGHT);
    printf("%c", 188);
    SetConsoleTextAttribute(hConsole, 0x0f);
    GoToXY(FIELD_WIDTH * 2 + 5, 0);
    printf("Copyright Daniil Pavlov, 2020");
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
    CHAR_INFO ch_buffer[FIELD_SIZE];

    ch_buffer[0].Char.UnicodeChar = '^';
    ch_buffer[0].Attributes = 0x0F;

    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            int position = i * FIELD_WIDTH * 2 + j * 2;
            if (field[j][i]) {
                ch_buffer[position].Attributes = 0x99;
                ch_buffer[position + 1].Attributes = 0x99;
                // ch_buffer[position].Char.UnicodeChar = '{';
            } else {
                char ch_var = j % 2 ? 0x70 : 0x80;
                ch_buffer[position].Attributes = j % 2 ? 0x70 : 0x80;
                ch_buffer[position + 1].Attributes = j % 2 ? 0x70 : 0x80;
                ch_buffer[position].Char.UnicodeChar = ' ';
                ch_buffer[position + 1].Char.UnicodeChar = ' ';
            } // end if
        } // end for
    } // end for

    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        int x = shape[i].X;
        int y = shape[i].Y;
        int position = y * FIELD_WIDTH * 2 + x * 2;
        ch_buffer[position].Attributes = 0x22;
        ch_buffer[position + 1].Attributes = 0x22;
        ch_buffer[position].Char.UnicodeChar = ' ';
        ch_buffer[position + 1].Char.UnicodeChar = ' ';
    } // end for

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD field_size = {.X=FIELD_WIDTH * 2, .Y=FIELD_HEIGHT};
    COORD corner = { .X = 0, .Y = 0 };
    SMALL_RECT rectangle = {
        .Top = 0,
        // .Left = FIELD_WIDTH * 2 + 10,
        .Left = 1,
        .Bottom = FIELD_HEIGHT - 1,
        // .Right = FIELD_WIDTH * 4 + 9
        .Right = FIELD_WIDTH * 2 + 1
    };
    WriteConsoleOutput(hConsole, 
        ch_buffer, 
        field_size,
        corner, 
        &rectangle);
    GoToXY(1, FIELD_WIDTH + 1);
    printf("SCORE: %d", score);
} // end drawField

int moveShapeDown() {
    COORD temp_shape[SHAPE_ELEMENT_COUNT];
    int result = 1;
    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        temp_shape[i].X = shape[i].X;
        temp_shape[i].Y = shape[i].Y + 1;
        if (isCollision(temp_shape[i].X, temp_shape[i].Y)) {
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

void dropShape() {
    while (moveShapeDown());
} // end dropShape

int isCollision(int x, int y) {
    int result = field[x][y] || y >= FIELD_HEIGHT ||
                 x < 0 || x >= FIELD_WIDTH ||
                 y < 0;
    return result;
} // end isCollision

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

int readUserInput() {

    int result = 0;

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
                if (virtual_key_code == VK_LEFT || virtual_key_code == VK_RIGHT ||
                    virtual_key_code == VK_SPACE || virtual_key_code == VK_DOWN) {
                    result = 1;
                } // end if
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
                } else if (virtual_key_code == VK_DOWN) {
                    printf("down");
                    dropShape();
                } // end if
            } // end if
        } // end for
    } // end if

    return result;

} // end readUserInput

void moveShape(int dir) {

    COORD temp_shape[SHAPE_ELEMENT_COUNT];

    for (int i = 0; i < SHAPE_ELEMENT_COUNT; i++) {
        temp_shape[i].X = shape[i].X + dir;
        temp_shape[i].Y = shape[i].Y;
        if (isCollision(temp_shape[i].X, temp_shape[i].Y)) {
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
        if (isCollision(temp_shape[i].X, temp_shape[i].Y)) {
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

void clearLine() {

    for (int i = 0; i < FIELD_HEIGHT; i++) {
        if (isFull(i)) {
            collapseLine(i);
            score += 100;
        } // end if
    } // end for

} // end clearLine

int isFull(int line) {
    int result = 1;

    for (int i = 0; i < FIELD_WIDTH; i++) {
        if (!field[i][line]) {
            result = 0;
            break;
        } // end if
    } // end for

    return result;
} // end isFull

void collapseLine(int line) {

    for (int i = line; i > 0; i--) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            field[j][i] = field[j][i - 1];
        } // end for
    } // end for

} // end collapseLine