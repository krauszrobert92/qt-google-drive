#include "xmlparser.h"
//#include <QDebug>

XMLParser::XMLParser(int type):
    xmlHandler(new XMLHandler(type))
{
}

XMLParser::~XMLParser()
{
    delete xmlHandler;
}

bool XMLParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attribs)
{
    return xmlHandler->startElement(namespaceURI, localName, qName, attribs);
}

bool XMLParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    return xmlHandler->endElement(namespaceURI, localName, qName);
}

bool XMLParser::characters(const QString &str)
{
    return xmlHandler->characters(str);
}

bool XMLParser::fatalError(const QXmlParseException &exception)
{
    return xmlHandler->fatalError(exception);
}

XMLHandler* XMLParser::getXMLHandler(void) const
{
    return xmlHandler;
}

void XMLParser::setType(int type)
{
    xmlHandler->setType(type);

//    for(int i = 0; i < xmlHandler->getTreeItemInfo()->getItems().count(); ++i)
//    {
//         qDebug() << "name =" << xmlHandler->getTreeItemInfo()->getItems()[i].name;
//    }
}
