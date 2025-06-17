#define THINGER_SERIAL_DEBUG

#include <ThingerESP32.h> 
#include <ESP32Servo.h>
#include "secrets.h" 

#define BUZZER_DURATION_MS 750          
#define ALIMENTAR_MEDIO_DURATION_MS 800		
#define ALIMENTAR_PEQUENO_DURATION_MS 400      
#define INDIVIDUAL_SERVO_MEDIO_DURATION_MS 1600 
#define INDIVIDUAL_SERVO_PEQUENO_DURATION_MS 800 
#define DEBOUNCE_DELAY_MS 3000          

const int LED_VERDE_PIN = 2;
const int SERVO1_PIN = 16;
const int SERVO2_PIN = 17;
const int BUZZER_PIN = 18;

const int SERVO_POS_INICIAL = 0;
const int SERVO_POS_ABERTA = 90;

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
Servo meuServo1;
Servo meuServo2;

unsigned long last_command_time = 0;

void tocarBuzzer() {
  Serial.println("Tocando o buzzer...");
  digitalWrite(BUZZER_PIN, HIGH);
  delay(BUZZER_DURATION_MS);
  digitalWrite(BUZZER_PIN, LOW);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n------------------------------------");
  Serial.println("Iniciando Alimentador Pet (V2.3 - Correção de Inicialização)");

  pinMode(LED_VERDE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_VERDE_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("Pinos configurados. Servos em standby.");

  thing.add_wifi(SSID, SSID_PASSWORD);

  thing["status_racao"] >> [](pson& out){
    out = "Ração Suficiente";
  };

  thing["alimentarMedio"] << [](pson& in){
    if (millis() - last_command_time < DEBOUNCE_DELAY_MS) return;
    last_command_time = millis();

    Serial.println(">>> Comando 'Alimentar Medio' recebido! <<<");
    tocarBuzzer();

    Serial.println("Acionando ambos os servos...");
    meuServo1.attach(SERVO1_PIN);
    meuServo2.attach(SERVO2_PIN);
    delay(50);

    meuServo1.write(SERVO_POS_ABERTA);
    meuServo2.write(SERVO_POS_ABERTA);
    delay(ALIMENTAR_MEDIO_DURATION_MS);                                                                                 
    meuServo1.write(SERVO_POS_INICIAL);
    meuServo2.write(SERVO_POS_INICIAL);
    delay(500); 

    meuServo1.detach();
    meuServo2.detach();
    
    Serial.println("Sequência de alimentação finalizada.");
    thing.write_bucket("historico_alimentacao", "Pet alimentado (completo)");
  };

  thing["alimentarPequeno"] << [](pson& in){
    if (millis() - last_command_time < DEBOUNCE_DELAY_MS) return;
    last_command_time = millis();

    Serial.println(">>> Comando 'Alimentar Pequeno' recebido! <<<");
    tocarBuzzer();

    Serial.println("Acionando ambos os servos...");
    meuServo1.attach(SERVO1_PIN);
    meuServo2.attach(SERVO2_PIN);
    delay(50); 

    meuServo1.write(SERVO_POS_ABERTA);
    meuServo2.write(SERVO_POS_ABERTA);
    delay(ALIMENTAR_PEQUENO_DURATION_MS);
    meuServo1.write(SERVO_POS_INICIAL);
    meuServo2.write(SERVO_POS_INICIAL);
    delay(500); 

    meuServo1.detach();
    meuServo2.detach();
    
    Serial.println("Sequência de alimentação finalizada.");
    thing.write_bucket("historico_alimentacao", "Pet alimentado (completo)");
  };

  thing["acionar_servo1_medio"] << [](pson& in){
    if (millis() - last_command_time < DEBOUNCE_DELAY_MS) return;
    last_command_time = millis();

    Serial.println("Acionando Servo 1 medio individualmente...");
    tocarBuzzer(); 
    
    meuServo1.attach(SERVO1_PIN);
    delay(50);

    meuServo1.write(SERVO_POS_ABERTA);
    delay(INDIVIDUAL_SERVO_MEDIO_DURATION_MS);
    meuServo1.write(SERVO_POS_INICIAL);
    delay(500);

    meuServo1.detach();

    Serial.println("Servo 1 retornou à posição inicial.");
    thing.write_bucket("historico_alimentacao", "Servo 1 acionado");
  };

  thing["acionar_servo1_pequeno"] << [](pson& in){
    if (millis() - last_command_time < DEBOUNCE_DELAY_MS) return;
    last_command_time = millis();

    Serial.println("Acionando Servo 1 pequeno individualmente...");
    tocarBuzzer(); 
    
    meuServo1.attach(SERVO1_PIN);
    delay(50);

    meuServo1.write(SERVO_POS_ABERTA);
    delay(INDIVIDUAL_SERVO_PEQUENO_DURATION_MS);                                                                                 
    meuServo1.write(SERVO_POS_INICIAL);
    delay(500);

    meuServo1.detach();

    Serial.println("Servo 1 retornou à posição inicial.");
    thing.write_bucket("historico_alimentacao", "Servo 1 acionado");
  };

  thing["acionar_servo2_medio"] << [](pson& in){
    if (millis() - last_command_time < DEBOUNCE_DELAY_MS) return;
    last_command_time = millis();

    Serial.println("Acionando Servo 2 medio individualmente...");
    tocarBuzzer();

    meuServo2.attach(SERVO2_PIN);
    delay(50);

    meuServo2.write(SERVO_POS_ABERTA);
    delay(INDIVIDUAL_SERVO_MEDIO_DURATION_MS);
    meuServo2.write(SERVO_POS_INICIAL);
    delay(500);

    meuServo2.detach();

    Serial.println("Servo 2 retornou à posição inicial.");
    thing.write_bucket("historico_alimentacao", "Servo 2 acionado");
  };

  thing["acionar_servo2_pequeno"] << [](pson& in){
    if (millis() - last_command_time < DEBOUNCE_DELAY_MS) return;
    last_command_time = millis();

    Serial.println("Acionando Servo 2 pequeno individualmente...");
    tocarBuzzer();

    meuServo2.attach(SERVO2_PIN);
    delay(50);

    meuServo2.write(SERVO_POS_ABERTA);
    delay(INDIVIDUAL_SERVO_PEQUENO_DURATION_MS);
    meuServo2.write(SERVO_POS_INICIAL);
    delay(500);

    meuServo2.detach();

    Serial.println("Servo 2 retornou à posição inicial.");
    thing.write_bucket("historico_alimentacao", "Servo 2 acionado");
  };
}

void loop() {
  thing.handle();
  digitalWrite(LED_VERDE_PIN, thing.is_connected());
}