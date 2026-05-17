const int photoresistorPin = A0;
const int redLedPin = 9;
const int buzzer = 8;

// Complete Pitch Dictionary for the entire song
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

// The Complete Melody Array (Part 1 and Part 2)
int melody[] = {
  // Part 1
  note_B4, 
  note_E5, note_G5, note_FS5, 
  note_E5, note_B5, 
  note_A5, 
  note_FS5, 
  note_E5, note_G5, note_FS5, 
  note_DS5, note_F5, 
  note_B4,
  
  // Part 2
  note_B4,
  note_E5, note_G5, note_FS5,
  note_E5, note_B5,
  note_D6, note_CS5,
  note_C5, note_A5,
  note_C5, note_B5, note_AS5,
  note_B4, note_G5,
  note_E5
};

// The exact musical durations for every single note matching the array above
int noteDurations[] = {
  // Part 1 Timings
  250,           // B4
  375, 125, 250, // E5, G5, FS5
  500, 250,      // E5, B5
  750,           // A5
  750,           // FS5
  375, 125, 250, // E5, G5, FS5
  500, 250,      // DS5, F5
  500,           // B4

  // Part 2 Timings
  250,           // B4
  375, 125, 250, // E5, G5, FS5
  500, 250,      // E5, B5
  500, 250,      // D6, CS5
  500, 250,      // C5, A5
  375, 125, 250, // C5, B5, AS5
  500, 250,      // B4, G5
  500            // E5
};

// Automatically calculate the exact length of the complete song
const int totalNotes = sizeof(melody) / sizeof(melody[0]);

void setup() {
  Serial.begin(9600);
  pinMode(redLedPin, OUTPUT); 
}

void loop() {
  int sensorValue = analogRead(photoresistorPin);
  Serial.println(sensorValue);

  // LED Logic
  if (sensorValue < 300) {
    digitalWrite(redLedPin, HIGH); 
  } else {
    digitalWrite(redLedPin, LOW);  
  }

  // Full Song Playback Logic
  if (sensorValue < 300) {
    
    for (int thisNote = 0; thisNote < totalNotes; thisNote++) {
      // Play the specific note from our full dictionary
      tone(buzzer, melody[thisNote], noteDurations[thisNote]);

      // Calculate the explicit rhythmic space between notes
      int pauseBetweenNotes = noteDurations[thisNote] * 1.30;
      delay(pauseBetweenNotes);
      
      noTone(buzzer); 
    }
    
    // 2-second epic pause before the full loop starts over again
    delay(2000); 
    
  } else {
    noTone(buzzer);
  }
  
  delay(10); 
}
