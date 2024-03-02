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
void car_err_fun(double *nom_x, double *delta_x, double *out_6290441625309936060);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5314558907334496336);
void car_H_mod_fun(double *state, double *out_7895949841719061513);
void car_f_fun(double *state, double dt, double *out_4613932111658201338);
void car_F_fun(double *state, double dt, double *out_7388683364835999016);
void car_h_25(double *state, double *unused, double *out_3327535608170938215);
void car_H_25(double *state, double *unused, double *out_2233137525527874346);
void car_h_24(double *state, double *unused, double *out_3234738581906136368);
void car_H_24(double *state, double *unused, double *out_5185254108149052547);
void car_h_30(double *state, double *unused, double *out_3758132126270666246);
void car_H_30(double *state, double *unused, double *out_6760833855655482544);
void car_h_26(double *state, double *unused, double *out_3891213047040668096);
void car_H_26(double *state, double *unused, double *out_5974640844401930570);
void car_h_27(double *state, double *unused, double *out_1294650421285170099);
void car_H_27(double *state, double *unused, double *out_8935597167455907455);
void car_h_29(double *state, double *unused, double *out_2590388325194524509);
void car_H_29(double *state, double *unused, double *out_6250602511341090360);
void car_h_28(double *state, double *unused, double *out_7541101478745343567);
void car_H_28(double *state, double *unused, double *out_7113742545298930682);
void car_h_31(double *state, double *unused, double *out_6964985136424960565);
void car_H_31(double *state, double *unused, double *out_6600848946635282046);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}