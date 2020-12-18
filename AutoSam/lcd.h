#ifndef lcd_h
#define lcd_h
#include "header.h"

void lcd1()
{

  if (screen == 0) //первый экран: IP и время
  {
    lcd.setCursor(0, 0); // устанавливаем курсор на первую позицию в верхней строке
    lcd.print(CurrentTime());
    lcd.setCursor(6, 0);
    lcd.print(millis2time());
    lcd.setCursor(15, 0);
    lcd.print(modesam);
    lcd.setCursor(0, 1); // устанавливаем курсор на первую позицию в нижней строке
    lcd.print(AirTemp);
    lcd.print((char)223);
    lcd.print("C "); // пишем символ градуса и символ цельсия
    lcd.print(Apressure);
    lcd.print("mm");
  }

  if (screen == 1) //первый экран: температура вверху колонны и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("ST");
    lcd.print(SteamTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 0);
    lcd.print("F");
    lcd.print(SteamTempS);
    lcd.setCursor(15, 0);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("PT");
    lcd.print(PipeTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 1);
    lcd.print("F");
    lcd.print(PipeTempS);
    lcd.setCursor(15, 1);
    lcd.print("%");
  }
  if (screen == 2) //первый экран: температура вверху колонны и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("TT");
    lcd.print(TankTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 0);
    lcd.print("S");
    lcd.print(TankTempF);
    lcd.setCursor(15, 0);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("WT");
    lcd.print(WaterTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
  }
}

void lcd2()
{

  if (screen == 0) //первый экран: IP и время
  {
    lcd.setCursor(0, 0); // устанавливаем курсор на первую позицию в верхней строке
    lcd.print(CurrentTime());
    lcd.setCursor(6, 0);
    lcd.print(millis2time());
    lcd.setCursor(15, 0);
    lcd.print(modesam);
    lcd.setCursor(0, 1); // устанавливаем курсор на первую позицию в нижней строке
    lcd.print(AirTemp);
    lcd.print((char)223);
    lcd.print("C "); // пишем символ градуса и символ цельсия
    lcd.print(Apressure);
    lcd.print("mm");
  }

  if (screen == 1) //первый экран: температура вверху колонны и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("ST");
    lcd.print(SteamTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 0);
    lcd.print("F");
    lcd.print(SteamTempS);
    lcd.setCursor(15, 0);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("TT");
    lcd.print(TankTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 1);
    lcd.print("S");
    lcd.print(TankTempF);
    lcd.setCursor(15, 1);
    lcd.print("%");
  }
}

void lcd3()
{

  if (screen == 0) //первый экран: IP и время
  {
    lcd.setCursor(0, 0); // устанавливаем курсор на первую позицию в верхней строке
    lcd.print(CurrentTime());
    lcd.setCursor(9, 0);
    lcd.print(SteamTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.print("s");

    lcd.setCursor(0, 1); // устанавливаем курсор на первую позицию в нижней строке
    lcd.print(Apressure);
    lcd.print("mm");
    lcd.setCursor(9, 1);
    lcd.print(AirTemp);
    lcd.print((char)223);
    lcd.print("C "); // пишем символ градуса и символ цельсия
    lcd.print("a");
  }
}

void lcd10()
{
  //***************************************************************************************  // обрабатываем дисплей

  if (screen == 0) //первый экран: IP и время
  {
    lcd.setCursor(0, 0); // устанавливаем курсор на первую позицию в верхней строке
    lcd.print(CurrentTime());
    lcd.setCursor(6, 0);
    lcd.print(millis2time());
    lcd.setCursor(15, 0);
    lcd.print(modesam);
    lcd.setCursor(0, 1); // устанавливаем курсор на первую позицию в нижней строке
    lcd.print(AirTemp);
    lcd.print((char)223);
    lcd.print("C "); // пишем символ градуса и символ цельсия
    lcd.print(Apressure);
    lcd.print("mm");
  }

  if (screen == 1) //первый экран: температура вверху колонны и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("Syx ");
    lcd.print(SteamTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(12, 0);
    lcd.print(deltaTs);

    lcd.setCursor(0, 1);
    lcd.print("F");
    lcd.print(SteamTempF);
    lcd.print("%");
    lcd.setCursor(9, 1);
    lcd.print("S");
    lcd.print(SteamTempS);
    lcd.print("%");
  }

  if (screen == 2) //второй экранн: пишем температуру в кубе и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("Bak ");
    lcd.print(TankTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(12, 0);
    lcd.print(deltaTs);

    lcd.setCursor(0, 1);
    lcd.print("F");
    lcd.print(TankTempF);
    lcd.print("%");
    lcd.setCursor(9, 1);
    lcd.print("S");
    lcd.print(TankTempS);
    lcd.print("%");
  }

  if (screen == 3) //третий экран: атмосферное давление и температура воздуха
  {
    lcd.setCursor(0, 0);
    lcd.print("Pressure");
    lcd.setCursor(9, 0);
    if (BMP280 == true)
    {
      lcd.print(Apressure); // пишем атмосферное давление
    }
    else
    {
      lcd.print("unknown"); // пишем заглушку
    }

    lcd.setCursor(0, 1);
    lcd.print(" Air T");
    lcd.setCursor(9, 1);
    if (BMP280 == true)
    {
      lcd.print(AirTemp); // пишем температуру воздуха
      lcd.print((char)223);
      lcd.print("C");
    } // пишем символ градуса и символ цельсия
    else
    {
      lcd.print("unknown"); // пишем заглушку
    }
  }

  if (screen == 4) //второй экран: температура флегмы/охлаждающей воды и температура в кубе
  {
    lcd.setCursor(0, 0);
    lcd.print("Water T");
    lcd.setCursor(9, 0);
    lcd.print(WaterTemp); // пишем температуру флегмы/охлаждающей воды
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия

    lcd.setCursor(0, 1);
    lcd.print(" Tank T");
    lcd.setCursor(9, 1);
    lcd.print(TankTemp); // пишем температуру в кубе
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
  }

  /*
    if(screen == 0)                 //первый экран: IP и время
     { lcd.setCursor(0, 0);         // устанавливаем курсор на первую позицию в верхней строке
    //   lcd.print("Time");             // пишем IP address
       lcd.print (CurrentTime());
       lcd.setCursor(8,0 );
       lcd.print (millis2time() );
       lcd.setCursor(0, 1);         // устанавливаем курсор на первую позицию в нижней строке
       lcd.print(WiFi.localIP()); } // пишем значение IP адреса

  */
}

#endif
