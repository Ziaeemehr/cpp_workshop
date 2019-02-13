function plotxppaut(file_name,C_Us,Lt_Us, N_plot)
%PlotXppaut
%Plot bifurcation diagrams in Matlab that have been saved by XPPAUT
%
%How to use:
%In AUTO (XPPAUT) click-->File--->Write Pts
%and save your bifurcation diagram in a .dat file
%In Matlab command window type: plotxppaut 
%Select .dat file saved by AUTO
%A bifurcation diagram will be plotted
%Cutomize the plot using Matlab figure editor
%Or edit program parameters to suit your needs--very useful for those who
%plot lots of bifurcation diagrams
%
%Version 1.0 Tested Under Matlab 6.1 (R12.1)

%---------------------------------------
%Version 1.0
%Time-stamp: <2005-05-03 17:06:11 msi800> 
%-------------------------------------------
%Mohammad S. Imtiaz
%Room 403
%The Neuroscience Group
%Discipline of Human Physiology
%School of Biomedical Sciences
%Faculty of Health
%The University of Newcastle
%Callaghan, NSW 2308
%Australia
%Tel: +61 02 49217857  (Office)
%Tel: +61 02 49215626  (Lab)
%Fax: +61 02 49217406
%Email: Mohammad.Imtiaz@newcastle.edu.au
%-------------------------------------------
%-------------------------------------------


%-------------------------------------------------------------------
%       USER DEFINED PARAMETERS -- CHANGE TO CUSTOMIZE OUTPUT PLOT
%-------------------------------------------------------------------
%Colors -- SHOULD BE VALID MATLAB COLORS
C_Ss = 'Black';   %STABLE STEADY STATE
% C_Us = 'Black';   %UNSTABLE STEADY STATE
C_Sp = 'Green';     %STABLE PERIODIC ORBIT
C_Up = 'Blue';    %UNSTABLE PERIODIC ORBIT

%LineStyle -- SHOULD BE VALID MATLAB LINESTYLES
Lt_Ss = '-';    %STABLE STEADY STATE
% Lt_Us = '--';   %UNSTABLE STEADY STATE
Lt_Sp = 'o';    %STABLE PERIODIC ORBIT
Lt_Up = 'o';    %UNSTABLE PERIODIC ORBIT

%Linewidths
Lw_Ss = 0.5;   %STABLE STEADY STATE
Lw_Us = 0.5;     %UNSTABLE STEADY STATE
Lw_Sp = 0.5;     %STABLE PERIODIC ORBIT
Lw_Up = 0.5;     %UNSTABLE PERIODIC ORBIT

%Markersize
Ms_Ss = 1;    %STABLE STEADY STATE
Ms_Us = 1;    %UNSTABLE STEADY STATE
Ms_Sp = 1;    %STABLE PERIODIC ORBIT
Ms_Up = 1;    %UNSTABLE PERIODIC ORBIT

%MarkerFaceColor -- SHOULD BE VALID MATLAB COLORS
Mfc_Ss = 'none';   %STABLE STEADY STATE
Mfc_Us = 'none';   %UNSTABLE STEADY STATE
Mfc_Sp = 'Red';    %STABLE PERIODIC ORBIT
Mfc_Up = 'none';   %UNSTABLE PERIODIC ORBIT

%	-----------------------------------------	
% END OF USER DEFINED PARAMETERS
%	-----------------------------------------

% [file_in,path] = uigetfile('*.dat','.dat file saved by AUTO (XPPAUT) ');
% file_name = [path file_in];

if(file_name),
  st = dlmread(file_name, ' ');
  
  if(nargin==3)
    figure(1), hold on
  else
    subplot(1,3,N_plot), hold on
  end
  
  temp=st;
  temp1=zeros(size(temp,1),8)*NaN;
  
  for n=1:size(temp1,1),
      if(temp(n,4)==1),
          temp1(n,[1 5]) = temp(n,[2 3]);
      end;
      if(temp(n,4)==2),
          temp1(n,[2 6]) = temp(n,[2 3]);
      end;
      if(temp(n,4)==3),
          temp1(n,[3 7]) = temp(n,[2 3]);
      end;
      if(temp(n,4)==4),
          temp1(n,[4 8]) = temp(n,[2 3]);
      end;
  end;
  
% $$$ figure;
%	-----------------------------------------
if(any(any(isfinite(temp1(:,[1 5])))))
  h_SS = plot(temp(:,1),temp1(:,[1 5]));
  set(h_SS,'color',C_Ss)
  set(h_SS,'linestyle',Lt_Ss)
  set(h_SS,'linewidth',Lw_Ss)
  set(h_SS,'Markersize',Ms_Ss)
  set(h_SS,'MarkerFacecolor',Mfc_Ss)
  hold on;
end
%	----------------------------------------- 
if(any(any(isfinite(temp1(:,[2 6])))))
  h_US = plot(temp(:,1),temp1(:,[2 6]), '*');
  set(h_US,'color',C_Us)
  set(h_US,'linestyle',Lt_Us)
  set(h_US,'linewidth',Lw_Us)
  set(h_US,'Markersize',Ms_Us)
  set(h_US,'MarkerFacecolor',Mfc_Us)
end
%	-----------------------------------------  
if(any(any(isfinite(temp1(:,[3 7])))))
  h_SP = plot(temp(:,1),temp1(:,[3 7]), '*');
  set(h_SP,'color',C_Sp)
  set(h_SP,'Marker',Lt_Sp)
  set(h_SP,'linewidth',Lw_Sp)
  set(h_SP,'Markersize',Ms_Sp)
  set(h_SP,'MarkerFacecolor',Mfc_Sp)
end
%	-----------------------------------------   
if(any(any(isfinite(temp1(:,[4 8])))))
  h_UP = plot(temp(:,1),temp1(:,[4 8]), '*');
  set(h_UP,'color',C_Up)
  set(h_UP,'Marker',Lt_Up)
  set(h_UP,'linewidth',Lw_Up)
  set(h_UP,'Markersize',Ms_Up)
  set(h_UP,'MarkerFacecolor',Mfc_Up)
end
%	-----------------------------------------    
  xlim([0,8])
  ylim([-10,10])
  %figdefault()  
  grid off;
  axis tight
  title('');
  
end;
