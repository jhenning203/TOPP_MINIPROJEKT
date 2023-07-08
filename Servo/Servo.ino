#include <CNCShield.h>
#define NO_OF_STEPS 200
#define SLEEP_BETWEEN_STEPS_MS 15
int incomingByte = 0;

// [direction] [Motor (x,y,z)]
//init with excel values
int gaps[18][3] = {{12,4,6},{6,4,12},{5,5,0},{4,6,12},{4,12,6},{5,0,5},{6,12,4},{12,6,4},{0,5,5},{12,4,6},{6,4,12},{0,5,5},{4,6,12},{4,12,6},{5,0,5},{6,12,4},{12,6,4},{0,5,5}};
int directions[18][3] = {{1,0,1}, {1,0,1},{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,0},{1,1,0},{0,1,0},{0,1,0},{0,1,0},{0,0,1},{0,1,1},{0,1,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1}};
int steps = 1024;
char dir[3] = {'x','y','z'};
CNCShield cnc_shield;
StepperMotor *motor[3] = {
  cnc_shield.get_motor(0),
  cnc_shield.get_motor(1),
  cnc_shield.get_motor(2)
};

void setup()
{
  Serial.begin(9600);
  cnc_shield.begin();
  cnc_shield.enable();
  littleMove(0,500);
  //rotate(1000,0,1000);
}

void littleMove(int move_direction, int speed){
  //load number of skipped steps for this direction for each stepper
  int *axis_steps = gaps[move_direction];
  //load forward or backward direction for each stepper
  int *axis_dir = directions[move_direction];
  Serial.println("**** Step_Skip_Array is: ");
  Serial.print(axis_steps[0]);Serial.print(", ");Serial.print(axis_steps[1]);Serial.print(", ");Serial.println(axis_steps[2]);

  Serial.println("move");
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

      //Serial.println(used_steps[0]);
      //Serial.println(this_axis_skipped_steps);
      if(used_steps[ax]>=this_axis_skipped_steps){
        used_steps[ax] = 0;
        motor[ax]->step();
      }
    }
  }
  delayMicroseconds(speed);
}

void makeOneStep(int ax, int direction){
  Serial.print(ax);
  Serial.print(" ");
  Serial.println(direction);
  motor[ax]->set_dir(direction);
  motor[ax]->step();
}

void readInput(){
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    littleMove(incomingByte, 500);
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
}

void loop(){

  readInput();

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