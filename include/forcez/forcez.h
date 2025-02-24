#ifndef _FORCEZ_
#define _FORCEZ_

#include "forcez/conf.h"

#include <optional>
#include <string>

namespace forcez {

std::optional<std::string> ExtractArchive(const Conf &);

} // namespace forcez

#endif
