
int cx = 0;
int blob_size=0;

const int img_width = 160;          // the image width in the camera
const int threshold_value = 2000;    // the threshold value for the blob size

const int left_nslp_pin=31; // nslp ==> awake & ready for PWM
const int left_dir_pin=29;  //forward or backword
const int left_pwm_pin=40;

const int right_nslp_pin=11;
const int right_dir_pin=30;
const int right_pwm_pin=39;

unsigned long currTime;
unsigned long prevTime;

double cumError, prevError;


unsigned long currTime_speed;
unsigned long prevTime_speed;

double cumError_speed, prevError_speed;



int cx_error, size_error, output_cx, output_size;

int left_speed = 0;
int right_speed = 0;

int right_dir = 1;
int left_dir = 1;
int left_wheel = 0;
int right_wheel = 0;

typedef struct
{
  int data[2] = {0,0};
  int len=0;
}List;
List list;



double PID_control(double input, double setPoint, double Kp, double Kd){
  //Implement PID controller
  currTime = millis();

  double diffTime = (double)(currTime - prevTime);

  double error = setPoint - input;


  double rateError = (error - prevError)/diffTime;

  double output = Kp*error + Kd*rateError;

  prevError = error;
  prevTime = currTime;

  //Serial.println(cumError);

  return output;
  
}


double PID_control_speed(double input, double setPoint, double Kp, double Kd){
  //Implement PID controller
  currTime_speed = millis();

  double diffTime = (double)(currTime_speed - prevTime_speed);

  double error = setPoint - input;


  double rateError = (error - prevError_speed)/diffTime;

  double output = Kp*error + Kd*rateError;

  prevError_speed = error;
  prevTime_speed = currTime_speed;

  //Serial.println(cumError);

  return output;
  
}

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);

  pinMode(left_nslp_pin,OUTPUT);
  pinMode(left_dir_pin,OUTPUT);
  pinMode(left_pwm_pin,OUTPUT);
  
  pinMode(right_nslp_pin,OUTPUT);
  pinMode(right_dir_pin,OUTPUT);
  pinMode(right_pwm_pin,OUTPUT);

  digitalWrite(left_nslp_pin,HIGH);   //enable the motor
  digitalWrite(right_nslp_pin,HIGH);   //enable the motor
  
  digitalWrite(left_dir_pin,HIGH);
  digitalWrite(right_dir_pin,HIGH);
}

void loop() {
  if(Serial1.available() > 0)
  {
    String s = detectString();
    String tempStr = "";
    for(int i = 0; i<s.length(); i++)
    {
      if(s[i]=='('){
        tempStr = "";
      }
      else if(s[i] == ','){
        list.data[0] = tempStr.toInt();
        tempStr = "";
      }
      else if(s[i]==')'){
        list.data[1] = tempStr.toInt();
        tempStr = "";
      }
      else{
        tempStr += s[i];
      }
    }
    //Serial.print(list.data[0]);
    //Serial.print('\t');
    //Serial.println(list.data[1]);

    cx = list.data[0];            //control angle
    blob_size = list.data[1];    //control speed

  //  cx_error = cx - img_width/2;         //negative, ball on left; positive, ball on right
   // size_error = blob_size - threshold_value;  //positive, close to ball; //negative, far away to ball

    output_cx = PID_control(cx, img_width/2, 0.9, 2);
    output_size = PID_control_speed(blob_size, threshold_value, 0.5, 2);

    Serial.println(output_cx);
   // Serial.println(output_size);
    
    left_dir = 1;
    right_dir = 1;
    if(output_cx > 0 && output_cx < 70)  //need to turn left, increase right wheel speed
    {
      right_wheel = (1.8*output_cx);
      left_wheel = 0;
    }
    else if(output_cx < 0) //need to turn right, increase left wheel speed
    {
      right_wheel = 0;
      left_wheel = (-1.8*output_cx);
    }
    else if(output_cx == 0)   //object is in the middle, no turning
    {
      left_wheel = 0;
      right_wheel = 0;
    }
    else                     //no object, rotate the car to find the object
    {
      left_dir = 0;
      left_wheel = 30;
      right_wheel = 30;
    }
    
    if(output_size > 0 && output_size < 1000)    //object is far away
    {
      left_speed = 100;
      right_speed = 100;
    } 
    else                                         //object is too close
    {
      left_speed = 0;
      right_speed = 0;
    }
    
    //Serial.println(output_size);
    clearData();
  }
/*
  Serial.print(left_wheel);
  Serial.print(" ");
  Serial.print(right_wheel);
  Serial.println();
  */ 
  // digitalWrite(left_dir_pin,LOW);
   //digitalWrite(right_dir_pin,HIGH);

     
  digitalWrite(left_dir_pin,left_dir);
  digitalWrite(right_dir_pin,right_dir);
  analogWrite(left_pwm_pin,left_wheel+left_speed);
  analogWrite(right_pwm_pin,right_wheel + right_speed);

}

String detectString()
{
  while(Serial1.read() != '{');
  return(Serial1.readStringUntil('}'));
}

void clearData()
{
  memset(list.data, sizeof(list.data),0);
  list.len = 0;
}
