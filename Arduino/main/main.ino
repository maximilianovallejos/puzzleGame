#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

int const SPK_PIN = 3;
int const VIB_PIN = 2;

byte dir = 0x27;
LiquidCrystal_I2C lcd( dir, 2, 1, 0, 4, 5, 6, 7);
bool displayEnabled = true;//for debug

const byte K_ROWS = 4;
const byte K_COLS = 4;

byte K_ROW_PINS[] = {39,41,43,45};
byte K_COL_PINS[] = {47,49,51,53};
char Keys [ K_ROWS ][ K_COLS ] =
    {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
     };
char RESTART_KEY = '#';

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
int vibError;

//tiempo total en milisegundos
const float TOTAL_GAME_TIME = 300000;
float gameStartedTime;//momento en q empieza el juego (primer movimiento)
unsigned long remainingGameTime;//tiempo restante
float getClockSpeed() //velocidad del reloj (aumenta por errores)
{
  return totalErrors * 2;
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
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(SPK_PIN, OUTPUT);
  pinMode(VIB_PIN, INPUT);

  //press # to start
  setupSession();
}

void setupSession()
{
  Serial.println("setupSession");
  gameStarted = false;
  totalErrors = 0;
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

  vibError = 0;

  gameNumber = random(1000, 10000);//el codigo de desactvacion de 4 digitos
  Serial.print("Game number: ");
  Serial.println(String(gameNumber));
  
  lvl1Setup();
  lvl2Setup();
  lvl3Setup();
  lvl4Setup();

  if(displayEnabled)
    {
      setupDisplay();
      showStartMessage();
    }
  
}

void loop() 
{
  
  
  input = keypad.getKey();
  if(input)
  {
    Serial.print("read input:");
    Serial.println(String(input));
    if(input == RESTART_KEY) 
    {
      restartGame = true;
    }
  }
  if(restartGame)
  {
    Serial.println("Restart game");
    setupSession();   
    return;
  }
  if(CHECK_MIN_TIME <= millis() - lastCheckTime)
  {
    if(timedOut)
    {
      Serial.println("Timeout GameOver");
      //perdio
       if(displayEnabled)
        updateDisplay();
    }
    else if(gameWon)
    {
      Serial.println("Game WON!!");
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
          if( digitalRead(VIB_PIN))
          {
            //vibError++;
          }
          if(anyLevelChanged)
          {
            updateLevelStates();
            updateIndicatorLeds();
            if(level1Completed && level2Completed && level3Completed && level4Completed)
            {
              gameWon = true;
            }
            
          }
          if(checkAnyError())
          {
            totalErrors++;
            Serial.println("User make an error");
            Serial.print("Total errors: ");
            Serial.println(String(totalErrors));
          }
           
          updateGameTimer();   
        }
      }
      else  
      {
        if(anyLevelChanged)
        {
           Serial.println("Start game");
          //empezo a jugar
          gameStarted = true;
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
   remainingGameTime = remainingGameTime - ((millis() - lastCheckTime) + (millis() - lastCheckTime) * getClockSpeed());
   //Serial.println(String(remainingGameTime));
   if(remainingGameTime > 15000)
   {
     if((remainingGameTime-1000)  % 5000 <= 100)
     {
      Serial.print("Time");
      Serial.println(String(remainingGameTime/1000));
      playTimeSound();
     }
   }
   else
   {
    if((remainingGameTime -1000)% 1000 <= 100)
    {
      Serial.print("Time");
      Serial.println(String(remainingGameTime/1000));
      playTimeSound();
    }
   }
}

void setupDisplay()
{
  Serial.println("setupDisplay");
  lcd.begin (16,2);    // Inicializar el display con 16 caraceres 2 lineas
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.home ();
 // lcd.print("asd");
 // lcd.setCursor ( 0, 1 );        // go to the 2nd line
  //lcd.print("asd2");
}

void showStartMessage()
{
  Serial.println("show start message");
   lcd.home ();
   lcd.print("Ready to GO!");
}

//dibuja en pantalla
void updateDisplay()
{
  if(gameStarted)
  {
    lcd.clear();
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
  tone(SPK_PIN, 350, 2000);
}

void playErrorSound()
{
  //hacer un beep largo
  tone(SPK_PIN, 150, 5000);
}

void playTimeSound()
{
  //hacer un beep corto
  tone(SPK_PIN, 250, 100);
}

//si algun nivel cambio
bool checkLevelsChanged()
{
  return checkChangesLvl1() || checkChangesLvl2() || checkChangesLvl3() || checkChangesLvl4() ;
}

bool checkAnyError()
{
  if(vibError >= 3)
  {
     Serial.println("vibration error");
  }
  bool hasError = checkErrorLvl1() || checkErrorLvl2() || checkErrorLvl3() || checkErrorLvl4() || vibError > 3;
  if(hasError)
  {
    vibError = 0;
  }
  return hasError;
}


