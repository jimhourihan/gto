//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#include "GtoParticleDisplay.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <dirent.h>

#include <TwkMath/TwkMathRandom.h>
#include <TwkUtil/TwkUtilFile.h>

#include <maya/MString.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MColor.h>
#include <maya/M3dView.h>
#include <maya/MDistance.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnData.h>
#include <maya/MFnStringData.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MMatrix.h>
#include <maya/MTime.h>
#include <maya/MDGContext.h>
#include <maya/MAnimControl.h>

#include <maya/MGlobal.h>

#include <string>

#include <math.h>

using namespace std;
using TwkMath::Rand48;

//*****************************************************************************
// ATTRIBUTES AND TYPE
MTypeId GtoParticleDisplay::id( 0x70331 );

MObject GtoParticleDisplay::m_skipProbAttr;
MObject GtoParticleDisplay::m_startFrameAttr;
MObject GtoParticleDisplay::m_frameOffsetAttr;
MObject GtoParticleDisplay::m_endFrameAttr;
MObject GtoParticleDisplay::m_frameAttr;
MObject GtoParticleDisplay::m_gtoBaseAttr;
MObject GtoParticleDisplay::m_displayPartsAttr;
MObject GtoParticleDisplay::m_displayVelScaleAttr;
MObject GtoParticleDisplay::m_displayPointSizeAttr;
MObject GtoParticleDisplay::m_displayVelAttr;

MObject GtoParticleDisplay::m_displayValueAttr;
MObject GtoParticleDisplay::m_displayValuePropertyAttr;

MObject GtoParticleDisplay::m_gtoParticleObjectAttr;
MObject GtoParticleDisplay::m_particleCountAttr;
MObject GtoParticleDisplay::m_particleCacheFrameAttr;

MObject GtoParticleDisplay::m_objNamesEnumAttr;
MObject GtoParticleDisplay::m_velNamesEnumAttr;

MObject GtoParticleDisplay::m_minDensityAttr;

//*****************************************************************************
GtoParticleDisplay::GtoParticleDisplay() :
    m_particleCacheSkip( INVALID_CACHE )
{
    m_bounds.clear();
}

//*****************************************************************************
GtoParticleDisplay::~GtoParticleDisplay()
{
    // Nothing
}

//*****************************************************************************
void *GtoParticleDisplay::creator()
{
    return new GtoParticleDisplay();
}

//*****************************************************************************
#define MAKE_NUM_ATTR( OBJ, LNAME, SNAME, UTYPE, ANIM, DFLT, SMIN, SMAX, HMIN, HMAX )   \
{                                                                                       \
    OBJ = nAttr.create( LNAME, SNAME, UTYPE, DFLT, &stat );                             \
    if ( !stat ) { return stat; }                                                       \
    stat = nAttr.setStorable( true ); if ( !stat ) { return stat; }                     \
    stat = nAttr.setKeyable( ANIM ); if ( !stat ) { return stat; }                      \
    stat = nAttr.setSoftMin( SMIN ); if ( !stat ) { return stat; }                      \
    stat = nAttr.setSoftMax( SMAX ); if ( !stat ) { return stat; }                      \
    stat = nAttr.setMin( HMIN ); if ( !stat ) { return stat; }                          \
    stat = nAttr.setMax( HMAX ); if ( !stat ) { return stat; }                          \
    stat = addAttribute( OBJ ); if ( !stat ) { return stat; }                           \
}

#define MAKE_STRING_ATTR( OBJ, LNAME, SNAME, DFLT )                                              \
{                                                                                                \
    MFnStringData fnsd;                                                                          \
    MObject defStrData = fnsd.create( MString( DFLT ), &stat );                                  \
    if ( !stat ) { return stat; }                                                                \
    OBJ = tAttr.create( LNAME, SNAME, MFnData::kString, defStrData, &stat );                     \
    if ( !stat ) { return stat; }                                                                \
    stat = nAttr.setStorable( true ); if ( !stat ) { return stat; }                              \
    stat = nAttr.setKeyable( false ); if ( !stat ) { return stat; }                              \
    stat = addAttribute( OBJ ); if ( !stat ) { return stat; }                                    \
}

#define MAKE_BOOL_ATTR( OBJ, LNAME, SNAME, DFLT )                                                \
{                                                                                                \
    OBJ = nAttr.create( LNAME, SNAME, MFnNumericData::kBoolean, DFLT, &stat );                   \
    if ( !stat ) { return stat; }                                                                \
    stat = nAttr.setStorable( true ); if ( !stat ) { return stat; }                              \
    stat = nAttr.setKeyable( false ); if ( !stat ) { return stat; }                              \
    stat = addAttribute( OBJ ); if ( !stat ) { return stat; }                                    \
}

#define MAKE_MSG_ATTR( OBJ, LNAME, SNAME )                                      \
{                                                                               \
    OBJ = mAttr.create( LNAME, SNAME, &stat ); if ( !stat ) { return stat; }    \
    stat = addAttribute( OBJ ); if ( !stat ) { return stat; }                   \
}

static bool fileExists( const char *fname )
{
#ifdef _MSC_VER
    struct _stat statBuf;
    int status = _stat( fname, &statBuf );
#else
    struct stat statBuf;
    int status = stat( fname, &statBuf );
#endif
    if ( status != 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

//*****************************************************************************
MStatus GtoParticleDisplay::initialize()
{
    MFnNumericAttribute nAttr;
    MFnTypedAttribute tAttr;
    MFnMessageAttribute mAttr;
    MStatus stat;

    MAKE_NUM_ATTR( m_skipProbAttr, "skipProb", "sp", MFnNumericData::kDouble,
                   true, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f );
    MAKE_NUM_ATTR( m_startFrameAttr, "startFrame", "stfr",
                   MFnNumericData::kShort,
                   false, 1, -1000, 1000, -5000, 5000 );
    MAKE_NUM_ATTR( m_endFrameAttr, "endFrame", "enfr",
                   MFnNumericData::kShort,
                   false, 100, -1000, 1000, -5000, 5000 );
    MAKE_NUM_ATTR( m_endFrameAttr, "frameOffset", "froff",
                   MFnNumericData::kShort,
                   false, 0, -1000, 1000, -5000, 5000 );
    MAKE_NUM_ATTR( m_frameAttr, "frame", "frm",
                   MFnNumericData::kShort,
                   true, 1, -1000, 1000, -5000, 5000 );
    MAKE_NUM_ATTR( m_particleCacheFrameAttr, "particleCacheFrame", "pcf",
                   MFnNumericData::kDouble,
                   false, INVALID_CACHE, 0.0, 0.0, 0.0, 0.0 );
    MAKE_STRING_ATTR( m_gtoBaseAttr,
                      "gtoBase", "pbas", "" );
    MAKE_STRING_ATTR( m_gtoParticleObjectAttr,
                      "gtoParticleObject", "pobj", "" );
    MAKE_BOOL_ATTR( m_displayPartsAttr,
                    "displayParts", "disp", false );

    MAKE_BOOL_ATTR( m_displayValueAttr,
                    "displayValue", "dspv", false );
    MAKE_STRING_ATTR( m_displayValuePropertyAttr,
                      "displayValueProperty", "dspvp", "density" );

    MAKE_NUM_ATTR( m_displayVelScaleAttr,
                   "displayVelScale", "dsvs",
                   MFnNumericData::kDouble,
                   true, 0.0416667, 0.0, 10.0, -1000.0, 1000.0 );
    MAKE_NUM_ATTR( m_displayPointSizeAttr,
                   "displayPointSize", "dsps",
                   MFnNumericData::kDouble,
                   true, 3, 0.1, 10.0, 0.0, 100.0 );
    MAKE_STRING_ATTR( m_displayVelAttr,
                      "displayVelAttr", "dvat", "velocity" );


    m_particleCountAttr = nAttr.create( "particleCount", "pcount", 
                                        MFnNumericData::kInt, 0, &stat );
    if ( !stat ) { return stat; }
    stat = nAttr.setStorable( false ); if ( !stat ) { return stat; }
    stat = nAttr.setKeyable( false ); if ( !stat ) { return stat; }
    stat = nAttr.setWritable( false ); if ( !stat ) { return stat; }
    stat = addAttribute( m_particleCountAttr ); if ( !stat ) { return stat; }

    MAKE_NUM_ATTR( m_minDensityAttr, "minDensity", "mind",
                   MFnNumericData::kDouble,
                   true, 0.0, 0, 0.025, 0.0, 10.0 );

    MAKE_STRING_ATTR( m_objNamesEnumAttr,
                      "objNames", "oNames", "" );
    MAKE_STRING_ATTR( m_velNamesEnumAttr,
                      "velNames", "vNames", "" );

    return MS::kSuccess;
}

//*****************************************************************************
MStatus GtoParticleDisplay::compute( const MPlug &plug, MDataBlock &data )
{
    return MS::kUnknownParameter;
}


//*****************************************************************************
MBoundingBox GtoParticleDisplay::boundingBox() const
{
    if( ( particleCacheFrame() == INVALID_CACHE )
        || ( m_particles.size() == 0 ) )
    {
        return MBoundingBox( MPoint( -DBL_MAX, -DBL_MAX, -DBL_MAX ),
                             MPoint( DBL_MAX, DBL_MAX, DBL_MAX ) );
    }
    else
    {
        return m_bounds;
    }
}

// *****************************************************************************
void GtoParticleDisplay::dataRead( Gto::Property *p )
{
    m_particles.resize( p->size );
    std::string dvp = displayValueProperty();
    
    if( p->name == "position" )
    {
        for( int i = 0; i < p->size; ++i )
        {
            m_particles[i].pos = MFloatPoint( p->floatData[i*3+0],
                                              p->floatData[i*3+1],
                                              p->floatData[i*3+2] );
            m_bounds.expand( MPoint( p->floatData[i*3+0],
                                     p->floatData[i*3+1],
                                     p->floatData[i*3+2] ) );
            m_particleCount = p->size;
        }
    }
    else if( ( p->name == displayVelAttr() )
             && ( p->width == 3 )
             && ( p->type == Gto::Float ) )
    {
        for( int i = 0; i < p->size; ++i )
        {
            m_particles[i].vel = MFloatPoint( p->floatData[i*3+0],
                                              p->floatData[i*3+1],
                                              p->floatData[i*3+2] );
        }
    }    
    if( p->name == dvp )
    {
        for( int i = 0; i < p->size; ++i )
        {
            if( p->type == Gto::Float )
            {
                if( p->width == 3 )
                {
                    m_particles[i].vec3fData[0] = p->floatData[i*3+0];
                    m_particles[i].vec3fData[0] = p->floatData[i*3+1];
                    m_particles[i].vec3fData[0] = p->floatData[i*3+2];
                    m_displayDataType = Vec3;
                }
                else
                {
                    m_particles[i].floatData = p->floatData[i];
                    m_displayDataType = Float;
                }
            }
            else if( p->type == Gto::Int )
            {
                m_particles[i].intData = p->int32Data[i]; 
                m_displayDataType = Int;
            }
        }
    }
}


//*****************************************************************************
void GtoParticleDisplay::draw( M3dView &view, const MDagPath &path,
                               M3dView::DisplayStyle style,
                               M3dView::DisplayStatus status )
{
    if( ! displayParts() )
    {
        return;
    }

    // Load the particles if they're not there.
    int currentFrame = frame();
    int ef = endFrame();
    int sf = startFrame();
    int nf = ef - sf + 1;
    if( nf <= 0 )
    {
        return;
    }

    currentFrame = ( ( currentFrame - sf ) % nf ) + sf;
    currentFrame += frameOffset();
    if( currentFrame < sf || currentFrame > ef )
    {
        return;
    }
    
    if( currentFrame != particleCacheFrame() )
    {
        std::string fname;
        attrValue( "gtoBase", fname );
        if( fname == "" )
        {
            return;
        }

        // Load the particles
        m_particles.clear();
        m_bounds.clear();
        m_particleCacheSkip = INVALID_CACHE;
        particleCacheFrame( INVALID_CACHE );
        m_particleCount = 0;
        m_particleObjNames.clear();
        m_velocityAttrNames.clear();

        gtoFileName( currentFrame, fname );

        // Need to check existence
        if ( ! fileExists( fname.c_str() ) )
        {
            MString noFileWarn( "Cannot load gto file: " );
            noFileWarn += fname.c_str();
            MGlobal::displayWarning( noFileWarn );
            return;
        }

        GtoCacheReader reader( this );
        try
        {
            reader.open( fname.c_str() );
        }
        catch( std::exception &e )
        {
            cerr << e.what() << endl;
            return;
        }
        setParticleCount();
        setEnumLists();
        particleCacheFrame( currentFrame );

    }
    double skip = skipProb();
    if( skip != m_particleCacheSkip )
    {
        Rand48 rand48;
        rand48.reset( 0 );
        for( int i = 0; i < m_particles.size(); ++i )
        {
            m_particles[i].drawMe = ( rand48.nextFloat( 0.0f, 1.0f ) < skip ) ? false : true;
        }
        m_particleCacheSkip = skip;
    }
    
    double minDen = 0.0;
    attrValue( "minDensity", minDen );
    if( minDen != m_minDensity )
    {
        for( int i = 0; i < m_particles.size(); ++i )
        {
            m_particles[i].drawMe = m_particles[i].floatData >= m_minDensity;
        }
        m_minDensity = minDen;
    }
    
    if( m_particles.size() == 0 )
    {
        return;
    }

    
    view.beginGL();
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT | GL_POINT_BIT );

    if ( ( style == M3dView::kFlatShaded ) ||
         ( style == M3dView::kGouraudShaded ) )
    {
        // Push the color settings
        glPushAttrib( GL_CURRENT_BIT );

        if ( status == M3dView::kActive )
        {
            view.setDrawColor( 13, M3dView::kActiveColors );
        }
        else
        {
            view.setDrawColor( 13, M3dView::kDormantColors );
        }

        // DRAW SURFACE STUFF HERE

        glPopAttrib();
    }

    // You have to draw a point over the start of the dot
    // because zero-velocity lines disappear.

    std::vector<Particle>::iterator iter;
    const float velScale = ( float )displayVelScale();
    const float pointSize = ( float )displayPointSize();
    const bool drawValue = displayValue();
    std::string drawValueProperty = displayValueProperty();

    glPointSize( pointSize );
    glLineWidth( pointSize );

    if( velScale != 0.0f )
    {
        glBegin( GL_LINES );
        for( iter = m_particles.begin(); iter != m_particles.end(); ++iter )
        {
            if( (*iter).drawMe )
            {
                glVertex3f( (*iter).pos[0], (*iter).pos[1], (*iter).pos[2] );
                glVertex3f( (*iter).pos[0] + velScale * (*iter).vel[0],
                            (*iter).pos[1] + velScale * (*iter).vel[1],
                            (*iter).pos[2] + velScale * (*iter).vel[2] );
            }
        }
        glEnd();
    }

    glBegin( GL_POINTS );
    for( iter = m_particles.begin(); iter != m_particles.end(); ++iter )
    {
        if( (*iter).drawMe )
        {
            glVertex3f( (*iter).pos[0], (*iter).pos[1], (*iter).pos[2] );
        }
    }
    glEnd();

    if( drawValue )
    {
        char idStr[16];
        int pindex = 0;
        if( m_displayDataType == Int )
        {
            for( iter = m_particles.begin(); iter != m_particles.end(); ++iter, ++pindex )
            {
                if( (*iter).drawMe )
                {
                    snprintf( idStr, 16, "%d", (*iter).intData );
                    view.drawText( idStr, 
                                   MPoint( (*iter).pos[0], (*iter).pos[1], 
                                           (*iter).pos[2] ),
                                   M3dView::kRight );
                }
            }
        }
        else if( m_displayDataType == Float )
        {
            for( iter = m_particles.begin(); iter != m_particles.end(); ++iter, ++pindex )
            {
                if( (*iter).drawMe )
                {
                    snprintf( idStr, 16, "%f", (*iter).floatData );
                    view.drawText( idStr, 
                                   MPoint( (*iter).pos[0], (*iter).pos[1], 
                                           (*iter).pos[2] ),
                                   M3dView::kRight );
                }
            }
        }
        else if( m_displayDataType == Vec3 )
        {
            for( iter = m_particles.begin(); iter != m_particles.end(); ++iter, ++pindex )
            {
                if( (*iter).drawMe )
                {
                    snprintf( idStr, 32, "[%f %f %f]", (*iter).vec3fData[0], 
                                                       (*iter).vec3fData[1], 
                                                       (*iter).vec3fData[2] );
                    view.drawText( idStr, 
                                   MPoint( (*iter).pos[0], (*iter).pos[1], 
                                           (*iter).pos[2] ),
                                   M3dView::kRight );
                }
            }
        }
    }
    
    glPopAttrib();
    view.endGL();    
}


//*****************************************************************************
void GtoParticleDisplay::attrValue( const char *attrName,
                                    double &val, double when ) const
{
    MStatus stat;

    MObject thisObj = thisMObject();

    MFnDependencyNode thisNode( thisObj, &stat );
    StatusCheck( stat, "MFnDependencyNode ctor" );

    MPlug plug = thisNode.findPlug( MString( attrName ), &stat );
    StatusCheck( stat, "MFnDependencyNode findPlug" );

    if( when == CURRENT_TIME )
    {
        stat = plug.getValue( val );
    }
    else
    {
        MDGContext dgc( MTime( when, MTime::kSeconds ) );
        stat = plug.getValue( val, dgc );
    }

    StatusCheck( stat, "GtoParticleDisplay::attrValue" );
}

//*****************************************************************************
void GtoParticleDisplay::attrValue( const char *attrName,
                                    short &val, double when ) const
{
    MStatus stat;

    MObject thisObj = thisMObject();

    MFnDependencyNode thisNode( thisObj, &stat );
    StatusCheck( stat, "MFnDependencyNode ctor" );

    MPlug plug = thisNode.findPlug( MString( attrName ), &stat );
    StatusCheck( stat, "MFnDependencyNode findPlug" );

    if( when == CURRENT_TIME )
    {
        stat = plug.getValue( val );
    }
    else
    {
        MDGContext dgc( MTime( when, MTime::kSeconds ) );
        stat = plug.getValue( val, dgc );
    }

    StatusCheck( stat, "GtoParticleDisplay::attrValue" );
}


//*****************************************************************************
void GtoParticleDisplay::attrValue( const char *attrName,
                                    bool &val, double when ) const
{
    MStatus stat;

    MObject thisObj = thisMObject();

    MFnDependencyNode thisNode( thisObj, &stat );
    StatusCheck( stat, "MFnDependencyNode ctor" );

    MPlug plug = thisNode.findPlug( MString( attrName ), &stat );
    StatusCheck( stat, "MFnDependencyNode findPlug" );

    if( when == CURRENT_TIME )
    {
        stat = plug.getValue( val );
    }
    else
    {
        MDGContext dgc( MTime( when, MTime::kSeconds ) );
        stat = plug.getValue( val, dgc );
    }

    StatusCheck( stat, "GtoParticleDisplay::attrValue" );
}

//*****************************************************************************
void GtoParticleDisplay::attrValue( const char *attrName, std::string &val ) const
{
    MStatus stat;

    MObject thisObj = thisMObject();

    MFnDependencyNode thisNode( thisObj, &stat );
    StatusCheck( stat, "MFnDependencyNode ctor" );

    MPlug plug = thisNode.findPlug( MString( attrName ), &stat );
    StatusCheck( stat, "MFnDependencyNode findPlug" );

    MString mstr;
    stat = plug.getValue( mstr );
    StatusCheck( stat, "MPlug getValue" );

    val.assign( mstr.asChar() );
}

//*****************************************************************************
// This is a specialization of the templated attrValue function.
void GtoParticleDisplay::attrValue( const char *attrName,
                                    MMatrix &mat, double when ) const
{
    MStatus stat;

    MObject thisObj = thisMObject();

    MFnDependencyNode thisNode( thisObj, &stat );
    StatusCheck( stat, "MFnDependencyNode ctor" );

    // Build matrix plug, and specify which element the plug refers to.
    // We use the first element. (the first dagPath of this locator)
    MPlug plug = thisNode.findPlug( MString( attrName ), &stat );
    StatusCheck( stat, "MFnDependencyNode findPlug" );
    plug = plug.elementByLogicalIndex( 0 );

    // Get the value of the attribute
    MObject matrixObject;
    if( when == CURRENT_TIME )
    {
        stat = plug.getValue( matrixObject );
    }
    else
    {
        MDGContext dgc( MTime( when, MTime::kSeconds ) );
        stat = plug.getValue( matrixObject, dgc );
    }
    StatusCheck( stat, "GtoParticleDisplay::attrValue() matrixPlug getValue" );

    MFnMatrixData matrixData( matrixObject, &stat );
    StatusCheck( stat, "GtoParticleDisplay::attrValue() matrixData ctor" );

    mat = matrixData.matrix( &stat );
    StatusCheck( stat, "GtoParticleDisplay::attrValue() matrixData matrix" );
}


// *****************************************************************************
void GtoParticleDisplay::setParticleCount()
{
    MStatus stat;

    MObject thisObj = thisMObject();

    MFnDependencyNode thisNode( thisObj, &stat );
    StatusCheck( stat, "MFnDependencyNode ctor" );

    MPlug plug = thisNode.findPlug( "particleCount", &stat );
    StatusCheck( stat, "MFnDependencyNode findPlug" );

    stat = plug.setValue( m_particleCount );
    StatusCheck( stat, "GtoParticleDisplay::attrValue" );
}

// *****************************************************************************
void GtoParticleDisplay::setEnumLists()
{
    MStatus stat;

    MObject thisObj = thisMObject();

    MFnDependencyNode thisNode( thisObj, &stat );
    StatusCheck( stat, "MFnDependencyNode ctor" );

    string objListString;
    for( int i = 0; i < m_particleObjNames.size(); ++i )
    {
        objListString += m_particleObjNames[i];
        if( i < m_particleObjNames.size()-1 )
            objListString +=  ":";
    }
    MPlug plug = thisNode.findPlug( "objNames", &stat );
    StatusCheck( stat, "MFnDependencyNode findPlug" );

    stat = plug.setValue( objListString.c_str() );
    StatusCheck( stat, "GtoParticleDisplay::attrValue" );

    string velListString;
    for( int i = 0; i < m_velocityAttrNames.size(); ++i )
    {
        velListString += m_velocityAttrNames[i];
        if( i < m_velocityAttrNames.size()-1 )
            velListString +=  ":";
    }

    plug = thisNode.findPlug( "velNames", &stat );
    StatusCheck( stat, "MFnDependencyNode findPlug" );

    stat = plug.setValue( velListString.c_str() );
    StatusCheck( stat, "GtoParticleDisplay::attrValue" );
}


// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

GtoCacheReader::GtoCacheReader( GtoParticleDisplay* pd ) :
    m_displayer( pd ),
    Gto::RawDataBaseReader()
{
    pd->attrValue( "gtoParticleObject", m_particleObj );
    pd->attrValue( "displayVelAttr", m_velocityAttr );
}


// *****************************************************************************
GtoCacheReader::~GtoCacheReader()
{
}


// *****************************************************************************
Gto::Reader::Request 
GtoCacheReader::object( const string &name, 
                        const string &protocol, 
                        unsigned int protocolVersion,
                        const Gto::Reader::ObjectInfo &info) 
{

    if( protocol == "particle" || protocol == "warped particle" )
    {
        m_displayer->m_particleObjNames.push_back( name );
        if( m_particleObj.size() > 0 && name != m_particleObj )
        {
            return Request(false);
        }
        return Parent::object(name, protocol, protocolVersion, info);
    }
    else
    {
        return Request(false);
    }
}


// *****************************************************************************
Gto::Reader::Request
GtoCacheReader::component(const string &name,
                          const std::string& interp,
                          const Gto::Reader::ComponentInfo &header)
{
    if (name == "points")
    {
        return Parent::component(name, interp, header);
    }
    else
    {
        return Request(false);
    }
}


// *****************************************************************************

Gto::Reader::Request
GtoCacheReader::property(const std::string& name,
                         const std::string& interp,
                         const PropertyInfo &header)
{
    if( header.type == Gto::Float && header.width == 3)
    {
        m_displayer->m_velocityAttrNames.push_back( name );
    }
    if( ( name == "position" )
        || ( name == "id" )
        || ( name == m_velocityAttr )
        || ( name == m_displayer->displayValueProperty() ) )
    {
        return Parent::property(name, interp, header);
    }
    else
    {
        return Request(false);
    }
}

// *****************************************************************************
void
GtoCacheReader::dataRead(const Gto::Reader::PropertyInfo &info)
{
    Parent::dataRead(info);
    Gto::Property *p  = 
        reinterpret_cast<Gto::Property*>(info.propertyData);

    //
    //  Tell the emitter about it
    //
    m_displayer->dataRead(p);

    //
    //  Nuke the data since we don't need it anymore
    //

    if (p->type == Gto::String)
    {
        delete [] p->stringData;
    }
    else
    {
        delete [] (char*)(p->voidData);
    }

    p->voidData = 0;
}

//*****************************************************************************
void GtoParticleDisplay::particleCacheFrame( double value ) const
{
    MStatus stat;

    MObject thisObj = thisMObject();

    MFnDependencyNode thisNode( thisObj, &stat );
    StatusCheck( stat, "MFnDependencyNode ctor" );

    MPlug plug = thisNode.findPlug( "particleCacheFrame", &stat );
    StatusCheck( stat, "MFnDependencyNode findPlug" );

    stat = plug.setValue( value );
    StatusCheck( stat, "GtoParticleDisplay::attrValue" );
}
