// RANDOM NUMBER GENERATOR
#include <time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_gamma.h>

#define RANDOM gsl_rng_uniform(gsl_rng_r)
#define RANDOM_INT(A) gsl_rng_uniform_int(gsl_rng_r, A)
#define RANDOM_GAUSS(S) gsl_ran_gaussian(gsl_rng_r, S)
#define RANDOM_POISSON(M) gsl_ran_poisson(gsl_rng_r, M)
// seed of the clock
// #define INITIALIZE_RANDOM(seed) {gsl_rng_env_setup(); if(!getenv("GSL_RNG_SEED")) gsl_rng_default_seed = time(0)+seed; gsl_rng_T=gsl_rng_default;  gsl_rng_r=gsl_rng_alloc(gsl_rng_T);}
// fixed seed
#define INITIALIZE_RANDOM_F(seed) {gsl_rng_env_setup(); if(!getenv("GSL_RNG_SEED")) gsl_rng_default_seed = seed; gsl_rng_T=gsl_rng_default;  gsl_rng_r=gsl_rng_alloc(gsl_rng_T);}
// #define INITIALIZE_RANDOM {gsl_rng_env_setup(); gsl_rng_T=gsl_rng_default;  gsl_rng_r=gsl_rng_alloc(gsl_rng_T);}
#define FREE_RANDOM gsl_rng_free(gsl_rng_r)


static const gsl_rng_type * gsl_rng_T;
static gsl_rng * gsl_rng_r;

