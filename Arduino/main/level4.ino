//secret keycode


//pines
const int LVL4_GREEN = 36;
const int LVL4_RED = 37;







bool lvl4KeyPressed;

void lvl4Setup()
{
  Serial.println("lvl4Setup");
  lvl4KeyPressed = false;
  lvl4Input = "";
  initTarget4();
}

void initTarget4()
{
  lvl4_secretCodeTarget = gameNumber;
}

void lvl4Update()
{
  if(!level4Completed)
  {
    if(lvl4Input.length() >= 4)
    {
      int inputN = lvl4Input.toInt();
      if(inputN == lvl4_secretCodeTarget)
      {
        Serial.println("lvl4 finished");
        level4Finished = true;
      }
     
    }
    //char input = keypad.getKey();//lo leo en main
    /*if(input != NO_KEY)
    {
      Serial.println("Read input lvl4");
      lvl4KeyPressed = true;
      lvl4Input = lvl4Input + String(input);
      Serial.println("lvl4Input " + lvl4Input);
    }*/
  }
}

void lvl4AddKey(String key)
{
  lvl4KeyPressed = true;
  lvl4Input = lvl4Input + key;
  Serial.println("lvl4Input " + lvl4Input);
}

bool checkChangesLvl4()
{
  if(lvl4KeyPressed)
  {
    lvl4KeyPressed = false;
    Serial.println("lvl4 changed");
    return true;
  }
  return false;
}

bool checkErrorLvl4()
{
  if(lvl4Input.length() >= 4)
  {
    int inputN = lvl4Input.toInt();
    if(inputN != lvl4_secretCodeTarget)
    {
      Serial.println("lvl4 error");
      lvl4Input = "";
      return true;
    }
  }
  return false;
}

void updateLvl4Leds()
{
  digitalWrite(LVL4_GREEN, level4Completed);
  digitalWrite(LVL4_RED, !level4Completed);
}

