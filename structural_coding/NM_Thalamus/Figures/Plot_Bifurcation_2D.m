function Plot_Bifurcation_2D()
% This function creates Figure 2 of
%
% A thalamocortical neural mass model of the EEG during NREM sleep and its
% response to auditory stimulation.
% M Schellenberger Costa, A Weigenand, H-VV Ngo, L Marshall, J Born, T Martinetz,
% JC Claussen.
% PLoS Computational Biology (in review).
%
% Please note that the coloring of the two spindle regions was done
% manually

% Load the data from xppaut
fname_Hopf  = 'Data/Bifurcation_Thalamus_Hopf.dat';
fname_Torus = 'Data/Bifurcation_Thalamus_Torus.dat';
fname_LP    = 'Data/Bifurcation_Thalamus_LP.dat';
fname_PD    = 'Data/Bifurcation_Thalamus_PD.dat';

% Create the figure
close all;
figure(1)
clf, shg

% Plot the data
plotxppaut(fname_PD,'Green','-');
plotxppaut(fname_Torus,'Blue','-');
plotxppaut(fname_Hopf,'Red','-');
plotxppaut(fname_LP,'Black','-');
xlabel('\bar{g}_{LK} [mS/cm^2]')
ylabel('\bar{g}_{h} [mS/cm^2]')
set(gca, 'XLim',[0, 0.08], ...
         'XTick',0:0.02:0.08, ...
         'YLim', [0, 0.09],...
         'YTick',0:0.02:0.08);
     
% Clean up the plot a bit for the legend
lines = get(gca, 'Children');
id = 1:numel(lines);
% Keep the real lines
id([1 3 5 7]) = [];
delete(lines(id))

% Add legends
legend('Period-doubling', 'Saddle-node', 'Torus', 'Hopf', 'location', 'northoutside', 'orientation','horizontal');

% Set the line style
lines = get(gca, 'Children');
for i=1:numel(lines)
    set(lines(i), 'Marker', 'none');
end

% Plot the lines indicating the spindle regime
hold on
plot([0, 0.02932],[0.06357,0.06357],'--','Color',[0.7,0.7,0.7],'LineWidth',0.5)
plot([0.0367, 0.08],[0.06357,0.06357],'--','Color',[0.7,0.7,0.7],'LineWidth',0.5)
plot([0.02932, 0.03613],[0.06357,0.06357],'-','Color',[0.7,0.7,0.7],'LineWidth',0.5)

plot([0.03613,0.03613], [0, 0.0412],'--','Color',[0.7,0.7,0.7],'LineWidth',0.5)
plot([0.03613,0.03613], [0.064, 0.1],'--','Color',[0.7,0.7,0.7],'LineWidth',0.5)
plot([0.03613,0.03613], [0.04146, 0.06357],'-','Color',[0.7,0.7,0.7],'LineWidth',0.5)

% Delta regime
plot([0.044,0.044], ylim,'--','Color',[0.7,0.7,0.7],'LineWidth',0.5)

% Indicate the parameter settings for the example time series
g_LK = [0.018, 0.0316, 0.052,  0.052, 0.025, 0.04];
g_h  = [0.062,  0.062,  0.066, 0.04, 0.025, 0.066];
plot(g_LK, g_h,'*', 'Color', 'black');
text(g_LK(1), g_h(1), 'S_{I}', 'horizontal','right', 'vertical','bottom')
text(g_LK(2), g_h(2), 'S_{II}', 'horizontal','center', 'vertical','bottom')
text(g_LK(3), g_h(3), 'D_{I}', 'horizontal','right', 'vertical','bottom')
text(g_LK(4), g_h(4), 'D_{II}', 'horizontal','right', 'vertical','bottom')
text(g_LK(5), g_h(5), 'C_{I}', 'horizontal','right', 'vertical','bottom')
text(g_LK(6), g_h(6), 'C_{II}', 'horizontal','center', 'vertical','bottom')
hold off

end