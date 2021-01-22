
#ifndef logic_h
#define logic_h

#include "header.h"




void samogon()
{
    // Управление самогонным аппаратом

    auto_status = "Дистилляция";
    if (TankTemp <= 35 && pcountsam == 0)
    {
        warning = "Не запущено";
        countsam = 0;
    }
    if (TankTemp > 35 && TankTemp < min_hot_temp)
    {
        warning = "Куб нагревается";
        countsam = 1;
    }
    if (TankTemp >= min_hot_temp && SteamTemp < min_hot_temp)
    {
        warning = "Сухопарник нагревается";
        countsam = 2;
    }
    if (TankTemp >= min_hot_temp && SteamTemp >= min_hot_temp && heating_rate_steam >= heating_rate)
    {
        warning = "Отбор голов";
        countsam = 3;
    }
    if (TankTemp >= min_hot_temp && SteamTemp >= min_hot_temp && heating_rate_steam < heating_rate)
    {
        warning = "Отбор тела";
        countsam = 4;
    }
    if (TankTemp >= min_hot_temp && SteamTemp >= min_hot_temp && SteamTempVolS < 45)
    {
        warning = "Отбор хвостов";
        countsam = 5;
    }
    if (TankTemp >= min_hot_temp && SteamTemp >= min_hot_temp && SteamTempVolS < 30)
    {
        warning = "Конец отбора";
        countsam = 6;
    }

    if (pcountsam != countsam)
    {
        tone(buz, 400, 200);
        pcountsam = countsam;
        telnet.println(" Статус дистилляции: ");
        telnet.print(warning);
    }

} //void samogon

//***********************************************************************************************************************

void rect()
{
    if (set_temp_steam == 0 && set_temp_pipe == 0) // если передан номер кнопки 7 и обе уставки равны 0 (ручное управление)
    {

        valve_auto_mode = false;
    }

    if (valve_set == false && readValve() == false && valve_auto_mode == false && auto_status != "Closed, Man" ) // если передан номер кнопки 7 и обе уставки равны 0 (ручное управление)
    {
        digitalWrite(valve, OFF); // выключаем клапан 
        auto_status = "Closed, Man";
        beep();
    }

    if (valve_set == true && readValve() == true && valve_auto_mode == false && auto_status != "Opened, Man") // ручное управление, клапан включен
    {
        digitalWrite(valve, ON);
        auto_status = "Opened, Man";
        beep();
    }

    // Управляем клапаном отбора по температуре пара перед дефлегматором SteamTemp
    if (set_temp_steam != 0) // если не ручной режим управления клапаном
    {
        if (digitalRead(valve) == false) // если клапан закрыт
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

    if (TankTemp >= 35 && TankTemp < min_hot_temp && SteamTemp < min_hot_temp)
    {
        warning = "Куб нагревается";
        countsam = 1;
    }
    if (TankTemp >= min_hot_temp && SteamTemp < min_hot_temp)
    {
        warning = "Колонна нагревается";
        countsam = 2;
    }

    if (TankTemp >= min_hot_temp && SteamTemp >= min_hot_temp)
    {
        warning = "Колонна нагрелась";
        countsam = 3;
    }
    if (TankTemp >= max_tank_temp && SteamTemp >= min_hot_temp)
    {
        warning = "Конец отбора";
        countsam = 4;
    }

    if (set_temp_steam != 0 && SteamTemp > set_temp_steam)
    {
        warning = "Уменьшить отбор (Отбор)";
        countsam = 5;
    }

    if (set_temp_pipe != 0 && PipeTemp > set_temp_pipe)
    {
        warning = "Уменьшить отбор (Царга)";
        countsam = 6;
    }

    if (pcountsam != countsam)
    {
        tone(buz, 400, 100);
        pcountsam = countsam;
        telnet.print(" Статус ректификации: ");
        telnet.println(warning);
    }
} // void rect

#endif
