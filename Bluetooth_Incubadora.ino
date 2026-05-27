#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include "BluetoothSerial.h"

// BLUETOOTH
BluetoothSerial BT_ESP;

// OLED
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// SENSOR DHT11
#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// PINES
#define LED_VERDE 18
#define LED_ROJO 19
#define RELAY 23
#define VENTILADOR 5
#define BUZZER 4

// VARIABLES DE CONTROL
// Control manual
bool relayManual = false;
bool relayEstado = false;
bool ventiladorManual = false;
bool ventiladorEstado = false;

// Control buzzer
bool buzzerActivo = true;

// Variables sensor
float t = 0;
float h = 0;

String estadoTemp = "";
String estadoHum = "";

// SETUP
void setup() {
  Serial.begin(115200);

  // Bluetooth
  BT_ESP.begin("INCUBADORA_ESP32");

  // Sensor
  dht.begin();

  // Pines
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);

  pinMode(RELAY, OUTPUT);
  pinMode(VENTILADOR, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  // Estados iniciales
  digitalWrite(RELAY, LOW);
  digitalWrite(VENTILADOR, LOW);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);

  digitalWrite(BUZZER, LOW);

  // OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("INCUBADORA");
  display.println("Iniciando...");
  display.display();

  delay(2000);
}

// LOOP
void loop() {
  leerBluetooth();
  leerSensor();
  clasificarVariables();
  controlarTemperatura();
  controlarLEDs();
  controlarBuzzer();
  mostrarSerial();
  mostrarOLED();

  delay(1000);
}

// FUNCIONES
// BLUETOOTH
void leerBluetooth() {
  if (BT_ESP.available()) {
    char dato = BT_ESP.read();

    Serial.print("Dato recibido: ");
    Serial.println(dato);

    // VENTILADOR
    // Encender ventilador manual
    if (dato == 'A') {
      ventiladorManual = true;
      ventiladorEstado = true;

      digitalWrite(VENTILADOR, HIGH);
    }

    // Apagar ventilador manual
    else if (dato == 'B') {
      ventiladorManual = true;
      ventiladorEstado = false;

      digitalWrite(VENTILADOR, LOW);
    }

    // RELAY
    // Encender relay manual
    else if (dato == 'C') {
      relayManual = true;
      relayEstado = true;

      digitalWrite(RELAY, HIGH);
    }
    // Apagar relay manual
    else if (dato == 'D') {
      relayManual = true;
      relayEstado = false;

      digitalWrite(RELAY, LOW);
    }

    // BUZZER
    // Activar buzzer
    else if (dato == 'E') {
      buzzerActivo = true;
    }
    // Desactivar buzzer
    else if (dato == 'F') {

      buzzerActivo = false;
      digitalWrite(BUZZER, LOW);
    }

    // REINICIAR SISTEMA
    else if (dato == 'R') {
      ESP.restart();
    }

    // MODO AUTOMATICO
    else if (dato == 'M') {

      relayManual = false;
      ventiladorManual = false;
    }
  }
}

// LEER SENSOR
void leerSensor() {

  t = dht.readTemperature();
  h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {

    Serial.println("Error leyendo sensor");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("ERROR SENSOR");
    display.display();

    delay(2000);
  }
}

// CLASIFICACION
void clasificarVariables() {

  // Temperatura
  if (t < 30) {
    estadoTemp = "Muy Baja";
  }
  else if (t < 33) {
    estadoTemp = "Baja";
  }
  else if (t <= 37) {
    estadoTemp = "Optima";
  }
  else if (t <= 39) {
    estadoTemp = "Alta";
  }
  else {
    estadoTemp = "Muy Alta";
  }

  // Humedad
  if (h < 30) {
    estadoHum = "Baja";
  }
  else if (h <= 75) {
    estadoHum = "Optima";
  }
  else if (h <= 90) {
    estadoHum = "Alta";
  }
  else {
    estadoHum = "Muy Alta";
  }
}

// CONTROL TEMPERATURA
void controlarTemperatura() {

  // RELAY AUTOMATICO
  if (!relayManual) {
    if (t < 33) {
      digitalWrite(RELAY, HIGH);
    }
    else {
      digitalWrite(RELAY, LOW);
    }
  }

  // VENTILADOR AUTOMATICO
  if (!ventiladorManual) {
    if (t > 38) {
      digitalWrite(VENTILADOR, HIGH);
    }
    else {
      digitalWrite(VENTILADOR, LOW);
    }
  }
}

// LEDS
void controlarLEDs() {
  if (estadoTemp == "Optima" && estadoHum == "Optima") {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROJO, LOW);
  }
  else {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, HIGH);
  }
}

// BUZZER
void controlarBuzzer() {
  if (!buzzerActivo) {
    digitalWrite(BUZZER, LOW);
    return;
  }
  if (estadoTemp != "Optima" || estadoHum != "Optima") {
    digitalWrite(BUZZER, HIGH);
  }
  else {
    digitalWrite(BUZZER, LOW);
  }
}

// SERIAL
void mostrarSerial() {

  Serial.println("==============");

  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" C -> ");
  Serial.println(estadoTemp);

  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" % -> ");
  Serial.println(estadoHum);
}

// OLED
void mostrarOLED() {

  display.clearDisplay();

  display.setCursor(0, 0);

  display.print("Temp: ");
  display.print(t);
  display.println(" C");

  display.print("Hum: ");
  display.print(h);
  display.println(" %");

  display.println();

  display.print("Temp: ");
  display.println(estadoTemp);

  display.print("Hum: ");
  display.println(estadoHum);

  display.display();
}