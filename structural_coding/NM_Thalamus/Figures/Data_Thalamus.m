function Data_Thalamus()
% This function creates the data depicted in Figure 3 of
%
% A thalamocortical neural mass model of the EEG during NREM sleep and its
% response to auditory stimulation.
% M Schellenberger Costa, A Weigenand, H-VV Ngo, L Marshall, J Born, T Martinetz,
% JC Claussen.
% PLoS Computational Biology (in review).

% Move to source folder(assuming it contains the figures folder
cd ..;

% Check if the executable exists and compile if needed
if(exist('Thalamus_mex.mesa64', 'file')==0)
    mex CXXFLAGS="\$CXXFLAGS -std=c++11 -O3" Thalamus_mex.cpp Thalamic_Column.cpp;
end

% Add the path to the simulation routine
addpath(pwd);

% Go back into figures folder
cd Figures;

for i=1:6
    switch (i)
        case 1
            Param_Thalamus = [0.018;      % g_LK
                              0.062];     % g_h
            Name           = 'I';
            
        case 2
            Param_Thalamus = [0.032;      % g_LK
                              0.062];     % g_h
            Name           = 'II';
            
        case 3
            Param_Thalamus = [0.025;      % g_LK
                              0.025];     % g_h
            Name           = 'III';
            
        case 4
            Param_Thalamus = [0.04;       % g_LK
                              0.066];     % g_h
            Name           = 'IV';
            
        case 5
            Param_Thalamus = [0.052;      % g_LK
                              0.066];     % g_h
            Name           = 'V';
            
        case 6
            Param_Thalamus = [0.052;      % g_LK
                              0.04];      % g_h
            Name           = 'VI';
    end
    
    T            = 15; % Duration of the simulation
    [Vt, Vr, ah] = Thalamus_mex(T, Param_Thalamus); %#ok<ASGLU>
    
    % Save the data
    save(['Data/Timeseries_Thalamus_',Name], 'Vt', 'Vr', 'ah', 'T');
end
end