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


#ifndef __LIBMCDRIVER_OPTRIS_SDK
#define __LIBMCDRIVER_OPTRIS_SDK

#include "libmcdriver_optris_types.hpp"

#include <memory>
#include <string>

#ifdef _WIN32

#define OPTRIS_CALLINGCONVENTION __stdcall

#else

#define OPTRIS_CALLINGCONVENTION 

#endif


namespace LibMCDriver_Optris {
	namespace Impl {


		typedef uint32_t(OPTRIS_CALLINGCONVENTION* POptrisPtr_get_serial) ();
		typedef int32_t(OPTRIS_CALLINGCONVENTION* POptrisPtr_usb_init) (const char* xml_config, const char* formats_def, const char* log_file);

		class COptrisSDK {
		private:
			bool m_bIsInitialized;

			void* m_LibraryHandle;
			void resetFunctionPtrs();

		public:

			POptrisPtr_get_serial optris_get_serial= nullptr;
			POptrisPtr_usb_init optris_usb_init = nullptr;

			COptrisSDK(const std::string& sDLLNameUTF8);
			~COptrisSDK();
		};


		typedef std::shared_ptr<COptrisSDK> POptrisSDK;


	} 
} 

#endif // __LIBMCDRIVER_Optris_SDK
