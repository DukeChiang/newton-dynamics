/* Copyright (c) <2003-2016> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

//////////////////////////////////////////////////////////////////////
#include "dCustomJointLibraryStdAfx.h"
#include "dCustomWheel.h"


IMPLEMENT_CUSTOM_JOINT(dCustomWheel);

dCustomWheel::dCustomWheel(const dMatrix& pinAndPivotFrame, NewtonBody* child, NewtonBody* parent)
	:dCustomSlidingContact(pinAndPivotFrame, child, parent)
	,m_steerAngle(0.0f)
	,m_steerSpeed(60.0f * dDegreeToRad)
{
}

dCustomWheel::dCustomWheel(const dMatrix& pinAndPivotFrameChild, const dMatrix& pinAndPivotFrameParent, NewtonBody* const child, NewtonBody* const parent)
	:dCustomSlidingContact(pinAndPivotFrameChild, pinAndPivotFrameParent, child, parent)
	,m_steerAngle(0.0f)
	,m_steerSpeed(60.0f * dDegreeToRad)
{
}

dCustomWheel::~dCustomWheel()
{
}

void dCustomWheel::Deserialize(NewtonDeserializeCallback callback, void* const userData)
{
	callback(userData, &m_steerAngle, sizeof(dFloat));
	callback(userData, &m_steerSpeed, sizeof(dFloat));
}

void dCustomWheel::Serialize(NewtonSerializeCallback callback, void* const userData) const
{
	dCustomSlidingContact::Serialize(callback, userData);

	callback(userData, &m_steerAngle, sizeof(dFloat));
	callback(userData, &m_steerSpeed, sizeof(dFloat));
}

/*
void dCustomWheel::EnableAngularLimits(bool state)
{
	m_options.m_option2 = state;
}

void dCustomWheel::SetAsAngularSpringDamper(bool state, dFloat springDamperRelaxation, dFloat spring, dFloat damper)
{
	m_angularSpring = spring;
	m_angularDamper = damper;
	m_options.m_option3 = state;
	m_angularSpringDamperRelaxation = dClamp(springDamperRelaxation, dFloat(0.0f), dFloat(0.999f));
	
}

void dCustomWheel::SetAngularLimits(dFloat minDist, dFloat maxDist)
{
	m_minAngle = -dAbs(minDist);
	m_maxAngle = dAbs(maxDist);
}

void dCustomWheel::SubmitConstraintSpringDamper(const dMatrix& matrix0, const dMatrix& matrix1, dFloat timestep)
{
	NewtonUserJointAddAngularRow(m_joint, -m_curJointAngle.GetAngle(), &matrix1.m_up[0]);
	NewtonUserJointSetRowSpringDamperAcceleration(m_joint, m_angularSpringDamperRelaxation, m_angularSpring, m_angularDamper);
}

void dCustomWheel::SubmitConstraintLimits(const dMatrix& matrix0, const dMatrix& matrix1, dFloat timestep)
{
	dFloat angle = m_curJointAngle.GetAngle() + m_angularOmega * timestep;
	if (angle < m_minAngle) {
		NewtonUserJointAddAngularRow(m_joint, 0.0f, &matrix1.m_up[0]);
		NewtonUserJointSetRowStiffness(m_joint, m_stiffness);
		NewtonUserJointSetRowMinimumFriction(m_joint, -m_angularFriction);

		const dFloat invtimestep = 1.0f / timestep;
		const dFloat speed = 0.5f * (m_minAngle - m_curJointAngle.GetAngle()) * invtimestep;
		const dFloat stopAccel = NewtonUserJointCalculateRowZeroAccelaration(m_joint) + speed * invtimestep;
		NewtonUserJointSetRowAcceleration(m_joint, stopAccel);
	} else if (angle > m_maxAngle) {
		NewtonUserJointAddAngularRow(m_joint, 0.0f, &matrix1.m_up[0]);
		NewtonUserJointSetRowStiffness(m_joint, 1.0f);
		NewtonUserJointSetRowMaximumFriction(m_joint, m_angularFriction);

		const dFloat invtimestep = 1.0f / timestep;
		const dFloat speed = 0.5f * (m_maxAngle - m_curJointAngle.GetAngle()) * invtimestep;
		const dFloat stopAccel = NewtonUserJointCalculateRowZeroAccelaration(m_joint) + speed * invtimestep;
		NewtonUserJointSetRowAcceleration(m_joint, stopAccel);

	} else if (m_angularFriction != 0.0f) {
		NewtonUserJointAddAngularRow(m_joint, 0, &matrix1.m_up[0]);
		NewtonUserJointSetRowStiffness(m_joint, m_stiffness);
		NewtonUserJointSetRowAcceleration(m_joint, -m_angularOmega / timestep);
		NewtonUserJointSetRowMinimumFriction(m_joint, -m_angularFriction);
		NewtonUserJointSetRowMaximumFriction(m_joint, m_angularFriction);
	}
}

void dCustomWheel::SubmitConstraintLimitSpringDamper(const dMatrix& matrix0, const dMatrix& matrix1, dFloat timestep)
{
	dFloat angle = m_curJointAngle.GetAngle() + m_angularOmega * timestep;
	if (angle < m_minAngle) {
		NewtonUserJointAddAngularRow(m_joint, 0.0f, &matrix1.m_up[0]);
		NewtonUserJointSetRowStiffness(m_joint, m_stiffness);
		NewtonUserJointSetRowMinimumFriction(m_joint, -m_angularFriction);

		const dFloat invtimestep = 1.0f / timestep;
		const dFloat speed = 0.5f * (m_minAngle - m_curJointAngle.GetAngle()) * invtimestep;
		const dFloat springAccel = NewtonCalculateSpringDamperAcceleration(timestep, m_angularSpring, m_curJointAngle.GetAngle(), m_angularDamper, m_angularOmega);
		const dFloat stopAccel = NewtonUserJointCalculateRowZeroAccelaration(m_joint) + speed * invtimestep + springAccel;
		NewtonUserJointSetRowAcceleration(m_joint, stopAccel);

	} else if (angle > m_maxAngle) {
		NewtonUserJointAddAngularRow(m_joint, 0.0f, &matrix1.m_up[0]);
		NewtonUserJointSetRowStiffness(m_joint, m_stiffness);
		NewtonUserJointSetRowMaximumFriction(m_joint, m_angularFriction);

		const dFloat invtimestep = 1.0f / timestep;
		const dFloat speed = 0.5f * (m_maxAngle - m_curJointAngle.GetAngle()) * invtimestep;
		const dFloat springAccel = NewtonCalculateSpringDamperAcceleration(timestep, m_angularSpring, m_curJointAngle.GetAngle(), m_angularDamper, m_angularOmega);
		const dFloat stopAccel = NewtonUserJointCalculateRowZeroAccelaration(m_joint) + speed * invtimestep + springAccel;
		NewtonUserJointSetRowAcceleration(m_joint, stopAccel);

	} else {
		dCustomWheel::SubmitConstraintSpringDamper(matrix0, matrix1, timestep);
	}
}

void dCustomWheel::SubmitAngularRow(const dMatrix& matrix0, const dMatrix& matrix1, dFloat timestep)
{
	dMatrix localMatrix(matrix0 * matrix1.Inverse());
	dVector euler0;
	dVector euler1;
	localMatrix.GetEulerAngles(euler0, euler1, m_pitchRollYaw);

	dVector rollPin(dSin(euler0[1]), dFloat(0.0f), dCos(euler0[1]), dFloat(0.0f));
	rollPin = matrix1.RotateVector(rollPin);

	NewtonUserJointAddAngularRow(m_joint, -euler0[0], &matrix0[0][0]);
	NewtonUserJointSetRowStiffness(m_joint, m_stiffness);
	NewtonUserJointAddAngularRow(m_joint, -euler0[2], &rollPin[0]);
	NewtonUserJointSetRowStiffness(m_joint, m_stiffness);

	// the joint angle can be determined by getting the angle between any two non parallel vectors
	m_curJointAngle.Update(euler0.m_y);

	// save the current joint Omega
	dVector omega0(0.0f);
	dVector omega1(0.0f);
	NewtonBodyGetOmega(m_body0, &omega0[0]);
	if (m_body1) {
		NewtonBodyGetOmega(m_body1, &omega1[0]);
	}
	m_angularOmega = (omega0 - omega1).DotProduct3(matrix1[1]);


	if (m_options.m_option2) {
		if (m_options.m_option3) {
			dCustomWheel::SubmitConstraintLimitSpringDamper(matrix0, matrix1, timestep);
		} else {
			dCustomWheel::SubmitConstraintLimits(matrix0, matrix1, timestep);
		}
	} else if (m_options.m_option3) {
		dCustomWheel::SubmitConstraintSpringDamper(matrix0, matrix1, timestep);
	} else if (m_angularFriction != 0.0f) {
		NewtonUserJointAddAngularRow(m_joint, 0, &matrix1.m_up[0]);
		NewtonUserJointSetRowStiffness(m_joint, m_stiffness);
		NewtonUserJointSetRowAcceleration(m_joint, -m_angularOmega / timestep);
		NewtonUserJointSetRowMinimumFriction(m_joint, -m_angularFriction);
		NewtonUserJointSetRowMaximumFriction(m_joint, m_angularFriction);
	}
}
*/

void dCustomWheel::Debug(dDebugDisplay* const debugDisplay) const
{
	dCustomSlidingContact::Debug(debugDisplay);
/*
	if (m_options.m_option2) {
		dMatrix matrix0;
		dMatrix matrix1;
		CalculateGlobalMatrix(matrix0, matrix1);

		const int subdiv = 12;
		dVector arch[subdiv + 1];
		const dFloat radius = debugDisplay->m_debugScale;

		if ((m_maxAngle > 1.0e-3f) || (m_minAngle < -1.0e-3f)) {
			// show pitch angle limits
			dVector point(dFloat(radius), dFloat(0.0f), dFloat(0.0f), dFloat(0.0f));

			dFloat minAngle = m_minAngle;
			dFloat maxAngle = m_maxAngle;
			if ((maxAngle - minAngle) >= dPi * 2.0f) {
				minAngle = 0.0f;
				maxAngle = dPi * 2.0f;
			}

			dFloat angleStep = (maxAngle - minAngle) / subdiv;
			dFloat angle0 = minAngle;

			matrix1.m_posit = matrix0.m_posit;
			debugDisplay->SetColor(dVector(0.0f, 0.5f, 0.0f, 0.0f));
			for (int i = 0; i <= subdiv; i++) {
				arch[i] = matrix1.TransformVector(dYawMatrix(angle0).RotateVector(point));
				debugDisplay->DrawLine(matrix1.m_posit, arch[i]);
				angle0 += angleStep;
			}

			for (int i = 0; i < subdiv; i++) {
				debugDisplay->DrawLine(arch[i], arch[i + 1]);
			}
		}
	}
*/
}

void dCustomWheel::SubmitAnglarStructuralRows(const dMatrix& matrix0, const dMatrix& matrix1, dFloat timestep)
{
	dMatrix localMatrix(matrix0 * matrix1.Inverse());
	dVector euler0;
	dVector euler1;
	localMatrix.GetEulerAngles(euler0, euler1, m_pitchRollYaw);

	dVector rollPin(dSin(euler0[1]), dFloat(0.0f), dCos(euler0[1]), dFloat(0.0f));
	rollPin = matrix1.RotateVector(rollPin);

	NewtonUserJointAddAngularRow(m_joint, -euler0[0], &matrix0[0][0]);
	NewtonUserJointSetRowStiffness(m_joint, m_stiffness);
	NewtonUserJointAddAngularRow(m_joint, -euler0[2], &rollPin[0]);
	NewtonUserJointSetRowStiffness(m_joint, m_stiffness);
	
	m_curJointAngle.Update(euler0.m_y);

return;
	const dFloat invTimeStep = 1.0f / timestep;
	const dFloat tol = m_steerSpeed * timestep;
	const dFloat angle = euler0.m_x;

m_steerAngle = 30.0f * dDegreeToRad;

	dFloat currentSpeed = 0.0f;
	if (angle > (m_steerAngle + tol)) {
		currentSpeed = -m_steerSpeed;
		dFloat predictAngle = angle + currentSpeed * timestep;
		if (predictAngle < m_steerAngle) {
			currentSpeed = 0.5f * (m_steerAngle - angle) * invTimeStep;
		}
	} else if (angle < (m_steerAngle - tol)) {
		currentSpeed = m_steerSpeed;
		dFloat predictAngle = angle + currentSpeed * timestep;
		if (predictAngle > m_steerAngle) {
			currentSpeed = 0.5f * (m_steerAngle - angle) * invTimeStep;
		}
	}
	NewtonUserJointAddAngularRow(m_joint, 0.0f, &matrix0.m_front[0]);
	dFloat accel = NewtonUserJointCalculateRowZeroAccelaration(m_joint) + currentSpeed * invTimeStep;
	NewtonUserJointSetRowAcceleration(m_joint, accel);
	NewtonUserJointSetRowStiffness(m_joint, m_stiffness);
}