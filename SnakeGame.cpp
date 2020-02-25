/*
  Pin layout
  0 = 5 | 8 = A3
  1 = 3 | 9 = A2
  2 = 11|10 = 8
  3 = 12|11 = 10
  4 = 2 |12 = 6
  5 = A0|13 = A1
  6 = 4 |14 = 13
  7 = 7 |15 = 9
*/

unsigned long PreviousTime = millis();

struct Point
{
  byte x, y;
};

enum Direction : byte
{
  Left,
  Right,
  Up,
  Down,
};

const byte MaxSnakeLength = 64;
Point SnakeBody[MaxSnakeLength + 1] = { 0 };
byte SnakeLength = 1;

Point Fruit = { 7, 7 };

Direction CurrentDirection = Direction::Right;
bool ChangedDirection = false;

void setup()
{ 
  for (byte i = 0; i < 3; i++)
  {
    SnakeBody[i] = { i, 0 };  
  }
  
  for (byte i = 2; i <= (2 + 16); i++)
  {
    pinMode(i, OUTPUT);
  }
  
  pinMode(A4, INPUT);
  digitalWrite(A4, LOW);
  pinMode(A5, INPUT);
  digitalWrite(A5, LOW);
  
  clearMatrix();
}

void loop()
{
  int read = analogRead(A5);
  
  if (!ChangedDirection)
  {
    if ((read > 900) && (CurrentDirection != Direction::Left))
    {
      CurrentDirection = Direction::Right;
      ChangedDirection = true;
    }
    else if ((read > 800) && (CurrentDirection != Direction::Right))
    {
      CurrentDirection = Direction::Left;
      ChangedDirection = true;
    }
    else if ((read > 400) && (CurrentDirection != Direction::Down))
    {
      CurrentDirection = Direction::Up;
      ChangedDirection = true;
    }
    else if ((read > 50) && (CurrentDirection != Direction::Up))
    {
      CurrentDirection = Direction::Down;
      ChangedDirection = true;
    }
  }
  
  unsigned long timeNow = millis();
  if ((timeNow - PreviousTime) >= 600)
  {
    PreviousTime = timeNow;
    
    ChangedDirection = false;
    
    moveSnake();
    
    if ((SnakeBody[0].x == Fruit.x) && (SnakeBody[0].y == Fruit.y))
    {
      if (SnakeLength < MaxSnakeLength)
      {
        ++SnakeLength;
        Fruit.x = random(0, 7);
        Fruit.y = random(0, 7);
      }
    }
  }

  drawGame();
}

void drawGame()
{
  for (byte i = 0; i < SnakeLength; i++)
  {
    drawPoint(SnakeBody[i]);
  }
  
  drawPoint(Fruit);
}

void moveSnake()
{
  for (char i = MaxSnakeLength - 2; i >= 0; --i)
  {
    SnakeBody[i + 1] = SnakeBody[i];
  }
  
  switch (CurrentDirection)
  {
    case Left:
      SnakeBody[0].x = (SnakeBody[1].x > 0 ? SnakeBody[0].x - 1 : 7);
      SnakeBody[0].y = SnakeBody[1].y;
      break;
    case Right:
      SnakeBody[0].x = (SnakeBody[1].x < 7 ? SnakeBody[0].x + 1 : 0);
      SnakeBody[0].y = SnakeBody[1].y;
      break;
    case Up:
      SnakeBody[0].x = SnakeBody[1].x;
      SnakeBody[0].y = (SnakeBody[1].y > 0 ? SnakeBody[0].y - 1 : 7);
      break;
    case Down:
      SnakeBody[0].x = SnakeBody[1].x;
      SnakeBody[0].y = (SnakeBody[1].y < 7 ? SnakeBody[0].y + 1 : 0);
      break;
  }
  
  for (byte i = 1; i < SnakeLength; i++)
  {
    if ((SnakeBody[0].x == SnakeBody[i].x) && (SnakeBody[0].y == SnakeBody[i].y))
    {
      while (true)
      {
        byte y = 0;
        for (byte x = 0; x < SnakeLength; x++)
        {
          if (x > 7)
          {
            ++y;
          }
          
          drawPoint({ x - (y * 8), y });
        }
      }
    }
  }
}

void drawPoint(const Point& point)
{
  clearMatrix();
  if ((point.x < 8) && (point.y < 8))
  {
    digitalWrite(point.x + 2, HIGH);
    digitalWrite(point.y + 10, LOW);
  }
  delay(1);
}

void clearMatrix()
{
  for (byte i = 0; i < 8; i++)
  {
    digitalWrite(i + 2, LOW);
    digitalWrite(i + 10, HIGH);
  }
}
