#include <string>
#include "xlibrary11.hpp"
using namespace XLibrary11;

enum Mode
{
	Title,
	Game,
};

// ランダムな座標を取得する
Float3 GetRandomPosition()
{

	return Float3(
		rand() / (float)RAND_MAX * App::GetWindowSize().x - App::GetWindowSize().x / 2,
		App::GetWindowSize().y / 2+rand() / (float)RAND_MAX * 400,
		0.0
	);
}

// 当たり判定
bool IsHit(
	Float3 position1,
	Float3 position2,
	float range
)
{
	if (position1.x - range < position2.x &&
		position1.x + range > position2.x &&
		position1.y - range< position2.y &&
		position1.y + range > position2.y)
	{
		// 当たった
		return true;
	}
	// 当たってない
	return false;
}
int MAIN()
{
	Mode mode = Title;		// 最初はタイトル画面にする

	Camera camera;
	camera.color = Float4(0.0, 0.0, 0.0, 1.0);

	const float playerSpeed = 3.0;	// 自機の速さ

	Sprite player(L"player1.png");	// 自機の画像
	player.scale = 2.0;


	const int enemyNum = 5;		// 敵の数
	const float enemySpeed = 2.0;	// 敵の速さ

	Sprite enemy(L"enemy.png");		// 敵の画像
	enemy.scale = 2.0;

	Float3 enemyPosition[enemyNum];	// 敵の座標

	const int bulletNum = 5;	// 玉の数
	const float bulletSpeed = 10.0;	// 弾の速さ
	int bulletIndex = 0;	// 次に発射する弾の番号
	// 次に発射するまでのカウント
	int bulletInterval = 0;

	Sprite bullet(L"bullet.png");	// 弾の画像
	bullet.scale = 2.0;

	Float3 bulletPosition[bulletNum];	// 弾の座標
	float bulletRadian[bulletNum];	// 弾の角度

	const int bullet2Num = 100;	// 敵玉の数
	const float bullet2Speed = 10.0;	// 弾の速さ
	int bullet2Index = 0;	// 次に発射する弾の番号
							// 次に発射するまでのカウント
	int bullet2Interval = 0;

	Sprite bullet2(L"bullet2.png");	// 弾の画像
	bullet2.scale = 2.0;

	Float3 bullet2Position[bullet2Num];	// 弾の座標
	float bullet2Radian[bullet2Num];	// 弾の角度


	Text titleText(L"シューティング", 32.0);
	titleText.scale = 2.0;
	titleText.color = Float4(0.0, 1.0, 1.0, 1.0);

	int score = 0;	// スコア

	Text scoreText(L"0", 10.0);
	scoreText.scale = 10.0;
	scoreText.color = Float4(0.0, 1.0, 1.0, 1.0);


	while (App::Refresh())
	{
		camera.Update();

		switch (mode)
		{
		case Title:

			if (App::GetKeyDown(VK_RETURN))
			{
				player.position = 0.0;
				score = 0;
				scoreText.Create(L"0", 10.0);


				// 敵の座標の初期化
				for (int i = 0; i < enemyNum; i++)
				{
					enemyPosition[i] = GetRandomPosition();
				}

				// 弾の座標の初期化
				for (int i = 0; i < bulletNum; i++)
				{
					// はるか彼方に飛ばす
					bulletPosition[i].x = 99999.0;
					bulletRadian[i] = 0.0;
				}


				//敵の弾の座標の初期化
				for (int i = 0; i < bullet2Num; i++)
				{
					// はるか彼方に飛ばす
					bullet2Position[i].x = 99999.0;
					bullet2Radian[i] = 0.0;
				}

				mode = Game;
			}

			camera.position = 0.0;

			titleText.Draw();

			scoreText.position = Float3(
				0.0, 200.0, 0.0
			);

			break;

		case Game:
			// 自機の移動の処理
			if (App::GetKey('D'))
				player.position.x += playerSpeed;

			if (App::GetKey('A'))
				player.position.x -= playerSpeed;

			if (App::GetKey('W'))
				player.position.y += playerSpeed;

			if (App::GetKey('S'))
				player.position.y -= playerSpeed;

			if (player.position.x > 300 + 5)
				player.position.x = 300 + 5;

			if (player.position.x < -300 - 5)
				player.position.x = -300 - 5;

			if (player.position.y > App::GetWindowSize().y / 2 + 5)
				player.position.y = App::GetWindowSize().y / 2 + 5;

			if (player.position.y <-App::GetWindowSize().y / 2 - 5)
				player.position.y = -App::GetWindowSize().y / 2 - 5;

			// 自機の描画
			player.Draw();


			// 弾を発射する処理
			bulletInterval++;
			if (bulletInterval > 10)
			{
				bulletInterval = 0;


				// 弾をプレイヤーの座標と角度に合わせる
				bulletPosition[bulletIndex] =
					player.position;
				bulletRadian[bulletIndex] =
					-11.0;

				bulletIndex++;
				if (bulletIndex >= bulletNum)
					bulletIndex = 0;
			}

			

			for (int i = 0; i < enemyNum; i++)
			{


				// 敵を動かす処理
				if (enemyPosition[i].y < -App::GetWindowSize().y)
				{
					enemyPosition[i] = GetRandomPosition();
				}
				enemyPosition[i] += Float3(
					0.0,
					-enemySpeed,
					0.0
				);
				enemy.position = enemyPosition[i];

				float hitRange = enemy.GetSize().x / 2.0 *
					enemy.scale.x;
				// 弾の当たり判定
				for (int j = 0; j < bulletNum; j++)
				{
					if (IsHit(
						enemyPosition[i],
						bulletPosition[j],
						hitRange
					))
					{
						enemyPosition[i] =
							camera.position +
							GetRandomPosition();
						score++;
						scoreText.Create(
							std::to_wstring(score),
							10.0
						);
					}
				}

				// 敵弾を発射する処理
				bullet2Interval++;
				if (bullet2Interval > 15)
				{
					bullet2Interval = 0;


					// 弾をプレイヤーの座標と角度に合わせる
					bullet2Position[bullet2Index] =
						enemy.position;
					bullet2Radian[bullet2Index] =
						-11.0;

					bullet2Index++;
					if (bullet2Index >= bullet2Num)
						bullet2Index = 0;
				}

				// 自機との当たり判定
				if (IsHit(
					enemyPosition[i],
					player.position,
					hitRange
				))
				{
					mode = Title;
				}

				// 自機との当たり判定
				if (IsHit(
					bullet2Position[i],
					player.position,
					hitRange
				))
				{
					mode = Title;
				}

				enemy.Draw();
			}

			for (int i = 0; i < bulletNum; i++)
			{
				bulletPosition[i] += Float3(
					cosf(bulletRadian[i]),
					sinf(bulletRadian[i]),
					0.0
				) * bulletSpeed;
				bullet.position = bulletPosition[i];

				// 弾の描画
				bullet.Draw();
			}


			for (int i = 0; i < bullet2Num; i++)
			{
				bullet2Position[i] -= Float3(
					cosf(bullet2Radian[i]),
					sinf(bullet2Radian[i]),
					0.0
				) * bulletSpeed;
				bullet2.position = bullet2Position[i];

				// 弾の描画
				bullet2.Draw();
			}

			scoreText.position = camera.position +
				Float3(0.0, 200.0, 0.0);
			scoreText.Draw();

			break;
		}
	}
	return 0;
}
