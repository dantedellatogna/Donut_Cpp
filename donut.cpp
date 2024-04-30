#include <iostream>
#include <cmath>
#include <stdio.h>
#include <unistd.h>


void gotoxy(int x, int y);

int main()
{
    // double theta{};
    // double phi{};

    double R{20};
    double r{5};

    double x{}, y{}, z{};

    for (int i{0}; i <= 180; i++)
    {
        for (int j{0}; j <= 180; j++)
        {
            x = cos(i) * (R + r * cos(j));
            y = sin(i) * (R + r * cos(j));
            z = r * sin(j);

            gotoxy((x + 50)/2, (y + 50) / 4);
            std::cout << "#" << std::endl;
        }

    }

    gotoxy(0, 100);
    return 0;
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}