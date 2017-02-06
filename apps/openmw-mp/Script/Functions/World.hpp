#ifndef OPENMW_WORLD_HPP
#define OPENMW_WORLD_HPP

#define WORLDFUNCTIONS \
    {"CreateBaseEvent",     WorldFunctions::CreateBaseEvent},\
    \
    {"AddWorldObject",       WorldFunctions::AddWorldObject},\
    {"SetBaseEventCell",    WorldFunctions::SetBaseEventCell},\
    \
    {"SetObjectRefId",       WorldFunctions::SetObjectRefId},\
    {"SetObjectRefNumIndex", WorldFunctions::SetObjectRefNumIndex},\
    {"SetObjectCharge",      WorldFunctions::SetObjectCharge},\
    {"SetObjectCount",       WorldFunctions::SetObjectCount},\
    {"SetObjectGoldValue",   WorldFunctions::SetObjectGoldValue},\
    {"SetObjectScale",       WorldFunctions::SetObjectScale},\
    {"SetObjectDoorState",   WorldFunctions::SetObjectDoorState},\
    {"SetObjectLockLevel",   WorldFunctions::SetObjectLockLevel},\
    {"SetObjectPosition",    WorldFunctions::SetObjectPosition},\
    {"SetObjectRotation",    WorldFunctions::SetObjectRotation},\
    \
    {"GetObjectChangesSize", WorldFunctions::GetObjectChangesSize},\
    \
    {"GetObjectRefId",       WorldFunctions::GetObjectRefId},\
    {"GetObjectRefNumIndex", WorldFunctions::GetObjectRefNumIndex},\
    {"GetObjectCharge",      WorldFunctions::GetObjectCharge},\
    {"GetObjectCount",       WorldFunctions::GetObjectCount},\
    {"GetObjectGoldValue",   WorldFunctions::GetObjectGoldValue},\
    {"GetObjectScale",       WorldFunctions::GetObjectScale},\
    {"GetObjectDoorState",   WorldFunctions::GetObjectDoorState},\
    {"GetObjectLockLevel",   WorldFunctions::GetObjectLockLevel},\
    {"GetObjectPosX",        WorldFunctions::GetObjectPosX},\
    {"GetObjectPosY",        WorldFunctions::GetObjectPosY},\
    {"GetObjectPosZ",        WorldFunctions::GetObjectPosZ},\
    {"GetObjectRotX",        WorldFunctions::GetObjectRotX},\
    {"GetObjectRotY",        WorldFunctions::GetObjectRotY},\
    {"GetObjectRotZ",        WorldFunctions::GetObjectRotZ},\
    \
    {"SendObjectDelete",     WorldFunctions::SendObjectDelete},\
    {"SendObjectPlace",      WorldFunctions::SendObjectPlace},\
    {"SendObjectScale",      WorldFunctions::SendObjectScale},\
    {"SendObjectLock",       WorldFunctions::SendObjectLock},\
    {"SendObjectUnlock",     WorldFunctions::SendObjectUnlock},\
    {"SendDoorState",        WorldFunctions::SendDoorState},\
    \
    {"SetHour",              WorldFunctions::SetHour},\
    {"SetMonth",             WorldFunctions::SetMonth},\
    {"SetDay",               WorldFunctions::SetDay}

class WorldFunctions
{
public:

    static void CreateBaseEvent(unsigned short pid) noexcept;

    static void AddWorldObject() noexcept;
    static void SetBaseEventCell(const char* cellDescription) noexcept;

    static void SetObjectRefId(const char* refId) noexcept;
    static void SetObjectRefNumIndex(int refNumIndex) noexcept;
    static void SetObjectCharge(int charge) noexcept;
    static void SetObjectCount(int count) noexcept;
    static void SetObjectGoldValue(int goldValue) noexcept;
    static void SetObjectScale(double scale) noexcept;
    static void SetObjectDoorState(int doorState) noexcept;
    static void SetObjectLockLevel(int lockLevel) noexcept;
    static void SetObjectPosition(double x, double y, double z) noexcept;
    static void SetObjectRotation(double x, double y, double z) noexcept;

    static unsigned int GetObjectChangesSize() noexcept;

    static const char *GetObjectRefId(unsigned int i) noexcept;
    static int GetObjectRefNumIndex(unsigned int i) noexcept;
    static int GetObjectCharge(unsigned int i) noexcept;
    static int GetObjectCount(unsigned int i) noexcept;
    static int GetObjectGoldValue(unsigned int i) noexcept;
    static double GetObjectScale(unsigned int i) noexcept;
    static int GetObjectDoorState(unsigned int i) noexcept;
    static int GetObjectLockLevel(unsigned int i) noexcept;
    static double GetObjectPosX(unsigned int i) noexcept;
    static double GetObjectPosY(unsigned int i) noexcept;
    static double GetObjectPosZ(unsigned int i) noexcept;
    static double GetObjectRotX(unsigned int i) noexcept;
    static double GetObjectRotY(unsigned int i) noexcept;
    static double GetObjectRotZ(unsigned int i) noexcept;

    static void SendObjectDelete() noexcept;
    static void SendObjectPlace() noexcept;
    static void SendObjectScale() noexcept;
    static void SendObjectLock() noexcept;
    static void SendObjectUnlock() noexcept;
    static void SendDoorState() noexcept;

    static void SetHour(unsigned short pid, double hour) noexcept;
    static void SetMonth(unsigned short pid, int month) noexcept;
    static void SetDay(unsigned short pid, int day) noexcept;
};


#endif //OPENMW_WORLD_HPP
