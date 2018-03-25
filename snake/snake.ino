#include <Kniwwelino.h>

#define SNAIK 4
#define FRUIT 100

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

struct Point {
  int x;
  int y;
};

bool screen[5][5];
unsigned long ftimer = 0;
unsigned long utimer = 0;
Point snake[SNAIK];
Point fruit;
bool fstate = 0;
int dir = RIGHT;
bool turntaken = false;
int score = 0;

void draw() {
  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 5; x++) { 
      Kniwwelino.drawPixel(x, y, screen[x][y]);
    }
  }
}

void setPixel(int x, int y, bool val) {
  screen[x][y] = val;
}

void clear() {
  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 5; x++) { 
      setPixel(x, y, 0);
    }
  }
}

void shiftSnake() {
  for (int i = SNAIK - 1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }
}

bool getPixel(int x, int y) {
  return screen[x][y];
}

void drawSnake() {
  for (int i = 0; i < SNAIK; i++) {
    setPixel(snake[i].x, snake[i].y, 1);
  }
}

void drawFruit() {
  setPixel(fruit.x, fruit.y, fstate);
  if (millis() - ftimer >= FRUIT) {
    fstate = !fstate;
    ftimer = millis();
  }
}

void checkButtons() {
  if (Kniwwelino.BUTTONAclicked() && !turntaken) {
    dir--;
    if (dir == -1) dir = 3;
    turntaken = 1;
  }
  if (Kniwwelino.BUTTONBclicked() && !turntaken) {
    dir++;
    if (dir == 4) dir = 0;
    turntaken = 1;
  }
}

void checkFruit () {
  if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
    fruit.x = random(0, 5);
    fruit.y = random(0, 5);
    Kniwwelino.RGBsetColor(0, 255, 0);
    delay(10);
    Kniwwelino.RGBsetColor(0, 0, 0);
    score++;
  }
}

bool allFilled() {
  bool res = 1;
  for (int y = 0; y < 5 && res; y++) {
    for (int x = 0; x < 5 && res; x++) { 
      if (!getPixel(x, y)) res = 0;
    }
  }
  return res;
}

void gameOver() {
  Kniwwelino.RGBsetColor(255, 0, 0);
  delay(10);
  Kniwwelino.RGBsetColor(0, 0, 0);
  delay(190);
  while (!allFilled()) {
    int x = random(0, 5);
    int y = random(0, 5);
    setPixel(x, y, 1);
    draw();
    delay(50);
  }
  delay(300);
  while (1) {
    Kniwwelino.MATRIXwriteAndWait(String("Score:"));
    for (int i = 0; i < 5; i++) {
      Kniwwelino.MATRIXwrite(String(score));
      delay(500);
      clear();
      draw();
      delay(100);
    }
  }
}

void setup() {
  Kniwwelino.begin(false, true);
  Serial.begin(115200);
  snake[0].x = 0;
  snake[0].y = 0;
  fruit.x = random(0, 5);
  fruit.y = random(0, 5);
  delay(5000);
  Serial.println("Ready");
}

void loop() {
  clear();
  drawFruit();
  drawSnake();
  checkButtons();
  checkFruit();
  draw();
  if (millis() - utimer >= 200) {
    turntaken = 0;
    utimer = millis();
    shiftSnake();
    switch (dir) {
      case UP:
        snake[0].y--;
        break;
      case DOWN:
        snake[0].y++;
        break;
      case LEFT:
        snake[0].x--;
        break;
      case RIGHT:
        snake[0].x++;
        break;
    }
    if (snake[0].x > 4) gameOver(); // snake[0].x = 0;
    if (snake[0].x < 0) gameOver(); // snake[0].x = 4;
    if (snake[0].y > 4) gameOver(); // snake[0].y = 0;
    if (snake[0].y < 0) gameOver(); // snake[0].y = 4;
  }
}











