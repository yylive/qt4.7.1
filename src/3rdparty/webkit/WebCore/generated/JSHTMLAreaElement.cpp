/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "JSHTMLAreaElement.h"

#include "HTMLAreaElement.h"
#include "HTMLNames.h"
#include "KURL.h"
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSHTMLAreaElement);

/* Hash table */

static const HashTableValue JSHTMLAreaElementTableValues[16] =
{
    { "accessKey", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementAccessKey), (intptr_t)setJSHTMLAreaElementAccessKey },
    { "alt", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementAlt), (intptr_t)setJSHTMLAreaElementAlt },
    { "coords", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementCoords), (intptr_t)setJSHTMLAreaElementCoords },
    { "href", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementHref), (intptr_t)setJSHTMLAreaElementHref },
    { "noHref", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementNoHref), (intptr_t)setJSHTMLAreaElementNoHref },
    { "shape", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementShape), (intptr_t)setJSHTMLAreaElementShape },
    { "target", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementTarget), (intptr_t)setJSHTMLAreaElementTarget },
    { "hash", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementHash), (intptr_t)0 },
    { "host", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementHost), (intptr_t)0 },
    { "hostname", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementHostname), (intptr_t)0 },
    { "pathname", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementPathname), (intptr_t)0 },
    { "port", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementPort), (intptr_t)0 },
    { "protocol", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementProtocol), (intptr_t)0 },
    { "search", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementSearch), (intptr_t)0 },
    { "constructor", DontEnum|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsHTMLAreaElementConstructor), (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSHTMLAreaElementTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 511, JSHTMLAreaElementTableValues, 0 };
#else
    { 36, 31, JSHTMLAreaElementTableValues, 0 };
#endif

/* Hash table for constructor */

static const HashTableValue JSHTMLAreaElementConstructorTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSHTMLAreaElementConstructorTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSHTMLAreaElementConstructorTableValues, 0 };
#else
    { 1, 0, JSHTMLAreaElementConstructorTableValues, 0 };
#endif

class JSHTMLAreaElementConstructor : public DOMConstructorObject {
public:
    JSHTMLAreaElementConstructor(ExecState* exec, JSDOMGlobalObject* globalObject)
        : DOMConstructorObject(JSHTMLAreaElementConstructor::createStructure(globalObject->objectPrototype()), globalObject)
    {
        putDirect(exec->propertyNames().prototype, JSHTMLAreaElementPrototype::self(exec, globalObject), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static const ClassInfo s_info;

    static PassRefPtr<Structure> createStructure(JSValue proto) 
    { 
        return Structure::create(proto, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount); 
    }
    
protected:
    static const unsigned StructureFlags = OverridesGetOwnPropertySlot | ImplementsHasInstance | DOMConstructorObject::StructureFlags;
};

const ClassInfo JSHTMLAreaElementConstructor::s_info = { "HTMLAreaElementConstructor", 0, &JSHTMLAreaElementConstructorTable, 0 };

bool JSHTMLAreaElementConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLAreaElementConstructor, DOMObject>(exec, &JSHTMLAreaElementConstructorTable, this, propertyName, slot);
}

bool JSHTMLAreaElementConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSHTMLAreaElementConstructor, DOMObject>(exec, &JSHTMLAreaElementConstructorTable, this, propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLAreaElementPrototypeTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSHTMLAreaElementPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSHTMLAreaElementPrototypeTableValues, 0 };
#else
    { 1, 0, JSHTMLAreaElementPrototypeTableValues, 0 };
#endif

const ClassInfo JSHTMLAreaElementPrototype::s_info = { "HTMLAreaElementPrototype", 0, &JSHTMLAreaElementPrototypeTable, 0 };

JSObject* JSHTMLAreaElementPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSHTMLAreaElement>(exec, globalObject);
}

const ClassInfo JSHTMLAreaElement::s_info = { "HTMLAreaElement", &JSHTMLElement::s_info, &JSHTMLAreaElementTable, 0 };

JSHTMLAreaElement::JSHTMLAreaElement(NonNullPassRefPtr<Structure> structure, JSDOMGlobalObject* globalObject, PassRefPtr<HTMLAreaElement> impl)
    : JSHTMLElement(structure, globalObject, impl)
{
}

JSObject* JSHTMLAreaElement::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return new (exec) JSHTMLAreaElementPrototype(JSHTMLAreaElementPrototype::createStructure(JSHTMLElementPrototype::self(exec, globalObject)));
}

bool JSHTMLAreaElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLAreaElement, Base>(exec, &JSHTMLAreaElementTable, this, propertyName, slot);
}

bool JSHTMLAreaElement::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSHTMLAreaElement, Base>(exec, &JSHTMLAreaElementTable, this, propertyName, descriptor);
}

JSValue jsHTMLAreaElementAccessKey(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->getAttribute(HTMLNames::accesskeyAttr));
    return result;
}

JSValue jsHTMLAreaElementAlt(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->getAttribute(HTMLNames::altAttr));
    return result;
}

JSValue jsHTMLAreaElementCoords(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->getAttribute(HTMLNames::coordsAttr));
    return result;
}

JSValue jsHTMLAreaElementHref(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->getURLAttribute(HTMLNames::hrefAttr));
    return result;
}

JSValue jsHTMLAreaElementNoHref(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsBoolean(imp->noHref());
    return result;
}

JSValue jsHTMLAreaElementShape(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->getAttribute(HTMLNames::shapeAttr));
    return result;
}

JSValue jsHTMLAreaElementTarget(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->getAttribute(HTMLNames::targetAttr));
    return result;
}

JSValue jsHTMLAreaElementHash(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->hash());
    return result;
}

JSValue jsHTMLAreaElementHost(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->host());
    return result;
}

JSValue jsHTMLAreaElementHostname(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->hostname());
    return result;
}

JSValue jsHTMLAreaElementPathname(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->pathname());
    return result;
}

JSValue jsHTMLAreaElementPort(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->port());
    return result;
}

JSValue jsHTMLAreaElementProtocol(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->protocol());
    return result;
}

JSValue jsHTMLAreaElementSearch(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* castedThis = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThis->impl());
    JSValue result = jsString(exec, imp->search());
    return result;
}

JSValue jsHTMLAreaElementConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSHTMLAreaElement* domObject = static_cast<JSHTMLAreaElement*>(asObject(slotBase));
    return JSHTMLAreaElement::getConstructor(exec, domObject->globalObject());
}
void JSHTMLAreaElement::put(ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    lookupPut<JSHTMLAreaElement, Base>(exec, propertyName, value, &JSHTMLAreaElementTable, this, slot);
}

void setJSHTMLAreaElementAccessKey(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSHTMLAreaElement* castedThisObj = static_cast<JSHTMLAreaElement*>(thisObject);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThisObj->impl());
    imp->setAttribute(HTMLNames::accesskeyAttr, valueToStringWithNullCheck(exec, value));
}

void setJSHTMLAreaElementAlt(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSHTMLAreaElement* castedThisObj = static_cast<JSHTMLAreaElement*>(thisObject);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThisObj->impl());
    imp->setAttribute(HTMLNames::altAttr, valueToStringWithNullCheck(exec, value));
}

void setJSHTMLAreaElementCoords(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSHTMLAreaElement* castedThisObj = static_cast<JSHTMLAreaElement*>(thisObject);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThisObj->impl());
    imp->setAttribute(HTMLNames::coordsAttr, valueToStringWithNullCheck(exec, value));
}

void setJSHTMLAreaElementHref(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSHTMLAreaElement* castedThisObj = static_cast<JSHTMLAreaElement*>(thisObject);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThisObj->impl());
    imp->setAttribute(HTMLNames::hrefAttr, valueToStringWithNullCheck(exec, value));
}

void setJSHTMLAreaElementNoHref(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSHTMLAreaElement* castedThisObj = static_cast<JSHTMLAreaElement*>(thisObject);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThisObj->impl());
    imp->setNoHref(value.toBoolean(exec));
}

void setJSHTMLAreaElementShape(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSHTMLAreaElement* castedThisObj = static_cast<JSHTMLAreaElement*>(thisObject);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThisObj->impl());
    imp->setAttribute(HTMLNames::shapeAttr, valueToStringWithNullCheck(exec, value));
}

void setJSHTMLAreaElementTarget(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSHTMLAreaElement* castedThisObj = static_cast<JSHTMLAreaElement*>(thisObject);
    HTMLAreaElement* imp = static_cast<HTMLAreaElement*>(castedThisObj->impl());
    imp->setAttribute(HTMLNames::targetAttr, valueToStringWithNullCheck(exec, value));
}

JSValue JSHTMLAreaElement::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSHTMLAreaElementConstructor>(exec, static_cast<JSDOMGlobalObject*>(globalObject));
}


}
