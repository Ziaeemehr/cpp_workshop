# NM_Thalamus
This repository contains the reference implementation of the model proposed in Schellenberger Costa and Weigenand et al. 2016, 
available here http://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1005022

For convenience we utilize MATLAB for data processing and plotting. Therefore the simulation comes with an additional 
source-file Thalamus_mex.cpp that can be compiled within MATLAB to utilize their C++-mex interface. 

The easiest way to reproduce the figures in the paper is to simply run the Data_Thalamus() function in the "Figures" folder within 
MATLAB, assuming the mex interface is set up. Afterwards simply run the respective plot functions for the different figures. 

Additionaly the figures folder provides the Thalamus.ode file, that defines the model for the bifurcation analysis in xppaut.

Please note that due to the stochastic nature of the simulation the time series will differ.
