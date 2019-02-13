% mex command is given by: 
% mex CXXFLAGS="\$CXXFLAGS -std=c++11 -fopenmp -O3" Compte.cpp Pyramidal_Neuron.cpp Inhibitory_Neuron.cpp

function Plots()
mex CXXFLAGS="\$CXXFLAGS -std=c++11 -fopenmp -O3" LDFLAGS="\$LDFLAGS -fopenmp" Compte.cpp Pyramidal_Neuron.cpp Inhibitory_Neuron.cpp

tic
[Ve, Vi, Ca] = Compte();
toc

time =linspace(0,10,length(Ve));
subplot(311)
plot(time,Ve);
subplot(312)
plot(time,Vi);
subplot(313)
plot(time,Ca);

save('Compte.mat');