/*++

Copyright (C) 2020 Autodesk Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


#ifndef __LIBMCDRIVER_XIMC_SDK
#define __LIBMCDRIVER_XIMC_SDK

#include "libmcdriver_ximc_types.hpp"

#include <memory>
#include <string>

#ifdef _WIN32

#define XIMC_CALLINGCONVENTION __stdcall

#else

#define XIMC_CALLINGCONVENTION 

#endif

#define XIMC_ENUMERATEFLAG_PROBE    0x01
#define XIMC_ENUMERATEFLAG_ALL_COM  0x02
#define XIMC_ENUMERATEFLAG_NETWORK  0x04

namespace LibMCDriver_Ximc {
	namespace Impl {

		typedef uint64_t ximc_device_enumeration_t;
		typedef int ximc_result_t;
		typedef int ximc_device_t;

		typedef struct {
			uint32_t m_MovementStatus;
			uint32_t m_MovementCommandStatus;
			uint32_t m_PowerState;
			uint32_t m_EncoderState;
			uint32_t m_WindingState;
			int32_t m_CurrentPosition;
			int32_t m_MicroCurrentPosition;
			int64_t m_EncoderPosition;
			int32_t m_CurrentSpeed;
			int32_t m_MicroCurrentSpeed;
			int32_t m_EngineCurrentInMilliAmpere;
			int32_t m_SupplyVoltagein10MilliVolts;
			int32_t m_USBCurrentInMilliAmpere;
			int32_t m_USBVoltageIn10MilliVolts;
			int32_t m_TemperatureInTenthDegreeCelsius;
			uint32_t m_StatusFlags;
			uint32_t m_GPIOFlags;
			uint32_t m_CommandBufferFreeSpace;
		} ximc_status_t;
		
		typedef struct {
			char Manufacturer [5];
			char ManufacturerId [3];
			char ProductDescription [9];
			uint32_t m_Major;
			uint32_t m_Minor;
			uint32_t m_Release;
		} ximc_device_information_t;
		
		typedef struct {
			uint32_t m_SpeedInStepsPerSecond;
			uint32_t m_MicroSpeedInMicrostepFractionsPerSecond;
			uint32_t m_AccelerationInStepsPerSquareSecond;
			uint32_t m_DecelerationInStepsPerSquareSecond;
			uint32_t m_AntiplaySpeedInStepsPerSecond;
			uint32_t m_MicroAntiplaySpeedInMicrostepsPerSecond;
			uint32_t m_MoveFlags;
		} ximc_move_settings_t;
		/*
		typedef struct {
			uint32_t m_NomVoltageIn10MilliVolts;
			uint32_t m_NomCurrentInMilliAmpere;
			uint32_t m_NomSpeedInStepsPerSecond;
			uint32_t m_MicroNomSpeedInMicrostepsPerSecond;
			uint32_t m_EngineFlags;
			int32_t m_AntiplayInSteps;
			uint32_t m_MicrostepMode;
			uint32_t m_StepsPerRev;
		} ximc_engine_settings_t;

		typedef struct{
			float L; //Inductance of the windings of the motor
			float R; //Resistance of the windings of the motor
			float Km; //Electromechanical ratio of the motor
			uint32_t BackEMFFlags;
		} ximc_emf_settings_t;

		typedef struct{
			uint32_t stepcloseloop_Kw; //Mixing ratio of the actual and set speed, default=50
			uint32_t stepcloseloop_Kp_low; //Position feedback in the low-speed zone, default=1000
			uint32_t stepcloseloop_Kp_high; //Position feedback in the high-speed zone, default=33
		} ximc_engine_advanced_setup_t;
		*/
		typedef void (XIMC_CALLINGCONVENTION *PXimcPtr_ximc_version) (char * pVersion);
		typedef ximc_device_enumeration_t(XIMC_CALLINGCONVENTION* PXimcPtr_enumerate_devices) (int enumerateFlags, const char* pHints);
		typedef ximc_result_t (XIMC_CALLINGCONVENTION* PXimcPtr_free_enumerate_devices) (ximc_device_enumeration_t deviceEnumeration);
		typedef int (XIMC_CALLINGCONVENTION* PXimcPtr_get_device_count) (ximc_device_enumeration_t deviceEnumeration);
		typedef char* (XIMC_CALLINGCONVENTION* PXimcPtr_get_device_name) (ximc_device_enumeration_t deviceEnumeration, int deviceIndex);
		typedef ximc_device_t(XIMC_CALLINGCONVENTION* PXimcPtr_open_device) (const char * uri);
		typedef ximc_result_t(XIMC_CALLINGCONVENTION* PXimcPtr_get_status) (ximc_device_t device_id, ximc_status_t * pStatus);
		
		typedef ximc_result_t(XIMC_CALLINGCONVENTION* PXimcPtr_get_device_information) (ximc_device_t device_id, ximc_device_information_t * deviceInformation);
		typedef ximc_result_t(XIMC_CALLINGCONVENTION* PXimcPtr_command_move) (ximc_device_t device_id, int position, int microposition);
		/*
		typedef ximc_result_t(XIMC_CALLINGCONVENTION* PXimcPtr_command_zero) (ximc_device_t device_id);
		typedef ximc_result_t(XIMC_CALLINGCONVENTION* PXimcPtr_get_engine_settings) (ximc_device_t id, ximc_engine_settings_t* engine_settings);
		typedef ximc_result_t(XIMC_CALLINGCONVENTION* PXimcPtr_get_emf_settings) (ximc_device_t id, ximc_emf_settings_t* engine_settings);
		typedef ximc_result_t(XIMC_CALLINGCONVENTION* PXimcPtr_set_engine_advanced_setup) (ximc_device_t id, const ximc_engine_advanced_setup_t* engine_advansed_setup);
		typedef ximc_result_t(XIMC_CALLINGCONVENTION* PXimcPtr_get_engine_advanced_setup) (ximc_device_t id, ximc_engine_advanced_setup_t* engine_advansed_setup);
		typedef ximc_result_t(XIMC_CALLINGCONVENTION* PXimcPtr_set_move_settings) (ximc_device_t device_id, const ximc_move_settings_t* moveSettings);
		*/
		class CXimcSDK {
		private:
			bool m_bIsInitialized;

			void* m_LibraryHandle;
			void resetFunctionPtrs ();
		public:

			PXimcPtr_ximc_version ximc_version = nullptr;
			PXimcPtr_enumerate_devices enumerate_devices = nullptr;
			PXimcPtr_free_enumerate_devices free_enumerate_devices = nullptr;
			PXimcPtr_get_device_count get_device_count = nullptr;
			PXimcPtr_get_device_name get_device_name = nullptr;
			PXimcPtr_open_device open_device = nullptr;
			PXimcPtr_get_status get_status = nullptr;
			
			PXimcPtr_get_device_information get_device_information = nullptr;
			PXimcPtr_command_move command_move = nullptr;
			/*
			PXimcPtr_command_zero command_zero = nullptr;
			PXimcPtr_get_engine_settings get_engine_settings = nullptr;
			PXimcPtr_get_emf_settings get_emf_settings = nullptr;
			PXimcPtr_set_engine_advanced_setup set_engine_advanced_setup = nullptr;
			PXimcPtr_get_engine_advanced_setup get_engine_advanced_setup = nullptr;
			PXimcPtr_set_move_settings set_move_settings = nullptr;
			*/
			CXimcSDK(const std::string & sDLLNameUTF8);
			~CXimcSDK();

		};


		typedef std::shared_ptr<CXimcSDK> PXimcSDK;

	} 
} 

#endif // __LIBMCDRIVER_XIMC_SDK
