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
void car_err_fun(double *nom_x, double *delta_x, double *out_5610746733953880877);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_659775030920310167);
void car_H_mod_fun(double *state, double *out_869202489952029923);
void car_f_fun(double *state, double dt, double *out_8586997816965019240);
void car_F_fun(double *state, double dt, double *out_832685054505812334);
void car_h_25(double *state, double *unused, double *out_8965051389129453778);
void car_H_25(double *state, double *unused, double *out_6637021278039698796);
void car_h_24(double *state, double *unused, double *out_6184140826246986001);
void car_H_24(double *state, double *unused, double *out_5234150989078334719);
void car_h_30(double *state, double *unused, double *out_2632384913832442088);
void car_H_30(double *state, double *unused, double *out_4893032454178236065);
void car_h_26(double *state, double *unused, double *out_1792976001465789445);
void car_H_26(double *state, double *unused, double *out_8505196825909052219);
void car_h_27(double *state, double *unused, double *out_247214469260761452);
void car_H_27(double *state, double *unused, double *out_7067795765978660976);
void car_h_29(double *state, double *unused, double *out_7018049695611112388);
void car_H_29(double *state, double *unused, double *out_4382801109863843881);
void car_h_28(double *state, double *unused, double *out_5395515401820195383);
void car_H_28(double *state, double *unused, double *out_8981543946776177161);
void car_h_31(double *state, double *unused, double *out_2514377519899065344);
void car_H_31(double *state, double *unused, double *out_6667667239916659224);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}