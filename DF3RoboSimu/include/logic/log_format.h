#pragma once

enum PIAttackState
{
    PIAttackState_NULL, PIAttackState_attack, PIAttackState_CD, PIAttackState_changemag
};

enum PIInstruction
{
    PIInstruction_NULL, PIInstruction_attack, PIInstruction_slowdown, PIInstruction_speedup, PIInstruction_defend, PIInstruction_changemag
};

struct PlayerInfo
{
    int HP;
    int MP;
    int MAG;
    float x;
    float y;
    float angle; // angle of robot
    float sightAngle; // angle of sight relative to angle of robot
    int speedState; //   1: speed x0.5   2: speed x1      3: speed x1.5
    int defenceState; // 1: normal       2: damage -50%
    int attackState; //  1: attack       2: CD            3: recharging 
                     // The instruction that the player uses in this frame
    int instruction; //  1: attack       2: freezing ray  3: speed up    4: sheld        5: recharge
    int instructionCD[4];
    // instructionCD[instruction - 1] saves the corresponding CD time, 0: no CD
};

struct MapInfo
{
    // -n: there is no such item on the map; after n frames, this item will appear
    // n: this item has existed for n frames; n = 0 means this item appears on the current frame
    int HP1;
    int HP2;
    int MP1;
    int MP2;
    int speedBuf;
    int defendBuff;
};

struct RepFrame
{
    PlayerInfo player1;
    PlayerInfo player2;
    MapInfo mapInfo;
};
