#!/bin/bash
g++ -std=c++11 -g simulationmain.cpp cell.cpp ue.cpp -o simu
./simu
g++ -std=c++11 -g simulationmain_wait.cpp cell.cpp ue.cpp -o wai
./wai
