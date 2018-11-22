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
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
#define comptePulseTheo 3200
#define PINBLANCHE 6
#define PINCOULEUR 7
#define IRInterieurPin 8
#define IRDevantPin 4    // lect 2
#define IRExterieurPin 3 //lect 1
double speedGauche = 0.42;
double speedDroite = 0.4;
#include <time.h>
#include <stdlib.h>
int servoMoteurDroit = 1;
int servoMoteurGauche = 0;
int inputButtonStart = 6;
int inputButtonDifficulte = 5;
int count = 1;

//Lumière
#define LED_PIN 10
#define NUM_LEDS 120
#define BRIGHTNESS 100
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

//#define PI 3.1415926
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

void Acceleration()
{
    for (int i = 1; i <= 100; i++)
    {
        float pourcentage = i * 0.01;

        MOTOR_SetSpeed(RIGHT, speedDroite * pourcentage);
        MOTOR_SetSpeed(LEFT, speedGauche * pourcentage);
        delay(30);
    }
}

void Decceleration()
{
    for (int i = 100; i >= 1; i--)
    {
        float pourcentage = i * 0.01;

        MOTOR_SetSpeed(RIGHT, speedDroite * pourcentage);
        MOTOR_SetSpeed(LEFT, speedGauche * pourcentage);
        delay(30);
    }
}

void MoveServoMoteur(int servo, int angle)
{
    //Les meilleurs valeurs sont 10 et 170 degrés pour tenir les balles et de 90 degrés pour les relaché
    SERVO_SetAngle(servo, angle);
}

int lireDistanceInfraRobot(int sensor)
{
    return ROBUS_ReadIR(sensor);
}

void Avancer_Cm(int distance)
{
    long compteur = 0;

    float circonference = 3.1415926 * 7.5;
    long pulseParCm = 32000 / circonference;
    long comptePulse = (pulseParCm * (distance));
    comptePulse = comptePulse / 10;

    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);

    speedGauche = 0.42;
    speedDroite = 0.4;
    Acceleration();
    while (compteur < comptePulse)
    {
        delay(5);
        compteur = ENCODER_Read(RIGHT);
    }
    Decceleration();

    delay(200);
}

void Reculer_Cm(int distance)
{
    long compteur = 0;

    float circonference = 3.1415926 * 7.5;
    long pulseParCm = 32000 / circonference;
    long comptePulse = (pulseParCm * (distance));
    comptePulse = comptePulse / 10;

    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);

    speedGauche = -0.42;
    speedDroite = -0.4;
    Acceleration();
    while (compteur < comptePulse)
    {
        delay(5);
        compteur = ENCODER_Read(RIGHT);
    }
    Decceleration();

    delay(200);
}

void Tourne_Gauche()
{
    int comptePulse;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(RIGHT, 0.4);
    MOTOR_SetSpeed(LEFT, 0);

    comptePulse = 0;
    while (comptePulse <= (1.18 * 3200))
    {
        comptePulse = ENCODER_Read(RIGHT);
        delay(10);
    }

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

void Tourne_Droite()
{
    int comptePulse;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0.2);
    comptePulse = 0;
    while (comptePulse <= (1.18 * 3200))
    {
        comptePulse = ENCODER_Read(LEFT);
        delay(10);
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

//À modifier pour que le robot tourne sur lui-même
void Demi_Tour()
{
    int comptePulse;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0.4);
    comptePulse = 0;
    while (comptePulse <= (1.18 * 3200))
    {
        comptePulse = ENCODER_Read(LEFT);
        delay(10);
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(RIGHT, 0.4);
    MOTOR_SetSpeed(LEFT, 0);
    comptePulse = 0;
    while (comptePulse <= (1.18 * 3200))
    {
        comptePulse = ENCODER_Read(RIGHT);
        delay(10);
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

// int IRDevantPin=8;

void suivre_la_ligne()
{
    static int derniereDirectionEnregistree = 0;
    Serial.println("Fonction suivre_la_ligne");
    
    int lectureInitiale;

    if (analogRead(IRDevantPin) < 600)
    {
        lectureInitiale = 0;
        MOTOR_SetSpeed(LEFT, 0.);
        MOTOR_SetSpeed(RIGHT, 0.2);
        while (analogRead(IRDevantPin) < 600)
            delay(50);
    }
    else
        lectureInitiale = 1;

    if ((analogRead(IRDevantPin) < 600) && derniereDirectionEnregistree == 1)
    {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0.2);
        while (analogRead(IRDevantPin) < 600)
            delay(50);
    }

    while (analogRead(IRExterieurPin) * 1.5 < 400)
    {

        MOTOR_SetSpeed(LEFT, 0.2);
        MOTOR_SetSpeed(RIGHT, 0);

        derniereDirectionEnregistree = 1;

        if (lectureInitiale == 1)
        {
            while (analogRead(IRDevantPin) > 600)
            {
                delay(50);
            }
        }
        else
        {
            while (analogRead(IRDevantPin) < 600)
            {
                delay(50);
            }
        }

        if (analogRead(IRExterieurPin) * 1.5 > 400)
        {
            break;
        }

        MOTOR_SetSpeed(LEFT, 0.);
        MOTOR_SetSpeed(RIGHT, 0.2);

        derniereDirectionEnregistree = 0;

        if (lectureInitiale == 1)
        {
            while (analogRead(IRDevantPin) < 600)
            {
                delay(50);
            }
        }
        else
        {
            while (analogRead(IRDevantPin) > 600)
            {
                delay(50);
            }
        }
    }
        /*
        int lecture = 1000;
        lecture = analogRead(IRInterieurPin);
        
            int lecture1, lecture2;
            lecture =  analogRead(IRInterieurPin);
            delay (50);
            lecture1 =  analogRead(IRExterieurPin);
            delay (50);
            lecture2 =  analogRead(IRDevantPin);
            delay (50);
             
            Serial.print("lecture :");
            Serial.println(lecture );
            Serial.print("lecture1 : ");
            lecture1= lecture1*1.5;
            Serial.println(lecture1 );
            Serial.print("lecture2 : ");
            Serial.println(lecture2 );
            delay (2000);
    
    delay(100);
    MOTOR_SetSpeed(LEFT, 0.);
    MOTOR_SetSpeed(RIGHT, 0.);
    delay(1000);*/
}

JEU getPositionBalles(int difficulte)
{
    int nbrAleatoire;
    JEU balles;

    int facile[6][2][2] = {{{1, 1}, {0, 0}},
                           {{1, 1}, {0, 2}},
                           {{3, 1}, {4, 0}},
                           {{3, 1}, {4, 2}},
                           {{1, 1}, {4, 0}},
                           {{3, 1}, {0, 2}}};

    int moyen[4][2][2] = {{{1, 1}, {2, 0}},
                          {{3, 1}, {2, 2}},
                          {{3, 2}, {4, 1}},
                          {{1, 0}, {0, 1}}};

    int difficile[4][2][2] = {{{0, 1}, {3, 1}},
                              {{0, 1}, {4, 1}},
                              {{4, 1}, {0, 1}},
                              {{3, 1}, {1, 1}}};

    difficulte = 1;
    if (difficulte == 0)
    {
        nbrAleatoire = rand() % 6;
        Serial.println(nbrAleatoire);
        balles.blancheX = facile[nbrAleatoire][0][0];
        balles.blancheY = facile[nbrAleatoire][0][1];
        balles.couleurX = facile[nbrAleatoire][1][0];
        balles.couleurY = facile[nbrAleatoire][1][1];
        //arrayARetourner={{facile[nbrAleatoire][0][0], facile[nbrAleatoire][0][1]}, {facile[nbrAleatoire][1][0], facile[nbrAleatoire][1][1]}};
        return balles;
    }
    else if (difficulte == 1)
    {
        nbrAleatoire = rand() % 4;
        nbrAleatoire = 2;
        balles.blancheX = moyen[nbrAleatoire][0][0];
        balles.blancheY = moyen[nbrAleatoire][0][1];
        balles.couleurX = moyen[nbrAleatoire][1][0];
        balles.couleurY = moyen[nbrAleatoire][1][1];
        //arrayARetourner={{moyen[nbrAleatoire][0][0], moyen[nbrAleatoire][0][1]},{moyen[nbrAleatoire][1][0], moyen[nbrAleatoire][1][1]}};
        return balles;
    }
    else
    {
        nbrAleatoire = rand() % 4;
        balles.blancheX = difficile[nbrAleatoire][0][0];
        balles.blancheY = difficile[nbrAleatoire][0][1];
        balles.couleurX = difficile[nbrAleatoire][1][0];
        balles.couleurY = difficile[nbrAleatoire][1][1];
        //arrayARetourner={{difficile[nbrAleatoire][0][0], difficile[nbrAleatoire][0][1]},{difficile[nbrAleatoire][1][0], difficile[nbrAleatoire][1][1]}};
        return balles;
    }
}

void rejoindreLigne(/*int *orientaion*/)
{
    Serial.println("Fonction rejoindreLigne :");
    MOTOR_SetSpeed(LEFT, 0.2);
    MOTOR_SetSpeed(RIGHT, 0.2);
    while (analogRead(IRExterieurPin * 1.5 < 400))
    {
        delay(50);
    }
}

void CalculerPositionX(int *positionX, int *orientation)
{
    Serial.println("Fonction CalculerPositionX :");
    if (*orientation == 0)
    {
        *positionX = *positionX + 1;
    }
    else if (*orientation == 2)
    {
        *positionX = *positionX - 1;
    }
    else
    {
        exit(1);
    }
}

void CalculerPositionY(int *positionY, int *orientation)
{
    Serial.println("Fonction CalculerPositionY :");
    if (*orientation == 1)
    {
        *positionY = *positionY + 1;
    }
    else if (*orientation == 3)
    {
        *positionY = *positionY - 1;
    }
    else
    {
        exit(1);
    }
}

void tournerDroite(int *orientation)
{
    Serial.println("Fonction tournerDroite :");
    MOTOR_SetSpeed(LEFT, 0.1);
    MOTOR_SetSpeed(RIGHT, -0.2);
    delay(500);
    int hasALine = 0;
    while (hasALine < 600)
    {
        hasALine = analogRead(IRDevantPin);
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    *orientation = (*orientation + 1) % 4;
}

void tournerGauche(int *orientation)
{
    Serial.println("Fonction tournerGauche :");
    MOTOR_SetSpeed(LEFT, -0.2);
    MOTOR_SetSpeed(RIGHT, 0.1);
    delay(500);
    int hasALine = 0;
    while (hasALine < 600)
    {
        hasALine = analogRead(IRDevantPin);
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    *orientation = (*orientation + 1) % 4;
}

void positionnerBalle(int balleX, int balleY, int *positionX, int *positionY, int *orientation)
{
    Serial.println("Fonction positionnerBalle :");
    //rejoindreLigne(orientation);
    //Ce while fait rejoindre la colonne en question
    do
    {
        suivre_la_ligne();
        CalculerPositionX(positionX, orientation);
        Serial.print("BalleX :");
        Serial.println(balleX);
        Serial.print("positionX :");
        Serial.println(*positionX);
        Serial.print("BalleY :");
        Serial.println(balleY);
        Serial.print("positionY :");
        Serial.println(*positionY);
    } while (*positionX != balleX);
    //Ce if fait tourner selon le besoin
    if (*positionY < balleY)
    {
        while (*orientation != 1)
        {
            tournerDroite(orientation);
            Serial.print("orientation :");
            Serial.println(*orientation);
        }
    }
    else if (*positionY > balleY)
    {
        while (*orientation != 3)
        {
            tournerDroite(orientation);
        }
    }
    //Ce while faite avancer jusqu'à la ligne en question
    do
    {
        suivre_la_ligne();
        CalculerPositionY(positionY, orientation);
        Serial.print("positionX :");
        Serial.println(*positionX);
        Serial.print("positionY :");
        Serial.println(*positionY);
    } while (*positionY != balleY);
}

void retourRobot(int *positionX, int *positionY, int *orientation)
{
    while (*orientation != 3)
    {
        tournerDroite(orientation);
    }
    while (*positionY != 0)
    {
        suivre_la_ligne();
        CalculerPositionY(positionY, orientation);
    }
    tournerGauche(orientation);
    //AvancerDroit();
    //signal d'entree sur la plateforme
    //Arrêter robot
}

void lancerPartie(int difficultee)
{
    Serial.println("Fonction lancerPartie :");
    int x = -1;
    int y = 1;
    int orientationReal = 0;
    int *positionX;
    int *positionY;
    int *orientation;

    positionX = &x;
    positionY = &y;
    orientation = &orientationReal;

    JEU balles;

    balles = getPositionBalles(difficultee);

    Serial.println(balles.blancheX);
    Serial.println(balles.blancheY);
    Serial.println(balles.couleurX);
    Serial.println(balles.couleurY);

    rejoindreLigne();

    if (balles.blancheX > balles.couleurX)
    {
        positionnerBalle(balles.blancheX, balles.blancheY, positionX, positionY, orientation);
        tournerDroite(orientation);
        if (balles.couleurX == balles.blancheX)
        {
            tournerDroite(orientation);
        }
        //SERVO_SetAngle(PINBLANCHE, 30);
    }

    positionnerBalle(balles.couleurX, balles.couleurY, positionX, positionY, orientation);
    SERVO_SetAngle(PINCOULEUR, -30);

    if (balles.blancheX < balles.couleurX)
    {
        positionnerBalle(balles.blancheX, balles.blancheY, positionX, positionY, orientation);
        tournerDroite(orientation);
        if (balles.couleurX == balles.blancheX)
        {
            tournerDroite(orientation);
        }
        //SERVO_SetAngle(PINBLANCHE, 30);
    }

    retourRobot(positionX, positionY, orientation);
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

void LEDSTRIP_Level(int level)
{
    int color;

    if (level == 0)
        color = CRGB::Blue;
    if (level == 1)
        color = CRGB::Yellow;
    if (level == 2)
        color = CRGB::Pink;

    memset(leds, 0, NUM_LEDS * 3);
    for (int i = 0; i < 60; i++)
    {

        //leds[i-1] = CRGB::Black;
        leds[i] = color;

        FastLED.show();
        delay(10);
    }

    for (int i = BRIGHTNESS; i >= 0; i--)
    {
        FastLED.setBrightness(i);
        FastLED.show();
        delay(10);
    }

    FastLED.setBrightness(BRIGHTNESS);
}

int choixDifficulte()
{
    int start = 0;
    int difficulte = 0;

    while (!start)
    {
        start = buttonPress(inputButtonStart);

        bool buttonDifficulteTrigger = buttonPress(inputButtonDifficulte);

        if (buttonDifficulteTrigger)
        {
            difficulte++;
            Serial.println(difficulte % 3);
            LEDSTRIP_Level(difficulte % 3);
        }
    }
    return difficulte;
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
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    MoveServoMoteur(servoMoteurGauche, 10);
    MoveServoMoteur(servoMoteurDroit, 170);
}

/* ****************************************************************************
Fonctions de boucle infini (nloop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop()
{

    //Attend le choix de difficulté de l'utilisateur
    int difficulte = choixDifficulte() % 3;

    lancerPartie(difficulte);
    //suivre_la_ligne();

    /*int distance = 0;
    while (true)
    {
        MOTOR_SetSpeed(RIGHT, 0.31);
        MOTOR_SetSpeed(LEFT, 0.3);

        distance = lireDistanceInfraRobot(0);

        if (distance > 250)
        {
            if (count == 1)
            {
                MOTOR_SetSpeed(RIGHT, 0);
                MOTOR_SetSpeed(LEFT, 0);
                delay(3000);

                Tourne_Droite();

                MoveServoMoteur(servoMoteurGauche, 90);

                delay(1000);

                MOTOR_SetSpeed(RIGHT, 0.1);
                MOTOR_SetSpeed(LEFT, 0.1);
                delay(1500);

                MOTOR_SetSpeed(RIGHT, 0);
                MOTOR_SetSpeed(LEFT, 0);

                MoveServoMoteur(servoMoteurGauche, 10);
            }
            else if (count == 2)
            {
                MOTOR_SetSpeed(RIGHT, 0);
                MOTOR_SetSpeed(LEFT, 0);
                delay(3000);

                Tourne_Droite();
            }
            else if (count == 3)
            {
                MOTOR_SetSpeed(RIGHT, 0);
                MOTOR_SetSpeed(LEFT, 0);
                delay(3000);

                Tourne_Droite();

                MoveServoMoteur(servoMoteurDroit, 90);

                delay(1000);

                MOTOR_SetSpeed(RIGHT, 0.1);
                MOTOR_SetSpeed(LEFT, 0.1);
                delay(1500);

                MOTOR_SetSpeed(RIGHT, 0);
                MOTOR_SetSpeed(LEFT, 0);

                MoveServoMoteur(servoMoteurDroit, 170);
            }
            else if (count == 4)
            {
                MOTOR_SetSpeed(RIGHT, 0);
                MOTOR_SetSpeed(LEFT, 0);
                delay(3000);

                Tourne_Droite();
                delay(30000);
            }
            count++;
        }
    }*/
}