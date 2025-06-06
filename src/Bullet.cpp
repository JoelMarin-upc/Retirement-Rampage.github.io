#include <raylib.h>
#include <vector>
#include "Bullet.h"
#include "Explosion.h"
#include "MapReader.h"
#include "Game.h"
#include "SoundEffects.h"

void Bullet::Update() {
    if (destroyed) {
        explosion.Update();
        return;
    }
    if (HasCollision()) Explode();
    //uses screen size
    else if (position.x > Game::screenWidth * 1.35 || position.x < - Game::screenWidth * 0.35 || position.y < - Game::screenHeight * 0.35 || position.y > Game::screenHeight * 1.35) destroyed = true;
    else Shoot();
    animation.Update();
}

bool Bullet::Charging() {
    velocityModule += velocityIncrease;
    barCounter = velocityModule / barVelocity;
    if (barCounter >= maxVelocity) {
        barCounter = 0;
        return false;
    }

    return true;
}

void Bullet::Shoot() {
    actualVelocity.y += gravity;
    actualVelocity.x += wind;
    position.x += actualVelocity.x * GetFrameTime();
    position.y += actualVelocity.y * GetFrameTime();
    animation.position = position;
}

void Bullet::InitialVelocity(Vector2 direction) { actualVelocity = { direction.x * velocityModule, direction.y * velocityModule }; }

void Bullet::Draw() {
    if (destroyed) explosion.Draw();
    else if (isProjectileOnAir == true)
    {
        /*DrawCircle(position.x, position.y, bulletRadius, RED);*/
        animation.Draw();
    }
    
}

void Bullet::DrawBar() {
    if (!destroyed && isProjectileOnAir == false) {
        float lenght = barCounter / 60;
        barPosition = { 1500 - 200, 60 };
        for (int i = 0; i < lenght; i++) {
            DrawRectangle(i * barSize.x + barPosition.x, barPosition.y, barSize.x, barSize.y, ORANGE);
        }
        if (lenght > 0) DrawRectangle(maxVelocity / 60 * barSize.x + barPosition.x, barPosition.y, barSize.x * 3, barSize.y, RED);
    }
}

bool Bullet::HasCollision() {
    bool collision = false;
    MapReader* mapObj = Game::GetMap();
    std::vector<MapTile> map = mapObj->GetOptimizedMap();
    //buscar posiciones players
    //std::vector<MapTile> players = mapObj->GetPlayers();
    //map.insert(map.end(), players.begin(), players.end());
    for (int i = 0; i < map.size(); i++) {
        if (CheckCollisionCircleRec(position, bulletRadius * 0.8f, map[i].GetRectangle())) {
            collision = true;
            break;
        }
    }
    return collision;
}

void Bullet::Explode() {
    explosion = Explosion(position, explosionRadius, explosionMiliseconds);
    destroyed = true;
    MapReader* mapObj = Game::GetMap();
    TurnManager* turnObj = Game::GetTurnManager();
    mapObj->DestroyTiles(explosion);
    turnObj->CheckPlayerHit(explosion);

}
