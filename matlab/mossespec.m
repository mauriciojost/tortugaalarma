
%Mostrar solo espectro.
% Muestra la transformada de Fourier de la señal dada. Y no la señal.
%   mossespec(Señal,Fmuestreo)
% Hecho por Mauri.

function U = mosespec(M,Fs)
close all
w = linspace(-Fs/2,Fs/2,length(M));
plot(w,abs(fftshift(fft(M))));
title('Señal (en frecuencia)')
xlabel('Frecuencia [Hz]');
ylabel('Y(f)');
grid;