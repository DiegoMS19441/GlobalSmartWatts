#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ThingerESP32.h>

#define pinoSensor 36

LiquidCrystal_I2C lcd(0x27, 16, 2);

int valorSensor = 0;
float voltagem, porcentagem;
float corrente = 0.50; 
float consumoKWh = 0.0;
float precoKWh = 0.60; 
float custoEmReais = 0.0;
unsigned long tempoAnterior = 0;

#define USERNAME "xdiegomsx"         
#define DEVICE_ID "SMARTWATTS"       
#define DEVICE_CREDENTIAL "VAD26193" 

#define SSID "Wokwi-GUEST"
#define SSID_PASSWORD "" 

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(9600);


  lcd.init();
  lcd.backlight();
  lcd.print("SmartWatts");
  delay(2000); 
  lcd.clear();

  thing.add_wifi(SSID, SSID_PASSWORD);


  thing["Custo em Reais"] >> outputValue(custoEmReais); 
}

void loop() {
  valorSensor = analogRead(pinoSensor);
  voltagem = valorSensor * (3.3 / 4095);
  porcentagem = (valorSensor * 100) / 4095;

  unsigned long tempoAtual = millis();
  float tempoHoras = (tempoAtual - tempoAnterior) / 3600000.0;
  tempoAnterior = tempoAtual;

  float potencia = voltagem * corrente;
  consumoKWh += (potencia * tempoHoras);

  float custoEmReais = consumoKWh * precoKWh;

  Serial.print("Consumo Acumulado: ");
  Serial.print(consumoKWh, 6);
  Serial.print(" kWh | Custo Total: R$ ");
  Serial.println(custoEmReais, 2);

  lcd.setCursor(0, 0); 
  lcd.print("Consumo:");
  lcd.print(consumoKWh, 2);
  lcd.print(" kWh");

  lcd.setCursor(0, 1); 
  lcd.print("Custo: R$");
  lcd.print(custoEmReais, 4);

  thing.handle();

  delay(2000);
}
