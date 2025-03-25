# Centrality Framework

## General information
The procedure of event centrality determination is described in Sec. 4.5 of this <a href="https://publikationen.ub.uni-frankfurt.de/opus4/frontdoor/deliver/index/docId/51779/file/main.pdf" target="_blank">thesis</a> and in this <a href="https://indico.gsi.de/event/8804/contributions/38283/attachments/27572/34451/Master_Report_ENG.pdf" target="_blank">note</a>.
It consists of two steps - division of events into centrality classes (slicing) and application of centrality hypothesis to a set of events (filling).
The slicing step can be run directly or include Glauber fit.
Running of these steps is described below, in the section "Examples".

## Installation
ROOT6 is needed for installation.
Follow CERN ROOT <a href="https://root.cern/install/" target="_blank">instructions</a> to install it.
Version compiled with c++17 flag is preferred, otherwise CMAKE_CXX_STANDARD flag needs to be explicitly specified (see below).

The Centrality framework is agnostic to the I/O format.
In the current implementation the <a href="https://github.com/HeavyIonAnalysis/AnalysisTree" target="_blank">AnalysisTree</a> interface is provided.
However one can use another data format (a proper interface should be developed then).

In order to install the Centrality framework together with AnalysisTree interface, one needs either (1) to have pre-installed AnalysisTree or (2) to install the AnalysisTree automatically together with the Centrality.
Option (2) is strongly recommended.\
(1) Type before performing the *cmake* command:

    export AnalysisTree_DIR=path-to-analysistree-installation/lib/cmake/AnalysisTree/
OR\
(2) Use following cmake flags when running the *cmake* command:

    -DCentrality_BUNDLED_AT=ON
    -DCentrality_BUNDLED_AT_VERSION=v2.3.2

Now perform following commands for the installation:

    git clone https://github.com/HeavyIonAnalysis/Centrality.git
    cd Centrality
    mkdir build install
    cd build
    source path-to-root-installation/bin/thisroot.sh
    cmake -DCMAKE_INSTALL_PREFIX=../install ../
    make -j install

*path-to-root-installation* and *path-to-analysistree-installation* must be replaced with your actual location of Root and AnalysisTree install directories respectively.

## Examples
Before running the Centrality framework you need to set up the environment variables:

    source path-to-centrality-installation/bin/CentralityConfig.sh

### Slicing
Let us consider the fitting procedure with an example of 2M Au+Au collisions generated with UrQMD at beam momentum 12*A* GeV/*c*.
For this you need to prepare an input file with histograms of the quantities used for centrality estimation (e.g. multiplicity of reconstructed charged tracks or energy deposit in the PSD), like input/multiplicity_qa.urqmd.12agev.root.
It contains histograms "hMult", "hE", "hB" with distributions of multiplicity of reconstructed charged tracks *M*(\*), energy deposit in the PSD *E*<sub>PSD</sub> and impact parameter *b* respectively (the last one is just for information).
Also it contains 2D histograms with pair-wise correlations of these quantities.
The task which runs 1D slicing the events into centrality classes is tasks/main.cpp (look for comments in this file for understanding the configuration example), and it produces a C++ executable install/bin/main.
To run this executable type:

    ./main ../../input/multiplicity_qa.urqmd.12agev.root hMult false false

where the first argument is the name of input file, the second argument - the name of input histogram, the third should be false if the centrality estimator quantity is anti-correlated with the impact parameter (like multiplicity) and vise versa (like *E*<sub>PSD</sub>), the fourth argument should be false if one runs a 1D slicing (**).

This run will produce several files, let us digest them.
The file centrality_getter.root contains a Centrality::Getter pointer, which is a result of centrality slicing.
In order to know, to which centrality class belongs an event with a centrality estimation quantity equal to a certain value (let's say *M* equals to 120 tracks), use the function GetCentrality():

    centr_getter_1d->GetCentrality(120)

what gives a result

    (double) 31.727854
i.e. the centrality of this event is 31.7%.

Also there is a TH1D histogram, where the events distribution among centrality classes is shown.
Use this histogram to check the correctness of slicing - if the centrality ranges have equal widths, than the number of events in centrality classes must agree within statistical errors.
Finally, there is a TCanvas with distribution of centrality-estimation quantity, where vertical lines show the slices into centrality classes.\
TH1D and TCanvas are also written into pdf-files produced during the executable run.

(\*) When creating a histogram with discrete quantity, such as *M*, choose the bin edges in such a way that integer values are in the center of bin, but not at its edge (see "hMult" histogram in input/multiplicity_qa.urqmd.12agev.root).\
(\*\*) Also there is a possibility to run a 2D slicing (i.e. to determine centrality classes simultaneously with two estimators - *M* and *E*<sub>PSD</sub>), however this procedure is not yet well-established and therefore deprecated.

### Filling

Once slicing is preformed and the centrality_getter.root file is produced, filling the root-file containing reconstructed events can be done - each event will be assigned with estimated centrality percentage.
This is done in the at_interface/CentralityFiller.*pp, which are managed by the task tasks/fill_centrality.cpp (look for comments in this file for understanding the configuration example).
It produces an executable fill_centrality which can be run:

    ./fill_centrality filelist.list centrality_getter.root

where filelist.list is a text file containing names of the AnalysisTree root-files to be worked on (en example of AnalysisTree file can be downloaded [here](https://sf.gsi.de/f/2c5e1a984fc44e0691fc/?dl=1)).
After running this exacutable a centrality.analysistree.root file is produced.
It is based on the input file with following changes:
branches RichRings and TrdTracks are missing, and branch RecEventHeader is replaced with AnaEventHeader branch.
The AnaEventHeader differs from RecEventHeader in one additional floating field - centrality_tracks, which contains centrality percentage of each event.

### MC-Glauber fit

Glauber model based fitting procedure is similar to one used by ALICE collaboration, <a href="http://inspirehep.net/record/1215085" target="_blank">Phys.Rev.C 88 (2013) 4, 044909</a>.

Before running fitting procedure one needs to create an MC-Glauber input file using this <a href="https://tglaubermc.hepforge.org/" target="_blank">software</a>.
(There is also an example file in the input directory - input/glauber_auau_sigma_30_100k.root with 100k Au+Au events with nucleon-nucleon interaction cross section equal to 30 mb).
Do not forget to adjust an MC Glauber file to the conditions of your analysis - properly set a collision system and a cross section of a nucleon-nucleon interaction.

Fitting of the multiplicity distribution is done with a histogram which is built in the following way.
From the input MC-Glauber file the event is taken, and the number of participants (*N*<sub>part</sub>) together with the number of nucleon-nucleon collisions (*N*<sub>coll</sub>) is read out. Then the number of ancestors is defined
$N_\text{A}(f) = fN_{\text{part}} + (1-f)N_{\text{coll}}$, where $f\in[0; 1]$ is a coefficient parametrizing a combination of soft and hard interactions.
(In soft interactions the average multiplicity of produced particles is proportional to *N*<sub>part</sub>, and in hard interactions - to *N*<sub>coll</sub>).
Then *N*<sub>A</sub> is rounded to the nearest integer value, and the negative binomial <a href="https://en.wikipedia.org/w/index.php?title=Negative_binomial_distribution&oldid=1222899545" target="_blank">distribution</a> $NBD(\mu, k)$ is called *N*<sub>A</sub> times to evaluate the multiplicity of particles in the current event, where $\mu = rq/p$ (average of the distribution) and $k = r$, with $r, q, p, k$ as defined <a href="https://en.wikipedia.org/w/index.php?title=Negative_binomial_distribution&oldid=1222899545" target="_blank">here</a>.
This procedure is repeated for each event from the MC Glauber file.

The configuration of MC-Glauber fitting is done in the task/glauber.cpp file, which produces an exacutable glauber after compilation.
Look into the comments in this file.
To run the glauber fitting executable type

    ./glauber f0 k0 k1,

where f0 is the value of $f$ for which the fit will be done, and $k0, k1$ denote the edges of an integer sequence of $k$ for which the fit will be done.
E.g. ./glauber 0.35 4 6 will run fits for $f=0.35$ and $k=4, 5, 6$.
However, one can run this executable with *two* arguments instead of *three*, specifying a single $k$ instead of a sequence: ./glauber 0.35 4.
This way is strongly recommended and is considered in a future explanations.

During MC Glauber fit with *glauber* executable an optimal $\mu$ value is found while $f$ and $k$ are input parameters defined by user in the runtime.
The output of the *glauber* executable contains 4 files.
Let us digest them:<br>
1) fit_0.35_4_4_50.root, where $f=0.35$, $k0=4$, $k1=4$ are set in the command line and described above, and $M=50$ is a lower edge of multiplicity range to be fitted (set in the executable source code).
The file contains a ROOT-tree with values of $f,\ k,\ \mu,\ \chi^2$ and $\sigma^2$, where $\mu$ is an optimal (with the least $\chi^2$) for pre-defined $f$ and $k$;
the $\sigma^2$ is the variance of the NBD for given $\mu$ and $k$.
This file also contains a fitting histogram (generated with given $f$ and $k$ and an optimal $\mu$).<br>
2) glauber_qa.root, where following histograms are stored:<br>
- fNcollHisto with $N_{\text{coll}}$ distribution from the input MC Glauber file;<br>
- fNpartHisto with $N_{\text{part}}$ distribution from the input MC Glauber file;<br>
- hMult with multiplicity distribution to be fitted taken from the input file;<br>
- glaub_0.35_0.5340_4.00_100000 with fitting histogram (the same as in the fit_0.35_4_4_50.root file);<br>
- nbd_0.534004_4.000000 with NBD for optimized $\mu$ and given $k$.<br>
There is also a TCanvas with following histograms (starting from the left upper corner and clock-wise):<br>
- fNcollHisto and fNpartHisto;<br>
- hMult and glaub_0.35_0.5340_4.00_100000;<br>
- glaub_0.35_0.5340_4.00_100000;<br>
- nbd_0.534004_4.000000.<br>
3) glauber.pdf with the TCanvas from the previous file printed in PDF file.<br>
4) mult_b.root with a 2D histogram, showing a correlation between impact parameter taken from the input MC Glauber file and multiplicity evaluated with given $f$ and $k$ and an optimal $\mu$.

A single run of the *glauber* executable allows for determination of an optimal $\mu$ for given $f$ and $k$.
In order to determine optimal $f$ and $k$ as well, one needs to run the *glauber* executable for various values of them.
The most convenient way to do it is the usage of the cluster for parallel calculations.
At GSI one can use <a href="https://hpc.gsi.de/virgo/" target="_blank">Virgo</a> cluster which is based on <a href="https://slurm.schedmd.com/documentation.html" target="_blank">Slurm</a> technology.
An example of batch scripts together with comments is here: <a href="https://github.com/HeavyIonAnalysis/Centrality/blob/master/macro/batch/batch_run.sh" target="_blank">batch_run.sh</a> and <a href="https://github.com/HeavyIonAnalysis/Centrality/blob/master/macro/batch/run.sh" target="_blank">run.sh</a>.
Set executive rights for both of them (chmod +x filename) and run:\

    ./run.sh

After running the jobs merge output fit_<...>.root files into a single one with *hadd* ROOT command.


## Doxygen documentation
    doxygen Doxyfile
File Doxygen/html/index.html with documentation will be created
