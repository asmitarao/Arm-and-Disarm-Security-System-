#include <Keypad.h>

char key , key1 , key2 , key3;
long duration;
bool flag= false;
int distance;
bool lock= true;
int copass1 = 0;
int copass2 = 0;
const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad
const int trigPin = 12;
const int echoPin = 11;
const int Buzzer = 10;

char * password = "123";
char * newpass;
int position = 0;

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
int strcount;
//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9,8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup()
{
Serial.begin(9600);
pinMode(Buzzer, OUTPUT);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT);

}

//If key is pressed, this key is stored in 'keypressed' variable
//If key is not equal to 'NO_KEY', then this key is printed out
//if count=17, then count is reset back to 0 (this means no key is pressed during the whole keypad scan process
void loop()
{
  char key = myKeypad.getKey();
  if (key != NO_KEY)
  {
    strcount ++;
    Serial.print(key);
  }
  if (key == '*' || key == '#')
  {
    strcount = 0;
    position = 0;
  }
  if (key != NO_KEY && key != password[position])
  {
    position = 0;
  }
  if(strcount >3)
  {
    position = 0;  
  }
  if (key == password[position])
  {
    position ++;
  }
  
  if (position == 3)
  {
    Serial.println("unlocked");
    lock = false;
    nobuzz();
  }
   
  if(lock)
  {
    int dis;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
  }

  if (((distance < 15 && distance > 0)|| flag ) && lock )
  {
    flag = true;
    buzz();
    //Serial.print("Distance: ");
   // Serial.println(distance);
  }
  delay(100);
  if(lock == false)
  {
    if(copass1 == 0)
    { 
      passchange();
    }
    copass1 ++;
  }
}


void buzz()
{
  tone(Buzzer, 400, 10); // play 400 Hz tone for 500 ms
  delay(100);  
}

void nobuzz()
{
  Serial.println(" ... \n");
  if(copass2 == 0)
  {
    tone(Buzzer, 2000, 1000);
    delay(100);
    tone(Buzzer, 1000, 1000);
    delay(10);
  } // unlocked
  copass2++;
}

void passchange()
{
  int r = 0;
  int y;
  while(1)
  {
    key1 = myKeypad.getKey();
    if (key1 != NO_KEY)
    {
      if(key1 == '*')
      {
       r++;
       Serial.println(key1);
      }
      else 
        r = 0;
    }
    if(r>3)
      break;
  }
  if(r>3)
  {
    Serial.println("ENTER NEW PASSWORD ... \n");
  }
}
