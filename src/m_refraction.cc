/* Copyright (C) 2003-2008 Patrick Eriksson <Patrick.Eriksson@rss.chalmers.se>

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



/*===========================================================================
  ===  File description 
  ===========================================================================*/

/*!
  \file   m_refraction.cc
  \author Patrick Eriksson <Patrick.Eriksson@rss.chalmers.se>
  \date   2003-01-09

  \brief  Workspace methods releated to refraction.

  These functions are listed in the doxygen documentation as entries of the
  file auto_md.h.
*/



/*===========================================================================
  === External declarations
  ===========================================================================*/

#include "absorption.h"
#include "arts.h"
#include "check_input.h"
#include "matpackI.h"
#include "messages.h"
#include "refraction.h"
#include "special_interp.h"
#include "abs_species_tags.h"



/*===========================================================================
  === WSMs
  ===========================================================================*/


/* Workspace method: Doxygen documentation will be auto-generated */
void refr_indexFieldAndGradients(
      // WS Output:
            Numeric&                    refr_index,
            Numeric&                    a_pressure,
            Numeric&                    a_temperature,
            Vector&                     a_vmr_list,
      // WS Generic Output:
            Tensor4&                    out,
      // WS Input:
      const Agenda&                     refr_index_agenda,
      const Index&                      atmosphere_dim,
      const Vector&                     p_grid,
      const Vector&                     lat_grid,
      const Vector&                     lon_grid,
      const Matrix&                     r_geoid,
      const Tensor3&                    z_field,
      const Tensor3&                    t_field,
      const Tensor4&                    vmr_field,
      // WS Generic Input:
      const Vector&                     p_values,
      const Vector&                     lat_values,
      const Vector&                     lon_values)
{
  // Check input
  chk_if_in_range( "atmosphere_dim", atmosphere_dim, 1, 3 );
  chk_atm_grids( atmosphere_dim, p_grid, lat_grid, lon_grid );
  chk_atm_surface( "r_geoid", r_geoid, atmosphere_dim, lat_grid, lon_grid );
  chk_atm_field( "z_field", z_field, atmosphere_dim, p_grid, lat_grid, 
                                                                    lon_grid );
  chk_atm_field( "t_field", t_field, atmosphere_dim, p_grid, lat_grid, 
                                                                    lon_grid );
  chk_atm_field( "first book of vmr_field", vmr_field(0,joker,joker,joker), 
                                  atmosphere_dim, p_grid, lat_grid, lon_grid );

  // Common variables
  const Index     np = p_values.nelem(), npgrid = p_grid.nelem(); 
        Index     nlat=1, nlon=1, nlatgrid = 1, nlongrid = 1;
        Numeric   dndr, dndlat, dndlon;

  if( atmosphere_dim == 1 )
    {
      out.resize(2,np,1,1);
    }
  else if( atmosphere_dim == 2 )
    {
      nlat     = lat_values.nelem();
      nlatgrid = lat_grid.nelem();
      out.resize(3,np,nlat,1);
    }
  else
    {
      nlat     = lat_values.nelem();
      nlatgrid = lat_grid.nelem();
      nlon     = lon_values.nelem();
      nlongrid = lon_grid.nelem();
      out.resize(4,np,nlat,nlon);
    }

  // Create a tensor with radius of the pressure surfaces
  Tensor3   r_field(npgrid,nlatgrid,nlongrid);
  //
  for( Index ilon=0; ilon<nlongrid; ilon++ )
    {
      for( Index ilat=0; ilat<nlatgrid; ilat++ )
        {
          for( Index ip=0; ip<npgrid; ip++ )
            {
              r_field(ip,ilat,ilon) = r_geoid(ilat,ilon) + 
                                                         z_field(ip,ilat,ilon);
            }
        }
    }


  for( Index ilon=0; ilon<nlon; ilon++ )
    {
      for( Index ilat=0; ilat<nlat; ilat++ )
        {
          ArrayOfGridPos   gp_p(np), gp_lat(1), gp_lon(1);
          Vector           r_grid( p_grid.nelem() ), r(np);
          Matrix           itw(np,2);

          if( atmosphere_dim == 1 )
            { 
              r_grid = r_field(joker,0,0);
            }
          else if( atmosphere_dim == 2 )
            { 
              gridpos( gp_lat, lat_grid, lat_values[ilat] );
              z_at_lat_2d( r_grid, p_grid, lat_grid, r_field(joker,joker,0), 
                                                                   gp_lat[0] );
            }
          else
            { 
              gridpos( gp_lat, lat_grid, lat_values[ilat] );
              gridpos( gp_lon, lon_grid, lon_values[ilon] );
              z_at_latlon( r_grid, p_grid, lat_grid, lon_grid, r_field, 
                                                        gp_lat[0], gp_lon[0] );
            }

          p2gridpos( gp_p, p_grid, p_values );
          interpweights( itw, gp_p );
          interp( r, itw, r_grid, gp_p );

          for( Index ip=0; ip<np; ip++ )
            {

              const Index   agenda_verb = ip + ilat + ilon;

              if( atmosphere_dim == 1 )
                {
                  refr_gradients_1d( refr_index, dndr, a_pressure, 
                               a_temperature, a_vmr_list, refr_index_agenda, 
                               agenda_verb,
                               p_grid, r_geoid(0,0), z_field(joker,0,0), 
                               t_field(joker,0,0), vmr_field(joker,joker,0,0),
                                                                       r[ip] );
                }

              else if( atmosphere_dim == 2 )
                {
                  refr_gradients_2d( refr_index, dndr, dndlat, a_pressure, 
                               a_temperature, a_vmr_list, refr_index_agenda,
                               agenda_verb,
                               p_grid, lat_grid, r_geoid(joker,0), 
                               z_field(joker,joker,0), t_field(joker,joker,0), 
                               vmr_field(joker,joker,joker,0),
                                                     r[ip], lat_values[ilat] );
                  out(2,ip,ilat,ilon) = dndlat;
                }
              
              else
                {
                  refr_gradients_3d( refr_index, dndr, dndlat, dndlon,
                               a_pressure, 
                               a_temperature, a_vmr_list, refr_index_agenda, 
                               agenda_verb,
                               p_grid, lat_grid, lon_grid, r_geoid, 
                               z_field, t_field, vmr_field,
                                   r[ip], lat_values[ilat], lon_values[ilon] );
                  out(2,ip,ilat,ilon) = dndlat;
                  out(3,ip,ilat,ilon) = dndlon;
                }

              out(0,ip,ilat,ilon) = refr_index;
              out(1,ip,ilat,ilon) = dndr;
            }
        }
    }
}


/* Workspace method: Doxygen documentation will be auto-generated */
void refr_indexIR(
        // WS Output
              Numeric&                  refr_index,
        // WS Input
        const Numeric&                  a_pressure,
        const Numeric&                  a_temperature,
        const Vector&                   a_vmr_list )
{
  //To suppress warning about unused parameter
  a_vmr_list.nelem();

  refr_index_ir( refr_index, a_pressure, a_temperature );
}


/* Workspace method: Doxygen documentation will be auto-generated */
void refr_indexThayer(
             Numeric&                    refr_index,
       const Numeric&                    a_pressure,
       const Numeric&                    a_temperature,
       const Vector&                     a_vmr_list,
       const ArrayOfArrayOfSpeciesTag&   abs_species )
{
  if( abs_species.nelem() != a_vmr_list.nelem() )
    throw runtime_error( "The number of tag groups differ between "
                                           "*a_vmr_list* and *abs_species*." );

  Index   firstH2O = find_first_species_tg( abs_species,
                                      species_index_from_species_name("H2O") );

  if( firstH2O < 0 )
    throw runtime_error(
       "Water vapour is a requiered (must be a tag group in *abs_species*)." );

  refr_index_thayer_1974( refr_index, a_pressure, a_temperature,
                                                        a_vmr_list[firstH2O] );
}


/* Workspace method: Doxygen documentation will be auto-generated */
void refr_indexUnit(
         Numeric&                    refr_index )
{
  refr_index = 1;
}
