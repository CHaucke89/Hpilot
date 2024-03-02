#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                       Code generated with SymPy 1.12                       *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_6290441625309936060) {
   out_6290441625309936060[0] = delta_x[0] + nom_x[0];
   out_6290441625309936060[1] = delta_x[1] + nom_x[1];
   out_6290441625309936060[2] = delta_x[2] + nom_x[2];
   out_6290441625309936060[3] = delta_x[3] + nom_x[3];
   out_6290441625309936060[4] = delta_x[4] + nom_x[4];
   out_6290441625309936060[5] = delta_x[5] + nom_x[5];
   out_6290441625309936060[6] = delta_x[6] + nom_x[6];
   out_6290441625309936060[7] = delta_x[7] + nom_x[7];
   out_6290441625309936060[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5314558907334496336) {
   out_5314558907334496336[0] = -nom_x[0] + true_x[0];
   out_5314558907334496336[1] = -nom_x[1] + true_x[1];
   out_5314558907334496336[2] = -nom_x[2] + true_x[2];
   out_5314558907334496336[3] = -nom_x[3] + true_x[3];
   out_5314558907334496336[4] = -nom_x[4] + true_x[4];
   out_5314558907334496336[5] = -nom_x[5] + true_x[5];
   out_5314558907334496336[6] = -nom_x[6] + true_x[6];
   out_5314558907334496336[7] = -nom_x[7] + true_x[7];
   out_5314558907334496336[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_7895949841719061513) {
   out_7895949841719061513[0] = 1.0;
   out_7895949841719061513[1] = 0;
   out_7895949841719061513[2] = 0;
   out_7895949841719061513[3] = 0;
   out_7895949841719061513[4] = 0;
   out_7895949841719061513[5] = 0;
   out_7895949841719061513[6] = 0;
   out_7895949841719061513[7] = 0;
   out_7895949841719061513[8] = 0;
   out_7895949841719061513[9] = 0;
   out_7895949841719061513[10] = 1.0;
   out_7895949841719061513[11] = 0;
   out_7895949841719061513[12] = 0;
   out_7895949841719061513[13] = 0;
   out_7895949841719061513[14] = 0;
   out_7895949841719061513[15] = 0;
   out_7895949841719061513[16] = 0;
   out_7895949841719061513[17] = 0;
   out_7895949841719061513[18] = 0;
   out_7895949841719061513[19] = 0;
   out_7895949841719061513[20] = 1.0;
   out_7895949841719061513[21] = 0;
   out_7895949841719061513[22] = 0;
   out_7895949841719061513[23] = 0;
   out_7895949841719061513[24] = 0;
   out_7895949841719061513[25] = 0;
   out_7895949841719061513[26] = 0;
   out_7895949841719061513[27] = 0;
   out_7895949841719061513[28] = 0;
   out_7895949841719061513[29] = 0;
   out_7895949841719061513[30] = 1.0;
   out_7895949841719061513[31] = 0;
   out_7895949841719061513[32] = 0;
   out_7895949841719061513[33] = 0;
   out_7895949841719061513[34] = 0;
   out_7895949841719061513[35] = 0;
   out_7895949841719061513[36] = 0;
   out_7895949841719061513[37] = 0;
   out_7895949841719061513[38] = 0;
   out_7895949841719061513[39] = 0;
   out_7895949841719061513[40] = 1.0;
   out_7895949841719061513[41] = 0;
   out_7895949841719061513[42] = 0;
   out_7895949841719061513[43] = 0;
   out_7895949841719061513[44] = 0;
   out_7895949841719061513[45] = 0;
   out_7895949841719061513[46] = 0;
   out_7895949841719061513[47] = 0;
   out_7895949841719061513[48] = 0;
   out_7895949841719061513[49] = 0;
   out_7895949841719061513[50] = 1.0;
   out_7895949841719061513[51] = 0;
   out_7895949841719061513[52] = 0;
   out_7895949841719061513[53] = 0;
   out_7895949841719061513[54] = 0;
   out_7895949841719061513[55] = 0;
   out_7895949841719061513[56] = 0;
   out_7895949841719061513[57] = 0;
   out_7895949841719061513[58] = 0;
   out_7895949841719061513[59] = 0;
   out_7895949841719061513[60] = 1.0;
   out_7895949841719061513[61] = 0;
   out_7895949841719061513[62] = 0;
   out_7895949841719061513[63] = 0;
   out_7895949841719061513[64] = 0;
   out_7895949841719061513[65] = 0;
   out_7895949841719061513[66] = 0;
   out_7895949841719061513[67] = 0;
   out_7895949841719061513[68] = 0;
   out_7895949841719061513[69] = 0;
   out_7895949841719061513[70] = 1.0;
   out_7895949841719061513[71] = 0;
   out_7895949841719061513[72] = 0;
   out_7895949841719061513[73] = 0;
   out_7895949841719061513[74] = 0;
   out_7895949841719061513[75] = 0;
   out_7895949841719061513[76] = 0;
   out_7895949841719061513[77] = 0;
   out_7895949841719061513[78] = 0;
   out_7895949841719061513[79] = 0;
   out_7895949841719061513[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_4613932111658201338) {
   out_4613932111658201338[0] = state[0];
   out_4613932111658201338[1] = state[1];
   out_4613932111658201338[2] = state[2];
   out_4613932111658201338[3] = state[3];
   out_4613932111658201338[4] = state[4];
   out_4613932111658201338[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_4613932111658201338[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_4613932111658201338[7] = state[7];
   out_4613932111658201338[8] = state[8];
}
void F_fun(double *state, double dt, double *out_7388683364835999016) {
   out_7388683364835999016[0] = 1;
   out_7388683364835999016[1] = 0;
   out_7388683364835999016[2] = 0;
   out_7388683364835999016[3] = 0;
   out_7388683364835999016[4] = 0;
   out_7388683364835999016[5] = 0;
   out_7388683364835999016[6] = 0;
   out_7388683364835999016[7] = 0;
   out_7388683364835999016[8] = 0;
   out_7388683364835999016[9] = 0;
   out_7388683364835999016[10] = 1;
   out_7388683364835999016[11] = 0;
   out_7388683364835999016[12] = 0;
   out_7388683364835999016[13] = 0;
   out_7388683364835999016[14] = 0;
   out_7388683364835999016[15] = 0;
   out_7388683364835999016[16] = 0;
   out_7388683364835999016[17] = 0;
   out_7388683364835999016[18] = 0;
   out_7388683364835999016[19] = 0;
   out_7388683364835999016[20] = 1;
   out_7388683364835999016[21] = 0;
   out_7388683364835999016[22] = 0;
   out_7388683364835999016[23] = 0;
   out_7388683364835999016[24] = 0;
   out_7388683364835999016[25] = 0;
   out_7388683364835999016[26] = 0;
   out_7388683364835999016[27] = 0;
   out_7388683364835999016[28] = 0;
   out_7388683364835999016[29] = 0;
   out_7388683364835999016[30] = 1;
   out_7388683364835999016[31] = 0;
   out_7388683364835999016[32] = 0;
   out_7388683364835999016[33] = 0;
   out_7388683364835999016[34] = 0;
   out_7388683364835999016[35] = 0;
   out_7388683364835999016[36] = 0;
   out_7388683364835999016[37] = 0;
   out_7388683364835999016[38] = 0;
   out_7388683364835999016[39] = 0;
   out_7388683364835999016[40] = 1;
   out_7388683364835999016[41] = 0;
   out_7388683364835999016[42] = 0;
   out_7388683364835999016[43] = 0;
   out_7388683364835999016[44] = 0;
   out_7388683364835999016[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_7388683364835999016[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_7388683364835999016[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7388683364835999016[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7388683364835999016[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_7388683364835999016[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_7388683364835999016[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_7388683364835999016[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_7388683364835999016[53] = -9.8000000000000007*dt;
   out_7388683364835999016[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_7388683364835999016[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_7388683364835999016[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7388683364835999016[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7388683364835999016[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_7388683364835999016[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_7388683364835999016[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_7388683364835999016[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7388683364835999016[62] = 0;
   out_7388683364835999016[63] = 0;
   out_7388683364835999016[64] = 0;
   out_7388683364835999016[65] = 0;
   out_7388683364835999016[66] = 0;
   out_7388683364835999016[67] = 0;
   out_7388683364835999016[68] = 0;
   out_7388683364835999016[69] = 0;
   out_7388683364835999016[70] = 1;
   out_7388683364835999016[71] = 0;
   out_7388683364835999016[72] = 0;
   out_7388683364835999016[73] = 0;
   out_7388683364835999016[74] = 0;
   out_7388683364835999016[75] = 0;
   out_7388683364835999016[76] = 0;
   out_7388683364835999016[77] = 0;
   out_7388683364835999016[78] = 0;
   out_7388683364835999016[79] = 0;
   out_7388683364835999016[80] = 1;
}
void h_25(double *state, double *unused, double *out_3327535608170938215) {
   out_3327535608170938215[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2233137525527874346) {
   out_2233137525527874346[0] = 0;
   out_2233137525527874346[1] = 0;
   out_2233137525527874346[2] = 0;
   out_2233137525527874346[3] = 0;
   out_2233137525527874346[4] = 0;
   out_2233137525527874346[5] = 0;
   out_2233137525527874346[6] = 1;
   out_2233137525527874346[7] = 0;
   out_2233137525527874346[8] = 0;
}
void h_24(double *state, double *unused, double *out_3234738581906136368) {
   out_3234738581906136368[0] = state[4];
   out_3234738581906136368[1] = state[5];
}
void H_24(double *state, double *unused, double *out_5185254108149052547) {
   out_5185254108149052547[0] = 0;
   out_5185254108149052547[1] = 0;
   out_5185254108149052547[2] = 0;
   out_5185254108149052547[3] = 0;
   out_5185254108149052547[4] = 1;
   out_5185254108149052547[5] = 0;
   out_5185254108149052547[6] = 0;
   out_5185254108149052547[7] = 0;
   out_5185254108149052547[8] = 0;
   out_5185254108149052547[9] = 0;
   out_5185254108149052547[10] = 0;
   out_5185254108149052547[11] = 0;
   out_5185254108149052547[12] = 0;
   out_5185254108149052547[13] = 0;
   out_5185254108149052547[14] = 1;
   out_5185254108149052547[15] = 0;
   out_5185254108149052547[16] = 0;
   out_5185254108149052547[17] = 0;
}
void h_30(double *state, double *unused, double *out_3758132126270666246) {
   out_3758132126270666246[0] = state[4];
}
void H_30(double *state, double *unused, double *out_6760833855655482544) {
   out_6760833855655482544[0] = 0;
   out_6760833855655482544[1] = 0;
   out_6760833855655482544[2] = 0;
   out_6760833855655482544[3] = 0;
   out_6760833855655482544[4] = 1;
   out_6760833855655482544[5] = 0;
   out_6760833855655482544[6] = 0;
   out_6760833855655482544[7] = 0;
   out_6760833855655482544[8] = 0;
}
void h_26(double *state, double *unused, double *out_3891213047040668096) {
   out_3891213047040668096[0] = state[7];
}
void H_26(double *state, double *unused, double *out_5974640844401930570) {
   out_5974640844401930570[0] = 0;
   out_5974640844401930570[1] = 0;
   out_5974640844401930570[2] = 0;
   out_5974640844401930570[3] = 0;
   out_5974640844401930570[4] = 0;
   out_5974640844401930570[5] = 0;
   out_5974640844401930570[6] = 0;
   out_5974640844401930570[7] = 1;
   out_5974640844401930570[8] = 0;
}
void h_27(double *state, double *unused, double *out_1294650421285170099) {
   out_1294650421285170099[0] = state[3];
}
void H_27(double *state, double *unused, double *out_8935597167455907455) {
   out_8935597167455907455[0] = 0;
   out_8935597167455907455[1] = 0;
   out_8935597167455907455[2] = 0;
   out_8935597167455907455[3] = 1;
   out_8935597167455907455[4] = 0;
   out_8935597167455907455[5] = 0;
   out_8935597167455907455[6] = 0;
   out_8935597167455907455[7] = 0;
   out_8935597167455907455[8] = 0;
}
void h_29(double *state, double *unused, double *out_2590388325194524509) {
   out_2590388325194524509[0] = state[1];
}
void H_29(double *state, double *unused, double *out_6250602511341090360) {
   out_6250602511341090360[0] = 0;
   out_6250602511341090360[1] = 1;
   out_6250602511341090360[2] = 0;
   out_6250602511341090360[3] = 0;
   out_6250602511341090360[4] = 0;
   out_6250602511341090360[5] = 0;
   out_6250602511341090360[6] = 0;
   out_6250602511341090360[7] = 0;
   out_6250602511341090360[8] = 0;
}
void h_28(double *state, double *unused, double *out_7541101478745343567) {
   out_7541101478745343567[0] = state[0];
}
void H_28(double *state, double *unused, double *out_7113742545298930682) {
   out_7113742545298930682[0] = 1;
   out_7113742545298930682[1] = 0;
   out_7113742545298930682[2] = 0;
   out_7113742545298930682[3] = 0;
   out_7113742545298930682[4] = 0;
   out_7113742545298930682[5] = 0;
   out_7113742545298930682[6] = 0;
   out_7113742545298930682[7] = 0;
   out_7113742545298930682[8] = 0;
}
void h_31(double *state, double *unused, double *out_6964985136424960565) {
   out_6964985136424960565[0] = state[8];
}
void H_31(double *state, double *unused, double *out_6600848946635282046) {
   out_6600848946635282046[0] = 0;
   out_6600848946635282046[1] = 0;
   out_6600848946635282046[2] = 0;
   out_6600848946635282046[3] = 0;
   out_6600848946635282046[4] = 0;
   out_6600848946635282046[5] = 0;
   out_6600848946635282046[6] = 0;
   out_6600848946635282046[7] = 0;
   out_6600848946635282046[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_6290441625309936060) {
  err_fun(nom_x, delta_x, out_6290441625309936060);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5314558907334496336) {
  inv_err_fun(nom_x, true_x, out_5314558907334496336);
}
void car_H_mod_fun(double *state, double *out_7895949841719061513) {
  H_mod_fun(state, out_7895949841719061513);
}
void car_f_fun(double *state, double dt, double *out_4613932111658201338) {
  f_fun(state,  dt, out_4613932111658201338);
}
void car_F_fun(double *state, double dt, double *out_7388683364835999016) {
  F_fun(state,  dt, out_7388683364835999016);
}
void car_h_25(double *state, double *unused, double *out_3327535608170938215) {
  h_25(state, unused, out_3327535608170938215);
}
void car_H_25(double *state, double *unused, double *out_2233137525527874346) {
  H_25(state, unused, out_2233137525527874346);
}
void car_h_24(double *state, double *unused, double *out_3234738581906136368) {
  h_24(state, unused, out_3234738581906136368);
}
void car_H_24(double *state, double *unused, double *out_5185254108149052547) {
  H_24(state, unused, out_5185254108149052547);
}
void car_h_30(double *state, double *unused, double *out_3758132126270666246) {
  h_30(state, unused, out_3758132126270666246);
}
void car_H_30(double *state, double *unused, double *out_6760833855655482544) {
  H_30(state, unused, out_6760833855655482544);
}
void car_h_26(double *state, double *unused, double *out_3891213047040668096) {
  h_26(state, unused, out_3891213047040668096);
}
void car_H_26(double *state, double *unused, double *out_5974640844401930570) {
  H_26(state, unused, out_5974640844401930570);
}
void car_h_27(double *state, double *unused, double *out_1294650421285170099) {
  h_27(state, unused, out_1294650421285170099);
}
void car_H_27(double *state, double *unused, double *out_8935597167455907455) {
  H_27(state, unused, out_8935597167455907455);
}
void car_h_29(double *state, double *unused, double *out_2590388325194524509) {
  h_29(state, unused, out_2590388325194524509);
}
void car_H_29(double *state, double *unused, double *out_6250602511341090360) {
  H_29(state, unused, out_6250602511341090360);
}
void car_h_28(double *state, double *unused, double *out_7541101478745343567) {
  h_28(state, unused, out_7541101478745343567);
}
void car_H_28(double *state, double *unused, double *out_7113742545298930682) {
  H_28(state, unused, out_7113742545298930682);
}
void car_h_31(double *state, double *unused, double *out_6964985136424960565) {
  h_31(state, unused, out_6964985136424960565);
}
void car_H_31(double *state, double *unused, double *out_6600848946635282046) {
  H_31(state, unused, out_6600848946635282046);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
