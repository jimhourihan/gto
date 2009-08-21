//
//  Copyright (c) 2009, Tweak Software
//  All rights reserved.
// 
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//     * Redistributions of source code must retain the above
//       copyright notice, this list of conditions and the following
//       disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
//     * Neither the name of the Tweak Software nor the names of its
//       contributors may be used to endorse or promote products
//       derived from this software without specific prior written
//       permission.
// 
//  THIS SOFTWARE IS PROVIDED BY Tweak Software ''AS IS'' AND ANY EXPRESS
//  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL Tweak Software BE LIABLE FOR
//  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
//  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//

#ifndef __GTOANIMCURVE_H__
#define __GTOANIMCURVE_H__

#include <vector>
#include <Gto/Writer.h>

namespace GtoIOPlugin {

class GtoAnimCurve
{
public:
    enum InfinityType // (Same values as MFnAnimCurve)
    {
        INF_CONSTANT        = 0,
        INF_LINEAR          = 1,
        INF_CYCLE           = 3,
        INF_CYCLERELATIVE   = 4,
        INF_OSCILLATE       = 5,
    };

    enum Flags
    {
        FLAG_NONE        = 0,
        FLAG_WEIGHTED    = 1<<0,
        FLAG_STATIC      = 1<<1,
    };

    struct Key
    {
        double time;        // MUST be in seconds
        double value;
        double inTanX;
        double inTanY;
        double outTanX;
        double outTanY;
    };
    
    typedef std::vector<Key> Keys;

    GtoAnimCurve( const char *name="", const char *interp="" );
    virtual ~GtoAnimCurve() {}

    std::string name() const { return m_name; }
    void name( const char *n ) { m_name = n; }
    
    bool isValid() const;
    bool isWeighted() const { return m_flags & FLAG_WEIGHTED; }
    bool isStatic() const { return m_flags & FLAG_STATIC; }
    
    const Keys &keys() const { return m_keys; }    
    Keys &keys() { return m_keys; }    
    void keys( Keys &k ) { m_keys = k; }

    const Flags &flags() const { return m_flags; }
    void flags( Flags f ) { m_flags = f; }
    
    const InfinityType &preInfinity() const { return m_preInfinity; }
    void preInfinity( InfinityType it ) { m_preInfinity = it; }

    const InfinityType &postInfinity() const { return m_postInfinity; }
    void postInfinity( InfinityType it ) { m_postInfinity = it; }

    std::string infinityStr( InfinityType inf ) const;

    void writeGtoHeader( Gto::Writer &writer ) const;
    void writeGtoData( Gto::Writer &writer ) const;

private:
    std::string m_name;
    std::string m_interp;          // GTO interpretation string

    // Curve data
    Keys m_keys;
    Flags m_flags;
    InfinityType m_preInfinity;
    InfinityType m_postInfinity;

    double m_machineTolerance;

    // Evaluation cache
    mutable const Key *m_lastKey;  // lastKey evaluated
    mutable size_t m_lastIndex;    // last index evaluated
    mutable size_t m_lastInterval; // last interval evaluated
    mutable bool m_isStep;         // whether or not this interval is a step interval
    mutable bool m_isLinear;       // whether or not this interval is linear
    mutable double m_fX1;          // start x of the segment
    mutable double m_fX4;          // end x of the segment
    mutable double m_fCoeff[4];    // bezier x parameters (only used for weighted curves)
    mutable double m_fPolyY[4];    // bezier y parameters
    
};



}  //  End namespace GtoIOPlugin

#endif    // End #ifdef __GTOANIMCURVE_H__


