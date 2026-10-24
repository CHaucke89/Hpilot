#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_3188021722861230961);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1354812413488475165);
void car_H_mod_fun(double *state, double *out_486694020902820743);
void car_f_fun(double *state, double dt, double *out_3031257425454158590);
void car_F_fun(double *state, double dt, double *out_6559781685228537124);
void car_h_25(double *state, double *unused, double *out_5032028916272017851);
void car_H_25(double *state, double *unused, double *out_8766861826894493636);
void car_h_24(double *state, double *unused, double *out_587737815668394072);
void car_H_24(double *state, double *unused, double *out_6594212227888994070);
void car_h_30(double *state, double *unused, double *out_5792936771002363629);
void car_H_30(double *state, double *unused, double *out_7161549288307809353);
void car_h_26(double *state, double *unused, double *out_7011077431626231152);
void car_H_26(double *state, double *unused, double *out_6375356277054257379);
void car_h_27(double *state, double *unused, double *out_5979571723008832952);
void car_H_27(double *state, double *unused, double *out_4937955217123866136);
void car_h_29(double *state, double *unused, double *out_791263503341517984);
void car_H_29(double *state, double *unused, double *out_6651317943993417169);
void car_h_28(double *state, double *unused, double *out_5036061870107170517);
void car_H_28(double *state, double *unused, double *out_6713027112646603873);
void car_h_31(double *state, double *unused, double *out_1135085123699116452);
void car_H_31(double *state, double *unused, double *out_7001564379287608855);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}