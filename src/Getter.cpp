#include "Getter.h"

namespace Centrality {


float Getter::GetCentrality(float value)
{
    const int ibin = borders_.FindBin(value);
    return ranges_.at(ibin-1);
}


}
