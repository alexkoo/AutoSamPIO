
#ifndef logic_h
#define logic_h

#include "header.h"

void samogon()
{
    // Управление самогонным аппаратом

    auto_status = "Дистилляция";
    if (TankTemp <= 35 && pcountsam == 0)
    {
        auto_status = "Не запущено";
        countsam = 0;
    }
    if (TankTemp > 35 && TankTemp < min_hot_temp)
    {
        auto_status = "Куб нагревается";
        countsam = 1;
    }
    if (TankTemp >= min_hot_temp && SteamTemp < min_hot_temp)
    {
        auto_status = "Сухопарник нагревается";
        countsam = 2;
    }
    if (TankTemp >= min_hot_temp && SteamTemp >= min_hot_temp && heating_rate_steam >= heating_rate)
    {
        auto_status = "Отбор голов";
        countsam = 3;
    }
    if (TankTemp >= min_hot_temp && SteamTemp >= min_hot_temp && heating_rate_steam < heating_rate)
    {
        auto_status = "Отбор тела";
        countsam = 4;
    }
    if (TankTemp >= min_hot_temp && SteamTemp >= min_hot_temp && SteamTempVolS < 45)
    {
        auto_status = "Отбор хвостов";
        countsam = 5;
    }
    if (TankTemp >= min_hot_temp && SteamTemp >= min_hot_temp && SteamTempVolS < 30)
    {
        auto_status = "Конец отбора";
        countsam = 6;
    }

    if (pcountsam != countsam)
    {
        tone(buz, 400, 200);
        pcountsam = countsam;
        telnet.println(" Статус дистилляции: ");
        telnet.print(auto_status);
    }

} //void samogon

//***********************************************************************************************************************

void rect()
{
    /*
    if (set_temp_steam == 0 && set_temp_pipe == 0) // если  обе уставки равны 0 (ручное управление)
    {
        valve_auto_mode = false;
    }
    */

    if (valve_auto_mode == true) // Управляем клапаном отбора по температуре пара перед дефлегматором SteamTemp
    {
        if (set_temp_steam != 0) // если не ручной режим управления клапаном
        {
            if (readValve() == false) // если клапан закрыт
            {
                if (millis() - valve_pause >= delay_steam * 1000) // если время задержки вышло (delay_steam задаётся в секундах),
                {
                    if (SteamTemp < set_temp_steam)
                    {
                        digitalWrite(valve, ON); // если температура ниже уставки, включаем клапан (лог. 0)
                        auto_status = "Opened, Auto, Steam";
                        beep();
                    }
                    else
                        valve_pause = millis(); // если температура всё ещё выше уставки, заводим таймер снова
                }
            }
            else // если клапан открыт
            {
                if (SteamTemp >= set_temp_steam) // если температура выше уставки,
                {
                    digitalWrite(valve, OFF); // выключаем клапан отбора
                    auto_status = "Closed, Auto, Steam";
                    beep();
                    valve_pause = millis(); // заводим таймер
                }
            }
        }

        //***************************************************************************************************************************
        // Управляем клапаном отбора по температуре пара в царге на 2/3 колонны PipeTemp
        if (set_temp_pipe != 0) // если не ручной режим управления клапаном
        {
            if (readValve() == false) // если клапан закрыт
            {
                if (millis() - valve_pause >= delay_pipe * 1000) // если время задержки вышло (delay_pipe задаётся в секундах),
                {
                    if (PipeTemp < set_temp_pipe)
                    {
                        digitalWrite(valve, ON); // если температура ниже уставки, включаем клапан (лог. 0)
                        auto_status = "Opened, Auto, Pipe";
                        beep();
                    }
                    else
                        valve_pause = millis(); // если температура всё ещё выше уставки, заводим таймер снова
                }
            }
            else // если клапан открыт
            {
                if (PipeTemp >= set_temp_pipe) // если температура выше уставки,
                {
                    digitalWrite(valve, OFF); // выключаем клапан отбора
                    auto_status = "Closed, Auto, Pipe";

                    beep();

                    valve_pause = millis();
                } // заводим таймер
            }
        }
    }

} // void rect

#endif
