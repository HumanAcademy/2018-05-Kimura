#include <string>
#include "xlibrary11.hpp"
using namespace XLibrary11;

enum Mode
{
	Title,
	Game,
};

// �����_���ȍ��W���擾����
Float3 GetRandomPosition()
{

	return Float3(
		rand() / (float)RAND_MAX * App::GetWindowSize().x - App::GetWindowSize().x / 2,
		App::GetWindowSize().y / 2+rand() / (float)RAND_MAX * 400,
		0.0
	);
}

// �����蔻��
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
		// ��������
		return true;
	}
	// �������ĂȂ�
	return false;
}
int MAIN()
{
	Mode mode = Title;		// �ŏ��̓^�C�g����ʂɂ���

	Camera camera;
	camera.color = Float4(0.0, 0.0, 0.0, 1.0);

	const float playerSpeed = 3.0;	// ���@�̑���

	Sprite player(L"player1.png");	// ���@�̉摜
	player.scale = 2.0;


	const int enemyNum = 5;		// �G�̐�
	const float enemySpeed = 2.0;	// �G�̑���

	Sprite enemy(L"enemy.png");		// �G�̉摜
	enemy.scale = 2.0;

	Float3 enemyPosition[enemyNum];	// �G�̍��W

	const int bulletNum = 5;	// �ʂ̐�
	const float bulletSpeed = 10.0;	// �e�̑���
	int bulletIndex = 0;	// ���ɔ��˂���e�̔ԍ�
	// ���ɔ��˂���܂ł̃J�E���g
	int bulletInterval = 0;

	Sprite bullet(L"bullet.png");	// �e�̉摜
	bullet.scale = 2.0;

	Float3 bulletPosition[bulletNum];	// �e�̍��W
	float bulletRadian[bulletNum];	// �e�̊p�x

	const int bullet2Num = 100;	// �G�ʂ̐�
	const float bullet2Speed = 10.0;	// �e�̑���
	int bullet2Index = 0;	// ���ɔ��˂���e�̔ԍ�
							// ���ɔ��˂���܂ł̃J�E���g
	int bullet2Interval = 0;

	Sprite bullet2(L"bullet2.png");	// �e�̉摜
	bullet2.scale = 2.0;

	Float3 bullet2Position[bullet2Num];	// �e�̍��W
	float bullet2Radian[bullet2Num];	// �e�̊p�x


	Text titleText(L"�V���[�e�B���O", 32.0);
	titleText.scale = 2.0;
	titleText.color = Float4(0.0, 1.0, 1.0, 1.0);

	int score = 0;	// �X�R�A

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


				// �G�̍��W�̏�����
				for (int i = 0; i < enemyNum; i++)
				{
					enemyPosition[i] = GetRandomPosition();
				}

				// �e�̍��W�̏�����
				for (int i = 0; i < bulletNum; i++)
				{
					// �͂邩�ޕ��ɔ�΂�
					bulletPosition[i].x = 99999.0;
					bulletRadian[i] = 0.0;
				}


				//�G�̒e�̍��W�̏�����
				for (int i = 0; i < bullet2Num; i++)
				{
					// �͂邩�ޕ��ɔ�΂�
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
			// ���@�̈ړ��̏���
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

			// ���@�̕`��
			player.Draw();


			// �e�𔭎˂��鏈��
			bulletInterval++;
			if (bulletInterval > 10)
			{
				bulletInterval = 0;


				// �e���v���C���[�̍��W�Ɗp�x�ɍ��킹��
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


				// �G�𓮂�������
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
				// �e�̓����蔻��
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

				// �G�e�𔭎˂��鏈��
				bullet2Interval++;
				if (bullet2Interval > 15)
				{
					bullet2Interval = 0;


					// �e���v���C���[�̍��W�Ɗp�x�ɍ��킹��
					bullet2Position[bullet2Index] =
						enemy.position;
					bullet2Radian[bullet2Index] =
						-11.0;

					bullet2Index++;
					if (bullet2Index >= bullet2Num)
						bullet2Index = 0;
				}

				// ���@�Ƃ̓����蔻��
				if (IsHit(
					enemyPosition[i],
					player.position,
					hitRange
				))
				{
					mode = Title;
				}

				// ���@�Ƃ̓����蔻��
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

				// �e�̕`��
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

				// �e�̕`��
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
