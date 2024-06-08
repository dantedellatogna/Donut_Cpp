#include <iostream>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include <vector>

#include <cstdlib>

double degrees_2_radians(float angle);
void clear_screen();
void gotoxy(int x, int y);
void light_system(std::vector<float> normal, std::vector<float> light);
std::vector<float> normal_surface(float costheta, float sintheta, float cosphi, float sinphi);

int main()
{
    float theta{}; // Big circle
    float costheta{};
    float sintheta{};

    float phi{}; // small circle
    float cosphi{};
    float sinphi{};

    float R{30};
    float r{15};

    float x{}, y{}, z{};
    std::vector<float> a{0, 0, 0};  // 3D Object
    std::vector<float> c{0, 0, 0};  // Camera
    std::vector<float> e{0, 0, 40}; // Plane
    std::vector<float> d(3);        // Camera transformation
    std::vector<float> b(2);        // Projection on 2D plane

    std::vector<float> normal(3); // Normal vector for each point in the surface

    std::vector<float> light{1, -1, 1};
    // std::vector<double> light{1, 0, 1};

    std::vector<float> origin{0, 0, 60};
    std::vector<float> x_rotation_transform(3);
    std::vector<float> y_rotation_transform(3);
    std::vector<float> z_rotation_transform(3);

    float rotation_angle{0}; // Rotation angle
    float cosrotation{};
    float sinrotation{};

    float tau = 2 * M_PI;

    while (true)
    {
        clear_screen();
        for (theta = 0; theta < tau; theta += 0.1)
        {
            costheta = cos(theta);
            sintheta = sin(theta);

            for (phi = 0; phi < tau; phi += 0.1)
            {
                cosphi = cos(phi);
                sinphi = sin(phi);

                x = costheta * (R + r * cosphi);
                y = sintheta * (R + r * cosphi);
                z = r * sinphi;

                cosrotation = cos(rotation_angle);
                sinrotation = sin(rotation_angle);

                // test
                y_rotation_transform[0] = x * cosrotation - z * sinrotation;
                y_rotation_transform[2] = x * sinrotation + z * cosrotation;

                a[0] = y_rotation_transform[0];
                a[1] = y;
                a[2] = y_rotation_transform[2] + origin[2];

                /*
                // rotation x_axis
                x_rotation_transform[0] = x;
                x_rotation_transform[1] = y * cosrotation - z * sinrotation;
                x_rotation_transform[2] = z * cosrotation + y * sinrotation;

                // rotation y-axis
                y_rotation_transform[0] = x_rotation_transform[0] * cosrotation - x_rotation_transform[2] * sinrotation;
                y_rotation_transform[1] = x_rotation_transform[1];
                y_rotation_transform[2] = x_rotation_transform[2] * cosrotation + x_rotation_transform[0] * sinrotation;

                // rotation z-axis
                z_rotation_transform[0] = y_rotation_transform[0] * cosrotation - y_rotation_transform[1] * sinrotation;
                z_rotation_transform[1] = y_rotation_transform[1] * cosrotation + y_rotation_transform[0] * sinrotation;
                z_rotation_transform[2] = y_rotation_transform[2];

                // projection
                a[0] = z_rotation_transform[0] + origin[0];
                a[1] = z_rotation_transform[1] + origin[1];
                a[2] = z_rotation_transform[2] + origin[2];
                                */
                // Calculate normal vector
                normal = normal_surface(costheta, sintheta, cosphi, sinphi);

                d[0] = a[0] - c[0];
                d[1] = a[1] - c[1];
                d[2] = a[2] - c[2];

                b[0] = (e[2] / d[2]) * d[0] - e[0];
                b[1] = (e[2] / d[2]) * d[1] - e[1];

                gotoxy((b[0] + 100), (b[1] + 50) / 2);

                // Light System
                light_system(normal, light);
                // usleep(500000);

                // gotoxy(0, 8);
                // std::cout << "Normal X: " << normal[0] << " Normal Y: " << normal[1] << " Normal Z: " << normal[2] << std::endl;
                // std::cout << "Light X: " << light[0] << " Light Y: " << light[1] << " Light Z: " << light[2];
            }
        }

        if (rotation_angle >= tau)
            rotation_angle -= tau;
        rotation_angle += 0.05;
        gotoxy(0, 10);
        std::cout << "Rotation angle: " << rotation_angle << std::endl;
        usleep(50000);
        // clear_screen();
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

void light_system(std::vector<float> normal, std::vector<float> light)
{
    float dot_product{}; // Dot product of Normal surface and Light vectors
    dot_product = normal[0] * light[0] + normal[1] * light[1] + normal[2] * light[2];

    if (dot_product <= 0) //|| dot_product > 0)
        std::cout << ".";
    else if (dot_product < 0.2)
        std::cout << "-";
    else if (dot_product < 0.5)
        std::cout << "~";
    else if (dot_product < 0.7)
        std::cout << ":";
    else if (dot_product < 0.9)
        std::cout << ";";
    else if (dot_product < 1)
        std::cout << "$";
    else if (dot_product < 1.2)
        std::cout << "#";
    else if (dot_product >= 1.2)
        std::cout << "@";
    // std::cout << dot_product << std::endl;
    // usleep(3000);
}

std::vector<float> normal_surface(float costheta, float sintheta, float cosphi, float sinphi)
{
    // theta -> Big circle; phi -> Small circle
    std::vector<float> tangent_R(3); // Tangent vector with respect to big circle
    std::vector<float> tangent_r(3); // Tangent vector with respectr to little circle
    std::vector<float> normal(3);

    float length{};

    tangent_R[0] = (-1) * sintheta;
    tangent_R[1] = costheta;
    tangent_R[2] = 0;

    tangent_r[0] = costheta * (-1) * sinphi;
    tangent_r[1] = sintheta * (-1) * sinphi;
    tangent_r[2] = cosphi;

    // Normal vector is the the cross-product of the tangent vectors
    normal[0] = tangent_R[1] * tangent_r[2] - tangent_R[2] * tangent_r[1];
    normal[1] = tangent_R[2] * tangent_r[0] - tangent_R[0] * tangent_r[2];
    normal[2] = tangent_R[0] * tangent_r[1] - tangent_R[1] * tangent_r[0];

    // Normalize normal vector
    length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    // gotoxy(0, 0);
    normal[0] /= length;
    normal[1] /= length;
    normal[2] /= length;
    // std::cout << normal[0] << std::endl;
    // std::cout << normal[1] << std::endl;
    // std::cout << normal[2] << std::endl;

    return normal;
}