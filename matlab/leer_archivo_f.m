## function leer_archivo_f(fname)  
function [w vector] = leer_archivo_f(fname)  

  fprintf('Leyendo archivo con FFT...\n');
  eval(sprintf("fd = fopen('%s','r');",fname));

  len = 0; fs = 0;
  cadena = fgets(fd);
  len = sscanf(cadena, "%d", 1);
  cadena = fgets(fd);
  fs = sscanf(cadena, "%f", 1);
  
  
  fprintf('Encontrados len=%u y fs=%f.\n', len, fs);

  vector = zeros(1,2);
  w = zeros(1,2);

  freq=0; real=0; imaginario=0;

  for i=(1:len)
    cadena = fgets(fd);
    A = sscanf(cadena,'%f -> %f + %fj\n',[3 inf]);
    
    freq = A(1); 
    real = A(2);
    imaginario = A(3);

    vector(i) = real + j* imaginario;
    w(i) = freq;
    %fprintf('freq=%f.\n', freq);
  endfor

  plot(w, abs(vector));  fclose(fd);

  fprintf('Archivo leido.\n');
endfunction
