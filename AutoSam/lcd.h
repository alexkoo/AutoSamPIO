#pragma once
#include "header.h"

static uint8_t lcd_max_num = 2;                //количество экранов от 0
 uint8_t lcd_num = 0;           // переменная "экран", от которой зависит, какие показания будут выводиться на дисплей
 uint32_t lcd_timer_set = 2000; // задержка автопереключения экрана, мс

void lcdChange()
{
  static uint32_t lcd_timer;                // таймер автопереключения экрана
  if (millis() - lcd_timer > lcd_timer_set) // автопереключение экранов
  {
    lcd_timer = millis();
    lcd_num++;
    lcd.clear();
    if (lcd_num > lcd_max_num)
      lcd_num = 0;
  }
}

void lcd0() // общиЙ 0 экран
{
  if (lcd_num == 0) // первый экран: IP и время
  {
    lcd.setCursor(0, 0); // устанавливаем курсор на первую позицию в верхней строке
    lcd.print(CurrentTime());
    lcd.setCursor(6, 0);
    lcd.print(millis2time());
    lcd.setCursor(15, 0);
    lcd.print(autosam_mode);
    lcd.setCursor(0, 1); // устанавливаем курсор на первую позицию в нижней строке
    lcd.print(air_temp);
    lcd.setCursor(4, 1);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(8, 1);
    lcd.print(atm_pressure);
    lcd.print("mm");
  }
}


void lcd1() // общиЙ 1 экран
{
  if (lcd_num == 1) // первый экран: температура вверху колонны и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("ST");
    lcd.print(steam_temp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 0);
    lcd.print("F");
    lcd.print(steam_temp_alc_st);
    lcd.setCursor(15, 0);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("PT");
    lcd.print(pipe_temp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 1);
    lcd.print("F");
    lcd.print(pipe_temp_alc_st);
    lcd.setCursor(15, 1);
    lcd.print("%");
  }
}

void lcdRect() // ректификация
{
  lcd0();
  lcd1();
   if (lcd_num == 2) // второй экран: температура вверху колонны и спирутозность
  {
    lcd.setCursor(0, 0);
    lcd.print("TT");
    lcd.print(tank_temp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
    lcd.setCursor(10, 0);
    lcd.print("S");
    lcd.print(tank_temp_alc_fl);
    lcd.setCursor(15, 0);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("WT");
    lcd.print(water_temp);
    lcd.print((char)223);
    lcd.print("C"); // пишем символ градуса и символ цельсия
  }
}

void lcdSam()
{
  lcd0();
  lcd1();
}

void lcdTerm()
{
  if (lcd_num == 0) // первый экран: IP и время
  {
    lcd.setCursor(0, 0); // устанавливаем курсор на первую позицию в верхней строке
    lcd.print(CurrentTime());
    lcd.setCursor(9, 0);
    lcd.print(steam_temp);
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

