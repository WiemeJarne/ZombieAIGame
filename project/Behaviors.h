/*=============================================================================*/
// Copyright 2020-2021 Elite Engine


//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "EBehaviorTree.h"
#include "SteeringBehaviors.h"

using namespace Elite;

//-----------------------------------------------------------------
// Behaviors
//-----------------------------------------------------------------

namespace BT_Actions
{
	//grabbing items
	BehaviorState GrabPistol(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntitiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntitiesInFOV) || !pvEntitiesInFOV)
			return BehaviorState::Failure;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		for (const auto& entity : *pvEntitiesInFOV) //loop over all the entities in the fov
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo from it

			if (itemInfo.Type != eItemType::PISTOL) continue; //if the item is not food continue to the next entity
			
			pInterface->Item_Grab(entity, itemInfo);
			pInterface->Inventory_AddItem(0, itemInfo);
			return BehaviorState::Success;
		}
		
		return BehaviorState::Failure; //no pistol was found and thus none was grabbed
	}

	BehaviorState GrabShotGun(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the fov
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the info from it

			if (itemInfo.Type != eItemType::SHOTGUN) continue; //if the item is not a shotgun continue to the next entity
			
			pInterface->Item_Grab(entity, itemInfo);
			pInterface->Inventory_AddItem(1, itemInfo);
			return BehaviorState::Success;
		}

		return BehaviorState::Failure; //no shotgun was found and thus none was grabbed
	}

	BehaviorState GrabMedkit(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the fov
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo from it

			if (itemInfo.Type != eItemType::MEDKIT) continue; //if the item is not a medkit continue to the next entity
			
			pInterface->Item_Grab(entity, itemInfo);
			pInterface->Inventory_AddItem(2, itemInfo);
			return BehaviorState::Success;
		}

		return BehaviorState::Failure; //no medkit was found and thus none was grabbed
	}

	BehaviorState GrabFood(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the fov
		{
			if (!(entity.Type == eEntityType::ITEM)) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo from it

			if (itemInfo.Type != eItemType::FOOD) continue; //if the item is not food continue to the next entity

			ItemInfo slot3;
			if (!pInterface->Inventory_GetItem(3, slot3)) //if slot 3 is empty grab and add the food in slot3
			{
				pInterface->Item_Grab(entity, itemInfo);
				pInterface->Inventory_AddItem(3, itemInfo);
				return BehaviorState::Success;
			}

			ItemInfo slot4;
			if (!pInterface->Inventory_GetItem(4, slot4)) //if slot 3 is empty grab and add the food in slot4
			{
				pInterface->Item_Grab(entity, itemInfo);
				pInterface->Inventory_AddItem(4, itemInfo);
				return BehaviorState::Success;
			}
		}	

		return BehaviorState::Failure; //no food was found and thus none was grabbed
	}

	//destroying items
	BehaviorState DestroyPistolInGrabrange(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo of the entity

			if (itemInfo.Type != eItemType::PISTOL) continue; //if the entity is not a pistol continue to the next entity
			
			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the pistol is in the grabrange
			{
				pInterface->Item_Destroy(entity);
				return BehaviorState::Success;
			}
		}

		return BehaviorState::Failure; //no pistol was found and thus none was destroyed
	}

	BehaviorState DestroyShotgunInGrabrange(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo of the entity

			if (itemInfo.Type != eItemType::SHOTGUN) continue; //if the entity is not a shotgun continue to the next entity
			
			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the shotgun is in the grabrange
			{
				pInterface->Item_Destroy(entity);
				return BehaviorState::Success;
			}
		}

		return BehaviorState::Failure; //no shotgun was found and thus none was destroyed
	}

	BehaviorState DestroyMedkitInGrabrange(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;
		
		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo of the entity

			if (itemInfo.Type != eItemType::MEDKIT) continue; //if the entity is not a medkit continue to the next entity
			
			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the medkit is in the grabrange
			{
				pInterface->Item_Destroy(entity);
				return BehaviorState::Success;
			}
		}

		return BehaviorState::Failure; //no medkit was found and thus none was destroyed
	}

	BehaviorState DestroyFoodInGrabrange(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo of the entity
			
			if (itemInfo.Type != eItemType::FOOD) continue; //if the entity is not food continue to the next entity
			
			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the food is in the grabrange
			{
				pInterface->Item_Destroy(entity);
				return BehaviorState::Success;
			}
		}

		return BehaviorState::Failure; //no food was found and thus none was destroyed
	}

	BehaviorState DestroyGarbageInGrabrange(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo of the entity

			if (itemInfo.Type != eItemType::GARBAGE) continue; //if the entity is not garbage continue to the next entity
			

			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the food is in the grabrange
			{
				pInterface->Item_Destroy(entity);
				return BehaviorState::Success;
			}
		}

		return BehaviorState::Failure; //no garbage was found and thus none was destroyed
	}

	//removing items out inventory
	BehaviorState RemoveEquipedPistol(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		if(pInterface->Inventory_RemoveItem(0)) //pistol is always saved in slot 0
			return BehaviorState::Success;

		return BehaviorState::Failure; //no pistol was found and thus none was removed
	}

	BehaviorState RemoveEquipedShotgun(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		if(pInterface->Inventory_RemoveItem(1)) //shotgun is always saved in slot 1
			return BehaviorState::Success;

		return BehaviorState::Failure; //no shotgun was found and thus none was removed
	}

	BehaviorState RemoveEquipedMedkit(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		if(pInterface->Inventory_RemoveItem(2)) //medkit is always saved in slot 2
			return BehaviorState::Success;

		return BehaviorState::Failure; //no medkit was found and thus none was removed
	}

	BehaviorState RemoveEquipedFoodWithLeastEnergy(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		ItemInfo slot3;
		ItemInfo slot4;

		if(pInterface->Inventory_GetItem(3, slot3)
			&& pInterface->Inventory_GetItem(4, slot4)) //check if there is food in slot3 and slot4, if so remove the food with the least amount of energy
		{
			if (pInterface->Food_GetEnergy(slot3) < pInterface->Food_GetEnergy(slot4))
			{
				pInterface->Inventory_RemoveItem(3);
				return BehaviorState::Success;
			}
			
			pInterface->Inventory_RemoveItem(4);
			return BehaviorState::Success;
		}

		if (pInterface->Inventory_GetItem(3, slot3)) //if there is no food in both check if there is food in slot3, if so is remove it
		{
			pInterface->Inventory_RemoveItem(3);
			return BehaviorState::Success;
		}
		
		if (pInterface->Inventory_GetItem(4, slot3)) //if there is no food in 3 check if there is food in slot4, if so remove it
		{
			pInterface->Inventory_RemoveItem(4);
			return BehaviorState::Success;
		}
		
		return BehaviorState::Failure; //no food was found and thus none was removed
	}

	//using items
	BehaviorState UsePistol(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		ItemInfo itemInfo;
		pInterface->Inventory_GetItem(0, itemInfo); //get the ItemInfo of the item in slot0

		if (itemInfo.Type != eItemType::PISTOL) //check if the item in slot0 is a pistol
			return BehaviorState::Failure;

		if(pInterface->Inventory_UseItem(0))
			return BehaviorState::Success;

		return BehaviorState::Failure; //using the pistol failed
	}

	BehaviorState UseShotgun(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		ItemInfo itemInfo;
		pInterface->Inventory_GetItem(1, itemInfo); //get the ItemInfo of the item in slot1

		if (itemInfo.Type != eItemType::SHOTGUN) //check if the item in slot1 is a shotgun
			return BehaviorState::Failure;

		if(pInterface->Inventory_UseItem(1))
			return BehaviorState::Success;

		return BehaviorState::Failure; //using the shotgun failed
	}

	BehaviorState UseMedkit(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		ItemInfo itemInfo;
		pInterface->Inventory_GetItem(2, itemInfo); //get the ItemInfo of the item in slot2

		if (itemInfo.Type != eItemType::SHOTGUN) //check if the item in slot1 is a medkit
			return BehaviorState::Failure;

		if (pInterface->Inventory_UseItem(2))
			return BehaviorState::Success;

		return BehaviorState::Failure; //using the medkit failed
	}

	BehaviorState UseFoodWithMostEnergy(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;
		
		ItemInfo slot3;
		pInterface->Inventory_GetItem(3, slot3);

		ItemInfo slot4;
		pInterface->Inventory_GetItem(4, slot4);

		if (slot3.Type == eItemType::FOOD && slot4.Type == eItemType::FOOD) //first check if slot 3 and 4 contain food if so use the one with the most energy
		{
			if (pInterface->Food_GetEnergy(slot3) == 0 && pInterface->Food_GetEnergy(slot4) == 0) //check at least 1 of 2 possible slots that contain food has energy
				return BehaviorState::Failure;

			if (pInterface->Food_GetEnergy(slot3) < pInterface->Food_GetEnergy(slot4))
			{
				pInterface->Inventory_UseItem(3); //the food in slot 3 has the most energy
				return BehaviorState::Success;
			}
			
			pInterface->Inventory_UseItem(4); //the food in slot 4 has the most energy
			return BehaviorState::Success;
		}

		if (slot3.Type == eItemType::FOOD) //if the previous if fails check if there is food in slot 3 and use it
		{
			if (pInterface->Food_GetEnergy(slot3) > 0)//check if the food in slot 3 has energy
			{
				pInterface->Inventory_UseItem(3);
				return BehaviorState::Success;
			}
		}

		if (slot4.Type == eItemType::FOOD) //if the previous if fails check if there is food in slot 4 and use it
		{
			if (pInterface->Food_GetEnergy(slot4) > 0) //check if the food in slot 4 has energy
			{
				pInterface->Inventory_UseItem(4);
				return BehaviorState::Success;
			}
		}

		//no food found so the action failed
		return BehaviorState::Failure; 
	}

	//house actions
	BehaviorState PathfindToHouseInFOV(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		std::vector<HouseInfo>* pvHousesInFOV;
		if (!pBlackboard->GetData("HousesInFOV", pvHousesInFOV) || !pvHousesInFOV)
			return BehaviorState::Failure;

		std::list<std::pair<Vector2, float>>* pvHousesEntered;
		if (!pBlackboard->GetData("HousesEntered", pvHousesEntered) || !pvHousesEntered)
			return BehaviorState::Failure;

		if (!pInterface->Agent_GetInfo().IsInHouse)
		{
			pBlackboard->ChangeData("posBeforeEnteredHouse", pInterface->Agent_GetInfo().Position);
		}

		HouseInfo houseInfo{};
		std::list<HouseInfo*> possibleHousesToEnter{};

		for (auto& houseInFOV : *pvHousesInFOV) //fill the possibleHousesToEnter list with the houses in the FOV
		{
			possibleHousesToEnter.push_back(&houseInFOV);
		}

		for (auto& houseInFOV : *pvHousesInFOV) //loop over all the houses in the FOV
		{
			for (const auto& enteredHouse : *pvHousesEntered) //loop over all the entered houses
			{
				//compare the center of the house in the FOV with the center of the enteredHouse
				//if they are not equal they are not the same house so continue to the next house
				if (houseInFOV.Center != enteredHouse.first)
				{
					possibleHousesToEnter.remove(&houseInFOV);
					continue;
				}
				//if they are the same house check if it has been entered in the last 30 seconds if so then continue to the next house
				if (enteredHouse.second <= 30.f)
				{
					possibleHousesToEnter.remove(&houseInFOV);
					continue;
				}
			}
		}

		if (possibleHousesToEnter.size() == 0)
			return BehaviorState::Failure; //no possible houses to enter

		houseInfo = *possibleHousesToEnter.front();

		pBlackboard->ChangeData("Target", pInterface->NavMesh_GetClosestPathPoint(houseInfo.Center));

		pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::seek);

		pBlackboard->ChangeData("isPathfindingToHouse", true);

		if (pInterface->Agent_GetInfo().Position.x - 0.5f <= houseInfo.Center.x && pInterface->Agent_GetInfo().Position.x + 0.5f >= houseInfo.Center.x
			&& pInterface->Agent_GetInfo().Position.y - 0.5f <= houseInfo.Center.y && pInterface->Agent_GetInfo().Position.y + 0.5f >= houseInfo.Center.y)
		{
			std::list<std::pair<Vector2, float>>* pvHousesEntered;
			if (!pBlackboard->GetData("HousesEntered", pvHousesEntered) || !pvHousesEntered)
				return BehaviorState::Failure;

			//check if the house where the agent is now in was already visited before if so set the amount of time since last visit to zero else push the house the the pvHousesEntered vector
			for (auto& enteredHouse : *pvHousesEntered)
			{
				if (houseInfo.Center == enteredHouse.first)
				{
					enteredHouse.second = 0.f;
					pBlackboard->ChangeData("isPathfindingToHouse", false);
					pBlackboard->ChangeData("hasReachedHouseCenter", true);
					return BehaviorState::Success;
				}
			}

			pvHousesEntered->push_back(std::pair<Vector2, float>(houseInfo.Center, 0.f));

			pBlackboard->ChangeData("isPathfindingToHouse", false);
			pBlackboard->ChangeData("hasReachedHouseCenter", true);

			return BehaviorState::Success;
		}

		return BehaviorState::Running;
	}

	BehaviorState GoOutHouse(Blackboard* pBlackboard)
	{
		Vector2 posBeforeEnteredHouse;
		if (!pBlackboard->GetData("posBeforeEnteredHouse", posBeforeEnteredHouse))
			return BehaviorState::Failure;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		SteeringBehaviorType pSteeringBehaviorType;
		if (!pBlackboard->GetData("SteeringBehaviorType", pSteeringBehaviorType))
			return BehaviorState::Failure;

		pBlackboard->ChangeData("Target", pInterface->NavMesh_GetClosestPathPoint(posBeforeEnteredHouse));

		pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::seek);

		if (pInterface->Agent_GetInfo().Position.x - 0.1 <= posBeforeEnteredHouse.x && pInterface->Agent_GetInfo().Position.x + 0.1 >= posBeforeEnteredHouse.x)
		{
			return BehaviorState::Success;
		}

		return BehaviorState::Running;
	}

	BehaviorState StopPathfindingToHouse(Blackboard* pBlackboard)
	{
		pBlackboard->ChangeData("isPathfindingToHouse", false);
		return BehaviorState::Success;
	}

	BehaviorState PathfindToNextHouse(Blackboard* pBlackboard) //looks at the houses that has been entered and pathfinds to the first that has not been entered in the past 30 seconds
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		std::list<std::pair<Vector2, float>>* pvHousesEntered;
		if (!pBlackboard->GetData("HousesEntered", pvHousesEntered) || !pvHousesEntered)
			return BehaviorState::Failure;

		if (!pInterface->Agent_GetInfo().IsInHouse)
		{
			pBlackboard->ChangeData("posBeforeEnteredHouse", pInterface->Agent_GetInfo().Position);
			pBlackboard->ChangeData("isPathfindingToHouse", true);
		}

		Vector2 houseCenter{};
		bool houseFound{};
		
		for (const auto& enteredHouse : *pvHousesEntered) //loop over all the entered houses
		{
			//check if the house has been entered in the past 30 seconds if so continue to the next house
			if (enteredHouse.second <= 30.f) continue;
			
			houseCenter = enteredHouse.first;
			houseFound = true;
		}

		if (!houseFound)
			return BehaviorState::Failure; //couldn't find a house that has not been entered in the past 30 seconds

		pBlackboard->ChangeData("Target", pInterface->NavMesh_GetClosestPathPoint(houseCenter));

		pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::seek);

		if (pInterface->Agent_GetInfo().Position.x - 0.5f <= houseCenter.x && pInterface->Agent_GetInfo().Position.x + 0.5f >= houseCenter.x
			&& pInterface->Agent_GetInfo().Position.y - 0.5f <= houseCenter.y && pInterface->Agent_GetInfo().Position.y + 0.5f >= houseCenter.y)
		{
			std::list<std::pair<Vector2, float>>* pvHousesEntered;
			if (!pBlackboard->GetData("HousesEntered", pvHousesEntered) || !pvHousesEntered)
				return BehaviorState::Failure;

			//check if the house where the agent is now in was already visited before if so set the amount of time since last visit to zero else push the house the the pvHousesEntered vector
			for (auto& enteredHouse : *pvHousesEntered)
			{
				if (houseCenter == enteredHouse.first)
				{
					enteredHouse.second = 0.f;
					pBlackboard->ChangeData("isPathfindingToHouse", false);
					pBlackboard->ChangeData("hasReachedHouseCenter", true);
					return BehaviorState::Success;
				}
			}

			return BehaviorState::Success;
		}

		return BehaviorState::Running;
	}

	//change steering behavior
	BehaviorState ChangeToWander(Blackboard* pBlackboard)
	{
		SteeringBehaviorType pSteeringBehaviorType;
		if (!pBlackboard->GetData("SteeringBehaviorType", pSteeringBehaviorType))
			return BehaviorState::Failure;

		if (pSteeringBehaviorType == SteeringBehaviorType::wander)
			return BehaviorState::Success;

		pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::wander);

		return BehaviorState::Success;
	}

	BehaviorState ChangeToFlee(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;

		pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::flee);

		Vector2 target;

		if (!pvEntetiesInFOV->empty())
		{
			for (const auto& entity : *pvEntetiesInFOV)
			{
				if ((entity.Type == eEntityType::ENEMY))
				{
					target = entity.Location;
				}
			}
		}
		else //if no enemy in vision then flee from own position
		{
			target = pInterface->Agent_GetInfo().Position;
		}

		pBlackboard->ChangeData("Target", target);

		return BehaviorState::Success;
	}

	BehaviorState ChangeToSprintFlee(Blackboard* pBlackboard)
	{
		SteeringBehaviorType pSteeringBehaviorType;
		if (!pBlackboard->GetData("SteeringBehaviorType", pSteeringBehaviorType))
			return BehaviorState::Failure;

		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;

		pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::flee);

		if (!pvEntetiesInFOV->empty())
		{
			Vector2 target;

			for (const auto& entity : *pvEntetiesInFOV)
			{
				if ((entity.Type == eEntityType::ENEMY))
				{
					target = entity.Location;
				}
			}

			pBlackboard->ChangeData("Target", target);
		}

		return BehaviorState::Success;
	}

	BehaviorState PathfindToItemInFOV(Blackboard* pBlackboard) //seeks to the location saved in the blackboard as posItemToSeek
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		std::vector<EntityInfo>* pvEntitiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntitiesInFOV) || !pvEntitiesInFOV)
			return BehaviorState::Failure;

		if (pvEntitiesInFOV->empty())
			return BehaviorState::Failure; //nothing to pathfind to

		for (const auto& entity : *pvEntitiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			pBlackboard->ChangeData("Target", pInterface->NavMesh_GetClosestPathPoint(entity.Location));
			pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::seek);
			
			if (pInterface->Agent_GetInfo().Position.x - 0.1f <= entity.Location.x && pInterface->Agent_GetInfo().Position.x + 0.1f >= entity.Location.x)
			{
				return BehaviorState::Success;
			}
		}

		return BehaviorState::Running;
	}

	//other
	BehaviorState GoToNextNotVisitedPosClosestToOrigin(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		std::list<Vector2>* lPositionsToVisit;
		if (!pBlackboard->GetData("positionsToVisit", lPositionsToVisit) || !lPositionsToVisit)
			return BehaviorState::Failure;

		Vector2 posClosestToOrigin{INFINITY, INFINITY};
		for (const auto& pos : *lPositionsToVisit)
		{
			if (pos.Magnitude() < posClosestToOrigin.Magnitude())
			{
				posClosestToOrigin = pos;
			}
		}

		//check if the agent reach a position
		if (pInterface->Agent_GetInfo().Position.x - 0.1f <= posClosestToOrigin.x && pInterface->Agent_GetInfo().Position.x + 0.1f >= posClosestToOrigin.x)
		{
			//remove the reached pos from the queue
			lPositionsToVisit->remove(posClosestToOrigin);
			return BehaviorState::Success;
		}

		pBlackboard->ChangeData("Target", pInterface->NavMesh_GetClosestPathPoint(posClosestToOrigin));

		pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::seek);

		return BehaviorState::Running;
	}

	BehaviorState FleePurgeZoneAgentIsIn(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntitiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntitiesInFOV) || !pvEntitiesInFOV)
			return BehaviorState::Failure;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		if (pvEntitiesInFOV->empty()) return BehaviorState::Failure;

		for (const auto& entity : *pvEntitiesInFOV)
		{
			if (entity.Type == eEntityType::PURGEZONE)
			{
				PurgeZoneInfo purgZoneInfo;
				pInterface->PurgeZone_GetInfo(entity, purgZoneInfo);

				if ((purgZoneInfo.Center - pInterface->Agent_GetInfo().Position).Magnitude() > purgZoneInfo.Radius)
				{
					return BehaviorState::Success;
				}

				pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::flee);
				pBlackboard->ChangeData("Target", purgZoneInfo.Center);
			}
		}

		return BehaviorState::Running;
	}

	BehaviorState FaceEnemy(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		SteeringBehaviorType pSteeringBehaviorType;
		if (!pBlackboard->GetData("SteeringBehaviorType", pSteeringBehaviorType))
			return BehaviorState::Failure;

		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return BehaviorState::Failure;

		Vector2 target;

		for (const auto& entity : *pvEntetiesInFOV)
		{
			if ((entity.Type == eEntityType::ENEMY))
			{
				target = entity.Location;
			}
		}

		pBlackboard->ChangeData("Target", target);

		Vector2 toTarget{ target - pInterface->Agent_GetInfo().Position };
		toTarget.Normalize();

		float desiredAngle{ acos(toTarget.Dot({ 1.f, 0.f })) };

		float cross{ toTarget.Cross({1.f, 0.f}) };

		if (cross > 0) //toTarget ligt rechts van unitX vector
		{
			desiredAngle *= -1;
		}

		if (desiredAngle - 0.02f <= pInterface->Agent_GetInfo().Orientation && desiredAngle + 0.02f >= pInterface->Agent_GetInfo().Orientation) // can be about 1 radians off
		{
			return BehaviorState::Success;
		}

		pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::face);

		return BehaviorState::Running;
	}

	BehaviorState Rotate180Degrees(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return BehaviorState::Failure;

		SteeringBehaviorType pSteeringBehaviorType;
		if (!pBlackboard->GetData("SteeringBehaviorType", pSteeringBehaviorType))
			return BehaviorState::Failure;

		bool isDoing180;
		if (!pBlackboard->GetData("isDoing180", isDoing180))
			return BehaviorState::Failure;

		float startRotationFor180;
		if (!pBlackboard->GetData("StartRotationFor180", startRotationFor180))
			return BehaviorState::Failure;

		if (!isDoing180)
		{
			startRotationFor180 = pInterface->Agent_GetInfo().Orientation;
		}

		Vector2 point;

		if (startRotationFor180 < 0 && startRotationFor180 > -(M_PI / 2.f))
		{
			point = { 1.f, -1.f };
		}
		else if (startRotationFor180 < M_PI / 2.f)
		{
			point = { -1.f, -1.f };
		}
		else if (startRotationFor180 > 0 && startRotationFor180 < M_PI / 2.f)
		{
			point = { 1.f, 1.f };
		}
		else
		{
			point = { 1.f, 1.f };
		}

		float desiredAngle{ acos(point.Dot({ 1.f, 0.f })) };

		if (!isDoing180)
		{
			pBlackboard->ChangeData("isDoing180", true);
			startRotationFor180 = pInterface->Agent_GetInfo().Orientation;

			Vector2 target;
			target = point * (-desiredAngle);
			target.x += pInterface->Agent_GetInfo().Position.x;
			target.y += pInterface->Agent_GetInfo().Position.y;

			pBlackboard->ChangeData("Target", target);
		}

		if (desiredAngle - M_PI / 2.f - 0.02f <= pInterface->Agent_GetInfo().Orientation && desiredAngle - M_PI / 2.f + 0.02f >= pInterface->Agent_GetInfo().Orientation) // can be about 1 radians off
		{
			pBlackboard->ChangeData("isDoing180", false);
			return BehaviorState::Success;
		}

		pBlackboard->ChangeData("SteeringBehaviorType", SteeringBehaviorType::face);

		return BehaviorState::Running;
	}

	BehaviorState StopRotate180(Blackboard* pBlackboard)
	{
		if (pBlackboard->ChangeData("isDoing180", false))
		{
			return BehaviorState::Success;
		}

		return BehaviorState::Failure;
	}
}

//-----------------------------------------------------------------
// Conditions
//-----------------------------------------------------------------

namespace BT_Conditions
{
	bool IsItemInFOV(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntitiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntitiesInFOV) || !pvEntitiesInFOV)
			return false;

		for (const auto& entity : *pvEntitiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity
			
			return true;
		}

		return false;
	}

	bool IsItemInGrabrange(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntitiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntitiesInFOV) || !pvEntitiesInFOV)
			return false;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		for (const auto& entity : *pvEntitiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity
			
			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo

			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the item is in the grabrange
			{
				return true;
			}
		}

		return false;
	}

	bool IsPistolInGrabRange(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return false;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (!(entity.Type == eEntityType::ITEM)) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo

			if (itemInfo.Type != eItemType::PISTOL) continue; //if the item is not a pistol continue to the next entity
			
			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the pistol is in grabrange
				return true;
		}

		return false;
	}

	bool IsShotgunInGrabRange(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return false;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (!(entity.Type == eEntityType::ITEM)) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo

			if (itemInfo.Type != eItemType::SHOTGUN) continue; //if the item is not a shotgun continue to the next entity
			
			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the shotgun is in the grabrange
				return true;
		}

		return false;
	}

	bool IsMedkitInGrabRange(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return false;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (!(entity.Type == eEntityType::ITEM)) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo

			if (itemInfo.Type != eItemType::MEDKIT) continue; //if the item is not a medkit continue to the next entity
			
			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the medkit is in grabrange
				return true;
		}

		return false;
	}

	bool IsFoodInGrabRange(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return false;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo

			if (itemInfo.Type != eItemType::FOOD) //if the item is not food continue to the next entity
			
			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the food is in grabrange
				return true;
		}

		return false;
	}

	bool IsGarbageInGrabRange(Blackboard* pBlackBoard)
	{
		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackBoard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return false;

		IExamInterface* pInterface;
		if (!pBlackBoard->GetData("Interface", pInterface) || !pInterface)
			return false;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo

			if (itemInfo.Type != eItemType::GARBAGE) continue; //if the item is not food continue to the next entity
			
			if ((itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //check if the garbage is in grabrange
				return true;
		}

		return false;
	}

	bool IsEnemyInFOV(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntetiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntetiesInFOV) || !pvEntetiesInFOV)
			return false;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		for (const auto& entity : *pvEntetiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type == eEntityType::ENEMY) //if the entity is an enemy return true
				return true;
		}

		return false;
	}

	bool IsHouseInFOV(Blackboard* pBlackboard)
	{
		std::vector<HouseInfo>* pvHousesInFOV;
		if (!pBlackboard->GetData("HousesInFOV", pvHousesInFOV) || !pvHousesInFOV)
			return false;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;
			

		if (!pvHousesInFOV->empty())
			return true;

		return false;
	}

	bool IsInHouse(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		return pInterface->Agent_GetInfo().IsInHouse;
	}

	bool IsOutside(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		return !pInterface->Agent_GetInfo().IsInHouse;
	}

	bool HasNotEnteredHouseInFOVInlast30Sec(Blackboard* pBlackboard)
	{
		std::vector<HouseInfo>* pvHousesInFOV;
		if (!pBlackboard->GetData("HousesInFOV", pvHousesInFOV) || !pvHousesInFOV)
			return false;

		std::list<std::pair<Vector2, float>>* pvHousesEntered;
		if (!pBlackboard->GetData("HousesEntered", pvHousesEntered) || !pvHousesEntered)
			return false;

		std::list<HouseInfo*> possibleHousesToEnter{};

		for (auto& houseInFOV : *pvHousesInFOV) //fill the possibleHousesToEnter list with the houses in the FOV
		{
			possibleHousesToEnter.push_back(&houseInFOV);
		}

		for (auto& houseInFOV : *pvHousesInFOV) //loop over all the houses in the FOV
		{
			for (const auto& enteredHouse : *pvHousesEntered) //loop over all the entered houses
			{
				//compare the center of the house in the FOV with the center of the enteredHouse
				//if they are not equal they are not the same house so continue to the next house
				if (houseInFOV.Center != enteredHouse.first)
				{
					possibleHousesToEnter.remove(&houseInFOV);
					continue;
				}
				//if they are the same house check if it has been entered in the last 30 seconds if so then continue to the next house
				if (enteredHouse.second <= 30.f)
				{
					possibleHousesToEnter.remove(&houseInFOV);
					continue;
				}
			}
		}

		if (possibleHousesToEnter.size() == 0)
			return false; //the house has never been entered before or the house was entered longer then 30 seconds ago

		return true; //the house has been entered in the last 30 seconds
	}

	bool HasBeenInHouseFor10Seconds(Blackboard* pBlackboard)
	{
		float* pAmountOfTimeInhouse;
		if (!pBlackboard->GetData("AmountOfTimeInHouse", pAmountOfTimeInhouse) || !pAmountOfTimeInhouse)
			return false;

		if (*pAmountOfTimeInhouse >= 10)
		{
			return true;
		}

		return false;
	}

	bool HasTakenDamage(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		if (pInterface->Agent_GetInfo().Bitten)
		{
			return true;
		}

		return false;
	}

	bool IsDoing180(Blackboard* pBlackboard)
	{
		bool isDoing180;
		if (!pBlackboard->GetData("isDoing180", isDoing180))
			return false;

		return isDoing180;
	}

	bool IsNotDoing180(Blackboard* pBlackboard)
	{
		bool isDoing180;
		if (!pBlackboard->GetData("isDoing180", isDoing180))
			return false;

		return !isDoing180;
	}

	bool HasPistol(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		ItemInfo itemInfo;
		pInterface->Inventory_GetItem(0, itemInfo); //pistol is always saved to slot 0

		if (itemInfo.Type == eItemType::PISTOL)
		{
			return true;
		}

		return false;
	}

	bool HasShotGun(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		ItemInfo itemInfo;
		pInterface->Inventory_GetItem(1, itemInfo); //shotgun is always saved to slot 1

		if (itemInfo.Type == eItemType::SHOTGUN)
		{
			return true;
		}

		return false;
	}

	bool HasMedkit(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		ItemInfo itemInfo;
		pInterface->Inventory_GetItem(2, itemInfo); //medkit is always saved to slot 2

		if (itemInfo.Type == eItemType::MEDKIT)
		{
			return true;
		}

		return false;
	}

	bool HasFood(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		ItemInfo itemInfo;
		if (!pInterface->Inventory_GetItem(3, itemInfo))
		{
			if (!pInterface->Inventory_GetItem(4, itemInfo))
				return false; //no food in both slots

			return true; //food in slot4
		}

		return true; //food in slot3 and maybe in slot4
	}

	bool CanSprint(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		if (pInterface->Agent_GetInfo().Stamina > 0)
		{
			return true;
		}

		return true;
	}

	bool HasEquipedPistolMoreAmmoThenPistolInGrabrange(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		std::vector<EntityInfo>* pvEntitiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntitiesInFOV) || !pvEntitiesInFOV)
			return false;

		for (const auto& entity : *pvEntitiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the enity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo

			if (itemInfo.Type == eItemType::PISTOL
				&& (itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //if the item is a pistol and it is in the grabrange
			{
				//get the equiped pistol
				ItemInfo equipedPistol;
				pInterface->Inventory_GetItem(0, equipedPistol);

				if (pInterface->Weapon_GetAmmo(itemInfo) < pInterface->Weapon_GetAmmo(equipedPistol)) //compare the ammo of the 2 pistols
					return true; //the equiped pistol hasw more ammo then the pistol in the grabrange
			}
		}

		return false;
	}

	bool HasEquipedShotgunMoreAmmoThenShotgunInGrabrange(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		std::vector<EntityInfo>* pvEntitiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntitiesInFOV) || !pvEntitiesInFOV)
			return false;

		for (const auto& entity : *pvEntitiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the enity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo

			if (itemInfo.Type == eItemType::SHOTGUN
				&& (itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //if the item is a shotgun and it is in the grabrange
			{
				//get the equiped shotgun
				ItemInfo equipedShotgun;
				pInterface->Inventory_GetItem(1, equipedShotgun); //shotgun always saved in slot 1

				if (pInterface->Weapon_GetAmmo(itemInfo) < pInterface->Weapon_GetAmmo(equipedShotgun)) //compare the ammo of the 2 shotguns
					return true; //the equiped shotgun hasw more ammo then the shotgun in the grabrange
			}
		}

		return false;
	}

	bool HasEquipedMedkitMoreHPThenMedkitInGrabrange(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		std::vector<EntityInfo>* pvEntitiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntitiesInFOV) || !pvEntitiesInFOV)
			return false;

		for (const auto& entity : *pvEntitiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the enity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo

			if (itemInfo.Type == eItemType::MEDKIT
				&& (itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //if the item is a medkit and it is in the grabrange
			{
				//get the equiped medkit
				ItemInfo equipedMedkit;
				pInterface->Inventory_GetItem(2, equipedMedkit); //medkit always saved in slot 2

				if (pInterface->Medkit_GetHealth(itemInfo) < pInterface->Medkit_GetHealth(equipedMedkit)) //compare the health of the 2 medkits
					return true; //the equiped medkit hasw more health then the medkit in the grabrange
			}
		}

		return false;
	}

	bool HasEquipedFoodMoreEnergyThenFoodInGrabrange(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		std::vector<EntityInfo>* pvEntitiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntitiesInFOV) || !pvEntitiesInFOV)
			return false;

		for (const auto& entity : *pvEntitiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::ITEM) continue; //if the entity is not an item continue to the next entity

			ItemInfo itemInfo;
			pInterface->Item_GetInfo(entity, itemInfo); //if the entity is an item get the ItemInfo

			if (itemInfo.Type == eItemType::FOOD
				&& (itemInfo.Location - pInterface->Agent_GetInfo().Position).Magnitude() <= pInterface->Agent_GetInfo().GrabRange) //if the item is food and it is in the grabrange
			{
				//get the equiped food
				//food always saved in slot 3 and 4
				ItemInfo slot3;
				ItemInfo slot4;

				bool hasFoodInSlot3{};
				bool hasFoodInSlot4{};

				if (pInterface->Inventory_GetItem(3, slot3))
					hasFoodInSlot3 = true;

				if (pInterface->Inventory_GetItem(4, slot4))
					hasFoodInSlot4 = true;

				if (!hasFoodInSlot3 && !hasFoodInSlot4)
					return false; //no food is equiped

				if (hasFoodInSlot3 && hasFoodInSlot4) //if the both slots have food in it check which one has the lowest energy and then compare that one with the food in the grabrange
				{
					if (pInterface->Food_GetEnergy(slot3) < pInterface->Food_GetEnergy(slot4)
						&& pInterface->Food_GetEnergy(itemInfo) < pInterface->Food_GetEnergy(slot3))
					{
						return true;
					}
					else if (pInterface->Food_GetEnergy(itemInfo) < pInterface->Food_GetEnergy(slot4))
					{
						return true;
					}
				}
				
				if (hasFoodInSlot3 && pInterface->Food_GetEnergy(itemInfo) < pInterface->Food_GetEnergy(slot3)) //if there is food in only 1 of the 2 slots check if it is slot3 and compare the food to the food in the grabrange
					return true;

				if (pInterface->Food_GetEnergy(itemInfo) < pInterface->Food_GetEnergy(slot4)) //if there is no food in slot3 then there is food in slot4 so compare that food to the food in the grabrange
					return true;
			}
		}

		return false;
	}

	bool HasEquipedPistolAmmo(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		ItemInfo equipedPistol;
		pInterface->Inventory_GetItem(0, equipedPistol); //pistol always saved in slot 0

		if (pInterface->Weapon_GetAmmo(equipedPistol) > 0)
			return true;

		return false;
	}

	bool HasEquipedShotgunAmmo(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		ItemInfo equipedShotgun;
		pInterface->Inventory_GetItem(1, equipedShotgun); //shotgun always saved in slot 1

		if (pInterface->Weapon_GetAmmo(equipedShotgun) > 0)
			return true;

		return false;
	}

	bool HasEquipedMedkitHealth(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		ItemInfo equipedMedkit;
		pInterface->Inventory_GetItem(2, equipedMedkit); //medkit always saved in slot 2

		if (pInterface->Medkit_GetHealth(equipedMedkit) > 0)
			return true;

		return false;
	}

	bool HasEquipedFoodEnergy(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		ItemInfo slot3;
		ItemInfo slot4;
		//food always saved in slot 3 and 4
		if (!pInterface->Inventory_GetItem(3, slot3)
			&& !pInterface->Inventory_GetItem(4, slot4))
			return false; //no food is equiped

		return true; //there is food in slot3 or slot4
	}

	bool NotMaxHealth(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		if (pInterface->Agent_GetInfo().Health < 10.f)
			return true;

		return false;
	}

	bool AlmostOutOfEnergy(Blackboard* pBlackboard)
	{
		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		if (pInterface->Agent_GetInfo().Energy <= 1.f)
			return true;

		return false;
	}

	bool HasNotExploreFullWorld(Blackboard* pBlackboard)
	{
		std::list<Vector2>* plPositionsToVisit;
		if (!pBlackboard->GetData("positionsToVisit", plPositionsToVisit) || !plPositionsToVisit)
			return false;

		if (plPositionsToVisit->empty())
			return false; //full world has been explored

		return true; //the full world has not yet been explored
	}

	bool HasExploreFullWorld(Blackboard* pBlackboard)
	{
		std::list<Vector2>* plPositionsToVisit;
		if (!pBlackboard->GetData("positionsToVisit", plPositionsToVisit) || !plPositionsToVisit)
			return false;

		if (plPositionsToVisit->empty())
			return true; //full world has been explored

		return false; //the full world has not yet been explored
	}

	bool IsInPurgeZone(Blackboard* pBlackboard)
	{
		std::vector<EntityInfo>* pvEntitiesInFOV;
		if (!pBlackboard->GetData("EntitiesInFOV", pvEntitiesInFOV) || !pvEntitiesInFOV)
			return false;

		IExamInterface* pInterface;
		if (!pBlackboard->GetData("Interface", pInterface) || !pInterface)
			return false;

		for (const auto& entity : *pvEntitiesInFOV) //loop over all the entities in the FOV
		{
			if (entity.Type != eEntityType::PURGEZONE) continue; //if the entity is not a purgezone continue to the next entity
			
			PurgeZoneInfo purgZoneInfo;
			pInterface->PurgeZone_GetInfo(entity, purgZoneInfo); //if the entity is a purgezone get the PurgeZoneInfo

			if ((purgZoneInfo.Center - pInterface->Agent_GetInfo().Position).Magnitude() < purgZoneInfo.Radius) //check if the agent is in the purgezone
				return true;
		}
		return false;
	}

	bool IsNotPathfindingToHouse(Blackboard* pBlackboard)
	{
		bool isPathfindingToHouse;
		if (!pBlackboard->GetData("isPathfindingToHouse", isPathfindingToHouse))
			return false;
		
		return !isPathfindingToHouse;
	}

	bool HasReachedHouseCenter(Blackboard* pBlackboard)
	{
		bool hasReachedHouseCenter;
		if (!pBlackboard->GetData("hasReachedHouseCenter", hasReachedHouseCenter))
			return false;

		return hasReachedHouseCenter;
	}
}