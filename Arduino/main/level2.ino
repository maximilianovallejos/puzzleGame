//color simon

//pines
const int LVL2_GREEN = 32;
const int LVL2_RED = 33;

void lvl2Setup()
{
}

void lvl2Update()
{
}

bool checkLvl2()
{
  return false;
}

void updateLvl2Leds()
{
  digitalWrite(LVL2_GREEN, level2Completed);
  digitalWrite(LVL2_RED, !level2Completed);
}
