function Plot_Timeseries_Thalamus()
% This function creates Figure 2 of
%
% A thalamocortical neural mass model of the EEG during NREM sleep and its
% response to auditory stimulation.
% M Schellenberger Costa, A Weigenand, H-VV Ngo, L Marshall, J Born, T Martinetz,
% JC Claussen.
% PLoS Computational Biology (in review).

% Path to panel tool
if(isempty(strfind(path, [pwd, '/Tools'])))
    addpath([pwd, '/Tools']);
end

% Names of the individual files
Names    = {'I', 'II', 'III', 'IV', 'V', 'VI'};
Labels   = {'S_{I}', 'S_{II}', 'C_{I}', 'C_{II}', 'D_{I}', 'D_{II}'};

T        = 15;
timeaxis = linspace(0,T,T*100);

% Create figure
figure(1);
clf

% Create panel
p = panel('no-manage-font');
p.pack(6,2);

% set margins
p.de.margintop = 1;
p.de.marginbottom = 6;
p.de.marginleft= 5;
p.de.marginright= 5;

for i=1:numel(Names)
    load(['Data/Timeseries_Thalamus_',Names{i}]);
    % Thalamic relay membrane voltage
    p(i,1).select();
    plot(timeaxis, Vt, 'Color', 'black');
    set(gca, 'XTickLabel', [], 'YTick', [-80, -50, -20], 'YLim', [-80, -20]);
    if(i==1)
        title('Thalamic relay population');
    end
    if(i==6)
    xlabel('Time [s]'); 
    set(gca, 'XTick', 0:5:15); 
    set(gca, 'XTick', 0:5:15, 'XTickLabel', 0:5:15);
    end
    
    % Reticular membrane voltage
    ylabel('V_{t} [mV]');
    p(i,2).select();
    plot(timeaxis, Vr, 'Color', 'black');
    set(gca, 'XTickLabel', [], 'YAxisLocation', 'right', 'YTick', [-80, -50, -20], 'YLim', [-80, -20]);
    if(i==1)
        title('Thalamic reticular population');
    end
    if(i>=5)
    set(gca, 'YTick', [-100, 0, 100], 'YLim', [-100, 100]);
    end        
    if(i==6)
    xlabel('Time [s]'); 
    set(gca, 'XTick', 0:5:15, 'XTickLabel', 0:5:15);
    end     
    ylabel('V_{r} [mV]');   
    % Add the title
    p(i).title(Labels(i));
end
end