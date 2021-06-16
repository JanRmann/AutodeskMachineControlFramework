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


Abstract: This is the class declaration of CDriver_Optris

*/


#ifndef __LIBMCDRIVER_OPTRIS_DRIVER_OPTRIS
#define __LIBMCDRIVER_OPTRIS_DRIVER_OPTRIS

#include "libmcdriver_optris_interfaces.hpp"

// Parent classes
#include "libmcdriver_optris_driver.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "libmcdriver_optris_sdk.hpp"


namespace LibMCDriver_Optris {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CDriver_Optris 
**************************************************************************************************************************/

class CDriver_Optris : public virtual IDriver_Optris, public virtual CDriver {
private:

	/**
	* Put private members here.
	*/

protected:

	/**
	* Put protected members here.
	*/
	std::string m_sName;
	LibMCEnv :: PDriverEnvironment m_pDriverEnvironment;
	LibMCEnv::PWorkingDirectory m_pWorkingDirectory;
	LibMCEnv::PWorkingFile m_pLibiriMagerDLL;
	LibMCEnv::PWorkingFile m_pGenericXML;
	LibMCEnv::PWorkingFile m_pFormatsDEF;
	POptrisSDK m_pOptrisSDK;
	

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CDriver_Optris(const std::string& sName, LibMCEnv::PDriverEnvironment pDriverEnvironment);

	/**
	* Public member functions to implement.
	*/

	void Initialize(const std::string& sDeviceName) override;

	void Configure(const std::string& sConfigurationString) override;

	std::string GetName() override;

	std::string GetType() override;

	void GetVersion(LibMCDriver_Optris_uint32& nMajor, LibMCDriver_Optris_uint32& nMinor, LibMCDriver_Optris_uint32& nMicro, std::string& sBuild) override;

	void GetHeaderInformation(std::string& sNameSpace, std::string& sBaseName) override;

	void QueryParameters() override;

	void Connect(const std::string& sDeviceName) override;

};

} // namespace Impl
} // namespace LibMCDriver_Optris

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIBMCDRIVER_OPTRIS_DRIVER_OPTRIS
