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


#ifndef __D_VEHICLE_VIRTUAL_TIRE_H__
#define __D_VEHICLE_VIRTUAL__TIRE_H__

#include "dStdafxVehicle.h"
#include "dVehicleTireInterface.h"

class dVehicleVirtualTire: public dVehicleTireInterface
{
	public:
	DVEHICLE_API dVehicleVirtualTire(dVehicleNode* const parent, const dMatrix& location, const dTireInfo& info);
	DVEHICLE_API virtual ~dVehicleVirtualTire();

	DVEHICLE_API virtual NewtonCollision* GetCollisionShape() const;

	NewtonCollision *m_tireShape;
};


#endif 

