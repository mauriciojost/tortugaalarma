% Genera el cociente entre 1uP y 2uP (1).
clear all;
Ki = 1024;
baseMB = [16 32 64 128 256 512 Ki 2*Ki 4*Ki 8*Ki 16*Ki 32*Ki 64*Ki]/Ki;
unuP = [0.01 0.02 0.05 0.13 0.31 0.72 1.54 3.41 7.30 16.62 34.90 70.57 154.36];
dosuP = [0.01 0.02 0.04 0.12 0.21 0.45 0.92 2.00 4.33 9.12 19.46 38.68 6000];
plot(baseMB, dosuP./unuP,'rd-');
title('Mediciones');
xlabel('Cantidad de muestras [Mi]');
ylabel('Relación T_2uP/T_1uP');
xmin=0;xmax=32;ymin=0.5;ymax=1;
axis([xmin xmax ymin ymax]);
grid;
print -djpeg100 E:\FFT\docs\relacion1.jpg












