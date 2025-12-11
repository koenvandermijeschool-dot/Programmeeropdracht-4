#include <iostream>
#include <cstdlib>  //Voor rand()
#include "otbord.h"

using namespace std;

using namespace std;
void infoBlokje(){
    cout << "Dit is het spel othello gemaakt door Dylan Kusters en Koen van der Mije." << 
    "s4912926 & s4809033" << endl;
    cout << "In dit spel kan je othello spelen tegen een mens, "<<
    "een computer of je kan 2 computers tegen elkaar laten spelen." << endl;
    cout << "Je kan ook de groote van het bord veranderen." << endl;
    cout << "Wanneer 2 computers tegen elkaar spelen kan je het aantal games veranderen en " <<
    "zullen de resultaten in het bestand 'output' worden opgeslagen." << endl;
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
//Menu wordt geprint waarbij de gebruiker keuzes kan maken om het spel te spelen.
gebruiker alle keuzes in dit menu kan maken .
void menu(){
    bool opnieuwSpelen = true; //Als false stopt spel
    
    while (opnieuwSpelen) {
        int breedte, hoogte; //Breedte en hoogte van het bord
        char keuze; //karakter voor menukeuze
        char karakter; //Karakter voor of speler een computer is
        
        cout << "=== Welkom bij Othello ===" << endl << endl;
        //Lees breedte en hoogte van gebruiker in
        cout << "Geef de breedte van het bord (standaard 8): ";
        breedte = leesGetal();  
        
        cout << "Geef de hoogte van het bord (standaard 8): ";
        hoogte = leesGetal();   
        
        if (breedte == 0) {
            breedte = 8;} 
        if (hoogte == 0) {
            hoogte = 8;}
        //Maak othellobord object maken
        OthelloBord bord(breedte, hoogte);
        //Lees van gebruiker in welke spelers computer zijn
        cout << "Is wit een Computer? (J/N): ";
        karakter = optieInlezen();
        bord.zetWitComputer(maakKleineLetter(karakter) == 'j');
        
        cout << "Is zwart een Computer? (J/N): ";
        karakter = optieInlezen();
        bord.zetZwartComputer(maakKleineLetter(karakter) == 'j');
        //Als beide computer, vraag voor aantal spellen
        if (bord.getZwartComputer() && bord.getWitComputer()) {
            int aantalSpellen;
            cin.ignore(1000, '\n'); //cin ignore nodig
            cout << "Hoeveel spellen wil je spelen? ";
            aantalSpellen = leesGetal();
            
            if (aantalSpellen <= 1) {
                aantalSpellen = 1;}
        
            cout << endl << "Start " << aantalSpellen << " spel(len)..." << endl;
            bord.speelMeerdereSpellen(aantalSpellen);
        } else { // Als niet computer tegen computer..
            cout << endl << "Bord aangemaakt!" << endl << endl;
            
            bool spelActief = true; //Of spel bezig is
            while (spelActief) {
                if (bord.isSpelAfgelopen()) { //Als spel klaar is toon eindstand
                    bord.afdrukken();
                    bord.toonEindstand();
                    spelActief = false;
                    break;
                }
                //Beurt wordt overgeslagen als speler geen huidige zetten heeft.
                if (!bord.heeftGeldigeZetten(bord.geefHuidigeSpeler())) {
                    cout << "Speler " << bord.geefHuidigeSpeler() 
                         << " heeft geen geldige zetten. Beurt wordt overgeslagen." << endl;
                    bord.wisselSpeler();
                    continue;
                }
                //Als computer aan de beurt is doe dan computer zet
                if (bord.isHuidigeSpelerComputer()) {
                    cout << "Computer aan de beurt..." << endl;
                    bord.afdrukken();
                    bord.doeComputerZet();
                    bord.wisselSpeler();
                    cout << endl;
                    continue;
                }
                //Toon menu
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
                        //Gebruiker geeft kolom van zet
                        cout << "Geef kolom (A-" << (char)('A' + breedte - 1) << "): ";
                        kolom = optieInlezen();
                        kolom = maakKleineLetter(kolom);
                        kolom = kolom - 'a' + 'A';
                        //Haal enter weg.
                        cin.get();
                        //Gebruiker geeft rij van zet
                        cout << "Geef rij (1-" << hoogte << "): ";
                        rij = leesGetal();
                        
                        if (bord.doeZet(kolom, rij)) {
                            bord.wisselSpeler();
                            bord.afdrukken();
                        }
                        break;
                    }
                    
                    case 'o':
                        //Maak zet ongedaan
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
        //Als spel geeindigt is, vraag gebruiker of die opnieuw wilt speler.
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
    infoBlokje();
    menu();
}
