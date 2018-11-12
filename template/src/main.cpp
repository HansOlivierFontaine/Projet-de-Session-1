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
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
#define comptePulseTheo 3200;
double speedGauche = 0.42;
double speedDroite = 0.4;

//#define PI 3.1415926
// -> defines...
// L'ensemble des fonctions y ont acces

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */

void Acceleration()
{
    for (int i = 1; i <= 100; i++)
    {
        float pourcentage = i * 0.01;

        MOTOR_SetSpeed(RIGHT , speedDroite * pourcentage);
        MOTOR_SetSpeed(LEFT, speedGauche * pourcentage);
        delay(30);
    }
}

void Decceleration()
{
    for (int i = 100; i >= 1; i--)
    {
        float pourcentage = i * 0.01;

        MOTOR_SetSpeed(RIGHT , speedDroite * pourcentage);
        MOTOR_SetSpeed(LEFT, speedGauche * pourcentage);
        delay(30);
    }
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
    MOTOR_SetSpeed(LEFT, 0.4);
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
        comptePulse = ENCODER_Read(LEFT);
        delay(10);
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
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
}

/* ****************************************************************************
Fonctions de boucle infini (nloop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop()
{
}
