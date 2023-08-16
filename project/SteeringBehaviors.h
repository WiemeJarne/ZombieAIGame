/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "Exam_HelperStructs.h"

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& agentInfo) = 0;

	//Seek Functions
	void SetTarget(const Elite::Vector2 & target) { m_Target = target; }

	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{ return static_cast<T*>(this); }

protected:
	Elite::Vector2 m_Target;
};
#pragma endregion

///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& agentInfo) override;
};

class Flee : public ISteeringBehavior
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& agentInfo) override;
};

class SprintFlee : public ISteeringBehavior
{
public:
	SprintFlee() = default;
	virtual ~SprintFlee() = default;

	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& agentInfo) override;
};

class Face final : public ISteeringBehavior
{
public:
	Face() = default;
	virtual ~Face() = default;

	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& agentInfo) override;
};

class Wander final : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	SteeringPlugin_Output CalculateSteering(float deltaT, AgentInfo& agentInfo) override;

	void SetWanderOffset(float offset) { m_OffsetDistance = offset; }
	void SetWanderRadius(float radius) { m_Radius = radius; }
	void SetMaxAngleChange(float rad) { m_MaxAngleChange = rad; }

protected:
	float m_OffsetDistance{ 6.f }; //offset in agent direction
	float m_Radius{ 4.f }; //wanderRadius
	float m_MaxAngleChange{ 10.f }; //max wanderingAngle change per frame
	float m_WanderAngle{ 0.f };
};

enum class SteeringBehaviorType
{
	none,
	wander,
	face,
	seek,
	flee,
	sprintFlee
};
#endif