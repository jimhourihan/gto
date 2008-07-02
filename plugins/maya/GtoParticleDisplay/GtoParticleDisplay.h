//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __GTOPARTICLEDISPLAY_H__
#define __GTOPARTICLEDISPLAY_H__

#include <Gto/Reader.h>
#include <Gto/RawData.h>

#include <maya/MPxLocatorNode.h>
#include <maya/MFloatPoint.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/MMatrix.h>
#include <maya/MBoundingBox.h>
#include <float.h>

#include <limits.h>
#include <stdlib.h>
#include <string>
#include <vector>

#define CURRENT_TIME FLT_MAX
#define INVALID_CACHE FLT_MAX

enum DisplayDataType
{
    Int,
    Float,
    Vec3
};

//*****************************************************************************
class GtoParticleDisplay : public MPxLocatorNode
{
public:
    GtoParticleDisplay();
    virtual ~GtoParticleDisplay();

    static void *creator();
    static MStatus initialize();

    virtual MStatus compute( const MPlug &plug, MDataBlock &data );

    virtual bool isBounded() const { return true; }
    virtual MBoundingBox boundingBox() const;

    virtual void draw( M3dView &view, const MDagPath &path,
                       M3dView::DisplayStyle style,
                       M3dView::DisplayStatus status );

    // Cause this node to NOT be hidden when Show->Locators is false in the UI
    virtual bool excludeAsLocator() const { return false; }

    void dataRead( Gto::Property *p );



    //*************************************************************************
    // ACCESS TO ATTRIBUTE VALUES
    //*************************************************************************
    double skipProb( double when = CURRENT_TIME ) const;
    int startFrame() const;
    int endFrame() const;
    int frameOffset() const;
    int frame() const;
    void gtoFileName( int frame, std::string &fname ) const;

    bool displayParts() const;
    double displayVelScale() const;
    double displayPointSize() const;
    const char *displayVelAttr() const;
    const char *displayOpacityAttr() const;
    bool displayValue() const;
    const char *displayValueProperty() const;
    double particleCacheFrame() const;
    void particleCacheFrame( double value ) const;

    //*************************************************************************
    // ATTRIBUTES
    //*************************************************************************
    static MObject m_skipProbAttr;

    // Simulation time range
    static MObject m_startFrameAttr;
    static MObject m_endFrameAttr;
    static MObject m_frameOffsetAttr;
    static MObject m_frameAttr;

    // Simulation cache location
    static MObject m_gtoBaseAttr;
    static MObject m_gtoParticleObjectAttr;

    // What to draw
    static MObject m_displayPartsAttr;
    static MObject m_displayPointSizeAttr;
    static MObject m_displayVelScaleAttr;
    static MObject m_displayVelAttr;

    static MObject m_displayValueAttr;
    static MObject m_minDensityAttr;
    static MObject m_displayValuePropertyAttr;

    // For info only
    static MObject m_particleCountAttr;
    static MObject m_objNamesEnumAttr;
    static MObject m_velNamesEnumAttr;
    static MObject m_particleCacheFrameAttr;


public:
    static MTypeId id;

protected:
    // Quickie particle struct
    struct Particle
    {
        Particle() : pos(), vel(), drawMe( true ) {}
        MFloatPoint pos;
        MFloatPoint vel;
        union
        {
            int intData;
            float floatData;
            float vec3fData[3];
        };
        bool drawMe;
    };

    // Functions for getting attribute values
    void attrValue( const char *attrName, double &val,
                    double when = CURRENT_TIME ) const;
    void attrValue( const char *attrName, short &val,
                    double when = CURRENT_TIME ) const;
    void attrValue( const char *attrName, bool &val,
                    double when = CURRENT_TIME ) const;
    void attrValue( const char *attrName, std::string &val ) const;
    void attrValue( const char *attrName, MMatrix &val,
                    double when = CURRENT_TIME ) const;

    void setParticleCount();
    void setEnumLists();

    // Cache of particles for display
    double m_particleCacheSkip;
    double m_minDensity;
    std::vector<Particle> m_particles;
    
    MBoundingBox m_bounds;
    int m_particleCount;
    
    std::vector<std::string> m_particleObjNames;
    std::vector<std::string> m_velocityAttrNames;
    DisplayDataType m_displayDataType;
    
    friend class GtoCacheReader;
};

// *****************************************************************************

class GtoCacheReader : public Gto::RawDataBaseReader
{
public:
    typedef Gto::RawDataBaseReader Parent;

    GtoCacheReader( GtoParticleDisplay* pd );
    virtual ~GtoCacheReader();

protected:
    virtual Request object(const std::string& name,
                           const std::string& protocol,
                           unsigned int protocolVersion,
                           const ObjectInfo &header);

    virtual Request component(const std::string& name,
                              const std::string& interp,
                              const ComponentInfo &header);

    virtual Request property(const std::string& name,
                             const std::string& interp,
                             const PropertyInfo &header);

    virtual void dataRead(const PropertyInfo&);

    GtoParticleDisplay *m_displayer;
    std::string m_particleObj;
    std::string m_velocityAttr;
};

//*****************************************************************************
// INLINE FUNCTIONS
//*****************************************************************************
inline void StatusCheck( MStatus &stat, const char *context = NULL )
{
    if ( !stat )
    {
        if ( context != NULL )
        {
            std::cerr << "MAYA Runtime error during: " << context
                     << std::endl;
        }
        else
        {
            std::cerr << "MAYA Runtime error." << std::endl;
        }

        std::cerr << "Additional information: "
                 << stat.errorString().asChar() << std::endl;
    }
}

//*****************************************************************************
inline double GtoParticleDisplay::skipProb( double when ) const
{
    double ret;
    attrValue( "skipProb", ret, when );
    return ret;
}

//*****************************************************************************
inline int GtoParticleDisplay::startFrame() const
{
    short ret;
    attrValue( "startFrame", ret );
    return ( int )ret;
}

//*****************************************************************************
inline int GtoParticleDisplay::endFrame() const
{
    short ret;
    attrValue( "endFrame", ret );
    return ( int )ret;
}

//*****************************************************************************
inline int GtoParticleDisplay::frameOffset() const
{
    short ret;
    attrValue( "frameOffset", ret );
    return ( int )ret;
}

//******************************************************************************
inline int GtoParticleDisplay::frame() const
{
    short ret;
    attrValue( "frame", ret );
    return ( int ) ret;
}

//*****************************************************************************
inline void GtoParticleDisplay::gtoFileName( int frame, std::string &fname ) const
{
    char buf[32];
    sprintf( buf, ".%04d.gto", frame );

    attrValue( "gtoBase", fname );

    fname += buf;
}

//*****************************************************************************
inline bool GtoParticleDisplay::displayParts() const
{
    bool ret;
    attrValue( "displayParts", ret );
    return ret;
}

//*****************************************************************************
inline bool GtoParticleDisplay::displayValue() const
{
    bool ret;
    attrValue( "displayValue", ret );
    return ret;
}

//*****************************************************************************
inline const char *GtoParticleDisplay::displayValueProperty() const
{
    std::string valueProperty( "density" );
    attrValue( "displayValueProperty", valueProperty );
    return valueProperty.c_str();
}

//*****************************************************************************
inline double GtoParticleDisplay::displayVelScale() const
{
    double ret;
    attrValue( "displayVelScale", ret );
    return ret;
}

//*****************************************************************************
inline double GtoParticleDisplay::displayPointSize() const
{
    double ret;
    attrValue( "displayPointSize", ret );
    return ret;
}

//******************************************************************************
inline const char *GtoParticleDisplay::displayVelAttr() const
{
    std::string velAttr( "velocity" );
    attrValue( "displayVelAttr", velAttr );
    return velAttr.c_str();
}

//*****************************************************************************
inline double GtoParticleDisplay::particleCacheFrame() const
{
    double ret;
    attrValue( "particleCacheFrame", ret );
    return ret;
}


#endif    // End #ifdef __GTOPARTICLEDISPLAY_H__
