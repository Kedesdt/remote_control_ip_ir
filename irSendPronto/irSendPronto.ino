#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const char* ssid = "renato";  // Substitua pelo nome da sua rede Wi-Fi
const char* password = "lablab01";  // Substitua pela senha da sua rede Wi-Fi

const uint16_t kIrLed = 15;  // GPIO do ESP8266 para enviar o sinal IR (recomendado: 4 - D2).
IRsend irsend(kIrLed);  // Inicializa o objeto IRsend.

uint16_t samsungProntoCode[100] = {
    0x0000, 0x006D, 0x0000, 0x0022,
    0x00AA, 0x00AA, 0x0014, 0x003F, 0x0014, 0x003F, 0x0014, 0x003F, 
    0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014,
    0x0014, 0x0014, 0x0014, 0x003F, 0x0014, 0x003F, 0x0014, 0x003F,
    0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 
    0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x003F, 0x0014, 0x0014,
    0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014,
    0x0014, 0x0014, 0x0014, 0x003F, 0x0014, 0x0014, 0x0014, 0x003F,
    0x0014, 0x003F, 0x0014, 0x003F, 0x0014, 0x003F, 0x0014, 0x003F,
    0x0014, 0x003F, 0x0014, 0x0706
};

WebServer server(80);  // Cria um servidor na porta 80 (HTTP).

void handleRoot() {
  // Página inicial do servidor
  String html = "<html><body>";
  html += "<h1>Controle Remoto IR</h1>";
  html += "<p>Envie o comando IR via GET:</p>";
  html += "<a href=\"/send?code=0000%20006D%20...\">Enviar</a>";
  html += "<p>Endereço IP do ESP32: " + WiFi.localIP().toString() + "</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleSend() {
  // Recebe o código IR via GET
  String code = server.arg("code");
  // Converte a string em um array de uint16_t
  Serial.println(code);
  int numValues = 0;
  uint16_t values[100];
  char* token = strtok((char*)code.c_str(), " ");
  while (token != NULL && numValues < 100) {
    values[numValues++] = (uint16_t)strtol(token, NULL, 16);
    token = strtok(NULL, " ");
  }
  for (int i = 0; i < numValues; i++){
    Serial.print(values[i]);
    Serial.print(" ");
  }
  Serial.println("");
  for (int i = 0; i < numValues; i++){
    Serial.print(samsungProntoCode[i]);
    Serial.print(" ");
  }
  Serial.print("Nvalues: ");
  Serial.println(numValues);
  // Envia os pulsos IR
  irsend.sendPronto(values, numValues);
  server.send(200, "text/plain", "Comando IR enviado!");
}

void setup() {
  irsend.begin();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/send", handleSend);

  server.begin();
}

void loop() {
  server.handleClient();
}
