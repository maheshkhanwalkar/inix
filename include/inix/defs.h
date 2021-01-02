#pragma once
#define DIV_UP(x, y) (((x) + (y) - 1) / (y))

// FIXME: this needs to be a real panic, not just an infinite loop!
#define panic() while(1);
