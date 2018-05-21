/*

Radio Fox Tranmitter

by Nathanael Wilson



This transmits a long tone, followed by a call in morse code.

It then waits an ammount of time then transmits it again.

This transmits by connecting to a radio's ptt and mic input.



*/

#define tonehz 600 //the aproxamite frequency of the tones in hz, in reality it will be a tad lower, and includes lots of harmonics.

#define dit 64 //the length of the dit in milliseconds. The dah and pauses are derived from this.

#define rest 60000 //the ammount of time between transmits in milliseconds

#define longlength 10000 //length of long tone in milliseconds

#define tx 11 //the pin of the board then keys the radio

#define audio 12 //the pin of the board then outputs the audio


  
//Do not change this array, or your morse code may not be converted properly.
String morseTable[] = {"0","12","2111","2121","211","1","1121","221","1111","11","1222","212","1211","22","21","222","1221","2212","121","111","2","112","1112","122","2112","2122","2211","22222","12222","11222","11122","11112","11111","21111","22111","22211","22221"};


String Text = "KM6BII Fox Hunt"; //This is what will be transmitted
String easterEgg = "KM6BII Foxhunt Test"; //This will be transmitted instead if pin 10 is held high at reset. 

bool egg = 0;

int TextChars = 15;
int CodeChars;

int note = 1000000 / tonehz; //converts the frequency into period
int duration;


void playtone(int note_duration){

  long elapsed_time = 0;
  long startTime=millis();
  if (note_duration > 0){

    digitalWrite(13, HIGH); //See when it is making a tone on the led
    
    while (elapsed_time < note_duration){

      digitalWrite(audio, HIGH);

      delayMicroseconds(note / 2);

      digitalWrite(audio, LOW);

      delayMicroseconds(note / 2);

      elapsed_time = millis()-startTime;
    }

    digitalWrite(13, LOW);

  }

  else{ //if it's a pause this will run

    delay(dit * 2);

  }
}

void playcode(String input){

  for (int i=0; i < input.length(); i++){
    Serial.print(input[i]);
    if (input[i] == '0'){ //See if it's a pause

      duration = 0;
    }

    else if (input[i] == '1'){ //See if it's a dit

      duration = dit;
    }

    else if (input[i] == '2'){ //See if it's a dah

      duration = dit * 3;
    }

    playtone(duration);

    delay(dit); //makes a pause between sounds, otherwise each letter would be continuous.
  }
  Serial.println();
}
String formMorse(String input){

    input.toUpperCase();

    String output = "";

    for(int i=0; i < input.length() ;i++){

        if (input[i] >= 65 && input[i] <= 90)

            output = output + morseTable[input[i]-64] + '0';

        else if (input[i] >= 48 && input[i] <= 57)

            output = output + morseTable[input[i]-21] + '0';

        else if (input[i] == 32)

            output = output + morseTable[0];

    }
    return output;

}


String EEgg = formMorse(easterEgg);
String code = formMorse(Text);

void setup(){ //set the pins to output mode

  pinMode(tx, OUTPUT);
  
  pinMode(10, INPUT);

  pinMode(audio, OUTPUT);

  pinMode(13, OUTPUT); //this is to see how the code looks with an led
  
  egg = digitalRead(10);
  
  Serial.begin(9600);
  Serial.println(egg);
  Serial.println(Text);
  Serial.println(code);
  Serial.println(easterEgg);
  Serial.println(EEgg);
  
}


void loop(){

  digitalWrite(tx, HIGH); //starts the radio transmitting

  playtone(longlength);

  delay(250);
  
  if (egg){
    //Serial.println(EEgg);
    playcode(EEgg);
  }
  else{
    //Serial.println(code);
    playcode(code);
  }

  digitalWrite(tx, LOW); //Stops the radio's transmission

  delay(rest);

}

