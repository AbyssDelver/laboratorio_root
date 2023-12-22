# Particle collision simulator
Dependecies
-----
the framework ROOT version 6.16 is required for this project. If you have good reason to think another version will also work, try changing the "find_package" in the CMakeLists.txt to your preffered version.

Running the simulation
-----
to build in release mode:

```
cmake -S ./ -B build/release -DBUILD_TESTING=True -DCMAKE_BUILD_TYPE=Release
```
and then 

```
cmake --build build/release
```
to run the program:
```
./build/release/lab1
```
The program will generate the data, and store it in the histos.root file, inside histograms, as TH1 objects (one dimensional histograms of the root framewrok). The generation might take time, even tens of minutes depending on the machine.

Analysing the data
-----
To analyse the data first open the root interpreter in the project directory by typing
```
root
```
from the laboratorio_root/ path.

Then inside the interpret run:

```
setStyle()
analysis()
```
The generated histograms should be saved in the histograms folder


