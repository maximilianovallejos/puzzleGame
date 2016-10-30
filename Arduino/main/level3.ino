//potenciometers

//pines
const int LVL3_GREEN = 34;
const int LVL3_RED = 35;

const int POT1 = A1;
const int POT2 = A2;
const int POT3 = A14;
const int POT4 = A15;

int const LEFT = 0;
int const RIGHT = 1023;
int const UP = 0;
int const DOWN = 1023;
int const NONE = 512;

int blinkStep;
int blinkTotalSteps;

//steps q espera cuando el nivel esta correcto
const int lvl3StepsToComplete = 15;
int lvl3CurrentSteps = 0;

void lvl3Setup()
{
  Serial.println("lvl3Setup");
  blinkStep = 0;
  blinkTotalSteps = 0;
  lvl3CurrentSteps = 0;
  initTarget3();
}

void initTarget3()
{
  String tSeed = String(gameNumber).substring(2,2);
  Serial.println("lvl3 seed " + tSeed);
  int iSeed = tSeed.toInt();

  //TODO: completar con valores correctos
  switch(iSeed)
  {
    case 1:
    case 6:
    //arriba abajo
      lvl3_value1Target = UP;
      lvl3_value2Target = NONE;
      lvl3_value3Target = DOWN;
      lvl3_value4Target = NONE;
      break;
    case 2:
    case 7:
      //izqquierda derecha
      lvl3_value1Target = NONE;
      lvl3_value2Target = LEFT;
      lvl3_value3Target = NONE;
      lvl3_value4Target = RIGHT;
      break;
    case 3:
    case 8:
      //derecha izquierda
      lvl3_value1Target = NONE;
      lvl3_value2Target = RIGHT;
      lvl3_value3Target = NONE;
      lvl3_value4Target = LEFT;
      break;
    case 4:
    case 9:
      //abajo arriba
      lvl3_value1Target = DOWN;
      lvl3_value2Target = NONE;
      lvl3_value3Target = UP;
      lvl3_value4Target = NONE;
      break;
    case 5:
    case 0:
      //abajo derecha
      lvl3_value1Target = DOWN;
      lvl3_value2Target = NONE;
      lvl3_value3Target = NONE;
      lvl3_value4Target = RIGHT;
      break;
  }
}


void lvl3Update()
{
  if(!level3Completed)
  {
    if(getCompletedRatio() >= 90)
    {
      if(lvl3CurrentSteps >= lvl3StepsToComplete)
      {
        Serial.println("lvl3 finished");
        level3Finished = true;
      }
      else
      {
        lvl3CurrentSteps ++;
      }
    }
    lvl3CurrentSteps = 0;
  }
}

bool checkChangesLvl3()
{
  if(100 < abs(512 - analogRead(POT1)) || 100 < abs(512 - analogRead(POT2)) || 100 < abs(512 - analogRead(POT3)) || 100 < abs(512 - analogRead(POT4)))
  {
    Serial.println("lvl3 changed");
    return true;
  }
  return false;
}

bool checkErrorLvl3()
{
  return false;
}

void updateLvl3Leds()
{
  if(level3Completed)
  {
    digitalWrite(LVL3_GREEN, HIGH);
  }
  else
  {
    blinkLed(LVL3_GREEN, (100 - getCompletedRatio()) * 10);
  }
  digitalWrite(LVL3_RED, !level3Completed);
}


//un step seria 100ms prende, 100ms apaga
void blinkLed(int pin, int stepsDelay )
{
  blinkTotalSteps = stepsDelay;
  if(blinkStep <= blinkTotalSteps)
  {
    blinkStep = 0;
    digitalWrite(pin, !digitalRead(pin));
  }
  else
  {
    blinkStep++;
  }
}

//de 0 a 100
int getCompletedRatio()
{
  int dif = abs(lvl3_value1Target - analogRead(POT1));
  dif += abs(lvl3_value2Target - analogRead(POT2));
  dif += abs(lvl3_value3Target - analogRead(POT3));
  dif += abs(lvl3_value4Target - analogRead(POT4));
  return map(dif, 0,(1023*4), 100,0);//0 diff da 100, 2046 dif da 50
}
