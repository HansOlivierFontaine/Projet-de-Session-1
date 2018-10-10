
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

double speedGauche = 0.625;
double speedDroite = 0.6;
int delayEntreMouvement = 200;
int nbCycle =1 ;
#define comptePulseTheo 3200 ;
//#define PI 3.1415926
// -> defines...
// L'ensemble des fonctions y ont acces

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */

void Acceleration()
{
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.1);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.1);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.3);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.26);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.5);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.46);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.7);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.66);
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 1);
    MOTOR_SetSpeed(RIGHT, speedDroite * 1);
    delay(50);
}

void Decceleration()
{
    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.6);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.6);

    delay(50);
    MOTOR_SetSpeed(LEFT, speedGauche * 0.4);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.4);
    delay(60);
    
    MOTOR_SetSpeed(LEFT, speedGauche * 0.1);
    MOTOR_SetSpeed(RIGHT, speedDroite * 0.1);
    delay(60);
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
   // ENCODER_Reset(RIGHT);
    // ENCODER_Reset(LEFT);

}
void PIDBasic()
{
    
    int comptePulseGauche = 0;
    int comptePulseDroit = 0;
    float rapportErreur = 0;

    comptePulseDroit = ENCODER_Read(RIGHT);
    comptePulseGauche = ENCODER_Read(LEFT);

    delay(10);
   /* 
    Serial.print("Moteur Gauche =");
    Serial.println(speedGauche);
    Serial.print("Moteur Droit ="); 
    Serial.println(speedDroite);
    */
    rapportErreur = (comptePulseDroit - comptePulseGauche);

    float KI = (nbCycle * 5000 ) - comptePulseGauche ; 
    Serial.print("nbCylce  =");
    Serial.println(nbCycle, 5);
    Serial.print("AVANT KI  =");
    Serial.println(KI, 5);
    KI = KI * 0.00002; 

    speedGauche += (rapportErreur * 0.0001);

    MOTOR_SetSpeed(LEFT, speedGauche);

    Serial.print("KI  =");
    Serial.println(KI, 5);
    Serial.print("Moteur Gauche =");
    Serial.println(speedGauche);
    /**/

    delay(5);
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
    nbCycle = 0;
    int temps = 0;
    int prochainCycle= 3200;

    while (Compteur < ComptePulse)
    {
       
       if(Compteur >= prochainCycle)
        {
            prochainCycle = Compteur + 3200;
            PIDParMoteur();
            temps = 0;
        }
        
        
        delay(5);           

        Compteur = ENCODER_Read(RIGHT);
       //PIDBasic();
    }
      Decceleration();

}
void Avancer200()
{
    long Compteur = 0;

    float circonference = 3.1415926 * 7.5;
    long PulseParCm = 32000 / circonference;
    long ComptePulse = (PulseParCm * (200));
    ComptePulse = ComptePulse / 10;

    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    speedGauche = 0.625;
    speedDroite = 0.6;
    Acceleration();
    nbCycle = 0;
    int temps = 0;
    int prochainCycle= 3200;

    while (Compteur < ComptePulse)
    {
       
       if(Compteur >= prochainCycle)
        {
            prochainCycle = Compteur + 3200;
            PIDParMoteur();
            temps = 0;
        }
        
        
        delay(5);           

        Compteur = ENCODER_Read(RIGHT);
       //PIDBasic();
    }
    
    MOTOR_SetSpeed(LEFT, 0.4);
    MOTOR_SetSpeed(RIGHT, 0.4);
    delay(50);
    MOTOR_SetSpeed(LEFT, 0.2);
    MOTOR_SetSpeed(RIGHT, 0.2);
    delay(50);
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

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
        while (comptePulse <= (3200 * 1.18))
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
        while (comptePulse <= (3200 * 1.18))
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
       int comptePulse = 0;
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels
   
    delay(1500);

   
    Avancer200();
    delay(500);
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
    Avancer_Cm(20);
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
        MOTOR_SetSpeed(RIGHT, 0.7);
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
    Avancer_Cm(50);
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

    /**/
}
