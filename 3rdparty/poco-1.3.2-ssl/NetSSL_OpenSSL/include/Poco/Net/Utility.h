//
// Utility.h
//
// $Id: //poco/1.3/NetSSL_OpenSSL/include/Poco/Net/Utility.h#1 $
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  Utility
//
// Definition of the Utility class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef NetSSL_Utility_INCLUDED
#define NetSSL_Utility_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/Context.h"


namespace Poco {
namespace Net {


class NetSSL_API Utility
	/// Class Utility. helper class for init & shutdown of the OpenSSL library
{
public:
	static int HTTPS_PORT;
		/// Default port value for HHTPS

	static Context::VerificationMode convertVerificationMode(const std::string& verMode);
		/// Non-case sensitive conversion of a string to a VerificationMode enum.
		/// If verMode is illegal an OptionException is thrown.

	static std::string convertCertificateError(long errCode);
		/// Converts an SSL error code into human readable form

	static std::string convertSSLError(SSL* pSSL, int errCode);
};


} } // namespace Poco::Net


#endif // NetSSL_Utility_INCLUDED
