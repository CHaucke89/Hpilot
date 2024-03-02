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
void live_H(double *in_vec, double *out_7937023460616626552);
void live_err_fun(double *nom_x, double *delta_x, double *out_1687216722465635178);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_1599895347954292229);
void live_H_mod_fun(double *state, double *out_1056906584894128593);
void live_f_fun(double *state, double dt, double *out_183933638026890231);
void live_F_fun(double *state, double dt, double *out_1040479320272208097);
void live_h_4(double *state, double *unused, double *out_3570086928689996447);
void live_H_4(double *state, double *unused, double *out_7714358732044892667);
void live_h_9(double *state, double *unused, double *out_4006471989357438461);
void live_H_9(double *state, double *unused, double *out_3445166406400211479);
void live_h_10(double *state, double *unused, double *out_4022074140549506913);
void live_H_10(double *state, double *unused, double *out_4941233296478487911);
void live_h_12(double *state, double *unused, double *out_4674873285621457998);
void live_H_12(double *state, double *unused, double *out_5712928933632697154);
void live_h_35(double *state, double *unused, double *out_1741169043564631776);
void live_H_35(double *state, double *unused, double *out_2967365901307683445);
void live_h_32(double *state, double *unused, double *out_2303378269810867446);
void live_H_32(double *state, double *unused, double *out_6862321551745590973);
void live_h_13(double *state, double *unused, double *out_765906276234838754);
void live_H_13(double *state, double *unused, double *out_137717136315470619);
void live_h_14(double *state, double *unused, double *out_4006471989357438461);
void live_H_14(double *state, double *unused, double *out_3445166406400211479);
void live_h_33(double *state, double *unused, double *out_2451336789265741965);
void live_H_33(double *state, double *unused, double *out_4215166279653193969);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}