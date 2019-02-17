#include "jsonimporter.hpp"

#include "label.hpp"

#include <string>
#include <vector>
#include <iterator> 
#include <map> 

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSaveFile>
#include <QString>
#include <QFile>

#include <iostream>

#include <components/esm/esmreader.hpp>

#include <components/esm/defs.hpp>

#include <components/esm/player.hpp>
#include <components/esm/queststate.hpp>
#include <components/esm/savedgame.hpp>
#include <components/esm/dialoguestate.hpp>
#include <components/esm/globalmap.hpp>
#include <components/esm/quickkeys.hpp>
#include <components/esm/npcstats.hpp>

#include <components/esm/records.hpp>


jsonimporter::jsonimporter(const std::string& input, const std::string& output) 
    : mInput(input)
    , mOutput(output)
{
    
}
jsonimporter::~jsonimporter()
{
    jsonsave["journal"] = questarray;

    jsonsave["character"] = character;

    QJsonDocument json_doc(jsonsave);
    QString json_string = json_doc.toJson();

    QFile save_file(QString::fromStdString(mOutput));
    if (!save_file.open(QIODevice::WriteOnly)) {
        std::cout << "failed to open save file" << std::endl;
        return;
    }
    save_file.write(json_string.toLocal8Bit());
    save_file.close();
}
void jsonimporter::convert()
{
    ESM::ESMReader esm;
    ToUTF8::Utf8Encoder encoder (ToUTF8::calculateEncoding("win1252"));
    esm.open(mInput);
    esm.setEncoder(&encoder);
    defaultsettings();
    while (esm.hasMoreRecs())
    {
        ESM::NAME n = esm.getRecName();
        esm.getRecHeader();
        switch (n.intval)
        {
            case ESM::REC_NPC_:
                convertnpc(esm);
                break;
            case ESM::REC_PLAY:
                convertplayer(esm);
                break;
            case ESM::REC_QUES:
                convertJournal(esm);
                break;
            case ESM::REC_DIAS:
                converttopics(esm);
                break;
            case ESM::REC_GMAP:
                convertmap(esm);
                break;
            case ESM::REC_KEYS:
                convertkeys(esm);
                break;
            case ESM::REC_CLAS:
                convertclass(esm);
                break;
            default:
                esm.skipRecord();
                break;
        }
    }

}
void jsonimporter::defaultsettings() {
    QJsonObject defaultsettings;
    defaultsettings["difficulty"] = QString::fromStdString("default");
    defaultsettings["enforcedLogLevel"] = QString::fromStdString("default");
    defaultsettings["physicsFramerate"] = QString::fromStdString("default");
    defaultsettings["consoleAllowed"] = QString::fromStdString("default");
    defaultsettings["bedRestAllowed"] = QString::fromStdString("default");
    defaultsettings["wildernessRestAllowed"] = QString::fromStdString("default");
    defaultsettings["waitAllowed"] = QString::fromStdString("default");
    defaultsettings["staffRank"] = 0;
    jsonsave["settings"] = defaultsettings;
}

void jsonimporter::convertclass(ESM::ESMReader& esm) {
    ESM::Class record;
    bool shit = false;
    record.load(esm, shit);
    QJsonObject customClass;
    customClass["specialization"] = record.mData.mSpecialization;
    customClass["name"] = QString::fromStdString(record.mName);
    customClass["description"] = QString::fromStdString(record.mDescription);
    std::string MinorSkills = "";
    std::string MajorSkills = "";
    std::string Attributes = "";
    for(int i = 0; i < 5; i++ ) {
        MinorSkills += (skillLabel(record.mData.mSkills[i][0]) + (i == 4 ? "" : ", "));
        MajorSkills += (skillLabel(record.mData.mSkills[i][1]) + (i == 4 ? "" : ", "));
    }
    for(int i = 0; i < 2; i++)
        Attributes += (skillLabel(record.mData.mAttribute[i]) + (i == 1 ? "" : ", "));
    customClass["minorSkills"] = QString::fromStdString(MinorSkills);
    customClass["majorSkills"] = QString::fromStdString(MajorSkills);
    customClass["majorAttributes"] = QString::fromStdString(Attributes);
    jsonsave["customClass"] = customClass;
}
void jsonimporter::convertkeys(ESM::ESMReader& esm) {
    ESM::QuickKeys record;
    record.load(esm);
    QJsonArray keyarray;
    for(ESM::QuickKeys::QuickKey quickkey : record.mKeys) {
        if(!quickkey.mId.empty()) {
        QJsonObject key;
        key["keyType"] = quickkey.mType;
        key["itemId"] = QString::fromStdString(quickkey.mId);
        keyarray.append(key);
        }
    }
    jsonsave["quickKeys"] = keyarray;
}
void jsonimporter::convertmap(ESM::ESMReader& esm) {
    ESM::GlobalMap record;
    record.load(esm);
    QJsonArray map;
    for(std::set< std::pair<int, int> >::iterator it = record.mMarkers.begin(); it != record.mMarkers.end(); ++it){
        int x = it->first;
        int y = it->second;
        map.append(QString::number(x) + QString::fromStdString(", ") + QString::number(y));
    }
    jsonsave["mapExplored"] = map;
}
void jsonimporter::converttopics(ESM::ESMReader& esm) {
    ESM::DialogueState record;
    record.load (esm);
    QJsonArray topicsarray;
    for (std::vector<std::string>::const_iterator iter (record.mKnownTopics.begin()); iter!=record.mKnownTopics.end(); ++iter)
        topicsarray.append(QString::fromStdString(*iter));
    jsonsave["topics"] = topicsarray;
        
}
void jsonimporter::convertJournal(ESM::ESMReader& esm) {
    ESM::QuestState record;
    record.load (esm);
    QJsonObject quest;
    quest["index"] = record.mState;
    quest["type"] = 0;
    quest["quest"] = QString::fromStdString(record.mTopic);
    questarray.append(quest);
}

void jsonimporter::convertnpc(ESM::ESMReader& esm) {
    ESM::NPC record;
    bool shit = false; // this needs to be refractored
    record.load (esm, shit);
    if(record.mId == "player") {
        character["gender"] = (record.isMale() ? 1 : 2);
        character["race"] =  QString::fromStdString(record.mRace);
        character["head"] =  QString::fromStdString(record.mHead);
        character["hair"] =  QString::fromStdString(record.mHair);
        character["class"] =  record.mClass.find("$dynamic") != std::string::npos ? QString::fromStdString("custom") : QString::fromStdString(record.mClass);

        QJsonObject login;
        login["name"] = QString::fromStdString(record.mName);
        jsonsave["login"] = login;
    }
}

void jsonimporter::convertplayer(ESM::ESMReader& esm){
    ESM::Player record;
    const ESM::Header& header = esm.getHeader();
    record.load (esm);

    character["birthsign"] = QString::fromStdString(record.mBirthsign);

    QJsonObject location;
    location["cell"] = QString::number(record.mCellId.mIndex.mX) + QString::fromStdString(", ") + QString::number(record.mCellId.mIndex.mY);
    location["posX"] = record.mObject.mPosition.pos[0];
    location["posY"] = record.mObject.mPosition.pos[1];
    location["posZ"] = record.mObject.mPosition.pos[2];
    location["rotX"] = record.mObject.mPosition.rot[0];
    location["rotZ"] = record.mObject.mPosition.rot[2];
    jsonsave["player"] = location;

    QJsonObject fame;
    fame["reputation"] = record.mObject.mNpcStats.mReputation;
    fame["bounty"] = record.mObject.mNpcStats.mBounty;
    jsonsave["fame"] = fame;

    QJsonObject shapeshift;
    shapeshift["isWerewolf"] = record.mObject.mNpcStats.mIsWerewolf;
    shapeshift["scale"] = record.mObject.mRef.mScale;
    jsonsave["shapeshift"] = shapeshift;

    QJsonObject stats;
    stats["healthBase"] = record.mObject.mCreatureStats.mDynamic[0].mBase;
    stats["magickaBase"] = record.mObject.mCreatureStats.mDynamic[1].mBase;
    stats["fatigueBase"] = record.mObject.mCreatureStats.mDynamic[2].mBase;
    stats["healthCurrent"] = record.mObject.mCreatureStats.mDynamic[0].mCurrent;
    stats["magickaCurrent"] = record.mObject.mCreatureStats.mDynamic[1].mCurrent;
    stats["fatigueCurrent"] = record.mObject.mCreatureStats.mDynamic[2].mCurrent;
    stats["level"] = record.mObject.mCreatureStats.mLevel;
    stats["levelProgress"] = record.mObject.mNpcStats.mLevelProgress;
    jsonsave["stats"] = stats;

    QJsonObject attributes;
    attributes["Strength"] = record.mObject.mCreatureStats.mAttributes[0].mBase;
    attributes["Agility"] = record.mObject.mCreatureStats.mAttributes[1].mBase;
    attributes["Willpower"] = record.mObject.mCreatureStats.mAttributes[2].mBase;
    attributes["Speed"] = record.mObject.mCreatureStats.mAttributes[3].mBase;
    attributes["Intelligence"] = record.mObject.mCreatureStats.mAttributes[4].mBase;
    attributes["Endurance"] = record.mObject.mCreatureStats.mAttributes[5].mBase;
    attributes["Luck"] = record.mObject.mCreatureStats.mAttributes[6].mBase;
    attributes["Personality"] = record.mObject.mCreatureStats.mAttributes[7].mBase;
    jsonsave["attributes"] = attributes;

    QJsonObject attributeSkillIncreases;
    attributeSkillIncreases["Strength"] = record.mObject.mNpcStats.mSkillIncrease[0];
    attributeSkillIncreases["Agility"] = record.mObject.mNpcStats.mSkillIncrease[1];
    attributeSkillIncreases["Willpower"] = record.mObject.mNpcStats.mSkillIncrease[2];
    attributeSkillIncreases["Speed"] = record.mObject.mNpcStats.mSkillIncrease[3];
    attributeSkillIncreases["Intelligence"] = record.mObject.mNpcStats.mSkillIncrease[4];
    attributeSkillIncreases["Endurance"] = record.mObject.mNpcStats.mSkillIncrease[5];
    attributeSkillIncreases["Luck"] = record.mObject.mNpcStats.mSkillIncrease[6];
    attributeSkillIncreases["Personality"] = record.mObject.mNpcStats.mSkillIncrease[7];
    jsonsave["attributeSkillIncreases"] = attributeSkillIncreases;

    QJsonObject skills;
    skills["Block"] = record.mObject.mNpcStats.mSkills[0].mBase;
    skills["Alchemy"] = record.mObject.mNpcStats.mSkills[1].mBase;
    skills["Restoration"] = record.mObject.mNpcStats.mSkills[2].mBase;
    skills["Conjuration"] = record.mObject.mNpcStats.mSkills[3].mBase;
    skills["Marksman"] = record.mObject.mNpcStats.mSkills[4].mBase;
    skills["Bluntweapon"] = record.mObject.mNpcStats.mSkills[5].mBase;
    skills["Shortblade"] = record.mObject.mNpcStats.mSkills[6].mBase;
    skills["Heavyarmor"] = record.mObject.mNpcStats.mSkills[7].mBase;
    skills["Handtohand"] = record.mObject.mNpcStats.mSkills[8].mBase;
    skills["Alteration"] = record.mObject.mNpcStats.mSkills[9].mBase;
    skills["Enchant"] = record.mObject.mNpcStats.mSkills[10].mBase;
    skills["Sneak"] = record.mObject.mNpcStats.mSkills[11].mBase;
    skills["Lightarmor"] = record.mObject.mNpcStats.mSkills[12].mBase;
    skills["Athletics"] = record.mObject.mNpcStats.mSkills[13].mBase;
    skills["Armorer"] = record.mObject.mNpcStats.mSkills[14].mBase;
    skills["Speechcraft"] = record.mObject.mNpcStats.mSkills[15].mBase;
    skills["Axe"] = record.mObject.mNpcStats.mSkills[16].mBase;
    skills["Security"] = record.mObject.mNpcStats.mSkills[17].mBase;
    skills["Acrobatics"] = record.mObject.mNpcStats.mSkills[18].mBase;
    skills["Destruction"] = record.mObject.mNpcStats.mSkills[19].mBase;
    skills["Longblade"] = record.mObject.mNpcStats.mSkills[20].mBase;
    skills["Illusion"] = record.mObject.mNpcStats.mSkills[21].mBase;
    skills["Mysticism"] = record.mObject.mNpcStats.mSkills[22].mBase;
    skills["Spear"] = record.mObject.mNpcStats.mSkills[23].mBase;
    skills["Mercantile"] = record.mObject.mNpcStats.mSkills[24].mBase;
    skills["Mediumarmor"] = record.mObject.mNpcStats.mSkills[25].mBase;
    skills["Unarmored"] = record.mObject.mNpcStats.mSkills[26].mBase;
    jsonsave["skills"] = skills;

    QJsonObject skillProgress;
    skillProgress["Block"] = record.mObject.mNpcStats.mSkills[0].mProgress;
    skillProgress["Alchemy"] = record.mObject.mNpcStats.mSkills[1].mProgress;
    skillProgress["Restoration"] = record.mObject.mNpcStats.mSkills[2].mProgress;
    skillProgress["Conjuration"] = record.mObject.mNpcStats.mSkills[3].mProgress;
    skillProgress["Marksman"] = record.mObject.mNpcStats.mSkills[4].mProgress;
    skillProgress["Bluntweapon"] = record.mObject.mNpcStats.mSkills[5].mProgress;
    skillProgress["Shortblade"] = record.mObject.mNpcStats.mSkills[6].mProgress;
    skillProgress["Heavyarmor"] = record.mObject.mNpcStats.mSkills[7].mProgress;
    skillProgress["Handtohand"] = record.mObject.mNpcStats.mSkills[8].mProgress;
    skillProgress["Alteration"] = record.mObject.mNpcStats.mSkills[9].mProgress;
    skillProgress["Enchant"] = record.mObject.mNpcStats.mSkills[10].mProgress;
    skillProgress["Sneak"] = record.mObject.mNpcStats.mSkills[11].mProgress;
    skillProgress["Lightarmor"] = record.mObject.mNpcStats.mSkills[12].mProgress;
    skillProgress["Athletics"] = record.mObject.mNpcStats.mSkills[13].mProgress;
    skillProgress["Armorer"] = record.mObject.mNpcStats.mSkills[14].mProgress;
    skillProgress["Speechcraft"] = record.mObject.mNpcStats.mSkills[15].mProgress;
    skillProgress["Axe"] = record.mObject.mNpcStats.mSkills[16].mProgress;
    skillProgress["Security"] = record.mObject.mNpcStats.mSkills[17].mProgress;
    skillProgress["Acrobatics"] = record.mObject.mNpcStats.mSkills[18].mProgress;
    skillProgress["Destruction"] = record.mObject.mNpcStats.mSkills[19].mProgress;
    skillProgress["Longblade"] = record.mObject.mNpcStats.mSkills[20].mProgress;
    skillProgress["Illusion"] = record.mObject.mNpcStats.mSkills[21].mProgress;
    skillProgress["Mysticism"] = record.mObject.mNpcStats.mSkills[22].mProgress;
    skillProgress["Spear"] = record.mObject.mNpcStats.mSkills[23].mProgress;
    skillProgress["Mercantile"] = record.mObject.mNpcStats.mSkills[24].mProgress;
    skillProgress["Mediumarmor"] = record.mObject.mNpcStats.mSkills[25].mProgress;
    skillProgress["Unarmored"] = record.mObject.mNpcStats.mSkills[26].mProgress;
    jsonsave["skillProgress"] = skillProgress;
    QJsonArray inventory;
    for(int i = 0; i < record.mObject.mInventory.mItems.size(); i++) {
        
        QJsonObject item;
        item["enchantmentCharge"] = record.mObject.mInventory.mItems[i].mRef.mEnchantmentCharge;
        item["count"] = record.mObject.mInventory.mItems[i].mCount;
        item["refID"] = QString::fromStdString(record.mObject.mInventory.mItems[i].mRef.mRefID);
        item["charge"] = record.mObject.mInventory.mItems[i].mRef.mChargeIntRemainder;
        item["soul"] = QString::fromStdString(record.mObject.mInventory.mItems[i].mRef.mSoul);
        inventory.append(item);
    }
    jsonsave["inventory"] = inventory;
    
    QJsonObject misc;
    misc["selectedSpell"] = QString::fromStdString(record.mObject.mCreatureStats.mSpells.mSelectedSpell);
    QJsonObject mark;
    mark["cell"] = QString::number(record.mMarkedCell.mIndex.mX) + QString::fromStdString(", ") + QString::number(record.mMarkedCell.mIndex.mY);
    mark["posX"] = record.mMarkedPosition.pos[0];
    mark["posY"] = record.mMarkedPosition.pos[1];
    mark["posZ"] = record.mMarkedPosition.pos[2];
    mark["rotX"] = record.mMarkedPosition.rot[0];
    mark["rotY"] = record.mMarkedPosition.rot[2];
    misc["markLocation"] = mark;
    jsonsave["miscellaneous"] = misc;

    QJsonArray spellarray;
    for(const auto &p : record.mObject.mCreatureStats.mSpells.mSpells)
        spellarray.append(QString::fromStdString(p.first)); // currently appends EVERY spell, regardless if part of racial ability or not
    jsonsave["spellbook"] = spellarray;

    QJsonObject factionRanks;
    QJsonObject factionReputation;
    QJsonArray factionExpulsion;
    for(std::map<std::string, ESM::NpcStats::Faction>::iterator it = record.mObject.mNpcStats.mFactions.begin(); it != record.mObject.mNpcStats.mFactions.end(); it++)
    {
        factionRanks[QString::fromStdString(it->first)] = it->second.mRank;
        factionReputation[QString::fromStdString(it->first)] = it->second.mReputation;
        if(it->second.mExpelled)
            factionExpulsion.append(QString::fromStdString(it->first));
    }
    jsonsave["factionRanks"] = factionRanks;
    jsonsave["factionReputation"] = factionReputation;
    jsonsave["factionExpulsion"] = factionExpulsion;
}