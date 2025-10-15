#ifndef API_H
#define API_H

struct Dados {
    char date[11];
    float tempMin;
    float tempMax;
    int probChuva;
};

extern Dados dados;

void formata_dados(const char *input, Dados *out);

#endif