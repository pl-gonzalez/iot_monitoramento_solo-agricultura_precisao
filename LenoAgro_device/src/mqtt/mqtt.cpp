#include <Arduino.h>
#include <string.h>
#include <stdlib.h>

#include <WiFi.h>
#include <PubSubClient.h>

#include "mqtt.h"

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

const char* MQTT_SERVER = "mqtt.flespi.io";
const int   MQTT_PORT = 1883;

// Token do Flespi.io
const char* MQTT_USER = "FlespiToken FLESPI_TOKEN";
const char* MQTT_PASS = "";

const char* MQTT_TOPIC = "central";



WiFiClient espClient;
PubSubClient client(espClient);

void mqtt_loop(){
    if (!client.connected()) {
        reconnect();
    }
    client.loop(); // mantém conexão e recebe mensagens

    
}


String formata_dados(const char* id, const char* cultura, float temperatura, float umidade, float ph_solo, const char* npk) {
    String mensagem =   String(id) + ";" +
                        String(cultura) + ";" +
                        String(temperatura) + ";" +
                        String(umidade) + ";" +
                        String(ph_solo) + ";" +
                        String(npk);

    return mensagem;
}


void callback(char* topic, byte* payload, unsigned int length) {

    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    // int p1 = message.indexOf(';');
    // int p2 = message.indexOf(';', p1 + 1);
    // int p3 = message.indexOf(';', p2 + 1);  
    // dados.date = message.substring(0, p1);
    // dados.tempMin = message.substring(p1 + 1, p2).toFloat();
    // dados.tempMax = message.substring(p2 + 1, p3).toFloat();
    // dados.probChuva = message.substring(p3 + 1).toInt();
    // dados.atualizado = true;
}

void publica_mqtt(const char* msg){
    client.publish(MQTT_TOPIC, msg);
    Serial.println("Msg Enviada");
}


void reconnect() {
  while (!client.connected()) {

    Serial.print("Conectando ao broker MQTT... ");
    if (client.connect("ESP32Client", MQTT_USER, MQTT_PASS)) {
        Serial.print("Conectado!\t");
        client.subscribe(MQTT_TOPIC);
        Serial.print("Inscrito no tópico: ");
        Serial.println(MQTT_TOPIC);
    } 
    else {
        Serial.print("Falha, rc=");
        Serial.print(client.state());
        Serial.println(" — Tentando novamente em 5s");
        delay(5000);
    }
  }
}


void init_mqtt(){

    Serial.println("Conectando ao Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);


    Serial.println("\nConectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);
}

