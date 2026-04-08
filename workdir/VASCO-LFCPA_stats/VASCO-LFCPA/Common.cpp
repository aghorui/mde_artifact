#include "Common.h"

size_t STAT_operation_count = 0;
size_t STAT_equality_comparisons = 0;
size_t STAT_enumerations_liveness = 0;
size_t STAT_enumerations_pointsto = 0;
size_t STAT_assignments_liveness = 0;
size_t STAT_assignments_pointsto = 0;
size_t STAT_constructions_liveness = 0;
size_t STAT_constructions_pointsto = 0;
LivenessMDE livenessMDE;
PointsToMDE pointsToMDE(livenessMDE);