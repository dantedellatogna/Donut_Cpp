#include <iostream>
#include <cmath>
#include <stdio.h>
#include <unistd.h>

void gotoxy(int x, int y);

int main()
{
    double theta{};
    double phi{};

    double R{30};
    double r{15};

    double x{}, y{}, z{};

    while (true)
    {
        for (theta = 0; theta < 180; theta++)
        {
            for (phi = 0; phi < 180; phi++)
            {
                x = cos(theta) * (R + r * cos(phi));
                y = sin(theta) * (R + r * cos(phi));
                z = r * sin(phi);

                gotoxy((x + 100) / 2, (y + 50) / 4);

                std::cout << "*";

                // usleep(300);
            }
        }
        // std::cout << "\033[2J";
    }

    gotoxy(0, 300);
    return 0;
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}