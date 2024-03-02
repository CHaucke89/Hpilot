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
void live_H(double *in_vec, double *out_2178946929806967284);
void live_err_fun(double *nom_x, double *delta_x, double *out_2730389382207294776);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_7922635659693727034);
void live_H_mod_fun(double *state, double *out_109398800325501579);
void live_f_fun(double *state, double dt, double *out_1610453264853987445);
void live_F_fun(double *state, double dt, double *out_5838711892882351142);
void live_h_4(double *state, double *unused, double *out_7920718728778162894);
void live_H_4(double *state, double *unused, double *out_5800048888584772358);
void live_h_9(double *state, double *unused, double *out_1499628144473020837);
void live_H_9(double *state, double *unused, double *out_5359476249860331788);
void live_h_10(double *state, double *unused, double *out_6546272648458725894);
void live_H_10(double *state, double *unused, double *out_7670525449792756189);
void live_h_12(double *state, double *unused, double *out_4950940537580838535);
void live_H_12(double *state, double *unused, double *out_581209488457960638);
void live_h_35(double *state, double *unused, double *out_4451190112700478825);
void live_H_35(double *state, double *unused, double *out_2164353543867053071);
void live_h_32(double *state, double *unused, double *out_3756076838932141546);
void live_H_32(double *state, double *unused, double *out_2694551676065016835);
void live_h_13(double *state, double *unused, double *out_5007971860488658838);
void live_H_13(double *state, double *unused, double *out_2302462473706090868);
void live_h_14(double *state, double *unused, double *out_1499628144473020837);
void live_H_14(double *state, double *unused, double *out_5359476249860331788);
void live_h_33(double *state, double *unused, double *out_8017280271852998915);
void live_H_33(double *state, double *unused, double *out_916553165521542547);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}