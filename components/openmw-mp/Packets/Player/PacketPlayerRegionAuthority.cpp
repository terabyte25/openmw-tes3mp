#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketWorldRegionAuthority.hpp"

mwmp::PacketWorldRegionAuthority::PacketWorldRegionAuthority(RakNet::RakPeerInterface *peer) : PlayerPacket(peer)
{
    packetID = ID_WORLD_REGION_AUTHORITY;
    priority = IMMEDIATE_PRIORITY;
    reliability = RELIABLE_ORDERED;
}

void mwmp::PacketWorldRegionAuthority::Packet(RakNet::BitStream *bs, bool send)
{
    PlayerPacket::Packet(bs, send);

    RW(player->authorityRegion, send, true);
}
