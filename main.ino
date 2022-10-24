//Projet créé par Yanis, Sény, Loïc et Driss

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialisation des tensions :
const int nbrTensions = 8; //nombre de tensions possibles

/*On appelle tableau une variable composée de données de même type, 
stockée de manière contiguë en mémoire (les unes à la suite des autres).

Un tableau est donc une suite de cases (espace mémoire) de même taille. 
La taille de chacune des cases est conditionnée par le type de donnée que le tableau contient.

Exemple : 

type Nom_du_tableau [Nombre d'éléments];

*/


float tension[nbrTensions] = {1.28, 2.04, 2.45, 2.89, 3.04, 3.19, 3.48, 3.70}; //tableau des tensions de chaque modèle de carte
int mecanismes[nbrTensions] = {1, 2, 3, 4, 5, 6, 3, 1}; //tableau des mécanismes d'authentification à activer suivant chaque cas de carte

/*Structure représentant 

Structures

C supporte les types composés avec la notion de structure. 
Pour définir une structure, il faut utiliser le mot-clé struct suivi du nom de la structure. 
Les membres doivent ensuite être déclarés entre accolades. 
Comme toute déclaration, un point-virgule termine le tout.

struct Personne
{
    int age;
    char *nom;
};

Pour accéder aux membres d'une structure, il faut utiliser l'opérateur "."

Exemple dans une fonction :

int main()
{
    struct Personne p;
    p.nom = "Driss";
    p.age = 36;
}

*/

//Cette structure représente la 
struct tensionStruct
{
    float value;
    float min;
    float max;
    int mecanisme;
};

//Structure répertoriant toutes les valeurs de tensionStruct pour chaque modèle de cartes
struct tensionStruct voltages[nbrTensions];

//Fonction setup
void setup()
{

    Serial.begin(9600); //Démarrage de la connexion entre l'ordinateur et l'arduino avec une vitesse de 6900 bits par seconde
    Serial.println("Projet créé par Yanis, Sény, Loïc et Driss.");
    //Boucle associant à chaque modèle de tension, sa valeur de tension avec les marges d'erreur de 5% [min et max] ainsi que les mécanismes associés au modèle
    for (int i = 0; i < nbrTensions; i++)
    {
        voltages[i].value = tension[i];
        voltages[i].min = tension[i] * 0.95;
        voltages[i].max = tension[i] * 1.05;
        voltages[i].mecanisme = mecanismes[i];
    }

    float valeur = 0; 
    float t = 0;
    // déclare les variables à virgules valeur et tension

    Serial.print("Inserez votre carte (appuyer sur les bons interrupteurs) et entrez un charactere ");
    Serial.println("au hasard pour lancer le programme.");

    while (Serial.available() == 0);
    while (Serial.available() > 0)
    {
        Serial.read();
    }

    valeur = analogRead(A0); //Tension lue par la broche A0
    t = valeur * (5.0 / 1023.0); // Transforme la mesure (nombre entier) en tension via un produit en croix
    Serial.print("Valeur de t : ");
    Serial.println(t);

    /* Boucle allant de i au nombre de tensions (ici 7).*/
    for (int i = 0; i < nbrTensions; i++)
    {
        if (t > voltages[i].min && t < voltages[i].max) //compare si notre tension mesurée est comprise dans notre intervalle de tensions minimum et maximum propre à chaque modèle.
        {

            bool r1 = true, r2 = true, r3 = true, r4 = true, r5 = true; //Initialisation des valeurs propres à chaque mécanisme d'authentification.
            
            /*
            Switch évalue l'expression qui lui est donnée et aiguille 
            le programme vers le cas correspondant à la valeur effective 
            de cette expression. Les cas sont indiqués par l'instruction case. 
            
            Exemple : 

            switch(x){

                case 1: printf(" nous sommes dans le cas où x = 1"); break;
                case 2: printf(" nous sommes dans le cas où x = 2"); break;
                case 3: printf(" nous sommes dans le cas où x = 3"); break;
                case 4: printf(" nous sommes dans le cas où x = 4"); break;
            }

            Dans notre cas, nous regardons à quels mécanismes notre tension est associée.
            */

            switch (voltages[i].mecanisme)
            {
            case 1:
                r1 = MA1();
                r3 = MA3();
                break;
            case 2:
                r1 = MA1();
                r4 = MA4();
                break;
            case 3:
                r2 = MA2();
                r5 = MA5();
                break;
            case 4:
                r2 = MA2();
                r3 = MA3();
                r5 = MA5();
                break;
            case 5:
                r1 = MA1();
                r2 = MA2();
                r3 = MA3();
                r5 = MA5();
                break;
            case 6:
                r2 = MA2();
                r3 = MA3();
                r5 = MA5();
                break;
            }

            if (r1 && r2 && r3 && r4 && r5) //Dans le cas où tous nos r sont égal à true, nous pouvons ouvrir le coffre (r est toujours un booléen)
            {
                Serial.println("Coffre ouvert");
                exit(-1); //Arret du programme
            }
            else
            {
                erreur();
            }
        }
    }
    erreur();
}

bool question1()
{

    Serial.println("Quand a ete cree l'agence ?");
    Serial.println("1 - 12/04/1990");
    Serial.println("2 - 13/04/1950");
    Serial.println("3 - 10/04/1987");
    Serial.println("4 - 18/09/1947");
    while (Serial.available() == 0);
    int a = Serial.parseInt();
    Serial.println("Question suivante (1/3) [reponse enregistree : " + String(a) + "]");

    if (a == 4)
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool question2()
{

    Serial.println("Quel est le fabricant du premier prototype STRONGBOX3000 ?");
    Serial.println("1 - El Chapo");
    Serial.println("2 - Alter Ego");
    Serial.println("3 - Pablo Escobar");
    Serial.println("4 - Jeff Bezos");
    while (Serial.available() == 0);
    int b = Serial.parseInt();
    Serial.println("Question suivante (2/3) [reponse enregistree : " + String(b) +
                   "]");
    if (b == 2)
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool question3()
{

    Serial.println("Quel est le nom de l'agence ?");
    Serial.println("1 - MarcoPolo");
    Serial.println("2 - MenInBlack");
    Serial.println("3 - CesiCorp");
    Serial.println("4 - CesiTaCarte");
    
    while (Serial.available() == 0);
    int c = Serial.parseInt();
    
    Serial.println("Questions terminees. [reponse enregistree : " + String(c) +"]");

    if (c == 4)
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool MA1()
{
    bool a = question1();
    
    bool b = question2();
    
    bool c = question3();
    

    if (a == true && b == true && c == true)
    {
        Serial.print("Premier niveau d'authentification valide");
        return true;
    }
    else
    {
        Serial.print("Premier niveau d'authentification invalide.");

        return false;
    }
}

int chiffrer(int M, int e);
int modexp(int a, int e, int n);
int cleAgent(String x);

int cleAgent(String x)
{

    int e[16] = {601, 619, 631, 641, 647, 653, 661, 673,
                 691, 701, 733, 739, 751, 797, 809, 811};
    int reId = 0;
    
    String d[16] = {"A", "B", "C", "D", "E", "F", "G", "H",
                    "I", "J", "K", "L", "M", "O", "P"};

    for (int i = 0; i < 16; i++)
    
    {
        if (x == d[i])
        {
            reId = e[i];
            return reId;
        }
        else
        {
            Serial.println("Clef non trouvée");
            exit(-1);
        }
    delay(500);
    }
}

int chiffrer(int M, int e)
{
    int C;
    int n;
    n = 43 * 67;
    
    C = modexp(M, e, n);
    return C;
}

int modexp(int a, int e, int n)
{
    long r;
    
    if (e < 0)
    {
        Serial.println("unhandled case");
        
        exit(-1);
    }
    if (a == 0 || n == 1)
    {
        r = 0;
    }
    else
    {
        r = 1;
        while (e > 0)
        {
            
            r = (r * (a % n)) % n;
            e = e - 1;
        }
    }
    return r;
}

bool MA2()
{

    bool pass = false;
    int v = 0;
    int e;
    int M;
    int Mp;
    int C;
    
    String x;

    Serial.println("Entrez votre code agent (x) : ");
    while (Serial.available() == 0);
    x = Serial.readString();
    Serial.println("X: " + x);
    delay(2000);

    M = random(0, 2881);
    Serial.print("M : ");
    Serial.println(M);
    delay(2000);

    e = cleAgent(x);
    Serial.println(e);
    delay(2000);

    C = chiffrer(M, e);
    delay(1000);

    Serial.println("Message à dechiffrer : ");
    Serial.print(C);
    while (Serial.available() == 0);

    Mp = Serial.parseInt();

    if (M == Mp)
    {
        Serial.println("Authentification 2 verifiee");
        pass = true;
    }
    else
    {
        Serial.println("Erreur d'authentification de niveau 2.");
        pass = false;
    }

    return pass;
}

bool MA3()
{
    bool isscan = false;
    int inp;
    Serial.println("Veuillez vous positionner pour le scan de votre oeil. Entrez 1");
    while (Serial.available() == 0);
    
    inp = Serial.parseInt();
    
    if (inp == 1)
    {
        isscan = true;
        Serial.println("Authentification 3 verifiee");
    }
    else
    {
        Serial.println("Erreur d'authentification de niveau 3.");
    }

    return isscan;
}

bool MA4()
{
    bool isscan = false;
    int inp;
    Serial.println("Veuillez vous positionner pour le scan de votre doigt. Entrez 1.");
    
    while (Serial.available() == 0);
    inp = Serial.parseInt();
    

    if (inp == 1)
    {
        isscan = true;
        Serial.println("Authentification 4 vérifiée");
    }
    else
    {
        Serial.println("Erreur d'authentification de niveau 4.");
    }

    return isscan;
}

bool MA5()
{

    bool iscorrect = false;
    int i = 0;
    int nbrcodes = 16;
    String codes[nbrcodes] = {"A4454", "B9656", "C9999", "D0000", "E6012", "F3099", "G4884", "H4444", "I0335", "J3636", "K8888", "L2292", "M1903", "N8526", "O8888"};

    String lAgent;
    String cardid;
    String saisie;

    // L agent
    Serial.println("Entrez votre lettre agent");
    while (Serial.available() == 0);
    lAgent = Serial.readString();
    
    Serial.println("Lettre agent enregistree : " + String(lAgent));

    // Card ID
    Serial.println("Entrez votre card_id");
    while (Serial.available() == 0);
    cardid = Serial.readString();
    
    Serial.println("CARD_ID enregistree : " + String(cardid));

    saisie = lAgent + cardid;
    

    while (i <= nbrcodes && iscorrect == false)
    {
        
        if (codes[i] != saisie)
        {

            if (i >= nbrcodes - 1)
            {
                Serial.println("Erreur d'authentification de niveau 5.");
                return iscorrect;
            }
            else
            {
                i++;
            }
        }
        else
        {
            Serial.println("Authentification 5 vérifiée");
            iscorrect = true;
        }
    }

    Serial.println(iscorrect);
    return iscorrect;
}

void erreur()
{
    Serial.println("Erreur dans le mecanisme d'authentification. Sécurité activee.");

}

void loop() {}
