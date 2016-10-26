//wires

//pines
const int WIRE1 = 20;
const int WIRE2 = 21;
const int WIRE3 = 22;
const int LVL1_GREEN = 30;
const int LVL1_RED = 31;


const int TOTAL_WIRES = 3;

bool wiresCurrentState[TOTAL_WIRES];
bool wiresLastState[TOTAL_WIRES];

void lvl1Setup()
{
  initTarget1();

  pinMode(WIRE1, INPUT);
  pinMode(WIRE2, INPUT);
  pinMode(WIRE3, INPUT);
  
  updateWiresState();
  updateLastState();
}

void initTarget1()
{
  String tSeed = String(gameNumber).substring(0,0);
  switch(tSeed.toInt())
  {
    case 1:
    case 6:
      lvl1_wiresTarget[0] = 0;
      lvl1_wiresTarget[1] = 0;
      lvl1_wiresTarget[2] = 1;
      break;
    case 2:
    case 7:
      lvl1_wiresTarget[0] = 0;
      lvl1_wiresTarget[1] = 1;
      lvl1_wiresTarget[2] = 1;
      break;
    case 3:
    case 8:
      lvl1_wiresTarget[0] = 1;
      lvl1_wiresTarget[1] = 0;
      lvl1_wiresTarget[2] = 1;
      break;
    case 4:
    case 9:
      lvl1_wiresTarget[0] = 0;
      lvl1_wiresTarget[1] = 1;
      lvl1_wiresTarget[2] = 0;
      break;
    case 5:
    case 0:
      lvl1_wiresTarget[0] = 1;
      lvl1_wiresTarget[1] = 0;
      lvl1_wiresTarget[2] = 0;
      break;
  }
}

void lvl1Update()
{
  updateWiresState();
  if(!level1Completed && wiresCompleted())
  {
    level1Finished = true;
  }
}

bool wiresCompleted()
{
  for(int i =0; i < TOTAL_WIRES; i++)
  {
    if(wiresCurrentState[i] != lvl1_wiresTarget[i])
      return false;
  }
  return true;
}

bool checkLvl1()
{
  //true si cambio el estado de un cable
  for(int i=0; i<TOTAL_WIRES; i++)
  {
    if(wiresCurrentState[i] != wiresLastState[i])
    {
      return true;
    }
  }
  return false;
}

bool checkErrorLvl1()
{
  for(int i=0; i<TOTAL_WIRES; i++)
  {
    //si hubo un cambio
    if(wiresCurrentState[i] != wiresLastState[i])
    {
      //si el cambio fue incorrecto
      if(wiresCurrentState[i] != lvl1_wiresTarget[i])
      {
        return true;
      }
    }
  }
  return false;
}

void updateLvl1Leds()
{
  digitalWrite(LVL1_GREEN, level1Completed);
  digitalWrite(LVL1_RED, !level1Completed);
}

void updateWiresState()
{
  updateLastState();
  wiresCurrentState[0] = digitalRead(WIRE1);
  wiresCurrentState[1] = digitalRead(WIRE2);
  wiresCurrentState[2] = digitalRead(WIRE3);
}

void updateLastState()
{
  wiresLastState[0] = wiresCurrentState[0];
  wiresLastState[1] = wiresCurrentState[1];
  wiresLastState[2] = wiresCurrentState[2];
}
