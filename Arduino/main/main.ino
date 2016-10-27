#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

int const SPK_PIN = 9;

byte dir = 0x27;
LiquidCrystal_I2C lcd( dir, 2, 1, 0, 4, 5, 6, 7);
bool displayEnabled = true;//for debug

const byte K_ROWS = 4;
const byte K_COLS = 4;

byte K_ROW_PINS[] = {7, 6, 5, 4};
byte K_COL_PINS[] = { 3, 2, 1, 0};
char Keys [ K_ROWS ][ K_COLS ] =
    {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
     };
char RESTART_KEY = '*';

Keypad keypad = Keypad(makeKeymap(Keys), K_ROW_PINS, K_COL_PINS, K_ROWS, K_COLS);
char input;

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
bool gameWon;
bool restartGame;

//tiempo total en milisegundos
const float TOTAL_GAME_TIME = 120000;
float gameStartedTime;//momento en q empieza el juego (primer movimiento)
unsigned long remainingGameTime;//tiempo restante
float getClockSpeed() //velocidad del reloj (aumenta por errores)
{
  return 1 + 0.2 * totalErrors;
}
bool isPlaying()
{
  return gameStarted && !timedOut && !gameWon;
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
  randomSeed(analogRead(0));

  //press # to start
  setupSession();
}

void setupSession()
{
  gameStarted = false;
  totalErrors = 0;
  gameStarted = false;
  timedOut = false;
  gameWon = false;
  restartGame = false;
  
  level1Completed = false;
  level2Completed = false;
  level3Completed = false;
  level4Completed = false;
  
  level1Finished = false;
  level2Finished = false;
  level3Finished = false;
  level4Finished = false;
  
  anyLevelChanged = false;
  level1Changed = false;
  level2Changed = false;
  level3Changed = false;
  level4Changed = false;

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
  input = keypad.getKey();
  if(input)
  {
    if(input == RESTART_KEY) 
    {
      restartGame = true;
    }
  }
  if(restartGame)
  {
    setupSession();   
    return;
  }
  if(CHECK_MIN_TIME <= millis() - lastCheckTime)
  {
    if(timedOut)
    {
      //perdio
       if(displayEnabled)
        updateDisplay();
    }
    else if(gameWon)
    {
      //gano
       if(displayEnabled)
        updateDisplay();
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
          if(anyLevelChanged)
          {
            updateLevelStates();
            updateIndicatorLeds();
            if(checkAnyError())
            {
              totalErrors++;
            }
            else if(level1Completed && level2Completed && level3Completed && level4Completed)
            {
              gameWon = true;
            }
            
          }
          updateGameTimer();   
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

   //hacer un sonido por segundo
   if(remainingGameTime % 1000 == 0)
   {
    playTimeSound();
   }
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
  if(isPlaying())
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
  else
  {
    lcd.noDisplay();
    lcd.clear();
    lcd.setCursor (0,1);
    if(timedOut)
    {
      lcd.print("Perdiste!");
    }
    else if(gameWon)
    {
      lcd.print("Ganaste!");
    }
    delay(500);
    lcd.display();
    delay(500);
    lcd.noDisplay();
    delay(500);
    lcd.display();
  }
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

void updateLevelStates()
{
  if(level1Finished)
  {
    level1Finished = false;
    level1Completed = true;
    playCompletedSound();
  }
  if(level2Finished)
  {
    level2Finished = false;
    level2Completed = true;
    playCompletedSound();
  }
  if(level3Finished)
  {
    level3Finished = false;
    level3Completed = true;
    playCompletedSound();
  }
  if(level4Finished)
  {
    level4Finished = false;
    level4Completed = true;
    playCompletedSound();
  }
}

void playCompletedSound()
{
  //hacer un beep corto
  tone(SPK_PIN, 350, 100);
}

void playErrorSound()
{
  //hacer un beep largo
  tone(SPK_PIN, 150, 1000);
}

void playTimeSound()
{
  //hacer un beep corto
  tone(SPK_PIN, 150, 100);
}

//si algun nivel cambio
bool checkLevelsChanged()
{
  return checkChangesLvl1() || checkChangesLvl2() || checkChangesLvl3() || checkChangesLvl4() ;
}

bool checkAnyError()
{
  return checkErrorLvl1() || checkErrorLvl2() || checkErrorLvl3() || checkErrorLvl4() ;
}


