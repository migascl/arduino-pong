//Inspirado no tutorial Pong de Arduino https://www.arduino.cc/en/Tutorial/TFTPong
//Reprogramado por Miguel Cardoso Leirosa

//Livraria
#include <TFT.h>
#include <SPI.h>

//Definir pinos do Display
int cs = 10;
int dc = 9;
int rst = 8;

TFT TFTscreen = TFT(cs, dc, rst);

//Variavéis Accionador
int mSec;
int mSecLimit;
int speedVal = 0;

//Variavéis Jogador 1
int player1Up = 3;
int player1Down = 2;
int player1PosX = 20;
int player1PosY = 64;
int oldPlayer1PosY;

//Variavéis Jogador 2
int player2Up = 5;
int player2Down = 4;
int player2PosX = 140;
int player2PosY = 64;
int oldPlayer2PosY;


//Variavéis Bola
int ballPosX = 80;
int ballPosY = 64;
int oldBallPosX;
int oldBallPosY;
int ballDirectionX = 1;
int ballDirectionY = 1;

char player1ScorePrint[2];
int player1Score = 1;

char player2ScorePrint[3];
int player2Score = 1;



void setup() {
//Controlos dos jogadores
  pinMode(player1Up, INPUT);
  pinMode(player1Down, INPUT);

  pinMode(player2Up, INPUT);
  pinMode(player2Down, INPUT);

//TFT Screen
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
}


//Accionador
void loop() {
  mSec ++;
  mSecLimit = (5000 - speedVal);

  
  if(mSec == mSecLimit){
    mSec = 0;
  }

  if(mSecLimit < 500){
    speedVal == 500;
  }
    
  if(mSec == (mSecLimit -= 1)){
      playerControls();
      playerVisuals();
      ballPhysics();
      ballVisuals();
      gameScore();
      TFTscreen.fill(255,255,255);
      TFTscreen.rect(80,0,1,128);
      if(mSecLimit < 1){
        speedVal == 0;
      }
  }
}

//Controlos dos Jogadores
void playerControls(){
    if(digitalRead(player1Up)){
      player1PosY += 1;
      }
      
    if(digitalRead(player1Down)){
      player1PosY -= 1;
      }

     if(player1PosY > 108){
      player1PosY = 108;
     }
     if(player1PosY < 0){
      player1PosY = 0;
     }

    if(digitalRead(player2Up)){
      player2PosY += 1;
      }
        
    if(digitalRead(player2Down)){
      player2PosY -= 1;
      }

     if(player2PosY > 108){
      player2PosY = 108;
     }
     if(player2PosY < 0){
      player2PosY = 0;
     }
}


//Gráficos Jogadores
void playerVisuals(){
  if (oldPlayer1PosY != player1PosY) {
    TFTscreen.fill(0,0,0);
    TFTscreen.rect(player1PosX, oldPlayer1PosY, 2, 25);
  }
  
TFTscreen.fill(255,0,0);
  TFTscreen.rect(player1PosX, player1PosY, 2, 25);



  if (oldPlayer2PosY != player2PosY) {
    TFTscreen.fill(0,0,0);
    TFTscreen.rect(player2PosX, oldPlayer2PosY, 2, 25);
  }
  
TFTscreen.fill(255,255,0);
  TFTscreen.rect(player2PosX, player2PosY, 2, 25);

oldPlayer1PosY = player1PosY;
oldPlayer2PosY = player2PosY;
}


//Comportamento da Bola
void ballPhysics(){
  //Se Jogador 2 Marcar
  if (ballPosX > 155){
    player1Score += 1;
    ballDirectionX = -ballDirectionX;
    ballPosX = 120;
    ballPosY = 64;
    player1PosX = 20;
    player1PosY = 64;
    player2PosX = 140;
    player2PosY = 64;
    mSec = 0;
    speedVal = 0;
    mSecLimit = 5000;
    tone(6, 800, 10);
    delay(100);
    tone(6, 1000, 40);
    delay(100);
    tone(6, 1500, 80);
    delay(100);
    TFTscreen.fill(255,0,0);
    TFTscreen.rect(0, 0, 160, 128);
    TFTscreen.setTextSize(5);
    TFTscreen.stroke(0,0,0);
    TFTscreen.text(player1ScorePrint, 0, 20);
    delay(1000);
    TFTscreen.noStroke();
    TFTscreen.fill(0,0,0);
    TFTscreen.rect(0, 0, 160, 128);
  }

  //Se Jogador 2 Marcar
  if (ballPosX < 0){
    player2Score += 1;
    ballDirectionX = -ballDirectionX;
    ballPosX = 30;
    ballPosY = 64;
    player1PosX = 20;
    player1PosY = 64;
    player2PosX = 140;
    player2PosY = 64;
    mSec = 0;
    speedVal = 0;
    mSecLimit = 5000;
    tone(6, 800, 10);
    delay(100);
    tone(6, 1000, 40);
    delay(100);
    tone(6, 1200, 80);
    delay(100);
    TFTscreen.fill(255,255,0);
    TFTscreen.rect(0, 0, 160, 128);
    TFTscreen.setTextSize(5);
    TFTscreen.stroke(0,0,0);
    TFTscreen.text(player2ScorePrint, 0, 20);
    delay(1000);
    TFTscreen.noStroke();
    TFTscreen.fill(0,0,0);
    TFTscreen.rect(0, 0, 160, 128);
  }
  
//Bola Intersecta Limites do Ecrã
  if (ballPosY > 128 || ballPosY < 0) {
    ballDirectionY = -ballDirectionY;

  }

//Bola Intersecta Jogador 1
  if (inPlayer1(ballPosX, ballPosY, player1PosX, player1PosY, 2, 25)) {
    ballDirectionY = ballDirectionY;
    ballDirectionX = -ballDirectionX;
    tone(6, 1500, 10);
    mSec = 0;
    speedVal += 200;
  }

//Bola Intersecta Jogador 2
  if (inPlayer2(ballPosX, ballPosY, player2PosX, player2PosY, 2, 25)) {
    ballDirectionY = ballDirectionY;
    ballDirectionX = -ballDirectionX;
    tone(6, 1200, 10);
    mSec = 0;
    speedVal += 200;
  }
  


//Direcção da Bola
ballPosX += ballDirectionX;
ballPosY += ballDirectionY;

}

//Gráficos da Bola
void ballVisuals(){
  
    TFTscreen.fill(0,0,0);
  if (oldBallPosX != ballPosX || oldBallPosY != ballPosY) {
    TFTscreen.rect(oldBallPosX, oldBallPosY, 6, 6);
  }

  TFTscreen.fill(255,255,255);
  TFTscreen.rect(ballPosX, ballPosY, 6, 6);

  oldBallPosX = ballPosX;
  oldBallPosY = ballPosY;

}

//Pontuação
void gameScore(){
  String player1ScoreVal = String(player1Score);
  player1ScoreVal.toCharArray(player1ScorePrint, 2);

  String player2ScoreVal = String(player2Score);
  player2ScoreVal.toCharArray(player2ScorePrint, 3);
}

//Verificador de colisão entre jogador-bola
boolean inPlayer1(int ballX, int ballY, int playerRectX, int playerRectY, int playerRectHeight, int playerRectWidth) {
  boolean result = false;

  if ((ballX >= playerRectX && ballX <= (playerRectX + playerRectHeight)) &&
      (ballY >= playerRectY && ballY <= (playerRectY + playerRectWidth))) {
    result = true;
  }
  return result;
}


boolean inPlayer2(int ballX, int ballY, int playerRectX, int playerRectY, int playerRectHeight, int playerRectWidth) {
  boolean result = false;

  if ((ballX >= playerRectX && ballX <= (playerRectX + playerRectHeight)) &&
      (ballY >= playerRectY && ballY <= (playerRectY + playerRectWidth))) {
    result = true;
  }
  return result;
}

