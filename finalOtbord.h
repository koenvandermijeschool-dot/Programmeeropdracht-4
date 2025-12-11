#include <stack>
using namespace std;
// Struct die één uitgevoerde zet representeert
struct Zet {
    char* bordsituatie;   // kopie van de volledige bord-array
    char speler;          // Welke speler deze zet heeft gedaan
    int bordGrootte;      // Hoe groot de bord-array is

    // Constructor: maakt een nieuwe zet met speler en kopie van het bord
    Zet(char s, char* bord, int grootte) 
        : speler(s), bordGrootte(grootte) 
    {
        bordsituatie = new char[grootte];   // Reserveer geheugen voor de kopie
        for (int i = 0; i < grootte; i++) { // Kopieer elk element
            bordsituatie[i] = bord[i];
        }
    }

    // Destructor: geeft het geheugen weer vrij
    ~Zet() {
        delete[] bordsituatie; 
    }
};
//Class Vakje met max 8 buren.
class vakje {
    private:
        char inhoud; //'W', 'Z' of '.'
        vakje* buurvakjes[8]; //pointer array van alle buren.

    public:
        vakje();
        char geefInhoud(); //Krijg inhoud van een vakje terug
        void zetInhoud(char c); //Verander de inhoud van een vakje
        vakje* geefBuur(int richting); //Geef pointer van een specifieke buur
        void zetBuur(int richting, vakje* buurvakje); // Geef een waarde aan de pointer van een specifieke buur
        vakje* maakRij(int breedte); // Maakt een dubbelverbonden pointer
        vakje* maakBord(int breedte, int hoogte); //Maakt de vakjes en pointers volledige othellobord
        void verwijderBord(vakje* linksboven); //Verwijdert alle vakjes en pointers van het bord
};

class OthelloBord {
    private:
        vakje* linksboven; //Het eerste vakje waarmee bord gemaakt en verwijderd wordt
        int breedte; //Breedte van het bord
        int hoogte; //Hoogte van het bord
        char huidigeSpeler; //De speler die nu aan de beurt is
        bool witComputer; //Boolean of witte speler computer is
        bool zwartComputer; //Boolean of zwarte speler computer is
        bool nietafdrukken; //Boolean voor als het bord niet geprint moet worden.
        stack<Zet*> zetGeschiedenis;  //Stack van alle zetten die gemaakt
        
    public:
        OthelloBord(int b, int h);//Constructor
        ~OthelloBord();//Deconstructor
        void afdrukken();//Printen van het othellobord
        bool doeZet(char kolom, int rij); //Hiermee kan een menselijke speler een zet doen
        char geefHuidigeSpeler(); //Geeft huidige speler terug ('W' of 'Z')
        void wisselSpeler(); //Andere speler komt aan de beurt
        vakje* vindVakje(int rij, int kolom); // Vindt een vakje met rij en kolom
        bool isZetGeldig(vakje* v, char speler); //Checkt of zet 
        int telOmslaanInRichting(vakje* v, int richting, char speler);
        void slaOmInRichting(vakje* v, int richting, char speler, int aantal); // Slaat steen om
        
        bool heeftGeldigeZetten(char speler); //Checkt of een speler een huidige zet heeft
        void doeComputerZet(); //Computer doet een random zet.
        void zetWitComputer(bool isComputer); //Witte computerspeler doet zet
        void zetZwartComputer(bool isComputer); //Zwart computerspeler doet zet
        bool isHuidigeSpelerComputer();//Geeft terug of huidige speler computer is
        bool isSpelAfgelopen(); //Check of spel geeindigt is
        int telStenen(char speler); //Telt hoeveel stenen elke kleur heeft
        void toonEindstand(); //Toont resultaten van het spel
        bool getWitComputer(); //Geeft bool witcomputer terug
        bool getZwartComputer(); //Geeft bool zwartcomputer terug
        void speelMeerdereSpellen(int aantalSpellen);
        void resetBord(); //Resetbord naar standaardpositie
        void zetNietAfdrukken(bool a); //Geef een waarde aan bool niet afdrukken.
        
        // Undo functies
        bool undoZet(); //Maakt vorige zet ongedaan
        void slaOpBordsituatie(char* buffer); //Slaat bord op in de stapel
        void herstelBordsituatie(char* buffer); //Haalt bord uit de stapel
        int geefAantalOngedaanMaken(); //Geeft aantal zetten dat ongedaan kan worden gemaakt
};

void menu(); //Menu waarmee 
