#!/bin/bash
g++ -std=c++11 -g simulationmain.cpp cell.cpp ue.cpp -o simu
./simu
g++ -std=c++11 -g simulationmain_wait2.cpp cell.cpp ue.cpp -o wai
./wai
