#ifndef montecarlo_h
#define montecarlo_h

/*===========================================================================
  === External declarations
  ===========================================================================*/
#include "messages.h"
#include "arts.h"
#include "ppath.h"
#include "matpackI.h"
#include "special_interp.h"
#include "check_input.h"
#include <stdexcept>
#include <cmath>
#include "rte.h"
#include "lin_alg.h"
#include "auto_md.h"
#include "logic.h"
#include "physics_funcs.h"
#include "xml_io.h"
#include "rng.h"

extern const Numeric DEG2RAD;
extern const Numeric RAD2DEG;
extern const Numeric PI;

void Cloudbox_ppath_rteCalc(
			     Ppath&                ppathcloud,
			     Ppath&                ppath,
			     Ppath&                ppath_step,
			     Vector&               rte_pos,
			     Vector&               rte_los,
			     Vector&               cum_l_step,
			     ArrayOfMatrix&        TArray,
			     ArrayOfMatrix&        ext_matArray,
			     ArrayOfVector&        abs_vecArray,
			     Vector&               t_ppath,
			     Vector&               scat_za_grid,
			     Vector&               scat_aa_grid,
			     Tensor3&              ext_mat,
			     Matrix&               abs_vec,
			     Numeric&              rte_pressure,
			     Numeric&              rte_temperature,
			     Vector&               rte_vmr_list,
			     Matrix&               i_rte,
			     GridPos&              rte_gp_p,
			     GridPos&              rte_gp_lat,
			     GridPos&              rte_gp_lon,
			     Matrix&               i_space,
			     Matrix&               ground_emission,
			     Matrix&               ground_los, 
			     Tensor4&              ground_refl_coeffs,
			     Index&                f_index,
			     Index&                scat_za_index,
			     Index&                scat_aa_index,
			     Tensor3&              ext_mat_spt,
			     Matrix&               abs_vec_spt,
			     Matrix&               pnd_ppath,
			     const Agenda&         ppath_step_agenda,
			     const Index&          atmosphere_dim,
			     const Vector&         p_grid,
			     const Vector&         lat_grid,
			     const Vector&         lon_grid,
			     const Tensor3&        z_field,
			     const Matrix&         r_geoid,
			     const Matrix&         z_ground,
			     const ArrayOfIndex&   cloudbox_limits,
			     const Index&          record_ppathcloud,
			     const Index&          record_ppath,
			     const Agenda&         opt_prop_gas_agenda,
			     const Agenda&         opt_prop_part_agenda,
			     const Agenda&         scalar_gas_absorption_agenda,
			     const Index&          stokes_dim,
			     const Tensor3&        t_field,
			     const Tensor4&        vmr_field,
			     const Agenda&         rte_agenda,
			     const Agenda&         i_space_agenda,
			     const Agenda&         ground_refl_agenda,
			     const Vector&         f_grid,
			     const Index&          photon_number,
			     const Index&          scattering_order,
			     const Tensor4&        pnd_field);


void cloudbox_ppath_start_stepping(
				   Ppath&          ppath,
				   const Index&          atmosphere_dim,
				   ConstVectorView       p_grid,
				   ConstVectorView       lat_grid,
				   ConstVectorView       lon_grid,
				   ConstTensor3View      z_field,
				   ConstMatrixView       r_geoid,
				   ConstMatrixView       z_ground,
				   ConstVectorView       rte_pos,
				   ConstVectorView       rte_los );
	  

void cum_l_stepCalc(
		      Vector& cum_l_step,
		      const Ppath& ppath
		      );


Matrix interp( ConstVectorView itw,
	       ArrayOfMatrix a,    
	       const GridPos&  tc );

Vector interp( ConstVectorView itw,
                ArrayOfVector a,    
	       const GridPos&  tc );

void interpTArray(Matrix& T,
		  Vector& Kabs,
		  Numeric& temperature,
		  MatrixView&  K,
		  Vector& rte_pos,//maybe these should be VectorViews?
		  Vector& rte_los,
		  VectorView& pnd_vec,
		  ArrayOfGridPos& gp,
		  const ArrayOfMatrix& TArray,
		  const ArrayOfMatrix& ext_matArray,
		  const ArrayOfVector& abs_vecArray,
		  const Vector& t_ppath,
		  const Matrix& pnd_ppath,
		  const Vector& cum_l_step,
		  const Numeric& pathlength,
		  const Index& stokes_dim,
		  const Ppath& ppath
		 );

void pha_mat_singleCalc(
			MatrixView& Z,			
			Numeric za_scat, 
			Numeric aa_scat, 
			Numeric za_inc, 
			Numeric aa_inc,
			const ArrayOfSingleScatteringData& scat_data_raw,
			const Index&          stokes_dim,
			const Index&                f_index,
			const Vector&         f_grid,
			const Tensor4& scat_theta, // CE: Included 
			const ArrayOfArrayOfArrayOfArrayOfGridPos& scat_theta_gps,
			const Tensor5& scat_theta_itws,
			const VectorView& pnd_vec     
			);

void Sample_los (
		 VectorView& rte_los,
		 Numeric& g_los_csc_theta,
		 Rng& rng,
		 const Index& sampling_method
		 );

void Sample_los_Z (
		   VectorView& new_rte_los,
		   Numeric& g_los_csc_theta,
		   MatrixView& Z,
		   Rng& rng,
		   const VectorView& rte_los,
		   const ArrayOfSingleScatteringData& scat_data_raw,
		   const Index&          stokes_dim,
		   const Index&                f_index,
		   const Vector&         f_grid,
		   const Tensor4& scat_theta, // CE: Included 
		   const ArrayOfArrayOfArrayOfArrayOfGridPos& scat_theta_gps,
		   const Tensor5& scat_theta_itws,
		   const VectorView& pnd_vec,
		   Numeric Csca
		   );

void Sample_ppathlength (
			 Numeric& pathlength, 
			 Numeric& g,
			 Numeric& K,
			 Rng& rng,
			 const ArrayOfMatrix& ext_matArray
			 );

void Sample_ppathlengthLOS (
			 Numeric& pathlength, 
			 Numeric& g,
			 Rng& rng,
			 const ArrayOfMatrix& ext_matArray,
			 const Numeric& dist_to_boundary
			 );


void TArrayCalc(
		//output
		ArrayOfMatrix& TArray,
		ArrayOfMatrix& ext_matArray,
		ArrayOfVector& abs_vecArray,
		Vector& t_ppath,
		Vector& scat_za_grid,
		Vector& scat_aa_grid,
		Tensor3& ext_mat,
		Matrix& abs_vec,
		Numeric&   rte_pressure,
		Numeric&   rte_temperature,
		Vector&    rte_vmr_list,
		Index&    scat_za_index,
		Index&    scat_aa_index,
		Tensor3& ext_mat_spt,
		Matrix& abs_vec_spt,
		Matrix&  pnd_ppath,
		//input
		const Ppath& ppath,
		const Agenda& opt_prop_gas_agenda,
		const Agenda& spt_calc_agenda,
		const Agenda& scalar_gas_absorption_agenda,
		const Index& stokes_dim,
		const Vector&    p_grid,
		const Vector&    lat_grid,
		const Vector&    lon_grid,
		const Tensor3&   t_field,
		const Tensor4&   vmr_field,
		const Index&     atmosphere_dim,
		const Tensor4&   pnd_field
 		);

#endif  // montecarlo_h
