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
void car_err_fun(double *nom_x, double *delta_x, double *out_7226686656405340864);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7867434532787478357);
void car_H_mod_fun(double *state, double *out_3288405267109084725);
void car_f_fun(double *state, double dt, double *out_8974995752329095985);
void car_F_fun(double *state, double dt, double *out_2278957145959582876);
void car_h_25(double *state, double *unused, double *out_6677369844743091603);
void car_H_25(double *state, double *unused, double *out_2625808263069423248);
void car_h_24(double *state, double *unused, double *out_4367008819681309639);
void car_H_24(double *state, double *unused, double *out_4798457862074922814);
void car_h_30(double *state, double *unused, double *out_6520183176391962458);
void car_H_30(double *state, double *unused, double *out_107475304562174621);
void car_h_26(double *state, double *unused, double *out_1518509908731116813);
void car_H_26(double *state, double *unused, double *out_6367311581943479472);
void car_h_27(double *state, double *unused, double *out_757773589685609210);
void car_H_27(double *state, double *unused, double *out_9118476168712095259);
void car_h_29(double *state, double *unused, double *out_482579527401103321);
void car_H_29(double *state, double *unused, double *out_6643273248882639262);
void car_h_28(double *state, double *unused, double *out_1701283442587271645);
void car_H_28(double *state, double *unused, double *out_4679642977317313011);
void car_h_31(double *state, double *unused, double *out_4998539002616109077);
void car_H_31(double *state, double *unused, double *out_6993519684176830948);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}