//secret keycode


//pines
const int LVL4_GREEN = 36;
const int LVL4_RED = 37;


const byte K_ROWS = 4;
const byte K_COLS = 4;

byte K_ROW_PINS[] = {7, 6, 5, 4};
byte K_COL_PINS[] = { 3, 2, 1, 0};

String lvl4Input;

char Keys [ K_ROWS ][ K_COLS ] =
    {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
     };

Keypad keypad = Keypad(makeKeymap(Keys), K_ROW_PINS, K_COL_PINS, K_ROWS, K_COLS);

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
    char input = keypad.getKey();
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

