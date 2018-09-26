const byte interruptPin = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);      // open the serial port at 115200 bps:    
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), my_interrupt_handler, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void my_interrupt_handler()
{
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 200) 
 {
   Serial.println("Interrupt triggered.");
 }
 last_interrupt_time = interrupt_time;
}

