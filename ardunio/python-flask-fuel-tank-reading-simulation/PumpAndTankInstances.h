#ifndef PUMP_AND_TANK_INSTANCES_H
#define PUMP_AND_TANK_INSTANCES_H

#include "GasPumpSimulation.h"

// Fuel Tanks
// 4 Regular, 4 Diesel, 2 Premium
FuelTank regularTanks[4] = {
  FuelTank("Regular Tank 1", 9, 10, 11, 10000.0, 2400.0, 72.5, 3200),
  FuelTank("Regular Tank 2", 9, 10, 11, 10000.0, 2400.0, 72.5, 3200),
  FuelTank("Regular Tank 3", 9, 10, 11, 10000.0, 2400.0, 72.5, 3200),
  FuelTank("Regular Tank 4", 9, 10, 11, 10000.0, 2400.0, 72.5, 3200)
};

FuelTank dieselTanks[4] = {
  FuelTank("Diesel Tank 1", 6, 5, 3, 10000.0, 2400.0, 72.5, 3000),
  FuelTank("Diesel Tank 2", 6, 5, 3, 10000.0, 2400.0, 72.5, 3000),
  FuelTank("Diesel Tank 3", 6, 5, 3, 10000.0, 2400.0, 72.5, 3000),
  FuelTank("Diesel Tank 4", 6, 5, 3, 10000.0, 2400.0, 72.5, 3000)
};

FuelTank premiumTanks[2] = {
  FuelTank("Premium Tank 1", 9, 10, 11, 10000.0, 2400.0, 72.5, 3200),
  FuelTank("Premium Tank 2", 9, 10, 11, 10000.0, 2400.0, 72.5, 3200)
};

// Fuel Pumps
// Pumps 1 - 4 are regular
// Pumps 5 - 8 are diesel
// Pumps 9 - 10 are premium
FuelPump pumps[10] = {
  FuelPump("Pump 1", &regularTanks[0]),
  FuelPump("Pump 2", &regularTanks[1]),
  FuelPump("Pump 3", &regularTanks[2]),
  FuelPump("Pump 4", &regularTanks[3]), 
  FuelPump("Pump 5", &dieselTanks[1]),
  FuelPump("Pump 6", &dieselTanks[2]),
  FuelPump("Pump 7", &dieselTanks[3]),
  FuelPump("Pump 8", &dieselTanks[0]),
  FuelPump("Pump 9", &premiumTanks[0]),
  FuelPump("Pump 10", &premiumTanks[1])
};

#endif
