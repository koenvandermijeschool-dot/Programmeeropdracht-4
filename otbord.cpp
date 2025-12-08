#include <iostream>
#include "otbord.h"
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

// --- vakje implementatie ---

vakje::vakje() : inhoud('.') {
    for (int i = 0; i < 8; i++) {
        buurvakjes[i] = nullptr;
    }
}

char vakje::geefInhoud() { return inhoud; }
void vakje::zetInhoud(char c) { inhoud = c; }

vakje* vakje::geefBuur(int richting) {
    if (richting >= 0 && richting < 8) return buurvakjes[richting];
    return nullptr;
}

void vakje::zetBuur(int richting, vakje* buurvakje) {
    if (richting >= 0 && richting < 8) buurvakjes[richting] = buurvakje;
}

vakje* vakje::maakRij(int breedte) {
    vakje* eerste = new vakje();
    vakje* huidig = eerste;
    for (int i = 1; i < breedte; i++) {
        vakje* volgend = new vakje();
        huidig->zetBuur(2, volgend);
        volgend->zetBuur(6, huidig);
        huidig = volgend;
    }
    return eerste;
}

vakje* vakje::maakBord(int breedte, int hoogte) {
    vakje* eersteRij = maakRij(breedte);
    vakje* huidigRijStart = eersteRij;

    for (int rij = 1; rij < hoogte; rij++) {
        vakje* nieuweRij = maakRij(breedte);
        vakje* boven = huidigRijStart;
        vakje* onder = nieuweRij;

        for (int kolom = 0; kolom < breedte; kolom++) {
            boven->zetBuur(4, onder);
            onder->zetBuur(0, boven);
            boven = boven->geefBuur(2);
            onder = onder->geefBuur(2);
        }
        huidigRijStart = nieuweRij;
    }

    // diagonaal instellen
    vakje* rijPointer = eersteRij;
    for (int rij = 0; rij < hoogte; rij++) {
        vakje* kolomPointer = rijPointer;
        for (int kolom = 0; kolom < breedte; kolom++) {
            if (kolomPointer->geefBuur(0) != nullptr && kolomPointer->geefBuur(0)->geefBuur(2) != nullptr)
                kolomPointer->zetBuur(1, kolomPointer->geefBuur(0)->geefBuur(2));

            if (kolomPointer->geefBuur(4) != nullptr) {
                if (kolomPointer->geefBuur(4)->geefBuur(2) != nullptr)
                    kolomPointer->zetBuur(3, kolomPointer->geefBuur(4)->geefBuur(2));
                if (kolomPointer->geefBuur(4)->geefBuur(6) != nullptr)
                    kolomPointer->zetBuur(5, kolomPointer->geefBuur(4)->geefBuur(6));
            }

            if (kolomPointer->geefBuur(0) != nullptr && kolomPointer->geefBuur(0)->geefBuur(6) != nullptr)
                kolomPointer->zetBuur(7, kolomPointer->geefBuur(0)->geefBuur(6));

            kolomPointer = kolomPointer->geefBuur(2);
        }
        rijPointer = rijPointer->geefBuur(4);
    }

    // beginpositie
    int midRij = hoogte / 2;
    int midKolom = breedte / 2;
    vakje* midden = eersteRij;
    for (int i = 0; i < midRij - 1; i++) midden = midden->geefBuur(4);
    for (int i = 0; i < midKolom - 1; i++) midden = midden->geefBuur(2);

    midden->zetInhoud('W');
    midden->geefBuur(2)->zetInhoud('Z');
    midden->geefBuur(4)->zetInhoud('Z');
    midden->geefBuur(4)->geefBuur(2)->zetInhoud('W');

    return eersteRij;
}

void vakje::verwijderBord(vakje* linksboven) {
    vakje* rijPointer = linksboven;
    while (rijPointer != nullptr) {
        vakje* volgendeRij = rijPointer->geefBuur(4);
        vakje* kolomPointer = rijPointer;
        while (kolomPointer != nullptr) {
            vakje* volgendeKolom = kolomPointer->geefBuur(2);
            delete kolomPointer;
            kolomPointer = volgendeKolom;
        }
        rijPointer = volgendeRij;
    }
}

// --- OthelloBord implementatie ---

OthelloBord::OthelloBord(int b, int h)
    : breedte(b), hoogte(h), huidigeSpeler('Z'), witComputer(false), zwartComputer(false), nietafdrukken(false) {
    vakje v;
    linksboven = v.maakBord(breedte, hoogte);
}

OthelloBord::~OthelloBord() {
    vakje v;
    v.verwijderBord(linksboven);
}

// Afdrukken van het bord
void OthelloBord::afdrukken() {
    cout << "  ";
    for (int kolom = 0; kolom < breedte; kolom++) cout << " " << (char)('A' + kolom);
    cout << endl;

    vakje* rijPointer = linksboven;
    for (int rij = 0; rij < hoogte; rij++) {
        cout << (rij + 1);
        if (rij + 1 < 10) cout << " ";
        cout << " ";
        vakje* kolomPointer = rijPointer;
        for (int kolom = 0; kolom < breedte; kolom++) {
            cout << kolomPointer->geefInhoud() << " ";
            kolomPointer = kolomPointer->geefBuur(2);
        }
        cout << endl;
        rijPointer = rijPointer->geefBuur(4);
    }
    cout << endl;
}

// Vind vakje
vakje* OthelloBord::vindVakje(int rij, int kolom) {
    if (rij < 0 || rij >= hoogte || kolom < 0 || kolom >= breedte) return nullptr;
    vakje* huidig = linksboven;
    for (int i = 0; i < rij; i++) huidig = huidig->geefBuur(4);
    for (int i = 0; i < kolom; i++) huidig = huidig->geefBuur(2);
    return huidig;
}

// Zet logica
int OthelloBord::telOmslaanInRichting(vakje* v, int richting, char speler) {
    char tegenstander = (speler == 'Z') ? 'W' : 'Z';
    vakje* huidig = v->geefBuur(richting);
    int aantal = 0;
    while (huidig != nullptr && huidig->geefInhoud() == tegenstander) {
        aantal++;
        huidig = huidig->geefBuur(richting);
    }
    if (huidig != nullptr && huidig->geefInhoud() == speler && aantal > 0) return aantal;
    return 0;
}

bool OthelloBord::isZetGeldig(vakje* v, char speler) {
    if (v->geefInhoud() != '.') return false;
    for (int richting = 0; richting < 8; richting++)
        if (telOmslaanInRichting(v, richting, speler) > 0) return true;
    return false;
}

void OthelloBord::slaOmInRichting(vakje* v, int richting, char speler, int aantal) {
    vakje* huidig = v->geefBuur(richting);
    for (int i = 0; i < aantal; i++) {
        huidig->zetInhoud(speler);
        huidig = huidig->geefBuur(richting);
    }
}

// doeZet met print-optie
bool OthelloBord::doeZet(char kolom, int rij, bool print) {
    int kolomIndex = kolom - 'A';
    int rijIndex = rij - 1;
    vakje* v = vindVakje(rijIndex, kolomIndex);
    if (v == nullptr) {
        if (print) cout << "Ongeldige positie!" << endl;
        return false;
    }
    if (!isZetGeldig(v, huidigeSpeler)) {
        if (print) cout << "Ongeldige zet! Je moet minimaal 1 steen omslaan." << endl;
        return false;
    }
    v->zetInhoud(huidigeSpeler);
    for (int richting = 0; richting < 8; richting++) {
        int aantal = telOmslaanInRichting(v, richting, huidigeSpeler);
        if (aantal > 0) slaOmInRichting(v, richting, huidigeSpeler, aantal);
    }
    if (print && !nietafdrukken) cout << "Zet uitgevoerd!" << endl;
    return true;
}

// --- Undo functionaliteit ---

OthelloBord OthelloBord::deepCopy() {
    OthelloBord kopie(breedte, hoogte);
    vakje* rijOrig = linksboven;
    vakje* rijKopie = kopie.linksboven;
    for (int i = 0; i < hoogte; i++) {
        vakje* kolOrig = rijOrig;
        vakje* kolKopie = rijKopie;
        for (int j = 0; j < breedte; j++) {
            kolKopie->zetInhoud(kolOrig->geefInhoud());
            kolOrig = kolOrig->geefBuur(2);
            kolKopie = kolKopie->geefBuur(2);
        }
        rijOrig = rijOrig->geefBuur(4);
        rijKopie = rijKopie->geefBuur(4);
    }
    kopie.huidigeSpeler = huidigeSpeler;
    kopie.witComputer = witComputer;
    kopie.zwartComputer = zwartComputer;
    kopie.nietafdrukken = nietafdrukken;
    return kopie;
}

void OthelloBord::pushSnapshot() {
    snapshots.push_back(deepCopy());
}

bool OthelloBord::undo() {
    if (snapshots.empty()) return false;

    OthelloBord laatste = snapshots.back();
    snapshots.pop_back();

    // Verwijder huidige bord
    vakje v;
    v.verwijderBord(linksboven);

    // Zet alles terug van snapshot
    linksboven = laatste.linksboven;
    breedte = laatste.breedte;
    hoogte = laatste.hoogte;
    huidigeSpeler = laatste.huidigeSpeler;
    witComputer = laatste.witComputer;
    zwartComputer = laatste.zwartComputer;
    nietafdrukken = laatste.nietafdrukken;

    // Zorg dat snapshot object de pointers niet meer delete bij destructor
    laatste.linksboven = nullptr;

    return true;
}


// Controleert of een speler geldige zetten heeft
bool OthelloBord::heeftGeldigeZetten(char speler) {
    for (int rij = 0; rij < hoogte; rij++) {
        for (int kolom = 0; kolom < breedte; kolom++) {
            vakje* v = vindVakje(rij, kolom);
            if (v != nullptr && isZetGeldig(v, speler)) {
                return true;
            }
        }
    }
    return false;
}

// Doet een random geldige zet voor de computer
void OthelloBord::doeComputerZet() {
    // Verzamel alle geldige zetten
    int geldigeZetten[64][2];  // Max 64 vakjes op 8x8 bord
    int aantalGeldig = 0;
    
    for (int rij = 0; rij < hoogte; rij++) {
        for (int kolom = 0; kolom < breedte; kolom++) {
            vakje* v = vindVakje(rij, kolom);
            if (v != nullptr && isZetGeldig(v, huidigeSpeler)) {
                geldigeZetten[aantalGeldig][0] = rij;
                geldigeZetten[aantalGeldig][1] = kolom;
                aantalGeldig++;
            }
        }
    }
    
    if (aantalGeldig == 0) {
        cout << "Computer heeft geen geldige zetten!" << endl;
        return;
    }
    
    // Kies een random zet
    int keuze = rand() % aantalGeldig;
    int gekozenRij = geldigeZetten[keuze][0];
    int gekozenKolom = geldigeZetten[keuze][1];
    
    char kolomLetter = 'A' + gekozenKolom;
    int rijNummer = gekozenRij + 1;
    if (!nietafdrukken){
        cout << "Computer (" << huidigeSpeler << ") doet zet: " 
         << kolomLetter << rijNummer << endl;
    }
    
    
    doeZet(kolomLetter, rijNummer);
    if (!nietafdrukken){
        afdrukken();
    }
    
}

void OthelloBord::zetWitComputer(bool isComputer) {
    witComputer = isComputer;
}

void OthelloBord::zetZwartComputer(bool isComputer) {
    zwartComputer = isComputer;
}

bool OthelloBord::getWitComputer(){
    return witComputer;
}
bool OthelloBord::getZwartComputer(){
    return zwartComputer;
}

void OthelloBord::zetNietAfdrukken(bool nietAfdrukken) {
    nietafdrukken = nietAfdrukken;
}

bool OthelloBord::isHuidigeSpelerComputer() {
    if (huidigeSpeler == 'W') {
        return witComputer;
    } else {
        return zwartComputer;
    }
}

// Controleert of het spel afgelopen is (geen geldige zetten meer voor beide spelers)
bool OthelloBord::isSpelAfgelopen() {
    return !heeftGeldigeZetten('Z') && !heeftGeldigeZetten('W');
}

// Telt het aantal stenen van een bepaalde speler
int OthelloBord::telStenen(char speler) {
    int aantal = 0;
    
    for (int rij = 0; rij < hoogte; rij++) {
        for (int kolom = 0; kolom < breedte; kolom++) {
            vakje* v = vindVakje(rij, kolom);
            if (v != nullptr && v->geefInhoud() == speler) {
                aantal++;
            }
        }
    }
    
    return aantal;
}

// Toont de eindstand en de winnaar
void OthelloBord::toonEindstand() {
    int aantalZwart = telStenen('Z');
    int aantalWit = telStenen('W');
    
    cout << endl;
    cout << "=== SPEL AFGELOPEN ===" << endl;
    cout << "Eindstand:" << endl;
    cout << "Zwart (Z): " << aantalZwart << " stenen" << endl;
    cout << "Wit (W): " << aantalWit << " stenen" << endl;
    cout << endl;
    
    if (aantalZwart > aantalWit) {
        cout << "*** ZWART WINT! ***" << endl;
    } else if (aantalWit > aantalZwart) {
        cout << "*** WIT WINT! ***" << endl;
    } else {
        cout << "*** GELIJKSPEL! ***" << endl;
    }
    cout << endl;
}

// Reset het bord naar de beginpositie
void OthelloBord::resetBord() {
    // Verwijder het oude bord
    vakje v;
    v.verwijderBord(linksboven);
    
    // Maak een nieuw bord
    linksboven = v.maakBord(breedte, hoogte);
    
    // Reset de huidige speler naar zwart
    huidigeSpeler = 'Z';
}

// Speelt meerdere spellen en houdt statistieken bij
void OthelloBord::speelMeerdereSpellen(int aantalSpellen) {
    int zwartWinsten = 0;
    int witWinsten = 0;
    int gelijkspellen = 0;

    ofstream bestand("output.txt");
    if (!bestand) {
        cout << "Kon bestand niet openen!" << endl;
}
    
    // Zet stille mode aan als er meer dan 1 spel is
    if (aantalSpellen > 1) {
        zetNietAfdrukken(true);
    }
    
    for (int spelNummer = 1; spelNummer <= aantalSpellen; spelNummer++) {
        if (!nietafdrukken) {
            cout << "\n=== SPEL " << spelNummer << " VAN " << aantalSpellen << " ===" << endl;
        }
        
        // Reset het bord voor een nieuw spel
        if (spelNummer > 1) {
            resetBord();
        }
        
        // Speel het spel
        while (!isSpelAfgelopen()) {
            // Check of huidige speler geen zetten heeft
            if (!heeftGeldigeZetten(huidigeSpeler)) {
                if (!nietafdrukken) {
                    cout << "Speler " << huidigeSpeler 
                         << " heeft geen geldige zetten. Beurt wordt overgeslagen." << endl;
                }
                wisselSpeler();
                continue;
            }
            
            // Computer doet een zet
            doeComputerZet();
            wisselSpeler();
        }
        
    
        int aantalZwart = telStenen('Z');
        int aantalWit = telStenen('W');
        
        if (!nietafdrukken) {
            cout << "\nSpel " << spelNummer << " afgelopen!" << endl;
            cout << "Zwart: " << aantalZwart << " - Wit: " << aantalWit << endl;
        }
        
        if (aantalZwart > aantalWit) {
            if (!nietafdrukken) cout << "Winnaar: ZWART" << endl;
            zwartWinsten++;
        } else if (aantalWit > aantalZwart) {
            if (!nietafdrukken) cout << "Winnaar: WIT" << endl;
            witWinsten++;
        } else {
            if (!nietafdrukken) cout << "GELIJKSPEL" << endl;
            gelijkspellen++;
        }
        bestand << spelNummer << " " << zwartWinsten << " " << witWinsten << " " << gelijkspellen << "\n";
        

        
    
    }
    
    // Zet stille mode weer uit
    zetNietAfdrukken(false);
    
    cout << "=== TOTAALSTATISTIEKEN ===" << endl;
    cout << "Totaal aantal spellen: " << aantalSpellen << endl;
    cout << "Zwart gewonnen: " << zwartWinsten << endl;
    cout << "Wit gewonnen: " << witWinsten << endl;
    cout << "Gelijkspel: " << gelijkspellen << endl;
         
    bestand.close();
}

void OthelloBord::doeZetZonderPrint(char kolom, int rij, char speler) {
    char huidig = huidigeSpeler;
    huidigeSpeler = speler;
    doeZet(kolom, rij, false);
    huidigeSpeler = huidig;
}

char OthelloBord::geefHuidigeSpeler() {
    return huidigeSpeler;
}

void OthelloBord::wisselSpeler() {
    huidigeSpeler = (huidigeSpeler == 'Z') ? 'W' : 'Z';
}

// Getter breedte/hoogte
int OthelloBord::geefBreedte() { 
    return breedte; 
}
int OthelloBord::geefHoogte() { 
    return hoogte; 
}

