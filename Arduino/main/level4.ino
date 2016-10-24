//secret keycode

//pines
const int LVL4_GREEN = 36;
const int LVL4_RED = 37;

void lvl4Setup()
{
  
}

void lvl4Update()
{
}

bool checkLvl4()
{
  return false;
}

void updateLvl4Leds()
{
  digitalWrite(LVL4_GREEN, level4Completed);
  digitalWrite(LVL4_RED, !level4Completed);
}

