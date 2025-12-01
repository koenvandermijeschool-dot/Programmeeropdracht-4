class vakje {
    private:
        char inhoud; // . of 'W' of 'Z'
        vakje* buurvakjes[8]; // Alle vakjes die naast het vakje kan liggen

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
        char huidigeSpeler;  // 'W' of 'Z'
        
    
    public:
        OthelloBord(int b, int h);  // Constructor
        ~OthelloBord();             // Destructor
        void afdrukken();
        bool doeZet(char kolom, int rij);
        char geefHuidigeSpeler();
        void wisselSpeler();
        vakje* vindVakje(int rij, int kolom);
        bool isZetGeldig(vakje* v, char speler);
        int telOmslaanInRichting(vakje* v, int richting, char speler);
        void slaOmInRichting(vakje* v, int richting, char speler, int aantal);
};

void menu();
