#define CUSTOM_SETTINGS
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_MUSIC_PLAYER_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_SKYPE_SHIELD


#include <OneSheeld.h>
int Rled = 10;
int Yled = 12;
int Gled = 11;
int skypeCall = 9;


const int timerButtonPin =  2; 
const int timerLedPin   =  4;
int state = 0;
int count = 0;


volatile byte var = 0;


/*possible voice command*/
const char playCommand[] = "play";
const char stopCommand[] = "stop";
const char redCommand[] = "red";
const char yellowCommand[] = "yellow";
const char greenCommand[] = "green";
const char callCommand[] = "call";
const char facebookpost[] = "post";


void setup()
  {
    pinMode(skypeCall, OUTPUT); /*port 9 - green LED (Skype)*/
    pinMode(Rled,OUTPUT); /*port 10 - red LED*/
    pinMode(Gled,OUTPUT); /*port 11 - green LED*/
    pinMode(Yled,OUTPUT); /*port 12 - yellow LED*/
  
    OneSheeld.begin();
    VoiceRecognition.start();
    VoiceRecognition.setOnError(error); 

    pinMode(timerLedPin,   OUTPUT);
    pinMode(timerButtonPin, INPUT_PULLUP);
    Serial.begin(9600);
asm(
  "ldi r25, %1\n"                      
  "inc r25 \n"
  "mov %0,r25"                         
  : "=r" (var) : "M" (42) : "r25"
  );
  
    cli();

    attachInterrupt(digitalPinToInterrupt(timerButtonPin), button, CHANGE);
  
    TCCR1A = 0;    
    TCCR1B = 0;
    TCNT1 = 0;

    TCCR1B |= (1 << CS12); 
    TCCR1B |= (1 << CS10);  
    OCR1A = 7812; 

    TCCR1B |= (1 << WGM12);   
    TIMSK1 |= (1 << OCIE1A);
    sei();

  }

void button() {
  count++;
  delay(30);
}

ISR(TIMER1_COMPA_vect) {
  if (state == 1) {
    digitalWrite(timerLedPin, HIGH);
    state = 0;
  }
  else {
    digitalWrite(timerLedPin, LOW);
    state = 1;
  }
}

void loop () 
  { 
    Serial.print("var = "); Serial.println(var);
    delay(1000);
    
    if(VoiceRecognition.isNewCommandReceived())
      {
        /*action for "play" command, compare "play" command and voice's command*/
        if(!strcmp(playCommand,VoiceRecognition.getLastCommand())) /* if(0) - false; if(!0) - true*/
          { 
            digitalWrite(skypeCall,LOW);
            digitalWrite(Rled,HIGH);
            digitalWrite(Gled,HIGH);  
            digitalWrite(Yled,HIGH);
                
          }
        
        /*action for "stop" command*/
        else if (!strcmp(stopCommand,VoiceRecognition.getLastCommand()))
          {
            digitalWrite(skypeCall,LOW);
            digitalWrite(Rled,LOW);
            digitalWrite(Gled,LOW); 
            digitalWrite(Yled,LOW);
             
          }

        /*action for "yellow" command*/
        else if (!strcmp(yellowCommand,VoiceRecognition.getLastCommand()))
          {
            digitalWrite(skypeCall,LOW);
            digitalWrite(Rled,LOW);
            digitalWrite(Gled,LOW);
            digitalWrite(Yled,HIGH);
              
            
            
          }

        /*action for "green" command*/
        else if (!strcmp(greenCommand,VoiceRecognition.getLastCommand()))
          { 
            digitalWrite(skypeCall,LOW); 
            digitalWrite(Rled,LOW);
            digitalWrite(Gled,HIGH);
            digitalWrite(Yled,LOW);
            
              
          }

        /*action for "red" command*/
        else if (!strcmp(redCommand,VoiceRecognition.getLastCommand()))
          {
            digitalWrite(skypeCall,LOW);
            digitalWrite(Rled,HIGH);
            digitalWrite(Gled,LOW);
            digitalWrite(Yled,LOW);
            
          }

        /*action for "call" command*/
        else if (!strcmp(callCommand,VoiceRecognition.getLastCommand()))
          {
            digitalWrite(skypeCall,HIGH);/*LED (port 9) blinking*/
            delay(2500);
            digitalWrite(skypeCall,LOW);
            digitalWrite(Rled,LOW);
            digitalWrite(Gled,LOW);
            digitalWrite(Yled,LOW);
            
            
            Skype.call("maksqwerty97");  /*Choiced skype user*/

          }

        /*action for "post" command*/
        else if (!strcmp(facebookpost,VoiceRecognition.getLastCommand()))
          {
            Facebook.post("Can somebody bring me pizza? Thank's.");
            
          } 
          
      }

      Serial.println(count);
      delay(100);
  }


void error(byte errorData)
  {
    switch(errorData)
      {
        case NETWORK_TIMEOUT_ERROR: Terminal.println("NETWORK TIMEOUT");break;
        case NETWORK_ERROR: Terminal.println("NETWORK ERROR");break;
        case AUDIO_ERROR: Terminal.println("AUDIO ERROR");break;
        case SERVER_ERROR: Terminal.println("NO SERVER");break;
        case SPEECH_TIMEOUT_ERROR: Terminal.println("SPEECH TIMEOUT");break;
        case NO_MATCH_ERROR: Terminal.println("NO MATCH");break;
        case RECOGNIZER_BUSY_ERROR: Terminal.println("BUSY");break;
      }
  }

