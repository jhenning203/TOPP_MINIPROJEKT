#include <CNCShield.h>
#define NO_OF_STEPS 200
#define SLEEP_BETWEEN_STEPS_MS 15
int incomingByte = 0;

// [direction] [Motor (x,y,z)]
//init with excel values
int gaps[18][3] = {{12,4,6},{6,4,12},{5,5,0},{4,6,12},{4,12,6},{5,0,5},{6,12,4},{12,6,4},{0,5,5},{12,4,6},{6,4,12},{0,5,5},{4,6,12},{4,12,6},{5,0,5},{6,12,4},{12,6,4},{0,5,5}};
int directions[18][3] = {{1,0,1}, {1,0,1},{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,0},{1,1,0},{0,1,0},{0,1,0},{0,1,0},{0,0,1},{0,1,1},{0,1,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1}};
int steps = 1024;
CNCShield cnc_shield;
StepperMotor *motor[3];

void setup()
{
  Serial.begin(9600);
  for(int i=0;i<3;i++){
    motor[i] = cnc_shield.get_motor(i);
  }
  littleMove(1);
}

void littleMove(int direction){
  //load all number of steps for this direction (number is the number of steps that should be skipped)
  int axis_steps[3] = {gaps[direction]};
  int axis_dir[3] = {directions[direction]};

  //iterate all steps an scan each time if a stepper has to move
  for(int i=0; i<steps; i++){
    //store the number of skipped steps for each axis
    int used_steps[3]={0,0,0};
    //for each axis
    for(int ax=0; ax<3; ax++){
      //
      int this_steps = axis_steps[ax];
      used_steps[ax]++;
      if(used_steps[ax]==this_steps){
        used_steps[ax] = 0;
        makeOneStep(ax,axis_dir[ax]);
      }
    }
  }
}

void makeOneStep(int ax, int direction){
  Serial.println("Step on "+ax+" to "+direction);
  motor[ax]->set_dir(direction);
  motor[ax]->step();
}

void readInput(){
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    littleMove(incomingByte);
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
}

void loop(){

  readInput();
}