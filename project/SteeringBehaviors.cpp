//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"

using namespace Elite;

//SEEK
//****
SteeringPlugin_Output Seek::CalculateSteering(float deltaT, AgentInfo& agentInfo)
{
	SteeringPlugin_Output steering{};
	
	const Vector2 toTarget{ m_Target - agentInfo.Position };

	steering.LinearVelocity = toTarget;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agentInfo.MaxLinearSpeed;

	return steering;
}

SteeringPlugin_Output Flee::CalculateSteering(float deltaT, AgentInfo& agentInfo)
{
	SteeringPlugin_Output steering{};

	const Vector2 toTarget{ m_Target - agentInfo.Position };

	steering.LinearVelocity = toTarget;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agentInfo.MaxLinearSpeed * -1;

	return steering;
}

SteeringPlugin_Output SprintFlee::CalculateSteering(float deltaT, AgentInfo& agentInfo)
{
	SteeringPlugin_Output steering{};

	const Vector2 toTarget{ m_Target - agentInfo.Position };

	steering.LinearVelocity = toTarget;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agentInfo.MaxLinearSpeed * -1;
	steering.RunMode = true;

	return steering;
}

SteeringPlugin_Output Face::CalculateSteering(float deltaT, AgentInfo& agentInfo)
{
	SteeringPlugin_Output steering{};
	steering.AutoOrient = false;

	Vector2 toTarget{ m_Target - agentInfo.Position };
	Vector2 rotationVector{ cosf(agentInfo.Orientation), sinf(agentInfo.Orientation) };

	if (AngleBetween(toTarget, rotationVector) < 0.1f && AngleBetween(toTarget, rotationVector) > -0.1f)
	{
		steering.AngularVelocity = 0.f;
	}
	else if (AngleBetween(toTarget, rotationVector) < 0)
	{
		steering.AngularVelocity = agentInfo.MaxAngularSpeed;
	}
	else if (AngleBetween(toTarget, rotationVector) > 0)
	{
		steering.AngularVelocity = -agentInfo.MaxAngularSpeed;
	}
	return steering;
}

SteeringPlugin_Output Wander::CalculateSteering(float deltaT, AgentInfo& agentInfo)
{
	Vector2 circleOrigin{};
	circleOrigin.x = agentInfo.Position.x + cosf(agentInfo.Orientation) * m_OffsetDistance;
	circleOrigin.y =agentInfo.Position.y + sinf(agentInfo.Orientation) * m_OffsetDistance;

	const int maxWanderAngle{ int(m_WanderAngle + m_MaxAngleChange) + 360 };
	const int minWanderAngle{ int(m_WanderAngle - m_MaxAngleChange) + 360 };

	m_WanderAngle = static_cast<float>(rand() % (maxWanderAngle - minWanderAngle) + minWanderAngle);
	
	while (m_WanderAngle > 360)
	{
		m_WanderAngle -= 360;
	}

	Vector2 randomPointOnCircle{};
	randomPointOnCircle.x = circleOrigin.x + cosf(ToRadians(m_WanderAngle)) * m_Radius;
	randomPointOnCircle.y = circleOrigin.y + sinf(ToRadians(m_WanderAngle)) * m_Radius;
	
	m_Target = randomPointOnCircle;

	return Seek::CalculateSteering(deltaT, agentInfo);
}