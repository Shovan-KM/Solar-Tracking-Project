#include <Servo.h> // Servo library 

Servo horizontal; // Horizontal servo
int servoh = 100; // Stand horizontal servo
int servohLimitHigh = 110;
int servohLimitLow = 50;

Servo vertical; // Vertical servo 
int servov = 40; // Stand vertical servo
int servovLimitHigh = 160;
int servovLimitLow = 10;

// LDR pin connections
int ldrlt = 2; // LDR top left - BOTTOM LEFT
int ldrrt = 3; // LDR top right - BOTTOM RIGHT 
int ldrld = 0; // LDR down left - TOP LEFT
int ldrrd = 1; // LDR down right - TOP RIGHT
int dust_ldr = 4; // LDR for dust detection

// Pump_Motor Control
const int motorPin = 5; // Pump_Motor connected to digital pin 5
const int ldrThreshold = 200; // Threshold for dust LDR

// Timing for serial printing
unsigned long lastPrintTime = 0;
const long printInterval = 500; 

unsigned long motorStartTime = 0;
const long motorRunDuration = 7000; // Run motor for 7 seconds


bool motorState = false;

// Energy calculation
int offset = 20; // offset value
double current = 1.0; // in Amperes
double totalEnergy = 0; // Total energy 
double voltage = 0; // Voltage in Volts

void setup() {
  Serial.begin(9600);
  // Servo connections
  horizontal.attach(9); 
  vertical.attach(10);
  horizontal.write(180);
  vertical.write(55);
  pinMode(motorPin, OUTPUT); 
  pinMode(dust_ldr, INPUT);
  delay(3000);
}

void loop() {
  int lt = analogRead(ldrlt); // Top left
  int rt = analogRead(ldrrt); // Top right
  int ld = analogRead(ldrld); // Down left
  int rd = analogRead(ldrrd); // Down right
  int pldr = analogRead(dust_ldr); // Pump LDR (dust detection)
  
  int dtime = 100;
  int tol = 50;
  
  // Average values for LDRs
  int avt = (lt + rt) / 2; // Average value top
  int avd = (ld + rd) / 2; // Average value down
  int avl = (lt + ld) / 2; // Average value left
  int avr = (rt + rd) / 2; // Average value right

  // Difference calculations
  int dvert = avt - avd; // Difference of up and down
  int dhoriz = avl - avr; // Difference of left and right
  
  // Servos movment on LDR readings
  adjustServos(dvert, dhoriz, avt, avd, avl, avr, tol);

  // Voltage and energy measurement
  measureVoltageAndEnergy();

  // Motor control
  motorControl(pldr, ldrThreshold);
  // Check if the motor has been on for the required duration
  if (motorState && millis() - motorStartTime >= motorRunDuration) {
    stopMotor();
  }

  // Serial printing
  performSerialPrinting(avt, avd, avl, avr, pldr, tol);

  delay(dtime);
}

void adjustServos(int dvert, int dhoriz, int avt, int avd, int avl, int avr, int tol) {
  if (-1*tol > dvert || dvert > tol) {
    if (avt > avd) {
      servov = ++servov;
      if (servov > servovLimitHigh) { 
        servov = servovLimitHigh;
      }
    } else if (avt < avd) {
      servov = --servov;
      if (servov < servovLimitLow) {
        servov = servovLimitLow;
      }
    }
    vertical.write(servov);
  }

  if (-1*tol > dhoriz || dhoriz > tol) {
    if (avl < avr) {
      servoh = --servoh;
      if (servoh < servohLimitLow) {
        servoh = servohLimitLow;
      }
    } else if (avl > avr) {
      servoh = ++servoh;
      if (servoh > servohLimitHigh) {
        servoh = servohLimitHigh;
      }
    }
    horizontal.write(servoh);
  }
}

void measureVoltageAndEnergy() {
  int volt = analogRead(A5); 
  voltage = map(volt, 0, 1023, 0, 2500) + offset; // Voltage calculation
  voltage /= 100; // Convert to volts

  double power = voltage * current; //  power in Watts
  double energy = power * (0.5 / 3600); //  energy calculation loop
  totalEnergy += energy * 3600;
}

void motorControl(int pldr, int ldrThreshold) {
  if (pldr < ldrThreshold && !motorState) {
    runMotor();
  } else if (pldr >= ldrThreshold && motorState) {
    stopMotor();
  }
}

void runMotor() {
  digitalWrite(motorPin, HIGH); // motor ON
  motorState = true;
  motorStartTime = millis(); // time when the motor turned on
}

void stopMotor() {
  digitalWrite(motorPin, LOW); // motor OFF
  motorState = false;
}

void performSerialPrinting(int avt, int avd, int avl, int avr, int pldr, int tol) {
  unsigned long currentMillis = millis();
  if (currentMillis - lastPrintTime >= printInterval) {
    Serial.print(" | ");
    Serial.print(avt);
    Serial.print(" | ");
    Serial.print(avd);
    Serial.print(" | ");
    Serial.print(avl);
    Serial.print(" | ");
    Serial.print(avr);
    Serial.print(" | ");
    Serial.print(pldr);
    Serial.print(" | ");
    Serial.print(voltage);
    Serial.print(" | ");
    Serial.print(totalEnergy / 3600); // Watt-hours
    Serial.print(" | ");
    Serial.print(motorState ? "ON" : "OFF"); // motor status
    Serial.println(" ");

    lastPrintTime = currentMillis;
  }
}
