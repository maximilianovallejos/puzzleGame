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
  updateWiresState();
  updateLastState();
}

void lvl1Update()
{
  updateWiresState();
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
