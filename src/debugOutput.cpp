#include "trace.h"
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
namespace js = rapidjson;

void printObject(const js::GenericValue<js::UTF8<> >& obj) {
	js::StringBuffer buffer;
	js::Writer<js::StringBuffer> writer(buffer);
	obj.Accept(writer);
	trace(ATTR("30")"Object:%s", buffer.GetString());
}