//
// Created by iatow on 19-8-31.
//

#ifndef KUNKUN_DATAHUB_H
#define KUNKUN_DATAHUB_H


class DataHub {
private:
    DataHub() = default;

public:
    static DataHub * getIns();

    std::string username;
};


#endif //KUNKUN_DATAHUB_H
