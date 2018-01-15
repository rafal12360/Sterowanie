#include <LiquidCrystal.h>

#define X_MAX_PIN          2
#define Y_MAX_PIN          15
#define Z_MAX_PIN          19

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62

const int rs = 16, en = 17, d4 = 23, d5 = 25, d6 = 27, d7 = 29;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
unsigned long actualTimeX,actualTimeY, actualTimeZ;
bool readDone=false;
int x_max,y_max,z_max,step_x=0,step_y=0,step_z=0,srl;
float factor_x, factor_y, factor_z, biggest_step;
void setup() {

  pinMode(X_ENABLE_PIN,OUTPUT);
  pinMode(X_DIR_PIN,OUTPUT);

  pinMode(X_STEP_PIN,OUTPUT);
  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(X_DIR_PIN, LOW);


  pinMode(Y_ENABLE_PIN,OUTPUT);
  pinMode(Y_DIR_PIN,OUTPUT);

  pinMode(Y_STEP_PIN,OUTPUT);
  digitalWrite(Y_ENABLE_PIN, LOW);
  digitalWrite(Y_DIR_PIN, LOW);


  pinMode(Z_ENABLE_PIN,OUTPUT);
  pinMode(Z_DIR_PIN,OUTPUT);
  pinMode(Z_STEP_PIN,OUTPUT);
  digitalWrite(Z_ENABLE_PIN, LOW);
  digitalWrite(Z_DIR_PIN, LOW);

 
  pinMode(X_MAX_PIN,INPUT_PULLUP);
  pinMode(Y_MAX_PIN,INPUT_PULLUP);
  pinMode(Z_MAX_PIN,INPUT_PULLUP);

  Serial.begin(115200);
  delay(500);

  lcd.begin(16, 4);
  lcd.print("Witaj!");
  actualTimeX=micros();
  actualTimeY=micros();
  actualTimeZ=micros();
}

String s;
int dirx;
int diry;
int dirz;
bool kranx, krany, kranz;

void loop() {

while(Serial.available())
{
  s = Serial.readStringUntil('\n');

  readDone=true;
  sscanf(s.c_str(),"x=%d y=%d z=%d",&step_x, &step_y, &step_z);

  if(step_x<0)
  {
    dirx=-1;
    step_x*=-1;
  
    digitalWrite(X_DIR_PIN, HIGH);
  }
  else{
    dirx=1;
    digitalWrite(X_DIR_PIN, LOW);
  }
      
  if(step_y<0)
  {
    diry=-1;
    step_y*=-1;
    digitalWrite(Y_DIR_PIN, HIGH);
  }
  else{
    diry=1;
    digitalWrite(Y_DIR_PIN, LOW);
  }
      
  if(step_z<0)
  {
    dirz=-1;
    step_z*=-1;
     digitalWrite(Z_DIR_PIN, HIGH);
  }
  else{
    dirz=1;
    digitalWrite(Z_DIR_PIN, LOW);
  }
  
      

  if(step_x>=step_y && step_x>=step_z){
    biggest_step=step_x;
  }
  else if (step_y>=step_x && step_y>=step_z){
    biggest_step=step_y;
  }
  else if(step_z>=step_x && step_z>=step_y){
    biggest_step=step_z;
  }

  factor_x = biggest_step/step_x;
  factor_y = biggest_step/step_y;
  factor_z = biggest_step/step_z;
}


x_max= digitalRead(X_MAX_PIN);
y_max= digitalRead(Y_MAX_PIN);
z_max= digitalRead(Z_MAX_PIN);

float base_time=300;
float timeX=factor_x*base_time;
float timeY=factor_y*base_time;
float timeZ=factor_z*base_time;

if(x_max && dirx==1){
  kranx=true;
  step_x=0;
}
if(y_max && diry==1 ){
  krany=true;
  step_y=0;
}
if(z_max && dirz==1 ){
  kranz=true;
  step_z=0;
}

if(step_x!=0){
  digitalWrite(X_STEP_PIN,LOW);
  if (micros()-actualTimeX > timeX){
    digitalWrite(X_STEP_PIN,HIGH);
    step_x--;
    actualTimeX=micros();
  }
}
if(step_y!=0){
  digitalWrite(Y_STEP_PIN,LOW);
  if (micros()-actualTimeY > timeY){
    digitalWrite(Y_STEP_PIN,HIGH);
    step_y--;
    actualTimeY=micros();
  }
}
if(step_z!=0){
  digitalWrite(Z_STEP_PIN,LOW);
  if (micros()-actualTimeZ > timeZ){
    digitalWrite(Z_STEP_PIN,HIGH);
    step_z--;
    actualTimeZ=micros();
  }
}



if(kranx && krany && kranz && readDone){
  kranx=false;
  krany=false;
  kranz=false;
  dirx=-1;
  diry=-1;
  dirz=-1;
  digitalWrite(X_DIR_PIN, HIGH);
  digitalWrite(Y_DIR_PIN, HIGH);
  digitalWrite(Z_DIR_PIN, HIGH);
  step_x=10000;
  step_y=10000;
  step_z=10000;
  Serial.println('E');
  readDone=false;
  
}

if (!z_max && !y_max && !x_max && step_y==0 && step_z==0 && step_x==0 && readDone){
  
  Serial.println('O');
  readDone=false;
}

}
