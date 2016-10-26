#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

byte dir = 0x27;
LiquidCrystal_I2C lcd( dir, 2, 1, 0, 4, 5, 6, 7);
bool displayEnabled = true;//for debug

int totalLevels = 4;

//estado del nivel
bool level1Completed;
bool level2Completed;
bool level3Completed;
bool level4Completed;

//si un nivel se acaba de terminar
bool level1Finished;
bool level2Finished;
bool level3Finished;
bool level4Finished;


//si un nivel cambio en algo
bool anyLevelChanged;
bool level1Changed;
bool level2Changed;
bool level3Changed;
bool level4Changed;

const int MAX_ERRORS = 3;
int totalErrors;
bool gameStarted;
bool timedOut;

//tiempo total en milisegundos
const float TOTAL_GAME_TIME = 120000;
float gameStartedTime;//momento en q empieza el juego (primer movimiento)
unsigned long remainingGameTime;//tiempo restante
float getClockSpeed() //velocidad del reloj (aumenta por errores)
{
  return 1 + 0.2 * totalErrors;
}

//update time
const unsigned long CHECK_MIN_TIME = 100;
unsigned long lastCheckTime = 0;


int gameNumber;

//level 1 - cables
bool lvl1_wiresTarget[3];//estado de los cables (conectado/desconectado). ej 0,1,0

//level 2 - simon
int lvl2_colorsTarget[6];//secuencia de colores. ej 1,2,2,4,1,3

//level 3 - potenciometros
//
int lvl3_value1Target;//valor correcto de potencia1. ej 280
int lvl3_value2Target;//valor correcto de potencia2. ej 900
int lvl3_value3Target;//valor correcto de potencia2. ej 900
int lvl3_value4Target;//valor correcto de potencia2. ej 900

//level 4 - keycode
int lvl4_secretCodeTarget;//valor del codigo. ej 5469



void setup()
{
  gameStarted = false;
  totalErrors = 0;

  randomSeed(analogRead(0));
  gameNumber = random(1000, 10000);//el codigo de desactvacion de 4 digitos
  
  lvl1Setup();
  lvl2Setup();
  lvl3Setup();
  lvl4Setup();

  if(displayEnabled)
    setupDisplay();
}

void loop() 
{
  if(CHECK_MIN_TIME <= millis() - lastCheckTime)
  {
    if(timedOut)
    {
      //perdio
    }
    else
    {
      lvl1Update();
      lvl2Update();
      lvl3Update();
      lvl4Update();
    
      anyLevelChanged = checkLevelsChanged();
      if(gameStarted)
      {
        if(remainingGameTime <= 0)
        {
          //se termino el tiempo
          timedOut = true;
        }
        else
        {
          if(checkAnyError())
          {
            totalErrors++;
          }
          updateGameTimer();   
          updateIndicatorLeds();
        }
      }
      else  
      {
        if(anyLevelChanged)
        {
          //empezo a jugar
          gameStartedTime = millis();
          remainingGameTime = TOTAL_GAME_TIME;
        }
      }
      if(displayEnabled)
        updateDisplay();
      lastCheckTime = millis();
    }
  }
}

//actualizar reloj
void updateGameTimer()
{
   remainingGameTime = remainingGameTime - (millis() - lastCheckTime) * getClockSpeed();
}

void setupDisplay()
{
  
  lcd.begin (16,2);    // Inicializar el display con 16 caraceres 2 lineas
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.home ();
 // lcd.print("asd");
 // lcd.setCursor ( 0, 1 );        // go to the 2nd line
  //lcd.print("asd2");
}

//dibuja en pantalla
void updateDisplay()
{
  //timer
  lcd.setCursor ( 0, 0 );
  lcd.print(getRemainingFormatedTime());

  //ingreso de codigo
  lcd.setCursor (0,1);
  lcd.print(getInputCode());

  //secret code
  lcd.setCursor (8,0);
  lcd.print(String(lvl4_secretCodeTarget));
}

String getRemainingFormatedTime()
{
  int seconds = remainingGameTime / 1000;
  return String (seconds) + "s";
}

String getInputCode()
{
  return "_ _ _ _";
}


//actualiza indicadores leds
void updateIndicatorLeds()
{
  updateLvl1Leds();
  updateLvl2Leds();
  updateLvl3Leds();
  updateLvl4Leds();
}

//si algun nivel cambio
bool checkLevelsChanged()
{
  return checkLvl1() || checkLvl2() || checkLvl3() || checkLvl4() ;
}

bool checkAnyError()
{
  return checkErrorLvl1() || checkErrorLvl2() || checkErrorLvl3() || checkErrorLvl4() ;
}


