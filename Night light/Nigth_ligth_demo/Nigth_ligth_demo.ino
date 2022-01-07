//Nigth light demo example


#include <avr/io.h>
#include <util/delay.h>

//defined group pins  
#define PIN_BUTTON_MODE PB2
#define PIN_BUTTON_ON_OFF PB4
#define PIN_DIMMER A3
#define PWM_ONE PB1
#define PWM_TWO PB0
analog_pin_t myPin = PIN_DIMMER;

int8_t count_pressed=0;

// state buttons
bool current_button_on_off = false;
bool last_button_on_off = false;
bool current_button_mode = false;
bool last_button_mode = false;

//enumeration 
enum state_nigth_light
{
  Off,
  On
};

enum mode_nigth_light
{
     Left_light,
     Right_light,
     Both_lights
};

//function for button_pressed_delay

bool pressed_button(bool last,int pin)
{
   bool current = digitalRead(pin); 
   if(last!=current)
   {
      delay(5);
      current=digitalRead(pin);    
   }
    return current;
}

 void configurate_current_state(state_nigth_light on_off)// on or off night light
 {
    switch(on_off)
    {
      case Off:digitalWrite(PWM_ONE,LOW);digitalWrite(PWM_TWO,LOW);
      break;
      case On:analogWrite(PWM_ONE,digitalRead(myPin)/4);analogWrite(PWM_TWO,digitalRead(myPin)/4);
      break;
      default: break;
    } 
  
 }

 void toggling_mode_nigth_light(mode_nigth_light mode_)
 {
      switch(mode_)
      {
        case Left_light:analogWrite(PWM_ONE,digitalRead(myPin)/4);digitalWrite(PWM_TWO,LOW);
        break;
        case Right_light:digitalWrite(PWM_ONE,LOW);analogWrite(PWM_TWO,digitalRead(myPin)/4);
        break;
        case Both_lights:analogWrite(PWM_ONE,digitalRead(myPin)/4);analogWrite(PWM_TWO,digitalRead(myPin)/4);  
        break;
        default:break;
      
      }  
 }

 state_nigth_light state = state_nigth_light::Off; //start state
 mode_nigth_light mode = mode_nigth_light::Left_light;

//setup pins
void setup()
{
          
    pinMode(PIN_BUTTON_MODE,INPUT_PULLUP); 
    pinMode(PIN_BUTTON_ON_OFF,INPUT_PULLUP);
    pinMode(PIN_DIMMER,INPUT);
    pinMode(PWM_ONE,OUTPUT);
    pinMode(PWM_TWO,OUTPUT);
    
}

void loop()
{
 
  current_button_on_off = pressed_button(last_button_on_off,PIN_BUTTON_ON_OFF);
  current_button_mode = pressed_button(last_button_mode,PIN_BUTTON_MODE);
  
  if(last_button_on_off==false && current_button_on_off==true)
  {   
      state=static_cast<state_nigth_light>(!state);
      configurate_current_state(state);
            
  }   
 
  last_button_on_off = current_button_on_off;
    
  if(state)
  {
   if(last_button_mode==false && current_button_mode==true)
   {
     toggling_mode_nigth_light(mode);
     count_pressed++;    
   }
   last_button_mode=current_button_mode;
   if(count_pressed>2)count_pressed=0;
   mode=static_cast<mode_nigth_light>(count_pressed);
  } 

}
