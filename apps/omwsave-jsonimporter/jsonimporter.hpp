#ifndef JSON_IMPORTER
#define JSON_IMPORTER

#include <string>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <components/esm/esmreader.hpp>

class jsonimporter {
    public:
        jsonimporter(const std::string& input, const std::string& output);
        ~jsonimporter();
        void defaultsettings();
        void convert();
        void convertJournal(ESM::ESMReader& esm);
        void convertnpc(ESM::ESMReader& esm);
        void convertplayer(ESM::ESMReader& esm);
        void convertsave(ESM::ESMReader& esm);
        void converttopics(ESM::ESMReader& esm);
        void convertmap(ESM::ESMReader& esm);
        void convertkeys(ESM::ESMReader& esm);
        void convertclass(ESM::ESMReader& esm);
        QJsonObject jsonsave;
        QJsonObject character;
        QJsonArray questarray;
    private:
        std::string mInput;
        std::string mOutput;
};

#endif