
%Mostrar solo espectro.
% Muestra la transformada de Fourier de la se�al dada. Y no la se�al.
%   mossespec(Se�al,Fmuestreo)
% Hecho por Mauri.

function U = mosespec(M,Fs)
close all
w = linspace(-Fs/2,Fs/2,length(M));
plot(w,abs(fftshift(fft(M))));
title('Se�al (en frecuencia)')
xlabel('Frecuencia [Hz]');
ylabel('Y(f)');
grid;