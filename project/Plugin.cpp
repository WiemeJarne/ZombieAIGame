#include "stdafx.h"
#include "Plugin.h"
#include "IExamInterface.h"
#include "Behaviors.h"

using namespace Elite;

//Called only once, during initialization
void Plugin::Initialize(IBaseInterface* pInterface, PluginInfo& info)
{
	//Retrieving the interface
	//This interface gives you access to certain actions the AI_Framework can perform for you
	m_pInterface = static_cast<IExamInterface*>(pInterface);

	//Create Blackboard
	Elite::Blackboard* pBlackboard{ new Blackboard() };

	//Add data to the Blackboard
	pBlackboard->AddData("SteeringBehaviorType", SteeringBehaviorType::none);
	pBlackboard->AddData("EntitiesInFOV", &m_vEntitiesInFOV);
	pBlackboard->AddData("HousesInFOV", &m_vHousesInFOV);
	pBlackboard->AddData("Interface", m_pInterface);
	pBlackboard->AddData("Target", m_Target);
	pBlackboard->AddData("HousesEntered", &m_vHousesEntered);
	pBlackboard->AddData("posBeforeEnteredHouse", Vector2());
	pBlackboard->AddData("AmountOfTimeInHouse", &m_AmountOfTimeInHouse);
	pBlackboard->AddData("isDoing180", bool());
	pBlackboard->AddData("StartRotationFor180", float());
	pBlackboard->AddData("positionsToVisit", &m_lPositionsToVisit);
	pBlackboard->AddData("isPathfindingToHouse", bool());
	pBlackboard->AddData("hasReachedHouseCenter", bool());

	//Create BehaviorTree
	m_pBehaviorTree =
	{
		new Elite::BehaviorTree
		(
			pBlackboard,

			new BehaviorSelector
			(
				{
					new BehaviorPartialSequence
					(
						{
							new BehaviorConditional(BT_Conditions::IsInPurgeZone),
							new BehaviorConditional(BT_Conditions::IsInHouse),
							new BehaviorAction(BT_Actions::GoOutHouse)
						}
					),

					new BehaviorSequence
					(
						{
							new BehaviorConditional(BT_Conditions::IsInPurgeZone),
							new BehaviorAction(BT_Actions::FleePurgeZoneAgentIsIn)
						}
					),

					new BehaviorSequence
					(
						{
							new BehaviorConditional(BT_Conditions::NotMaxHealth),
							new BehaviorConditional(BT_Conditions::HasMedkit),
							new BehaviorConditional(BT_Conditions::HasEquipedMedkitHealth),
							new BehaviorAction(BT_Actions::UseMedkit)
						}
					),

					new BehaviorSequence
					(
						{
							new BehaviorConditional(BT_Conditions::AlmostOutOfEnergy),
							new BehaviorConditional(BT_Conditions::HasFood),
							new BehaviorConditional(BT_Conditions::HasEquipedFoodEnergy),
							new BehaviorAction(BT_Actions::UseFoodWithMostEnergy)
						}
					),

					new BehaviorSequence
					(
						{
							new BehaviorConditional(BT_Conditions::IsEnemyInFOV),

							new BehaviorSelector //this selector checks if the agent is doing a 180 if he is then the 180 is stopped
							(
								{
									new BehaviorConditional(BT_Conditions::IsNotDoing180),
									new BehaviorAction(BT_Actions::StopRotate180)
								}
							),

							new BehaviorSelector
							(
								{
									new BehaviorSequence
									(
										{
											new BehaviorConditional(BT_Conditions::HasPistol),
											new BehaviorConditional(BT_Conditions::HasEquipedPistolAmmo),
											new BehaviorAction(BT_Actions::FaceEnemy),
											new BehaviorAction(BT_Actions::UsePistol)
										}
									),

									new BehaviorSequence
									(
										{
											new BehaviorConditional(BT_Conditions::HasShotGun),
											new BehaviorConditional(BT_Conditions::HasEquipedShotgunAmmo),
											new BehaviorAction(BT_Actions::FaceEnemy),
											new BehaviorAction(BT_Actions::UseShotgun)
										}
									),

									new BehaviorSequence
									(
										{
											new BehaviorConditional(BT_Conditions::CanSprint),
											new BehaviorAction(BT_Actions::ChangeToSprintFlee)
										}
									),

									new BehaviorAction(BT_Actions::ChangeToFlee)
								}
							)
						}
					),

					new BehaviorSelector
					(
						{
							new BehaviorPartialSequence
							(
								{
									new BehaviorConditional(BT_Conditions::HasTakenDamage),
									new BehaviorConditional(BT_Conditions::HasPistol),
									new BehaviorAction(BT_Actions::Rotate180Degrees)
								}
							),

							new BehaviorSequence
							(
								{
									new BehaviorConditional(BT_Conditions::HasTakenDamage),
									new BehaviorConditional(BT_Conditions::HasShotGun),
									new BehaviorAction(BT_Actions::Rotate180Degrees)
								}
							),

							new BehaviorSequence
							(
								{
									new BehaviorConditional(BT_Conditions::HasTakenDamage),

									new BehaviorSelector
									(
										{
											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::CanSprint),
													new BehaviorAction(BT_Actions::ChangeToSprintFlee)
												}
											),

											new BehaviorAction(BT_Actions::ChangeToFlee)
										}
									)
								}
							)
						}
					),

					new BehaviorSequence
					(
						{
							new BehaviorConditional(BT_Conditions::IsItemInGrabrange),

							new BehaviorSelector
							(
								{
									new BehaviorSequence
									(
										{
											new BehaviorConditional(BT_Conditions::IsGarbageInGrabRange),
											new BehaviorAction(BT_Actions::DestroyGarbageInGrabrange)
										}
									),

									new BehaviorSelector
									(
										{
											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsPistolInGrabRange),
													new BehaviorConditional(BT_Conditions::HasPistol),
													new BehaviorConditional(BT_Conditions::HasEquipedPistolMoreAmmoThenPistolInGrabrange),
													new BehaviorAction(BT_Actions::DestroyPistolInGrabrange)
												}
											),

											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsPistolInGrabRange),
													new BehaviorConditional(BT_Conditions::HasPistol),
													new BehaviorAction(BT_Actions::RemoveEquipedPistol)
												}
											),

											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsPistolInGrabRange),
													new BehaviorAction(BT_Actions::GrabPistol)
												}
											)
										}
									),

									new BehaviorSelector
									(
										{
											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsShotgunInGrabRange),
													new BehaviorConditional(BT_Conditions::HasShotGun),
													new BehaviorConditional(BT_Conditions::HasEquipedShotgunMoreAmmoThenShotgunInGrabrange),
													new BehaviorAction(BT_Actions::DestroyPistolInGrabrange)
												}
											),

											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsShotgunInGrabRange),
													new BehaviorConditional(BT_Conditions::HasShotGun),
													new BehaviorAction(BT_Actions::RemoveEquipedShotgun)
												}
											),

											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsShotgunInGrabRange),
													new BehaviorAction(BT_Actions::GrabShotGun)
												}
											)
										}
									),

									new BehaviorSelector
									(
										{
											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsMedkitInGrabRange),
													new BehaviorConditional(BT_Conditions::HasMedkit),
													new BehaviorConditional(BT_Conditions::HasEquipedMedkitMoreHPThenMedkitInGrabrange),
													new BehaviorAction(BT_Actions::DestroyMedkitInGrabrange)
												}
											),

											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsMedkitInGrabRange),
													new BehaviorConditional(BT_Conditions::HasMedkit),
													new BehaviorAction(BT_Actions::RemoveEquipedMedkit)
												}
											),

											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsMedkitInGrabRange),
													new BehaviorAction(BT_Actions::GrabMedkit)
												}
											)
										}
									),

									new BehaviorSelector
									(
										{
											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsFoodInGrabRange),
													new BehaviorConditional(BT_Conditions::HasFood),
													new BehaviorConditional(BT_Conditions::HasEquipedFoodMoreEnergyThenFoodInGrabrange),
													new BehaviorAction(BT_Actions::DestroyFoodInGrabrange)
												}
											),

											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsFoodInGrabRange),
													new BehaviorConditional(BT_Conditions::HasFood),
													new BehaviorAction(BT_Actions::RemoveEquipedFoodWithLeastEnergy)
												}
											),

											new BehaviorSequence
											(
												{
													new BehaviorConditional(BT_Conditions::IsFoodInGrabRange),
													new BehaviorAction(BT_Actions::GrabFood)
												}
											)
										}
									)
								}
							)
						}
					),

					new BehaviorPartialSequence
					(
						{
							new BehaviorConditional(BT_Conditions::IsInHouse),
							new BehaviorConditional(BT_Conditions::HasBeenInHouseFor10Seconds),
							new BehaviorAction(BT_Actions::GoOutHouse)
						}
					),

					new BehaviorPartialSequence
					(
						{
							new BehaviorConditional(BT_Conditions::IsHouseInFOV),
							new BehaviorConditional(BT_Conditions::HasNotEnteredHouseInFOVInlast30Sec),
							new BehaviorAction(BT_Actions::PathfindToHouseInFOV)
						}
					),

					new BehaviorPartialSequence
					(
						{
							new BehaviorConditional(BT_Conditions::IsItemInFOV),
							new BehaviorConditional(BT_Conditions::IsNotPathfindingToHouse),
							new BehaviorAction(BT_Actions::PathfindToItemInFOV)
						}
					),

					new BehaviorSequence
					(
						{
							new BehaviorConditional(BT_Conditions::HasReachedHouseCenter),
							new BehaviorAction(BT_Actions::ChangeToWander)
						}
					),

					new BehaviorPartialSequence
					(
						{
							new BehaviorConditional(BT_Conditions::IsOutside),
							new BehaviorConditional(BT_Conditions::IsNotPathfindingToHouse),
							new BehaviorConditional(BT_Conditions::HasNotExploreFullWorld),
							new BehaviorAction(BT_Actions::GoToNextNotVisitedPosClosestToOrigin)
						}
					),

					new BehaviorPartialSequence
					(
						{
							new BehaviorConditional(BT_Conditions::HasExploreFullWorld),
							new BehaviorAction(BT_Actions::PathfindToNextHouse)
						}
					)
				}
			)
		)
	};

	//Bit information about the plugin
	//Please fill this in!!
	info.BotName = "Jarne";
	info.Student_FirstName = "Jarne";
	info.Student_LastName = "Wieme";
	info.Student_Class = "2DAE15";
}

//Called only once
void Plugin::DllInit()
{
	//Called when the plugin is loaded

	//create the steeringBehaviors
	m_pWander = new Wander();
	m_pFace = new Face();
	m_pSeek = new Seek();
	m_pFlee = new Flee();
	m_pSprintFlee = new SprintFlee();

	//fill the list m_lPositionsToVisit with position for the agent to visit
	for (int x{ -350 }; x < 350; x += 20)
	{
		for (int y{ -350 }; y < 350; y += 20)
		{
			m_lPositionsToVisit.push_back(Vector2(static_cast<float>(x), static_cast<float>(y)));
		}
	}
}

//Called only once
void Plugin::DllShutdown()
{
	//Called wheb the plugin gets unloaded
	SAFE_DELETE(m_pBehaviorTree);
	SAFE_DELETE(m_pWander);
}

//Called only once, during initialization
void Plugin::InitGameDebugParams(GameDebugParams& params)
{
	params.AutoFollowCam = true; //Automatically follow the AI? (Default = true)
	params.RenderUI = true; //Render the IMGUI Panel? (Default = true)
	params.SpawnEnemies = true; //Do you want to spawn enemies? (Default = true)
	params.EnemyCount = 20; //How many enemies? (Default = 20)
	params.GodMode = false; //GodMode > You can't die, can be useful to inspect certain behaviors (Default = false)
	params.LevelFile = "GameLevel.gppl";
	params.AutoGrabClosestItem = true; //A call to Item_Grab(...) returns the closest item that can be grabbed. (EntityInfo argument is ignored)
	params.StartingDifficultyStage = 1;
	params.InfiniteStamina = false;
	params.SpawnDebugPistol = true;
	params.SpawnDebugShotgun = true;
	params.SpawnPurgeZonesOnMiddleClick = true;
	params.PrintDebugMessages = true;
	params.ShowDebugItemNames = true;
	params.Seed = 36;
}

//Only Active in DEBUG Mode
//(=Use only for Debug Purposes)
void Plugin::Update(float dt)
{
	for (const auto& pos : m_lPositionsToVisit) //draw the position that the agent has not yet visited
	{
		m_pInterface->Draw_Point(pos, 1, Vector3(1.f, 1.f, 1.f));
	}

	//Demo Event Code
	//In the end your AI should be able to walk around without external input
	if (m_pInterface->Input_IsMouseButtonUp(Elite::InputMouseButton::eLeft))
	{
		//Update target based on input
		Elite::MouseData mouseData = m_pInterface->Input_GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
		const Elite::Vector2 pos = Elite::Vector2(static_cast<float>(mouseData.X), static_cast<float>(mouseData.Y));
		m_Target = m_pInterface->Debug_ConvertScreenToWorld(pos);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Space))
	{
		m_CanRun = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Left))
	{
		m_AngSpeed -= Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Right))
	{
		m_AngSpeed += Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_G))
	{
		m_GrabItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_U))
	{
		m_UseItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_R))
	{
		m_RemoveItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyUp(Elite::eScancode_Space))
	{
		m_CanRun = false;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Delete))
	{
		m_pInterface->RequestShutdown();
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Minus))
	{
		if (m_InventorySlot > 0)
			--m_InventorySlot;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Plus))
	{
		if (m_InventorySlot < 4)
			++m_InventorySlot;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Q))
	{
		ItemInfo info = {};
		m_pInterface->Inventory_GetItem(m_InventorySlot, info);
		std::cout << (int)info.Type << std::endl;
	}

	if (m_pInterface->Agent_GetInfo().IsInHouse)
	{
		m_AmountOfTimeInHouse += dt;
	}
	else
	{
		m_AmountOfTimeInHouse = 0.f;
	}
}

//Update
//This function calculates the new SteeringOutput, called once per frame
SteeringPlugin_Output Plugin::UpdateSteering(float dt)
{	
	m_pBehaviorTree->Update(dt);

	m_vEntitiesInFOV = GetEntitiesInFOV();
	m_vHousesInFOV = GetHousesInFOV();

	//if the agent is outside the set the bool hasReachedHouseCenter to false
	if (!m_pInterface->Agent_GetInfo().IsInHouse)
	{
		m_pBehaviorTree->GetBlackboard()->ChangeData("hasReachedHouseCenter", false);
	}

	//remove the position that are in the FOV range of the agent
	for (const auto& pos : m_lPositionsToVisit)
	{
		if (pos.x - m_pInterface->Agent_GetInfo().FOV_Range <= m_pInterface->Agent_GetInfo().Position.x && pos.x + m_pInterface->Agent_GetInfo().FOV_Range >= m_pInterface->Agent_GetInfo().Position.x
			&& pos.y - m_pInterface->Agent_GetInfo().FOV_Range <= m_pInterface->Agent_GetInfo().Position.y && pos.y + m_pInterface->Agent_GetInfo().FOV_Range >= m_pInterface->Agent_GetInfo().Position.y)
		{
			m_lPositionsToVisit.remove(pos);
			break;
		}
	}

	//update the amount of time it has been since the entered houses where entered
	for (auto& house : m_vHousesEntered)
	{
		house.second += dt;
	}

	//get the steeringBehaviorType and assing the correct behavior to m_pSteeringBehavior
	SteeringBehaviorType steeringBehaviorType;
	if (m_pBehaviorTree->GetBlackboard()->GetData("SteeringBehaviorType", steeringBehaviorType))
	{
		Vector2 target;

		switch (steeringBehaviorType)
		{
		case SteeringBehaviorType::none:
			m_pSteeringBehavior = nullptr;
			break;

		case SteeringBehaviorType::wander:
			m_pSteeringBehavior = m_pWander;
			break;

		case SteeringBehaviorType::face:
			m_pSteeringBehavior = m_pFace;
			m_pBehaviorTree->GetBlackboard()->GetData("Target", target);
			m_pFace->SetTarget(target);
			break;

		case SteeringBehaviorType::seek:
			m_pSteeringBehavior = m_pSeek;
			m_pBehaviorTree->GetBlackboard()->GetData("Target", target);
			m_pSeek->SetTarget(target);
			break;

		case SteeringBehaviorType::flee:
			m_pSteeringBehavior = m_pFlee;
			m_pBehaviorTree->GetBlackboard()->GetData("Target", target);
			m_pFlee->SetTarget(target);
			break;

		case SteeringBehaviorType::sprintFlee:
			m_pSteeringBehavior = m_pSprintFlee;
			m_pBehaviorTree->GetBlackboard()->GetData("Target", target);
			m_pSprintFlee->SetTarget(target);
			break;
		}
	}

	if(m_pSteeringBehavior)
	return m_pSteeringBehavior->CalculateSteering(dt, m_pInterface->Agent_GetInfo());

	return SteeringPlugin_Output();
}

//This function should only be used for rendering debug elements
void Plugin::Render(float dt) const
{
	//This Render function should only contain calls to Interface->Draw_... functions
	m_pInterface->Draw_SolidCircle(m_Target, .7f, { 0,0 }, { 1, 0, 0 });
}

std::vector<HouseInfo> Plugin::GetHousesInFOV() const
{
	std::vector<HouseInfo> vHousesInFOV = {};

	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetHouseByIndex(i, hi))
		{
			vHousesInFOV.push_back(hi);
			continue;
		}

		break;
	}

	return vHousesInFOV;
}

std::vector<EntityInfo> Plugin::GetEntitiesInFOV() const
{
	std::vector<EntityInfo> vEntitiesInFOV = {};

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetEntityByIndex(i, ei))
		{
			vEntitiesInFOV.push_back(ei);
			continue;
		}

		break;
	}

	return vEntitiesInFOV;
}