## function leer_archivo_t(fname)  
function [w vector] = leer_archivo_t(fname)  

1000.000000
2048
1.000000+0.000000j


  fprintf('Leyendo archivo de senal...\n');
  eval(sprintf("fd = fopen('%s','r');",fname));

  len = 0; fs = 0;
  cadena = fgets(fd);
  fs = sscanf(cadena, "%f", 1);
  cadena = fgets(fd);
  len = sscanf(cadena, "%d", 1);
  
  fprintf('Encontrados len=%u y fs=%f.\n', len, fs);

  vector = zeros(1,2);
  

  real=0; imaginario=0;

  for i=(1:len)
    cadena = fgets(fd);
    A = sscanf(cadena,'%f+%fj\n',[2 inf]);
    
    real = A(1);
    imaginario = A(2);

    vector(i) = real + j* imaginario;
  endfor
  hold off;
  t = linspace(0, len/fs, len);
  plot(t, vector);
  fclose(fd);

  fprintf('Archivo leido.\n');
endfunction
