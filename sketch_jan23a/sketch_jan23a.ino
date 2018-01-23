
float integral=0,differential=0,proportional=0;
float previous_error=0;
int current_val=0;
const int zero_bit_val=0;// add relevant data

const int analog_in=0;//add relevant data
const int analog_out_1=0;//add relevant data
const int analog_out_2=0;//add relevant data
const int kp=0,kd=0,ki=0;//add relevant data
const float slope=0;//add relevant data

int mapped_value;
float goal;
float scaling_factor=0;//add relevant data
int int_cost;

float cost;
int inertial_threshold=0;//add relevant data

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

int clip(int x)
{
  if(x>255)
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
 
 if(mapped_value>512)
  {
    goal=512+180*slope;
  }
 else
  {
    goal=512-180*slope;
  }
  
  // put your setup code here, to run once:

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
  cost*=scaling_factor;
  int_cost=clip(abs(cost));
  
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
