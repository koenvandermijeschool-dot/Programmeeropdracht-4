

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
    
    public:
        OthelloBord(int b, int h);  // Constructor
        ~OthelloBord();             // Destructor
        void afdrukken();           
};

void menu();
