% function [expon w rfft] = creexp3(Fs,Fc,qm)
% Fs Frecuencia de muestreo.
% Fc Frecuencia de la exponencial compleja.
% qm cantidad de muestras.
%Hecho por Mauri.
function expon = creexp3(Fs,Fc,qm)
    %t=linspace(0,qm/Fs,qm);
    %expon = exp(j*2*pi*Fc*t);
    %expon = cos(2*pi*Fc*t) + j * sin(2*pi*Fc*t);

    expon = zeros(1,qm);

    for i=(1:qm)
      expon(i) = cos(pi*2*(Fc/Fs)*(i-1)) + j*sin(pi*2*(Fc/Fs)*(i-1));
    endfor

		M = expon;
		w = linspace(-Fs/2,Fs/2,length(M));
		rfft = fft(M);
		plot(w,abs(fftshift(rfft)));
		title('Señal (en frecuencia)')
		xlabel('Frecuencia [Hz]');
		ylabel('Y(f)');
		grid;
