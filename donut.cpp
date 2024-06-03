#include <iostream>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include <vector>

#include <cstdlib>

void clear_screen();

void gotoxy(int x, int y);
void light_system(std::vector<double> a, std::vector<double> light, double light_angle);
void light_system2(std::vector<double> a, std::vector<double> light, double light_angle);

int main()
{
    double theta{};
    double phi{};

    double R{30};
    double r{15};

    double x{}, y{}, z{};
    std::vector<double> a{0, 0, 0};  // 3D Object
    std::vector<double> c{0, 0, 0};  // Camera
    std::vector<double> e{0, 0, 65}; // Plane
    std::vector<double> d(3);        // Camera transformation
    std::vector<double> b(2);        // Projection on 2D plane

    double light_angle{45};
    std::vector<double> light{0, -1, 1};
    // std::vector<double> light{1, 0, 1};

    std::vector<double> y_rotation_transform{0, 0, 0};

    std::vector<double> rotation_angle{0, 0, 0}; // Rotation angle

    while (true)
    {
        for (theta = 0; theta < 180; theta++)
        {
            for (phi = 0; phi < 180; phi++)
            {
                x = cos(theta) * (R + r * cos(phi));
                y = sin(theta) * (R + r * cos(phi));
                z = r * sin(phi);

                // rotation x
                y_rotation_transform[0] = x * cos(rotation_angle[1]) - z * sin(rotation_angle[1]);
                y_rotation_transform[2] = x * sin(rotation_angle[1]) + z * cos(rotation_angle[1]);

                // projection
                a[0] = y_rotation_transform[0];
                a[1] = y;
                a[2] = y_rotation_transform[2] + 70;
                /*
                a[0] = x;
                a[1] = y;
                a[2] = z + 30;
                */

                d[0] = a[0] - c[0];
                d[1] = a[1] - c[1];
                d[2] = a[2] - c[2];

                b[0] = (e[2] / d[2]) * d[0] - e[0];
                b[1] = (e[2] / d[2]) * d[1] - e[1];

                // std::cout<<a[0]<<std::endl;

                // gotoxy((x + 100) / 2, (y + 50) / 4);
                gotoxy((b[0] + 100) / 2, (b[1] + 70) / 4);

                // Light System
                //light_system(a, light, light_angle);
                light_system2(a, light, light_angle);
                // usleep(300);
            }
        }
        // std::cout << "\033[2J";
        // light_angle += 0.5;
        // light[0] = cos(light_angle);
        // light[1] = -sin(light_angle);
        // light[2] = sin(light_angle);

        if (rotation_angle[1] == 360)
            rotation_angle[1] = 0;
        rotation_angle[1] += 0.1;
        clear_screen();
    }

    gotoxy(0, 300);
    return 0;
}

void clear_screen()
{
#ifdef WINDOWS
    std::system("cls");
#else
    // Assume POSIX
    std::system("clear");
#endif
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

void light_system(std::vector<double> a, std::vector<double> light, double light_angle)
{

    double dot_al{}; // Dot product of 3D Object and Light vectors
    double a_mag{};  // 3D Object vector magnitude
    double l_mag{};  // Ligth vector magnitude

    double angle_between{};

    dot_al = a[0] * light[0] + a[1] * light[1] + a[2] * light[2];
    a_mag = sqrt((pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2)));
    l_mag = sqrt((pow(light[0], 2) + pow(light[1], 2) + pow(light[2], 2)));

    angle_between = acos(dot_al / (a_mag * l_mag)); // returns radians. Need to transform it to degrees.
    angle_between = angle_between * 180 / M_PI;
    // std::cout <<angle_between<<std::endl;

    if (angle_between >= 70)
        std::cout << ".";
    else if (angle_between >= 50)
        std::cout << ",";
    else if (angle_between >= 30)
        std::cout << "~";
    else if (angle_between >= 10)
        std::cout << "#";
    else if (angle_between < 10)
        std::cout << "@";
}

void light_system2(std::vector<double> a, std::vector<double> light, double light_angle)
{

    double dot_al{}; // Dot product of 3D Object and Light vectors
    dot_al = a[0] * light[0] + a[1] * light[1] + a[2] * light[2];

    if (dot_al >= 100)
        std::cout << "$";
    else if (dot_al < 100)
        std::cout << ".";
    
    //std::cout << dot_al << std::endl;
    //usleep(3000);
}