/*!
  \file   oem.cc
  \author Simon Pfreundschuh <simonpf@chalmers.se>
  \date   Fri Apr 17 16:39:25 2015

  \brief Optimal inversion methods for linear and non-linear inverse problems.
*/

#include "arts.h"
#include <iostream>
#include "lin_alg.h"
#include "stdlib.h"
#include "math.h"
#include "oem.h"

using std::ostream;
using std::endl;
using std::setw;
using std::scientific;
using std::fixed;


//------------------------------------------------------------------------------------
//
//   Functions for displaying progress of inversions
//
//------------------------------------------------------------------------------------

void separator( ostream& stream,
                Index length )
{
    for (Index i = 0; i < length; i++)
        stream << "-";
    stream << endl;
}

//! Initial log message, linear
/*!

  \param[in] stream Stream to print message to.
*/
void log_init_li( ostream& stream )
{
    stream << endl;
    stream << "Starting linear OEM inversion:" << endl << endl;
    separator( stream, 52 );
    stream << setw(6) << "Step";
    stream << setw(15) << "     Chi^2     ";
    stream << setw(15) << "     Chi^2_x   ";
    stream << setw(15) << "     Chi^2_y   ";
    stream << endl;
    separator( stream, 52 );
}

//! Step log message, linear
/*!

  \param[in] stream Stream to print message to.
  \param[in] step_number Current step number.
  \param[in] cost Current value of cost function.
  \param[in] cost_x x-component of current cost function value.
  \param[in] cost_y y-component of current cost function value.
*/
void log_step_li( ostream& stream,
                  Index step_number,
                  Numeric cost,
                  Numeric cost_x,
                  Numeric cost_y )
{
    stream << setw(5) << step_number << " ";
    stream << scientific;
    stream << setw(15) << cost;
    stream << setw(15) << cost_x;
    stream << setw(15) << cost_y;
    stream << endl;
}

//! Final log message, linear
/*!

  \param[in] stream Stream to print message to.
  \param[in] converged converged flag.
  \param[in] cost Value of cost function.
  \param[in] cost_x x-component of cost function.
  \param[in] cost_y y-component of cost function.
  \param[in] iter Final step number.
  \param[in] maxIter  Maximum step number.
*/
void log_finalize_li( ostream& stream )
{
    separator( stream, 52 );
    stream << endl;
}

//! Initial log message, Gauss-Newton
/*!

  \param[in] stream Stream to print message to.
  \param[in] tol Tolerance.
  \param[in] maxIter Maximum Iterations.
*/
void log_init_gn( ostream& stream,
                  Numeric tol,
                  Index maxIter )
{
    stream << endl;
    stream << "Starting OEM inversion: " << endl << endl;
    stream << "\tMethod: Gauss-Newton" << endl;
    stream << "\tStop criterion: " << tol << endl;
    stream << "\tMax. iterations: " << maxIter << endl << endl;
    separator( stream, 67 );
    stream << setw(6) << "Step";
    stream << setw(15) << "     Chi^2     ";
    stream << setw(15) << "     Chi^2_x   ";
    stream << setw(15) << "     Chi^2_y   ";
    stream << setw(15) << "     d_i^2     ";
    stream << endl;
    separator( stream, 67 );
}

//! Step log message, Gauss-Newton
/*!

  \param[in] stream Stream to print message to.
  \param[in] step_number Current step number.
  \param[in] cost Current value of cost function.
  \param[in] cost_x x-component of current cost function value.
  \param[in] cost_y y-component of current cost function value.
  \param[in] di2 Convergence criterion.
*/
void log_step_gn( ostream& stream,
                  Index step_number,
                  Numeric cost,
                  Numeric cost_x,
                  Numeric cost_y,
                  Numeric di2 )
{
    stream << setw(5) << step_number << " ";
    stream << scientific;
    stream << setw(15) << cost;
    stream << setw(15) << cost_x;
    stream << setw(15) << cost_y;
    stream << setw(15) << di2;
    stream << endl;
}

//! Final log message, Gauss-Newton
/*!

  \param[in] stream Stream to print message to.
  \param[in] converged converged flag.
  \param[in] cost Value of cost function.
  \param[in] cost_x x-component of cost function.
  \param[in] cost_y y-component of cost function.
  \param[in] iter Final step number.
  \param[in] maxIter  Maximum step number.
*/
void log_finalize_gn( ostream& stream,
                      bool converged,
                      Index iter,
                      Index maxIter )
{
    separator( stream, 67 );
    stream << endl;
    if ( converged )
        stream << "\tConverged: YES";
    else if ( iter == maxIter )
    {
        stream << "\tConverged: NO" << endl;
        stream << "\tMaximum no. of iterations was reached.";
    }
    stream << endl << endl;
}

//! Initial log message, Levenberg-Marquardt
/*!

  \param[in] stream Stream to print message to.
  \param[in] tol Tolerance.
  \param[in] maxIter Maximum Iterations.
*/
void log_init_lm( ostream& stream,
                  Numeric tol,
                  Index maxIter )
{
    stream << endl;
    stream << "Starting OEM inversion: " << endl  << endl;
    stream << "     Method: Levenberg-Marquardt " << endl;
    stream << "     Stop criterion: " << tol << endl;
    stream << "     Max. iterations: " << maxIter << endl << endl;
    separator( stream, 75 );
    stream << setw(6) << "Step";
    stream << setw(15) << "     Chi^2      ";
    stream << setw(15) << "     Chi^2_x    ";
    stream << setw(15) << "     Chi^2_y    ";
    stream << setw(9) << "  gamma  ";
    stream << setw(15) << "     d_i^2     ";
    stream << endl;
    separator( stream, 75 );
}

void log_gamma_step_lm( ostream& stream,
                        Numeric cost,
                        Numeric cost_x,
                        Numeric cost_y,
                        Numeric gamma )
{
    stream << setw(6) << "";
    stream << scientific;
    stream << setw(15) << cost;
    stream << setw(15) << cost_x;
    stream << setw(15) << cost_y;
    stream.unsetf(ios_base::floatfield);
    stream << setw(9) << gamma;
    stream << endl;
}

//! Step log message, Levenberg-Marquardt
/*!

  \param[in] stream Stream to print message to.
  \param[in] step_number Current step number.
  \param[in] cost Current value of cost function.
  \param[in] cost_x x-component of current cost function value.
  \param[in] cost_y y-component of current cost function value.
  \param[in] gamma Current gamma value.
  \param[in] di2 Convergence criterion.
*/
void log_step_lm( ostream& stream,
                  Index step_number,
                  Numeric cost,
                  Numeric cost_x,
                  Numeric cost_y,
                  Numeric gamma,
                  Numeric di2 )
{
    stream << setw(6) << step_number;
    stream << scientific;
    stream << setw(15) << cost;
    stream << setw(15) << cost_x;
    stream << setw(15) << cost_y;
    stream.unsetf(ios_base::floatfield);
    stream << setw(9) << gamma;
    stream << scientific;
    stream << setw(15) << di2;
    stream << endl;
}

//! Final log message, Levenberg-Marquardt
/*!

  \param[in] stream Stream to print message to.
  \param[in] converged converged flag.
  \param[in] cost Value of cost function.
  \param[in] cost_x x-component of cost function.
  \param[in] cost_y y-component of cost function.
  \param[in] gamma Final gamma value.
  \param[in] gamma_max Maximum gamma value.
  \param[in] iter Final step number.
  \param[in] maxIter  Maximum step number.
*/
void log_finalize_lm( ostream& stream,
                      bool converged,
                      Numeric cost,
                      Numeric cost_x,
                      Numeric cost_y,
                      Numeric gamma,
                      Numeric gamma_max,
                      Index iter,
                      Index maxIter )
{
    separator( stream, 75 );
    stream << endl << "Finished Levenberg-Marquardt iterations:" << endl;
    if ( converged )
        stream << "\t Converged: YES" << endl;
    else if ( gamma == gamma_max )
    {
        stream << "\t Converged: NO" << endl;
        stream << "\t Iteration aborted because gamma > gamma_max.";
    }
    else if ( iter == maxIter )
    {
        stream << "\t Converged: NO" << endl;
        stream << "\t Iteration aborted because maximum no. of iterations was reached.";
    }
    stream << endl;

    stream << "\t Chi^2: " << cost << endl;
    stream << "\t Chi^2_x: " << cost_x << endl;
    stream << "\t Chi^2_y: " << cost_y << endl;
    stream << endl << endl << endl;
}



//------------------------------------------------------------------------------------
//
//   Calculation of the cost function
//
//------------------------------------------------------------------------------------

//! Calculation of y-part of cost function
/*!
  This version is suitable if no term is already at hand.

  \param[out] cost_y    Const function value
  \param[in]  y         Measurement vector.
  \param[in]  yf        Fitted measurement.
  \param[in]  SeInv     Inverse of relevenaty covariance matrix
  \param[in]  normfac   Normalisation factor. The cost is scaled with 1/normfac

  \author Patrick Eriksson 
  \date   2015-10-05
*/
void oem_cost_y( Numeric& cost_y, 
                 ConstVectorView y, 
                 ConstVectorView yf,
                 ConstMatrixView SeInv,
                 const Numeric&  normfac )
{
  Vector dy = y; dy -= yf;
  Vector tmp( y.nelem() );
  mult( tmp, SeInv, dy );
  cost_y = dy * tmp;
  cost_y /= normfac;
}



//! Calculation of x-part of cost function
/*!
  This version is suitable if no term is already at hand.

  \param[out] cost_x    Const function value
  \param[in]  x         State vector.
  \param[in]  xa        A priori state.
  \param[in]  SxInv     Inverse of relevenaty covariance matrix
  \param[in]  normfac   Normalisation factor. The cost is scaled with 1/normfac

  \author Patrick Eriksson 
  \date   2015-10-05
*/
void oem_cost_x( Numeric& cost_x, 
                 ConstVectorView x, 
                 ConstVectorView xa,
                 ConstMatrixView SxInv,
                 const Numeric&  normfac )
{
  Vector dx = x; dx -= xa;
  Vector tmp( x.nelem() );
  mult( tmp, SxInv, dx );
  cost_x = dx * tmp;
  cost_x /= normfac;
}




//------------------------------------------------------------------------------------
//
//   OEM versions using the inverse of covariance matrices
//
//------------------------------------------------------------------------------------


//! Linear OEM, n-form.
/*!

Computes the inverse of a linear forward model by computing the MAP
solution as described in Rodgers, Inverse Methods for Atmospheric Sounding,
p. 67. This function uses the n-form (eq. (4.3)) which requires the solution
of a linear system of equations of size n-times-n.

Requires the inverses of the covariance matrices for the state and measurement
vector to be provided as arguments.

  \return Convergence status, see *oem_diagnostics*
  \param[out] x The optimal, estimated state vector consisting of n elements.
  \param[out] G The gain matrix.
  \param[in] xa The a priori state vector
  \param[in] yf The value of the forward model at a priori.
  \param[in] y The measurement vector consisting of m elements.
  \param[in] K The weighting function (m,n)-matrix
  \param[in] SeInv The inverse of the measurement error covariance (m,m)-matrix
  \param[in] SxInv The inverse of the a priori covariance (n,n)-matrix
*/
Index oem_linear_nform( Vector& x,
                        Matrix& G,
                        Matrix& J,
                        Vector& yf,
                        Numeric& cost_y, 
                        Numeric& cost_x,
                        ForwardModel &F,
                        ConstVectorView xa,
                        ConstVectorView x_norm,
                        ConstVectorView y,
                        ConstMatrixView SeInv,
                        ConstMatrixView SxInv,
                        const Numeric& cost_start,
                        const bool& verbose )
{
    const Index m = J.nrows();
    const Index n = J.ncols();

    // Check dimensions for consistency.
    assert( xa.nelem() == n );
    assert( x_norm.nelem() == 0  || x_norm.nelem() == n );
    assert( y.nelem() == m );
    assert( (SeInv.ncols() == m) && (SeInv.nrows() == m) );
    assert( (SxInv.ncols() == n) && (SxInv.nrows() == n) );

    // Initialize log output.
    if (verbose)
      {
        log_init_li( cout );
        log_step_li( cout, 0, cost_start, 0, cost_start );
      }

    // n-form (eq. (4.4)).
    Matrix tmp_nm(n,m), tmp_nn(n,n), tmp_nn2(n,n);
    ArrayOfIndex indx(n);
    Vector tmp_m(m);

    mult( tmp_nm, transpose(J), SeInv );
    mult( tmp_nn, tmp_nm, J); // tmp_nn = K^T S_e^{-1} K
    tmp_nn += SxInv;

    // Compute Gain matrix.
    inv( tmp_nn2, tmp_nn );
    mult( G, tmp_nn2, tmp_nm );

    // Compute x
    tmp_m = y;
    tmp_m -= yf;
    mult( x, G, tmp_m );
    x += xa;

    // Calculate yf and cost values
    //
    F.evaluate( yf, x );
    //
    oem_cost_y( cost_y, y, yf, SeInv, (Numeric) m );
    oem_cost_x( cost_x, x, xa, SxInv, (Numeric) m );

    // Finalise log output.
    if (verbose)
      {
        log_step_li( cout, 1, cost_y+cost_x, cost_x, cost_y );
        log_finalize_li( cout );
      }

    // Return convergence status
    return 1;   // Should be set to 9 if failure when calculating yf
}




//! Gauss-Newton non-linear OEM using precomputed inverses, n-form.
/*!
  Computes the optimal nonlinear inverse of a given forward model using the
  Gauss-Newton method as given in eq. (5.8) in Rodgers book. The forward model
  is given by an object implementing the interface described by the ForwardModel
  class. Convergence is determined using equation (5.29). The given
  tolerance is scaled by the problem size n. If the method doesn't converge it
  abords after the given maximum number of iterations.

  During execution two additional n-times-m and one n-times-n matrix is
  allocated. In addition to that, space for 4 length-n vectors and two length-m
  vectors is allocated. The given Matrix and Vector views may not overlap.

  \param[out] x The optimal inverse x.
  \param[out] yf The fitted measurement vector from the second-last GN iteration.
  \param[out] G The gain matrix.
  \param[out] J The jacobian as computed in the second-last GN iteration.
  \param[in] y The length-m, input measurement vector.
  \param[in] xa The size-n a-priori state vector.
  \param[in] SeInv The inverse of the measurement error covariance (m,m)-matrix
  \param[in] SxInv The inverse of the a priori covariance (n,n)-matrix
  \param[in] K The ForwardModel representing the forward model to invert.
  \param[in] tol The convergence criterium before scaling by the problem size.
  \param[in] maxIter Tha maximum number of iterations in case of no convergence.
  \param[in] verbose If true, log message are printed to stdout.

  \return True if the method has converged, false otherwise.
*/
Index oem_gauss_newton( Vector& x,
                        Matrix& G,
                        Matrix& J,
                        Vector& yf,
                        Numeric& cost_y, 
                        Numeric& cost_x,
                        Index& iter,
                        ForwardModel &F,
                        ConstVectorView xa,
                        ConstVectorView x_norm,
                        ConstVectorView y,
                        ConstMatrixView SeInv,
                        ConstMatrixView SxInv,
                        const Numeric& cost_start,
                        const Index maxIter,
                        const Numeric tol,
                        const bool& verbose )
{
    const Index m = J.nrows();
    const Index n = J.ncols();

    // Check dimensions for consistency.
    assert( xa.nelem() == n );
    assert( x_norm.nelem() == 0  || x_norm.nelem() == n );
    assert( y.nelem() == m );
    assert( (SeInv.ncols() == m) && (SeInv.nrows() == m) );
    assert( (SxInv.ncols() == n) && (SxInv.nrows() == n) );

    Numeric di2;

    // Following Rodgers' notation, we use Ki for the notation: J = Ki
    Matrix KiTSeInv(n,m), KiTSeInvKi(n,n), KiTSeInvKi2(n,n);
    Vector tm1(m), tm2(m), tn1(n), tn2(n), dx(n);

    cost_x = 0.0; cost_y = 0.0;

    iter = 0;
    bool converged = false;

    // Initialize log output.
    if (verbose)
      {
        log_init_gn( cout, tol, maxIter );
        log_step_gn( cout, 0, cost_start, 0, cost_start, NAN );
      }

    // Set the starting vector.
    x = xa;

    while ( (!converged) && (iter < maxIter) )
    {
        // Compute Jacobian and y_i.
        F.evaluate_jacobian( yf, J, x);

        mult( KiTSeInv, transpose(J), SeInv );
        mult( KiTSeInvKi, KiTSeInv, J );
        KiTSeInvKi += SxInv;

        // tm1 = K_i(x_i - x_a)

        tn1 = x;
        tn1 -= xa;
        mult( tn2, SxInv, tn1 );

        tm1 = y;
        tm1 -= yf;
        // tn1 = K_i^T * S_e^{-1} * (y - F(x_i))
        mult( tn1, KiTSeInv, tm1 );

        // This vector is used to test for convergence later.
        // See eqn. (5.31).
        tn1 -= tn2;

        solve( dx, KiTSeInvKi, tn1 );
        x += dx;

        di2 = dx * tn1;
        if ( di2 <= tol * (Numeric) n )
        {
            converged = true;
        }


        // Increase convergence counter and check for convergence.
        iter++;

        // Compute cost function contributions.
        tn1 = xa;
        tn1 -= x;
        mult( tn2, SxInv, tn1 );
        cost_x = tn2*tn1;
        cost_x /= (Numeric) m;

        mult( tm2, SeInv, tm1 );
        cost_y = tm1*tm2;
        cost_y /= (Numeric) m;

        // Print log.
        if ( verbose )
            log_step_gn( cout, iter, cost_x + cost_y, cost_x, cost_y, di2 );
    }

    // Compute fitted measurement vector and jacobian.
    F.evaluate_jacobian( yf, J, x );

    // Compute gain matrix.
    inv( KiTSeInvKi2, KiTSeInvKi );
    mult( G, KiTSeInvKi2, KiTSeInv );

    // Finalize log output.
    if ( verbose )
        log_finalize_gn( cout, converged, iter, maxIter );

    if( converged )
      return 0;
    else
      return 1;      
}



//! Non-linear OEM using Levenberg-Marquardt method.
/*!
  Inverts a given non-linear forward model using the Levenberg-Marquardt method.
  The implementation follows Eq. (5.36) in Rodger's book.

  Communication with the forward model is performed in the same way as in
  oem_gauss_newton().

  The start value for gamma is given by the parameter gamma_start. If a new x
  value is found, gamma is decreased by a factor gamma_scale_dec. If the ost
  is increased, gamma is increased by a factor gamma_scale_inc. If gamma falls
  below gamma_threshold, it is set to zero. If no lower cost can be obtained
  with gamma = 0, gamma is set to 1. If gamma becomes larger than gamma_max and
  the cost can not be reduced, the iteration is stopped.

  During the execution, space for two n-times-n and one n-times-m matrices is
  allocated as well as space for 5 length-n vectors and two length-m vectors.

  \param[out] x The optimal estimator of the state vector.
  \param[out] yf The fitted state vector as computed in the second-last LM
  iteration.
  \param[out] G The gain matrix.
  \param[out] J The jacobian as computed in the second-last LM iteration.
  \param[in] y The size-m input measurement vector.
  \param[in] xa The size-n a priori state vector.
  \param[in] K A forward model object implementing the FowardModel class.
  \param[in] Seinv The inverse of the covariance matrix of the measurement error.
  \param[in] Sxinv The inverse of the covariance matrix of the a priori error.
  \param[in] tol The normalized convergence criterion.
  \param[in] maxIter The maximum number of iterations before abortion.
  \param[in] gamma_start The start value of the gamma factor.
  \param[in] gamma_scale_dec The factor to decrease gamma by if the cost function
  was descreased.
  \param[in] gamma_scale_inc The factor to increase gamma by if the cost function
  could not be decreased.
  \param[in] gamma_max The maximum gamma value. If gamma == gamma_max and the cost
  function can not be decreased, the iteration is aborted.
  \param[in] gamma_threshold The minimum value that gamma can take on before it is
  set to zero.
  \param[in] verbose If true, log messages are printed to stdout.

  \return True if the method has converged, false otherwise.
*/
bool oem_levenberg_marquardt( Vector& x,
                              Vector& yf,
                              Matrix& G,
                              Matrix& J,
                              ConstVectorView y,
                              ConstVectorView xa,
                              ConstMatrixView SeInv,
                              ConstMatrixView SxInv,
                              ForwardModel &K,
                              Numeric tol,
                              Index maxIter,
                              Numeric gamma_start,
                              Numeric gamma_scale_dec,
                              Numeric gamma_scale_inc,
                              Numeric gamma_max,
                              Numeric gamma_threshold,
                              bool verbose )
{

    Index n( xa.nelem() ), m( y.nelem() );
    Numeric di2, cost, cost_old, cost_x, cost_y, gamma;
    cost = 0.0; cost_x = 0.0; cost_y = 0.0;

    Vector xnew(n), yi(m);
    Vector tm(m), tn1(n), tn2(n), tn3(n), dx(n);
    Matrix KiTSeInv(n,m), KiTSeInvKi(n,n), KiTSeInvKi2(n,n);

    if ( verbose )
        log_init_lm( cout, tol, maxIter );

    // Set starting vector.
    x = xa;

    gamma = gamma_start;
    cost_old = 0.0;

    Index iter = 0;
    bool converged = false;

    while ( (!converged) && (iter < maxIter) )
    {
        // Compute Jacobian and y_i.
        K.evaluate_jacobian( yi, J, x);

        mult( KiTSeInv, transpose(J), SeInv );
        mult( KiTSeInvKi, KiTSeInv, J );

        tn1 = x;
        tn1 -= xa;
        mult( tn2, SxInv, tn1 );

        tm = y;
        tm -= yi;
        mult( tn1, KiTSeInv, tm );

        tn1 -= tn2;

        // Compute old_cost for first iteration.
        if (iter == 0)
        {
            mult( yi, SeInv, tm );
            cost_y = tm * yi;
            cost_y /= (Numeric) m;
            cost_old = cost_y;
        }

        bool found_x = false;
        while ( !found_x )
        {
            KiTSeInvKi2 = SxInv;
            KiTSeInvKi2 *= ( 1.0 + gamma );
            KiTSeInvKi2 += KiTSeInvKi;

            // This vector is used to test for convergence later.
            // See eqn. (5.31).

            solve( dx, KiTSeInvKi2, tn1 );
            xnew = x;
            xnew += dx;

            // Evaluate cost function.

            K.evaluate( yi, xnew );

            tm = y;
            tm -= yi;
            mult( yi, SeInv, tm );
            cost_y = tm*yi;
            cost_y /= (Numeric) m;

            tn2 = xnew;
            tn2 -= xa;
            mult( tn3, SxInv, tn2 );
            cost_x = tn3 * tn2;
            cost_x /= (Numeric) m;
            cost = cost_x + cost_y;

            // If cost has decreased, keep new x and
            // scale gamma.
            if (cost < cost_old)
            {

                if ( gamma >= (gamma_threshold * gamma_scale_dec))
                    gamma /= gamma_scale_dec;
                else
                    gamma = 0;

                x = xnew;
                cost_old = cost;
                found_x = true;

            }
            // Else try to increase gamma and look for a
            // new x.
            else
            {
                if ( gamma < gamma_threshold )
                           gamma = gamma_threshold;
                else
                {
                    if ( gamma < gamma_max )
                    {
                        gamma *= gamma_scale_inc;
                        if (gamma > gamma_max)
                            gamma = gamma_max;
                    }
                    // Gamma exceeds maximum. Abort.
                    else
                    {
                        iter = maxIter;
                        break;
                    }
                }

                if ( verbose )
                    log_gamma_step_lm( cout, cost, cost_x, cost_y, gamma );

            }
        }

        // Increase iteration counter and check for convergence.
        iter++;

        di2 = dx * tn1;
        di2 /= (Numeric) n;

        if ( di2 <= tol )
        {
            converged = true;
        }

        if ( verbose )
            log_step_lm( cout, iter, cost, cost_x, cost_y, gamma, di2 );

    }

    // Compute fitted measurement vector and jacobian.
    K.evaluate_jacobian( yf, J, x );

    // Compute gain matrix.

    KiTSeInvKi += SxInv;
    inv( KiTSeInvKi2, KiTSeInvKi );
    mult( G, KiTSeInvKi2, KiTSeInv );

    if ( verbose )
        log_finalize_lm( cout, converged, cost, cost_x, cost_y,
                         gamma, gamma_max, iter, maxIter );
    return converged;
}




//------------------------------------------------------------------------------------
//
//   OEM versions so far not used
//
//------------------------------------------------------------------------------------


//! Linear OEM, m-form.
/*!

Computes the inverse of a linear forward model by computing the MAP
solution as described in Rodgers, Inverse Methods for Atmospheric Sounding,
p. 67. This function uses the m-form ( Eq. (4.6) ) which requires the solution
of a linear system of equations of size m-times-m.

For the execution 1 n-times-m matrices, 2 m-times-m matrices and a vector with m
elements are allocated. The given Matrix and Vector views may not overlap.

  \param[out] x The optimal, estimated state vector consisting of n elements.
  \param[in] y The measurement vector consisting of m elements.
  \param[in,out] yf On input yf should contain the value of the forward model
  at the linearization point. On output yf should contain the fitted measurement
  vector.
  \param[in] xa The mean a priori state vector.
  \param[in] K The weighting function (m,n)-matrix.
  \param[in] Se The measurement error covariance (m,m)-matrix.
  \param[in] Sx The a priori covariance (n,n)-matrix.
  \param[out] G The gain matrix.
*/
void oem_linear_mform( VectorView x,
                       MatrixView G,
                       ConstVectorView xa,
                       ConstVectorView yf,
                       ConstVectorView y,
                       ConstMatrixView K,
                       ConstMatrixView Se,
                       ConstMatrixView Sx )
{
    Index m = K.nrows();
    Index n = K.ncols();

    // Check dimensions for consistency.
    assert( x.nelem() == n );
    assert( xa.nelem() == n );
    assert( y.nelem() == m );

    assert( (Se.ncols() == m) && (Se.nrows() == m) );
    assert( (Sx.ncols() == n) && (Sx.nrows() == n) );

    // m-form (eq. (4.6)).
    Matrix tmp_nm(n,m);
    Matrix tmp_mm(m,m), tmp_mm2(m,m);
    Vector tmp_m(m);

    mult( tmp_nm, Sx, transpose(K) ); // tmp_nm = S_a * K^T
    mult( tmp_mm, K, tmp_nm);
    tmp_mm += Se;

    // Compute gain matrix.
    inv( tmp_mm2, tmp_mm );
    mult( G, tmp_nm, tmp_mm2 );

    tmp_m = y;
    tmp_m -= yf;
    mult( x, G, tmp_m );

    x += xa;
}



//! Non-linear OEM using Gauss-Newton method.
/*!
  Computes the optimal nonlinear inverse of a given forward model using the
  Gauss-Newton method as given in eq. (5.10) in Rodgers book. The forward model
  is given by an object implementing the interface described by the ForwardModel
  class. Convergence is determined using equation (5.33). The given
  tolerance is scaled by the problem size m. If the method doesn't converge it
  abords after the given maximum number of iterations.

  During the execution, space for up to 6 additional matrices and vectors is
  allocated. The given Matrix and Vector views should not overlap.

  \param[out] x The optimal inverse x.
  \param[in] y The measurement vector containing m measurements.
  \param[in] xa The size-n a-priori state vector.
  \param[in] K The ForwardModel representing the forward model to invert.
  \param[in] Se The measurement error covariance (m,m)-matrix
  \param[in] Sx The a priori covariance (n,n)-matrix
  \param[in] tol The convergence criterium before scaling by the problem size.
  \param maxIter Tha maximum number of iterations in case of no convergence.
*/
bool oem_gauss_newton_m_form( VectorView x,
                              ConstVectorView y,
                              ConstVectorView xa,
                              ForwardModel &K,
                              ConstMatrixView Se,
                              ConstMatrixView Sx,
                              Numeric tol,
                              Index maxIter )
{
    Index n( x.nelem() ), m( y.nelem() );
    Numeric di2;
    Matrix Ki(m,n), SxKiT(m, n), KiSxKiT(m, m);
    Vector xi(n), yi(m), tm(m), tm2(m), tn(n), yi_old(m);

    Index iter = 0;
    bool converged = false;

    // Set the starting vector.
    x = xa;

    while ((!converged) && (iter < maxIter))
    {

        // Compute Jacobian and y_i.
        K.evaluate_jacobian( yi, Ki, x );
        // If not in the first iteration, check for
        // convergence here.
        if (iter > 0)
        {
            yi_old -= yi;

            solve( tm, Se, yi_old );
            mult( tm2, KiSxKiT, tm );
            // TODO: Optimize using LU decomp.
            solve( tm, Se, tm2);
            di2 = yi_old * tm;
            if ( fabs( di2 ) < tol * (Numeric) n )
            {
                converged = true;
                break;
            }
        }

        mult( SxKiT, Sx, transpose(Ki) );
        mult( KiSxKiT, Ki, SxKiT );
        KiSxKiT += Se;

        // tm = K_i(x_i - x_a)
        tn = x;
        tn -= xa;
        mult( tm, Ki, tn );

        tm -= yi;
        tm += y;

        solve( tm2, KiSxKiT, tm );
        mult( x, SxKiT, tm2 );
        x += xa;

        // Increase iteration counter and store yi.
        iter++;
        yi_old = yi;
    }
    return converged;
}

//! Non-linear OEM using Gauss-Newton method.
/*!
  Computes the optimal nonlinear inverse of a given forward model using the
  Gauss-Newton method as given in eq. (5.9) in Rodgers book. The forward model
  is given by an object implementing the interface described by the ForwardModel
  class. Convergence is determined using equation (5.29). The given
  tolerance is scaled by the problem size m. If the method doesn't converge it
  abords after the given maximum number of iterations.

  During the execution, space for up to 6 additional matrices and vectors is
  allocated. The given Matrix and Vector views should not overlap.

  \param[out] x The optimal inverse x.
  \param[in] y The measurement vector containing m measurements.
  \param[in] xa The size-n a-priori state vector.
  \param[in] K The ForwardModel representing the forward model to invert.
  \param[in] Se The measurement error covariance (m,m)-matrix
  \param[in] Sx The a priori covariance (n,n)-matrix
  \param[in] tol The convergence criterium before scaling by the problem size.
  \param maxIter Tha maximum number of iterations in case of no convergence.

  \return True if the method has converged, false otherwise.
*/
bool oem_gauss_newton_n_form( VectorView x,
                              ConstVectorView y,
                              ConstVectorView xa,
                              ForwardModel &K,
                              ConstMatrixView Se,
                              ConstMatrixView Sx,
                              Numeric tol,
                              Index maxIter )
{
    Index n( x.nelem() ), m( y.nelem() );
    Numeric di2;
    Matrix Ki(m,n), KiTSeInvKi(n, n), KiTSeInv(m,n), SeInv(m,m), SxInv(n,n);
    Vector xi(n), dx(n), dx_old(n), yi(m), tm(m), tn(n), sInvDx(n);

    Index iter = 0;
    bool converged = false;

    // Required matrix inverses.
    inv(SeInv, Se);
    inv(SxInv, Sx);

    // Set the starting vector.
    x = xa;
    dx_old = 0;

    while ((!converged) && (iter < maxIter))
    {

        // Compute Jacobian and y_i.
        K.evaluate_jacobian( yi, Ki, x );

        mult( KiTSeInv, transpose( Ki ), SeInv );
        mult( KiTSeInvKi, KiTSeInv, Ki );
        KiTSeInvKi += SxInv;

        // tm = K_i(x_i - x_a)
        tn = x;
        tn -= xa;
        mult( tm, Ki, tn );

        tm -= yi;
        tm += y;

        mult( tn, KiTSeInv, tm );
        solve( dx, KiTSeInvKi, tn );

        x = xa;
        x += dx;

        // Check for convergence.
        tm = y;
        tm -= yi;
        mult( sInvDx, KiTSeInv, tm );
        mult( tn, SxInv, dx_old );
        sInvDx -= tn;

        dx -= dx_old;
        di2 = dx * sInvDx;

        if ( di2 <= tol * (Numeric) m )
        {
            converged = true;
        }

        // Increase iteration counter and store yi.
        iter++;
        dx_old = dx;
    }
    return converged;
}


