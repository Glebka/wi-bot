#define BTN 3
#define LED1 7
#define LED2 6
#define LED3 5
#define LED4 4

void setup() {
  Serial.begin(9600);
  pinMode(BTN, OUTPUT);
  pinMode(LED1, INPUT);
  pinMode(LED2, INPUT);
  pinMode(LED3, INPUT);
  pinMode(LED4, INPUT);
  digitalWrite(BTN, LOW);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  
  Serial.println("Battery Level");
}

int batteryLevel = 0;
int l1=0,l2=0,l3=0,l4=0;

void loop() {
  digitalWrite(BTN, HIGH);
  delay(100);
  digitalWrite(BTN, LOW);
  delay(500);
  l1 = digitalRead(LED1);
  l2 = digitalRead(LED2);
  l3 = digitalRead(LED3);
  l4 = digitalRead(LED4);
  batteryLevel = (l4 << 3);
  batteryLevel = batteryLevel | (l3 << 2);
  batteryLevel = batteryLevel | (l2 << 1);
  batteryLevel = batteryLevel | l1;
  
  Serial.print("Bits: ");
  Serial.print(l1, DEC);
  Serial.print(l2, DEC);
  Serial.print(l3, DEC);
  Serial.print(l4, DEC);
  Serial.print('\n');
  Serial.print("Level: ");
  Serial.print(batteryLevel, DEC);
  Serial.print('\n');
  delay(40000);
}
