/*
// TETRIS for Lilygo TTGO (ESP32)

// Forked Github Repo (modified by emanuelefavero): 
@see https://macsbug.wordpress.com/2018/01/20/tetris-with-m5stack/
*/

#include <SPI.h>
#include <TFT_eSPI.h>
#include "tet.h"
#include "esp_sleep.h"       // For deep sleep


TFT_eSPI tft = TFT_eSPI(); 

uint16_t BlockImage[8][12][12];                            // Block
uint16_t backBuffer[220][110];                             // GAME AREA
const int Length = 11;     // the number of pixels for a side of a block
const int Width  = 10;     // the number of horizontal blocks
const int Height = 20;     // the number of vertical blocks
int screen[Width][Height] = {0}; //it shows color-numbers of all positions
struct Point {int X, Y;};
struct Block {Point square[4][4]; int numRotate, color;};
Point pos; Block block;
int rot, fall_cnt = 0;
bool started = false, gameover = false;
boolean but_A = false, but_LEFT = false, but_RIGHT = false;
int game_speed = 20; // 25msec
Block blocks[7] = {
  {{{{-1,0},{0,0},{1,0},{2,0}},{{0,-1},{0,0},{0,1},{0,2}},
  {{0,0},{0,0},{0,0},{0,0}},{{0,0},{0,0},{0,0},{0,0}}},2,1},
  {{{{0,-1},{1,-1},{0,0},{1,0}},{{0,0},{0,0},{0,0},{0,0}},
  {{0,0},{0,0},{0,0},{0,0}},{{0,0},{0,0},{0,0},{0,0}}},1,2},
  {{{{-1,-1},{-1,0},{0,0},{1,0}},{{-1,1},{0,1},{0,0},{0,-1}},
  {{-1,0},{0,0},{1,0},{1,1}},{{1,-1},{0,-1},{0,0},{0,1}}},4,3},
  {{{{-1,0},{0,0},{0,1},{1,1}},{{0,-1},{0,0},{-1,0},{-1,1}},
  {{0,0},{0,0},{0,0},{0,0}},{{0,0},{0,0},{0,0},{0,0}}},2,4},
  {{{{-1,0},{0,0},{1,0},{1,-1}},{{-1,-1},{0,-1},{0,0},{0,1}},
  {{-1,1},{-1,0},{0,0},{1,0}},{{0,-1},{0,0},{0,1},{1,1}}},4,5},
  {{{{-1,1},{0,1},{0,0},{1,0}},{{0,-1},{0,0},{1,0},{1,1}},
  {{0,0},{0,0},{0,0},{0,0}},{{0,0},{0,0},{0,0},{0,0}}},2,6},
  {{{{-1,0},{0,0},{1,0},{0,-1}},{{0,-1},{0,0},{0,1},{-1,0}},
  {{-1,0},{0,0},{1,0},{0,1}},{{0,-1},{0,0},{0,1},{1,0}}},4,7}
};
extern uint8_t tetris_img[];
#define GREY 0x5AEB
int pom=0;
int pom2=0;
int pom3=0;
int pom4=0;

int score=0;
int lvl=1;

int leftButton=0;
int rightButton=35;

// Inactivity timer
unsigned long inactivityStartTime = 0;
bool inactivityTimerRunning = false;

void setup(void) {
  pinMode(37,INPUT_PULLUP);
  
  pinMode(leftButton,INPUT_PULLUP);
  pinMode(rightButton,INPUT_PULLUP);
  tft.init();
  tft.setRotation(0);
  tft.setSwapBytes(true);

  tft.pushImage(0, 0, 135, 240,tet);
  delay(3000);
  tft.fillScreen(TFT_BLACK);  
  tft.drawLine(11,19,122,19,GREY);

  tft.drawLine(11,19,11,240,GREY);
  tft.drawLine(122,19,122,240,GREY);
  
  tft.drawString("SCORE:"+String(score),14,8,1);
  tft.drawString("LVL:"+String(lvl),88,8,1);

  // Make Block
  makeBlock( 0, TFT_BLACK);    // Type No, Color
  makeBlock( 1, 0x00F0);       // DDDD     RED
  makeBlock( 2, 0xFBE4);       // DD,DD    PUPLE 
  makeBlock( 3, 0xFF00);       // D__,DDD  BLUE
  makeBlock( 4, 0xFF87);       // DD_,_DD  GREEN 
  makeBlock( 5, 0x87FF);       // __D,DDD  YELLO
  makeBlock( 6, 0xF00F);       // _DD,DD_  LIGHT GREEN
  makeBlock( 7, 0xF8FC);       // _D_,DDD  PINK

  putStartPos();                             // Start Position
  for (int i = 0; i < 4; ++i) screen[pos.X + 
  block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = block.color;
  draw();                                    // draw block
}

void loop() {
  if (gameover || !started) {
    checkInactivity(); // Check for inactivity to enter sleep mode

    if (digitalRead(leftButton) == 0) {
      for (int j = 0; j < Height; ++j)
        for (int i = 0; i < Width; ++i)
          screen[i][j] = 0;

      gameover = false;
      started = true;
      score = 0;
      game_speed = 20;
      lvl = 1;

      putStartPos();  // Start Position
      for (int i = 0; i < 4; ++i)
        screen[pos.X + block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = block.color;

      tft.drawString("SCORE:" + String(score), 14, 8, 1);
      tft.drawString("LVL:" + String(lvl), 88, 8, 1);
      draw();

      // Reset inactivity timer on game start
      inactivityTimerRunning = false;
    }

    return;
  }

  // Reset inactivity timer when game is running
  inactivityTimerRunning = false;

  if (gameover == false) {
    Point next_pos;
    int next_rot = rot;
    getNextPosRot(&next_pos, &next_rot);
    reviseScreen(next_pos, next_rot);
    delay(game_speed);  // SPEED ADJUST
  }
}

//========================================================================
void draw() {                               // draw 120x240 in the center
  for (int i = 0; i < Width; ++i) for (int j = 0; j < Height; ++j)
  for (int k = 0; k < Length; ++k) for (int l = 0; l < Length; ++l)
    backBuffer[j * Length + l][i * Length + k] = BlockImage[screen[i][j]][k][l];
    tft.pushImage(12, 20, 110, 220,*backBuffer);
}

void putStartPos() {
  game_speed=20;
  pos.X = 4; pos.Y = 1;
  block = blocks[random(7)];
  rot = random(block.numRotate);
}

bool getSquares(Block block, Point pos, int rot, Point* squares) {
  bool overlap = false;
  for (int i = 0; i < 4; ++i) {
    Point p;
    p.X = pos.X + block.square[rot][i].X;
    p.Y = pos.Y + block.square[rot][i].Y;
    overlap |= p.X < 0 || p.X >= Width || p.Y < 0 || p.Y >= 
      Height || screen[p.X][p.Y] != 0;
    squares[i] = p;
  }
  return !overlap;
}

void SetGameOver() {
  for (int i = 0; i < Width; ++i) for (int j = 0; j < Height; ++j)
  if (screen[i][j] != 0) screen[i][j] = 4;
  gameover = true;
 
}

void clearKeys() { but_A=false; but_LEFT=false; but_RIGHT=false;}

bool keyPadLoop(){
  if(digitalRead(leftButton)==0 && digitalRead(rightButton)==1){
    if(pom==0)
  {
    pom=1;
    clearKeys();
    but_LEFT =true;
    return true;}
    }else {pom=0;}
    
  if(digitalRead(rightButton)==0 && digitalRead(leftButton)==1){
  if(pom2==0)
  {pom2=1;clearKeys();but_RIGHT=true;return true;}
  } else {pom2=0;}

  if(digitalRead(37)==0){
    if(pom3==0)
  {pom3=1;clearKeys();but_A    =true;return true;}
  }else {pom3=0;}

  if(digitalRead(rightButton)==0 && digitalRead(leftButton)==0){
    if(pom4==0)
  {pom4=1;clearKeys();but_A =true;return true;}
  }else {pom4=0;}
  
  return false;
}

void getNextPosRot(Point* pnext_pos, int* pnext_rot) {
  bool received = keyPadLoop();

  if (but_LEFT) started = true;
  if (!started) return;
  pnext_pos->X = pos.X;
  pnext_pos->Y = pos.Y;
  if ((fall_cnt = (fall_cnt + 1) % 10) == 0) pnext_pos->Y += 1;
  else if (1) {
    if (but_LEFT) { but_LEFT = false; pnext_pos->X -= 1;}
    else if (but_RIGHT) { but_RIGHT = false; pnext_pos->X += 1;}
    else if (but_A) { but_A = false;
      *pnext_rot = (*pnext_rot + block.numRotate - 1)%block.numRotate; 
    }
  }
}

void deleteLine() {
  for (int j = 0; j < Height; ++j) {
    bool Delete = true;
    for (int i = 0; i < Width; ++i) if (screen[i][j] == 0) Delete = false;
    if (Delete) {
      score++;
      if(score%5==0) {
        lvl++;
        game_speed=game_speed-4;
        tft.drawString("LVL:"+String(lvl),88,8,1);
      }
      
      tft.drawString("SCORE:"+String(score),14,8,1);

      for (int k = j; k >= 1; --k) {
        for (int i = 0; i < Width; ++i) {
          screen[i][k] = screen[i][k - 1];
        }
      }
    }
  }
}

void reviseScreen(Point next_pos, int next_rot) {
  if (!started) return;

  Point next_squares[4];

  for (int i = 0; i < 4; ++i) screen[pos.X + 
    block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = 0;

  if (getSquares(block, next_pos, next_rot, next_squares)) {
    for (int i = 0; i < 4; ++i){
      screen[next_squares[i].X][next_squares[i].Y] = block.color;
    }

    pos = next_pos; rot = next_rot;
  } else {
    for (int i = 0; i < 4; ++i) screen[pos.X + 
    block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = block.color;

    if (next_pos.Y == pos.Y + 1) {
      deleteLine(); putStartPos();

      if (!getSquares(block, pos, rot, next_squares)) {
        for (int i = 0; i < 4; ++i) screen[pos.X + 
        block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = block.color;
        SetGameOver();
      }
    }
  }

  draw();
}

void makeBlock( int n , uint16_t color ){            // Make Block color       
  for ( int i =0 ; i < 12; i++ ) for ( int j =0 ; j < 12; j++ ){
    BlockImage[n][i][j] = color;                           // Block color
    if ( i == 0 || j == 0 ) BlockImage[n][i][j] = 0;       // TFT_BLACK Line
  } 
}

void checkInactivity() {
  // Start or update inactivity timer
  if (!inactivityTimerRunning) {
    inactivityStartTime = millis();
    inactivityTimerRunning = true;
  } else if (millis() - inactivityStartTime >= 10000) {
    // 10 seconds of inactivity
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(31, 116);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.println("Zzz...");
    delay(1000);
    esp_deep_sleep_start(); // * Enter deep sleep mode
  }
}
