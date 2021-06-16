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

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 'AS IS' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL AUTODESK INC. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


Abstract: This is a stub class definition of CDriver_Optris

*/

#include "libmcdriver_optris_driver_optris.hpp"
#include "libmcdriver_optris_interfaceexception.hpp"

// Include custom headers here.


using namespace LibMCDriver_Optris::Impl;

/*************************************************************************************************************************
 Class definition of CDriver_Optris 
**************************************************************************************************************************/

CDriver_Optris::CDriver_Optris(const std::string& sName, LibMCEnv::PDriverEnvironment pDriverEnvironment)
    : m_sName(sName), m_pDriverEnvironment(pDriverEnvironment)
{
}
void CDriver_Optris::Configure(const std::string& sConfigurationString)
{
	m_pWorkingDirectory = m_pDriverEnvironment->CreateWorkingDirectory();
	m_pLibiriMagerDLL = m_pWorkingDirectory->StoreDriverData("libirimager.dll", "libirimager");
	m_pGenericXML = m_pWorkingDirectory->StoreDriverData("generic.xml", "generic");
	m_pFormatsDEF = m_pWorkingDirectory->StoreDriverData("Formats.def", "formats");

	SetDllDirectory(m_pWorkingDirectory->GetAbsoluteFilePath().c_str());
	m_pOptrisSDK = std::make_shared<COptrisSDK>(m_pLibiriMagerDLL->GetAbsoluteFileName());

//	int32_t m_optris_init = m_pOptrisSDK->optris_usb_init(m_pWorkingDirectory->GetAbsoluteFilePath().c_str(), m_pWorkingDirectory->GetAbsoluteFilePath().c_str(), 0);
//	uint32_t m_optris_serial = m_pOptrisSDK->optris_get_serial();
	

}

std::string CDriver_Optris::GetName()
{
	return m_sName;
}

std::string CDriver_Optris::GetType()
{
	return "optris";
}

void CDriver_Optris::GetVersion(LibMCDriver_Optris_uint32& nMajor, LibMCDriver_Optris_uint32& nMinor, LibMCDriver_Optris_uint32& nMicro, std::string& sBuild)
{
	nMajor = 1;
	nMinor = 0;
	nMicro = 0;
}

void CDriver_Optris::GetHeaderInformation(std::string& sNameSpace, std::string& sBaseName)
{
	sNameSpace = "LibMcDriver_Optris";
	sBaseName = "libmcdriver_optris";
}

void CDriver_Optris::QueryParameters()
{
	
}

void CDriver_Optris::Connect(const std::string& sDeviceName)
{
	throw ELibMCDriver_OptrisInterfaceException(LIBMCDRIVER_OPTRIS_ERROR_NOTIMPLEMENTED);
}

void CDriver_Optris::Initialize(const std::string& sDeviceName)
{
	throw ELibMCDriver_OptrisInterfaceException(LIBMCDRIVER_OPTRIS_ERROR_NOTIMPLEMENTED);
}

