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

**main** has 2 arguments:
  1. input file name
  2. input histo name

Example how to run it with test input file:

    ./main ../input/test_input.root hMreco

To modify options (centrality ranges, detector type, etc) change corresponding lines in file main.cpp:

    bf.SetRanges( 10,0,100 );   // number of bins, min, max value
or

    bf.SetRanges( {0,10,30,60,100} );  // centrality bins borders with array

    bf.IsSpectator(true);  // true if impact parameter b correlated with estimator (spectators eneggy), false - anticorrelated (multiplicity of produced particles) 
    std::string outfilename = "test.root";

Minimal example on how to obtain centrality for a given event:

    cd macro
    root -l TestGetter.C

### Glauber

HowTo for **glauber** is coming soon

    ./glauber 0.9 10 10 200
