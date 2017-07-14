//--------------------------------------------------------------------------------
//
//�@enemy.h
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
class CBodyCicle;

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define ENEMY_NUM_MAX (50)

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef enum
{
	ENEMY_SAKE = 0,
	ENEMY_TABAKO,
	ENEMY_MAN,
	ENEMY_MAX
}ENEMY_TYPE;

typedef struct
{
	ENEMY_TYPE type;
	CBodyCicle *pBody;
	bool bUse;
}ENEMY;

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int nID);

#endif