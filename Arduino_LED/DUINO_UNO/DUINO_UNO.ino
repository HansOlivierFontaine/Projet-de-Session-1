
#include <FastLED.h>
//https://www.tweaking4all.com/hardware/arduino/arduino-ws2812-led/

/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
#define BIT0 2
#define BIT1 3
#define BIT2 4
#define DEMO false
#define ACTIF true

#define FACILE 1
#define MOYEN 2
#define DUR 3
#define OFF 0
uint64_t couleurDifficulte;

#define LED_PIN     8
#define NUM_LEDS    120
#define BRIGHTNESS  100
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
int difficulte =0;
int mode = DEMO;
bool SPOT_mode = DEMO;


uint32_t colors[142] = 
    {
    0xF0F8FF,
    0x9966CC,
    0xFAEBD7,
    0x00FFFF,
    0x7FFFD4,
    0xF0FFFF,
    0xF5F5DC,
    0xFFE4C4,
    0x000000,
    0xFFEBCD,
    0x0000FF,
    0x8A2BE2,
    0xA52A2A,
    0xDEB887,
    0x5F9EA0,
    0x7FFF00,
    0xD2691E,
    0xFF7F50,
    0x6495ED,
    0xFFF8DC,
    0xDC143C,
    0x00FFFF,
    0x00008B,
    0x008B8B,
    0xB8860B,
    0xA9A9A9,
    0x006400,
    0xBDB76B,
    0x8B008B,
    0x556B2F,
    0xFF8C00,
    0x9932CC,
    0x8B0000,
    0xE9967A,
    0x8FBC8F,
    0x483D8B,
    0x2F4F4F,
    0x00CED1,
    0x9400D3,
    0xFF1493,
    0x00BFFF,
    0x696969,
    0x1E90FF,
    0xB22222,
    0xFFFAF0,
    0x228B22,
    0xFF00FF,
    0xDCDCDC,
    0xF8F8FF,
    0xFFD700,
    0xDAA520,
    0x808080,
    0x008000,
    0xADFF2F,
    0xF0FFF0,
    0xFF69B4,
    0xCD5C5C,
    0x4B0082,
    0xFFFFF0,
    0xF0E68C,
    0xE6E6FA,
    0xFFF0F5,
    0x7CFC00,
    0xFFFACD,
    0xADD8E6,
    0xF08080,
    0xE0FFFF,
    0xFAFAD2,
    0x90EE90,
    0xD3D3D3,
    0xFFB6C1,
    0xFFA07A,
    0x20B2AA,
    0x87CEFA,
    0x778899,
    0xB0C4DE,
    0xFFFFE0,
    0x00FF00,
    0x32CD32,
    0xFAF0E6,
    0xFF00FF,
    0x800000,
    0x66CDAA,
    0x0000CD,
    0xBA55D3,
    0x9370DB,
    0x3CB371,
    0x7B68EE,
    0x00FA9A,
    0x48D1CC,
    0xC71585,
    0x191970,
    0xF5FFFA,
    0xFFE4E1,
    0xFFE4B5,
    0xFFDEAD,
    0x000080,
    0xFDF5E6,
    0x808000,
    0x6B8E23,
    0xFFA500,
    0xFF4500,
    0xDA70D6,
    0xEEE8AA,
    0x98FB98,
    0xAFEEEE,
    0xDB7093,
    0xFFEFD5,
    0xFFDAB9,
    0xCD853F,
    0xFFC0CB,
    0xCC5533,
    0xDDA0DD,
    0xB0E0E6,
    0x800080,
    0xFF0000,
    0xBC8F8F,
    0x4169E1,
    0x8B4513,
    0xFA8072,
    0xF4A460,
    0x2E8B57,
    0xFFF5EE,
    0xA0522D,
    0xC0C0C0,
    0x87CEEB,
    0x6A5ACD,
    0x708090,
    0xFFFAFA,
    0x00FF7F,
    0x4682B4,
    0xD2B48C,
    0x008080,
    0xD8BFD8,
    0xFF6347,
    0x40E0D0,
    0xEE82EE,
    0xF5DEB3,
    0xFFFFFF,
    0xF5F5F5,
    0xFFFF00,
    0x9ACD32,
    };


void LED_BarFade()
{

    memset(leds, 0, NUM_LEDS * 3);
    for(int i = 0 ; i < 60; i++ ) {

    //leds[i-1] = CRGB::Black;
    leds[i] = couleurDifficulte ;
    
    FastLED.show();
    delay(10);
    }

    for(int i =BRIGHTNESS; i >= 0 ; i--)
    {
      FastLED.setBrightness( i );
        FastLED.show();
      delay(10);
    }

   FastLED.setBrightness(BRIGHTNESS);
}


int SPOT1_CenterLed = 0;
int SPOT2_CenterLed = 0;
int SPOT3_CenterLed = 0;


int SPOT_Count =0 ;
int SPOT_CenterLed = 0;
int SPOT_Brightness = 0;
uint64_t SPOT_Color = CRGB::Red;

bool CHASER_Direction = 1;
int CHASER_Position = 3 ;

void LED_Chaser()
{
    if(CHASER_Direction == 1)
        CHASER_Position++;
    else
        CHASER_Position--;

    if(CHASER_Position >= NUM_LEDS)
        CHASER_Direction = !CHASER_Direction;
    
    if(CHASER_Position <= 2)
        CHASER_Direction = !CHASER_Direction;

    leds[CHASER_Position+2] = CRGB::Black ;
    leds[CHASER_Position-2] = CRGB::Black ;
    leds[CHASER_Position] = couleurDifficulte ;
    leds[CHASER_Position-1] = couleurDifficulte ;
    leds[CHASER_Position+1] = couleurDifficulte ;
        
    FastLED.show();
}

void newSPOT_Center()
{
    SPOT_CenterLed = (random() + 5) %(NUM_LEDS-3);
    
    SPOT1_CenterLed = (random() + 5) %(NUM_LEDS-3);
    SPOT2_CenterLed = (random() + 5) %(NUM_LEDS-3);
    SPOT3_CenterLed = (random() + 5) %(NUM_LEDS-3);

    if(SPOT_mode == DEMO)
        SPOT_Color = colors[random()%142];
    else
        SPOT_Color = couleurDifficulte;
}

void LED_Spot()
{
    SPOT_Count++;
    
    if(SPOT_Count >= 20)
    {
        newSPOT_Center();

        SPOT_Brightness = 0;
        SPOT_Count = 0;

        for(int i=0 ; i <= NUM_LEDS ; i++)
        {
            leds[i] = CRGB::Black;
        }
        leds[SPOT_CenterLed-1] = SPOT_Color;
        leds[SPOT_CenterLed] = SPOT_Color;
        leds[SPOT_CenterLed+1] = SPOT_Color;

    if(SPOT_mode == DEMO)
    {
        leds[SPOT1_CenterLed-1] = SPOT_Color;
        leds[SPOT1_CenterLed] = SPOT_Color;
        leds[SPOT1_CenterLed+1] = SPOT_Color;

        leds[SPOT2_CenterLed-1] = SPOT_Color;
        leds[SPOT2_CenterLed] = SPOT_Color;
        leds[SPOT2_CenterLed+1] = SPOT_Color;

        leds[SPOT3_CenterLed-1] = SPOT_Color;
        leds[SPOT3_CenterLed] = SPOT_Color;
        leds[SPOT3_CenterLed+1] = SPOT_Color;
    }
        
    }

    if(SPOT_Count <= 10)
        SPOT_Brightness += 10;
    else
        SPOT_Brightness -= 10;

    FastLED.setBrightness(SPOT_Brightness);
    FastLED.show();
}

void LED_Clear()
{ 
    for(int i=0 ; i <= NUM_LEDS ; i++)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
    delay(10);
}

void setup()
{
     delay( 3000 ); // power-up safety delay
     Serial.begin(9600);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    leds[0] = CRGB::Black ;

    SPOT_CenterLed = 20;

    pinMode(BIT0 ,INPUT);
    pinMode(BIT1 ,INPUT);
    pinMode(BIT2 ,INPUT);

    couleurDifficulte = CRGB::Red;
    
     mode = DEMO;
    difficulte = OFF;
    
}

void lectureBits()
{
    int tempo = 0;

    tempo = digitalRead(BIT1);
    tempo = tempo*10 + digitalRead(BIT0);

    mode = digitalRead(BIT2);

    if(tempo == 01)
        difficulte = FACILE ;
    else if(tempo == 10)
        difficulte = MOYEN ;
    else if(tempo == 11)
        difficulte = DUR ;
    else if(tempo == 00)
        difficulte = OFF ; 

     Serial.print(difficulte);
     Serial.println(mode);

}


void loop()
{
    lectureBits(); 
    
    if(difficulte == FACILE)
        couleurDifficulte = CRGB::DarkGreen;
    if(difficulte == MOYEN)
        couleurDifficulte = CRGB::Yellow ;
    if(difficulte == DUR)
        couleurDifficulte = CRGB::Red;

   if(mode == OFF && difficulte == OFF)
   {
       SPOT_mode = DEMO;
        LED_Spot(); //Sleep mode 
   }
    else if(mode == OFF && difficulte != OFF)   
    {
        SPOT_mode = ACTIF;
        LED_Spot();
    }    
    else if(mode == ACTIF) 
        LED_Chaser();
  
    delay(50);
  
}


/*
//A SPECIFIER : PIN pour mode  et deux pour difficulté
int PINDIFF0 = 53;
int PINDIFF1 = 51;
int PINMODE = 49;
//Difficulte 0-3 , mode 0-1 (actif ou demo/standby)
void commandeLED(int difficulte , int mode)
{
    digitalWrite(PINMODE , mode);

    if(difficulte == 0)
    {
        digitalWrite(PINDIFF0 , 0);
        digitalWrite(PINDIFF1 , 0);
    }
    else if(difficulte == 1)
    {
        digitalWrite(PINDIFF0 , 1);
        digitalWrite(PINDIFF1 , 0);
    }
    else if(difficulte == 2)
    {
        digitalWrite(PINDIFF0 , 0);
        digitalWrite(PINDIFF1 , 1);
    }
    else if(difficulte == 3)
    {
        digitalWrite(PINDIFF0 , 1);
        digitalWrite(PINDIFF1 , 1);
    }
}
*/
