#ifndef BAR_VALUE_FUNCTIONS_H
#define BAR_VALUE_FUNCTIONS_H

#include "core/bar_value.h"
#include <stdbool.h>

void update_bar_value(BarValue* bar, float change, bool isNegativeBar);
void set_bar_value(BarValue* bar, float newValue, bool isNegativeBar);
void get_bar_value(const BarValue* bar, float* percentage);

#endif // BAR_VALUE_FUNCTIONS_H

