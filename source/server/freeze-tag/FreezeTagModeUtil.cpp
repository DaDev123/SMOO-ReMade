#include "server/freeze-tag/FreezeTagMode.hpp"
#include "rs/util.hpp"

bool FreezeTagMode::isPlayerLastSurvivor(PuppetInfo* player) {
    if (!mInfo->mIsPlayerRunner) {
        return false; // If player is on the chaser team, just return false instantly
    }

    if (mInfo->mRunnerPlayers.size() == 0) {
        return false; // If there's no other player on the runner team, last survivor stuff is disabled
    }

    for (int i = 0; i < mInfo->mRunnerPlayers.size(); i++) {
        PuppetInfo* other = mInfo->mRunnerPlayers.at(i);
        if (other == player) {
            continue; // If the puppet getting updated is the one currently being checked, skip this one
        }

        if (!other->isFreezeTagFreeze) {
            return false; // Found another non-frozen player, not last survivor
        }
    }

    return true; // Last survivor check passed!
}

bool FreezeTagMode::areAllOtherRunnersFrozen(PuppetInfo* player) {
    if (mInfo->mRunnerPlayers.size() < 2 - mInfo->mIsPlayerRunner) {
        return false; // Verify there is at least two runners (including yourself), otherwise disable this functionality
        // RCL TODO: if there are only two players, then the round can never be won by chasers?
    }

    if (mInfo->mIsPlayerRunner && !mInfo->mIsPlayerFreeze) {
        return false; // If you are a runner but aren't frozen then skip
    }

    for (int i = 0; i < mInfo->mRunnerPlayers.size(); i++) {
        PuppetInfo* other = mInfo->mRunnerPlayers.at(i);
        if (other == player) {
            continue; // If the puppet getting updated is the one currently being checked, skip this one
        }

        if (!other->isFreezeTagFreeze) {
            return false; // Found a non-frozen player on the runner team, cancel
        }
    }

    return true; // All runners are frozen!
}

PlayerActorHakoniwa* FreezeTagMode::getPlayerActorHakoniwa() {
    PlayerActorBase* playerBase = rs::getPlayerActor(mCurScene);
    bool isYukimaru = !playerBase->getPlayerInfo();

    if (isYukimaru) {
        return nullptr;
    }

    return (PlayerActorHakoniwa*)playerBase;
}
