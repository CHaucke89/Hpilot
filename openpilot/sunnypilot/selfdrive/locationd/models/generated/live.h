#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_7579122694480484886);
void live_err_fun(double *nom_x, double *delta_x, double *out_8154161731087038539);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_5360108858589404221);
void live_H_mod_fun(double *state, double *out_6911202627417235213);
void live_f_fun(double *state, double dt, double *out_1800654182029259001);
void live_F_fun(double *state, double dt, double *out_4780521995777417009);
void live_h_4(double *state, double *unused, double *out_4619853067874627164);
void live_H_4(double *state, double *unused, double *out_6315176194367080500);
void live_h_9(double *state, double *unused, double *out_6097834091519074789);
void live_H_9(double *state, double *unused, double *out_4844348944078023646);
void live_h_10(double *state, double *unused, double *out_8436055592929012637);
void live_H_10(double *state, double *unused, double *out_1735556512989984052);
void live_h_12(double *state, double *unused, double *out_1862059282108491671);
void live_H_12(double *state, double *unused, double *out_6936275219414674167);
void live_h_35(double *state, double *unused, double *out_2565330242073526422);
void live_H_35(double *state, double *unused, double *out_8764905821969863740);
void live_h_32(double *state, double *unused, double *out_1006437108313454186);
void live_H_32(double *state, double *unused, double *out_8036998742028788274);
void live_h_13(double *state, double *unused, double *out_8775087625111455945);
void live_H_13(double *state, double *unused, double *out_5893121277819415274);
void live_h_14(double *state, double *unused, double *out_6097834091519074789);
void live_H_14(double *state, double *unused, double *out_4844348944078023646);
void live_h_33(double *state, double *unused, double *out_7318148682283128280);
void live_H_33(double *state, double *unused, double *out_5614348817331006136);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}