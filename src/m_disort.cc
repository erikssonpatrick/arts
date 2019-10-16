/* Copyright (C) 2006-2012 Claudia Emde <claudia.emde@dlr.de>

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
   USA. 
*/

/*!
  \file   m_disort.cc
  \author Claudia Emde <claudia.emde@dlr.de>
  \date   2006-02-06
  
  \brief  This file contains functions to use the multiple scattering 
  program DISORT.
  
  These functions are listed in the doxygen documentation as entries of the
  file auto_md.h
*/

/*===========================================================================
  === External declarations
  ===========================================================================*/

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include "array.h"
#include "arts.h"
#include "auto_md.h"
#include "disort.h"
#include "m_general.h"
#include "math_funcs.h"
#include "messages.h"
#include "wsv_aux.h"
#include "xml_io.h"

/* Workspace method: Doxygen documentation will be auto-generated */
void DisortCalc(Workspace& ws,
                // WS Output:
                Tensor7& doit_i_field,
                // WS Input
                const Index& atmfields_checked,
                const Index& atmgeom_checked,
                const Index& scat_data_checked,
                const Index& cloudbox_checked,
                const Index& cloudbox_on,
                const ArrayOfIndex& cloudbox_limits,
                const Agenda& propmat_clearsky_agenda,
                const Index& atmosphere_dim,
                const Tensor4& pnd_field,
                const Tensor3& t_field,
                const Tensor3& z_field,
                const Tensor4& vmr_field,
                const Vector& p_grid,
                const ArrayOfArrayOfSingleScatteringData& scat_data,
                const Vector& f_grid,
                const Vector& scat_za_grid,
                const Index& stokes_dim,
                const Numeric& surface_skin_t,
                const Vector& surface_scalar_reflectivity,
                const Index& nstreams,
                const String& pfct_method,
                const Index& Npfct,
                const Index& pseudo_spherical,
                const Index& cdisort_quiet,
                const Verbosity& verbosity) {
  // Don't do anything if there's no cloudbox defined.
  if (!cloudbox_on) {
    CREATE_OUT0;
    out0 << "  Cloudbox is off, DISORT calculation will be skipped.\n";
    return;
  }

  // FIXME: so far surface is implictly assumed at lowest atmospheric level.
  // That should be fixed (using z_surface and allowing other altitudes) at some
  // point.

  // FIXME: At the moment, combining scattering elements stored on different
  //  scattering angle grids is only possible for pfct_method 'interpolate'.

  check_disort_input(cloudbox_on,
                     atmfields_checked,
                     atmgeom_checked,
                     cloudbox_checked,
                     scat_data_checked,
                     atmosphere_dim,
                     stokes_dim,
                     cloudbox_limits,
                     scat_data,
                     scat_za_grid,
                     nstreams,
                     pfct_method,
                     pnd_field.ncols());

  init_ifield(
      doit_i_field, f_grid, cloudbox_limits, scat_za_grid.nelem(), stokes_dim);

  Vector albedo(f_grid.nelem(), 0.);
  Numeric btemp;

  get_disortsurf_props(
      albedo, btemp, f_grid, surface_skin_t, surface_scalar_reflectivity);

  run_cdisort(
      ws,
      doit_i_field,
      f_grid,
      p_grid,
      z_field,
      t_field,
      vmr_field,
      pnd_field,
      scat_data,
      propmat_clearsky_agenda,
      cloudbox_limits,
      btemp,
      albedo,
      scat_za_grid,
      nstreams,
      Npfct,
      pseudo_spherical,
      cdisort_quiet,
      verbosity);
}
