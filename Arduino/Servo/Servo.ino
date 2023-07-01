#include <CNCShield.h>
//#include <NewPing.h>
#include <Servo.h>

int incomingByte = 0;

// [direction] [Motor (x,y,z)]
//init with excel values
int gaps[18][3] = {{12,4,6},{6,4,12},{5,5,0},{4,6,12},{4,12,6},{5,0,5},{6,12,4},{12,6,4},{0,5,5},{12,4,6},{6,4,12},{0,5,5},{4,6,12},{4,12,6},{5,0,5},{6,12,4},{12,6,4},{0,5,5}};
int directions[18][3] = {{1,0,1}, {1,0,1},{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,0},{1,1,0},{0,1,0},{0,1,0},{0,1,0},{0,0,1},{0,1,1},{0,1,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1}};
char dir_name[3] = {'x','y','z'};

//Create motor objects
CNCShield cnc_shield;
StepperMotor *motor[3] = {
  cnc_shield.get_motor(0),
  cnc_shield.get_motor(1),
  cnc_shield.get_motor(2)
};

//Global vars for sonar
/*int sonarResultA = 100;
NewPing sonarA;
#define sonarTrigA 9
#define sonarEchoA 10
int sonarResultB = 100;
NewPing sonarB;
#define sonarTrigB 9
#define sonarEchoB 10
int sonarResultC = 100;
NewPing sonarC;
#define sonarTrigC 9
#define sonarEchoC 10
#define maxSonarDistance 400
bool scanning_environment = false;*/


//Global vars for servo
#define servoStepDelay 10
Servo servoA;
#define servoPinA 20
#define servoStartPosA 0
#define servoEndPosA 180
Servo servoB;
#define servoPinB 21
#define servoStartPosB 0
#define servoEndPosB 180


//Global vars for motors
bool next_move_is_rotation = false;
bool make_next_move = true;
int rotation_direction = 1;
int rotation_duration = 1000;
int next_move_length = 1000;
int next_move_direction = 0;
int speed = step_delay = 500; //microseconds

bool drop_lighter_signal = false;


void setup()
{
  Serial.begin(9600);
  cnc_shield.begin();
  cnc_shield.enable();

  /*if(scanning_environment){
    sonarA = sonar(sonarTrigA, sonarEchoA, maxSonarDistance);
    sonarB = sonar(sonarTrigB, sonarEchoB, maxSonarDistance);
    sonarC = sonar(sonarTrigC, sonarEchoC, maxSonarDistance);
  }*/

  servoA.attach(servoPinA);
  servoB.attach(servoPinB);

  //make fun move
  rotate(1000,0,step_delay);
  rotate(1000,1,step_delay);
}

void loop(){

  /*if(scanning_environment){
    updateSensors();
  }*/
  
  readInputCommands();

  if(drop_lighter_signal){
    drop();
    drop_lighter_signal = false;
  }

  if(make_next_move){
    if(next_move_is_rotation){
      rotate(rotation_duration, rotation_direction, step_delay);
    }else{
      //Serial.print("Make move with length ");Serial.print(next_move_length);Serial.print(" to direction ");Serial.println(next_move_length);
      littleMove(next_move_length, next_move_direction, step_delay);
    }
  }
  
  

  delay(3000);

}

void littleMove(int move_length, int move_direction, int speed){
  //load number of skipped steps for this direction for each stepper
  int *axis_steps = gaps[move_direction];
  //load forward or backward direction for each stepper
  int *axis_dir = directions[move_direction];
  Serial.println("**** Step_Skip_Array is: ");
  Serial.print(axis_steps[0]);Serial.print(", ");Serial.print(axis_steps[1]);Serial.print(", ");Serial.println(axis_steps[2]);

  //iterate all steps an scan each time if a stepper has to move
  //store the number of skipped steps for each axis
  int used_steps[3]={0,0,0};

  //set correct rotation direction
  for(int ax=0; ax<3; ax++){
    motor[ax]->set_dir(axis_dir[ax]);
  }

  //do certain amount of steps and use only specific
  for(int i=0; i<steps; i++){
    
      //for each axis
      for(int ax=0; ax<3; ax++){
        
          //extract skipped steps for this axis
          int this_axis_skipped_steps = axis_steps[ax];
    
          //increase axis counter
          used_steps[ax]+=1;
    
          if(used_steps[ax]>=this_axis_skipped_steps){
              Serial.print("Made on step on axis ");Serial.println(dir_name[ax]);
              used_steps[ax] = 0;
              motor[ax]->step();
          }
      }
    delayMicroseconds(speed);
  } 
}

void readInputCommands(){
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);

    //TODO update values
  }
  else{
    Serial.println("no command recieved");
  }
}


void rotate(int steps, int direction, int step_delay){
  for(int ax=0; ax<3; ax++){
    motor[ax]->set_dir(direction);
  }
  for(int i=0; i<steps; i++){
    //for each axis
    for(int ax=0; ax<3; ax++){ 
      motor[ax]->step();
    }
    delayMicroseconds(step_delay);
  }
}

void drop(){
  int j = servoStartPosB;
  for(int i=servoStartPosA;i<servoStopPosA; i++){
    j++;
    servoA.write(i);
    servoB.write(j);
    delay(servoStepDelay);
  }
}

void updateSensors(){
  /*sonarResultA = sonarA.ping_cm();
  sonarResultB = sonarB.ping_cm();
  sonarResultC = sonarC.ping_cm();*/

  //send scanned values
}


