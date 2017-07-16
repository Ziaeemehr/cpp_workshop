#ifndef CONFIG_H
#define CONFIG_H

constexpr double    dt = 1.e-2;
constexpr int     PARN = 50;
constexpr double PARka = 2.0;
constexpr double PARka_over_N = PARka/(double)PARN;
constexpr double PARw = M_PI;
constexpr auto num_steps = 4000;


#endif