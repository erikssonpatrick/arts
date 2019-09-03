/* Copyright 2018, Richard Larsson
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/**
 * @file linemixing.h
 * @author Richard Larsson
 * @date 2018-08-06
 * 
 * @brief Line mixing calculation implementation
 * 
 * This file contains only experimental code to test the F90-routines.
 * It might evolve to replace it at some point but that is beyond present ability
 */

#ifndef linemixing_h
#define linemixing_h

#include "absorption.h"
#include "complex.h"
#include "constants.h"
#include "linerecord.h"
#include "rational.h"

/** Compute the rotational energy of a Hund b case molecule
 * 
 * @param J Rotational angular momentum w/ spin
 * @param N Rotational angular momentum w/0 spin
 * @param J2 Rotational angular momentum w/ spin for other level
 * @param B Main rotation constant
 * @param D Main rotation constant squared
 * @param H Main rotation constant cubic
 * @param gamma Second rotation constant offset
 * @param gamma_D Second rotation constant
 * @param gamma_H Second rotation constant squared
 * @param lambda Third energy constant offset
 * @param lambda_D Third energy constant 
 * @param lambda_H Third energy constant squared
 * @return Numeric Energy of level
 */
template <class T>
inline Numeric rotational_energy_hund_b_molecule(T J,
                                                 T N,
                                                 T J2,
                                                 Numeric B,
                                                 Numeric D,
                                                 Numeric H,
                                                 Numeric gamma,
                                                 Numeric gamma_D,
                                                 Numeric gamma_H,
                                                 Numeric lambda,
                                                 Numeric lambda_D,
                                                 Numeric lambda_H) {
  if (N == J and N == J2) {
    const Numeric JJ1 = J * (J + 1);
    const Numeric JJ2 = JJ1 * JJ1;
    const Numeric JJ3 = JJ1 * JJ1 * JJ1;
    return +(B * JJ1 - D * JJ2 + H * JJ3) -
           (gamma + gamma_D * JJ1 + gamma_H * JJ2) +
           2 / 3 * (lambda + lambda_D * JJ1 + lambda_H * JJ2);
  } else if (N == (J - 1) and N == (J2 - 1)) {
    const Numeric JJ1 = J * (J - 1);
    const Numeric JJ2 = JJ1 * JJ1;
    const Numeric JJ3 = JJ1 * JJ1 * JJ1;
    return +(B * JJ1 - D * JJ2 + H * JJ3) +
           (gamma + gamma_D * JJ1 + gamma_H * JJ2) * (J - 1) +
           (lambda + lambda_D * JJ1 + lambda_H * JJ2) *
               (2 / 3 - 2 * J / (2 * J + 1));
  } else if (N == (J + 1) and N == (J2 + 1)) {
    const Numeric JJ1 = (J + 2) * (J + 1);
    const Numeric JJ2 = JJ1 * JJ1;
    const Numeric JJ3 = JJ1 * JJ1 * JJ1;
    return +(B * JJ1 - D * JJ2 + H * JJ3) -
           (gamma + gamma_D * JJ1 + gamma_H * JJ2) * (J + 2) +
           (lambda + lambda_D * JJ1 + lambda_H * JJ2) *
               (2 / 3 - 2 * (J + 1) / (2 * J + 1));

  } else if ((N == (J + 1) and N == (J2 - 1)) or
             (N == (J - 1) and N == (J2 + 1))) {
    const Numeric JJ1 = J * J + J + 1;
    const Numeric JJ2 = JJ1 * JJ1;
    return (lambda + lambda_D * JJ1 + lambda_H * JJ2) * 2 *
           std::sqrt(J * J + J) / (2 * J + 1);
  } else {
    return 0;
  }
}

/** Molecular constant */
namespace Molecule {

/**  Name of molecules */
enum class Name { O2_66, CO2_626 };

/** O2-66 constants */
namespace O2_66 {
constexpr Numeric g_S = 2.002084;
constexpr Numeric ge_l = 2.77e-3;
constexpr Numeric g_r = -1.16e-4;

constexpr Numeric B = 43100.44276e6;
constexpr Numeric D = 145.1271e3;
constexpr Numeric H = 49e-3;

constexpr Numeric lambda = 59501.3438e6;
constexpr Numeric lambda_D = 58.3680e3;
constexpr Numeric lambda_H = 290.8e-3;

constexpr Numeric gamma = -252.58634e6;
constexpr Numeric gamma_D = -243.42;
constexpr Numeric gamma_H = -1.46e-3;

constexpr Numeric mass = 31.989830;

/** Hamiltonian frequency
 * 
 * @param J Rotational constant
 * @param dcol +1, 0, -1
 * @param drow +1, 0, -1
 * @return Numeric Frequency
 */
template <class T>
Numeric hamiltonian_freq(T J, int dcol = 0, int drow = 0) {
  /*
      Matrix:
        (-1,-1), (0,-1), (1,-1)
        (-1, 0), (0, 0), (1, 0)
        (-1, 1), (0, 1), (1, 1)
      */
  return rotational_energy_hund_b_molecule(J + dcol,
                                           J,
                                           J + drow,
                                           B,
                                           D,
                                           H,
                                           gamma,
                                           gamma_D,
                                           gamma_H,
                                           lambda,
                                           lambda_D,
                                           lambda_H);
}
};  // namespace O2_66

/** CO-626 constants */
namespace CO2_626 {
constexpr Numeric B = Conversion::kaycm2freq(0.39021);
template <class T>

/** Hamiltonian frequency
 * 
 * @param J Rotational constant
 * @return constexpr Numeric Frequency
 */
constexpr Numeric hamiltonian_freq(T J) {
  return B * J * (J + 1);
}
}  // namespace CO2_626
};  // namespace Molecule

/** Adiabatic factor computations */
class AdiabaticFactor {
 public:

  /** Methods to compute factor */
  enum class Type { Hartmann };

  /** Description of data in Hartmann Type */
  enum class HartmannPos : Index { dc, SIZE };

  /** Construct a new Adiabatic Factor object
   * 
   * Checks that the type and the input agrees
   * 
   * @param v data of how to compute the adiabatic factor
   * @param t type of adiabatic factor
   */
  AdiabaticFactor(const Vector& v, Type t) : mtype(t), mdata(v) {
    bool error = false;
    switch (mtype) {
      case Type::Hartmann:
        if (Index(HartmannPos::SIZE) not_eq mdata.nelem()) error = true;
        break;
    }

    if (error)
      throw std::runtime_error(
          "Bad initializaton of BasisRate, type and size disagree...");
  }

  /** Hartmann AF
   * 
   * @param L Rotational angular momentum
   * @param B0 Rotational constant
   * @param T Temperature
   * @param main_mass Mass of main molecule
   * @param collider_mass Mass of colliding molecule
   * @return Numeric Adiabatic factor
   */
  Numeric mol_X(const Numeric& L,
                const Numeric& B0,
                const Numeric& T,
                const Numeric& main_mass,
                const Numeric& collider_mass) const;

  /** Get AF
   * 
   * @param L Rotational angular momentum
   * @param B0 Rotational constant
   * @param T Temperature
   * @param main_mass Mass of main molecule
   * @param collider_mass Mass of colliding molecule
   * @return Numeric Adiabatic factor
   */
  Numeric get(const Numeric& L,
              const Numeric& B0,
              const Numeric& T,
              const Numeric& main_mass,
              const Numeric& collider_mass) const {
    switch (mtype) {
      case Type::Hartmann:
        return mol_X(L, B0, T, main_mass, collider_mass);
    }
    return 0;
  }

 private:
  Type mtype;
  Vector mdata;
};

/** Basis rate of transitions */
class BasisRate {
 public:

  /** Type of basis rate */
  enum class Type { Hartmann };

  /** Type of basis rate */
  enum class HartmannPos : Index { a1, a2, a3, SIZE };

  /** Construct a new Basis Rate object
   * 
   * Checks that the type and the input agrees
   * 
   * @param v data for the basis rate
   * @param t type describing position of data
   */
  BasisRate(const Vector& v, Type t) : mtype(t), mdata(v) {
    bool error = false;
    switch (mtype) {
      case Type::Hartmann:
        if (Index(HartmannPos::SIZE) not_eq mdata.nelem()) error = true;
        break;
    }

    if (error)
      throw std::runtime_error(
          "Bad initializaton of BasisRate, type and size disagree...");
  }

  /** Computes the basis rate using Hartman method
   * 
   * @param L Rotational angular momentum
   * @param B0 Rotational angular momentum constant
   * @param T Temperautre
   * @return Numeric Basis rate
   */
  Numeric mol_X(const Numeric& L, const Numeric& B0, const Numeric& T) const;

  /** Get the basis rate
   * 
   * @param L Rotational angular momentum
   * @param B0 Rotational angular momentum constant
   * @param T Temperautre
   * @return Numeric Basis rate
   */
  Numeric get(const Numeric& L, const Numeric& B0, const Numeric& T) const {
    switch (mtype) {
      case Type::Hartmann:
        return mol_X(L, B0, T);
    }
    return 0;
  }

 private:
  Type mtype;
  Vector mdata;
};

/** Struct to help keep position of the two matched outputs clear */
struct OffDiagonalElementOutput {
  Numeric ij, ji;
};

/** Methods to compute off diagonal elements */
namespace OffDiagonalElement {

/** Type of off diagonal element computations */
enum class Type { CO2_IR, O2_66_MW };

/** CO2 IR off diagonal element computer
 * 
 * @param j_line Line at pos j
 * @param k_line Line at pos k
 * @param j_rho Population density at pos j
 * @param k_rho Population density at pos k
 * @param br Basis rate computer
 * @param af Adiabatic factor computer
 * @param T Temperature
 * @param B0 Rotational constant
 * @param main_mass Mass of main molecule
 * @param collider_mass Mass of collider molecule
 * @return OffDiagonalElementOutput for j and k
 */
OffDiagonalElementOutput CO2_IR(const LineRecord& j_line,
                                const LineRecord& k_line,
                                const Numeric& j_rho,
                                const Numeric& k_rho,
                                const BasisRate& br,
                                const AdiabaticFactor& af,
                                const Numeric& T,
                                const Numeric& B0,
                                const Numeric& main_mass,
                                const Numeric& collider_mass);

/** O2-66 MW off diagonal element computer 
 * 
 * @param line1 Line at pos 1
 * @param line2 Line at pos 2
 * @param rho1 Population density at pos 1
 * @param rho2 Population density at pos 2
 * @param T Temperature
 * @param collider_mass Mass of collider
 * @return OffDiagonalElementOutput for 1 and 2
 */
OffDiagonalElementOutput O2_66_MW(const LineRecord& line1,
                                  const LineRecord& line2,
                                  const Numeric& rho1,
                                  const Numeric& rho2,
                                  const Numeric& T,
                                  const Numeric& collider_mass);
};  // namespace OffDiagonalElement

/** Energy corrected sudden relaxation matrix using Hartmann's method
 * 
 * @param abs_lines One band of lines
 * @param main_species Species tag of these lines
 * @param collider_species Species tag of collider
 * @param collider_species_vmr VMR of collider
 * @param partition_functions Method to compute the partition function
 * @param T Temperature
 * @param size Number of elements
 * @return Relaxation Matrix
 */
Matrix hartmann_ecs_interface(const ArrayOfLineRecord& abs_lines,
                              const ArrayOfSpeciesTag& main_species,
                              const ArrayOfSpeciesTag& collider_species,
                              const Vector& collider_species_vmr,
                              const SpeciesAuxData& partition_functions,
                              const Numeric& T,
                              const Index& size);

/** Compute the population density
 * 
 * @param abs_lines One band of lines
 * @param partition_functions Method to compute the partition function
 * @param T Temperature
 * @return Vector The population density for each line
 */
Vector population_density_vector(const ArrayOfLineRecord& abs_lines,
                                 const SpeciesAuxData& partition_functions,
                                 const Numeric& T);

/** Dipole vector
 * 
 * @param abs_lines One band of lines
 * @param partition_functions Method to compute the partition function
 * @return Vector Dipole for each line
 */
Vector dipole_vector(const ArrayOfLineRecord& abs_lines,
                     const SpeciesAuxData& partition_functions);

/** Type of reduced dipole */
enum class RedPoleType { ElectricRoVibDipole, MagneticQuadrapole };

/** Reduced dipole vector
 * 
 * @param abs_lines One band of lines
 * @param type Type of reduced dipole
 * @return Vector Reduced dipole for each line
 */
Vector reduced_dipole_vector(const ArrayOfLineRecord& abs_lines,
                             const RedPoleType type);

/** Computes G for Rosenkranz's line mixing coefficients
 * 
 * @param abs_lines One band of lines
 * @param W Relaxation Matrix
 * @param d0 Dipole vector
 * @return Vector G for each line
 */
Vector rosenkranz_scaling_second_order(const ArrayOfLineRecord& abs_lines,
                                       const Matrix& W,
                                       const Vector& d0);

/** Computes DV for Rosenkranz's line mixing coefficients
 * 
 * @param abs_lines One band of lines
 * @param W Relaxation Matrix
 * @return Vector DV for each line
 */
Vector rosenkranz_shifting_second_order(const ArrayOfLineRecord& abs_lines,
                                        const Matrix& W);

/** Computes Y for Rosenkranz's line mixing coefficients
 * 
 * @param abs_lines One band of lines
 * @param W Relaxation Matrix
 * @param d0 Dipole vector
 * @return Vector Y for each line
 */
Vector rosenkranz_first_order(const ArrayOfLineRecord& abs_lines,
                              const Matrix& W,
                              const Vector& d0);

/** To keep track of (y0 + y1 * (T0 / T - 1.)) * pow(T0 / T, n) */
struct SecondOrderLineMixingCoeffs {
  Numeric y0, y1;
};

/** Fit to a second order line mixing formula the input
 * 
 * Finds best fit [c0, c1] of
 * y(x) = (c0 + c1 * (x0 / x - 1.)) * pow(x0 / x, exp)
 * 
 * @param y Y-axis
 * @param x X-axis
 * @param exp Exponent
 * @param x0 Zero-value of x0
 * @return SecondOrderLineMixingCoeffs 
 */
SecondOrderLineMixingCoeffs compute_2nd_order_lm_coeff(ConstVectorView y,
                                                       ConstVectorView x,
                                                       const Numeric exp,
                                                       const Numeric x0);

/** Equivalent line strengths
 * 
 * @param population The population density for each line
 * @param dipole Dipole for each line
 * @param M Solver
 * @return ComplexVector Equivalent line strengths of each line
 */
ComplexVector equivalent_linestrengths(
    const Vector& population,
    const Vector& dipole,
    const Eigen::ComplexEigenSolver<Eigen::MatrixXcd>& M);

/** Sum of line strengths
 * 
 * @param population The population density for each line
 * @param dipole Dipole for each line
 * @return Numeric The sum
 */
Numeric total_linestrengths(const Vector& population, const Vector& dipole);

/** CO2 IR training algorithm for linearization
 * 
 * @param Ji J init for all lines
 * @param Jf J final for all lines
 * @param l2i l2 init for all lines
 * @param l2f l2 final for all lines
 * @param F0 Central frequency
 * @param d Dipole for each line
 * @param rho The population density for each line
 * @param gamma Pressure broadening for each line
 * @param T Temperature
 * @return Matrix Relaxation Matrix
 */
Matrix CO2_ir_training(const ArrayOfRational& Ji,
                       const ArrayOfRational& Jf,
                       const ArrayOfRational& l2i,
                       const ArrayOfRational& l2f,
                       const Vector& F0,
                       const Vector& d,
                       const Vector& rho,
                       const Vector& gamma,
                       const Numeric& T);

#endif  // linemixing_h
