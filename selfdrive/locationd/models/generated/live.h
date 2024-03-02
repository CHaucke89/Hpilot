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
void live_H(double *in_vec, double *out_7867863003839610759);
void live_err_fun(double *nom_x, double *delta_x, double *out_6614048919106816820);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_2996437008730679516);
void live_H_mod_fun(double *state, double *out_5938837053370187441);
void live_f_fun(double *state, double dt, double *out_5943072507876609054);
void live_F_fun(double *state, double dt, double *out_3595316619751676824);
void live_h_4(double *state, double *unused, double *out_8504012056138441135);
void live_H_4(double *state, double *unused, double *out_6815516860047217919);
void live_h_9(double *state, double *unused, double *out_5642394112647587368);
void live_H_9(double *state, double *unused, double *out_471702075217229551);
void live_h_10(double *state, double *unused, double *out_5651455266028130303);
void live_H_10(double *state, double *unused, double *out_6972082645737282945);
void live_h_12(double *state, double *unused, double *out_3489187020732200148);
void live_H_12(double *state, double *unused, double *out_5249968836619600701);
void live_h_35(double *state, double *unused, double *out_6804786205546796194);
void live_H_35(double *state, double *unused, double *out_3448854802674610543);
void live_h_32(double *state, double *unused, double *out_1638716263979791125);
void live_H_32(double *state, double *unused, double *out_3317690017733870074);
void live_h_13(double *state, double *unused, double *out_5873540218692768727);
void live_H_13(double *state, double *unused, double *out_8265398910829609374);
void live_h_14(double *state, double *unused, double *out_5642394112647587368);
void live_H_14(double *state, double *unused, double *out_471702075217229551);
void live_h_33(double *state, double *unused, double *out_4706085852221705334);
void live_H_33(double *state, double *unused, double *out_6747731490599103886);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}