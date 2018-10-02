
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

long Counter = 0;
float circumference = 3.1415926 * 7.5;
float CenterCircumference = 3.1415926 * 220;
float Wheelcircumference = 3.1415926 * 75;
long GearParCm = 32000 / Wheelcircumference;
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
// -> defines...
// L'ensemble des fonctions y ont acces

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
void Avancer_Cm(int input)
{

    int distance = input * 1.10;
    long GearParCm = 32000 / circumference;
    long GearCount = (GearParCm * (distance));
    GearCount = GearCount / 10;

    Serial.print("START");
    ENCODER_Reset(LEFT);
    MOTOR_SetSpeed(LEFT, 0.5);
    MOTOR_SetSpeed(RIGHT, 0.5);
    while (Counter < GearCount)
    {
        Counter = ENCODER_Read(LEFT);
        delay(5);
    }
    MOTOR_SetSpeed(LEFT, 0.0);
    MOTOR_SetSpeed(RIGHT, 0.0);

    Serial.print("END");
    delay(200);
}

void Tourne_Deg(int degree)
{
    int Turn = degree;
    long GearCount = (GearParCm * (Turn));
    GearCount = GearCount / 10;

    //don't do it!
    MOTOR_SetSpeed(RIGHT, -0.2);
    MOTOR_SetSpeed(LEFT, 0.2);
    delay(700);

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

void Tourne(int Dir, int delai)
{
    if (Dir == RIGHT)
    {
        MOTOR_SetSpeed(RIGHT, 0);
        MOTOR_SetSpeed(LEFT, 0.6);
        delay(delai);

        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
    }
    else if (Dir == LEFT)
    {
        MOTOR_SetSpeed(RIGHT, 0.6);
        MOTOR_SetSpeed(LEFT, 0);
        delay(delai);

        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
    }
}

/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

//Initiation des variables globales
void setup()
{
    BoardInit();
    //Initiation
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop()
{
    // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
    delay(2000);
    Avancer_Cm(190);

    //DEBUT DU U
    Tourne(LEFT, 680); //90 degree

    Avancer_Cm(30);

    Tourne(RIGHT, 680); //90 degree

    Avancer_Cm(25);

    Tourne(RIGHT, 680); //90 degree

    Avancer_Cm(30);
    //FIN DU U

    Tourne(LEFT, 360); //45 degree

    Avancer_Cm(50);

    Tourne(LEFT, 680); //90 degree

    Avancer_Cm(50);

    Tourne(RIGHT, 360); //45 degree

    Avancer_Cm(30);

    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0.6);
    delay(250);

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    Avancer_Cm(75);

    //Tourne_Deg(90);
    delay(5000);
}
