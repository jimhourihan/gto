//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************

#include <iostream>
#include <map>

#include <UT/UT_Vector4.h>
#include <GEO/GEO_Detail.h>
#include <GEO/GEO_AttribDict.h>
#include <GEO/GEO_PointList.h>
#include <GB/GB_Attribute.h>
#include <GU/GU_PrimPart.h>
#include <Gto/Protocols.h>  

#include "Particle.h"

namespace HGto {

int Particle::m_particleObjNumber = 0;


// *****************************************************************************
std::string Particle::newName()
{
    char str[16];
    snprintf( str, 16, "hParticleObject%d", m_particleObjNumber++ );
    return std::string( str );
}


// *****************************************************************************
Particle::Particle( const GEO_PrimParticle *particle ) : 
    Object( newName(), GTO_PROTOCOL_PARTICLE, 1 )
{
    GEO_Detail *parent = particle->getParent();
    const GEO_PointList &points = parent->points();
    const GEO_PointAttribDict &attribs = parent->pointAttribs();

    const GEO_ParticleVertex *p = particle->iterateInit();
    size_t pId = 0;
    do
    {
        const UT_Vector4 &v = p->getPos();
        m_pointsPositions.push_back( TwkMath::Vec3f( v.x(), v.y(), v.z() ) );
    } while( p = particle->iterateNext( p ) );
            
    GB_Attribute *attrib = attribs.getHead();
    do
    {
        std::string name = attrib->getName();
        GB_AttribType type = attrib->getType();
        int size = attrib->getSize();
        int offset = attribs.getOffset( attrib );

        switch( type )
        {
        case GB_ATTRIB_FLOAT:
            {
                FloatAttribute *a = new FloatAttribute( name, points.entries() );
                for( size_t i = 0; i < points.entries(); ++i )
                { 
                    float *value = (float *)points[i]->getAttribData(offset);
                    a->data()[i] = *value;
                }
                m_attributes.push_back( a );
            }
            break;
        case GB_ATTRIB_INT:
            {
                IntAttribute *a = new IntAttribute( name, points.entries() );
                for( size_t i = 0; i < points.entries(); ++i )
                { 
                    int *value = (int *)points[i]->getAttribData(offset);
                    a->data()[i] = *value;
                }
                m_attributes.push_back( a );
            }
            break;
        case GB_ATTRIB_STRING:
            {
                // Unsupported (for now)
//                 StringAttribute *attr = new StringAttribute( name );
//                 m_attributes.push_back( attr );
            }
            break;
        case GB_ATTRIB_MIXED:
                // Unsupported
            break;
        case GB_ATTRIB_INDEX:
                // Unsupported
            break;
        case GB_ATTRIB_VECTOR:
            {
                VectorAttribute *a = new VectorAttribute( name, points.entries() );
                for( size_t i = 0; i < points.entries(); ++i )
                { 
                    TwkMath::Vec3f *value = (TwkMath::Vec3f *)points[i]->getAttribData(offset);
                    a->data()[i] = *value;
                }
                m_attributes.push_back( a );
            }
            break;
        }

        // Wee !HACK! to seperate the "life" parameter into "age" and "lifespan"
        if (m_attributes.back()->name() == "life")
        {
            // Change the name "life" to "age"
            m_attributes.back()->name() = "age";

            // Add in the lifespan parameter
            FloatAttribute *a = new FloatAttribute( "lifespan", points.entries() );
            for( size_t i = 0; i < points.entries(); ++i )
            { 
                TwkMath::Vec2f *value = (TwkMath::Vec2f *)points[i]->getAttribData(offset);
                a->data()[i] = (*value)[1];
            }
            m_attributes.push_back( a );

        }


    } while( attrib = (GB_Attribute*)attrib->next() );

}

// *****************************************************************************
Particle::~Particle()
{
}


// *****************************************************************************
void Particle::writeHeader( Gto::Writer &writer )
{
    writer.beginObject( m_name.c_str(), m_protocol.c_str(), m_protoVersion );

    Object::writeHeader( writer );

    writer.beginComponent( GTO_COMPONENT_POINTS );
    writer.property( GTO_PROPERTY_POSITION, Gto::Float, m_pointsPositions.size(), 3 );
    for( int i = 0; i < m_attributes.size(); ++i )
    {
         m_attributes[i]->writeHeader( writer );
    }
    writer.endComponent();

    writer.endObject();
}


// *****************************************************************************
void Particle::writeData( Gto::Writer &writer )
{
    Object::writeData( writer );
    
    writer.propertyData( (void *)&m_pointsPositions.front() );
    for( int i = 0; i < m_attributes.size(); ++i )
    {
         m_attributes[i]->writeData( writer );
    }
}


// *******************************************************************************************
// HEEEEEINOUS !HACK! TO HELP COMBINE age AND lifespan ATTRIBUTES INTO A SINGLE life ATTRIBUTE
// *******************************************************************************************
std::vector<TwkMath::Vec2f> *Particle::combineAgeAndLifespan(void)
{
    std::vector<TwkMath::Vec2f> *retVec = new std::vector<TwkMath::Vec2f>;

    HGto::FloatAttribute *ageAttr = NULL;
    HGto::FloatAttribute *lifespanAttr = NULL;


    // Find age and lifespan
    for (int i = 0; i < m_attributes.size(); i++)
    {
        if (dynamic_cast<FloatAttribute*>( m_attributes[i] ))
        {
            HGto::FloatAttribute *fattr = (HGto::FloatAttribute*)m_attributes[i];

            if (fattr->name() == "age") ageAttr = fattr;
            if (fattr->name() == "lifespan") lifespanAttr = fattr;
        }
    }

    // Sanity check
    if (ageAttr == NULL || lifespanAttr == NULL)
    {
        cerr << "Age and Lifespan complements do not exist!" << endl;
        return retVec;
    }

    // Add the necessary data where it belongs...
    for (int i = 0; i < ageAttr->data().size(); i++)
    {
        retVec->push_back(TwkMath::Vec2f(ageAttr->data()[i], lifespanAttr->data()[i]));
    }

    return retVec;
}

// *****************************************************************************
void Particle::declareHoudini( GU_Detail &gdp )
{
    bool hacksDone = false;

    GU_PrimParticle *part = GU_PrimParticle::build(&gdp, 
                                                    m_pointsPositions.size());

    // Get the ParticleVertex pointer
    GEO_ParticleVertex *pvtx = part->iterateInit();

    //
    // Add all the proper parameters and their data
    //
    for (int i = 0; i < m_attributes.size(); i++)
    {
        pvtx = part->iterateInit();

        if (dynamic_cast<FloatAttribute*>( m_attributes[i] ))
        {
            HGto::FloatAttribute *fattr = (HGto::FloatAttribute*)m_attributes[i];

            // Mega !HACK! to combine age and lifespan into a single 'life' param
            if (fattr->name() == "age" || fattr->name() == "lifespan")
            {
                // Makes sure a double 'life' parameter isn't created
                if (hacksDone) continue;

                std::vector<TwkMath::Vec2f> *combined = combineAgeAndLifespan();

                int attrIndex = gdp.addPointAttrib("life", sizeof(float)*2, GB_ATTRIB_FLOAT, 0);

                // Copy over all the attribute data
                for (int j = 0; j < combined->size(); j++)
                {
                    GEO_Point *ppt = pvtx->getPt();
                    memcpy(ppt->getAttribData(attrIndex), &((*combined)[j]), sizeof(float)*2);
                    pvtx = part->iterateNext( pvtx );
                }

                delete combined;
                hacksDone = true;
            }
            else
            {
                int attrIndex = gdp.addPointAttrib(fattr->name().c_str(), sizeof(float), GB_ATTRIB_FLOAT, 0);

                // Copy over all the attribute data
                for (int j = 0; j < fattr->data().size(); j++)
                {
                    GEO_Point *ppt = pvtx->getPt();
                    memcpy(ppt->getAttribData(attrIndex), &(fattr->data()[j]), sizeof(float));
                    pvtx = part->iterateNext( pvtx );
                }
            }
        }
        else if (dynamic_cast<VectorAttribute*>( m_attributes[i] ))
        {
            HGto::VectorAttribute *vattr = (HGto::VectorAttribute*)m_attributes[i];

            int attrIndex = gdp.addPointAttrib(vattr->name().c_str(), sizeof(float)*3, GB_ATTRIB_VECTOR, 0);

            // Copy over all the attribute data
            for (int j = 0; j < vattr->data().size(); j++)
            {
                GEO_Point *ppt = pvtx->getPt();
                memcpy(ppt->getAttribData(attrIndex), &(vattr->data()[j]), sizeof(float)*3);
                pvtx = part->iterateNext( pvtx );
            }
        }
        else if (dynamic_cast<IntAttribute*>( m_attributes[i] ))
        {
            HGto::IntAttribute *iattr = (HGto::IntAttribute*)m_attributes[i];

            int attrIndex = gdp.addPointAttrib(iattr->name().c_str(), sizeof(int), GB_ATTRIB_INT, 0);

            // Copy over all the attribute data
            for (int j = 0; j < iattr->data().size(); j++)
            {
                GEO_Point *ppt = pvtx->getPt();
                memcpy(ppt->getAttribData(attrIndex), &(iattr->data()[j]), sizeof(int));
                pvtx = part->iterateNext( pvtx );
            }
        }
    }


    //
    // Add the particle positions
    //
    pvtx = part->iterateInit();
    size_t pId = 0;
    do
    {
        // Assign the positions
        const TwkMath::Vec3f &v = m_globalMatrix * m_pointsPositions[pId++];
        pvtx->getPos().assign((const float*)&v, 3);
    } while( pvtx = part->iterateNext( pvtx ) );
    

    //
    // Make the particles spheres
    //
    part->getRenderAttribs().setType(GEO_PARTICLE_SPHERE);
}


}  //  End namespace HGto
