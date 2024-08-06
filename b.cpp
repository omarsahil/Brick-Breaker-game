#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <dos.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 10
#define BALL_SIZE 10
#define BRICK_ROWS 5
#define BRICK_COLS 10
#define BRICK_WIDTH 60
#define BRICK_HEIGHT 20
#define DELAY 10
#define PADDLE_SPEED 20 // Adjust this value to increase paddle speed

int paddleX, paddleY;
int ballX, ballY;
int ballSpeedX = 5, ballSpeedY = 5;
int bricks[BRICK_ROWS][BRICK_COLS];
int score = 0;

void initGame() {
    // Initialize paddle position
    paddleX = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
    paddleY = SCREEN_HEIGHT - PADDLE_HEIGHT - 20;

    // Initialize ball position
    ballX = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
    ballY = SCREEN_HEIGHT - PADDLE_HEIGHT - 30;

    // Initialize bricks
    int startX = 50, startY = 50;
    for (int i = 0; i < BRICK_ROWS; i++) {
        for (int j = 0; j < BRICK_COLS; j++) {
            bricks[i][j] = 1; // 1 means brick is active (visible)
        }
        startX = 50; // Reset startX for next row
        startY += BRICK_HEIGHT + 10;
    }
}

void drawPaddle() {
    setcolor(RED); // Set the outline color of the paddle
    setfillstyle(SOLID_FILL, RED); // Set solid fill with red color
    bar(paddleX, paddleY, paddleX + PADDLE_WIDTH, paddleY + PADDLE_HEIGHT); // Draw filled rectangle (paddle)
}

void drawBall() {
    setcolor(WHITE); // Set the outline color of the ball
    setfillstyle(SOLID_FILL, WHITE); // Set solid fill with white color
    fillellipse(ballX, ballY, BALL_SIZE, BALL_SIZE); // Draw a filled circle (ball)
}

void drawBricks() {
    int startX = 50, startY = 50;
    for (int i = 0; i < BRICK_ROWS; i++) {
        for (int j = 0; j < BRICK_COLS; j++) {
            if (bricks[i][j] == 1) {
                setfillstyle(SOLID_FILL, GREEN); // Set fill color to green
                bar(startX, startY, startX + BRICK_WIDTH, startY + BRICK_HEIGHT); // Draw green brick
            }
            startX += BRICK_WIDTH + 10;
        }
        startX = 50; // Reset startX for next row
        startY += BRICK_HEIGHT + 10;
    }
}

void updateBall() {
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Ball collision with walls
    if (ballX <= 0 || ballX >= SCREEN_WIDTH - BALL_SIZE) {
        ballSpeedX = -ballSpeedX;
    }
    if (ballY <= 0) {
        ballSpeedY = -ballSpeedY;
    }

    // Ball collision with paddle
    if (ballY + BALL_SIZE >= paddleY && ballX >= paddleX && ballX <= paddleX + PADDLE_WIDTH) {
        ballSpeedY = -ballSpeedY;
    }

    // Ball collision with bricks
    int brickHit = 0;
    int startX = 50, startY = 50;
    for (int i = 0; i < BRICK_ROWS; i++) {
        for (int j = 0; j < BRICK_COLS; j++) {
            if (bricks[i][j] == 1) {
                if (ballX + BALL_SIZE >= startX && ballX <= startX + BRICK_WIDTH &&
                    ballY + BALL_SIZE >= startY && ballY <= startY + BRICK_HEIGHT) {
                    bricks[i][j] = 0; // Brick is broken
                    brickHit = 1;
                    score++;
                    // Determine ball's next movement after hitting the brick
                    if (ballX + BALL_SIZE > startX && ballX < startX + BRICK_WIDTH) {
                        ballSpeedY = -ballSpeedY;
                    }
                    if (ballY + BALL_SIZE > startY && ballY < startY + BRICK_HEIGHT) {
                        ballSpeedX = -ballSpeedX;
                    }
                }
            }
            startX += BRICK_WIDTH + 10;
        }
        startX = 50; // Reset startX for next row
        startY += BRICK_HEIGHT + 10;
    }

    // Ball falls below the paddle (Game over condition)
    if (ballY >= SCREEN_HEIGHT) {
        printf("Game Over! Your Score: %d\n", score);
        delay(2000); // Delay to see the score before closing
        exit(0);
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    initGame();

    // Double buffering implementation
    int page = 0;
    int gameover = 0;
    while (!gameover) {
        // Draw to off-screen buffer
        setactivepage(page);
        cleardevice();

        drawPaddle();
        drawBall();
        drawBricks();
        setcolor(WHITE);
        outtextxy(10, 10, "Score:");
        char scoreStr[10];
        sprintf(scoreStr, "%d", score);
        outtextxy(60, 10, scoreStr);

        // Swap buffers
        setvisualpage(page);
        page = 1 - page; // Toggle between 0 and 1

        // Update ball position and handle collisions
        updateBall();

        // Handle paddle movement
        if (kbhit()) {
            char ch = getch();
            switch (ch) {
                case 'a':
                    paddleX -= PADDLE_SPEED; // Move left faster
                    break;
                case 'd':
                    paddleX += PADDLE_SPEED; // Move right faster
                    break;
                case 27: // ASCII value for Escape key
                    gameover = 1;
                    break;
            }
        }

        // Boundary check for paddle
        if (paddleX < 0) {
            paddleX = 0;
        }
        if (paddleX > SCREEN_WIDTH - PADDLE_WIDTH) {
            paddleX = SCREEN_WIDTH - PADDLE_WIDTH;
        }

        delay(DELAY);
    }

    closegraph();
    return 0;
}