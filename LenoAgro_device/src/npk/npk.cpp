#include <Arduino.h>
#include "npk.h"


/**
 * Mede nivel de NPK
 */

int medir_N() {
    int nAlto = digitalRead(N_ALTO);
    int nMedio = digitalRead(N_MEDIO);
    int nBaixo = digitalRead(N_BAIXO);

    int nivel = 0;
    
    if (nAlto == 0){
        Serial.print("Nivel N Alto\t");
        return N_ALTO;
    }
    
    if (nMedio == 0){
        Serial.print("Nivel N Medio\t");
        return N_OK;
    }
    
    if (nBaixo == 0){
        Serial.print("Nivel N Baixo\t");
        return BAIXO_N;
    }
    
    return nivel;
}

int medir_P() {
  int pAlto = digitalRead(P_ALTO);
  int pMedio = digitalRead(P_MEDIO);
  int pBaixo = digitalRead(P_BAIXO);
  
  int nivel = 0;

  if (pAlto == 0){
    Serial.print("Nivel P Alto\t");
    return P_ALTO;
  }

  if (pMedio == 0){
    Serial.print("Nivel P Medio\t");
    return P_OK;
  }

  if (pBaixo == 0 ){
    Serial.print("Nivel P Baixo\t");
    return BAIXO_P;
  }
  return nivel;

  
}
void init_npk(){
  pinMode(N_ALTO, INPUT_PULLUP);
  pinMode(N_MEDIO, INPUT_PULLUP);
  pinMode(N_BAIXO, INPUT_PULLUP);

  pinMode(P_ALTO, INPUT_PULLUP);
  pinMode(P_MEDIO, INPUT_PULLUP);
  pinMode(P_BAIXO, INPUT_PULLUP);

  pinMode(K_ALTO, INPUT_PULLUP);
  pinMode(K_MEDIO, INPUT_PULLUP);
  pinMode(K_BAIXO, INPUT_PULLUP);

}

int medir_K() {
    int kAlto = digitalRead(K_ALTO);
    int kMedio = digitalRead(K_MEDIO);
    int kBaixo = digitalRead(K_BAIXO);

    int nivel = 0;

    if (kAlto == 0){
        Serial.println("Nivel K Alto");
        return K_ALTO;
    }
    
    if (kMedio == 0){
        Serial.println("Nivel K Medio");
        return K_OK;
    }
    
    if (kBaixo == 0){
        Serial.println("Nivel K Baixo");
        return BAIXO_K;
    }

    return nivel;
    
}


