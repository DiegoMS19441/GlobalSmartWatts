// Digite "thinger.io" no Library Manager.
// Instale a biblioteca thinger.io (by Alvaro Luis Bustamante)
#include <ThingerESP32.h>

// Insira as credenciais do dispositivo configurado na plataforma:
#define USERNAME "xdiegomsx"         // Seu nome de usuário na plataforma thinger.io
#define DEVICE_ID "SMARTWATTS"       // Device ID criado na plataforma
#define DEVICE_CREDENTIAL "VAD26193" // Credencial do device criada na plataforma

// Credenciais do gateway público do Wokwi:
#define SSID "Wokwi-GUEST"
#define SSID_PASSWORD "" // Nada aqui...

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// Definição dos pinos
#define PINO_POTENCIOMETRO 4 // Pino do potenciômetro
#define PINO_LED 26          // Pino do LED

// Variáveis para armazenar valores
int valorPotenciometro = 0; // Valor da leitura do potenciômetro
int pwm = 0;                // Valor do PWM para controle do LED
float valorCorrente = 0;    // Valor da corrente convertida

void setup()
{
  Serial.begin(115200);
  thing.add_wifi(SSID, SSID_PASSWORD);
  delay(500);

  // Configuração do LED como saída
  pinMode(PINO_LED, OUTPUT);

  // Adicionando o recurso "Sensor" à plataforma Thinger.io para enviar o valor da corrente
  thing["SensorCorrente"] >> outputValue(valorCorrente); // Usando o operador ">>" para enviar o valor
}

void leituraSensor()
{
  // Leitura do valor do potenciômetro (0 a 4095)
  valorPotenciometro = analogRead(PINO_POTENCIOMETRO);

  // Função map() para converter a leitura do potenciômetro em um valor de PWM (0 a 255)
  pwm = map(valorPotenciometro, 0, 1023, 0, 255);

  // Exemplo de conversão de um valor de 0 a 4095 para um intervalo de 2 a 20 amperes
  valorCorrente = map(valorPotenciometro, 0, 4095, 2, 20);

  // Envia a leitura de corrente para a plataforma Thingera
  Serial.print("Corrente elétrica: ");
  Serial.println(valorCorrente);

  // Aciona o LED proporcionalmente à leitura do potenciômetro (PWM)
  analogWrite(PINO_LED, pwm); // Ajusta a intensidade do LED conforme a leitura do potenciômetro

  // Se a corrente for maior que 20 amperes, pisque o LED
  if (valorCorrente > 18)
  {
    digitalWrite(PINO_LED, HIGH); // Acende o LED
    delay(500);                   // Espera meio segundo
    digitalWrite(PINO_LED, LOW);  // Apaga o LED
    delay(500);                   // Espera meio segundo
  }

  delay(200); // Intervalo de 200 milissegundos entre as leituras
}

void loop()
{
  // Chamada da função para leitura do sensor e controle do LED
  leituraSensor();

  // Comunicação com a plataforma Thinger.io
  thing.handle();

  // A requisição ocorrerá a cada 10 segundos.
  delay(10);
}
