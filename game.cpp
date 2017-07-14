//--------------------------------------------------------------------------------
//
//　game.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include <time.h>
#include "input.h"
#include "game.h"
#include "fade.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "explosionEffect.h"
#include "score.h"
#include "timer.h"
#include "KF_BodyContact.h"
#include "UI_bullet.h"
#include "sound.h"
#include "UI_life.h"
#include "UIgame.h"

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitGame(void)
{
	srand((unsigned)time(NULL));

	InitUIBullet();
	InitPlayer();
	InitEnemy();
	InitBullet();
	InitBodyContact();
	InitExplosion();
	InitScore();
	InitTimer();
	InitExplosionEffect();
	InitUILife();
	InitUIGame();
	PlaySound(SOUND_LABEL_BGM_GAME);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitGame(void)
{
	UninitPlayer();
	UninitEnemy();
	UninitBullet();
	UninitBodyContact();
	UninitExplosion();
	UninitScore();
	UninitTimer();
	UninitUIBullet();
	UninitExplosionEffect();
	UninitUILife();
	UninitUIGame();
	StopSound(SOUND_LABEL_BGM_GAME);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateGame(void)
{
	UpdatePlayer();
	UpdateEnemy();
	UpdateBullet();
	UpdateBodyContact();
	UpdateExplosion();
	UpdateScore();
	UpdateTimer();
	UpdateUIBullet();
	UpdateExplosionEffect();
	UpdateUILife();
	UpdateUIGame();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawGame(void)
{
	DrawPlayer();
	DrawEnemy();
	DrawBullet();
	DrawTimer();
	DrawExplosionEffect();
	DrawUIGame();
	DrawScore();
	DrawUIBullet();
	DrawUILife();
}