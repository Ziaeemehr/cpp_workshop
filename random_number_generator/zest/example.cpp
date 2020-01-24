#include <random>
#include <iostream>

// include Zest and import it into the global namespace
#include "include/zest.hpp"
using namespace zest;

int main () {
  std::mt19937 urbg;
  
  Ziggurat<StandardNormal, std::mt19937> ziggurat_for_std_normal; // constructs the Ziggurat
  std::cout << "normal variate: " << ziggurat_for_std_normal (urbg) << std::endl;
  
  Ziggurat<StandardExponential, std::mt19937> ziggurat_for_std_exponential;
  std::cout << "exponential variate: " << ziggurat_for_std_exponential (urbg) << std::endl;
  
  Ziggurat<StandardCauchy, std::mt19937> ziggurat_for_std_cauchy;
  std::cout << "Cauchy variate: " << ziggurat_for_std_cauchy (urbg) << std::endl;

  Ziggurat<ChiSquared</*dof=*/3>, std::mt19937>  ziggurat_for_chi_sq_w_3_dof;
  std::cout << "Chi-squared variate: " << ziggurat_for_chi_sq_w_3_dof (urbg) << std::endl;

  // Other distributions need extra parameters
  // so a distribution object must be constructed and passed to the Ziggurat constructor:
  Weibull weibull_dist {/*shape =*/ 2.5, /*scale =*/ 3};      // default is shape = 1.0, scale = 1.0
  Ziggurat<Weibull, std::mt19937> ziggurat_for_weibull {weibull_dist};
  std::cout << "Weibull variate: " << ziggurat_for_weibull (urbg) << std::endl;
  
  Gamma gamma_dist {/*shape =*/ 2.5, /*scale =*/ 3};          // default is shape = 1.0, scale = 1.0
  Ziggurat<Gamma, std::mt19937> ziggurat_for_gamma {gamma_dist};
  std::cout << "Gamma variate: " << ziggurat_for_gamma (urbg) << std::endl;
  
  // default is normal_mean = 0.0, normal_stddev = 1.0
  LogNormal log_normal_dist {/*normal_mean =*/ 2.5, /*normal_stddev =*/ 3};
  Ziggurat<LogNormal, std::mt19937> ziggurat_for_log_normal {log_normal_dist};
  std::cout << "Log-normal variate: " << ziggurat_for_log_normal (urbg) << std::endl;
  
  StudentT student_t_dist {/*dof =*/ 5.0};
  Ziggurat<StudentT, std::mt19937> ziggurat_for_student_t {student_t_dist};
  std::cout << "Student's t variate: " << ziggurat_for_student_t (urbg) << std::endl;
  
  FisherF fisher_f_dist {/*dof1 =*/ 2.0, /*dof2 =*/ 7.0};     // default is dof1 = 1.0, dof2 = 1.0
  Ziggurat<FisherF, std::mt19937> ziggurat_for_fisher_f {fisher_f_dist};
  std::cout << "Fisher's f variate: " << ziggurat_for_fisher_f (urbg) << std::endl;
  
  // one can also construct Ziggurat for normal, Cauchy, and exponential distributions with non-standard parameters
  Normal normal_dist {/*mean =*/ -2, /*stddev =*/ 1.5};
  Ziggurat<Normal, std::mt19937> ziggurat_for_normal {normal_dist};
  std::cout << "Normal variate: " << ziggurat_for_normal (urbg) << std::endl;
  
  Cauchy cauchy_dist {/*mode =*/ 2.5, /*scale =*/ 3};
  Ziggurat<Cauchy, std::mt19937> ziggurat_for_cauchy {cauchy_dist};
  std::cout << "Cauchy variate: " << ziggurat_for_cauchy (urbg) << std::endl;
  
  Exponential exponential_dist {/*rate =*/ 0.5, /*mode =*/ 1};  // -1
  Ziggurat<Exponential, std::mt19937> ziggurat_for_exponential {exponential_dist};
  std::cout << "Exponential variate: " << ziggurat_for_exponential (urbg) << std::endl;
  
  return 0;
}
