#include <raylib.h>
#include <vector>
#include "Shotgun.h"
#include "Explosion.h"
#include "MapReader.h"
#include "Game.h"
#include "SoundEffects.h"

void Shotgun::Update() {
    --distanceFrames;
    if (destroyed) {
        explosion.Update();
        return;
    }
    else if (HasCollision() || distanceFrames < 0) Explode();
    // uses screen size
    else if (position.x > Game::screenWidth || position.x < 0 || position.y < 0 || position.y > Game::screenHeight) destroyed = true;
    else Shoot();
    animation.Update();
}

void Shotgun::Shoot() {
    position.x += actualVelocity.x * GetFrameTime();
    position.y += actualVelocity.y * GetFrameTime();
    animation.position = position;
}

void Shotgun::InitialVelocity(Vector2 direction) { actualVelocity = { direction.x * velocityModule, direction.y * velocityModule}; }

void Shotgun::Draw() {
    if (destroyed) explosion.Draw();
    else if (isProjectileOnAir == true)
    {
        animation.Draw();
    }
}

bool Shotgun::HasCollision() {
    bool collision = false;
    MapReader* mapObj = Game::GetMap();
    std::vector<MapTile> map = mapObj->GetOptimizedMap();
    //std::vector<MapTile> players = mapObj->GetPlayers();
    //map.insert(map.end(), players.begin(), players.end());
    TurnManager* turnObj = Game::GetTurnManager();
    if (distanceFrames <= playerOutFrames) {
        for (int i = 0;i < turnObj->playerList.size();++i) {
            if (CheckCollisionCircleRec(position, bulletRadius, turnObj->playerList[i]->GetRectangle())) {
                collision = true;
                turnObj->playerList[i]->GetDamaged(position);
                break;
            }
        }
    }
    for (int i = 0; i < map.size(); i++) {
        if (CheckCollisionCircleRec(position, bulletRadius, map[i].GetRectangle())) {
            collision = true;
            break;
        }
    }
    return collision;
}

void Shotgun::Explode() {
    //explosion = Explosion(position, explosionRadius, explosionMiliseconds);
    DrawCircle(position.x, position.y, explosionRadius , WHITE);
    destroyed = true;
  //  TurnManager* turnObj = Game::GetTurnManager();
    //turnObj->CheckPlayerHit(explosion);
    //MapReader* mapObj = dynamic_cast<MapReader*>(Game::gameObjects[0].get());
   // mapObj->DestroyTiles(explosion);
}
