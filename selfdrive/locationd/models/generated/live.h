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
void live_H(double *in_vec, double *out_5293441557815059236);
void live_err_fun(double *nom_x, double *delta_x, double *out_1546401775213992318);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_2887183460710572165);
void live_H_mod_fun(double *state, double *out_346680257265563192);
void live_f_fun(double *state, double dt, double *out_2387280456333129282);
void live_F_fun(double *state, double dt, double *out_288898949053543158);
void live_h_4(double *state, double *unused, double *out_4123763787477834067);
void live_H_4(double *state, double *unused, double *out_1347067503067223419);
void live_h_9(double *state, double *unused, double *out_309747609063080236);
void live_H_9(double *state, double *unused, double *out_2810100233287554064);
void live_h_10(double *state, double *unused, double *out_4520923907172893335);
void live_H_10(double *state, double *unused, double *out_9067229632294002227);
void live_h_12(double *state, double *unused, double *out_4532453151220973685);
void live_H_12(double *state, double *unused, double *out_1968166528114817086);
void live_h_35(double *state, double *unused, double *out_6869040696842091009);
void live_H_35(double *state, double *unused, double *out_4713729560439830795);
void live_h_32(double *state, double *unused, double *out_3827151762527483138);
void live_H_32(double *state, double *unused, double *out_7931710141062108263);
void live_h_13(double *state, double *unused, double *out_6559362347018312854);
void live_H_13(double *state, double *unused, double *out_5140151606010235324);
void live_h_14(double *state, double *unused, double *out_309747609063080236);
void live_H_14(double *state, double *unused, double *out_2810100233287554064);
void live_h_33(double *state, double *unused, double *out_5499250580730826846);
void live_H_33(double *state, double *unused, double *out_7864286565078688399);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}