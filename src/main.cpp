#include <Arduino.h>

#define XPIN 34
#define YPIN 35

int xPos, yPos, xMap = 0, yMap = 0, xSnPos = 20, ySnPos = 20, dir = 0;

class Field {
public:
  bool isApple;
  bool isSnake = false;
  bool isBorder = false;
};

Field board[42][42];
hw_timer_t *LoopTimer = NULL;

void IRAM_ATTR onTimer() {

  switch (dir) {
  case 0:
    xSnPos = xSnPos + 1;
    break;
  case 1:
    ySnPos = ySnPos + 1;
    break;
  case 2:
    xSnPos = xSnPos - 1;
    break;
  case 3:
    ySnPos = ySnPos - 1;
    break;
  }

  board[ySnPos][xSnPos].isSnake = true;

  for (int row = 0; row < 42; row++) {
    for (int col = 0; col < 42; col++) {
      if (board[row][col].isBorder) {
        Serial.print("#");
      } else if (board[row][col].isSnake) {
        Serial.print("S");
      } else {
        Serial.print(" ");
      }
    }
    Serial.println("");
  }
}

void setup() {
  LoopTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(LoopTimer, &onTimer, true);
  timerAlarmWrite(LoopTimer, 500000, true);
  timerAlarmEnable(LoopTimer);
  Serial.begin(921600);
  pinMode(xPos, INPUT);
  pinMode(yPos, INPUT);
  board[xSnPos][ySnPos].isSnake = true;
  for (int bor = 0; bor < 42; bor++) {
    board[0][bor].isBorder = true;
    board[41][bor].isBorder = true;
    board[bor][0].isBorder = true;
    board[bor][41].isBorder = true;
  }
}

void loop() {
  xPos = analogRead(XPIN);
  yPos = analogRead(YPIN);
  xMap = map(xPos, 0, 4095, -512, 512);
  yMap = map(yPos, 0, 4095, 512, -512);
  if (yMap < 0 && -yMap >= abs(xMap)) {
    Serial.println("Bottom");
    if (dir != 3) {
      dir = 1;
    }
  } else if (xMap > 0 && xMap >= abs(yMap)) {
    Serial.println("Right");
    if (dir != 2) {
      dir = 0;
    }
  } else if (xMap < -100 && -xMap >= abs(yMap)) {
    Serial.println("Left");
    if (dir != 0) {
      dir = 2;
    }
  } else if (yMap > 100 && yMap >= abs(xMap)) {
    Serial.println("Top");
    if (dir != 1) {
      dir = 3;
    }
  }

  delay(250);
}