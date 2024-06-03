#include <iostream>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include <vector>

#include <cstdlib>

void clear_screen();

void gotoxy(int x, int y);
void light_system(std::vector<double> normal, std::vector<double> light, double light_angle);
std::vector<double> normal_surface(std::vector<double> a, double theta, double phi);

int main()
{
    double theta{}; // Big circle
    double phi{};   // small circle

    double R{30};
    double r{15};

    double x{}, y{}, z{};
    std::vector<double> a{0, 0, 0};  // 3D Object
    std::vector<double> c{0, 0, 0};  // Camera
    std::vector<double> e{0, 0, 65}; // Plane
    std::vector<double> d(3);        // Camera transformation
    std::vector<double> b(2);        // Projection on 2D plane

    std::vector<double> normal(3); // Normal vector for each point in the surface

    double light_angle{45};
    std::vector<double> light{0, 1, 1};
    // std::vector<double> light{1, 0, 1};

    std::vector<double> x_rotation_transform{0, 0, 0};
    std::vector<double> y_rotation_transform{0, 0, 0};
    std::vector<double> z_rotation_transform{0, 0, 0};

    double rotation_angle; // Rotation angle

    while (true)
    {
        for (theta = 0; theta < 180; theta++)
        {
            for (phi = 0; phi < 180; phi++)
            {
                x = cos(theta) * (R + r * cos(phi));
                y = sin(theta) * (R + r * cos(phi));
                z = r * sin(phi);

                // rotation x_axis
                // x_rotation_transform[1] = y * cos(rotation_angle) - z * sin(rotation_angle);
                // x_rotation_transform[2] = y * sin(rotation_angle) + z * cos(rotation_angle);

                // rotation y-axis
                y_rotation_transform[0] = x * cos(rotation_angle) - z * sin(rotation_angle);
                y_rotation_transform[2] = x * sin(rotation_angle) + z * cos(rotation_angle);

                // rotation z-axis
                // z_rotation_transform[0] = x * cos(rotation_angle) - y * sin(rotation_angle);
                // z_rotation_transform[1] = x * sin(rotation_angle) + y * cos(rotation_angle);

                // projection
                a[0] = x_rotation_transform[0] + y_rotation_transform[0] + z_rotation_transform[0];
                a[1] = y;
                a[2] = x_rotation_transform[2] + y_rotation_transform[2] + z_rotation_transform[2] + 70;

                // Calculate normal vector
                normal = normal_surface(a, theta, phi);

                d[0] = a[0] - c[0];
                d[1] = a[1] - c[1];
                d[2] = a[2] - c[2];

                b[0] = (e[2] / d[2]) * d[0] - e[0];
                b[1] = (e[2] / d[2]) * d[1] - e[1];

                gotoxy((b[0] + 100) / 2, (b[1] + 70) / 4);

                // Light System
                light_system(normal, light, light_angle);
                // usleep(300);
            }
        }

        if (rotation_angle == 360)
            rotation_angle = 0;
        rotation_angle += 0.1;
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

void light_system(std::vector<double> normal, std::vector<double> light, double light_angle)
{

    double dot_product{}; // Dot product of Normal surface and Light vectors
    dot_product = normal[0] * light[0] + normal[1] * light[1] + normal[2] * light[2];

    if (dot_product < -1)
        std::cout << ".";
    else if (dot_product < 0)
        std::cout << ",";
    else if (dot_product < 0.2)
        std::cout << "-";
    else if (dot_product < 1)
        std::cout << ";";
    else if (dot_product < 1.2)
        std::cout << "#";
    else if (dot_product >= 1.2)
        std::cout << "@";
    // std::cout << dot_product << std::endl;
    // usleep(3000);
}

std::vector<double> normal_surface(std::vector<double> a, double theta, double phi)
{
    // theta -> Big circle; phi -> Small circle
    std::vector<double> tangent_R(3); // Tangent vector with respect to big circle
    std::vector<double> tangent_r(3); // Tangent vector with respectr to little circle
    std::vector<double> normal(3);

    double length{};

    tangent_R[0] = -sin(theta);
    tangent_R[1] = cos(theta);
    tangent_R[2] = 0;

    tangent_r[0] = cos(theta) * (-sin(phi));
    tangent_r[1] = sin(theta) * (-sin(phi));
    tangent_r[2] = cos(phi);

    // Normal vector is the the cross-product of the tangent vectors
    normal[0] = tangent_R[1] * tangent_r[2] - tangent_R[2] * tangent_r[1];
    normal[1] = tangent_R[0] * tangent_r[2] - tangent_R[2] * tangent_r[0];
    normal[2] = tangent_R[0] * tangent_r[1] - tangent_R[1] * tangent_r[0];

    // Normalize normal vector
    length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    normal[0] /= length;
    normal[1] /= length;
    normal[2] /= length;

    return normal;
}