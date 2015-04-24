/*
 * randgen.c
 *
 *  Created on: Mar 26, 2015
 *      Author: oceaquaris
 */


#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


void randgen_init(void)
{
    srand(time(NULL) + getpid());
    return;
}


#define M_1_SQRT2PI   0.398942280401433  /* 1/sqrt(2pi) with 15 decimal places*/
/**
 * @brief Calculates standard normal distribution y values given x.
 *
 * @param x
 *        X value for standard bell curve.
 *
 * @return Corresponding Y value for X.
 */
double stdnorm_distribution(double x)
{
    return (M_1_SQRT2PI * exp((-1.0/2.0) * x * x)); /* 1/sqrt(2pi) * e^(-x^2 / 2) */
}


/**
 * @brief Calculates normal distribution y values given x, the mean, and
 *        standard deviation.
 *
 * @param x
 *        X value for standard bell curve.
 * @param mean
 *        The mean value of the bell curve.
 * @param stdev
 *        Standard deviation of the graph.
 *
 * @return Corresponding Y value for X.
 */
double norm_distribution(double x, double mean, double stdev)
{
    return (M_1_SQRT2PI * (1.0/stdev) * exp((-1.0 * (x - mean) * (x - mean)) /
            (2.0 * stdev * stdev)));
}


/**
 * @note 'max' is non-inclusive.
 */
int randint(int min, int max)
{
    return rand() % (max-min) + min;
}
