#include <Keypad.h>
#include <Servo.h>

// ----- CONFIGURACIÓN DEL TECLADO -----
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[4] = {2,3,4,5}; 
byte colPins[4] = {6,7,8,9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ----- PINES -----
Servo servo;
const int servoPin = 10;
const int buzzerPin = 11;
const int ledVerde = 12;
const int ledRojo = 13;

// ----- CONTRASEÑA -----
String password = "1234";
String input = "";

int intentos = 0;

void setup() {
  servo.attach(servoPin);
  servo.write(0); // puerta cerrada

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == '#') { // Confirmar contraseña
      verificarPassword();
    } 
    else if (key == '*') { // Borrar
      input = "";
    } 
    else {
      input += key;
    }
  }
}

void verificarPassword() {
  if (input == password) {
    accesoCorrecto();
  } else {
    accesoIncorrecto();
  }
  input = "";
}

void accesoCorrecto() {
  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledRojo, LOW);

  tone(buzzerPin, 1000, 200);

  servo.write(90); // abrir
  delay(3000);
  servo.write(0); // cerrar

  digitalWrite(ledVerde, LOW);
  intentos = 0;
}

void accesoIncorrecto() {
  intentos++;

  digitalWrite(ledRojo, HIGH);
  tone(buzzerPin, 500, 500);
  delay(1000);
  digitalWrite(ledRojo, LOW);

  if (intentos >= 3) {
    alarma();
  }
}

void alarma() {
  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, 300);
    digitalWrite(ledRojo, HIGH);
    delay(300);
    noTone(buzzerPin);
    digitalWrite(ledRojo, LOW);
    delay(300);
  }
  intentos = 0;
}