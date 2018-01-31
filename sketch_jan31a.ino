
float integral=0,differential=0,proportional=0;
float previous_error=0;
int current_val=0;
const int zero_bit_val=0;// add relevant data

const int analog_in=A0;//add relevant data
const int analog_out_1=10;//add relevant data
const int analog_out_2=9;//add relevant data
const int kp=10,kd=0,ki=0;//add relevant data
const float slope=360.0/1023.0;//add relevant data

int mapped_value;
float goal;
float scaling_factor=0;//add relevant data
int int_cost;

float cost;
int inertial_threshold=30;//add relevant data

int map_val(int x)
{
  int y;
  if(x>zero_bit_val)
    {
      y=x+zero_bit_val;
    }
  else 
  {
    y=x-zero_bit_val;
  }
  return y;
}

int map_val_general(int x)
{
  int y;
  y=(x+1023-zero_bit_val)%1023;
  return y;
}

int clip(int x)
{
  if(x>250)                // you can change this as per requirement of smaller response time 
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
 mapped_value=current_val;
 
 pinMode(analog_out_1, OUTPUT);
 pinMode(analog_out_2, OUTPUT);
 goal=((int)(mapped_value+180*slope))%1023;
  // put your setup code here, to run once:

}

void loop()
{                                                                                                                             
  current_val=analogRead(analog_in);
   
  mapped_value=current_val;
  
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
    // clockwise rotation this could be different later 
    analogWrite(analog_out_1,int_cost);
    
    analogWrite(analog_out_2,0);
    
  }
  if(cost<0)
  {
    // anti-clockwise rotation this could be different later 
    analogWrite(analog_out_2,int_cost);
    analogWrite(analog_out_1,0);
    
  }
  
}
