//secret keycode


//pines
const int LVL4_GREEN = 36;
const int LVL4_RED = 37;




String lvl4Input;



bool lvl4KeyPressed;

void lvl4Setup()
{
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
        level1Finished = true;
      }
    }
    //char input = keypad.getKey();//lo leo en main
    if(input)
    {
      lvl4KeyPressed = true;
      lvl4Input = lvl4Input + String(input);
    }
  }
}

bool checkChangesLvl4()
{
  if(lvl4KeyPressed)
  {
    lvl4KeyPressed = false;
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

