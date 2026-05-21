# Light-Controlled Embedded Melody System (v2.0)

An interactive embedded systems project that utilizes an Arduino Uno R4 WiFi to orchestrate hardware peripherals based on environmental ambient light levels. When light drops below a defined threshold, the system triggers an automated red LED warning status, executes a complete 30-note musical arrangement of the *Harry Potter Theme*, and drives a synchronized, dynamic scrolling marquee on an I2C Liquid Crystal Display.

### Project Demonstration
https://github.com/user-attachments/assets/4655389c-c5a7-403e-aacd-fa913438a219

---

## System Architecture & Components
* **Microcontroller:** Arduino Uno R4 WiFi (C++ Engine)
* **Display:** 1602 LCD with I2C Backplane Interface Module (PCF8574)
* **Audio:** Passive Piezo Buzzer (Inductive Load)
* **Sensors:** Photoresistor (Light Dependent Resistor / LDR)
* **Passives:** 10kΩ Resistor (Voltage Divider anchor), 220Ω Resistor (LED Current Limiting)
* **Visual Indicators:** High-Efficiency 5mm Red LED

---

## Circuit Topology & Data Flow

1. **Ambient Light Sensing:** The Photoresistor circuit continually maps lighting conditions to an analog voltage range ($0\text{V} - 5\text{V}$). To convert the photoresistor's changing resistance into a readable signal, it is wired into a **Voltage Divider circuit** using a fixed 10KΩ resistor to Ground. 
   * *Bright Light:* Sensor resistance drops, allowing voltage to reach Analog Pin `A0` (yielding values up to ~900).
   * *Darkness:* Sensor resistance spikes, pulling the analog pin close to 0V (dropping values below 300), initializing the control loops.
2. **Threshold Evaluation:** When the ambient value drops below a baseline of `300` (Dark Mode), the digital control loops execute.
3. **Synchronized Output:** The Arduino simultaneously manages pulse-width frequencies to the buzzer, current to the indicator LED, and high-speed multi-character string data transmission down the I2C serial bus.

---

## Engineering Challenges & Robust Solutions

Merging these hardware components introduced real-world physical and programmatic bottlenecks that required custom engineering workarounds:

### 1. Refresh Rate Flooding & Screen Flicker (Resolved via State Machine)
* **The Problem:** In initial iterations, commanding the LCD to print and clear text inside the hyper-fast `void loop()` caused the liquid crystals to continually reset. The screen appeared blurry, dim, and plagued by heavy static distortion.
* **The Solution:** Implemented a **State Machine software pattern** using a tracking variable (`lastState`). The system tracks its current operational state and blocks redundant write commands, updating the physical LCD screen *only once* exactly when a lighting state transition occurs.

### 2. Time Signature Distortion (Resolved via Index Mapping)
* **The Problem:** Adding standard scrolling text routines required blocking delays (`delay()`) to regulate text movement speed. When combined with the audio loop, these extra pauses distorted the song arrays, stretching out the time signature and ruining the melody's rhythm.
* **The Solution:** Removed all arbitrary text delays. Instead, the text sliding engine uses a **substring slicing algorithm** (`.substring(start, end)`) mapped directly to the active musical note index (`thisNote`). The natural time delays between musical notes now serve as the clock rhythm for the marquee animation, syncing the text steps perfectly with the music beats.

### 3. Voltage Sag & Signal Corruption (Resolved via Software Debouncing)
* **The Problem:** When Dark Mode initialized, the simultaneous power draw of the red LED, the LCD crystal array, and the inductive buzzer coil caused a sudden drop in voltage (voltage sag) across the breadboard's common power rail. This drop starved the photoresistor circuit, forcing a corrupted, unstable analog reading that tricked the code into thinking the lights were on, resetting the song after a single note.
* **The Solution:** Developed a **dual-sample software debounce filter** inside the playback routine. The microcontroller takes an initial reading, pauses 5 milliseconds to let the electrical power rail stabilize under load, and takes a second verification reading. The loop only cuts the song if both readings confidently confirm a permanent change in environment.

---

## Code Structure Insights

The core engine relies on parallel memory arrays (`melody[]` and `noteDurations[]`) parsed via a `for` loop. It utilizes a clean split-boundary substring method to simulate a moving text marquee within a fixed 16-character window without requiring intensive coordinate math:

```cpp
// 16-character padding spaces on both ends protect against out-of-bounds memory reading
const String scrollText = "                Harry Potter                ";

// Executed inside the note playback sequence
for (int thisNote = 0; thisNote < totalNotes; thisNote++) {
    // Dynamically slice a 16-character window out of memory shifted by the current note count
    String currentFrame = scrollText.substring(thisNote, 16 + thisNote);
    lcd.setCursor(0, 1);
    lcd.print(currentFrame);
    
    // Play tone...
}
