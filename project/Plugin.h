#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include "EBehaviorTree.h"
#include "SteeringBehaviors.h"

class IBaseInterface;
class IExamInterface;

class Plugin :public IExamPlugin
{
public:
	Plugin() {};
	virtual ~Plugin() {};

	void Initialize(IBaseInterface* pInterface, PluginInfo& info) override;
	void DllInit() override;
	void DllShutdown() override;

	void InitGameDebugParams(GameDebugParams& params) override;
	void Update(float dt) override;

	SteeringPlugin_Output UpdateSteering(float dt) override;
	void Render(float dt) const override;

private:
	//Interface, used to request data from/perform actions with the AI Framework
	IExamInterface* m_pInterface = nullptr;
	std::vector<HouseInfo> GetHousesInFOV() const;
	std::vector<EntityInfo> GetEntitiesInFOV() const;

	Elite::Vector2 m_Target = {};
	bool m_CanRun = false; //Demo purpose
	bool m_GrabItem = false; //Demo purpose
	bool m_UseItem = false; //Demo purpose
	bool m_RemoveItem = false; //Demo purpose
	float m_AngSpeed = 0.f; //Demo purpose

	UINT m_InventorySlot = 0;
	ISteeringBehavior* m_pSteeringBehavior;

	Elite::BehaviorTree* m_pBehaviorTree;

	std::vector<EntityInfo> m_vEntitiesInFOV;
	std::vector<HouseInfo> m_vHousesInFOV;

	std::list<std::pair<Elite::Vector2, float>> m_vHousesEntered; //the Vector2 is the center of the house, the float is the time passed since it has been entered

	float m_AmountOfTimeInHouse;

	std::list<Elite::Vector2> m_lPositionsToVisit;

	//Behaviors
	ISteeringBehavior* m_pWander;
	ISteeringBehavior* m_pFace;
	ISteeringBehavior* m_pSeek;
	ISteeringBehavior* m_pFlee;
	ISteeringBehavior* m_pSprintFlee;
};

//ENTRY
//This is the first function that is called by the host program
//The plugin returned by this function is also the plugin used by the host program
extern "C"
{
	__declspec (dllexport) IPluginBase* Register()
	{
		return new Plugin();
	}
}