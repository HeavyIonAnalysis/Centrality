#include "AnalysisTree/TaskManager.hpp"
#include "CentralityFiller.hpp"

using namespace AnalysisTree;

void fill_centrality(const std::string& filelist, const std::string& centrality_file, const std::string& output) {

  auto* man = TaskManager::GetInstance();
  man->SetOutputName(output, "aTree");

  // The output file will be based on the input one with additional branches (and possibly with removed other branches)
  man->SetWriteMode(eBranchWriteMode::kCopyTree);

  // Branches TrdTracks and RichRings will be removed from the output file, since they are irrelevant for the following analysis.
  // The branch RecEventHeader, which is an input branch, will be also removed, because a new one, based on it, will be created.
  man->SetBranchesExclude({"TrdTracks", "RecEventHeader", "RichRings"});

  // Initialize the Centrality::Getter prepared at the slicing step: 1-st argument - file, 2-nd argument - name of the getter.
  auto* task = new CentralityFiller(centrality_file, "centr_getter_1d");

  // The name of the EventHeader in the input file, on base of which an output EventHeader will be prepared
  task->SetInputEventHeader("RecEventHeader");

  // The name of an output EventHeader and name of field with centrality value
  task->SetOutput("AnaEventHeader", "centrality_tracks");

  // Uncomment one of two following blocks:

  // ***** get centrality from multiplicity stored in the event header *****
  task->SetInput("RecEventHeader", "M");
  // ***** end of get centrality from multiplicity stored in the event header *****

  //   // ***** get centrality from multiplicity of tracks stored in track detector passing set of cuts *****
  //   Note: the cuts which you use here must be the same which were used when creating an input histogram
  //   with a centrality-estimator quantity distribution for the slicing step
  //
  //   task->SetInput("VtxTracks");
  //
  //   AnalysisTree::SimpleCut vtx_chi2_track_cut = AnalysisTree::RangeCut("VtxTracks.vtx_chi2", 0, 3);
  //   AnalysisTree::SimpleCut nhits_cut          = AnalysisTree::RangeCut("VtxTracks.nhits", 4, 100);
  //   AnalysisTree::SimpleCut chi2_cut({"VtxTracks.chi2", "VtxTracks.ndf"},
  //                                    [](std::vector<double> par) { return par[0] / par[1] < 3; });
  //   AnalysisTree::SimpleCut eta_cut = AnalysisTree::RangeCut("VtxTracks.eta", 0.2, 6);
  //
  //   auto* vertex_tracks_cuts =
  //     new AnalysisTree::Cuts("VtxTracks", {vtx_chi2_track_cut, nhits_cut, chi2_cut, eta_cut});
  //
  //   task -> SetTrackCuts(vertex_tracks_cuts);
  //
  //   // ***** end of get centrality from multiplicity of tracks stored in track detector passing set of cuts *****

  man->AddTask(task);

  man->Init({filelist}, {"rTree"});
  man->Run(-1);
  man->Finish();
}

int main(int argc, char** argv) {
  if (argc <= 2) {
    std::cout << "Not enough arguments! Please use:" << std::endl;
    std::cout << "   ./fill_centrality filelist centrality_file" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string filelist = argv[1];
  const std::string centrality_file = argv[2];
  const std::string output_file = "centrality.analysistree.root";

  fill_centrality(filelist, centrality_file, output_file);
  return EXIT_SUCCESS;
}
