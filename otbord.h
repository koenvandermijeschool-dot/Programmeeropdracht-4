#ifndef OTBORD_H
#define OTBORD_H

#include <vector>
#include "Vakje.h"

class OthelloBord {
private:
    vakje* linksboven;
    int breedte;
    int hoogte;
    char huidigeSpeler;  // 'W' of 'Z'
    bool witComputer;
    bool zwartComputer;
    bool nietafdrukken;
    
    std::vector<OthelloBord> snapshots;  // sla hele bord objecten op

public:
    OthelloBord(int b, int h);
    ~OthelloBord();

    void afdrukken();
    bool doeZet(char kolom, int rij, bool print = true);
    char geefHuidigeSpeler();
    void wisselSpeler();
    vakje* vindVakje(int rij, int kolom);
    bool isZetGeldig(vakje* v, char speler);
    int telOmslaanInRichting(vakje* v, int richting, char speler);
    void slaOmInRichting(vakje* v, int richting, char speler, int aantal);

    bool heeftGeldigeZetten(char speler);
    void doeComputerZet();
    void zetWitComputer(bool isComputer);
    void zetZwartComputer(bool isComputer);
    bool isHuidigeSpelerComputer();
    bool isSpelAfgelopen();
    int telStenen(char speler);
    void toonEindstand();
    bool getWitComputer();
    bool getZwartComputer();
    void speelMeerdereSpellen(int aantalSpellen);
    void resetBord();
    void zetNietAfdrukken(bool a);

    // Undo / snapshot functies
    void doeZetZonderPrint(char kolom, int rij, char speler);
    OthelloBord deepCopy();
    void pushSnapshot();
    bool undo();

    int geefBreedte();
    int geefHoogte();
};

void menu();

#endif
