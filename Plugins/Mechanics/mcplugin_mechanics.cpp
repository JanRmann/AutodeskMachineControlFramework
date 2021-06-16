/*++

Copyright (C) 2020 Autodesk Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the Autodesk Inc. nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL AUTODESK INC. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "libmcplugin_statefactory.hpp"
#include "libmcplugin_interfaceexception.hpp"
#include "libmcplugin_state.hpp"

#include "libmcdriver_marlin_dynamic.hpp"
#include "libmcdriver_ximc_dynamic.hpp"
//#include "libmcdriver_optris_dynamic.hpp"

#include "libmcenv_drivercast.hpp"

using namespace LibMCPlugin::Impl;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif


/*************************************************************************************************************************
 Import functionality for Driver into current plugin
**************************************************************************************************************************/
typedef LibMCDriver_Marlin::PDriver_Marlin PDriver_Marlin;
typedef LibMCEnv::CDriverCast <LibMCDriver_Marlin::CDriver_Marlin, LibMCDriver_Marlin::CWrapper> PDriverCast_Marlin;

typedef LibMCDriver_Ximc::PDriver_Ximc PDriver_Ximc;
typedef LibMCEnv::CDriverCast <LibMCDriver_Ximc::CDriver_Ximc, LibMCDriver_Ximc::CWrapper> PDriverCast_Ximc;

//typedef LibMCDriver_Optris::PDriver_Optris PDriver_Optris;
//typedef LibMCEnv::CDriverCast <LibMCDriver_Optris::CDriver_Optris, LibMCDriver_Optris::CWrapper> PDriverCast_Optris;


/*************************************************************************************************************************
 Class definition of CMechanicsData
**************************************************************************************************************************/
class CMechanicsData : public virtual CPluginData {
protected:
	// We need to globally store driver wrappers in the plugin
	PDriverCast_Marlin m_DriverCast_Marlin;
	PDriverCast_Ximc m_DriverCast_Ximc;
//	PDriverCast_Optris m_DriverCast_Optris;

public:

	PDriver_Marlin acquireMarlinDriver(LibMCEnv::PStateEnvironment pStateEnvironment)
	{
		return m_DriverCast_Marlin.acquireDriver(pStateEnvironment, "marlin");
	}

	PDriver_Ximc acquireXimcDriver(LibMCEnv::PStateEnvironment pStateEnvironment)
	{
		return m_DriverCast_Ximc.acquireDriver(pStateEnvironment, "ximc");
	}

	//PDriver_Optris acquireOptrisDriver(LibMCEnv::PStateEnvironment pStateEnvironment)
	//{
	//	return m_DriverCast_Optris.acquireDriver(pStateEnvironment, "optris");
	//}

};

/*************************************************************************************************************************
 Class definition of CMechanicsData
**************************************************************************************************************************/
typedef CState<CMechanicsData> CMechanicsState;


/*************************************************************************************************************************
 Class definition of CMechanicsState_Init
**************************************************************************************************************************/
class CMechanicsState_Init : public virtual CMechanicsState {
public:

	CMechanicsState_Init(const std::string& sStateName, PPluginData pPluginData)
		: CMechanicsState(getStateName(), sStateName, pPluginData)
	{
	}

	static const std::string getStateName()
	{
		return "init";
	}


	void Execute(LibMCEnv::PStateEnvironment pStateEnvironment)
	{

		if (pStateEnvironment.get() == nullptr)
			throw ELibMCPluginInterfaceException(LIBMCPLUGIN_ERROR_INVALIDPARAM);

		std::string sCOMPort = pStateEnvironment->GetStringParameter("connectionsettings", "comport");
		auto nBaudRate = pStateEnvironment->GetIntegerParameter("connectionsettings", "baudrate");
		auto nStatusUpdateInterval = pStateEnvironment->GetIntegerParameter("connectionsettings", "statusupdateinterval");
		auto nConnectTimeout = pStateEnvironment->GetIntegerParameter("connectionsettings", "connecttimeout");

		// auto pXimcDriver = m_pPluginData->acquireXimcDriver(pStateEnvironment);
		// pXimcDriver->Initialize("xi-com:\\\\.\\COM4");

		auto pDriver = m_pPluginData->acquireMarlinDriver(pStateEnvironment);
		pDriver->Connect(sCOMPort, nBaudRate, nStatusUpdateInterval, nConnectTimeout); 

		//auto pOptrisDriver = m_pPluginData->acquireOptrisDriver(pStateEnvironment);
		//pOptrisDriver->Connect("Device1");

		pStateEnvironment->SetNextState("idle");
	}

};


/*************************************************************************************************************************
 Class definition of CMechanicsState_Idle
**************************************************************************************************************************/
class CMechanicsState_Idle : public virtual CMechanicsState {
public:

	CMechanicsState_Idle(const std::string& sStateName, PPluginData pPluginData)
		: CMechanicsState(getStateName(), sStateName, pPluginData)
	{
	}

	static const std::string getStateName()
	{
		return "idle";
	}


	void Execute(LibMCEnv::PStateEnvironment pStateEnvironment)
	{
		auto pXimcDriver = m_pPluginData->acquireXimcDriver(pStateEnvironment);
		pXimcDriver->QueryParameters();
		
		auto pDriver = m_pPluginData->acquireMarlinDriver(pStateEnvironment);
		pDriver->QueryParameters();


		if (pStateEnvironment.get() == nullptr)
			throw ELibMCPluginInterfaceException(LIBMCPLUGIN_ERROR_INVALIDPARAM);

		LibMCEnv::PSignalHandler pHandlerInstance;
		if (pStateEnvironment->WaitForSignal("signal_recoatlayer", 0, pHandlerInstance)) {
			pStateEnvironment->StoreSignal ("recoatsignal", pHandlerInstance);

			pStateEnvironment->SetNextState("recoating");

		}
		else if (pStateEnvironment->WaitForSignal("signal_movex", 0, pHandlerInstance)) {
			pStateEnvironment->StoreSignal("movexsignal", pHandlerInstance);

			pStateEnvironment->SetNextState("movingx");

		}
		else if (pStateEnvironment->WaitForSignal("signal_movey", 0, pHandlerInstance)) {
			pStateEnvironment->StoreSignal("moveysignal", pHandlerInstance);

			pStateEnvironment->SetNextState("movingy");

		}
		else {
			pStateEnvironment->SetNextState("idle");
		}

	}

};

/*************************************************************************************************************************
 Class definition of CMechanicsState_Recoating
**************************************************************************************************************************/
class CMechanicsState_Recoating : public virtual CMechanicsState {
public:

	CMechanicsState_Recoating(const std::string& sStateName, PPluginData pPluginData)
		: CMechanicsState(getStateName(), sStateName, pPluginData)
	{
	}

	static const std::string getStateName()
	{
		return "recoating";
	}


	void Execute(LibMCEnv::PStateEnvironment pStateEnvironment)
	{

		if (pStateEnvironment.get() == nullptr)
			throw ELibMCPluginInterfaceException(LIBMCPLUGIN_ERROR_INVALIDPARAM);

		pStateEnvironment->LogMessage("Recoating...");
		auto pDriver = m_pPluginData->acquireMarlinDriver(pStateEnvironment);
		//auto pXimcDriver = m_pPluginData->acquireXimcDriver(pStateEnvironment);

		double dist = pStateEnvironment->GetDoubleParameter("recoater", "dist");
		double mult = pStateEnvironment->GetDoubleParameter("recoater", "mult");
		double speed = pStateEnvironment->GetDoubleParameter("recoater", "speed");
		double speedr = pStateEnvironment->GetDoubleParameter("recoater", "speedr");

		//int64_t currentpositionz = pStateEnvironment->GetIntegerParameter("ximcstate", "currentposition");
		
		//pXimcDriver->MoveToZ(currentpositionz-1000, 000);
		
		if (!pDriver->CanExecuteMovement())
			throw std::runtime_error("could not execute movement");

		pDriver->StartHoming();

		if (!pDriver->CanExecuteMovement())
			throw std::runtime_error("could not execute movement");

		pDriver->MoveToXY(dist, dist / speed * speedr, 0, speed);

		if (!pDriver->CanExecuteMovement())
			throw std::runtime_error("could not execute movement");

		pDriver->MoveToXY(10.0, 0.0, 0, 100.0);

		while (pDriver->IsMoving())
		{
			pDriver->UpdatePositionState();
			pStateEnvironment->Sleep(100);
		}
		auto pSignalHandler = pStateEnvironment->RetrieveSignal("recoatsignal");
		pSignalHandler->SetBoolResult("success", true);
		pSignalHandler->SignalHandled();


		pStateEnvironment->SetNextState("idle");
	}

};

/*************************************************************************************************************************
 Class definition of CMechanicsState_MovingX
**************************************************************************************************************************/
class CMechanicsState_MovingX : public virtual CMechanicsState {
public:

	CMechanicsState_MovingX (const std::string& sStateName, PPluginData pPluginData)
		: CMechanicsState(getStateName(), sStateName, pPluginData)
	{
	}

	static const std::string getStateName()
	{
		return "movingx";
	}


	void Execute(LibMCEnv::PStateEnvironment pStateEnvironment)
	{

		if (pStateEnvironment.get() == nullptr)
			throw ELibMCPluginInterfaceException(LIBMCPLUGIN_ERROR_INVALIDPARAM);

		pStateEnvironment->LogMessage("Moving X...");
		auto pDriver = m_pPluginData->acquireMarlinDriver(pStateEnvironment);

		double currentpositionx = pStateEnvironment->GetDoubleParameter("connectionstate", "currentx");
		double speed = pStateEnvironment->GetDoubleParameter("recoater", "speed");
		
		auto pSignalHandler = pStateEnvironment->RetrieveSignal("movexsignal");
		auto deltax = (double)pSignalHandler->GetDouble("deltax");
		

		if (!pDriver->CanExecuteMovement())
			throw std::runtime_error("could not execute movement");

		pDriver->MoveToXY(currentpositionx + deltax, 0, 0, speed);

		while (pDriver->IsMoving())
		{
			pDriver->UpdatePositionState();
			pStateEnvironment->Sleep(100);
		}

		pSignalHandler = pStateEnvironment->RetrieveSignal("movexsignal");
		pSignalHandler->SetBoolResult("success", true);
		pSignalHandler->SignalHandled();

		pDriver->DisableSteppers();

		pStateEnvironment->SetNextState("idle");
	}

};

/*************************************************************************************************************************
 Class definition of CMechanicsState_MovingY
**************************************************************************************************************************/

class CMechanicsState_MovingY : public virtual CMechanicsState {
public:

	CMechanicsState_MovingY(const std::string& sStateName, PPluginData pPluginData)
		: CMechanicsState(getStateName(), sStateName, pPluginData)
	{
	}

	static const std::string getStateName()
	{
		return "movingy";
	}


	void Execute(LibMCEnv::PStateEnvironment pStateEnvironment)
	{

		if (pStateEnvironment.get() == nullptr)
			throw ELibMCPluginInterfaceException(LIBMCPLUGIN_ERROR_INVALIDPARAM);

		pStateEnvironment->LogMessage("Moving Y...");
		auto pDriver = m_pPluginData->acquireMarlinDriver(pStateEnvironment);

		double currentpositiony = pStateEnvironment->GetDoubleParameter("connectionstate", "currenty");
		double speedr = pStateEnvironment->GetDoubleParameter("recoater", "speedr");

		auto pSignalHandler = pStateEnvironment->RetrieveSignal("moveysignal");
		auto deltay = (double)pSignalHandler->GetDouble("deltay");


		if (!pDriver->CanExecuteMovement())
			throw std::runtime_error("could not execute movement");

		pDriver->MoveToXY(0, currentpositiony + deltay, 0, speedr);

		while (pDriver->IsMoving())
		{
			pDriver->UpdatePositionState();
			pStateEnvironment->Sleep(100);
		}

		pSignalHandler = pStateEnvironment->RetrieveSignal("moveysignal");
		pSignalHandler->SetBoolResult("success", true);
		pSignalHandler->SignalHandled();

		pDriver->DisableSteppers();

		pStateEnvironment->SetNextState("idle");
	}

};


/*************************************************************************************************************************
 Class definition of CMainState_FatalError
**************************************************************************************************************************/
class CMechanicsState_FatalError : public virtual CMechanicsState {
public:

	CMechanicsState_FatalError(const std::string& sStateName, PPluginData pPluginData)
		: CMechanicsState(getStateName(), sStateName, pPluginData)
	{
	}

	static const std::string getStateName()
	{
		return "fatalerror";
	}


	void Execute(LibMCEnv::PStateEnvironment pStateEnvironment)
	{
		if (pStateEnvironment.get() == nullptr)
			throw ELibMCPluginInterfaceException(LIBMCPLUGIN_ERROR_INVALIDPARAM);

		pStateEnvironment->SetNextState("fatalerror");
	}

};



/*************************************************************************************************************************
 Class definition of CStateFactory
**************************************************************************************************************************/

CStateFactory::CStateFactory(const std::string& sInstanceName)
{
	m_pPluginData = std::make_shared<CMechanicsData>();
}

IState* CStateFactory::CreateState(const std::string& sStateName)
{

	IState* pStateInstance = nullptr;

	if (createStateInstanceByName<CMechanicsState_Init>(sStateName, pStateInstance, m_pPluginData))
		return pStateInstance;

	if (createStateInstanceByName<CMechanicsState_Idle>(sStateName, pStateInstance, m_pPluginData))
		return pStateInstance;

	if (createStateInstanceByName<CMechanicsState_Recoating>(sStateName, pStateInstance, m_pPluginData))
		return pStateInstance;

	if (createStateInstanceByName<CMechanicsState_MovingX>(sStateName, pStateInstance, m_pPluginData))
		return pStateInstance;

	if (createStateInstanceByName<CMechanicsState_MovingY>(sStateName, pStateInstance, m_pPluginData))
		return pStateInstance;

	if (createStateInstanceByName<CMechanicsState_FatalError>(sStateName, pStateInstance, m_pPluginData))
		return pStateInstance;

	throw ELibMCPluginInterfaceException(LIBMCPLUGIN_ERROR_INVALIDSTATENAME);

}


#ifdef _MSC_VER
#pragma warning(pop)
#endif
