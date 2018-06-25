#ifndef _UNIT_H_
#define _UNIT_H_

#include "GridMap.h"
#include "cocos2d.h"
#include "ui/UILoadingBar.h"
#include "UnitManager.h"
#include"PathFinder.h"
#include"SystemHeader.h"
#include"SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;
using namespace ui;

//默认顺序：按BASE ,POWERPLANT,BARRACKS,WARFACTORY,OREREFINERY
//每种建筑单位的价格（默认顺序）
const int COST[8] = { 2500, 1800, 1500, 2000, 2000 , 400, 300, 1000};
//每种建筑单位的HP（默认顺序）
const int HEALTH[5] = { 5000,1500,1800,2400,2000 };
//每种建筑单位的发电/耗电量（默认顺序）
const int POWER[5] = { 50,100,-30,-50,-40 };
//每种建筑单位的金钱产量（默认顺序）
const int MONEY_PRODUCE[5] = {50,0,0,0,100};
//每种建筑单位的SIZE
const GridDimen SIZES[5] = { GridDimen(6,4),GridDimen(4,4),
GridDimen(4,4),GridDimen(6,4),GridDimen(6,4) };
const int FIGHTER_HEALTH[3] = {400,200,1200};
const GridDimen FIGHTER_SIZES[3] = { GridDimen(1,1),GridDimen(1,1),GridDimen(1,1) };
const float UNIT_MOVE_SPEED[3] = { 8,12,6 };
const int ATTACK_FORCE[3] = { 30,20,80 };
const int ATTACK_SPEED[3] = { 0,0,0 };
const GridDimen AUTO_ATTACK_RANGE[3] = { GridDimen(8,8),GridDimen(12,12),GridDimen(16,16) };
const int MANUAL_ATTACK_RANGE[3] = { 0,0,0 };
const string BUILDING_BG_BAR[5] = { "20150914105931941.png","20150914105931941.png","20150914105931941.png","20150914105931941.png","20150914105931941.png" };
const string BUILDING_HP_BAR[5] = { "20150914105957633.png","20150914105957633.png","20150914105957633.png","20150914105957633.png","20150914105957633.png" };
const string FIGHTER_HP_BAR[3] = { "20150914105957633.png","20150914105957633.png","20150914105957633.png" };
const string FIGHTER_BG_BAR[3] = { "20150914105931941.png","20150914105931941.png","20150914105931941.png" };
const int PRODUCEGITIME = 360;
const int PRODUCEATTACKDOGTIME = 240;
const int PRODUCETANKTIME = 720;
const float BUILDINGTIME[5] = { 0.25,0.2,0.2,0.15,0.15 };
//BASE 8s,POWERPLANT 17s,BARRACKS 20s,WARFACTORY 40s,OREREFINERY 40s

class Unit;
class UnitManager;

class Unit:public Sprite
{
	/*CC_SYNTHESIZE定义变量，并且直接定义默认的get/set方法。
	protected: varType varName;\
*/
	CC_SYNTHESIZE(UnitTypes, _unitType, UnitType);
	CC_SYNTHESIZE(int, _id, ID);
	CC_SYNTHESIZE(CampTypes, _camp, Camp);
	CC_SYNTHESIZE(int, _health, Health);
	CC_SYNTHESIZE(int, _moneyCost, MoneyCost);
	CC_SYNTHESIZE(int, _currentHp, CurrentHp);
	CC_SYNTHESIZE(bool, _underAttack, UnderAttack);
	CC_SYNTHESIZE(GridDimen, _unitSize, UnitSize);
	CC_SYNTHESIZE(GridRect, _unitRect, UnitRect);
	CC_SYNTHESIZE(GridVec2, _unitCoord, UnitCoord);
	CC_SYNTHESIZE(Vec2, _destination, Destination);
	CC_SYNTHESIZE(int, _attackID, AttackID);
	CC_SYNTHESIZE(UnitTypes, _producingUnitType, ProducingUnitType);
	CC_SYNTHESIZE(int, _producingState, ProducingState);
	CC_SYNTHESIZE(int, _enermyId, EnermyId);
	CC_SYNTHESIZE(std::vector<GridVec2>, _gridPath, GridPath);
	CC_SYNTHESIZE(bool, _isBuilding, IsBuilding);
	CC_SYNTHESIZE(bool, _autoAttack, AutoAttack);
	//CC_SYNTHESIZE(UnitManager*，_unitManager, unitManager);
public:
	UnitManager * _unitManager = nullptr;
	LoadingBar *_hpBar=nullptr;
	Sprite * hpBGSprite=nullptr;
	//std::vector<GridVec2> _gridPath;
	void setUnitManager(UnitManager * uM) { _unitManager = uM; }
	virtual bool _stdcall init(CampTypes camp,UnitTypes Type,GridVec2 point, TMXTiledMap* map,GridMap *gridmap , int id=0);
	//CC_SYNTHESIZE(GridDimen, _unitView, UnitView);
	GridMap * _battleMap=nullptr;
	TMXTiledMap* _tiledMap = nullptr;
	//~Unit();
	static Unit* Unit::create(const std::string& filename);
	//virtual void initHpBar();
	//virtual bool setPositionInGridMap(GridRect rectPos,GridMap * map)const;
	//单位收到伤害，掉血，血条缩短 当hp<=0是会自动调用unschedule凹函数
	bool getDamage(int hurt);
	//将单位从和_battleMap种移除 有火焰特效
	void removeFromMap();
	void hideHpBar();
	void displayHpBar();
	virtual void deleteUnit() {}

	//brief try to find path
	virtual void tryToFindPath() {};

	//find a path to the destination
	virtual std::vector<GridVec2> findPath(const GridVec2 & destination) const { return _gridPath; };

	//optimize path 
	virtual void optimizePath() {};

	virtual void move();
	virtual void attack() {}
	//初始化血条
	void initHpBar(UnitTypes type);
	virtual void attackUpdate(float dt) {}
	virtual void startAttackUpdate() {}
	virtual void stopAttackUpdate() {}
	virtual GridVec2 findEmptyPosToProduce() { return GridVec2(0, 0); }
	virtual GridVec2 findEmptyPosToProduceTank() { return GridVec2(0, 0); }
	virtual void startProduceUnit(UnitTypes proUnitType) {}
	virtual void stopProduceUnit() {}
	virtual void buildingUpdate() {}
	virtual void produceUpdate(float fd) {}
};


class BuildingUnit :public Unit
{
	int _produceProcess;
	int _produceTime;
	float _buildingProgress;
	Sprite * _progressbgSprite = nullptr;
	Sprite * progress = nullptr;
	CC_SYNTHESIZE(int,_powerCost,PowerCost);
	CC_SYNTHESIZE(int,_moneyProduce,MoneyProduce);
	CC_SYNTHESIZE(UnitTypes, _buildingType, BuildingType);
public:
	//void setProperties(UnitTypes buildingtype);
	//static BuildingUnit * create(const std::string& filename);
	//初始化时会自动调用initHpBar和setPositionInGridMap
	virtual bool _stdcall init(CampTypes camp,UnitTypes buildingType,GridVec2 point, TMXTiledMap* map,GridMap *gridmap, int id=0 );
	
	//在网格地图种设置该单位的信息
	bool setPositionInGridMap(GridRect rectPos, GridMap * map);
	//从地图中移除单位
	virtual void deleteUnit();
	void startProduceUnit(UnitTypes proUnitType);
	void stopProduceUnit();
	void produceUpdate(float fd);
	
	virtual void buildingUpdate();
	void updateBuildingBar(float dt);
	//virtual void startProduce();
	//bool setPositionInTiledMap();
	//virtual void sellBuilding();
};
class FightUnit :public Unit
{
	CC_SYNTHESIZE(UnitTypes, _fighterType, FighterType);
	CC_SYNTHESIZE(bool,_attacking,Attacking);
	CC_SYNTHESIZE(float,_moveSpeed,MoveSpeed);
	CC_SYNTHESIZE(int ,_attackForce,AttackForce);
	CC_SYNTHESIZE(int, _attackSpeed, AttackSpeed);
	CC_SYNTHESIZE(int ,_atkIDPosition,AtkIDPosition);
	CC_SYNTHESIZE(int ,_manualAttackScope,ManualAttackScope);
	CC_SYNTHESIZE(GridDimen ,_autoAttackScope,AutoAttackScope);
public:
	virtual bool _stdcall init(CampTypes camp,UnitTypes type, GridVec2 coord,
		TMXTiledMap* map, GridMap * gridmap,int id=0 );
	//bool setPositionInTiledMap();
	bool setPositionInGirdMap(GridRect rectPos, int id);
	
	//brief try to find path
	void tryToFindPath();

	//optimize path 
	void optimizePath();

	//find a path to the destination
	std::vector<GridVec2> findPath(const GridVec2 & destination);

	void move();

	void moveEndedCallBack();

	virtual void shootBullet();
	virtual void attack();
	//void initHpBar(UnitTypes type) {}
	bool setPositionInGridMap(GridRect rectPos, GridMap * map) ;
	//virtual void autoAttack();
	int searchNearEnemy();
	//virtual bool setPosition();
	
	//void autoAttackUpdate();
};
//爆炸特效类
class ExplosionEffect : public cocos2d::ParticleFire
{
public:
	 bool init();

	CREATE_FUNC(ExplosionEffect);
private:
	void remove(float f);
};
#endif // !_UNIT_H_
