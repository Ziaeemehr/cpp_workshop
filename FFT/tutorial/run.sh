# g++ -o FFTW_2d -I<location of include> -L<location of library> -lfftw3 01.cpp
# g++ -o prog -I  /usr/local/include/ -L /usr/lib/x86_64-linux-gnu -lfftw3 01.cpp

g++ -O3 01.cpp -o myFileFFTW -lm -lfftw3
