
#pragma once
#include "header.h"

void temp_status()
{
    // Управление самогонным аппаратом

    uint8_t process_status_prev = 0;

    if (tank_temp < 35)
    {
        status_process = 0;
    }

    if (tank_temp >= 35 && tank_temp < min_hot_temp)
    {
        status_process = 1;
    }
    if ((tank_temp >= min_hot_temp && (steam_temp || pipe_temp) < min_hot_temp && (heating_rate_steam || heating_rate_pipe) < heating_rate))
    {
        status_process = 2;
    }
    if (tank_temp >= min_hot_temp && (steam_temp || pipe_temp) < min_hot_temp && (heating_rate_steam || heating_rate_pipe) >= heating_rate)
    {
        status_process = 3;
    }

    if (tank_temp >= min_hot_temp && (steam_temp || pipe_temp) >= min_hot_temp && (heating_rate_steam || heating_rate_pipe) >= heating_rate)
    {
        status_process = 4;
    }

    if (tank_temp >= min_hot_temp && (steam_temp || pipe_temp) >= min_hot_temp && (heating_rate_steam || heating_rate_pipe) < heating_rate)
    {
        status_process = 5;
    }

    if (tank_temp >= min_hot_temp && (steam_temp || pipe_temp) >= min_hot_temp && steam_temp_alc_st < 45)
    {
        status_process = 6;
    }
    if (tank_temp >= min_hot_temp && steam_temp >= min_hot_temp && steam_temp_alc_st < 20)
    {
        status_process = 7;
    }

    if (process_status_prev != status_process)
    {
        tone(buzzer_pin, 400, 200);

        // telnet.println(" process_status ");
        // telnet.print(process_status);
        if ((status_process = (3 || 4)))
        {
            for (int i = 0; i < 4; i++)
            {
                tone(buzzer_pin, 400, 200);
            }
        }

        process_status_prev = status_process;
    }

} // void samogon

//***********************************************************************************************************************

void rectification()
{

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
                        digitalWrite(valve_pin, ON); // если температура ниже уставки, включаем клапан (лог. 0)
                        status_auto = 1;
                        status_valve = 1;
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
                    digitalWrite(valve_pin, OFF); // выключаем клапан отбора
                    status_auto = 1;
                    status_valve = 0;
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
                        digitalWrite(valve_pin, ON); // если температура ниже уставки, включаем клапан (лог. 0)
                        status_auto = 2;
                        status_valve = 1;
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
                    digitalWrite(valve_pin, OFF); // выключаем клапан отбора
                    status_auto = 2;
                    status_valve = 0;
                    beep();

                    valve_pause = millis();
                } // заводим таймер
            }
        }
    }

} // void rectification
