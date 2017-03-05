#include <Arduino.h>
#include <LiquidCrystal_I2C/LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Encoder/Encoder.h>
#include <Keypad/Keypad.h>

#define buttPin 4
#define tapButton 3

LiquidCrystal_I2C lcd(0x27,20,4);
Encoder myEnc(5, 6);

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns

String strNum1 = "";
String strNum2 = "";
String stringWord = "";
String strCode = "";
String shirmShablon = "____";
String calcProgres = "";
String calcProgres2 = "";
String pustishka = "";

uint8_t operationCode = 0;
uint8_t part = 0;
uint8_t project = 0;

int encOld;
int encNew;
int debounceTime = 200;
int numCode;
int tmChapter = 0;

unsigned long activateDebounce;
unsigned long tmCurrent, tmResult = 500.0, bpmDotTm, debounce;

bool menuKeySelect = true;
bool dot = true;

float num1 = 0;
float num2 = 0;
float result = 0;
float bpm, oldBpm;

char menueSelect;

char keys[ROWS][COLS] =
        {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
        };

char alph[30]{'a','b','c','d','e','f','g','h','i','j',
              'k','l','m','n','o','p','q','r','s','t',
              'u','v','w','x','y','z',' ','.','<','_'};

byte rowPins[ROWS] = {36, 34, 32, 30}; //connect to the row pinouts of the keyb
byte colPins[COLS] = {28, 26, 24, 22}; //connect to the column pinouts of the keyb

Keypad keyb = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void Calculator()
{
    char key = keyb.getKey();

    switch (part)
    {
        case 0:
        {
            if(key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' ||
               key == '7' || key == '8' || key == '9' || key == '0')
            {
                strNum1 = strNum1 + key;
                num1 = strNum1.toFloat();
                calcProgres = pustishka + num1;

                lcd.clear();
                lcd.print(strNum1);
                lcd.setCursor(0, 2);
                lcd.print(calcProgres);

                Serial.println(strNum1);
            }

            switch (key)
            {
                case '*':
                {
                    strNum1 = strNum1 + '.';
                    num1 = strNum1.toFloat();
                    calcProgres = pustishka + num1;

                    lcd.clear();
                    lcd.print(strNum1);
                    lcd.setCursor(0, 2);
                    lcd.print(calcProgres);

                    Serial.println(strNum1);
                }
                    break;
                case 'A':
                {
                    operationCode = 1;//+
                    part++;

                    if(strNum1 == "")
                    {
                        num1 = result;
                        calcProgres = pustishka + result + " + ";
                    } else
                    {
                        calcProgres = calcProgres + " + ";
                    }

                    lcd.clear();
                    lcd.print("+");
                    lcd.setCursor(0, 2);
                    lcd.print(calcProgres);

                    Serial.println("+");
                }
                    break;
                case 'B':
                {
                    operationCode = 2;//-
                    part++;

                    if(strNum1 == "")
                    {
                        num1 = result;
                        calcProgres = pustishka + result + " - ";
                    } else
                    {
                        calcProgres = calcProgres + " - ";
                    }

                    lcd.clear();
                    lcd.print("-");
                    lcd.setCursor(0, 2);
                    lcd.print(calcProgres);

                    Serial.println("-");
                }
                    break;
                case 'C':
                {
                    operationCode = 3;//*
                    part++;

                    if(strNum1 == "")
                    {
                        num1 = result;
                        calcProgres = pustishka + result + " * ";
                    }else
                    {
                        calcProgres = calcProgres + " * ";
                    }

                    lcd.clear();
                    lcd.print("*");
                    lcd.setCursor(0, 2);
                    lcd.print(calcProgres);

                    Serial.println("*");
                }
                    break;
                case 'D':
                {
                    operationCode = 4;// /
                    part++;

                    if(strNum1 == "")
                    {
                        num1 = result;
                        calcProgres = pustishka + result + " / ";
                    }else
                    {
                        calcProgres = calcProgres + " / ";
                    }

                    lcd.clear();
                    lcd.print("/");
                    lcd.setCursor(0, 2);
                    lcd.print(calcProgres);

                    Serial.println("/");
                }
                    break;
            }
            break;
        }

        case 1:
        {
            if(key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' ||
               key == '7' || key == '8' || key == '9' || key == '0')
            {
                strNum2 = strNum2 + key;
                num2 = strNum2.toFloat();
                calcProgres2 = pustishka + num2;

                lcd.clear();
                lcd.print(strNum2);
                lcd.setCursor(0, 2);
                lcd.print(calcProgres);
                lcd.print(calcProgres2);

                Serial.println(strNum2);
            }

            switch (key)
            {
                case '*':
                {
                    strNum2 = strNum2 + '.';
                    num2 = strNum2.toFloat();
                    calcProgres2 = pustishka + num2;

                    lcd.clear();
                    lcd.print(strNum2);
                    lcd.setCursor(0, 2);
                    lcd.print(calcProgres);
                    lcd.print(calcProgres2);

                    Serial.println(strNum2);
                }
                    break;
                case '#':
                {
                    switch (operationCode)
                    {
                        case 1:
                        {
                            lcd.clear();
                            lcd.setCursor(0,2);
                            lcd.print(calcProgres);
                            lcd.print(calcProgres2);
                            lcd.setCursor(0,3);
                            lcd.print("= ");
                            result = num1 + num2;
                            lcd.print(result);

                            Serial.print(num1);
                            Serial.print(" + ");
                            Serial.print(num2);
                            Serial.print(" = ");
                            Serial.println(result);
                        }
                            break;
                        case 2:
                        {
                            lcd.clear();
                            lcd.setCursor(0,2);
                            lcd.print(calcProgres);
                            lcd.print(calcProgres2);
                            lcd.setCursor(0,3);
                            lcd.print("= ");
                            result = num1 - num2;
                            lcd.print(result);

                            Serial.print(num1);
                            Serial.print(" - ");
                            Serial.print(num2);
                            Serial.print(" = ");
                            Serial.println(result);
                        }
                            break;
                        case 3:
                        {
                            lcd.clear();
                            lcd.setCursor(0,2);
                            lcd.print(calcProgres);
                            lcd.print(calcProgres2);
                            lcd.setCursor(0,3);
                            lcd.print("= ");
                            result = num1 * num2;
                            lcd.print(result);

                            Serial.print(num1);
                            Serial.print(" * ");
                            Serial.print(num2);
                            Serial.print(" = ");
                            Serial.println(result);
                        }
                            break;
                        case 4:
                        {
                            lcd.clear();
                            lcd.setCursor(0,2);
                            lcd.print(calcProgres);
                            lcd.print(calcProgres2);
                            lcd.setCursor(0,3);
                            lcd.print("= ");
                            result = num1 / num2;
                            lcd.print(result);

                            Serial.print(num1);
                            Serial.print(" / ");
                            Serial.print(num2);
                            Serial.print(" = ");
                            Serial.println(result);
                        }
                            break;
                    }
                    calcProgres = "";
                    strNum1 = "";
                    strNum2 = "";
                    num1 = 0;
                    num2 = 0;
                    operationCode = 0;
                    part = 0;
                }
                    break;
            }
            break;
        }
    }
}

void AddWord()
{
    encNew = myEnc.read();
    encNew /= 2;

    if (encNew != encOld)
    {
        if(encNew > 28)
        {
            encNew = 0;
            myEnc.write(encNew);
        }
        if(encNew < 0)
        {
            encNew = 28;
            myEnc.write(encNew * 2);
        }
        lcd.clear();
        lcd.print(stringWord);
        lcd.print(alph[encNew]);

        encOld = encNew;
    }

    if(digitalRead(buttPin) == 0)
    {
        if(millis() >= activateDebounce)
        {
            if(encNew == 27)
            {
                lcd.clear();

                if(stringWord != "")
                {
                    lcd.setCursor(0,3);
                    lcd.print("Word: ");
                    lcd.print(stringWord);

                    stringWord = "";
                } else
                {
                    encNew = 0;
                    myEnc.write(encNew);

                    lcd.print(alph[encNew]);
                }
            }
            else if (encNew == 28)
            {
                stringWord.remove(stringWord.length() - 1);

                lcd.clear();
                lcd.print(stringWord);
                lcd.print(alph[encNew]);
            }
            else
            {
                stringWord = stringWord + alph[encNew];

                lcd.clear();
                lcd.print(stringWord);
                lcd.print(alph[29]);
            }

            activateDebounce = millis() + debounceTime;
        }

    }
}

void CodeLock()
{
    char key = keyb.getKey();

    if(key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' ||
       key == '7' || key == '8' || key == '9' || key == '0')
    {
        strCode = strCode + key;
        numCode = strCode.toInt();

        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Enter pass");
        lcd.setCursor(7, 1);
        lcd.print(numCode);
        shirmShablon.remove(shirmShablon.length()-1);

        lcd.print(shirmShablon);
        lcd.setCursor(3, 3);
        lcd.print("Status: ");

        if(shirmShablon.length() == 0)
        {
            strCode = "";
            shirmShablon = "____";

            lcd.clear();
            lcd.setCursor(4, 0);
            lcd.print("Enter pass");
            lcd.setCursor(7, 1);
            lcd.print(numCode);
            lcd.setCursor(3, 3);
            lcd.print("Status: ");
        }

        if(numCode == 6063)
        {
            lcd.print("Opened");
        } else
        {
            lcd.print("Locked");
        }
    }
}

void GetBpmTap()
{
    if(digitalRead(tapButton) == 0 && millis() >= debounce)
    {
        if(tmChapter == 0)
        {
            tmCurrent = millis();
            tmChapter++;
        }
        else
        {
            tmChapter--;
            tmResult = millis() - tmCurrent;
            bpm = 1000.0 / tmResult * 60.0;
        }
        debounce = millis() + 200;
    }
}

void setup()
{
    Serial.begin(57600);
    lcd.begin();
    lcd.clear();
    lcd.print("SELECT DEMO PROJECT:");
    lcd.setCursor(0, 1);
    lcd.print("1-Calc 2-EncodeEnter");
    lcd.setCursor(0, 2);
    lcd.print("3-Code 4-BPM Counter");

    pinMode(buttPin,INPUT_PULLUP);
    pinMode(tapButton, INPUT_PULLUP);
    bpm = 1000.0 / tmResult * 60.0;
}

void loop()
{
    if(menuKeySelect)
    {
        menueSelect = keyb.getKey();
    }

    if (menueSelect == '1')
    {
        menuKeySelect = false;
        lcd.clear();
        lcd.print(0);
        project = 1;
    }

    if (menueSelect == '2')
    {
        menuKeySelect = false;
        lcd.clear();
        lcd.print(alph[encNew]);
        project = 2;
    }

    if (menueSelect == '3')
    {
        menuKeySelect = false;
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Enter pass");
        lcd.setCursor(7, 1);
        lcd.print(shirmShablon);
        lcd.setCursor(3, 3);
        lcd.print("Status: Locked");
        project = 3;
    }

    if (menueSelect == '4')
    {
        menuKeySelect = false;
        project = 4;
    }

    switch (project)
    {
        case 1:
        {
            Calculator();
            menueSelect = '0';
        }
            break;
        case 2:
        {
            AddWord();
            menueSelect = '0';
        }
            break;
        case 3:
        {
            CodeLock();
            menueSelect = '0';
        }
            break;
        case 4:
        {
            GetBpmTap();
            if(bpm != oldBpm)
            {
                lcd.clear();
                lcd.print(bpm);
                oldBpm = bpm;
            }
            if(millis() >= bpmDotTm)
            {
                lcd.setCursor(19, 0);
                if(dot)
                {
                    lcd.print("*");
                    dot = false;
                } else
                {
                    lcd.print(" ");
                    dot = true;
                }
                bpmDotTm = millis() + tmResult / 2;
            }
            menueSelect = '0';
        }
            break;
    }
}