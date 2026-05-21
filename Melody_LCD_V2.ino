#include <LiquidCrystal_I2C.h>

const int photoresistorPin = A0;
const int redLedPin = 9;
const int buzzer = 8;

// Complete Pitch Dictionary
const int note_D4  = 294;
const int note_G4  = 392;
const int note_A4  = 440;
const int note_AS4 = 466;
const int note_B4  = 494;
const int note_C5  = 523;
const int note_CS5 = 554;
const int note_D5  = 587;
const int note_DS5 = 622;
const int note_E5  = 659;
const int note_F5  = 698;
const int note_FS5 = 740;
const int note_G5  = 784;
const int note_A5  = 880;
const int note_AS5 = 932;
const int note_B5  = 988;
const int note_D6  = 1175;

int melody[] = {
  note_B4, note_E5, note_G5, note_FS5, note_E5, note_B5, note_A5, note_FS5, 
  note_E5, note_G5, note_FS5, note_DS5, note_F5, note_B4, note_B4, note_E5, 
  note_G5, note_FS5, note_E5, note_B5, note_D6, note_CS5, note_C5, note_A5, 
  note_C5, note_B5, note_AS5, note_B4, note_G5, note_E5
};

int noteDurations[] = {
  250, 375, 125, 250, 500, 250, 750, 750, 375, 125, 250, 500, 250, 500,
  250, 375, 125, 250, 500, 250, 500, 250, 500, 250, 375, 125, 250, 500, 250, 500
};

const int totalNotes = sizeof(melody) / sizeof(melody[0]);

LiquidCrystal_I2C lcd(0x27, 16, 2); 

String lastState = ""; 

// Fixed standard spaces padding the text
const String scrollText = "                Harry Potter                ";

void setup() {
  Serial.begin(9600);
  pinMode(redLedPin, OUTPUT); 

  lcd.init();       
  lcd.clear();      
  lcd.backlight();  
}

void loop() {
  int sensorValue = analogRead(photoresistorPin);
  Serial.println(sensorValue);

  // --- DARK MODE BRANCH ---
  if (sensorValue < 300) {
    digitalWrite(redLedPin, HIGH); 
    
    if (lastState != "dark") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Status: Dark"); // Bottom line left blank on purpose for marquee
      lastState = "dark"; 
    }
    
    // Play the song sequence
    for (int thisNote = 0; thisNote < totalNotes; thisNote++) {
      // Take a reading, wait 5 milliseconds for voltage to settle, then verify
      int initialCheck = analogRead(photoresistorPin);
      delay(5);
      int liveCheck = analogRead(photoresistorPin);

      if (initialCheck >= 300 && liveCheck >= 300) {
        break; // Only quit if BOTH readings confirm it's actually bright
      }

      String currentFrame = scrollText.substring(thisNote, 16 + thisNote);
      lcd.setCursor(0, 1);
      lcd.print(currentFrame);
      
      tone(buzzer, melody[thisNote], noteDurations[thisNote]);
      int pauseBetweenNotes = noteDurations[thisNote] * 1.30;
      delay(pauseBetweenNotes);
      noTone(buzzer); 
    }
    
    delay(1000); 
    
  } 
  // --- LIGHT MODE BRANCH ---
  else {
    digitalWrite(redLedPin, LOW);  
    noTone(buzzer);
    
    if (lastState != "light") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Status: Light");
      lcd.setCursor(0, 1);
      lcd.print("System Ready");
      lastState = "light"; 
    }
  }
  
  delay(50); 
}