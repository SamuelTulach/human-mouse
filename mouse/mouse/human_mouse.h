#pragma once

/*
 * HumanMouse
 * Made by Samuel Tulach
 * Based on:
 * - https://github.com/SRL/SRL-6/blob/master/lib/core/mouse.simba
*/

#include <cmath>
#include <cstdint>
#include <random>
#include <thread>
#include <Windows.h>

class HumanMouse
{
private:
    int32_t TargetX = 0;
    int32_t TargetY = 0;
    std::thread MainThread;
    bool ShouldExit = false;
    int32_t MouseSpeed = 10;

    void MoveSystem(int32_t x, int32_t y)
    {
        mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
    }

    double Distance(double x1, double y1, double x2, double y2)
    {
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

    double Hypot(double x, double y)
    {
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    int Random(int maxValue)
    {
        if (maxValue == 0)
            return 0;
        return rand() % maxValue;
    }

    void ThreadEntry()
    {
        srand(time(nullptr));

        double veloX = 0;
        double veloY = 0;
        double windX = 0;
        double windY = 0;

        auto msp = MouseSpeed;
        auto sqrt2 = sqrt(2);
        auto sqrt3 = sqrt(3);
        auto sqrt5 = sqrt(5);

        while (true)
        {
            if (ShouldExit)
                break;

            POINT p;
            GetCursorPos(&p);

            double xs = p.x;
            double ys = p.y;

            double xe = TargetX;
            double ye = TargetY;

            double gravity = 5.0f;
            double wind = 6.0f;

            double targetArea = 20.f;

            auto tDist = static_cast<int>(Distance(round(xs), round(ys), round(xe), round(ye)));

            auto dist = Hypot(xs - xe, ys - ye);
            wind = min(wind, dist);

            if (dist < 1)
                dist = 1;

            auto d = (round(round(static_cast<double>(tDist)) * 0.3) / 7);

            if (d > 25)
                d = 25;

            if (d < 5)
                d = 5;

            double rCnc = Random(6);

            if (rCnc == 1)
                d = 2;

            double maxStep;

            if (d <= round(dist))
                maxStep = d;
            else
                maxStep = round(dist);

            if (dist >= targetArea)
            {
                windX = windX / sqrt3 + (Random((int)(round(wind) * 2 + 1)) - wind) / sqrt5;
                windY = windY / sqrt3 + (Random((int)(round(wind) * 2 + 1)) - wind) / sqrt5;
            }
            else
            {
                windX = windX / sqrt2;
                windY = windY / sqrt2;
            }

            veloX = veloX + windX;
            veloY = veloY + windY;
            veloX = veloX + gravity * (xe - xs) / dist;
            veloY = veloY + gravity * (ye - ys) / dist;

            if (Hypot(veloX, veloY) > maxStep)
            {
                auto randomDist = maxStep / 2.0 + Random((int)(round(maxStep) / 2));
                auto veloMag = sqrt(veloX * veloX + veloY * veloY);
                veloX = (veloX / veloMag) * randomDist;
                veloY = (veloY / veloMag) * randomDist;
            }

            auto lastX = (int)round(xs);
            auto lastY = (int)round(ys);
            xs = xs + veloX;
            ys = ys + veloY;

            // if (lastX != round(xs) || (lastY != round(ys)))
            //    SetCursorPosition(new PointF((float)Math.Round(xs), (float)Math.Round(ys)));

            //if (lastX != round(xs) || (lastY != round(ys)))
            //    SetCursorPos(round(xs), round(ys));

            if (lastX != round(xs) || (lastY != round(ys)))
                MoveSystem(round(veloX), round(veloY));

            //Sleep(1);

            uint64_t currentTime = __rdtsc();
            while (currentTime + 10000000 > __rdtsc())
            {
                
            }
        }
    }
public:
    void SetTarget(int32_t x, int32_t y)
    {
        TargetX = x;
        TargetY = y;
    }

    void Start()
    {
        ShouldExit = false;
        MainThread = std::thread([this] { this->ThreadEntry(); });
    }

    void Stop()
    {
        ShouldExit = true;
        if (MainThread.joinable())
            MainThread.join();
    }
};
