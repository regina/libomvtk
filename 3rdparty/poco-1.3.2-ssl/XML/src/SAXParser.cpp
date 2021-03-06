//
// SAXParser.cpp
//
// $Id: //poco/1.3/XML/src/SAXParser.cpp#2 $
//
// Library: XML
// Package: SAX
// Module:  SAX
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/SAX/SAXParser.h"
#include "Poco/SAX/SAXException.h"
#include "Poco/SAX/EntityResolverImpl.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/XML/NamespaceStrategy.h"
#include <sstream>


namespace Poco {
namespace XML {


SAXParser::SAXParser():
	_namespaces(true),
	_namespacePrefixes(false)
{
}


SAXParser::SAXParser(const XMLString& encoding):
	_engine(encoding),
	_namespaces(true),
	_namespacePrefixes(false)
{
}


SAXParser::~SAXParser()
{
}


void SAXParser::setEncoding(const XMLString& encoding)
{
	_engine.setEncoding(encoding);
}

	
const XMLString& SAXParser::getEncoding() const
{
	return _engine.getEncoding();
}


void SAXParser::addEncoding(const XMLString& name, Poco::TextEncoding* pEncoding)
{
	_engine.addEncoding(name, pEncoding);
}


void SAXParser::setEntityResolver(EntityResolver* pResolver)
{
	_engine.setEntityResolver(pResolver);
}


EntityResolver* SAXParser::getEntityResolver() const
{
	return _engine.getEntityResolver();
}


void SAXParser::setDTDHandler(DTDHandler* pDTDHandler)
{
	_engine.setDTDHandler(pDTDHandler);
}


DTDHandler* SAXParser::getDTDHandler() const
{
	return _engine.getDTDHandler();
}


void SAXParser::setContentHandler(ContentHandler* pContentHandler)
{
	_engine.setContentHandler(pContentHandler);
}


ContentHandler* SAXParser::getContentHandler() const
{
	return _engine.getContentHandler();
}


void SAXParser::setErrorHandler(ErrorHandler* pErrorHandler)
{
	_engine.setErrorHandler(pErrorHandler);
}


ErrorHandler* SAXParser::getErrorHandler() const
{
	return _engine.getErrorHandler();
}


void SAXParser::setFeature(const XMLString& featureId, bool state)
{
	if (featureId == XMLReader::FEATURE_VALIDATION || featureId == XMLReader::FEATURE_STRING_INTERNING)
		throw SAXNotSupportedException(fromXMLString(XMLReader::FEATURE_VALIDATION));
	else if (featureId == XMLReader::FEATURE_EXTERNAL_GENERAL_ENTITIES)
		_engine.setExternalGeneralEntities(state);
	else if (featureId == XMLReader::FEATURE_EXTERNAL_PARAMETER_ENTITIES)
		_engine.setExternalParameterEntities(state);
	else if (featureId == XMLReader::FEATURE_NAMESPACES)
		_namespaces = state;
	else if (featureId == XMLReader::FEATURE_NAMESPACE_PREFIXES)
		_namespacePrefixes = state;
	else throw SAXNotRecognizedException(fromXMLString(featureId));
}


bool SAXParser::getFeature(const XMLString& featureId) const
{
	if (featureId == XMLReader::FEATURE_VALIDATION || featureId == XMLReader::FEATURE_STRING_INTERNING)
		throw SAXNotSupportedException(fromXMLString(XMLReader::FEATURE_VALIDATION));
	else if (featureId == XMLReader::FEATURE_EXTERNAL_GENERAL_ENTITIES)
		return _engine.getExternalGeneralEntities();
	else if (featureId == XMLReader::FEATURE_EXTERNAL_PARAMETER_ENTITIES)
		return _engine.getExternalParameterEntities();
	else if (featureId == XMLReader::FEATURE_NAMESPACES)
		return _namespaces;
	else if (featureId == XMLReader::FEATURE_NAMESPACE_PREFIXES)
		return _namespacePrefixes;
	else throw SAXNotRecognizedException(fromXMLString(featureId));
}


void SAXParser::setProperty(const XMLString& propertyId, const XMLString& value)
{
	if (propertyId == XMLReader::PROPERTY_DECLARATION_HANDLER || propertyId == XMLReader::PROPERTY_LEXICAL_HANDLER)
		throw SAXNotSupportedException(std::string("property does not take a string value: ") + fromXMLString(propertyId));
	else
		throw SAXNotRecognizedException(fromXMLString(propertyId));
}


void SAXParser::setProperty(const XMLString& propertyId, void* value)
{
	if (propertyId == XMLReader::PROPERTY_DECLARATION_HANDLER)
		_engine.setDeclHandler(reinterpret_cast<DeclHandler*>(value));
	else if (propertyId == XMLReader::PROPERTY_LEXICAL_HANDLER)
		_engine.setLexicalHandler(reinterpret_cast<LexicalHandler*>(value));
	else throw SAXNotRecognizedException(fromXMLString(propertyId));
}


void* SAXParser::getProperty(const XMLString& propertyId) const
{
	if (propertyId == XMLReader::PROPERTY_DECLARATION_HANDLER)
		return _engine.getDeclHandler();
	else if (propertyId == XMLReader::PROPERTY_LEXICAL_HANDLER)
		return _engine.getLexicalHandler();
	else throw SAXNotSupportedException(fromXMLString(propertyId));
}


void SAXParser::parse(InputSource* pInputSource)
{
	if (pInputSource->getByteStream() || pInputSource->getCharacterStream())
	{
		setupParse();
		_engine.parse(pInputSource);
	}
	else parse(pInputSource->getSystemId());
}


void SAXParser::parse(const XMLString& systemId)
{
	setupParse();
	EntityResolverImpl entityResolver;
	InputSource* pInputSource = entityResolver.resolveEntity(0, systemId);
	if (pInputSource)
	{
		try
		{
			_engine.parse(pInputSource);
		}
		catch (...)
		{
			entityResolver.releaseInputSource(pInputSource);
			throw;
		}
		entityResolver.releaseInputSource(pInputSource);
	}
	else throw XMLException("Cannot resolve system identifier", fromXMLString(systemId));
}


void SAXParser::parseString(const std::string& xml)
{
	parseMemoryNP(xml.data(), xml.size());
}


void SAXParser::parseMemoryNP(const char* xml, std::size_t size)
{
	setupParse();
	_engine.parse(xml, size);
}


void SAXParser::setupParse()
{
	if (_namespaces && !_namespacePrefixes)
		_engine.setNamespaceStrategy(new NoNamespacePrefixesStrategy);
	else if (_namespaces && _namespacePrefixes)
		_engine.setNamespaceStrategy(new NamespacePrefixesStrategy);
	else
		_engine.setNamespaceStrategy(new NoNamespacesStrategy);
}


} } // namespace Poco::XML
