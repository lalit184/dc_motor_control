
float integral=0,differential=0,proportional=0;
float previous_error=0;
int current_val=0;

const int analog_in=A0;//input pin for analog read from potentiometer
const int analog_out_1=9;//this is for control signal for gnd port of motor and anti-clockwise rotation
const int analog_out_2=10;//this is for control signal for 12v port of motor and clockwise rotation
const int kp=2.8,kd=14,ki=0.1;//control system constants

int mapped_value; //remapping bitvalue if angle vs analog read is discontinuous 
float goal; // always 180 degrees from current or +512 bits



float cost;
int int_cost; // (int)cost
int inertial_threshold=45;//minimal control signal for achieving motion of motor 

int map_val(int x)  //generates a continuos map between angle and bit value
{
  int y;
  y=(x+1023-zero_bit_val)%1023;
  return y;
}


int clip(int x)
{
  if(x>150)                // you can change this as per requirement of smaller response time 
  {
    x=255;
  }
  if(x<inertial_threshold)
  {
    x=inertial_threshold;
  }
  return x;
}

void setup()
{
 Serial.begin(9600); 
 current_val=analogRead(analog_in);
 mapped_value=map_val(current_val);
 
 pinMode(analog_out_1, OUTPUT);
 pinMode(analog_out_2, OUTPUT);

 goal=(mapped_value+512)%1023;
  

}

void loop()
{                                                                                                                             
  current_val=analogRead(analog_in);
  mapped_value=map_val(current_val);
  
  previous_error=proportional;
  proportional =goal-mapped_value;
  integral+=proportional;
  differential=proportional-previous_error;

  cost=kp*proportional+ki*integral+kd*differential;
 // cost*=scaling_factor;
  int_cost=clip(abs(cost));
  
  Serial.println(int_cost);
  
  if(cost>0)
  {
    // execute anticlockwise rool i.e decrease angle 
    analogWrite(analog_out_1,int_cost);
    analogWrite(analog_out_2,0);
    
  }
  if(cost<0)
  {
    // execute clockwise roll i.e increase angle 
    analogWrite(analog_out_2,int_cost);
    analogWrite(analog_out_1,0);

  }


  
}

