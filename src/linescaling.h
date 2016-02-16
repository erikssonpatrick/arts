/* Copyright (C) 2015
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

#include "absorption.h"


void GetLineScalingData(Numeric& q_t, 
                        Numeric& q_ref,
                        Numeric& partition_ratio, 
                        Numeric& K1, 
                        Numeric& K2, 
                        Numeric& abs_nlte_ratio, 
                        Numeric& src_nlte_ratio, 
                        const SpeciesAuxData::AuxType& partition_type,
                        const ArrayOfGriddedField1& partition_data,
                        const Numeric& atm_t,
                        const Numeric& line_t,
                        const Numeric& line_f,
                        const Numeric& line_elow,
                        const bool&    do_nlte,
                        const Numeric& line_evlow,
                        const Numeric& line_evupp,
                        const Index& line_evlow_index,
                        const Index& line_evupp_index,
                        ConstVectorView atm_t_nlte);

void GetLineScalingData_dT(Numeric& dq_t_dT,
                           Numeric& dK2_dT,
                           Numeric& dpartition_ratio_dT, 
                           Numeric& dabs_nlte_ratio_dT, 
                           Numeric& atm_tv_low,
                           Numeric& atm_tv_upp,
                           const Numeric& q_t,
                           const Numeric& abs_nlte_ratio,  
                           const SpeciesAuxData::AuxType& partition_type,
                           const ArrayOfGriddedField1& partition_data,
                           const Numeric& atm_t,
                           const Numeric& line_t,
                           const Numeric& dt,
                           const Numeric& line_f,
                           const bool&    do_nlte,
                           const Numeric& line_evlow,
                           const Numeric& line_evupp,
                           const Index& line_evlow_index,
                           const Index& line_evupp_index,
                           ConstVectorView atm_t_nlte);

void GetLineScalingData_dF0(Numeric& dK2_dF0, 
                            Numeric& dabs_nlte_ratio_dF0,
                            const Numeric& atm_t,
                            const Numeric& line_t,
                            const Numeric& atm_tv_low,
                            const Numeric& atm_tv_upp,
                            const Numeric& line_evlow,
                            const Numeric& line_evupp,
                            const Numeric& line_f);

void GetChangeInPartitionRatio(Numeric& dQ_dT, 
                               const Numeric& q_t,
                               const SpeciesAuxData::AuxType& partition_type,
                               const ArrayOfGriddedField1& partition_data,
                               const Numeric& atm_t,
                               const Numeric& dT);

void CalculatePartitionFctFromData( Numeric& q_ref, 
                                    Numeric& q_t, 
                                    const Numeric& ref, 
                                    const Numeric& t,
                                    ConstVectorView t_grid, 
                                    ConstVectorView q_grid, 
                                    const Index& interp_order);

void CalculatePartitionFctFromData_perturbed( Numeric& dQ_dT, 
                                              const Numeric& t,
                                              const Numeric& dT,
                                              const Numeric& q_t,
                                              ConstVectorView t_grid, 
                                              ConstVectorView q_grid, 
                                              const Index& interp_order);

void CalculatePartitionFctFromCoeff(Numeric& q_ref, 
                                    Numeric& q_t, 
                                    const Numeric& ref, 
                                    const Numeric& t,
                                    ConstVectorView q_grid);

void CalculatePartitionFctFromCoeff_dT(Numeric& dQ_dT, 
                                       const Numeric& t,
                                       ConstVectorView q_grid);

void CalculatePartitionFctFromVibrotCoeff(Numeric& q_ref, 
                                          Numeric& q_t, 
                                          const Numeric& ref, 
                                          const Numeric& t_vib,
                                          const Numeric& t_rot,
                                          ConstVectorView qvib_grid,
                                          ConstVectorView qrot_grid);

void CalculatePartitionFctFromVibrotCoeff_dT(Numeric& dQ_dT, 
                                             const Numeric& t_vib,
                                             const Numeric& t_rot,
                                             ConstVectorView qvib_grid,
                                             ConstVectorView qrot_grid);