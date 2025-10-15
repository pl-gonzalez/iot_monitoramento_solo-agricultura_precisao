#include <Arduino.h>
#include <WiFi.h>

#include "DHTesp.h"

#include "npk/npk.h"
#include "pH/ph.h"
#include "dht/dht.h"
#include "mqtt/mqtt.h"

/**
 * ID e cultura serão dados inseridos no código por enquanto
 */
#define ID "2602"
#define CULTURA "cana-de-açucar"

DHTesp dhtSensor;

unsigned long lastUpdate = 0;
const unsigned long interval = 20000; // 20 segundos

void setup() {
  Serial.begin(115200);

  dhtSensor.setup(DHT_PIN, DHTesp :: DHT22);
  init_mqtt();
  init_npk();
  init_ph();
  
  delay(3000);
  
}


void loop() {

  if (!client.connected()) {
        reconnect();
    }
    client.loop(); // mantém conexão e recebe mensagens

  // executa tarefas periodicamente sem travar o loop
    unsigned long now = millis();
    if (now - lastUpdate >= interval) {
        lastUpdate = now;
        
        float temperatura = temp_umidade().temperature;
        float umidade = temp_umidade().humidity;
        float ph_solo = medir_ph_solo();
        
        
        // Mediçoes de nivel de NPK retornam o estado de cada sensor
        int nivel_n = medir_N();
        int nivel_p = medir_P();
        int nivel_k = medir_K();
        
            
            if (nivel_n == BAIXO_N && nivel_p == BAIXO_P  && nivel_k == BAIXO_K) {
              String msg = formata_dados(ID, CULTURA, temperatura, umidade, ph_solo, "NPK");

              publica_mqtt(msg.c_str());
            }
            // se n p
            else if (nivel_n == BAIXO_N && nivel_p == BAIXO_P){
              String msg = formata_dados(ID, CULTURA, temperatura, umidade, ph_solo, "NP");

              publica_mqtt(msg.c_str());
            }
            // se n k
            else if (nivel_n == BAIXO_N && nivel_k == BAIXO_K){ 
              String msg = formata_dados(ID, CULTURA, temperatura, umidade, ph_solo, "NK");

              publica_mqtt(msg.c_str());
            }
            // se p k
            else if (nivel_p == BAIXO_P && nivel_k == BAIXO_K){ 
              String msg = formata_dados(ID, CULTURA, temperatura, umidade, ph_solo, "PK");

              publica_mqtt(msg.c_str());
            }
            // se n
            else if (nivel_n == BAIXO_N){ 
              String msg = formata_dados(ID, CULTURA, temperatura, umidade, ph_solo, "N");

              publica_mqtt(msg.c_str());
            }
            // se p
            else if (nivel_p == BAIXO_P){ 
              String msg = formata_dados(ID, CULTURA, temperatura, umidade, ph_solo, "P");

              publica_mqtt(msg.c_str());
            }
            // se k
            else if (nivel_k == BAIXO_K){ 
              String msg = formata_dados(ID, CULTURA, temperatura, umidade, ph_solo, "K");
              
              publica_mqtt(msg.c_str());
            }
            
            else if (ph_solo < 5.5f){ 
              String msg = formata_dados(ID, CULTURA, temperatura, umidade, ph_solo, "Cal");
              
              publica_mqtt(msg.c_str());
            }
            
            else {
              String msg = formata_dados(ID, CULTURA, temperatura, umidade, ph_solo, "OK");
              
              publica_mqtt(msg.c_str());
            }
            
          }
        }
          
          
          