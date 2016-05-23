/* Copyright (C) 2014
   Richard Larsson <ric.larsson@gmail.com>

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */
   
   

#include "zeeman.h"
#include "global_data.h"

/*!
 *  Defines the phase of the propagation matrix.
 *  Read Larsson et al. (2013) for an explanation.
 * 
 *  \param  K       Out:    The rotation extinction matrix.
 *  \param  theta   In:     Angle between the magnetic field and the
 *                          propagation path. In radians.
 *  \param  eta     In:     Angle to rotate planar polarization clockwise to
 *                          fit the general coordinate system. In radians.
 *  \param  DM      In:     Change in the secondary rotational quantum number.
 * 
 *  \author Richard Larsson
 *  \date   2012-08-03
 */
void phase_matrix(MatrixView K, const Numeric& theta, const Numeric& eta, const Index& DM)
{
    assert(K.nrows() == 4 );
    assert(K.ncols() == 4 );
    
    const Numeric
    S2T = sin(theta)*sin(theta),  
    CT  = cos(theta), 
    CE2 = cos(2*eta), 
    SE2 = sin(2*eta);
    
    
    switch( DM )
    {
        case -1: // Transitions anti-parallel to the magnetic field
            K(0,0) =   0;  K(0,1) =           0;  K(0,2) =           0;  K(0,3) =           0;
            K(1,0) =   0;  K(1,1) =           0;  K(1,2) =      2 * CT;  K(1,3) =   S2T * SE2;
            K(2,0) =   0;  K(2,1) =    - 2 * CT;  K(2,2) =           0;  K(2,3) = - S2T * CE2;
            K(3,0) =   0;  K(3,1) = - S2T * SE2;  K(3,2) =   S2T * CE2;  K(3,3) =           0;
            break;
        case  1: // Transitions parallel to the magnetic field
            K(0,0) =   0;  K(0,1) =           0;  K(0,2) =           0;  K(0,3) =           0;
            K(1,0) =   0;  K(1,1) =           0;  K(1,2) =    - 2 * CT;  K(1,3) =   S2T * SE2;
            K(2,0) =   0;  K(2,1) =      2 * CT;  K(2,2) =           0;  K(2,3) = - S2T * CE2;
            K(3,0) =   0;  K(3,1) = - S2T * SE2;  K(3,2) =   S2T * CE2;  K(3,3) =           0;
            break;
        case  0:// Transitions perpendicular to the magnetic field
            K(0,0) =   0;  K(0,1) =           0;  K(0,2) =           0;  K(0,3) =           0;
            K(1,0) =   0;  K(1,1) =           0;  K(1,2) =           0;  K(1,3) = - S2T * SE2;
            K(2,0) =   0;  K(2,1) =           0;  K(2,2) =           0;  K(2,3) =   S2T * CE2;
            K(3,0) =   0;  K(3,1) =   S2T * SE2;  K(3,2) = - S2T * CE2;  K(3,3) =           0;
            break;
        default:
            throw std::runtime_error("Impossible Delta M to phase matrix");
            break;
    };
};


/*!
 *  Defines the attenuation of the propagation matrix.
 *  Read Larsson et al. (2013) for an explanation.
 * 
 *  \param  K       Out:    The rotation extinction matrix.
 *  \param  theta   In:     Angle between the magnetic field and the
 *                          propagation path. In radians.
 *  \param  eta     In:     Angle to rotate planar polarization clockwise to
 *                          fit the general coordinate system. In radians.
 *  \param  DM      In:     Change in the secondary rotational quantum number.
 * 
 *  \author Richard Larsson
 *  \date   2012-08-03
 */
void attenuation_matrix(MatrixView K, const Numeric& theta, const Numeric& eta, const Index& DM)
{
    assert(K.nrows() == 4 );
    assert(K.ncols() == 4 );
    
    const Numeric 
    S2T  = sin(theta)*sin(theta),  
    C2T  = cos(theta)*cos(theta),  
    CT   = cos(theta), 
    CE2  = cos(2*eta), 
    SE2  = sin(2*eta);
    
    switch( DM )
    {
        case -1: // Transitions anti-parallel to the magnetic field
            K(0,0) =     1 + C2T;  K(0,1) =   S2T * CE2;  K(0,2) =   S2T * SE2;  K(0,3) =    2 * CT;
            K(1,0) =   S2T * CE2;  K(1,1) =     1 + C2T;  K(1,2) =           0;  K(1,3) =         0;
            K(2,0) =   S2T * SE2;  K(2,1) =           0;  K(2,2) =     1 + C2T;  K(2,3) =         0;
            K(3,0) =      2 * CT;  K(3,1) =           0;  K(3,2) =           0;  K(3,3) =   1 + C2T;
            break;
        case  1: // Transitions parallel to the magnetic field
            K(0,0) =     1 + C2T;  K(0,1) =   S2T * CE2;  K(0,2) =   S2T * SE2;  K(0,3) =  - 2 * CT;
            K(1,0) =   S2T * CE2;  K(1,1) =     1 + C2T;  K(1,2) =           0;  K(1,3) =         0;
            K(2,0) =   S2T * SE2;  K(2,1) =           0;  K(2,2) =     1 + C2T;  K(2,3) =         0;
            K(3,0) =    - 2 * CT;  K(3,1) =           0;  K(3,2) =           0;  K(3,3) =   1 + C2T;
            break;
        case  0: // Transitions perpendicular to the magnetic field
            K(0,0) =         S2T;  K(0,1) = - S2T * CE2;  K(0,2) = - S2T * SE2;  K(0,3) =         0;
            K(1,0) = - S2T * CE2;  K(1,1) =         S2T;  K(1,2) =           0;  K(1,3) =         0;
            K(2,0) = - S2T * SE2;  K(2,1) =           0;  K(2,2) =         S2T;  K(2,3) =         0;
            K(3,0) =           0;  K(3,1) =           0;  K(3,2) =           0;  K(3,3) =       S2T;
            break;
        default:
            throw std::runtime_error("Impossible Delta M to attenuation matrix");
            break;
    };
};


/*!
 *  Defines the derivative of the phase of the propagation matrix with regards to theta
 * 
 *  \param  dK      Out:    The rotation extinction matrix derivative with regards to theta.
 *  \param  theta   In:     Angle between the magnetic field and the
 *                          propagation path. In radians.
 *  \param  eta     In:     Angle to rotate planar polarization clockwise to
 *                          fit the general coordinate system. In radians.
 *  \param  DM      In:     Change in the secondary rotational quantum number.
 * 
 *  \author Richard Larsson
 *  \date   2015-08-14
 */
void dphase_matrix_dtheta(MatrixView dK, const Numeric& theta, const Numeric& eta, const Index& DM)
{
    assert(dK.nrows() == 4 );
    assert(dK.ncols() == 4 );
    
    const Numeric 
    ST   = sin(theta),
    CTST = sin(theta)*cos(theta), 
    CE2  = cos(2*eta), 
    SE2  = sin(2*eta);
    
    
    switch( DM )
    {
        case -1: // Transitions anti-parallel to the magnetic field
            dK(0,0) =   0;  dK(0,1) =                0;  dK(0,2) =                0;  dK(0,3) =                0;
            dK(1,0) =   0;  dK(1,1) =                0;  dK(1,2) =         - 2 * ST;  dK(1,3) =   2 * CTST * SE2;
            dK(2,0) =   0;  dK(2,1) =           2 * ST;  dK(2,2) =                0;  dK(2,3) = - 2 * CTST * CE2;
            dK(3,0) =   0;  dK(3,1) = - 2 * CTST * SE2;  dK(3,2) =   2 * CTST * CE2;  dK(3,3) =                0;
            break;
        case  1: // Transitions parallel to the magnetic field
            dK(0,0) =   0;  dK(0,1) =                0;  dK(0,2) =                0;  dK(0,3) =                0;
            dK(1,0) =   0;  dK(1,1) =                0;  dK(1,2) =           2 * ST;  dK(1,3) =   2 * CTST * SE2;
            dK(2,0) =   0;  dK(2,1) =        -  2 * ST;  dK(2,2) =                0;  dK(2,3) = - 2 * CTST * CE2;
            dK(3,0) =   0;  dK(3,1) = - 2 * CTST * SE2;  dK(3,2) =   2 * CTST * CE2;  dK(3,3) =                0;
            break;
        case  0:// Transitions perpendicular to the magnetic field
            dK(0,0) =   0;  dK(0,1) =                0;  dK(0,2) =                0;  dK(0,3) =                0;
            dK(1,0) =   0;  dK(1,1) =                0;  dK(1,2) =                0;  dK(1,3) = - 2 * CTST * SE2;
            dK(2,0) =   0;  dK(2,1) =                0;  dK(2,2) =                0;  dK(2,3) =   2 * CTST * CE2;
            dK(3,0) =   0;  dK(3,1) =   2 * CTST * SE2;  dK(3,2) = - 2 * CTST * CE2;  dK(3,3) =                0;
            break;
        default:
            throw std::runtime_error("Impossible Delta M to phase matrix");
            break;
    };
};


/*!
 *  Defines the derivative of the phase of the propagation matrix with regards to eta
 * 
 *  \param  dK      Out:    The rotation extinction matrix derivative with regards to eta.
 *  \param  theta   In:     Angle between the magnetic field and the
 *                          propagation path. In radians.
 *  \param  eta     In:     Angle to rotate planar polarization clockwise to
 *                          fit the general coordinate system. In radians.
 *  \param  DM      In:     Change in the secondary rotational quantum number.
 * 
 *  \author Richard Larsson
 *  \date   2015-08-14
 */
void dphase_matrix_deta(MatrixView dK, const Numeric& theta, const Numeric& eta, const Index& DM)
{
    assert(dK.nrows() == 4 );
    assert(dK.ncols() == 4 );
    
    const Numeric 
    S2T = sin(theta)*sin(theta),  
    CE2 = cos(2*eta),
    SE2 = sin(2*eta);
    
    
    switch( DM )
    {
        case -1: // Transitions anti-parallel to the magnetic field
            dK(0,0) =   0;  dK(0,1) =               0;  dK(0,2) =               0;  dK(0,3) =               0;
            dK(1,0) =   0;  dK(1,1) =               0;  dK(1,2) =               0;  dK(1,3) =   2 * S2T * CE2;
            dK(2,0) =   0;  dK(2,1) =               0;  dK(2,2) =               0;  dK(2,3) =   2 * S2T * SE2;
            dK(3,0) =   0;  dK(3,1) = - 2 * S2T * CE2;  dK(3,2) = - 2 * S2T * SE2;  dK(3,3) =               0;
            break;
        case  1: // Transitions parallel to the magnetic field
            dK(0,0) =   0;  dK(0,1) =               0;  dK(0,2) =               0;  dK(0,3) =               0;
            dK(1,0) =   0;  dK(1,1) =               0;  dK(1,2) =               0;  dK(1,3) =   2 * S2T * CE2;
            dK(2,0) =   0;  dK(2,1) =               0;  dK(2,2) =               0;  dK(2,3) =   2 * S2T * SE2;
            dK(3,0) =   0;  dK(3,1) = - 2 * S2T * CE2;  dK(3,2) = - 2 * S2T * SE2;  dK(3,3) =               0;
            break;
        case  0:// Transitions perpendicular to the magnetic field
            dK(0,0) =   0;  dK(0,1) =               0;  dK(0,2) =               0;  dK(0,3) =               0;
            dK(1,0) =   0;  dK(1,1) =               0;  dK(1,2) =               0;  dK(1,3) = - 2 * S2T * CE2;
            dK(2,0) =   0;  dK(2,1) =               0;  dK(2,2) =               0;  dK(2,3) = - 2 * S2T * SE2;
            dK(3,0) =   0;  dK(3,1) =   2 * S2T * CE2;  dK(3,2) =   2 * S2T * SE2;  dK(3,3) =               0;
            break;
        default:
            throw std::runtime_error("Impossible Delta M to phase matrix");
            break;
    };
};


/*!
 *  Defines the derivative of the attenuation of the propagation matrix with regards to theta.
 * 
 *  \param  dK      Out:    The rotation extinction matrix derivative with regards to theta.
 *  \param  theta   In:     Angle between the magnetic field and the
 *                          propagation path. In radians.
 *  \param  eta     In:     Angle to rotate planar polarization clockwise to
 *                          fit the general coordinate system. In radians.
 *  \param  DM      In:     Change in the secondary rotational quantum number.
 * 
 *  \author Richard Larsson
 *  \date   2015-08-14
 */
void dattenuation_matrix_dtheta(MatrixView dK, const Numeric& theta, const Numeric& eta, const Index& DM)
{
    assert(dK.nrows() == 4 );
    assert(dK.ncols() == 4 );
    
    const Numeric 
    CTST = cos(theta)*sin(theta), 
    ST   = sin(theta),
    CE2  = cos(2*eta), 
    SE2  = sin(2*eta);
    
    switch( DM )
    {
        case -1: // Transitions anti-parallel to the magnetic field
            dK(0,0) =       - 2 * CTST;  dK(0,1) =   2 * CTST * CE2;  dK(0,2) =   2 * CTST * SE2;  dK(0,3) =   - 2 * ST;
            dK(1,0) =   2 * CTST * CE2;  dK(1,1) =       - 2 * CTST;  dK(1,2) =                0;  dK(1,3) =          0;
            dK(2,0) =   2 * CTST * SE2;  dK(2,1) =                0;  dK(2,2) =       - 2 * CTST;  dK(2,3) =          0;
            dK(3,0) =         - 2 * ST;  dK(3,1) =                0;  dK(3,2) =                0;  dK(3,3) = - 2 * CTST;
            break;
        case  1: // Transitions parallel to the magnetic field
            dK(0,0) =       - 2 * CTST;  dK(0,1) =   2 * CTST * CE2;  dK(0,2) =   2 * CTST * SE2;  dK(0,3) =     2 * ST;
            dK(1,0) =   2 * CTST * CE2;  dK(1,1) =       - 2 * CTST;  dK(1,2) =                0;  dK(1,3) =          0;
            dK(2,0) =   2 * CTST * SE2;  dK(2,1) =                0;  dK(2,2) =       - 2 * CTST;  dK(2,3) =          0;
            dK(3,0) =           2 * ST;  dK(3,1) =                0;  dK(3,2) =                0;  dK(3,3) = - 2 * CTST;
            break;
        case  0: // Transitions perpendicular to the magnetic field
            dK(0,0) =         2 * CTST;  dK(0,1) = - 2 * CTST * CE2;  dK(0,2) = - 2 * CTST * SE2;  dK(0,3) =          0;
            dK(1,0) = - 2 * CTST * CE2;  dK(1,1) =         2 * CTST;  dK(1,2) =                0;  dK(1,3) =          0;
            dK(2,0) = - 2 * CTST * SE2;  dK(2,1) =                0;  dK(2,2) =         2 * CTST;  dK(2,3) =          0;
            dK(3,0) =                0;  dK(3,1) =                0;  dK(3,2) =                0;  dK(3,3) =   2 * CTST;
            break;
        default:
            throw std::runtime_error("Impossible Delta M to attenuation matrix");
            break;
    };
};


/*!
 *  Defines the derivative of the attenuation of the propagation matrix with regards to eta.
 * 
 *  \param  dK      Out:    The rotation extinction matrix derivative with regards to eta.
 *  \param  theta   In:     Angle between the magnetic field and the
 *                          propagation path. In radians.
 *  \param  eta     In:     Angle to rotate planar polarization clockwise to
 *                          fit the general coordinate system. In radians.
 *  \param  DM      In:     Change in the secondary rotational quantum number.
 * 
 *  \author Richard Larsson
 *  \date   2015-08-14
 */
void dattenuation_matrix_deta(MatrixView dK, const Numeric& theta, const Numeric& eta, const Index& DM)
{
    assert(dK.nrows() == 4 );
    assert(dK.ncols() == 4 );
    
    const Numeric
    SE2 = sin(2*eta),
    CE2 = cos(2*eta),
    ST2 = sin(theta)*sin(theta);
    
    switch( DM )
    {
        case -1: // Transitions anti-parallel to the magnetic field
            dK(0,0) =               0;  dK(0,1) = - 2 * ST2 * SE2;  dK(0,2) =   2 * ST2 * CE2;  dK(0,3) =   0;
            dK(1,0) = - 2 * ST2 * SE2;  dK(1,1) =               0;  dK(1,2) =               0;  dK(1,3) =   0;
            dK(2,0) =   2 * ST2 * CE2;  dK(2,1) =               0;  dK(2,2) =               0;  dK(2,3) =   0;
            dK(3,0) =               0;  dK(3,1) =               0;  dK(3,2) =               0;  dK(3,3) =   0;
            break;
        case  1: // Transitions parallel to the magnetic field
            dK(0,0) =               0;  dK(0,1) = - 2 * ST2 * SE2;  dK(0,2) =   2 * ST2 * CE2;  dK(0,3) =   0;
            dK(1,0) = - 2 * ST2 * SE2;  dK(1,1) =               0;  dK(1,2) =               0;  dK(1,3) =   0;
            dK(2,0) =   2 * ST2 * CE2;  dK(2,1) =               0;  dK(2,2) =               0;  dK(2,3) =   0;
            dK(3,0) =               0;  dK(3,1) =               0;  dK(3,2) =               0;  dK(3,3) =   0;
            break;
        case  0: // Transitions perpendicular to the magnetic field
            dK(0,0) =               0;  dK(0,1) =   2 * ST2 * SE2;  dK(0,2) = - 2 * ST2 * CE2;  dK(0,3) =   0;
            dK(1,0) =   2 * ST2 * SE2;  dK(1,1) =               0;  dK(1,2) =               0;  dK(1,3) =   0;
            dK(2,0) = - 2 * ST2 * CE2;  dK(2,1) =               0;  dK(2,2) =               0;  dK(2,3) =   0;
            dK(3,0) =               0;  dK(3,1) =               0;  dK(3,2) =               0;  dK(3,3) =   0;
            break;
        default:
            throw std::runtime_error("Impossible Delta M to attenuation matrix");
            break;
    };
};


Numeric gs_caseb(const Rational& N, const Rational& J, const Rational& S, const Numeric& GS) { return (GS*((J*(J+1)+S*(S+1)-N*(N+1))/(J*(J+1))/2).toNumeric()); }
Numeric gs_casea(const Rational& Omega, const Rational& J, const Rational& Lambda, const Rational& Sigma, const Numeric& GS) { return GS*(Omega/2/J/(J+1)*(Lambda+2*Sigma)).toNumeric(); }


/*!
 * Return the relative strength of the split Zeeman line parts as found in
 * Berdyugina and Solnaki (2002). Note that this is the same as the general case
 * of Schadee (1978).
 * 
 * \param  __U__   Void.
 * \param  m       In:     Secondary rotational quantum number.
 * \param  j       In:     Spin-Orbit Coupling number.
 * \param  DJ      In:     Change in the main rotational quantum number.
 * \param  DM      In:     Change in the secondary rotational quantum number.
 * 
 * \author Richard Larsson
 * \date   2012-10-26
 */
Numeric relative_strength(const Rational& M, const Rational& J, const Index& dj, const Index& dm)
{
    
    // Variable to be returned.
    Rational ret_val;
    
    static const Rational three_fourths(3,4),one_and_a_half(3,2);
    
    switch ( dj )
    {
        case -1:
            switch ( dm )
            {
                case -1: // Transitions anti-parallel to the magnetic field
                    ret_val = three_fourths*(J+M)*(J-1+M)/(2*J*(2*J-1)*(2*J+1));
                    break;
                case  0: // Transitions perpendicular to the magnetic field
                    ret_val = one_and_a_half*(J*J-M*M)/(J*(2*J-1)*(2*J+1));
                    break;
                case +1: // Transitions parallel to the magnetic field
                    ret_val = three_fourths*(J-M)*(J-1-M)/(2*J*(2*J-1)*(2*J+1));
                    break;
                default:
                    throw std::runtime_error("Something is extremely wrong.");
                    break;
            }
            break;
        case  0:
            switch ( dm )
            {
                case -1: // Transitions anti-parallel to the magnetic field
                    ret_val = three_fourths*(J+M)*(J+1-M)/(2*J*(J+1)*(2*J+1));
                    break;
                case  0: // Transitions perpendicular to the magnetic field
                    ret_val = one_and_a_half*M*M/(J*(J+1)*(2*J+1));
                    break;
                case +1: // Transitions parallel to the magnetic field
                    ret_val = three_fourths*(J-M)*(J+1+M)/(2*J*(J+1)*(2*J+1));
                    break;
                default:
                    throw std::runtime_error("Something is extremely wrong.");
                    break;
            }
            break;
        case +1:
            switch ( dm )
            {
                case -1: // Transitions anti-parallel to the magnetic field
                    ret_val = three_fourths*(J+1-M)*(J+2-M)/(2*(J+1)*(2*J+1)*(2*J+3));
                    break;
                case  0: // Transitions perpendicular to the magnetic field
                    ret_val = one_and_a_half*((J+1)*(J+1)-M*M)/((J+1)*(2*J+1)*(2*J+3));
                    break;
                case +1: // Transitions parallel to the magnetic field
                    ret_val = three_fourths*(J+1+M)*(J+2+M)/(2*(J+1)*(2*J+1)*(2*J+3));
                    break;
                default:
                    throw std::runtime_error("Something is extremely wrong.");
                    break;
            }
            break;
        default:
            throw std::runtime_error("Something is extremely wrong.");
            break;
    }

    return ret_val.toNumeric();
}


/*!
 * Return the frequency change of the split Zeeman lines following
 * Berdyugina and Solnaki (2002) and Lenoir (1967).
 * 
 * \param  lr      In:     The line record with quantum numbers.
 * \param  H_mag   In:     The magnitude of the magnetic field.
 * \param  GS      In:     The Lande factor.
 * 
 * \author Richard Larsson
 * \date   2016-05-19 (combination of two precious versions from 2012)
 */
Numeric frequency_change(const LineRecord& lr,
                         const Numeric& H_mag, 
                         const Numeric& GS)
{
    Numeric Upper_E_part, Lower_E_part;
    
    // Lower:
    assert(abs(lr.QuantumNumbers().Lower()[QN_M])<=lr.QuantumNumbers().Lower()[QN_J]);
    switch(lr.QuantumNumbers().Lower()[QN_Hund].toIndex())
    {
        case Hund_Case_A:
            // This follows Berdyugina and Solnaki
            Lower_E_part = lr.QuantumNumbers().Lower()[QN_M].toNumeric() * 
            gs_casea(lr.QuantumNumbers().Lower()[QN_Omega],
                     lr.QuantumNumbers().Lower()[QN_J],
                     lr.QuantumNumbers().Lower()[QN_S],
                     lr.QuantumNumbers().Lower()[QN_Lambda],
                     GS);
            break;
        case Hund_Case_B:
            // This follows Lenoir
            if( lr.QuantumNumbers().Lower()[QN_J] == 0 )
            {
                Lower_E_part = 0;
            }
            else
            {
                Lower_E_part = lr.QuantumNumbers().Lower()[QN_M].toNumeric() * 
                gs_caseb(lr.QuantumNumbers().Lower()[QN_N],
                         lr.QuantumNumbers().Lower()[QN_J],
                         lr.QuantumNumbers().Lower()[QN_S],
                         GS);
            }
            break;
        default:
            throw std::runtime_error("Does not recognize Hund case.\n");
            break;
    }
    
    // Upper:
    assert(abs(lr.QuantumNumbers().Upper()[QN_M])<=lr.QuantumNumbers().Upper()[QN_J]);
    switch(lr.QuantumNumbers().Upper()[QN_Hund].toIndex())
    {
        case Hund_Case_A:
            // This follows Berdyugina and Solnaki
            Upper_E_part = lr.QuantumNumbers().Upper()[QN_M].toNumeric() * 
            gs_casea(lr.QuantumNumbers().Upper()[QN_Omega],
                     lr.QuantumNumbers().Upper()[QN_J],
                     lr.QuantumNumbers().Upper()[QN_S],
                     lr.QuantumNumbers().Upper()[QN_Lambda],
                     GS);
            break;
        case Hund_Case_B:
            // This follows Lenoir
            Upper_E_part = lr.QuantumNumbers().Upper()[QN_M].toNumeric() * 
            gs_caseb(lr.QuantumNumbers().Upper()[QN_N],
                     lr.QuantumNumbers().Upper()[QN_J],
                     lr.QuantumNumbers().Upper()[QN_S],
                     GS);
            break;
        default:
            throw std::runtime_error("Does not recognize Hund case.\n");
            break;
    }
    
    // convert from energy state to frequency and be done with it
    return H_mag * (Lower_E_part-Upper_E_part) / PLANCK_CONST * BOHR_MAGNETON;
    
}


void xsec_species_line_mixing_wrapper_with_zeeman(  Tensor4View propmat_clearsky, 
                                                    Tensor3View nlte_source,
                                                    ArrayOfTensor3& dpropmat_clearsky_dx,
                                                    ArrayOfMatrix&  dnlte_dx_source,
                                                    ArrayOfMatrix&  nlte_dsource_dx,
                                                    const ArrayOfArrayOfSpeciesTag& abs_species, 
                                                    const PropmatPartialsData& flag_partials,
                                                    const Index& abs_lineshape_ls, 
                                                    const Index& abs_lineshape_lsn, 
                                                    const Numeric& abs_lineshape_cutoff, 
                                                    const ArrayOfLineRecord& lr, 
                                                    const Vector&  Zeeman_DF,
                                                    const Vector&  planck_BT,
                                                    const Matrix&  dplanck_BT,
                                                    const SpeciesAuxData& isotopologue_ratios, 
                                                    const SpeciesAuxData& partition_functions,
                                                    const Matrix& abs_t_nlte, 
                                                    const Matrix& abs_vmrs, 
                                                    const Vector& abs_p,
                                                    const Vector& abs_t, 
                                                    const Vector& f_grid,
                                                    const Vector& rtp_mag,
                                                    const Vector& r_path_los,
                                                    const Numeric& lm_p_lim,
                                                    const Numeric& theta, 
                                                    const Numeric& eta, 
                                                    const Numeric& H_mag, 
                                                    const Index& DM, 
                                                    const Index& this_species,
                                                    const Verbosity& verbosity )
                                                
                                                
{
    
    const bool do_src  =  !nlte_source.empty();
    const Index nq = flag_partials.nelem();
    const Index nf = f_grid.nelem();
    const Numeric n = abs_vmrs(this_species, 0)*number_density( abs_p[0],abs_t[0]);
    const Numeric dn_dT = abs_vmrs(this_species, 0)*dnumber_density_dt( abs_p[0],abs_t[0]);
    
    // Setting up variables
    Matrix attenuation(nf, 1),phase(nf, 1),source(do_src?nf:0,do_src?1:0),
    attenuation_du(nf, 1),phase_du(nf, 1),attenuation_dv(nf, 1),phase_dv(nf, 1),attenuation_dw(nf, 1),phase_dw(nf, 1),
    source_du(do_src?nf:0,do_src?1:0),source_dv(do_src?nf:0,do_src?1:0),source_dw(do_src?nf:0,do_src?1:0);
    
        
    ArrayOfMatrix partial_attenuation(nq), partial_phase(nq), partial_source(do_src?nq:0);
    for(Index iq = 0; iq<nq; iq++)
    {
        if(flag_partials(iq)!=JQT_NOT_JQT)
        {
            partial_attenuation[iq].resize(nf, 1);
            partial_phase[iq].resize(nf, 1);
            if(do_src)
                partial_source[iq].resize(nf, 1);
        }
    }
    
    //Geometry for normal calculations
    Matrix  K_a(4,4), K_b(4,4);
    attenuation_matrix(K_a, theta*DEG2RAD, eta*DEG2RAD, DM);
    phase_matrix(K_b, theta*DEG2RAD, eta*DEG2RAD, DM);
    
    // FIXME: Have to perturb for magnetic u, v, and w since it is too complicated otherwise
    Numeric dB=0.0;
    if(nq)
        dB = flag_partials.Magnetic_Field_Perturbation();
    
    //Geometry for derivative calculations
    Matrix  dK_a_dtheta(4,4), dK_b_dtheta(4,4),dK_a_deta(4,4), dK_b_deta(4,4),
    dK_a_du(4,4),dK_a_dv(4,4),dK_a_dw(4,4),dK_b_du(4,4),dK_b_dv(4,4),dK_b_dw(4,4);
    
    // JACOBIAN SETUP START
    if(flag_partials.do_zeeman_eta())
    {
        dattenuation_matrix_deta(dK_a_deta, theta*DEG2RAD, eta*DEG2RAD, DM);
        dphase_matrix_deta(dK_b_deta, theta*DEG2RAD, eta*DEG2RAD, DM);
    }
    
    if(flag_partials.do_zeeman_theta())
    {
        dattenuation_matrix_dtheta(dK_a_dtheta, theta*DEG2RAD, eta*DEG2RAD, DM);
        dphase_matrix_dtheta(dK_b_dtheta, theta*DEG2RAD, eta*DEG2RAD, DM);
    }
    
    Numeric H_dummy, deta, dtheta;
    Vector dmag = rtp_mag;
    
    if(flag_partials.do_zeeman_u())
    {
        dmag[0]+=dB;
        set_magnetic_parameters(H_dummy,deta,dtheta,0,0,0,0,dmag,r_path_los);
        attenuation_matrix(dK_a_du, dtheta*DEG2RAD, deta*DEG2RAD, DM);
        phase_matrix(dK_b_du, dtheta*DEG2RAD, deta*DEG2RAD, DM);
        dmag[0]-=dB;
    }
    
    if(flag_partials.do_zeeman_v())
    {
        dmag[1]+=dB;
        set_magnetic_parameters(H_dummy,deta,dtheta,0,0,0,0,dmag,r_path_los);
        attenuation_matrix(dK_a_dv, dtheta*DEG2RAD, deta*DEG2RAD, DM);
        phase_matrix(dK_b_dv, dtheta*DEG2RAD, deta*DEG2RAD, DM);
        dmag[1]-=dB;
    }
    
    if(flag_partials.do_zeeman_w())
    {
        dmag[2]+=dB;
        set_magnetic_parameters(H_dummy,deta,dtheta,0,0,0,0,dmag,r_path_los);
        attenuation_matrix(dK_a_dw, dtheta*DEG2RAD, deta*DEG2RAD, DM);
        phase_matrix(dK_b_dw, dtheta*DEG2RAD, deta*DEG2RAD, DM);
        dmag[2]-=dB;
    }
    // JACOBIAN SETUP END
    
    for(Index iv=0;iv<nf;iv++)
    {
        attenuation(iv,0)=0.;phase(iv,0)=0.;
        if(flag_partials.do_zeeman_u())
        {
            attenuation_du(iv,0)=0.;
            phase_du(iv,0)=0.;
        }
        if(flag_partials.do_zeeman_v())
        {
            attenuation_dv(iv,0)=0.;
            phase_dv(iv,0)=0.;
        }
        if(flag_partials.do_zeeman_w())
        {
            attenuation_dw(iv,0)=0.;
            phase_dw(iv,0)=0.;
        }
        if(do_src)
            source(iv,0)=0.;
        for(Index iq = 0; iq < nq; iq++)
        {
            if(flag_partials(iq)!=JQT_NOT_JQT)
            {
                partial_attenuation[iq](iv,0)=0.;
                partial_phase[iq](iv,0)=0.;
                if(do_src)
                    partial_source[iq](iv,0)=0.;
            }
        }
    }

    xsec_species_line_mixing_wrapper(   attenuation,         source,         phase, 
                                        partial_attenuation, partial_source, partial_phase, flag_partials,
                                        f_grid, abs_p, abs_t, abs_t_nlte, abs_vmrs, abs_species, 
                                        this_species, lr, Zeeman_DF, H_mag,
                                        abs_lineshape_ls,abs_lineshape_lsn,lm_p_lim,abs_lineshape_cutoff,
                                        isotopologue_ratios, partition_functions, verbosity ); // Now in cross section
    if(flag_partials.do_zeeman_u())
    {
        dmag[0]+=dB;
        Vector Zeeman_DF_dv = Zeeman_DF;
        Zeeman_DF_dv*=sqrt(dmag*dmag)/H_mag;
        
        xsec_species_line_mixing_wrapper(         attenuation_du,         source_du,         phase_du, 
                                                  partial_attenuation, partial_source, partial_phase, PropmatPartialsData(ArrayOfRetrievalQuantity(0)),
                                                  f_grid, abs_p, abs_t, abs_t_nlte, abs_vmrs, abs_species, 
                                                  this_species, lr, Zeeman_DF_dv, H_mag+flag_partials.Magnetic_Field_Perturbation(),
                                                  abs_lineshape_ls,abs_lineshape_lsn,lm_p_lim,abs_lineshape_cutoff,
                                                  isotopologue_ratios, partition_functions, verbosity ); // Now in cross section
        dmag[0]-=dB;
    }
    if(flag_partials.do_zeeman_v())
    {
        dmag[1]+=dB;
        Vector Zeeman_DF_dv = Zeeman_DF;
        Zeeman_DF_dv*=sqrt(dmag*dmag)/H_mag;
        
        xsec_species_line_mixing_wrapper(         attenuation_dv,         source_dv,         phase_dv, 
                                                  partial_attenuation, partial_source, partial_phase, PropmatPartialsData(ArrayOfRetrievalQuantity(0)),
                                                  f_grid, abs_p, abs_t, abs_t_nlte, abs_vmrs, abs_species, 
                                                  this_species, lr, Zeeman_DF_dv, H_mag+flag_partials.Magnetic_Field_Perturbation(),
                                                  abs_lineshape_ls,abs_lineshape_lsn,lm_p_lim,abs_lineshape_cutoff,
                                                  isotopologue_ratios, partition_functions, verbosity ); // Now in cross section
        dmag[1]-=dB;
    }
    if(flag_partials.do_zeeman_w())
    {
        dmag[2]+=dB;
        Vector Zeeman_DF_dv = Zeeman_DF;
        Zeeman_DF_dv*=sqrt(dmag*dmag)/H_mag;
        
        xsec_species_line_mixing_wrapper(         attenuation_dw,         source_dw,         phase_dw, 
                                                  partial_attenuation, partial_source, partial_phase, PropmatPartialsData(ArrayOfRetrievalQuantity(0)),
                                                  f_grid, abs_p, abs_t, abs_t_nlte, abs_vmrs, abs_species, 
                                                  this_species, lr, Zeeman_DF_dv, H_mag+flag_partials.Magnetic_Field_Perturbation(),
                                                  abs_lineshape_ls,abs_lineshape_lsn,lm_p_lim,abs_lineshape_cutoff,
                                                  isotopologue_ratios, partition_functions, verbosity ); // Now in cross section
        dmag[2]-=dB;
    }

    //Add things to the total propagation
    for(Index iv=0;iv<nf;iv++)
    {
        for(Index is1 = 0;is1<4;is1++)
        {
            if(do_src)
                nlte_source(this_species,iv,is1) += source(iv,0)*K_a(is1,0)*planck_BT[iv]*n;
            for(Index is2 = 0;is2<4;is2++)
            {
                propmat_clearsky(this_species,iv,is1,is2) += (attenuation(iv,0)*K_a(is1,is2)+2.0*phase(iv,0)*K_b(is1,is2))*n;
                
                for(Index iq = 0; iq<nq; iq++)
                {
                    if(flag_partials(iq)==JQT_magnetic_u)
                    {
                        dpropmat_clearsky_dx[iq](iv,is1,is2) += 
                        (
                            attenuation_du(iv,0)*dK_a_du(is1,is2)+2.0*phase_du(iv,0)*dK_b_du(is1,is2) -
                            (attenuation(iv,0)*K_a(is1,is2)+2.0*phase(iv,0)*K_b(is1,is2))
                        )*n/dB;
                        if(do_src&&is2==0)
                            dnlte_dx_source[iq](iv,is1) += (source_du(iv,0)*dK_a_du(is1,is2)-source(iv,0)*K_a(is1,is2))*n*planck_BT[iv];
                    }
                    else if(flag_partials(iq)==JQT_magnetic_v)
                    {
                        dpropmat_clearsky_dx[iq](iv,is1,is2) += 
                        (
                            attenuation_dv(iv,0)*dK_a_dv(is1,is2)+2.0*phase_dv(iv,0)*dK_b_dv(is1,is2) -
                            (attenuation(iv,0)*K_a(is1,is2)+2.0*phase(iv,0)*K_b(is1,is2))
                        )*n/dB;
                        if(do_src&&is2==0)
                            dnlte_dx_source[iq](iv,is1) += (source_dv(iv,0)*dK_a_dv(is1,is2)-source(iv,0)*K_a(is1,is2))*n*planck_BT[iv];
                    }
                    else if(flag_partials(iq)==JQT_magnetic_w)
                    {
                        dpropmat_clearsky_dx[iq](iv,is1,is2) += 
                        (
                            attenuation_dw(iv,0)*dK_a_dw(is1,is2)+2.0*phase_dw(iv,0)*dK_b_dw(is1,is2) -
                            (attenuation(iv,0)*K_a(is1,is2)+2.0*phase(iv,0)*K_b(is1,is2))
                        )*n/dB;
                        if(do_src&&is2==0)
                            dnlte_dx_source[iq](iv,is1) += (source_dw(iv,0)*dK_a_dw(is1,is2)-source(iv,0)*K_a(is1,is2))*n*planck_BT[iv];
                    }
                    else if(flag_partials(iq)==JQT_magnetic_theta)
                    {
                        dpropmat_clearsky_dx[iq](iv,is1,is2) += (attenuation(iv,0)*dK_a_dtheta(is1,is2)+2.0*phase(iv,0)*dK_b_dtheta(is1,is2))*n;
                        if(do_src&&is2==0)
                            dnlte_dx_source[iq](iv,is1) += source(iv,0)*n*dK_a_dtheta(is1,is2)*planck_BT[iv];
                    }
                    else if(flag_partials(iq)==JQT_magnetic_eta)
                    {
                        dpropmat_clearsky_dx[iq](iv,is1,is2) += (attenuation(iv,0)*dK_a_deta(is1,is2)+2.0*phase(iv,0)*dK_b_deta(is1,is2))*n;
                        if(do_src&&is2==0)
                            dnlte_dx_source[iq](iv,is1) += source(iv,0)*n*dK_a_deta(is1,is2)*planck_BT[iv];
                    }
                    else if(flag_partials(iq)==JQT_temperature&&do_src)
                    {
                        dpropmat_clearsky_dx[iq](iv,is1,is2) += (partial_attenuation[iq](iv,0)* n + attenuation(iv,0) * dn_dT) * K_a(is1,is2)
                                                              +   2.0*(partial_phase[iq](iv,0) * n  + phase(iv,0)      * dn_dT) * K_b(is1,is2);
                        if(is2==0)
                        {
                            dnlte_dx_source[iq](iv,is1) += (partial_source[iq](iv,0)*n + source(iv,0)*dn_dT)*K_a(is1,is2)*planck_BT[iv];
                            nlte_dsource_dx[iq](iv,is1) += source(iv,0)*n*K_a(is1,is2)*dplanck_BT(0,iv);//zeroth index is the temperature derivative
                        }
                    }
                    else if(flag_partials(iq)==JQT_frequency&&do_src)
                    {
                        dpropmat_clearsky_dx[iq](iv,is1,is2) += (partial_attenuation[iq](iv,0)*K_a(is1,is2)+2.0*partial_phase[iq](iv,0)*K_b(is1,is2))*n;
                        if(is2==0)
                        {
                            dnlte_dx_source[iq](iv,is1) += partial_source[iq](iv,0)*n*K_a(is1,is2)*planck_BT[iv];
                            nlte_dsource_dx[iq](iv,is1) += source(iv,0)*n*K_a(is1,is2)*dplanck_BT(1,iv);//first index is the frequency derivative
                        }
                    }
                    else if(flag_partials(iq)==JQT_temperature)
                    {   
                        dpropmat_clearsky_dx[iq](iv,is1,is2) += (partial_attenuation[iq](iv,0)* n + attenuation(iv,0) * dn_dT) * K_a(is1,is2)
                                                              +   2.0*(partial_phase[iq](iv,0) * n  + phase(iv,0)      * dn_dT) * K_b(is1,is2);
                    }
                    else if(flag_partials(iq)==JQT_VMR)
                    {   
                        //WARNING:  if VMR starts being used for p_partial derivatives, then this fails...
                        dpropmat_clearsky_dx[iq](iv,is1,is2) += 
                        attenuation(iv,0) * n/abs_vmrs(this_species, 0) * K_a(is1,is2)
                        + 2.0*phase(iv,0) * n/abs_vmrs(this_species, 0) * K_b(is1,is2);
                        if(do_src&&is2==0)
                            dnlte_dx_source[iq](iv,is1) += 
                            partial_source[iq](iv,0)*n*K_a(is1,is2)*planck_BT[iv]/abs_vmrs(this_species, 0);
                    }
                    else if(flag_partials(iq)!=JQT_NOT_JQT)
                    {
                        dpropmat_clearsky_dx[iq](iv,is1,is2) += (partial_attenuation[iq](iv,0)*K_a(is1,is2)+2.0*partial_phase[iq](iv,0)*K_b(is1,is2))*n;
                        if(do_src&&is2==0)
                            dnlte_dx_source[iq](iv,is1) += 
                            partial_source[iq](iv,0)*n*K_a(is1,is2)*planck_BT[iv];
                    }
                }
            }
        }
    }
}


void set_magnetic_parameters(Numeric& H_mag,
                             Numeric& eta,
                             Numeric& theta,
                             const Index manual_zeeman_tag,
                             const Numeric& manual_zeeman_eta,
                             const Numeric& manual_zeeman_theta,
                             const Numeric& manual_zeeman_magnetic_field_strength,
                             ConstVectorView rtp_mag,
                             ConstVectorView r_path_los)
{
//Get the magnitude of the magnetic field and store a local unit Vector for simplified angle calculations.
  H_mag = manual_zeeman_tag != 0?manual_zeeman_magnetic_field_strength:sqrt( rtp_mag * rtp_mag );

  if(manual_zeeman_tag!=0)
  { // Leaving it up to the user to manually tag the angles for simplified magnetic fields.
    eta   = manual_zeeman_eta;
    theta = manual_zeeman_theta;
  }
  else if(H_mag==0.0)
  {
      eta = 0.0;
      theta = 0.0;
  }
  else
  { 
    const Numeric 
    aa=DEG2RAD*r_path_los[1], 
    za=DEG2RAD*r_path_los[0], 
    Bu = rtp_mag[0], 
    Bv = rtp_mag[1], 
    Bw = rtp_mag[2],
    cosaa=cos(aa),
    cosza=cos(za),
    sinaa=sin(aa),
    sinza=sin(za),
    cosaa2 = cosaa*cosaa,
    //cosza2 = cosza*cosza,
    sinza2 = sinza*sinza,
    sinaa2 = sinaa*sinaa,
    Bu2 = Bu*Bu,
    Bv2 = Bv*Bv,
    Bw2 = Bw*Bw,
    H2  = H_mag*H_mag,
    term1=(Bu*(cosaa2*sinza2 - 1.) + Bw*cosaa*cosza*sinza + Bv*cosaa*sinaa*sinza2),
    term2=(Bv*(sinaa2*sinza2 - 1.) + Bw*sinaa*cosza*sinza + Bu*cosaa*sinaa*sinza2),
    term3=( - Bw*sinza2            + Bu*cosaa*cosza*sinza + Bv*sinaa*cosza*sinza),
    eta_test=(PI - acos(((Bu*cosaa*cosza - Bw*sinza + Bv*sinaa*cosza)*sqrt(term1*term1 + term2*term2 +term3*term3))/(H2)))*RAD2DEG,
    x1 = (Bv*cosaa - Bu*sinaa),
    x2 = -((Bu2*cosaa-Bv2*cosaa+2.0*Bu*Bv*sinaa)*cosaa*sinza2 - Bu2 - Bw2 + 2.0*Bu*Bw*cosaa*cosza*sinza + (Bw2*cosza - Bv2*cosza+ 2.0*Bv*Bw*sinaa*sinza)*cosza),
    fx2=sqrt(x2),
    x3 = 1.0/H2,
    x=x1*fx2*x3;
    
    theta = acos((Bw*cosza + Bu*cosaa*sinza + Bv*sinaa*sinza)/H_mag) * RAD2DEG;
    
    if ((abs(x)-(Numeric)1.0)>0.0) // Numerical drifts can cause this...
      eta = 0.0;
    else 
    {
      eta=acos(x)*RAD2DEG;
      
      if(eta_test>90.0) eta*=-1.0;
    }
  }
}


void set_magnetic_parameters_derivative(
    Numeric& dH_du,
    Numeric& dH_dv,
    Numeric& dH_dw,
    Numeric& deta_du,
    Numeric& deta_dv,
    Numeric& deta_dw,
    Numeric& dtheta_du,
    Numeric& dtheta_dv,
    Numeric& dtheta_dw,
    ConstVectorView rtp_mag,
    ConstVectorView r_path_los)
{
    const Numeric& Bu = rtp_mag[0],Bv = rtp_mag[1],Bw = rtp_mag[2];
    
    const Numeric
    Bu2=Bu*Bu, Bv2 = Bv*Bv, Bw2 = Bw*Bw, 
    H_mag = sqrt(Bu2+Bv2+Bw2), H2 = H_mag*H_mag,
    aa=DEG2RAD*r_path_los[1], za=DEG2RAD*r_path_los[0], 
    cosaa=cos(aa), cosza=cos(za), sinaa=sin(aa), sinza=sin(za),
    sinza2=sinza*sinza,cosza2=cosza*cosza,cosaa2=cosaa*cosaa,sinaa2=sinaa*sinaa,
    a=(Bw*cos(za) + Bu*cos(aa)*sin(za) + Bv*sin(aa)*sin(za)),
    b=-cosaa*sinza*Bv2 + Bu*sinaa*sinza*Bv - cosaa*sinza*Bw2 + Bu*cosza*Bw,
    term1=(Bu*(cosaa2*sinza2 - 1.) + Bw*cosaa*cosza*sinza + Bv*cosaa*sinaa*sinza2),
    term2=(Bv*(sinaa2*sinza2 - 1.) + Bw*sinaa*cosza*sinza + Bu*cosaa*sinaa*sinza2),
    term3=(Bw*(cosza2 - 1.) + Bu*cosaa*cosza*sinza + Bv*sinaa*cosza*sinza),
    eta_test=(PI - acos(((Bu*cosaa*cosza - Bw*sinza + 
    Bv*sinaa*cosza)*sqrt(term1*term1/(H_mag*H_mag) + term2*term2/(H_mag*H_mag) +
    term3*term3/(H_mag*H_mag)))/(H_mag)))*RAD2DEG; 
    
    const Numeric
    x1 = (Bv*cosaa - Bu*sinaa),
    x2 = -((Bu2*cosaa-Bv2*cosaa+2.0*Bu*Bv*sinaa)*cosaa*sinza2
    - Bu2 - Bw2 + 2.0*Bu*Bw*cosaa*cosza*sinza +
    (Bw2*cosza - Bv2*cosza+ 2.0*Bv*Bw*sinaa*sinza)*cosza),
    fx2=sqrt(x2),
    x3 = 1.0/H2,
    x=x1*fx2*x3, // eta = acos(x) * RAD2DEG;
    dx1_dBu = -sinaa,
    dx1_dBv = cosaa,
    dx2_dBu = 2.0*Bu - cosaa*sinza2*(2.0*Bu*cosaa + 2.0*Bv*sinaa) - 2.0*Bw*cosaa*cosza*sinza,
    dx2_dBv = cosza*(2.0*Bv*cosza - 2.0*Bw*sinaa*sinza) + cosaa*sinza2*(2.0*Bv*cosaa - 2.0*Bu*sinaa),
    dx2_dBw = 2.0*Bw - cosza*(2.0*Bw*cosza + 2.0*Bv*sinaa*sinza) - 2.0*Bu*cosaa*cosza*sinza,
    dx3_dBu = -2.0*Bu*x3*x3,
    dx3_dBv = -2.0*Bv*x3*x3,
    dx3_dBw = -2.0*Bw*x3*x3,
    c1 = fx2*x3, c2 = x1*0.5*x3/fx2,c3=x1*fx2,
    d_acos  = -1/sqrt(1-x*x) * (eta_test>90.0?-RAD2DEG:RAD2DEG); //RAD2DEG is here for convenience...
    
    dH_du = Bu/H_mag;
    dH_dv = Bv/H_mag;
    dH_dw = Bw/H_mag;
    
    dtheta_du = b/sqrt(H_mag*H_mag-a*a)*H_mag*H_mag * RAD2DEG;
    dtheta_dv = dtheta_du * (- sinaa*sinza*Bu2 + Bv*cosaa*sinza*Bu - sinaa*sinza*Bw2 + Bv*cosza*Bw)/b;
    dtheta_dw = dtheta_du * (-(cosza*Bu2 - Bw*cosaa*sinza*Bu + cosza*Bv2 - Bw*sinaa*sinza*Bv)/b);
    
    deta_du = d_acos * (dx1_dBu*c1 + dx2_dBu*c2 +  dx3_dBu*c3);
    deta_dv = d_acos * (dx1_dBv*c1 + dx2_dBv*c2 +  dx3_dBv*c3);
    deta_dw = d_acos * (/* empty */  dx2_dBw*c2 +  dx3_dBw*c3);
}




void set_quantumnumbers( LineRecord& this_LR,
                         const Rational& hund,
                         const SpeciesAuxData& isotopologue_quantum)
{
  Rational Lambda, S;
  
  S = Rational((Index) (2*isotopologue_quantum.getParam(this_LR.Species(), 
                                                        this_LR.Isotopologue())[0].data[AuxIndex_S]),2);
  S.Simplify();
  
  Lambda = (Index) isotopologue_quantum.getParam(this_LR.Species(), 
                                         this_LR.Isotopologue())[0].data[AuxIndex_Lambda];
  
  this_LR.SetQuantumNumberLower(QN_S, S);
  this_LR.SetQuantumNumberUpper(QN_S, S);
  
  this_LR.SetQuantumNumberLower(QN_Lambda, Lambda);
  this_LR.SetQuantumNumberUpper(QN_Lambda, Lambda);
  
  // Flag quantum number
  this_LR.SetQuantumNumberLower(QN_Hund, hund);
  this_LR.SetQuantumNumberUpper(QN_Hund, hund);
}

void alter_linerecord( LineRecord& new_LR,
                       Numeric& Test_RS,
                       const Numeric& old_LS,
                       const Rational& J_up,
                       const Rational& J_lo,
                       const Rational& M_up,
                       const Rational& M_lo)
{
    // Find the relative strength
    const Numeric RS = relative_strength(M_lo, J_lo, (J_up-J_lo).toIndex(), (M_up-M_lo).toIndex());
    
    // Setup a test that the relative strength is reasonable
    Test_RS += RS;
    new_LR.setI0( old_LS * RS );
    
    // Set quantum numbers
    new_LR.SetQuantumNumberLower(QN_M, M_lo);
    new_LR.SetQuantumNumberUpper(QN_M, M_up);
}



void create_Zeeman_linerecordarrays(
        ArrayOfArrayOfLineRecord& aoaol,
        const ArrayOfArrayOfSpeciesTag& abs_species,
        const ArrayOfArrayOfLineRecord& abs_lines_per_species,
        const SpeciesAuxData& isotopologue_quantum,
        const Verbosity& verbosity)
{
    CREATE_OUT3;
    // Note that this function assumes that all tests that are not line specifice are done elsewhere
    
    using global_data::species_data;
    
    // This margin is for relative strength and can perhaps be lowered by returning RS as Rational?
    const Numeric margin    = 1e-4; 

      // For all species
      for(Index II = 0; II<abs_species.nelem(); II++)
      {
          // If the species isn't Zeeman, look at the next species
          if(!is_zeeman(abs_species[II])) continue;
          
          // If there are no lines give up on this species
          if(!abs_lines_per_species[II].nelem()) continue;
          
          const Index nis = isotopologue_quantum.nisotopologues(abs_lines_per_species[II][0].Species());
          
          const SpeciesRecord& sr = species_data[abs_lines_per_species[II][0].Species()];
          
          for(Index is=0;is<nis;is++)
          {
              const IsotopologueRecord& ir = sr.Isotopologue()[is];
              
              if(ir.isContinuum())
                  continue;
              
              const ArrayOfGriddedField1& aogf1 =
              isotopologue_quantum.getParam(abs_lines_per_species[II][0].Species(),is);
              if(!aogf1.nelem())
              {
                  std::ostringstream os;
                  os<<"No data in isotopologue_quantum for species "<< sr.Name();
              }
              
              const GriddedField1& gf1 = aogf1[0];
              if(gf1.data.nelem()!=AuxIndex_TotalCount)
              {
                  std::ostringstream os;
                  os << "There are undefined isotopologues in *isotopologue_quantum* for species " <<sr.Name() <<"\n";
                  os << "All isotopologues must be defined for a species.\n";
                  throw std::runtime_error(os.str());
              }
          }
                  
          // One line record array per type of polarizer is created
          aoaol.push_back(ArrayOfLineRecord()); // First is negative
          aoaol.push_back(ArrayOfLineRecord()); // Second is 0
          aoaol.push_back(ArrayOfLineRecord()); // Third is positive

          ArrayOfLineRecord& temp_abs_lines_sm = aoaol[aoaol.nelem()-3]; // sigma minus
          ArrayOfLineRecord& temp_abs_lines_pi = aoaol[aoaol.nelem()-2]; // pi
          ArrayOfLineRecord& temp_abs_lines_sp = aoaol[aoaol.nelem()-1]; // sigma plus

          temp_abs_lines_sm.resize(0);
          temp_abs_lines_sp.resize(0);
          temp_abs_lines_pi.resize(0);

          temp_abs_lines_sm.reserve(25000);
          temp_abs_lines_sp.reserve(25000);
          temp_abs_lines_pi.reserve(25000);

          // Else loop over all the lines in the species.
          for (Index ii = 0; ii< abs_lines_per_species[II].nelem(); ii++)
          {
              // local LineRecord
              LineRecord temp_LR = abs_lines_per_species[II][ii];
              const Numeric this_linestrength = temp_LR.I0();
              Rational hund;
                
              set_hund_case(hund, isotopologue_quantum, temp_LR);
              Numeric RS_sum     = 0; //Sum relative strength (which ought be close to one by the end)
              // Only look at lines which have no change in the main rotational number
              
              bool test=true;
              
              // Test that hund cases are properly defined
              if(hund==0)
              {
                  if(    temp_LR.QuantumNumbers().Upper()[QN_Omega].isUndefined() 
                      || temp_LR.QuantumNumbers().Lower()[QN_Omega].isUndefined())
                      test=false;
              }
              else if(hund==1)
              {
                  if(    temp_LR.QuantumNumbers().Upper()[QN_N].isUndefined() 
                      || temp_LR.QuantumNumbers().Lower()[QN_N].isUndefined())
                      test=false;
              }
              else //Case a is 0, case b is 1
              {
                  std::ostringstream os;
                  os << "There are undefined Hund cases for\n" << temp_LR;
                  throw std::runtime_error(os.str());
              }
              
              if(!test)
              {
                  std::ostringstream os;
                  os<<"Need to define the main quantum numbers for\n"<<
                  temp_LR;
                  throw std::runtime_error(os.str());
              }
              
              // Quantum numbers
              set_quantumnumbers(temp_LR, hund, isotopologue_quantum);
              const Rational J_up = temp_LR.QuantumNumbers().Upper()[QN_J], 
              J_lo = temp_LR.QuantumNumbers().Lower()[QN_J];
            
              test = J_up.isUndefined()||J_lo.isUndefined();
              if(test)
              {
                  std::ostringstream os;
                  os<<"Need to define J for\n"<<
                  temp_LR;
                  throw std::runtime_error(os.str());
              }
              
              test = J_lo>J_up;
              const Rational  J    = (test?J_up:J_lo);
              
              for ( Rational M = -J; M<=J; M++ )
              {
                  /*
                   *                              Note that:
                   *                              sp := sigma plus,  which means DM =  1
                   *                              sm := sigma minus, which means DM = -1
                   *                              pi := planar,      which means DM =  0
                   */
                  if(test)
                  {
                      alter_linerecord( temp_LR, RS_sum, this_linestrength, J_up, J_lo, M, M-1  );
                      temp_abs_lines_sm.push_back(temp_LR);
                      
                      alter_linerecord( temp_LR, RS_sum, this_linestrength, J_up, J_lo, M, M  );
                      temp_abs_lines_pi.push_back(temp_LR);
                      
                      alter_linerecord( temp_LR, RS_sum, this_linestrength, J_up, J_lo, M, M+1  );
                      temp_abs_lines_sp.push_back(temp_LR);
                  }
                  else 
                  {
                      alter_linerecord( temp_LR, RS_sum, this_linestrength, J_up, J_lo, M+1, M  );
                      temp_abs_lines_sm.push_back(temp_LR);
                      
                      alter_linerecord( temp_LR, RS_sum, this_linestrength, J_up, J_lo, M  , M  );
                      temp_abs_lines_pi.push_back(temp_LR);
                      
                      alter_linerecord( temp_LR, RS_sum, this_linestrength, J_up, J_lo, M-1, M  );
                      temp_abs_lines_sp.push_back(temp_LR);
                  }
              }
              
              if (abs(RS_sum-1.)>margin) //Reasonable confidence?
              {
                  std::ostringstream os;
                  os << "The sum of relative strengths is not close to one. This is severly problematic and "
                  "you should look into why this happens.\nIt is currently " << RS_sum 
                  << " with J_lo: "<<J_lo<<", J_up: "<<J_up<<" for line: "<<
                  temp_LR <<"\n";
                  throw std::runtime_error(os.str());
              }
          }
      }
}



void set_hund_case(Rational& hund, const SpeciesAuxData& isotopologue_quantum,const LineRecord& temp_LR)
{
  hund = (Index) isotopologue_quantum.getParam(temp_LR.Species(), 
                                               temp_LR.Isotopologue())[0].data[AuxIndex_Hund];
}

void set_GS(Numeric& GS, const SpeciesAuxData& isotopologue_quantum,const LineRecord& temp_LR)
{
    GS = isotopologue_quantum.getParam(temp_LR.Species(), 
                                       temp_LR.Isotopologue())[0].data[AuxIndex_GS];
}


void set_strength_partial_derivative_matrix(ArrayOfMatrix& A, ArrayOfMatrix& B, const ArrayOfRetrievalQuantity flag_partials, const Vector& f_grid)
{
    for(Index ii=0; ii<flag_partials.nelem(); ii++)
    {
        if(flag_partials[ii].MainTag() == "Zeeman" &&  flag_partials[ii].Subtag() == "Magnetic Strength" && flag_partials[ii].SubSubtag() == "From Propagation")
        {
            A[ii].resize(f_grid.nelem(),1);
            B[ii].resize(f_grid.nelem(),1);
            A[ii]=0;
            B[ii]=0;
            return;
        }
    }
}

Index part_mag_strength(const ArrayOfRetrievalQuantity& flag_partials)
{
    for(Index ii=0; ii<flag_partials.nelem(); ii++)
        if(flag_partials[ii].MainTag() == "Zeeman" &&  flag_partials[ii].Subtag() == "Magnetic Strength" && flag_partials[ii].SubSubtag() == "From Propagation")
            return ii;
    return -1;
}

Index part_mag_theta(const ArrayOfRetrievalQuantity& flag_partials)
{
    for(Index ii=0; ii<flag_partials.nelem(); ii++)
        if(flag_partials[ii].MainTag() == "Zeeman" &&  flag_partials[ii].Subtag() == "Magnetic Theta" && flag_partials[ii].SubSubtag() == "From Propagation")
            return ii;
        return -1;
}