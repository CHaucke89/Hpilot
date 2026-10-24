#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_6596695233229181449) {
   out_6596695233229181449[0] = delta_x[0] + nom_x[0];
   out_6596695233229181449[1] = delta_x[1] + nom_x[1];
   out_6596695233229181449[2] = delta_x[2] + nom_x[2];
   out_6596695233229181449[3] = delta_x[3] + nom_x[3];
   out_6596695233229181449[4] = delta_x[4] + nom_x[4];
   out_6596695233229181449[5] = delta_x[5] + nom_x[5];
   out_6596695233229181449[6] = delta_x[6] + nom_x[6];
   out_6596695233229181449[7] = delta_x[7] + nom_x[7];
   out_6596695233229181449[8] = delta_x[8] + nom_x[8];
   out_6596695233229181449[9] = delta_x[9] + nom_x[9];
   out_6596695233229181449[10] = delta_x[10] + nom_x[10];
   out_6596695233229181449[11] = delta_x[11] + nom_x[11];
   out_6596695233229181449[12] = delta_x[12] + nom_x[12];
   out_6596695233229181449[13] = delta_x[13] + nom_x[13];
   out_6596695233229181449[14] = delta_x[14] + nom_x[14];
   out_6596695233229181449[15] = delta_x[15] + nom_x[15];
   out_6596695233229181449[16] = delta_x[16] + nom_x[16];
   out_6596695233229181449[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5518483594158638078) {
   out_5518483594158638078[0] = -nom_x[0] + true_x[0];
   out_5518483594158638078[1] = -nom_x[1] + true_x[1];
   out_5518483594158638078[2] = -nom_x[2] + true_x[2];
   out_5518483594158638078[3] = -nom_x[3] + true_x[3];
   out_5518483594158638078[4] = -nom_x[4] + true_x[4];
   out_5518483594158638078[5] = -nom_x[5] + true_x[5];
   out_5518483594158638078[6] = -nom_x[6] + true_x[6];
   out_5518483594158638078[7] = -nom_x[7] + true_x[7];
   out_5518483594158638078[8] = -nom_x[8] + true_x[8];
   out_5518483594158638078[9] = -nom_x[9] + true_x[9];
   out_5518483594158638078[10] = -nom_x[10] + true_x[10];
   out_5518483594158638078[11] = -nom_x[11] + true_x[11];
   out_5518483594158638078[12] = -nom_x[12] + true_x[12];
   out_5518483594158638078[13] = -nom_x[13] + true_x[13];
   out_5518483594158638078[14] = -nom_x[14] + true_x[14];
   out_5518483594158638078[15] = -nom_x[15] + true_x[15];
   out_5518483594158638078[16] = -nom_x[16] + true_x[16];
   out_5518483594158638078[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_6297549404548103342) {
   out_6297549404548103342[0] = 1.0;
   out_6297549404548103342[1] = 0.0;
   out_6297549404548103342[2] = 0.0;
   out_6297549404548103342[3] = 0.0;
   out_6297549404548103342[4] = 0.0;
   out_6297549404548103342[5] = 0.0;
   out_6297549404548103342[6] = 0.0;
   out_6297549404548103342[7] = 0.0;
   out_6297549404548103342[8] = 0.0;
   out_6297549404548103342[9] = 0.0;
   out_6297549404548103342[10] = 0.0;
   out_6297549404548103342[11] = 0.0;
   out_6297549404548103342[12] = 0.0;
   out_6297549404548103342[13] = 0.0;
   out_6297549404548103342[14] = 0.0;
   out_6297549404548103342[15] = 0.0;
   out_6297549404548103342[16] = 0.0;
   out_6297549404548103342[17] = 0.0;
   out_6297549404548103342[18] = 0.0;
   out_6297549404548103342[19] = 1.0;
   out_6297549404548103342[20] = 0.0;
   out_6297549404548103342[21] = 0.0;
   out_6297549404548103342[22] = 0.0;
   out_6297549404548103342[23] = 0.0;
   out_6297549404548103342[24] = 0.0;
   out_6297549404548103342[25] = 0.0;
   out_6297549404548103342[26] = 0.0;
   out_6297549404548103342[27] = 0.0;
   out_6297549404548103342[28] = 0.0;
   out_6297549404548103342[29] = 0.0;
   out_6297549404548103342[30] = 0.0;
   out_6297549404548103342[31] = 0.0;
   out_6297549404548103342[32] = 0.0;
   out_6297549404548103342[33] = 0.0;
   out_6297549404548103342[34] = 0.0;
   out_6297549404548103342[35] = 0.0;
   out_6297549404548103342[36] = 0.0;
   out_6297549404548103342[37] = 0.0;
   out_6297549404548103342[38] = 1.0;
   out_6297549404548103342[39] = 0.0;
   out_6297549404548103342[40] = 0.0;
   out_6297549404548103342[41] = 0.0;
   out_6297549404548103342[42] = 0.0;
   out_6297549404548103342[43] = 0.0;
   out_6297549404548103342[44] = 0.0;
   out_6297549404548103342[45] = 0.0;
   out_6297549404548103342[46] = 0.0;
   out_6297549404548103342[47] = 0.0;
   out_6297549404548103342[48] = 0.0;
   out_6297549404548103342[49] = 0.0;
   out_6297549404548103342[50] = 0.0;
   out_6297549404548103342[51] = 0.0;
   out_6297549404548103342[52] = 0.0;
   out_6297549404548103342[53] = 0.0;
   out_6297549404548103342[54] = 0.0;
   out_6297549404548103342[55] = 0.0;
   out_6297549404548103342[56] = 0.0;
   out_6297549404548103342[57] = 1.0;
   out_6297549404548103342[58] = 0.0;
   out_6297549404548103342[59] = 0.0;
   out_6297549404548103342[60] = 0.0;
   out_6297549404548103342[61] = 0.0;
   out_6297549404548103342[62] = 0.0;
   out_6297549404548103342[63] = 0.0;
   out_6297549404548103342[64] = 0.0;
   out_6297549404548103342[65] = 0.0;
   out_6297549404548103342[66] = 0.0;
   out_6297549404548103342[67] = 0.0;
   out_6297549404548103342[68] = 0.0;
   out_6297549404548103342[69] = 0.0;
   out_6297549404548103342[70] = 0.0;
   out_6297549404548103342[71] = 0.0;
   out_6297549404548103342[72] = 0.0;
   out_6297549404548103342[73] = 0.0;
   out_6297549404548103342[74] = 0.0;
   out_6297549404548103342[75] = 0.0;
   out_6297549404548103342[76] = 1.0;
   out_6297549404548103342[77] = 0.0;
   out_6297549404548103342[78] = 0.0;
   out_6297549404548103342[79] = 0.0;
   out_6297549404548103342[80] = 0.0;
   out_6297549404548103342[81] = 0.0;
   out_6297549404548103342[82] = 0.0;
   out_6297549404548103342[83] = 0.0;
   out_6297549404548103342[84] = 0.0;
   out_6297549404548103342[85] = 0.0;
   out_6297549404548103342[86] = 0.0;
   out_6297549404548103342[87] = 0.0;
   out_6297549404548103342[88] = 0.0;
   out_6297549404548103342[89] = 0.0;
   out_6297549404548103342[90] = 0.0;
   out_6297549404548103342[91] = 0.0;
   out_6297549404548103342[92] = 0.0;
   out_6297549404548103342[93] = 0.0;
   out_6297549404548103342[94] = 0.0;
   out_6297549404548103342[95] = 1.0;
   out_6297549404548103342[96] = 0.0;
   out_6297549404548103342[97] = 0.0;
   out_6297549404548103342[98] = 0.0;
   out_6297549404548103342[99] = 0.0;
   out_6297549404548103342[100] = 0.0;
   out_6297549404548103342[101] = 0.0;
   out_6297549404548103342[102] = 0.0;
   out_6297549404548103342[103] = 0.0;
   out_6297549404548103342[104] = 0.0;
   out_6297549404548103342[105] = 0.0;
   out_6297549404548103342[106] = 0.0;
   out_6297549404548103342[107] = 0.0;
   out_6297549404548103342[108] = 0.0;
   out_6297549404548103342[109] = 0.0;
   out_6297549404548103342[110] = 0.0;
   out_6297549404548103342[111] = 0.0;
   out_6297549404548103342[112] = 0.0;
   out_6297549404548103342[113] = 0.0;
   out_6297549404548103342[114] = 1.0;
   out_6297549404548103342[115] = 0.0;
   out_6297549404548103342[116] = 0.0;
   out_6297549404548103342[117] = 0.0;
   out_6297549404548103342[118] = 0.0;
   out_6297549404548103342[119] = 0.0;
   out_6297549404548103342[120] = 0.0;
   out_6297549404548103342[121] = 0.0;
   out_6297549404548103342[122] = 0.0;
   out_6297549404548103342[123] = 0.0;
   out_6297549404548103342[124] = 0.0;
   out_6297549404548103342[125] = 0.0;
   out_6297549404548103342[126] = 0.0;
   out_6297549404548103342[127] = 0.0;
   out_6297549404548103342[128] = 0.0;
   out_6297549404548103342[129] = 0.0;
   out_6297549404548103342[130] = 0.0;
   out_6297549404548103342[131] = 0.0;
   out_6297549404548103342[132] = 0.0;
   out_6297549404548103342[133] = 1.0;
   out_6297549404548103342[134] = 0.0;
   out_6297549404548103342[135] = 0.0;
   out_6297549404548103342[136] = 0.0;
   out_6297549404548103342[137] = 0.0;
   out_6297549404548103342[138] = 0.0;
   out_6297549404548103342[139] = 0.0;
   out_6297549404548103342[140] = 0.0;
   out_6297549404548103342[141] = 0.0;
   out_6297549404548103342[142] = 0.0;
   out_6297549404548103342[143] = 0.0;
   out_6297549404548103342[144] = 0.0;
   out_6297549404548103342[145] = 0.0;
   out_6297549404548103342[146] = 0.0;
   out_6297549404548103342[147] = 0.0;
   out_6297549404548103342[148] = 0.0;
   out_6297549404548103342[149] = 0.0;
   out_6297549404548103342[150] = 0.0;
   out_6297549404548103342[151] = 0.0;
   out_6297549404548103342[152] = 1.0;
   out_6297549404548103342[153] = 0.0;
   out_6297549404548103342[154] = 0.0;
   out_6297549404548103342[155] = 0.0;
   out_6297549404548103342[156] = 0.0;
   out_6297549404548103342[157] = 0.0;
   out_6297549404548103342[158] = 0.0;
   out_6297549404548103342[159] = 0.0;
   out_6297549404548103342[160] = 0.0;
   out_6297549404548103342[161] = 0.0;
   out_6297549404548103342[162] = 0.0;
   out_6297549404548103342[163] = 0.0;
   out_6297549404548103342[164] = 0.0;
   out_6297549404548103342[165] = 0.0;
   out_6297549404548103342[166] = 0.0;
   out_6297549404548103342[167] = 0.0;
   out_6297549404548103342[168] = 0.0;
   out_6297549404548103342[169] = 0.0;
   out_6297549404548103342[170] = 0.0;
   out_6297549404548103342[171] = 1.0;
   out_6297549404548103342[172] = 0.0;
   out_6297549404548103342[173] = 0.0;
   out_6297549404548103342[174] = 0.0;
   out_6297549404548103342[175] = 0.0;
   out_6297549404548103342[176] = 0.0;
   out_6297549404548103342[177] = 0.0;
   out_6297549404548103342[178] = 0.0;
   out_6297549404548103342[179] = 0.0;
   out_6297549404548103342[180] = 0.0;
   out_6297549404548103342[181] = 0.0;
   out_6297549404548103342[182] = 0.0;
   out_6297549404548103342[183] = 0.0;
   out_6297549404548103342[184] = 0.0;
   out_6297549404548103342[185] = 0.0;
   out_6297549404548103342[186] = 0.0;
   out_6297549404548103342[187] = 0.0;
   out_6297549404548103342[188] = 0.0;
   out_6297549404548103342[189] = 0.0;
   out_6297549404548103342[190] = 1.0;
   out_6297549404548103342[191] = 0.0;
   out_6297549404548103342[192] = 0.0;
   out_6297549404548103342[193] = 0.0;
   out_6297549404548103342[194] = 0.0;
   out_6297549404548103342[195] = 0.0;
   out_6297549404548103342[196] = 0.0;
   out_6297549404548103342[197] = 0.0;
   out_6297549404548103342[198] = 0.0;
   out_6297549404548103342[199] = 0.0;
   out_6297549404548103342[200] = 0.0;
   out_6297549404548103342[201] = 0.0;
   out_6297549404548103342[202] = 0.0;
   out_6297549404548103342[203] = 0.0;
   out_6297549404548103342[204] = 0.0;
   out_6297549404548103342[205] = 0.0;
   out_6297549404548103342[206] = 0.0;
   out_6297549404548103342[207] = 0.0;
   out_6297549404548103342[208] = 0.0;
   out_6297549404548103342[209] = 1.0;
   out_6297549404548103342[210] = 0.0;
   out_6297549404548103342[211] = 0.0;
   out_6297549404548103342[212] = 0.0;
   out_6297549404548103342[213] = 0.0;
   out_6297549404548103342[214] = 0.0;
   out_6297549404548103342[215] = 0.0;
   out_6297549404548103342[216] = 0.0;
   out_6297549404548103342[217] = 0.0;
   out_6297549404548103342[218] = 0.0;
   out_6297549404548103342[219] = 0.0;
   out_6297549404548103342[220] = 0.0;
   out_6297549404548103342[221] = 0.0;
   out_6297549404548103342[222] = 0.0;
   out_6297549404548103342[223] = 0.0;
   out_6297549404548103342[224] = 0.0;
   out_6297549404548103342[225] = 0.0;
   out_6297549404548103342[226] = 0.0;
   out_6297549404548103342[227] = 0.0;
   out_6297549404548103342[228] = 1.0;
   out_6297549404548103342[229] = 0.0;
   out_6297549404548103342[230] = 0.0;
   out_6297549404548103342[231] = 0.0;
   out_6297549404548103342[232] = 0.0;
   out_6297549404548103342[233] = 0.0;
   out_6297549404548103342[234] = 0.0;
   out_6297549404548103342[235] = 0.0;
   out_6297549404548103342[236] = 0.0;
   out_6297549404548103342[237] = 0.0;
   out_6297549404548103342[238] = 0.0;
   out_6297549404548103342[239] = 0.0;
   out_6297549404548103342[240] = 0.0;
   out_6297549404548103342[241] = 0.0;
   out_6297549404548103342[242] = 0.0;
   out_6297549404548103342[243] = 0.0;
   out_6297549404548103342[244] = 0.0;
   out_6297549404548103342[245] = 0.0;
   out_6297549404548103342[246] = 0.0;
   out_6297549404548103342[247] = 1.0;
   out_6297549404548103342[248] = 0.0;
   out_6297549404548103342[249] = 0.0;
   out_6297549404548103342[250] = 0.0;
   out_6297549404548103342[251] = 0.0;
   out_6297549404548103342[252] = 0.0;
   out_6297549404548103342[253] = 0.0;
   out_6297549404548103342[254] = 0.0;
   out_6297549404548103342[255] = 0.0;
   out_6297549404548103342[256] = 0.0;
   out_6297549404548103342[257] = 0.0;
   out_6297549404548103342[258] = 0.0;
   out_6297549404548103342[259] = 0.0;
   out_6297549404548103342[260] = 0.0;
   out_6297549404548103342[261] = 0.0;
   out_6297549404548103342[262] = 0.0;
   out_6297549404548103342[263] = 0.0;
   out_6297549404548103342[264] = 0.0;
   out_6297549404548103342[265] = 0.0;
   out_6297549404548103342[266] = 1.0;
   out_6297549404548103342[267] = 0.0;
   out_6297549404548103342[268] = 0.0;
   out_6297549404548103342[269] = 0.0;
   out_6297549404548103342[270] = 0.0;
   out_6297549404548103342[271] = 0.0;
   out_6297549404548103342[272] = 0.0;
   out_6297549404548103342[273] = 0.0;
   out_6297549404548103342[274] = 0.0;
   out_6297549404548103342[275] = 0.0;
   out_6297549404548103342[276] = 0.0;
   out_6297549404548103342[277] = 0.0;
   out_6297549404548103342[278] = 0.0;
   out_6297549404548103342[279] = 0.0;
   out_6297549404548103342[280] = 0.0;
   out_6297549404548103342[281] = 0.0;
   out_6297549404548103342[282] = 0.0;
   out_6297549404548103342[283] = 0.0;
   out_6297549404548103342[284] = 0.0;
   out_6297549404548103342[285] = 1.0;
   out_6297549404548103342[286] = 0.0;
   out_6297549404548103342[287] = 0.0;
   out_6297549404548103342[288] = 0.0;
   out_6297549404548103342[289] = 0.0;
   out_6297549404548103342[290] = 0.0;
   out_6297549404548103342[291] = 0.0;
   out_6297549404548103342[292] = 0.0;
   out_6297549404548103342[293] = 0.0;
   out_6297549404548103342[294] = 0.0;
   out_6297549404548103342[295] = 0.0;
   out_6297549404548103342[296] = 0.0;
   out_6297549404548103342[297] = 0.0;
   out_6297549404548103342[298] = 0.0;
   out_6297549404548103342[299] = 0.0;
   out_6297549404548103342[300] = 0.0;
   out_6297549404548103342[301] = 0.0;
   out_6297549404548103342[302] = 0.0;
   out_6297549404548103342[303] = 0.0;
   out_6297549404548103342[304] = 1.0;
   out_6297549404548103342[305] = 0.0;
   out_6297549404548103342[306] = 0.0;
   out_6297549404548103342[307] = 0.0;
   out_6297549404548103342[308] = 0.0;
   out_6297549404548103342[309] = 0.0;
   out_6297549404548103342[310] = 0.0;
   out_6297549404548103342[311] = 0.0;
   out_6297549404548103342[312] = 0.0;
   out_6297549404548103342[313] = 0.0;
   out_6297549404548103342[314] = 0.0;
   out_6297549404548103342[315] = 0.0;
   out_6297549404548103342[316] = 0.0;
   out_6297549404548103342[317] = 0.0;
   out_6297549404548103342[318] = 0.0;
   out_6297549404548103342[319] = 0.0;
   out_6297549404548103342[320] = 0.0;
   out_6297549404548103342[321] = 0.0;
   out_6297549404548103342[322] = 0.0;
   out_6297549404548103342[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5522725547351082526) {
   out_5522725547351082526[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5522725547351082526[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5522725547351082526[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5522725547351082526[3] = dt*state[12] + state[3];
   out_5522725547351082526[4] = dt*state[13] + state[4];
   out_5522725547351082526[5] = dt*state[14] + state[5];
   out_5522725547351082526[6] = state[6];
   out_5522725547351082526[7] = state[7];
   out_5522725547351082526[8] = state[8];
   out_5522725547351082526[9] = state[9];
   out_5522725547351082526[10] = state[10];
   out_5522725547351082526[11] = state[11];
   out_5522725547351082526[12] = state[12];
   out_5522725547351082526[13] = state[13];
   out_5522725547351082526[14] = state[14];
   out_5522725547351082526[15] = state[15];
   out_5522725547351082526[16] = state[16];
   out_5522725547351082526[17] = state[17];
}
void F_fun(double *state, double dt, double *out_1242349556554524426) {
   out_1242349556554524426[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1242349556554524426[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1242349556554524426[2] = 0;
   out_1242349556554524426[3] = 0;
   out_1242349556554524426[4] = 0;
   out_1242349556554524426[5] = 0;
   out_1242349556554524426[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1242349556554524426[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1242349556554524426[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1242349556554524426[9] = 0;
   out_1242349556554524426[10] = 0;
   out_1242349556554524426[11] = 0;
   out_1242349556554524426[12] = 0;
   out_1242349556554524426[13] = 0;
   out_1242349556554524426[14] = 0;
   out_1242349556554524426[15] = 0;
   out_1242349556554524426[16] = 0;
   out_1242349556554524426[17] = 0;
   out_1242349556554524426[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1242349556554524426[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1242349556554524426[20] = 0;
   out_1242349556554524426[21] = 0;
   out_1242349556554524426[22] = 0;
   out_1242349556554524426[23] = 0;
   out_1242349556554524426[24] = 0;
   out_1242349556554524426[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1242349556554524426[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1242349556554524426[27] = 0;
   out_1242349556554524426[28] = 0;
   out_1242349556554524426[29] = 0;
   out_1242349556554524426[30] = 0;
   out_1242349556554524426[31] = 0;
   out_1242349556554524426[32] = 0;
   out_1242349556554524426[33] = 0;
   out_1242349556554524426[34] = 0;
   out_1242349556554524426[35] = 0;
   out_1242349556554524426[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1242349556554524426[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1242349556554524426[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1242349556554524426[39] = 0;
   out_1242349556554524426[40] = 0;
   out_1242349556554524426[41] = 0;
   out_1242349556554524426[42] = 0;
   out_1242349556554524426[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1242349556554524426[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1242349556554524426[45] = 0;
   out_1242349556554524426[46] = 0;
   out_1242349556554524426[47] = 0;
   out_1242349556554524426[48] = 0;
   out_1242349556554524426[49] = 0;
   out_1242349556554524426[50] = 0;
   out_1242349556554524426[51] = 0;
   out_1242349556554524426[52] = 0;
   out_1242349556554524426[53] = 0;
   out_1242349556554524426[54] = 0;
   out_1242349556554524426[55] = 0;
   out_1242349556554524426[56] = 0;
   out_1242349556554524426[57] = 1;
   out_1242349556554524426[58] = 0;
   out_1242349556554524426[59] = 0;
   out_1242349556554524426[60] = 0;
   out_1242349556554524426[61] = 0;
   out_1242349556554524426[62] = 0;
   out_1242349556554524426[63] = 0;
   out_1242349556554524426[64] = 0;
   out_1242349556554524426[65] = 0;
   out_1242349556554524426[66] = dt;
   out_1242349556554524426[67] = 0;
   out_1242349556554524426[68] = 0;
   out_1242349556554524426[69] = 0;
   out_1242349556554524426[70] = 0;
   out_1242349556554524426[71] = 0;
   out_1242349556554524426[72] = 0;
   out_1242349556554524426[73] = 0;
   out_1242349556554524426[74] = 0;
   out_1242349556554524426[75] = 0;
   out_1242349556554524426[76] = 1;
   out_1242349556554524426[77] = 0;
   out_1242349556554524426[78] = 0;
   out_1242349556554524426[79] = 0;
   out_1242349556554524426[80] = 0;
   out_1242349556554524426[81] = 0;
   out_1242349556554524426[82] = 0;
   out_1242349556554524426[83] = 0;
   out_1242349556554524426[84] = 0;
   out_1242349556554524426[85] = dt;
   out_1242349556554524426[86] = 0;
   out_1242349556554524426[87] = 0;
   out_1242349556554524426[88] = 0;
   out_1242349556554524426[89] = 0;
   out_1242349556554524426[90] = 0;
   out_1242349556554524426[91] = 0;
   out_1242349556554524426[92] = 0;
   out_1242349556554524426[93] = 0;
   out_1242349556554524426[94] = 0;
   out_1242349556554524426[95] = 1;
   out_1242349556554524426[96] = 0;
   out_1242349556554524426[97] = 0;
   out_1242349556554524426[98] = 0;
   out_1242349556554524426[99] = 0;
   out_1242349556554524426[100] = 0;
   out_1242349556554524426[101] = 0;
   out_1242349556554524426[102] = 0;
   out_1242349556554524426[103] = 0;
   out_1242349556554524426[104] = dt;
   out_1242349556554524426[105] = 0;
   out_1242349556554524426[106] = 0;
   out_1242349556554524426[107] = 0;
   out_1242349556554524426[108] = 0;
   out_1242349556554524426[109] = 0;
   out_1242349556554524426[110] = 0;
   out_1242349556554524426[111] = 0;
   out_1242349556554524426[112] = 0;
   out_1242349556554524426[113] = 0;
   out_1242349556554524426[114] = 1;
   out_1242349556554524426[115] = 0;
   out_1242349556554524426[116] = 0;
   out_1242349556554524426[117] = 0;
   out_1242349556554524426[118] = 0;
   out_1242349556554524426[119] = 0;
   out_1242349556554524426[120] = 0;
   out_1242349556554524426[121] = 0;
   out_1242349556554524426[122] = 0;
   out_1242349556554524426[123] = 0;
   out_1242349556554524426[124] = 0;
   out_1242349556554524426[125] = 0;
   out_1242349556554524426[126] = 0;
   out_1242349556554524426[127] = 0;
   out_1242349556554524426[128] = 0;
   out_1242349556554524426[129] = 0;
   out_1242349556554524426[130] = 0;
   out_1242349556554524426[131] = 0;
   out_1242349556554524426[132] = 0;
   out_1242349556554524426[133] = 1;
   out_1242349556554524426[134] = 0;
   out_1242349556554524426[135] = 0;
   out_1242349556554524426[136] = 0;
   out_1242349556554524426[137] = 0;
   out_1242349556554524426[138] = 0;
   out_1242349556554524426[139] = 0;
   out_1242349556554524426[140] = 0;
   out_1242349556554524426[141] = 0;
   out_1242349556554524426[142] = 0;
   out_1242349556554524426[143] = 0;
   out_1242349556554524426[144] = 0;
   out_1242349556554524426[145] = 0;
   out_1242349556554524426[146] = 0;
   out_1242349556554524426[147] = 0;
   out_1242349556554524426[148] = 0;
   out_1242349556554524426[149] = 0;
   out_1242349556554524426[150] = 0;
   out_1242349556554524426[151] = 0;
   out_1242349556554524426[152] = 1;
   out_1242349556554524426[153] = 0;
   out_1242349556554524426[154] = 0;
   out_1242349556554524426[155] = 0;
   out_1242349556554524426[156] = 0;
   out_1242349556554524426[157] = 0;
   out_1242349556554524426[158] = 0;
   out_1242349556554524426[159] = 0;
   out_1242349556554524426[160] = 0;
   out_1242349556554524426[161] = 0;
   out_1242349556554524426[162] = 0;
   out_1242349556554524426[163] = 0;
   out_1242349556554524426[164] = 0;
   out_1242349556554524426[165] = 0;
   out_1242349556554524426[166] = 0;
   out_1242349556554524426[167] = 0;
   out_1242349556554524426[168] = 0;
   out_1242349556554524426[169] = 0;
   out_1242349556554524426[170] = 0;
   out_1242349556554524426[171] = 1;
   out_1242349556554524426[172] = 0;
   out_1242349556554524426[173] = 0;
   out_1242349556554524426[174] = 0;
   out_1242349556554524426[175] = 0;
   out_1242349556554524426[176] = 0;
   out_1242349556554524426[177] = 0;
   out_1242349556554524426[178] = 0;
   out_1242349556554524426[179] = 0;
   out_1242349556554524426[180] = 0;
   out_1242349556554524426[181] = 0;
   out_1242349556554524426[182] = 0;
   out_1242349556554524426[183] = 0;
   out_1242349556554524426[184] = 0;
   out_1242349556554524426[185] = 0;
   out_1242349556554524426[186] = 0;
   out_1242349556554524426[187] = 0;
   out_1242349556554524426[188] = 0;
   out_1242349556554524426[189] = 0;
   out_1242349556554524426[190] = 1;
   out_1242349556554524426[191] = 0;
   out_1242349556554524426[192] = 0;
   out_1242349556554524426[193] = 0;
   out_1242349556554524426[194] = 0;
   out_1242349556554524426[195] = 0;
   out_1242349556554524426[196] = 0;
   out_1242349556554524426[197] = 0;
   out_1242349556554524426[198] = 0;
   out_1242349556554524426[199] = 0;
   out_1242349556554524426[200] = 0;
   out_1242349556554524426[201] = 0;
   out_1242349556554524426[202] = 0;
   out_1242349556554524426[203] = 0;
   out_1242349556554524426[204] = 0;
   out_1242349556554524426[205] = 0;
   out_1242349556554524426[206] = 0;
   out_1242349556554524426[207] = 0;
   out_1242349556554524426[208] = 0;
   out_1242349556554524426[209] = 1;
   out_1242349556554524426[210] = 0;
   out_1242349556554524426[211] = 0;
   out_1242349556554524426[212] = 0;
   out_1242349556554524426[213] = 0;
   out_1242349556554524426[214] = 0;
   out_1242349556554524426[215] = 0;
   out_1242349556554524426[216] = 0;
   out_1242349556554524426[217] = 0;
   out_1242349556554524426[218] = 0;
   out_1242349556554524426[219] = 0;
   out_1242349556554524426[220] = 0;
   out_1242349556554524426[221] = 0;
   out_1242349556554524426[222] = 0;
   out_1242349556554524426[223] = 0;
   out_1242349556554524426[224] = 0;
   out_1242349556554524426[225] = 0;
   out_1242349556554524426[226] = 0;
   out_1242349556554524426[227] = 0;
   out_1242349556554524426[228] = 1;
   out_1242349556554524426[229] = 0;
   out_1242349556554524426[230] = 0;
   out_1242349556554524426[231] = 0;
   out_1242349556554524426[232] = 0;
   out_1242349556554524426[233] = 0;
   out_1242349556554524426[234] = 0;
   out_1242349556554524426[235] = 0;
   out_1242349556554524426[236] = 0;
   out_1242349556554524426[237] = 0;
   out_1242349556554524426[238] = 0;
   out_1242349556554524426[239] = 0;
   out_1242349556554524426[240] = 0;
   out_1242349556554524426[241] = 0;
   out_1242349556554524426[242] = 0;
   out_1242349556554524426[243] = 0;
   out_1242349556554524426[244] = 0;
   out_1242349556554524426[245] = 0;
   out_1242349556554524426[246] = 0;
   out_1242349556554524426[247] = 1;
   out_1242349556554524426[248] = 0;
   out_1242349556554524426[249] = 0;
   out_1242349556554524426[250] = 0;
   out_1242349556554524426[251] = 0;
   out_1242349556554524426[252] = 0;
   out_1242349556554524426[253] = 0;
   out_1242349556554524426[254] = 0;
   out_1242349556554524426[255] = 0;
   out_1242349556554524426[256] = 0;
   out_1242349556554524426[257] = 0;
   out_1242349556554524426[258] = 0;
   out_1242349556554524426[259] = 0;
   out_1242349556554524426[260] = 0;
   out_1242349556554524426[261] = 0;
   out_1242349556554524426[262] = 0;
   out_1242349556554524426[263] = 0;
   out_1242349556554524426[264] = 0;
   out_1242349556554524426[265] = 0;
   out_1242349556554524426[266] = 1;
   out_1242349556554524426[267] = 0;
   out_1242349556554524426[268] = 0;
   out_1242349556554524426[269] = 0;
   out_1242349556554524426[270] = 0;
   out_1242349556554524426[271] = 0;
   out_1242349556554524426[272] = 0;
   out_1242349556554524426[273] = 0;
   out_1242349556554524426[274] = 0;
   out_1242349556554524426[275] = 0;
   out_1242349556554524426[276] = 0;
   out_1242349556554524426[277] = 0;
   out_1242349556554524426[278] = 0;
   out_1242349556554524426[279] = 0;
   out_1242349556554524426[280] = 0;
   out_1242349556554524426[281] = 0;
   out_1242349556554524426[282] = 0;
   out_1242349556554524426[283] = 0;
   out_1242349556554524426[284] = 0;
   out_1242349556554524426[285] = 1;
   out_1242349556554524426[286] = 0;
   out_1242349556554524426[287] = 0;
   out_1242349556554524426[288] = 0;
   out_1242349556554524426[289] = 0;
   out_1242349556554524426[290] = 0;
   out_1242349556554524426[291] = 0;
   out_1242349556554524426[292] = 0;
   out_1242349556554524426[293] = 0;
   out_1242349556554524426[294] = 0;
   out_1242349556554524426[295] = 0;
   out_1242349556554524426[296] = 0;
   out_1242349556554524426[297] = 0;
   out_1242349556554524426[298] = 0;
   out_1242349556554524426[299] = 0;
   out_1242349556554524426[300] = 0;
   out_1242349556554524426[301] = 0;
   out_1242349556554524426[302] = 0;
   out_1242349556554524426[303] = 0;
   out_1242349556554524426[304] = 1;
   out_1242349556554524426[305] = 0;
   out_1242349556554524426[306] = 0;
   out_1242349556554524426[307] = 0;
   out_1242349556554524426[308] = 0;
   out_1242349556554524426[309] = 0;
   out_1242349556554524426[310] = 0;
   out_1242349556554524426[311] = 0;
   out_1242349556554524426[312] = 0;
   out_1242349556554524426[313] = 0;
   out_1242349556554524426[314] = 0;
   out_1242349556554524426[315] = 0;
   out_1242349556554524426[316] = 0;
   out_1242349556554524426[317] = 0;
   out_1242349556554524426[318] = 0;
   out_1242349556554524426[319] = 0;
   out_1242349556554524426[320] = 0;
   out_1242349556554524426[321] = 0;
   out_1242349556554524426[322] = 0;
   out_1242349556554524426[323] = 1;
}
void h_4(double *state, double *unused, double *out_8006082199547920028) {
   out_8006082199547920028[0] = state[6] + state[9];
   out_8006082199547920028[1] = state[7] + state[10];
   out_8006082199547920028[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_2525742194704195770) {
   out_2525742194704195770[0] = 0;
   out_2525742194704195770[1] = 0;
   out_2525742194704195770[2] = 0;
   out_2525742194704195770[3] = 0;
   out_2525742194704195770[4] = 0;
   out_2525742194704195770[5] = 0;
   out_2525742194704195770[6] = 1;
   out_2525742194704195770[7] = 0;
   out_2525742194704195770[8] = 0;
   out_2525742194704195770[9] = 1;
   out_2525742194704195770[10] = 0;
   out_2525742194704195770[11] = 0;
   out_2525742194704195770[12] = 0;
   out_2525742194704195770[13] = 0;
   out_2525742194704195770[14] = 0;
   out_2525742194704195770[15] = 0;
   out_2525742194704195770[16] = 0;
   out_2525742194704195770[17] = 0;
   out_2525742194704195770[18] = 0;
   out_2525742194704195770[19] = 0;
   out_2525742194704195770[20] = 0;
   out_2525742194704195770[21] = 0;
   out_2525742194704195770[22] = 0;
   out_2525742194704195770[23] = 0;
   out_2525742194704195770[24] = 0;
   out_2525742194704195770[25] = 1;
   out_2525742194704195770[26] = 0;
   out_2525742194704195770[27] = 0;
   out_2525742194704195770[28] = 1;
   out_2525742194704195770[29] = 0;
   out_2525742194704195770[30] = 0;
   out_2525742194704195770[31] = 0;
   out_2525742194704195770[32] = 0;
   out_2525742194704195770[33] = 0;
   out_2525742194704195770[34] = 0;
   out_2525742194704195770[35] = 0;
   out_2525742194704195770[36] = 0;
   out_2525742194704195770[37] = 0;
   out_2525742194704195770[38] = 0;
   out_2525742194704195770[39] = 0;
   out_2525742194704195770[40] = 0;
   out_2525742194704195770[41] = 0;
   out_2525742194704195770[42] = 0;
   out_2525742194704195770[43] = 0;
   out_2525742194704195770[44] = 1;
   out_2525742194704195770[45] = 0;
   out_2525742194704195770[46] = 0;
   out_2525742194704195770[47] = 1;
   out_2525742194704195770[48] = 0;
   out_2525742194704195770[49] = 0;
   out_2525742194704195770[50] = 0;
   out_2525742194704195770[51] = 0;
   out_2525742194704195770[52] = 0;
   out_2525742194704195770[53] = 0;
}
void h_10(double *state, double *unused, double *out_380609954534953758) {
   out_380609954534953758[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_380609954534953758[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_380609954534953758[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_4075325651889070173) {
   out_4075325651889070173[0] = 0;
   out_4075325651889070173[1] = 9.8100000000000005*cos(state[1]);
   out_4075325651889070173[2] = 0;
   out_4075325651889070173[3] = 0;
   out_4075325651889070173[4] = -state[8];
   out_4075325651889070173[5] = state[7];
   out_4075325651889070173[6] = 0;
   out_4075325651889070173[7] = state[5];
   out_4075325651889070173[8] = -state[4];
   out_4075325651889070173[9] = 0;
   out_4075325651889070173[10] = 0;
   out_4075325651889070173[11] = 0;
   out_4075325651889070173[12] = 1;
   out_4075325651889070173[13] = 0;
   out_4075325651889070173[14] = 0;
   out_4075325651889070173[15] = 1;
   out_4075325651889070173[16] = 0;
   out_4075325651889070173[17] = 0;
   out_4075325651889070173[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_4075325651889070173[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_4075325651889070173[20] = 0;
   out_4075325651889070173[21] = state[8];
   out_4075325651889070173[22] = 0;
   out_4075325651889070173[23] = -state[6];
   out_4075325651889070173[24] = -state[5];
   out_4075325651889070173[25] = 0;
   out_4075325651889070173[26] = state[3];
   out_4075325651889070173[27] = 0;
   out_4075325651889070173[28] = 0;
   out_4075325651889070173[29] = 0;
   out_4075325651889070173[30] = 0;
   out_4075325651889070173[31] = 1;
   out_4075325651889070173[32] = 0;
   out_4075325651889070173[33] = 0;
   out_4075325651889070173[34] = 1;
   out_4075325651889070173[35] = 0;
   out_4075325651889070173[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_4075325651889070173[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_4075325651889070173[38] = 0;
   out_4075325651889070173[39] = -state[7];
   out_4075325651889070173[40] = state[6];
   out_4075325651889070173[41] = 0;
   out_4075325651889070173[42] = state[4];
   out_4075325651889070173[43] = -state[3];
   out_4075325651889070173[44] = 0;
   out_4075325651889070173[45] = 0;
   out_4075325651889070173[46] = 0;
   out_4075325651889070173[47] = 0;
   out_4075325651889070173[48] = 0;
   out_4075325651889070173[49] = 0;
   out_4075325651889070173[50] = 1;
   out_4075325651889070173[51] = 0;
   out_4075325651889070173[52] = 0;
   out_4075325651889070173[53] = 1;
}
void h_13(double *state, double *unused, double *out_2019993501661232517) {
   out_2019993501661232517[0] = state[3];
   out_2019993501661232517[1] = state[4];
   out_2019993501661232517[2] = state[5];
}
void H_13(double *state, double *unused, double *out_686531630628137031) {
   out_686531630628137031[0] = 0;
   out_686531630628137031[1] = 0;
   out_686531630628137031[2] = 0;
   out_686531630628137031[3] = 1;
   out_686531630628137031[4] = 0;
   out_686531630628137031[5] = 0;
   out_686531630628137031[6] = 0;
   out_686531630628137031[7] = 0;
   out_686531630628137031[8] = 0;
   out_686531630628137031[9] = 0;
   out_686531630628137031[10] = 0;
   out_686531630628137031[11] = 0;
   out_686531630628137031[12] = 0;
   out_686531630628137031[13] = 0;
   out_686531630628137031[14] = 0;
   out_686531630628137031[15] = 0;
   out_686531630628137031[16] = 0;
   out_686531630628137031[17] = 0;
   out_686531630628137031[18] = 0;
   out_686531630628137031[19] = 0;
   out_686531630628137031[20] = 0;
   out_686531630628137031[21] = 0;
   out_686531630628137031[22] = 1;
   out_686531630628137031[23] = 0;
   out_686531630628137031[24] = 0;
   out_686531630628137031[25] = 0;
   out_686531630628137031[26] = 0;
   out_686531630628137031[27] = 0;
   out_686531630628137031[28] = 0;
   out_686531630628137031[29] = 0;
   out_686531630628137031[30] = 0;
   out_686531630628137031[31] = 0;
   out_686531630628137031[32] = 0;
   out_686531630628137031[33] = 0;
   out_686531630628137031[34] = 0;
   out_686531630628137031[35] = 0;
   out_686531630628137031[36] = 0;
   out_686531630628137031[37] = 0;
   out_686531630628137031[38] = 0;
   out_686531630628137031[39] = 0;
   out_686531630628137031[40] = 0;
   out_686531630628137031[41] = 1;
   out_686531630628137031[42] = 0;
   out_686531630628137031[43] = 0;
   out_686531630628137031[44] = 0;
   out_686531630628137031[45] = 0;
   out_686531630628137031[46] = 0;
   out_686531630628137031[47] = 0;
   out_686531630628137031[48] = 0;
   out_686531630628137031[49] = 0;
   out_686531630628137031[50] = 0;
   out_686531630628137031[51] = 0;
   out_686531630628137031[52] = 0;
   out_686531630628137031[53] = 0;
}
void h_14(double *state, double *unused, double *out_705784034083826196) {
   out_705784034083826196[0] = state[6];
   out_705784034083826196[1] = state[7];
   out_705784034083826196[2] = state[8];
}
void H_14(double *state, double *unused, double *out_5608530626999568066) {
   out_5608530626999568066[0] = 0;
   out_5608530626999568066[1] = 0;
   out_5608530626999568066[2] = 0;
   out_5608530626999568066[3] = 0;
   out_5608530626999568066[4] = 0;
   out_5608530626999568066[5] = 0;
   out_5608530626999568066[6] = 1;
   out_5608530626999568066[7] = 0;
   out_5608530626999568066[8] = 0;
   out_5608530626999568066[9] = 0;
   out_5608530626999568066[10] = 0;
   out_5608530626999568066[11] = 0;
   out_5608530626999568066[12] = 0;
   out_5608530626999568066[13] = 0;
   out_5608530626999568066[14] = 0;
   out_5608530626999568066[15] = 0;
   out_5608530626999568066[16] = 0;
   out_5608530626999568066[17] = 0;
   out_5608530626999568066[18] = 0;
   out_5608530626999568066[19] = 0;
   out_5608530626999568066[20] = 0;
   out_5608530626999568066[21] = 0;
   out_5608530626999568066[22] = 0;
   out_5608530626999568066[23] = 0;
   out_5608530626999568066[24] = 0;
   out_5608530626999568066[25] = 1;
   out_5608530626999568066[26] = 0;
   out_5608530626999568066[27] = 0;
   out_5608530626999568066[28] = 0;
   out_5608530626999568066[29] = 0;
   out_5608530626999568066[30] = 0;
   out_5608530626999568066[31] = 0;
   out_5608530626999568066[32] = 0;
   out_5608530626999568066[33] = 0;
   out_5608530626999568066[34] = 0;
   out_5608530626999568066[35] = 0;
   out_5608530626999568066[36] = 0;
   out_5608530626999568066[37] = 0;
   out_5608530626999568066[38] = 0;
   out_5608530626999568066[39] = 0;
   out_5608530626999568066[40] = 0;
   out_5608530626999568066[41] = 0;
   out_5608530626999568066[42] = 0;
   out_5608530626999568066[43] = 0;
   out_5608530626999568066[44] = 1;
   out_5608530626999568066[45] = 0;
   out_5608530626999568066[46] = 0;
   out_5608530626999568066[47] = 0;
   out_5608530626999568066[48] = 0;
   out_5608530626999568066[49] = 0;
   out_5608530626999568066[50] = 0;
   out_5608530626999568066[51] = 0;
   out_5608530626999568066[52] = 0;
   out_5608530626999568066[53] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_6596695233229181449) {
  err_fun(nom_x, delta_x, out_6596695233229181449);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5518483594158638078) {
  inv_err_fun(nom_x, true_x, out_5518483594158638078);
}
void pose_H_mod_fun(double *state, double *out_6297549404548103342) {
  H_mod_fun(state, out_6297549404548103342);
}
void pose_f_fun(double *state, double dt, double *out_5522725547351082526) {
  f_fun(state,  dt, out_5522725547351082526);
}
void pose_F_fun(double *state, double dt, double *out_1242349556554524426) {
  F_fun(state,  dt, out_1242349556554524426);
}
void pose_h_4(double *state, double *unused, double *out_8006082199547920028) {
  h_4(state, unused, out_8006082199547920028);
}
void pose_H_4(double *state, double *unused, double *out_2525742194704195770) {
  H_4(state, unused, out_2525742194704195770);
}
void pose_h_10(double *state, double *unused, double *out_380609954534953758) {
  h_10(state, unused, out_380609954534953758);
}
void pose_H_10(double *state, double *unused, double *out_4075325651889070173) {
  H_10(state, unused, out_4075325651889070173);
}
void pose_h_13(double *state, double *unused, double *out_2019993501661232517) {
  h_13(state, unused, out_2019993501661232517);
}
void pose_H_13(double *state, double *unused, double *out_686531630628137031) {
  H_13(state, unused, out_686531630628137031);
}
void pose_h_14(double *state, double *unused, double *out_705784034083826196) {
  h_14(state, unused, out_705784034083826196);
}
void pose_H_14(double *state, double *unused, double *out_5608530626999568066) {
  H_14(state, unused, out_5608530626999568066);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
