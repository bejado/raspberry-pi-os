#ifndef _P_HVS_H
#define _P_HVS_H

#include "peripherals/base.h"

#define SCALER_DISPLIST0                        (PBASE+0x00400020)
#define SCALER_DISPLIST1                        (PBASE+0x00400024)
#define SCALER_DISPLIST2                        (PBASE+0x00400028)

#define SCALER_CTL0_END                         1U << 31
#define SCALER_CTL0_VALID                       1U << 30
#define SCALER_CTL0_UNITY                       1U << 4

#define SCALER_CTL0_RGBA_EXPAND_ZERO            0
#define SCALER_CTL0_RGBA_EXPAND_LSB             1
#define SCALER_CTL0_RGBA_EXPAND_MSB             2
#define SCALER_CTL0_RGBA_EXPAND_ROUND           3

#define SCALER_CTL0_HFLIP                       1U << 16
#define SCALER_CTL0_VFLIP                       1U << 15

#endif  /*_P_HVS_H */
