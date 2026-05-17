# Arduino-Light-Controlled-Melody
An interactive embedded system using an Arduino Uno R4, an analog light sensor, and a passive buzzer to play a the Harry Potter, Hedwig's Theme song, in darkness.

# Ambient Light-Controlled Embedded Audio System (Hedwig's Theme)

An interactive hardware/software project built using the Arduino Uno R4 WiFi. The system utilizes an analog light sensor to dynamically trigger a synchronized visual indicator (LED) and a multi-octave musical composition (Harry Potter's "Hedwig's Theme") via a passive buzzer when a specific ambient darkness threshold is met.

## Hardware Components
* **Microcontroller:** Arduino Uno R4 WiFi
* **Inputs:** Photoresistor (Light Dependent Resistor / LDR)
* **Outputs:** 5mm Red LED, Passive Buzzer
* **Passives:** 10K Ω Resistor (Pull-down/Voltage Divider), 220 Ω Resistor (Current Limiting)

## Circuit Design & Engineering Concepts

### 1. The Analog Voltage Divider
The Arduino's microcontroller cannot read physical resistance directly; it can only measure voltage between 0V and 5V. To convert the photoresistor's changing resistance into a readable voltage signal, I engineered a **Voltage Divider circuit** using a fixed 10K Ω resistor connected to Ground as an anchor. 
* **Bright Light:** The photoresistor's internal semiconductor material drops to a very low resistance, allowing the majority of the 5V pressure field to reach the analog pin (`A0`). The Serial Monitor reads high raw digital values (up to ~900).
* **Darkness:** The photoresistor's resistance spikes drastically, grabbing the majority of the voltage drop. The analog pin drops closer to Ground (0V), yielding low raw values (below 300), which triggers the software loop.

### 2. Frequency Modulation & Array-Based Memory Architecture
Instead of using repetitive, unoptimized procedural code, the melody is structured using parallel memory arrays:
* `melody[]`: Houses precise musical pitches mapped to standard Hertz frequencies (e.g., Note B4 at 494Hz, Note E5 at 659Hz).
* `noteDurations[]`: Stores the precise arithmetic millisecond lengths for each note to maintain accurate time signatures.

The program uses a `for` loop to dynamically parse these arrays, utilizing the 3-parameter `tone(pin, frequency, duration)` function to manage the pulse-width modulation without overlapping audio signals.

## How to Run the Code
1. Clone this repository or copy the sketch into your Arduino IDE.
2. Wire the breadboard following standard series separation (ensuring the LED loop and sensor loops maintain distinct pathways to the common Ground rail).
3. Open the Serial Monitor and ensure your baud rate dropdown is set to `9600` to match the `Serial.begin(9600)` configuration.
4. Cover the photoresistor to drop the sensor value below 300 and initialize the melody.

## Project Video Link: https://github.com/user-attachments/assets/2ba9c918-593d-4613-82a7-8a743890f44b
