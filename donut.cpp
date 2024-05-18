#include <iostream>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include <vector>

void gotoxy(int x, int y);

int main()
{
    double theta{};
    double phi{};

    double R{30};
    double r{10};

    double x{}, y{}, z{};
    std::vector<double> a{0, 0, 0}; // 3D Object
    std::vector<double> c{0, 0, 0}; // Camera
    std::vector<double> e{0, 0, 20}; // Plane
    std::vector<double> d(3); // Camera transformation
    std::vector<double> b(2); // Projection on 2D plane

    double light_angle{45};
    std::vector<double> light{cos(light_angle), -sin(light_angle), cos(light_angle)};
    //std::vector<double> light{1, 0, 1};

    double dot_al{};
    double a_mag{};
    double l_mag{};

    double angle_between{};

    while (true)
    {
        for (theta = 0; theta < 180; theta++)
        {
            for (phi = 0; phi < 180; phi++)
            {
                x = cos(theta) * (R + r * cos(phi));
                y = sin(theta) * (R + r * cos(phi));
                z = r * sin(phi);

                a[0] = x;
                a[1] = y;
                a[2] = z + 30;

                d[0] = a[0] - c[0];
                d[1] = a[1] - c[1];
                d[2] = a[2] - c[2];

                b[0] = (e[2]/d[2])*d[0] - e[0];
                b[1] = (e[2]/d[2])*d[1] - e[1];


                // std::cout<<a[0]<<std::endl;

                //gotoxy((x + 100) / 2, (y + 50) / 4);
                gotoxy((b[0] + 100) / 2, (b[1] + 50) / 4);

                dot_al = a[0]*light[0] + a[1]*light[1] + a[2]*light[2];
                a_mag = sqrt((pow(a[0],2) + pow(a[1], 2) + pow(a[2], 2)));
                l_mag = sqrt((pow(light[0], 2) + pow(light[1], 2) + pow(light[2], 2)));

                angle_between = acos(dot_al/(a_mag*l_mag)); // returns radians. Need to transform it to degrees.
                angle_between = angle_between * 180 / M_PI;
                //std::cout <<angle_between<<std::endl;

                if (angle_between >= 90)
                    std::cout << ".";
                else if (angle_between >= 70)
                    std::cout << ",";
                else if (angle_between >= 50)
                    std::cout << ";";
                else if (angle_between >= 30)
                    std::cout << "#";
                else if (angle_between < 30)
                    std::cout << "@";
                
                // usleep(300);
            }
        }
        // std::cout << "\033[2J";
        light_angle += 0.1;
        light[0] = cos(light_angle);
        light[1] = -sin(light_angle);
        light[2] += cos(light_angle)*0;
    }

    gotoxy(0, 300);
    return 0;
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}