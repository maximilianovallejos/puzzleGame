//potenciometers

//pines
const int LVL3_GREEN = 34;
const int LVL3_RED = 35;

void lvl3Setup()
{
}

void lvl3Update()
{
}

bool checkLvl3()
{
  return false;
}

void updateLvl3Leds()
{
  digitalWrite(LVL3_GREEN, level3Completed);
  digitalWrite(LVL3_RED, !level3Completed);
}
