int totalLevels = 4;

//estado del nivel
bool level1Complete;
bool level2Complete;
bool level3Complete;
bool level4Complete;

//si un nivel se acaba de terminar
bool level1Finished;
bool level2Finished;
bool level3Finished;
bool level4Finished;

//si un nivel cambio en algo
bool level1Changed;
bool level2Changed;
bool level3Changed;
bool level4Changed;

//tiempo total en milisegundos
float totalTime = 120000;

int maxErrors;
int totalErrors;

//level 1 - cables
bool lvl1_wiresState[3];//estado de los cables (conectado/desconectado). ej 0,1,0

//level 2 - simon
int lvl2_colors[6];//secuencia de colores. ej 1,2,2,4,1,3

//level 3 - potenciometros
//
int lvl3_value1;//valor correcto de potencia1. ej 280
int lvl3_value2;//valor correcto de potencia2. ej 900

//level 4 - keycode
int lvl4_secretCode;//valor del codigo. ej 5469



void setup() {
  

}

void loop() 
{

  checkLevelsChanged();

  updateTimer();
  updateDisplay();
  updateIndicatorLeds();
}

//actualizar reloj
void updateTimer()
{
}

//dibuja en pantalla
void updateDisplay()
{
}

//actualiza indicadores leds
void updateIndicatorLeds()
{
  checkLvl1();
  checkLvl2();
  checkLvl3();
  checkLvl4();
}

//si algun nivel cambio
bool checkLevelsChanged()
{
  return checkLvl1() && checkLvl2() && checkLvl3() && checkLvl4() ;
}


