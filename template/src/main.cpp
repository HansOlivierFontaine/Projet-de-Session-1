/*
Projet: CharlUS V0.000001
Equipe: P-16
Auteurs: Gibraltar
Description: Code du brave robot qui ne fais que se perdre
Date: Le jour de la naissance de Gibraltar
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <fastLED.h>
#include <time.h>
#include <stdlib.h>
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
double speedGauche = 0.21;
double speedDroite = 0.2;
int servoMoteurDroit = 1;
int servoMoteurGauche = 0;
int inputButtonStart = 6;
int inputButtonDifficulte = 5;
int positionX = 0;
int positionY = 0;
int orientation = 0;
int IRCentrePin = 13;
int IRGauchePin = 12;
int IRDroitePin = 11;

int valeurMaxDroit = 0;
int valeurMaxGauche = 0;
int valeurMaxCentre = 0;

//Lumière
#define LED_PIN 8
#define NUM_LEDS 120
#define BRIGHTNESS 100
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define PINDURE  53
#define PINMOYEN  23
#define PINFACILE  45
#define PINLEDMODE  49


// -> defines...
// L'ensemble des fonctions y ont acces

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
struct JEU
{
    int blancheX;
    int blancheY;
    int couleurX;
    int couleurY;
};

void MoveServoMoteur(int servo, int angle)
{
    SERVO_SetAngle(servo, angle);
}

int LireDistanceInfraRobot(int sensor)
{
    return ROBUS_ReadIR(sensor);
}

void Suivre_la_ligne()
{
    int gage = 350;
    int gageDroit = 500;

    MOTOR_SetSpeed(RIGHT, speedDroite);
    MOTOR_SetSpeed(LEFT, speedGauche);

    delay(250);

    int centre = 0;
    int gauche = 0;
    int droite = 0;

    while (true)
    {
        int pressStop = analogRead(inputButtonStart);

        if (pressStop >= 1000)
        {
            while (true)
            {
                MOTOR_SetSpeed(RIGHT, 0);
                MOTOR_SetSpeed(LEFT, 0);
            }
        }

        centre = analogRead(IRCentrePin);
        gauche = analogRead(IRGauchePin);
        droite = analogRead(IRDroitePin);

        if (centre < gage && gauche < gage && droite < gageDroit)
        {
            //Serial.println("go back");
            //MOTOR_SetSpeed(LEFT, -0.01);
            //MOTOR_SetSpeed(RIGHT, -0.01);
        }
        else if (centre > gage && gauche < gage && droite < gageDroit)
        {
            //Serial.println("go straight");
            MOTOR_SetSpeed(LEFT, 0.2);
            MOTOR_SetSpeed(RIGHT, 0.2);
        }
        else if (centre > gage && gauche > gage && droite < gageDroit)
        {
            //Serial.println("go leftish"); //slight boost left engine
            MOTOR_SetSpeed(LEFT, 0.15);
            MOTOR_SetSpeed(RIGHT, 0.2);
        }
        else if (centre > gage && gauche < gage && droite > gageDroit)
        {
            //Serial.println("go rightish"); //slight boost right engine
            MOTOR_SetSpeed(LEFT, 0.2);
            MOTOR_SetSpeed(RIGHT, 0.15);
        }
        else if (centre < gage && gauche > gage && droite < gageDroit)
        {
            //Serial.println("go full left"); //big change turn right
            MOTOR_SetSpeed(LEFT, 0.05);
            MOTOR_SetSpeed(RIGHT, 0.15);
        }
        else if (centre < gage && gauche < gage && droite > gageDroit)
        {
            //Serial.println("go full right"); // big change turn left
            MOTOR_SetSpeed(LEFT, 0.15);
            MOTOR_SetSpeed(RIGHT, 0.05);
        }
        else if (centre > gage && gauche > gage && droite > gageDroit)
        {
            delay(500);
            break;
        }
    }
    Serial.println("INTERSECTION");
}

JEU GetPositionBalles(int difficulte)
{
    int nbrAleatoire;
    JEU balles;

    int facile[11][2][2] = {{{2, 1}, {3, 2}}, //3-6
                            {{2, 2}, {3, 1}}, //4-5
                            {{2, 1}, {2, 2}}, //3-4
                            {{2, 2}, {2, 1}}, //4-3
                            //{{3, 2}, {1, 2}},  //3-1
                            //{{2, 2}, {1, 1}},  //4-1
                            {{1, 1}, {3, 1}}, //1-5
                            {{3, 1}, {1, 1}}, //5-1
                            //{{2, 1}, {1, 2}},  //3-2
                            {{2, 2}, {1, 2}},  //4-2
                            {{1, 2}, {3, 2}},  //2-6
                            {{3, 2}, {1, 2}},  //6-2
                            {{2, 1}, {3, 1}},  //3-5
                            {{2, 2}, {3, 2}}}; //4-6

    int moyen[4][2][2] = {/*{{1, 1}, {3, 2}},*/ //1-6
                          //{{1, 2}, {3, 1}},  //2-5
                          //{{3, 2}, {1, 1}},  //6-1
                          //{{3, 1}, {1, 2}},  //5-2
                          {{1, 2}, {2, 2}},  //2-4
                          {{1, 1}, {2, 1}},  //1-3
                          {{3, 1}, {2, 1}},  //5-3
                          {{3, 2}, {2, 2}}}; //6-4

    int difficile[4][2][2] = {/*{{1, 1}, {1, 2}},*/ //1-2
                              {{3, 1}, {3, 2}},     //5-6
                              /*{{1, 2}, {1, 1}},*/ //2-1
                              {{3, 2}, {3, 1}},     //6-5
                              //{{1, 1}, {2, 2}},     //1-4
                              //{{1, 2}, {2, 1}},     //2-3
                              {{3, 1}, {2, 2}},  //5-4
                              {{3, 2}, {2, 1}}}; //6-3

    if (difficulte == 0)
    {
        nbrAleatoire = rand() % 14;
        Serial.println(nbrAleatoire);
        balles.blancheX = facile[nbrAleatoire][0][0];
        balles.blancheY = facile[nbrAleatoire][0][1];
        balles.couleurX = facile[nbrAleatoire][1][0];
        balles.couleurY = facile[nbrAleatoire][1][1];
        return balles;
    }
    else if (difficulte == 1)
    {
        nbrAleatoire = rand() % 8;
        Serial.println(nbrAleatoire);
        balles.blancheX = moyen[nbrAleatoire][0][0];
        balles.blancheY = moyen[nbrAleatoire][0][1];
        balles.couleurX = moyen[nbrAleatoire][1][0];
        balles.couleurY = moyen[nbrAleatoire][1][1];
        return balles;
    }
    else
    {
        nbrAleatoire = rand() % 6;
        Serial.println(nbrAleatoire);
        balles.blancheX = difficile[nbrAleatoire][0][0];
        balles.blancheY = difficile[nbrAleatoire][0][1];
        balles.couleurX = difficile[nbrAleatoire][1][0];
        balles.couleurY = difficile[nbrAleatoire][1][1];
        return balles;
    }
}

void RejoindreLigne()
{
    Serial.println("Fonction RejoindreLigne :");
    MOTOR_SetSpeed(RIGHT, 0.2);
    MOTOR_SetSpeed(LEFT, 0.21);

    int centre = 0;
    int gauche = 0;
    int droite = 0;

    while (true)
    {
        centre = analogRead(IRCentrePin);
        gauche = analogRead(IRGauchePin);
        droite = analogRead(IRDroitePin);

        if (centre < 500 && gauche < 350 && droite < 350)
        {
            break;
        }
    }
}

void CalculerPositionX()
{
    Serial.println("Fonction CalculerPositionX :");
    if (orientation == 1)
    {
        positionX++;
    }
    else if (orientation == 3)
    {
        positionX--;
    }
    Serial.println(positionX);
}

void CalculerPositionY()
{
    Serial.println("Fonction CalculerPositionY :");
    if (orientation == 0)
    {
        positionY--;
    }
    else if (orientation == 2)
    {
        positionY++;
    }
}

void TournerDroite90()
{
    int comptePulse = 0;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(RIGHT, -0.2);
    MOTOR_SetSpeed(LEFT, 0.2);

    delay(1000); //Environ 90 degrés

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    delay(1000);

    orientation = (orientation + 1) % 4;
    Serial.println(orientation);
}

void TournerGauche90()
{
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(RIGHT, 0.2);
    MOTOR_SetSpeed(LEFT, -0.2);

    delay(1000); //Environ 90 degrés

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    delay(1000);

    orientation = (orientation - 1) % 4;

    if (orientation == -1)
    {
        orientation = 3;
    }
    Serial.println(orientation);
}

void TournerDroite180()
{
    MOTOR_SetSpeed(RIGHT, 0.2);
    MOTOR_SetSpeed(LEFT, -0.2);

    delay(2200); //Environ 90 degrés

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    if (orientation == 1)
    {
        orientation = 3;
    }
    else if (orientation == 0)
    {
        orientation = 2;
    }
    else if (orientation == 2)
    {
        orientation = 0;
    }
    else if (orientation == 3)
    {
        orientation = 1;
    }
    Serial.println(orientation);
}

void TournerGauche180()
{
    MOTOR_SetSpeed(RIGHT, -0.2);
    MOTOR_SetSpeed(LEFT, 0.2);

    delay(2200); //Environ 90 degrés

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    if (orientation == 1)
    {
        orientation = 3;
    }
    else if (orientation == 0)
    {
        orientation = 2;
    }
    else if (orientation == 2)
    {
        orientation = 0;
    }
    else if (orientation == 3)
    {
        orientation = 1;
    }
    Serial.println(orientation);
}

void positionnerBalle(int balleX, int balleY)
{
    Serial.println("Fonction positionnerBalle :");
    //Ce while fait rejoindre la colonne en question
    while (positionX != balleX)
    {
        Suivre_la_ligne();
        CalculerPositionX();

        Serial.print("BalleX :");
        Serial.println(balleX);
        Serial.print("positionX :");
        Serial.println(positionX);
        Serial.print("BalleY :");
        Serial.println(balleY);
        Serial.print("positionY :");
        Serial.println(positionY);
    }
    //Ce if fait tourner selon le besoin
    if (positionY < balleY)
    {
        if (orientation == 1)
        {
            TournerDroite90();
        }
        if (orientation == 3)
        {
            TournerGauche90();
        }
    }
    else if (positionY > balleY)
    {
        if (orientation == 1)
        {
            TournerGauche90();
        }
        if (orientation == 3)
        {
            TournerDroite90();
        }
    }
    //Ce while faite avancer jusqu'à la ligne en question
    while (positionY != balleY)
    {
        Suivre_la_ligne();
        CalculerPositionY();
        Serial.print("positionX :");
        Serial.println(positionX);
        Serial.print("positionY :");
        Serial.println(positionY);
    }
}

void retourRobot()
{
    Serial.println("Nice");
    while (positionX > 1)
    {
        if (positionX <= 1)
        {
            break;
        }
        Suivre_la_ligne();
        CalculerPositionX();
    }
    Serial.println("Bonjour");

    if (positionY == 1)
    {
        TournerGauche90();
        Suivre_la_ligne();
        TournerDroite90();
        Suivre_la_ligne();
    }
    else if (positionY == 2)
    {
        TournerDroite90();
        Suivre_la_ligne();
        TournerGauche90();
        Suivre_la_ligne();
    }

    else if (positionY == 1 && orientation == 0)
    {
        TournerDroite180();
        Suivre_la_ligne();
        TournerDroite90();
        Suivre_la_ligne();
    }

    else if (positionY == 2 && orientation == 2)
    {
        TournerDroite180();
        Suivre_la_ligne();
        TournerDroite90();
        Suivre_la_ligne();
    }
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);

    delay(5000);

    Serial.println("180 degrés");
    TournerGauche180();
}

void commandeLED(int difficulte , int mode)
{

    Serial.print( difficulte);
    Serial.print(" ");
    Serial.println(mode);

    digitalWrite(PINLEDMODE , mode);
    if(difficulte == 0)
    {
        digitalWrite(PINFACILE , 1);
        digitalWrite(PINMOYEN , 1);
        digitalWrite(PINDURE , 1);
    }
    else if(difficulte == 1)
    {
        digitalWrite(PINFACILE , 1);
        digitalWrite(PINMOYEN , 0);
        digitalWrite(PINDURE , 0);
    }
    else if(difficulte == 2)
    {
        digitalWrite(PINFACILE , 0);
        digitalWrite(PINMOYEN , 1);
        digitalWrite(PINDURE , 0);
    }
    else if(difficulte == 3)
    {
        digitalWrite(PINFACILE , 0);
        digitalWrite(PINMOYEN , 0);
        digitalWrite(PINDURE , 1);
    }
}

void LancerPartie(int difficultee)
{
    Serial.println("Fonction LancerPartie :");

    positionX = 0;
    positionY = 0;
    orientation = 1;

    JEU balles;

    balles = GetPositionBalles(difficultee);

    Serial.println(balles.blancheX);
    Serial.println(balles.blancheY);
    Serial.println(balles.couleurX);
    Serial.println(balles.couleurY);

    commandeLED(difficultee + 1, 1);

    Suivre_la_ligne(); //Changera pour suivre la ligne
    positionX++;

    TournerGauche90();

    Suivre_la_ligne();
    positionY++;

    TournerDroite90();

    delay(3000);

    if (balles.blancheX > balles.couleurX)
    {
        positionnerBalle(balles.blancheX, balles.blancheY);

        if (balles.couleurX < positionX)
        {
            if (orientation == 2)
            {
                TournerDroite90();
            }
            else if (orientation == 1)
            {
                TournerDroite180();
            }
        }
        //Faire code pour lever le servoMoteur
        SERVO_SetAngle(servoMoteurDroit, 120);

        positionnerBalle(balles.couleurX, balles.couleurY);

        if (orientation == 0)
        {
            TournerGauche90();
        }
        else if (orientation == 2)
        {
            TournerDroite90();
        }
        //Faire code pour lever le servoMoteur
        SERVO_SetAngle(servoMoteurGauche, 60);
    }
    else if (balles.blancheX < balles.couleurX)
    {
        positionnerBalle(balles.couleurX, balles.couleurY);

        if (balles.blancheX < positionX)
        {
            if (orientation == 2)
            {
                TournerDroite90();
            }
            else if (orientation == 1)
            {
                TournerDroite180();
            }
        }
        //Faire code pour lever le servoMoteur
        SERVO_SetAngle(servoMoteurDroit, 120);

        delay(3000);
        positionnerBalle(balles.blancheX, balles.blancheY);

        if (orientation == 0)
        {
            TournerGauche90();
        }
        else if (orientation == 2)
        {
            TournerDroite90();
        }
        //Faire code pour lever le servoMoteur
        //MoveServoMoteur(servoMoteurGauche, 90);
        SERVO_SetAngle(servoMoteurGauche, 60);
    }
    else if (balles.blancheY < balles.couleurY)
    {
        positionnerBalle(balles.blancheX, balles.blancheY);

        TournerDroite90();
        //Faire code pour lever le servoMoteur
        //MoveServoMoteur(servoMoteurGauche, 90);
        SERVO_SetAngle(servoMoteurDroit, 120);

        positionnerBalle(balles.couleurX, balles.couleurY);

        if (orientation == 0)
        {
            TournerGauche90();
        }
        else if (orientation == 2)
        {
            TournerDroite90();
        }
        SERVO_SetAngle(servoMoteurGauche, 60);
    }
    else if (balles.blancheY > balles.couleurY)
    {
        positionnerBalle(balles.couleurX, balles.couleurY);

        TournerDroite90();
        //Faire code pour lever le servoMoteur
        SERVO_SetAngle(servoMoteurDroit, 120);

        positionnerBalle(balles.blancheX, balles.blancheY);

        if (orientation == 0)
        {
            TournerGauche90();
        }
        else if (orientation == 2)
        {
            TournerDroite90();
        }
        SERVO_SetAngle(servoMoteurGauche, 60);
    }

    retourRobot();
}

bool buttonPress(int analogButtonInput)
{
    int press = analogRead(analogButtonInput);

    if (press == 1023)
    {
        int outputPress = 1023;

        while (outputPress >= 1000)
        {
            outputPress = analogRead(analogButtonInput);
            Serial.println(outputPress);
        }
        return true;
    }
    return false;
}

int choixDifficulte()
{
    int start = 0;
    int difficulte = 0;

    commandeLED(0,0);
    while (!start)
    {
        start = buttonPress(inputButtonStart);

        bool buttonDifficulteTrigger = buttonPress(inputButtonDifficulte);

        if (buttonDifficulteTrigger)
        {
            difficulte++;
            commandeLED((difficulte % 3) + 1, 0);
        }
    }
    
    commandeLED((difficulte % 3) + 1, 1);
    return difficulte % 3;
}

void InitialisationCapteurDeLigne()
{
    valeurMaxDroit = 0;
    valeurMaxGauche = 0;
    valeurMaxCentre = 0;

    while (valeurMaxDroit < 150 && valeurMaxGauche < 150 && valeurMaxCentre < 150)
    {
        valeurMaxDroit = analogRead(IRDroitePin) * 0.8;
        valeurMaxGauche = analogRead(IRGauchePin) * 0.8;
        valeurMaxCentre = analogRead(IRCentrePin) * 0.8;

        Serial.println(analogRead(IRDroitePin));
        Serial.println(analogRead(IRGauchePin));
        Serial.println(analogRead(IRCentrePin));
    }
    Serial.println("Yes");
}

/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

void setup()
{
    BoardInit();
    //pinMode(LED_BUILTIN, OUTPUT);
    pinMode(11, INPUT);
    pinMode(12, INPUT);
    pinMode(13, INPUT);
    pinMode(14, INPUT);
    pinMode(15, INPUT);

    pinMode(PINDURE ,OUTPUT);
    pinMode(PINMOYEN ,OUTPUT);
    pinMode(PINFACILE ,OUTPUT);
    pinMode(PINLEDMODE ,OUTPUT);

    Serial.begin(9600);

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    MoveServoMoteur(servoMoteurGauche, 90);
    MoveServoMoteur(servoMoteurDroit, 90);
}

/* ****************************************************************************
Fonctions de boucle infini (nloop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop()
{
    /*
    delay(5000);

    SERVO_SetAngle(servoMoteurDroit, 120);
    SERVO_SetAngle(servoMoteurGauche, 60);

    delay(5000);*/

    int difficulte = choixDifficulte();

    SERVO_SetAngle(servoMoteurDroit, 20);
    SERVO_SetAngle(servoMoteurGauche, 150);

    //InitialisationCapteurDeLigne();
    Serial.println("Droite : ");
    Serial.println(analogRead(IRDroitePin));
    Serial.println("Gauche : ");
    Serial.println(analogRead(IRGauchePin));
    Serial.println("Centre : ");
    Serial.println(analogRead(IRCentrePin));

    delay(2000);

    LancerPartie(difficulte);
}