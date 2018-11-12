
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
#include <signal.h>
#include "capteur.h"
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
#define comptePulseTheo 3200 ;
double speedGauche = 0.625;
double speedDroite = 0.6;
int delayEntreMouvement = 200;

//#define PI 3.1415926
// -> defines...
// L'ensemble des fonctions y ont acces

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */

void Acceleration()
{
for(int i=0; i<100; i++)
{
MOTOR_SetSpeed(RIGHT, i/100);
MOTOR_SetSpeed(LEFT, i/100);
}
}

void Decceleration()
{
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.7);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.7);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.5);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.5);
    delay(60);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.3);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.3);
    delay(60);
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}





void PIDParMoteur(int valeurCunt)
{ 
    int  comptePulseG = 0;
    int comptePulseD = 0;
    int  comptePulseGTot = 0;
    int comptePulseDTot = 0;
    int nbCycle =1 ;
    

    float compteErreurDTot = 0;
    float compteErreurGTot = 0;
    float compteErreurD = 0;
    float compteErreurG = 0;

    int comptePulseTheoCycle= 0 ;
     
    // 1 seconde
    
    comptePulseD = ENCODER_Read(RIGHT);
    comptePulseG = ENCODER_Read(LEFT);
    
    comptePulseDTot+=   comptePulseD;
    comptePulseGTot+= comptePulseG;

    delay(10);

    compteErreurD = valeurCunt - comptePulseD;
    compteErreurG = valeurCunt - comptePulseG;

    comptePulseTheoCycle = nbCycle*valeurCunt;


    compteErreurDTot = comptePulseTheoCycle - comptePulseDTot;
    speedDroite+= (compteErreurDTot * 0.00002)+(compteErreurD * 0.0001) ;
    
    compteErreurGTot = comptePulseTheoCycle - comptePulseGTot;
    speedGauche+= (compteErreurGTot * 0.00002)+(compteErreurG * 0.0001) ;
 

    nbCycle++;
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);

}
int lireDistanceInfraRobot(int sensor)
{
    ROBUS_ReadIR(sensor);
}

void pognerBallon()
{}

void signalHandlerDistance(int signalSent)
{
}

void mouvementAttaquant()
{
    float captH = lireDistanceInfraRobot();
    float captB = lireDistanceInfraRobot();

   if(captB <= captH-20)
    {
        MOTOR_SetSpeed(RIGHT, 0.5);
        MOTOR_SetSpeed(LEFT, 0.5);

        while(captB > 5){
            delay(10);
            captB = lireDistanceInfraRobot();
        };

        while(captH > 15 || couleur == blanc || couleur == noir){};

        if(couleur != blanc && couleur != noir)
        {
            if(couleur == vert)
            {
                Tourne90;
                Tourne90;
                MOTOR_SetSpeed(RIGHT, 0.5);
                MOTOR_SetSpeed(LEFT, 0.5);
                while(captH > 15 || couleur == blanc || couleur == noir){};
            }
            while(couleur != noir)
            {
                delay(10);
            };
            MOTOR_SetSpeed(RIGHT, -0.5);
            MOTOR_SetSpeed(LEFT, -0.5);
            delay(2000);
            MOTOR_SetSpeed(RIGHT, 0.0);
            MOTOR_SetSpeed(LEFT, 0.0);
        }
    }
    else if (captH < 50 && captB < 50)
    {
        compteur = 0;

        MOTOR_SetSpeed(RIGHT, 0.5);
        MOTOR_SetSpeed(LEFT, 0.5);

        /*
        while(captH < 50 && captB < 50 && compteur < 200)
        {
            delay(10);
            compteur += 1;
        }

        MOTOR_SetSpeed(RIGHT, -0.5);
        MOTOR_SetSpeed(LEFT, -0.5);

        Tourne45;
        */

    }else if(couleur == noir)
    {
        MOTOR_SetSpeed(RIGHT, -1.0);
        MOTOR_SetSpeed(LEFT, -1.0);
        delay(2000);
    }else
    {
        MOTOR_SetSpeed(RIGHT, 0.5);
        MOTOR_SetSpeed(LEFT, 0.5);
        delay(250);
        Tourne45;
    }
    delay(1000);


}

void Avancer_Cm(int distance)
{
    long Compteur = 0;

    float circonference = 3.1415926 * 7.5;
    long PulseParCm = 32000 / circonference;
    long ComptePulse = (PulseParCm * (distance));
    ComptePulse = ComptePulse / 10;

    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    speedGauche = 0.625;
    speedDroite = 0.6;
    Acceleration();
    int temps = 0;
    while (Compteur < ComptePulse)
    {
        if(temps >= 500)
        {
          
           PIDParMoteur(3200);
            temps = 0; 
        }
        else
            temps +=5;

        delay(5);    
        Compteur = ENCODER_Read(RIGHT);
       //PIDBasic();
    }
    Decceleration();

    delay(200);
}


void Tourne45(int direction)
{
    int comptePulse;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    if (direction == RIGHT)
    {
        MOTOR_SetSpeed(RIGHT, 0);
        MOTOR_SetSpeed(LEFT, 0.6);
        comptePulse = 0;
        while (comptePulse <= (3200 * 0.6))
        {
            comptePulse = ENCODER_Read(LEFT);
            delay(10);
        }
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
    }
    else if (direction == LEFT)
    {
        MOTOR_SetSpeed(RIGHT, 0.6);
        MOTOR_SetSpeed(LEFT, 0);
        comptePulse = 0;
        while (comptePulse <= (3200 * 0.6))
        {
            comptePulse = ENCODER_Read(RIGHT);
            delay(10);
        }
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
    }
}

void Tourne90(int direction)
{
    int comptePulse;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    if (direction == RIGHT)
    {
        MOTOR_SetSpeed(RIGHT, 0);
        MOTOR_SetSpeed(LEFT, 0.6);
        comptePulse = 0;
        while (comptePulse <= (3200 * 1.2))
        {
            comptePulse = ENCODER_Read(LEFT);
            delay(10);
        }
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
    }
    else if (direction == LEFT)
    {
        MOTOR_SetSpeed(RIGHT, 0.6);
        MOTOR_SetSpeed(LEFT, 0);
        comptePulse = 0;
        while (comptePulse <= (3200 * 1.2))
        {
            comptePulse = ENCODER_Read(RIGHT);
            delay(10);
        }
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

void setup()
{
    BoardInit();
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
}

/* ****************************************************************************
Fonctions de boucle infini (loop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop()
{
    int iSecret = rand() % 1 + 1;

    serial.println(iSecret);

    /*Bouton bouton;
    while(!bouton)
    {
        delay(10);
    }
    dealy(5000);
    signal(1, signalHandlerDistance);
    mouvementAttaquant();*/
    /*
       int comptePulse = 0;
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
   
    delay(2000);
    
    Avancer_Cm(200);
    delay(delayEntreMouvement);
    //DEBUT DU U
    
    Tourne90(LEFT);
    
    delay(delayEntreMouvement);
    Avancer_Cm(30);
    delay(delayEntreMouvement);

    //LIGNE BASSE U
    Tourne90(RIGHT);
    delay(delayEntreMouvement);
    Avancer_Cm(20);
    delay(delayEntreMouvement);


    Tourne90(RIGHT);
    delay(delayEntreMouvement);
    Avancer_Cm(35);
    //FIN DU U 
    delay(delayEntreMouvement);

   
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
        MOTOR_SetSpeed(RIGHT, 0.6);
        MOTOR_SetSpeed(LEFT, 0);
        comptePulse = 0;
        while(comptePulse <= (3200*0.8) )
        {
            comptePulse = ENCODER_Read(RIGHT);
            delay(10);
        }
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0); 

    //Tourne45(LEFT);


    delay(delayEntreMouvement);
    Avancer_Cm(48);
    delay(delayEntreMouvement);

     ENCODER_Reset(LEFT);
     ENCODER_Reset(RIGHT);
        MOTOR_SetSpeed(RIGHT, 0.6);
        MOTOR_SetSpeed(LEFT, 0);
        comptePulse = 0;
        while(comptePulse <= (3200*0.85) )
        {
            comptePulse = ENCODER_Read(RIGHT);
            delay(10);
        }
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0); 
    //Tourne90(LEFT);

    delay(delayEntreMouvement);
    Avancer_Cm(43);
    delay(delayEntreMouvement);
    Tourne45(RIGHT);
    delay(delayEntreMouvement);
    Avancer_Cm(20); 

     ENCODER_Reset(LEFT);
     ENCODER_Reset(RIGHT);
        MOTOR_SetSpeed(RIGHT, 0);
        MOTOR_SetSpeed(LEFT, 0.6);
        comptePulse = 0;
        while(comptePulse <= (3200*0.1) )
        {
            comptePulse = ENCODER_Read(LEFT);
            delay(10);
        }
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0); 

    delay(delayEntreMouvement);
    Avancer_Cm(65);
   
   /* 
   delay(delayEntreMouvement);
    
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0.6);
    delay(290);
        
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    delay(500);
    
    Avancer_Cm(50);

    */
}
