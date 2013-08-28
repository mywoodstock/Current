// TODO(dkorolev): Readme, build instructions and a reference from this file.
//
// Please see fncas.h or Makefile for build instructions.

#include <cassert>
#include <cstdio>
#include <limits>
#include <string>
#include <vector>

#include "fncas.h"

template<typename T> typename fncas::output<T>::type f(const T& x) {
  return 1 + x[0] * x[1] + x[2] - x[3] + 1;
}

int main() {
  std::vector<double> x({5,8,9,7});
  printf("%lf\n", f(x));
  auto e = f(fncas::x(4));
  printf("%s\n", e.debug_as_string().c_str());
  printf("%lf\n", e.eval(x));
}
