#include <iostream>
#include <cstdlib>  //Voor rand()
#include "otbord.h"

using namespace std;

using namespace std;

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
void menu(){
    bool opnieuwSpelen = true;
    
    while (opnieuwSpelen) {
        int breedte, hoogte;
        char keuze;
        char karakter;
        
        cout << "=== Welkom bij Othello ===" << endl << endl;
        
        cout << "Geef de breedte van het bord (standaard 8): ";
        breedte = leesGetal();  
        
        cout << "Geef de hoogte van het bord (standaard 8): ";
        hoogte = leesGetal();   
        
        if (breedte == 0) breedte = 8;
        if (hoogte == 0) hoogte = 8;
        
        OthelloBord bord(breedte, hoogte);
        
        cout << "Is wit een Computer? (J/N): ";
        karakter = optieInlezen();
        bord.zetWitComputer(maakKleineLetter(karakter) == 'j');
        
        cout << "Is zwart een Computer? (J/N): ";
        karakter = optieInlezen();
        bord.zetZwartComputer(maakKleineLetter(karakter) == 'j');

        if (bord.getZwartComputer() && bord.getWitComputer()) {
            int aantalSpellen;
            cin.ignore(1000, '\n');
            cout << "Hoeveel spellen wil je spelen? ";
            aantalSpellen = leesGetal();
            
            if (aantalSpellen <= 0) aantalSpellen = 1;
            
            cout << endl << "Start " << aantalSpellen << " spel(len)..." << endl;
            bord.speelMeerdereSpellen(aantalSpellen);
        } else {
            cout << endl << "Bord aangemaakt!" << endl << endl;
            
            bool spelActief = true;
            while (spelActief) {
                if (bord.isSpelAfgelopen()) {
                    bord.afdrukken();
                    bord.toonEindstand();
                    spelActief = false;
                    break;
                }
                
                if (!bord.heeftGeldigeZetten(bord.geefHuidigeSpeler())) {
                    cout << "Speler " << bord.geefHuidigeSpeler() 
                         << " heeft geen geldige zetten. Beurt wordt overgeslagen." << endl;
                    bord.wisselSpeler();
                    continue;
                }
                
                if (bord.isHuidigeSpelerComputer()) {
                    cout << "Computer aan de beurt..." << endl;
                    bord.afdrukken();
                    bord.doeComputerZet();
                    bord.wisselSpeler();
                    cout << endl;
                    continue;
                }
                
                cout << "=== MENU ===" << endl;
                cout << "Huidige speler: " << bord.geefHuidigeSpeler() << endl;
                cout << "T: Toon bord" << endl;
                cout << "D: Doe een zet" << endl;
                cout << "O: Ongedaan maken (zetten beschikbaar: " 
                     << bord.geefAantalOngedaanMaken() << ")" << endl;
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
                        
                        cout << "Geef kolom (A-" << (char)('A' + breedte - 1) << "): ";
                        kolom = optieInlezen();
                        kolom = maakKleineLetter(kolom);
                        kolom = kolom - 'a' + 'A';

                        cin.get();

                        cout << "Geef rij (1-" << hoogte << "): ";
                        rij = leesGetal();
                        
                        if (bord.doeZet(kolom, rij)) {
                            bord.wisselSpeler();
                            bord.afdrukken();
                        }
                        break;
                    }
                    
                    case 'o':
                        if (bord.undoZet()) {
                            bord.afdrukken();
                        }
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
        
        cout << "\nWil je opnieuw spelen? (J/N): ";
        karakter = optieInlezen();
        cin.ignore(1000, '\n');  
        
        if (maakKleineLetter(karakter) != 'j') {
            opnieuwSpelen = false;
            cout << "Bedankt voor het spelen!" << endl;
        }
        cout << endl;
    }
}


int main() {
    menu();
}
