#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_6596695233229181449);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5518483594158638078);
void pose_H_mod_fun(double *state, double *out_6297549404548103342);
void pose_f_fun(double *state, double dt, double *out_5522725547351082526);
void pose_F_fun(double *state, double dt, double *out_1242349556554524426);
void pose_h_4(double *state, double *unused, double *out_8006082199547920028);
void pose_H_4(double *state, double *unused, double *out_2525742194704195770);
void pose_h_10(double *state, double *unused, double *out_380609954534953758);
void pose_H_10(double *state, double *unused, double *out_4075325651889070173);
void pose_h_13(double *state, double *unused, double *out_2019993501661232517);
void pose_H_13(double *state, double *unused, double *out_686531630628137031);
void pose_h_14(double *state, double *unused, double *out_705784034083826196);
void pose_H_14(double *state, double *unused, double *out_5608530626999568066);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}