//
// Copyright (C) 2004 Tweak Films
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//

#include <RiGto/RiGtoObject.h>
#include <RiGto/RiGtoException.h>
#include <RiGtoStub/Stubs.h>
#include <Gto/Protocols.h>
#include <stdio.h>
#include <iostream>

using namespace RiGtoStub;

namespace RiGto {

//******************************************************************************
Object::Object( const std::string &name, 
                const unsigned int protocolVersion,
                Reader *reader )
  : m_name( name ), 
    m_protocolVersion( protocolVersion ),
    m_reader( reader ),
    m_transformRef( NULL ),
    m_transformOpen( NULL ),
    m_transformClose( NULL ),
    m_numInstances( 1 )
{
    // Nothing
}

//******************************************************************************
Object::~Object()
{
    delete[] m_transformRef;
    delete[] m_transformOpen;
    delete[] m_transformClose;
}

//******************************************************************************
void *Object::component( const std::string &name,
                         ReaderPhase rp ) const
{
    // During all phases we read the "object" component only
    if ( name == GTO_COMPONENT_OBJECT )
    {
        return ( void * )OBJECT_C;
    }

    return ( void * )false;
}

//******************************************************************************
void *Object::property( const std::string &name,
                        void *componentData,
                        ReaderPhase rp ) const
{
    // During all three phases we read the "globalMatrix"
    // property of the "object" component
    if ( (( int )componentData) == OBJECT_C )
    {
        if ( name == GTO_PROPERTY_GLOBAL_MATRIX )
        {
            return ( void * )OBJECT_GLOBALMATRIX_P;
        }
    }

    return ( void * )false;
}

//******************************************************************************
void *Object::data( void *componentData,
                    void *propertyData,
                    size_t numItems,
                    size_t itemWidth,
                    size_t numBytes,
                    ReaderPhase rp )
{
    // During the "reference" phase, we stuff the
    // globalMatrix float property into transformRef.
    // 
    // During the "open" phase, we stuff the
    // globalMatrix float property into transformOpen.
    //
    // During the "close" phase, we stuff the
    // globalMatrix float property into transformClose.
    if ( (( int )componentData) == OBJECT_C &&
         (( int )propertyData) == OBJECT_GLOBALMATRIX_P )
    {
        if ( itemWidth != 16 ||
             numBytes != ( 16 * numItems * sizeof( float ) ) )
        {
            std::cerr << "ERROR: "
                      << "Wrong number of floats for transform in object:"
                      << m_name << std::endl;
            return NULL;
        }

        if ( rp == READER_REF )
        {
            m_numInstances = numItems;
        }

        if ( m_numInstances == 0 ||
             m_numInstances != numItems )
        {
            std::cerr << "ERROR: "
                      << "Rest/Open/Close number of instances incompatible "
                      << "in object: " << m_name << std::endl;
            return NULL;
        }

        if ( rp == READER_REF )
        {
            return transformRefData();
        }
        else if ( rp == READER_OPEN )
        {
            return transformOpenData();
        }
        else if ( rp == READER_CLOSE )
        {
            return transformCloseData();
        }
    }

    return NULL;
}

//******************************************************************************
void Object::dataRead( void *componentData,
                       void *propertyData,
                       ReaderPhase rp )
{
    // Nothing
}

//******************************************************************************
void Object::doneReading( ReaderPhase rp )
{
    // Nothing
}

//******************************************************************************
void Object::declareRi() const
{
    if( m_numInstances == 0 )
    {
        std::cerr << "WARNING: " << m_name << " has 0 instances." << std::endl;
    }

    // Make an alias.
    const float *tOpen = m_transformOpen == NULL ?
        m_transformRef : m_transformOpen;
    
    for ( int inst = 0; inst < m_numInstances; ++inst )
    {
        int instGM = 16 * inst;

        // Begin the attribute block
        const char *nameCstr = m_name.c_str();
        Stub_RiAttributeBegin();
        Stub_DeclareObjectName( nameCstr );

        // Put the global matrix on the stack.
        Stub_RtMatrix matrix;
        int row, col, i;
        for ( col = 0, i = 0; col < 4; ++col )
        {
            for ( row = 0; row < 4; ++row, ++i )
            {
                if( m_transformRef != NULL )
                {
                    matrix[row][col] = m_transformRef[instGM + i];
                }
                else
                {
                    matrix[row][col] = (row == col ? 1 : 0);
                }
            }
        }
        Stub_DeclareObjectRefToWorld( matrix );

        // Declare the global transform
        if ( tOpen != NULL )
        {
            if ( m_transformClose != NULL )
            {
                // Motion blur
                Stub_RiMotionBegin( 0.0f, 1.0f );

                // Transform Open
                // Notice that matrix is written transposed.
                for ( col = 0, i = 0; col < 4; ++col )
                {
                    for ( row = 0; row < 4; ++row, ++i )
                    {
                        matrix[row][col] = tOpen[instGM + i];
                    }
                }
                Stub_RiConcatTransform( matrix );

                // Transform Close
                // Notice that matrix is written transposed.
                for ( col = 0, i = 0; col < 4; ++col )
                {
                    for ( row = 0; row < 4; ++row, ++i )
                    {
                        matrix[row][col] = m_transformClose[instGM + i];
                    }
                }
                Stub_RiConcatTransform( matrix );

                Stub_RiMotionEnd();
            }
            else
            {
                // No motion blur
                // Transform Open
                // Notice that matrix is written transposed.
                for ( col = 0, i = 0; col < 4; ++col )
                {
                    for ( row = 0; row < 4; ++row, ++i )
                    {
                        matrix[row][col] = tOpen[instGM + i];
                    }
                }
                Stub_RiConcatTransform( matrix );
            }
        }

        // Call derived class Ri calls
        internalDeclareRi();

        // End the attribute block
        Stub_RiAttributeEnd();
    }
}

//******************************************************************************
void *Object::transformRefData()
{
    if ( m_transformRef != NULL )
    {
        std::cerr << "ERROR: ref-phase object.globalTransform property data "
                  << "received twice for object: " << m_name << std::endl;
        return NULL;
    }

    m_transformRef = new float[16 * m_numInstances];

    return ( void * )m_transformRef;
}

//******************************************************************************
void *Object::transformOpenData()
{
    if ( m_transformOpen != NULL )
    {
        std::cerr << "ERROR: open-phase object.globalTransform property data "
                  << "received twice for object: " << m_name << std::endl;
        return NULL;
    }

    m_transformOpen = new float[16 * m_numInstances];
    return ( void * )m_transformOpen;
}

//******************************************************************************
void *Object::transformCloseData()
{
    if ( m_transformClose != NULL )
    {
        std::cerr << "ERROR: close-phase object.globalTransform property data "
                  << "received twice for object: " << m_name << std::endl;
        return NULL;
    }
    
    m_transformClose = new float[16 * m_numInstances];
    return ( void * )m_transformClose;
}

} // End namespace RiGto
