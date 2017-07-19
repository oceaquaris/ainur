/*
 * Utilizes the xorshift128+ algorithm.
 * Field Overview:
 *  static:
 *      s_128[2]
 *      rnd_128_rotl
 *      rnd_128_init_timeEntropy
 *  extern:
 *      rnd_128_jump
 *      rnd_128_next
 *      rnd_init
 */

#include <fcntl.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "rnd.h"
#include "utils.h"



static uint64_t s_128[2] = {0,0};



static inline uint64_t rnd_128_rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}



/**
 * @brief Use time as a source of entropy.
 */
static void rnd_128_init_timeEntropy(void) {
    //our struct to contain the time of day
    struct timeval tv;

    gettimeofday(&tv, NULL);

    s_128[0] = ( reverse(tv.tv_sec) ^ tv.tv_sec );
    s_128[1] = ( reverse(tv.tv_usec) ^ tv.tv_usec );
    return;
}



/**
 * @fn void rnd_128_jump (void)
 *
 * @brief This is the jump function for the generator. It is equivalent
 *        to 2^64 calls to next(); it can be used to generate 2^64
 *        non-overlapping subsequences for parallel computations.
 */
void rnd_128_jump(void) {
    static const uint64_t JUMP[] = { 0xbeac0467eba5facb, 0xd86b048b86aa9922 };
    uint64_t s0 = 0, s1 = 0;

    for(int i = 0; i < sizeof JUMP / sizeof *JUMP; i++) {
        for(int b = 0; b < 64; b++) {
            if (JUMP[i] & UINT64_C(1) << b) {
                s0 ^= s_128[0];
                s1 ^= s_128[1];
            }
            rnd_128_next();
        }
    }

    s_128[0] = s0;
    s_128[1] = s1;
}



/**
 * @fn uint64_t rnd_128_next (void)
 *
 * @brief Retrieve the next random number.
 *
 * @return The next random number.
 */
uint64_t rnd_128_next(void) {
    const uint64_t s0 = s_128[0];
    uint64_t s1 = s_128[1];
    const uint64_t result = s0 + s1;

    s1 ^= s0;
    s_128[0] = rnd_128_rotl(s0, 55) ^ s1 ^ (s1 << 14); // a, b
    s_128[1] = rnd_128_rotl(s1, 36); // c

    return result;
}



/**
 * @fn void rnd_init (void)
 *
 * @brief Initialize the xorshift128+ random number generator using
 *        \/dev\/urandom. Utilize rnd_128_init_timeEntropy in case of
 *        failure to open the urandom directory.
 */
void rnd_init(void) {
    int filedesc = open("/dev/urandom", O_RDONLY);

    if(filedesc != -1) {
        size_t seed_size = sizeof(uint64_t)*2;
        if(read(filedesc, s_128, seed_size) != ((ssize_t)seed_size)) {
            rnd_128_init_timeEntropy();
            return;
        }
    }
    else {
        rnd_128_init_timeEntropy();
    }
    return;
}



/**
 * @fn double rnd_normal( double x )
 *
 * @brief Calculates the Normal distribution.
 *
 *        Calculates N(x) where N is the normal distribution.
 *
 *        Approximates to a power series:
 *          N(x) =  1 - n(x)*(b1*t + b2*t^2 + b3*t^3 + b4*t^4 + b5*t^5) + Err
 *          where t = 1 / (1 + 0.2316419*x)
 *
 *        Maximum absolute error is 7.5e^-8.
 *
 * @param x Value to calculate the normal of.
 * @return The value of the Normal.
 */
double rnd_normal( double x )
{
    double t,
           series;
    const double b1 =  0.319381530,
                 b2 = -0.356563782,gkfdjskgfijgkgjpjgdfgdsf
                 b3 =  1.781477937,
                 b4 = -1.821255978,
                 b5 =  1.330274429,
                 p  =  0.2316419,
                 c  =  0.39894228;

    t = 1. / ( 1. + p * FABS(x) );
    series = (1. - c * exp( -x * x / 2. ) * t *
            ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
    return (x > 0.) ? 1. - series : series;
}
