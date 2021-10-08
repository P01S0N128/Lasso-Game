/* CS101 Project
   Developed by Kushagra Mishra 200010042

   Modifications added:
     1) Made changes to the appearance of the game (Changed background color, Added Timer, Level, A wee-little cowboy holding the lasso etc.).

     2) Added a bomb which subtracts the score if caught (Red in color).

     3) Added a special coin (Blue in color) which increases the score by 2 instead of 1 and additionally gives a time boost 2 seconds for level 3.

     4) Made the coin and the bomb start from random positions and have varying trajectories (Trajectory will always be a parabola for the coin but a bit more generalized for the bomb).

     5) Added a timer to the game. If the timer runs out, the game ends.

     6) Added 3 separate levels, each getting progressively harder. Each level has a few additional rules :
              i)   Level 1: The most basic level. Just catch the coins within the time limit to advance to the next level. No bombs on this level. Catch 15 coins within 60 seconds to
                            advance to the next level.

              ii)  Level 2: This level introduces bombs and is a bit more time constrained. A concept of minimum score (Lives) is also introduced here. If your score goes below -3, You lose.
                            Catch 15 coins within 60 seconds to advance to the next level.

              iii) Level 3: This level is the final and the hardest level. Time constrains, Bombs and a penalty of one score on failing to catch a coin. Catch 20 coins within 70 seconds to
                            win the game.

     7) A list of controls is displayed on the game screen while playing.

     8) An additional help document by the name of README.txt has been created which contains all the relevant information about the game.

*/

#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <conio.h>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__



class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;
  double coin_start_pos_offset_x[10]; //Will change the x-coordinate of the start position
  double coin_start_pos_offset_y[10]; //Will change the y-coordinate of the start position
  double coin_angle_offset[10]; //Will change the initial angle of velocity
  double coin_speed_offset[5]; //Will change the speed of the coin

  bool IsSpecial=false; //Used to check if the coin is special
  bool IsActive; //Used to deactivate inactive invisible coins

  // Moving parts
  Circle coin_circle;

 public:
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }

  void initCoin();
  void resetCoin();
  void makeCoinInvisible();

  bool ReturnIsSpecial() { return IsSpecial; }
  bool ReturnIsActive() { return IsActive; }

}; // End class Coin

#endif

//bomb.h
#ifndef  _BOMB_H_
#define _BOMB_H_

class Bomb : public MovingObject {
  double bomb_start_x;
  double bomb_start_y;
  double release_speed;
  double release_angle_deg;
  double bomb_ax;
  double bomb_ay;
  double bomb_start_pos_offset_x[10]; //Will change the x-coordinate of the start position
  double bomb_start_pos_offset_y[10]; //Will change the y-coordinate of the start position
  double bomb_angle_offset[10]; //Will change the initial angle of velocity
  double bomb_speed_offset[5]; //Will change the bomb speed
  double bomb_horizontal_acceleration_offset[10]; //Will change the bomb acceleration
  bool IsActive; //Used to deactivate existing invisible bombs



  // Moving parts
  Circle bomb_circle;

 public:
 Bomb(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    bomb_ax = argax;
    bomb_ay = argay;
    initBomb();
  }

  void initBomb();
  void resetBomb();
  void makeBombInvisible();

  bool ReturnIsActive() { return IsActive; }


}; // End class Bomb

#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1
#define BOMB_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100
#define COIN_SPEED_STEP 10
#define BOMB_SPEED_STEP 10
#define BOMB_ACC_STEP 5

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define BOMB_SPEED 120
#define BOMB_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30
#define BOMB_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5
#define BOMB_SIZE 5


class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  Bomb *the_bomb;
  int num_coins;

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  bool yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  void check_for_bomb(Bomb *bomb);
  int getNumCoins() { return num_coins; }
  void penalty() { num_coins--; } //Gives a penalty

  void makeInvisible(); //Used to make the lasso invisible

  double getLassoMidPtX();  //Returns Lasso band's mid point's x-coordinate
  double getLassoMidPtY();  //Returns Lasso band's mid point's y-coordinate
  double getLassoEndPtX(); //Returns Lasso band's end point's x-coordinate
  double getLassoEndPtY(); //Returns Lasso band's end point's y-coordinate


}; // End class Lasso

#endif

//coin.cpp

void Coin::initCoin() {
  for(int i=0; i<10; i++)
    coin_start_pos_offset_x[i]=(i-5)*20;
  for(int i=0; i<10; i++)
    coin_start_pos_offset_y[i]=i*10;
  for(int i=0; i<10; i++)
    coin_angle_offset[i]=(i-5)*5;
  for(int i=0; i<5; i++)
    coin_speed_offset[i]=(i-2)*COIN_SPEED_STEP;
  coin_start_x = (PLAY_X_START+WINDOW_X+coin_start_pos_offset_x[rand()%10])/2; //Changes the x-coordinate of the start position
  coin_start_y = PLAY_Y_HEIGHT-coin_start_pos_offset_y[rand()%10]; //Changes the y-coordinate of the start position
  coin_circle.reset(coin_start_x+coin_start_pos_offset_x[rand()%10], coin_start_y-coin_start_pos_offset_y[rand()%5], COIN_SIZE); //Resets the coin to a random location
  coin_circle.setColor(COLOR("yellow"));
  coin_circle.setFill(true);
  IsActive= true; //Initializes an active coin

  addPart(&coin_circle);
}

void Coin::resetCoin() {
  double coin_speed = COIN_SPEED+coin_speed_offset[rand()%5];
  double coin_angle_deg = COIN_ANGLE_DEG+coin_angle_offset[rand()%10]; // Offsets the coin angle by a random amount.
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  if(rand()%10==0) //1 in 10 chance of the coin being special
    IsSpecial=true;
  else
    IsSpecial=false;
  if(IsSpecial==true){
    coin_circle.setColor(COLOR("blue"));
    coin_circle.setFill(true);
  }
  else{
    coin_circle.setColor(COLOR("yellow"));
    coin_circle.setFill(true);
  }
  reset_all(coin_start_x+coin_start_pos_offset_x[rand()%10], coin_start_y-coin_start_pos_offset_y[rand()%10], coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::makeCoinInvisible(){
  coin_circle.setColor(COLOR("black"));
  coin_circle.setFill(false);
  IsActive= false; //Deactivates the coin
}

//bomb.cpp

void Bomb::initBomb() {
  for(int i=0; i<10; i++)
    bomb_start_pos_offset_x[i]=(i-5)*10;
  for(int i=0; i<10; i++)
    bomb_start_pos_offset_y[i]=i*5;
  for(int i=0; i<10; i++)
    bomb_angle_offset[i]=(i-5)*5;
  for(int i=0; i<5; i++)
    bomb_speed_offset[i]= (i-2)*BOMB_SPEED_STEP;
  for(int i=0; i<10; i++)
    bomb_horizontal_acceleration_offset[i]=(i-5)*BOMB_ACC_STEP;
  bomb_start_x = (PLAY_X_START+WINDOW_X+bomb_start_pos_offset_x[rand()%10])/2; //Changes the x-coordinate of the start position
  bomb_start_y = PLAY_Y_HEIGHT-bomb_start_pos_offset_y[rand()%10]; //Changes the y-coordinate of the start position
  bomb_circle.reset(bomb_start_x+bomb_start_pos_offset_x[rand()%10], bomb_start_y-bomb_start_pos_offset_y[rand()%10], BOMB_SIZE); //Resets the bomb to a random location
  bomb_circle.setColor(COLOR("red"));
  bomb_circle.setFill(true);
  IsActive= true; //Initializes an active bomb

  addPart(&bomb_circle);
}

void Bomb::resetBomb() {
  double bomb_speed = BOMB_SPEED+bomb_speed_offset[rand()%5]; //Offsets the bomb's speed by a random amount
  double bomb_angle_deg = BOMB_ANGLE_DEG+bomb_angle_offset[rand()%10]; // Offsets the bomb's angle of release by a random amount.
  bomb_ax = bomb_horizontal_acceleration_offset[rand()%10]; // Offsets the bomb's horizontal acceleration by a random amount
  bomb_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(bomb_start_x+bomb_start_pos_offset_x[rand()%10], bomb_start_y-bomb_start_pos_offset_y[rand()%10], bomb_speed, bomb_angle_deg, bomb_ax, bomb_ay, paused, rtheta);
}

void Bomb::makeBombInvisible(){
  bomb_circle.setColor(COLOR("black"));
  bomb_circle.setFill(false);
  IsActive=false; //Deactivates the bomb
}


//lasso.cpp

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("white"));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  the_bomb = NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("white"));

  lasso_band.setColor(COLOR("white"));
  draw_lasso_band();

} // End Lasso::initLasso()

bool Lasso::yank() { //This function was changed from type void to type bool so that it returns true iff a special coin is caught
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_bomb != NULL){
    num_coins--;
    the_bomb->resetBomb();
    the_bomb= NULL;
  }

  if(the_coin != NULL) {
    if(the_coin->ReturnIsSpecial()==false){
      num_coins++;
      the_coin->resetCoin();
      the_coin= NULL;  //Resets the pointer to the coin
      return false;
    }
    else{
      num_coins+=2;
      the_coin->resetCoin();
      the_coin= NULL;  //Resets the pointer to the coin
      return true;
    }
  }
  return false;
} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS && coinPtr->ReturnIsActive()) {  //The coin is only counted if its active i.e IsActive is true
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
  }
} // End Lasso::check_for_coin()

void Lasso::check_for_bomb(Bomb *bombPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double bomb_x = bombPtr->getXPos();
  double bomb_y = bombPtr->getYPos();
  double xdiff = (lasso_x - bomb_x);
  double ydiff = (lasso_y - bomb_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS && bombPtr->ReturnIsActive()) {  //The bomb is only counted if its active i.e IsActive is true
    the_bomb = bombPtr;
    the_bomb->getAttachedTo(this);
  }
} // End Lasso::check_for_bomb()

double Lasso::getLassoMidPtX(){
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  return lasso_start_x-xlen/2;
} // End Lasso::getLassoMidptX()

double Lasso::getLassoMidPtY(){
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double ylen = len*sin(arad);
  return lasso_start_y+ylen/2;
} // End Lasso::getLassoMidPtY()

double Lasso::getLassoEndPtX(){
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  return lasso_start_x-xlen;
} // End Lasso::getLassoEndPtX()

double Lasso::getLassoEndPtY(){
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double ylen = len*sin(arad);
  return lasso_start_y+ylen;
} // End Lasso::getLassoEndPtY()

void Lasso::makeInvisible(){
  lasso_circle.setColor(COLOR("black"));
  lasso_circle.setFill(false);
  lasso_loop.setColor(COLOR("black"));
  lasso_loop.setFill(false);
  lasso_line.setColor(COLOR("black"));
  lasso_band.setColor(COLOR("black"));
} // End Lasso::makeInvisible()

void levels(int n); //Function prototype for levels, n represents the level

main_program {

//Initializing the playing window
  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  Rectangle Window(WINDOW_X/2, WINDOW_Y/2, WINDOW_X, WINDOW_Y);  // A rectangle covering the whole window used to change the background color
  Window.setColor(COLOR("black"));
  Window.setFill(true);
  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));

//Controls Display
  Text Controls(WINDOW_X-100, 30, "CONTROLS: ");
  Text SpeedUp(WINDOW_X-100, 60, "= : Increase Lasso Speed ");
  Text SpeedDown(WINDOW_X-100, 90, "- : Decrease Lasso Speed ");
  Text DecreaseAngle(WINDOW_X-100, 120, "[ : Decrease Tossing angle ");
  Text IncreaseAngle(WINDOW_X-100, 150, "] : Increase Tossing angle ");
  Text Toss_Loop(WINDOW_X-100, 180, "T : Toss  ;  L : Loop ");
  Text Yank_Quit(WINDOW_X-100, 210, "Y : Yank  ;  Q : Quit ");

//Drawing the cowboy non-moving parts
  Circle Face(PLAY_X_START, PLAY_Y_HEIGHT-150, 25);
  Line Body(PLAY_X_START, PLAY_Y_HEIGHT-125, PLAY_X_START, PLAY_Y_HEIGHT-30);
  Line Leg1(PLAY_X_START, PLAY_Y_HEIGHT-30, PLAY_X_START-25, PLAY_Y_HEIGHT);
  Line Leg2(PLAY_X_START, PLAY_Y_HEIGHT-30, PLAY_X_START+25, PLAY_Y_HEIGHT);
  Rectangle Hat1(PLAY_X_START, PLAY_Y_HEIGHT-165, 80, 10);
  Rectangle Hat2(PLAY_X_START, PLAY_Y_HEIGHT-170, 50, 30);
  Face.setColor(COLOR("white"));
  Body.setColor(COLOR("white"));
  Leg1.setColor(COLOR("white"));
  Leg2.setColor(COLOR("white"));
  Hat1.setColor(COLOR("brown"));
  Hat2.setColor(COLOR("brown"));
  Face.setFill(true);
  Hat1.setFill(true);
  Hat2.setFill(true);

//Level 1 call
  levels(1);
  wait(3);
} // End main_program


void levels(int n){
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;
  float TimeLimit;
  if(n==1)
    TimeLimit= 60;
  else if(n==2)
    TimeLimit=60;
  else
    TimeLimit=70;
  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  string msg("Cmd: _");
  string Time("Time: ");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char Level[256];
  char coinScoreStr[256];
  char NextLevel[256];
  char Lives[256];
  sprintf(Level, "Level: %d", n);
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  sprintf(NextLevel, "Coins Rem: %d", 15-lasso.getNumCoins());
  sprintf(Lives, "Lives Rem: %d", 3);
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);
  Text Time_Rem(PLAY_X_START+50, PLAY_Y_HEIGHT+80, Time);
  Text level(PLAY_X_START-50, 50, Level);
  Text nextlevel(PLAY_X_START+300, PLAY_Y_HEIGHT+20, NextLevel);
  Text livesrem(PLAY_X_START+300, PLAY_Y_HEIGHT+50, Lives);

//Making the cow-boy moving parts
  Line Arm1(PLAY_X_START, PLAY_Y_HEIGHT-100, lasso.getLassoEndPtX(), lasso.getLassoEndPtY());
  Line Arm2(PLAY_X_START, PLAY_Y_HEIGHT-100, lasso.getLassoMidPtX(), lasso.getLassoMidPtY());
  Arm1.setColor(COLOR("white"));
  Arm2.setColor(COLOR("white"));


  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);

  double bomb_speed = BOMB_SPEED;
  double bomb_angle_deg = BOMB_ANGLE_DEG;
  double bomb_ax = 0;
  double bomb_ay = BOMB_G;
  Bomb bomb(bomb_speed, bomb_angle_deg, bomb_ax, bomb_ay, paused, rtheta);

  if(n==1)
    bomb.makeBombInvisible(); //Makes the bomb inactive for level 1

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;
  // After every BOMB_GAP sec, make the bomb jump
  double last_bomb_jump_end = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin or bomb stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  for(;;) {
    if(TimeLimit<0){ //If the player runs out of time, the game ends
        Text Sorry(WINDOW_X/2, WINDOW_Y/2, "SORRY :/ YOU RAN OUT OF TIME :(( ");
        Text FS(WINDOW_X/2, WINDOW_Y/2+30, coinScoreStr);
        wait(20);
        exit(0) ;
    }
    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
      case 't':
	lasso.unpause();
	Arm2.setColor(COLOR("black"));
	break;
      case 'y':
	if(lasso.yank()){
        if(n==3)
          TimeLimit+=3; //Gives 3 extra seconds for level 3
	}
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	if(n!=1)
	  lasso.check_for_bomb(&bomb);
	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
	exit(0);
      default:
	break;
      }
    }
    Arm1.reset(PLAY_X_START, PLAY_Y_HEIGHT-100, lasso.getLassoEndPtX(), lasso.getLassoEndPtY());
    Arm2.reset(PLAY_X_START, PLAY_Y_HEIGHT-100, lasso.getLassoMidPtX(), lasso.getLassoMidPtY());
    if(lasso.isPaused())
      Arm2.setColor(COLOR("white"));
    lasso.nextStep(stepTime);

    coin.nextStep(stepTime);

    if(n!=1)
      bomb.nextStep(stepTime);

    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin();
      if(n==3)
        lasso.penalty();  //Gives penalty on missing the coin on level 3
      last_coin_jump_end = currTime;
    }
    if(n!=1){
      if(bomb.isPaused()) {
        if((currTime-last_bomb_jump_end) >= BOMB_GAP) {
	      bomb.unpause();
        }
      }


      if(bomb.getYPos() > PLAY_Y_HEIGHT) {
        bomb.resetBomb();
        last_bomb_jump_end = currTime;
      }
    }

    if(lasso.getNumCoins()<1){
      sprintf(Lives, "Lives Rem: %d", 3+lasso.getNumCoins());
      livesrem.setMessage(Lives);
    }

      switch (n) { //Different levels have different rules
      case 1:

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    sprintf(NextLevel, "Coins Rem: %d", 15-lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);
    nextlevel.setMessage(NextLevel);
    if(lasso.getNumCoins()>=15){ //If a score of 15 is reached, the player advances to level 2
        lasso.makeInvisible();
        coin.makeCoinInvisible();
        charPressed.setColor(COLOR("white"));
        coinScore.setColor(COLOR("white"));
        Time_Rem.setColor(COLOR("white"));
        level.setColor(COLOR("white"));
        nextlevel.setColor(COLOR("white"));
        Arm1.setColor(COLOR("black"));
        Arm2.setColor(COLOR("black"));
        levels(2); //Level 2 call
    }
        break;

      case 2:
    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    sprintf(NextLevel, "Coins Rem: %d", 15-lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);
    nextlevel.setMessage(NextLevel);
    if(lasso.getNumCoins()>=15){ //If a score of 15 is reached, the player advances to level 3
        lasso.makeInvisible();
        coin.makeCoinInvisible();
        bomb.makeBombInvisible();
        charPressed.setColor(COLOR("white"));
        coinScore.setColor(COLOR("white"));
        Time_Rem.setColor(COLOR("white"));
        level.setColor(COLOR("white"));
        nextlevel.setColor(COLOR("white"));
        Arm1.setColor(COLOR("black"));
        Arm2.setColor(COLOR("black"));
        levels(3); //Level 3 call
    }
        break;

      case 3:
    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    sprintf(NextLevel, "Coins Rem: %d", 20-lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);
    nextlevel.setMessage(NextLevel);
    if(lasso.getNumCoins()>=20){ //If a score of 20 is reached, the player wins
        Text Congrats(WINDOW_X/2, WINDOW_Y/2, "CONGRATULATIONS!!! YOU WON!!! :D" );
        wait(20);
        exit(0);
    }
        break;
    }

    if(lasso.getNumCoins()==-3){ //If the player goes below a score of -3, the player loses
       Text Sorry(WINDOW_X/2, WINDOW_Y/2, "SORRY :/ YOU LOST :(( ");
       wait(20);
       exit(0);
    }


    stepCount++;

// Time display
    std::stringstream Timer;

    Timer<<floor(TimeLimit);
    Time= "Time: "+Timer.str();
    Time_Rem.setMessage(Time);
    currTime += stepTime;
    TimeLimit-=stepTime;
    wait(stepTime);
  } // End for(;;)

} // End of levels()

