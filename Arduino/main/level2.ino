//color simon

//pines
const int LVL2_GREEN = 32;
const int LVL2_RED = 33;

void lvl2Setup()
{
  initTarget2();
}

void initTarget2()
{
  String tSeed = String(gameNumber).substring(1,1);
  //lvl2_colorsTarget
  switch(tSeed.toInt())
  {
    case 1:
    case 6:
      lvl2_colorsTarget[0] = 0;
      lvl2_colorsTarget[1] = 0;
      lvl2_colorsTarget[2] = 1;
      break;
    case 2:
    case 7:
      lvl2_colorsTarget[0] = 0;
      lvl2_colorsTarget[1] = 1;
      lvl2_colorsTarget[2] = 1;
      break;
    case 3:
    case 8:
      lvl2_colorsTarget[0] = 1;
      lvl2_colorsTarget[1] = 0;
      lvl2_colorsTarget[2] = 1;
      break;
    case 4:
    case 9:
      lvl2_colorsTarget[0] = 0;
      lvl2_colorsTarget[1] = 1;
      lvl2_colorsTarget[2] = 0;
      break;
    case 5:
    case 0:
      lvl2_colorsTarget[0] = 1;
      lvl2_colorsTarget[1] = 0;
      lvl2_colorsTarget[2] = 0;
      break;
  }
}

void lvl2Update()
{
}

bool checkChangesLvl2()
{
  return false;
}

bool checkErrorLvl2()
{
  return false;
}

void updateLvl2Leds()
{
  digitalWrite(LVL2_GREEN, level2Completed);
  digitalWrite(LVL2_RED, !level2Completed);
}
