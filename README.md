# Centrality Framework

## Installation

    cd centrality
    mkdir build
    cd build
    source /path-to-root/thisroot.sh
    cmake ../
    make


## Examples and short description 

### General

**main** has 3 arguments:
  1. input file name
  2. input histo name
  3. is input histo is 2D (false or true)

Example how to run it with test input file:

    ./main ../input/test_input.root hMreco false

or in case of 2D histogram

    ./main ../input/test_input.root hMEcorr true

To modify options (centrality ranges, detector type, etc) change corresponding lines in file main.cpp:

    bf.SetRanges( 10,0,100 );   // number of bins, min, max value
or

    bf.SetRanges( {0,10,30,60,100} );  // centrality bins borders with array

    bf.IsSpectator(true);  // true if impact parameter b correlated with estimator (spectators eneggy), false - anticorrelated (multiplicity of produced particles) 
    std::string outfilename = "test.root";

Minimal example on how to obtain centrality for a given event:

    cd macro
    root -l TestGetter.C

### MC-Glauber fit

Glauber model based fitting procedure is similar to one used by ALICE collaboration:

http://inspirehep.net/record/1215085

Before running fitting procedure one needs to create MC-Glauber input file using software 
(there is also a test file in input directory)

https://tglaubermc.hepforge.org/downloads/

Fitting function is
```math
   F(f, \mu, k) = N_A(f)  F_{NBD}(\mu, k)
```
where $`F_{NDB}`$ is Negative Binomial Distribution, $`N_A`$ is defined for example as: 
```math
   N_A(f) = f N_{part} + (1-f) N_{coll}
```
$`N_{part}`$ and $`N_{coll}`$ are parameters from MC-Glauber.

To modify options (input files, fit function, fit ranges, etc) change corresponding lines in file glauber/main.cpp:

**glauber** has 2 arguments:
  1. f value
  2. k value

$`\mu`$ value is fitted automatically.

### Doxygen documentation
    doxygen Doxyfile
File Doxygen/html/index.html with documentation will be created
