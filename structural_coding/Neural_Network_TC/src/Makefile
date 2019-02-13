CXX=g++
CXXFLAGS  = -std=c++11  -O2 -fopenmp -lm

OBJS= Bazhenov.o Inhibitory_Neuron.o  Pyramidal_Neuron.o Reticular_Neuron.o \
Thalamocortical_Neuron.o 

DEP= Inhibitory_Neuron.h Pyramidal_Neuron.h Reticular_Neuron.h \
Thalamocortical_Neuron.h

prog: $(OBJS)
	$(CXX) $(CXXFLAGS) -o prog $(OBJS)

%.o :%.cpp  $(DEP)
	$(CXX) -c  $< $(CXXFLAGS) 

clean:
	rm -rf  *.pyc ./../data/*.txt ./../data/*.png ./*.txt
	rm -f *.png *.out *.pyc ../data/*.npy ../data/*.bin ../data/*.npz
	rm -f ../data/*.pdf
eradicate:
	rm -rf *.o  *.pyc ./../data/*.txt  ./../data/*.png ./*.txt
	rm -f *.png prog* *.out *.pyc ../data/*.npy ../data/*.bin
	rm -f ../data/*.npz ../data/*.pdf


.PHONY: clean, eradicate



# Bazhenov.o : Bazhenov.cpp $(DEP)
# 	$(CXX) $(CXXFLAGS) -c Bazhenov.cpp

# Inhibitory_Neuron.o : Inhibitory_Neuron.cpp $(DEP)
# 	$(CXX) $(CXXFLAGS) -c $< 

# Pyramidal_Neuron.o : Pyramidal_Neuron.cpp $(DEP)
# 	$(CXX) $(CXXFLAGS) -c $< 

# Reticular_Neuron.o :Reticular_Neuron.cpp $(DEP)
# 	$(CXX) $(CXXFLAGS) -c $< 

# Thalamocortical_Neuron.o : Thalamocortical_Neuron.cpp $(DEP)
# 	$(CXX) $(CXXFLAGS) -c $< 
