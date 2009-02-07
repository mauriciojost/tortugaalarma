% Genera la figura 2.
clear all;
Ki = 1024;
baseMB = [16 32 64 128 256 512 Ki 2*Ki 4*Ki 8*Ki 16*Ki 32*Ki 64*Ki]/Ki;
unuP = [0.01 0.02 0.05 0.13 0.31 0.72 1.54 3.41 7.30 16.62 34.90 70.57 154.36];
dosuP = [0.01 0.02 0.04 0.12 0.21 0.45 0.92 2.00 4.33 9.12 19.46 38.68 6000];
plot(baseMB, unuP,'rd--');
hold on
plot(baseMB, dosuP,'g+-');
title('Mediciones');
xlabel('Cantidad de muestras [Mi]');
ylabel('Tiempo de ejecución [Seg*]');
xmin=0;xmax=1.2;ymin=0;ymax=2.2;
axis([xmin xmax ymin ymax]);
grid;
legend('1uP','2uP');
print -djpeg100 E:\FFT\docs\fftfig2.jpg






