//
// Created by iatow on 19-8-31.
//

#include "INCLUDE.h"

#include "DataHub.h"

DataHub* DataHub::getIns() {
    static DataHub ins;

    return &ins;
}
