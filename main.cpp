#include <Novice.h>
#include <Vector2.h>

const char kWindowTitle[] = "ファントムガイド";//ファントムガイド

enum Scene //シーン切り替え
{
	Title,//タイトル
	Setting,//設定画面
	stageSelect,//ステージセレクト
	Tutorial,//チュートリアル
	stage1,//ステージ1
	stage2,//ステージ2
	stage3,//ステージ3
	stage4,//ステージ4
	stage5,//ステージ5
	stage6,//ステージ6
	stage7,//ステージ7
	stage8,//ステージ8
	stage9,//ステージ9
	stage10,//ステージ10
	GameOver,//ゲームオーバー
	GameClear,//ゲームクリア
};

int Scene = Title;
const int blockSize = 128;//ブロックのサイズ

struct Player//プレイヤー
{
	Vector2 position;
	Vector2 LeftTop;
	Vector2 RightTop;
	Vector2 LeftBottom;
	Vector2 RightBottom;
	Vector2 speed;
};
struct PlayerTile//プレイヤーのマップチップ
{
	Vector2 LeftTop;
	Vector2 LeftBottom;
	Vector2 RightTop;
	Vector2 RightBottom;
};

struct Block//マップチップやブロック
{
	Vector2 position;
	Vector2 LeftTop;
	Vector2 RightTop;
	Vector2 LeftBottom;
	Vector2 RightBottom;
	bool Flag;
};

struct SE
{
	int Timer;
	const int EndTimer;
	bool Flag;
};

///==========================================================================
//Noviceに関する関数
///==========================================================================

//Novice
void NoviceDrawQuadPlayer(Player player, float Size, int Imges)
{
	Novice::DrawQuad(
		(int)player.LeftTop.x + (int)player.position.x, (int)player.LeftTop.y + (int)player.position.y
		, (int)player.RightTop.x + (int)player.position.x, (int)player.RightTop.y + (int)player.position.y
		, (int)player.LeftBottom.x + (int)player.position.x, (int)player.LeftBottom.y + (int)player.position.y
		, (int)player.RightBottom.x + (int)player.position.x, (int)player.RightBottom.y + (int)player.position.y
		, 0, 0
		, (int)Size, (int)Size
		, Imges, WHITE);
}
void NoviceDrawQuadBlock(Block player, float SizeX, float SizeY, int Imges)
{
	Novice::DrawQuad(
		(int)player.LeftTop.x + (int)player.position.x, (int)player.LeftTop.y + (int)player.position.y
		, (int)player.RightTop.x + (int)player.position.x, (int)player.RightTop.y + (int)player.position.y
		, (int)player.LeftBottom.x + (int)player.position.x, (int)player.LeftBottom.y + (int)player.position.y
		, (int)player.RightBottom.x + (int)player.position.x, (int)player.RightBottom.y + (int)player.position.y
		, 0, 0
		, (int)SizeX, (int)SizeY
		, Imges, WHITE);
}
void NoviceBreakDrawQuadBlock(Block block,float BlockHp,float Size,const int Imges1, const int Imges2, const int Imges3)
{
	if (BlockHp == 3.0f)
	{
		NoviceDrawQuadBlock(block, Size, Size, Imges1);
	}

	if (BlockHp == 2.0f)
	{
		NoviceDrawQuadBlock(block, Size, Size, Imges2);
	}

	if (BlockHp == 1.0f)
	{
		NoviceDrawQuadBlock(block, Size, Size, Imges3);
	}
}
void NoviceMapChip(bool& KeyFlag,const int mapX, const int mapY, int stageMap[][13], const int BlockImges, const int goalImges, const int rockGoalImges)
{
	for (int y = 0; y < mapY; y++)
	{
		for (int x = 0; x < mapX; x++)
		{

			if (stageMap[y][x] == 11)//ブロック
			{
				Novice::DrawSprite(x * blockSize, y * blockSize, BlockImges, 1.0f, 1.0f, 0.0f, WHITE);//マップチップの生成
			}

			if (stageMap[y][x] == 99)//ブロック
			{
				if (KeyFlag == false)
				{
					Novice::DrawSprite(x * blockSize, y * blockSize, goalImges, 1.0f, 1.0f, 0.0f, WHITE);//マップチップの生成
				}
				if (KeyFlag == true)
				{
					Novice::DrawSprite(x * blockSize, y * blockSize, rockGoalImges, 1.0f, 1.0f, 0.0f, WHITE);//マップチップの生成
				}
			}

		}
	}
}
void NoviceGoalMapChip(Player* ghost,PlayerTile* TileGhost, int stageMap[][13],bool& stageClearFlag)
{
	//ゴール
	if (stageMap[(int)TileGhost->RightBottom.y][(int)TileGhost->RightBottom.x] == 99 && stageMap[(int)TileGhost->LeftBottom.y][(int)TileGhost->LeftBottom.x] == 99 ||
		stageMap[(int)TileGhost->RightTop.y][(int)TileGhost->RightTop.x] == 99 && stageMap[(int)TileGhost->LeftTop.y][(int)TileGhost->LeftTop.x] == 99 ||
		stageMap[(int)TileGhost->RightTop.y][(int)TileGhost->RightTop.x] == 99 && stageMap[(int)TileGhost->RightBottom.y][(int)TileGhost->RightBottom.x] == 99 ||
		stageMap[(int)TileGhost->LeftTop.y][(int)TileGhost->LeftTop.x] == 99 && stageMap[(int)TileGhost->LeftBottom.y][(int)TileGhost->LeftBottom.x]   == 99)
	{
		ghost->speed.y = 0;
		ghost->speed.x = 0;
		stageClearFlag = true;
	}
}
void NoviceDrawSprite(Player player, int textureHandleImges)
{
	Novice::DrawSprite((int)player.position.x, (int)player.position.y, textureHandleImges, 1, 1, 0.0f, WHITE);
}

///==========================================================================
//初期化,加算に関する関数
///==========================================================================

//ステージギミック位置リセット
void arrowReset(Block* rightArrow, Block* leftArrow, Block* upArrow, Block* dwonArrow, Block* dwonArrow2) {

	//右
	rightArrow->position.x = 0;
	rightArrow->position.y = 0;
	rightArrow->Flag = false;

	rightArrow->LeftTop.x     = blockSize * 3;
	rightArrow->RightTop.x    = blockSize * 3 + (float)blockSize;
	rightArrow->LeftBottom.x  = blockSize * 3;
	rightArrow->RightBottom.x = blockSize * 3 + (float)blockSize;

	rightArrow->LeftTop.y     = blockSize * 7;
	rightArrow->RightTop.y    = blockSize * 7;
	rightArrow->LeftBottom.y  = blockSize * 7+ (float)blockSize;
	rightArrow->RightBottom.y = blockSize * 7+ (float)blockSize;

	//左
	leftArrow->position.x = 0;
	leftArrow->position.y = 0;
	leftArrow->Flag = false;

	leftArrow->LeftTop.x     = blockSize * 5;
	leftArrow->RightTop.x    = blockSize * 5 + (float)blockSize;
	leftArrow->LeftBottom.x  = blockSize * 5;
	leftArrow->RightBottom.x = blockSize * 5 + (float)blockSize;

	leftArrow->LeftTop.y     = blockSize * 7;
	leftArrow->RightTop.y    = blockSize * 7;
	leftArrow->LeftBottom.y  = blockSize * 7 + (float)blockSize;
	leftArrow->RightBottom.y = blockSize * 7 + (float)blockSize;

	//上
	upArrow->position.x = 0;
	upArrow->position.y = 0;
	upArrow->Flag = false;

	upArrow->LeftTop.x     = blockSize * 7;
	upArrow->RightTop.x    = blockSize * 7 + (float)blockSize;
	upArrow->LeftBottom.x  = blockSize * 7;
	upArrow->RightBottom.x = blockSize * 7 + (float)blockSize;

	upArrow->LeftTop.y      = blockSize * 7;
	upArrow->RightTop.y     = blockSize * 7;
	upArrow->LeftBottom.y   = blockSize * 7 + (float)blockSize;
	upArrow->RightBottom.y  = blockSize * 7 + (float)blockSize;

	//下
	dwonArrow->position.x = 0;
	dwonArrow->position.y = 0;
	dwonArrow->Flag = false;

	dwonArrow->LeftTop.x     = blockSize * 9;
	dwonArrow->RightTop.x    = blockSize * 9 + (float)blockSize;
	dwonArrow->LeftBottom.x  = blockSize * 9;
	dwonArrow->RightBottom.x = blockSize * 9 + (float)blockSize;

	dwonArrow->LeftTop.y     = blockSize * 7;
	dwonArrow->RightTop.y    = blockSize * 7;
	dwonArrow->LeftBottom.y  = blockSize * 7 + (float)blockSize;
	dwonArrow->RightBottom.y = blockSize * 7 + (float)blockSize;

	//下
	dwonArrow2->position.x = 0;
	dwonArrow2->position.y = 0;
	dwonArrow2->Flag = false;

	dwonArrow2->LeftTop.x = blockSize * 10;
	dwonArrow2->RightTop.x = blockSize * 10 + (float)blockSize;
	dwonArrow2->LeftBottom.x = blockSize * 10;
	dwonArrow2->RightBottom.x = blockSize * 10 + (float)blockSize;

	dwonArrow2->LeftTop.y = blockSize * 7;
	dwonArrow2->RightTop.y = blockSize * 7;
	dwonArrow2->LeftBottom.y = blockSize * 7 + (float)blockSize;
	dwonArrow2->RightBottom.y = blockSize * 7 + (float)blockSize;
	
}
//スピードを入れる
float speed(float speed, float position)//進ませる処理
{
	float newPosition = 0;

	position += speed;

	newPosition = position;
	return newPosition;
}
void keySpeed(Block* enemy,int BlockSize,float &speedX,float &speedY,int x,int y,bool& startFlag) {
	
	if (startFlag == false)
	{
		
		if (enemy->LeftTop.y <= BlockSize * x)
		{
			speedY *= -1;//0の場合は変数(Zeo)を使用。
		}
		if (enemy->LeftBottom.y >= BlockSize * y)
		{
			speedY *= -1;//0の場合は変数(Zeo)を使用。
		}

		if (enemy->LeftTop.x <= BlockSize * x)
		{
			speedX *= -1;//0の場合は変数(Zeo)を使用。
		}
		if (enemy->RightTop.x >= BlockSize * y)
		{
			speedX *= -1;//0の場合は変数(Zeo)を使用。
		}
		enemy->position.y += speedY;
		enemy->position.x += speedX;
	}
	if (startFlag == true)
	{
		speedX = 0;
		speedY = 0;
	}
}

void playerRum(Player* ghost,PlayerTile* TileGhost,float& ghostPosX,float& ghostPosY,int BlockSize)
{
	ghost->LeftTop.x     += ghost->position.x;
	ghost->LeftBottom.x  += ghost->position.x;
	ghost->RightTop.x    += ghost->position.x;
	ghost->RightBottom.x += ghost->position.x;
	ghost->LeftTop.y     += ghost->position.y;
	ghost->LeftBottom.y  += ghost->position.y;
	ghost->RightTop.y    += ghost->position.y;
	ghost->RightBottom.y += ghost->position.y;
	ghostPosX = (ghost->LeftTop.x + ghost->RightBottom.x) / 2;
	ghostPosY = (ghost->LeftTop.y + ghost->RightBottom.y) / 2;

	ghost->position.x = 0;
	ghost->position.y = 0;

	TileGhost->LeftTop.y = ghost->LeftTop.y / BlockSize;
	TileGhost->LeftTop.x = ghost->LeftTop.x / BlockSize;
	TileGhost->LeftBottom.y = ghost->LeftBottom.y / BlockSize;
	TileGhost->LeftBottom.x = ghost->LeftBottom.x / BlockSize;
	TileGhost->RightTop.y = ghost->RightTop.y / BlockSize;
	TileGhost->RightTop.x = ghost->RightTop.x / BlockSize;
	TileGhost->RightBottom.y = ghost->RightBottom.y / BlockSize;
	TileGhost->RightBottom.x = ghost->RightBottom.x / BlockSize;
}
void BlockRum(Block* player)
{
	player->LeftTop.x += player->position.x;
	player->LeftBottom.x += player->position.x;
	player->RightTop.x += player->position.x;
	player->RightBottom.x += player->position.x;
	player->LeftTop.y += player->position.y;
	player->LeftBottom.y += player->position.y;
	player->RightTop.y += player->position.y;
	player->RightBottom.y += player->position.y;
	
	player->position.x = 0;
	player->position.y = 0;
}
void playerReset(Player* player, int BlockSize,int playerSize,int ghostRadius, float PosX, float PosY,float speedX,float speedY) {

	player->position.x = 0;
	player->position.y = 0;
	player->LeftTop.x = ((float)BlockSize * PosX) + 10 + ghostRadius;
	player->LeftTop.y = ((float)BlockSize * PosY) + ghostRadius;
	player->RightTop.x = ((float)BlockSize * PosX) + (float)playerSize + 10 + ghostRadius;
	player->RightTop.y = ((float)BlockSize * PosY) + ghostRadius;
	player->LeftBottom.x = ((float)BlockSize * PosX) + 10 + ghostRadius;
	player->LeftBottom.y = ((float)blockSize * PosY) + (float)playerSize + ghostRadius;
	player->RightBottom.x = ((float)blockSize * PosX) + (float)playerSize + 10 + ghostRadius;
	player->RightBottom.y = ((float)blockSize * PosY) + (float)playerSize + ghostRadius;
	player->speed.x = speedX;
	player->speed.y = speedY;
}
void blockReset(Block* block,int BlockSize,float x,float y) {
	
	block->position.x = 0;
	block->position.y = 0;
	block->LeftTop.x      = ((float)BlockSize * x) - 1;
	block->LeftTop.y      = ((float)BlockSize * y) - 1;
	block->RightTop.x     = ((float)BlockSize * x) + (float)BlockSize - 1;
	block->RightTop.y     = ((float)BlockSize * y) - 1;
	block->LeftBottom.x   = ((float)BlockSize * x) - 1;
	block->LeftBottom.y   = ((float)blockSize * y) + (float)BlockSize - 1;
	block->RightBottom.x  = ((float)blockSize * x) + (float)BlockSize - 1;
	block->RightBottom.y  = ((float)blockSize * y) + (float)BlockSize - 1;
	block->Flag = false;
}

///==========================================================================
//当たり判定に関する関数
///==========================================================================

//マウスの当たり判定
void mouseCollision(Block* player, int mousePosX, int mousePosY, bool& mouseClickFlag)
{

	if (player->LeftTop.x + player->position.x <= mousePosX && mousePosX <= player->RightTop.x + player->position.x
		&& player->LeftTop.y + player->position.y <= mousePosY && mousePosY <= player->RightBottom.y + player->position.y
		&& mouseClickFlag == true)
	{
		player->Flag = true;
	}

}
void mouseGetCondition(Block* rightArrow, Block* leftArrow, Block* upArrow, Block* dwonArrow, Block* dwonArrow2,int& mousePosX,int& mousePosY,bool& mouseMoveFlag,bool& mouseClickFlag)
{
	if (rightArrow->Flag == true || leftArrow->Flag == true || upArrow->Flag == true || dwonArrow->Flag == true || dwonArrow2->Flag == true)
	{
		mouseMoveFlag = true;
	}

	if (mouseMoveFlag == true && mouseClickFlag == true)//動かせるようになったら
	{
		//右
		if (rightArrow->Flag == true && leftArrow->Flag == false && upArrow->Flag == false && dwonArrow->Flag == false && dwonArrow2->Flag == false)
		{
			rightArrow->position.x = ((float)mousePosX - blockSize * 3) - blockSize / 2;
			rightArrow->position.y = ((float)mousePosY - blockSize * 7) - blockSize / 2;
		}

		//左
		if (leftArrow->Flag == true && rightArrow->Flag == false && upArrow->Flag == false && dwonArrow->Flag == false && dwonArrow2->Flag == false)
		{
			leftArrow->position.x = ((float)mousePosX - blockSize * 5) - blockSize / 2;
			leftArrow->position.y = ((float)mousePosY - blockSize * 7) - blockSize / 2;
		}

		//上
		if (upArrow->Flag == true && rightArrow->Flag == false && leftArrow->Flag == false && dwonArrow->Flag == false && dwonArrow2->Flag == false)
		{
			upArrow->position.x = ((float)mousePosX - blockSize * 7) - blockSize / 2;
			upArrow->position.y = ((float)mousePosY - blockSize * 7) - blockSize / 2;
		}

		//下
		if (dwonArrow->Flag == true && rightArrow->Flag == false && leftArrow->Flag == false && upArrow->Flag == false && dwonArrow2->Flag == false)
		{
			dwonArrow->position.x = ((float)mousePosX - blockSize * 9) - blockSize / 2;
			dwonArrow->position.y = ((float)mousePosY - blockSize * 7) - blockSize / 2;
		}

		//下2
		if (dwonArrow2->Flag == true && rightArrow->Flag == false && leftArrow->Flag == false && upArrow->Flag == false && dwonArrow->Flag == false)
		{
			dwonArrow2->position.x = ((float)mousePosX - blockSize * 10) - blockSize / 2;
			dwonArrow2->position.y = ((float)mousePosY - blockSize * 7)  - blockSize / 2;
		}

	}

	if (mouseMoveFlag == true && mouseClickFlag == false)
	{
		mouseMoveFlag = false;
		rightArrow->Flag = false;
		leftArrow->Flag = false;
		upArrow->Flag = false;
		dwonArrow->Flag = false;
		dwonArrow2->Flag = false;
	}
}

//おばけと矢印の当たり判定(外面)
void RightArrowCollision(Player* player, Block Arrow, float ghostPosY, bool& mouseMoveFlag, float ArrowSpeedX, float ArrowSpeedY)
{
	if (Arrow.LeftTop.x + Arrow.position.x <= player->LeftTop.x && Arrow.RightTop.x + Arrow.position.x >= player->LeftTop.x)
	{
		if (Arrow.LeftTop.y + Arrow.position.y <= ghostPosY && ghostPosY <= Arrow.LeftBottom.y + Arrow.position.y)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				player->speed.x = ArrowSpeedX;
				player->speed.y = ArrowSpeedY;
			}
		}
	}
}
void LeftArrowCollision(Player* player, Block Arrow, float ghostPosY, bool& mouseMoveFlag, float ArrowSpeedX, float ArrowSpeedY)
{
	if (Arrow.LeftTop.x + Arrow.position.x <= player->RightTop.x && Arrow.RightTop.x + Arrow.position.x >= player->LeftTop.x)
	{
		if (Arrow.LeftTop.y + Arrow.position.y <= ghostPosY && ghostPosY <= Arrow.LeftBottom.y + Arrow.position.y)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				player->speed.x = ArrowSpeedX;
				player->speed.y = ArrowSpeedY;
			}
		}
	}
}
void UpArrowCollision(Player* player, Block Arrow, float ghostPosX, bool& mouseMoveFlag, float ArrowSpeedX, float ArrowSpeedY)
{
	if (Arrow.LeftTop.y + Arrow.position.y <= player->LeftBottom.y && player->LeftTop.y <= Arrow.LeftBottom.y + Arrow.position.y)
	{
		if (Arrow.LeftTop.x + Arrow.position.x <= ghostPosX && ghostPosX <= Arrow.RightTop.x + Arrow.position.x)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				player->speed.x = ArrowSpeedX;
				player->speed.y = ArrowSpeedY;
			}
		}
	}
}
void DwonArrowCollision(Player* player, Block Arrow, float ghostPosX, bool& mouseMoveFlag, float ArrowSpeedX, float ArrowSpeedY)
{
	if (Arrow.LeftTop.y + Arrow.position.y < player->LeftTop.y && player->LeftBottom.y <= Arrow.LeftBottom.y + Arrow.position.y)
	{
		if (Arrow.LeftBottom.x + Arrow.position.x < ghostPosX && ghostPosX < Arrow.RightBottom.x + Arrow.position.x)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				player->speed.x = ArrowSpeedX;
				player->speed.y = ArrowSpeedY;
			}
		}
	}
}

//壊れるブロックとの当たり判定
void RightBlockCollision(Player* player, Block Arrow, float ghostPosY, bool& mouseMoveFlag,bool& starSpeedStopFlag, float ArrowSpeedX, float ArrowSpeedY, float& hp, float& BlockTimer)
{
	if (Arrow.LeftTop.x + Arrow.position.x <= player->LeftTop.x + player->position.x && Arrow.RightTop.x + Arrow.position.x >= player->LeftTop.x + player->position.x)
	{
		if (Arrow.LeftTop.y + Arrow.position.y <= ghostPosY && ghostPosY <= Arrow.LeftBottom.y + Arrow.position.y)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{	
				
				if (2 < BlockTimer)
				{
					hp -= 1;
					starSpeedStopFlag = true;
					BlockTimer = 0;
				}
				player->speed.x = ArrowSpeedX;
				player->speed.y = ArrowSpeedY;
			}
		}
	}
}
void LeftBlockCollision(Player* player, Block Arrow, float ghostPosY, bool& mouseMoveFlag, bool& starSpeedStopFlag, float ArrowSpeedX, float ArrowSpeedY, float& hp, float& BlockTimer)
{
	if (Arrow.LeftTop.x + Arrow.position.x <= player->RightTop.x + player->position.x && Arrow.RightTop.x + Arrow.position.x >= player->LeftTop.x + player->position.x)
	{
		if (Arrow.LeftTop.y + Arrow.position.y <= ghostPosY && ghostPosY <= Arrow.LeftBottom.y + Arrow.position.y)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				if (2 < BlockTimer)
				{
					hp -= 1;
					starSpeedStopFlag = true;
					BlockTimer = 0;
				}
				player->speed.x = ArrowSpeedX;
				player->speed.y = ArrowSpeedY;
			}
		}
	}
}
void UpBlockCollision(Player* player, Block Arrow, float ghostPosX, bool& mouseMoveFlag, bool& starSpeedStopFlag, float ArrowSpeedX, float ArrowSpeedY, float& hp, float& BlockTimer)
{
	if (Arrow.LeftTop.y + Arrow.position.y <= player->LeftBottom.y + player->position.y && player->LeftTop.y + player->position.y <= Arrow.LeftBottom.y + Arrow.position.y)
	{
		if (Arrow.LeftTop.x + Arrow.position.x <= ghostPosX && ghostPosX <= Arrow.RightTop.x + Arrow.position.x)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				if (2 < BlockTimer)
				{
					hp -= 1;
					starSpeedStopFlag = true;
					BlockTimer = 0;
				}
				player->speed.x = ArrowSpeedX;
				player->speed.y = ArrowSpeedY;
			}
		}
	}
}
void DwonBlockCollision(Player* player, Block Arrow, float ghostPosX, bool& mouseMoveFlag, bool& starSpeedStopFlag, float ArrowSpeedX, float ArrowSpeedY, float& hp, float& BlockTimer)
{
	if (Arrow.LeftTop.y + Arrow.position.y < player->LeftTop.y + player->position.y && player->LeftBottom.y + player->position.y <= Arrow.LeftBottom.y + Arrow.position.y)
	{
		if (Arrow.LeftBottom.x + Arrow.position.x < ghostPosX && ghostPosX < Arrow.RightBottom.x + Arrow.position.x)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				if (2 < BlockTimer)
				{
					hp -= 1;
					starSpeedStopFlag = true;
					BlockTimer = 0;
				}
				player->speed.x = ArrowSpeedX;
				player->speed.y = ArrowSpeedY;
			}
		}
	}
}

//オンオフブロックとの当たり判定
void onOffRightBlockCollision(Player* player, Block Arrow, float ghostPosY, bool& mouseMoveFlag, bool& starSpeedStopFlag, float ArrowSpeedX, float ArrowSpeedY, bool& onOffFlag)
{
	if (Arrow.LeftTop.x + Arrow.position.x <= player->LeftTop.x + player->position.x && Arrow.RightTop.x + Arrow.position.x >= player->LeftTop.x + player->position.x)
	{
		if (Arrow.LeftTop.y + Arrow.position.y <= ghostPosY && ghostPosY <= Arrow.LeftBottom.y + Arrow.position.y)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				if (onOffFlag == false)
				{
					starSpeedStopFlag = true;
					player->speed.x = ArrowSpeedX;
					player->speed.y = ArrowSpeedY;
				}
			}
		}
	}
}
void onOffLeftBlockCollision (Player* player, Block Arrow, float ghostPosY, bool& mouseMoveFlag, bool& starSpeedStopFlag, float ArrowSpeedX, float ArrowSpeedY, bool& onOffFlag)
{
	if (Arrow.LeftTop.x + Arrow.position.x <= player->RightTop.x + player->position.x && Arrow.RightTop.x + Arrow.position.x >= player->LeftTop.x + player->position.x)
	{
		if (Arrow.LeftTop.y + Arrow.position.y <= ghostPosY && ghostPosY <= Arrow.LeftBottom.y + Arrow.position.y)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				if (onOffFlag == false)
				{
					starSpeedStopFlag = true;
					player->speed.x = ArrowSpeedX;
					player->speed.y = ArrowSpeedY;
				}
			}
		}
	}
}
void onOffUpBlockCollision   (Player* player, Block Arrow, float ghostPosX, bool& mouseMoveFlag, bool& starSpeedStopFlag, float ArrowSpeedX, float ArrowSpeedY, bool& onOffFlag)
{
	if (Arrow.LeftTop.y + Arrow.position.y <= player->LeftBottom.y + player->position.y && player->LeftTop.y + player->position.y <= Arrow.LeftBottom.y + Arrow.position.y)
	{
		if (Arrow.LeftTop.x + Arrow.position.x <= ghostPosX && ghostPosX <= Arrow.RightTop.x + Arrow.position.x)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				if (onOffFlag == false)
				{
					starSpeedStopFlag = true;
					player->speed.x = ArrowSpeedX;
					player->speed.y = ArrowSpeedY;
				}
			}
		}
	}
}
void onOffDwonBlockCollision (Player* player, Block Arrow, float ghostPosX, bool& mouseMoveFlag, bool& starSpeedStopFlag, float ArrowSpeedX, float ArrowSpeedY, bool& onOffFlag)
{
	if (Arrow.LeftTop.y + Arrow.position.y < player->LeftTop.y + player->position.y && player->LeftBottom.y + player->position.y <= Arrow.LeftBottom.y + Arrow.position.y)
	{
		if (Arrow.LeftBottom.x + Arrow.position.x < ghostPosX && ghostPosX < Arrow.RightBottom.x + Arrow.position.x)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				if (onOffFlag == false)
				{
					starSpeedStopFlag = true;
					player->speed.x = ArrowSpeedX;
					player->speed.y = ArrowSpeedY;
				}
			}
		}
	}
}

//スイッチブロックとの当たり判定
void switchRightBlockCollision(Player* player, Block Arrow, float ghostPosY, bool& mouseMoveFlag,bool& onOffFlag)
{
	if (Arrow.LeftTop.x + Arrow.position.x <= player->LeftTop.x && Arrow.RightTop.x + Arrow.position.x >= player->LeftTop.x)
	{
		if (Arrow.LeftTop.y + Arrow.position.y <= ghostPosY && ghostPosY <= Arrow.LeftBottom.y + Arrow.position.y)
		{
			if (mouseMoveFlag == true)
			{
				onOffFlag = true;
			}
		}
	}
}
void switchLeftBlockCollision( Player* player, Block Arrow, float ghostPosY, bool& mouseMoveFlag,bool& onOffFlag)
{
	if (Arrow.LeftTop.x + Arrow.position.x <= player->RightTop.x && Arrow.RightTop.x + Arrow.position.x >= player->LeftTop.x)
	{
		if (Arrow.LeftTop.y + Arrow.position.y <= ghostPosY && ghostPosY <= Arrow.LeftBottom.y + Arrow.position.y)
		{
			if (mouseMoveFlag == true)
			{
				onOffFlag = true;
			}
		}

	}
}
void switchUpBlockCollision(   Player* player, Block Arrow, float ghostPosX, bool& mouseMoveFlag,bool& onOffFlag)
{
	if (Arrow.LeftTop.y + Arrow.position.y <= player->LeftBottom.y && player->LeftTop.y <= Arrow.LeftBottom.y + Arrow.position.y)
	{
		if (Arrow.LeftTop.x + Arrow.position.x <= ghostPosX && ghostPosX <= Arrow.RightTop.x + Arrow.position.x)
		{
			if (mouseMoveFlag == true)
			{
				onOffFlag = true;
			}
		}
	}

}
void switchDwonBlockCollision( Player* player, Block Arrow, float ghostPosX, bool& mouseMoveFlag,bool& onOffFlag)
{
	if (Arrow.LeftTop.y + Arrow.position.y < player->LeftTop.y && player->LeftBottom.y <= Arrow.LeftBottom.y + Arrow.position.y)
	{
		if (Arrow.LeftBottom.x + Arrow.position.x < ghostPosX && ghostPosX < Arrow.RightBottom.x + Arrow.position.x)
		{
			mouseMoveFlag = true;
			if (mouseMoveFlag == true)
			{
				onOffFlag = true;
			}
		}
	}
}

//鍵との当たり判定
void RightKeyCollision(Player* player, Block Arrow, float ghostPosY, bool& mouseMoveFlag, bool& Flag)
{
	if (Arrow.LeftTop.x + Arrow.position.x <= player->LeftTop.x + player->position.x && Arrow.RightTop.x + Arrow.position.x >= player->LeftTop.x + player->position.x)
	{
		if (Arrow.LeftTop.y + Arrow.position.y <= ghostPosY && ghostPosY <= Arrow.LeftBottom.y + Arrow.position.y)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				Flag = false;
			}
		}
	}
}
void LeftKeyCollision(Player* player, Block Arrow, float ghostPosY, bool& mouseMoveFlag, bool& Flag)
{
	if (Arrow.LeftTop.x + Arrow.position.x <= player->RightTop.x + player->position.x && Arrow.RightTop.x + Arrow.position.x >= player->LeftTop.x + player->position.x)
	{
		if (Arrow.LeftTop.y + Arrow.position.y <= ghostPosY && ghostPosY <= Arrow.LeftBottom.y + Arrow.position.y)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				Flag = false;
			}
		}
	}
}
void UpKeyCollision(Player* player, Block Arrow, float ghostPosX, bool& mouseMoveFlag,   bool& Flag)
{
	if (Arrow.LeftTop.y + Arrow.position.y <= player->LeftBottom.y + player->position.y && player->LeftTop.y + player->position.y <= Arrow.LeftBottom.y + Arrow.position.y)
	{
		if (Arrow.LeftTop.x + Arrow.position.x <= ghostPosX && ghostPosX <= Arrow.RightTop.x + Arrow.position.x)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				Flag = false;
			}
		}
	}
}
void DwonKeyCollision(Player* player, Block Arrow, float ghostPosX, bool& mouseMoveFlag, bool& Flag)
{
	if (Arrow.LeftTop.y + Arrow.position.y < player->LeftTop.y + player->position.y && player->LeftBottom.y + player->position.y <= Arrow.LeftBottom.y + Arrow.position.y)
	{
		if (Arrow.LeftBottom.x + Arrow.position.x < ghostPosX && ghostPosX < Arrow.RightBottom.x + Arrow.position.x)
		{
			if (mouseMoveFlag == false)//矢印を持っていなかったら
			{
				Flag = false;
			}
		}
	}
}



//マップチップの当たり判定(ブロック)
void MapChipCondition(Player* ghost,PlayerTile* TileGhost, int stageMap[][13], int& touchNumber,float& touchNumberTimer,bool& startSpeedStopFlag)
{
	//左
	if (stageMap[(int)TileGhost->LeftTop.y][(int)TileGhost->LeftTop.x] == 11 && stageMap[(int)TileGhost->LeftBottom.y][(int)TileGhost->LeftBottom.x] == 11)
	{
		if (2 < touchNumberTimer)
		{
			touchNumber -= 1;
			touchNumberTimer = 0;
		}
		ghost->speed.x = -10;
		ghost->speed.y = 0;
		startSpeedStopFlag = true;
	}

	//右
	if (stageMap[(int)TileGhost->RightTop.y][(int)TileGhost->RightTop.x] == 11 && stageMap[(int)TileGhost->RightBottom.y][(int)TileGhost->RightBottom.x] == 11)
	{
		if (2 < touchNumberTimer)
		{
			touchNumber -= 1;
			touchNumberTimer = 0;
		}
		ghost->speed.x = 10;
		ghost->speed.y = 0;
		startSpeedStopFlag = true;
	}

	//上
	if (stageMap[(int)TileGhost->RightTop.y][(int)TileGhost->RightTop.x] == 11 && stageMap[(int)TileGhost->LeftTop.y][(int)TileGhost->LeftTop.x] == 11)
	{
		if (2 < touchNumberTimer)
		{
			touchNumber -= 1;
			touchNumberTimer = 0;
		}
		ghost->speed.y = 10;
		ghost->speed.x = 0;
		startSpeedStopFlag = true;
	}

	//下
	if (stageMap[(int)TileGhost->RightBottom.y][(int)TileGhost->RightBottom.x] == 11 && stageMap[(int)TileGhost->LeftBottom.y][(int)TileGhost->LeftBottom.x] == 11)
	{
		if (2 < touchNumberTimer)
		{
			touchNumber -= 1;
			touchNumberTimer = 0;
		}
		ghost->speed.y = -10;
		ghost->speed.x = 0;
		startSpeedStopFlag = true;
	}

}

//中身を判定にした当たり判定
void allCollision(Player* player, Block* Arrow, bool& mouseMoveFlag, float ArrowSpeedX, float ArrowSpeedY, bool& StartFlag)
{
	if (Arrow->LeftTop.x + Arrow->position.x< player->LeftTop.x + player->position.x - 12 && Arrow->RightTop.x   + Arrow->position.x> player->RightTop.x   + player->position.x + 12 &&
		Arrow->LeftTop.y + Arrow->position.y< player->LeftTop.y + player->position.y - 12 && Arrow->LeftBottom.y + Arrow->position.y> player->LeftBottom.y + player->position.y + 12)
	{
		if (mouseMoveFlag == false)//矢印を持っていなかったら
		{
			player->speed.x = ArrowSpeedX;
			player->speed.y = ArrowSpeedY;
			
		}
		StartFlag = true;
	}
}
void allCollisionFlag(Player* player, Block Arrow, bool& mouseMoveFlag,bool& Flag)
{
	if (Arrow.LeftTop.x + Arrow.position.x< player->LeftTop.x && Arrow.RightTop.x + Arrow.position.x > player->RightTop.x &&
		Arrow.LeftTop.y + Arrow.position.y < player->LeftTop.y && Arrow.LeftBottom.y + Arrow.position.y > player->LeftBottom.y)
	{
		if (mouseMoveFlag == false)//矢印を持っていなかったら
		{
			Flag = true;
		}
	}
}

///==========================================================================
//その他関数
///==========================================================================

//シーンの切り替え
void SceneChange(int Timer, int HalfTimer, int ChangeScene)
{
	if (Timer > HalfTimer) {
		Scene = ChangeScene;
	}

	Timer = 0;
}

//SE再生
void SeFlow(SE* se, int Handle, int SEHandle, float SEVolume)
{
	if (se->Flag == true)//trueになったら流す
	{
		se->Timer++;
		if (se->Timer <= se->EndTimer)
		{
			if (!Novice::IsPlayingAudio(Handle) || Handle == -1)
			{
				Handle = Novice::PlayAudio(SEHandle, false, SEVolume);//SEを流す
			}
		}
		else
		{
			se->Timer = 0;
			se->Flag = false;
		}
	}
	else
	{
		Novice::StopAudio(Handle);//SEを停止
	}
}
float VolumeCount(float VolumeCount, float Volume, const float suuzi, const float isVolume)
{
	float NewVolume = 0;
	if (VolumeCount == suuzi)
	{
		Volume = 0.0f;
		Volume += isVolume;
	}
	NewVolume = Volume;
	return NewVolume;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	float kWindowWidth = 1664; // ウィンドウの横幅
	float kWindowHeight = 1024; // ウィンドウの縦幅

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, (int)kWindowWidth, (int)kWindowHeight);

	
	const int playerSize = 64;
	const int mapX = 13;
	const int mapY = 8;

	int Stage1Map[mapY][mapX] =
	{
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,99,10,10,10,12,10,10,10,10,10,10,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
	};

	int Stage2Map[mapY][mapX] =
	{
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,10,10,10,10,10,10,10,10,10,10,10,11},
		{11,10,11,11,11,11,11,11,11,11,11,11,11},
		{11,10,11,11,11,11,11,11,11,11,11,11,11},
		{11,99,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
	};

	int Stage3Map[mapY][mapX] =
	{
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,10,10,10,10,11,11,11,11},
		{11,11,11,11,11,10,11,11,10,11,11,11,11},
		{11,11,11,11,11,10,11,99,10,11,11,11,11},
		{11,10,10,10,10,10,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
	};

	int Stage4Map[mapY][mapX] =
	{
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,10,10,10,10,10,11,11,11,11,11,11,11},
		{11,10,11,11,11,11,11,11,11,11,11,11,11},
		{11,10,10,10,10,10,10,10,10,11,11,11,11},
		{11,10,11,11,11,11,10,11,10,11,99,11,11},
		{11,10,11,99,11,11,10,11,10,11,10,11,11},
		{11,10,11,10,10,10,10,11,10,10,10,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
	};

	int Stage5Map[mapY][mapX] =
	{
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,10,10,10,10,10,10,10,10,11,11},
		{11,10,10,10,11,10,11,11,10,11,10,11,11},
		{11,10,11,10,11,10,10,11,10,11,10,11,11},
		{11,10,11,10,11,11,11,11,10,11,10,11,11},
		{11,10,11,10,10,10,10,11,10,11,10,11,11},
		{11,99,11,10,10,10,10,12,10,10,10,99,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
	};

	int Stage6Map[mapY][mapX] =
	{
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,10,10,10,10,13,10,14,10,10,10,99,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
	};

	int Stage7Map[mapY][mapX] =
	{
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
        {11,11,11,99,10,10,10,10,10,10,10,10,11},
        {11,11,99,11,11,10,11,11,11,10,11,10,11},
        {11,11,10,11,11,10,11,11,11,10,11,10,11},
        {11,11,10,10,10,10,10,10,10,10,10,10,11},
        {11,11,10,11,10,10,11,11,10,11,10,11,11},
        {11,11,10,10,10,10,10,10,10,11,10,99,11},
        {11,11,11,11,11,11,11,11,11,11,11,11,11},
	};

	int Stage8Map[mapY][mapX] =
	{
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,10,10,10,10,10,10,10,10,10,10,10,11},
		{11,11,11,10,11,11,11,11,11,11,11,10,11},
		{11,11,11,10,11,11,11,11,11,11,11,10,11},
		{11,11,11,10,11,11,11,11,11,11,11,99,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
	};

	int Stage9Map[mapY][mapX] =
	{
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,11,11,99,10,10,10,10,10,10,10,10,11},
		{11,11,11,10,11,11,11,10,11,11,11,10,11},
		{11,10,10,10,11,11,11,10,11,99,10,10,11},
		{11,11,11,10,11,11,11,10,11,11,11,11,11},
		{11,11,11,10,10,10,10,10,11,11,11,11,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
	};

	int Stage10Map[mapY][mapX] =
	{
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
		{11,10,10,10,10,10,10,10,10,10,11,11,11},
		{11,10,11,10,11,11,10,11,11,10,10,10,11},
		{11,10,11,10,10,11,10,11,11,11,11,10,11},
		{11,10,11,11,10,11,10,11,11,11,11,10,11},
		{11,10,10,10,10,10,10,10,10,10,11,99,11},
		{11,10,10,10,10,10,10,10,10,10,10,10,11},
		{11,11,11,11,11,11,11,11,11,11,11,11,11},
	};

	//プレイヤー画像

	Player player{//ニシト
		{0,0},
		{0,0},
		{0 + (float)blockSize,0},
		{0,0 + (float)blockSize},
		{0 + (float)blockSize,0 + (float)blockSize},
		{0.0f,0.0f},
	};

	float clearNisitoPosX = kWindowWidth;
	float clearNisitoPosY = kWindowHeight - blockSize;

	int ghostRadius = 32;//おばけの半径

	Player ghost{//おばけ
		{0,0},
		{((float)blockSize * 11) + 10 + ghostRadius                     ,(float)blockSize * 3 + ghostRadius},
		{((float)blockSize * 11) + (float)playerSize + 10 + ghostRadius ,(float)blockSize * 3 + ghostRadius},
		{((float)blockSize * 11) + 10 + ghostRadius                     ,((float)blockSize * 3) + (float)playerSize + ghostRadius},
		{((float)blockSize * 11) + (float)playerSize + 10 + ghostRadius ,((float)blockSize * 3) + (float)playerSize + ghostRadius},
		{10.0f,0.0f},
	};

	PlayerTile TileGhost{
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
	};


	float ghostPosX = 0;//中心X
	float ghostPosY = 0;//中心Y

	//クリア時のプレイヤー
	float clearGhostPosX = kWindowWidth;
	float clearGhostPosY = kWindowHeight - blockSize;
	float clearGhostSpeed = 8;

	//マップチップブロック

	Block setting{//設定
		{0,0},
		{kWindowWidth - blockSize,0},
		{(kWindowWidth - blockSize) + (float)blockSize,0},
		{kWindowWidth - blockSize,0 + (float)blockSize},
		{(kWindowWidth - blockSize) + (float)blockSize,0 + (float)blockSize},
		{false},
	};

	Block turnBack{//戻る
		{0,0},
		{0,0},
		{0 + (float)blockSize,0},
		{0 ,0 + (float)blockSize},
		{0 + (float)blockSize,0 + (float)blockSize},
		{false},
	};

	//ギミック画像

	Block rightArrow{//右向き矢印→
		{0,0},
		{blockSize * 3                   ,blockSize * 7},
		{blockSize * 3 + (float)blockSize,blockSize * 7},
		{blockSize * 3                   ,blockSize * 7 + (float)blockSize},
		{blockSize * 3 + (float)blockSize,blockSize * 7 + (float)blockSize},
		{false},
	};

	Block leftArrow{//左向き矢印←
		{0,0},
		{blockSize * 5                   ,blockSize * 7},
		{blockSize * 5 + (float)blockSize,blockSize * 7},
		{blockSize * 5                   ,blockSize * 7 + (float)blockSize},
		{blockSize * 5 + (float)blockSize,blockSize * 7 + (float)blockSize},
		{false},
	};

	Block upArrow{//上向き矢印↑
		{0,0},
		{blockSize * 7                   ,blockSize * 7},
		{blockSize * 7 + (float)blockSize,blockSize * 7},
		{blockSize * 7                   ,blockSize * 7 + (float)blockSize},
		{blockSize * 7 + (float)blockSize,blockSize * 7 + (float)blockSize},
		{false},
	};

	Block dwonArrow{//下向き矢印↓
		{0,0},
		{blockSize * 9                   ,blockSize * 7},
		{blockSize * 9 + (float)blockSize,blockSize * 7},
		{blockSize * 9                   ,blockSize * 7 + (float)blockSize},
		{blockSize * 9 + (float)blockSize,blockSize * 7 + (float)blockSize},
		{false},
	};

	Block dwonArrow2{//下向き矢印↓
		{0,0},
		{blockSize * 9                   ,blockSize * 7},
		{blockSize * 9 + (float)blockSize,blockSize * 7},
		{blockSize * 9                   ,blockSize * 7 + (float)blockSize},
		{blockSize * 9 + (float)blockSize,blockSize * 7 + (float)blockSize},
		{false},
	};

	Block breakableBlocks{//壊せるブロック
		{0,0},
		{blockSize * 3                   ,blockSize * 3},
		{blockSize * 3 + (float)blockSize,blockSize * 3},
		{blockSize * 3                   ,blockSize * 3 + (float)blockSize},
		{blockSize * 3 + (float)blockSize,blockSize * 3 + (float)blockSize},
		{false},
	};

	float BlockHp = 3.0f;
	float BlockTimer = 0.0f;

	Block onOffBlock{
		{0,0},
		{blockSize * 7                   ,blockSize * 4},
		{blockSize * 7 + (float)blockSize,blockSize * 4},
		{blockSize * 7                   ,blockSize * 4 + (float)blockSize},
		{blockSize * 7 + (float)blockSize,blockSize * 4 + (float)blockSize},
		{false},
	};

	Block onOffBlock2{
		{0,0},
		{blockSize * 7                   ,blockSize * 4},
		{blockSize * 7 + (float)blockSize,blockSize * 4},
		{blockSize * 7                   ,blockSize * 4 + (float)blockSize},
		{blockSize * 7 + (float)blockSize,blockSize * 4 + (float)blockSize},
		{false},
	};

	float onOffTimer = 0.0f;

	Block switchBlock{
		{0,0},
		{blockSize * 4                   ,blockSize * 4},
		{blockSize * 4 + (float)blockSize,blockSize * 4},
		{blockSize * 4                   ,blockSize * 4 + (float)blockSize},
		{blockSize * 4 + (float)blockSize,blockSize * 4 + (float)blockSize},
		{false},
	};

	Block key{
		{0,0},
		{blockSize * 4                   ,blockSize * 4},
		{blockSize * 4 + (float)blockSize,blockSize * 4},
		{blockSize * 4                   ,blockSize * 4 + (float)blockSize},
		{blockSize * 4 + (float)blockSize,blockSize * 4 + (float)blockSize},
		{false},
	};

	bool KeyFlag = false;
	float keySpeedX = 10.0f;
	float keySpeedY = 10.0f;

	//ステージ選択

	Block stageOne{//ステージ1の画像
		{0,0},
		{blockSize * 2                   ,kWindowHeight - blockSize * 6},
		{blockSize * 2 + (float)blockSize,kWindowHeight - blockSize * 6},
		{blockSize * 2                   ,kWindowHeight - blockSize * 6 + (float)blockSize},
		{blockSize * 2 + (float)blockSize,kWindowHeight - blockSize * 6 + (float)blockSize},
		{false},
	};

	Block stageTwo{//ステージ2の画像
		{0,0},
		{blockSize * 4                   ,kWindowHeight - blockSize * 6},
		{blockSize * 4 + (float)blockSize,kWindowHeight - blockSize * 6},
		{blockSize * 4                   ,kWindowHeight - blockSize * 6 + (float)blockSize},
		{blockSize * 4 + (float)blockSize,kWindowHeight - blockSize * 6 + (float)blockSize},
		{false},
	};

	Block stageThree{//ステージ3の画像
		{0,0},
		{blockSize * 6                   ,kWindowHeight - blockSize * 6},
		{blockSize * 6 + (float)blockSize,kWindowHeight - blockSize * 6},
		{blockSize * 6                   ,kWindowHeight - blockSize * 6 + (float)blockSize},
		{blockSize * 6 + (float)blockSize,kWindowHeight - blockSize * 6 + (float)blockSize},
		{false},
	};

	Block stageFour{//ステージ4の画像
		{0,0},
		{blockSize * 8                   ,kWindowHeight - blockSize * 6},
		{blockSize * 8 + (float)blockSize,kWindowHeight - blockSize * 6},
		{blockSize * 8                   ,kWindowHeight - blockSize * 6 + (float)blockSize},
		{blockSize * 8 + (float)blockSize,kWindowHeight - blockSize * 6 + (float)blockSize},
		{false},
	};

	Block stageFive{//ステージ5の画像
		{0,0},
		{blockSize * 10                   ,kWindowHeight - blockSize * 6},
		{blockSize * 10 + (float)blockSize,kWindowHeight - blockSize * 6},
		{blockSize * 10                   ,kWindowHeight - blockSize * 6 + (float)blockSize},
		{blockSize * 10 + (float)blockSize,kWindowHeight - blockSize * 6 + (float)blockSize},
		{false},
	};

	Block stageSix{//ステージ6の画像
		{0,0},
		{blockSize * 2                   ,kWindowHeight - blockSize * 4},
		{blockSize * 2 + (float)blockSize,kWindowHeight - blockSize * 4},
		{blockSize * 2                   ,kWindowHeight - blockSize * 4 + (float)blockSize},
		{blockSize * 2 + (float)blockSize,kWindowHeight - blockSize * 4 + (float)blockSize},
		{false},
	};

	Block stageSeven{//ステージ7の画像
		{0,0},
		{blockSize * 4                   ,kWindowHeight - blockSize * 4},
		{blockSize * 4 + (float)blockSize,kWindowHeight - blockSize * 4},
		{blockSize * 4                   ,kWindowHeight - blockSize * 4 + (float)blockSize},
		{blockSize * 4 + (float)blockSize,kWindowHeight - blockSize * 4 + (float)blockSize},
		{false},
	};

	Block stageEight{//ステージ8の画像
		{0, 0},
		{ blockSize * 6                   ,kWindowHeight - blockSize * 4 },
		{ blockSize * 6 + (float)blockSize,kWindowHeight - blockSize * 4 },
		{ blockSize * 6                   ,kWindowHeight - blockSize * 4 + (float)blockSize },
		{ blockSize * 6 + (float)blockSize,kWindowHeight - blockSize * 4 + (float)blockSize },
		{ false },
	};

	Block stageNine{//ステージ9の画像
		{0, 0},
		{ blockSize * 8                   ,kWindowHeight - blockSize * 4 },
		{ blockSize * 8 + (float)blockSize,kWindowHeight - blockSize * 4 },
		{ blockSize * 8                   ,kWindowHeight - blockSize * 4 + (float)blockSize },
		{ blockSize * 8 + (float)blockSize,kWindowHeight - blockSize * 4 + (float)blockSize },
		{ false },
	};



	Block stageTen{//ステージ10の画像
		{0, 0},
		{ blockSize * 10                   ,kWindowHeight - blockSize * 4 },
		{ blockSize * 10 + (float)blockSize,kWindowHeight - blockSize * 4 },
		{ blockSize * 10                   ,kWindowHeight - blockSize * 4 + (float)blockSize },
		{ blockSize * 10 + (float)blockSize,kWindowHeight - blockSize * 4 + (float)blockSize },
		{ false },
	};
	//押せるボタン

	Block gameStartKey{//スタートキーの画像
		{0,0},
		{blockSize * 6                   ,kWindowHeight - blockSize * 2},
		{blockSize * 6 + (float)blockSize,kWindowHeight - blockSize * 2},
		{blockSize * 6                   ,kWindowHeight - blockSize * 2 + (float)blockSize},
		{blockSize * 6 + (float)blockSize,kWindowHeight - blockSize * 2 + (float)blockSize},
		{false},
	};

	Block StartKey{//スタートキーの画像
		{0,0},
		{blockSize * 6                   ,kWindowHeight - blockSize * 1},
		{blockSize * 6 + (float)blockSize,kWindowHeight - blockSize * 1},
		{blockSize * 6                   ,kWindowHeight - blockSize * 1 + (float)blockSize},
		{blockSize * 6 + (float)blockSize,kWindowHeight - blockSize * 1 + (float)blockSize},
		{false},
	};

	Block nextStage{//(次のステージ)の画像
		{0,0},
		{kWindowWidth - blockSize -100                   ,kWindowHeight - blockSize * 2},
		{kWindowWidth - blockSize -100 + (float)blockSize,kWindowHeight - blockSize * 2},
		{kWindowWidth - blockSize -100                   ,kWindowHeight - blockSize * 2 + (float)blockSize},
		{kWindowWidth - blockSize -100 + (float)blockSize,kWindowHeight - blockSize * 2 + (float)blockSize},
		{false},
	};

	Block title{//(タイトルへ)の画像
		{0,0},
		{kWindowWidth - blockSize -100                   ,kWindowHeight - blockSize * 3 - 10},
		{kWindowWidth - blockSize -100 + (float)blockSize,kWindowHeight - blockSize * 3 - 10},
		{kWindowWidth - blockSize -100                   ,kWindowHeight - blockSize * 3 - 10 + (float)blockSize},
		{kWindowWidth - blockSize -100 + (float)blockSize,kWindowHeight - blockSize * 3 - 10 + (float)blockSize},
		{false},
	};

	Block retry{//やり直しの画像
		{0,0},
		{kWindowWidth - blockSize - 10                   ,0},
		{kWindowWidth - blockSize - 10 + (float)blockSize,0},
		{kWindowWidth - blockSize - 10                   ,0 + (float)blockSize},
		{kWindowWidth - blockSize - 10 + (float)blockSize,0 + (float)blockSize},
		{false},
	};

	Block retryKey{//やり直しの画像
		{0,0},
		{kWindowWidth - blockSize - 100                   ,kWindowHeight - blockSize * 2},
		{kWindowWidth - blockSize - 100 + (float)blockSize,kWindowHeight - blockSize * 2},
		{kWindowWidth - blockSize - 100                   ,kWindowHeight - blockSize * 2 + (float)blockSize},
		{kWindowWidth - blockSize - 100 + (float)blockSize,kWindowHeight - blockSize * 2 + (float)blockSize},
		{false},
	};

	Block BGMdownVolume{//BGMのアップボリュームの画像
		{0,0},
		{672                   ,352},
		{672 + (float)blockSize,352},
		{672                   ,352 + (float)blockSize},
		{672 + (float)blockSize,352 + (float)blockSize},
		{false},
	};

	Block BGMupVolume{//BGMのアップボリュームの画像
		{0,0},
		{1216                   ,352},
		{1216 + (float)blockSize,352},
		{1216                   ,352 + (float)blockSize},
		{1216 + (float)blockSize,352 + (float)blockSize},
		{false},
	};

	Block SEdownVolume{//SEのアップボリュームの画像
		{0,0},
		{672                   ,535},
		{672 + (float)blockSize,535},
		{672                   ,535 + (float)blockSize},
		{672 + (float)blockSize,535 + (float)blockSize},
		{false},
	};

	Block SEupVolume{//SEのアップボリュームの画像
		{0,0},
		{1216                   ,535},
		{1216 + (float)blockSize,535},
		{1216                   ,535 + (float)blockSize},
		{1216 + (float)blockSize,535 + (float)blockSize},
		{false},
	};

	float spped_0 = 0.0f;
	float arrowSpeedX = 10.0f;
	float arrowSpeedY = 10.0f;


	//壁に触れた数
	int touchNumber = 0;
	float touchNumberTimer = 0.0f;

	//マウス
	int mousePosX = 0;
	int mousePosY = 0;
	int mouseConutTimer = 0;//クリックした時間
	bool mouseClickFlag = false;//クリックしたか
	bool mouseMoveFlag = false;//マウスが動いているか

	//パーティクル
	const int MAX = 300;
	unsigned int color[MAX] = { 0xFFFFFFFF };

	int particlePosX[MAX];
	int particlePosY[MAX];
	int particleSpeedX[MAX];
	int particleSpeedY[MAX];

	bool isShot[MAX] = {};


	//乱数
	int randMax = blockSize / 2;
	int randMin = randMax / 2;

	//タイトルの画像
	float titlePosY = -kWindowHeight;

	int titlePlayerPosX = (int)kWindowWidth;
	int titleGhostPosX = 1200;
	int titleGhostPosY = 600;

	int titleSpeed = 0;

	//数字
	float Zeo   = 0.0f;
	float One   = 1.0f;
	float Two   = 2.0f;
	float Three = 3.0f;
	float Four  = 4.0f;
	float Five  = 5.0f;

	///==========================================================================
	//画像の変数
	///==========================================================================

	//プレイヤー
	int playerImges = Novice::LoadTexture("./Resources/images/player/nisito.png");//ニシト
	int GhostImges = Novice::LoadTexture("./Resources/images/player/Ghost.png");//おばけ
	int gameOverGhostImges = Novice::LoadTexture("./Resources/images/player/gameOverGhost.png");//ゲームオーバー時のおばけ

	//ギミック画像
	int rightArrowImges = Novice::LoadTexture("./Resources/images/gimmick/rightArrow.png");//右方向
	int leftArrowImges = Novice::LoadTexture ("./Resources/images/gimmick/leftArrow.png");//左矢印
	int upArrowImges = Novice::LoadTexture   ("./Resources/images/gimmick/upArrow.png");//上矢印
	int dwonArrowImges = Novice::LoadTexture("./Resources/images/gimmick/dwonArrow.png");//下矢印

	int breakableBlocksImges = Novice::LoadTexture("./Resources/images/gimmick/breakableBlocks.png");//壊せるブロック HP3
	int breakableBlocks2Imges = Novice::LoadTexture("./Resources/images/gimmick/breakableBlocks2.png");//壊せるブロック HP2
	int breakableBlocks3Imges = Novice::LoadTexture("./Resources/images/gimmick/breakableBlocks3.png");//壊せるブロック HP1

	int onBlockImges = Novice::LoadTexture("./Resources/images/gimmick/onBlock.png");//onブロック
	int offBlockImges = Novice::LoadTexture("./Resources/images/gimmick/offBlock.png");//offブロック
	int switchOnImges = Novice::LoadTexture("./Resources/images/gimmick/switchOn.png");//スイッチオン
	int switchOffImges = Novice::LoadTexture("./Resources/images/gimmick/switchOff.png");//スイッチオフ

	int keyImges = Novice::LoadTexture("./Resources/images/gimmick/Key.png");//キー

	//マップチップの画像
	//int blockImges = Novice::LoadTexture("./Resources/images/mapChip/block.png");//ブロック
	int GraveImges = Novice::LoadTexture("./Resources/images/mapChip/Grave.png");//Grave
	int rockGoalImges = Novice::LoadTexture("./Resources/images/mapChip/rockGoal.png");//城
	int goalImges = Novice::LoadTexture("./Resources/images/mapChip/goal.png");//ゴール

	//背景
	int backgroundImges = Novice::LoadTexture("./Resources/images/background/background.png");//背景
	int titleBackgroundImges = Novice::LoadTexture("./Resources/images/background/titleBackground.png");//タイトル背景
	int titleImges = Novice::LoadTexture("./Resources/images/background/title.png");//タイトル
	int mouseImges = Novice::LoadTexture("./Resources/images/background/mouse.png");//タイトルのマウス

	//int makingImges = Novice::LoadTexture("./Resources/images/background/making.png");//作成中

	//文字背景
	
	//タイトル
	int attentionImges = Novice::LoadTexture("./Resources/images/mozi/attention.png");//タイトル前注意

	//設定
	int settingBackgroundImges = Novice::LoadTexture("./Resources/images/background/settingBackground.png");//設定

	//ステージセレクト
	int stageSelectImges = Novice::LoadTexture("./Resources/images/mozi/stageSelect.png");//ステージセレクト

	//ステージ
	int stageClearImges = Novice::LoadTexture("./Resources/images/mozi/StageClear.png");//ステージクリア
	//int GameOverImges = Novice::LoadTexture("./Resources/images/mozi/GameOver.png");//ゲームオーバー
	int residueImges = Novice::LoadTexture("./Resources/images/mozi/residue.png");//残り
	int stageNumbertImges = Novice::LoadTexture("./Resources/images/mozi/stageNumbert.png");//ステージ〇

	//パーティクル画像
	//int candyImges = Novice::LoadTexture("./Resources/images/particle/candy.png");//キャンディー

	//クリックできる画像

	//タイトル画面
	int turnBackImges = Novice::LoadTexture("./Resources/images/clickButton/turnBack.png");//戻るバー
	int gameStartImges = Novice::LoadTexture("./Resources/images/clickButton/gameStart.png");//スタートバー
	int settingImges = Novice::LoadTexture("./Resources/images/clickButton/setting.png");//タイトルにある設定バー

	//設定画面
	int downVolumeImges = Novice::LoadTexture("./Resources/images/clickButton/downVolume.png");//ダウンボリューム
	int upVolumeImges = Novice::LoadTexture("./Resources/images/clickButton/upVolume.png");//アップボリューム

	//ステージセレクト画面
	int stageOneImges = Novice::LoadTexture("./Resources/images/clickButton/stageOne.png");//ステージ1
	int stageTwoImges = Novice::LoadTexture("./Resources/images/clickButton/stageTwo.png");//ステージ2
	int stageThreeImges = Novice::LoadTexture("./Resources/images/clickButton/stageThree.png");//ステージ3
	int stageFourImges = Novice::LoadTexture("./Resources/images/clickButton/stageFour.png");//ステージ4
	int stageFiveImges = Novice::LoadTexture("./Resources/images/clickButton/stageFive.png");//ステージ5

	int stageSixImges = Novice::LoadTexture("./Resources/images/clickButton/stageSix.png");//ステージ6
	int stageSevenImges = Novice::LoadTexture("./Resources/images/clickButton/stageSeven.png");//ステージ7
	int stageEightImges = Novice::LoadTexture("./Resources/images/clickButton/stageEight.png");//ステージ8
	int stageNineImges = Novice::LoadTexture("./Resources/images/clickButton/stageNine.png");//ステージ9
	int stageTenImges = Novice::LoadTexture("./Resources/images/clickButton/stageTen.png");//ステージ10

	int StartImges = Novice::LoadTexture("./Resources/images/clickButton/Start.png");//スタートバー
	int nextStageImges = Novice::LoadTexture("./Resources/images/clickButton/nextStage.png");//次のステージ
	int stageSelectBackImges = Novice::LoadTexture("./Resources/images/clickButton/title.png");//タイトル
	int retryImges = Novice::LoadTexture("./Resources/images/clickButton/retry.png");//リトライ
	
	//ゲームクリア時
	int gameClearImges = Novice::LoadTexture("./Resources/images/background/GameClear.png");//ゲームクリア

	//ゲームオーバー時
	int gameOverImges = Novice::LoadTexture("./Resources/images/mozi/GameOver.png");//ゲームオーバー
	int retryKeyImges = Novice::LoadTexture("./Resources/images/clickButton/retryKey.png");//リトライ

	//数字	
	int zeroImges = Novice::LoadTexture("./Resources/images/number/Zero.png");//0
	int oneImges = Novice::LoadTexture("./Resources/images/number/one.png");//1
	int twoImges = Novice::LoadTexture("./Resources/images/number/two.png");//2
	int threeImges = Novice::LoadTexture("./Resources/images/number/three.png");//3
	int fourImges = Novice::LoadTexture("./Resources/images/number/four.png");//4
	int fiveImges = Novice::LoadTexture("./Resources/images/number/five.png");//5
	int sixImges = Novice::LoadTexture("./Resources/images/number/six.png");//6
	int sevenImges = Novice::LoadTexture("./Resources/images/number/seven.png");//7
	int eightImges = Novice::LoadTexture("./Resources/images/number/eight.png");//8
	int nineImges = Novice::LoadTexture("./Resources/images/number/nine.png");//9
	int FImges = Novice::LoadTexture("./Resources/images/number/F.png");//9


	//数字
	int suuziImges[10] =
	{
		zeroImges,
		oneImges,
		twoImges,
		threeImges,
		fourImges,
		fiveImges,
		sixImges,
		sevenImges,
		eightImges,
		nineImges,
	};
	
	int MaxImges = Novice::LoadTexture("./Resources/images/mozi/MAX.png");//MAX

	int ghostLeftArrowImges = Novice::LoadTexture("./Resources/images/ghostLeftArrow.png");//おばけの左方向
	int ghostRightArrowImges = Novice::LoadTexture("./Resources/images/ghostRightArrow.png");//おばけの右方向
	int ghostDwonArrowImges = Novice::LoadTexture("./Resources/images/ghostDwonArrow.png");//おばけの下方向

	///==========================================================================
	//BGM・SEの変数
	///==========================================================================

	//BGM

	//タイトルのBGMを読み込み
	int titleBGMHandle = Novice::LoadAudio("./Resources/sounds/BGM/TitleBGM.mp3");
	//プレイハンドルの宣言
	int titleHandle = -1;

	//ゲームオーバーのBGMを読み込み
	int gameBGMHandle = Novice::LoadAudio("./Resources/sounds/BGM/StageBGM.mp3");
	//プレイハンドルの宣言
	int gameHandle = -1;

	//ゲームクリアのBGMを読み込み
	int clearBGMHandle = Novice::LoadAudio("./Resources/sounds/BGM/clearBGM.mp3");
	//プレイハンドルの宣言
	int clearHandle = -1;

	//ゲームオーバーのBGMを読み込み
	int gameOverBGMHandle = Novice::LoadAudio("./Resources/sounds/BGM/gameOverBGM.mp3");
	//プレイハンドルの宣言
	int gameOverHandle = -1;

	//ゲームオーバーのBGMを読み込み
	int allClearBGMHandle = Novice::LoadAudio("./Resources/sounds/BGM/allClearBGM.mp3");
	//プレイハンドルの宣言
	int allClearHandle = -1;

	//SE

	//クリックのSEを読み込み
	int clickSEHandle = Novice::LoadAudio("./Resources/sounds/SE/clickSE.mp3");
	//プレイハンドルの宣言
	int clickHandle = -1;

	SE clickSE
	{
		{0},//タイマー
		{1},//再生を終了させるタイマー
		{false},//ならすかどうか決めるフラグ
	};

	//クリックのSEを読み込み
	int switchSEHandle = Novice::LoadAudio("./Resources/sounds/SE/switchSE.mp3");
	//プレイハンドルの宣言
	int switchHandle = -1;

	SE switchSE
	{
		{0},//タイマー
		{1},//再生を終了させるタイマー
		{false},//ならすかどうか決めるフラグ
	}; 

	//クリックのSEを読み込み
	int rockReleaseSEHandle = Novice::LoadAudio("./Resources/sounds/SE/rockReleaseSE.mp3");
	//プレイハンドルの宣言
	int rockReleaseHandle = -1;

	SE rockReleaseSE
	{
		{0},//タイマー
		{1},//再生を終了させるタイマー
		{false},//ならすかどうか決めるフラグ
	};

	//クリックのSEを読み込み
	int retrySEHandle = Novice::LoadAudio("./Resources/sounds/SE/retrySE.mp3");
	//プレイハンドルの宣言
	int retryHandle = -1;

	SE retrySE
	{
		{0},//タイマー
		{1},//再生を終了させるタイマー
		{false},//ならすかどうか決めるフラグ
	};

	float BGMVolumeCount = 3;
	float SEVolumeCount  = 3;
	float BGMVolume = 1.0f;
	float SEVolume  = 1.0f;

	///==========================================================================
	//タイマーの変数
	///==========================================================================

	const int HalfTimer = 60;

	int SceneChangeTimer = 0;//シーンを切り替えるためにずっと動いてるタイマー

	int StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
	int clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー

	///==========================================================================
	//フラグの変数
	///==========================================================================

	bool startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
	bool StartFlag = false;//スタートするかどうかのフラグ

	bool stageClearFlag = false;//ステージをクリアしたか
	bool GameOverFlag = false;//ゲームオーバーしたか

	//ギミック
	bool SteppingFlag = false;//ギミックを踏んだかどうか
	bool onOffFlag = false;//ギミックがオンかオフか

	//bool Flag = false;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		if (rightArrow.Flag == false && leftArrow.Flag == false && upArrow.Flag == false && dwonArrow.Flag == false && dwonArrow2.Flag == false)
		{
			if (Novice::IsTriggerMouse(0))//マウスを長押ししてなくて
			{
				Novice::GetMousePosition(&mousePosX, &mousePosY);//マウスの位置を常に更新しておく
			}
		}
		else
		{
			Novice::GetMousePosition(&mousePosX, &mousePosY);//マウスの位置を常に更新しておく
		}

		SceneChangeTimer++;//シーンを切り替えるためのタイマーを進ませておく
		BlockTimer++;//壊れるブロックのHPの減るTimerを進ませておく
		touchNumberTimer++; 
		onOffTimer++;

		///==========================================================================
		//パーティクルの更新処理
		///==========================================================================

		if (Scene != Title || Scene != Setting || Scene != stageSelect)
		{
			for (int a = 0; a <= MAX; a++)
			{
				if (isShot[a] == false)
				{
					isShot[a] = true;
					color[a] = WHITE;
					particlePosX[a] = (int)ghostPosX - (ghostRadius);//パーティクルを出すX座標を計算
					particlePosY[a] = (int)ghostPosY - (ghostRadius);//パーティクルを出すY座標を計算
					particleSpeedX[a] = rand() % randMax - randMin;//パーティクルを出すX座標を計算
					particleSpeedY[a] = 0;//パーティクルを出すY座標を計算
					break;
				}
			}

			for (int a = 0; a <= MAX; a++)
			{
				if (isShot[a] == true)
				{
					//bulletPosX[a] += bulletSpeedX[a];
					particlePosY[a] += particleSpeedY[a];

					color[a] -= 0x00000015;


					if (color[a] < 0xFFFFFF01)
					{
						isShot[a] = false;
						particleSpeedX[a] = 0;
						particleSpeedY[a] = 0;
					}
				}
			}
		}

		

		///==========================================================================
		//switch文の始まり
		///==========================================================================

		switch (Scene) {
		case Title:
			if (titlePosY >= 0)//タイトルバーが降りきって
			{
				mouseCollision(&setting, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&gameStartKey, mousePosX, mousePosY, mouseClickFlag);
			}

			if (titlePosY >= 0)//タイトルバーが降りきって
			{
				if (gameStartKey.Flag == true)//スタートをクリックしたら
				{
					titleGhostPosX = 1200;
					titleGhostPosY = 600;
					SceneChange(SceneChangeTimer, HalfTimer, stageSelect);//Sceneをステージセレクトに切り替える
					gameStartKey.Flag = false;
				}
			}

			titlePlayerPosX += titleSpeed;
			titleGhostPosX -= (int)ghost.speed.x;

			titleSpeed = (int)-ghost.speed.x;

			if (titlePlayerPosX < blockSize * 3)
			{
				titlePlayerPosX = 1200 * 2;
			}

			if (titleGhostPosX < blockSize * 3)
			{
				titleGhostPosX = 1200 * 2;
			}

			if (setting.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					SceneChange(SceneChangeTimer, HalfTimer, Setting);
				}
				clickSE.Flag = true;
				setting.Flag = false;
			}

			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {//マウスをクリックしていたら
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}

			break;

		case stageSelect:



			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================
			if (!Novice::IsTriggerMouse(1)) {
				if (Novice::IsPressMouse(0)) {//マウスをクリックしていたら
					mouseConutTimer++;
					mouseClickFlag = true;
				}
				else
				{
					mouseConutTimer = 0;
					mouseClickFlag = false;
				}
			}
			///==========================================================================
			//マウスの当たり判定の処理
			///==========================================================================

			mouseCollision(&stageOne, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&stageTwo, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&stageThree, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&stageFour, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&stageFive, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&stageSix, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&stageSeven, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&stageEight, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&stageNine, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&stageTen, mousePosX, mousePosY, mouseClickFlag);

			///==========================================================================
			//ステージ1の処理
			///==========================================================================

			if (stageOne.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					///==========================================================================
					//初期化
					///==========================================================================
					clearGhostPosX = kWindowWidth;
					clearNisitoPosX = kWindowWidth;
					stageClearFlag = false;//一回タイトルなどの戻った時用
					GameOverFlag = false;//一回タイトルなどの戻った時用
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ

					playerReset(&ghost, blockSize, playerSize, ghostRadius, 11, 3, 10.0f, 0.0f);

					

					touchNumber = 0;
					BlockHp = 0;

					SceneChange(SceneChangeTimer, HalfTimer, stage1);
				}
				clickSE.Flag = true;
				stageOne.Flag = false;
			}

			///==========================================================================
			//ステージ2の処理
			///==========================================================================

			if (stageTwo.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					///==========================================================================
					//初期化
					///==========================================================================
					clearGhostPosX = kWindowWidth;
					clearNisitoPosX = kWindowWidth;
					stageClearFlag = false;//一回タイトルなどの戻った時用
					GameOverFlag = false;//一回タイトルなどの戻った時用
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ

					playerReset(&ghost, blockSize, playerSize, ghostRadius, 11, 3, 10.0f, 0.0f);
					
					touchNumber = 0;
					BlockHp = 0;
					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

					SceneChange(SceneChangeTimer, HalfTimer, stage2);
				}
				clickSE.Flag = true;
				stageTwo.Flag = false;
			}

			///==========================================================================
			//ステージ3の処理
			///==========================================================================

			if (stageThree.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					///==========================================================================
					//初期化
					///==========================================================================
					clearGhostPosX = kWindowWidth;
					clearNisitoPosX = kWindowWidth;
					stageClearFlag = false;//一回タイトルなどの戻った時用
					GameOverFlag = false;//一回タイトルなどの戻った時用
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ
					touchNumber = 0;
					BlockHp = 0;

					playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 5, 0.0f, 0.0f);

					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

					SceneChange(SceneChangeTimer, HalfTimer, stage3);
				}
				clickSE.Flag = true;
				stageTwo.Flag = false;
			}

			///==========================================================================
			//ステージ4の処理
			///==========================================================================

			if (stageFour.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					///==========================================================================
					//初期化
					///==========================================================================
					clearGhostPosX = kWindowWidth;
					clearNisitoPosX = kWindowWidth;
					stageClearFlag = false;//一回タイトルなどの戻った時用
					GameOverFlag = false;//一回タイトルなどの戻った時用
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ
					BlockHp = 3;

					blockReset(&breakableBlocks, blockSize, 4, 3);
					playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 5, 10.0f, 0.0f);
					
					touchNumber = 1;
					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

					SceneChange(SceneChangeTimer, HalfTimer, stage4);
				}
				clickSE.Flag = true;
				stageThree.Flag = false;
			}

			///==========================================================================
			//ステージ5の処理
			///==========================================================================

			if (stageFive.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					///==========================================================================
					//初期化
					///==========================================================================
					clearGhostPosX = kWindowWidth;
					clearNisitoPosX = kWindowWidth;
					stageClearFlag = false;//一回タイトルなどの戻った時用
					GameOverFlag = false;//一回タイトルなどの戻った時用
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ

					playerReset(&ghost, blockSize, playerSize, ghostRadius, 6, 3, 10.0f, 0.0f);

					blockReset(&breakableBlocks, blockSize, 7, 6);
					blockReset(&onOffBlock, blockSize, 7, 4);
					blockReset(&switchBlock, blockSize, 4, 4);

					onOffFlag = false;

					touchNumber = 1;
					BlockHp = 3;
					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

					SceneChange(SceneChangeTimer, HalfTimer, stage5);
				}
				clickSE.Flag = true;
				stageThree.Flag = false;
			}

			///==========================================================================
			//ステージ6の処理
			///==========================================================================

			if (stageSix.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					///==========================================================================
					//初期化
					///==========================================================================
					clearGhostPosX = kWindowWidth;
					clearNisitoPosX = kWindowWidth;
					stageClearFlag = false;//一回タイトルなどの戻った時用
					GameOverFlag = false;//一回タイトルなどの戻った時用
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ

					playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 4, 10.0f, 0.0f);
					blockReset(&onOffBlock, blockSize, 7, 4);
					blockReset(&switchBlock, blockSize, 4, 4);

					onOffFlag = false;

					touchNumber = 0;
					BlockHp = 0;
					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, stage6);
				}
				clickSE.Flag = true;
				stageSix.Flag = false;
			}

			///==========================================================================
			//ステージ7の処理
			///==========================================================================

			if (stageSeven.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					///==========================================================================
					//初期化
					///==========================================================================
					clearGhostPosX = kWindowWidth;
					clearNisitoPosX = kWindowWidth;
					stageClearFlag = false;//一回タイトルなどの戻った時用
					GameOverFlag = false;//一回タイトルなどの戻った時用
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ
					playerReset(&ghost, blockSize, playerSize, ghostRadius, 6, 1, 0.0f, 0.0f);
					blockReset(&onOffBlock, blockSize, 2, 3);
					blockReset(&onOffBlock2, blockSize, 4, 1);
					blockReset(&switchBlock, blockSize, 9, 4);
					blockReset(&breakableBlocks, blockSize, 3, 4);

					onOffFlag = false;

					touchNumber = 0;
					BlockHp = 3;
					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, stage7);
				}
				clickSE.Flag = true;
				stageSeven.Flag = false;
			}

			///==========================================================================
			//ステージ8の処理
			///==========================================================================

			if (stageEight.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					///==========================================================================
					//初期化
					///==========================================================================
					clearGhostPosX = kWindowWidth;
					clearNisitoPosX = kWindowWidth;
					stageClearFlag = false;//一回タイトルなどの戻った時用
					GameOverFlag = false;//一回タイトルなどの戻った時用
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ
					playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 3, 0.0f, 0.0f);

					blockReset(&onOffBlock, blockSize, 2, 3);
					blockReset(&onOffBlock2, blockSize, 4, 1);
					blockReset(&switchBlock, blockSize, 9, 4);
					blockReset(&breakableBlocks, blockSize, 3, 4);
					blockReset(&key, blockSize, 3, 5);
					key.position.x = 0;
					key.position.y = 0;
					KeyFlag = true;
					onOffFlag = false;

					touchNumber = 0;
					BlockHp = 0;
					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, stage8);
				}
				clickSE.Flag = true;
				stageEight.Flag = false;
			}

			///==========================================================================
			//ステージ9の処理
			///==========================================================================

			if (stageNine.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					///==========================================================================
					//初期化
					///==========================================================================
					clearGhostPosX = kWindowWidth;
					clearNisitoPosX = kWindowWidth;
					stageClearFlag = false;//一回タイトルなどの戻った時用
					GameOverFlag = false;//一回タイトルなどの戻った時用
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ
					playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 4, 0.0f, 0.0f);
					blockReset(&key, blockSize, 5, 6);
					key.position.x = 0;
					key.position.y = 0;
					KeyFlag = true;
					onOffFlag = false;

					touchNumber = 0;
					BlockHp = 0;
					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, stage9);
				}
				clickSE.Flag = true;
				stageNine.Flag = false;
			}

			///==========================================================================
			//ステージ10の処理
			///==========================================================================

			if (stageTen.Flag == true)
			{
				if (clickSE.Flag == true)
				{
					///==========================================================================
					//初期化
					///==========================================================================
					clearGhostPosX = kWindowWidth;
					clearNisitoPosX = kWindowWidth;
					stageClearFlag = false;//一回タイトルなどの戻った時用
					GameOverFlag = false;//一回タイトルなどの戻った時用
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ
					playerReset(&ghost, blockSize, playerSize, ghostRadius, 3, 1, 0.0f, 0.0f);

					blockReset(&onOffBlock, blockSize, 2, 6);
					blockReset(&onOffBlock2, blockSize, 11, 4);
					blockReset(&switchBlock, blockSize, 4, 5);
					blockReset(&breakableBlocks, blockSize, 10, 6);
					blockReset(&key, blockSize, 1, 5);
					key.position.x = 0;
					key.position.y = 0;
					KeyFlag = true;
					onOffFlag = false;

					touchNumber = 1;
					BlockHp = 3;
					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, stage10);
				}
				clickSE.Flag = true;
				stageTen.Flag = false;
			}
			break;

		case stage1:

			///==========================================================================
			//座標の更新処理
			///==========================================================================

			playerRum(&ghost, &TileGhost, ghostPosX, ghostPosY, blockSize);

			///==========================================================================
			//おばけの更新処理
			///==========================================================================

			ghost.position.x = speed(-ghost.speed.x, ghost.position.x);
			ghost.position.y = speed(ghost.speed.y, ghost.position.y);

			if (StartFlag == false)
			{
				ghost.speed.x = 0;
				ghost.speed.y = 0;
			}

			if (StartFlag == true)
			{
				StartSETimer++;
			}

			if (startSpeedStopFlag == false && StartFlag == true)
			{
				ghost.speed.x = 10;
			}



			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}

			///==========================================================================
			//マップの当たり判定の処理
			///==========================================================================


			//壊れるブロック
			if (BlockHp != 0)
			{
				RightBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, BlockHp, BlockTimer);
				LeftBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, BlockHp, BlockTimer);
				UpBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, 0, arrowSpeedY, BlockHp, BlockTimer);
				DwonBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, 0, -arrowSpeedY, BlockHp, BlockTimer);
			}
			

			//おばけとブロック


			if (StartFlag == true)
			{
				MapChipCondition(&ghost,&TileGhost, Stage1Map, touchNumber, touchNumberTimer, startSpeedStopFlag);

				NoviceGoalMapChip(&ghost,&TileGhost, Stage1Map,stageClearFlag);

				//矢印とおばけ
			}

			///==========================================================================
			//マウスの当たり判定の処理
			///==========================================================================

			//矢印
			mouseCollision(&rightArrow, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&leftArrow, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&upArrow, mousePosX, mousePosY, mouseClickFlag);
			mouseCollision(&dwonArrow, mousePosX, mousePosY, mouseClickFlag);

			//スタートボタン
			mouseCollision(&StartKey, mousePosX, mousePosY, mouseClickFlag);

			if (stageClearFlag == true)//クリアしたら
			{
				//次へ進むボタン
				mouseCollision(&nextStage, mousePosX, mousePosY, mouseClickFlag);
				//タイトルへ
				mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
			}

			if (GameOverFlag == true)
			{
				//リトライ
				mouseCollision(&retryKey, mousePosX, mousePosY, mouseClickFlag);
				//タイトルへ
				mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
			}

			//矢印
			mouseGetCondition(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2, mousePosX, mousePosY, mouseMoveFlag, mouseClickFlag);

			//スタートキー
			if (StartKey.Flag == true)
			{
				StartFlag = true;
			}

			if (StartFlag == true)
			{
				StartKey.Flag = false;
			}


			///==========================================================================
			//シーン切り替えの更新処理
			///==========================================================================

			//次へ進む
			if (stageClearFlag == true)//ステージクリアかつ
			{
				if (title.Flag == true)
				{
					title.Flag = false;
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, Title);
				}

				if (nextStage.Flag == true)//押されたら
				{
					clickSE.Flag = true;
					if (clickSE.Flag == true)//SEを鳴らして
					{

						

						///==========================================================================
						//初期化
						///==========================================================================
						StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
						clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
						startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
						StartFlag = false;//スタートするかどうかのフラグ
						playerReset(&ghost, blockSize, playerSize, ghostRadius, 11, 3, 0.0f, 0.0f);

						clearGhostPosX = kWindowWidth;
						clearNisitoPosX = kWindowWidth;

						nextStage.Flag = false;
						stageClearFlag = false;

						touchNumber = 0;
						//シーンの切り替え
						SceneChange(SceneChangeTimer, HalfTimer, stage2);

					}
						
					//clickSE.Flag = false;
				}
			}



			///==========================================================================
			//ステージクリアの更新処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				clearGhostPosX -= clearGhostSpeed;
				if (clearGhostPosX < 0 - playerSize)
				{
					clearNisitoPosX -= clearGhostSpeed * 5;
				}
			}

			///==========================================================================
			//ゲームオーバーの更新処理
			///==========================================================================

			if (touchNumber < 0)
			{
				GameOverFlag = true;
			}

			if (GameOverFlag == true)
			{
				if (retryKey.Flag == true)
				{
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ
					stageOne.Flag = false;
					playerReset(&ghost, blockSize, playerSize, ghostRadius, 11, 3, 10.0f, 0.0f);
					
					touchNumber = 0;
					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする
					GameOverFlag = false;
					retryKey.Flag = false;
				}

				if (title.Flag == true)
				{
					title.Flag = false;
					GameOverFlag = false;
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, Title);
				}
			}

			break;

		
		case stage2:

		///==============================================================================================================================================
		//ステージ2
		///===============================================================================================================================================


			///==========================================================================
			//座標の更新処理
			///==========================================================================

			playerRum(&ghost, &TileGhost, ghostPosX, ghostPosY, blockSize);

			///==========================================================================
			//おばけの更新処理
			///==========================================================================

			ghost.position.x = speed(-ghost.speed.x, ghost.position.x);
			ghost.position.y = speed(ghost.speed.y, ghost.position.y);

			if (StartFlag == false)
			{
				ghost.speed.x = 0;
				ghost.speed.y = 0;
			}

			if (StartFlag == true)
			{
				StartSETimer++;

			}

			if (startSpeedStopFlag == false && StartFlag == true)
			{
				ghost.speed.x = 10;
			}



			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}

			///==========================================================================
			//マップの当たり判定の処理
			///==========================================================================

			//おばけとブロック


			if (StartFlag == true)
			{
				MapChipCondition(&ghost, &TileGhost, Stage2Map, touchNumber, touchNumberTimer, startSpeedStopFlag);
				NoviceGoalMapChip(&ghost, &TileGhost, Stage2Map, stageClearFlag);

				//矢印とおばけ

				//下矢印
				allCollision(&ghost, &dwonArrow, mouseMoveFlag, spped_0, arrowSpeedY, startSpeedStopFlag);

			}

			///==========================================================================
			//マウスの当たり判定の処理
			///==========================================================================


			if (StartFlag == false)
			{
				mouseCollision(&rightArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&leftArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&upArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&dwonArrow, mousePosX, mousePosY, mouseClickFlag);
			}

			if (SceneChangeTimer > HalfTimer && mouseMoveFlag == false)
			{
				//スタートボタン
				mouseCollision(&StartKey, mousePosX, mousePosY, mouseClickFlag);
			}

			if (StartFlag == true)
			{
				//リトライボタン
				mouseCollision(&retry, mousePosX, mousePosY, mouseClickFlag);
			}

			if (stageClearFlag == true)//クリアしたら
			{
				//次へ進むボタン
				mouseCollision(&nextStage, mousePosX, mousePosY, mouseClickFlag);
				//タイトルへ
				mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
			}

			mouseGetCondition(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2, mousePosX, mousePosY, mouseMoveFlag, mouseClickFlag);

			//スタートキー
			if (StartKey.Flag == true)
			{
				StartFlag = true;
			}

			if (StartFlag == true)
			{
				StartKey.Flag = false;
			}

			//retryキー
			if (retry.Flag == true)
			{
				StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
				clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
				startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
				StartFlag = false;//スタートするかどうかのフラグ
				stageTwo.Flag = false;
				playerReset(&ghost, blockSize, playerSize, ghostRadius, 11, 3, 10.0f, 0.0f);
				touchNumber = 0;
				arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

				retry.Flag = false;
			}

			
			if (stageClearFlag == true)//ステージクリアかつ
			{
				if (title.Flag == true)
				{
					title.Flag = false;
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, Title);
				}

				if (nextStage.Flag == true)//押されたら
				{
					clickSE.Flag = true;
					
					if (clickSE.Flag == true)//SEを鳴らして
					{
						///==========================================================================
						//初期化
						///==========================================================================
						StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
						clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
						startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
						StartFlag = false;//スタートするかどうかのフラグ
						playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 5, 0.0f, 0.0f);
						touchNumber = 0;
						arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

						clearGhostPosX = kWindowWidth;
						clearNisitoPosX = kWindowWidth;

						stageClearFlag = false;
						nextStage.Flag = false;
						//シーンの切り替え
						SceneChange(SceneChangeTimer, HalfTimer, stage3);
					}
					
				}
			}

			///==========================================================================
			//ステージクリアの更新処理;
			///==========================================================================

			if (stageClearFlag == true)
			{
				clearGhostPosX -= clearGhostSpeed;
				if (clearGhostPosX < 0 - playerSize)
				{
					clearNisitoPosX -= clearGhostSpeed * 5;
				}
			}

			break;

		case stage3:

			///==========================================================================
			//座標の更新処理
			///==========================================================================

			playerRum(&ghost, &TileGhost, ghostPosX, ghostPosY, blockSize);

			///==========================================================================
			//おばけの更新処理
			///==========================================================================

			ghost.position.x -= speed(ghost.speed.x, ghost.position.x);
			ghost.position.y = speed(ghost.speed.y, ghost.position.y);

			if (StartFlag == false)
			{
				ghost.speed.x = 0;
				ghost.speed.y = 0;
			}

			if (StartFlag == true)
			{
				StartSETimer++;

			}

			if (startSpeedStopFlag == false && StartFlag == true)
			{
				ghost.speed.x = -10;
			}

			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}

			///==========================================================================
			//マップの当たり判定の処理
			///==========================================================================

			//おばけとブロック

			if (StartFlag == true)
			{
				MapChipCondition(&ghost, &TileGhost, Stage3Map, touchNumber, touchNumberTimer, startSpeedStopFlag);

				//ゴール
				NoviceGoalMapChip(&ghost, &TileGhost, Stage3Map, stageClearFlag);

				//矢印とおばけ

				//右矢印
				allCollision(&ghost, &rightArrow, mouseMoveFlag, -arrowSpeedX, spped_0, startSpeedStopFlag);

				//左矢印
				allCollision(&ghost, &leftArrow, mouseMoveFlag, arrowSpeedX, spped_0, startSpeedStopFlag);

				//上矢印
				allCollision(&ghost, &upArrow, mouseMoveFlag, spped_0, -arrowSpeedY, startSpeedStopFlag);

				//下矢印
				allCollision(&ghost, &dwonArrow, mouseMoveFlag, spped_0, arrowSpeedY, startSpeedStopFlag);

			}

			///==========================================================================
			//マウスの当たり判定の処理
			///==========================================================================


			if (StartFlag == false)
			{
				mouseCollision(&rightArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&leftArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&upArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&dwonArrow, mousePosX, mousePosY, mouseClickFlag);
			}

			if (SceneChangeTimer > HalfTimer && mouseMoveFlag == false)
			{
				//スタートボタン
				mouseCollision(&StartKey, mousePosX, mousePosY, mouseClickFlag);
			}

			if (StartFlag == true)
			{
				//リトライボタン
				mouseCollision(&retry, mousePosX, mousePosY, mouseClickFlag);
			}

			if (stageClearFlag == true)//クリアしたら
			{
				//次へ進むボタン
				mouseCollision(&nextStage, mousePosX, mousePosY, mouseClickFlag);
				//タイトルへ
				mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
			}

			mouseGetCondition(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2, mousePosX, mousePosY, mouseMoveFlag, mouseClickFlag);

			if (mouseMoveFlag == true && mouseClickFlag == false)
			{
				mouseMoveFlag = false;
				rightArrow.Flag = false;
				leftArrow.Flag = false;
				upArrow.Flag = false;
				dwonArrow.Flag = false;
			}

			//スタートキー
			if (StartKey.Flag == true)
			{
				StartFlag = true;
			}

			if (StartFlag == true)
			{
				StartKey.Flag = false;
			}

			//retryキー
			if (retry.Flag == true)
			{
				StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
				clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
				startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
				StartFlag = false;//スタートするかどうかのフラグ
				touchNumber = 0;
				stageThree.Flag = false;

				playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 5, 10.0f, 0.0f);
				arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

				retry.Flag = false;
			}

			if (stageClearFlag == true)//ステージクリアかつ
			{
				if (title.Flag == true)
				{
					title.Flag = false;
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, Title);
				}

				if (nextStage.Flag == true)//押されたら
				{
					clickSE.Flag = true;

					if (clickSE.Flag == true)//SEを鳴らして
					{
						///==========================================================================
						//初期化
						///==========================================================================
						StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
						clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
						startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
						StartFlag = false;//スタートするかどうかのフラグ
						BlockHp = 3;
						breakableBlocks =
						{
							{0,0},
							{blockSize * 4                   ,blockSize * 3},
							{blockSize * 4 + (float)blockSize,blockSize * 3},
							{blockSize * 4                   ,blockSize * 3 + (float)blockSize},
							{blockSize * 4 + (float)blockSize,blockSize * 3 + (float)blockSize},
							{false},
						};

						blockReset(&breakableBlocks, blockSize, 4, 3);
						playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 5, 0.0f, 0.0f);
						arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

						clearGhostPosX = kWindowWidth;
						clearNisitoPosX = kWindowWidth;
						touchNumber = 1;
						stageClearFlag = false;
						nextStage.Flag = false;
						//シーンの切り替え
						SceneChange(SceneChangeTimer, HalfTimer, stage4);
					}

				}
			}



			///==========================================================================
			//ステージクリアの更新処理;
			///==========================================================================

			if (stageClearFlag == true)
			{
				clearGhostPosX -= clearGhostSpeed;
				if (clearGhostPosX < 0 - playerSize)
				{
					clearNisitoPosX -= clearGhostSpeed * 5;
				}
			}


				break;

		case stage4:

			///==============================================================================================================================================
		    //ステージ4
		    ///===============================================================================================================================================


			///==========================================================================
			//座標の更新処理
			///==========================================================================

			playerRum(&ghost, &TileGhost, ghostPosX, ghostPosY, blockSize);

			

			///==========================================================================
			//おばけの更新処理
			///==========================================================================

			ghost.position.x -= speed(ghost.speed.x, ghost.position.x);
			ghost.position.y = speed(ghost.speed.y, ghost.position.y);

			if (StartFlag == false)
			{
				ghost.speed.x = 0;
				ghost.speed.y = 0;
			}

			if (StartFlag == true)
			{
				StartSETimer++;

			}

			if (startSpeedStopFlag == false && StartFlag == true)
			{
				ghost.speed.y = 10;
			}



			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}

			///==========================================================================
			//マップの当たり判定の処理
			///==========================================================================

			//壊れるブロック
			if (BlockHp != 0)
			{
				RightBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, BlockHp, BlockTimer);
				LeftBlockCollision( &ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, BlockHp, BlockTimer);
				UpBlockCollision   (&ghost, breakableBlocks, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, arrowSpeedY, BlockHp, BlockTimer);
				DwonBlockCollision( &ghost, breakableBlocks, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, -arrowSpeedY, BlockHp, BlockTimer);
			}

			//おばけとブロック

			if (StartFlag == true)
			{
				MapChipCondition(&ghost, &TileGhost, Stage4Map, touchNumber, touchNumberTimer, startSpeedStopFlag);
				NoviceGoalMapChip(&ghost, &TileGhost, Stage4Map, stageClearFlag);

				//矢印とおばけ

				//右矢印
				allCollision(&ghost, &rightArrow, mouseMoveFlag, -arrowSpeedX, spped_0, startSpeedStopFlag);

				//左矢印
				allCollision(&ghost, &leftArrow, mouseMoveFlag, arrowSpeedX, spped_0, startSpeedStopFlag);

				//上矢印
				allCollision(&ghost, &upArrow, mouseMoveFlag, spped_0, -arrowSpeedY, startSpeedStopFlag);

				//下矢印
				allCollision(&ghost, &dwonArrow, mouseMoveFlag, spped_0, arrowSpeedY, startSpeedStopFlag);
			}

			///==========================================================================
			//マウスの当たり判定の処理
			///==========================================================================


			if (StartFlag == false)
			{
				mouseCollision(&rightArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&leftArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&upArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&dwonArrow, mousePosX, mousePosY, mouseClickFlag);
			}

			if (SceneChangeTimer > HalfTimer && mouseMoveFlag == false)
			{
				//スタートボタン
				mouseCollision(&StartKey, mousePosX, mousePosY, mouseClickFlag);
			}

			if (StartFlag == true)
			{
				//リトライボタン
				mouseCollision(&retry, mousePosX, mousePosY, mouseClickFlag);
			}

			if (stageClearFlag == true)//クリアしたら
			{
				//次へ進むボタン
				mouseCollision(&nextStage, mousePosX, mousePosY, mouseClickFlag);
				//タイトルへ
				mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
			}

			mouseGetCondition(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2, mousePosX, mousePosY, mouseMoveFlag, mouseClickFlag);

			//スタートキー
			if (StartKey.Flag == true)
			{
				StartFlag = true;
			}

			if (StartFlag == true)
			{
				StartKey.Flag = false;
			}

			//retryキー
			if (retry.Flag == true)
			{
				StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
				clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
				startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
				StartFlag = false;//スタートするかどうかのフラグ
				stageFour.Flag = false;
				BlockHp = 3;
				ghost =
				{//おばけ
					{0,0},
					{((float)blockSize * 1) + 10 + ghostRadius                     ,(float)blockSize * 5 + ghostRadius},
					{((float)blockSize * 1) + (float)playerSize + 10 + ghostRadius ,(float)blockSize * 5 + ghostRadius},
					{((float)blockSize * 1) + 10 + ghostRadius                     ,((float)blockSize * 5) + (float)playerSize + ghostRadius},
					{((float)blockSize * 1) + (float)playerSize + 10 + ghostRadius ,((float)blockSize * 5) + (float)playerSize + ghostRadius},
					{0.0f,0.0f},
				};
				touchNumber = 1;
				arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

				retry.Flag = false;
			}


			if (stageClearFlag == true)//ステージクリアかつ
			{
				if (title.Flag == true)
				{
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, Title);
					title.Flag = false;
				}

				if (nextStage.Flag == true)//押されたら
				{
					clickSE.Flag = true;

					if (clickSE.Flag == true)//SEを鳴らして
					{
						///==========================================================================
						//初期化
						///==========================================================================
						StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
						clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
						startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
						StartFlag = false;//スタートするかどうかのフラグ
						playerReset(&ghost, blockSize, playerSize, ghostRadius, 6, 3, 0.0f, 0.0f);
						blockReset(&breakableBlocks, blockSize, 7, 6);

						BlockHp = 3;
						touchNumber = 1;
						arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

						clearGhostPosX = kWindowWidth;
						clearNisitoPosX = kWindowWidth;

						stageClearFlag = false;
						nextStage.Flag = false;
						//シーンの切り替え
						SceneChange(SceneChangeTimer, HalfTimer, stage5);
					}

				}
			}

			///==========================================================================
			//ステージクリアの更新処理;
			///==========================================================================

			if (stageClearFlag == true)
			{
				clearGhostPosX -= clearGhostSpeed;
				if (clearGhostPosX < 0 - playerSize)
				{
					clearNisitoPosX -= clearGhostSpeed * 5;
				}
			}


			break;

			case stage5:
			///==============================================================================================================================================
			//ステージ5
			///===============================================================================================================================================


			///==========================================================================
			//座標の更新処理
			///==========================================================================

				playerRum(&ghost, &TileGhost, ghostPosX, ghostPosY, blockSize);

				///==========================================================================
				//おばけの更新処理
				///==========================================================================

				ghost.position.x -= speed(ghost.speed.x, ghost.position.x);
				ghost.position.y = speed(ghost.speed.y, ghost.position.y);

				if (StartFlag == false)
				{
					ghost.speed.x = 0;
					ghost.speed.y = 0;
				}

				if (StartFlag == true)
				{
					StartSETimer++;

				}

				if (startSpeedStopFlag == false && StartFlag == true)
				{
					ghost.speed.x = -10;
				}



				///==========================================================================
				//マウスを押した時の処理
				///==========================================================================

				if (Novice::IsPressMouse(0)) {
					mouseConutTimer++;
					mouseClickFlag = true;
				}
				else
				{
					mouseConutTimer = 0;
					mouseClickFlag = false;
				}

				///==========================================================================
				//マップの当たり判定の処理
				///==========================================================================

				//壊れるブロック
				if (BlockHp != 0)
				{
					RightBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, BlockHp, BlockTimer);
					LeftBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, BlockHp, BlockTimer);
					UpBlockCollision(&ghost, breakableBlocks, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, arrowSpeedY, BlockHp, BlockTimer);
					DwonBlockCollision(&ghost, breakableBlocks, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, -arrowSpeedY, BlockHp, BlockTimer);
				}

				//おばけとブロック
				if (StartFlag == true)
				{
					MapChipCondition(&ghost, &TileGhost, Stage5Map, touchNumber, touchNumberTimer, startSpeedStopFlag);

					//ゴール
					NoviceGoalMapChip(&ghost, &TileGhost, Stage5Map, stageClearFlag);

					//矢印とおばけ

					//右矢印
					allCollision(&ghost, &rightArrow, mouseMoveFlag, -arrowSpeedX, spped_0, startSpeedStopFlag);

					//左矢印
					allCollision(&ghost, &leftArrow, mouseMoveFlag, arrowSpeedX, spped_0, startSpeedStopFlag);

					//上矢印
					allCollision(&ghost, &upArrow, mouseMoveFlag, spped_0, -arrowSpeedY, startSpeedStopFlag);

					//下矢印
					allCollision(&ghost, &dwonArrow, mouseMoveFlag, spped_0, arrowSpeedY, startSpeedStopFlag);
				}

				///==========================================================================
				//マウスの当たり判定の処理
				///==========================================================================


				if (StartFlag == false)
				{
					mouseCollision(&rightArrow, mousePosX, mousePosY, mouseClickFlag);
					mouseCollision(&leftArrow, mousePosX, mousePosY, mouseClickFlag);
					mouseCollision(&upArrow, mousePosX, mousePosY, mouseClickFlag);
					mouseCollision(&dwonArrow, mousePosX, mousePosY, mouseClickFlag);
				}

				if (SceneChangeTimer > HalfTimer && mouseMoveFlag == false)
				{
					//スタートボタン
					mouseCollision(&StartKey, mousePosX, mousePosY, mouseClickFlag);
				}

				if (StartFlag == true)
				{
					//リトライボタン
					mouseCollision(&retry, mousePosX, mousePosY, mouseClickFlag);
				}

				if (stageClearFlag == true)//クリアしたら
				{
					//次へ進むボタン
					mouseCollision(&nextStage, mousePosX, mousePosY, mouseClickFlag);
					//タイトルへ
					mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
				}

				mouseGetCondition(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2, mousePosX, mousePosY, mouseMoveFlag, mouseClickFlag);

				//スタートキー
				if (StartKey.Flag == true)
				{
					StartFlag = true;
				}

				if (StartFlag == true)
				{
					StartKey.Flag = false;
				}

				//retryキー
				if (retry.Flag == true)
				{
					StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
					clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
					startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
					StartFlag = false;//スタートするかどうかのフラグ
					stageFive.Flag = false;
					playerReset(&ghost, blockSize, playerSize, ghostRadius, 6, 3, 0.0f, 0.0f);
					BlockHp = 3;
					touchNumber = 1;
					arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

					retry.Flag = false;
				}


				if (stageClearFlag == true)//ステージクリアかつ
				{
					if (title.Flag == true)
					{
						title.Flag = false;
						//シーンの切り替え
						SceneChange(SceneChangeTimer, HalfTimer, Title);
					}

					if (nextStage.Flag == true)//押されたら
					{
						clickSE.Flag = true;

						if (clickSE.Flag == true)//SEを鳴らして
						{
							///==========================================================================
							//初期化
							///==========================================================================
							StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
							clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
							startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
							StartFlag = false;//スタートするかどうかのフラグ
							playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 4, 0.0f, 0.0f);
							blockReset(&onOffBlock, blockSize, 7, 4);
							blockReset(&switchBlock, blockSize, 4, 4);
							
							onOffFlag = false;
							touchNumber = 0;
							arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

							clearGhostPosX = kWindowWidth;
							clearNisitoPosX = kWindowWidth;

							onOffFlag = false;
							stageClearFlag = false;
							nextStage.Flag = false;
							//シーンの切り替え
							SceneChange(SceneChangeTimer, HalfTimer, stage6);
						}

					}
				}

				///==========================================================================
				//ステージクリアの更新処理;
				///==========================================================================

				if (stageClearFlag == true)
				{
					clearGhostPosX -= clearGhostSpeed;
					if (clearGhostPosX < 0 - playerSize)
					{
						clearNisitoPosX -= clearGhostSpeed * 5;
					}
				}

				break;

		case stage6:

			///==============================================================================================================================================
			//ステージ6
			///===============================================================================================================================================


			///==========================================================================
			//座標の更新処理
			///==========================================================================

			playerRum(&ghost, &TileGhost, ghostPosX, ghostPosY, blockSize);

			///==========================================================================
			//おばけの更新処理
			///==========================================================================

			ghost.position.x -= speed(ghost.speed.x, ghost.position.x);
			ghost.position.y = speed(ghost.speed.y, ghost.position.y);

			if (StartFlag == false)
			{
				ghost.speed.x = 0;
				ghost.speed.y = 0;
			}

			if (StartFlag == true)
			{
				StartSETimer++;

			}

			if (startSpeedStopFlag == false && StartFlag == true)
			{
				ghost.speed.x = -10;
			}



			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}

			///==========================================================================
			//マップの当たり判定の処理
			///==========================================================================

			//壊れるブロック
			if (BlockHp != 0)
			{
				RightBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, BlockHp, BlockTimer);
				LeftBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, BlockHp, BlockTimer);
				UpBlockCollision(&ghost, breakableBlocks, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, arrowSpeedY, BlockHp, BlockTimer);
				DwonBlockCollision(&ghost, breakableBlocks, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, -arrowSpeedY, BlockHp, BlockTimer);
			}

			if (onOffFlag == false)
			{
				//オンオフブロック
				onOffRightBlockCollision(&ghost, onOffBlock, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, onOffFlag);
				onOffLeftBlockCollision(&ghost, onOffBlock, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, onOffFlag);
				onOffUpBlockCollision(&ghost, onOffBlock, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, -arrowSpeedY, onOffFlag);
				onOffDwonBlockCollision(&ghost, onOffBlock, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, arrowSpeedY, onOffFlag);
			}

			//スイッチブロック
			switchRightBlockCollision(&ghost, switchBlock, ghostPosY, SteppingFlag, onOffFlag);
			switchLeftBlockCollision( &ghost, switchBlock, ghostPosY, SteppingFlag, onOffFlag);
			switchUpBlockCollision(   &ghost, switchBlock, ghostPosX, SteppingFlag, onOffFlag);
			switchDwonBlockCollision( &ghost, switchBlock, ghostPosX, SteppingFlag, onOffFlag);

			//おばけとブロック
			if (StartFlag == true)
			{
				MapChipCondition(&ghost, &TileGhost, Stage6Map, touchNumber, touchNumberTimer, startSpeedStopFlag);

				//ゴール
				NoviceGoalMapChip(&ghost, &TileGhost, Stage6Map, stageClearFlag);

				//矢印とおばけ

				//右矢印
				allCollision(&ghost, &rightArrow, mouseMoveFlag, -arrowSpeedX, spped_0, startSpeedStopFlag);

				//左矢印
				allCollision(&ghost, &leftArrow, mouseMoveFlag, arrowSpeedX, spped_0, startSpeedStopFlag);

				//上矢印
				allCollision(&ghost, &upArrow, mouseMoveFlag, spped_0, -arrowSpeedY, startSpeedStopFlag);

				//下矢印
				allCollision(&ghost, &dwonArrow, mouseMoveFlag, spped_0, arrowSpeedY, startSpeedStopFlag);
			}

			///==========================================================================
			//マウスの当たり判定の処理
			///==========================================================================


			if (StartFlag == false)
			{
				mouseCollision(&rightArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&leftArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&upArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&dwonArrow, mousePosX, mousePosY, mouseClickFlag);
			}

			if (SceneChangeTimer > HalfTimer && mouseMoveFlag == false)
			{
				//スタートボタン
				mouseCollision(&StartKey, mousePosX, mousePosY, mouseClickFlag);
			}

			if (StartFlag == true)
			{
				//リトライボタン
				mouseCollision(&retry, mousePosX, mousePosY, mouseClickFlag);
			}

			if (stageClearFlag == true)//クリアしたら
			{
				//次へ進むボタン
				mouseCollision(&nextStage, mousePosX, mousePosY, mouseClickFlag);
				//タイトルへ
				mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
			}

			mouseGetCondition(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2, mousePosX, mousePosY, mouseMoveFlag, mouseClickFlag);

			if (mouseMoveFlag == true && mouseClickFlag == false)
			{
				mouseMoveFlag = false;
				rightArrow.Flag = false;
				leftArrow.Flag = false;
				upArrow.Flag = false;
				dwonArrow.Flag = false;
			}

			//スタートキー
			if (StartKey.Flag == true)
			{
				StartFlag = true;
			}

			if (StartFlag == true)
			{
				StartKey.Flag = false;
			}

			//retryキー
			if (retry.Flag == true)
			{
				StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
				clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
				startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
				StartFlag = false;//スタートするかどうかのフラグ
				stageSix.Flag = false;
				playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 4, 0.0f, 0.0f);
				onOffFlag = false;
				BlockHp = 3;
				touchNumber = 1;
				arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

				retry.Flag = false;
			}


			if (stageClearFlag == true)//ステージクリアかつ
			{
				if (title.Flag == true)
				{
					title.Flag = false;
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, Title);
				}

				if (nextStage.Flag == true)//押されたら
				{
					clickSE.Flag = true;

					if (clickSE.Flag == true)//SEを鳴らして
					{
						///==========================================================================
						//初期化
						///==========================================================================
						StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
						clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
						startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
						StartFlag = false;//スタートするかどうかのフラグ
						playerReset(&ghost, blockSize, playerSize, ghostRadius, 6, 1, 0.0f, 0.0f);
						blockReset(&onOffBlock, blockSize, 2, 3);
						blockReset(&onOffBlock2, blockSize, 4, 1);
						blockReset(&switchBlock, blockSize, 9, 4);
						blockReset(&breakableBlocks, blockSize, 3, 4);
						
						onOffFlag = false;

						onOffFlag = false;
						touchNumber = 0;
						BlockHp = 3;
						arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

						clearGhostPosX = kWindowWidth;
						clearNisitoPosX = kWindowWidth;

						stageClearFlag = false;
						nextStage.Flag = false;
						//シーンの切り替え
						SceneChange(SceneChangeTimer, HalfTimer, stage7);
					}

				}
			}

			///==========================================================================
			//ステージクリアの更新処理;
			///==========================================================================

			if (stageClearFlag == true)
			{
				clearGhostPosX -= clearGhostSpeed;
				if (clearGhostPosX < 0 - playerSize)
				{
					clearNisitoPosX -= clearGhostSpeed * 5;
				}
			}

				break;

		case stage7:

					///==============================================================================================================================================
					//ステージ7
					///===============================================================================================================================================


					///==========================================================================
					//座標の更新処理
					///==========================================================================

					playerRum(&ghost, &TileGhost, ghostPosX, ghostPosY, blockSize);

					///==========================================================================
					//おばけの更新処理
					///==========================================================================

					ghost.position.x -= speed(ghost.speed.x, ghost.position.x);
					ghost.position.y = speed(ghost.speed.y, ghost.position.y);

					if (StartFlag == false)
					{
						ghost.speed.x = 0;
						ghost.speed.y = 0;
					}

					if (StartFlag == true)
					{
						StartSETimer++;

					}

					if (startSpeedStopFlag == false && StartFlag == true)
					{
						ghost.speed.x = 10;
					}



					///==========================================================================
					//マウスを押した時の処理
					///==========================================================================

					if (Novice::IsPressMouse(0)) {
						mouseConutTimer++;
						mouseClickFlag = true;
					}
					else
					{
						mouseConutTimer = 0;
						mouseClickFlag = false;
					}

					///==========================================================================
					//マップの当たり判定の処理
					///==========================================================================

					//壊れるブロック
					if (BlockHp != 0)
					{
						RightBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, BlockHp, BlockTimer);
						LeftBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, BlockHp, BlockTimer);
						UpBlockCollision(&ghost, breakableBlocks, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, -arrowSpeedY, BlockHp, BlockTimer);
						DwonBlockCollision(&ghost, breakableBlocks, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, arrowSpeedY, BlockHp, BlockTimer);
					}

					if (onOffFlag == false)
					{
						//オンオフブロック
						onOffRightBlockCollision(&ghost, onOffBlock, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, onOffFlag);
						onOffLeftBlockCollision(&ghost, onOffBlock, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, onOffFlag);
						onOffUpBlockCollision(&ghost, onOffBlock, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, -arrowSpeedY, onOffFlag);
						onOffDwonBlockCollision(&ghost, onOffBlock, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, arrowSpeedY, onOffFlag);

						onOffRightBlockCollision(&ghost, onOffBlock2, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, onOffFlag);
						onOffLeftBlockCollision(&ghost, onOffBlock2, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, onOffFlag);
						onOffUpBlockCollision(&ghost, onOffBlock2, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, -arrowSpeedY, onOffFlag);
						onOffDwonBlockCollision(&ghost, onOffBlock2, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, arrowSpeedY, onOffFlag);
					}

					//スイッチブロック
					switchRightBlockCollision(&ghost, switchBlock, ghostPosY, SteppingFlag, onOffFlag);
					switchLeftBlockCollision(&ghost, switchBlock, ghostPosY, SteppingFlag, onOffFlag);
					switchUpBlockCollision(&ghost, switchBlock, ghostPosX, SteppingFlag, onOffFlag);
					switchDwonBlockCollision(&ghost, switchBlock, ghostPosX, SteppingFlag, onOffFlag);

					//おばけとブロック
					if (StartFlag == true)
					{
						MapChipCondition(&ghost, &TileGhost, Stage7Map, touchNumber, touchNumberTimer, startSpeedStopFlag);

						//ゴール
						NoviceGoalMapChip(&ghost, &TileGhost, Stage7Map, stageClearFlag);

						//矢印とおばけ

						//右矢印
						allCollision(&ghost, &rightArrow, mouseMoveFlag, -arrowSpeedX, spped_0, startSpeedStopFlag);

						//左矢印
						allCollision(&ghost, &leftArrow, mouseMoveFlag, arrowSpeedX, spped_0, startSpeedStopFlag);

						//上矢印
						allCollision(&ghost, &upArrow, mouseMoveFlag, spped_0, -arrowSpeedY, startSpeedStopFlag);

						//下矢印
						allCollision(&ghost, &dwonArrow, mouseMoveFlag, spped_0, arrowSpeedY, startSpeedStopFlag);
					}

					///==========================================================================
					//マウスの当たり判定の処理
					///==========================================================================


					if (StartFlag == false)
					{
						mouseCollision(&rightArrow, mousePosX, mousePosY, mouseClickFlag);
						mouseCollision(&leftArrow, mousePosX, mousePosY, mouseClickFlag);
						mouseCollision(&upArrow, mousePosX, mousePosY, mouseClickFlag);
						mouseCollision(&dwonArrow, mousePosX, mousePosY, mouseClickFlag);
					}

					if (SceneChangeTimer > HalfTimer && mouseMoveFlag == false)
					{
						//スタートボタン
						mouseCollision(&StartKey, mousePosX, mousePosY, mouseClickFlag);
					}

					if (StartFlag == true)
					{
						//リトライボタン
						mouseCollision(&retry, mousePosX, mousePosY, mouseClickFlag);
					}

					if (stageClearFlag == true)//クリアしたら
					{
						//次へ進むボタン
						mouseCollision(&nextStage, mousePosX, mousePosY, mouseClickFlag);
						//タイトルへ
						mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
					}

					mouseGetCondition(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2, mousePosX, mousePosY, mouseMoveFlag, mouseClickFlag);

					//スタートキー
					if (StartKey.Flag == true)
					{
						StartFlag = true;
					}

					if (StartFlag == true)
					{
						StartKey.Flag = false;
					}

					//retryキー
					if (retry.Flag == true)
					{
						StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
						clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
						startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
						StartFlag = false;//スタートするかどうかのフラグ
						stageSeven.Flag = false;
						rockReleaseSE.Flag = false;
						playerReset(&ghost, blockSize, playerSize, ghostRadius, 6, 1, 0.0f, 0.0f);
						onOffFlag = false;
						BlockHp = 3;
						touchNumber = 0;
						arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

						retry.Flag = false;
					}


					if (stageClearFlag == true)//ステージクリアかつ
					{
						if (title.Flag == true)
						{
							title.Flag = false;
							//シーンの切り替え
							SceneChange(SceneChangeTimer, HalfTimer, Title);
						}

						if (nextStage.Flag == true)//押されたら
						{
							clickSE.Flag = true;

							if (clickSE.Flag == true)//SEを鳴らして
							{
								///==========================================================================
								//初期化
								///==========================================================================
								StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
								clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
								startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
								StartFlag = false;//スタートするかどうかのフラグ
								playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 3, 0.0f, 0.0f);
								blockReset(&key, blockSize, 3, 5);
								key.position.x = 0;
								key.position.y = 0;
								KeyFlag = true;
								onOffFlag = false;
								touchNumber = 0;
								arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

								clearGhostPosX = kWindowWidth;
								clearNisitoPosX = kWindowWidth;

								stageClearFlag = false;
								nextStage.Flag = false;
								//シーンの切り替え
								SceneChange(SceneChangeTimer, HalfTimer, stage8);
							}

						}
					}

					///==========================================================================
					//ステージクリアの更新処理;
					///==========================================================================

					if (stageClearFlag == true)
					{
						clearGhostPosX -= clearGhostSpeed;
						if (clearGhostPosX < 0 - playerSize)
						{
							clearNisitoPosX -= clearGhostSpeed * 5;
						}
				}

			break;

					case stage8:

						///==============================================================================================================================================
						//ステージ8
						///===============================================================================================================================================


						///==========================================================================
						//座標の更新処理
						///==========================================================================

						playerRum(&ghost, &TileGhost, ghostPosX, ghostPosY, blockSize);
						BlockRum(&key);

						///==========================================================================
						//おばけの更新処理
						///==========================================================================

						ghost.position.x = speed(-ghost.speed.x, ghost.position.x);
						ghost.position.y = speed(ghost.speed.y, ghost.position.y);

						if (StartFlag == false)
						{
							ghost.speed.x = 0;
							ghost.speed.y = 0;
						}

						if (StartFlag == true)
						{
							StartSETimer++;

						}

						if (startSpeedStopFlag == false && StartFlag == true)
						{
							ghost.speed.x = -10;
						}

						///==========================================================================
						//鍵の更新処理
						///==========================================================================

						if (StartFlag == false)
						{
							keySpeed(&key, blockSize, Zeo, keySpeedY, 3, 7, StartFlag);
						}

						///==========================================================================
						//マウスを押した時の処理
						///==========================================================================

						if (Novice::IsPressMouse(0)) {
							mouseConutTimer++;
							mouseClickFlag = true;
						}
						else
						{
							mouseConutTimer = 0;
							mouseClickFlag = false;
						}

						///==========================================================================
						//マップの当たり判定の処理
						///==========================================================================

						//おばけとブロック
						if (StartFlag == true)
						{
							MapChipCondition(&ghost, &TileGhost, Stage8Map, touchNumber, touchNumberTimer, startSpeedStopFlag);
							if (KeyFlag == false)
							{
								//ゴール
								NoviceGoalMapChip(&ghost, &TileGhost, Stage8Map, stageClearFlag);
							}

							//矢印とおばけ

							//下矢印
							allCollision(&ghost, &dwonArrow, mouseMoveFlag, spped_0, arrowSpeedY, startSpeedStopFlag);

							//カギ
							RightKeyCollision(&ghost, key, ghostPosY, mouseMoveFlag, KeyFlag);
							LeftKeyCollision (&ghost, key, ghostPosY, mouseMoveFlag, KeyFlag);
							UpKeyCollision   (&ghost, key, ghostPosX, mouseMoveFlag, KeyFlag);
							DwonKeyCollision (&ghost, key, ghostPosX, mouseMoveFlag, KeyFlag);
						}

						///==========================================================================
						//マウスの当たり判定の処理
						///==========================================================================


						if (StartFlag == false)
						{
							mouseCollision(&rightArrow, mousePosX, mousePosY, mouseClickFlag);
							mouseCollision(&leftArrow, mousePosX, mousePosY, mouseClickFlag);
							mouseCollision(&upArrow, mousePosX, mousePosY, mouseClickFlag);
							mouseCollision(&dwonArrow, mousePosX, mousePosY, mouseClickFlag);
						}

						if (SceneChangeTimer > HalfTimer && mouseMoveFlag == false)
						{
							//スタートボタン
							mouseCollision(&StartKey, mousePosX, mousePosY, mouseClickFlag);
						}

						if (StartFlag == true)
						{
							//リトライボタン
							mouseCollision(&retry, mousePosX, mousePosY, mouseClickFlag);
						}

						if (stageClearFlag == true)//クリアしたら
						{
							//次へ進むボタン
							mouseCollision(&nextStage, mousePosX, mousePosY, mouseClickFlag);
							//タイトルへ
							mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
						}

						mouseGetCondition(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2, mousePosX, mousePosY, mouseMoveFlag, mouseClickFlag);

						//スタートキー
						if (StartKey.Flag == true)
						{
							StartFlag = true;
						}

						if (StartFlag == true)
						{
							StartKey.Flag = false;
						}

						//retryキー
						if (retry.Flag == true)
						{
							StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
							clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
							startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
							StartFlag = false;//スタートするかどうかのフラグ
							stageEight.Flag = false;
							rockReleaseSE.Flag = false;
							playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 3, 0.0f, 0.0f);
							blockReset(&key, blockSize, 3, 6);
							key.position.x = 0;
							key.position.y = 0;
							KeyFlag = true;

							onOffFlag = false;
							BlockHp = 3;
							touchNumber = 0;
							arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

							retry.Flag = false;
						}


						if (stageClearFlag == true)//ステージクリアかつ
						{
							if (title.Flag == true)
							{
								title.Flag = false;
								//シーンの切り替え
								SceneChange(SceneChangeTimer, HalfTimer, Title);
							}

							if (nextStage.Flag == true)//押されたら
							{
								clickSE.Flag = true;

								if (clickSE.Flag == true)//SEを鳴らして
								{
									///==========================================================================
									//初期化
									///==========================================================================
									StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
									clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
									startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
									StartFlag = false;//スタートするかどうかのフラグ
									playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 4, 0.0f, 0.0f);
									blockReset(&key, blockSize, 5, 6);

									key.position.x = 0;
									key.position.y = 0;
									KeyFlag = true;
									onOffFlag = false;
									touchNumber = 0;
									BlockHp = 0;
									arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

									clearGhostPosX = kWindowWidth;
									clearNisitoPosX = kWindowWidth;

									stageClearFlag = false;
									nextStage.Flag = false;
									//シーンの切り替え
									SceneChange(SceneChangeTimer, HalfTimer, stage9);
								}

							}
						}

						///==========================================================================
						//ステージクリアの更新処理;
						///==========================================================================

						if (stageClearFlag == true)
						{
							clearGhostPosX -= clearGhostSpeed;
							if (clearGhostPosX < 0 - playerSize)
							{
								clearNisitoPosX -= clearGhostSpeed * 5;
							}
						}

			break;

		case stage9:

			///==============================================================================================================================================
			//ステージ9
			///===============================================================================================================================================


			///==========================================================================
			//座標の更新処理
			///==========================================================================

			playerRum(&ghost, &TileGhost, ghostPosX, ghostPosY, blockSize);
			BlockRum(&key);

			///==========================================================================
			//おばけの更新処理
			///==========================================================================

			ghost.position.x = speed(-ghost.speed.x, ghost.position.x);
			ghost.position.y = speed(ghost.speed.y, ghost.position.y);

			if (StartFlag == false)
			{
				ghost.speed.x = 0;
				ghost.speed.y = 0;
			}

			if (StartFlag == true)
			{
				StartSETimer++;

			}

			if (startSpeedStopFlag == false && StartFlag == true)
			{
				ghost.speed.x = -10;
			}

			///==========================================================================
			//鍵の更新処理
			///==========================================================================

			if (StartFlag == false)
			{
				keySpeed(&key, blockSize, keySpeedX, Zeo, 4, 7, StartFlag);
			}

			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}

			///==========================================================================
			//マップの当たり判定の処理
			///==========================================================================

			//おばけとブロック
			if (StartFlag == true)
			{
				MapChipCondition(&ghost, &TileGhost, Stage9Map, touchNumber, touchNumberTimer, startSpeedStopFlag);
				if (KeyFlag == false)
				{
					//ゴール
					NoviceGoalMapChip(&ghost, &TileGhost, Stage9Map, stageClearFlag);
				}

				//矢印とおばけ

				//右矢印
				allCollision(&ghost, &rightArrow, mouseMoveFlag, -arrowSpeedX, spped_0, startSpeedStopFlag);

				//左矢印
				allCollision(&ghost, &leftArrow, mouseMoveFlag, arrowSpeedX, spped_0, startSpeedStopFlag);

				//上矢印
				allCollision(&ghost, &upArrow, mouseMoveFlag, spped_0, -arrowSpeedY, startSpeedStopFlag);

				//下矢印
				allCollision(&ghost, &dwonArrow, mouseMoveFlag, spped_0, arrowSpeedY, startSpeedStopFlag);

				
				//カギ
				RightKeyCollision(&ghost, key, ghostPosY, mouseMoveFlag, KeyFlag);
				LeftKeyCollision(&ghost, key, ghostPosY, mouseMoveFlag, KeyFlag);
				UpKeyCollision(&ghost, key, ghostPosX, mouseMoveFlag, KeyFlag);
				DwonKeyCollision(&ghost, key, ghostPosX, mouseMoveFlag, KeyFlag);
			}

			///==========================================================================
			//マウスの当たり判定の処理
			///==========================================================================


			if (StartFlag == false)
			{
				mouseCollision(&rightArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&leftArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&upArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&dwonArrow, mousePosX, mousePosY, mouseClickFlag);
			}

			if (SceneChangeTimer > HalfTimer && mouseMoveFlag == false)
			{
				//スタートボタン
				mouseCollision(&StartKey, mousePosX, mousePosY, mouseClickFlag);
			}

			if (StartFlag == true)
			{
				//リトライボタン
				mouseCollision(&retry, mousePosX, mousePosY, mouseClickFlag);
			}

			if (stageClearFlag == true)//クリアしたら
			{
				//次へ進むボタン
				mouseCollision(&nextStage, mousePosX, mousePosY, mouseClickFlag);
				//タイトルへ
				mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
			}

			mouseGetCondition(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2, mousePosX, mousePosY, mouseMoveFlag, mouseClickFlag);

			//スタートキー
			if (StartKey.Flag == true)
			{
				StartFlag = true;
			}

			if (StartFlag == true)
			{
				StartKey.Flag = false;
			}

			//retryキー
			if (retry.Flag == true)
			{
				StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
				clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
				startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
				StartFlag = false;//スタートするかどうかのフラグ
				stageEight.Flag = false;
				rockReleaseSE.Flag = false;
				playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 4, 0.0f, 0.0f);
				blockReset(&key, blockSize, 4, 6);
				key.position.x = 0;
				key.position.y = 0;
				KeyFlag = true;

				onOffFlag = false;
				BlockHp = 3;
				touchNumber = 0;
				arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

				retry.Flag = false;
			}


			if (stageClearFlag == true)//ステージクリアかつ
			{
				if (title.Flag == true)
				{
					title.Flag = false;
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, Title);
				}

				if (nextStage.Flag == true)//押されたら
				{
					clickSE.Flag = true;

					if (clickSE.Flag == true)//SEを鳴らして
					{
						///==========================================================================
						//初期化
						///==========================================================================
						StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
						clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
						startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
						StartFlag = false;//スタートするかどうかのフラグ
						playerReset(&ghost, blockSize, playerSize, ghostRadius, 5, 1, 0.0f, 0.0f);
						blockReset(&key, blockSize, 1, 5);
						blockReset(&breakableBlocks, blockSize, 10, 6);
						blockReset(&switchBlock, blockSize, 4, 5);
						blockReset(&onOffBlock, blockSize, 2, 6);
						blockReset(&onOffBlock2, blockSize, 11, 4);

						key.position.x = 0;
						key.position.y = 0;
						KeyFlag = true;
						onOffFlag = false;
						touchNumber = 1;
						BlockHp = 3;
						arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

						clearGhostPosX = kWindowWidth;
						clearNisitoPosX = kWindowWidth;

						stageClearFlag = false;
						nextStage.Flag = false;
						//シーンの切り替え
						SceneChange(SceneChangeTimer, HalfTimer, stage10);
					}

				}
			}

			///==========================================================================
			//ステージクリアの更新処理;
			///==========================================================================

			if (stageClearFlag == true)
			{
				clearGhostPosX -= clearGhostSpeed;
				if (clearGhostPosX < 0 - playerSize)
				{
					clearNisitoPosX -= clearGhostSpeed * 5;
				}
			}

			break;



			break;

		case stage10:

			///==============================================================================================================================================
			//ステージ10
			///===============================================================================================================================================


			///==========================================================================
			//座標の更新処理
			///==========================================================================

			playerRum(&ghost, &TileGhost, ghostPosX, ghostPosY, blockSize);
			BlockRum(&key);

			///==========================================================================
			//おばけの更新処理
			///==========================================================================

			ghost.position.x = speed(-ghost.speed.x, ghost.position.x);
			ghost.position.y = speed(ghost.speed.y, ghost.position.y);

			if (StartFlag == false)
			{
				ghost.speed.x = 0;
				ghost.speed.y = 0;
			}

			if (StartFlag == true)
			{
				StartSETimer++;

			}

			if (startSpeedStopFlag == false && StartFlag == true)
			{
				ghost.speed.x = 10;
			}

			///==========================================================================
			//鍵の更新処理
			///==========================================================================

			if (StartFlag == false)
			{
				keySpeed(&key, blockSize, Zeo, keySpeedY, 3, 7, StartFlag);
			}

			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}

			///==========================================================================
			//マップの当たり判定の処理
			///==========================================================================

			//おばけとブロック
			if (StartFlag == true)
			{
				MapChipCondition(&ghost, &TileGhost, Stage10Map, touchNumber, touchNumberTimer, startSpeedStopFlag);
				if (KeyFlag == false)
				{
					//ゴール
					NoviceGoalMapChip(&ghost, &TileGhost, Stage10Map, stageClearFlag);
				}

				//壊れるブロック
				if (BlockHp != 0)
				{
					RightBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, BlockHp, BlockTimer);
					LeftBlockCollision(&ghost, breakableBlocks, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, BlockHp, BlockTimer);
				}

				if (onOffFlag == false)
				{
					//オンオフブロック
					onOffRightBlockCollision(&ghost, onOffBlock, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, onOffFlag);
					onOffLeftBlockCollision(&ghost, onOffBlock, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, onOffFlag);
					onOffUpBlockCollision(&ghost, onOffBlock, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, -arrowSpeedY, onOffFlag);
					onOffDwonBlockCollision(&ghost, onOffBlock, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, arrowSpeedY, onOffFlag);

					onOffRightBlockCollision(&ghost, onOffBlock2, ghostPosY, mouseMoveFlag, startSpeedStopFlag, arrowSpeedX, 0, onOffFlag);
					onOffLeftBlockCollision(&ghost, onOffBlock2, ghostPosY, mouseMoveFlag, startSpeedStopFlag, -arrowSpeedX, 0, onOffFlag);
					onOffUpBlockCollision(&ghost, onOffBlock2, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, -arrowSpeedY, onOffFlag);
					onOffDwonBlockCollision(&ghost, onOffBlock2, ghostPosX, mouseMoveFlag, startSpeedStopFlag, 0, arrowSpeedY, onOffFlag);
				}

				//スイッチブロック
				switchRightBlockCollision(&ghost, switchBlock, ghostPosY, SteppingFlag, onOffFlag);
				switchLeftBlockCollision(&ghost, switchBlock, ghostPosY, SteppingFlag, onOffFlag);
				switchUpBlockCollision(&ghost, switchBlock, ghostPosX, SteppingFlag, onOffFlag);
				switchDwonBlockCollision(&ghost, switchBlock, ghostPosX, SteppingFlag, onOffFlag);

				//矢印とおばけ

				//右矢印
				allCollision(&ghost, &rightArrow, mouseMoveFlag, -arrowSpeedX, spped_0, startSpeedStopFlag);

				//左矢印
				allCollision(&ghost, &leftArrow, mouseMoveFlag, arrowSpeedX, spped_0, startSpeedStopFlag);

				//上矢印
				allCollision(&ghost, &upArrow, mouseMoveFlag, spped_0, -arrowSpeedY, startSpeedStopFlag);

				//下矢印
				allCollision(&ghost, &dwonArrow, mouseMoveFlag, spped_0, arrowSpeedY, startSpeedStopFlag);
				allCollision(&ghost, &dwonArrow2, mouseMoveFlag, spped_0, arrowSpeedY, startSpeedStopFlag);



				
				//カギ
				RightKeyCollision(&ghost, key, ghostPosY, mouseMoveFlag, KeyFlag);
				LeftKeyCollision(&ghost, key, ghostPosY, mouseMoveFlag, KeyFlag);
				UpKeyCollision(&ghost, key, ghostPosX, mouseMoveFlag, KeyFlag);
				DwonKeyCollision(&ghost, key, ghostPosX, mouseMoveFlag, KeyFlag);
			}

			///==========================================================================
			//マウスの当たり判定の処理
			///==========================================================================


			if (StartFlag == false)
			{
				mouseCollision(&rightArrow, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&leftArrow , mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&upArrow   , mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&dwonArrow,  mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&dwonArrow2, mousePosX, mousePosY, mouseClickFlag);
			}

			if (SceneChangeTimer > HalfTimer && mouseMoveFlag == false)
			{
				//スタートボタン
				mouseCollision(&StartKey, mousePosX, mousePosY, mouseClickFlag);
			}

			if (StartFlag == true)
			{
				//リトライボタン
				mouseCollision(&retry, mousePosX, mousePosY, mouseClickFlag);
			}

			if (stageClearFlag == true)//クリアしたら
			{
				
				//タイトルへ
				mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
			}

			if (stageClearFlag == true)//クリアしたら
			{
				if (nextStage.Flag == true)//押されたら
				{
					clickSE.Flag = true;

					if (clickSE.Flag == true)//SEを鳴らして
					{
						///==========================================================================
						//初期化
						///==========================================================================
						StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
						clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
						startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
						StartFlag = false;//スタートするかどうかのフラグ
						playerReset(&ghost, blockSize, playerSize, ghostRadius, 1, 5, 0.0f, 0.0f);
						onOffFlag = false;
						touchNumber = 0;
						arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

						clearGhostPosX = kWindowWidth;
						clearNisitoPosX = kWindowWidth;

						stageClearFlag = false;
						nextStage.Flag = false;
						//シーンの切り替え
						SceneChange(SceneChangeTimer, HalfTimer, GameClear);
					}

				}
			}

			mouseGetCondition(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2, mousePosX, mousePosY, mouseMoveFlag, mouseClickFlag);

			//スタートキー
			if (StartKey.Flag == true)
			{
				StartFlag = true;
			}

			if (StartFlag == true)
			{
				StartKey.Flag = false;
			}

			//retryキー
			if (retry.Flag == true)
			{
				StartSETimer = 0;//ゲームのスタートでスタートキーを押した際にSEを切るためにあるタイマー
				clearBGMTimer = 0;//ゲームのクリアで鳴らしたBGMを切るためにあるタイマー
				startSpeedStopFlag = false;//スタートした時のスピードを止めるフラグ
				StartFlag = false;//スタートするかどうかのフラグ
				stageNine.Flag = false;
				playerReset(&ghost, blockSize, playerSize, ghostRadius, 3, 1, 0.0f, 0.0f);
				blockReset(&key, blockSize, 1, 5);
				key.position.x = 0;
				key.position.y = 0;
				KeyFlag = true;

				onOffFlag = false;
				BlockHp = 3;
				touchNumber = 1;
				arrowReset(&rightArrow, &leftArrow, &upArrow, &dwonArrow, &dwonArrow2);//矢印の位置のりせっとをする

				retry.Flag = false;
			}


			if (stageClearFlag == true)//ステージクリアかつ
			{
				if (title.Flag == true)
				{
					title.Flag = false;
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, Title);
				}
			}

			///==========================================================================
			//ステージクリアの更新処理;
			///==========================================================================

			if (stageClearFlag == true)
			{
				clearGhostPosX -= clearGhostSpeed;
				if (clearGhostPosX < 0 - playerSize)
				{
					clearNisitoPosX -= clearGhostSpeed * 5;
				}
			}

			break;
		case GameClear:

			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}


			//タイトルへ
			mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);


			if (title.Flag == true)
			{
				//シーンの切り替え
				SceneChange(SceneChangeTimer, HalfTimer, Title);
				title.Flag = false;
				stageOne.Flag = false;
				stageTwo.Flag = false;
				stageThree.Flag = false;
				stageFour.Flag = false;
				stageFive.Flag = false;
				stageSix.Flag = false;
				stageSeven.Flag = false;
				stageEight.Flag = false;
				stageNine.Flag = false;
				stageTen.Flag = false;
				touchNumber = 0;
			}


			break;
		case Setting:

			mouseCollision(&turnBack, mousePosX, mousePosY, mouseClickFlag);


			if (turnBack.Flag == true)
			{
				clickSE.Flag = true;
				if (clickSE.Flag == true)
				{
					SceneChange(SceneChangeTimer, HalfTimer, Title);
				}
				turnBack.Flag = false;
			}

			if (!Novice::IsTriggerMouse(0))
			{
				mouseCollision(&BGMupVolume, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&BGMdownVolume, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&SEupVolume, mousePosX, mousePosY, mouseClickFlag);
				mouseCollision(&SEdownVolume, mousePosX, mousePosY, mouseClickFlag);
			}

			if (BGMupVolume.Flag == true && mouseConutTimer == 0)
			{
				if (BGMupVolume.Flag == true)
				{
					if (BGMVolumeCount != 5)
					{
						BGMVolumeCount += 1;
					}
					BGMupVolume.Flag = false;
				}
			}

			if (BGMdownVolume.Flag == true && mouseConutTimer == 0)
			{
				if (BGMdownVolume.Flag == true)
				{
					if (BGMVolumeCount != 0)
					{
						BGMVolumeCount -= 1;
					}
					BGMdownVolume.Flag = false;
				}
			}

			if (SEupVolume.Flag == true && mouseConutTimer == 0)
			{
				if (SEupVolume.Flag == true)
				{
					if (SEVolumeCount != 5)
					{
						SEVolumeCount += 1;
					}
					SEupVolume.Flag = false;
				}
			}

			if (SEdownVolume.Flag == true && mouseConutTimer == 0)
			{
				if (SEdownVolume.Flag == true)
				{
					if (SEVolumeCount != 0)
					{
						SEVolumeCount -= 1;
					}
					SEdownVolume.Flag = false;
				}
			}

			BGMVolume = VolumeCount(BGMVolumeCount, BGMVolume, Zeo, Zeo);
			BGMVolume = VolumeCount(BGMVolumeCount, BGMVolume, One, One);
			BGMVolume = VolumeCount(BGMVolumeCount, BGMVolume, Two, Two);
			BGMVolume = VolumeCount(BGMVolumeCount, BGMVolume, Three, Three);
			BGMVolume = VolumeCount(BGMVolumeCount, BGMVolume, Four, Four);
			BGMVolume = VolumeCount(BGMVolumeCount, BGMVolume, Five, Five);
				
			SEVolume = VolumeCount(SEVolumeCount, SEVolume, Zeo, Zeo);
			SEVolume = VolumeCount(SEVolumeCount, SEVolume, One, One);
			SEVolume = VolumeCount(SEVolumeCount, SEVolume, Two, Two);
			SEVolume = VolumeCount(SEVolumeCount, SEVolume, Three, Three);
			SEVolume = VolumeCount(SEVolumeCount, SEVolume, Four, Four);
			SEVolume = VolumeCount(SEVolumeCount, SEVolume, Five, Five);
				
			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {//マウスをクリックしていたら
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}

			break;

			case GameOver:

			///==========================================================================
			//マウスを押した時の処理
			///==========================================================================

			if (Novice::IsPressMouse(0)) {
				mouseConutTimer++;
				mouseClickFlag = true;
			}
			else
			{
				mouseConutTimer = 0;
				mouseClickFlag = false;
			}


			///==========================================================================
			//ゲームオーバー時の移動の処理
			///==========================================================================

				clearGhostPosX  -= 2.0f;
				clearNisitoPosX -= 2.0f;
			
			///==========================================================================
			//シーン切り替えの処理
			///==========================================================================

			//タイトルへ
			mouseCollision(&title, mousePosX, mousePosY, mouseClickFlag);
		

				if (title.Flag == true)
				{
					//シーンの切り替え
					SceneChange(SceneChangeTimer, HalfTimer, Title);
					title.Flag = false;
					stageOne.Flag = false;
					stageTwo.Flag = false;
					stageThree.Flag = false;
					stageFour.Flag = false;
					stageFive.Flag = false;
					stageSix.Flag = false;
					stageSeven.Flag = false;
					stageEight.Flag = false;
					stageNine.Flag = false;
					stageTen.Flag = false;
					touchNumber = 0;
				}

				break;
		}

		if (Scene != GameOver)
		{
			if (touchNumber < 0)
			{
				clearGhostPosX = kWindowWidth;
				clearNisitoPosX = clearGhostPosX + playerSize;
				KeyFlag = false;
				Scene = GameOver;
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓音楽処理ここから
		///

		switch (Scene)
		{
		case Title:
			Novice::StopAudio(gameBGMHandle);//ゲームBGMを停止
			Novice::StopAudio(gameHandle);//ゲームBGMを停止
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(gameOverBGMHandle);//ゲームオーバーBGMを停止
			Novice::StopAudio(gameOverHandle);//ゲームオーバーBGMを停止
			Novice::StopAudio(clearBGMHandle);//ゲームクリアBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (titlePosY >= 0)//タイトルバーが降りきってたら
			{
				if (!Novice::IsPlayingAudio(titleHandle) || titleHandle == -1) {//タイトルBGMがなっていなかったら
					titleHandle = Novice::PlayAudio(titleBGMHandle, false, BGMVolume);//タイトルBGMを流す
				}
			}

			///==========================================================================
			// SE
			///==========================================================================

			SeFlow(&clickSE, clickHandle, clickSEHandle,SEVolume);//クリックのSEを流す

			break;

		case stageSelect:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (!Novice::IsPlayingAudio(titleHandle) || titleHandle == -1) {//タイトルBGMがなっていなかったら
				titleHandle = Novice::PlayAudio(titleBGMHandle, false, BGMVolume);//タイトルBGMを流す
			}

			///==========================================================================
			// SE
			///==========================================================================

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す

			break;

		case stage1:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (stageClearFlag == false)//クリアしてなかったら
			{
				if (!Novice::IsPlayingAudio(gameHandle) || gameHandle == -1) {
					gameHandle = Novice::PlayAudio(gameBGMHandle, false, BGMVolume);//ゲームBGMを流す
				}
			}

			if (stageClearFlag == true)//クリアしたら
			{
				Novice::StopAudio(gameBGMHandle);//タイトルBGMを停止
				Novice::StopAudio(gameHandle);//タイトルBGMを停止

				if (clearBGMTimer == 0)//ループしないBGMの時にタイマーの処理を追加するクリア後にタイマーは初期化
				{
					if (!Novice::IsPlayingAudio(clearHandle) || clearHandle == -1) {
						clearHandle = Novice::PlayAudio(clearBGMHandle, false, BGMVolume);//クリアBGMを流す
					}
				}
				clearBGMTimer++;
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す


			break;

		case stage2:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止
			Novice::StopAudio(clearBGMHandle);//クリアBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (stageClearFlag == false)//クリアしてなかったら
			{
				if (!Novice::IsPlayingAudio(gameHandle) || gameHandle == -1) {
					gameHandle = Novice::PlayAudio(gameBGMHandle, false, BGMVolume);//ゲームBGMを流す
				}
			}

			if (stageClearFlag == true)//クリアしたら
			{
				Novice::StopAudio(gameBGMHandle);//タイトルBGMを停止
				Novice::StopAudio(gameHandle);//タイトルBGMを停止

				if (clearBGMTimer == 0)//ループしないBGMの時にタイマーの処理を追加するクリア後にタイマーは初期化
				{
					if (!Novice::IsPlayingAudio(clearHandle) || clearHandle == -1) {
						clearHandle = Novice::PlayAudio(clearBGMHandle, false, BGMVolume);//クリアBGMを流す
					}
				}
				clearBGMTimer++;
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			if (retry.Flag == true)
			{
				retrySE.Flag = true;
			}

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す
			SeFlow(&retrySE, retryHandle, retrySEHandle, SEVolume);//リトライのSEを流す


			break;

		case stage3:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止
			Novice::StopAudio(clearBGMHandle);//クリアBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (stageClearFlag == false)//クリアしてなかったら
			{
				if (!Novice::IsPlayingAudio(gameHandle) || gameHandle == -1) {
					gameHandle = Novice::PlayAudio(gameBGMHandle, false, BGMVolume);//ゲームBGMを流す
				}
			}

			if (stageClearFlag == true)//クリアしたら
			{
				Novice::StopAudio(gameBGMHandle);//タイトルBGMを停止
				Novice::StopAudio(gameHandle);//タイトルBGMを停止

				if (clearBGMTimer == 0)//ループしないBGMの時にタイマーの処理を追加するクリア後にタイマーは初期化
				{
					if (!Novice::IsPlayingAudio(clearHandle) || clearHandle == -1) {
						clearHandle = Novice::PlayAudio(clearBGMHandle, false, BGMVolume);//クリアBGMを流す
					}
				}
				clearBGMTimer++;
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			if (retry.Flag == true)
			{
				retrySE.Flag = true;
			}

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す
			SeFlow(&retrySE, retryHandle, retrySEHandle, SEVolume);//リトライのSEを流す

			break;

		case stage4:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止
			Novice::StopAudio(clearBGMHandle);//クリアBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (stageClearFlag == false)//クリアしてなかったら
			{
				if (!Novice::IsPlayingAudio(gameHandle) || gameHandle == -1) {
					gameHandle = Novice::PlayAudio(gameBGMHandle, false, BGMVolume);//ゲームBGMを流す
				}
			}

			if (stageClearFlag == true)//クリアしたら
			{
				Novice::StopAudio(gameBGMHandle);//タイトルBGMを停止
				Novice::StopAudio(gameHandle);//タイトルBGMを停止

				if (clearBGMTimer == 0)//ループしないBGMの時にタイマーの処理を追加するクリア後にタイマーは初期化
				{
					if (!Novice::IsPlayingAudio(clearHandle) || clearHandle == -1) {
						clearHandle = Novice::PlayAudio(clearBGMHandle, false, BGMVolume);//クリアBGMを流す
					}
				}
				clearBGMTimer++;
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			if (retry.Flag == true)
			{
				retrySE.Flag = true;
			}

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す
			SeFlow(&retrySE, retryHandle, retrySEHandle, SEVolume);//リトライのSEを流す

			break;

		case stage5:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止
			Novice::StopAudio(clearBGMHandle);//クリアBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (stageClearFlag == false)//クリアしてなかったら
			{
				if (!Novice::IsPlayingAudio(gameHandle) || gameHandle == -1) {
					gameHandle = Novice::PlayAudio(gameBGMHandle, false, BGMVolume);//ゲームBGMを流す
				}
			}

			if (stageClearFlag == true)//クリアしたら
			{
				Novice::StopAudio(gameBGMHandle);//タイトルBGMを停止
				Novice::StopAudio(gameHandle);//タイトルBGMを停止

				if (clearBGMTimer == 0)//ループしないBGMの時にタイマーの処理を追加するクリア後にタイマーは初期化
				{
					if (!Novice::IsPlayingAudio(clearHandle) || clearHandle == -1) {
						clearHandle = Novice::PlayAudio(clearBGMHandle, false, BGMVolume);//クリアBGMを流す
					}
				}
				clearBGMTimer++;
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			if (retry.Flag == true)
			{
				retrySE.Flag = true;
			}

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す
			SeFlow(&retrySE, retryHandle, retrySEHandle, SEVolume);//リトライのSEを流す

			break;
		
		case stage6:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止
			Novice::StopAudio(clearBGMHandle);//クリアBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (stageClearFlag == false)//クリアしてなかったら
			{
				if (!Novice::IsPlayingAudio(gameHandle) || gameHandle == -1) {
					gameHandle = Novice::PlayAudio(gameBGMHandle, false, BGMVolume);//ゲームBGMを流す
				}
			}

			if (stageClearFlag == true)//クリアしたら
			{
				Novice::StopAudio(gameBGMHandle);//タイトルBGMを停止
				Novice::StopAudio(gameHandle);//タイトルBGMを停止

				if (clearBGMTimer == 0)//ループしないBGMの時にタイマーの処理を追加するクリア後にタイマーは初期化
				{
					if (!Novice::IsPlayingAudio(clearHandle) || clearHandle == -1) {
						clearHandle = Novice::PlayAudio(clearBGMHandle, false, BGMVolume);//クリアBGMを流す
					}
				}
				clearBGMTimer++;
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			if (retry.Flag == true)
			{
				retrySE.Flag = true;
			}

			if (onOffFlag == true && switchSE.Timer == 0)
			{
				switchSE.Flag = true;
			}

			
			SeFlow(&switchSE, switchHandle, switchSEHandle, SEVolume);//スイッチのSEを流す
			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す
			SeFlow(&retrySE, retryHandle, retrySEHandle, SEVolume);//リトライのSEを流す

			if (onOffFlag == true && switchSE.Timer != 0)
			{
				switchSE.Flag = false;
			}

			break;

		case stage7:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止
			Novice::StopAudio(clearBGMHandle);//クリアBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (stageClearFlag == false)//クリアしてなかったら
			{
				if (!Novice::IsPlayingAudio(gameHandle) || gameHandle == -1) {
					gameHandle = Novice::PlayAudio(gameBGMHandle, false, BGMVolume);//ゲームBGMを流す
				}
			}

			if (stageClearFlag == true)//クリアしたら
			{
				Novice::StopAudio(gameBGMHandle);//タイトルBGMを停止
				Novice::StopAudio(gameHandle);//タイトルBGMを停止

				if (clearBGMTimer == 0)//ループしないBGMの時にタイマーの処理を追加するクリア後にタイマーは初期化
				{
					if (!Novice::IsPlayingAudio(clearHandle) || clearHandle == -1) {
						clearHandle = Novice::PlayAudio(clearBGMHandle, false, BGMVolume);//クリアBGMを流す
					}
				}
				clearBGMTimer++;
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			if (retry.Flag == true)
			{
				retrySE.Flag = true;
			}

			if (onOffFlag == true && switchSE.Timer == 0)
			{
				switchSE.Flag = true;
			}

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す
			SeFlow(&retrySE, retryHandle, retrySEHandle, SEVolume);//リトライのSEを流す
			SeFlow(&switchSE, switchHandle, switchSEHandle, SEVolume);//スイッチのSEを流す

			if (onOffFlag == true && switchSE.Timer != 0)
			{
				switchSE.Flag = false;
			}

			break;

		case stage8:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止
			Novice::StopAudio(clearBGMHandle);//クリアBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (stageClearFlag == false)//クリアしてなかったら
			{
				if (!Novice::IsPlayingAudio(gameHandle) || gameHandle == -1) {
					gameHandle = Novice::PlayAudio(gameBGMHandle, false, BGMVolume);//ゲームBGMを流す
				}
			}

			if (stageClearFlag == true)//クリアしたら
			{
				Novice::StopAudio(gameBGMHandle);//タイトルBGMを停止
				Novice::StopAudio(gameHandle);//タイトルBGMを停止

				if (clearBGMTimer == 0)//ループしないBGMの時にタイマーの処理を追加するクリア後にタイマーは初期化
				{
					if (!Novice::IsPlayingAudio(clearHandle) || clearHandle == -1) {
						clearHandle = Novice::PlayAudio(clearBGMHandle, false, BGMVolume);//クリアBGMを流す
					}
				}
				clearBGMTimer++;
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			if (retry.Flag == true)
			{
				retrySE.Flag = true;
			}

			if (KeyFlag == false && rockReleaseSE.Timer == 0)
			{
				rockReleaseSE.Flag = true;
			}

			SeFlow(&rockReleaseSE, rockReleaseHandle, rockReleaseSEHandle, SEVolume);//ロック解除のSEを流す
			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す
			SeFlow(&retrySE, retryHandle, retrySEHandle, SEVolume);//リトライのSEを流す

			if (KeyFlag == false && rockReleaseSE.Timer != 0)
			{
				rockReleaseSE.Flag = false;
			}

			break;

		case stage9:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止
			Novice::StopAudio(clearBGMHandle);//クリアBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (stageClearFlag == false)//クリアしてなかったら
			{
				if (!Novice::IsPlayingAudio(gameHandle) || gameHandle == -1) {
					gameHandle = Novice::PlayAudio(gameBGMHandle, false, BGMVolume);//ゲームBGMを流す
				}
			}

			if (stageClearFlag == true)//クリアしたら
			{
				Novice::StopAudio(gameBGMHandle);//タイトルBGMを停止
				Novice::StopAudio(gameHandle);//タイトルBGMを停止

				if (clearBGMTimer == 0)//ループしないBGMの時にタイマーの処理を追加するクリア後にタイマーは初期化
				{
					if (!Novice::IsPlayingAudio(clearHandle) || clearHandle == -1) {
						clearHandle = Novice::PlayAudio(clearBGMHandle, false, BGMVolume);//クリアBGMを流す
					}
				}
				clearBGMTimer++;
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			if (retry.Flag == true)
			{
				retrySE.Flag = true;
			}

			if (onOffFlag == true && switchSE.Timer != 0)
			{
				switchSE.Flag = true;
			}

			if (KeyFlag == false && rockReleaseSE.Timer == 0)
			{
				rockReleaseSE.Flag = true;
			}

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す
			SeFlow(&retrySE, retryHandle, retrySEHandle, SEVolume);//リトライのSEを流す
			SeFlow(&switchSE, switchHandle, switchSEHandle, SEVolume);//スイッチのSEを流す
			SeFlow(&rockReleaseSE, rockReleaseHandle, rockReleaseSEHandle, SEVolume);//ロック解除のSEを流す

			if (onOffFlag == true && switchSE.Timer != 0)
			{
				switchSE.Flag = false;
			}

			if (KeyFlag == false && rockReleaseSE.Timer != 0)
			{
				rockReleaseSE.Flag = false;
			}

			break;

		case stage10:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止
			Novice::StopAudio(clearBGMHandle);//クリアBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (stageClearFlag == false)//クリアしてなかったら
			{
				if (!Novice::IsPlayingAudio(gameHandle) || gameHandle == -1) {
					gameHandle = Novice::PlayAudio(gameBGMHandle, false, BGMVolume);//ゲームBGMを流す
				}
			}

			if (stageClearFlag == true)//クリアしたら
			{
				Novice::StopAudio(gameBGMHandle);//タイトルBGMを停止
				Novice::StopAudio(gameHandle);//タイトルBGMを停止

				if (clearBGMTimer == 0)//ループしないBGMの時にタイマーの処理を追加するクリア後にタイマーは初期化
				{
					if (!Novice::IsPlayingAudio(allClearHandle) || allClearHandle == -1) {
						clearHandle = Novice::PlayAudio(allClearBGMHandle, false, BGMVolume);//クリアBGMを流す
					}
				}
				clearBGMTimer++;
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			if (retry.Flag == true)
			{
				retrySE.Flag = true;
			}

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す
			SeFlow(&retrySE, retryHandle, retrySEHandle, SEVolume);//リトライのSEを流す

			break;

		case Setting:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (!Novice::IsPlayingAudio(titleHandle) || titleHandle == -1) {//タイトルBGMがなっていなかったら
				titleHandle = Novice::PlayAudio(titleBGMHandle, false, BGMVolume);//タイトルBGMを流す
			}

			///==========================================================================
			// SE
			///==========================================================================

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す

			break;
		case GameOver:
			Novice::StopAudio(titleBGMHandle);//タイトルBGMを停止
			Novice::StopAudio(titleHandle);//タイトルBGMを停止
			Novice::StopAudio(clearBGMHandle);//クリアBGMを停止
			Novice::StopAudio(gameBGMHandle);//ゲームBGMを停止
			Novice::StopAudio(gameHandle);//ゲームBGMを停止

			///==========================================================================
			// BGM
			///==========================================================================

			if (!Novice::IsPlayingAudio(gameOverHandle) || gameOverHandle == -1) {//タイトルBGMがなっていなかったら
				gameOverHandle = Novice::PlayAudio(gameOverBGMHandle, false, BGMVolume);//タイトルBGMを流す
			}

			///==========================================================================
			// SE
			///==========================================================================

			if (StartFlag == true && StartSETimer == 0)//スタートを押したら
			{
				clickSE.Flag = true;
			}

			if (retry.Flag == true)
			{
				retrySE.Flag = true;
			}

			SeFlow(&clickSE, clickHandle, clickSEHandle, SEVolume);//クリックのSEを流す
			SeFlow(&retrySE, retryHandle, retrySEHandle, SEVolume);//リトライのSEを流す

			break;

		}


		///
		/// ↑音楽処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///==========================================================================
		//パーティクルの描画
		///==========================================================================

		Novice::SetBlendMode(BlendMode::kBlendModeNormal);//ブレンドのReset
		Novice::SetBlendMode(BlendMode::kBlendModeAdd);

		if (ghost.speed.x != 0 || ghost.speed.y != 0) {
			for (int a = 0; a < MAX; a++)
			{
				if (isShot[a] == true)
				{
					if (color[a] > 1)
					{
						Novice::DrawSprite(particlePosX[a], particlePosY[a], GhostImges, 1.0f, 1.0f, 0.0f, color[a]);//パーティクルの描画
					}
				}

			}
		}

		Novice::SetBlendMode(BlendMode::kBlendModeNormal);//ブレンドのReset

		Novice::DrawSprite(0, 0, titleBackgroundImges, 1.0f, 1.0f, 0.0f, WHITE);

		switch (Scene)
		{
		case Title:

			Novice::DrawSprite(0, (int)titlePosY, titleImges, 1, 1, 0.0f, WHITE);

			NoviceDrawQuadBlock(setting, playerSize * 2, playerSize * 2, settingImges);

			if (titlePosY <= 0) {
				titlePosY += 5;
				Novice::DrawSprite(0, 0, backgroundImges, 1, 1, 0.0f, 0xFFFF99);//薄暗くする
				Novice::DrawSprite(0, 0, attentionImges, 1, 1, 0.0f, WHITE);//注意書き
				
			}
			else
			{
				NoviceDrawQuadBlock(gameStartKey, blockSize * 2, blockSize - 48, gameStartImges);
				Novice::DrawSprite((int)kWindowWidth - blockSize * 4, (int)kWindowHeight - blockSize * 2 -70, mouseImges, 2, 2, 0.0f, WHITE);
				Novice::DrawSprite(titleGhostPosX, titleGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(titlePlayerPosX, titleGhostPosY, playerImges, 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(blockSize * 3, titleGhostPosY - 64, goalImges, 1, 1, 0.0f, WHITE);
			}


			break;

		case stageSelect:

			Novice::DrawSprite(0, 0, stageSelectImges, 1, 1, 0.0f, WHITE);

			NoviceDrawQuadBlock(stageOne, blockSize, blockSize, stageOneImges);//ステージ1
			NoviceDrawQuadBlock(stageTwo, blockSize, blockSize, stageTwoImges);//ステージ2
			NoviceDrawQuadBlock(stageThree, blockSize, blockSize, stageThreeImges);//ステージ3
			NoviceDrawQuadBlock(stageFour, blockSize, blockSize, stageFourImges);//ステージ4
			NoviceDrawQuadBlock(stageFive, blockSize, blockSize, stageFiveImges);//ステージ5
			NoviceDrawQuadBlock(stageSix, blockSize, blockSize, stageSixImges);//ステージ6
			NoviceDrawQuadBlock(stageSeven, blockSize, blockSize, stageSevenImges);//ステージ7
			NoviceDrawQuadBlock(stageEight, blockSize, blockSize, stageEightImges);//ステージ8
			NoviceDrawQuadBlock(stageNine, blockSize, blockSize, stageNineImges);//ステージ9
			NoviceDrawQuadBlock(stageTen, blockSize, blockSize, stageTenImges);//ステージ10

			break;

		case stage1:
			
			///==========================================================================
			//マップタイルの描画
			///==========================================================================

			//マップタイルの描画
			NoviceMapChip(KeyFlag,mapX, mapY, Stage1Map, GraveImges, goalImges, rockGoalImges);

			//おばけの描画
			NoviceDrawQuadPlayer(ghost, playerSize, GhostImges);

			//残り壁数
			Novice::DrawSprite(0, 0, residueImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(128, 0, suuziImges[touchNumber], 1, 1, 0.0f, WHITE);

			if (StartFlag == false)//スタート前に表示する
			{
				NoviceDrawQuadBlock(StartKey, 655, 220, StartImges);
				Novice::DrawSprite((int)ghost.LeftTop.x - blockSize, (int)ghost.LeftTop.y - ghostRadius, ghostLeftArrowImges, 1, 1, 0.0f, WHITE);
			}

			Novice::DrawSprite(blockSize * 5, 0, stageNumbertImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((blockSize * 7) + 20, 0, suuziImges[1], 1, 1, 0.0f, WHITE);

			///==========================================================================
			//ステージクリアの描画処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				Novice::DrawSprite(0, 0, stageClearImges, 1, 1, 0.0f, WHITE);//ゲームクリアの文字を出力

				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//ステージセレクトへ
				NoviceDrawQuadBlock(nextStage, 655, 220, nextStageImges);//次のステージへ

				Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
				Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト
			}

			if (GameOverFlag == true)
			{
				Novice::DrawSprite(0, 0, gameOverImges, 1, 1, 0.0f, WHITE);
				NoviceDrawQuadBlock(retryKey, 655, 220, retryKeyImges);//リトライ
				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//Titleへ
			}

			break;

		case stage2:

			///==========================================================================
			//マップタイルの描画
			///==========================================================================

				//マップタイルの描画
			NoviceMapChip(KeyFlag, mapX, mapY, Stage2Map, GraveImges, goalImges, rockGoalImges);

			//下矢印
			NoviceDrawQuadBlock(dwonArrow, blockSize, blockSize, dwonArrowImges);

			//おばけの描画
			NoviceDrawQuadPlayer(ghost, playerSize, GhostImges);

			//残り壁数
			Novice::DrawSprite(0, 0, residueImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(128, 0, suuziImges[touchNumber], 1, 1, 0.0f, WHITE);

			//スタートボタン
			if (StartFlag == false)
			{
				NoviceDrawQuadBlock(StartKey, 655, 220, StartImges);
				Novice::DrawSprite((int)ghost.LeftTop.x - blockSize, (int)ghost.LeftTop.y - ghostRadius, ghostLeftArrowImges, 1, 1, 0.0f, WHITE);
			}

			Novice::DrawSprite(blockSize * 5, 0, stageNumbertImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((blockSize * 7) + 20, 0, suuziImges[2], 1, 1, 0.0f, WHITE);


			//リトライボタン
			NoviceDrawQuadBlock(retry, 512, 256, retryImges);
			
			///==========================================================================
			//ステージクリアの描画処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				Novice::DrawSprite(0, 0, stageClearImges, 1, 1, 0.0f, WHITE);//ゲームクリアの文字を出力

				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//ステージセレクトへ
				NoviceDrawQuadBlock(nextStage, 655, 220, nextStageImges);//次のステージへ

				Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
				Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト
			}

			break;

		case stage3:

			///==========================================================================
			//マップタイルの描画
			///==========================================================================

				//マップタイルの描画
			NoviceMapChip(KeyFlag, mapX, mapY, Stage3Map, GraveImges, goalImges, rockGoalImges);

			//右矢印
			NoviceDrawQuadBlock(rightArrow, blockSize, blockSize, rightArrowImges);

			//左矢印
			NoviceDrawQuadBlock(leftArrow, blockSize, blockSize, leftArrowImges);

			//上矢印
			NoviceDrawQuadBlock(upArrow, blockSize, blockSize, upArrowImges);

			//下矢印
			NoviceDrawQuadBlock(dwonArrow, blockSize, blockSize, dwonArrowImges);

			//おばけの描画
			NoviceDrawQuadPlayer(ghost, playerSize, GhostImges);

			Novice::DrawSprite(blockSize * 5, 0, stageNumbertImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((blockSize * 7) + 20, 0, suuziImges[3], 1, 1, 0.0f, WHITE);


			//残り壁数
			Novice::DrawSprite(0, 0, residueImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(128, 0, suuziImges[touchNumber], 1, 1, 0.0f, WHITE);

			//スタートボタン
			if (StartFlag == false)
			{
				NoviceDrawQuadBlock(StartKey, 655, 220, StartImges);
				Novice::DrawSprite((int)ghost.RightTop.x, (int)ghost.RightTop.y - ghostRadius, ghostRightArrowImges, 1, 1, 0.0f, WHITE);
			}

			//リトライボタン
			NoviceDrawQuadBlock(retry, 512, 256, retryImges);

			///==========================================================================
			//ステージクリアの描画処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				Novice::DrawSprite(0, 0, stageClearImges, 1, 1, 0.0f, WHITE);//ゲームクリアの文字を出力

				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//ステージセレクトへ
				NoviceDrawQuadBlock(nextStage, 655, 220, nextStageImges);//次のステージへ

				Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
				Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト
			}

			break;

		case stage4:

			///==========================================================================
			//マップタイルの描画
			///==========================================================================

			//マップタイルの描画
			NoviceMapChip(KeyFlag, mapX, mapY, Stage4Map, GraveImges, goalImges, rockGoalImges);

			//右矢印
			NoviceDrawQuadBlock(rightArrow, blockSize, blockSize, rightArrowImges);

			//左矢印
			NoviceDrawQuadBlock(leftArrow, blockSize, blockSize, leftArrowImges);

			//上矢印
			NoviceDrawQuadBlock(upArrow, blockSize, blockSize, upArrowImges);

			//下矢印
			NoviceDrawQuadBlock(dwonArrow, blockSize, blockSize, dwonArrowImges);

			//壊せるブロックの描画
			NoviceBreakDrawQuadBlock(breakableBlocks, BlockHp, blockSize, breakableBlocksImges, breakableBlocks2Imges, breakableBlocks3Imges);

			//おばけの描画
			NoviceDrawQuadPlayer(ghost, playerSize, GhostImges);

			Novice::DrawSprite(blockSize * 5, 0, stageNumbertImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((blockSize * 7) + 20, 0, suuziImges[4], 1, 1, 0.0f, WHITE);


			//残り壁数
			Novice::DrawSprite(0, 0, residueImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(128, 0, suuziImges[touchNumber], 1, 1, 0.0f, WHITE);

			//スタートボタン
			if (StartFlag == false)
			{
				NoviceDrawQuadBlock(StartKey, 655, 220, StartImges);
				Novice::DrawSprite((int)ghost.LeftTop.x - ghostRadius, (int)ghost.LeftBottom.y, ghostDwonArrowImges, 1, 1, 0.0f, WHITE);
			}

			//リトライボタン
			NoviceDrawQuadBlock(retry, 512, 256, retryImges);

			///==========================================================================
			//ステージクリアの描画処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				Novice::DrawSprite(0, 0, stageClearImges, 1, 1, 0.0f, WHITE);//ゲームクリアの文字を出力

				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//ステージセレクトへ
				NoviceDrawQuadBlock(nextStage, 655, 220, nextStageImges);//次のステージへ

				Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
				Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト
			}

			break;

		case stage5:

			///==========================================================================
			//マップタイルの描画
			///==========================================================================

			//マップタイルの描画
			NoviceMapChip(KeyFlag, mapX, mapY, Stage5Map, GraveImges, goalImges, rockGoalImges);

			//壊せるブロックの描画
			NoviceBreakDrawQuadBlock(breakableBlocks, BlockHp, blockSize, breakableBlocksImges, breakableBlocks2Imges, breakableBlocks3Imges);

			//右矢印
			NoviceDrawQuadBlock(rightArrow, blockSize, blockSize, rightArrowImges);

			//左矢印
			NoviceDrawQuadBlock(leftArrow, blockSize, blockSize, leftArrowImges);

			//上矢印
			NoviceDrawQuadBlock(upArrow, blockSize, blockSize, upArrowImges);

			//下矢印
			NoviceDrawQuadBlock(dwonArrow, blockSize, blockSize, dwonArrowImges);

			//おばけの描画
			NoviceDrawQuadPlayer(ghost, playerSize, GhostImges);

			Novice::DrawSprite(blockSize * 5, 0, stageNumbertImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((blockSize * 7) + 20, 0, suuziImges[5], 1, 1, 0.0f, WHITE);


			//残り壁数
			Novice::DrawSprite(0, 0, residueImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(128, 0, suuziImges[touchNumber], 1, 1, 0.0f, WHITE);

			//スタートボタン
			if (StartFlag == false)
			{
				NoviceDrawQuadBlock(StartKey, 655, 220, StartImges);
				Novice::DrawSprite((int)ghost.RightTop.x, (int)ghost.RightTop.y - ghostRadius, ghostRightArrowImges, 1, 1, 0.0f, WHITE);
			}

			//リトライボタン
			NoviceDrawQuadBlock(retry, 512, 256, retryImges);

			///==========================================================================
			//ステージクリアの描画処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				Novice::DrawSprite(0, 0, stageClearImges, 1, 1, 0.0f, WHITE);//ゲームクリアの文字を出力

				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//ステージセレクトへ
				NoviceDrawQuadBlock(nextStage, 655, 220, nextStageImges);//次のステージへ

				Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
				Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト
			}

			break;

		case stage6:

			///==========================================================================
			//マップタイルの描画
			///==========================================================================

			//マップタイルの描画
			NoviceMapChip(KeyFlag, mapX, mapY, Stage6Map, GraveImges, goalImges, rockGoalImges);

			//おばけの描画
			NoviceDrawQuadPlayer(ghost, playerSize, GhostImges);

			//残り壁数
			Novice::DrawSprite(0, 0, residueImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(128, 0, suuziImges[touchNumber], 1, 1, 0.0f, WHITE);

			Novice::DrawSprite(blockSize * 5, 0, stageNumbertImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((blockSize * 7) + 20, 0, suuziImges[6], 1, 1, 0.0f, WHITE);


			//スタートボタン
			if (StartFlag == false)
			{
				NoviceDrawQuadBlock(StartKey, 655, 220, StartImges);
				Novice::DrawSprite((int)ghost.RightTop.x, (int)ghost.RightTop.y - ghostRadius, ghostRightArrowImges, 1, 1, 0.0f, WHITE);
			}

			//リトライボタン
			NoviceDrawQuadBlock(retry, 512, 256, retryImges);

			if (onOffFlag == false)
			{
				NoviceDrawQuadBlock(onOffBlock, blockSize, blockSize, onBlockImges);
				NoviceDrawQuadBlock(switchBlock, blockSize, blockSize, switchOffImges);
			}

			if (onOffFlag == true)
			{
				NoviceDrawQuadBlock(onOffBlock, blockSize, blockSize, offBlockImges);
				NoviceDrawQuadBlock(switchBlock, blockSize, blockSize, switchOnImges);
			}

			///==========================================================================
			//ステージクリアの描画処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				Novice::DrawSprite(0, 0, stageClearImges, 1, 1, 0.0f, WHITE);//ゲームクリアの文字を出力

				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//ステージセレクトへ
				NoviceDrawQuadBlock(nextStage, 655, 220, nextStageImges);//次のステージへ

				Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
				Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト
			}

			break;

		case stage7:

			///==========================================================================
			//マップタイルの描画
			///==========================================================================

			//マップタイルの描画
			NoviceMapChip(KeyFlag, mapX, mapY, Stage7Map, GraveImges, goalImges, rockGoalImges);

			//壊せるブロックの描画
			NoviceBreakDrawQuadBlock(breakableBlocks, BlockHp, blockSize, breakableBlocksImges, breakableBlocks2Imges, breakableBlocks3Imges);

			//右矢印
			NoviceDrawQuadBlock(rightArrow, blockSize, blockSize, rightArrowImges);

			//左矢印
			NoviceDrawQuadBlock(leftArrow, blockSize, blockSize, leftArrowImges);

			//上矢印
			NoviceDrawQuadBlock(upArrow, blockSize, blockSize, upArrowImges);

			//下矢印
			NoviceDrawQuadBlock(dwonArrow, blockSize, blockSize, dwonArrowImges);

			//おばけの描画
			NoviceDrawQuadPlayer(ghost, playerSize, GhostImges);

			//残り壁数
			Novice::DrawSprite(0, 0, residueImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(128, 0, suuziImges[touchNumber], 1, 1, 0.0f, WHITE);

			Novice::DrawSprite(blockSize * 5, 0, stageNumbertImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((blockSize * 7) + 20, 0, suuziImges[7], 1, 1, 0.0f, WHITE);


			//スタートボタン
			if (StartFlag == false)
			{
				NoviceDrawQuadBlock(StartKey, 655, 220, StartImges);
				Novice::DrawSprite((int)ghost.RightTop.x - blockSize - ghostRadius * 2, (int)ghost.RightTop.y - ghostRadius, ghostLeftArrowImges, 1, 1, 0.0f, WHITE);
			}

			//リトライボタン
			NoviceDrawQuadBlock(retry, 512, 256, retryImges);

			if (onOffFlag == false)
			{
				NoviceDrawQuadBlock(onOffBlock, blockSize, blockSize, onBlockImges);
				NoviceDrawQuadBlock(onOffBlock2, blockSize, blockSize, onBlockImges);
				NoviceDrawQuadBlock(switchBlock, blockSize, blockSize, switchOffImges);
			}

			if (onOffFlag == true)
			{
				NoviceDrawQuadBlock(onOffBlock, blockSize, blockSize, offBlockImges);
				NoviceDrawQuadBlock(onOffBlock2, blockSize, blockSize, offBlockImges);
				NoviceDrawQuadBlock(switchBlock, blockSize, blockSize, switchOnImges);
			}

			///==========================================================================
			//ステージクリアの描画処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				Novice::DrawSprite(0, 0, stageClearImges, 1, 1, 0.0f, WHITE);//ゲームクリアの文字を出力

				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//ステージセレクトへ
				NoviceDrawQuadBlock(nextStage, 655, 220, nextStageImges);//次のステージへ

				Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
				Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト
			}

			break;

		case stage8:

			///==========================================================================
			//マップタイルの描画
			///==========================================================================

			//マップタイルの描画
			NoviceMapChip(KeyFlag, mapX, mapY, Stage8Map, GraveImges, goalImges, rockGoalImges);

			//下矢印
			NoviceDrawQuadBlock(dwonArrow, blockSize, blockSize, dwonArrowImges);

			//おばけの描画
			NoviceDrawQuadPlayer(ghost, playerSize, GhostImges);

			//鍵の描画
			if (KeyFlag == true)
			{
				NoviceDrawQuadBlock(key, blockSize, blockSize, keyImges);
			}

			//残り壁数
			Novice::DrawSprite(0, 0, residueImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(128, 0, suuziImges[touchNumber], 1, 1, 0.0f, WHITE);

			Novice::DrawSprite(blockSize * 5, 0, stageNumbertImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((blockSize * 7) + 20, 0, suuziImges[8], 1, 1, 0.0f, WHITE);


			//スタートボタン
			if (StartFlag == false)
			{
				NoviceDrawQuadBlock(StartKey, 655, 220, StartImges);
				Novice::DrawSprite((int)ghost.RightTop.x, (int)ghost.RightTop.y - ghostRadius, ghostRightArrowImges, 1, 1, 0.0f, WHITE);
			}

			//リトライボタン
			NoviceDrawQuadBlock(retry, 512, 256, retryImges);

			///==========================================================================
			//ステージクリアの描画処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				Novice::DrawSprite(0, 0, stageClearImges, 1, 1, 0.0f, WHITE);//ゲームクリアの文字を出力

				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//ステージセレクトへ
				NoviceDrawQuadBlock(nextStage, 655, 220, nextStageImges);//次のステージへ

				Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
				Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト
			}

			break;

		case stage9:

			///==========================================================================
			//マップタイルの描画
			///==========================================================================

			//マップタイルの描画
			NoviceMapChip(KeyFlag, mapX, mapY, Stage9Map, GraveImges, goalImges, rockGoalImges);

			//右矢印
			NoviceDrawQuadBlock(rightArrow, blockSize, blockSize, rightArrowImges);

			//左矢印
			NoviceDrawQuadBlock(leftArrow, blockSize, blockSize, leftArrowImges);

			//上矢印
			NoviceDrawQuadBlock(upArrow, blockSize, blockSize, upArrowImges);

			//下矢印
			NoviceDrawQuadBlock(dwonArrow, blockSize, blockSize, dwonArrowImges);

			//おばけの描画
			NoviceDrawQuadPlayer(ghost, playerSize, GhostImges);

			Novice::DrawSprite(blockSize * 5, 0, stageNumbertImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((blockSize * 7) + 20, 0, suuziImges[9], 1, 1, 0.0f, WHITE);


			//鍵の描画
			if (KeyFlag == true)
			{
				NoviceDrawQuadBlock(key, blockSize, blockSize, keyImges);
			}

			//残り壁数
			Novice::DrawSprite(0, 0, residueImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(128, 0, suuziImges[touchNumber], 1, 1, 0.0f, WHITE);

			//スタートボタン
			if (StartFlag == false)
			{
				NoviceDrawQuadBlock(StartKey, 655, 220, StartImges);
				Novice::DrawSprite((int)ghost.RightTop.x, (int)ghost.RightTop.y - ghostRadius, ghostRightArrowImges, 1, 1, 0.0f, WHITE);
			}

			//リトライボタン
			NoviceDrawQuadBlock(retry, 512, 256, retryImges);

			///==========================================================================
			//ステージクリアの描画処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				Novice::DrawSprite(0, 0, stageClearImges, 1, 1, 0.0f, WHITE);//ゲームクリアの文字を出力

				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//ステージセレクトへ
				NoviceDrawQuadBlock(nextStage, 655, 220, nextStageImges);//次のステージへ

				Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
				Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト
			}

			break;


		case stage10:

			///==========================================================================
			//マップタイルの描画
			///==========================================================================

			//マップタイルの描画
			NoviceMapChip(KeyFlag, mapX, mapY, Stage10Map, GraveImges, goalImges, rockGoalImges);

			//壊せるブロックの描画
			NoviceBreakDrawQuadBlock(breakableBlocks, BlockHp, blockSize, breakableBlocksImges, breakableBlocks2Imges, breakableBlocks3Imges);

			//右矢印
			NoviceDrawQuadBlock(rightArrow, blockSize, blockSize, rightArrowImges);

			//左矢印
			NoviceDrawQuadBlock(leftArrow, blockSize, blockSize, leftArrowImges);

			//上矢印
			NoviceDrawQuadBlock(upArrow, blockSize, blockSize, upArrowImges);

			//下矢印
			NoviceDrawQuadBlock(dwonArrow, blockSize, blockSize, dwonArrowImges);
			NoviceDrawQuadBlock(dwonArrow2, blockSize, blockSize, dwonArrowImges);

			Novice::DrawSprite(blockSize * 5, 0, stageNumbertImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((blockSize * 7) + 20, 0, FImges, 1, 1, 0.0f, WHITE);

			//おばけの描画
			NoviceDrawQuadPlayer(ghost, playerSize, GhostImges);

			//鍵の描画
			if (KeyFlag == true)
			{
				NoviceDrawQuadBlock(key, blockSize, blockSize, keyImges);
			}

			//残り壁数
			Novice::DrawSprite(0, 0, residueImges, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(128, 0, suuziImges[touchNumber], 1, 1, 0.0f, WHITE);

			//スタートボタン
			if (StartFlag == false)
			{
				NoviceDrawQuadBlock(StartKey, 655, 220, StartImges);
				Novice::DrawSprite((int)ghost.LeftTop.x - blockSize, (int)ghost.LeftTop.y - ghostRadius, ghostLeftArrowImges, 1, 1, 0.0f, WHITE);
			}

			//リトライボタン
			NoviceDrawQuadBlock(retry, 512, 256, retryImges);

			if (onOffFlag == false)
			{
				NoviceDrawQuadBlock(onOffBlock, blockSize, blockSize, onBlockImges);
				NoviceDrawQuadBlock(onOffBlock2, blockSize, blockSize, onBlockImges);
				NoviceDrawQuadBlock(switchBlock, blockSize, blockSize, switchOffImges);
			}

			if (onOffFlag == true)
			{
				NoviceDrawQuadBlock(onOffBlock, blockSize, blockSize, offBlockImges);
				NoviceDrawQuadBlock(onOffBlock2, blockSize, blockSize, offBlockImges);
				NoviceDrawQuadBlock(switchBlock, blockSize, blockSize, switchOnImges);
			}

			///==========================================================================
			//ステージクリアの描画処理
			///==========================================================================

			if (stageClearFlag == true)
			{
				Novice::DrawSprite(0, 0, gameClearImges, 1, 1, 0.0f, WHITE);//ゲームクリアの文字を出力

				NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//ステージセレクトへ

				Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
				Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト
			}

			break;

		case Setting:

			NoviceDrawQuadBlock(turnBack, blockSize, blockSize, turnBackImges);

			NoviceDrawQuadBlock(BGMdownVolume, blockSize, blockSize, downVolumeImges);
			NoviceDrawQuadBlock(BGMupVolume, blockSize, blockSize, upVolumeImges);
			NoviceDrawQuadBlock(SEdownVolume, blockSize, blockSize, downVolumeImges);
			NoviceDrawQuadBlock(SEupVolume, blockSize, blockSize, upVolumeImges);

			//設定画面の背景
			Novice::DrawSprite(0, 0, settingBackgroundImges, 1, 1, 0.0f, WHITE);

			//音量の数字

			if (BGMVolumeCount == 5)
			{
				Novice::DrawSprite(930, 350, MaxImges, 1, 1, 0.0f, BLACK);
			}
			else
			{
				Novice::DrawSprite(930, 350, suuziImges[(int)BGMVolumeCount], 1, 1, 0.0f, BLACK);
			}

			if (SEVolumeCount == 5)
			{
				Novice::DrawSprite(930, 530, MaxImges, 1, 1, 0.0f, BLACK);
			}
			else
			{
				Novice::DrawSprite(930, 530, suuziImges[(int)SEVolumeCount], 1, 1, 0.0f, BLACK);
			}

			break;

		case GameClear:

			Novice::DrawSprite(0, 0, gameClearImges, 1, 1, 0.0f, WHITE);
			NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//Titleへ

			Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, GhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
			Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト

			break;

		case GameOver:

			Novice::DrawSprite(0, 0, gameOverImges, 1, 1, 0.0f, WHITE);
			NoviceDrawQuadBlock(title, 655, 220, stageSelectBackImges);//Titleへ

			Novice::DrawSprite((int)clearGhostPosX, (int)clearGhostPosY, gameOverGhostImges, 1, 1, 0.0f, WHITE);//クリア時のおばけ
			Novice::DrawSprite((int)clearNisitoPosX, (int)clearNisitoPosY, playerImges, 1, 1, 0.0f, WHITE);//クリア時のニシト

			break;
		}

		///==========================================================================
		//デバックの描画
		///==========================================================================

		//Novice::ScreenPrintf(0, 0, "Mouse X : %d", mousePosX);
		//Novice::ScreenPrintf(0, 20, "Mouse Y : %d", mousePosY);
		//Novice::ScreenPrintf(0, 40, "count = %d", mouseConutTimer);
		//Novice::ScreenPrintf(0, 60, "yazirusiPosX = %f", rightArrow.position.x);
		//Novice::ScreenPrintf(0, 80, " %f<= ghostPosY:%f", upArrow.LeftTop.y + upArrow.position.y, ghostPosY);
		//Novice::ScreenPrintf(0, 100, "%d", StartFlag);
		//Novice::ScreenPrintf(0, 120, "%f", ghost.speed.x);
		//Novice::ScreenPrintf(0, 140, "StartFlag:%d", StartFlag);
		//Novice::ScreenPrintf(0, 160, "SceneChangeTimer:%d", SceneChangeTimer);
		//Novice::ScreenPrintf(0, 180, "BlockHp:%f", BlockHp); 
		//Novice::ScreenPrintf(0, 200, "BlockTimer:%f", BlockTimer);
		//Novice::ScreenPrintf(0, 220, "BGMVolumeCount:%f", BGMVolumeCount);
		//Novice::ScreenPrintf(0, 240, "BGMVolume:%f", BGMVolume);
		//Novice::ScreenPrintf(0, 260, "SEVolumeCount:%f", SEVolumeCount);
		//Novice::ScreenPrintf(0, 280, "touchNumber:%d", touchNumber);
		//Novice::ScreenPrintf(0, 300, "clearGhostPosX:%f", clearGhostPosX); 
		//Novice::ScreenPrintf(0, 320, "clearNisitoPosX:%f", clearNisitoPosX);
		//Novice::ScreenPrintf(0, 340, "onOffTimer:%f", onOffTimer);
		//Novice::ScreenPrintf(0, 360, "SteppingFlag:%d", SteppingFlag);
		//Novice::ScreenPrintf(0, 380, "switchSE.Timer:%f", switchSE.Timer); 
		//Novice::ScreenPrintf(0, 400, "mouseMoveFlag:%d", mouseMoveFlag); 
		//Novice::ScreenPrintf(0, 420, "mouseClickFlag:%d", mouseClickFlag);
		//Novice::ScreenPrintf(0, 440, "dwonArrow.Flag:%d", dwonArrow.Flag);
		//Novice::ScreenPrintf(0, 460, "dwonArrow2.Flag:%d", dwonArrow2.Flag);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
