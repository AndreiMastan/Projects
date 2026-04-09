%% discretizare functie
s=tf('s');
f=1200;
Hc=(160000*s^4 + 2.161e08*s^3 + 1.596e11*s^2 + 6.49e13*s + 1.016e16)/(1.905e07 *s^4 + 3.048e10 *s^3 + 1.524e13* s^2 + 2.438e15 *s);
Hcd=c2d(Hc,1/f);
Hcd.Variable='z^-1';
Hcd

%%
U_in=48;
U_out=20;
I_outmax=25;
f=1200;
miu=0.4166;
C=0.00227;
L=0.00111;
R=U_out/I_outmax;

A=[0 -1/L; 1/C -1/R/C];
B=[U_in/L; 0];
C_matrix=[0 1];
D=0;
sys=ss(A,B,C_matrix,D);
[num,den]=ss2tf(A,B,C_matrix,D);
Hp=tf(num,den);

%%
Hc
Hp