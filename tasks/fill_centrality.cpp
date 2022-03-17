#include "AnalysisTree/TaskManager.hpp"
#include "CentralityFiller.hpp"

using namespace AnalysisTree;

void fill_centrality(const std::string& filelist, const std::string& centrality_file, const std::string& output) {

  auto* man = TaskManager::GetInstance();
  man->SetOutputName(output, "aTree");
  man->SetOutputTreeConfig(OutputTreeConfig(eBranchWriteMode::kCopyTree));

  auto* task = new CentralityFiller(centrality_file, "centr_getter_1d");
  task->SetInput("RecEventHeader", "M");
  task->SetOutput("AnaEventHeader", "centrality_tracks");

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
