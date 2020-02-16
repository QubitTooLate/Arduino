/*
  0 = 5 | 8 = A3
  1 = 3 | 9 = A2
  2 = 11|10 = 8
  3 = 12|11 = 10
  4 = 2 |12 = 6
  5 = A0|13 = A1
  6 = 4 |14 = 13
  7 = 7 |15 = 9
*/

typedef struct defPoint
{
  char x;
  char y;
} Point;

Point point;

void setup()
{
  for (byte i = 2; i <= (2 + 16); i++)
  {
    pinMode(i, OUTPUT);
  }
  
  clearMatrix();
  
  Serial.begin(9600);
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

void loop()
{
  if (Serial.available() > 0)
  {
    Serial.readBytes((char*)&point, 2);
    clearMatrix();
    drawPoint(point);
  }
}