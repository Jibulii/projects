#include "highscorejson.h"
#include "ioexception.hh"
#include <vector>
#include <string>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>


QString const ASSET_FOLDER("Assets/");

void SaveJson(int points, int saved, QString name,  QString time){
        QFile jsonFile (ASSET_FOLDER + "scores.json");

        if (!jsonFile.open(QIODevice::ReadWrite)) {
            throw Common::IoException("Could not read file");
        }

        //The file is read.
        QJsonDocument systemJSON = QJsonDocument::fromJson(jsonFile.readAll());
        jsonFile.close();
        jsonFile.open(QIODevice::ReadWrite | QIODevice::Truncate);

        //A Json-object is created.
        QJsonObject jsonObj;
        jsonObj["name"] = name;
        jsonObj["points"] = points;
        jsonObj["saved"] = saved;
        jsonObj["time"] = time;

        //The object is appended to an array.
        QJsonArray Array;
        Array = systemJSON.isNull() ? Array : systemJSON.array();
        Array.append(jsonObj);

        //The document is written back to the file.
        QJsonDocument saveDoc(Array);
        jsonFile.write(saveDoc.toJson(QJsonDocument::Compact));
        jsonFile.close();
}

std::vector<std::vector<std::string> > ReadJson()
{
    std::vector< std::vector <std::string > > returnVector;
    QFile jsonfile (ASSET_FOLDER + "scores.json");
    if (!jsonfile.open(QFile::ReadOnly))
    {
        throw Common::IoException("Could not read file");
    }
    QJsonDocument systemJSON = QJsonDocument::fromJson(jsonfile.readAll());

    if (systemJSON.isNull()) {
        throw Common::IoException("Could not read file");
    }
    else {
        auto scoresArray=systemJSON.array();
        //The scores are read from the file, changed to string and assigned to a vector.
        for (auto i = 0; i!=scoresArray.size(); i++) {
            std::vector<std::string> helpVector;
            auto scoreObj = scoresArray.at(i).toObject();
            auto name = scoreObj.value("name").toString().toStdString();
            auto points = std::to_string(scoreObj.value("points").toInt());
            auto saved = std::to_string(scoreObj.value("saved").toInt());
            auto time = scoreObj.value("time").toString().toStdString();
            helpVector.push_back(name);
            helpVector.push_back(points);
            helpVector.push_back(saved);
            helpVector.push_back(time);

            //The vector is pushed back to the returnVector
            returnVector.push_back(helpVector);
        }
        jsonfile.close();
    }
    //The vector is sorted with the points.
    std::sort(returnVector.begin(), returnVector.end(),
              [](const std::vector<std::string>& a, const std::vector<std::string>& b) {
      return std::stoi(a[1]) > std::stoi(b[1]);
    });
    return returnVector;
}
