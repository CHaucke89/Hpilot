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
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_3188021722861230961) {
   out_3188021722861230961[0] = delta_x[0] + nom_x[0];
   out_3188021722861230961[1] = delta_x[1] + nom_x[1];
   out_3188021722861230961[2] = delta_x[2] + nom_x[2];
   out_3188021722861230961[3] = delta_x[3] + nom_x[3];
   out_3188021722861230961[4] = delta_x[4] + nom_x[4];
   out_3188021722861230961[5] = delta_x[5] + nom_x[5];
   out_3188021722861230961[6] = delta_x[6] + nom_x[6];
   out_3188021722861230961[7] = delta_x[7] + nom_x[7];
   out_3188021722861230961[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1354812413488475165) {
   out_1354812413488475165[0] = -nom_x[0] + true_x[0];
   out_1354812413488475165[1] = -nom_x[1] + true_x[1];
   out_1354812413488475165[2] = -nom_x[2] + true_x[2];
   out_1354812413488475165[3] = -nom_x[3] + true_x[3];
   out_1354812413488475165[4] = -nom_x[4] + true_x[4];
   out_1354812413488475165[5] = -nom_x[5] + true_x[5];
   out_1354812413488475165[6] = -nom_x[6] + true_x[6];
   out_1354812413488475165[7] = -nom_x[7] + true_x[7];
   out_1354812413488475165[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_486694020902820743) {
   out_486694020902820743[0] = 1.0;
   out_486694020902820743[1] = 0.0;
   out_486694020902820743[2] = 0.0;
   out_486694020902820743[3] = 0.0;
   out_486694020902820743[4] = 0.0;
   out_486694020902820743[5] = 0.0;
   out_486694020902820743[6] = 0.0;
   out_486694020902820743[7] = 0.0;
   out_486694020902820743[8] = 0.0;
   out_486694020902820743[9] = 0.0;
   out_486694020902820743[10] = 1.0;
   out_486694020902820743[11] = 0.0;
   out_486694020902820743[12] = 0.0;
   out_486694020902820743[13] = 0.0;
   out_486694020902820743[14] = 0.0;
   out_486694020902820743[15] = 0.0;
   out_486694020902820743[16] = 0.0;
   out_486694020902820743[17] = 0.0;
   out_486694020902820743[18] = 0.0;
   out_486694020902820743[19] = 0.0;
   out_486694020902820743[20] = 1.0;
   out_486694020902820743[21] = 0.0;
   out_486694020902820743[22] = 0.0;
   out_486694020902820743[23] = 0.0;
   out_486694020902820743[24] = 0.0;
   out_486694020902820743[25] = 0.0;
   out_486694020902820743[26] = 0.0;
   out_486694020902820743[27] = 0.0;
   out_486694020902820743[28] = 0.0;
   out_486694020902820743[29] = 0.0;
   out_486694020902820743[30] = 1.0;
   out_486694020902820743[31] = 0.0;
   out_486694020902820743[32] = 0.0;
   out_486694020902820743[33] = 0.0;
   out_486694020902820743[34] = 0.0;
   out_486694020902820743[35] = 0.0;
   out_486694020902820743[36] = 0.0;
   out_486694020902820743[37] = 0.0;
   out_486694020902820743[38] = 0.0;
   out_486694020902820743[39] = 0.0;
   out_486694020902820743[40] = 1.0;
   out_486694020902820743[41] = 0.0;
   out_486694020902820743[42] = 0.0;
   out_486694020902820743[43] = 0.0;
   out_486694020902820743[44] = 0.0;
   out_486694020902820743[45] = 0.0;
   out_486694020902820743[46] = 0.0;
   out_486694020902820743[47] = 0.0;
   out_486694020902820743[48] = 0.0;
   out_486694020902820743[49] = 0.0;
   out_486694020902820743[50] = 1.0;
   out_486694020902820743[51] = 0.0;
   out_486694020902820743[52] = 0.0;
   out_486694020902820743[53] = 0.0;
   out_486694020902820743[54] = 0.0;
   out_486694020902820743[55] = 0.0;
   out_486694020902820743[56] = 0.0;
   out_486694020902820743[57] = 0.0;
   out_486694020902820743[58] = 0.0;
   out_486694020902820743[59] = 0.0;
   out_486694020902820743[60] = 1.0;
   out_486694020902820743[61] = 0.0;
   out_486694020902820743[62] = 0.0;
   out_486694020902820743[63] = 0.0;
   out_486694020902820743[64] = 0.0;
   out_486694020902820743[65] = 0.0;
   out_486694020902820743[66] = 0.0;
   out_486694020902820743[67] = 0.0;
   out_486694020902820743[68] = 0.0;
   out_486694020902820743[69] = 0.0;
   out_486694020902820743[70] = 1.0;
   out_486694020902820743[71] = 0.0;
   out_486694020902820743[72] = 0.0;
   out_486694020902820743[73] = 0.0;
   out_486694020902820743[74] = 0.0;
   out_486694020902820743[75] = 0.0;
   out_486694020902820743[76] = 0.0;
   out_486694020902820743[77] = 0.0;
   out_486694020902820743[78] = 0.0;
   out_486694020902820743[79] = 0.0;
   out_486694020902820743[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3031257425454158590) {
   out_3031257425454158590[0] = state[0];
   out_3031257425454158590[1] = state[1];
   out_3031257425454158590[2] = state[2];
   out_3031257425454158590[3] = state[3];
   out_3031257425454158590[4] = state[4];
   out_3031257425454158590[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3031257425454158590[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3031257425454158590[7] = state[7];
   out_3031257425454158590[8] = state[8];
}
void F_fun(double *state, double dt, double *out_6559781685228537124) {
   out_6559781685228537124[0] = 1;
   out_6559781685228537124[1] = 0;
   out_6559781685228537124[2] = 0;
   out_6559781685228537124[3] = 0;
   out_6559781685228537124[4] = 0;
   out_6559781685228537124[5] = 0;
   out_6559781685228537124[6] = 0;
   out_6559781685228537124[7] = 0;
   out_6559781685228537124[8] = 0;
   out_6559781685228537124[9] = 0;
   out_6559781685228537124[10] = 1;
   out_6559781685228537124[11] = 0;
   out_6559781685228537124[12] = 0;
   out_6559781685228537124[13] = 0;
   out_6559781685228537124[14] = 0;
   out_6559781685228537124[15] = 0;
   out_6559781685228537124[16] = 0;
   out_6559781685228537124[17] = 0;
   out_6559781685228537124[18] = 0;
   out_6559781685228537124[19] = 0;
   out_6559781685228537124[20] = 1;
   out_6559781685228537124[21] = 0;
   out_6559781685228537124[22] = 0;
   out_6559781685228537124[23] = 0;
   out_6559781685228537124[24] = 0;
   out_6559781685228537124[25] = 0;
   out_6559781685228537124[26] = 0;
   out_6559781685228537124[27] = 0;
   out_6559781685228537124[28] = 0;
   out_6559781685228537124[29] = 0;
   out_6559781685228537124[30] = 1;
   out_6559781685228537124[31] = 0;
   out_6559781685228537124[32] = 0;
   out_6559781685228537124[33] = 0;
   out_6559781685228537124[34] = 0;
   out_6559781685228537124[35] = 0;
   out_6559781685228537124[36] = 0;
   out_6559781685228537124[37] = 0;
   out_6559781685228537124[38] = 0;
   out_6559781685228537124[39] = 0;
   out_6559781685228537124[40] = 1;
   out_6559781685228537124[41] = 0;
   out_6559781685228537124[42] = 0;
   out_6559781685228537124[43] = 0;
   out_6559781685228537124[44] = 0;
   out_6559781685228537124[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_6559781685228537124[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_6559781685228537124[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6559781685228537124[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6559781685228537124[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_6559781685228537124[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_6559781685228537124[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_6559781685228537124[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_6559781685228537124[53] = -9.8100000000000005*dt;
   out_6559781685228537124[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_6559781685228537124[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_6559781685228537124[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6559781685228537124[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6559781685228537124[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_6559781685228537124[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_6559781685228537124[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_6559781685228537124[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6559781685228537124[62] = 0;
   out_6559781685228537124[63] = 0;
   out_6559781685228537124[64] = 0;
   out_6559781685228537124[65] = 0;
   out_6559781685228537124[66] = 0;
   out_6559781685228537124[67] = 0;
   out_6559781685228537124[68] = 0;
   out_6559781685228537124[69] = 0;
   out_6559781685228537124[70] = 1;
   out_6559781685228537124[71] = 0;
   out_6559781685228537124[72] = 0;
   out_6559781685228537124[73] = 0;
   out_6559781685228537124[74] = 0;
   out_6559781685228537124[75] = 0;
   out_6559781685228537124[76] = 0;
   out_6559781685228537124[77] = 0;
   out_6559781685228537124[78] = 0;
   out_6559781685228537124[79] = 0;
   out_6559781685228537124[80] = 1;
}
void h_25(double *state, double *unused, double *out_5032028916272017851) {
   out_5032028916272017851[0] = state[6];
}
void H_25(double *state, double *unused, double *out_8766861826894493636) {
   out_8766861826894493636[0] = 0;
   out_8766861826894493636[1] = 0;
   out_8766861826894493636[2] = 0;
   out_8766861826894493636[3] = 0;
   out_8766861826894493636[4] = 0;
   out_8766861826894493636[5] = 0;
   out_8766861826894493636[6] = 1;
   out_8766861826894493636[7] = 0;
   out_8766861826894493636[8] = 0;
}
void h_24(double *state, double *unused, double *out_587737815668394072) {
   out_587737815668394072[0] = state[4];
   out_587737815668394072[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6594212227888994070) {
   out_6594212227888994070[0] = 0;
   out_6594212227888994070[1] = 0;
   out_6594212227888994070[2] = 0;
   out_6594212227888994070[3] = 0;
   out_6594212227888994070[4] = 1;
   out_6594212227888994070[5] = 0;
   out_6594212227888994070[6] = 0;
   out_6594212227888994070[7] = 0;
   out_6594212227888994070[8] = 0;
   out_6594212227888994070[9] = 0;
   out_6594212227888994070[10] = 0;
   out_6594212227888994070[11] = 0;
   out_6594212227888994070[12] = 0;
   out_6594212227888994070[13] = 0;
   out_6594212227888994070[14] = 1;
   out_6594212227888994070[15] = 0;
   out_6594212227888994070[16] = 0;
   out_6594212227888994070[17] = 0;
}
void h_30(double *state, double *unused, double *out_5792936771002363629) {
   out_5792936771002363629[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7161549288307809353) {
   out_7161549288307809353[0] = 0;
   out_7161549288307809353[1] = 0;
   out_7161549288307809353[2] = 0;
   out_7161549288307809353[3] = 0;
   out_7161549288307809353[4] = 1;
   out_7161549288307809353[5] = 0;
   out_7161549288307809353[6] = 0;
   out_7161549288307809353[7] = 0;
   out_7161549288307809353[8] = 0;
}
void h_26(double *state, double *unused, double *out_7011077431626231152) {
   out_7011077431626231152[0] = state[7];
}
void H_26(double *state, double *unused, double *out_6375356277054257379) {
   out_6375356277054257379[0] = 0;
   out_6375356277054257379[1] = 0;
   out_6375356277054257379[2] = 0;
   out_6375356277054257379[3] = 0;
   out_6375356277054257379[4] = 0;
   out_6375356277054257379[5] = 0;
   out_6375356277054257379[6] = 0;
   out_6375356277054257379[7] = 1;
   out_6375356277054257379[8] = 0;
}
void h_27(double *state, double *unused, double *out_5979571723008832952) {
   out_5979571723008832952[0] = state[3];
}
void H_27(double *state, double *unused, double *out_4937955217123866136) {
   out_4937955217123866136[0] = 0;
   out_4937955217123866136[1] = 0;
   out_4937955217123866136[2] = 0;
   out_4937955217123866136[3] = 1;
   out_4937955217123866136[4] = 0;
   out_4937955217123866136[5] = 0;
   out_4937955217123866136[6] = 0;
   out_4937955217123866136[7] = 0;
   out_4937955217123866136[8] = 0;
}
void h_29(double *state, double *unused, double *out_791263503341517984) {
   out_791263503341517984[0] = state[1];
}
void H_29(double *state, double *unused, double *out_6651317943993417169) {
   out_6651317943993417169[0] = 0;
   out_6651317943993417169[1] = 1;
   out_6651317943993417169[2] = 0;
   out_6651317943993417169[3] = 0;
   out_6651317943993417169[4] = 0;
   out_6651317943993417169[5] = 0;
   out_6651317943993417169[6] = 0;
   out_6651317943993417169[7] = 0;
   out_6651317943993417169[8] = 0;
}
void h_28(double *state, double *unused, double *out_5036061870107170517) {
   out_5036061870107170517[0] = state[0];
}
void H_28(double *state, double *unused, double *out_6713027112646603873) {
   out_6713027112646603873[0] = 1;
   out_6713027112646603873[1] = 0;
   out_6713027112646603873[2] = 0;
   out_6713027112646603873[3] = 0;
   out_6713027112646603873[4] = 0;
   out_6713027112646603873[5] = 0;
   out_6713027112646603873[6] = 0;
   out_6713027112646603873[7] = 0;
   out_6713027112646603873[8] = 0;
}
void h_31(double *state, double *unused, double *out_1135085123699116452) {
   out_1135085123699116452[0] = state[8];
}
void H_31(double *state, double *unused, double *out_7001564379287608855) {
   out_7001564379287608855[0] = 0;
   out_7001564379287608855[1] = 0;
   out_7001564379287608855[2] = 0;
   out_7001564379287608855[3] = 0;
   out_7001564379287608855[4] = 0;
   out_7001564379287608855[5] = 0;
   out_7001564379287608855[6] = 0;
   out_7001564379287608855[7] = 0;
   out_7001564379287608855[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_3188021722861230961) {
  err_fun(nom_x, delta_x, out_3188021722861230961);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1354812413488475165) {
  inv_err_fun(nom_x, true_x, out_1354812413488475165);
}
void car_H_mod_fun(double *state, double *out_486694020902820743) {
  H_mod_fun(state, out_486694020902820743);
}
void car_f_fun(double *state, double dt, double *out_3031257425454158590) {
  f_fun(state,  dt, out_3031257425454158590);
}
void car_F_fun(double *state, double dt, double *out_6559781685228537124) {
  F_fun(state,  dt, out_6559781685228537124);
}
void car_h_25(double *state, double *unused, double *out_5032028916272017851) {
  h_25(state, unused, out_5032028916272017851);
}
void car_H_25(double *state, double *unused, double *out_8766861826894493636) {
  H_25(state, unused, out_8766861826894493636);
}
void car_h_24(double *state, double *unused, double *out_587737815668394072) {
  h_24(state, unused, out_587737815668394072);
}
void car_H_24(double *state, double *unused, double *out_6594212227888994070) {
  H_24(state, unused, out_6594212227888994070);
}
void car_h_30(double *state, double *unused, double *out_5792936771002363629) {
  h_30(state, unused, out_5792936771002363629);
}
void car_H_30(double *state, double *unused, double *out_7161549288307809353) {
  H_30(state, unused, out_7161549288307809353);
}
void car_h_26(double *state, double *unused, double *out_7011077431626231152) {
  h_26(state, unused, out_7011077431626231152);
}
void car_H_26(double *state, double *unused, double *out_6375356277054257379) {
  H_26(state, unused, out_6375356277054257379);
}
void car_h_27(double *state, double *unused, double *out_5979571723008832952) {
  h_27(state, unused, out_5979571723008832952);
}
void car_H_27(double *state, double *unused, double *out_4937955217123866136) {
  H_27(state, unused, out_4937955217123866136);
}
void car_h_29(double *state, double *unused, double *out_791263503341517984) {
  h_29(state, unused, out_791263503341517984);
}
void car_H_29(double *state, double *unused, double *out_6651317943993417169) {
  H_29(state, unused, out_6651317943993417169);
}
void car_h_28(double *state, double *unused, double *out_5036061870107170517) {
  h_28(state, unused, out_5036061870107170517);
}
void car_H_28(double *state, double *unused, double *out_6713027112646603873) {
  H_28(state, unused, out_6713027112646603873);
}
void car_h_31(double *state, double *unused, double *out_1135085123699116452) {
  h_31(state, unused, out_1135085123699116452);
}
void car_H_31(double *state, double *unused, double *out_7001564379287608855) {
  H_31(state, unused, out_7001564379287608855);
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
