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
void err_fun(double *nom_x, double *delta_x, double *out_7159053432330882537) {
   out_7159053432330882537[0] = delta_x[0] + nom_x[0];
   out_7159053432330882537[1] = delta_x[1] + nom_x[1];
   out_7159053432330882537[2] = delta_x[2] + nom_x[2];
   out_7159053432330882537[3] = delta_x[3] + nom_x[3];
   out_7159053432330882537[4] = delta_x[4] + nom_x[4];
   out_7159053432330882537[5] = delta_x[5] + nom_x[5];
   out_7159053432330882537[6] = delta_x[6] + nom_x[6];
   out_7159053432330882537[7] = delta_x[7] + nom_x[7];
   out_7159053432330882537[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1073064832695516914) {
   out_1073064832695516914[0] = -nom_x[0] + true_x[0];
   out_1073064832695516914[1] = -nom_x[1] + true_x[1];
   out_1073064832695516914[2] = -nom_x[2] + true_x[2];
   out_1073064832695516914[3] = -nom_x[3] + true_x[3];
   out_1073064832695516914[4] = -nom_x[4] + true_x[4];
   out_1073064832695516914[5] = -nom_x[5] + true_x[5];
   out_1073064832695516914[6] = -nom_x[6] + true_x[6];
   out_1073064832695516914[7] = -nom_x[7] + true_x[7];
   out_1073064832695516914[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_1645572812354200312) {
   out_1645572812354200312[0] = 1.0;
   out_1645572812354200312[1] = 0;
   out_1645572812354200312[2] = 0;
   out_1645572812354200312[3] = 0;
   out_1645572812354200312[4] = 0;
   out_1645572812354200312[5] = 0;
   out_1645572812354200312[6] = 0;
   out_1645572812354200312[7] = 0;
   out_1645572812354200312[8] = 0;
   out_1645572812354200312[9] = 0;
   out_1645572812354200312[10] = 1.0;
   out_1645572812354200312[11] = 0;
   out_1645572812354200312[12] = 0;
   out_1645572812354200312[13] = 0;
   out_1645572812354200312[14] = 0;
   out_1645572812354200312[15] = 0;
   out_1645572812354200312[16] = 0;
   out_1645572812354200312[17] = 0;
   out_1645572812354200312[18] = 0;
   out_1645572812354200312[19] = 0;
   out_1645572812354200312[20] = 1.0;
   out_1645572812354200312[21] = 0;
   out_1645572812354200312[22] = 0;
   out_1645572812354200312[23] = 0;
   out_1645572812354200312[24] = 0;
   out_1645572812354200312[25] = 0;
   out_1645572812354200312[26] = 0;
   out_1645572812354200312[27] = 0;
   out_1645572812354200312[28] = 0;
   out_1645572812354200312[29] = 0;
   out_1645572812354200312[30] = 1.0;
   out_1645572812354200312[31] = 0;
   out_1645572812354200312[32] = 0;
   out_1645572812354200312[33] = 0;
   out_1645572812354200312[34] = 0;
   out_1645572812354200312[35] = 0;
   out_1645572812354200312[36] = 0;
   out_1645572812354200312[37] = 0;
   out_1645572812354200312[38] = 0;
   out_1645572812354200312[39] = 0;
   out_1645572812354200312[40] = 1.0;
   out_1645572812354200312[41] = 0;
   out_1645572812354200312[42] = 0;
   out_1645572812354200312[43] = 0;
   out_1645572812354200312[44] = 0;
   out_1645572812354200312[45] = 0;
   out_1645572812354200312[46] = 0;
   out_1645572812354200312[47] = 0;
   out_1645572812354200312[48] = 0;
   out_1645572812354200312[49] = 0;
   out_1645572812354200312[50] = 1.0;
   out_1645572812354200312[51] = 0;
   out_1645572812354200312[52] = 0;
   out_1645572812354200312[53] = 0;
   out_1645572812354200312[54] = 0;
   out_1645572812354200312[55] = 0;
   out_1645572812354200312[56] = 0;
   out_1645572812354200312[57] = 0;
   out_1645572812354200312[58] = 0;
   out_1645572812354200312[59] = 0;
   out_1645572812354200312[60] = 1.0;
   out_1645572812354200312[61] = 0;
   out_1645572812354200312[62] = 0;
   out_1645572812354200312[63] = 0;
   out_1645572812354200312[64] = 0;
   out_1645572812354200312[65] = 0;
   out_1645572812354200312[66] = 0;
   out_1645572812354200312[67] = 0;
   out_1645572812354200312[68] = 0;
   out_1645572812354200312[69] = 0;
   out_1645572812354200312[70] = 1.0;
   out_1645572812354200312[71] = 0;
   out_1645572812354200312[72] = 0;
   out_1645572812354200312[73] = 0;
   out_1645572812354200312[74] = 0;
   out_1645572812354200312[75] = 0;
   out_1645572812354200312[76] = 0;
   out_1645572812354200312[77] = 0;
   out_1645572812354200312[78] = 0;
   out_1645572812354200312[79] = 0;
   out_1645572812354200312[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_2346849298263271361) {
   out_2346849298263271361[0] = state[0];
   out_2346849298263271361[1] = state[1];
   out_2346849298263271361[2] = state[2];
   out_2346849298263271361[3] = state[3];
   out_2346849298263271361[4] = state[4];
   out_2346849298263271361[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_2346849298263271361[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_2346849298263271361[7] = state[7];
   out_2346849298263271361[8] = state[8];
}
void F_fun(double *state, double dt, double *out_5087158014064977170) {
   out_5087158014064977170[0] = 1;
   out_5087158014064977170[1] = 0;
   out_5087158014064977170[2] = 0;
   out_5087158014064977170[3] = 0;
   out_5087158014064977170[4] = 0;
   out_5087158014064977170[5] = 0;
   out_5087158014064977170[6] = 0;
   out_5087158014064977170[7] = 0;
   out_5087158014064977170[8] = 0;
   out_5087158014064977170[9] = 0;
   out_5087158014064977170[10] = 1;
   out_5087158014064977170[11] = 0;
   out_5087158014064977170[12] = 0;
   out_5087158014064977170[13] = 0;
   out_5087158014064977170[14] = 0;
   out_5087158014064977170[15] = 0;
   out_5087158014064977170[16] = 0;
   out_5087158014064977170[17] = 0;
   out_5087158014064977170[18] = 0;
   out_5087158014064977170[19] = 0;
   out_5087158014064977170[20] = 1;
   out_5087158014064977170[21] = 0;
   out_5087158014064977170[22] = 0;
   out_5087158014064977170[23] = 0;
   out_5087158014064977170[24] = 0;
   out_5087158014064977170[25] = 0;
   out_5087158014064977170[26] = 0;
   out_5087158014064977170[27] = 0;
   out_5087158014064977170[28] = 0;
   out_5087158014064977170[29] = 0;
   out_5087158014064977170[30] = 1;
   out_5087158014064977170[31] = 0;
   out_5087158014064977170[32] = 0;
   out_5087158014064977170[33] = 0;
   out_5087158014064977170[34] = 0;
   out_5087158014064977170[35] = 0;
   out_5087158014064977170[36] = 0;
   out_5087158014064977170[37] = 0;
   out_5087158014064977170[38] = 0;
   out_5087158014064977170[39] = 0;
   out_5087158014064977170[40] = 1;
   out_5087158014064977170[41] = 0;
   out_5087158014064977170[42] = 0;
   out_5087158014064977170[43] = 0;
   out_5087158014064977170[44] = 0;
   out_5087158014064977170[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_5087158014064977170[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_5087158014064977170[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_5087158014064977170[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_5087158014064977170[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_5087158014064977170[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_5087158014064977170[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_5087158014064977170[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_5087158014064977170[53] = -9.8000000000000007*dt;
   out_5087158014064977170[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_5087158014064977170[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_5087158014064977170[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5087158014064977170[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5087158014064977170[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_5087158014064977170[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_5087158014064977170[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_5087158014064977170[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5087158014064977170[62] = 0;
   out_5087158014064977170[63] = 0;
   out_5087158014064977170[64] = 0;
   out_5087158014064977170[65] = 0;
   out_5087158014064977170[66] = 0;
   out_5087158014064977170[67] = 0;
   out_5087158014064977170[68] = 0;
   out_5087158014064977170[69] = 0;
   out_5087158014064977170[70] = 1;
   out_5087158014064977170[71] = 0;
   out_5087158014064977170[72] = 0;
   out_5087158014064977170[73] = 0;
   out_5087158014064977170[74] = 0;
   out_5087158014064977170[75] = 0;
   out_5087158014064977170[76] = 0;
   out_5087158014064977170[77] = 0;
   out_5087158014064977170[78] = 0;
   out_5087158014064977170[79] = 0;
   out_5087158014064977170[80] = 1;
}
void h_25(double *state, double *unused, double *out_870579470757970545) {
   out_870579470757970545[0] = state[6];
}
void H_25(double *state, double *unused, double *out_36661252871310026) {
   out_36661252871310026[0] = 0;
   out_36661252871310026[1] = 0;
   out_36661252871310026[2] = 0;
   out_36661252871310026[3] = 0;
   out_36661252871310026[4] = 0;
   out_36661252871310026[5] = 0;
   out_36661252871310026[6] = 1;
   out_36661252871310026[7] = 0;
   out_36661252871310026[8] = 0;
}
void h_24(double *state, double *unused, double *out_3746370680203545777) {
   out_3746370680203545777[0] = state[4];
   out_3746370680203545777[1] = state[5];
}
void H_24(double *state, double *unused, double *out_3521264345931013110) {
   out_3521264345931013110[0] = 0;
   out_3521264345931013110[1] = 0;
   out_3521264345931013110[2] = 0;
   out_3521264345931013110[3] = 0;
   out_3521264345931013110[4] = 1;
   out_3521264345931013110[5] = 0;
   out_3521264345931013110[6] = 0;
   out_3521264345931013110[7] = 0;
   out_3521264345931013110[8] = 0;
   out_3521264345931013110[9] = 0;
   out_3521264345931013110[10] = 0;
   out_3521264345931013110[11] = 0;
   out_3521264345931013110[12] = 0;
   out_3521264345931013110[13] = 0;
   out_3521264345931013110[14] = 1;
   out_3521264345931013110[15] = 0;
   out_3521264345931013110[16] = 0;
   out_3521264345931013110[17] = 0;
}
void h_30(double *state, double *unused, double *out_595385408473464656) {
   out_595385408473464656[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4564357582998918224) {
   out_4564357582998918224[0] = 0;
   out_4564357582998918224[1] = 0;
   out_4564357582998918224[2] = 0;
   out_4564357582998918224[3] = 0;
   out_4564357582998918224[4] = 1;
   out_4564357582998918224[5] = 0;
   out_4564357582998918224[6] = 0;
   out_4564357582998918224[7] = 0;
   out_4564357582998918224[8] = 0;
}
void h_26(double *state, double *unused, double *out_5453127054407331223) {
   out_5453127054407331223[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3778164571745366250) {
   out_3778164571745366250[0] = 0;
   out_3778164571745366250[1] = 0;
   out_3778164571745366250[2] = 0;
   out_3778164571745366250[3] = 0;
   out_3778164571745366250[4] = 0;
   out_3778164571745366250[5] = 0;
   out_3778164571745366250[6] = 0;
   out_3778164571745366250[7] = 1;
   out_3778164571745366250[8] = 0;
}
void h_27(double *state, double *unused, double *out_4131093796902875100) {
   out_4131093796902875100[0] = state[3];
}
void H_27(double *state, double *unused, double *out_2340763511814975007) {
   out_2340763511814975007[0] = 0;
   out_2340763511814975007[1] = 0;
   out_2340763511814975007[2] = 0;
   out_2340763511814975007[3] = 1;
   out_2340763511814975007[4] = 0;
   out_2340763511814975007[5] = 0;
   out_2340763511814975007[6] = 0;
   out_2340763511814975007[7] = 0;
   out_2340763511814975007[8] = 0;
}
void h_29(double *state, double *unused, double *out_3632171459917797341) {
   out_3632171459917797341[0] = state[1];
}
void H_29(double *state, double *unused, double *out_4054126238684526040) {
   out_4054126238684526040[0] = 0;
   out_4054126238684526040[1] = 1;
   out_4054126238684526040[2] = 0;
   out_4054126238684526040[3] = 0;
   out_4054126238684526040[4] = 0;
   out_4054126238684526040[5] = 0;
   out_4054126238684526040[6] = 0;
   out_4054126238684526040[7] = 0;
   out_4054126238684526040[8] = 0;
}
void h_28(double *state, double *unused, double *out_1626891117090862856) {
   out_1626891117090862856[0] = state[0];
}
void H_28(double *state, double *unused, double *out_9136525255754056614) {
   out_9136525255754056614[0] = 1;
   out_9136525255754056614[1] = 0;
   out_9136525255754056614[2] = 0;
   out_9136525255754056614[3] = 0;
   out_9136525255754056614[4] = 0;
   out_9136525255754056614[5] = 0;
   out_9136525255754056614[6] = 0;
   out_9136525255754056614[7] = 0;
   out_9136525255754056614[8] = 0;
}
void h_31(double *state, double *unused, double *out_4508028999011992895) {
   out_4508028999011992895[0] = state[8];
}
void H_31(double *state, double *unused, double *out_6015290994349598) {
   out_6015290994349598[0] = 0;
   out_6015290994349598[1] = 0;
   out_6015290994349598[2] = 0;
   out_6015290994349598[3] = 0;
   out_6015290994349598[4] = 0;
   out_6015290994349598[5] = 0;
   out_6015290994349598[6] = 0;
   out_6015290994349598[7] = 0;
   out_6015290994349598[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_7159053432330882537) {
  err_fun(nom_x, delta_x, out_7159053432330882537);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1073064832695516914) {
  inv_err_fun(nom_x, true_x, out_1073064832695516914);
}
void car_H_mod_fun(double *state, double *out_1645572812354200312) {
  H_mod_fun(state, out_1645572812354200312);
}
void car_f_fun(double *state, double dt, double *out_2346849298263271361) {
  f_fun(state,  dt, out_2346849298263271361);
}
void car_F_fun(double *state, double dt, double *out_5087158014064977170) {
  F_fun(state,  dt, out_5087158014064977170);
}
void car_h_25(double *state, double *unused, double *out_870579470757970545) {
  h_25(state, unused, out_870579470757970545);
}
void car_H_25(double *state, double *unused, double *out_36661252871310026) {
  H_25(state, unused, out_36661252871310026);
}
void car_h_24(double *state, double *unused, double *out_3746370680203545777) {
  h_24(state, unused, out_3746370680203545777);
}
void car_H_24(double *state, double *unused, double *out_3521264345931013110) {
  H_24(state, unused, out_3521264345931013110);
}
void car_h_30(double *state, double *unused, double *out_595385408473464656) {
  h_30(state, unused, out_595385408473464656);
}
void car_H_30(double *state, double *unused, double *out_4564357582998918224) {
  H_30(state, unused, out_4564357582998918224);
}
void car_h_26(double *state, double *unused, double *out_5453127054407331223) {
  h_26(state, unused, out_5453127054407331223);
}
void car_H_26(double *state, double *unused, double *out_3778164571745366250) {
  H_26(state, unused, out_3778164571745366250);
}
void car_h_27(double *state, double *unused, double *out_4131093796902875100) {
  h_27(state, unused, out_4131093796902875100);
}
void car_H_27(double *state, double *unused, double *out_2340763511814975007) {
  H_27(state, unused, out_2340763511814975007);
}
void car_h_29(double *state, double *unused, double *out_3632171459917797341) {
  h_29(state, unused, out_3632171459917797341);
}
void car_H_29(double *state, double *unused, double *out_4054126238684526040) {
  H_29(state, unused, out_4054126238684526040);
}
void car_h_28(double *state, double *unused, double *out_1626891117090862856) {
  h_28(state, unused, out_1626891117090862856);
}
void car_H_28(double *state, double *unused, double *out_9136525255754056614) {
  H_28(state, unused, out_9136525255754056614);
}
void car_h_31(double *state, double *unused, double *out_4508028999011992895) {
  h_31(state, unused, out_4508028999011992895);
}
void car_H_31(double *state, double *unused, double *out_6015290994349598) {
  H_31(state, unused, out_6015290994349598);
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
