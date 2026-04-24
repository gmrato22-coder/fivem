#pragma once

namespace Core {

    class OffsetsClass {
    public:

        //Class Addrs
        uintptr_t m_World, //48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81
            m_ReplayInterFace, //48 8D 0D ? ? ? ? 89 44 24 30 E8 ? ? ? ? 48 83 C4 28 C3 48 8B 05 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B CF
            m_ViewPort, //48 8B 15 ? ? ? ? 48 8D 2D ? ? ? ? 48 8B CD
            m_BlipList, //4C 8D 05 ? ? ? ? 0F B7 C1
            m_CamGameplayDirector, // 48 8B 05 ? ? ? ? 48 8B 98 ? ? ? ? EB
            m_LocalPlayer;


        //Function Addrs
        uintptr_t m_SilentAim,
            m_InfiniteAmmo0,
            m_InfiniteAmmo1,
            m_MagicBulletsPatch,
            m_ArmsKinematics,
            m_LegsKinematics,
            m_InfiniteCombatRoll,
            m_GiveWeapon;

        //Offsets
        uintptr_t m_LastVehicle,
            m_Handling,
            m_VehicleEngineHealth,
            m_PlayerInfo,
            m_FragInst,
            m_Armor,
            m_Recoil,
            m_PedFlag,
            m_Spread,
            m_WeaponManager,
            m_WeaponInfo,
            m_VehicleDoorsLockState,
            m_EntityType,
            m_Speed,
            m_MaxHealth,
            m_VehicleDriver,
            m_VehicleGravity,
            m_CObject,
            m_NoRagDoll,
            m_CWeapon,
            m_PlayerId,
            m_CitizenNamesModBase,
            m_NetIdToNamesEntry,
            m_SeatBealt,
            m_AimCPedPatternResult;



        //Infos
        uint32_t CurrentBuild;
        std::string ServerIp;



    };

    inline OffsetsClass g_Offsets;
}