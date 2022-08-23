#include "instance_of.h"

#include <vector>

namespace
{

static_assert(tools::instance_of<std::vector<int>, std::vector>);
static_assert(!tools::instance_of<int, std::vector>);
static_assert(!tools::instance_of<std::vector<int>&, std::vector>);

} // namespace


int main() {}
