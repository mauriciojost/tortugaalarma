% function cosen = crecos3(Fs,Fc,qm)
% Fs Frecuencia de muestreo.
% Fc Frecuencia del coseno.
% qm cantidad de muestras.
%Hecho por Mauri.
function cosen = crecos3(Fs,Fc,qm)
    t=linspace(0,qm/Fs,qm);
    cosen = cos(2*pi*Fc*t);
		M = cosen;
		w = linspace(-Fs/2,Fs/2,length(M));
		rfft = fft(M);
		plot(w,abs(fftshift(rfft)));
		title('Señal (en frecuencia)')
		xlabel('Frecuencia [Hz]');
		ylabel('Y(f)');
		grid;
