#include "header.h"

void samogon()
{
    // Управление самогонным аппаратом
    // TankTemp температура в баке
    // SteamTemp - температура в сухопарнике

    if (TankTemp <= 35 && pcountsam == 0)
    {
        warning = "Не запущено";
        countsam = 0;
    }
    if (TankTemp > 35 && TankTemp < minBoil)
    {
        warning = "Куб нагревается";
        countsam = 1;
    }
    if (TankTemp >= minBoil && SteamTemp < minBoil)
    {
        warning = "Сухопарник нагревается";
        countsam = 2;
    }
    if (TankTemp >= minBoil && SteamTemp >= minBoil && deltaTs >= deltaT0)
    {
        warning = "Отбор голов";
        countsam = 3;
    }
    if (TankTemp >= minBoil && SteamTemp >= minBoil && deltaTs < deltaT0)
    {
        warning = "Отбор тела";
        countsam = 4;
    }
    if (TankTemp >= minBoil && SteamTemp >= minBoil && SteamTempS < 45)
    {
        warning = "Отбор хвостов";
        countsam = 5;
    }
    if (TankTemp >= minBoil && SteamTemp >= minBoil && SteamTempS < 30)
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
    // Управляем клапаном отбора по температуре пара перед дефлегматором SteamTemp
    if (SetTemp1 != 0) // если не ручной режим управления клапаном

        if (valveset == true && SetTemp1 == 0 && SetTemp2 == 0) // если передан номер кнопки 6 и обе уставки равны 0 (ручное управление)
        {
            digitalWrite(valve, LOW);                   // включаем клапан (лог. 0)
            telnet.println("Valve is OPENED manually"); // выводим сообщение на UART
            AutoStatus = "Opened, Man";
        }
    {
        if (digitalRead(valve) == true) // если клапан закрыт
        {
            if (millis() - valve_pause >= Delay1 * 1000) // если время задержки вышло (Delay1 задаётся в секундах),
            {
                if (SteamTemp < SetTemp1)
                {
                    digitalWrite(valve, LOW); // если температура ниже уставки, включаем клапан (лог. 0)
                    telnet.println("Valve is OPENED automatically");
                    AutoStatus = "Opened, Auto";
                }
                else
                    valve_pause = millis(); // если температура всё ещё выше уставки, заводим таймер снова
            }
        }
        else // если клапан открыт
        {
            if (SteamTemp >= SetTemp1) // если температура выше уставки,
            {
                digitalWrite(valve, HIGH); // выключаем клапан отбора
                telnet.println("Valve is CLOSED automatically");
                AutoStatus = "Closed, Auto";
                valve_pause = millis();
            } // заводим таймер
        }
    }
    //***************************************************************************************************************************
    // Управляем клапаном отбора по температуре пара в царге на 2/3 колонны PipeTemp
    if (SetTemp2 != 0) // если не ручной режим управления клапаном

        if (valveset == true && SetTemp1 == 0 && SetTemp2 == 0) // если передан номер кнопки 7 и обе уставки равны 0 (ручное управление)
        {
            digitalWrite(valve, HIGH); // выключаем клапан (лог. 1)

            telnet.println("Valve is CLOSED manually"); // выводим сообщение на UART
            AutoStatus = "Closed, Man";
        }

    {
        if (digitalRead(valve) == true) // если клапан закрыт
        {
            if (millis() - valve_pause >= Delay2 * 1000) // если время задержки вышло (Delay2 задаётся в секундах),
            {
                if (PipeTemp < SetTemp2)
                {
                    digitalWrite(valve, LOW); // если температура ниже уставки, включаем клапан (лог. 0)

                    telnet.println("Valve is OPENED automatically");
                    AutoStatus = "Opened, Auto";
                }
                else
                    valve_pause = millis(); // если температура всё ещё выше уставки, заводим таймер снова
            }
        }
        else // если клапан открыт
        {
            if (PipeTemp >= SetTemp2) // если температура выше уставки,
            {
                digitalWrite(valve, HIGH); // выключаем клапан отбора
                telnet.println("Valve is CLOSED automatically");
                AutoStatus = "Closed, Auto";
                valve_pause = millis();
            } // заводим таймер
        }
    }

    if (TankTemp >= 35 && TankTemp < minBoil && SteamTemp < minBoil)
    {
        warning = "Куб нагревается";
        countsam = 1;
    }
    if (TankTemp >= minBoil && SteamTemp < minBoil)
    {
        warning = "Колонна нагревается";
        countsam = 2;
    }

    if (TankTemp >= minBoil && SteamTemp >= minBoil)
    {
        warning = "Колонна нагрелась";
        countsam = 3;
    }
    if (TankTemp >= maxTank && SteamTemp >= minBoil)
    {
        warning = "Конец отбора";
        countsam = 4;
    }

    if (SetTemp1 != 0 && SteamTemp >= SetTemp1)
    {
        warning = "Уменьшить отбор (Отбор)";
        countsam = 5;
    }

    if (SetTemp2 != 0 && PipeTemp >= SetTemp2)
    {
        warning = "Уменьшить отбор (Царга)";
        countsam = 6;
    }

    if (pcountsam != countsam)
    {
        tone(buz, 400, 200);
        pcountsam = countsam;
        telnet.println(" Статус ректификации: ");
        telnet.print(warning);
    }
} // void rect