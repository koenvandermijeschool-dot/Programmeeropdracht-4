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
    
    // Maak standaard beginpositie
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
OthelloBord::OthelloBord(int b, int h) : breedte(b), hoogte(h) {
    // Maak een tijdelijk vakje object om maakBord aan te roepen
    vakje v;
    linksboven = v.maakBord(breedte, hoogte);
}

// Destructor van OthelloBord
OthelloBord::~OthelloBord() {
    // Maak een tijdelijk vakje object om verwijderBord aan te roepen
    vakje v;
    v.verwijderBord(linksboven);
}

// Afdrukken van het bord
void OthelloBord::afdrukken() {
    vakje* rijPointer = linksboven;
    
    for (int rij = 0; rij < hoogte; rij++) {
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
    
    if (breedte == 0) {
        breedte = 8;}
    if (hoogte == 0) {
        hoogte = 8;}
    
    OthelloBord bord(breedte, hoogte);
    cout << endl << "Bord aangemaakt!" << endl << endl;
    
    bool spelActief = true;
    while (spelActief) {
        cout << "=== MENU ===" << endl;
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
                
            case 'd':
                cout << "Zet functie nog niet geimplementeerd." << endl;
                break;
                
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
