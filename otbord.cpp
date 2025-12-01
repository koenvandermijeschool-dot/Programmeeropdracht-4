#include <iostream>
#include "otbord.h"
using namespace std;

// Zet alle pointers van een vakje op nullptr (constructor)
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
    return nullptr;
}

// Geef een waarde aan de pointer van een specifieke buur 
void vakje::zetBuur(int richting, vakje* buurvakje) {
    if (richting >= 0 && richting < 8) {
        buurvakjes[richting] = buurvakje;
    }
}

// Maakt een dubbelverbonden pointerlijst (1 rij van het bord)
vakje* vakje::maakRij(int breedte) {
    vakje* eerste = new vakje();
    vakje* huidig = eerste;
    
    for (int i = 1; i < breedte; i++) {
        vakje* volgend = new vakje();
        huidig->zetBuur(2, volgend);      // pointer naar rechts
        volgend->zetBuur(6, huidig);      // pointer naar links
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
            bovenVakje->zetBuur(4, onderVakje);   // onder
            onderVakje->zetBuur(0, bovenVakje);   // boven
            
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
    
    // Maak beginpositie(witte en zwarte vakjes)
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

// Print het bord in de terminal
void drukBordAf(vakje* linksboven, int breedte, int hoogte) {
    vakje* rijPointer = linksboven;
    
    for (int rij = 0; rij < hoogte; rij++) {
        vakje* kolomPointer = rijPointer;
        
        for (int kolom = 0; kolom < breedte; kolom++) {
            cout << kolomPointer->geefInhoud() << " ";
            kolomPointer = kolomPointer->geefBuur(2);  // naar rechts
        }
        cout << endl;
        
        rijPointer = rijPointer->geefBuur(4);  // naar onder
    }
    cout << endl;
}

// Constructor van OthelloBord
OthelloBord::OthelloBord(int b, int h) : breedte(b), hoogte(h), huidigeSpeler('Z') {
    vakje v;
    linksboven = v.maakBord(breedte, hoogte);
}

// Destructor van OthelloBord
OthelloBord::~OthelloBord() {
    vakje v;
    v.verwijderBord(linksboven);
}

// Afdrukken van het bord
void OthelloBord::afdrukken() {
    // Print kolom letters bovenaan
    cout << "  ";
    for (int kolom = 0; kolom < breedte; kolom++) {
        cout << " " << (char)('A' + kolom);
    }
    cout << endl;
    
    vakje* rijPointer = linksboven;
    
    for (int rij = 0; rij < hoogte; rij++) {
        // Print rij nummer
        cout << (rij + 1) << " ";
        if (rij + 1 < 10) cout << " ";  // Extra spatie voor uitlijning
        
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
    huidigeSpeler = (huidigeSpeler == 'Z') ? 'W' : 'Z';
}

// Vindt een specifiek vakje op het bord op basis van rij en kolom (0-indexed)
vakje* OthelloBord::vindVakje(int rij, int kolom) {
    if (rij < 0 || rij >= hoogte || kolom < 0 || kolom >= breedte) {
        return nullptr;
    }
    
    vakje* huidig = linksboven;
    
    // Ga naar de juiste rij
    for (int i = 0; i < rij; i++) {
        huidig = huidig->geefBuur(4);  // naar beneden
    }
    
    // Ga naar de juiste kolom
    for (int i = 0; i < kolom; i++) {
        huidig = huidig->geefBuur(2);  // naar rechts
    }
    
    return huidig;
}

// Telt hoeveel stenen omgeslagen kunnen worden in een bepaalde richting
int OthelloBord::telOmslaanInRichting(vakje* v, int richting, char speler) {
    char tegenstander = (speler == 'Z') ? 'W' : 'Z';
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

// Slaat stenen om in een bepaalde richting
void OthelloBord::slaOmInRichting(vakje* v, int richting, char speler, int aantal) {
    vakje* huidig = v->geefBuur(richting);
    
    for (int i = 0; i < aantal; i++) {
        huidig->zetInhoud(speler);
        huidig = huidig->geefBuur(richting);
    }
}

// Voert een zet uit op het bord
bool OthelloBord::doeZet(char kolom, int rij) {
    // Converteer kolom letter naar index (A=0, B=1, etc.)
    int kolomIndex = kolom - 'A';
    int rijIndex = rij - 1; 
    
    vakje* v = vindVakje(rijIndex, kolomIndex);
    
    if (v == nullptr) {
        cout << "Ongeldige positie!" << endl;
        return false;
    }
    
    if (!isZetGeldig(v, huidigeSpeler)) {
        cout << "Ongeldige zet! Je moet minimaal 1 steen omslaan." << endl;
        return false;
    }
    
    // Plaats de steen
    v->zetInhoud(huidigeSpeler);
    
    // Sla stenen om in alle geldige richtingen
    for (int richting = 0; richting < 8; richting++) {
        int aantal = telOmslaanInRichting(v, richting, huidigeSpeler);
        if (aantal > 0) {
            slaOmInRichting(v, richting, huidigeSpeler, aantal);
        }
    }
    
    cout << "Zet uitgevoerd!" << endl;
    return true;
}


// Leest een karakter in dat niet een enter is.
char optieInlezen(){
    char k = cin.get();
    while (k == '\n'){
        k = cin.get();
    }
    return k;
}


//Maakt een kleine letter van een grote letter
char maakKleineLetter(char a){
    if (a >= 'A' && a <= 'Z'){
        return a + ('a' - 'A');
    } return a;
}


//Lees een getal in.
int leesGetal()
{
    int getalbuffer = 0;
    char k;

    while ((k = cin.get()) != '\n')
    {
        if (k >= '0' && k <= '9')   
        {
            getalbuffer = getalbuffer * 10 + (k - '0');
        }
    }

    return getalbuffer;
}


// Menu die gebruiker optie laat kiezen van othello(non-member)
void menu() {
    int breedte, hoogte;
    char keuze;
    
    cout << "=== Welkom bij Othello ===" << endl << endl;
    
    cout << "Geef de breedte van het bord (standaard 8): ";
    breedte = leesGetal();  
    
    cout << "Geef de hoogte van het bord (standaard 8): ";
    hoogte = leesGetal();   
    
    if (breedte == 0) breedte = 8;
    if (hoogte == 0) hoogte = 8;
    
    OthelloBord bord(breedte, hoogte);
    cout << endl << "Bord aangemaakt!" << endl << endl;
    
    bool spelActief = true;
    while (spelActief) {
        cout << "=== MENU ===" << endl;
        cout << "Huidige speler: " << bord.geefHuidigeSpeler() << endl;
        cout << "T: Toon bord" << endl;
        cout << "D: Doe een zet" << endl;
        cout << "S: Stop het spel" << endl;
        cout << "Keuze: ";
        
        keuze = optieInlezen();           
        keuze = maakKleineLetter(keuze);  
        
        switch(keuze) {
            case 't':
                bord.afdrukken();
                break;
                
            case 'd': {
                char kolom;
                int rij;
                
                // Lees kolom
                cout << "Geef kolom (A-" << (char)('A' + breedte - 1) << "): ";
                kolom = optieInlezen();
                kolom = maakKleineLetter(kolom);
                kolom = kolom - 'a' + 'A';  // Maak hoofdletter voor verwerking

                cin.get();

                // Lees rij
                cout << "Geef rij (1-" << hoogte << "): ";
                rij = leesGetal();
                
                if (bord.doeZet(kolom, rij)) {
                    bord.wisselSpeler();
                    bord.afdrukken();
                }
                break;
            }
                
            case 's':
                cout << "Spel gestopt." << endl;
                spelActief = false;
                break;
                
            default:
                cout << "Ongeldige keuze, probeer opnieuw." << endl;
                break;
        }
        cout << endl;
    }
}
