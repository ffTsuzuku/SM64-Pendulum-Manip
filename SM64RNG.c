/*
This program will simulate how long it takes for a pendulum to reach angle 0. 
This is important because angle 0 causes the pendalum to accerlate. 
It's the angle we need to constantly swing through to gain higher 
pendulum amplitudes. Thats because one we go past angle 0, we will always deaccelerate. 
This program will print out 2 pieces of information, the first being how many frames does it take
for the pendulum to swing through angle 0 without any RNG manipulation. And the Second
is how long will it take the game to orient the pedulum in a position so that is is more than half way 
through a full circle. 
*/

#include<stdio.h>
#include<stdlib.h>
#include"Callers.h"

#define startFrame 0 //The frame at which the simulation starts
#define lastFrame 2000000000 // The last frame which gets simulated

int currentFrame = startFrame;
unsigned short rngIndex = 0;
int swingCount = 0; //keeps track of how many times its swung back and forth
int atTop = 0;

unsigned short rng_function(unsigned short input)
{
  if(input == 0x560A) input = 0;
  unsigned short s0 = (unsigned char) input << 8;
  s0 = ((unsigned char) s0 << 1) ^ input;
  short s1 = (s0 >> 1) ^ 0xFF80;
  if((s0 & 1) == 0){
    if(s1 == 0xAA55){ input = 0; rngIndex = 0;}
    else input = s1 ^ 0x1FF4;
  }
  else input = s1 ^ 0x8180;

  rngIndex += 1; //increment the rngindex value so you can get new value next time
  return (unsigned short) input;
}

struct Pendulum * initializePendulum()
{
  //In SM64 Angles are unsigned shorts,
  //So values in a circle range from 0 to 65535
  struct Pendulum* pendulumObj;
  pendulumObj = malloc(sizeof(struct Pendulum));

  //Values Used here are the same ones used in game
  pendulumObj->angle = 6500;
  pendulumObj->accelerationVector = 0;
  pendulumObj->angularVelocity= 0;
  pendulumObj->accelerationMagnitude = 13;
  pendulumObj->waitTime = 0;

  return pendulumObj;
}

void updatePendulum(struct Pendulum *pendulumObj)
{
  if(pendulumObj == NULL)
    return; 

  if(pendulumObj->waitTime > 0)
    pendulumObj->waitTime--;

  //Pendulum Needs to Start Swinging
  else{
    if(pendulumObj->angle >0){ //We need to be swinging down
      pendulumObj->accelerationVector = -1;
    }
    else if(pendulumObj->angle < 0){ //Swing up
      pendulumObj->accelerationVector = 1;
    }

    pendulumObj->angularVelocity = pendulumObj->angularVelocity + pendulumObj->accelerationVector * pendulumObj->accelerationMagnitude;
    pendulumObj->angle = pendulumObj->angle + pendulumObj->angularVelocity;

    //Are we at the end of the swing?
    if(pendulumObj->angularVelocity == 0){
      atTop +=1;
      if(atTop == 2){
	swingCount += 1;
	atTop = 0;
      }
      //In SM64 accleration magnitude can only have a value of 42 or 13. 
      pendulumObj->accelerationMagnitude = ((rng_function(rngIndex) % 3) == 0) ? 42 : 13;
      if(rng_function(rngIndex) %2 == 0)
	pendulumObj->waitTime = (int)rng_function(rngIndex)/65536 * 30 + 5;
    }
  } 
}

int main()
{
  //unsigned short value = rng_function(21674);
  //printf("Value: %d\n",value);
  int currentFrame = startFrame;
  int found = 0;
  int firstZeroFrame = 0;
  struct Pendulum *pendulumObj = NULL;
  pendulumObj = initializePendulum();

  rngIndex = 0;

  while(currentFrame < lastFrame && found != 1){
    currentFrame++;
    updatePendulum(pendulumObj);
    /*    printf("Pendulum Angle: %d ",pendulumObj->angle);
    printf("@Frame %d ",currentFrame);
    printf("Acceleration %d ", pendulumObj->accelerationMagnitude);
    printf("WaitTime %d ", pendulumObj->waitTime);
    printf("rngIndex: %d ", (short) rngIndex);
    printf("Swing: %d\n",swingCount);
    */
    
    if(pendulumObj->angle == 0 && firstZeroFrame == 0){
      firstZeroFrame = currentFrame;
      // Print Out the stats for the first Angle 0
      printf("Pendulum Angle: %d ",pendulumObj->angle);
      printf("@Frame %d ",currentFrame);
      printf("Acceleration %d ", pendulumObj->accelerationMagnitude);
      printf("WaitTime %d ", pendulumObj->waitTime);
      printf("rngIndex: %d ", (short) rngIndex);
      printf("Swing: %d\n",swingCount);
    }
    if(pendulumObj->angle >= 16384)
       found = 1;
  }

  // Print out the stats for the first time the pendulum rotated at least half
  // way through an entire circle. 
  printf("Pendulum Angle: %d ",pendulumObj->angle);
  printf("@Frame %d ",currentFrame);
  printf("Acceleration %d ", pendulumObj->accelerationMagnitude);
  printf("WaitTime %d ", pendulumObj->waitTime);
  printf("rngIndex: %d ", (short) rngIndex);
  printf("Swing: %d\n",swingCount);

  free(pendulumObj);
  
  return 0;
}
