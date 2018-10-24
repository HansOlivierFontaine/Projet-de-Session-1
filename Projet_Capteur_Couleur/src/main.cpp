/*
*CONTIENT :librairy du capteur
*MANQUE : transformation des couleurs, lecture couleurs, code Sifflet , séquence de départ
*FONCTIONNEL : S.O
*NON-FONCTIONNEL : tout
*NOTES: Refaire les déplacements (intégrer la direction avant/arriere)
*/

#include <LibRobus.h>
#include "capteur.h"

#define COULEUREQUIPE 2

#define comptePulseTheo 3200 
#define AVANT true
#define ARRIERE false

//compare R,G,B au valeurs pour detecter noir/blanc/couleur
#define NOIR 200
#define BLANC 850
#define ZONEBUT 0
#define ZONECOUL 1
#define ZONEJEU 2

/* VARIABLES GLOBALES */
double speedGauche = 0.6;
double speedDroite = 0.6;
int delayEntreMouvement = 200;
bool deplacement = ARRIERE;
int captCoul = COULEUREQUIPE;
int nbCycle =1 ;



void Acceleration()
{
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.1);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.1);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.3);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.3);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.5);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.5);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.7);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.7);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 1);
    MOTOR_SetSpeed(RIGHT, speedDroite * 1);
    delay(50);
}

void Decceleration()
{
    MOTOR_SetSpeed(LEFT,speedGauche * 0.6);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.6);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche *3);
    MOTOR_SetSpeed(RIGHT, speedDroite *3);
    delay(50);
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

void PIDParMoteur()
{ 
    int comptePulseG = 0;
    int comptePulseD = 0;
    int comptePulseGTot = 0;
    int comptePulseDTot = 0;

    float compteErreurDTot = 0;
    float compteErreurGTot = 0;
    float compteErreurD = 0;
    float compteErreurG = 0;

    int comptePulseTheoCycle= 0 ;
     
    // 1 seconde
    
    comptePulseD = ENCODER_Read(RIGHT) - (nbCycle*3200);
    comptePulseG = ENCODER_Read(LEFT) - (nbCycle*3200);
    
    comptePulseDTot+= comptePulseD;
    comptePulseGTot+= comptePulseG;

    delay(10);

    compteErreurD = comptePulseTheo - comptePulseD;
    compteErreurG = comptePulseTheo - comptePulseG;

    comptePulseTheoCycle = nbCycle*comptePulseTheo;


    compteErreurDTot = comptePulseTheoCycle - comptePulseDTot;
    speedDroite+= (compteErreurDTot * 0.00002)+(compteErreurD * 0.0001) ;
    
    compteErreurGTot = comptePulseTheoCycle - comptePulseGTot;
    speedGauche+= (compteErreurGTot * 0.00002)+(compteErreurG * 0.0001) ;
 

    nbCycle++;
}
void Avancer_Cm(int distance , int direction)
{
    long Compteur = 0;

    float circonference = 3.1415926 * 7.5;
    long PulseParCm = 32000 / circonference;
    long ComptePulse = (PulseParCm * (distance));
    ComptePulse = ComptePulse / 10;

    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    speedGauche = 0.6;
    speedDroite = 0.6;
    nbCycle = 0;
    int temps = 0;
    int prochainCycle= 3200;

    if(direction == ARRIERE)
    {
      speedDroite = speedDroite * -1;
      speedGauche = speedGauche * -1;
      ComptePulse = ComptePulse * -1 ;
    }
        Serial.print("C / G / D : " );
        Serial.print(ComptePulse);
        Serial.print(" " );
        Serial.print(speedGauche);
        Serial.print(" " );
        Serial.println(speedDroite);

       Acceleration(); 
    while (abs(Compteur) < abs(ComptePulse) && captCoul == COULEUREQUIPE)
    {
        //getCouleur();
        distance += 3;

       if(Compteur >= prochainCycle)
        {
            prochainCycle = Compteur + 3200;
            //PIDParMoteur();
            temps = 0;
        }
        
        delay(5);           

        Compteur = ENCODER_Read(RIGHT);
    }
      Decceleration();
}
void Tourne_Pulse(int direction, float NbPulse)
{
    int comptePulse;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    if (direction == RIGHT)
    {
        MOTOR_SetSpeed(RIGHT, 0);
        MOTOR_SetSpeed(LEFT, 0.6);
        comptePulse = 0;
        while (comptePulse <= (NbPulse*3200))
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
        while (comptePulse <= (NbPulse*3200))
        {
            comptePulse = ENCODER_Read(RIGHT);
            delay(10);
        }
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
    }
}

void setup()
{
  BoardInit();
  pinMode(ledPin, OUTPUT);  // Set the sensor's LED as output
  digitalWrite(ledPin, HIGH);  // Initially turn LED light source on

  //setupCapteur();
  /**/
}

void getCouleur()
{
    getRGBC();//les couleurs sont RGB (0-1023) dans colorData[3]
    printADJD_CUSTOM();
    delay(1000);

    if(colorData[0] < NOIR && colorData[3] < NOIR &&colorData[2] < NOIR )
    {
        Serial.println("Zone du but");
        captCoul = ZONEBUT;
    }
    else if(colorData[0] < BLANC && colorData[3] < BLANC &&colorData[2] < BLANC )
    {
        Serial.println("Zone de jeu");
        captCoul = ZONEJEU;
    }
    else
    { 
        Serial.println("Zone de défense");
        captCoul = ZONECOUL;
    }
}

// ||
void loop()
{
    int distance =0;
    delay(200);

    if(captCoul == COULEUREQUIPE) //si dans sa zone
    {
        Avancer_Cm(60 , deplacement);
        distance =0;
        deplacement = !deplacement; 
      //  Serial.println(deplacement);
    }
    else if(captCoul == ZONEBUT) //noir
    {
        deplacement = !deplacement;
        Avancer_Cm(10 , deplacement);
        delay(250);
        if(deplacement == AVANT)
            Tourne_Pulse(RIGHT,0.2);
        else
            Tourne_Pulse(LEFT,0.2);
    }
    else // blanc / ZONEJEU
    {
      deplacement = !deplacement;
      Avancer_Cm(10 , deplacement);
      delay(250);
      if(deplacement == AVANT)
      Tourne_Pulse(LEFT,0.2);
      else
      Tourne_Pulse(RIGHT,0.2);

      deplacement = !deplacement ; //continue son déplacement originel
    } 
}