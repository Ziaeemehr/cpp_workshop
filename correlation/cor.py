from scipy.stats.stats import pearsonr

p1 = [525., 300., 450., 300., 400., 500., 550., 125., 300., 400., 500., 550.]
p2 = [250., 225., 275., 350., 325., 375., 450., 400., 500., 550., 600., 525.]
print pearsonr(p1,p2)
#(0.1746680664120466, 0.58716519218300223)
# 0.174668
