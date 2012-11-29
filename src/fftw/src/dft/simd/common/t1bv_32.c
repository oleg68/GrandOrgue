/*
 * Copyright (c) 2003, 2007-11 Matteo Frigo
 * Copyright (c) 2003, 2007-11 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Sat Apr 28 11:02:01 EDT 2012 */

#include "codelet-dft.h"

#ifdef HAVE_FMA

/* Generated by: ../../../genfft/gen_twiddle_c.native -fma -reorder-insns -schedule-for-pipeline -simd -compact -variables 4 -pipeline-latency 8 -n 32 -name t1bv_32 -include t1b.h -sign 1 */

/*
 * This function contains 217 FP additions, 160 FP multiplications,
 * (or, 119 additions, 62 multiplications, 98 fused multiply/add),
 * 104 stack variables, 7 constants, and 64 memory accesses
 */
#include "t1b.h"

static void t1bv_32(R *ri, R *ii, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DVK(KP831469612, +0.831469612302545237078788377617905756738560812);
     DVK(KP980785280, +0.980785280403230449126182236134239036973933731);
     DVK(KP668178637, +0.668178637919298919997757686523080761552472251);
     DVK(KP198912367, +0.198912367379658006911597622644676228597850501);
     DVK(KP923879532, +0.923879532511286756128183189396788286822416626);
     DVK(KP707106781, +0.707106781186547524400844362104849039284835938);
     DVK(KP414213562, +0.414213562373095048801688724209698078569671875);
     {
	  INT m;
	  R *x;
	  x = ii;
	  for (m = mb, W = W + (mb * ((TWVL / VL) * 62)); m < me; m = m + VL, x = x + (VL * ms), W = W + (TWVL * 62), MAKE_VOLATILE_STRIDE(rs)) {
	       V T26, T25, T2a, T2i, T24, T2c, T2g, T2k, T2h, T27;
	       {
		    V T4, T1z, T2o, T32, T2r, T3f, Tf, T1A, T34, T2O, T1D, TC, T33, T2L, T1C;
		    V Tr, T2C, T3a, T2F, T3b, T1r, T21, T1k, T20, TQ, TM, TS, TL, T2t, TJ;
		    V T10, T2u;
		    {
			 V Tt, T9, T2p, Te, T2q, TA, Tu, Tx;
			 {
			      V T1, T1x, T2, T1v;
			      T1 = LD(&(x[0]), ms, &(x[0]));
			      T1x = LD(&(x[WS(rs, 24)]), ms, &(x[0]));
			      T2 = LD(&(x[WS(rs, 16)]), ms, &(x[0]));
			      T1v = LD(&(x[WS(rs, 8)]), ms, &(x[0]));
			      {
				   V T5, Tc, T7, Ta, T2m, T2n;
				   T5 = LD(&(x[WS(rs, 4)]), ms, &(x[0]));
				   Tc = LD(&(x[WS(rs, 12)]), ms, &(x[0]));
				   T7 = LD(&(x[WS(rs, 20)]), ms, &(x[0]));
				   Ta = LD(&(x[WS(rs, 28)]), ms, &(x[0]));
				   {
					V T1y, T3, T1w, T6, Td, T8, Tb, Ts, Tz;
					Ts = LD(&(x[WS(rs, 30)]), ms, &(x[0]));
					T1y = BYTW(&(W[TWVL * 46]), T1x);
					T3 = BYTW(&(W[TWVL * 30]), T2);
					T1w = BYTW(&(W[TWVL * 14]), T1v);
					T6 = BYTW(&(W[TWVL * 6]), T5);
					Td = BYTW(&(W[TWVL * 22]), Tc);
					T8 = BYTW(&(W[TWVL * 38]), T7);
					Tb = BYTW(&(W[TWVL * 54]), Ta);
					Tt = BYTW(&(W[TWVL * 58]), Ts);
					Tz = LD(&(x[WS(rs, 6)]), ms, &(x[0]));
					T4 = VSUB(T1, T3);
					T2m = VADD(T1, T3);
					T1z = VSUB(T1w, T1y);
					T2n = VADD(T1w, T1y);
					T9 = VSUB(T6, T8);
					T2p = VADD(T6, T8);
					Te = VSUB(Tb, Td);
					T2q = VADD(Tb, Td);
					TA = BYTW(&(W[TWVL * 10]), Tz);
				   }
				   Tu = LD(&(x[WS(rs, 14)]), ms, &(x[0]));
				   T2o = VADD(T2m, T2n);
				   T32 = VSUB(T2m, T2n);
				   Tx = LD(&(x[WS(rs, 22)]), ms, &(x[0]));
			      }
			 }
			 {
			      V Tv, To, Ty, Ti, Tj, Tm, Th;
			      Th = LD(&(x[WS(rs, 2)]), ms, &(x[0]));
			      T2r = VADD(T2p, T2q);
			      T3f = VSUB(T2p, T2q);
			      Tf = VADD(T9, Te);
			      T1A = VSUB(T9, Te);
			      Tv = BYTW(&(W[TWVL * 26]), Tu);
			      To = LD(&(x[WS(rs, 26)]), ms, &(x[0]));
			      Ty = BYTW(&(W[TWVL * 42]), Tx);
			      Ti = BYTW(&(W[TWVL * 2]), Th);
			      Tj = LD(&(x[WS(rs, 18)]), ms, &(x[0]));
			      Tm = LD(&(x[WS(rs, 10)]), ms, &(x[0]));
			      {
				   V T1f, T1h, T1a, T1c, T18, T2A, T2B, T1p;
				   {
					V T15, T17, T1o, T1m;
					{
					     V Tw, T2M, Tp, T2N, TB, Tk, Tn, T1n, T14, T16;
					     T14 = LD(&(x[WS(rs, 31)]), ms, &(x[WS(rs, 1)]));
					     T16 = LD(&(x[WS(rs, 15)]), ms, &(x[WS(rs, 1)]));
					     Tw = VSUB(Tt, Tv);
					     T2M = VADD(Tt, Tv);
					     Tp = BYTW(&(W[TWVL * 50]), To);
					     T2N = VADD(TA, Ty);
					     TB = VSUB(Ty, TA);
					     Tk = BYTW(&(W[TWVL * 34]), Tj);
					     Tn = BYTW(&(W[TWVL * 18]), Tm);
					     T15 = BYTW(&(W[TWVL * 60]), T14);
					     T17 = BYTW(&(W[TWVL * 28]), T16);
					     T1n = LD(&(x[WS(rs, 7)]), ms, &(x[WS(rs, 1)]));
					     {
						  V T2J, Tl, T2K, Tq, T1l;
						  T1l = LD(&(x[WS(rs, 23)]), ms, &(x[WS(rs, 1)]));
						  T34 = VSUB(T2M, T2N);
						  T2O = VADD(T2M, T2N);
						  T1D = VFMA(LDK(KP414213562), Tw, TB);
						  TC = VFNMS(LDK(KP414213562), TB, Tw);
						  T2J = VADD(Ti, Tk);
						  Tl = VSUB(Ti, Tk);
						  T2K = VADD(Tn, Tp);
						  Tq = VSUB(Tn, Tp);
						  T1o = BYTW(&(W[TWVL * 12]), T1n);
						  T1m = BYTW(&(W[TWVL * 44]), T1l);
						  {
						       V T1e, T1g, T19, T1b;
						       T1e = LD(&(x[WS(rs, 27)]), ms, &(x[WS(rs, 1)]));
						       T1g = LD(&(x[WS(rs, 11)]), ms, &(x[WS(rs, 1)]));
						       T19 = LD(&(x[WS(rs, 3)]), ms, &(x[WS(rs, 1)]));
						       T1b = LD(&(x[WS(rs, 19)]), ms, &(x[WS(rs, 1)]));
						       T33 = VSUB(T2J, T2K);
						       T2L = VADD(T2J, T2K);
						       T1C = VFMA(LDK(KP414213562), Tl, Tq);
						       Tr = VFNMS(LDK(KP414213562), Tq, Tl);
						       T1f = BYTW(&(W[TWVL * 52]), T1e);
						       T1h = BYTW(&(W[TWVL * 20]), T1g);
						       T1a = BYTW(&(W[TWVL * 4]), T19);
						       T1c = BYTW(&(W[TWVL * 36]), T1b);
						  }
					     }
					}
					T18 = VSUB(T15, T17);
					T2A = VADD(T15, T17);
					T2B = VADD(T1o, T1m);
					T1p = VSUB(T1m, T1o);
				   }
				   {
					V TG, TI, TZ, TX;
					{
					     V T1i, T2E, T1d, T2D, TH, TY, TF;
					     TF = LD(&(x[WS(rs, 1)]), ms, &(x[WS(rs, 1)]));
					     T1i = VSUB(T1f, T1h);
					     T2E = VADD(T1f, T1h);
					     T1d = VSUB(T1a, T1c);
					     T2D = VADD(T1a, T1c);
					     TH = LD(&(x[WS(rs, 17)]), ms, &(x[WS(rs, 1)]));
					     TY = LD(&(x[WS(rs, 25)]), ms, &(x[WS(rs, 1)]));
					     T2C = VADD(T2A, T2B);
					     T3a = VSUB(T2A, T2B);
					     TG = BYTW(&(W[0]), TF);
					     {
						  V TW, T1j, T1q, TP, TR, TK;
						  TW = LD(&(x[WS(rs, 9)]), ms, &(x[WS(rs, 1)]));
						  T2F = VADD(T2D, T2E);
						  T3b = VSUB(T2E, T2D);
						  T1j = VADD(T1d, T1i);
						  T1q = VSUB(T1i, T1d);
						  TI = BYTW(&(W[TWVL * 32]), TH);
						  TZ = BYTW(&(W[TWVL * 48]), TY);
						  TP = LD(&(x[WS(rs, 29)]), ms, &(x[WS(rs, 1)]));
						  TX = BYTW(&(W[TWVL * 16]), TW);
						  TR = LD(&(x[WS(rs, 13)]), ms, &(x[WS(rs, 1)]));
						  TK = LD(&(x[WS(rs, 5)]), ms, &(x[WS(rs, 1)]));
						  T1r = VFMA(LDK(KP707106781), T1q, T1p);
						  T21 = VFNMS(LDK(KP707106781), T1q, T1p);
						  T1k = VFMA(LDK(KP707106781), T1j, T18);
						  T20 = VFNMS(LDK(KP707106781), T1j, T18);
						  TQ = BYTW(&(W[TWVL * 56]), TP);
						  TM = LD(&(x[WS(rs, 21)]), ms, &(x[WS(rs, 1)]));
						  TS = BYTW(&(W[TWVL * 24]), TR);
						  TL = BYTW(&(W[TWVL * 8]), TK);
					     }
					}
					T2t = VADD(TG, TI);
					TJ = VSUB(TG, TI);
					T10 = VSUB(TX, TZ);
					T2u = VADD(TX, TZ);
				   }
			      }
			 }
		    }
		    {
			 V T2s, TT, T2x, T2P, T2Y, T2G, T37, T2v, T2w, TO, T2W, T30, T2U, TN, T2V;
			 T2s = VSUB(T2o, T2r);
			 T2U = VADD(T2o, T2r);
			 TN = BYTW(&(W[TWVL * 40]), TM);
			 TT = VSUB(TQ, TS);
			 T2x = VADD(TQ, TS);
			 T2P = VSUB(T2L, T2O);
			 T2V = VADD(T2L, T2O);
			 T2Y = VADD(T2C, T2F);
			 T2G = VSUB(T2C, T2F);
			 T37 = VSUB(T2t, T2u);
			 T2v = VADD(T2t, T2u);
			 T2w = VADD(TL, TN);
			 TO = VSUB(TL, TN);
			 T2W = VSUB(T2U, T2V);
			 T30 = VADD(T2U, T2V);
			 {
			      V T1Y, T12, T1X, TV, T3n, T3t, T3m, T3q;
			      {
				   V T3o, T36, T3r, T3h, T3k, T3p, T3d, T3s, T2H, T2Q, T2Z, T31;
				   {
					V T35, T3g, T38, T2y, T11, TU, T3c, T3j;
					T35 = VADD(T33, T34);
					T3g = VSUB(T33, T34);
					T38 = VSUB(T2w, T2x);
					T2y = VADD(T2w, T2x);
					T11 = VSUB(TO, TT);
					TU = VADD(TO, TT);
					T3c = VFNMS(LDK(KP414213562), T3b, T3a);
					T3j = VFMA(LDK(KP414213562), T3a, T3b);
					T3o = VFNMS(LDK(KP707106781), T35, T32);
					T36 = VFMA(LDK(KP707106781), T35, T32);
					T3r = VFNMS(LDK(KP707106781), T3g, T3f);
					T3h = VFMA(LDK(KP707106781), T3g, T3f);
					{
					     V T3i, T39, T2z, T2X;
					     T3i = VFMA(LDK(KP414213562), T37, T38);
					     T39 = VFNMS(LDK(KP414213562), T38, T37);
					     T2z = VSUB(T2v, T2y);
					     T2X = VADD(T2v, T2y);
					     T1Y = VFNMS(LDK(KP707106781), T11, T10);
					     T12 = VFMA(LDK(KP707106781), T11, T10);
					     T1X = VFNMS(LDK(KP707106781), TU, TJ);
					     TV = VFMA(LDK(KP707106781), TU, TJ);
					     T3k = VSUB(T3i, T3j);
					     T3p = VADD(T3i, T3j);
					     T3d = VADD(T39, T3c);
					     T3s = VSUB(T39, T3c);
					     T2H = VADD(T2z, T2G);
					     T2Q = VSUB(T2z, T2G);
					     T2Z = VSUB(T2X, T2Y);
					     T31 = VADD(T2X, T2Y);
					}
				   }
				   {
					V T3v, T3u, T3l, T3e;
					T3l = VFNMS(LDK(KP923879532), T3k, T3h);
					T3n = VFMA(LDK(KP923879532), T3k, T3h);
					T3t = VFMA(LDK(KP923879532), T3s, T3r);
					T3v = VFNMS(LDK(KP923879532), T3s, T3r);
					T3e = VFNMS(LDK(KP923879532), T3d, T36);
					T3m = VFMA(LDK(KP923879532), T3d, T36);
					{
					     V T2R, T2T, T2I, T2S;
					     T2R = VFNMS(LDK(KP707106781), T2Q, T2P);
					     T2T = VFMA(LDK(KP707106781), T2Q, T2P);
					     T2I = VFNMS(LDK(KP707106781), T2H, T2s);
					     T2S = VFMA(LDK(KP707106781), T2H, T2s);
					     ST(&(x[WS(rs, 16)]), VSUB(T30, T31), ms, &(x[0]));
					     ST(&(x[0]), VADD(T30, T31), ms, &(x[0]));
					     ST(&(x[WS(rs, 8)]), VFMAI(T2Z, T2W), ms, &(x[0]));
					     ST(&(x[WS(rs, 24)]), VFNMSI(T2Z, T2W), ms, &(x[0]));
					     T3q = VFNMS(LDK(KP923879532), T3p, T3o);
					     T3u = VFMA(LDK(KP923879532), T3p, T3o);
					     ST(&(x[WS(rs, 18)]), VFMAI(T3l, T3e), ms, &(x[0]));
					     ST(&(x[WS(rs, 14)]), VFNMSI(T3l, T3e), ms, &(x[0]));
					     ST(&(x[WS(rs, 28)]), VFNMSI(T2T, T2S), ms, &(x[0]));
					     ST(&(x[WS(rs, 4)]), VFMAI(T2T, T2S), ms, &(x[0]));
					     ST(&(x[WS(rs, 20)]), VFMAI(T2R, T2I), ms, &(x[0]));
					     ST(&(x[WS(rs, 12)]), VFNMSI(T2R, T2I), ms, &(x[0]));
					}
					ST(&(x[WS(rs, 26)]), VFMAI(T3v, T3u), ms, &(x[0]));
					ST(&(x[WS(rs, 6)]), VFNMSI(T3v, T3u), ms, &(x[0]));
				   }
			      }
			      {
				   V T1U, T13, T1s, TE, T1M, T1I, T1N, T1B, T1V, T1E;
				   {
					V Tg, TD, T1G, T1H;
					Tg = VFMA(LDK(KP707106781), Tf, T4);
					T1U = VFNMS(LDK(KP707106781), Tf, T4);
					T26 = VSUB(Tr, TC);
					TD = VADD(Tr, TC);
					T1G = VFMA(LDK(KP198912367), TV, T12);
					T13 = VFNMS(LDK(KP198912367), T12, TV);
					T1s = VFNMS(LDK(KP198912367), T1r, T1k);
					T1H = VFMA(LDK(KP198912367), T1k, T1r);
					ST(&(x[WS(rs, 2)]), VFMAI(T3n, T3m), ms, &(x[0]));
					ST(&(x[WS(rs, 30)]), VFNMSI(T3n, T3m), ms, &(x[0]));
					ST(&(x[WS(rs, 22)]), VFNMSI(T3t, T3q), ms, &(x[0]));
					ST(&(x[WS(rs, 10)]), VFMAI(T3t, T3q), ms, &(x[0]));
					TE = VFMA(LDK(KP923879532), TD, Tg);
					T1M = VFNMS(LDK(KP923879532), TD, Tg);
					T1I = VSUB(T1G, T1H);
					T1N = VADD(T1G, T1H);
					T1B = VFMA(LDK(KP707106781), T1A, T1z);
					T25 = VFNMS(LDK(KP707106781), T1A, T1z);
					T1V = VADD(T1C, T1D);
					T1E = VSUB(T1C, T1D);
				   }
				   {
					V T1W, T2e, T2f, T23;
					{
					     V T28, T1Z, T1S, T1O, T1t, T1Q, T1F, T1P, T22, T29;
					     T28 = VFNMS(LDK(KP668178637), T1X, T1Y);
					     T1Z = VFMA(LDK(KP668178637), T1Y, T1X);
					     T1S = VFMA(LDK(KP980785280), T1N, T1M);
					     T1O = VFNMS(LDK(KP980785280), T1N, T1M);
					     T1t = VADD(T13, T1s);
					     T1Q = VSUB(T13, T1s);
					     T1F = VFMA(LDK(KP923879532), T1E, T1B);
					     T1P = VFNMS(LDK(KP923879532), T1E, T1B);
					     T1W = VFMA(LDK(KP923879532), T1V, T1U);
					     T2e = VFNMS(LDK(KP923879532), T1V, T1U);
					     T22 = VFMA(LDK(KP668178637), T21, T20);
					     T29 = VFNMS(LDK(KP668178637), T20, T21);
					     {
						  V T1K, T1u, T1R, T1T, T1L, T1J;
						  T1K = VFMA(LDK(KP980785280), T1t, TE);
						  T1u = VFNMS(LDK(KP980785280), T1t, TE);
						  T1R = VFMA(LDK(KP980785280), T1Q, T1P);
						  T1T = VFNMS(LDK(KP980785280), T1Q, T1P);
						  T1L = VFMA(LDK(KP980785280), T1I, T1F);
						  T1J = VFNMS(LDK(KP980785280), T1I, T1F);
						  T2f = VADD(T28, T29);
						  T2a = VSUB(T28, T29);
						  T23 = VADD(T1Z, T22);
						  T2i = VSUB(T1Z, T22);
						  ST(&(x[WS(rs, 23)]), VFNMSI(T1R, T1O), ms, &(x[WS(rs, 1)]));
						  ST(&(x[WS(rs, 9)]), VFMAI(T1R, T1O), ms, &(x[WS(rs, 1)]));
						  ST(&(x[WS(rs, 25)]), VFMAI(T1T, T1S), ms, &(x[WS(rs, 1)]));
						  ST(&(x[WS(rs, 7)]), VFNMSI(T1T, T1S), ms, &(x[WS(rs, 1)]));
						  ST(&(x[WS(rs, 1)]), VFMAI(T1L, T1K), ms, &(x[WS(rs, 1)]));
						  ST(&(x[WS(rs, 31)]), VFNMSI(T1L, T1K), ms, &(x[WS(rs, 1)]));
						  ST(&(x[WS(rs, 17)]), VFMAI(T1J, T1u), ms, &(x[WS(rs, 1)]));
						  ST(&(x[WS(rs, 15)]), VFNMSI(T1J, T1u), ms, &(x[WS(rs, 1)]));
					     }
					}
					T24 = VFNMS(LDK(KP831469612), T23, T1W);
					T2c = VFMA(LDK(KP831469612), T23, T1W);
					T2g = VFMA(LDK(KP831469612), T2f, T2e);
					T2k = VFNMS(LDK(KP831469612), T2f, T2e);
				   }
			      }
			 }
		    }
	       }
	       T2h = VFMA(LDK(KP923879532), T26, T25);
	       T27 = VFNMS(LDK(KP923879532), T26, T25);
	       {
		    V T2j, T2l, T2d, T2b;
		    T2j = VFNMS(LDK(KP831469612), T2i, T2h);
		    T2l = VFMA(LDK(KP831469612), T2i, T2h);
		    T2d = VFMA(LDK(KP831469612), T2a, T27);
		    T2b = VFNMS(LDK(KP831469612), T2a, T27);
		    ST(&(x[WS(rs, 21)]), VFMAI(T2j, T2g), ms, &(x[WS(rs, 1)]));
		    ST(&(x[WS(rs, 11)]), VFNMSI(T2j, T2g), ms, &(x[WS(rs, 1)]));
		    ST(&(x[WS(rs, 27)]), VFNMSI(T2l, T2k), ms, &(x[WS(rs, 1)]));
		    ST(&(x[WS(rs, 5)]), VFMAI(T2l, T2k), ms, &(x[WS(rs, 1)]));
		    ST(&(x[WS(rs, 29)]), VFMAI(T2d, T2c), ms, &(x[WS(rs, 1)]));
		    ST(&(x[WS(rs, 3)]), VFNMSI(T2d, T2c), ms, &(x[WS(rs, 1)]));
		    ST(&(x[WS(rs, 13)]), VFMAI(T2b, T24), ms, &(x[WS(rs, 1)]));
		    ST(&(x[WS(rs, 19)]), VFNMSI(T2b, T24), ms, &(x[WS(rs, 1)]));
	       }
	  }
     }
     VLEAVE();
}

static const tw_instr twinstr[] = {
     VTW(0, 1),
     VTW(0, 2),
     VTW(0, 3),
     VTW(0, 4),
     VTW(0, 5),
     VTW(0, 6),
     VTW(0, 7),
     VTW(0, 8),
     VTW(0, 9),
     VTW(0, 10),
     VTW(0, 11),
     VTW(0, 12),
     VTW(0, 13),
     VTW(0, 14),
     VTW(0, 15),
     VTW(0, 16),
     VTW(0, 17),
     VTW(0, 18),
     VTW(0, 19),
     VTW(0, 20),
     VTW(0, 21),
     VTW(0, 22),
     VTW(0, 23),
     VTW(0, 24),
     VTW(0, 25),
     VTW(0, 26),
     VTW(0, 27),
     VTW(0, 28),
     VTW(0, 29),
     VTW(0, 30),
     VTW(0, 31),
     {TW_NEXT, VL, 0}
};

static const ct_desc desc = { 32, XSIMD_STRING("t1bv_32"), twinstr, &GENUS, {119, 62, 98, 0}, 0, 0, 0 };

void XSIMD(codelet_t1bv_32) (planner *p) {
     X(kdft_dit_register) (p, t1bv_32, &desc);
}
#else				/* HAVE_FMA */

/* Generated by: ../../../genfft/gen_twiddle_c.native -simd -compact -variables 4 -pipeline-latency 8 -n 32 -name t1bv_32 -include t1b.h -sign 1 */

/*
 * This function contains 217 FP additions, 104 FP multiplications,
 * (or, 201 additions, 88 multiplications, 16 fused multiply/add),
 * 59 stack variables, 7 constants, and 64 memory accesses
 */
#include "t1b.h"

static void t1bv_32(R *ri, R *ii, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DVK(KP195090322, +0.195090322016128267848284868477022240927691618);
     DVK(KP980785280, +0.980785280403230449126182236134239036973933731);
     DVK(KP555570233, +0.555570233019602224742830813948532874374937191);
     DVK(KP831469612, +0.831469612302545237078788377617905756738560812);
     DVK(KP382683432, +0.382683432365089771728459984030398866761344562);
     DVK(KP923879532, +0.923879532511286756128183189396788286822416626);
     DVK(KP707106781, +0.707106781186547524400844362104849039284835938);
     {
	  INT m;
	  R *x;
	  x = ii;
	  for (m = mb, W = W + (mb * ((TWVL / VL) * 62)); m < me; m = m + VL, x = x + (VL * ms), W = W + (TWVL * 62), MAKE_VOLATILE_STRIDE(rs)) {
	       V T4, T1D, T2P, T3h, Tf, T1y, T2K, T3i, TC, T1w, T2G, T3e, Tr, T1v, T2D;
	       V T3d, T1k, T20, T2y, T3a, T1r, T21, T2v, T39, TV, T1X, T2r, T37, T12, T1Y;
	       V T2o, T36;
	       {
		    V T1, T1C, T3, T1A, T1B, T2, T1z, T2N, T2O;
		    T1 = LD(&(x[0]), ms, &(x[0]));
		    T1B = LD(&(x[WS(rs, 24)]), ms, &(x[0]));
		    T1C = BYTW(&(W[TWVL * 46]), T1B);
		    T2 = LD(&(x[WS(rs, 16)]), ms, &(x[0]));
		    T3 = BYTW(&(W[TWVL * 30]), T2);
		    T1z = LD(&(x[WS(rs, 8)]), ms, &(x[0]));
		    T1A = BYTW(&(W[TWVL * 14]), T1z);
		    T4 = VSUB(T1, T3);
		    T1D = VSUB(T1A, T1C);
		    T2N = VADD(T1, T3);
		    T2O = VADD(T1A, T1C);
		    T2P = VSUB(T2N, T2O);
		    T3h = VADD(T2N, T2O);
	       }
	       {
		    V T6, Td, T8, Tb;
		    {
			 V T5, Tc, T7, Ta;
			 T5 = LD(&(x[WS(rs, 4)]), ms, &(x[0]));
			 T6 = BYTW(&(W[TWVL * 6]), T5);
			 Tc = LD(&(x[WS(rs, 12)]), ms, &(x[0]));
			 Td = BYTW(&(W[TWVL * 22]), Tc);
			 T7 = LD(&(x[WS(rs, 20)]), ms, &(x[0]));
			 T8 = BYTW(&(W[TWVL * 38]), T7);
			 Ta = LD(&(x[WS(rs, 28)]), ms, &(x[0]));
			 Tb = BYTW(&(W[TWVL * 54]), Ta);
		    }
		    {
			 V T9, Te, T2I, T2J;
			 T9 = VSUB(T6, T8);
			 Te = VSUB(Tb, Td);
			 Tf = VMUL(LDK(KP707106781), VADD(T9, Te));
			 T1y = VMUL(LDK(KP707106781), VSUB(T9, Te));
			 T2I = VADD(T6, T8);
			 T2J = VADD(Tb, Td);
			 T2K = VSUB(T2I, T2J);
			 T3i = VADD(T2I, T2J);
		    }
	       }
	       {
		    V Tt, TA, Tv, Ty;
		    {
			 V Ts, Tz, Tu, Tx;
			 Ts = LD(&(x[WS(rs, 6)]), ms, &(x[0]));
			 Tt = BYTW(&(W[TWVL * 10]), Ts);
			 Tz = LD(&(x[WS(rs, 14)]), ms, &(x[0]));
			 TA = BYTW(&(W[TWVL * 26]), Tz);
			 Tu = LD(&(x[WS(rs, 22)]), ms, &(x[0]));
			 Tv = BYTW(&(W[TWVL * 42]), Tu);
			 Tx = LD(&(x[WS(rs, 30)]), ms, &(x[0]));
			 Ty = BYTW(&(W[TWVL * 58]), Tx);
		    }
		    {
			 V Tw, TB, T2E, T2F;
			 Tw = VSUB(Tt, Tv);
			 TB = VSUB(Ty, TA);
			 TC = VFNMS(LDK(KP382683432), TB, VMUL(LDK(KP923879532), Tw));
			 T1w = VFMA(LDK(KP923879532), TB, VMUL(LDK(KP382683432), Tw));
			 T2E = VADD(Ty, TA);
			 T2F = VADD(Tt, Tv);
			 T2G = VSUB(T2E, T2F);
			 T3e = VADD(T2E, T2F);
		    }
	       }
	       {
		    V Ti, Tp, Tk, Tn;
		    {
			 V Th, To, Tj, Tm;
			 Th = LD(&(x[WS(rs, 2)]), ms, &(x[0]));
			 Ti = BYTW(&(W[TWVL * 2]), Th);
			 To = LD(&(x[WS(rs, 26)]), ms, &(x[0]));
			 Tp = BYTW(&(W[TWVL * 50]), To);
			 Tj = LD(&(x[WS(rs, 18)]), ms, &(x[0]));
			 Tk = BYTW(&(W[TWVL * 34]), Tj);
			 Tm = LD(&(x[WS(rs, 10)]), ms, &(x[0]));
			 Tn = BYTW(&(W[TWVL * 18]), Tm);
		    }
		    {
			 V Tl, Tq, T2B, T2C;
			 Tl = VSUB(Ti, Tk);
			 Tq = VSUB(Tn, Tp);
			 Tr = VFMA(LDK(KP382683432), Tl, VMUL(LDK(KP923879532), Tq));
			 T1v = VFNMS(LDK(KP382683432), Tq, VMUL(LDK(KP923879532), Tl));
			 T2B = VADD(Ti, Tk);
			 T2C = VADD(Tn, Tp);
			 T2D = VSUB(T2B, T2C);
			 T3d = VADD(T2B, T2C);
		    }
	       }
	       {
		    V T1g, T1i, T1o, T1m, T1a, T1c, T1d, T15, T17, T18;
		    {
			 V T1f, T1h, T1n, T1l;
			 T1f = LD(&(x[WS(rs, 7)]), ms, &(x[WS(rs, 1)]));
			 T1g = BYTW(&(W[TWVL * 12]), T1f);
			 T1h = LD(&(x[WS(rs, 23)]), ms, &(x[WS(rs, 1)]));
			 T1i = BYTW(&(W[TWVL * 44]), T1h);
			 T1n = LD(&(x[WS(rs, 15)]), ms, &(x[WS(rs, 1)]));
			 T1o = BYTW(&(W[TWVL * 28]), T1n);
			 T1l = LD(&(x[WS(rs, 31)]), ms, &(x[WS(rs, 1)]));
			 T1m = BYTW(&(W[TWVL * 60]), T1l);
			 {
			      V T19, T1b, T14, T16;
			      T19 = LD(&(x[WS(rs, 27)]), ms, &(x[WS(rs, 1)]));
			      T1a = BYTW(&(W[TWVL * 52]), T19);
			      T1b = LD(&(x[WS(rs, 11)]), ms, &(x[WS(rs, 1)]));
			      T1c = BYTW(&(W[TWVL * 20]), T1b);
			      T1d = VSUB(T1a, T1c);
			      T14 = LD(&(x[WS(rs, 3)]), ms, &(x[WS(rs, 1)]));
			      T15 = BYTW(&(W[TWVL * 4]), T14);
			      T16 = LD(&(x[WS(rs, 19)]), ms, &(x[WS(rs, 1)]));
			      T17 = BYTW(&(W[TWVL * 36]), T16);
			      T18 = VSUB(T15, T17);
			 }
		    }
		    {
			 V T1e, T1j, T2w, T2x;
			 T1e = VMUL(LDK(KP707106781), VSUB(T18, T1d));
			 T1j = VSUB(T1g, T1i);
			 T1k = VSUB(T1e, T1j);
			 T20 = VADD(T1j, T1e);
			 T2w = VADD(T15, T17);
			 T2x = VADD(T1a, T1c);
			 T2y = VSUB(T2w, T2x);
			 T3a = VADD(T2w, T2x);
		    }
		    {
			 V T1p, T1q, T2t, T2u;
			 T1p = VSUB(T1m, T1o);
			 T1q = VMUL(LDK(KP707106781), VADD(T18, T1d));
			 T1r = VSUB(T1p, T1q);
			 T21 = VADD(T1p, T1q);
			 T2t = VADD(T1m, T1o);
			 T2u = VADD(T1g, T1i);
			 T2v = VSUB(T2t, T2u);
			 T39 = VADD(T2t, T2u);
		    }
	       }
	       {
		    V TR, TT, TZ, TX, TL, TN, TO, TG, TI, TJ;
		    {
			 V TQ, TS, TY, TW;
			 TQ = LD(&(x[WS(rs, 9)]), ms, &(x[WS(rs, 1)]));
			 TR = BYTW(&(W[TWVL * 16]), TQ);
			 TS = LD(&(x[WS(rs, 25)]), ms, &(x[WS(rs, 1)]));
			 TT = BYTW(&(W[TWVL * 48]), TS);
			 TY = LD(&(x[WS(rs, 17)]), ms, &(x[WS(rs, 1)]));
			 TZ = BYTW(&(W[TWVL * 32]), TY);
			 TW = LD(&(x[WS(rs, 1)]), ms, &(x[WS(rs, 1)]));
			 TX = BYTW(&(W[0]), TW);
			 {
			      V TK, TM, TF, TH;
			      TK = LD(&(x[WS(rs, 29)]), ms, &(x[WS(rs, 1)]));
			      TL = BYTW(&(W[TWVL * 56]), TK);
			      TM = LD(&(x[WS(rs, 13)]), ms, &(x[WS(rs, 1)]));
			      TN = BYTW(&(W[TWVL * 24]), TM);
			      TO = VSUB(TL, TN);
			      TF = LD(&(x[WS(rs, 5)]), ms, &(x[WS(rs, 1)]));
			      TG = BYTW(&(W[TWVL * 8]), TF);
			      TH = LD(&(x[WS(rs, 21)]), ms, &(x[WS(rs, 1)]));
			      TI = BYTW(&(W[TWVL * 40]), TH);
			      TJ = VSUB(TG, TI);
			 }
		    }
		    {
			 V TP, TU, T2p, T2q;
			 TP = VMUL(LDK(KP707106781), VSUB(TJ, TO));
			 TU = VSUB(TR, TT);
			 TV = VSUB(TP, TU);
			 T1X = VADD(TU, TP);
			 T2p = VADD(TG, TI);
			 T2q = VADD(TL, TN);
			 T2r = VSUB(T2p, T2q);
			 T37 = VADD(T2p, T2q);
		    }
		    {
			 V T10, T11, T2m, T2n;
			 T10 = VSUB(TX, TZ);
			 T11 = VMUL(LDK(KP707106781), VADD(TJ, TO));
			 T12 = VSUB(T10, T11);
			 T1Y = VADD(T10, T11);
			 T2m = VADD(TX, TZ);
			 T2n = VADD(TR, TT);
			 T2o = VSUB(T2m, T2n);
			 T36 = VADD(T2m, T2n);
		    }
	       }
	       {
		    V T3q, T3u, T3t, T3v;
		    {
			 V T3o, T3p, T3r, T3s;
			 T3o = VADD(T3h, T3i);
			 T3p = VADD(T3d, T3e);
			 T3q = VSUB(T3o, T3p);
			 T3u = VADD(T3o, T3p);
			 T3r = VADD(T36, T37);
			 T3s = VADD(T39, T3a);
			 T3t = VBYI(VSUB(T3r, T3s));
			 T3v = VADD(T3r, T3s);
		    }
		    ST(&(x[WS(rs, 24)]), VSUB(T3q, T3t), ms, &(x[0]));
		    ST(&(x[0]), VADD(T3u, T3v), ms, &(x[0]));
		    ST(&(x[WS(rs, 8)]), VADD(T3q, T3t), ms, &(x[0]));
		    ST(&(x[WS(rs, 16)]), VSUB(T3u, T3v), ms, &(x[0]));
	       }
	       {
		    V T3f, T3j, T3c, T3k, T38, T3b;
		    T3f = VSUB(T3d, T3e);
		    T3j = VSUB(T3h, T3i);
		    T38 = VSUB(T36, T37);
		    T3b = VSUB(T39, T3a);
		    T3c = VMUL(LDK(KP707106781), VSUB(T38, T3b));
		    T3k = VMUL(LDK(KP707106781), VADD(T38, T3b));
		    {
			 V T3g, T3l, T3m, T3n;
			 T3g = VBYI(VSUB(T3c, T3f));
			 T3l = VSUB(T3j, T3k);
			 ST(&(x[WS(rs, 12)]), VADD(T3g, T3l), ms, &(x[0]));
			 ST(&(x[WS(rs, 20)]), VSUB(T3l, T3g), ms, &(x[0]));
			 T3m = VBYI(VADD(T3f, T3c));
			 T3n = VADD(T3j, T3k);
			 ST(&(x[WS(rs, 4)]), VADD(T3m, T3n), ms, &(x[0]));
			 ST(&(x[WS(rs, 28)]), VSUB(T3n, T3m), ms, &(x[0]));
		    }
	       }
	       {
		    V T2L, T31, T2R, T2Y, T2A, T2Z, T2U, T32, T2H, T2Q;
		    T2H = VMUL(LDK(KP707106781), VSUB(T2D, T2G));
		    T2L = VSUB(T2H, T2K);
		    T31 = VADD(T2K, T2H);
		    T2Q = VMUL(LDK(KP707106781), VADD(T2D, T2G));
		    T2R = VSUB(T2P, T2Q);
		    T2Y = VADD(T2P, T2Q);
		    {
			 V T2s, T2z, T2S, T2T;
			 T2s = VFNMS(LDK(KP382683432), T2r, VMUL(LDK(KP923879532), T2o));
			 T2z = VFMA(LDK(KP923879532), T2v, VMUL(LDK(KP382683432), T2y));
			 T2A = VSUB(T2s, T2z);
			 T2Z = VADD(T2s, T2z);
			 T2S = VFMA(LDK(KP382683432), T2o, VMUL(LDK(KP923879532), T2r));
			 T2T = VFNMS(LDK(KP382683432), T2v, VMUL(LDK(KP923879532), T2y));
			 T2U = VSUB(T2S, T2T);
			 T32 = VADD(T2S, T2T);
		    }
		    {
			 V T2M, T2V, T34, T35;
			 T2M = VBYI(VSUB(T2A, T2L));
			 T2V = VSUB(T2R, T2U);
			 ST(&(x[WS(rs, 10)]), VADD(T2M, T2V), ms, &(x[0]));
			 ST(&(x[WS(rs, 22)]), VSUB(T2V, T2M), ms, &(x[0]));
			 T34 = VSUB(T2Y, T2Z);
			 T35 = VBYI(VSUB(T32, T31));
			 ST(&(x[WS(rs, 18)]), VSUB(T34, T35), ms, &(x[0]));
			 ST(&(x[WS(rs, 14)]), VADD(T34, T35), ms, &(x[0]));
		    }
		    {
			 V T2W, T2X, T30, T33;
			 T2W = VBYI(VADD(T2L, T2A));
			 T2X = VADD(T2R, T2U);
			 ST(&(x[WS(rs, 6)]), VADD(T2W, T2X), ms, &(x[0]));
			 ST(&(x[WS(rs, 26)]), VSUB(T2X, T2W), ms, &(x[0]));
			 T30 = VADD(T2Y, T2Z);
			 T33 = VBYI(VADD(T31, T32));
			 ST(&(x[WS(rs, 30)]), VSUB(T30, T33), ms, &(x[0]));
			 ST(&(x[WS(rs, 2)]), VADD(T30, T33), ms, &(x[0]));
		    }
	       }
	       {
		    V TE, T1P, T1I, T1Q, T1t, T1M, T1F, T1N;
		    {
			 V Tg, TD, T1G, T1H;
			 Tg = VSUB(T4, Tf);
			 TD = VSUB(Tr, TC);
			 TE = VSUB(Tg, TD);
			 T1P = VADD(Tg, TD);
			 T1G = VFNMS(LDK(KP555570233), TV, VMUL(LDK(KP831469612), T12));
			 T1H = VFMA(LDK(KP555570233), T1k, VMUL(LDK(KP831469612), T1r));
			 T1I = VSUB(T1G, T1H);
			 T1Q = VADD(T1G, T1H);
		    }
		    {
			 V T13, T1s, T1x, T1E;
			 T13 = VFMA(LDK(KP831469612), TV, VMUL(LDK(KP555570233), T12));
			 T1s = VFNMS(LDK(KP555570233), T1r, VMUL(LDK(KP831469612), T1k));
			 T1t = VSUB(T13, T1s);
			 T1M = VADD(T13, T1s);
			 T1x = VSUB(T1v, T1w);
			 T1E = VSUB(T1y, T1D);
			 T1F = VSUB(T1x, T1E);
			 T1N = VADD(T1E, T1x);
		    }
		    {
			 V T1u, T1J, T1S, T1T;
			 T1u = VADD(TE, T1t);
			 T1J = VBYI(VADD(T1F, T1I));
			 ST(&(x[WS(rs, 27)]), VSUB(T1u, T1J), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 5)]), VADD(T1u, T1J), ms, &(x[WS(rs, 1)]));
			 T1S = VBYI(VADD(T1N, T1M));
			 T1T = VADD(T1P, T1Q);
			 ST(&(x[WS(rs, 3)]), VADD(T1S, T1T), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 29)]), VSUB(T1T, T1S), ms, &(x[WS(rs, 1)]));
		    }
		    {
			 V T1K, T1L, T1O, T1R;
			 T1K = VSUB(TE, T1t);
			 T1L = VBYI(VSUB(T1I, T1F));
			 ST(&(x[WS(rs, 21)]), VSUB(T1K, T1L), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 11)]), VADD(T1K, T1L), ms, &(x[WS(rs, 1)]));
			 T1O = VBYI(VSUB(T1M, T1N));
			 T1R = VSUB(T1P, T1Q);
			 ST(&(x[WS(rs, 13)]), VADD(T1O, T1R), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 19)]), VSUB(T1R, T1O), ms, &(x[WS(rs, 1)]));
		    }
	       }
	       {
		    V T1W, T2h, T2a, T2i, T23, T2e, T27, T2f;
		    {
			 V T1U, T1V, T28, T29;
			 T1U = VADD(T4, Tf);
			 T1V = VADD(T1v, T1w);
			 T1W = VSUB(T1U, T1V);
			 T2h = VADD(T1U, T1V);
			 T28 = VFNMS(LDK(KP195090322), T1X, VMUL(LDK(KP980785280), T1Y));
			 T29 = VFMA(LDK(KP195090322), T20, VMUL(LDK(KP980785280), T21));
			 T2a = VSUB(T28, T29);
			 T2i = VADD(T28, T29);
		    }
		    {
			 V T1Z, T22, T25, T26;
			 T1Z = VFMA(LDK(KP980785280), T1X, VMUL(LDK(KP195090322), T1Y));
			 T22 = VFNMS(LDK(KP195090322), T21, VMUL(LDK(KP980785280), T20));
			 T23 = VSUB(T1Z, T22);
			 T2e = VADD(T1Z, T22);
			 T25 = VADD(Tr, TC);
			 T26 = VADD(T1D, T1y);
			 T27 = VSUB(T25, T26);
			 T2f = VADD(T26, T25);
		    }
		    {
			 V T24, T2b, T2k, T2l;
			 T24 = VADD(T1W, T23);
			 T2b = VBYI(VADD(T27, T2a));
			 ST(&(x[WS(rs, 25)]), VSUB(T24, T2b), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 7)]), VADD(T24, T2b), ms, &(x[WS(rs, 1)]));
			 T2k = VBYI(VADD(T2f, T2e));
			 T2l = VADD(T2h, T2i);
			 ST(&(x[WS(rs, 1)]), VADD(T2k, T2l), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 31)]), VSUB(T2l, T2k), ms, &(x[WS(rs, 1)]));
		    }
		    {
			 V T2c, T2d, T2g, T2j;
			 T2c = VSUB(T1W, T23);
			 T2d = VBYI(VSUB(T2a, T27));
			 ST(&(x[WS(rs, 23)]), VSUB(T2c, T2d), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 9)]), VADD(T2c, T2d), ms, &(x[WS(rs, 1)]));
			 T2g = VBYI(VSUB(T2e, T2f));
			 T2j = VSUB(T2h, T2i);
			 ST(&(x[WS(rs, 15)]), VADD(T2g, T2j), ms, &(x[WS(rs, 1)]));
			 ST(&(x[WS(rs, 17)]), VSUB(T2j, T2g), ms, &(x[WS(rs, 1)]));
		    }
	       }
	  }
     }
     VLEAVE();
}

static const tw_instr twinstr[] = {
     VTW(0, 1),
     VTW(0, 2),
     VTW(0, 3),
     VTW(0, 4),
     VTW(0, 5),
     VTW(0, 6),
     VTW(0, 7),
     VTW(0, 8),
     VTW(0, 9),
     VTW(0, 10),
     VTW(0, 11),
     VTW(0, 12),
     VTW(0, 13),
     VTW(0, 14),
     VTW(0, 15),
     VTW(0, 16),
     VTW(0, 17),
     VTW(0, 18),
     VTW(0, 19),
     VTW(0, 20),
     VTW(0, 21),
     VTW(0, 22),
     VTW(0, 23),
     VTW(0, 24),
     VTW(0, 25),
     VTW(0, 26),
     VTW(0, 27),
     VTW(0, 28),
     VTW(0, 29),
     VTW(0, 30),
     VTW(0, 31),
     {TW_NEXT, VL, 0}
};

static const ct_desc desc = { 32, XSIMD_STRING("t1bv_32"), twinstr, &GENUS, {201, 88, 16, 0}, 0, 0, 0 };

void XSIMD(codelet_t1bv_32) (planner *p) {
     X(kdft_dit_register) (p, t1bv_32, &desc);
}
#endif				/* HAVE_FMA */
