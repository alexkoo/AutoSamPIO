
#ifndef logic_h
#define logic_h

#include "header.h"

void samogon()
{
    // Управление самогонным аппаратом

    auto_status = "Дистилляция";
    if (tank_temp <= 35 && pcountsam == 0)
    {
        auto_status = "Не запущено";
        countsam = 0;
    }
    if (tank_temp > 35 && tank_temp < min_hot_temp)
    {
        auto_status = "Куб нагревается";
        countsam = 1;
    }
    if (tank_temp >= min_hot_temp && steam_temp < min_hot_temp)
    {
        auto_status = "Сухопарник нагревается";
        countsam = 2;
    }
    if (tank_temp >= min_hot_temp && steam_temp >= min_hot_temp && heating_rate_steam >= heating_rate)
    {
        auto_status = "Отбор голов";
        countsam = 3;
    }
    if (tank_temp >= min_hot_temp && steam_temp >= min_hot_temp && heating_rate_steam < heating_rate)
    {
        auto_status = "Отбор тела";
        countsam = 4;
    }
    if (tank_temp >= min_hot_temp && steam_temp >= min_hot_temp && steam_temp_alc_st < 45)
    {
        auto_status = "Отбор хвостов";
        countsam = 5;
    }
    if (tank_temp >= min_hot_temp && steam_temp >= min_hot_temp && steam_temp_alc_st < 30)
    {
        auto_status = "Конец отбора";
        countsam = 6;
    }

    if (pcountsam != countsam)
    {
        tone(buzzer_pin, 400, 200);
        pcountsam = countsam;
        telnet.println(" Статус дистилляции: ");
        telnet.print(auto_status);
    }

} //void samogon

//***********************************************************************************************************************

void rectification()
{
    /*
    if (set_temp_steam == 0 && set_temp_pipe == 0) // если  обе уставки равны 0 (ручное управление)
    {
        valve_auto_mode = false;
    }
    */

    if (valve_auto_mode == true) // Управляем клапаном отбора по температуре пара перед дефлегматором steam_temp
    {
        if (set_temp_steam != 0) // если не ручной режим управления клапаном
        {
            if (readValve() == false) // если клапан закрыт
            {
                if (millis() - valve_pause >= delay_steam * 1000) // если время задержки вышло (delay_steam задаётся в секундах),
                {
                    if (steam_temp < set_temp_steam)
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
                if (steam_temp >= set_temp_steam) // если температура выше уставки,
                {
                    digitalWrite(valve, OFF); // выключаем клапан отбора
                    auto_status = "Closed, Auto, Steam";
                    beep();
                    valve_pause = millis(); // заводим таймер
                }
            }
        }

        //***************************************************************************************************************************
        // Управляем клапаном отбора по температуре пара в царге на 2/3 колонны pipe_temp
        if (set_temp_pipe != 0) // если не ручной режим управления клапаном
        {
            if (readValve() == false) // если клапан закрыт
            {
                if (millis() - valve_pause >= delay_pipe * 1000) // если время задержки вышло (delay_pipe задаётся в секундах),
                {
                    if (pipe_temp < set_temp_pipe)
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
                if (pipe_temp >= set_temp_pipe) // если температура выше уставки,
                {
                    digitalWrite(valve, OFF); // выключаем клапан отбора
                    auto_status = "Closed, Auto, Pipe";

                    beep();

                    valve_pause = millis();
                } // заводим таймер
            }
        }
    }

} // void rectification

#endif
