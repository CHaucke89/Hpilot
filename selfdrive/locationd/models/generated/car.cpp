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
void err_fun(double *nom_x, double *delta_x, double *out_5610746733953880877) {
   out_5610746733953880877[0] = delta_x[0] + nom_x[0];
   out_5610746733953880877[1] = delta_x[1] + nom_x[1];
   out_5610746733953880877[2] = delta_x[2] + nom_x[2];
   out_5610746733953880877[3] = delta_x[3] + nom_x[3];
   out_5610746733953880877[4] = delta_x[4] + nom_x[4];
   out_5610746733953880877[5] = delta_x[5] + nom_x[5];
   out_5610746733953880877[6] = delta_x[6] + nom_x[6];
   out_5610746733953880877[7] = delta_x[7] + nom_x[7];
   out_5610746733953880877[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_659775030920310167) {
   out_659775030920310167[0] = -nom_x[0] + true_x[0];
   out_659775030920310167[1] = -nom_x[1] + true_x[1];
   out_659775030920310167[2] = -nom_x[2] + true_x[2];
   out_659775030920310167[3] = -nom_x[3] + true_x[3];
   out_659775030920310167[4] = -nom_x[4] + true_x[4];
   out_659775030920310167[5] = -nom_x[5] + true_x[5];
   out_659775030920310167[6] = -nom_x[6] + true_x[6];
   out_659775030920310167[7] = -nom_x[7] + true_x[7];
   out_659775030920310167[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_869202489952029923) {
   out_869202489952029923[0] = 1.0;
   out_869202489952029923[1] = 0;
   out_869202489952029923[2] = 0;
   out_869202489952029923[3] = 0;
   out_869202489952029923[4] = 0;
   out_869202489952029923[5] = 0;
   out_869202489952029923[6] = 0;
   out_869202489952029923[7] = 0;
   out_869202489952029923[8] = 0;
   out_869202489952029923[9] = 0;
   out_869202489952029923[10] = 1.0;
   out_869202489952029923[11] = 0;
   out_869202489952029923[12] = 0;
   out_869202489952029923[13] = 0;
   out_869202489952029923[14] = 0;
   out_869202489952029923[15] = 0;
   out_869202489952029923[16] = 0;
   out_869202489952029923[17] = 0;
   out_869202489952029923[18] = 0;
   out_869202489952029923[19] = 0;
   out_869202489952029923[20] = 1.0;
   out_869202489952029923[21] = 0;
   out_869202489952029923[22] = 0;
   out_869202489952029923[23] = 0;
   out_869202489952029923[24] = 0;
   out_869202489952029923[25] = 0;
   out_869202489952029923[26] = 0;
   out_869202489952029923[27] = 0;
   out_869202489952029923[28] = 0;
   out_869202489952029923[29] = 0;
   out_869202489952029923[30] = 1.0;
   out_869202489952029923[31] = 0;
   out_869202489952029923[32] = 0;
   out_869202489952029923[33] = 0;
   out_869202489952029923[34] = 0;
   out_869202489952029923[35] = 0;
   out_869202489952029923[36] = 0;
   out_869202489952029923[37] = 0;
   out_869202489952029923[38] = 0;
   out_869202489952029923[39] = 0;
   out_869202489952029923[40] = 1.0;
   out_869202489952029923[41] = 0;
   out_869202489952029923[42] = 0;
   out_869202489952029923[43] = 0;
   out_869202489952029923[44] = 0;
   out_869202489952029923[45] = 0;
   out_869202489952029923[46] = 0;
   out_869202489952029923[47] = 0;
   out_869202489952029923[48] = 0;
   out_869202489952029923[49] = 0;
   out_869202489952029923[50] = 1.0;
   out_869202489952029923[51] = 0;
   out_869202489952029923[52] = 0;
   out_869202489952029923[53] = 0;
   out_869202489952029923[54] = 0;
   out_869202489952029923[55] = 0;
   out_869202489952029923[56] = 0;
   out_869202489952029923[57] = 0;
   out_869202489952029923[58] = 0;
   out_869202489952029923[59] = 0;
   out_869202489952029923[60] = 1.0;
   out_869202489952029923[61] = 0;
   out_869202489952029923[62] = 0;
   out_869202489952029923[63] = 0;
   out_869202489952029923[64] = 0;
   out_869202489952029923[65] = 0;
   out_869202489952029923[66] = 0;
   out_869202489952029923[67] = 0;
   out_869202489952029923[68] = 0;
   out_869202489952029923[69] = 0;
   out_869202489952029923[70] = 1.0;
   out_869202489952029923[71] = 0;
   out_869202489952029923[72] = 0;
   out_869202489952029923[73] = 0;
   out_869202489952029923[74] = 0;
   out_869202489952029923[75] = 0;
   out_869202489952029923[76] = 0;
   out_869202489952029923[77] = 0;
   out_869202489952029923[78] = 0;
   out_869202489952029923[79] = 0;
   out_869202489952029923[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_8586997816965019240) {
   out_8586997816965019240[0] = state[0];
   out_8586997816965019240[1] = state[1];
   out_8586997816965019240[2] = state[2];
   out_8586997816965019240[3] = state[3];
   out_8586997816965019240[4] = state[4];
   out_8586997816965019240[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_8586997816965019240[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_8586997816965019240[7] = state[7];
   out_8586997816965019240[8] = state[8];
}
void F_fun(double *state, double dt, double *out_832685054505812334) {
   out_832685054505812334[0] = 1;
   out_832685054505812334[1] = 0;
   out_832685054505812334[2] = 0;
   out_832685054505812334[3] = 0;
   out_832685054505812334[4] = 0;
   out_832685054505812334[5] = 0;
   out_832685054505812334[6] = 0;
   out_832685054505812334[7] = 0;
   out_832685054505812334[8] = 0;
   out_832685054505812334[9] = 0;
   out_832685054505812334[10] = 1;
   out_832685054505812334[11] = 0;
   out_832685054505812334[12] = 0;
   out_832685054505812334[13] = 0;
   out_832685054505812334[14] = 0;
   out_832685054505812334[15] = 0;
   out_832685054505812334[16] = 0;
   out_832685054505812334[17] = 0;
   out_832685054505812334[18] = 0;
   out_832685054505812334[19] = 0;
   out_832685054505812334[20] = 1;
   out_832685054505812334[21] = 0;
   out_832685054505812334[22] = 0;
   out_832685054505812334[23] = 0;
   out_832685054505812334[24] = 0;
   out_832685054505812334[25] = 0;
   out_832685054505812334[26] = 0;
   out_832685054505812334[27] = 0;
   out_832685054505812334[28] = 0;
   out_832685054505812334[29] = 0;
   out_832685054505812334[30] = 1;
   out_832685054505812334[31] = 0;
   out_832685054505812334[32] = 0;
   out_832685054505812334[33] = 0;
   out_832685054505812334[34] = 0;
   out_832685054505812334[35] = 0;
   out_832685054505812334[36] = 0;
   out_832685054505812334[37] = 0;
   out_832685054505812334[38] = 0;
   out_832685054505812334[39] = 0;
   out_832685054505812334[40] = 1;
   out_832685054505812334[41] = 0;
   out_832685054505812334[42] = 0;
   out_832685054505812334[43] = 0;
   out_832685054505812334[44] = 0;
   out_832685054505812334[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_832685054505812334[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_832685054505812334[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_832685054505812334[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_832685054505812334[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_832685054505812334[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_832685054505812334[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_832685054505812334[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_832685054505812334[53] = -9.8000000000000007*dt;
   out_832685054505812334[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_832685054505812334[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_832685054505812334[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_832685054505812334[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_832685054505812334[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_832685054505812334[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_832685054505812334[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_832685054505812334[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_832685054505812334[62] = 0;
   out_832685054505812334[63] = 0;
   out_832685054505812334[64] = 0;
   out_832685054505812334[65] = 0;
   out_832685054505812334[66] = 0;
   out_832685054505812334[67] = 0;
   out_832685054505812334[68] = 0;
   out_832685054505812334[69] = 0;
   out_832685054505812334[70] = 1;
   out_832685054505812334[71] = 0;
   out_832685054505812334[72] = 0;
   out_832685054505812334[73] = 0;
   out_832685054505812334[74] = 0;
   out_832685054505812334[75] = 0;
   out_832685054505812334[76] = 0;
   out_832685054505812334[77] = 0;
   out_832685054505812334[78] = 0;
   out_832685054505812334[79] = 0;
   out_832685054505812334[80] = 1;
}
void h_25(double *state, double *unused, double *out_8965051389129453778) {
   out_8965051389129453778[0] = state[6];
}
void H_25(double *state, double *unused, double *out_6637021278039698796) {
   out_6637021278039698796[0] = 0;
   out_6637021278039698796[1] = 0;
   out_6637021278039698796[2] = 0;
   out_6637021278039698796[3] = 0;
   out_6637021278039698796[4] = 0;
   out_6637021278039698796[5] = 0;
   out_6637021278039698796[6] = 1;
   out_6637021278039698796[7] = 0;
   out_6637021278039698796[8] = 0;
}
void h_24(double *state, double *unused, double *out_6184140826246986001) {
   out_6184140826246986001[0] = state[4];
   out_6184140826246986001[1] = state[5];
}
void H_24(double *state, double *unused, double *out_5234150989078334719) {
   out_5234150989078334719[0] = 0;
   out_5234150989078334719[1] = 0;
   out_5234150989078334719[2] = 0;
   out_5234150989078334719[3] = 0;
   out_5234150989078334719[4] = 1;
   out_5234150989078334719[5] = 0;
   out_5234150989078334719[6] = 0;
   out_5234150989078334719[7] = 0;
   out_5234150989078334719[8] = 0;
   out_5234150989078334719[9] = 0;
   out_5234150989078334719[10] = 0;
   out_5234150989078334719[11] = 0;
   out_5234150989078334719[12] = 0;
   out_5234150989078334719[13] = 0;
   out_5234150989078334719[14] = 1;
   out_5234150989078334719[15] = 0;
   out_5234150989078334719[16] = 0;
   out_5234150989078334719[17] = 0;
}
void h_30(double *state, double *unused, double *out_2632384913832442088) {
   out_2632384913832442088[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4893032454178236065) {
   out_4893032454178236065[0] = 0;
   out_4893032454178236065[1] = 0;
   out_4893032454178236065[2] = 0;
   out_4893032454178236065[3] = 0;
   out_4893032454178236065[4] = 1;
   out_4893032454178236065[5] = 0;
   out_4893032454178236065[6] = 0;
   out_4893032454178236065[7] = 0;
   out_4893032454178236065[8] = 0;
}
void h_26(double *state, double *unused, double *out_1792976001465789445) {
   out_1792976001465789445[0] = state[7];
}
void H_26(double *state, double *unused, double *out_8505196825909052219) {
   out_8505196825909052219[0] = 0;
   out_8505196825909052219[1] = 0;
   out_8505196825909052219[2] = 0;
   out_8505196825909052219[3] = 0;
   out_8505196825909052219[4] = 0;
   out_8505196825909052219[5] = 0;
   out_8505196825909052219[6] = 0;
   out_8505196825909052219[7] = 1;
   out_8505196825909052219[8] = 0;
}
void h_27(double *state, double *unused, double *out_247214469260761452) {
   out_247214469260761452[0] = state[3];
}
void H_27(double *state, double *unused, double *out_7067795765978660976) {
   out_7067795765978660976[0] = 0;
   out_7067795765978660976[1] = 0;
   out_7067795765978660976[2] = 0;
   out_7067795765978660976[3] = 1;
   out_7067795765978660976[4] = 0;
   out_7067795765978660976[5] = 0;
   out_7067795765978660976[6] = 0;
   out_7067795765978660976[7] = 0;
   out_7067795765978660976[8] = 0;
}
void h_29(double *state, double *unused, double *out_7018049695611112388) {
   out_7018049695611112388[0] = state[1];
}
void H_29(double *state, double *unused, double *out_4382801109863843881) {
   out_4382801109863843881[0] = 0;
   out_4382801109863843881[1] = 1;
   out_4382801109863843881[2] = 0;
   out_4382801109863843881[3] = 0;
   out_4382801109863843881[4] = 0;
   out_4382801109863843881[5] = 0;
   out_4382801109863843881[6] = 0;
   out_4382801109863843881[7] = 0;
   out_4382801109863843881[8] = 0;
}
void h_28(double *state, double *unused, double *out_5395515401820195383) {
   out_5395515401820195383[0] = state[0];
}
void H_28(double *state, double *unused, double *out_8981543946776177161) {
   out_8981543946776177161[0] = 1;
   out_8981543946776177161[1] = 0;
   out_8981543946776177161[2] = 0;
   out_8981543946776177161[3] = 0;
   out_8981543946776177161[4] = 0;
   out_8981543946776177161[5] = 0;
   out_8981543946776177161[6] = 0;
   out_8981543946776177161[7] = 0;
   out_8981543946776177161[8] = 0;
}
void h_31(double *state, double *unused, double *out_2514377519899065344) {
   out_2514377519899065344[0] = state[8];
}
void H_31(double *state, double *unused, double *out_6667667239916659224) {
   out_6667667239916659224[0] = 0;
   out_6667667239916659224[1] = 0;
   out_6667667239916659224[2] = 0;
   out_6667667239916659224[3] = 0;
   out_6667667239916659224[4] = 0;
   out_6667667239916659224[5] = 0;
   out_6667667239916659224[6] = 0;
   out_6667667239916659224[7] = 0;
   out_6667667239916659224[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_5610746733953880877) {
  err_fun(nom_x, delta_x, out_5610746733953880877);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_659775030920310167) {
  inv_err_fun(nom_x, true_x, out_659775030920310167);
}
void car_H_mod_fun(double *state, double *out_869202489952029923) {
  H_mod_fun(state, out_869202489952029923);
}
void car_f_fun(double *state, double dt, double *out_8586997816965019240) {
  f_fun(state,  dt, out_8586997816965019240);
}
void car_F_fun(double *state, double dt, double *out_832685054505812334) {
  F_fun(state,  dt, out_832685054505812334);
}
void car_h_25(double *state, double *unused, double *out_8965051389129453778) {
  h_25(state, unused, out_8965051389129453778);
}
void car_H_25(double *state, double *unused, double *out_6637021278039698796) {
  H_25(state, unused, out_6637021278039698796);
}
void car_h_24(double *state, double *unused, double *out_6184140826246986001) {
  h_24(state, unused, out_6184140826246986001);
}
void car_H_24(double *state, double *unused, double *out_5234150989078334719) {
  H_24(state, unused, out_5234150989078334719);
}
void car_h_30(double *state, double *unused, double *out_2632384913832442088) {
  h_30(state, unused, out_2632384913832442088);
}
void car_H_30(double *state, double *unused, double *out_4893032454178236065) {
  H_30(state, unused, out_4893032454178236065);
}
void car_h_26(double *state, double *unused, double *out_1792976001465789445) {
  h_26(state, unused, out_1792976001465789445);
}
void car_H_26(double *state, double *unused, double *out_8505196825909052219) {
  H_26(state, unused, out_8505196825909052219);
}
void car_h_27(double *state, double *unused, double *out_247214469260761452) {
  h_27(state, unused, out_247214469260761452);
}
void car_H_27(double *state, double *unused, double *out_7067795765978660976) {
  H_27(state, unused, out_7067795765978660976);
}
void car_h_29(double *state, double *unused, double *out_7018049695611112388) {
  h_29(state, unused, out_7018049695611112388);
}
void car_H_29(double *state, double *unused, double *out_4382801109863843881) {
  H_29(state, unused, out_4382801109863843881);
}
void car_h_28(double *state, double *unused, double *out_5395515401820195383) {
  h_28(state, unused, out_5395515401820195383);
}
void car_H_28(double *state, double *unused, double *out_8981543946776177161) {
  H_28(state, unused, out_8981543946776177161);
}
void car_h_31(double *state, double *unused, double *out_2514377519899065344) {
  h_31(state, unused, out_2514377519899065344);
}
void car_H_31(double *state, double *unused, double *out_6667667239916659224) {
  H_31(state, unused, out_6667667239916659224);
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
