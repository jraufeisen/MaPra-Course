#include <cmath>
#include <iostream>
#include <random>
#include "unit.h"
#include "Spielbrett.h"

void NetzwerkMain();


 const unsigned int Schwierigkeitsgrad = 0;



double miniMax(Spielbrett brett, Feld farbe, std::vector<int>& besteZuege , int tiefe = 4) {
    Feld gegenfarbe  = farbe == rot ? gelb : rot;

    
    //Wechlse noch mit min und max je nachdem ob u an der reihe bist
    if (tiefe == 0 || brett.spielIstBeendet()) {
        std::cout << brett.heuristischeBewertung(farbe) << std::endl;

        return brett.heuristischeBewertung(farbe);
    }
    
    double maxWert = -INFINITY;
    // Generiere moegliche Zuege:
    for (unsigned int i = 0; i < AnzahlSpalten; i++) {
        if (brett.setzeStein(i, farbe) == true ) { //Sofern das ein gueltieger Zug ist
            double wert = miniMax(brett, gegenfarbe, besteZuege, tiefe-1);
           

            brett.entferneStein(i);
            if (wert >= maxWert) {
                maxWert = wert;
                
                
                if (tiefe == 4) {
                    //Einer der besten Zuege
                    besteZuege.push_back(i);
                }
                
            }
        }
    }

    return maxWert;
}


int errechneBestenZug(Spielbrett brett, Feld farbe) {

    std::vector<int> besteZuege;
    miniMax(brett, farbe, besteZuege);
 
    std::cout << "Zuege: ";
    for (auto i = besteZuege.begin(); i != besteZuege.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    
    
    return besteZuege[0];
}




int main()
{
    int nextMove, Gegenzug;
    Feld meineFarbe, gegnerFarbe;
    // Netzwerkspiel? Rufe NetzwerkMain() auf.

    Start(Schwierigkeitsgrad);
    
    
    for(unsigned int Spiel = 1; Spiel <= AnzahlSpiele; Spiel++)
    {
        Spielbrett brett = Spielbrett(6,7);
        
        
        if (Spiel % 2 == 1) { //Wir fangen an mit gelb
            meineFarbe = gelb;
            gegnerFarbe = rot;

            nextMove = errechneBestenZug(brett, meineFarbe); 
            brett.setzeStein(nextMove, meineFarbe);
        } else { //Das Programm der Umgebung unit.o beginnt mit gelb
            meineFarbe = rot;
            gegnerFarbe = gelb;
            nextMove = -1;
        }
        
        Gegenzug = NaechsterZug(nextMove);
        
        while (Gegenzug >= 0) { //Falls Spiel vorbei, wird kleiner Null zurueckgegeben
            
            brett.setzeStein(Gegenzug, gegnerFarbe);
            
            //nextMove = besterZug in aktuellem Spielfeld
            nextMove = errechneBestenZug(brett, meineFarbe); 
            brett.setzeStein(nextMove, meineFarbe);

            Gegenzug = NaechsterZug(nextMove);

        }
       
        
    }

    return 0;
}

enum class SpielStatus {
    Verbindungsfehler,
    Niederlage,
    Unentschieden,
    Sieg
};

// Spielt ein einzelnes Netzwerkspiel gegen einen anderen Computer im Netzwerk.
// Sollte das Spiel beendet sein oder ein Netzwerkfehler auftreten, muss diese Methode
// das zugehoerige Element der Enumeration SpielStatus zurueckgeben.
SpielStatus Netzwerkspiel( Feld MeineFarbe ) {

    // TODO Implementiere Netzwerkprotokoll
    
    return SpielStatus::Verbindungsfehler;
}

void NetzwerkMain() {
    int command;
    
    // Einleseschleife für Befehle. Terminiert, wenn ein gültiger Befehl gewählt wurde.
    while (true) {    
        std::cout << "\n";
        std::cout << "1 = VERBINDE  mit einem anderen Spieler\n";
        std::cout << "2 = WARTE     auf einen anderen Spieler" << std::endl;

        std::cin >> command;
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        } else if (command >= 1 && command <= 2) {
            break;
        }
    }
    
    if (command == 0) {
        return;
    }
    
    Feld MeineFarbe=  gelb,
         GegnerFarbe= rot;
         
    if (command == 1) {
        std::string ip;        
        unsigned short port = 0;
        
        std::cout << "Bitte geben Sie die IP ein, mit der Sie sich verbinden wollen. \n";
        std::cout << "Format: xxx.xxx.xxx.xxx" << std::endl;
        std::cin >> ip;
        std::cout << "Bitte geben Sie den Port ein, mit dem Sie sich verbinden wollen. \n";
        std::cin >> port;
        
        if (!Verbinde(ip.c_str(), port)) {
            std::cout << "Verbindung fehlgeschlagen." << std::endl;
            return;
        }

        std::cout << "Verbindung hergestellt." << std::endl;
        
        // Ich verbinde mich -> Meine Farbe ist rot
        std::swap(MeineFarbe, GegnerFarbe);
    } else {
        unsigned short port = 0;
        
        std::cout << "Bitte geben Sie den Port ein, mit dem sich Ihr Mitspieler verbinden soll. \n";
        std::cin >> port;
        std::cout << "Warte auf Gegner..." << std::endl;
        
        if (!WarteAufVerbindung(port)) {
            std::cout << "Kein Gegner gefunden.";
            return;
        }
        std::cout << "Verbindung hergestellt." << std::endl;
        
        // Ich warte auf Spieler -> meine Farbe ist gelb
    }
    
    bool nochEinSpiel = true;
    while (nochEinSpiel) {
        auto ergebnis = Netzwerkspiel( MeineFarbe );

        if (ergebnis == SpielStatus::Verbindungsfehler) {
            nochEinSpiel = false;
            std::cout << "Verbindungsfehler!" << std::endl;
        } else {
            switch(ergebnis) {
                case SpielStatus::Niederlage: 
                    std::cout << "Sie haben verloren!" << std::endl;
                    break;
                case SpielStatus::Unentschieden:
                    std::cout << "Es gab ein Unentschieden!" << std::endl;
                    break;
                case SpielStatus::Sieg:
                    std::cout << "Sie haben gewonnen!" << std::endl;
                    break;
                default:
                    std::cout << "Verbindungsfehler!" << std::endl;
                    SchliesseVerbindung();
                    return;
            }

            std::cout << "Nochmal spielen? [y/n]" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            char antwort;
            std::cin >> antwort;
            nochEinSpiel = ! (!std::cin || (antwort != 'Y' && antwort != 'y') );

            if (nochEinSpiel) {
                std::swap( MeineFarbe, GegnerFarbe );
                NaechstesNetzwerkspiel();
            }
        }
    }
    
    SchliesseVerbindung();
}
