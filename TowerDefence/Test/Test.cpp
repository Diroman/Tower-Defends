#include "stdafx.h"
#include "gtest\gtest.h"
#include "../TowerDefence/Effect.h"
#include "../TowerDefence/Enemy.h"
#include "../TowerDefence/Structure.h"
#include "../TowerDefence/Castle.h"
#include "../TowerDefence/LandSpace.h"

TEST(oCastle) {
	Castle A(string("My"), 500, 300);

	ASSERT_EQ(500, A.GetHP());
	ASSERT_TRUE(A.SubHP(100));
	ASSERT_EQ(400, A.GetHP());
	ASSERT_FALSE(A.SubHP(450));

	ASSERT_EQ(300, A.GetMoney());
	A.AddMoney(50);
	ASSERT_EQ(350, A.GetMoney());
	ASSERT_TRUE(A.SubMoney(100));
	ASSERT_EQ(250, A.GetMoney());
	ASSERT_FALSE(A.SubMoney(300));
	ASSERT_EQ(250, A.GetMoney());
}

TEST(oEnemy) {
	Enemy A;
	Enemy B;
	Enemy C(100, 10, 20);

	A.ChangeSpeed(4);
	B.SubHP(20);
	
	ASSERT_EQ(6, A.GetSpeed());
	ASSERT_EQ(80, B.GetHP());
	ASSERT_EQ(20, A.GetMoney());
	ASSERT_TRUE(B.CheckHP());		// true - B has HP
	A.SubHP(100);
	ASSERT_FALSE(A.CheckHP());		// false - A hasn't HP
	ASSERT_EQ(10, C.GetSpeed());
	ASSERT_EQ(100, C.GetHP());
	ASSERT_TRUE(C.CheckHP());
}

TEST(oEffect) {

	Freeze A(5, 1);
	Venom B(7, 2);
	Weakness C(9, 2);

	ASSERT_EQ(5, A.GetValue());
	ASSERT_EQ(1, A.GetType());
	ASSERT_EQ(1, A.GetTime());
	ASSERT_TRUE(A.CheckTime());
	ASSERT_EQ(7, B.GetValue());
	ASSERT_EQ(2, B.GetType());
	ASSERT_EQ(2, B.GetTime());
	ASSERT_TRUE(B.CheckTime());
	_sleep(3000);
	ASSERT_EQ(9, C.GetValue());
	ASSERT_EQ(3, C.GetType());
	ASSERT_EQ(2, C.GetTime());
	ASSERT_FALSE(C.CheckTime());
}

TEST(oStructure) {
	
	int chrc[3] = { 100, 5, 20 };
	time_t tm = 10;
	Freeze *A = new Freeze(5, 10);
	SimpleTower AT(chrc, 2);
	MagicTower MT(chrc, 3, A);
	Trap TT(chrc, 4, A);

	ASSERT_EQ(1, AT.GetLevel());
	ASSERT_EQ(100, AT.GetChar()[0]);
	ASSERT_EQ(5, AT.GetChar()[1]);
	ASSERT_EQ(20, AT.GetChar()[2]);
	ASSERT_EQ(2, AT.GetSpeed());
	ASSERT_EQ(1, MT.GetLevel());
	ASSERT_EQ(3, MT.GetSpeed());
	ASSERT_EQ(1, TT.GetLevel());
	ASSERT_EQ(4, TT.GetSpeed());
	delete A;
}


int _tmain(int argc, _TCHAR *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}