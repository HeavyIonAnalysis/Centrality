#include "Getter.h"
#include "BordersFinder.h"
#include "BordersFinder2D.h"

#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <iomanip>
#include <chrono>

#include <TSystem.h>
#include <TROOT.h>
#include "TTree.h"
#include "TFile.h"
#include "TH2.h"

#include <boost/program_options.hpp>
#include <TBenchmark.h>

int main(int argc, char **argv) {
  using namespace std;

  namespace po = boost::program_options;


  TBenchmark benchmark;
  benchmark.Start("centrality");

  vector<float> ranges{0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80, 90, 100};

  po::options_description opts("Centrality Analysis Options");
  opts.add_options()
      // TODO help message
      ("help", "help message")
      ("input,i", po::value<string>()->required())
      ("output,o", po::value<string>()->default_value("centrality.root"))
      ("histogram,h", po::value<string>()->required())
      ("getter,g", po::value<string>()->default_value("centr_getter"))
      ("mode,m", po::value<string>()->default_value("1D"))
      ("axis", po::value<string>()->default_value(""))
      ("spectator,s", po::value<bool>()->default_value(false))
      ("limits", po::value<vector<float> >()->multitoken());

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, opts), vm);
  po::notify(vm);

  string input_file_name{vm["input"].as<string>()};
  string output_file_name{vm["output"].as<string>()};
  string obj_name{vm["histogram"].as<string>()};
  string mode{vm["mode"].as<string>()};
  string axis{vm["axis"].as<string>()};
  bool isSpectator{vm["spectator"].as<bool>()};

  vector<float> limits;
  if (!vm["limits"].empty()) {
    limits = vm["limits"].as<vector<float> >();
  }

  string getter_name{vm["getter"].as<string>() + "_" + mode};

  Info(__func__, "Processing %s", input_file_name.c_str());
  unique_ptr<TFile> fInPtr{TFile::Open(input_file_name.c_str(), "read")};

  if (!fInPtr) {
    return 1;
  }

  if (mode == "1D") {
    Info(__func__, "1D");

    auto h2 = dynamic_cast<TH2 *>(fInPtr->Get(obj_name.c_str()));
    auto h1 = dynamic_cast<TH1 *>(fInPtr->Get(obj_name.c_str()));

    TH1 *h = nullptr;

    if (h2) {
      if (axis == "x" || axis == "X") {
        h = h2->ProjectionX();
      } else if (axis == "y" || axis == "Y") {
        h = h2->ProjectionY();
      } else {
        Error(__func__, "Axis is not set");
        return 1;
      }

      Info(__func__, "Axis '%s'", axis.c_str());
    } else if (h1) {
      h = h1;
    } else {
      Error(__func__, "Object '%s' does not exist or not TH1 derived", obj_name.c_str());
      return 1;
    }

    Centrality::BordersFinder bf;
    bf.SetHisto(h);
    bf.SetRanges(40, 0, 100);   // number of bins, min, max value
//    bf.SetRanges(ranges);
    // centrality bins borders with array

    if (limits.size() == 2) {
      Info(__func__, "Setting limits (%f, %f)", limits[0], limits[1]);
      bf.SetLimits(limits[0], limits[1]);
    }

    bf.IsSpectator(isSpectator);
    // true if impact parameter b correlated with estimator (spectators energy),
    // false - anticorrelated (multiplicity of produced particles)

    bf.FindBorders();
    bf.SaveBorders(output_file_name, getter_name);

  } else if (mode == "2D") {
    Info(__func__, "2D");

    auto h2 = dynamic_cast<TH2 *>(fInPtr->Get(obj_name.c_str()));
    if (!h2) {
      Error(__func__, "Object '%s' does not exist or not TH2 derived", obj_name.c_str());
      return 1;
    }
    Info(__func__, "Object '%s'", h2->GetName());

    Centrality::BordersFinder2D bf;
    bf.SetHisto2D(h2);

    if (limits.size() == 4) {
      bf.CropHist(limits[0], limits[1], limits[2], limits[3]);
    }

    auto h1d = bf.Convert();
    bf.SetHisto(h1d);

//    bf.SetRanges(ranges);   // number of bins, min, max value
    bf.SetRanges(40, 0, 100);   // number of bins, min, max value
    bf.IsSpectator(isSpectator);
    // true if impact parameter b correlated with estimator (spectators eneggy),
    // false - anticorrelated (multiplicity of produced particles)

    bf.FindBorders();
    bf.SaveBorders2D(output_file_name, getter_name);

  } else {
    Error(__func__, "Mode '%s' does not exists", mode.c_str());
    return 1;
  }

  benchmark.Stop("centrality");
  benchmark.Print("centrality");

  return 0;

}
