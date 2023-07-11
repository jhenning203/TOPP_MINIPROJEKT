#include <CNCShield.h>
//#include <NewPing.h>
#include <Servo.h>

int incomingByte = 0;

// [direction] [Motor (x,y,z)]
//init with excel values
int gaps[18][3] =       {{12,4,6},{6,4,12}, {5,5,0},{4,6,12}, {4,12,6},{5,0,5},{6,12,4},{12,6,4},{0,5,5}, {12,4,6},   {6,4,12}, {0,5,5},{4,6,12},{4,12,6},{5,0,5},{6,12,4},{12,6,4},{0,5,5}};
int directions[18][3] = {{1,0,1}, {1,0,1},  {1,0,0},{1,0,0},  {1,0,0},{1,0,0},{1,1,0},  {1,1,0},{0,1,0},  {0,1,0},    {0,1,0},  {0,0,1},{0,1,1},{0,1,1},{0,0,1},  {0,0,1},{0,0,1},  {0,0,1}};
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
#define servoStepDelay 20
Servo servoA;
#define servoPinA 13

int SPosA = 0;

Servo servoB;
#define servoPinB 12
#define servoDist 45
int SPosB = 0;



//Global vars for motors
bool next_move_is_rotation = false;
bool make_next_move = false;
int rotation_direction = 1;
int rotation_duration = 4000;
int next_move_length = 20000;
int next_move_direction = 0;
int step_delay = 100; //microseconds
int step_delay_rotation = 500;

bool drop_lighter_signal = false;


void setup()
{
  Serial.begin(115200);
  Serial.println("****Reboot****");
  cnc_shield.begin();
  

  /*if(scanning_environment){
    sonarA = sonar(sonarTrigA, sonarEchoA, maxSonarDistance);
    sonarB = sonar(sonarTrigB, sonarEchoB, maxSonarDistance);
    sonarC = sonar(sonarTrigC, sonarEchoC, maxSonarDistance);
  }*/



  //make fun move
  rotate(1000,0,step_delay_rotation);
  rotate(1000,1,step_delay_rotation);

  drop();
}

void loop(){
  Serial.println("next loop iteration **********");
  Serial.print("next move :");
  Serial.println(make_next_move);
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
      Serial.println("Rotation start");
      rotate(rotation_duration, rotation_direction, step_delay_rotation);
      Serial.println("Rotation end");
    }else{
      //Serial.print("Make move with length ");Serial.print(next_move_length);Serial.print(" to direction ");Serial.println(next_move_length);
      littleMove(next_move_length, next_move_direction, step_delay);
    }
  }

  
  /*
  15:16:38.473 -> I received: 108
15:16:38.473 -> Left Rotation
15:16:38.473 -> I received: 10
15:16:38.473 -> End
15:16:38.473 -> 1
15:16:40.646 -> re
15:16:43.634 -> I received: 114
15:16:43.634 -> Left Rotation
15:16:43.634 -> I received: 10
15:16:43.634 -> End
  */
  

  

}

void littleMove(int move_length, int move_direction, int move_step_delay){
  cnc_shield.enable();

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
  for(int i=0; i<move_length; i++){
    
      //for each axis
      for(int ax=0; ax<3; ax++){
        
          //extract skipped steps for this axis
          int this_axis_skipped_steps = axis_steps[ax];
    
          //increase axis counter
          used_steps[ax]+=1;
    
          if(used_steps[ax]>=this_axis_skipped_steps && this_axis_skipped_steps != 0){
              //Serial.print("Made on step on axis ");Serial.println(dir_name[ax]);
              used_steps[ax] = 0;
              motor[ax]->step();
          }
      }
    delayMicroseconds(move_step_delay);
  } 
  cnc_shield.disable();
}

void readInputCommands(){
  int state = 0;
  if (Serial.available() > 0) {
    // read the incoming byte:
    while(Serial.available() > 0){
      incomingByte = Serial.read();

      Serial.print("I received: ");
      Serial.println(incomingByte, DEC);

      //recieve fresh command
      if(state == 0){
        state = 1;


        if(incomingByte == 112){
          // drop();
        }
        //directions 0 - 9
        else if(incomingByte >= 48 && incomingByte <= 57){
          make_next_move = true;
          next_move_is_rotation = false;
          int real_direction = (incomingByte - 48);
          next_move_direction = real_direction;
          Serial.print("Direction ");
          Serial.println(real_direction);
        }
        //directions a - h
        else if(incomingByte >= 97 && incomingByte <= 104){
          make_next_move = true;
          next_move_is_rotation = false;
          int real_direction = (incomingByte - 87);
          next_move_direction = real_direction;
          Serial.print("Direction ");
          Serial.println(real_direction);
        }
        else if(incomingByte == 108){
          make_next_move = true;
          next_move_is_rotation = true;
          rotation_direction = 0;
          Serial.println("Left Rotation");
        }
        else if(incomingByte == 114){
          make_next_move = true;
          next_move_is_rotation = true;
          rotation_direction = 1;
          Serial.println("Left Rotation");
        }
        //set speed with s
        else if(incomingByte == 115){
          Serial.print("Set speed");
          state = 2;
        }
        else {
          make_next_move = false;
        }

        if(incomingByte == 10){
         Serial.println("Wrong Command");
         state = 0;
        }
      }
      //recieve part of command
      else if(state == 1){
        if(incomingByte == 10){
          Serial.println("End of Command");
          state = 0;
        }
      }

      else if(state == 2){
        step_delay = incomingByte;
        Serial.print("Set speed to");
        Serial.println(step_delay);
        state = 0;
        delay(2000);
      }
    }
    //TODO update values
  }
  else{
    Serial.println("no command recieved");
    delay(1000);
    make_next_move = false;
  }
}


void rotate(int steps, int direction, int step_delay){
  cnc_shield.enable();

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

  cnc_shield.disable();
}

void drop(){
  servoA.attach(servoPinA);
  servoB.attach(servoPinB);

  Serial.println("Drop Lighter");
  SPosB=servoDist;
  for(SPosA=0;SPosA<servoDist; SPosA++){
    SPosB--;
    servoA.write(SPosA);
    delay(servoStepDelay);
    servoB.write(SPosB);
    
  }
  Serial.println("Dropped");
  for(SPosA=servoDist;SPosA>=0; SPosA--){
    SPosB++;
    servoA.write(SPosA);
    delay(servoStepDelay);
    servoB.write(SPosB);
    
  }

    servoA.detach();
  servoB.detach();
  
}

void updateSensors(){
  /*sonarResultA = sonarA.ping_cm();
  sonarResultB = sonarB.ping_cm();
  sonarResultC = sonarC.ping_cm();*/

  //send scanned values
}