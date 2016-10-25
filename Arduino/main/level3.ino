//potenciometers

//pines
const int LVL3_GREEN = 34;
const int LVL3_RED = 35;

void lvl3Setup()
{
  initTarget3();
}

void initTarget3()
{
  String tSeed = String(gameNumber).substring(2,2);
  int iSeed = tSeed.toInt();

  //TODO: completar con valores correctos
  switch(iSeed)
  {
    case 1:
    case 6:
    //arriba abajo
      lvl3_value1Target = 1024;
      lvl3_value2Target = 512;
      lvl3_value3Target = 0;
      lvl3_value4Target = 512;
      break;
    case 2:
    case 7:
      //izqquierda derecha
      lvl3_value1Target = 512;
      lvl3_value2Target = 0;
      lvl3_value3Target = 512;
      lvl3_value4Target = 1024;
      break;
    case 3:
    case 8:
      //derecha izquierda
      lvl3_value1Target = 1024;
      lvl3_value2Target = 1024;
      lvl3_value3Target = 1024;
      lvl3_value4Target = 1024;
      break;
    case 4:
    case 9:
      //abajo arriba
      lvl3_value1Target = 1024;
      lvl3_value2Target = 1024;
      lvl3_value3Target = 1024;
      lvl3_value4Target = 1024;
      break;
    case 5:
    case 0:
      //abajo derecha
      lvl3_value1Target = 1024;
      lvl3_value2Target = 1024;
      lvl3_value3Target = 1024;
      lvl3_value4Target = 1024;
      break;
  }
}

void lvl3Update()
{
}

bool checkLvl3()
{
  return false;
}

bool checkErrorLvl3()
{
  return false;
}

void updateLvl3Leds()
{
  digitalWrite(LVL3_GREEN, level3Completed);
  digitalWrite(LVL3_RED, !level3Completed);
}
