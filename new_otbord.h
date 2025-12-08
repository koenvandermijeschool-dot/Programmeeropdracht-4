#include <stack>
using namespace std;

// Struct om een zet op te slaan
struct Zet {
    char* bordsituatie;  // Kopie van het hele bord
    char speler;         // Speler die de zet deed
    int bordGrootte;
    
    Zet(char s, char* bord, int grootte) : speler(s), bordGrootte(grootte) {
        bordsituatie = new char[grootte];
        for (int i = 0; i < grootte; i++) {
            bordsituatie[i] = bord[i];
        }
    }
    
    ~Zet() {
        delete[] bordsituatie;
    }
};

class vakje {
    private:
        char inhoud;
        vakje* buurvakjes[8];

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

class OthelloBord {
    private:
        vakje* linksboven;
        int breedte;
        int hoogte;
        char huidigeSpeler;
        bool witComputer;
        bool zwartComputer;
        bool nietafdrukken;
        stack<Zet*> zetGeschiedenis;  // Standaard STL stack!
        
    public:
        OthelloBord(int b, int h);
        ~OthelloBord();
        void afdrukken();
        bool doeZet(char kolom, int rij);
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
        
        // Undo functies
        bool undoZet();
        void slaOpBordsituatie(char* buffer);
        void herstelBordsituatie(char* buffer);
        int geefAantalOngedaanMaken();
};

void menu();
