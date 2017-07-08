/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin min_non_smo.cpp$$
$spell
	smo
$$

$section abs_normal min_non_smo: Example and Test$$

$head Purpose$$
We minimize the function
$latex f : \B{R}^3 \rightarrow \B{R}$$ defined by
$latex \[
\begin{array}{rcl}
f( x_0, x_1, x_2  ) & = & x_0^2 + 2 (x_0 + x_1)^2 + | x_2 |
\end{array}
\] $$

$head Discussion$$
This routine uses $cref abs_min_linear$$ which uses $cref lp_box$$,
a linear programming algorithm.
You can see that the convergence is linear for this algorithm by setting
$icode%level% = 1%$$.
A higher order of convergence would requite quadratic, instead of linear,
sub-problems.
The routine $cref qp_box$$,
which uses $cref qp_interior$$,
is a beginning prototypes for such an algorithm.


$head Source$$
$srcfile%example/abs_normal/min_non_smo.cpp%
	0%// BEGIN C++%// END C++%
1%$$

$end
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include "min_non_smo.hpp"

bool min_non_smo(void)
{	bool ok = true;
	//
	using CppAD::AD;
	using CppAD::ADFun;
	//
	typedef CPPAD_TESTVECTOR(size_t)       s_vector;
	typedef CPPAD_TESTVECTOR(double)       d_vector;
	typedef CPPAD_TESTVECTOR( AD<double> ) ad_vector;
	//
	size_t level = 0;    // level of tracing
	size_t n     = 3;    // size of x
	size_t m     = 1;    // size of y
	size_t s     = 1;    // number of data points and absolute values
	//
	// start recording the function f(x)
	ad_vector ax(n), ay(m);
	for(size_t j = 0; j < n; j++)
		ax[j] = double(j + 1);
	Independent( ax );
	//
	ay[0]  =  ax[0] * ax[0];
	ay[0] += 2.0 * (ax[0] + ax[1]) * (ax[0] + ax[1]);
	ay[0] += fabs( ax[2] );
	ADFun<double> f(ax, ay);
	//
	// create its abs_normal representation in g, a
	ADFun<double> g, a;
	f.abs_normal_fun(g, a);

	// check dimension of domain and range space for g
	ok &= g.Domain() == n + s;
	ok &= g.Range()  == m + s;

	// check dimension of domain and range space for a
	ok &= a.Domain() == n;
	ok &= a.Range()  == s;

	// epsilon
	d_vector epsilon(2);
	double eps = 1e-3;
	epsilon[0] = eps;
	epsilon[1] = eps;

	// maxitr
	s_vector maxitr(3);
	maxitr[0] = 100;
	maxitr[1] = 20;
	maxitr[2] = 20;

	// b_in
	double b_in = 1.0;

	// call min_non_smo
	d_vector x_in(n), x_out(n);
	for(size_t j = 0; j < n; j++)
		x_in[j]  = double(j + 1);

	//
	ok &= CppAD::min_non_smo(
		level, g, a, epsilon, maxitr, b_in, x_in, x_out
	);
	//
	for(size_t j = 0; j < n; j++)
		ok &= std::fabs( x_out[j] ) < eps;

	return ok;
}
// END C++
