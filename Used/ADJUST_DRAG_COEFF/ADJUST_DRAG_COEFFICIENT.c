#include "udf.h"

/******************************************************************************
 Modify particle and cell velocity as function of volume of fluid in a cell.  
******************************************************************************/

DEFINE_DPM_SCALAR_UPDATE(modify_drag_by_velocity, c, t, init,p)
{
	real alpha_q;
	
	// obtain volume fraction of water
	c = P_CELL(p);
	
	// thread dependent on simulation parameter (0- primary phase 1-secondary phase)
	t = THREAD_SUB_THREAD(P_CELL_THREAD(p),1);
	alpha_q = C_VOF(c,t);

	printf("\n VOF: %g \n", alpha_q); fflush(stdout);
	
	// scale velocities ... 
	//... of particles
	P_VEL(p)[0] *= alpha_q;
	P_VEL(p)[1] *= alpha_q;
	P_VEL(p)[2] *= alpha_q;
	
	//... of cell fluid
	C_U(c,t) *= alpha_q;
	C_V(c,t) *= alpha_q;
	C_W(c,t) *= alpha_q;	
}


/******************************************************************************
Modify particle drag coefficient as function of volume of fluid in a cell.
******************************************************************************/

DEFINE_DPM_DRAG(modify_drag_coeff, Re, p)
{
	real drag_force, alpha_q;
	
	// set drag force based on Reynolds number
	if (Re < 0.01) {drag_force = 18.0;}
	else if (Re < 20.0) {drag_force = 18.0 + 2.367 * pow(Re, 0.82 - 0.05 * log10(Re));}
	else {drag_force = 18.0 + 3.483 * pow(Re, 0.6305);}
		
	//obtain volume fraction of water 
	cell_t c;
	Thread *t;
	c = P_CELL(p);
	
	// thread dependent on simulation parameter (0- primary phase 1-secondary phase)
	t = THREAD_SUB_THREAD(P_CELL_THREAD(p),1);
	alpha_q = C_VOF(c,t);
	printf("\n VOF: %g \n", alpha_q); fflush(stdout);

	drag_force = drag_force * alpha_q;
	
	printf("\n DF: %g \n", drag_force); fflush(stdout);
	
	return drag_force;
}
