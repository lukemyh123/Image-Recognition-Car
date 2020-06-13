
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

int right_dir = 1;
int left_dir = 1;
int left_wheel = 0;
int right_wheel = 0;

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
    int num = s.toInt();
    if(num == 0)
    {
      left_wheel = 20;
      right_wheel = 20;
      Serial.println("Other");
    }
    else if(num == 1)
    {
      right_wheel = 50;
      left_wheel = 50;
      Serial.println("Speed_limit_30");
    }
    else if(num == 2)
    {
      right_wheel = 0;
      left_wheel = 0;
      Serial.println("Stop_Sign");
    }
    else if(num == 3)
    {
      right_wheel = 50;
      left_wheel = 20;
      Serial.println("Right_turn");
    }
    else if(num == 4)
    {
      right_wheel = 20;
      left_wheel = 50;
      Serial.println("Left_turn");
    }
    
  }
  digitalWrite(left_dir_pin,left_dir);
  digitalWrite(right_dir_pin,right_dir);
  analogWrite(left_pwm_pin,left_wheel);
  analogWrite(right_pwm_pin,right_wheel);
  

}

String detectString()
{
  while(Serial1.read() != '{');
  return(Serial1.readStringUntil('}'));
}
