/*
  @brief:The Pendalum Struct is the main object which will be observed. 
  @accelerationVector: Desribes if the pendulum is swinging up or down
  @angularVelocity: Controls how much an angle will change per frame
  @Angle: Desribes where it is doing a swing position. 0 being perfectly still
  @waitTime: Describes how long the pendalum will pause for, when it reaches the
  max height of it's swing. 
*/
struct Pendulum{
  int accelerationVector;
  int angularVelocity;
  int accelerationMagnitude;
  int angle;
  int waitTime;
};

/*
  @brief: This function is called whenever an rng value needs to be obtained
  @param input: this defines the current index of the value you want, since
  the values have a period and get cycled, all available values can be reached.
*/ 
unsigned short rng_function(unsigned short input);

/*
  @brief: This function is called when a Pendulum needs to be created
  The initialized values are obtained from the actual game. 
*/

struct Pendulum * initializePendulum();

/*Function to update pendulum */
void updatePendulum(struct Pendulum *pendulumObj);
