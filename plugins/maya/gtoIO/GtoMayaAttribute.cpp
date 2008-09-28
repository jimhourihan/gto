//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************

#include <maya/MGlobal.h>
#include <maya/MStatus.h>
#include <maya/MPlug.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MAnimUtil.h>
#include <maya/MObjectArray.h>
#include <maya/MFnAnimCurve.h>

#include "TwkMaya.h"

#include "GtoMayaAttribute.h"

namespace GtoIOPlugin {

#define UNKNOWN_TYPE Gto::NumberOfDataTypes

GtoMayaAttribute::GtoMayaAttribute( MObjectHandle nodeObj, 
                                    MObjectHandle attributeObj,
                                    const char *gtoComponentName )
    : m_nodeObj( nodeObj ),
      m_attrObj( attributeObj ),
      m_interp( "" ),
      m_animCurve( NULL ),
      m_dataType( UNKNOWN_TYPE )
{
    MStatus status;
    MFnAttribute attr( m_attrObj.objectRef(), &status );  STATUS;
    MPlug plug( m_nodeObj.objectRef(), m_attrObj.objectRef() );
    if( plug.isNull() )
    {
        std::cerr << "Null plug in GtoMayaAttribute constructor" << std::endl;
        return;
    }

    std::string attrName = plug.partialName(false,false,false,false,false,true).asChar();
    
    size_t sep = attrName.find( "__" );
    if( sep != attrName.npos )
    {
        m_componentName = attrName.substr(0,sep);
        m_propertyName = attrName.substr(sep+2,attrName.size());
    }
    else
    {
        if( gtoComponentName )
        {
            m_componentName = gtoComponentName;
        }
        else
        {
            m_componentName = "maya";
        }
        m_propertyName = attrName;
    }
    
    
    if( MAnimUtil::isAnimated( plug ) )
    {
        MObjectArray mayaCurves;
        MAnimUtil::findAnimation( plug, mayaCurves );
        if( mayaCurves.length() == 1 )
        {
            MFnAnimCurve mayaCurve( mayaCurves[0] );
            m_animCurve = new GtoAnimCurve();

            convert( mayaCurve, (*m_animCurve) );

            std::string curveName = m_componentName;
            curveName += ".";
            curveName += m_propertyName;
            m_animCurve->name( curveName.c_str() );
        }
        else
        {
            MGlobal::displayWarning( "Found more or less than one curve connected to " + plug.name() );
        }
    }
    
    const MFn::Type &attributeType = m_attrObj.objectRef().apiType();

    if( attr.accepts( MFnData::kString ) )
    {
        m_dataType = Gto::String;
    }
    else if( (attributeType == MFn::kDoubleLinearAttribute)
            || (attributeType == MFn::kDoubleAngleAttribute)
            || (attributeType == MFn::kTimeAttribute ) )
    {
        m_dataType = Gto::Double;
    }
    else if( (attributeType == MFn::kFloatLinearAttribute)
            || (attributeType == MFn::kFloatAngleAttribute) )
    {
        m_dataType = Gto::Float;
    }
    else if( attributeType == MFn::kEnumAttribute )
    {
        m_dataType = Gto::String;
    }
    else if( attr.accepts( MFnData::kNumeric ) )
    {
        MFnNumericAttribute numAttr( m_attrObj.objectRef(), &status );
        if( ! status )
        {
            std::cerr << "" << plug.info() << ": "; STATUS;
            return;
        }
        MFnNumericData::Type type = numAttr.unitType( &status );                STATUS;
    
        switch( type )
        {
        case MFnNumericData::kFloat: 
            m_dataType = Gto::Float;
            break;
        case MFnNumericData::kDouble: 
            m_dataType = Gto::Double;
            break;
        case MFnNumericData::kShort: 
            m_dataType = Gto::Short;
            break;
        case MFnNumericData::kInt: 
            m_dataType = Gto::Int;
            break;
        case MFnNumericData::kByte: 
            m_dataType = Gto::Byte;
            break;
        case MFnNumericData::kChar: 
            m_dataType = Gto::Byte;
            break;
        case MFnNumericData::kBoolean:
            m_dataType = Gto::Byte;
            break;
        default:
            {
                std::string errStr = "ERROR: Unknown numeric data type for attribute: ";
                errStr += plug.info().asChar();
                MGlobal::displayError( errStr.c_str() );
                return;
            }
        }   
    }   
    else 
    {
        std::string errStr = "ERROR: Unknown data type for attribute: ";
        errStr += plug.info().asChar();
        MGlobal::displayError( errStr.c_str() );
        return;
    }
}


GtoMayaAttribute::~GtoMayaAttribute()
{
    if( m_animCurve )
    {
        delete m_animCurve;
        m_animCurve = NULL;
    }
}


void GtoMayaAttribute::writeGtoHeader( Gto::Writer *writer ) const
{
    if( m_dataType == UNKNOWN_TYPE ) return;
    if( ! m_nodeObj.isValid() )
    {
        MGlobal::displayError( "ERROR: Detected invalid m_nodeObj in GtoMayaAttribute::writeGtoHeader" );
        return;
    }
    if( ! m_attrObj.isValid() )
    {
        MGlobal::displayError( "ERROR: Detected invalid m_attrObj in GtoMayaAttribute::writeGtoHeader" );
        return;
    }

    if( m_dataType == Gto::String )
    {
        if( m_attrObj.objectRef().apiType() == MFn::kEnumAttribute )
        {
            MStatus status;
            MPlug plug( m_nodeObj.objectRef(), m_attrObj.objectRef() );

            char index;
            status = plug.getValue( index );  STATUS;
            
            MFnEnumAttribute enumAttr( m_attrObj.objectRef(), &status );          STATUS;
            MString value = enumAttr.fieldName( index, &status );                 STATUS;
            
            writer->intern( value.asChar() );
        }
        else
        {
            MStatus status;
            MPlug plug( m_nodeObj.objectRef(), m_attrObj.objectRef() );

            MString value;
            status = plug.getValue( value );  STATUS;
            writer->intern( value.asChar() );
        }
    }

    writer->property( m_propertyName.c_str(), m_dataType, 1, 1, m_interp.c_str() );
}


void GtoMayaAttribute::writeGtoData( Gto::Writer *writer ) const
{
    if( m_dataType == UNKNOWN_TYPE ) return;
    if( ! m_nodeObj.isValid() )
    {
        MGlobal::displayError( "ERROR: Detected invalid m_nodeObj in GtoMayaAttribute::writeGtoData" );
        return;
    }
    if( ! m_attrObj.isValid() )
    {
        MGlobal::displayError( "ERROR: Detected invalid m_attrObj in GtoMayaAttribute::writeGtoData" );
        return;
    }

    MStatus status;

    MFnAttribute attr( m_attrObj.objectRef(), &status );  STATUS;
    MPlug plug( m_nodeObj.objectRef(), m_attrObj.objectRef() );

    if( plug.isNull() )
    {
        std::cerr << "Null plug in writeGtoData()" << std::endl;
        return;
    }
    
    switch( m_dataType )
    {
    case Gto::String:
        {
            if( m_attrObj.objectRef().apiType() == MFn::kEnumAttribute )
            {
                MStatus status;
                MPlug plug( m_nodeObj.objectRef(), m_attrObj.objectRef() );

                char index;
                status = plug.getValue( index );  STATUS;

                MFnEnumAttribute enumAttr( m_attrObj.objectRef(), &status );          STATUS;
                MString value = enumAttr.fieldName( index, &status );                 STATUS;

                int stringID = writer->lookup( value.asChar() );
                writer->propertyData( &stringID );
            }
            else
            {
                MString value;
                status = plug.getValue( value );  STATUS;
                int stringID = writer->lookup( value.asChar() );
                writer->propertyData( &stringID );
            }
            break;
        }
    case Gto::Float:
        {
            float value;
            status = plug.getValue( value );  STATUS;
            writer->propertyData( &value );
            break;
        }
    case Gto::Double:
        {
            double value;
            status = plug.getValue( value );  STATUS;
            writer->propertyData( &value );
            break;
        }
    case Gto::Short:
        {
            short value;
            status = plug.getValue( value );  STATUS;
            writer->propertyData( &value );
            break;
        }
    case Gto::Int:
        {
            int value;
            status = plug.getValue( value );  STATUS;
            writer->propertyData( &value );
            break;
        }
    case Gto::Byte:
        {
            char value;
            status = plug.getValue( value );  STATUS;
            writer->propertyData( &value );
            break;
        }
    default:
        MGlobal::displayError( "ERROR: Unknown data type in GtoMayaAttribute::writeGtoData()" );
    }
}


// *****************************************************************************
bool GtoMayaAttribute::canExport( MPlug &plug, int depth )
{
//#define CANTEXPORT(x) { std::cerr << plug.info() << ": " << x << std::endl; return false; }
#define CANTEXPORT(x) { return false; }

    if( plug.isNull() )                                 CANTEXPORT( "isNull" );;

    if( plug.isArray() )                                CANTEXPORT( "isArray" );
    if( plug.isElement() )                              CANTEXPORT( "isElement" );
    if( plug.isProcedural() )                           CANTEXPORT( "isProcedural" );
    if( depth == 0 && plug.isCompound() )               CANTEXPORT( "isCompound" );

    if( plug.isChild() )
    {
        MPlug parent( plug.parent() );
        return canExport( parent, depth+1 );
    }

    MFnAttribute attr( plug.attribute() ); 

    if( ! attr.isStorable() )                           CANTEXPORT( "!isStorable" );
    if( attr.isIndeterminant() )                        CANTEXPORT( "isIndeterminant" );
    if( attr.isHidden() )                               CANTEXPORT( "isHidden" );

    if( attr.accepts( MFnData::kPlugin ) )              CANTEXPORT( "kPlugin" );
    if( attr.accepts( MFnData::kPluginGeometry ) )      CANTEXPORT( "kPluginGeometry" );
    if( attr.accepts( MFnData::kMatrix ) )              CANTEXPORT( "kMatrix" );
    if( attr.accepts( MFnData::kMesh ) )                CANTEXPORT( "kMesh" );    
    if( attr.accepts( MFnData::kLattice ) )             CANTEXPORT( "kLattice" );
    if( attr.accepts( MFnData::kNurbsCurve ) )          CANTEXPORT( "kNurbsCurve" );
    if( attr.accepts( MFnData::kNurbsSurface ) )        CANTEXPORT( "kNurbsSurface" );
    if( attr.accepts( MFnData::kSphere ) )              CANTEXPORT( "kSphere" );
    if( attr.accepts( MFnData::kDynSweptGeometry ) )    CANTEXPORT( "kDynSweptGeometry" );
    if( attr.accepts( MFnData::kSubdSurface ) )         CANTEXPORT( "kSubdSurface" );

    if( attr.accepts( MFnData::kStringArray ) )         CANTEXPORT( "kIntArray" );
    if( attr.accepts( MFnData::kIntArray ) )            CANTEXPORT( "kIntArray" );
    if( attr.accepts( MFnData::kDoubleArray ) )         CANTEXPORT( "kSubdSurface" );
    if( attr.accepts( MFnData::kPointArray ) )          CANTEXPORT( "kSubdSurface" );
    if( attr.accepts( MFnData::kVectorArray ) )         CANTEXPORT( "kSubdSurface" );
    if( attr.accepts( MFnData::kComponentList ) )       CANTEXPORT( "kSubdSurface" );

    return true;
}


// *****************************************************************************
bool GtoMayaAttribute::sortByComponent(const GtoMayaAttribute* s1, 
                                       const GtoMayaAttribute* s2 )
{
    if( ! ( s1 && s2 ) ) return false;
    return (s1->componentName() < s2->componentName());
}

}  //  End namespace GtoIOPlugin
