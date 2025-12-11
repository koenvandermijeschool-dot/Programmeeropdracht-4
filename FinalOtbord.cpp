#include <iostream>
#include "otbord.h"
#include <fstream>
using namespace std;



// Zet alle pointers van een vakje op nullptr
vakje::vakje() : inhoud('.') {
    for (int i = 0; i < 8; i++) {
        buurvakjes[i] = nullptr;
    }
}

// Haalt inhoud van een vakje op
char vakje::geefInhoud() {
    return inhoud;
}

// Geef een waarde aan de inhoud van een vakje
void vakje::zetInhoud(char c) {
    inhoud = c;
}

// Geef pointer van een specifieke buur
vakje* vakje::geefBuur(int richting) {
    if (richting >= 0 && richting < 8) {
        return buurvakjes[richting];
    }
    return nullptr; // Als het geen buurt is
}

// Geef een waarde aan de pointer van een specifieke buur 
void vakje::zetBuur(int richting, vakje* buurvakje) {
    if (richting >= 0 && richting < 8) {
        buurvakjes[richting] = buurvakje;
    }
}

// Maakt een dubbelverbonden pointerlijst
vakje* vakje::maakRij(int breedte) {
    vakje* eerste = new vakje();
    vakje* huidig = eerste;
    
    for (int i = 1; i < breedte; i++) {
        vakje* volgend = new vakje();
        huidig->zetBuur(2, volgend); // pointer naar rechts (2)
        volgend->zetBuur(6, huidig); // pointer naar links (6)
        huidig = volgend;
    }
    
    return eerste;
}

// Maakt het volledige pointer-bord
vakje* vakje::maakBord(int breedte, int hoogte) {
    // Maakt eerst de bovenste rij
    vakje* eersteRij = maakRij(breedte);
    vakje* huidigRijStart = eersteRij;
    
    //Maak de rest van de rijen en verbind ze verticaal
    for (int rij = 1; rij < hoogte; rij++) {
        vakje* nieuweRij = maakRij(breedte);
        
        vakje* bovenVakje = huidigRijStart;
        vakje* onderVakje = nieuweRij;
        
        for (int kolom = 0; kolom < breedte; kolom++) {
            bovenVakje->zetBuur(4, onderVakje);   // onder (4)
            onderVakje->zetBuur(0, bovenVakje);   // boven (0)
            
            bovenVakje = bovenVakje->geefBuur(2);
            onderVakje = onderVakje->geefBuur(2);
        }
        
        huidigRijStart = nieuweRij;
    }
    
    // Verbind ze diagonaal
    vakje* rijPointer = eersteRij;
    for (int rij = 0; rij < hoogte; rij++) {
        vakje* kolomPointer = rijPointer;
        
        for (int kolom = 0; kolom < breedte; kolom++) {
            // Rechtsboven (1)
            if (kolomPointer->geefBuur(0) != nullptr && 
                kolomPointer->geefBuur(0)->geefBuur(2) != nullptr) {
                kolomPointer->zetBuur(1, kolomPointer->geefBuur(0)->geefBuur(2));
            }
            
            // Rechtsonder (3)
            if (kolomPointer->geefBuur(4) != nullptr && 
                kolomPointer->geefBuur(4)->geefBuur(2) != nullptr) {
                kolomPointer->zetBuur(3, kolomPointer->geefBuur(4)->geefBuur(2));
            }
            
            // Linksonder (5)
            if (kolomPointer->geefBuur(4) != nullptr && 
                kolomPointer->geefBuur(4)->geefBuur(6) != nullptr) {
                kolomPointer->zetBuur(5, kolomPointer->geefBuur(4)->geefBuur(6));
            }
            
            // Linksboven (7)
            if (kolomPointer->geefBuur(0) != nullptr && 
                kolomPointer->geefBuur(0)->geefBuur(6) != nullptr) {
                kolomPointer->zetBuur(7, kolomPointer->geefBuur(0)->geefBuur(6));
            }
            
            kolomPointer = kolomPointer->geefBuur(2);
        }
        
        rijPointer = rijPointer->geefBuur(4);
    }
    
    // Maak beginpositie van zwart en wit
    int midRij = hoogte / 2;
    int midKolom = breedte / 2;
    
    vakje* midden = eersteRij;
    for (int i = 0; i < midRij - 1; i++) {
        midden = midden->geefBuur(4);
    }
    for (int i = 0; i < midKolom - 1; i++) {
        midden = midden->geefBuur(2);
    }
    
    midden->zetInhoud('W');
    midden->geefBuur(2)->zetInhoud('Z');
    midden->geefBuur(4)->zetInhoud('Z');
    midden->geefBuur(4)->geefBuur(2)->zetInhoud('W');
    
    return eersteRij;
}

// Ruimt het hele bord op
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


OthelloBord::OthelloBord(int b, int h) : breedte(b), hoogte(h), huidigeSpeler('Z'), witComputer(false), zwartComputer(false), nietafdrukken(false) {
    vakje v;
    linksboven = v.maakBord(breedte, hoogte);
}

// Destructor van OthelloBord
OthelloBord::~OthelloBord() {
    vakje v;
    v.verwijderBord(linksboven);
    
    //Ruim alle zetten in de stack op
    while (!zetGeschiedenis.empty()) {
        Zet* zet = zetGeschiedenis.top();
        zetGeschiedenis.pop();
        delete zet;
    }}

// Afdrukken van het bord
void OthelloBord::afdrukken() {
    // Print kolom letters bovenaan
    char letterKolom = 'A'; //ASCII waarde van A
    cout << "  ";
    for (int kolom = 0; kolom < breedte; kolom++) {
        char letter = (char)letterKolom+kolom; //ASCII waarde omzetten naar letter en 1 verhogen
        cout << " " << (letter);
    }
    cout << endl;
    
    vakje* rijPointer = linksboven;
    
    // Print rij nummers
    for (int rij = 0; rij < hoogte; rij++) {
        cout << (rij + 1) << " ";
        if (rij + 1 < 10) {
            cout << " ";
        }
        
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

// Geeft de huidige speler terug
char OthelloBord::geefHuidigeSpeler() {
    return huidigeSpeler;
}

// Wisselt van speler
void OthelloBord::wisselSpeler() {
    if (huidigeSpeler == 'Z') {
        huidigeSpeler = 'W';
    }
    else{
        huidigeSpeler = 'Z';
    }
}


// Vindt een specifiek vakje op het bord op basis van rij en kolom
vakje* OthelloBord::vindVakje(int rij, int kolom) {
    if (rij < 0 || rij >= hoogte || kolom < 0 || kolom >= breedte) {
        return nullptr;
    }
    
    vakje* huidig = linksboven;
    
    // Ga naar de juiste rij
    for (int i = 0; i < rij; i++) {
        huidig = huidig->geefBuur(4); // naar beneden
    }
    
    // Ga naar de juiste kolom
    for (int i = 0; i < kolom; i++) {
        huidig = huidig->geefBuur(2); // naar rechts
    }
    
    return huidig;
}

// Telt hoeveel stenen omgeslagen kunnen worden in een bepaalde richting
int OthelloBord::telOmslaanInRichting(vakje* v, int richting, char speler) {
    char tegenstander;
    if (speler == 'W') {
        tegenstander = 'Z';
    }
    else{
        tegenstander = 'W';
    }

    vakje* huidig = v->geefBuur(richting);
    int aantal = 0;
    
    // Tel tegenstander stenen
    while (huidig != nullptr && huidig->geefInhoud() == tegenstander) {
        aantal++;
        huidig = huidig->geefBuur(richting);
    }
    
    // Check of er een eigen steen aan het einde is
    if (huidig != nullptr && huidig->geefInhoud() == speler && aantal > 0) {
        return aantal;
    }
    
    return 0;
}

// Controleert of een zet geldig is voor een bepaalde speler
bool OthelloBord::isZetGeldig(vakje* v, char speler) {
    // Vakje moet leeg zijn
    if (v->geefInhoud() != '.') {
        return false;
    }
    
    // Check alle 8 richtingen
    for (int richting = 0; richting < 8; richting++) {
        if (telOmslaanInRichting(v, richting, speler) > 0) {
            return true;
        }
    }
    
    return false;
}

// Draait stenen om in een bepaalde richting
void OthelloBord::slaOmInRichting(vakje* v, int richting, char speler, int aantal) {
    vakje* huidig = v->geefBuur(richting);
    
    for (int i = 0; i < aantal; i++) {
        huidig->zetInhoud(speler);
        huidig = huidig->geefBuur(richting);
    }
}

// Voert een zet uit op het bord
bool OthelloBord::doeZet(char kolom, int rij) {
    int kolomIndex = kolom - 'A';
    int rijIndex = rij - 1; 
    
    vakje* v = vindVakje(rijIndex, kolomIndex);
    
    if (v == nullptr) {
        cout << "Ongeldige positie!" << endl;
        return false;
    }
    
    if (!isZetGeldig(v, huidigeSpeler)) {
        cout << "Ongeldige zet! Je moet minimaal 1 steen draaien." << endl;
        return false;
    }
    
    // Sla het bord op
    int bordGrootte = breedte * hoogte;
    char* bordsituatie = new char[bordGrootte];
    slaOpBordsituatie(bordsituatie);
    
    Zet* nieuweZet = new Zet(huidigeSpeler, bordsituatie, bordGrootte);
    zetGeschiedenis.push(nieuweZet);
    
    delete[] bordsituatie;
    
    // Plaats de steen
    v->zetInhoud(huidigeSpeler);
    
    // Draai stenen om in alle geldige richtingen
    for (int richting = 0; richting < 8; richting++) {
        int aantal = telOmslaanInRichting(v, richting, huidigeSpeler);
        if (aantal > 0) {
            slaOmInRichting(v, richting, huidigeSpeler, aantal);
        }
    }
    if (!nietafdrukken){
        cout << "Zet uitgevoerd!" << endl;
    }
    
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
    int geldigeZetten[100][2];  // Max 100 vakjes op 10x10 bord
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
    
    // Als er geen enkele geldige zet is
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

// Zet witComputer op True of False
void OthelloBord::zetWitComputer(bool isComputer) {
    witComputer = isComputer;
}

// Zet zwartComputer op True of False
void OthelloBord::zetZwartComputer(bool isComputer) {
    zwartComputer = isComputer;
}

// Geeft terug of wit een computer is
bool OthelloBord::getWitComputer(){
    return witComputer;
}

// Geeft terug of zwart een computer is
bool OthelloBord::getZwartComputer(){
    return zwartComputer;
}

// Zet nietafdrukken op True of False
void OthelloBord::zetNietAfdrukken(bool afdrukken) {
    nietafdrukken = afdrukken;
}

// Geeft terug of de huidige speler een computer is of niet
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

// Reset het bord naar de beginpositie door alles te verwijderen van het vorige spel
void OthelloBord::resetBord() {
    vakje v;
    v.verwijderBord(linksboven);
    
    linksboven = v.maakBord(breedte, hoogte);
    huidigeSpeler = 'Z';
    
    // Wis is de zetgeschiedenis
    while (!zetGeschiedenis.empty()) {
        Zet* zet = zetGeschiedenis.top();
        zetGeschiedenis.pop();
        delete zet;
    }}

// Speelt meerdere spellen en houdt statistieken bij
void OthelloBord::speelMeerdereSpellen(int aantalSpellen) {
    int zwartWinsten = 0;
    int witWinsten = 0;
    int gelijkspellen = 0;

    ofstream bestand("output.txt");
    if (!bestand) {
        cout << "Kon bestand niet openen!" << endl;
}
    
    // Zorgt dat het niet afgedrukt wordt als er meer dan 1 spel is
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
            // Check of de computer die aan zet is nog zetten heeft
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
            // Beurt voorbij
            wisselSpeler();
        }
        
        // Telt het aantal stenen van beide kleuren
        int aantalZwart = telStenen('Z');
        int aantalWit = telStenen('W');
        
        // Print de eindstand als het 1 spel was
        if (!nietafdrukken) {
            cout << "\nSpel " << spelNummer << " afgelopen!" << endl;
            cout << "Zwart: " << aantalZwart << " - Wit: " << aantalWit << endl;
        }
        
        // Print wie de winnaar is indien nodig en voegt dit toe in de statistieken
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
    
    // Zet stille mode weer uit na dat de partijen tussen de computers gespeeld zijn
    zetNietAfdrukken(false);
    
    // Print de totaalstatistieken
    cout << "=== TOTAALSTATISTIEKEN ===" << endl;
    cout << "Totaal aantal spellen: " << aantalSpellen << endl;
    cout << "Zwart gewonnen: " << zwartWinsten << endl;
    cout << "Wit gewonnen: " << witWinsten << endl;
    cout << "Gelijkspel: " << gelijkspellen << endl;
         
    bestand.close();
}

// Sla het hele bord op
void OthelloBord::slaOpBordsituatie(char* buffer) {
    int index = 0;
    for (int rij = 0; rij < hoogte; rij++) {
        for (int kolom = 0; kolom < breedte; kolom++) {
            vakje* v = vindVakje(rij, kolom);
            buffer[index++] = v->geefInhoud();
        }
    }
}

// Herstelt een bordsituatie met behulp van een gegeven buffer
void OthelloBord::herstelBordsituatie(char* buffer) {
    int index = 0;
    for (int rij = 0; rij < hoogte; rij++) {
        for (int kolom = 0; kolom < breedte; kolom++) {
            vakje* v = vindVakje(rij, kolom);
            v->zetInhoud(buffer[index++]); // Geeft het vakje de waarde uit de buffer
        }
    }
}

// Geeft het aantal zetten dat ongedaan gemaakt kan worden
int OthelloBord::geefAantalOngedaanMaken() {
    return zetGeschiedenis.size();
}

// Maak de laatste zet ongedaan
bool OthelloBord::undoZet() {
    // Check hoeveel spelers computers zijn
    bool tegenComputer = (witComputer && !zwartComputer) || (!witComputer && zwartComputer);
    int aantalOngedaan;
    if (tegenComputer == True) { // 2 zetten als je tegen computer speelt
        aantalOngedaan = 2;
    } 
    else{
        aantalOngedaan = 1; // 1 zet als je tegen een persoon speelt
    }
    
    // Check of er genoeg zetten zijn om ongedaan te doen
    if (zetGeschiedenis.size() < aantalOngedaan) {
        cout << "Niet genoeg zetten om ongedaan te maken!" << endl;
        return false;
    }
    
    // Maak het juiste aantal zetten ongedaan
    for (int i = 0; i < aantalOngedaan; i++) {
        if (zetGeschiedenis.empty()) {
            break;
        }
        
        Zet* laatsteZet = zetGeschiedenis.top();
        zetGeschiedenis.pop();
        
        // Herstel de bordsituatie
        herstelBordsituatie(laatsteZet->bordsituatie);
        
        // Herstel de speler
        huidigeSpeler = laatsteZet->speler;
        
        delete laatsteZet;
    }
    
    if (tegenComputer) {
        cout << "Laatste 2 zetten ongedaan gemaakt!" << endl;
    } else {
        cout << "Zet ongedaan gemaakt!" << endl;
    }
    cout << "Speler " << huidigeSpeler << " is aan de beurt." << endl;
    
    return true;
}
