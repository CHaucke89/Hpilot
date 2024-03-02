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
void car_err_fun(double *nom_x, double *delta_x, double *out_7159053432330882537);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1073064832695516914);
void car_H_mod_fun(double *state, double *out_1645572812354200312);
void car_f_fun(double *state, double dt, double *out_2346849298263271361);
void car_F_fun(double *state, double dt, double *out_5087158014064977170);
void car_h_25(double *state, double *unused, double *out_870579470757970545);
void car_H_25(double *state, double *unused, double *out_36661252871310026);
void car_h_24(double *state, double *unused, double *out_3746370680203545777);
void car_H_24(double *state, double *unused, double *out_3521264345931013110);
void car_h_30(double *state, double *unused, double *out_595385408473464656);
void car_H_30(double *state, double *unused, double *out_4564357582998918224);
void car_h_26(double *state, double *unused, double *out_5453127054407331223);
void car_H_26(double *state, double *unused, double *out_3778164571745366250);
void car_h_27(double *state, double *unused, double *out_4131093796902875100);
void car_H_27(double *state, double *unused, double *out_2340763511814975007);
void car_h_29(double *state, double *unused, double *out_3632171459917797341);
void car_H_29(double *state, double *unused, double *out_4054126238684526040);
void car_h_28(double *state, double *unused, double *out_1626891117090862856);
void car_H_28(double *state, double *unused, double *out_9136525255754056614);
void car_h_31(double *state, double *unused, double *out_4508028999011992895);
void car_H_31(double *state, double *unused, double *out_6015290994349598);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}