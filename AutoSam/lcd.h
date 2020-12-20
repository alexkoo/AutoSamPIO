#ifndef lcd_h
#define lcd_h
#include "header.h"

void lcd1()
{

  if (lcd_num == 0) //первый экран: IP и время
  {
    lcd.setCursor(0, 0); // устанавливаем курсор на первую позицию в верхней строке
    lcd.print(CurrentTime());
    lcd.setCursor(6, 0);
    lcd.print(millis2time());
    lcd.setCursor(15, 0);
    lcd.print(autosam_mode);
    lcd.setCursor(0, 1); // устанавливаем курсор на первую позицию в нижней строке
    lcd.print(air_temp);
    lcd.print((char)223);
    lcd.print("C "); // пишем символ градуса и символ цельсия
    lcd.print(atm_pressure);
    lcd.print("mm");
  }

  if (lcd_num == 1) //первый экран: температура вверху колонны и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("ST");
    lcd.print(SteamTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 0);
    lcd.print("F");
    lcd.print(SteamTempVolS);
    lcd.setCursor(15, 0);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("PT");
    lcd.print(PipeTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 1);
    lcd.print("F");
    lcd.print(PipeTempVolS);
    lcd.setCursor(15, 1);
    lcd.print("%");
  }
  if (lcd_num == 2) //первый экран: температура вверху колонны и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("TT");
    lcd.print(TankTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 0);
    lcd.print("S");
    lcd.print(TankTempVolF);
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

  if (lcd_num == 0) //первый экран: IP и время
  {
    lcd.setCursor(0, 0); // устанавливаем курсор на первую позицию в верхней строке
    lcd.print(CurrentTime());
    lcd.setCursor(6, 0);
    lcd.print(millis2time());
    lcd.setCursor(15, 0);
    lcd.print(autosam_mode);
    lcd.setCursor(0, 1); // устанавливаем курсор на первую позицию в нижней строке
    lcd.print(air_temp);
    lcd.print((char)223);
    lcd.print("C "); // пишем символ градуса и символ цельсия
    lcd.print(atm_pressure);
    lcd.print("mm");
  }

  if (lcd_num == 1) //первый экран: температура вверху колонны и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("ST");
    lcd.print(SteamTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 0);
    lcd.print("F");
    lcd.print(SteamTempVolS);
    lcd.setCursor(15, 0);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("TT");
    lcd.print(TankTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 1);
    lcd.print("S");
    lcd.print(TankTempVolF);
    lcd.setCursor(15, 1);
    lcd.print("%");
  }
}

void lcd3()
{

  if (lcd_num == 0) //первый экран: IP и время
  {
    lcd.setCursor(0, 0); // устанавливаем курсор на первую позицию в верхней строке
    lcd.print(CurrentTime());
    lcd.setCursor(9, 0);
    lcd.print(SteamTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.print("s");

    lcd.setCursor(0, 1); // устанавливаем курсор на первую позицию в нижней строке
    lcd.print(atm_pressure);
    lcd.print("mm");
    lcd.setCursor(9, 1);
    lcd.print(air_temp);
    lcd.print((char)223);
    lcd.print("C "); // пишем символ градуса и символ цельсия
    lcd.print("a");
  }
}

void lcd10()
{
  //***************************************************************************************  // обрабатываем дисплей

  if (lcd_num == 0) //первый экран: IP и время
  {
    lcd.setCursor(0, 0); // устанавливаем курсор на первую позицию в верхней строке
    lcd.print(CurrentTime());
    lcd.setCursor(6, 0);
    lcd.print(millis2time());
    lcd.setCursor(15, 0);
    lcd.print(autosam_mode);
    lcd.setCursor(0, 1); // устанавливаем курсор на первую позицию в нижней строке
    lcd.print(air_temp);
    lcd.print((char)223);
    lcd.print("C "); // пишем символ градуса и символ цельсия
    lcd.print(atm_pressure);
    lcd.print("mm");
  }

  if (lcd_num == 1) //первый экран: температура вверху колонны и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("Syx ");
    lcd.print(SteamTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(12, 0);
    lcd.print(heating_rate_steam);

    lcd.setCursor(0, 1);
    lcd.print("F");
    lcd.print(SteamTempVolF);
    lcd.print("%");
    lcd.setCursor(9, 1);
    lcd.print("S");
    lcd.print(SteamTempVolS);
    lcd.print("%");
  }

  if (lcd_num == 2) //второй экранн: пишем температуру в кубе и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("Bak ");
    lcd.print(TankTemp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(12, 0);
    lcd.print(heating_rate_steam);

    lcd.setCursor(0, 1);
    lcd.print("F");
    lcd.print(TankTempVolF);
    lcd.print("%");
    lcd.setCursor(9, 1);
    lcd.print("S");
    lcd.print(TankTempVolS);
    lcd.print("%");
  }

  if (lcd_num == 3) //третий экран: атмосферное давление и температура воздуха
  {
    lcd.setCursor(0, 0);
    lcd.print("Pressure");
    lcd.setCursor(9, 0);
    if (BMP280 == true)
    {
      lcd.print(atm_pressure); // пишем атмосферное давление
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
      lcd.print(air_temp); // пишем температуру воздуха
      lcd.print((char)223);
      lcd.print("C");
    } // пишем символ градуса и символ цельсия
    else
    {
      lcd.print("unknown"); // пишем заглушку
    }
  }

  if (lcd_num == 4) //второй экран: температура флегмы/охлаждающей воды и температура в кубе
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
    if(lcd_num == 0)                 //первый экран: IP и время
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
