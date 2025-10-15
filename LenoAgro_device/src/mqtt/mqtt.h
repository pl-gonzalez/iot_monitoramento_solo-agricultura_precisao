#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>


struct Dados{
    String date;
    float tempMin;
    float tempMax;
    int probChuva;
    bool atualizado;
};

extern WiFiClient espClient;
extern PubSubClient client;

extern struct Dados dados;



void init_mqtt();
void mqtt_loop();
void reconnect();
String formata_dados(const char* id, const char* cultura, float temperatura, float umidade, float ph_solo, const char* npk);
void publica_mqtt(const char* msg);

#endif