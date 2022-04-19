#include "AnalysisTree/TaskManager.hpp"
#include "CentralityFiller.hpp"

using namespace AnalysisTree;

void fill_centrality(const std::string& filelist, const std::string& centrality_file, const std::string& output) {

  auto* man = TaskManager::GetInstance();
  man->SetOutputName(output, "aTree");
  man->SetWriteMode(eBranchWriteMode::kCopyTree);
  man->SetBranchesExclude({"TrdTracks", "RecEventHeader", "RichRings"});

  auto* task = new CentralityFiller(centrality_file, "centr_getter_1d");
  task->SetInputEventHeader("RecEventHeader");
  task->SetOutput("AnaEventHeader", "centrality_tracks");
  
  // Uncomment one of two following blocks:
  
  // ***** get centrality from multiplicity stored in the event header *****
  task->SetInput("RecEventHeader", "M");
  // ***** end of get centrality from multiplicity stored in the event header *****
  
//   // ***** get centrality from multiplicity of tracks stored in track detector passing set of cuts *****
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
