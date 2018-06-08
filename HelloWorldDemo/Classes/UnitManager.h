#ifndef _UNITMANAGER_H_
#define _UNITMANAGER_H_

#include<cocos2d.h>
#include"GridMap.h"
#include"Unit.h"
#include<map>
#include<vector>
#include"GridMap.h"
#include"GameMessages.pb.h"
#include"SystemHeader.h"
using namespace std;
USING_NS_CC;
class Unit;
class UnitManager;
//class BuildingUnit;
class UnitManager :public Node
{
	CC_SYNTHESIZE(GameMessageGroup *, _msgGroup, MessageGrop);
	CC_SYNTHESIZE(CampTypes, _playerCamp, PlayerCamp);
	CC_SYNTHESIZE(GridMap *, _gridMap, Gridmap);
	CC_SYNTHESIZE(TMXTiledMap *, _tileMap, TileMap);
	CC_SYNTHESIZE(int, _nextId, NextId);
	CC_SYNTHESIZE(int, _waitingGINum, WaitingGINum);
	CC_SYNTHESIZE(int, _waitingTankNum, WaitingTankNum);
	CC_SYNTHESIZE(int, _waitingAttackDogNum, WaitingAttackDogNum);
	vector<int> _selectedUnitID;
	map<int, int> _barracksId;
	map<int,int> _warFactoryId;
	map<int, int>newAttackUnit;
	map<int, int>attackingUnit;
	//Base * base = nullptr;
public:
	vector<UnitTypes>_fighterProduceSeq;
	map<int, Unit *>_unitIdMap;
	CREATE_FUNC(UnitManager);
	bool init();
	GridVec2 getUnitPos(int id);
	CampTypes getUnitCamp(int id);
	Unit * getUnitPtr(int id);
	//void produceFighter(int type ,int id);
	void updateUnitState();
	void destoryUnit(int id);
	void checkWinOrLose();
	Unit * creatUnit(CampTypes camp, UnitTypes type, const  GridVec2& pos, int id = 0);
	void creatProduceMessage(UnitTypes unitType, const GridVec2 & pos);
	void creatMoveMessage(int id, const Vec2 & pos);
	void createAttackMessage(int id1,int id2 ,int damage);
	void selectUnits(const GridRect &range);
	void choosePosOrUnit(const GridVec2 & pos);
	void deselectAllUnits();
	friend GridRect transferRectToGridRect(const Rect & rect);
	void unitAttackUpdate();
	void fighterUnitProductionUpdate();
};	



#endif // !_UNITMANAGER_H


