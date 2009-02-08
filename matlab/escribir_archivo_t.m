## function escribir_archivo_t(fname, vector, fs)  
function escribir_archivo_t(fname, vector, fs)  

  fprintf('Escribiendo archivo con senal...\n');
  eval(sprintf("fd = fopen('%s','w');",fname));

  fprintf(fd,'%f\n',fs); % Escritura de la frec. de muestreo (fs).
  len = length(vector);
  fprintf(fd,'%d\n',len); % Escritura de la cantidad de elementos.
  for i=(1:len)
    fprintf(fd,'%f+%fj\n',real(vector(i)),imag(vector(i)));
  end
  fclose(fd);

  fprintf('Archivo escrito.\n');
endfunction
