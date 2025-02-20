/*
    Programador.....: Joel Veiga
    Data............: 19/02/2025
    Obdervação......: Desenvolvimento de um Sistema de Alarme com Teclado Matricial e Sensor PIR utilizando Arduino. 
    
*/
#include <Keypad.h>

const int BAUD_RATE = 9600;

const byte POWER_LED = 2;   
const byte POWER_BUTTON = 3; 
const byte PIR_SENSOR = 5;
const byte BUZZER = 4;       

const byte STANDBY = 0;
const byte PIR_DETECT = 1;

const byte DEBUG = true;

byte alarmState = STANDBY;
bool systemActive = true;

// Configuração do teclado 4x4
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};


byte rowPins[ROWS] = {13, 12, 11, 10};  
byte colPins[COLS] = {9, 8, 7, 6};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const String correctCode = "1234";   // Código correto para desligar o sistema
String enteredCode = "";

void setup(void) {
    Serial.begin(BAUD_RATE);
  
    pinMode(POWER_LED, OUTPUT);
    pinMode(POWER_BUTTON, INPUT_PULLUP);  // Botão com resistencia pull-up
    pinMode(PIR_SENSOR, INPUT);
    pinMode(BUZZER, OUTPUT);

    digitalWrite(POWER_LED, HIGH);
    digitalWrite(BUZZER, LOW);

    if (DEBUG) Serial.println("Sistema LIGADO e pronto para detectar movimento...");
}

void loop(void) {
    // Detecta quando o botão é pressionado
    if (digitalRead(POWER_BUTTON) == LOW) { 
        delay(200); 
        if (systemActive) {
            systemActive = false;
            alarmState = STANDBY; 
            digitalWrite(POWER_LED, LOW);
            digitalWrite(BUZZER, LOW); 
            if (DEBUG) Serial.println("Sistema DESLIGADO pelo botão.");
        } else {
            systemActive = true;  // Liga o sistema
            alarmState = STANDBY;  
            digitalWrite(POWER_LED, HIGH);
            if (DEBUG) Serial.println("Sistema LIGADO pelo botão.");
        }
    }

    // Captura a entrada do teclado (Código para desativar)
    char key = keypad.getKey();
    if (key) {
        enteredCode += key;
        if (DEBUG) Serial.println(enteredCode);

        if (enteredCode.length() == 4) {
            if (enteredCode == correctCode) {
                systemActive = false;
                alarmState = STANDBY;   
                digitalWrite(POWER_LED, LOW);
                digitalWrite(BUZZER, LOW);
                if (DEBUG) Serial.println("Código correto. Sistema DESLIGADO.");
            } else {
                if (DEBUG) Serial.println("Código incorreto.");
            }
            enteredCode = ""; // Limpa o código após tentativa
        }
    }

    // O PIR ativa automaticamente o buzzer se detectar movimento
    if (systemActive && alarmState == PIR_DETECT) {
      digitalWrite(BUZZER, HIGH);
      delay(5000); 
      digitalWrite(BUZZER, LOW);
      alarmState = STANDBY; // Reseta o estado do alarme
    }


    if (digitalRead(PIR_SENSOR) == HIGH) {
    	Serial.println("Movimento detectado!");
    	digitalWrite(BUZZER, HIGH);
	}
}
