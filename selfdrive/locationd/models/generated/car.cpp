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
void err_fun(double *nom_x, double *delta_x, double *out_7226686656405340864) {
   out_7226686656405340864[0] = delta_x[0] + nom_x[0];
   out_7226686656405340864[1] = delta_x[1] + nom_x[1];
   out_7226686656405340864[2] = delta_x[2] + nom_x[2];
   out_7226686656405340864[3] = delta_x[3] + nom_x[3];
   out_7226686656405340864[4] = delta_x[4] + nom_x[4];
   out_7226686656405340864[5] = delta_x[5] + nom_x[5];
   out_7226686656405340864[6] = delta_x[6] + nom_x[6];
   out_7226686656405340864[7] = delta_x[7] + nom_x[7];
   out_7226686656405340864[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7867434532787478357) {
   out_7867434532787478357[0] = -nom_x[0] + true_x[0];
   out_7867434532787478357[1] = -nom_x[1] + true_x[1];
   out_7867434532787478357[2] = -nom_x[2] + true_x[2];
   out_7867434532787478357[3] = -nom_x[3] + true_x[3];
   out_7867434532787478357[4] = -nom_x[4] + true_x[4];
   out_7867434532787478357[5] = -nom_x[5] + true_x[5];
   out_7867434532787478357[6] = -nom_x[6] + true_x[6];
   out_7867434532787478357[7] = -nom_x[7] + true_x[7];
   out_7867434532787478357[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_3288405267109084725) {
   out_3288405267109084725[0] = 1.0;
   out_3288405267109084725[1] = 0;
   out_3288405267109084725[2] = 0;
   out_3288405267109084725[3] = 0;
   out_3288405267109084725[4] = 0;
   out_3288405267109084725[5] = 0;
   out_3288405267109084725[6] = 0;
   out_3288405267109084725[7] = 0;
   out_3288405267109084725[8] = 0;
   out_3288405267109084725[9] = 0;
   out_3288405267109084725[10] = 1.0;
   out_3288405267109084725[11] = 0;
   out_3288405267109084725[12] = 0;
   out_3288405267109084725[13] = 0;
   out_3288405267109084725[14] = 0;
   out_3288405267109084725[15] = 0;
   out_3288405267109084725[16] = 0;
   out_3288405267109084725[17] = 0;
   out_3288405267109084725[18] = 0;
   out_3288405267109084725[19] = 0;
   out_3288405267109084725[20] = 1.0;
   out_3288405267109084725[21] = 0;
   out_3288405267109084725[22] = 0;
   out_3288405267109084725[23] = 0;
   out_3288405267109084725[24] = 0;
   out_3288405267109084725[25] = 0;
   out_3288405267109084725[26] = 0;
   out_3288405267109084725[27] = 0;
   out_3288405267109084725[28] = 0;
   out_3288405267109084725[29] = 0;
   out_3288405267109084725[30] = 1.0;
   out_3288405267109084725[31] = 0;
   out_3288405267109084725[32] = 0;
   out_3288405267109084725[33] = 0;
   out_3288405267109084725[34] = 0;
   out_3288405267109084725[35] = 0;
   out_3288405267109084725[36] = 0;
   out_3288405267109084725[37] = 0;
   out_3288405267109084725[38] = 0;
   out_3288405267109084725[39] = 0;
   out_3288405267109084725[40] = 1.0;
   out_3288405267109084725[41] = 0;
   out_3288405267109084725[42] = 0;
   out_3288405267109084725[43] = 0;
   out_3288405267109084725[44] = 0;
   out_3288405267109084725[45] = 0;
   out_3288405267109084725[46] = 0;
   out_3288405267109084725[47] = 0;
   out_3288405267109084725[48] = 0;
   out_3288405267109084725[49] = 0;
   out_3288405267109084725[50] = 1.0;
   out_3288405267109084725[51] = 0;
   out_3288405267109084725[52] = 0;
   out_3288405267109084725[53] = 0;
   out_3288405267109084725[54] = 0;
   out_3288405267109084725[55] = 0;
   out_3288405267109084725[56] = 0;
   out_3288405267109084725[57] = 0;
   out_3288405267109084725[58] = 0;
   out_3288405267109084725[59] = 0;
   out_3288405267109084725[60] = 1.0;
   out_3288405267109084725[61] = 0;
   out_3288405267109084725[62] = 0;
   out_3288405267109084725[63] = 0;
   out_3288405267109084725[64] = 0;
   out_3288405267109084725[65] = 0;
   out_3288405267109084725[66] = 0;
   out_3288405267109084725[67] = 0;
   out_3288405267109084725[68] = 0;
   out_3288405267109084725[69] = 0;
   out_3288405267109084725[70] = 1.0;
   out_3288405267109084725[71] = 0;
   out_3288405267109084725[72] = 0;
   out_3288405267109084725[73] = 0;
   out_3288405267109084725[74] = 0;
   out_3288405267109084725[75] = 0;
   out_3288405267109084725[76] = 0;
   out_3288405267109084725[77] = 0;
   out_3288405267109084725[78] = 0;
   out_3288405267109084725[79] = 0;
   out_3288405267109084725[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_8974995752329095985) {
   out_8974995752329095985[0] = state[0];
   out_8974995752329095985[1] = state[1];
   out_8974995752329095985[2] = state[2];
   out_8974995752329095985[3] = state[3];
   out_8974995752329095985[4] = state[4];
   out_8974995752329095985[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_8974995752329095985[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_8974995752329095985[7] = state[7];
   out_8974995752329095985[8] = state[8];
}
void F_fun(double *state, double dt, double *out_2278957145959582876) {
   out_2278957145959582876[0] = 1;
   out_2278957145959582876[1] = 0;
   out_2278957145959582876[2] = 0;
   out_2278957145959582876[3] = 0;
   out_2278957145959582876[4] = 0;
   out_2278957145959582876[5] = 0;
   out_2278957145959582876[6] = 0;
   out_2278957145959582876[7] = 0;
   out_2278957145959582876[8] = 0;
   out_2278957145959582876[9] = 0;
   out_2278957145959582876[10] = 1;
   out_2278957145959582876[11] = 0;
   out_2278957145959582876[12] = 0;
   out_2278957145959582876[13] = 0;
   out_2278957145959582876[14] = 0;
   out_2278957145959582876[15] = 0;
   out_2278957145959582876[16] = 0;
   out_2278957145959582876[17] = 0;
   out_2278957145959582876[18] = 0;
   out_2278957145959582876[19] = 0;
   out_2278957145959582876[20] = 1;
   out_2278957145959582876[21] = 0;
   out_2278957145959582876[22] = 0;
   out_2278957145959582876[23] = 0;
   out_2278957145959582876[24] = 0;
   out_2278957145959582876[25] = 0;
   out_2278957145959582876[26] = 0;
   out_2278957145959582876[27] = 0;
   out_2278957145959582876[28] = 0;
   out_2278957145959582876[29] = 0;
   out_2278957145959582876[30] = 1;
   out_2278957145959582876[31] = 0;
   out_2278957145959582876[32] = 0;
   out_2278957145959582876[33] = 0;
   out_2278957145959582876[34] = 0;
   out_2278957145959582876[35] = 0;
   out_2278957145959582876[36] = 0;
   out_2278957145959582876[37] = 0;
   out_2278957145959582876[38] = 0;
   out_2278957145959582876[39] = 0;
   out_2278957145959582876[40] = 1;
   out_2278957145959582876[41] = 0;
   out_2278957145959582876[42] = 0;
   out_2278957145959582876[43] = 0;
   out_2278957145959582876[44] = 0;
   out_2278957145959582876[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_2278957145959582876[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_2278957145959582876[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2278957145959582876[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2278957145959582876[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_2278957145959582876[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_2278957145959582876[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_2278957145959582876[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_2278957145959582876[53] = -9.8000000000000007*dt;
   out_2278957145959582876[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_2278957145959582876[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_2278957145959582876[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2278957145959582876[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2278957145959582876[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_2278957145959582876[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_2278957145959582876[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_2278957145959582876[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2278957145959582876[62] = 0;
   out_2278957145959582876[63] = 0;
   out_2278957145959582876[64] = 0;
   out_2278957145959582876[65] = 0;
   out_2278957145959582876[66] = 0;
   out_2278957145959582876[67] = 0;
   out_2278957145959582876[68] = 0;
   out_2278957145959582876[69] = 0;
   out_2278957145959582876[70] = 1;
   out_2278957145959582876[71] = 0;
   out_2278957145959582876[72] = 0;
   out_2278957145959582876[73] = 0;
   out_2278957145959582876[74] = 0;
   out_2278957145959582876[75] = 0;
   out_2278957145959582876[76] = 0;
   out_2278957145959582876[77] = 0;
   out_2278957145959582876[78] = 0;
   out_2278957145959582876[79] = 0;
   out_2278957145959582876[80] = 1;
}
void h_25(double *state, double *unused, double *out_6677369844743091603) {
   out_6677369844743091603[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2625808263069423248) {
   out_2625808263069423248[0] = 0;
   out_2625808263069423248[1] = 0;
   out_2625808263069423248[2] = 0;
   out_2625808263069423248[3] = 0;
   out_2625808263069423248[4] = 0;
   out_2625808263069423248[5] = 0;
   out_2625808263069423248[6] = 1;
   out_2625808263069423248[7] = 0;
   out_2625808263069423248[8] = 0;
}
void h_24(double *state, double *unused, double *out_4367008819681309639) {
   out_4367008819681309639[0] = state[4];
   out_4367008819681309639[1] = state[5];
}
void H_24(double *state, double *unused, double *out_4798457862074922814) {
   out_4798457862074922814[0] = 0;
   out_4798457862074922814[1] = 0;
   out_4798457862074922814[2] = 0;
   out_4798457862074922814[3] = 0;
   out_4798457862074922814[4] = 1;
   out_4798457862074922814[5] = 0;
   out_4798457862074922814[6] = 0;
   out_4798457862074922814[7] = 0;
   out_4798457862074922814[8] = 0;
   out_4798457862074922814[9] = 0;
   out_4798457862074922814[10] = 0;
   out_4798457862074922814[11] = 0;
   out_4798457862074922814[12] = 0;
   out_4798457862074922814[13] = 0;
   out_4798457862074922814[14] = 1;
   out_4798457862074922814[15] = 0;
   out_4798457862074922814[16] = 0;
   out_4798457862074922814[17] = 0;
}
void h_30(double *state, double *unused, double *out_6520183176391962458) {
   out_6520183176391962458[0] = state[4];
}
void H_30(double *state, double *unused, double *out_107475304562174621) {
   out_107475304562174621[0] = 0;
   out_107475304562174621[1] = 0;
   out_107475304562174621[2] = 0;
   out_107475304562174621[3] = 0;
   out_107475304562174621[4] = 1;
   out_107475304562174621[5] = 0;
   out_107475304562174621[6] = 0;
   out_107475304562174621[7] = 0;
   out_107475304562174621[8] = 0;
}
void h_26(double *state, double *unused, double *out_1518509908731116813) {
   out_1518509908731116813[0] = state[7];
}
void H_26(double *state, double *unused, double *out_6367311581943479472) {
   out_6367311581943479472[0] = 0;
   out_6367311581943479472[1] = 0;
   out_6367311581943479472[2] = 0;
   out_6367311581943479472[3] = 0;
   out_6367311581943479472[4] = 0;
   out_6367311581943479472[5] = 0;
   out_6367311581943479472[6] = 0;
   out_6367311581943479472[7] = 1;
   out_6367311581943479472[8] = 0;
}
void h_27(double *state, double *unused, double *out_757773589685609210) {
   out_757773589685609210[0] = state[3];
}
void H_27(double *state, double *unused, double *out_9118476168712095259) {
   out_9118476168712095259[0] = 0;
   out_9118476168712095259[1] = 0;
   out_9118476168712095259[2] = 0;
   out_9118476168712095259[3] = 1;
   out_9118476168712095259[4] = 0;
   out_9118476168712095259[5] = 0;
   out_9118476168712095259[6] = 0;
   out_9118476168712095259[7] = 0;
   out_9118476168712095259[8] = 0;
}
void h_29(double *state, double *unused, double *out_482579527401103321) {
   out_482579527401103321[0] = state[1];
}
void H_29(double *state, double *unused, double *out_6643273248882639262) {
   out_6643273248882639262[0] = 0;
   out_6643273248882639262[1] = 1;
   out_6643273248882639262[2] = 0;
   out_6643273248882639262[3] = 0;
   out_6643273248882639262[4] = 0;
   out_6643273248882639262[5] = 0;
   out_6643273248882639262[6] = 0;
   out_6643273248882639262[7] = 0;
   out_6643273248882639262[8] = 0;
}
void h_28(double *state, double *unused, double *out_1701283442587271645) {
   out_1701283442587271645[0] = state[0];
}
void H_28(double *state, double *unused, double *out_4679642977317313011) {
   out_4679642977317313011[0] = 1;
   out_4679642977317313011[1] = 0;
   out_4679642977317313011[2] = 0;
   out_4679642977317313011[3] = 0;
   out_4679642977317313011[4] = 0;
   out_4679642977317313011[5] = 0;
   out_4679642977317313011[6] = 0;
   out_4679642977317313011[7] = 0;
   out_4679642977317313011[8] = 0;
}
void h_31(double *state, double *unused, double *out_4998539002616109077) {
   out_4998539002616109077[0] = state[8];
}
void H_31(double *state, double *unused, double *out_6993519684176830948) {
   out_6993519684176830948[0] = 0;
   out_6993519684176830948[1] = 0;
   out_6993519684176830948[2] = 0;
   out_6993519684176830948[3] = 0;
   out_6993519684176830948[4] = 0;
   out_6993519684176830948[5] = 0;
   out_6993519684176830948[6] = 0;
   out_6993519684176830948[7] = 0;
   out_6993519684176830948[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_7226686656405340864) {
  err_fun(nom_x, delta_x, out_7226686656405340864);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7867434532787478357) {
  inv_err_fun(nom_x, true_x, out_7867434532787478357);
}
void car_H_mod_fun(double *state, double *out_3288405267109084725) {
  H_mod_fun(state, out_3288405267109084725);
}
void car_f_fun(double *state, double dt, double *out_8974995752329095985) {
  f_fun(state,  dt, out_8974995752329095985);
}
void car_F_fun(double *state, double dt, double *out_2278957145959582876) {
  F_fun(state,  dt, out_2278957145959582876);
}
void car_h_25(double *state, double *unused, double *out_6677369844743091603) {
  h_25(state, unused, out_6677369844743091603);
}
void car_H_25(double *state, double *unused, double *out_2625808263069423248) {
  H_25(state, unused, out_2625808263069423248);
}
void car_h_24(double *state, double *unused, double *out_4367008819681309639) {
  h_24(state, unused, out_4367008819681309639);
}
void car_H_24(double *state, double *unused, double *out_4798457862074922814) {
  H_24(state, unused, out_4798457862074922814);
}
void car_h_30(double *state, double *unused, double *out_6520183176391962458) {
  h_30(state, unused, out_6520183176391962458);
}
void car_H_30(double *state, double *unused, double *out_107475304562174621) {
  H_30(state, unused, out_107475304562174621);
}
void car_h_26(double *state, double *unused, double *out_1518509908731116813) {
  h_26(state, unused, out_1518509908731116813);
}
void car_H_26(double *state, double *unused, double *out_6367311581943479472) {
  H_26(state, unused, out_6367311581943479472);
}
void car_h_27(double *state, double *unused, double *out_757773589685609210) {
  h_27(state, unused, out_757773589685609210);
}
void car_H_27(double *state, double *unused, double *out_9118476168712095259) {
  H_27(state, unused, out_9118476168712095259);
}
void car_h_29(double *state, double *unused, double *out_482579527401103321) {
  h_29(state, unused, out_482579527401103321);
}
void car_H_29(double *state, double *unused, double *out_6643273248882639262) {
  H_29(state, unused, out_6643273248882639262);
}
void car_h_28(double *state, double *unused, double *out_1701283442587271645) {
  h_28(state, unused, out_1701283442587271645);
}
void car_H_28(double *state, double *unused, double *out_4679642977317313011) {
  H_28(state, unused, out_4679642977317313011);
}
void car_h_31(double *state, double *unused, double *out_4998539002616109077) {
  h_31(state, unused, out_4998539002616109077);
}
void car_H_31(double *state, double *unused, double *out_6993519684176830948) {
  H_31(state, unused, out_6993519684176830948);
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
