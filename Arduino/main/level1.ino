//wires

//pines
const int WIRE1 = 52;
const int WIRE2 = 50;
const int WIRE3 = 48;
const int LVL1_GREEN = 30;
const int LVL1_RED = 31;


const int TOTAL_WIRES = 3;

bool wiresCurrentState[TOTAL_WIRES];
bool wiresLastState[TOTAL_WIRES];

void lvl1Setup()
{
  Serial.println("lvl1Setup");
  initTarget1();

  pinMode(WIRE1, INPUT_PULLUP);
  pinMode(WIRE2, INPUT_PULLUP);
  pinMode(WIRE3, INPUT_PULLUP);
  
  updateWiresState();
  updateLastState();

  Serial.println("wires");
  Serial.println(digitalRead(WIRE1));
  Serial.println(digitalRead(WIRE2));
  Serial.println(digitalRead(WIRE3));
  Serial.println("--");
}

void initTarget1()
{
  String tSeed = String(gameNumber).substring(0,0);
  Serial.println("lvl1 seed " + tSeed);
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
    Serial.println("lvl1 finished");
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

bool checkChangesLvl1()
{
  //true si cambio el estado de un cable
  for(int i=0; i<TOTAL_WIRES; i++)
  {
    if(wiresCurrentState[i] != wiresLastState[i])
    {
    
      Serial.println("lvl1 changed");
      Serial.println("wires");
      Serial.println(digitalRead(WIRE1));
      Serial.println(digitalRead(WIRE2));
      Serial.println(digitalRead(WIRE3));
      Serial.println("--");
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
        Serial.println("lvl1 error");
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
