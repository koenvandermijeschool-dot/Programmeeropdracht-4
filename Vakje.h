#ifndef VAKJE_H
#define VAKJE_H

class vakje {
private:
    char inhoud;
    vakje* buurvakjes[8]; // 0=N, 1=NE, 2=E, 3=SE, 4=S, 5=SW, 6=W, 7=NW

public:
    vakje();

    char geefInhoud();
    void zetInhoud(char c);

    vakje* geefBuur(int richting);
    void zetBuur(int richting, vakje* buurvakje);

    vakje* maakRij(int breedte);
    vakje* maakBord(int breedte, int hoogte);

    void verwijderBord(vakje* linksboven);
};

#endif
